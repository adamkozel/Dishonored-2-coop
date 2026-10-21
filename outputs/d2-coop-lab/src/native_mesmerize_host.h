#pragma once
#include <cstdio>
// 1214gh (#131) lab experiment: a native Mesmerize object in the host's world. Emily's Mesmerize acts through its world
// object (models/powers/mesmerize/mesmerize_object.def: arkCpntMesmerize, its AI attention source and FX). Her copy acts
// on her world's replicas only, whose AI the host owns, so the host's NPCs were never mesmerized (#131, carry-body-v1658a).
// This spawns the original object in the host's world and starts it the way the power does (confirmed static, Ghidra,
// FUN_1406212e0): place the object, configure its component as FUN_14072bf30 does with the world anchor 0x3fff0010 (the
// caster's aim data does not exist on the host), and request FSM state 1 (FUN_14072bed0). Stop requests state 2, as the
// power's cancel FUN_140621660 does when the state is 3. The component's own update then picks the host's NPCs.
// Lab host on the guard map only; one cached object per world. coop_mesmerize_native probe|spawn x y z|start|stop.
// 1214gn: the host also runs it for the partner's own cast (serviceMesmerizeHostFollow).
constexpr const char* kMesmerizeObjectDefinition="models/powers/mesmerize/mesmerize_object.def";
constexpr std::uintptr_t kMesmerizeComponentType=0x39dcdc0; // DAT_1439dcdc0, arkCpntMesmerize (FUN_1406212e0)
struct MesmerizeHostObject {std::uintptr_t actor=0,component=0,game=0,definition=0;unsigned handle=0;std::uint64_t epoch=0;ULONGLONG createdAt=0,startedAt=0,stoppedAt=0,componentAt=0;};
MesmerizeHostObject mesmerizeHost{};
unsigned mesmerizeHostRequest=0;float mesmerizeHostSpawnAt[3]{};
// 1214gj: the anchor entity (a static level entity; the world handle 0x3fff0010 did not resolve, take mesmerize-host-v1718a).
unsigned mesmerizeHostAnchor=0;bool mesmerizeHostAnchorResolved=false,mesmerizeHostSelfResolved=false;
const char* mesmerizeHostResult="idle";std::uintptr_t mesmerizeHostProbeDefinition=0;
std::uint64_t mesmerizeHostProbes=0,mesmerizeHostSpawned=0,mesmerizeHostStarts=0,mesmerizeHostStops=0,mesmerizeHostRefused=0;
// 1214gm: the native Mesmerize code finds the spirit through the local player: FUN_14040ccf0 (the mesmerize spots
// around the spirit, reached from the NPC AI) reads FUN_1406dc560(*(*(player+0x390)+0xd4)), the player's Mesmerize
// power and its world object handle (confirmed static). Corvo has no Mesmerize power, so +0x390 is null and forcing
// state 3 crashed there (take mesmerize-host-v1721a, c0000005 at 40cd3c reading 0xd4). While the host object runs,
// the player gets a phantom power block (zeroed, +0xd4 = the object's handle) in that slot when it is null; the slot
// is restored to null when the object is back in state 0, on a world change, or before another spawn.
std::uintptr_t mesmerizeHostPhantom=0,mesmerizeHostPhantomPlayer=0;bool mesmerizeHostPhantomSet=false,mesmerizeHostSlotWasSet=false;
std::uint64_t mesmerizeHostPhantomOns=0,mesmerizeHostPhantomOffs=0;
bool mesmerizeHostApis(){
    static const unsigned char resolve[]={0x89,0x4c,0x24,0x08,0x48,0x83,0xec,0x28},state[]={0x48,0x89,0x5c,0x24,0x08,0x57,0x48,0x83,0xec,0x20},
        component[]={0x48,0x89,0x5c,0x24,0x10,0x44,0x89,0x44,0x24,0x18};
    return gameImage&&!std::memcmp(gameImage+0x6dc560,resolve,sizeof(resolve))&&!std::memcmp(gameImage+0x72bed0,state,sizeof(state))&&
        !std::memcmp(gameImage+0x877720,component,sizeof(component));
}
std::uintptr_t mesmerizeHostResolve(unsigned handle){return reinterpret_cast<std::uintptr_t(*)(unsigned)>(gameImage+0x6dc560)(handle);}
std::uintptr_t mesmerizeHostLocalPlayer(){
    static const unsigned char getter[]={0x48,0x81,0xc1,0xb0,0x30,0x31,0x00,0x33,0xd2,0xe9};
    if(std::memcmp(gameImage+0x8a8ba0,getter,sizeof(getter))||!observedGame)return 0;
    return reinterpret_cast<std::uintptr_t(*)(std::uintptr_t)>(gameImage+0x8a8ba0)(observedGame);}
