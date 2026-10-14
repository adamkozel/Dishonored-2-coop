// Included in the private bridge namespace. Local research only: the hook
// calls the original once for ordinary damage. The owned Doppelganger
// presentation firewall may return an empty native result before observation.
// Captures are bounded, opt-in, and are not network replication commands.
using NativeDamage=void(*)(void*,void*,void*);
NativeDamage realNativeDamage=nullptr;
std::atomic<ULONGLONG> damageObserveUntil{0};
std::atomic<std::uint64_t> damageObserveGeneration{0};
SRWLOCK damageObserveLock=SRWLOCK_INIT;
struct DamageObservation {
    ULONGLONG tick=0;DWORD thread=0;NativeHealth before{},after{};bool afterValid=false;
    std::uint32_t inflictorHandle=0,attackerHandle=0,protectedRegions=0;std::uint64_t flags=0;
    float resultBefore=0,damageAmount=0,inflictedDamage=0,manaDamage=0;unsigned char resultFlags=0;
    float actorPosition[3]{};
    std::array<unsigned char,144> info{};
};
std::array<DamageObservation,128> damageObservations{};
unsigned damageObservationCount=0;std::uint64_t damageObservationRejected=0,damageObservationOverflow=0;
void armDamageObservation(bool enable){
    AcquireSRWLockExclusive(&damageObserveLock);
    if(!enable){damageObserveUntil.store(0);damageObserveGeneration.fetch_add(1);}
    else if(damageObserveUntil.load()<=GetTickCount64()){
        damageObservationCount=0;damageObservationRejected=damageObservationOverflow=0;
        damageObserveGeneration.fetch_add(1);damageObserveUntil.store(GetTickCount64()+60000);
    }
    ReleaseSRWLockExclusive(&damageObserveLock);
}
void observeNativeDamage(void* component,void* info,void* result){
    if(suppressShadowDamage(info,result))return;
    const auto tick=GetTickCount64(),generation=damageObserveGeneration.load();
    if(tick>=damageObserveUntil.load()){realNativeDamage(component,info,result);return;}
    DamageObservation e{};e.tick=tick;e.thread=GetCurrentThreadId();
    const auto c=reinterpret_cast<std::uintptr_t>(component),i=reinterpret_cast<std::uintptr_t>(info),r=reinterpret_cast<std::uintptr_t>(result);
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t owner=0;std::uint32_t handle=0;
    const bool valid=memoryRead(c+0x10,&owner,8)&&owner&&memoryRead(owner+0xa8,&handle,4)&&
        readNativeHealth(memoryRead,base,handle,e.before)&&e.before.component==c&&e.before.actor==owner&&e.before.logic==1&&
        memoryRead(i,e.info.data(),e.info.size())&&memoryRead(r,&e.resultBefore,4)&&std::isfinite(e.resultBefore)&&
        memoryRead(owner+0x300,e.actorPosition,12)&&std::isfinite(e.actorPosition[0])&&std::isfinite(e.actorPosition[1])&&std::isfinite(e.actorPosition[2]);
    realNativeDamage(component,info,result);
    bool captured=valid;
    if(captured){
        // Offsets independently recovered from arkDamageParams reflection at
        // 15a1b40 and arkDamageResult reflection at 15a2570.
        std::memcpy(&e.flags,e.info.data()+0x10,8);std::memcpy(&e.inflictorHandle,e.info.data()+0x50,4);
        std::memcpy(&e.attackerHandle,e.info.data()+0x54,4);std::memcpy(&e.damageAmount,e.info.data()+8,4);
        e.afterValid=readNativeHealth(memoryRead,base,handle,e.after)&&e.after.game==e.before.game&&
            e.after.actor==e.before.actor&&e.after.component==e.before.component&&e.after.declaration==e.before.declaration;
        captured=memoryRead(r,&e.inflictedDamage,4)&&memoryRead(r+4,&e.manaDamage,4)&&memoryRead(r+8,&e.protectedRegions,4)&&
            memoryRead(r+0x18,&e.resultFlags,1)&&std::isfinite(e.inflictedDamage)&&std::isfinite(e.manaDamage)&&std::isfinite(e.damageAmount);
    }
    AcquireSRWLockExclusive(&damageObserveLock);
    if(generation==damageObserveGeneration.load()&&GetTickCount64()<damageObserveUntil.load()){
        if(!captured)++damageObservationRejected;
        else if(damageObservationCount<damageObservations.size())damageObservations[damageObservationCount++]=e;
        else ++damageObservationOverflow;
    }
    ReleaseSRWLockExclusive(&damageObserveLock);
}
void writeDamageObservation(ULONGLONG tick){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.damage.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    // Copy the bounded record outside the native callback; no file IO is done
    // while holding a lock needed by a damage event.
    std::array<DamageObservation,128> copy{};unsigned count=0;std::uint64_t refused=0,overflow=0,generation=0;ULONGLONG until=0;
    AcquireSRWLockShared(&damageObserveLock);copy=damageObservations;count=damageObservationCount;refused=damageObservationRejected;
    overflow=damageObservationOverflow;generation=damageObserveGeneration.load();until=damageObserveUntil.load();ReleaseSRWLockShared(&damageObserveLock);
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\"observationOnly\":true,\"active\":%s,\"until\":%llu,\"generation\":%llu,\"rejected\":%llu,\"overflow\":%llu,\"events\":[",tick<until?"true":"false",until,generation,refused,overflow);
    for(unsigned n=0;n<count;++n){const auto& e=copy[n];
        fprintf(f,"%s{\"tick\":%llu,\"thread\":%lu,\"game\":\"%llx\",\"actor\":\"%llx\",\"component\":\"%llx\",\"handle\":%u,\"inflictorHandle\":%u,\"attackerHandle\":%u,\"damageAmount\":%.9g,\"damageType\":\"%llx\",\"before\":%.9g,\"maximumBefore\":%.9g,\"afterValid\":%s,\"after\":%.9g,\"logicAfter\":%u,\"resultBefore\":%.9g,\"inflictedDamage\":%.9g,\"manaDamage\":%.9g,\"protectedRegions\":%u,\"resultFlags\":%u,\"rawInfo\":\"",
            n?",":"",e.tick,e.thread,e.before.game,e.before.actor,e.before.component,e.before.handle,e.inflictorHandle,e.attackerHandle,e.damageAmount,e.flags,e.before.current,e.before.maximum,
            e.afterValid?"true":"false",e.afterValid?e.after.current:0.f,e.afterValid?e.after.logic:0,e.resultBefore,e.inflictedDamage,e.manaDamage,e.protectedRegions,static_cast<unsigned>(e.resultFlags));
        for(auto byte:e.info)fprintf(f,"%02x",byte);fputs("\"}",f);
    }
    fputs("]}\n",f);const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
