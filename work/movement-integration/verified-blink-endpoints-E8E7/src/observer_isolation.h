#pragma once
// Local test-fixture isolation, never a world-authority protocol. Temporarily
// hold the two known interfering NPCs so the visible guard can be measured.
struct ObserverIsolation {
    std::array<NativeAiComponentState,10> held{};
    unsigned count=0;
    std::uint64_t epoch=0,peerSession=0,peerEpoch=0;
    ULONGLONG until=0;
    bool ready=false,sourceFixture=false,movementFixture=false;
};
ObserverIsolation observerIsolation{};
std::uint64_t isolationStarted=0,isolationReleased=0,isolationLost=0,isolationRefused=0;
const char* isolationResult="idle";
void stopObserverIsolation(const char* reason){
    if(!observerIsolation.until)return;
    for(unsigned i=observerIsolation.count;i>0;--i){
        if(releaseNativeAiReference(observerIsolation.held[i-1],observerIsolation.epoch))++isolationReleased;else ++isolationLost;
    }
    observerIsolation={};isolationResult=reason;
}
bool isolationPairReady(ULONGLONG tick,bool sourceFixture){
    return localRole==(sourceFixture?0u:1u)&&guardPairReady(tick);
}
void beginObserverIsolation(ULONGLONG tick,bool sourceFixture=false,bool movementFixture=false){
    const auto refuse=[&](const char* reason){++isolationRefused;isolationResult=reason;};
    if(observerIsolation.until){refuse("already_held");return;}
    if(GetCurrentThreadId()!=engineThread.load()||!isolationPairReady(tick,sourceFixture)||!nativeAiRequestsMatch()){refuse("unsupported_scope");return;}
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    std::vector<NativeWorldNpc> catalog;unsigned unbound=0;
    if(!readWorldNpcCatalog(memoryRead,base,observedGame,catalog,unbound)){refuse("catalog_unavailable");return;}
    // Movement fixtures cover the office and its outer balcony. The tutorial
    // guards are separate native actors; leaving them live spoiled the first
    // reverse-side keyhole recording with normal enemy damage. Each exact
    // spawner/definition gets its own reversible decision/reflex reference.
    constexpr const char* names[]={"ld_spawner_ramsey_escape","ld_spawner_guard_escape_2","ld_spawner_guard_escape_1","ld_spawner_guard_tutorial_1","ld_spawner_guard_tutorial_2"};
    constexpr const char* definitions[]={"models/characters/def/story/mortimer_ramsey.def",kCombatGuardDefinition,kCombatGuardDefinition,
        "models/characters/def/generic/dunwall_guard_male_small_01.def","models/characters/def/generic/dunwall_guard_male_small_01.def"};
    ObserverIsolation next{};next.sourceFixture=sourceFixture;next.movementFixture=movementFixture;
    const unsigned targets=movementFixture?5:2;
    for(unsigned n=0;n<targets;++n){
        const NativeWorldNpc* target=nullptr;
        for(const auto& npc:catalog)if(npc.spawnerName==names[n]){
            if(target||!npc.unique||!npc.reciprocal||npc.definition!=definitions[n]){refuse("ambiguous_target");return;}target=&npc;
        }
        NativeAiComponentState attack{};
        if(!target||!readNativeAiComponent(memoryRead,base,observedGame,target->handle,NativeAiKind::Attack,attack)||
           (attack.flags&0xc)||attack.schedulerActive){refuse("target_attacking");return;}
        for(unsigned k=0;k<2;++k){
            auto& s=next.held[n*2+k];const auto kind=k?NativeAiKind::Reflex:NativeAiKind::Decision;
            if(!readNativeAiComponent(memoryRead,base,observedGame,target->handle,kind,s)||(s.flags&0xc)!=8||
               (s.flags&0x7fffff0)||!s.schedulerActive){refuse("target_not_active");return;}
        }
    }
    next.epoch=outgoing.worldEpoch;next.peerSession=remote.session;next.peerEpoch=remote.worldEpoch;next.until=tick+(movementFixture?60000:30000);
    observerIsolation=next;
    for(unsigned i=0;i<targets*2;++i){
        auto& s=observerIsolation.held[i];
        reinterpret_cast<void(*)(void*,void*)>(gameImage+0x7e7800)(reinterpret_cast<void*>(s.component),reinterpret_cast<void*>(s.manager));
        ++observerIsolation.count;
        std::uint32_t flags=0;
        if(!memoryRead(s.component+0x1c,&flags,4)||((flags>>6)&0x1fffff)!=1){stopObserverIsolation("pause_failed");refuse("pause_failed");return;}
    }
    ++isolationStarted;isolationResult="pausing";
}
void serviceObserverIsolation(ULONGLONG tick){
    if(!observerIsolation.until)return;
    if(tick>=observerIsolation.until){stopObserverIsolation("expired");return;}
    if(!isolationPairReady(tick,observerIsolation.sourceFixture)||outgoing.worldEpoch!=observerIsolation.epoch||remote.session!=observerIsolation.peerSession||remote.worldEpoch!=observerIsolation.peerEpoch){stopObserverIsolation("scope_changed");return;}
    bool ready=true;
    for(unsigned i=0;i<observerIsolation.count;++i){
        const auto& held=observerIsolation.held[i];
        NativeAiComponentState s{},attack{};
        if(!readNativeAiComponent(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),held.game,held.handle,held.kind,s)||
           s.component!=held.component||s.manager!=held.manager||(s.flags&0xc)!=8||((s.flags>>6)&0x1fffff)!=1||
           !readNativeAiComponent(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),held.game,held.handle,NativeAiKind::Attack,attack)||
           (attack.flags&0xc)||attack.schedulerActive){stopObserverIsolation("native_state_changed");return;}
        if(s.schedulerActive||((s.flags>>4)&3)!=2)ready=false;
    }
    if(!ready&&observerIsolation.ready){stopObserverIsolation("ownership_changed");return;}
    observerIsolation.ready=ready;if(ready)isolationResult="held";
}
void writeObserverIsolation(ULONGLONG tick){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.observer-isolation.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\"testFixtureOnly\":true,\"active\":%s,\"ready\":%s,\"remainingMs\":%llu,\"heldReferences\":%u,\"started\":%llu,\"released\":%llu,\"lost\":%llu,\"refused\":%llu,\"result\":\"%s\",\"handles\":[%u,%u],\"movementFixture\":%s,\"thirdHandle\":%u}\n",
        observerIsolation.until?"true":"false",observerIsolation.ready?"true":"false",observerIsolation.until>tick?observerIsolation.until-tick:0,
        observerIsolation.count,isolationStarted,isolationReleased,isolationLost,isolationRefused,isolationResult,observerIsolation.held[0].handle,observerIsolation.held[2].handle,
        observerIsolation.movementFixture?"true":"false",observerIsolation.held[4].handle);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
