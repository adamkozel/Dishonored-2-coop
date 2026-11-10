#pragma once
// 2240 (#263): a watchdog for the co-op saves switch. In 18 of 672 VM takes the host's switch stopped in phase 2
// ("profile_saving"): the status file was never written again after SaveProfile, and the game later exited with
// 0x80000003. The engine thread stops somewhere between SaveProfile and the end of the profile load. While a switch is
// running (phase 1 to 3), this thread checks once a second. When the switch service has not run for 5 s, it writes
// <status>.coop-saves-stall.json once: the last step the switch reached (coopSavesStep) and the stack of every
// thread in the process (game addresses as RVAs). It suspends each thread only to read its registers.
inline volatile LONG coopSavesStep=0;
inline volatile LONG coopSavesWatchdogStarted=0;
struct CoopStallFrames {DWORD thread=0;unsigned count=0;std::uint64_t pc[32]{};};
// Unwinds a copy of a thread context. Any fault ends the walk (the thread may have moved on).
inline unsigned coopStallUnwind(CONTEXT c,std::uint64_t* out,unsigned max){
    unsigned n=0;
    __try{
        for(;n<max&&c.Rip;++n){
            out[n]=c.Rip;DWORD64 image=0;const auto* entry=RtlLookupFunctionEntry(c.Rip,&image,nullptr);
            if(!entry){if(!c.Rsp)break;c.Rip=*reinterpret_cast<const DWORD64*>(c.Rsp);c.Rsp+=8;continue;}
            void* handler=nullptr;DWORD64 establisher=0;
            RtlVirtualUnwind(UNW_FLAG_NHANDLER,image,c.Rip,const_cast<PRUNTIME_FUNCTION>(entry),&c,&handler,&establisher,nullptr);
        }
    }__except(EXCEPTION_EXECUTE_HANDLER){}
    return n;
}
inline void coopStallFrameJson(FILE* f,std::uint64_t pc){
    const auto base=reinterpret_cast<std::uint64_t>(gameImage);HMODULE module=nullptr;char name[MAX_PATH]{};
    if(GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS|GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,reinterpret_cast<LPCSTR>(pc),&module)&&module){
        GetModuleFileNameA(module,name,sizeof(name));const char* slash=std::strrchr(name,'\\');
        fprintf(f,"\"%s+0x%llx\"",slash?slash+1:name,static_cast<unsigned long long>(pc-reinterpret_cast<std::uint64_t>(module)));
        (void)base;return;}
    fprintf(f,"\"0x%llx\"",static_cast<unsigned long long>(pc));
}
inline DWORD WINAPI coopSavesWatchdog(void*){
    for(;;){
        Sleep(1000);
        const int phase=coopSaves.phase;const ULONGLONG at=coopSaves.serviceAt;const DWORD engine=engineThread.load();
        if(!phase||!at||!engine)continue;
        const ULONGLONG now=GetTickCount64();if(now<at||now-at<5000)continue;
        std::vector<CoopStallFrames> threads;
        HANDLE snap=CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,0);
        if(snap!=INVALID_HANDLE_VALUE){THREADENTRY32 t{};t.dwSize=sizeof(t);
            for(BOOL ok=Thread32First(snap,&t);ok&&threads.size()<96;ok=Thread32Next(snap,&t)){
                if(t.th32OwnerProcessID!=GetCurrentProcessId()||t.th32ThreadID==GetCurrentThreadId())continue;
                HANDLE h=OpenThread(THREAD_SUSPEND_RESUME|THREAD_GET_CONTEXT|THREAD_QUERY_INFORMATION,FALSE,t.th32ThreadID);if(!h)continue;
                CONTEXT c{};c.ContextFlags=CONTEXT_FULL;bool got=false;
                if(SuspendThread(h)!=static_cast<DWORD>(-1)){got=GetThreadContext(h,&c)!=FALSE;ResumeThread(h);}
                CloseHandle(h);if(!got)continue;
                CoopStallFrames r{};r.thread=t.th32ThreadID;r.count=coopStallUnwind(c,r.pc,32);
                if(r.thread==engine)threads.insert(threads.begin(),r);else threads.push_back(r);
            }
            CloseHandle(snap);}
        if(*statusPath){char path[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.coop-saves-stall.json",statusPath);FILE* f=nullptr;
            if(!fopen_s(&f,path,"wb")&&f){
                fprintf(f,"{\"phase\":%d,\"result\":\"%s\",\"step\":%ld,\"serviceAgeMs\":%llu,\"engineThread\":%lu,\"threads\":[",phase,coopSaves.result,
                    static_cast<long>(coopSavesStep),static_cast<unsigned long long>(now-at),static_cast<unsigned long>(engine));
                for(std::size_t i=0;i<threads.size();++i){fprintf(f,"%s{\"thread\":%lu,\"engine\":%s,\"frames\":[",i?",":"",static_cast<unsigned long>(threads[i].thread),threads[i].thread==engine?"true":"false");
                    for(unsigned k=0;k<threads[i].count;++k){if(k)fputc(',',f);coopStallFrameJson(f,threads[i].pc[k]);}
                    fputs("]}",f);}
                fputs("]}\n",f);fclose(f);}}
        return 0; // once per game
    }
}
inline void startCoopSavesWatchdog(){
    if(InterlockedCompareExchange(&coopSavesWatchdogStarted,1,0))return;
    HANDLE thread=CreateThread(nullptr,0,coopSavesWatchdog,nullptr,0,nullptr);if(thread)CloseHandle(thread);else InterlockedExchange(&coopSavesWatchdogStarted,0);
}
