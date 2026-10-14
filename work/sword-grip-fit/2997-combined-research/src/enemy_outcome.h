#pragma once
// Explicit, fixed-guard kill outcome. Local native attacks remain responsible
// for their original kill; the peer reconstructs the canonical death damage.
struct EnemyOutcomeLease {
    CoopEnemyOutcomePacket key{};NativeHealth victim{};NativeWorldNpc npc{};
    std::uintptr_t partner=0;std::uint32_t partnerHandle=0;
    ULONGLONG born=0,until=0,lastSend=0;
    std::uint64_t generation=0;unsigned cursor=0;
    bool ready=false,consumed=false,sentKill=false,acknowledged=false;
};
EnemyOutcomeLease enemyOutcome{};CoopEnemyOutcomePacket enemyLastKill{};
std::uint64_t enemyOutcomeArm=0,enemyOutcomeSequence=0,enemyOutcomeLastSession=0,enemyOutcomeLastSequence=0;
std::uint64_t enemyOutcomeSent=0,enemyOutcomeReceived=0,enemyOutcomeRejected=0,enemyOutcomeCaptured=0,enemyOutcomeApplied=0,enemyOutcomeAcknowledged=0,enemyOutcomeMerged=0;
float enemyOutcomeBefore=0,enemyOutcomeAfter=0,enemyOutcomeInflicted=0;
const char* enemyOutcomeResult="idle";
bool enemyOutcomeScope(ULONGLONG tick,NativeHealth& victim){
    const auto& e=enemyOutcome;const auto& b=e.key.binding;
    if(!e.until||tick>=e.until||!guardPairReady(tick))return false;
    const auto& h=localRole?remote:outgoing;const auto& c=localRole?outgoing:remote;
    if(b.hostSession!=h.session||b.clientSession!=c.session||b.hostEpoch!=h.worldEpoch||b.clientEpoch!=c.worldEpoch||b.mapId!=h.mapId||e.victim.game!=observedGame)return false;
    NativeWorldNpc npc{};NativeHealth player{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    return readWorldNpcIdentity(memoryRead,base,observedGame,e.victim.handle,npc)&&npc.unique&&npc.reciprocal&&npc.actor==e.victim.actor&&
        npc.spawnerHandle==e.npc.spawnerHandle&&npc.spawnerName==kCombatGuardSpawner&&npc.definition==kCombatGuardDefinition&&
        readNativeHealth(memoryRead,base,e.victim.handle,victim)&&victim.actor==e.victim.actor&&victim.component==e.victim.component&&victim.game==e.victim.game&&
        victim.logic==1&&victim.maximum==40&&(victim.current==40||victim.current==0)&&
        worldIdentityEntity(memoryRead,observedGame,e.partnerHandle)==e.partner&&
        readNativeHealth(memoryRead,base,1,player)&&player.logic==1&&player.current>0;
}
bool enemyOutcomeSend(CoopEnemyOutcomePacket p){
    if(!sockets||!connection||connectionState!=k_ESteamNetworkingConnectionState_Connected)return false;
    p.binding.sequence=++enemyOutcomeSequence;p.binding.role=localRole;
    if(!validEnemyOutcome(p,remote,outgoing))return false;
    if(sockets->SendMessageToConnection(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)!=k_EResultOK)return false;
    ++enemyOutcomeSent;return true;
}
bool enemyOutcomeControl(EnemyOutcomeMessage kind,unsigned sourceRole=2){
    auto p=enemyOutcome.key;p.kind=kind;p.sourceRole=sourceRole;
    if(kind==EnemyOutcomeMessage::Arm){if(localRole)p.hostArm=0;else p.clientArm=0;}
    if(kind==EnemyOutcomeMessage::Kill||kind==EnemyOutcomeMessage::Ack){p.event=1;p.declaration=coopMapId(kEnemyAssassinationDecl);}
    return enemyOutcomeSend(p);
}
void stopEnemyOutcome(const char* reason,bool notify=true){
    if(!enemyOutcome.until)return;
    if(notify&&enemyOutcome.key.hostArm&&enemyOutcome.key.clientArm)enemyOutcomeControl(EnemyOutcomeMessage::Stop);
    enemyOutcome={};enemyOutcomeResult=reason;
}
void beginEnemyOutcome(ULONGLONG tick){
    NativeHealth victim{};
    if(enemyMeleeArmed()){++enemyOutcomeRejected;enemyOutcomeResult="owner_conflict";return;}
    if(enemyOutcome.until){++enemyOutcomeRejected;enemyOutcomeResult="already_armed";return;}
    if(!guardPairReady(tick)||guardLink.phase!=GuardPhase::Active||guardLink.key.mode!=1||!guardReadLocal()||!guardReadHealth(victim)||victim.current!=40||
       !probeFollow||!avatarAiHeld||!probeActor||worldIdentityEntity(memoryRead,observedGame,probeHandle)!=probeActor||!swordDamageApiMatches()){
        ++enemyOutcomeRejected;enemyOutcomeResult="unavailable";return;
    }
    auto& e=enemyOutcome;e.victim=victim;e.npc=guardLink.npc;e.partner=probeActor;e.partnerHandle=probeHandle;e.born=tick;e.until=tick+60000;
    e.key.binding=guardLink.key;e.key.binding.kind=GuardMessage::Stop;e.key.binding.role=localRole;
    if(localRole)e.key.clientArm=++enemyOutcomeArm;else e.key.hostArm=++enemyOutcomeArm;
    armDamageObservation(true);AcquireSRWLockShared(&damageObserveLock);e.generation=damageObserveGeneration.load();e.cursor=damageObservationCount;ReleaseSRWLockShared(&damageObserveLock);
    enemyOutcomeResult="armed";if(!enemyOutcomeControl(EnemyOutcomeMessage::Arm))stopEnemyOutcome("send_failed");
}
bool enemyPlayerSword(std::uint32_t handle){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage),weapon=worldIdentityEntity(memoryRead,observedGame,handle);
    std::uintptr_t vt=0,decl=0,name=0,list=0,cpnt=0,owner=0;std::uint32_t parent=0,count=0;std::string path;
    if(!weapon||!memoryRead(weapon,&vt,8)||vt!=base+0x1ce1e98||!memoryRead(weapon+0xac,&parent,4)||parent!=1||
       !memoryRead(weapon+0x2e8,&decl,8)||!decl||!memoryRead(decl,&vt,8)||vt!=base+0x1ab58f8||!memoryRead(decl+8,&name,8)||
       !worldIdentityString(memoryRead,name,path)||path!="models/weapons/ply_sword/ply_sword"||!memoryRead(weapon+0x268,&list,8)||!list||
       !memoryRead(weapon+0x270,&count,4)||!(count&0xffffff)||(count&0xffffff)>64)return false;
    for(unsigned i=0;i<(count&0xffffff);++i){std::uintptr_t c=0;if(!memoryRead(list+i*8,&c,8)||!c||!memoryRead(c,&vt,8))return false;
        if(vt==base+0x1b9d508){if(cpnt||!memoryRead(c+0x10,&owner,8)||owner!=weapon)return false;cpnt=c;}}
    return cpnt&&worldIdentityEntity(memoryRead,observedGame,handle)==weapon&&memoryRead(weapon+0xac,&parent,4)&&parent==1;
}
bool enemyNearVictim(const float* position){
    float at[3]{};if(!memoryRead(enemyOutcome.victim.actor+0x300,at,12))return false;
    float square=0;for(unsigned i=0;i<3;++i){if(!std::isfinite(at[i])||!std::isfinite(position[i]))return false;const float d=at[i]-position[i];square+=d*d;}
    return square<=9.f;
}
void enemyReleaseGuard(){
    stopSwordLink("enemy_died");stopPartnerEnemy("enemy_died");stopGuardLink("enemy_died",false);
}
void receiveEnemyOutcome(const CoopEnemyOutcomePacket& p,ULONGLONG tick){
    ++enemyOutcomeReceived;NativeHealth victim{};
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load()||!validEnemyOutcome(p,outgoing,remote)||!enemyOutcomeScope(tick,victim)||
       !guardSameBinding(enemyOutcome.key.binding,p.binding)||(enemyOutcomeLastSession==remote.session&&p.binding.sequence<=enemyOutcomeLastSequence)){
        ++enemyOutcomeRejected;return;
    }
    enemyOutcomeLastSession=remote.session;enemyOutcomeLastSequence=p.binding.sequence;
    auto& e=enemyOutcome;auto& peerArm=localRole?e.key.hostArm:e.key.clientArm;const auto incoming=localRole?p.hostArm:p.clientArm;
    if(p.kind==EnemyOutcomeMessage::Arm){
        if(e.consumed)return;
        if(peerArm&&peerArm!=incoming){++enemyOutcomeRejected;return;}peerArm=incoming;
        if(!enemyOutcomeControl(EnemyOutcomeMessage::Ready))stopEnemyOutcome("send_failed");return;
    }
    if(!enemySameBinding(e.key,p)){++enemyOutcomeRejected;return;}
    if(p.kind==EnemyOutcomeMessage::Stop){stopEnemyOutcome("peer_stopped",false);return;}
    if(p.kind==EnemyOutcomeMessage::Ready){if(!e.consumed){e.ready=true;enemyOutcomeResult="ready";}return;}
    if(p.kind==EnemyOutcomeMessage::Ack){if(!e.sentKill||e.acknowledged){++enemyOutcomeRejected;return;}e.acknowledged=true;++enemyOutcomeAcknowledged;enemyOutcomeResult="acknowledged";return;}
    if(e.consumed){
        if(e.sentKill&&victim.current==0){++enemyOutcomeMerged;enemyOutcomeControl(EnemyOutcomeMessage::Ack,p.sourceRole);return;}
        ++enemyOutcomeRejected;return;
    }
    float partnerPosition[3]{};
    if(guardLink.phase!=GuardPhase::Active||!guardSameBinding(guardLink.key,e.key.binding)||victim.current!=40||
       !memoryRead(e.partner+0x300,partnerPosition,12)||!enemyNearVictim(partnerPosition)||!enemyNearVictim(remote.position)||
       !enemyConsume(e.ready,e.consumed,e.key,p)){++enemyOutcomeRejected;return;}
    enemyOutcomeBefore=victim.current;enemyReleaseGuard();
    if(!applyNativeAssassination(victim,e.partnerHandle,enemyOutcomeInflicted)){++enemyOutcomeRejected;enemyOutcomeResult="native_call_failed";return;}
    NativeHealth after{};
    if(!readNativeHealth(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),victim.handle,after)||after.actor!=victim.actor||after.component!=victim.component||after.current!=0||enemyOutcomeInflicted!=40){
        ++enemyOutcomeRejected;enemyOutcomeResult="readback_failed";return;
    }
    enemyOutcomeAfter=after.current;++enemyOutcomeApplied;enemyOutcomeResult="kill_applied";
    if(!enemyOutcomeControl(EnemyOutcomeMessage::Ack,p.sourceRole))enemyOutcomeResult="ack_failed";
}
void serviceEnemyOutcome(ULONGLONG tick){
    if(!enemyOutcome.until)return;NativeHealth victim{};
    if(!enemyOutcomeScope(tick,victim)){stopEnemyOutcome(tick>=enemyOutcome.until?"expired":"scope_changed");return;}
    auto& e=enemyOutcome;
    if(e.consumed)return;
    if(guardLink.phase!=GuardPhase::Active||!guardSameBinding(guardLink.key,e.key.binding)){stopEnemyOutcome("guard_changed");return;}
    if(e.generation!=damageObserveGeneration.load()||tick>=damageObserveUntil.load()){stopEnemyOutcome("capture_changed");return;}
    if(tick-e.lastSend>=200){e.lastSend=tick;if(!enemyOutcomeControl(EnemyOutcomeMessage::Arm)||
        (e.key.hostArm&&e.key.clientArm&&!enemyOutcomeControl(EnemyOutcomeMessage::Ready))){stopEnemyOutcome("send_failed");return;}}
    if(!e.ready)return;
    std::array<DamageObservation,128> copy{};unsigned count=0;
    AcquireSRWLockShared(&damageObserveLock);copy=damageObservations;count=damageObservationCount;ReleaseSRWLockShared(&damageObserveLock);
    for(;e.cursor<count;++e.cursor){const auto& hit=copy[e.cursor];
        if(hit.tick<e.born||tick-hit.tick>=250||hit.before.game!=e.victim.game||hit.before.actor!=e.victim.actor||hit.before.component!=e.victim.component||
           hit.before.current!=40||hit.before.maximum!=40||!hit.afterValid||hit.after.current!=0||hit.attackerHandle!=1||hit.damageAmount!=0||hit.flags!=0x8c||
           hit.inflictedDamage!=40||!(hit.resultFlags&1)||!enemyPlayerSword(hit.inflictorHandle))continue;
        std::uintptr_t decl=0;std::memcpy(&decl,hit.info.data(),8);if(!nativeAssassinationDeclaration(decl)||!enemyNearVictim(outgoing.position))continue;
        e.consumed=true;e.sentKill=true;++e.cursor;++enemyOutcomeCaptured;
        auto packet=e.key;packet.kind=EnemyOutcomeMessage::Kill;packet.sourceRole=localRole;packet.event=1;packet.declaration=coopMapId(kEnemyAssassinationDecl);enemyLastKill=packet;
        const bool dispatched=enemyOutcomeSend(packet);enemyReleaseGuard();enemyOutcomeResult=dispatched?"kill_sent":"send_failed";return;
    }
    if(victim.current==0)stopEnemyOutcome("unobserved_death");
}
void repeatEnemyOutcome(){
    NativeHealth victim{};
    if(!enemyOutcomeScope(GetTickCount64(),victim)||!enemyOutcome.sentKill||!enemySameBinding(enemyOutcome.key,enemyLastKill)){++enemyOutcomeRejected;return;}
    enemyOutcomeSend(enemyLastKill);
}
void writeEnemyOutcomeStatus(ULONGLONG tick){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.enemy-outcome.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;const auto& e=enemyOutcome;
    fprintf(f,"{\"armed\":%s,\"ready\":%s,\"consumed\":%s,\"sentKill\":%s,\"acknowledged\":%s,\"remainingMs\":%llu,\"hostArm\":%llu,\"clientArm\":%llu,\"binding\":%llu,\"result\":\"%s\",\"sent\":%llu,\"received\":%llu,\"rejected\":%llu,\"captured\":%llu,\"applied\":%llu,\"acks\":%llu,\"merged\":%llu,\"before\":%.9g,\"after\":%.9g,\"inflicted\":%.9g}\n",
        e.until?"true":"false",e.ready?"true":"false",e.consumed?"true":"false",e.sentKill?"true":"false",e.acknowledged?"true":"false",e.until>tick?e.until-tick:0,
        e.key.hostArm,e.key.clientArm,e.key.binding.binding,enemyOutcomeResult,enemyOutcomeSent,enemyOutcomeReceived,enemyOutcomeRejected,enemyOutcomeCaptured,enemyOutcomeApplied,enemyOutcomeAcknowledged,enemyOutcomeMerged,enemyOutcomeBefore,enemyOutcomeAfter,enemyOutcomeInflicted);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
