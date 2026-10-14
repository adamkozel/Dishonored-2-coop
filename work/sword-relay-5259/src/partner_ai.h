#pragma once
// Local host-only perception experiment. Changes one directional relationship
// through the native per-entity setter (the outer API propagates to factions).
// No network message can arm it. Native AI consequences need checkpoint reset.
struct PartnerEnemyLease {
    NativeFaction guard{},partner{};
    std::uint64_t epoch=0,peerSession=0,peerEpoch=0;
    ULONGLONG until=0,lastCheck=0;
};
PartnerEnemyLease partnerEnemy{};
std::uint64_t partnerEnemyStarted=0,partnerEnemyReleased=0,partnerEnemyRefused=0,partnerEnemyLost=0;
DWORD partnerEnemyThread=0;
const char* partnerEnemyResult="idle";
bool partnerEnemyApiMatches(){
    const unsigned char set[]={0x48,0x89,0x5c,0x24,0x18,0x55,0x56,0x41,0x55,0x41,0x56,0x41,0x57,0x48,0x83,0xec};
    const unsigned char remove[]={0x40,0x53,0x55,0x56,0x41,0x56,0x41,0x57,0x48,0x83,0xec,0x20,0x4d,0x8b,0xf0,0x48};
    return gameImage&&!std::memcmp(gameImage+0x605990,set,sizeof(set))&&!std::memcmp(gameImage+0x6060a0,remove,sizeof(remove));
}
const NativeFactionRelation* partnerRelation(const NativeFaction& s,std::uint32_t handle){
    const auto it=std::find_if(s.relations.begin(),s.relations.end(),[&](const auto& r){return r.handle==handle;});return it==s.relations.end()?nullptr:&*it;
}
bool partnerEnemyScope(NativeFaction& guard,NativeFaction& partner){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    return partnerEnemy.until&&GetCurrentThreadId()==engineThread.load()&&partnerEnemyApiMatches()&&
        outgoing.worldEpoch==partnerEnemy.epoch&&observedGame==partnerEnemy.guard.game&&
        readNativeFaction(memoryRead,base,partnerEnemy.guard.handle,guard)&&readNativeFaction(memoryRead,base,partnerEnemy.partner.handle,partner)&&
        guard.game==partnerEnemy.guard.game&&guard.actor==partnerEnemy.guard.actor&&guard.component==partnerEnemy.guard.component&&
        partner.game==guard.game&&partner.actor==partnerEnemy.partner.actor&&partner.component==partnerEnemy.partner.component;
}
void stopPartnerEnemy(const char* reason){
    if(!partnerEnemy.until)return;
    NativeFaction guard{},partner{};bool released=false;
    if(partnerEnemyScope(guard,partner)){
        const auto* relation=partnerRelation(guard,partner.handle);
        // A different native override is not ours to remove.
        if(relation&&relation->overridden&&relation->overrideValue==2){
            reinterpret_cast<void(*)(void*,void*,void*)>(gameImage+0x6060a0)(reinterpret_cast<void*>(guard.game+0x1f7bb8),reinterpret_cast<void*>(guard.actor),reinterpret_cast<void*>(partner.actor));
            NativeFaction after{};
            if(readNativeFaction(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),guard.handle,after)){
                const auto* r=partnerRelation(after,partner.handle);released=r&&!r->overridden&&r->effective==relation->base;
            }
        }
    }
    if(released)++partnerEnemyReleased;else ++partnerEnemyLost;
    partnerEnemy={};partnerEnemyResult=reason;
}
void beginPartnerEnemy(ULONGLONG tick,bool escape=false){
    const auto refuse=[&](const char* reason){++partnerEnemyRefused;partnerEnemyResult=reason;};
    if(partnerEnemy.until){refuse("duplicate");return;}
    if(localRole||GetCurrentThreadId()!=engineThread.load()||!partnerEnemyApiMatches()||!guardPairReady(tick)||!probeFollow||!avatarAiHeld||
       !probeActor||probeGame!=observedGame||guardLink.phase!=GuardPhase::Idle||aiDecisionHold.expires){refuse("unavailable");return;}
    NativeWorldNpc npc{};NativeFaction guard{},partner{};
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    bool resolved=false;
    if(!escape)resolved=guardResolve(npc);
    else {
        std::vector<NativeWorldNpc> catalog;unsigned unbound=0,matches=0;
        if(readWorldNpcCatalog(memoryRead,base,observedGame,catalog,unbound))for(const auto& n:catalog)
            if(n.spawnerName=="ld_spawner_guard_escape_1"){
                if(!n.unique||!n.reciprocal||n.definition!="models/characters/def/generic/guard_male_small_01.def"){matches=2;break;}
                npc=n;++matches;
            }
        resolved=matches==1;
    }
    if(!resolved||!readNativeFaction(memoryRead,base,npc.handle,guard)||!readNativeFaction(memoryRead,base,probeHandle,partner)||
       guard.actor!=npc.actor||guard.game!=observedGame||partner.game!=observedGame||partner.actor!=probeActor||
       guard.logic!=1||partner.logic!=1||partner.name!="Loyalists"){refuse("identity");return;}
    const auto* target=partnerRelation(guard,partner.handle);const auto* player=partnerRelation(guard,1);
    if(!target||target->overridden||target->effective!=0||!player||player->effective!=2){refuse("relationship");return;}
    partnerEnemy={guard,partner,outgoing.worldEpoch,remote.session,remote.worldEpoch,tick+8000,tick};
    partnerEnemyThread=GetCurrentThreadId();
    reinterpret_cast<void(*)(void*,void*,void*,unsigned)>(gameImage+0x605990)(reinterpret_cast<void*>(guard.game+0x1f7bb8),reinterpret_cast<void*>(guard.actor),reinterpret_cast<void*>(partner.actor),2);
    NativeFaction after{};
    if(!readNativeFaction(memoryRead,base,guard.handle,after)){stopPartnerEnemy("readback_failed");return;}
    const auto* applied=partnerRelation(after,partner.handle);
    if(!applied||!applied->overridden||applied->overrideValue!=2||applied->effective!=2){stopPartnerEnemy("apply_failed");return;}
    ++partnerEnemyStarted;partnerEnemyResult="active";
}
void servicePartnerEnemy(ULONGLONG tick){
    if(!partnerEnemy.until)return;
    if(tick>=partnerEnemy.until){stopPartnerEnemy("expired");return;}
    if(!guardPairReady(tick)||!probeFollow||!avatarAiHeld||probeActor!=partnerEnemy.partner.actor||
       remote.session!=partnerEnemy.peerSession||remote.worldEpoch!=partnerEnemy.peerEpoch||guardLink.phase!=GuardPhase::Idle||aiDecisionHold.expires){stopPartnerEnemy("scope_changed");return;}
    if(tick-partnerEnemy.lastCheck<100)return;partnerEnemy.lastCheck=tick;
    NativeFaction guard{},partner{};
    if(!partnerEnemyScope(guard,partner)){stopPartnerEnemy("identity_changed");return;}
    const auto* relation=partnerRelation(guard,partner.handle);
    if(!relation||!relation->overridden||relation->overrideValue!=2)stopPartnerEnemy("ownership_changed");
}
void writePartnerEnemyStatus(ULONGLONG tick){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.partner-ai.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\"active\":%s,\"remainingMs\":%llu,\"result\":\"%s\",\"started\":%llu,\"released\":%llu,\"refused\":%llu,\"lost\":%llu,\"thread\":%lu,\"guardHandle\":%u,\"partnerHandle\":%u,\"epoch\":%llu}\n",
        partnerEnemy.until?"true":"false",partnerEnemy.until>tick?partnerEnemy.until-tick:0,partnerEnemyResult,partnerEnemyStarted,partnerEnemyReleased,partnerEnemyRefused,partnerEnemyLost,partnerEnemyThread,partnerEnemy.guard.handle,partnerEnemy.partner.handle,partnerEnemy.epoch);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