bool installDamageObserver(){
    // Fifteen bytes of complete non-relative instructions, including the live
    // RAX=entry RSP used by the untouched prologue. FF25 preserves registers.
    const unsigned char prologue[]={0x48,0x8b,0xc4,0x48,0x89,0x58,0x10,0x48,0x89,0x68,0x18,0x48,0x89,0x70,0x20};
    auto* target=gameImage+0x717950;if(std::memcmp(target,prologue,sizeof(prologue)))return false;
    auto* trampoline=static_cast<unsigned char*>(VirtualAlloc(nullptr,64,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));if(!trampoline)return false;
    std::memcpy(trampoline,prologue,sizeof(prologue));
    unsigned char jump[15]{0xff,0x25,0,0,0,0};void* resume=target+sizeof(prologue);std::memcpy(jump+6,&resume,8);jump[14]=0x90;
    std::memcpy(trampoline+sizeof(prologue),jump,14);DWORD old=0,unused=0;
    if(!VirtualProtect(trampoline,64,PAGE_EXECUTE_READ,&old)){VirtualFree(trampoline,0,MEM_RELEASE);return false;}
    FlushInstructionCache(GetCurrentProcess(),trampoline,64);realNativeDamage=reinterpret_cast<NativeDamage>(trampoline);
    void* wrapper=reinterpret_cast<void*>(&observeNativeDamage);std::memcpy(jump+6,&wrapper,8);
    if(!VirtualProtect(target,sizeof(jump),PAGE_EXECUTE_READWRITE,&old))return false;
    std::memcpy(target,jump,sizeof(jump));FlushInstructionCache(GetCurrentProcess(),target,sizeof(jump));
    return VirtualProtect(target,sizeof(jump),old,&unused)!=0;
}
