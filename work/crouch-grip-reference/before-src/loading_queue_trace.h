#pragma once
// Observe the native replacement queue at its existing engine scheduling point.
using NativeReplacementUpdate=void(*)(void*);
NativeReplacementUpdate realReplacementUpdate=nullptr;
NativeSwitchEvent queueEvents[32]{};
std::uint64_t queueTraceSequence=0,queueCalls=0;
NativeSwitchEvent lastQueueContext{};
struct LoadingRoleRestore {
    std::uint64_t arm=0,queuedAt=0,completedAt=0;
    std::uintptr_t game=0,beforePlayer=0,afterPlayer=0;
    unsigned beforeLogic=0,afterLogic=0,beforePhase=0,afterPhase=0,queueAfter=0;
    int beforeCharacter=0,afterCharacter=0;
    bool completed=false;
    NativeHealth checkpointHealth{};
    float replacedHealth=0,restoredHealth=0;
    bool healthRestored=false;
    float position[3]{};double displacement=0;
    const char* result="idle";
};
LoadingRoleRestore loadingRoleRestore{};
bool queueLoadingRole(const NativeSwitchEvent& e) {
    if(loadingRoleRestore.arm!=loadingProbe.armSequence){loadingRoleRestore={};loadingRoleRestore.arm=loadingProbe.armSequence;}
    if(localRole!=1||!loadingProbe.armed||loadingProbe.restoreTarget!=2||e.tick>=loadingProbe.armUntil||
       loadingRoleRestore.queuedAt||!loadingProbe.loadingEntryAt||e.tick<loadingProbe.loadingEntryAt||e.game!=loadingProbe.loadingGame||e.activeGame!=e.game||e.from!=15||e.pending||
       e.phase!=3||!e.characterValid||e.character!=1)return false;
    const unsigned char signature[]={0x48,0x89,0x5c,0x24,0x08,0x48,0x89,0x74,0x24,0x10,0x57,0x48,0x83,0xec,0x50,0x8b,0xfa,0x48,0x8b,0xd9};
    NativeCharacterState s{};std::uintptr_t name=0;char map[192]{};
    const unsigned char healthSetter[]={0x48,0x8b,0xc4,0x48,0x89,0x58,0x08,0x57,0x48,0x81,0xec,0xa0,0,0,0};
    if(std::memcmp(gameImage+0x8a7650,signature,sizeof(signature))||
       !readNativeCharacter(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),s,e.game)||s.player!=e.player||s.queued||
       std::strcmp(s.definitionName,s.variantNames[1])||!memoryRead(e.game+0x337138,&name,8)||
       !nativeName(name,map,sizeof(map))||std::strcmp(map,kGuardMap)||
       !memoryRead(e.player+0x300,loadingRoleRestore.position,sizeof(loadingRoleRestore.position))||
       std::memcmp(gameImage+0x7185b0,healthSetter,sizeof(healthSetter))||
       !readNativeHealth(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),1,loadingRoleRestore.checkpointHealth,true)||
       loadingRoleRestore.checkpointHealth.game!=e.game||loadingRoleRestore.checkpointHealth.actor!=e.player||
       loadingRoleRestore.checkpointHealth.current<=0||loadingRoleRestore.checkpointHealth.current>loadingRoleRestore.checkpointHealth.maximum)return false;
    for(float p:loadingRoleRestore.position)if(!std::isfinite(p))return false;
    auto& r=loadingRoleRestore;r.game=e.game;r.beforePlayer=e.player;r.beforeLogic=e.from;r.beforePhase=e.phase;
    r.beforeCharacter=e.character;r.queuedAt=e.tick;r.result="queued";
    reinterpret_cast<void(*)(void*,int)>(gameImage+0x8a7650)(reinterpret_cast<void*>(e.game),2);
    consoleOutput("COOP: queued fully loaded character restore game=%llx logic=%u phase=%u\n",e.game,e.from,e.phase);
    return true;
}
void checkLoadingRole(void* game) {
    auto& r=loadingRoleRestore;NativeSwitchEvent e{};NativeCharacterState s{};float position[3]{};
    if(!nativeSwitchContext(reinterpret_cast<std::uintptr_t>(game)+0x1f6ec8,e)||
       !readNativeCharacter(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),s,e.game)||
       !memoryRead(s.player+0x300,position,sizeof(position))){r.result="unreadable_after_update";return;}
    r.afterPlayer=s.player;r.afterLogic=e.from;r.afterPhase=e.phase;r.afterCharacter=s.character;r.queueAfter=s.queued;
    double distance=0;for(unsigned i=0;i<3;++i){const double d=position[i]-r.position[i];distance+=d*d;}r.displacement=std::sqrt(distance);
    r.completed=e.game==r.game&&e.activeGame==e.game&&e.from==15&&!e.pending&&e.phase==3&&s.character==2&&
        !s.queued&&s.player!=r.beforePlayer&&!std::strcmp(s.definitionName,s.variantNames[2])&&std::isfinite(r.displacement)&&r.displacement<0.001;
    // The shipped character-choice replacement creates a full-health player.
    // Carry the loaded checkpoint's actual health through that replacement,
    // using its native setter while both worlds still remain in loading.
    if(r.completed) {
        NativeHealth before{},after{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
        r.completed=readNativeHealth(memoryRead,base,1,before,true)&&before.game==r.game&&before.actor==s.player&&
            before.maximum==r.checkpointHealth.maximum&&before.current>0&&r.checkpointHealth.current<=before.maximum;
        if(r.completed) {
            r.replacedHealth=before.current;
            reinterpret_cast<void(*)(void*,float)>(gameImage+0x7185b0)(reinterpret_cast<void*>(before.component),r.checkpointHealth.current);
            r.healthRestored=readNativeHealth(memoryRead,base,1,after,true)&&after.actor==before.actor&&after.component==before.component&&
                after.game==r.game&&after.current==r.checkpointHealth.current&&after.maximum==r.checkpointHealth.maximum;
            r.restoredHealth=after.current;r.completed=r.healthRestored;
        }
    }
    r.completedAt=GetTickCount64();r.result=r.completed?"restored_before_gameplay":"restore_not_verified";
    consoleOutput("COOP: loading role result=%s character=%d logic=%u phase=%u queue=%u\n",r.result,s.character,e.from,e.phase,s.queued);
}
void restoreLoadingRoleAtBoundary(const NativeSwitchEvent& event) {
    if(!loadingProbe.armed||loadingProbe.restoreTarget!=2||localRole!=1||event.from!=15||event.pending||event.phase!=3||
       event.to!=1||event.caller!=0x7879cd||event.thread!=engineThread.load()||!realReplacementUpdate)return;
    const unsigned char enter[]={0x48,0x8b,0x05,0xc9,0x24,0x10,0x03,0x48,0x89,0x01,0x48,0x89,0x15,0xbf,0x24,0x10,0x03,0x48,0x8b,0xc1,0xc3};
    const unsigned char leave[]={0x48,0x8b,0x01,0x48,0x89,0x05,0xa6,0x24,0x10,0x03,0xc3};
    const unsigned char update[]={0x48,0x89,0x4c,0x24,0x08,0x55,0x53,0x56,0x57,0x41,0x54,0x41,0x55,0x41,0x56,0x41,0x57,0x48,0x83,0xec,0x78};
    std::uintptr_t sessionGame=0;
    if(std::memcmp(gameImage+0x8a2dc0,enter,sizeof(enter))||std::memcmp(gameImage+0x8a2de0,leave,sizeof(leave))||std::memcmp(gameImage+0x8ab380,update,sizeof(update))||
       !memoryRead(reinterpret_cast<std::uintptr_t>(gameImage)+0x25af110+0x19a8,&sessionGame,8)||sessionGame!=event.game)return;
    // Use the shipped scoped-game helper used by CommitSwitch itself, then
    // process only the single character replacement requested by this probe.
    std::uintptr_t previous=0;
    reinterpret_cast<void(*)(void*,void*,bool)>(gameImage+0x8a2dc0)(&previous,reinterpret_cast<void*>(event.game),true);
    auto scoped=event;scoped.activeGame=event.game;
    if(queueLoadingRole(scoped)) {
        // The inner replacement pass alone leaves the queue live and the new
        // entity at its initial transform. Its native owner also initializes
        // entities, dispatches component work, restores the saved transform,
        // and clears the queue. Run that complete maintenance transaction.
        reinterpret_cast<void(*)(void*)>(gameImage+0x8ab380)(reinterpret_cast<void*>(event.game));
        checkLoadingRole(reinterpret_cast<void*>(event.game));
    }
    reinterpret_cast<void(*)(void*)>(gameImage+0x8a2de0)(&previous);
}
void observeReplacementUpdate(void* game) {
    if(GetCurrentThreadId()==engineThread.load()) {
        NativeSwitchEvent e{};e.tick=GetTickCount64();e.thread=GetCurrentThreadId();
        e.caller=reinterpret_cast<std::uintptr_t>(_ReturnAddress())-reinterpret_cast<std::uintptr_t>(gameImage);
        if(nativeSwitchContext(reinterpret_cast<std::uintptr_t>(game)+0x1f6ec8,e)) {
            ++queueCalls;
            const auto& p=lastQueueContext;
            if(e.game!=p.game||e.player!=p.player||e.from!=p.from||e.pending!=p.pending||e.phase!=p.phase||
               e.characterValid!=p.characterValid||e.character!=p.character) {
                e.sequence=++queueTraceSequence;queueEvents[(e.sequence-1)%32]=e;lastQueueContext=e;
            }
        }
    }
    realReplacementUpdate(game);
}
bool installReplacementTrace() {
    const unsigned char original[]={0x48,0x8b,0xc4,0x48,0x89,0x48,0x08,0x55,0x53,0x56,0x57,0x41,0x54,0x41,0x55,0x41,0x56,0x41,0x57};
    auto* target=gameImage+0x8a8fb0;
    if(std::memcmp(target,original,sizeof(original)))return false;
    auto* trampoline=static_cast<unsigned char*>(VirtualAlloc(nullptr,64,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));
    if(!trampoline)return false;
    std::memcpy(trampoline,original,sizeof(original));
    unsigned char jump[19]{0xff,0x25,0,0,0,0};void* resume=target+sizeof(original);
    std::memcpy(jump+6,&resume,8);std::memset(jump+14,0x90,5);
    std::memcpy(trampoline+sizeof(original),jump,14);
    DWORD old=0,unused=0;
    if(!VirtualProtect(trampoline,64,PAGE_EXECUTE_READ,&old)){VirtualFree(trampoline,0,MEM_RELEASE);return false;}
    FlushInstructionCache(GetCurrentProcess(),trampoline,64);
    realReplacementUpdate=reinterpret_cast<NativeReplacementUpdate>(trampoline);
    void* wrapper=reinterpret_cast<void*>(&observeReplacementUpdate);std::memcpy(jump+6,&wrapper,8);
    if(!VirtualProtect(target,sizeof(jump),PAGE_EXECUTE_READWRITE,&old))return false;
    std::memcpy(target,jump,sizeof(jump));FlushInstructionCache(GetCurrentProcess(),target,sizeof(jump));
    return VirtualProtect(target,sizeof(jump),old,&unused)!=FALSE;
}
void writeReplacementTrace() {
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.replacement.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\"role\":%u,\"calls\":%llu,\"changes\":%llu,\"events\":[",localRole,queueCalls,queueTraceSequence);
    const auto first=queueTraceSequence>32?queueTraceSequence-32:0;
    for(auto i=first;i<queueTraceSequence;++i) {
        const auto& e=queueEvents[i%32];
        fprintf(f,"%s{\"sequence\":%llu,\"tick\":%llu,\"caller\":\"0x%llx\",\"game\":\"0x%llx\",\"activeGame\":\"0x%llx\",\"player\":\"0x%llx\",\"logic\":%u,\"pending\":%u,\"worldPhase\":%u,\"characterValid\":%s,\"character\":%d}",
            i==first?"":",",e.sequence,e.tick,e.caller,e.game,e.activeGame,e.player,e.from,e.pending,e.phase,e.characterValid?"true":"false",e.character);
    }
    const auto& r=loadingRoleRestore;
    fprintf(f,"],\"restore\":{\"arm\":%llu,\"queuedAt\":%llu,\"completedAt\":%llu,\"completed\":%s,\"result\":\"%s\",\"game\":\"0x%llx\",\"beforePlayer\":\"0x%llx\",\"afterPlayer\":\"0x%llx\",\"beforeLogic\":%u,\"afterLogic\":%u,\"beforePhase\":%u,\"afterPhase\":%u,\"beforeCharacter\":%d,\"afterCharacter\":%d,\"queueAfter\":%u,\"displacement\":%.9g,\"checkpointHealth\":%.9g,\"replacementHealth\":%.9g,\"restoredHealth\":%.9g,\"healthRestored\":%s}}\n",
        r.arm,r.queuedAt,r.completedAt,r.completed?"true":"false",r.result,r.game,r.beforePlayer,r.afterPlayer,r.beforeLogic,r.afterLogic,r.beforePhase,r.afterPhase,r.beforeCharacter,r.afterCharacter,r.queueAfter,r.displacement,r.checkpointHealth.current,r.replacedHealth,r.restoredHealth,r.healthRestored?"true":"false");
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}

