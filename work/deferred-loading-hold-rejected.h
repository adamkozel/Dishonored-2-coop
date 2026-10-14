#pragma once
// Native state-switch observation plus an explicitly armed, bounded local
// loading experiment. Normal requests are forwarded unchanged.
using NativeLogicSwitch=void(*)(void*,int);
NativeLogicSwitch realLogicSwitch=nullptr;
struct NativeSwitchEvent {
    std::uint64_t sequence=0,tick=0;
    std::uintptr_t manager=0,game=0,player=0,caller=0,activeGame=0;
    unsigned thread=0,from=0,pending=0,phase=0,after=0;
    int to=0,character=0;
    bool characterValid=false;
};
SRWLOCK switchTraceLock=SRWLOCK_INIT;
NativeSwitchEvent switchEvents[32]{};
std::uint64_t switchTraceSequence=0;
struct LoadingProbe {
    bool armed=false,held=false;
    std::uint64_t armUntil=0,startedAt=0,releasedAt=0,started=0,released=0,refused=0,invalidated=0,samples=0;
    std::uintptr_t manager=0,game=0,player=0;
    float position[3]{};double maxDistance=0;
    const char* result="idle";
};
LoadingProbe loadingProbe{};
bool nativeSwitchContext(std::uintptr_t manager,NativeSwitchEvent& e) {
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    std::uintptr_t vt=0,stack=0;unsigned count=0;
    e.manager=manager;
    if(!memoryRead(manager+8,&e.game,8)||!e.game||manager!=e.game+0x1f6ec8||
       !memoryRead(e.game,&vt,8)||vt!=base+0x1c940a8||!memoryRead(manager+0x530,&count,4)||
       !(count&0xffffff)||(count&0xffffff)>32||!memoryRead(manager+0x528,&stack,8)||!stack||
       !memoryRead(stack+((count&0xffffff)-1)*4,&e.from,4)||e.from>16||
       !memoryRead(manager+0x538,&e.pending,4)||e.pending>16)return false;
    memoryRead(e.game+0x341058,&e.phase,4);memoryRead(e.game+0x3130b0,&e.player,8);
    NativeCharacterState s{};
    memoryRead(base+0x39a5290,&e.activeGame,8);
    e.characterValid=readNativeCharacter(memoryRead,base,s,e.game);
    e.character=s.character;
    return true;
}
void armLoadingProbe() {
    if(localRole!=1||GetCurrentThreadId()!=engineThread.load()||!realLogicSwitch||
       !coopGameplayReady(outgoing)||std::strcmp(mapName,kGuardMap)||loadingProbe.armed||loadingProbe.held||
       guardLink.phase!=GuardPhase::Idle||aiDecisionHold.expires||cinematicHold.id) {
        ++loadingProbe.refused;return;
    }
    loadingProbe.armed=true;loadingProbe.armUntil=GetTickCount64()+15000;loadingProbe.result="armed";
}
bool holdLoadingSwitch(const NativeSwitchEvent& e) {
    if(e.thread!=engineThread.load()||localRole!=1)return false;
    if(loadingProbe.held)return e.manager==loadingProbe.manager&&e.from==15&&e.to==1&&e.pending==0;
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
    networkFrameOwned.store(false,std::memory_order_release);
    return true;
}
void serviceLoadingProbe(std::uint64_t tick) {
    if(GetCurrentThreadId()!=engineThread.load())return;
    if(loadingProbe.armed&&tick>=loadingProbe.armUntil){loadingProbe.armed=false;loadingProbe.result="arm_expired";}
    if(!loadingProbe.held)return;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    std::uintptr_t game=0;NativeSwitchEvent e{};
    // The session owns this world after load completion. Never dereference a
    // saved manager unless the session still owns the same game object.
    if(!memoryRead(base+0x25af110+0x19a8,&game,8)||game!=loadingProbe.game) {
        if(tick-loadingProbe.startedAt<1000)return;
        loadingProbe.held=false;++loadingProbe.invalidated;loadingProbe.result="owner_changed";return;
    }
    if(!nativeSwitchContext(loadingProbe.manager,e)||e.game!=game||e.from!=15||e.pending||e.phase!=3) {
        loadingProbe.held=false;++loadingProbe.invalidated;loadingProbe.result="state_changed";return;
    }
    float position[3]{};
    if(e.player==loadingProbe.player&&memoryRead(e.player+0x300,position,sizeof(position))) {
        double squared=0;for(unsigned i=0;i<3;++i){const double d=position[i]-loadingProbe.position[i];squared+=d*d;}
        if(std::isfinite(squared)){const double distance=std::sqrt(squared);if(distance>loadingProbe.maxDistance)loadingProbe.maxDistance=distance;++loadingProbe.samples;}
    }
    if(tick-loadingProbe.startedAt<2000)return;
    loadingProbe.held=false;realLogicSwitch(reinterpret_cast<void*>(loadingProbe.manager),1);
    loadingProbe.releasedAt=tick;++loadingProbe.released;loadingProbe.result="released";
}
void traceLogicSwitch(void* manager,int target) {
    NativeSwitchEvent e{};e.tick=GetTickCount64();e.thread=GetCurrentThreadId();e.to=target;
    e.caller=reinterpret_cast<std::uintptr_t>(_ReturnAddress())-reinterpret_cast<std::uintptr_t>(gameImage);
    nativeSwitchContext(reinterpret_cast<std::uintptr_t>(manager),e);
    if(!holdLoadingSwitch(e))realLogicSwitch(manager,target);
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
    NativeSwitchEvent snapshot[32]{};std::uint64_t count=0;
    AcquireSRWLockShared(&switchTraceLock);count=switchTraceSequence;std::memcpy(snapshot,switchEvents,sizeof(snapshot));ReleaseSRWLockShared(&switchTraceLock);
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
    const auto& p=loadingProbe;
    fprintf(f,"],\"probe\":{\"armed\":%s,\"held\":%s,\"result\":\"%s\",\"startedAt\":%llu,\"releasedAt\":%llu,\"started\":%llu,\"released\":%llu,\"refused\":%llu,\"invalidated\":%llu,\"samples\":%llu,\"maxDistance\":%.9g,\"game\":\"0x%llx\",\"player\":\"0x%llx\"}}\n",
        p.armed?"true":"false",p.held?"true":"false",p.result,p.startedAt,p.releasedAt,p.started,p.released,p.refused,p.invalidated,p.samples,p.maxDistance,p.game,p.player);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
