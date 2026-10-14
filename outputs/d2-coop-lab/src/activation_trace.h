#pragma once
// 1163 read-only observer of the native idEntity::Activate (0x140875290,
// entity, activator): the path behind script triggers, relays and the
// console "trigger" command. Mission progression (objectives, level
// transitions) runs through it in each player's own world. This observer only
// records activations; the original call is always forwarded unchanged.
using NativeEntityActivate=void(*)(void*,void*);
NativeEntityActivate realEntityActivate=nullptr;
const unsigned char entityActivatePrefix[15]={0x48,0x89,0x5c,0x24,0x10,0x48,0x89,0x74,0x24,0x18,0x57,0x48,0x83,0xec,0x20};
struct ActivationEvent {
    std::uint64_t sequence=0,tick=0,epoch=0;
    std::uintptr_t entity=0,activator=0;
    std::uint32_t handle=0,activatorHandle=0,logic=0,flags=0;
    bool localActivator=false,engine=false;
};
constexpr unsigned kActivationCapacity=256;
ActivationEvent activationEvents[kActivationCapacity]{};
std::atomic<std::uint64_t> activationSequence{0};
SRWLOCK activationLock=SRWLOCK_INIT;
void observeEntityActivate(void* entity,void* activator){
    ActivationEvent e{};e.tick=GetTickCount64();e.epoch=outgoing.worldEpoch;e.logic=outgoing.gameLogicState;
    e.entity=reinterpret_cast<std::uintptr_t>(entity);e.activator=reinterpret_cast<std::uintptr_t>(activator);
    e.engine=GetCurrentThreadId()==engineThread.load();e.localActivator=e.activator&&e.activator==observedPlayer;
    if(e.entity){memoryRead(e.entity+0xa8,&e.handle,4);unsigned char f=0;memoryRead(e.entity+0xd0,&f,1);e.flags=f;}
    if(e.activator)memoryRead(e.activator+0xa8,&e.activatorHandle,4);
    AcquireSRWLockExclusive(&activationLock);
    e.sequence=++activationSequence;activationEvents[(e.sequence-1)%kActivationCapacity]=e;
    ReleaseSRWLockExclusive(&activationLock);
    realEntityActivate(entity,activator);
}
bool installEntityActivateObserver(){
    auto* target=gameImage+0x875290;if(std::memcmp(target,entityActivatePrefix,sizeof(entityActivatePrefix)))return false;
    // First 15 bytes are four complete instructions without branches or
    // RIP-relative operands; the original continues at entry+15.
    auto* trampoline=static_cast<unsigned char*>(VirtualAlloc(nullptr,64,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));if(!trampoline)return false;
    std::memcpy(trampoline,entityActivatePrefix,15);unsigned char jump[15]{0xff,0x25,0,0,0,0};jump[14]=0x90;
    void* resume=target+15;std::memcpy(jump+6,&resume,8);std::memcpy(trampoline+15,jump,14);
    DWORD old=0,unused=0;if(!VirtualProtect(trampoline,64,PAGE_EXECUTE_READ,&old)){VirtualFree(trampoline,0,MEM_RELEASE);return false;}
    FlushInstructionCache(GetCurrentProcess(),trampoline,64);void* wrapper=reinterpret_cast<void*>(&observeEntityActivate);std::memcpy(jump+6,&wrapper,8);
    if(!VirtualProtect(target,15,PAGE_EXECUTE_READWRITE,&old)){VirtualFree(trampoline,0,MEM_RELEASE);return false;}
    realEntityActivate=reinterpret_cast<NativeEntityActivate>(trampoline);std::memcpy(target,jump,15);FlushInstructionCache(GetCurrentProcess(),target,15);
    return VirtualProtect(target,15,old,&unused)!=0;
}
void writeActivationTrace(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.activations.json",statusPath)>=static_cast<int>(sizeof(path))||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=static_cast<int>(sizeof(temp)))return;
    ActivationEvent copy[kActivationCapacity];std::uint64_t last=0;
    AcquireSRWLockShared(&activationLock);last=activationSequence.load();std::memcpy(copy,activationEvents,sizeof(copy));ReleaseSRWLockShared(&activationLock);
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\"role\":%u,\"count\":%llu,\"events\":[",localRole,last);
    const auto first=last>kActivationCapacity?last-kActivationCapacity:0;
    for(auto i=first;i<last;++i){const auto& e=copy[i%kActivationCapacity];
        fprintf(f,"%s{\"sequence\":%llu,\"tick\":%llu,\"epoch\":%llu,\"handle\":\"0x%x\",\"entity\":\"0x%llx\",\"activator\":\"0x%llx\",\"activatorHandle\":\"0x%x\",\"local\":%s,\"engine\":%s,\"logic\":%u,\"flags\":%u}",
            i==first?"":",",e.sequence,e.tick,e.epoch,e.handle,e.entity,e.activator,e.activatorHandle,e.localActivator?"true":"false",e.engine?"true":"false",e.logic,e.flags);}
    fprintf(f,"]}\n");const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
