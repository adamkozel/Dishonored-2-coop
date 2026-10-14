#pragma once
// One nonfatal, independently observed player sword event. This experiment
// temporarily owns health application for the exact guard; its ordinary pose
// stream still owns transforms. It is not general or simultaneous combat.
struct EnemyMeleeLease {
    CoopEnemyMeleePacket key{};NativeHealth victim{};NativeWorldNpc npc{};
    std::uintptr_t partner=0;std::uint32_t partnerHandle=0;
    ULONGLONG born=0,until=0,lastSend=0;
    std::uint64_t generation=0;unsigned cursor=0;
    bool ready=false,consumed=false,sentHit=false,applied=false,acknowledged=false;
};
EnemyMeleeLease enemyMelee{};CoopEnemyMeleePacket enemyMeleeLastHit{};
std::uint64_t meleeArm=0,meleeSequence=0,meleeLastSession=0,meleeLastSequence=0;
std::uint64_t meleeSent=0,meleeReceived=0,meleeRejected=0,meleeCaptured=0,meleeApplied=0,meleeAcks=0,meleeHealthBypassed=0;
float meleeBefore=0,meleeAfter=0,meleeInflicted=0;const char* meleeResult="idle";
bool enemyMeleeArmed(){return enemyMelee.until!=0;}
bool enemyMeleeScope(ULONGLONG tick,NativeHealth& victim){
    const auto& e=enemyMelee;const auto& b=e.key.binding;
    if(!e.until||tick>=e.until||!guardPairReady(tick)||guardLink.phase!=GuardPhase::Active||!guardSameBinding(guardLink.key,b)||
       !probeFollow||!probeFollowExact||!avatarAiHeld||probeActor!=e.partner||probeHandle!=e.partnerHandle)return false;
    const auto& h=localRole?remote:outgoing;const auto& c=localRole?outgoing:remote;
    if(b.hostSession!=h.session||b.clientSession!=c.session||b.hostEpoch!=h.worldEpoch||b.clientEpoch!=c.worldEpoch||b.mapId!=h.mapId||e.victim.game!=observedGame)return false;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);NativeWorldNpc npc{};NativeHealth player{};
    return readWorldNpcIdentity(memoryRead,base,observedGame,e.victim.handle,npc)&&npc.unique&&npc.reciprocal&&npc.actor==e.victim.actor&&
        npc.spawnerHandle==e.npc.spawnerHandle&&npc.spawnerName==kCombatGuardSpawner&&npc.definition==kCombatGuardDefinition&&
        readNativeHealth(memoryRead,base,e.victim.handle,victim)&&victim.game==e.victim.game&&victim.actor==e.victim.actor&&
        victim.component==e.victim.component&&victim.declaration==e.victim.declaration&&victim.logic==1&&victim.maximum==40&&(victim.current==40||victim.current==30)&&
        worldIdentityEntity(memoryRead,observedGame,e.partnerHandle)==e.partner&&readNativeHealth(memoryRead,base,1,player)&&player.logic==1&&player.current>0;
}
bool enemyMeleeHoldsHealth(){
    NativeHealth victim{};
    if(!localRole||!enemyMelee.ready||!enemyMeleeScope(GetTickCount64(),victim))return false;
    ++meleeHealthBypassed;return true;
}
bool enemyMeleeSend(CoopEnemyMeleePacket p){
    if(!sockets||!connection||connectionState!=k_ESteamNetworkingConnectionState_Connected)return false;
    p.binding.sequence=++meleeSequence;p.binding.role=localRole;
    if(!validEnemyMelee(p,remote,outgoing)||coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)!=k_EResultOK)return false;
    ++meleeSent;return true;
}
bool enemyMeleeControl(EnemyMeleeMessage kind,unsigned sourceRole=2){
    auto p=enemyMelee.key;p.kind=kind;p.sourceRole=sourceRole;
    if(kind==EnemyMeleeMessage::Arm){if(localRole)p.hostArm=0;else p.clientArm=0;}
    if(kind==EnemyMeleeMessage::Ack){p.event=1;p.declaration=coopMapId(kPlayerSwordDamageDecl);}
    return enemyMeleeSend(p);
}
void stopEnemyMelee(const char* reason,bool notify=true,bool endGuard=false){
    if(!enemyMelee.until)return;
    const bool incomplete=enemyMelee.consumed&&!enemyMelee.applied&&!enemyMelee.acknowledged;
    const bool sameGuard=guardSameBinding(guardLink.key,enemyMelee.key.binding);
    if(notify&&enemyMelee.key.hostArm&&enemyMelee.key.clientArm)enemyMeleeControl(EnemyMeleeMessage::Stop);
    enemyMelee={};meleeResult=reason;
    if(sameGuard&&(endGuard||incomplete))stopGuardLink("melee_incomplete");
}
void beginEnemyMelee(ULONGLONG tick){
    NativeHealth victim{};
    if(enemyMelee.until||enemyOutcome.until){++meleeRejected;meleeResult="owner_conflict";return;}
    if(!guardPairReady(tick)||guardLink.phase!=GuardPhase::Active||guardLink.key.mode!=1||!guardReadLocal()||!guardReadHealth(victim)||victim.current!=40||
       !probeFollow||!probeFollowExact||!avatarAiHeld||!probeActor||worldIdentityEntity(memoryRead,observedGame,probeHandle)!=probeActor||!swordDamageApiMatches()){
        ++meleeRejected;meleeResult="unavailable";return;
    }
    auto& e=enemyMelee;e.victim=victim;e.npc=guardLink.npc;e.partner=probeActor;e.partnerHandle=probeHandle;e.born=tick;e.until=tick+15000;
    e.key.binding=guardLink.key;e.key.binding.kind=GuardMessage::Stop;e.key.binding.role=localRole;
    if(localRole)e.key.clientArm=++meleeArm;else e.key.hostArm=++meleeArm;
    armDamageObservation(true);AcquireSRWLockShared(&damageObserveLock);e.generation=damageObserveGeneration.load();e.cursor=damageObservationCount;ReleaseSRWLockShared(&damageObserveLock);
    meleeResult="armed";if(!enemyMeleeControl(EnemyMeleeMessage::Arm))stopEnemyMelee("send_failed");
}
bool enemyMeleeNear(const float* position){
    float at[3]{};if(!memoryRead(enemyMelee.victim.actor+0x300,at,12))return false;float square=0;
    for(unsigned i=0;i<3;++i){if(!std::isfinite(at[i])||!std::isfinite(position[i]))return false;const float d=at[i]-position[i];square+=d*d;}
    return square<=9;
}
void receiveEnemyMelee(const CoopEnemyMeleePacket& p,ULONGLONG tick){
    ++meleeReceived;NativeHealth victim{};
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load()||!validEnemyMelee(p,outgoing,remote)||!enemyMeleeScope(tick,victim)||
       !guardSameBinding(enemyMelee.key.binding,p.binding)||(meleeLastSession==remote.session&&p.binding.sequence<=meleeLastSequence)){
        ++meleeRejected;return;
    }
    meleeLastSession=remote.session;meleeLastSequence=p.binding.sequence;
    auto& e=enemyMelee;auto& peerArm=localRole?e.key.hostArm:e.key.clientArm;const auto incoming=localRole?p.hostArm:p.clientArm;
    if(p.kind==EnemyMeleeMessage::Arm){
        if(e.consumed)return;
        if(peerArm&&peerArm!=incoming){++meleeRejected;return;}peerArm=incoming;
        if(!enemyMeleeControl(EnemyMeleeMessage::Ready))stopEnemyMelee("send_failed");return;
    }
    if(!enemyMeleeSameBinding(e.key,p)){++meleeRejected;return;}
    if(p.kind==EnemyMeleeMessage::Stop){stopEnemyMelee("peer_stopped",false);return;}
    if(p.kind==EnemyMeleeMessage::Ready){if(!e.consumed){e.ready=true;meleeResult="ready";}return;}
    if(p.kind==EnemyMeleeMessage::Ack){
        if(!e.sentHit||e.acknowledged||victim.current!=30){++meleeRejected;return;}
        e.acknowledged=true;++meleeAcks;meleeResult="acknowledged";return;
    }
    float position[3]{};
    if(e.consumed||victim.current!=40||(localRole&&!guardAiPaused())||!memoryRead(e.partner+0x300,position,12)||
       !enemyMeleeNear(position)||!enemyMeleeNear(remote.position)||!enemyMeleeConsume(e.ready,e.consumed,e.key,p)){++meleeRejected;return;}
    meleeBefore=victim.current;
    if(!applyNativePlayerSword(victim,e.partnerHandle,p.geometry,meleeInflicted)){
        ++meleeRejected;stopEnemyMelee("native_call_failed",true,true);return;
    }
    NativeHealth after{};
    if(!readNativeHealth(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),victim.handle,after)||after.actor!=victim.actor||after.component!=victim.component||
       after.current!=30||meleeInflicted!=10){++meleeRejected;stopEnemyMelee("native_result_failed",true,true);return;}
    meleeAfter=after.current;e.applied=true;++meleeApplied;meleeResult="hit_applied";
    if(!enemyMeleeControl(EnemyMeleeMessage::Ack,p.sourceRole))stopEnemyMelee("ack_failed",true,true);
}
void serviceEnemyMelee(ULONGLONG tick){
    if(!enemyMelee.until)return;NativeHealth victim{};
    if(!enemyMeleeScope(tick,victim)){stopEnemyMelee(tick>=enemyMelee.until?"expired":"scope_changed");return;}
    auto& e=enemyMelee;
    if(localRole&&guardLink.pose.sequence&&guardLink.pose.health!=40&&guardLink.pose.health!=30){stopEnemyMelee("unexpected_host_health",true,true);return;}
    if(e.consumed){if(victim.current!=30)stopEnemyMelee("completed_health_changed",true,true);return;}
    if(e.generation!=damageObserveGeneration.load()||tick>=damageObserveUntil.load()){stopEnemyMelee("capture_changed");return;}
    if(tick-e.lastSend>=200){e.lastSend=tick;if(!enemyMeleeControl(EnemyMeleeMessage::Arm)||
        (e.key.hostArm&&e.key.clientArm&&!enemyMeleeControl(EnemyMeleeMessage::Ready))){stopEnemyMelee("send_failed");return;}}
    if(!e.ready)return;
    std::array<DamageObservation,128> copy{};unsigned count=0;
    AcquireSRWLockShared(&damageObserveLock);copy=damageObservations;count=damageObservationCount;ReleaseSRWLockShared(&damageObserveLock);
    const auto capturedAt=GetTickCount64();
    for(;e.cursor<count;++e.cursor){const auto& hit=copy[e.cursor];
        if(hit.tick<e.born||hit.tick>capturedAt||capturedAt-hit.tick>=250||hit.before.game!=e.victim.game||hit.before.actor!=e.victim.actor||
           hit.before.component!=e.victim.component||hit.before.current!=40||hit.before.maximum!=40||!hit.afterValid||hit.after.current!=30||
           hit.attackerHandle!=1||hit.damageAmount!=10||hit.flags!=0x80||hit.inflictedDamage!=10||!(hit.resultFlags&1)||!enemyPlayerSword(hit.inflictorHandle))continue;
        std::uintptr_t decl=0;std::memcpy(&decl,hit.info.data(),8);if(!nativePlayerSwordDeclaration(decl)||!enemyMeleeNear(outgoing.position))continue;
        EnemyMeleeGeometry geometry{};float location[3]{};
        std::memcpy(&geometry.regions,hit.info.data()+0x20,4);std::memcpy(location,hit.info.data()+0x24,12);
        std::memcpy(geometry.normal,hit.info.data()+0x30,12);std::memcpy(geometry.direction,hit.info.data()+0x3c,12);std::memcpy(&geometry.region,hit.info.data()+0x4c,4);
        for(unsigned i=0;i<3;++i)geometry.offset[i]=location[i]-hit.actorPosition[i];
        if(!validEnemyMeleeGeometry(geometry))continue;
        e.consumed=true;e.sentHit=true;++e.cursor;++meleeCaptured;
        auto packet=e.key;packet.kind=EnemyMeleeMessage::Hit;packet.sourceRole=localRole;packet.event=1;packet.declaration=coopMapId(kPlayerSwordDamageDecl);
        packet.geometry=geometry;enemyMeleeLastHit=packet;
        if(!enemyMeleeSend(packet)){stopEnemyMelee("send_failed",true,true);return;}
        meleeResult="hit_sent";return;
    }
    if(victim.current!=40)stopEnemyMelee("unobserved_health_change",true,true);
}
void repeatEnemyMelee(){
    NativeHealth victim{};
    if(!enemyMeleeScope(GetTickCount64(),victim)||!enemyMelee.sentHit||!enemyMeleeSameBinding(enemyMelee.key,enemyMeleeLastHit)){++meleeRejected;return;}
    enemyMeleeSend(enemyMeleeLastHit);
}
void writeEnemyMeleeStatus(ULONGLONG tick){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.enemy-melee.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;const auto& e=enemyMelee;
    fprintf(f,"{\"armed\":%s,\"ready\":%s,\"consumed\":%s,\"sentHit\":%s,\"appliedHit\":%s,\"acknowledged\":%s,\"remainingMs\":%llu,\"hostArm\":%llu,\"clientArm\":%llu,\"binding\":%llu,\"result\":\"%s\",\"sent\":%llu,\"received\":%llu,\"rejected\":%llu,\"captured\":%llu,\"applied\":%llu,\"acks\":%llu,\"healthBypassed\":%llu,\"before\":%.9g,\"after\":%.9g,\"inflicted\":%.9g}\n",
        e.until?"true":"false",e.ready?"true":"false",e.consumed?"true":"false",e.sentHit?"true":"false",e.applied?"true":"false",e.acknowledged?"true":"false",
        e.until>tick?e.until-tick:0,e.key.hostArm,e.key.clientArm,e.key.binding.binding,meleeResult,meleeSent,meleeReceived,meleeRejected,meleeCaptured,meleeApplied,meleeAcks,meleeHealthBypassed,meleeBefore,meleeAfter,meleeInflicted);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