bool mesmerizeHostPhantomOn(unsigned handle){
    const auto player=mesmerizeHostLocalPlayer();std::uintptr_t slot=0;if(!player||!memoryRead(player+0x390,&slot,8))return false;
    if(slot&&slot!=mesmerizeHostPhantom){mesmerizeHostSlotWasSet=true;return false;} // a real power: never replaced
    if(!mesmerizeHostPhantom){mesmerizeHostPhantom=reinterpret_cast<std::uintptr_t>(VirtualAlloc(nullptr,0x1000,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));if(!mesmerizeHostPhantom)return false;}
    *reinterpret_cast<unsigned*>(mesmerizeHostPhantom+0xd4)=handle;*reinterpret_cast<std::uintptr_t*>(player+0x390)=mesmerizeHostPhantom;
    mesmerizeHostPhantomPlayer=player;mesmerizeHostPhantomSet=true;++mesmerizeHostPhantomOns;return true;}
void mesmerizeHostPhantomOff(){
    if(!mesmerizeHostPhantomSet)return;std::uintptr_t slot=0;
    if(mesmerizeHostPhantomPlayer&&mesmerizeHostPhantomPlayer==mesmerizeHostLocalPlayer()&&memoryRead(mesmerizeHostPhantomPlayer+0x390,&slot,8)&&slot==mesmerizeHostPhantom)
        *reinterpret_cast<std::uintptr_t*>(mesmerizeHostPhantomPlayer+0x390)=0;
    mesmerizeHostPhantomSet=false;mesmerizeHostPhantomPlayer=0;++mesmerizeHostPhantomOffs;}
// FUN_14072bf30 with a static anchor: the object position and its forward in the anchor's frame.
bool mesmerizeHostConfigureAt(std::uintptr_t component,unsigned anchorHandle,const float* op,const float* orow){
    const auto anchor=mesmerizeHostResolve(anchorHandle);mesmerizeHostAnchorResolved=anchor!=0;if(!anchor)return false;
    float ap[3]{},ar[9]{};
    if(!memoryRead(anchor+0x300,ap,12)||!memoryRead(anchor+0x30c,ar,36))return false;
    const float rel[3]{op[0]-ap[0],op[1]-ap[1],op[2]-ap[2]};
    const float offset[3]{rel[0]*ar[0]+rel[1]*ar[1]+rel[2]*ar[2],rel[0]*ar[3]+rel[1]*ar[4]+rel[2]*ar[5],rel[0]*ar[6]+rel[1]*ar[7]+rel[2]*ar[8]};
    const float forward[3]{orow[0]*ar[0]+orow[1]*ar[1]+orow[2]*ar[2],orow[0]*ar[3]+orow[1]*ar[4]+orow[2]*ar[5],orow[0]*ar[6]+orow[1]*ar[7]+orow[2]*ar[8]};
    *reinterpret_cast<unsigned char*>(component+0x2b2)=1;*reinterpret_cast<unsigned*>(component+0x120)=anchorHandle;
    std::memcpy(reinterpret_cast<void*>(component+0x108),offset,12);std::memcpy(reinterpret_cast<void*>(component+0x114),forward,12);
    return true;
}
bool mesmerizeHostConfigure(std::uintptr_t component,std::uintptr_t actor,unsigned anchorHandle){
    float op[3]{},orow[3]{};if(!memoryRead(actor+0x300,op,12)||!memoryRead(actor+0x30c,orow,12))return false;
    return mesmerizeHostConfigureAt(component,anchorHandle,op,orow);}
