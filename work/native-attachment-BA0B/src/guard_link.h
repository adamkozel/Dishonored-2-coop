#pragma once
// One-guard, eight-second developer experiment. No observation packet can enter
// this mutation path. Both ends explicitly arm and negotiate fresh incarnations.
struct GuardLink {
    GuardPhase phase=GuardPhase::Idle;
    CoopGuardPacket key{},pose{};
    NativeWorldNpc npc{};
    NativeAiComponentState held[2]{};
    unsigned heldCount=0;
    std::uintptr_t game=0,frame=0;
    std::uint64_t epoch=0;
    ULONGLONG deadline=0,phaseTick=0,poseTick=0,lastSend=0;
};
GuardLink guardLink{};
std::uint64_t guardSequence=0,guardRequest=0,guardBinding=0,guardLastSequence=0,guardLastSession=0;
std::uint64_t guardSent=0,guardReceived=0,guardRejected=0,guardApplied=0,guardBindings=0,guardReleased=0,guardReleaseLost=0;
std::uint64_t guardHealthApplied=0,guardHealthStimuli=0,guardHealthRefused=0;
float guardHealthBefore=0,guardHealthAfter=0;
DWORD guardHealthThread=0;
const char* guardResult="idle";
CoopGuardPacket guardLastPose{};
ULONGLONG guardNudgeUntil=0;
ULONGLONG guardStreamBlockedUntil=0;
float guardNudgeOriginal[12]{};

bool guardNativeApiMatches() {
    const unsigned char transform[]={0x48,0x8b,0xc4,0x44,0x88,0x48,0x20,0x48,0x89,0x50,0x10,0x55,0x56,0x57,0x48,0x8d};
    const unsigned char pause[]={0x40,0x53,0x48,0x83,0xec,0x20,0x48,0x8b,0xd9,0xe8,0xf2,0x41,0x24,0x00,0x48,0x8b};
    const unsigned char resume[]={0x40,0x53,0x48,0x83,0xec,0x20,0x48,0x8b,0xd9,0xe8,0x32,0x42,0x24,0x00,0x48,0x8b};
    const unsigned char health[]={0x48,0x8b,0xc4,0x48,0x89,0x58,0x08,0x57,0x48,0x81,0xec,0xa0,0,0,0};
    return nativeAiRequestsMatch()&&!std::memcmp(gameImage+0x87d200,transform,sizeof(transform))&&
        !std::memcmp(gameImage+0x5a33b0,pause,sizeof(pause))&&!std::memcmp(gameImage+0x5a33d0,resume,sizeof(resume))&&
        !std::memcmp(gameImage+0x7185b0,health,sizeof(health));
}

