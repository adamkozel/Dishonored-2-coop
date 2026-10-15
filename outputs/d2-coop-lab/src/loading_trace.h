#pragma once
// Native state-switch observation plus an explicitly armed, bounded local
// loading experiment. Normal requests are forwarded unchanged.
using NativeLogicSwitch=void(*)(void*,int);
NativeLogicSwitch realLogicSwitch=nullptr;
struct NativeSwitchEvent {
    std::uint64_t sequence=0,tick=0;
    std::uintptr_t manager=0,game=0,player=0,caller=0,activeGame=0;
    unsigned thread=0,from=0,pending=0,phase=0,after=0,stackCount=0;
    int to=0,character=0;
    bool characterValid=false;
};
SRWLOCK switchTraceLock=SRWLOCK_INIT;
NativeSwitchEvent switchEvents[32]{};
std::uint64_t switchTraceSequence=0;
// 1214bc (stage 6 diagnostic): the call stack at each native loading-entry
// switch (to 15), as image RVAs, so the routine a script LevelTransition or a
// mission end calls to change the map can be identified statically.
struct LoadingEntryStack{std::uint64_t sequence=0,tick=0;unsigned count=0,caller=0;std::uint32_t rva[24]{};};
LoadingEntryStack loadingEntryStacks[4]{};std::uint64_t loadingEntryStackCount=0;
struct LoadingProbe {
    bool armed=false,held=false;
    std::uint64_t armUntil=0,startedAt=0,releasedAt=0,started=0,released=0,refused=0,invalidated=0,samples=0;
    std::uintptr_t manager=0,game=0,player=0,armGame=0;
    std::uintptr_t loadingGame=0;std::uint64_t loadingEntryAt=0;
    std::uint64_t armSequence=0;
    int restoreTarget=0;
    float position[3]{};double maxDistance=0;
    const char* result="idle";
};
LoadingProbe loadingProbe{};
void pumpTransport(ULONGLONG tick);
void writeStatus(ULONGLONG tick);
void writeSwitchTrace();
void restoreLoadingRoleAtBoundary(const NativeSwitchEvent& e);
bool nativeSwitchContext(std::uintptr_t manager,NativeSwitchEvent& e) {
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    std::uintptr_t vt=0,stack=0;unsigned count=0;
    e.manager=manager;
    if(!memoryRead(manager+8,&e.game,8)||!e.game||manager!=e.game+0x1f6ec8||
       !memoryRead(e.game,&vt,8)||vt!=base+0x1c940a8)return false;
    // The native loading initializer has an empty state stack. Read owner
    // fields before checking that stack so trace zeros never masquerade as
    // actual phase/player observations.
    if(!memoryRead(e.game+0x341058,&e.phase,4)||!memoryRead(e.game+0x3130b0,&e.player,8)||
       !memoryRead(manager+0x538,&e.pending,4)||!memoryRead(manager+0x530,&count,4))return false;
    e.stackCount=count&0xffffff;
    if(!e.stackCount||e.stackCount>32||!memoryRead(manager+0x528,&stack,8)||!stack||
       !memoryRead(stack+((count&0xffffff)-1)*4,&e.from,4)||e.from>16||
       !memoryRead(manager+0x538,&e.pending,4)||e.pending>16)return false;
    NativeCharacterState s{};
    memoryRead(base+0x39a5290,&e.activeGame,8);
    e.characterValid=readNativeCharacter(memoryRead,base,s,e.game);
    e.character=s.character;
    return true;
}
void armLoadingProbe(int restoreTarget=0) {
    if(localRole!=1||GetCurrentThreadId()!=engineThread.load()||!realLogicSwitch||
       !coopGameplayReady(outgoing)||std::strcmp(mapName,kGuardMap)||loadingProbe.armed||loadingProbe.held||
       guardLink.phase!=GuardPhase::Idle||aiDecisionHold.expires||cinematicHold.id||(restoreTarget!=0&&restoreTarget!=2)) {
        ++loadingProbe.refused;return;
    }
    loadingProbe.armed=true;loadingProbe.armUntil=GetTickCount64()+15000;loadingProbe.result="armed";
    loadingProbe.armGame=observedGame;loadingProbe.restoreTarget=restoreTarget;++loadingProbe.armSequence;
    loadingProbe.loadingGame=0;loadingProbe.loadingEntryAt=0;
}
bool holdLoadingSwitch(const NativeSwitchEvent& e) {
    if(e.thread!=engineThread.load()||localRole!=1)return false;
    if(loadingProbe.held)return false;
    if(!loadingProbe.armed||e.tick>=loadingProbe.armUntil||e.caller!=0x7879cd||
       e.from!=15||e.to!=1||e.pending||e.phase!=3||!e.characterValid)return false;
    std::uintptr_t name=0;char map[192]{};float position[3]{};
    if(!memoryRead(e.game+0x337138,&name,8)||!nativeName(name,map,sizeof(map))||std::strcmp(map,kGuardMap)||
       !memoryRead(e.player+0x300,position,sizeof(position)))return false;
    for(float x:position)if(!std::isfinite(x))return false;
    loadingProbe.armed=false;loadingProbe.held=true;loadingProbe.startedAt=e.tick;loadingProbe.releasedAt=0;
    loadingProbe.manager=e.manager;loadingProbe.game=e.game;loadingProbe.player=e.player;
    std::memcpy(loadingProbe.position,position,sizeof(position));loadingProbe.maxDistance=0;loadingProbe.samples=0;
    ++loadingProbe.started;loadingProbe.result="holding";
    consoleOutput("COOP: entered native loading hold game=%llx manager=%llx thread=%u\n",e.game,e.manager,e.thread);
    networkFrameOwned.store(false,std::memory_order_release);
    return true;
}
void serviceLoadingProbe(std::uint64_t tick) {
    if(GetCurrentThreadId()!=engineThread.load())return;
    if(loadingProbe.armed&&tick>=loadingProbe.armUntil){loadingProbe.armed=false;loadingProbe.result="arm_expired";}
}
void waitLoadingProbe() {
    // Keep the native finish-load call stack intact. Returning before its
    // requested switch is committed breaks assumptions in subsequent native
    // completion work. Only read state and pump transport while it waits.
    const bool previousMutation=nativeMutationAllowed;nativeMutationAllowed=false;
    writeSwitchTrace();
    while(GetTickCount64()-loadingProbe.startedAt<2000) {
        NativeSwitchEvent e{};float position[3]{};
        if(!nativeSwitchContext(loadingProbe.manager,e)||e.game!=loadingProbe.game||e.from!=15||e.pending||e.phase!=3||
           e.player!=loadingProbe.player||!memoryRead(e.player+0x300,position,sizeof(position))) {
            ++loadingProbe.invalidated;loadingProbe.result="state_changed";break;
        }
        double squared=0;for(unsigned i=0;i<3;++i){const double d=position[i]-loadingProbe.position[i];squared+=d*d;}
        if(std::isfinite(squared)){const double distance=std::sqrt(squared);if(distance>loadingProbe.maxDistance)loadingProbe.maxDistance=distance;++loadingProbe.samples;}
        const auto tick=GetTickCount64();pumpTransport(tick);writeStatus(tick);Sleep(10);
    }
    nativeMutationAllowed=previousMutation;
    loadingProbe.held=false;loadingProbe.releasedAt=GetTickCount64();++loadingProbe.released;
    if(!std::strcmp(loadingProbe.result,"holding"))loadingProbe.result="released";
    consoleOutput("COOP: native loading hold ended duration=%llu samples=%llu\n",loadingProbe.releasedAt-loadingProbe.startedAt,loadingProbe.samples);
}
void traceLogicSwitch(void* manager,int target) {
    NativeSwitchEvent e{};e.tick=GetTickCount64();e.thread=GetCurrentThreadId();e.to=target;
    e.caller=reinterpret_cast<std::uintptr_t>(_ReturnAddress())-reinterpret_cast<std::uintptr_t>(gameImage);
    nativeSwitchContext(reinterpret_cast<std::uintptr_t>(manager),e);
    if(target==15){
        void* frames[40]{};const USHORT n=RtlCaptureStackBackTrace(1,40,frames,nullptr);
        const auto base=reinterpret_cast<std::uintptr_t>(gameImage);LoadingEntryStack s{};s.tick=e.tick;s.caller=static_cast<unsigned>(e.caller);
        for(USHORT i=0;i<n&&s.count<24;++i){const auto a=reinterpret_cast<std::uintptr_t>(frames[i]);if(a>base&&a<base+0x2000000)s.rva[s.count++]=static_cast<std::uint32_t>(a-base);}
        AcquireSRWLockExclusive(&switchTraceLock);s.sequence=++loadingEntryStackCount;loadingEntryStacks[(s.sequence-1)%4]=s;ReleaseSRWLockExclusive(&switchTraceLock);
    }
    if(e.thread==engineThread.load()&&e.caller==0x787a55&&target==15&&e.game&&e.manager==e.game+0x1f6ec8) {
        std::uintptr_t vt=0;unsigned count=99,pending=99,phase=99;
        if(memoryRead(e.game,&vt,8)&&vt==reinterpret_cast<std::uintptr_t>(gameImage)+0x1c940a8&&
           memoryRead(e.manager+0x530,&count,4)&&!(count&0xffffff)&&
           memoryRead(e.manager+0x538,&pending,4)&&!pending&&
           memoryRead(e.game+0x341058,&phase,4)&&phase<=3) {
            if(loadingProbe.armed&&e.tick<loadingProbe.armUntil){loadingProbe.loadingGame=e.game;loadingProbe.loadingEntryAt=e.tick;}
            checkpointObserveLoadingEntry(e);
        }
    }
    if(e.thread==engineThread.load()&&e.caller==0x7879cd&&target==1) {
        restoreLoadingRoleAtBoundary(e);
        nativeSwitchContext(reinterpret_cast<std::uintptr_t>(manager),e);
        e.tick=GetTickCount64();
    }
    if(checkpointOwnsLoading()) {
        if(checkpointLoadingBoundary(e))target=2;
    }else if(holdLoadingSwitch(e))waitLoadingProbe();
    else if(missionTransitionBoundary(e))waitMissionTransition();
    realLogicSwitch(manager,target);
    memoryRead(reinterpret_cast<std::uintptr_t>(manager)+0x538,&e.after,4);
    AcquireSRWLockExclusive(&switchTraceLock);
    e.sequence=++switchTraceSequence;switchEvents[(e.sequence-1)%32]=e;
    ReleaseSRWLockExclusive(&switchTraceLock);
}
bool installNativeSwitchTrace() {
    const unsigned char original[]={0x40,0x56,0x57,0x41,0x56,0x48,0x83,0xec,0x30,0x48,0xc7,0x44,0x24,0x20,0xfe,0xff,0xff,0xff};
    auto* target=gameImage+0x7885b0;
    if(std::memcmp(target,original,sizeof(original)))return false;
    auto* trampoline=static_cast<unsigned char*>(VirtualAlloc(nullptr,64,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));
    if(!trampoline)return false;
    std::memcpy(trampoline,original,sizeof(original));
    unsigned char jump[18]{0xff,0x25,0,0,0,0};void* resume=target+sizeof(original);
    std::memcpy(jump+6,&resume,8);std::memset(jump+14,0x90,4);
    std::memcpy(trampoline+sizeof(original),jump,14);
    DWORD old=0,unused=0;
    if(!VirtualProtect(trampoline,64,PAGE_EXECUTE_READ,&old)){VirtualFree(trampoline,0,MEM_RELEASE);return false;}
    FlushInstructionCache(GetCurrentProcess(),trampoline,64);
    realLogicSwitch=reinterpret_cast<NativeLogicSwitch>(trampoline);
    void* wrapper=reinterpret_cast<void*>(&traceLogicSwitch);std::memcpy(jump+6,&wrapper,8);
    if(!VirtualProtect(target,sizeof(jump),PAGE_EXECUTE_READWRITE,&old))return false;
    std::memcpy(target,jump,sizeof(jump));FlushInstructionCache(GetCurrentProcess(),target,sizeof(jump));
    return VirtualProtect(target,sizeof(jump),old,&unused)!=FALSE;
}
void writeSwitchTrace() {
    NativeSwitchEvent snapshot[32]{};std::uint64_t count=0;LoadingEntryStack stacks[4]{};std::uint64_t stackCount=0;
    AcquireSRWLockShared(&switchTraceLock);count=switchTraceSequence;std::memcpy(snapshot,switchEvents,sizeof(snapshot));stackCount=loadingEntryStackCount;std::memcpy(stacks,loadingEntryStacks,sizeof(stacks));ReleaseSRWLockShared(&switchTraceLock);
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.loading.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\"role\":%u,\"switches\":%llu,\"events\":[",localRole,count);
    const auto first=count>32?count-32:0;
    for(auto i=first;i<count;++i) {
        const auto& e=snapshot[i%32];
        fprintf(f,"%s{\"sequence\":%llu,\"tick\":%llu,\"thread\":%u,\"caller\":\"0x%llx\",\"manager\":\"0x%llx\",\"game\":\"0x%llx\",\"activeGame\":\"0x%llx\",\"player\":\"0x%llx\",\"from\":%u,\"to\":%d,\"pendingBefore\":%u,\"pendingAfter\":%u,\"worldPhase\":%u,\"characterValid\":%s,\"character\":%d}",
            i==first?"":",",e.sequence,e.tick,e.thread,e.caller,e.manager,e.game,e.activeGame,e.player,e.from,e.to,e.pending,e.after,e.phase,e.characterValid?"true":"false",e.character);
    }
    fprintf(f,"],\"entryStacks\":[");
    for(auto i=stackCount>4?stackCount-4:0;i<stackCount;++i){const auto& s=stacks[i%4];
        fprintf(f,"%s{\"sequence\":%llu,\"tick\":%llu,\"caller\":\"0x%x\",\"rva\":[",i==(stackCount>4?stackCount-4:0)?"":",",s.sequence,s.tick,s.caller);
        for(unsigned j=0;j<s.count;++j)fprintf(f,"%s\"0x%x\"",j?",":"",s.rva[j]);
        fprintf(f,"]}");}
    const auto& p=loadingProbe;
    fprintf(f,"],\"probe\":{\"armed\":%s,\"held\":%s,\"result\":\"%s\",\"startedAt\":%llu,\"releasedAt\":%llu,\"started\":%llu,\"released\":%llu,\"refused\":%llu,\"invalidated\":%llu,\"samples\":%llu,\"maxDistance\":%.9g,\"game\":\"0x%llx\",\"player\":\"0x%llx\"}}\n",
        p.armed?"true":"false",p.held?"true":"false",p.result,p.startedAt,p.releasedAt,p.started,p.released,p.refused,p.invalidated,p.samples,p.maxDistance,p.game,p.player);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