// The cached object of this world, created once at a transform (the lab command and the partner's cast share it).
bool mesmerizeHostSpawn(ULONGLONG tick,const float* at,const float* axis){
    auto& h=mesmerizeHost;const auto definition=lookupEntityDefinition(kMesmerizeObjectDefinition);if(!definition){mesmerizeHostResult="definition_missing";return false;}
    unsigned char parameters[12]{};reinterpret_cast<void*(*)(void*)>(gameImage+0x146520)(parameters);
    void* actor=reinterpret_cast<void*(*)(void*,void*,void*)>(gameImage+0x8a78f0)(reinterpret_cast<void*>(observedGame),reinterpret_cast<void*>(definition),parameters);
    if(!actor){mesmerizeHostResult="native_creation_refused";return false;}
    h.game=observedGame;h.epoch=outgoing.worldEpoch;h.definition=definition;h.actor=reinterpret_cast<std::uintptr_t>(actor);h.createdAt=tick;++mesmerizeHostSpawned;
    reinterpret_cast<void(*)(void*,const float*)>(gameImage+0x87c320)(actor,at);
    reinterpret_cast<void(*)(void*,const float*)>(gameImage+0x87c2c0)(actor,axis);
    reinterpret_cast<void(*)(void*,void*,int)>(gameImage+0x8aecc0)(reinterpret_cast<void*>(observedGame),actor,-1);
    memoryRead(h.actor+0xa8,&h.handle,4);
    reinterpret_cast<void(*)(void*)>(gameImage+0x8784b0)(actor);
    // 1214gi: the components attach after the spawn frame (take mesmerize-host-v1716a found none at once); the
    // service resolves the Mesmerize component on the following frames.
    mesmerizeHostResult="spawned";return true;
}
// 1214gn (#131): the partner's cast drives the host object. Her Mesmerize acts on her world's replicas only, whose AI
// the host owns (carry-body-v1658a). The host object proven in mesmerize-host-v1722b (anchored to a static spawner, the
// phantom power slot, state 3 requested directly) now follows her world stream: when her spirit appears (her state 1
// or 3, a new event) the host places its object at her spirit's transform (spawned once per world, then moved),
// anchors it to the spawner of the NPC nearest to it, sets the phantom slot and requests state 1; once hers is active
// (state 3, at least 200 ms later) it requests state 3 (the host's appear scene never ends, mesmerize-host-v1722a).
// Her retire (state 2), a newer cast or 1.5 s without her stream requests state 2. Host only, in an armed product
// session or the lab movement fixture on the guard map.
struct MesmerizeHostFollow {std::uint64_t event=0,token=0,peerToken=0;ULONGLONG seen=0,waitFrom=0,beganAt=0;bool begun=false,forced=false,ended=false;} mesmerizeHostFollow;
std::uint64_t mesmerizeHostFollowEvents=0,mesmerizeHostFollowBegins=0,mesmerizeHostFollowForced=0,mesmerizeHostFollowStops=0,mesmerizeHostFollowRefused=0;
const char* mesmerizeHostFollowResult="idle";unsigned mesmerizeHostFollowAnchor=0;float mesmerizeHostFollowAnchorDistance=0;
unsigned mesmerizeHostNearestSpawner(const float* at,float& distance){
    std::vector<NativeWorldNpc> catalog;unsigned unbound=0;distance=0;
    if(!readWorldNpcCatalog(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,catalog,unbound))return 0;
    unsigned best=0;float bestSquared=1e12f;
    for(const auto& n:catalog){if(!n.spawner||!n.spawnerHandle||n.definition.find("/d2coop/")!=std::string::npos)continue;
        const float dx=n.position[0]-at[0],dy=n.position[1]-at[1],dz=n.position[2]-at[2],d=dx*dx+dy*dy+dz*dz;if(d<bestSquared){bestSquared=d;best=n.spawnerHandle;}}
    if(best)distance=std::sqrt(bestSquared);return best;}
