#pragma once
// Host-authored root and final skeleton stream. This carries presentation and
// native AI ownership, not client hit requests or authoritative damage events.
struct NpcRenderIdentity {
    NativeNpcAuthority native{};
    std::uintptr_t animation=0,render=0,model=0,skeleton=0;
    unsigned animationGeneration=0,joints=0,slot=0;
    bool corpse=false; // 1113: shared-death corpse presentation (health0, AI not paused)
};
struct NpcRenderState {NpcRenderIdentity id{};ULONGLONG until=0;std::uint64_t arm=0,sequence=0;CoopNpcPose pose{};};
SRWLOCK npcPresentationLock=SRWLOCK_INIT;
std::vector<NpcRenderState> npcPresentation;
std::atomic<std::uint64_t> npcBodiesApplied{0},npcBodiesRefused{0},npcFramesOwned{0},npcChildUpdates{0};
std::array<std::atomic<std::uint64_t>,128> npcActorBodyApplied{},npcActorBodySequence{};
unsigned npcPoseIntervalMs=50;
bool npcRenderCurrent(const NpcRenderIdentity& id){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);const auto& n=id.native;std::uintptr_t p=0;unsigned u=0;
    const auto eq=[&](std::uintptr_t a,std::uintptr_t expected){return memoryRead(a,&p,8)&&p==expected;};
    if(!eq(base+0x39a5290,n.health.game)||!eq(n.health.game,base+0x1c940a8)||
       !memoryRead(n.health.game+0x341058,&u,4)||u!=3||!memoryRead(n.health.game+0x1f7400,&u,4)||u||
       !eq(n.health.game+0x1f73f0,n.health.logicStack)||!memoryRead(n.health.game+0x1f73f8,&u,4)||u!=n.health.logicCount||
       !memoryRead(n.health.logicStack+((u&0xffffff)-1)*4,&u,4)||u!=1||
       worldIdentityEntity(memoryRead,n.health.game,n.npc.handle)!=n.npc.actor||!eq(n.npc.actor+0x358,n.frame)||
       !eq(n.frame+0x10,n.npc.actor)||!memoryRead(n.frame+0x18,&u,4)||u!=n.frameGeneration||
       !eq(id.animation,base+0x1b80408)||!eq(id.animation+0x10,n.npc.actor)||
       !memoryRead(id.animation+0x18,&u,4)||u!=id.animationGeneration||!eq(id.animation+0x38,id.render)||
       !eq(id.render+0x1fe8,id.model)||!eq(id.model+0x50,id.skeleton)||!eq(id.animation+0x80,id.skeleton)||
       !memoryRead(id.animation+0x190,&u,4)||(u&0xffffff)!=id.joints)return false;
    float health=0;
    if(!eq(n.health.component+0x10,n.npc.actor)||!memoryRead(n.health.component+0x18,&u,4)||u!=n.healthGeneration||
       !memoryRead(n.health.component+0x20,&health,4)||!std::isfinite(health)||(id.corpse?health!=0:health<=0))return false;
    if(id.corpse)return true; // death already applied natively; the corpse AI is not paused-owned
    for(unsigned i=0;i<3;++i){const auto& s=n.ai[i];
        if(!eq(s.component,base+nativeAiVtable(s.kind))||!eq(s.component+0x10,n.npc.actor)||
           !memoryRead(s.component+0x18,&u,4)||u!=n.aiGeneration[i]||!memoryRead(s.component+0x1c,&u,4))return false;
        if(i<2){if((u&12)!=8||((u>>4)&3)!=2||!((u>>6)&0x1fffff))return false;}
        else if(u&12)return false;
    }return true;
}
void clearNpcPresentation(std::uint64_t arm=0){
    AcquireSRWLockExclusive(&npcPresentationLock);
    if(!arm)npcPresentation.clear();else npcPresentation.erase(std::remove_if(npcPresentation.begin(),npcPresentation.end(),[&](const auto& p){return p.arm==arm;}),npcPresentation.end());
    ReleaseSRWLockExclusive(&npcPresentationLock);
}
bool npcCombatFixtureOwnsFrame(void* frame);
bool handleNpcReferenceFrame(void* frame){
    bool owned=false;AcquireSRWLockShared(&npcPresentationLock);
    for(const auto& p:npcPresentation)if(p.id.native.frame==reinterpret_cast<std::uintptr_t>(frame)&&GetTickCount64()<p.until){owned=npcRenderCurrent(p.id);break;}
    ReleaseSRWLockShared(&npcPresentationLock);if(!owned)owned=npcCombatFixtureOwnsFrame(frame);if(!owned)return false;
    ++npcFramesOwned;NativeChildFrames children{};
    if(readNativeChildFrames(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),reinterpret_cast<std::uintptr_t>(frame),children))
        for(unsigned i=0;i<children.count;++i){realReferenceFrameUpdate(reinterpret_cast<void*>(children.children[i].frame));++npcChildUpdates;}
    return true;
}
void applyNpcAuthorityBody(void* component,void* actor){
    AcquireSRWLockShared(&npcPresentationLock);
    for(const auto& p:npcPresentation){if(p.id.animation!=reinterpret_cast<std::uintptr_t>(component)||p.id.native.npc.actor!=reinterpret_cast<std::uintptr_t>(actor)||GetTickCount64()>=p.until)continue;
        if(npcRenderCurrent(p.id)){
            for(unsigned j=0;j<p.id.joints;++j){const auto matrix=npcWireMatrix(p.pose.local[j]);
                reinterpret_cast<void(*)(void*,std::uint16_t,const BodyMatrix*)>(gameImage+0x9cfa0)(reinterpret_cast<void*>(p.id.animation+0x40),static_cast<std::uint16_t>(j),&matrix);}
            reinterpret_cast<void(*)(void*)>(gameImage+0x9dc40)(reinterpret_cast<void*>(p.id.animation+0x40));++npcBodiesApplied;
            if(p.id.slot<npcActorBodyApplied.size()){++npcActorBodyApplied[p.id.slot];npcActorBodySequence[p.id.slot].store(p.sequence);}
        }else ++npcBodiesRefused;break;
    }ReleaseSRWLockShared(&npcPresentationLock);
}
struct NpcAuthorityChannel {
    NpcAiLease lease{};NpcRenderIdentity render{};NpcPoseBuffer buffer{};
    std::uint64_t arm=0,peerArm=0,rig=0,peerSequence=0,sequence=0;
    unsigned peerHandle=0;
    ULONGLONG started=0,helloAt=0,readyAt=0,peerAt=0,poseAt=0,lastSend=0,lastCheck=0;
    unsigned lifecycleDiff=0;
    // 1113 corpse authority: after a shared death the host streams the corpse for
    // a bounded settle window; the client holds its corpse animated meanwhile.
    bool corpse=false;ULONGLONG corpseUntil=0;std::uintptr_t corpsePhysics=0;
    bool enabled=true,ready=false;
    const char* result="negotiating";
    std::uint64_t sent=0,received=0,rejected=0,poses=0,roots=0;
    bool poseStale=false;std::uint64_t poseGaps=0,poseRecoveries=0,poseFailures=0;
    const char* poseFailure="none";
    int poseFailureJoint=-1;float poseFailureMatrix[12]{},poseFailureRoot[12]{};
};
std::vector<NpcAuthorityChannel> npcChannels;
std::uint64_t npcArmCounter=0,npcStreamSession=0,npcStreamPeerSession=0,npcStreamEpoch=0,npcStreamPeerEpoch=0,npcStreamMap=0,npcPacketsRejected=0;
std::uint64_t npcDecodeRejected=0,npcPoseScopeRejected=0;
unsigned npcStreamIneligible=0,npcStreamUnavailable=0,npcStreamUnbound=0;
const char* npcStreamResult="disabled";
int npcStreamRequest=-1;
struct NpcStreamSession {
    DoorSessionScope scope{};
    bool enabled=false,reloadPending=false,reloadReady=false,menuSuspended=false;
    ULONGLONG deadline=0,menuSince=0;
    std::uint64_t reloads=0,rearms=0,menuSuspensions=0,menuResumes=0,menuPausedMs=0,ignored=0;
    std::uint64_t acquired=0,released=0,retired=0,lost=0;
    const char* result="disabled";
};
NpcStreamSession npcStreamSessionState{};
bool doorCheckpointBarrierActive();
bool npcStreamContext(ULONGLONG tick){
    const auto& s=npcStreamSessionState;
    return npcAuthorityStreamActive&&s.enabled&&!s.reloadPending&&!s.menuSuspended&&!doorCheckpointBarrierActive()&&nativeMutationAllowed&&GetCurrentThreadId()==engineThread.load()&&coopNpcPairReady(tick)&&
        outgoing.session==npcStreamSession&&remote.session==npcStreamPeerSession&&outgoing.worldEpoch==npcStreamEpoch&&
        remote.worldEpoch==npcStreamPeerEpoch&&outgoing.mapId==npcStreamMap;
}
NpcAuthorityHeader npcHeader(const NpcAuthorityChannel& c,NpcAuthorityMessage kind){
    NpcAuthorityHeader h{};h.role=localRole;h.kind=kind;h.sequence=c.sequence+1;h.sourceSession=npcStreamSession;h.targetSession=npcStreamPeerSession;
    h.sourceEpoch=npcStreamEpoch;h.targetEpoch=npcStreamPeerEpoch;h.map=npcStreamMap;h.sourceArm=c.arm;h.sourceHandle=c.lease.binding.npc.handle;
    h.rig=c.rig;h.joints=c.render.joints;h.size=static_cast<unsigned short>(kind==NpcAuthorityMessage::Pose?npcPoseBytes(h.joints):sizeof(CoopNpcControl));
    if(kind!=NpcAuthorityMessage::Hello){h.targetArm=c.peerArm;h.targetHandle=c.peerHandle;}return h;
}
bool sendNpcControl(NpcAuthorityChannel& c,NpcAuthorityMessage kind){
    if(!sockets||!connection||connectionState!=k_ESteamNetworkingConnectionState_Connected)return false;
    CoopNpcControl p{};p.h=npcHeader(c,kind);const auto& n=c.lease.binding.npc;
    std::memcpy(p.spawner,n.spawnerName.c_str(),n.spawnerName.size());std::memcpy(p.definition,n.definition.c_str(),n.definition.size());
    if(!validNpcControl(p,remote,outgoing))return false;
    ++c.sequence;
    if(coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)!=k_EResultOK)return false;
    ++c.sent;return true;
}
void stopNpcChannel(NpcAuthorityChannel& c,const char* reason,bool notify=true){
    if(!c.enabled)return;clearNpcPresentation(c.arm);
    if(c.corpsePhysics){std::uintptr_t vt=0;
        if(memoryRead(c.corpsePhysics,&vt,8)&&vt==reinterpret_cast<std::uintptr_t>(gameImage)+0x1c54008){
            reinterpret_cast<void(*)(void*)>(gameImage+0x734d90)(reinterpret_cast<void*>(c.corpsePhysics));
            reinterpret_cast<void(*)(void*)>(gameImage+0x734a10)(reinterpret_cast<void*>(c.corpsePhysics));}
        c.corpsePhysics=0;}
    if(notify&&c.peerArm)sendNpcControl(c,NpcAuthorityMessage::Stop);
    releaseNpcAiLease(c.lease);
    auto& s=npcStreamSessionState;s.released+=c.lease.released;s.retired+=c.lease.retired;s.lost+=c.lease.lost;
    c.enabled=c.ready=false;c.result=reason;c.buffer={};
}
bool npcCorpseHoldAnimated(NpcAuthorityChannel& c){
    if(c.corpsePhysics)return true;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t list=0;unsigned flags=0,mode=99,pending=0;std::uintptr_t physics=0;
    if(!memoryRead(c.lease.binding.npc.actor+0x268,&list,8)||!list||!memoryRead(c.lease.binding.npc.actor+0x270,&flags,4)||(flags&0xffffff)>256)return false;
    for(unsigned i=0;i<(flags&0xffffff);++i){std::uintptr_t p=0,vt=0;if(!memoryRead(list+i*8,&p,8)||!p||!memoryRead(p,&vt,8))return false;if(vt==base+0x1c54008){if(physics)return false;physics=p;}}
    if(!physics||!memoryRead(physics+0x238,&mode,4)||mode!=0||!memoryRead(physics+0x23c,&pending,4)||pending!=0xffffffff)return false;
    reinterpret_cast<void(*)(void*)>(gameImage+0x736ef0)(reinterpret_cast<void*>(physics));c.corpsePhysics=physics;return true;
}
void stopNpcCombatFixture(const char* reason);
void clearNpcCombatCapture();
void stopNpcAttack(const char* reason);
void stopNpcStream(const char* reason){
    stopNpcAttack(reason);
    clearNpcCombatCapture();
    stopNpcCombatFixture(reason);
    clearNpcPresentation();for(auto& c:npcChannels)stopNpcChannel(c,reason);npcAuthorityStreamActive=false;npcStreamResult=reason;
}
void disableNpcStreamSession(const char* reason){
    stopNpcStream(reason);auto& s=npcStreamSessionState;s.enabled=s.reloadPending=s.reloadReady=s.menuSuspended=false;s.menuSince=0;s.result=reason;
}
void beginNpcCheckpointRecovery(){
    auto& s=npcStreamSessionState;if(!s.enabled||s.reloadPending)return;
    if(outgoing.session!=s.scope.session||remote.session!=s.scope.peerSession||outgoing.mapId!=s.scope.map||outgoing.worldEpoch!=s.scope.epoch){disableNpcStreamSession("checkpoint_scope_changed");return;}
    s.reloadPending=true;s.reloadReady=s.menuSuspended=false;s.menuSince=0;s.deadline=GetTickCount64()+60000;++s.reloads;s.result="waiting_shared_checkpoint";
    stopNpcStream("shared_checkpoint");
}
void finishNpcCheckpointRecovery(){
    auto& s=npcStreamSessionState;if(s.enabled&&s.reloadPending&&doorSessionRenewalReady(s.scope,outgoing,remote))s.reloadReady=true;
}
void beforeNpcNativeLoad(){
    // Called before the original native command destroys actor components.
    if(npcStreamSessionState.reloadPending)stopNpcStream("native_checkpoint_command");
    else disableNpcStreamSession("uncoordinated_native_load");
}
// 1166 dynamic relevance. The host owns the bound set: every 2 s it binds the
// nearest eligible NPCs within 55 m of EITHER player (at most 24 bound) and
// retires channels whose NPC is beyond 75 m of both players (Stop to the
// client). The client binds only on a host Hello, so both sets are the host's.
constexpr float kNpcRelevanceAdd=55.f,kNpcRelevanceDrop=75.f;constexpr unsigned kNpcRelevanceMax=24;
ULONGLONG npcRelevanceAt=0;std::uint64_t npcRelevanceScans=0,npcRelevanceAdded=0,npcRelevanceDropped=0,npcClientBound=0,npcClientBindRefused=0;
float npcNearestPlayerSq(const float* p){
    float best=1e30f;
    for(const auto* c:{outgoing.position,remote.position}){float d=0;for(unsigned k=0;k<3;++k){const float v=p[k]-c[k];d+=v*v;}best=(std::min)(best,d);}
    return best;
}
unsigned npcEnabledChannels(){unsigned n=0;for(const auto& c:npcChannels)if(c.enabled)++n;return n;}
bool bindNpcChannel(const NativeNpcAuthority& n,ULONGLONG tick){
    NativeSkeleton s{};unsigned generation=0;
    if(!readNativeSkeleton(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),n.npc.handle,s)||s.game!=n.health.game||s.actor!=n.npc.actor||
       !memoryRead(s.animation+0x18,&generation,4)||generation==~0u){++npcStreamUnavailable;return false;}
    const auto rig=npcAuthorityRig(s);if(!rig){++npcStreamUnavailable;return false;}
    // Reuse a retired slot (per-slot body counters are 128 wide).
    unsigned slot=static_cast<unsigned>(npcChannels.size());
    for(unsigned i=0;i<npcChannels.size();++i)if(!npcChannels[i].enabled){slot=i;break;}
    if(slot>=npcActorBodyApplied.size())return false;
    NpcAuthorityChannel c{};c.lease.binding=n;c.lease.epoch=outgoing.worldEpoch;c.arm=++npcArmCounter;c.rig=rig;c.started=tick;
    c.render={n,s.animation,s.render,s.model,s.skeleton,generation,static_cast<unsigned>(s.parents.size()),slot};
    npcActorBodyApplied[slot].store(0);npcActorBodySequence[slot].store(0);
    if(slot==npcChannels.size())npcChannels.push_back(std::move(c));else npcChannels[slot]=std::move(c);
    return true;
}
void refreshNpcRelevance(ULONGLONG tick){
    if(localRole||tick-npcRelevanceAt<2000)return;npcRelevanceAt=tick;++npcRelevanceScans;
    // Retire channels whose NPC is far from both players (corpses included).
    for(auto& c:npcChannels){if(!c.enabled)continue;NativeNpcAuthority fresh{};
        if(!readNativeNpcAuthority(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,c.lease.binding.npc.handle,fresh))continue;
        if(npcNearestPlayerSq(fresh.npc.position)>kNpcRelevanceDrop*kNpcRelevanceDrop){stopNpcChannel(c,"relevance_left");++npcRelevanceDropped;}}
    unsigned bound=npcEnabledChannels();if(bound>=kNpcRelevanceMax)return;
    std::vector<NativeNpcAuthority> catalog;
    if(!readNativeNpcAuthorityCatalog(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,catalog,npcStreamIneligible,npcStreamUnavailable,npcStreamUnbound))return;
    catalog.erase(std::remove_if(catalog.begin(),catalog.end(),[&](const auto& n){
        if(npcNearestPlayerSq(n.npc.position)>kNpcRelevanceAdd*kNpcRelevanceAdd)return true;
        return std::any_of(npcChannels.begin(),npcChannels.end(),[&](const auto& c){return c.enabled&&(c.lease.binding.npc.handle==n.npc.handle||
            (c.lease.binding.npc.spawnerName==n.npc.spawnerName&&c.lease.binding.npc.definition==n.npc.definition));});}),catalog.end());
    std::stable_sort(catalog.begin(),catalog.end(),[&](const auto& a,const auto& b){return npcNearestPlayerSq(a.npc.position)<npcNearestPlayerSq(b.npc.position);});
    for(const auto& n:catalog){if(bound>=kNpcRelevanceMax)break;if(bindNpcChannel(n,tick)){++bound;++npcRelevanceAdded;}}
}
// Client: bind the NPC named by a host Hello from the current eligible catalog.
bool bindNpcForHostHello(const CoopNpcControl& p,ULONGLONG tick){
    if(!localRole||p.h.kind!=NpcAuthorityMessage::Hello||npcEnabledChannels()>=kNpcRelevanceMax)return false;
    std::vector<NativeNpcAuthority> catalog;
    if(!readNativeNpcAuthorityCatalog(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,catalog,npcStreamIneligible,npcStreamUnavailable,npcStreamUnbound)){++npcClientBindRefused;return false;}
    for(const auto& n:catalog)if(n.npc.spawnerName==p.spawner&&n.npc.definition==p.definition){
        if(bindNpcChannel(n,tick)){++npcClientBound;return true;}break;}
    ++npcClientBindRefused;return false;
}
void beginNpcStream(ULONGLONG tick){
    if(npcAuthorityStreamActive)return;
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load()||!coopNpcPairReady(tick)||!guardNativeApiMatches()||!bodyNativeFunctions()||
       npcAuthorityHoldActive||observerIsolation.until||aiDecisionHold.expires||guardLink.phase!=GuardPhase::Idle||guardAiOwned){npcStreamResult="context_or_existing_owner_refused";return;}
    std::vector<NativeNpcAuthority> catalog;
    if(!readNativeNpcAuthorityCatalog(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,catalog,npcStreamIneligible,npcStreamUnavailable,npcStreamUnbound)){
        npcStreamResult="no_eligible_catalog";return;}
    // 1091/1166: the host binds the nearest eligible NPCs within 55 m of either
    // player (at most 24); the client starts empty and binds on host Hello.
    // An empty set is valid: relevance adds NPCs as players move.
    if(localRole)catalog.clear();
    else{catalog.erase(std::remove_if(catalog.begin(),catalog.end(),[&](const auto& n){return npcNearestPlayerSq(n.npc.position)>kNpcRelevanceAdd*kNpcRelevanceAdd;}),catalog.end());
     std::stable_sort(catalog.begin(),catalog.end(),[&](const auto& a,const auto& b){return npcNearestPlayerSq(a.npc.position)<npcNearestPlayerSq(b.npc.position);});
     if(catalog.size()>kNpcRelevanceMax)catalog.resize(kNpcRelevanceMax);}
    clearNpcPresentation();npcChannels.clear();for(auto& c:npcActorBodyApplied)c.store(0);for(auto& c:npcActorBodySequence)c.store(0);
    for(auto& n:catalog)bindNpcChannel(n,GetTickCount64());
    npcRelevanceAt=GetTickCount64();
    npcStreamSession=outgoing.session;npcStreamPeerSession=remote.session;npcStreamEpoch=outgoing.worldEpoch;npcStreamPeerEpoch=remote.worldEpoch;npcStreamMap=outgoing.mapId;
    npcAuthorityStreamActive=true;npcStreamResult="negotiating";
}
void receiveNpcControl(const CoopNpcControl& p,ULONGLONG tick){
    if(npcStreamSessionState.menuSuspended||npcStreamSessionState.reloadPending){++npcStreamSessionState.ignored;return;}
    if(!npcStreamContext(tick)||!validNpcControl(p,outgoing,remote)){++npcPacketsRejected;return;}
    auto it=std::find_if(npcChannels.begin(),npcChannels.end(),[&](const auto& c){return c.enabled&&c.lease.binding.npc.spawnerName==p.spawner&&c.lease.binding.npc.definition==p.definition;});
    if(it==npcChannels.end()&&bindNpcForHostHello(p,tick))
        it=std::find_if(npcChannels.begin(),npcChannels.end(),[&](const auto& c){return c.enabled&&c.lease.binding.npc.spawnerName==p.spawner&&c.lease.binding.npc.definition==p.definition;});
    if(it==npcChannels.end()){++npcPacketsRejected;return;}auto& c=*it;++c.received;
    if(p.h.rig!=c.rig||p.h.joints!=c.render.joints||p.h.sequence<=c.peerSequence){++c.rejected;return;}
    if(p.h.kind==NpcAuthorityMessage::Hello){
        if(c.peerArm&&(c.peerArm!=p.h.sourceArm||c.peerHandle!=p.h.sourceHandle)){stopNpcChannel(c,"peer_incarnation_changed");return;}
        c.peerArm=p.h.sourceArm;c.peerHandle=p.h.sourceHandle;
        if(localRole&&!c.lease.references&&!c.corpse){const bool acquired=acquireNpcAiLease(c.lease);npcStreamSessionState.acquired+=c.lease.acquired;if(!acquired){stopNpcChannel(c,"ai_acquire_failed");return;}c.result="pausing";}
    }else{
        if(p.h.sourceArm!=c.peerArm||p.h.targetArm!=c.arm||p.h.sourceHandle!=c.peerHandle||p.h.targetHandle!=c.lease.binding.npc.handle){++c.rejected;return;}
        if(p.h.kind==NpcAuthorityMessage::Stop){stopNpcChannel(c,"peer_stopped",false);return;}
        if(p.h.kind==NpcAuthorityMessage::Ready){c.ready=true;c.result="host_authority";}
    }
    c.peerSequence=p.h.sequence;c.peerAt=tick;
}
void receiveNpcPose(const CoopNpcPose& p,ULONGLONG tick){
    if(npcStreamSessionState.menuSuspended||npcStreamSessionState.reloadPending){++npcStreamSessionState.ignored;return;}
    if(!npcStreamContext(tick)||!validNpcPose(p,outgoing,remote)){++npcPacketsRejected;++npcPoseScopeRejected;return;}
    auto it=std::find_if(npcChannels.begin(),npcChannels.end(),[&](const auto& c){return c.enabled&&c.arm==p.h.targetArm;});
    if(it==npcChannels.end()){++npcPacketsRejected;return;}auto& c=*it;++c.received;
    if(!c.ready||(!c.lease.ready&&!c.corpse)||p.h.sourceArm!=c.peerArm||p.h.sourceHandle!=c.peerHandle||p.h.targetHandle!=c.lease.binding.npc.handle||
       p.h.rig!=c.rig||p.h.joints!=c.render.joints||!c.buffer.push(p,presentationClock())){++c.rejected;return;}
    c.poseAt=tick;c.peerAt=tick;++c.poses;
}
void sendNpcPose(NpcAuthorityChannel& c){
    NativeSkeleton s{};
    const auto refuse=[&](const char* reason){++c.rejected;++c.poseFailures;c.poseFailure=reason;};
    if(!readNativeSkeleton(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),c.lease.binding.npc.handle,s)){refuse("native_skeleton_read");return;}
    if(!s.stable||s.logic!=1||s.game!=c.lease.binding.health.game||s.actor!=c.lease.binding.npc.actor||s.animation!=c.render.animation){refuse("skeleton_identity");return;}
    if(npcAuthorityRig(s)!=c.rig){refuse("rig_identity");return;}
    CoopNpcPose p{};p.h=npcHeader(c,NpcAuthorityMessage::Pose);p.sampleUs=static_cast<std::uint64_t>(presentationClock()*1000.);
    std::memcpy(p.root,s.root,sizeof(p.root));p.health=c.lease.binding.health.current;p.maximum=c.lease.binding.health.maximum;
    for(unsigned j=0;j<p.h.joints;++j)for(unsigned col=0;col<4;++col)for(unsigned row=0;row<3;++row)p.local[j][col*3+row]=s.localPose[j][col*4+row];
    if(!validNpcPose(p,remote,outgoing)){
        c.poseFailureJoint=-1;std::memcpy(c.poseFailureRoot,p.root,sizeof(p.root));
        for(unsigned j=0;j<p.h.joints;++j)if(!bodyRigid(npcWireMatrix(p.local[j]),3.f)){
            c.poseFailureJoint=static_cast<int>(j);std::memcpy(c.poseFailureMatrix,p.local[j],sizeof(c.poseFailureMatrix));break;
        }
        refuse(c.poseFailureJoint>=0?"joint_transform":"pose_header_or_root");return;
    }
    NpcPoseWire wire{};if(!encodeNpcPose(p,wire)){refuse("pose_encoding");return;}
    ++c.sequence;if(coopSendMessage(connection,wire.data(),p.h.size,k_nSteamNetworkingSend_UnreliableNoNagle,nullptr)==k_EResultOK){++c.sent;++c.poses;}else refuse("pose_transport");
}
void serviceNpcStream(ULONGLONG tick){
    auto& s=npcStreamSessionState;
    if(npcStreamRequest>=0){const int request=npcStreamRequest;npcStreamRequest=-1;
        if(!request){disableNpcStreamSession("disabled");return;}
        if(s.enabled)return;
        s={};s.scope=doorSessionScope(outgoing,remote);s.enabled=true;beginNpcStream(tick);
        if(!npcAuthorityStreamActive){s.enabled=false;s.result=npcStreamResult;}else s.result="active";return;
    }
    if(!s.enabled)return;
    if(!sockets||!connection||connectionState!=k_ESteamNetworkingConnectionState_Connected||outgoing.session!=s.scope.session||remote.session!=s.scope.peerSession){disableNpcStreamSession("session_disconnected");return;}
    if(doorCheckpointBarrierActive()){stopNpcStream("checkpoint_barrier");clearNpcPresentation();return;}
    if(s.reloadPending){
        if(tick>s.deadline){disableNpcStreamSession("checkpoint_renewal_timeout");return;}
        if(!s.reloadReady||!doorSessionRenewalReady(s.scope,outgoing,remote)||!lastReceive||tick-lastReceive>=250)return;
        s.scope=doorSessionScope(outgoing,remote);s.reloadPending=s.reloadReady=false;beginNpcStream(tick);
        if(!npcAuthorityStreamActive){disableNpcStreamSession("renewed_catalog_refused");return;}
        ++s.rearms;s.result="renewed_after_shared_checkpoint";
    }
    if(!doorSessionCanRetainMenu(s.scope,outgoing,remote)){disableNpcStreamSession("world_or_native_phase_changed");return;}
    if(!lastReceive||tick-lastReceive>=kSessionStallLimitMs){disableNpcStreamSession("session_pose_stale");return;}
    if(tick-lastReceive>=1000||!coopWorldPairReady(outgoing,remote)){
        if(!s.menuSuspended){s.menuSuspended=true;s.menuSince=tick;++s.menuSuspensions;clearNpcPresentation();}
        s.result="native_menu_suspended";return;
    }
    if(s.menuSuspended){
        if(tick-lastReceive>=250)return;
        s.menuPausedMs+=tick>=s.menuSince?tick-s.menuSince:0;s.menuSince=0;s.menuSuspended=false;++s.menuResumes;s.result="resumed_after_native_menu";
        for(auto& c:npcChannels)if(c.enabled){c.ready=false;c.buffer.resume();c.started=c.peerAt=tick;c.poseAt=c.helloAt=c.readyAt=c.lastSend=c.lastCheck=0;}
    }
    if(!npcAuthorityStreamActive)return;if(!npcStreamContext(tick)){stopNpcStream("world_or_pair_changed");return;}
    refreshNpcRelevance(tick);
    unsigned ready=0;
    // 1097: body poses share a bounded budget (~160 KB/s) so roots, partner
    // presentation and combat messages keep their latency; mission 1096 queued
    // ~970 ms at 20 Hz x 12 NPCs. Per-NPC interval 50..400 ms (stale at 500).
    ULONGLONG poseInterval=50;{unsigned long long bytes=0;for(const auto& x:npcChannels)if(x.enabled&&x.ready)bytes+=npcPoseBytes(x.render.joints);
        poseInterval=(std::min<ULONGLONG>)(400,(std::max<ULONGLONG>)(50,bytes*1000ull/160000ull));}
    npcPoseIntervalMs=static_cast<unsigned>(poseInterval);
    for(auto& c:npcChannels){if(!c.enabled)continue;
        if(tick<c.started)continue;
        if(npcStreamPeerExpired(tick,c.started,c.peerAt,c.peerArm!=0)){stopNpcChannel(c,"peer_timeout");continue;}
        if(tick-c.lastCheck>=100){c.lastCheck=tick;NativeNpcAuthority fresh{};
            const bool freshRead=readNativeNpcAuthority(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,c.lease.binding.npc.handle,fresh);
            const bool same=freshRead&&sameNpcAuthorityIdentity(c.lease.binding,fresh,false);
            const bool dead=same&&fresh.health.logic==1&&std::isfinite(fresh.health.current)&&fresh.health.current<=0&&fresh.health.maximum>0;
            if(!same||(!npcAuthorityLiving(fresh)&&!dead)){
                c.lifecycleDiff=!freshRead?0x80000000u:((npcAuthorityIdentityDiff(c.lease.binding,fresh)&~0xe00u)|(!npcAuthorityLiving(fresh)?0x40000000u:0u));
                stopNpcChannel(c,"native_lifecycle_changed");continue;}
            c.lease.binding.health=fresh.health;
            if(dead&&!c.corpse&&c.ready){c.corpse=true;c.corpseUntil=tick+8000;c.result=localRole?"client_corpse":"host_corpse";}
            if(c.corpse){
                // 1113d: corpses stay host-streamed (budgeted) for the stream lifetime; releasing
                // the client ragdoll mid-session moved it 0.27-0.86 m (1116).
                if(localRole)npcCorpseHoldAnimated(c);
            }
            else if(localRole&&c.lease.references){const bool paused=npcAuthorityPaused(fresh);
                if(!paused&&(c.lease.ready||tick-c.started>1500)){stopNpcChannel(c,"ai_ownership_changed");continue;}
                c.lease.ready=paused;if(paused){c.ready=true;c.result="client_replica";}}
        }
        if(tick-c.helloAt>=500){c.helloAt=tick;sendNpcControl(c,NpcAuthorityMessage::Hello);}
        if(localRole&&c.ready&&tick-c.readyAt>=500){c.readyAt=tick;sendNpcControl(c,NpcAuthorityMessage::Ready);}
        if(!localRole&&c.ready&&tick-c.lastSend>=poseInterval){if(bulkTransportCongested(tick))++bulkCongestionSkips;else{c.lastSend=tick;sendNpcPose(c);}}
        if(localRole&&c.ready){
            const bool stale=npcStreamPoseStale(tick,c.started,c.poseAt);
            if(stale&&!c.poseStale)++c.poseGaps;
            if(!stale&&c.poseStale)++c.poseRecoveries;
            c.poseStale=stale;if(stale)c.result="client_pose_stale";
            // presentNpcStream still expires old render data at350ms. The
            // reliable heartbeat, current world/actor and AI lease checks
            // above retain sole gameplay authority during this visual gap.
        }
        if(c.ready)++ready;
    }
    npcStreamResult=ready?"active":"negotiating_or_retired";
    if(std::none_of(npcChannels.begin(),npcChannels.end(),[](const auto& c){return c.enabled;}))npcStreamResult="no_bound_npcs"; // 1166: relevance re-adds
}
void presentNpcStream(ULONGLONG tick){
    if(!localRole||!npcStreamContext(tick)){clearNpcPresentation();return;}
    std::vector<NpcRenderState> next;
    for(auto& c:npcChannels){if(!c.enabled||!c.ready||(!c.lease.ready&&!c.corpse)||!c.poseAt||tick-c.poseAt>=350)continue;
        NpcRenderState p{};p.id=c.render;p.id.corpse=c.corpse;p.until=c.poseAt+350;p.arm=c.arm;
        if(!c.buffer.sample(presentationClock(),p.pose)||!npcRenderCurrent(p.id))continue;p.sequence=p.pose.h.sequence;
        reinterpret_cast<void(*)(void*,const float*,const float*,bool,int)>(gameImage+0x87d200)(reinterpret_cast<void*>(c.lease.binding.npc.actor),p.pose.root,p.pose.root+3,true,0);
        ++c.roots;next.push_back(std::move(p));
    }
    AcquireSRWLockExclusive(&npcPresentationLock);npcPresentation=std::move(next);ReleaseSRWLockExclusive(&npcPresentationLock);
}
void writeNpcStream(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.npc-stream.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;
    fprintf(f,"{\"enabled\":%s,\"result\":\"%s\",\"epoch\":%llu,\"peerEpoch\":%llu,\"rejected\":%llu,\"bodyApplied\":%llu,\"bodyRefused\":%llu,\"framesOwned\":%llu,\"childrenUpdated\":%llu,\"poseIntervalMs\":%u,\"entries\":[",npcAuthorityStreamActive?"true":"false",npcStreamResult,npcStreamEpoch,npcStreamPeerEpoch,npcPacketsRejected,npcBodiesApplied.load(),npcBodiesRefused.load(),npcFramesOwned.load(),npcChildUpdates.load(),npcPoseIntervalMs);
    for(unsigned i=0;i<npcChannels.size();++i){const auto& c=npcChannels[i];fprintf(f,"%s{\"handle\":%u,\"arm\":%llu,\"peerArm\":%llu,\"peerHandle\":%u,\"enabled\":%s,\"ready\":%s,\"result\":\"%s\",\"joints\":%u,\"rig\":\"%llx\",\"references\":%u,\"released\":%llu,\"lost\":%llu,\"sent\":%llu,\"received\":%llu,\"rejected\":%llu,\"poses\":%llu,\"roots\":%llu,\"lifecycleDiff\":%u}",i?",":"",c.lease.binding.npc.handle,c.arm,c.peerArm,c.peerHandle,c.enabled?"true":"false",c.ready?"true":"false",c.result,c.render.joints,c.rig,c.lease.references,c.lease.released,c.lease.lost,c.sent,c.received,c.rejected,c.poses,c.roots,c.lifecycleDiff);}
    fprintf(f,"],\"bodyEntries\":[");for(unsigned i=0;i<npcChannels.size();++i)fprintf(f,"%s{\"arm\":%llu,\"handle\":%u,\"applied\":%llu,\"sequence\":%llu}",i?",":"",npcChannels[i].arm,npcChannels[i].lease.binding.npc.handle,npcActorBodyApplied[i].load(),npcActorBodySequence[i].load());
    fputs("],\"poseDiagnostics\":[",f);for(unsigned i=0;i<npcChannels.size();++i){const auto& c=npcChannels[i];fprintf(f,"%s{\"arm\":%llu,\"handle\":%u,\"stale\":%s,\"gaps\":%llu,\"recoveries\":%llu,\"failures\":%llu,\"lastFailure\":\"%s\",\"joint\":%d,\"matrix\":[",i?",":"",c.arm,c.lease.binding.npc.handle,c.poseStale?"true":"false",c.poseGaps,c.poseRecoveries,c.poseFailures,c.poseFailure,c.poseFailureJoint);
        for(unsigned j=0;j<12;++j)fprintf(f,"%s%.9g",j?",":"",c.poseFailureMatrix[j]);fputs("],\"root\":[",f);
        for(unsigned j=0;j<12;++j)fprintf(f,"%s%.9g",j?",":"",c.poseFailureRoot[j]);fputs("]}",f);
    }
    SteamNetConnectionRealTimeStatus_t status{};const bool qos=sockets&&connection&&sockets->GetConnectionRealTimeStatus(connection,&status,0,nullptr)==k_EResultOK;
    const auto& s=npcStreamSessionState;
    fprintf(f,"],\"relevance\":{\"bound\":%u,\"scans\":%llu,\"added\":%llu,\"dropped\":%llu,\"clientBound\":%llu,\"clientRefused\":%llu",npcEnabledChannels(),npcRelevanceScans,npcRelevanceAdded,npcRelevanceDropped,npcClientBound,npcClientBindRefused);
    fprintf(f,"},\"transport\":{\"available\":%s,\"sendRate\":%d,\"pendingUnreliable\":%d,\"pendingReliable\":%d,\"queueUs\":%lld},\"decodeRejected\":%llu,\"poseScopeRejected\":%llu,",qos?"true":"false",status.m_nSendRateBytesPerSecond,status.m_cbPendingUnreliable,status.m_cbPendingReliable,status.m_usecQueueTime,npcDecodeRejected,npcPoseScopeRejected);
    fprintf(f,"\"session\":{\"enabled\":%s,\"result\":\"%s\",\"reloadPending\":%s,\"reloadReady\":%s,\"reloads\":%llu,\"rearms\":%llu,\"menuSuspended\":%s,\"menuSuspensions\":%llu,\"menuResumes\":%llu,\"menuPausedMs\":%llu,\"ignored\":%llu,\"acquired\":%llu,\"released\":%llu,\"retired\":%llu,\"lost\":%llu}}\n",s.enabled?"true":"false",s.result,s.reloadPending?"true":"false",s.reloadReady?"true":"false",s.reloads,s.rearms,s.menuSuspended?"true":"false",s.menuSuspensions,s.menuResumes,s.menuPausedMs,s.ignored,s.acquired,s.released,s.retired,s.lost);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
