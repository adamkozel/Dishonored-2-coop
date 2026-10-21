#pragma once
#include <cstdio>
// 1214gr (#131) lab experiment: a native shade for Emily's Doppelganger in the host world. Her shade is the native clone
// models/powers/doppelganger/decoy_entity.def (an arkNPC in the player faction with the decoy AI, attention source and
// health; clone lifecycle v433), created by arkCpntPowerDoppelganger in her world only, so the host's NPCs never
// perceived it (carry-body-v1658b). The native cast FUN_14043c950 creates it through CreateClone (FUN_14043cdc0:
// FUN_1408ae9b0(game, definition, aim+0x94, the caster's orientation turned by pi)), and OnCloneSpawned (FUN_14043e0c0)
// posts an AI stimulus (FUN_1408112c0(FUN_140810240()+0xc8e8, 0x73, 0x10, handle): kind 5 and the clone position)
// (confirmed static). The host spawns the same definition at her streamed shade transform the same way, posts the same
// stimulus, keeps it at her shade (a native transform only when it drifts more than 1 m), and removes it with the native
// removal message (1439a6f30, also posted by OnCloneSpawned and the Mesmerize state 2) when her shade ends. One puppet
// per clone slot; a puppet lost natively (killed) is not respawned for the same cast. Host only, in an armed product
// session or the lab movement fixture on the guard map; coop_shade_native follow_off|follow_on (lab) for a control cast.
constexpr const char* kShadeHostDefinition="models/powers/doppelganger/decoy_entity.def";
struct ShadeHostPuppet {std::uintptr_t actor=0,game=0;unsigned handle=0,source=0;std::uint64_t epoch=0,event=0,lostEvent=0;ULONGLONG createdAt=0,seen=0;unsigned pins=0;float drift=0,at[3]{};};
std::array<ShadeHostPuppet,kDoppelgangerWireSlots> shadeHostPuppets{};
bool shadeHostFollowEnabled=true;
std::uint64_t shadeHostSpawned=0,shadeHostRemoved=0,shadeHostLost=0,shadeHostPins=0,shadeHostRefused=0,shadeHostStimuli=0;
const char* shadeHostResult="idle";
bool shadeHostApis(){
    static const unsigned char spawn[]={0x40,0x57,0x41,0x56,0x41,0x57,0x48,0x83,0xec,0x40,0x48,0xc7},
        message[]={0x40,0x53,0x48,0x83,0xec,0x30,0x45,0x33,0xc9,0x48,0x8b,0xda},
        post[]={0x40,0x53,0x48,0x83,0xec,0x20,0x0f,0xb6,0x01,0x48,0x8b,0xd9},
        ai[]={0x48,0x8b,0x05,0x49,0x50,0x19,0x03,0x48,0x05,0x68,0x89,0x1e},
        stimulus[]={0x48,0x89,0x5c,0x24,0x08,0x48,0x89,0x74,0x24,0x10,0x44,0x89},
        place[]={0x48,0x8b,0xc4,0x44,0x88,0x48,0x20,0x48,0x89,0x50,0x10,0x55};
    return gameImage&&mesmerizeHostApis()&&!std::memcmp(gameImage+0x8ae9b0,spawn,12)&&!std::memcmp(gameImage+0x8847f0,message,12)&&
        !std::memcmp(gameImage+0x882fc0,post,12)&&!std::memcmp(gameImage+0x810240,ai,12)&&!std::memcmp(gameImage+0x8112c0,stimulus,12)&&
        !std::memcmp(gameImage+0x87d200,place,12);
}
bool shadeHostAlive(const ShadeHostPuppet& p){return p.actor&&p.handle&&p.game==observedGame&&p.epoch==outgoing.worldEpoch&&mesmerizeHostResolve(p.handle)==p.actor;}
void shadeHostRemove(ShadeHostPuppet& p,const char* why){
    if(shadeHostAlive(p)){
        alignas(16) unsigned char message[64]{};
        reinterpret_cast<void*(*)(std::uintptr_t,void*,const void*)>(gameImage+0x8847f0)(p.actor,message,gameImage+0x39a6f30);
        reinterpret_cast<void(*)(void*)>(gameImage+0x882fc0)(message);++shadeHostRemoved;}
    const auto lost=p.lostEvent;p={};p.lostEvent=lost;shadeHostResult=why;
}
bool shadeHostSpawn(ShadeHostPuppet& p,const CoopDoppelgangerWorldPacket& q,ULONGLONG tick){
    const auto definition=lookupEntityDefinition(kShadeHostDefinition);if(!definition){shadeHostResult="definition_missing";++shadeHostRefused;return false;}
    void* actor=reinterpret_cast<void*(*)(std::uintptr_t,std::uintptr_t,const float*,const float*,std::uintptr_t,char,std::uintptr_t,std::uintptr_t,std::uintptr_t)>(gameImage+0x8ae9b0)(
        observedGame,definition,q.transform.data(),q.transform.data()+3,0,0,0,0,0);
    if(!actor){shadeHostResult="native_creation_refused";++shadeHostRefused;return false;}
    const auto lost=p.lostEvent;p={};p.lostEvent=lost;p.actor=reinterpret_cast<std::uintptr_t>(actor);p.game=observedGame;p.epoch=outgoing.worldEpoch;p.event=q.event;p.source=q.nativeHandle;
    p.createdAt=p.seen=tick;memoryRead(p.actor+0xa8,&p.handle,4);std::memcpy(p.at,q.transform.data(),12);++shadeHostSpawned;
    const auto ai=reinterpret_cast<std::uintptr_t(*)()>(gameImage+0x810240)();
    if(ai&&p.handle){auto* s=reinterpret_cast<unsigned*(*)(std::uintptr_t,unsigned,unsigned,unsigned)>(gameImage+0x8112c0)(ai+0xc8e8,0x73,0x10,p.handle);
        if(s){s[0]=5;std::memcpy(s+1,q.transform.data(),12);++shadeHostStimuli;}}
    shadeHostResult="spawned";return true;
}
void serviceShadeHost(ULONGLONG tick){
    if(GetCurrentThreadId()!=engineThread.load()||!nativeMutationAllowed)return;
    for(auto& p:shadeHostPuppets)if(p.actor&&(p.game!=observedGame||p.epoch!=outgoing.worldEpoch)){p={};shadeHostResult="world_retired";}
    if(!shadeHostApis())return;
    const bool streaming=localRole==0&&doppelgangerWorldCurrent(tick)&&movementBankActive()&&coopGameplayReady(outgoing)&&coopGameplayReady(remote);
    const bool scope=streaming&&(shadeHostFollowEnabled||productMode)&&
        (productSessionArmed()||(observerIsolation.ready&&observerIsolation.movementFixture&&!std::strcmp(mapName,kGuardMap)));
    for(unsigned k=0;k<kDoppelgangerWireSlots;++k){
        auto& p=shadeHostPuppets[k];const auto& s=doppelgangerWorldStreams[k];const auto& q=s.presented;
        const bool hers=scope&&s.lease>tick&&q.present&&q.event&&validDoppelgangerTransform(q.transform);
        if(p.actor&&!shadeHostAlive(p)){p.lostEvent=p.event;const auto lost=p.lostEvent;p={};p.lostEvent=lost;++shadeHostLost;shadeHostResult="puppet_lost";}
        if(p.actor&&hers&&q.event==p.event)p.seen=tick;
        if(p.actor&&((hers&&q.event!=p.event)||tick-p.seen>1000)){shadeHostRemove(p,"removed");}
        if(!hers||q.event==p.lostEvent)continue;
        if(!p.actor){shadeHostSpawn(p,q,tick);continue;}
        float at[3]{};if(!memoryRead(p.actor+0x300,at,12))continue;std::memcpy(p.at,at,12);
        float d=0;for(unsigned i=0;i<3;++i){const float x=at[i]-q.transform[i];d+=x*x;}p.drift=std::sqrt(d);
        if(p.drift>1.f){reinterpret_cast<void(*)(void*,const float*,const float*,bool,int)>(gameImage+0x87d200)(reinterpret_cast<void*>(p.actor),q.transform.data(),q.transform.data()+3,true,0);++p.pins;++shadeHostPins;}
    }
}
void writeShadeHost(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.shade-host.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;
    fprintf(f,"{\"result\":\"%s\",\"enabled\":%s,\"spawned\":%llu,\"removed\":%llu,\"lost\":%llu,\"pins\":%llu,\"refused\":%llu,\"stimuli\":%llu,\"slots\":[",
        shadeHostResult,shadeHostFollowEnabled?"true":"false",shadeHostSpawned,shadeHostRemoved,shadeHostLost,shadeHostPins,shadeHostRefused,shadeHostStimuli);
    for(unsigned k=0;k<kDoppelgangerWireSlots;++k){const auto& p=shadeHostPuppets[k];const auto& q=doppelgangerWorldStreams[k].presented;
        fprintf(f,"%s{\"puppet\":%s,\"handle\":%u,\"source\":%u,\"event\":%llu,\"lostEvent\":%llu,\"pins\":%u,\"drift\":%.3f,\"at\":[%.3f,%.3f,%.3f],\"partnerPresent\":%u,\"partnerEvent\":%llu,\"partnerAt\":[%.3f,%.3f,%.3f]}",
            k?",":"",p.actor?"true":"false",p.handle,p.source,p.event,p.lostEvent,p.pins,p.drift,p.at[0],p.at[1],p.at[2],q.present,q.event,q.transform[0],q.transform[1],q.transform[2]);}
    fputs("]}\n",f);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
