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
// 1214gu: with Deadly Shade her clone is the deadly shadow (a combat NPC); the puppet uses the same definition, so it
// fights the host's NPCs natively.
constexpr const char* kShadeHostDeadlyDefinition="models/powers/doppelganger/deadlyshadow_entity.def";
struct ShadeHostPuppet {std::uintptr_t actor=0,game=0;unsigned handle=0,source=0,kind=0;std::uint64_t epoch=0,event=0,lostEvent=0;ULONGLONG createdAt=0,seen=0;unsigned pins=0;float drift=0,at[3]{};};
std::array<ShadeHostPuppet,kDoppelgangerWireSlots> shadeHostPuppets{};
bool shadeHostFollowEnabled=true;
std::uint64_t shadeHostSpawned=0,shadeHostRemoved=0,shadeHostLost=0,shadeHostPins=0,shadeHostRefused=0,shadeHostStimuli=0;
// 1214gt: her shade runs (the native decoy flees: 25 m in 7 s in partner-shade-v1727c) while the host puppet stood, so
// the 1 m pin teleported it 62 times. A native transform is a teleport that refreshes observable membership (1BF/1BE;
// avatar_perception.h), which would drop the puppet from the guards' perception each time. The puppet now follows her
// shade every frame: moves under 3 m run inside the partner avatar's continuous-transform scope (the observable refresh
// keeps its membership for that owner), larger jumps are plain teleports.
std::uint64_t shadeHostContinuous=0,shadeHostTeleports=0;
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
// 1214gs: a player component update (FUN_1403a9f90, its owner cast to the player by FUN_1408f2e90) lists the player's
// clones through the player's Doppelganger power: *(player+0x398), its handle array +0xb8 and count +0xc0 (the same
// fields FUN_14043c950 keeps), with no null check (confirmed static). Corvo has no Doppelganger, so the host crashed there
// right after its puppet appeared (partner-shade-v1727a: c0000005 at 3a9fd8 reading 0xb8). While host puppets exist
// the host player gets a phantom power block (zeroed; +0xb8 its own handle array of the live puppets, +0xc0 their
// count, +0xd4 the last one) in that slot when it is null, as the Mesmerize phantom slot does (1214gm); it is set before
// a spawn and cleared when no puppet remains, on a world change, or if the slot is no longer ours.
std::uintptr_t shadeHostPhantom=0,shadeHostPhantomPlayer=0;bool shadeHostPhantomSet=false,shadeHostSlotWasSet=false;
std::uint64_t shadeHostPhantomOns=0,shadeHostPhantomOffs=0;ULONGLONG shadeHostLastPuppetAt=0;
bool shadeHostPhantomOn(){
    const auto player=mesmerizeHostLocalPlayer();std::uintptr_t slot=0;if(!player||!memoryRead(player+0x398,&slot,8))return false;
    if(slot&&slot!=shadeHostPhantom){shadeHostSlotWasSet=true;return false;} // a real power: never replaced
    if(!shadeHostPhantom){shadeHostPhantom=reinterpret_cast<std::uintptr_t>(VirtualAlloc(nullptr,0x1000,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));if(!shadeHostPhantom)return false;
        *reinterpret_cast<std::uintptr_t*>(shadeHostPhantom+0xb8)=shadeHostPhantom+0x800;*reinterpret_cast<unsigned*>(shadeHostPhantom+0xd4)=0x3fff0010;}
    if(!slot){*reinterpret_cast<std::uintptr_t*>(player+0x398)=shadeHostPhantom;++shadeHostPhantomOns;}
    shadeHostPhantomPlayer=player;shadeHostPhantomSet=true;return true;}
void shadeHostPhantomOff(){
    if(!shadeHostPhantomSet)return;std::uintptr_t slot=0;
    if(shadeHostPhantomPlayer&&shadeHostPhantomPlayer==mesmerizeHostLocalPlayer()&&memoryRead(shadeHostPhantomPlayer+0x398,&slot,8)&&slot==shadeHostPhantom)
        *reinterpret_cast<std::uintptr_t*>(shadeHostPhantomPlayer+0x398)=0;
    shadeHostPhantomSet=false;shadeHostPhantomPlayer=0;++shadeHostPhantomOffs;}
// The phantom's clone list: the live puppets' handles (the extra handle is a puppet about to be counted).
void shadeHostPhantomList(unsigned extra=0){
    if(!shadeHostPhantom)return;unsigned n=0,last=0x3fff0010;auto* list=reinterpret_cast<unsigned*>(shadeHostPhantom+0x800);
    for(const auto& p:shadeHostPuppets)if(p.actor&&p.handle&&n<8){list[n++]=p.handle;last=p.handle;}
    if(extra&&n<8){list[n++]=extra;last=extra;}
    *reinterpret_cast<unsigned*>(shadeHostPhantom+0xc0)=n;*reinterpret_cast<unsigned*>(shadeHostPhantom+0xd4)=last;}
