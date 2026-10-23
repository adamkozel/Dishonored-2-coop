#pragma once
// 1214id (#160): a Corvo partner's Devouring Swarm acts on the host's NPCs. Her world's NPCs are pose-driven replicas with
// paused AI, so her own rats find no prey (partner-corvo-power-v1792a: 25 rats, no bite in either world). While her swarm
// is present (her swarm world stream), the host runs a native swarm at her swarm's origin, as the power's own spawn does
// (FUN_14043da90; chapter 108, confirmed static):
// - spawn 0x8ae9b0(game, devouring_swarm.def, origin, the identity rotation 0x14228a780, the host player, 0...), then at
//   once the host player's team index (+0x2e0), which the swarm's perception registers with, and the AI stimulus 0x73
//   kind 0xb at the origin (the cast's own sound stimulus);
// - it ends with her swarm: the swarm's own flee request (0x1406a6650 on its arkCpntRatSwarm, D2_Entity_GetComponent
//   0x140877720 with the type 0x1439e89a0), then the native removal 8 s later if it is still there.
// The rats belong to the host player (kills and chaos count for the host; the host's own swarm power is untouched). While
// the puppet runs, the host's cosmetic copy of her rats is hidden (the real rats are there).
constexpr const char* kSwarmHostDefinition="models/characters/animals/rat/devouring_swarm.def";
// 1214ja (#182): her Bigger Swarm (world packet present 2) is spawned as the big swarm on the host too.
constexpr const char* kSwarmHostBigDefinition="models/characters/animals/rat/devouring_swarm_big.def";
struct SwarmHostPuppet {std::uintptr_t actor=0,game=0;std::uint32_t handle=0;std::uint64_t epoch=0,event=0,lastEvent=0;ULONGLONG createdAt=0,endAt=0;bool ending=false;float at[3]{};};
SwarmHostPuppet swarmHostPuppet{};std::uint64_t swarmHostSpawned=0,swarmHostEnded=0,swarmHostRemoved=0,swarmHostRefused=0,swarmHostStimuli=0;
const char* swarmHostResult="idle";bool swarmHostEnabled=true;int swarmHostTeam=-1;
bool swarmHostAlive(const SwarmHostPuppet& p){return p.actor&&p.handle&&p.game==observedGame&&p.epoch==outgoing.worldEpoch&&mesmerizeHostResolve(p.handle)==p.actor;}
bool swarmHostPuppetActive(){return swarmHostAlive(swarmHostPuppet);}
void swarmHostRemove(SwarmHostPuppet& p,const char* why){
    if(swarmHostAlive(p)){alignas(16) unsigned char message[64]{};
        reinterpret_cast<void*(*)(std::uintptr_t,void*,const void*)>(gameImage+0x8847f0)(p.actor,message,gameImage+0x39a6f30);
        reinterpret_cast<void(*)(void*)>(gameImage+0x882fc0)(message);++swarmHostRemoved;}
    const auto last=p.lastEvent;p={};p.lastEvent=last;swarmHostResult=why;}
void swarmHostEnd(SwarmHostPuppet& p,ULONGLONG tick){
    if(!swarmHostAlive(p)){swarmHostRemove(p,"ended_gone");return;}
    const auto rat=reinterpret_cast<std::uintptr_t(*)(std::uintptr_t,const void*,int)>(gameImage+0x877720)(p.actor,gameImage+0x39e89a0,0);
    if(rat)reinterpret_cast<void(*)(std::uintptr_t)>(gameImage+0x6a6650)(rat);
    p.ending=true;p.endAt=tick+8000;++swarmHostEnded;swarmHostResult=rat?"fleeing":"ending_no_component";}