bool guardPairReady(ULONGLONG tick) {
    return observedGame&&connectionState==k_ESteamNetworkingConnectionState_Connected&&lastReceive&&tick-lastReceive<1000&&
        coopWorldPairReady(outgoing,remote)&&!std::strcmp(mapName,kGuardMap);
}
void guardFillScope(CoopGuardPacket& p) {
    const auto& host=localRole?remote:outgoing;const auto& client=localRole?outgoing:remote;
    p.hostSession=host.session;p.clientSession=client.session;p.hostEpoch=host.worldEpoch;p.clientEpoch=client.worldEpoch;
    p.mapId=host.mapId;p.mode=guardLink.key.mode;p.entityKey=guardEntityKey(p.mode);
}
bool guardScopeCurrent() {
    CoopGuardPacket now{};guardFillScope(now);const auto& k=guardLink.key;
    return guardLink.game==observedGame&&guardLink.epoch==outgoing.worldEpoch&&k.hostSession==now.hostSession&&k.clientSession==now.clientSession&&
        k.hostEpoch==now.hostEpoch&&k.clientEpoch==now.clientEpoch&&k.mapId==now.mapId;
}
bool guardReadHealth(NativeHealth& health){
    return readNativeHealth(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),guardLink.npc.handle,health)&&
        health.game==guardLink.game&&health.actor==guardLink.npc.actor&&health.logic==1&&health.maximum==40&&health.current>=1&&health.current<=40;
}
bool guardSend(GuardMessage kind) {
    if(!sockets||!connection||connectionState!=k_ESteamNetworkingConnectionState_Connected)return false;
    auto p=guardLink.key;p.role=localRole;p.kind=kind;p.sequence=++guardSequence;
    if(kind==GuardMessage::Pose){
        NativeHealth health{};if(!guardReadHealth(health))return false;
        std::memcpy(p.position,guardLink.npc.position,12);std::memcpy(p.orientation,guardLink.npc.orientation,36);
        p.healthValid=1;p.health=health.current;p.maximumHealth=health.maximum;
    }else{p.healthValid=0;p.health=p.maximumHealth=0;}
    const auto mode=kind==GuardMessage::Pose?k_nSteamNetworkingSend_UnreliableNoNagle:k_nSteamNetworkingSend_ReliableNoNagle;
    if(sockets->SendMessageToConnection(connection,&p,sizeof(p),mode,nullptr)!=k_EResultOK)return false;
    ++guardSent;if(kind==GuardMessage::Pose)guardLastPose=p;return true;
}
bool guardResolve(NativeWorldNpc& result,unsigned mode=0) {
    std::vector<NativeWorldNpc> catalog;unsigned unbound=0;
    if(!readWorldNpcCatalog(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,catalog,unbound))return false;
    unsigned matches=0;
    for(const auto& n:catalog)if(n.spawnerName==guardSpawner(mode)) {
        if(!n.unique||!n.reciprocal||n.definition!=guardDefinition(mode))return false;
        result=n;++matches;
    }
    return matches==1;
}
bool guardReadLocal() {
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    NativeWorldNpc n{};std::uintptr_t frame=0,vt=0,owner=0,set=0;
    if(!guardScopeCurrent()||!readWorldNpcIdentity(memoryRead,base,guardLink.game,guardLink.npc.handle,n)||!n.unique||
       n.actor!=guardLink.npc.actor||n.spawnerHandle!=guardLink.npc.spawnerHandle||n.spawnerName!=guardSpawner(guardLink.key.mode)||n.definition!=guardDefinition(guardLink.key.mode)||
       !memoryRead(n.actor+0x358,&frame,8)||!frame||!memoryRead(frame,&vt,8)||vt!=base+0x1c573e8||
       !memoryRead(frame+0x10,&owner,8)||owner!=n.actor||!memoryRead(vt+22*8,&set,8)||set!=base+0x751d00||
       (guardLink.frame&&frame!=guardLink.frame))return false;
    guardLink.npc=std::move(n);guardLink.frame=frame;return true;
}
bool guardAttackInactive() {
    NativeAiComponentState attack{};
    return readNativeAiComponent(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),guardLink.game,guardLink.npc.handle,NativeAiKind::Attack,attack)&&
        (attack.flags&0xc)==0&&!attack.schedulerActive;
}
void guardSetTransform(const float* position,const float* orientation) {
    reinterpret_cast<void(*)(void*,const float*,const float*,bool,int)>(gameImage+0x87d200)(
        reinterpret_cast<void*>(guardLink.npc.actor),position,orientation,true,0);
}
void restoreGuardNudge() {
    if(!guardNudgeUntil)return;guardNudgeUntil=0;
    if(!localRole&&guardReadLocal())guardSetTransform(guardNudgeOriginal,guardNudgeOriginal+3);
}
void stopGuardLink(const char* reason,bool notifyPeer=true) {
    if(guardLink.phase==GuardPhase::Idle)return;
    stopGuardEquipment(reason);
    stopGuardWeapon(reason);
    guardFrameUntil.store(0,std::memory_order_release);guardControlledFrame.store(nullptr,std::memory_order_release);
    stopGuardBody();
    if(notifyPeer&&guardLink.key.binding)guardSend(GuardMessage::Stop);
    restoreGuardNudge();
    for(unsigned i=guardLink.heldCount;i>0;--i) {
        if(releaseNativeAiReference(guardLink.held[i-1],guardLink.epoch))++guardReleased;else ++guardReleaseLost;
    }
    guardAiOwned=false;guardStreamBlockedUntil=0;guardLink={};guardResult=reason;
    consoleOutput("COOP: guard link stopped reason=%s released=%llu lost=%llu\n",reason,guardReleased,guardReleaseLost);
}
bool guardAcquireAi() {
    if(aiDecisionHold.expires||guardAiOwned||!nativeAiRequestsMatch()||!guardReadLocal()||!guardAttackInactive())return false;
    const NativeAiKind kinds[]={NativeAiKind::Decision,NativeAiKind::Reflex};
    for(unsigned i=0;i<2;++i) {
        auto& s=guardLink.held[i];
        if(!readNativeAiComponent(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),guardLink.game,guardLink.npc.handle,kinds[i],s)||
           (s.flags&0xc)!=8||(s.flags&0x7fffff0)||!s.schedulerActive)return false;
    }
    guardAiOwned=true;
    for(unsigned i=0;i<2;++i) {
        const auto& s=guardLink.held[i];
        reinterpret_cast<void(*)(void*,void*)>(gameImage+0x7e7800)(reinterpret_cast<void*>(s.component),reinterpret_cast<void*>(s.manager));
        ++guardLink.heldCount;
        std::uint32_t flags=0;if(!memoryRead(s.component+0x1c,&flags,4)||((flags>>6)&0x1fffff)!=1)return false;
    }
    return true;
}
bool guardAiPaused() {
    for(unsigned i=0;i<2;++i) {
        NativeAiComponentState s{};const auto& held=guardLink.held[i];
        if(!readNativeAiComponent(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),guardLink.game,guardLink.npc.handle,held.kind,s)||
           s.component!=held.component||s.manager!=held.manager||(s.flags&0xc)!=8||((s.flags>>6)&0x1fffff)<1||
           ((s.flags>>4)&3)!=2||s.schedulerActive)return false;
    }
    return true;
}
bool guardApplyHealth(float wanted,bool stimulus){
    const auto tick=GetTickCount64();NativeHealth before{},after{};
    if(GetCurrentThreadId()!=engineThread.load()||guardLink.phase!=GuardPhase::Active||tick>=guardLink.deadline||
       !guardPairReady(tick)||!guardReadLocal()||!guardAttackInactive()||!guardNativeApiMatches()||!guardReadHealth(before)||
       !std::isfinite(wanted)||wanted<1||wanted>before.current||
       (stimulus?(localRole||wanted!=35||before.current!=40):(!localRole||!guardAiPaused()||tick-guardLink.poseTick>=250))){++guardHealthRefused;return false;}
    if(wanted==before.current)return true;
    // Native setter clamps and emits healthChanged. This does not replay a
    // hit, attribute an attacker, invoke lethal damage, or synthesize death.
    reinterpret_cast<void(*)(void*,float)>(gameImage+0x7185b0)(reinterpret_cast<void*>(before.component),wanted);
    if(!guardReadHealth(after)||after.component!=before.component||after.current!=wanted){++guardHealthRefused;return false;}
    guardHealthBefore=before.current;guardHealthAfter=after.current;guardHealthThread=GetCurrentThreadId();
    if(stimulus)++guardHealthStimuli;else ++guardHealthApplied;
    return true;
}
void beginGuardHealthStimulus(){if(!guardApplyHealth(35,true)&&guardLink.phase==GuardPhase::Active)guardResult="health_stimulus_refused";}
void beginGuardLink(ULONGLONG tick,bool combat=false) {
    if(guardLink.phase!=GuardPhase::Idle){guardResult="already_armed";return;}
    if(aiDecisionHold.expires||!guardPairReady(tick)||!guardNativeApiMatches()){guardResult="unsupported_scope";return;}
    NativeWorldNpc npc{};if(!guardResolve(npc,combat?1:0)){guardResult="guard_unavailable";return;}
    guardLink.game=observedGame;guardLink.epoch=outgoing.worldEpoch;guardLink.npc=std::move(npc);
    guardLink.key.mode=combat?1:0;guardLink.deadline=tick+(combat?60000:8000);guardLink.phaseTick=tick;guardFillScope(guardLink.key);
    guardLink.phase=GuardPhase::Armed;guardResult="armed";
    NativeHealth health{};
    if(!guardReadLocal()||!guardAttackInactive()||!guardReadHealth(health)){stopGuardLink("unsupported_guard");return;}
    if(localRole) {
        guardLink.key.request=++guardRequest;guardLink.key.clientHandle=guardLink.npc.handle;
        guardLink.phase=GuardPhase::Requested;
        if(!guardSend(GuardMessage::Request))stopGuardLink("send_failed");
    }
}
void receiveGuardPacket(const CoopGuardPacket& p,ULONGLONG tick) {
    ++guardReceived;
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load()||!guardPairReady(tick)||
       !validCoopGuard(p,outgoing,remote)||p.mode!=guardLink.key.mode||!guardPhaseAccepts(guardLink.phase,p.kind,localRole)||!guardScopeCurrent()||
       tick>=guardLink.deadline||(guardLastSession==remote.session&&p.sequence<=guardLastSequence)){++guardRejected;return;}
    if(p.kind==GuardMessage::Request) {
        if(!guardReadLocal()){stopGuardLink("identity_changed");return;}
        guardLink.key=p;guardLink.key.hostHandle=guardLink.npc.handle;guardLink.key.binding=++guardBinding;
        guardLink.phase=GuardPhase::Offered;
        if(!guardSend(GuardMessage::Offer))stopGuardLink("send_failed");
    }else if(p.kind==GuardMessage::Offer) {
        if(p.request!=guardLink.key.request||p.clientHandle!=guardLink.npc.handle){++guardRejected;return;}
        guardLink.key=p;guardLink.phase=GuardPhase::Accepted;
        if(!guardSend(GuardMessage::Accept))stopGuardLink("send_failed");
    }else {
        if(!guardSameBinding(p,guardLink.key)){++guardRejected;return;}
        if(p.kind==GuardMessage::Accept) {
            guardLink.phase=GuardPhase::Committed;
            if(!guardSend(GuardMessage::Commit))stopGuardLink("send_failed");
        }else if(p.kind==GuardMessage::Commit) {
            if(!guardAcquireAi()){stopGuardLink("ai_acquire_failed");return;}
            guardLink.phase=GuardPhase::Pausing;
        }else if(p.kind==GuardMessage::Ready) {guardLink.phase=GuardPhase::Active;++guardBindings;guardResult="active";}
        else if(p.kind==GuardMessage::Pose) {guardLink.pose=p;guardLink.poseTick=tick;guardLastPose=p;}
        else if(p.kind==GuardMessage::Stop) {stopGuardLink("peer_stopped",false);return;}
    }
    guardLink.phaseTick=tick;guardLastSession=remote.session;guardLastSequence=p.sequence;
}
void beginGuardNudge(ULONGLONG tick) {
    if(localRole||guardLink.key.mode||guardLink.phase!=GuardPhase::Active||guardNudgeUntil||!guardReadLocal())return;
    std::memcpy(guardNudgeOriginal,guardLink.npc.position,12);std::memcpy(guardNudgeOriginal+3,guardLink.npc.orientation,36);
    float position[3];std::memcpy(position,guardNudgeOriginal,12);position[0]+=0.35f;
    guardNudgeUntil=tick+1000;guardSetTransform(position,guardLink.npc.orientation);
}
void serviceGuardLink(ULONGLONG tick) {
    if(guardLink.phase==GuardPhase::Idle)return;
    if(guardNudgeUntil&&tick>=guardNudgeUntil)restoreGuardNudge();
    if(tick>=guardLink.deadline){stopGuardLink("expired");return;}
    if(!guardPairReady(tick)||!guardReadLocal()){stopGuardLink("scope_changed");return;}
    // Only the explicitly selected host combat guard may start an attack.
    // Its client replica must remain unable to run an independent attack.
    if((localRole||!guardLink.key.mode)&&!guardAttackInactive()){stopGuardLink("combat_activated");return;}
    if(guardLink.phase!=GuardPhase::Active&&tick-guardLink.phaseTick>=1500){stopGuardLink("handshake_timeout");return;}
    if(guardLink.phase==GuardPhase::Pausing) {
        if(guardAiPaused()) {
            guardLink.phase=GuardPhase::Active;guardLink.poseTick=tick;guardResult="active";++guardBindings;
            if(!guardSend(GuardMessage::Ready))stopGuardLink("send_failed");
        }else if(tick-guardLink.phaseTick>=250)stopGuardLink("ai_pause_timeout");
        return;
    }
    if(guardLink.phase!=GuardPhase::Active)return;
    if(!localRole) {
        if(tick<guardStreamBlockedUntil)return;
        if(tick-guardLink.lastSend>=33){guardLink.lastSend=tick;if(!guardSend(GuardMessage::Pose))stopGuardLink("send_failed");else {sendGuardBody(tick);sendGuardWeapon(tick);}}
        return;
    }
    if(tick-guardLink.poseTick>=250){stopGuardLink("pose_stale");return;}
    if(!guardAiPaused()){stopGuardLink("ai_control_changed");return;}
    if(!guardLink.pose.sequence)return;
    if(!guardApplyHealth(guardLink.pose.health,false)){stopGuardLink("health_scope_changed");return;}
    guardControlledFrame.store(reinterpret_cast<void*>(guardLink.frame),std::memory_order_release);
    guardFrameUntil.store((std::min)(tick+200,guardLink.poseTick+250),std::memory_order_release);
    guardSetTransform(guardLink.pose.position,guardLink.pose.orientation);++guardApplied;
}
void writeGuardStatus(ULONGLONG tick) {
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.guard.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    const auto& p=guardLastPose;const auto& k=guardLink.key;
    fprintf(f,"{\"role\":%u,\"phase\":%u,\"result\":\"%s\",\"remainingMs\":%llu,\"binding\":%llu,\"request\":%llu,\"hostHandle\":%u,\"clientHandle\":%u,\"sent\":%llu,\"received\":%llu,\"rejected\":%llu,\"bindings\":%llu,\"applied\":%llu,\"heldReferences\":%u,\"released\":%llu,\"releaseLost\":%llu,\"poseSequence\":%llu,\"poseBinding\":%llu,\"poseAgeMs\":%llu,\"pose\":[",
        localRole,static_cast<unsigned>(guardLink.phase),guardResult,guardLink.deadline>tick?guardLink.deadline-tick:0,k.binding,k.request,k.hostHandle,k.clientHandle,
        guardSent,guardReceived,guardRejected,guardBindings,guardApplied,guardLink.heldCount,guardReleased,guardReleaseLost,p.sequence,p.binding,guardLink.poseTick?tick-guardLink.poseTick:0);
    for(unsigned i=0;i<3;++i)fprintf(f,"%s%.9g",i?",":"",p.position[i]);
    for(unsigned i=0;i<9;++i)fprintf(f,",%.9g",p.orientation[i]);
    fprintf(f,"],\"frameOwned\":%s,\"nudgeActive\":%s,\"protocolVersion\":3,\"mode\":%u,\"packetBytes\":164,\"healthApplied\":%llu,\"healthStimuli\":%llu,\"healthRefused\":%llu,\"healthBefore\":%.9g,\"healthAfter\":%.9g,\"healthThread\":%lu,\"poseHealth\":%.9g,\"poseMaximumHealth\":%.9g,\"childrenUpdated\":%llu,\"childrenRefused\":%llu}\n",
        guardFrameUntil.load()>tick?"true":"false",guardNudgeUntil?"true":"false",guardLink.key.mode,guardHealthApplied,guardHealthStimuli,guardHealthRefused,guardHealthBefore,guardHealthAfter,guardHealthThread,p.health,p.maximumHealth,guardChildrenUpdated.load(),guardChildrenRefused.load());
    const bool ok=!ferror(f);const int close=fclose(f);if(ok&&!close)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}

