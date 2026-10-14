#pragma once
// Developer-only: one hit/15 seconds or twelve hits/60 seconds, mutually armed.
// Neither side can arm the other. Native pointers and handles resolve locally.
struct SwordLink {
    CoopSwordPacket key{};NativeHealth player{},partner{};NativeWorldNpc guard{};
    ULONGLONG until=0,born=0,lastSend=0;std::uint64_t generation=0;
    unsigned cursor=0;bool ready=false;std::uint64_t consumed=0;
    std::uint64_t requiredGuardBinding=0;
};
SwordLink swordLink{};CoopSwordPacket swordLastHit{};
std::uint64_t swordArm=0,swordSequence=0,swordLastSession=0,swordLastSequence=0;
std::uint64_t swordSent=0,swordReceived=0,swordRejected=0,swordCaptured=0,swordApplied=0,swordStopped=0;
float swordBefore=0,swordAfter=0,swordInflicted=0;DWORD swordThread=0;
float swordNativeRecovery=0;
const char* swordResult="idle";
void swordFillScope(CoopSwordPacket& p){
    const auto& h=localRole?remote:outgoing;const auto& c=localRole?outgoing:remote;
    p.hostSession=h.session;p.clientSession=c.session;p.hostEpoch=h.worldEpoch;p.clientEpoch=c.worldEpoch;p.mapId=h.mapId;p.guardKey=swordGuardKey();
}
bool swordSendPacket(CoopSwordPacket p){
    if(!sockets||!connection||connectionState!=k_ESteamNetworkingConnectionState_Connected)return false;
    p.sequence=++swordSequence;p.role=localRole;
    if(sockets->SendMessageToConnection(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)!=k_EResultOK)return false;
    ++swordSent;return true;
}
bool swordControl(SwordMessage kind){
    auto p=swordLink.key;p.kind=kind;
    if(kind==SwordMessage::Arm){if(localRole)p.hostArm=0;else p.clientArm=0;}
    return swordSendPacket(p);
}
void stopSwordLink(const char* reason,bool notify=true){
    if(!swordLink.until)return;
    if(notify&&swordLink.key.hostArm&&swordLink.key.clientArm)swordControl(SwordMessage::Stop);
    swordLink={};++swordStopped;swordResult=reason;
}
bool swordScope(ULONGLONG tick,NativeHealth& player){
    if(!swordLink.until||tick>=swordLink.until||!guardPairReady(tick))return false;
    if(swordLink.requiredGuardBinding&&(guardLink.phase!=GuardPhase::Active||guardLink.key.mode!=1||guardLink.key.binding!=swordLink.requiredGuardBinding))return false;
    CoopSwordPacket now{};swordFillScope(now);now.hostArm=swordLink.key.hostArm;now.clientArm=swordLink.key.clientArm;
    // During negotiation one arm can still be zero.
    const auto& k=swordLink.key;
    if(now.hostSession!=k.hostSession||now.clientSession!=k.clientSession||now.hostEpoch!=k.hostEpoch||now.clientEpoch!=k.clientEpoch||now.mapId!=k.mapId)return false;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);NativeWorldNpc guard{};
    if(!readNativeHealth(memoryRead,base,1,player)||player.game!=swordLink.player.game||player.game!=observedGame||player.actor!=swordLink.player.actor||
       player.component!=swordLink.player.component||player.current<=0||player.maximum!=70||player.logic!=1||
       !readWorldNpcIdentity(memoryRead,base,observedGame,swordLink.guard.handle,guard)||guard.actor!=swordLink.guard.actor||!guard.unique||!guard.reciprocal||
       guard.spawnerHandle!=swordLink.guard.spawnerHandle||guard.spawnerName!=kSwordGuardSpawner||guard.definition!=kSwordGuardDefinition)return false;
    if(!localRole){
        NativeHealth partner{};
        if(!probeFollow||!avatarAiHeld||probeActor!=swordLink.partner.actor||probeHandle!=swordLink.partner.handle||
           !readNativeHealth(memoryRead,base,probeHandle,partner)||partner.game!=observedGame||partner.actor!=swordLink.partner.actor||
           partner.component!=swordLink.partner.component||partner.current!=1||partner.maximum!=1)return false;
    }
    return true;
}
void beginSwordLink(ULONGLONG tick,bool cumulative=false){
    if(swordLink.until){++swordRejected;swordResult="duplicate_arm";return;}
    NativeHealth player{},partner{};NativeWorldNpc guard{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(!guardPairReady(tick)||!swordDamageApiMatches()||
       (guardLink.phase!=GuardPhase::Idle&&!(guardLink.phase==GuardPhase::Active&&guardLink.key.mode==1))||aiDecisionHold.expires||
       !readNativeHealth(memoryRead,base,1,player)||player.game!=observedGame||player.current!=56||player.maximum!=70||player.logic!=1||
       (!localRole&&(!probeFollow||!avatarAiHeld||!readNativeHealth(memoryRead,base,probeHandle,partner)||partner.actor!=probeActor||partner.current!=1||partner.maximum!=1))){++swordRejected;swordResult="unavailable";return;}
    std::vector<NativeWorldNpc> catalog;unsigned unbound=0,matches=0;
    if(readWorldNpcCatalog(memoryRead,base,observedGame,catalog,unbound))for(const auto& n:catalog)
        if(n.spawnerName==kSwordGuardSpawner&&n.definition==kSwordGuardDefinition&&n.unique&&n.reciprocal){guard=n;++matches;}
    if(matches!=1){++swordRejected;swordResult="identity";return;}
    swordLink.player=player;swordLink.partner=partner;swordLink.guard=guard;swordLink.born=tick;swordLink.until=tick+(cumulative?60000:15000);swordLink.key.limit=cumulative?12:1;
    if(guardLink.phase==GuardPhase::Active)swordLink.requiredGuardBinding=guardLink.key.binding;
    swordFillScope(swordLink.key);if(localRole)swordLink.key.clientArm=++swordArm;else swordLink.key.hostArm=++swordArm;
    if(!localRole){
        armDamageObservation(true);AcquireSRWLockShared(&damageObserveLock);
        swordLink.generation=damageObserveGeneration.load();swordLink.cursor=damageObservationCount;ReleaseSRWLockShared(&damageObserveLock);
    }
    swordResult="armed";if(!swordControl(SwordMessage::Arm))stopSwordLink("send_failed");
}
void receiveSwordPacket(const CoopSwordPacket& p,ULONGLONG tick){
    ++swordReceived;NativeHealth player{};
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load()||!validCoopSword(p,outgoing,remote)||!swordScope(tick,player)||p.limit!=swordLink.key.limit||
       (swordLastSession==remote.session&&p.sequence<=swordLastSequence)){++swordRejected;return;}
    swordLastSession=remote.session;swordLastSequence=p.sequence;
    auto& k=swordLink.key;auto& peerArm=localRole?k.hostArm:k.clientArm;const auto incoming=localRole?p.hostArm:p.clientArm;
    if(p.kind==SwordMessage::Arm){
        if(peerArm&&peerArm!=incoming){++swordRejected;return;}peerArm=incoming;
        if(!swordControl(SwordMessage::Ready))stopSwordLink("send_failed");return;
    }
    if(!swordSameBinding(k,p)){++swordRejected;return;}
    if(p.kind==SwordMessage::Stop){stopSwordLink("peer_stopped",false);return;}
    if(p.kind==SwordMessage::Ready){swordLink.ready=true;if(!swordLink.consumed)swordResult="ready";return;}
    if(!localRole||!swordConsumeHit(swordLink.ready,swordLink.consumed,k,p)){++swordRejected;return;}
    // Consumption precedes the native call, including on a failed readback.
    const float expected=swordLink.consumed==1?56.f:swordAfter;
    if(!swordPlayerHealthReady(player.current,expected,swordLink.key.limit>1)){++swordRejected;stopSwordLink("health_changed");return;}
    swordNativeRecovery+=player.current-expected;
    float position[3]{};if(!memoryRead(player.actor+0x300,position,12)){++swordRejected;swordResult="position";return;}
    for(unsigned i=0;i<3;++i){position[i]+=p.hitOffset[i];if(!std::isfinite(position[i])){++swordRejected;swordResult="position";return;}}
    swordBefore=player.current;swordThread=GetCurrentThreadId();
    if(!applyNativeSwordSample(player,swordLink.guard.handle,position,p.direction,swordInflicted,swordLink.key.limit>1)){++swordRejected;swordResult="native_call";return;}
    NativeHealth after{};
    if(!readNativeHealth(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),1,after)||after.actor!=player.actor||after.component!=player.component){++swordRejected;swordResult="readback";return;}
    swordAfter=after.current;++swordApplied;swordResult="hit_applied";
}
void serviceSwordLink(ULONGLONG tick){
    if(!swordLink.until)return;NativeHealth player{};
    if(!swordScope(tick,player)){stopSwordLink(tick>=swordLink.until?"expired":"scope_changed");return;}
    // Capture ownership is part of the lease even before a guard is made
    // hostile or after its first hit has been consumed.
    if(!localRole&&(damageObserveGeneration.load()!=swordLink.generation||tick>=damageObserveUntil.load())){stopSwordLink("capture_changed");return;}
    if(tick-swordLink.lastSend>=200){
        swordLink.lastSend=tick;
        if(!swordControl(SwordMessage::Arm)||(swordLink.key.hostArm&&swordLink.key.clientArm&&!swordControl(SwordMessage::Ready))){stopSwordLink("send_failed");return;}
    }
    if(localRole||!swordLink.ready||swordLink.consumed>=swordLink.key.limit)return;
    if(!partnerEnemy.until||partnerEnemy.guard.handle!=swordLink.guard.handle||partnerEnemy.partner.handle!=swordLink.partner.handle)return;
    std::array<DamageObservation,128> hits{};unsigned count=0;std::uint64_t generation=0;
    AcquireSRWLockShared(&damageObserveLock);count=damageObservationCount;hits=damageObservations;generation=damageObserveGeneration.load();ReleaseSRWLockShared(&damageObserveLock);
    if(generation!=swordLink.generation){stopSwordLink("capture_changed");return;}
    for(;swordLink.cursor<count;++swordLink.cursor){const auto& e=hits[swordLink.cursor];
        if(e.tick<swordLink.born||tick-e.tick>=250||e.before.game!=observedGame||e.before.actor!=swordLink.partner.actor||e.before.component!=swordLink.partner.component||
           e.before.handle!=swordLink.partner.handle||e.attackerHandle!=swordLink.guard.handle||e.damageAmount!=20||e.flags!=0x80||!e.afterValid||e.after.current!=1||e.inflictedDamage!=0)continue;
        std::uintptr_t decl=0;std::memcpy(&decl,e.info.data(),8);if(!nativeSwordDeclaration(decl))continue;
        auto hit=swordLink.key;hit.kind=SwordMessage::Hit;hit.event=swordLink.consumed+1;
        std::memcpy(hit.hitOffset,e.info.data()+0x24,12);std::memcpy(hit.direction,e.info.data()+0x3c,12);
        for(unsigned i=0;i<3;++i)hit.hitOffset[i]-=e.actorPosition[i];
        auto check=hit;check.sequence=1;check.role=0;if(!validCoopSword(check,remote,outgoing)){++swordRejected;continue;}
        swordLink.consumed=hit.event;++swordLink.cursor;++swordCaptured;swordLastHit=hit;swordResult="hit_sent";
        if(!swordSendPacket(hit))stopSwordLink("send_failed");return;
    }
}
void repeatSwordPacket(){
    NativeHealth player{};
    if(localRole||!swordScope(GetTickCount64(),player)||!swordLink.consumed||!swordSameBinding(swordLink.key,swordLastHit)){++swordRejected;return;}
    swordSendPacket(swordLastHit);
}
void writeSwordLinkStatus(ULONGLONG tick){
    char path[MAX_PATH]{},temp[MAX_PATH]{};if(std::snprintf(path,sizeof(path),"%s.sword-link.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\"armed\":%s,\"ready\":%s,\"consumed\":%s,\"remainingMs\":%llu,\"hostArm\":%llu,\"clientArm\":%llu,\"result\":\"%s\",\"sent\":%llu,\"received\":%llu,\"rejected\":%llu,\"captured\":%llu,\"applied\":%llu,\"stopped\":%llu,\"thread\":%lu,\"before\":%.9g,\"after\":%.9g,\"inflicted\":%.9g,\"nativeRecovery\":%.9g}\n",
        swordLink.until?"true":"false",swordLink.ready?"true":"false",swordLink.consumed?"true":"false",swordLink.until>tick?swordLink.until-tick:0,swordLink.key.hostArm,swordLink.key.clientArm,swordResult,swordSent,swordReceived,swordRejected,swordCaptured,swordApplied,swordStopped,swordThread,swordBefore,swordAfter,swordInflicted,swordNativeRecovery);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}