bool swarmHostSpawn(SwarmHostPuppet& p,const CoopSwarmWorldPacket& q,ULONGLONG tick){
    const auto definition=lookupEntityDefinition(q.present==2?kSwarmHostBigDefinition:kSwarmHostDefinition);if(!definition){swarmHostResult="definition_missing";++swarmHostRefused;return false;}
    if(!observedPlayer){swarmHostResult="no_host_player";++swarmHostRefused;return false;}
    void* actor=reinterpret_cast<void*(*)(std::uintptr_t,std::uintptr_t,const float*,const void*,std::uintptr_t,char,std::uintptr_t,std::uintptr_t,std::uintptr_t)>(gameImage+0x8ae9b0)(
        observedGame,definition,q.sourceOrigin.data(),gameImage+0x228a780,observedPlayer,0,0,0,0);
    if(!actor){swarmHostResult="native_creation_refused";++swarmHostRefused;return false;}
    const auto e=reinterpret_cast<std::uintptr_t>(actor);std::int32_t team=0;
    if(memoryRead(observedPlayer+0x2e0,&team,4)){*reinterpret_cast<std::int32_t*>(e+0x2e0)=team;swarmHostTeam=team;}
    const auto last=p.lastEvent;p={};p.lastEvent=last;p.actor=e;p.game=observedGame;p.epoch=outgoing.worldEpoch;p.event=q.event;p.lastEvent=q.event;p.createdAt=tick;
    memoryRead(e+0xa8,&p.handle,4);std::memcpy(p.at,q.sourceOrigin.data(),12);++swarmHostSpawned;
    const auto ai=reinterpret_cast<std::uintptr_t(*)()>(gameImage+0x810240)();std::uint32_t owner=0;memoryRead(observedPlayer+0xa8,&owner,4);
    if(ai&&owner){auto* s=reinterpret_cast<unsigned*(*)(std::uintptr_t,unsigned,unsigned,unsigned)>(gameImage+0x8112c0)(ai+0xc8e8,0x73,0x10,owner);
        if(s){s[0]=0xb;std::memcpy(s+1,q.sourceOrigin.data(),12);++swarmHostStimuli;}}
    swarmHostResult="spawned";return true;}
void serviceSwarmHost(ULONGLONG tick){
    auto& p=swarmHostPuppet;
    if(p.actor&&(p.game!=observedGame||p.epoch!=outgoing.worldEpoch)){const auto last=p.lastEvent;p={};p.lastEvent=last;swarmHostResult="world_changed";}
    if(!swarmHostEnabled||localRole!=0||!remoteIsCorvo()||!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load()){if(p.actor&&!p.ending)swarmHostEnd(p,tick);if(p.ending&&tick>=p.endAt)swarmHostRemove(p,"disabled");return;}
    const auto& s=swarmWorldStream;const bool current=swarmWorldCurrent(tick)&&s.lease>tick&&s.presented.present&&s.presented.event;
    if(current&&!p.actor&&s.presented.event!=p.lastEvent&&coopGameplayReady(outgoing))swarmHostSpawn(p,s.presented,tick);
    if(p.actor&&!p.ending&&(!current||s.presented.event!=p.event))swarmHostEnd(p,tick);
    if(p.ending&&tick>=p.endAt)swarmHostRemove(p,"removed_after_flee");}
void writeSwarmHost(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.swarm-host.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;const auto& p=swarmHostPuppet;
    fprintf(f,"{\"enabled\":%s,\"alive\":%s,\"handle\":%u,\"event\":%llu,\"ending\":%s,\"team\":%d,\"at\":[%.3f,%.3f,%.3f],\"spawned\":%llu,\"ended\":%llu,\"removed\":%llu,\"refused\":%llu,\"stimuli\":%llu,\"result\":\"%s\"}\n",
        swarmHostEnabled?"true":"false",swarmHostAlive(p)?"true":"false",p.handle,static_cast<unsigned long long>(p.event),p.ending?"true":"false",swarmHostTeam,p.at[0],p.at[1],p.at[2],
        static_cast<unsigned long long>(swarmHostSpawned),static_cast<unsigned long long>(swarmHostEnded),static_cast<unsigned long long>(swarmHostRemoved),
        static_cast<unsigned long long>(swarmHostRefused),static_cast<unsigned long long>(swarmHostStimuli),swarmHostResult);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);}