void serviceMesmerizeHostFollow(ULONGLONG tick){
    auto& f=mesmerizeHostFollow;auto& h=mesmerizeHost;const auto& s=mesmerizeWorldStream;
    const bool streaming=localRole==0&&mesmerizeWorldStreamCurrent(tick)&&s.lease>tick&&movementBankActive()&&coopGameplayReady(outgoing)&&coopGameplayReady(remote);
    const bool scope=streaming&&(productSessionArmed()||(observerIsolation.ready&&observerIsolation.movementFixture&&!std::strcmp(mapName,kGuardMap)));
    const bool hers=scope&&(s.presented.state==1||s.presented.state==3);
    int state=-1;if(h.component)memoryRead(h.component+0x2c,&state,4);
    const auto request=[&](int next){reinterpret_cast<void(*)(std::uintptr_t,int)>(gameImage+0x72bed0)(h.component,next);};
    const auto stop=[&](){f.ended=true;if(!f.begun)return;h.stoppedAt=tick;
        if(state==1||state==3){request(2);++mesmerizeHostStops;++mesmerizeHostFollowStops;mesmerizeHostFollowResult="stopped";}else mesmerizeHostFollowResult="ended_native";};
    if(hers&&(s.presented.event!=f.event||s.token!=f.token||s.peerToken!=f.peerToken)){
        if(f.event&&!f.ended)stop();
        f={};f.event=s.presented.event;f.token=s.token;f.peerToken=s.peerToken;f.waitFrom=tick;++mesmerizeHostFollowEvents;mesmerizeHostFollowResult="cast_seen";
    }
    if(!f.event||f.ended)return;
    if(hers)f.seen=tick;
    if((s.presented.event==f.event&&s.presented.state==2)||tick-f.seen>1500){stop();return;}
    if(!f.begun){
        const auto refuse=[&](const char* why){mesmerizeHostFollowResult=why;++mesmerizeHostFollowRefused;f.ended=true;};
        if(!mesmerizeHostApis()||!powerFixtureApiMatches()){refuse("api_mismatch");return;}
        const float* at=s.presented.transform.data();const float* axis=at+3;
        if(!h.actor){if(!mesmerizeHostSpawn(tick,at,axis))refuse("spawn_refused");else mesmerizeHostFollowResult="spawned";return;}
        if(!h.component){if(tick-h.createdAt>=10000)refuse("component_missing");return;}
        if(state!=0){if(tick-f.waitFrom>5000)refuse("previous_not_idle");return;}
        reinterpret_cast<void(*)(void*,const float*)>(gameImage+0x87c320)(reinterpret_cast<void*>(h.actor),at);
        reinterpret_cast<void(*)(void*,const float*)>(gameImage+0x87c2c0)(reinterpret_cast<void*>(h.actor),axis);
        float distance=0;const unsigned anchor=mesmerizeHostNearestSpawner(at,distance);mesmerizeHostFollowAnchor=anchor;mesmerizeHostFollowAnchorDistance=distance;
        if(!anchor){refuse("no_anchor");return;}
        if(!mesmerizeHostConfigureAt(h.component,anchor,at,axis)){refuse("configure_failed");return;}
        if(!mesmerizeHostPhantomOn(h.handle)){refuse(mesmerizeHostSlotWasSet?"player_has_mesmerize":"phantom_refused");return;}
        request(1);h.startedAt=tick;h.stoppedAt=0;++mesmerizeHostStarts;++mesmerizeHostFollowBegins;f.begun=true;f.beganAt=tick;mesmerizeHostFollowResult="started";return;
    }
    if(!f.forced&&state==1&&s.presented.event==f.event&&s.presented.state==3&&tick-f.beganAt>=200){request(3);f.forced=true;++mesmerizeHostFollowForced;mesmerizeHostFollowResult="active";}
}
void serviceMesmerizeHost(ULONGLONG tick){
    auto& h=mesmerizeHost;
    if(h.actor&&(h.game!=observedGame||h.epoch!=outgoing.worldEpoch)){h={};mesmerizeHostOwnedComponent.store(0);mesmerizeHostResult="world_retired";mesmerizeHostPhantomSet=false;mesmerizeHostPhantomPlayer=0;mesmerizeHostFollow={};}
    // 1214gm: the phantom slot ends once the object is back in state 0 after a stop.
    if(mesmerizeHostPhantomSet&&h.component&&h.stoppedAt&&tick-h.stoppedAt>3000&&GetCurrentThreadId()==engineThread.load()){int s=-1;if(memoryRead(h.component+0x2c,&s,4)&&s==0)mesmerizeHostPhantomOff();}
    // 1214gi: resolve the object's arkCpntMesmerize once it is attached, and verify it belongs to the object.
    if(h.actor&&!h.component&&GetCurrentThreadId()==engineThread.load()&&mesmerizeHostApis()&&tick-h.createdAt<10000){
        const auto c=nativeComponent(h.actor,kMesmerizeComponentType);std::uintptr_t owner=0;
        if(c&&memoryRead(c+0x10,&owner,8)&&owner==h.actor){h.component=c;mesmerizeHostOwnedComponent.store(c);h.componentAt=tick;}}
    if(GetCurrentThreadId()==engineThread.load()&&nativeMutationAllowed)serviceMesmerizeHostFollow(tick);
    if(!mesmerizeHostRequest)return;
    const unsigned request=mesmerizeHostRequest;mesmerizeHostRequest=0;
    const auto refuse=[](const char* why){mesmerizeHostResult=why;++mesmerizeHostRefused;};
    if(GetCurrentThreadId()!=engineThread.load()||!nativeMutationAllowed||!coopGameplayReady(outgoing)){refuse("not_ready");return;}
    // Lab host on the guard map only (live AI: the movement fixture's isolation pauses the office guards).
    if(productMode||localRole||std::strcmp(mapName,kGuardMap)){refuse("lab_host_guard_map_only");return;}
    if(!mesmerizeHostApis()||!powerFixtureApiMatches()){refuse("api_mismatch");return;}
    if(request==1){++mesmerizeHostProbes;mesmerizeHostProbeDefinition=lookupEntityDefinition(kMesmerizeObjectDefinition);mesmerizeHostResult=mesmerizeHostProbeDefinition?"definition_found":"definition_missing";return;}
    if(request==2){
        if(h.actor){refuse("already_spawned");return;}
        static const float axis[9]{1,0,0,0,1,0,0,0,1};
        if(!mesmerizeHostSpawn(tick,mesmerizeHostSpawnAt,axis))++mesmerizeHostRefused;
        return;
    }
    if(!h.actor||!h.component){refuse("spawn_required");return;}
    int state=-1;if(!memoryRead(h.component+0x2c,&state,4)){refuse("component_unreadable");return;}
    if(request==3){
        if(state==1||state==3){refuse("already_started");return;}
        mesmerizeHostSelfResolved=mesmerizeHostResolve(h.handle)==h.actor;
        if(!mesmerizeHostConfigure(h.component,h.actor,mesmerizeHostAnchor?mesmerizeHostAnchor:0x3fff0010u)){refuse("configure_failed");return;}
        if(!mesmerizeHostPhantomOn(h.handle)){refuse(mesmerizeHostSlotWasSet?"player_has_mesmerize":"phantom_refused");return;}
        reinterpret_cast<void(*)(std::uintptr_t,int)>(gameImage+0x72bed0)(h.component,1);h.startedAt=tick;++mesmerizeHostStarts;mesmerizeHostResult="started";return;
    }
    // 1214gl: request the active state 3 directly (state 1 stayed for 20 s in mesmerize-host-v1719a/v1720a).
    if(request==5){
        if(state!=1){refuse("force3_needs_state_1");return;}
        reinterpret_cast<void(*)(std::uintptr_t,int)>(gameImage+0x72bed0)(h.component,3);mesmerizeHostResult="forced_3";return;
    }
    if(request==4){
        if(state!=3){refuse("stop_needs_state_3");return;}
        reinterpret_cast<void(*)(std::uintptr_t,int)>(gameImage+0x72bed0)(h.component,2);h.stoppedAt=tick;++mesmerizeHostStops;mesmerizeHostResult="stopped";return;
    }
}
void writeMesmerizeHost(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.mesmerize-host.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;const auto& h=mesmerizeHost;
    int state=-1,previous=-1;unsigned anchor=0,count=0,handles[5]{};unsigned char committed=0,blocking=0;float offset[3]{};
    if(h.component){memoryRead(h.component+0x2c,&state,4);memoryRead(h.component+0x28,&previous,4);memoryRead(h.component+0x120,&anchor,4);
        memoryRead(h.component+0x2b2,&committed,1);memoryRead(h.component+0x2b1,&blocking,1);memoryRead(h.component+0x108,offset,12);
        std::uintptr_t list=0;if(memoryRead(h.component+0x1a8,&list,8)&&list&&memoryRead(h.component+0x1b0,&count,4)){count&=0xffffff;
            for(unsigned i=0;i<count&&i<5;++i)memoryRead(list+i*0x70,&handles[i],4);}}
    std::uintptr_t vtables[12]{};unsigned componentCount=0;
    if(h.actor){std::uintptr_t list=0;unsigned flags=0;if(memoryRead(h.actor+0x268,&list,8)&&list&&memoryRead(h.actor+0x270,&flags,4)){componentCount=flags&0xffffff;
        for(unsigned i=0;i<componentCount&&i<12;++i){std::uintptr_t c=0,vt=0;if(memoryRead(list+i*8,&c,8)&&c&&memoryRead(c,&vt,8))vtables[i]=vt-reinterpret_cast<std::uintptr_t>(gameImage);}}}
    fprintf(f,"{\"components\":%u,\"componentVtables\":[",componentCount);for(unsigned i=0;i<componentCount&&i<12;++i)fprintf(f,"%s\"0x%llx\"",i?",":"",static_cast<unsigned long long>(vtables[i]));
    fprintf(f,"],\"fsm\":[");
    if(h.component){const auto image=reinterpret_cast<std::uintptr_t>(gameImage);bool first=true;
        for(unsigned off=0x20;off<0xa8;off+=8){std::uintptr_t v=0;if(!memoryRead(h.component+off,&v,8))break;
            fprintf(f,"%s{\"off\":\"0x%x\",\"rva\":\"0x%llx\",\"raw\":\"0x%llx\"}",first?"":",",off,v>=image&&v<image+0x4000000?static_cast<unsigned long long>(v-image):0ull,static_cast<unsigned long long>(v));first=false;}}
    {unsigned scene1=0,scene3=0,candidates=0;float timer=0;std::uintptr_t clist=0;
     if(h.component){memoryRead(h.component+0x1dc,&scene1,4);memoryRead(h.component+0x1e0,&scene3,4);memoryRead(h.component+0x1e8,&timer,4);
      if(memoryRead(h.component+0x198,&clist,8)&&clist)memoryRead(h.component+0x1a0,&candidates,4);candidates&=0xffffff;}
     fprintf(f,"],\"phantomSet\":%s,\"phantomOns\":%llu,\"phantomOffs\":%llu,\"slotWasSet\":%s",mesmerizeHostPhantomSet?"true":"false",mesmerizeHostPhantomOns,mesmerizeHostPhantomOffs,mesmerizeHostSlotWasSet?"true":"false");
    fprintf(f,",\"appearScene\":%u,\"activeScene\":%u,\"activeTimer\":%.3f,\"candidates\":%u",scene1,scene3,timer,candidates);}
    {const auto& w=mesmerizeHostFollow;const auto& s=mesmerizeWorldStream;
     fprintf(f,",\"follow\":{\"result\":\"%s\",\"events\":%llu,\"begins\":%llu,\"forced\":%llu,\"stops\":%llu,\"refused\":%llu,\"event\":%llu,\"begun\":%s,\"forcedNow\":%s,\"ended\":%s,"
              "\"anchor\":%u,\"anchorDistance\":%.2f,\"partnerState\":%u,\"partnerEvent\":%llu,\"leased\":%s,\"productArmed\":%s}",
        mesmerizeHostFollowResult,mesmerizeHostFollowEvents,mesmerizeHostFollowBegins,mesmerizeHostFollowForced,mesmerizeHostFollowStops,mesmerizeHostFollowRefused,w.event,
        w.begun?"true":"false",w.forced?"true":"false",w.ended?"true":"false",mesmerizeHostFollowAnchor,mesmerizeHostFollowAnchorDistance,s.presented.state,s.presented.event,
        s.lease>GetTickCount64()?"true":"false",productSessionArmed()?"true":"false");}
    fprintf(f,",\"componentAt\":%llu,\"result\":\"%s\",\"probes\":%llu,\"definitionFound\":%s,\"spawned\":%llu,\"starts\":%llu,\"stops\":%llu,\"refused\":%llu,\"handle\":%u,\"component\":%s,"
              "\"state\":%d,\"previousSlot\":%d,\"committed\":%u,\"anchor\":%u,\"offset\":[%.2f,%.2f,%.2f],\"flag2b1\":%u,\"targets\":%u,\"targetHandles\":[%u,%u,%u,%u,%u],"
              "\"createdAt\":%llu,\"startedAt\":%llu,\"stoppedAt\":%llu,\"partnerBlocked\":%llu,\"anchorHandle\":%u,\"anchorResolved\":%s,\"selfResolved\":%s}\n",
        h.componentAt,mesmerizeHostResult,mesmerizeHostProbes,mesmerizeHostProbeDefinition?"true":"false",mesmerizeHostSpawned,mesmerizeHostStarts,mesmerizeHostStops,mesmerizeHostRefused,h.handle,h.component?"true":"false",
        state,previous,committed,anchor,offset[0],offset[1],offset[2],blocking,count,handles[0],handles[1],handles[2],handles[3],handles[4],
        h.createdAt,h.startedAt,h.stoppedAt,mesmerizeHostPartnerBlocked.load(),mesmerizeHostAnchor,mesmerizeHostAnchorResolved?"true":"false",mesmerizeHostSelfResolved?"true":"false");
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