bool shadeHostAlive(const ShadeHostPuppet& p){return p.actor&&p.handle&&p.game==observedGame&&p.epoch==outgoing.worldEpoch&&mesmerizeHostResolve(p.handle)==p.actor;}
void shadeHostRemove(ShadeHostPuppet& p,const char* why){
    if(shadeHostAlive(p)){
        alignas(16) unsigned char message[64]{};
        reinterpret_cast<void*(*)(std::uintptr_t,void*,const void*)>(gameImage+0x8847f0)(p.actor,message,gameImage+0x39a6f30);
        reinterpret_cast<void(*)(void*)>(gameImage+0x882fc0)(message);++shadeHostRemoved;}
    const auto lost=p.lostEvent;p={};p.lostEvent=lost;shadeHostResult=why;
}
bool shadeHostSpawn(ShadeHostPuppet& p,const CoopDoppelgangerWorldPacket& q,ULONGLONG tick){
    const auto definition=lookupEntityDefinition(q.kind==2?kShadeHostDeadlyDefinition:kShadeHostDefinition);if(!definition){shadeHostResult="definition_missing";++shadeHostRefused;return false;}
    if(!shadeHostPhantomOn()){shadeHostResult=shadeHostSlotWasSet?"player_has_doppelganger":"phantom_refused";++shadeHostRefused;return false;}
    void* actor=reinterpret_cast<void*(*)(std::uintptr_t,std::uintptr_t,const float*,const float*,std::uintptr_t,char,std::uintptr_t,std::uintptr_t,std::uintptr_t)>(gameImage+0x8ae9b0)(
        observedGame,definition,q.transform.data(),q.transform.data()+3,0,0,0,0,0);
    if(!actor){shadeHostResult="native_creation_refused";++shadeHostRefused;return false;}
    const auto lost=p.lostEvent;p={};p.lostEvent=lost;p.actor=reinterpret_cast<std::uintptr_t>(actor);p.game=observedGame;p.epoch=outgoing.worldEpoch;p.event=q.event;p.source=q.nativeHandle;p.kind=q.kind==2?2u:1u;
    p.createdAt=p.seen=tick;memoryRead(p.actor+0xa8,&p.handle,4);std::memcpy(p.at,q.transform.data(),12);++shadeHostSpawned;shadeHostPhantomList();
    const auto ai=reinterpret_cast<std::uintptr_t(*)()>(gameImage+0x810240)();
    if(ai&&p.handle){auto* s=reinterpret_cast<unsigned*(*)(std::uintptr_t,unsigned,unsigned,unsigned)>(gameImage+0x8112c0)(ai+0xc8e8,0x73,0x10,p.handle);
        if(s){s[0]=5;std::memcpy(s+1,q.transform.data(),12);++shadeHostStimuli;}}
    shadeHostResult="spawned";return true;
}
void serviceShadeHost(ULONGLONG tick){
    if(GetCurrentThreadId()!=engineThread.load()||!nativeMutationAllowed)return;
    for(auto& p:shadeHostPuppets)if(p.actor&&(p.game!=observedGame||p.epoch!=outgoing.worldEpoch)){p={};shadeHostResult="world_retired";shadeHostPhantomSet=false;shadeHostPhantomPlayer=0;}
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
        if(p.drift>.05f){const bool continuous=p.drift<3.f;
            struct Scope{std::uintptr_t previous;explicit Scope(std::uintptr_t owner):previous(continuousAvatarTransformOwner){continuousAvatarTransformOwner=owner;}~Scope(){continuousAvatarTransformOwner=previous;}} scope(continuous?p.actor:0);
            reinterpret_cast<void(*)(void*,const float*,const float*,bool,int)>(gameImage+0x87d200)(reinterpret_cast<void*>(p.actor),q.transform.data(),q.transform.data()+3,true,0);
            ++p.pins;++shadeHostPins;if(continuous)++shadeHostContinuous;else ++shadeHostTeleports;}
    }
    bool any=false;for(const auto& p:shadeHostPuppets)any=any||p.actor;
    // The phantom stays 5 s after the last puppet (a late native clone event must not meet a null slot).
    if(any){shadeHostPhantomList();shadeHostLastPuppetAt=tick;}else if(shadeHostPhantomSet){shadeHostPhantomList();if(tick-shadeHostLastPuppetAt>5000)shadeHostPhantomOff();}
}
void writeShadeHost(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.shade-host.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;
    fprintf(f,"{\"result\":\"%s\",\"enabled\":%s,\"spawned\":%llu,\"removed\":%llu,\"lost\":%llu,\"pins\":%llu,\"refused\":%llu,\"stimuli\":%llu,\"slots\":[",
        shadeHostResult,shadeHostFollowEnabled?"true":"false",shadeHostSpawned,shadeHostRemoved,shadeHostLost,shadeHostPins,shadeHostRefused,shadeHostStimuli);
    for(unsigned k=0;k<kDoppelgangerWireSlots;++k){const auto& p=shadeHostPuppets[k];const auto& q=doppelgangerWorldStreams[k].presented;
        fprintf(f,"%s{\"kind\":%u,\"partnerKind\":%u,\"puppet\":%s,\"handle\":%u,\"source\":%u,\"event\":%llu,\"lostEvent\":%llu,\"pins\":%u,\"drift\":%.3f,\"at\":[%.3f,%.3f,%.3f],\"partnerPresent\":%u,\"partnerEvent\":%llu,\"partnerAt\":[%.3f,%.3f,%.3f]}",
            k?",":"",p.kind,q.kind,p.actor?"true":"false",p.handle,p.source,p.event,p.lostEvent,p.pins,p.drift,p.at[0],p.at[1],p.at[2],q.present,q.event,q.transform[0],q.transform[1],q.transform[2]);}
    fprintf(f,"],\"continuous\":%llu,\"teleports\":%llu",shadeHostContinuous,shadeHostTeleports);
    fprintf(f,",\"phantomSet\":%s,\"phantomOns\":%llu,\"phantomOffs\":%llu,\"slotWasSet\":%s}\n",shadeHostPhantomSet?"true":"false",shadeHostPhantomOns,shadeHostPhantomOffs,shadeHostSlotWasSet?"true":"false");
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
