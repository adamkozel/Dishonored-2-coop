#pragma once

// Included in the engine bridge's private namespace. Developer-only experiment:
// one authored guard, one native pause reference, a nonrenewable two-second lease.
struct AiDecisionHold {
    NativeAiDecisionState target{};
    std::uint64_t epoch=0,peerSession=0,peerEpoch=0;
    ULONGLONG expires=0;
};
AiDecisionHold aiDecisionHold{};
std::uint64_t aiHoldStarted=0,aiHoldReleased=0,aiHoldRefused=0,aiHoldInvalidated=0;
std::uint32_t aiHoldLastHandle=0;
const char* aiHoldResult="idle";

bool nativeAiRequestsMatch() {
    const unsigned char bytes[]={0x48,0x89,0x5c,0x24,0x08,0x48,0x89,0x74,0x24,0x10,0x57,0x48,0x83,0xec,0x20,
        0x48,0x8b,0xf2,0x33,0xd2,0x48,0x8b,0xd9,0x44,0x8d,0x42,0x01,0x48,0x8d,0x8e,0x08,0x0c};
    return gameImage&&!std::memcmp(gameImage+0x7e7800,bytes,sizeof(bytes))&&!std::memcmp(gameImage+0x7e79c0,bytes,sizeof(bytes));
}
bool aiHoldPairReady(ULONGLONG tick) {
    return localRole==1&&lastReceive&&tick-lastReceive<1000&&connectionState==k_ESteamNetworkingConnectionState_Connected&&
        coopWorldPairReady(outgoing,remote)&&!std::strcmp(mapName,"campaign/dunwall/escape/tower/dunwall_escape_tower_p");
}
bool releaseNativeAiReference(const NativeAiComponentState& s,std::uint64_t epoch) {
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    std::uintptr_t game=0,manager=0,vt=0,owner=0,list=0;std::uint32_t flags=0,count=0;
    // Validate lifetime without relying on the scheduler's changing list. A
    // discarded world must never turn a saved pointer into a mutation target.
    bool valid=outgoing.worldEpoch==epoch&&memoryRead(base+0x39a5290,&game,8)&&game==s.game&&
        memoryRead(game,&vt,8)&&vt==base+0x1c940a8&&memoryRead(game+0x1e8960,&manager,8)&&manager==s.manager&&
        worldIdentityEntity(memoryRead,game,s.handle)==s.actor&&
        memoryRead(s.actor+0x268,&list,8)&&list&&memoryRead(s.actor+0x270,&count,4)&&(count&0xffffff)<=256;
    unsigned matches=0;
    if(valid)for(unsigned i=0;i<(count&0xffffff);++i) {
        std::uintptr_t component=0;
        if(!memoryRead(list+i*8,&component,8)){valid=false;break;}
        if(component==s.component)++matches;
    }
    valid=valid&&matches==1&&memoryRead(s.component,&vt,8)&&vt==base+nativeAiVtable(s.kind)&&
        memoryRead(s.component+0x10,&owner,8)&&owner==s.actor&&memoryRead(s.component+0x1c,&flags,4)&&
        (flags&0xc)==8&&((flags>>6)&0x1fffff)>0&&nativeAiRequestsMatch();
    if(!valid)return false;
    // Native request owns scheduler locking and reference-counted release.
    // Never restore old flags, force OnResume, or release another system's refs.
    reinterpret_cast<void(*)(void*,void*)>(gameImage+0x7e79c0)(reinterpret_cast<void*>(s.component),reinterpret_cast<void*>(s.manager));
    return true;
}
void releaseAiDecisionHold(const char* reason) {
    if(!aiDecisionHold.expires)return;
    const auto held=aiDecisionHold;aiDecisionHold={};
    if(!releaseNativeAiReference(held.target,held.epoch)) {
        ++aiHoldInvalidated;aiHoldResult="identity_invalidated";
        consoleOutput("COOP: AI decision hold invalidated handle=%08x reason=%s\n",held.target.handle,reason);return;
    }
    ++aiHoldReleased;aiHoldResult=reason;
    consoleOutput("COOP: AI decision hold released handle=%08x reason=%s\n",held.target.handle,reason);
}
void serviceAiDecisionHold(ULONGLONG tick) {
    if(!aiDecisionHold.expires)return;
    if(tick>=aiDecisionHold.expires){releaseAiDecisionHold("expired");return;}
    if(observedGame!=aiDecisionHold.target.game||outgoing.worldEpoch!=aiDecisionHold.epoch||
       remote.session!=aiDecisionHold.peerSession||remote.worldEpoch!=aiDecisionHold.peerEpoch||!aiHoldPairReady(tick))
        releaseAiDecisionHold("scope_changed");
}
void beginAiDecisionHold(ULONGLONG tick) {
    auto refuse=[](const char* reason){++aiHoldRefused;aiHoldResult=reason;consoleOutput("COOP: AI decision hold refused reason=%s\n",reason);};
    if(aiDecisionHold.expires||guardAiOwned||npcAuthorityHoldActive||npcAuthorityStreamActive){refuse("already_held");return;}
    if(!aiHoldPairReady(tick)||!nativeAiRequestsMatch()){refuse("unsupported_scope");return;}
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    std::vector<NativeWorldNpc> catalog;unsigned unbound=0;
    if(!readWorldNpcCatalog(memoryRead,base,observedGame,catalog,unbound)){refuse("catalog_unavailable");return;}
    const NativeWorldNpc* guard=nullptr;
    for(const auto& n:catalog)if(n.spawnerName=="ld_spawner_guard_tutorial_1") {
        if(guard||!n.unique||!n.reciprocal||n.definition!="models/characters/def/generic/dunwall_guard_male_small_01.def") {
            refuse("ambiguous_guard");return;
        }
        guard=&n;
    }
    NativeAiDecisionState s{};
    if(!guard||!readNativeAiDecision(memoryRead,base,observedGame,guard->handle,s)||!s.tree||
       (s.flags&0xc)!=8||(s.flags&0x7fffff0)||!s.schedulerActive) {refuse("decision_not_active");return;}
    aiDecisionHold={s,outgoing.worldEpoch,remote.session,remote.worldEpoch,tick+2000};aiHoldLastHandle=s.handle;
    reinterpret_cast<void(*)(void*,void*)>(gameImage+0x7e7800)(reinterpret_cast<void*>(s.component),reinterpret_cast<void*>(s.manager));
    std::uint32_t flags=0;
    if(!memoryRead(s.component+0x1c,&flags,4)||((flags>>6)&0x1fffff)!=1) {
        releaseAiDecisionHold("unexpected_pause_result");refuse("unexpected_pause_result");return;
    }
    ++aiHoldStarted;aiHoldResult="holding";
    consoleOutput("COOP: AI decision hold started handle=%08x poolIndex=%u lease=2000ms\n",s.handle,s.poolIndex);
}
