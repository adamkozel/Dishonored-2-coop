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
constexpr const char* kMesmerizeObjectDefinition="models/powers/mesmerize/mesmerize_object.def";
constexpr std::uintptr_t kMesmerizeComponentType=0x39dcdc0; // DAT_1439dcdc0, arkCpntMesmerize (FUN_1406212e0)
struct MesmerizeHostObject {std::uintptr_t actor=0,component=0,game=0,definition=0;unsigned handle=0;std::uint64_t epoch=0;ULONGLONG createdAt=0,startedAt=0,stoppedAt=0,componentAt=0;};
MesmerizeHostObject mesmerizeHost{};
unsigned mesmerizeHostRequest=0;float mesmerizeHostSpawnAt[3]{};
const char* mesmerizeHostResult="idle";std::uintptr_t mesmerizeHostProbeDefinition=0;
std::uint64_t mesmerizeHostProbes=0,mesmerizeHostSpawned=0,mesmerizeHostStarts=0,mesmerizeHostStops=0,mesmerizeHostRefused=0;
bool mesmerizeHostApis(){
    static const unsigned char resolve[]={0x89,0x4c,0x24,0x08,0x48,0x83,0xec,0x28},state[]={0x48,0x89,0x5c,0x24,0x08,0x57,0x48,0x83,0xec,0x20},
        component[]={0x48,0x89,0x5c,0x24,0x10,0x44,0x89,0x44,0x24,0x18};
    return gameImage&&!std::memcmp(gameImage+0x6dc560,resolve,sizeof(resolve))&&!std::memcmp(gameImage+0x72bed0,state,sizeof(state))&&
        !std::memcmp(gameImage+0x877720,component,sizeof(component));
}
std::uintptr_t mesmerizeHostResolve(unsigned handle){return reinterpret_cast<std::uintptr_t(*)(unsigned)>(gameImage+0x6dc560)(handle);}
// FUN_14072bf30 with the world anchor: the object position and its forward in the anchor's frame.
bool mesmerizeHostConfigure(std::uintptr_t component,std::uintptr_t actor){
    const auto anchor=mesmerizeHostResolve(0x3fff0010u);if(!anchor)return false;
    float ap[3]{},ar[9]{},op[3]{},orow[3]{};
    if(!memoryRead(anchor+0x300,ap,12)||!memoryRead(anchor+0x30c,ar,36)||!memoryRead(actor+0x300,op,12)||!memoryRead(actor+0x30c,orow,12))return false;
    const float rel[3]{op[0]-ap[0],op[1]-ap[1],op[2]-ap[2]};
    const float offset[3]{rel[0]*ar[0]+rel[1]*ar[1]+rel[2]*ar[2],rel[0]*ar[3]+rel[1]*ar[4]+rel[2]*ar[5],rel[0]*ar[6]+rel[1]*ar[7]+rel[2]*ar[8]};
    const float forward[3]{orow[0]*ar[0]+orow[1]*ar[1]+orow[2]*ar[2],orow[0]*ar[3]+orow[1]*ar[4]+orow[2]*ar[5],orow[0]*ar[6]+orow[1]*ar[7]+orow[2]*ar[8]};
    *reinterpret_cast<unsigned char*>(component+0x2b2)=1;*reinterpret_cast<unsigned*>(component+0x120)=0x3fff0010u;
    std::memcpy(reinterpret_cast<void*>(component+0x108),offset,12);std::memcpy(reinterpret_cast<void*>(component+0x114),forward,12);
    return true;
}
void serviceMesmerizeHost(ULONGLONG tick){
    auto& h=mesmerizeHost;
    if(h.actor&&(h.game!=observedGame||h.epoch!=outgoing.worldEpoch)){h={};mesmerizeHostOwnedComponent.store(0);mesmerizeHostResult="world_retired";}
    // 1214gi: resolve the object's arkCpntMesmerize once it is attached, and verify it belongs to the object.
    if(h.actor&&!h.component&&GetCurrentThreadId()==engineThread.load()&&mesmerizeHostApis()&&tick-h.createdAt<10000){
        const auto c=nativeComponent(h.actor,kMesmerizeComponentType);std::uintptr_t owner=0;
        if(c&&memoryRead(c+0x10,&owner,8)&&owner==h.actor){h.component=c;mesmerizeHostOwnedComponent.store(c);h.componentAt=tick;}}
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
        const auto definition=lookupEntityDefinition(kMesmerizeObjectDefinition);if(!definition){refuse("definition_missing");return;}
        unsigned char parameters[12]{};reinterpret_cast<void*(*)(void*)>(gameImage+0x146520)(parameters);
        void* actor=reinterpret_cast<void*(*)(void*,void*,void*)>(gameImage+0x8a78f0)(reinterpret_cast<void*>(observedGame),reinterpret_cast<void*>(definition),parameters);
        if(!actor){refuse("native_creation_refused");return;}
        h.game=observedGame;h.epoch=outgoing.worldEpoch;h.definition=definition;h.actor=reinterpret_cast<std::uintptr_t>(actor);h.createdAt=tick;++mesmerizeHostSpawned;
        const float axis[9]{1,0,0,0,1,0,0,0,1};
        reinterpret_cast<void(*)(void*,const float*)>(gameImage+0x87c320)(actor,mesmerizeHostSpawnAt);
        reinterpret_cast<void(*)(void*,const float*)>(gameImage+0x87c2c0)(actor,axis);
        reinterpret_cast<void(*)(void*,void*,int)>(gameImage+0x8aecc0)(reinterpret_cast<void*>(observedGame),actor,-1);
        memoryRead(h.actor+0xa8,&h.handle,4);
        reinterpret_cast<void(*)(void*)>(gameImage+0x8784b0)(actor);
        // 1214gi: the components attach after the spawn frame (take mesmerize-host-v1716a found none at once); the
        // service resolves the Mesmerize component on the following frames.
        mesmerizeHostResult="spawned";return;
    }
    if(!h.actor||!h.component){refuse("spawn_required");return;}
    int state=-1;if(!memoryRead(h.component+0x2c,&state,4)){refuse("component_unreadable");return;}
    if(request==3){
        if(state==1||state==3){refuse("already_started");return;}
        if(!mesmerizeHostConfigure(h.component,h.actor)){refuse("configure_failed");return;}
        reinterpret_cast<void(*)(std::uintptr_t,int)>(gameImage+0x72bed0)(h.component,1);h.startedAt=tick;++mesmerizeHostStarts;mesmerizeHostResult="started";return;
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
    fprintf(f,"],\"componentAt\":%llu,\"result\":\"%s\",\"probes\":%llu,\"definitionFound\":%s,\"spawned\":%llu,\"starts\":%llu,\"stops\":%llu,\"refused\":%llu,\"handle\":%u,\"component\":%s,"
              "\"state\":%d,\"previousSlot\":%d,\"committed\":%u,\"anchor\":%u,\"offset\":[%.2f,%.2f,%.2f],\"flag2b1\":%u,\"targets\":%u,\"targetHandles\":[%u,%u,%u,%u,%u],"
              "\"createdAt\":%llu,\"startedAt\":%llu,\"stoppedAt\":%llu,\"partnerBlocked\":%llu}\n",
        h.componentAt,mesmerizeHostResult,mesmerizeHostProbes,mesmerizeHostProbeDefinition?"true":"false",mesmerizeHostSpawned,mesmerizeHostStarts,mesmerizeHostStops,mesmerizeHostRefused,h.handle,h.component?"true":"false",
        state,previous,committed,anchor,offset[0],offset[1],offset[2],blocking,count,handles[0],handles[1],handles[2],handles[3],handles[4],
        h.createdAt,h.startedAt,h.stoppedAt,mesmerizeHostPartnerBlocked.load());
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
