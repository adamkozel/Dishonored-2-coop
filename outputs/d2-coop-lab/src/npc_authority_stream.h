#pragma once
#include "player_scene_probe.h"
#include "npc_corpse_trace.h"
// Host-authored root and final skeleton stream. This carries presentation and
// native AI ownership, not client hit requests or authoritative damage events.
struct NpcRenderIdentity {
    NativeNpcAuthority native{};
    std::uintptr_t animation=0,render=0,model=0,skeleton=0;
    unsigned animationGeneration=0,joints=0,slot=0;
    bool corpse=false; // 1113: shared-death corpse presentation (health0, AI not paused)
    bool unconscious=false; // 1214a: shared knockout (health > 0, AI destroyed by the knockout)
    bool possessed=false; // 1214ij (#160): a host NPC her Possession drives (its AI may be awake)
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
    // 1214ij (#160): partner-corvo-power-v1800b refused all 621 frames of her possessed body here: the alert guard's AI
    // was not paused. Her Possession owns that body as the game owns a possessed one.
    if(id.corpse||id.unconscious||id.possessed)return true; // death/knockout already applied natively; that AI is not paused-owned
    for(unsigned i=0;i<3;++i){const auto& s=n.ai[i];
        if(!eq(s.component,base+nativeAiVtable(s.kind))||!eq(s.component+0x10,n.npc.actor)||
           !memoryRead(s.component+0x18,&u,4)||u!=n.aiGeneration[i]||!memoryRead(s.component+0x1c,&u,4))return false;
        if(i<2){if((u&12)!=8||((u>>4)&3)!=2||!((u>>6)&0x1fffff))return false;}
        // 1214ix (#180): or paused like them, by a forced client lease (mission-partner-powers-v1813a: the forced replica
        // was ready, but every frame was refused here, 0 bodies and 0 roots, and it stood 0.74 m from the host guard).
        else if((u&12)&&(((u>>4)&3)!=2||!((u>>6)&0x1fffff)))return false;
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
    // 1214n: when the corpse appeared, and the host's Dead reconciliation for a death
    // the combat channel never shared.
    ULONGLONG corpseAt=0,deadSentAt=0;unsigned deadSends=0;const char* deathShare="none";
    // 1214ae: a body the client carries (host: carriedUntil; client: localCarryUntil).
    ULONGLONG carriedUntil=0,localCarryUntil=0,carryControlAt=0;std::uint64_t carryControls=0,carryPoses=0;
    // 1214ai: host corpse following a client's lethal paired scene (npc_paired.h).
    ULONGLONG pairedCorpseUntil=0;
    // 1214aj: host NPC following a client's lethal paired scene without an AI lease.
    bool hostPairedFollow=false;
    // 1214ef: the host's own paired takedown on this NPC (poses every 50 ms until 3 s after).
    ULONGLONG hostOwnPairedUntil=0;std::uint64_t priorityPoses=0;
    // 1214en: the NPC is down (knocked down) on the host: host poses carry it (priority while down); the client keeps
    // its replica laying so its own native rules offer the knocked-down takedowns (take 1529b).
    ULONGLONG hostDownUntil=0,peerDownAt=0;bool downApplied=false;std::uint64_t downSent=0,downReceived=0,downApplies=0;
    // 1214eo: host: the knocked-out body is held animated while a client's knocked-down choke victim pose drives it.
    bool victimHeld=false;std::uint64_t victimHolds=0,victimReleases=0;
    bool corpseFreezeEnded=false; // 1214fj (#127)
    bool ownPairedCorpse=false;ULONGLONG ownPairedCorpseUntil=0; // 1214hh (#140): client, the corpse of her own paired scene
    bool settledHold=false; // 1214hj (#140): host, the corpse holds her settled pose
    bool forcedLease=false; // 1214iu (#180): client, bound while its attack AI ran; its lease is forced
    // 1214eg: receiver pose rejections by reason: not ready, AI lease not ready, identity, replay, source clock.
    std::array<std::uint64_t,5> poseRejects{};double lastPoseLateMs=0;
    // 1213u (#46): a paired takedown by the CLIENT on this host-owned NPC. Client:
    // localPaired keeps its native victim animation (host poses are not applied);
    // a choke that knocks the replica out keeps it natively unconscious. Host:
    // the NPC's AI is held for the scene and it is knocked out at choke_win.
    bool localPaired=false,unconscious=false,hostPairedLease=false;unsigned pairedVariant=0;
    ULONGLONG pairedUntil=0,pairedHoldAt=0,pairedKnockoutAt=0,unconsciousSentAt=0,unconsciousAt=0,reversePoseAt=0,reverseSentAt=0;unsigned unconsciousSends=0;std::uint64_t reversePoses=0;float reverseRoot[3]{},presentedRoot[3]{};const char* pairedResult="none";
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
// 1214he (#140): the partner's game in an NPC stream session (its NPCs are host-owned replicas).
bool npcStreamReplicaSession(){return localRole==1&&npcAuthorityStreamActive&&npcStreamSessionState.enabled;}
bool npcStreamContext(ULONGLONG tick){
    const auto& s=npcStreamSessionState;
    return npcAuthorityStreamActive&&s.enabled&&!s.reloadPending&&!s.menuSuspended&&!doorCheckpointBarrierActive()&&nativeMutationAllowed&&GetCurrentThreadId()==engineThread.load()&&coopNpcPairReady(tick)&&
        outgoing.session==npcStreamSession&&remote.session==npcStreamPeerSession&&outgoing.worldEpoch==npcStreamEpoch&&
        remote.worldEpoch==npcStreamPeerEpoch&&outgoing.mapId==npcStreamMap;
}
void hostNpcPaired(NpcAuthorityChannel& c,const CoopNpcControl& p,ULONGLONG tick);
void serviceHostNpcPaired(NpcAuthorityChannel& c,ULONGLONG tick);
void clientNpcUnconscious(NpcAuthorityChannel& c);
void clientNpcDead(NpcAuthorityChannel& c);
unsigned npcCombatDeathShare(std::uint64_t arm);
float npcReconcileDeclAmount=0;std::uint64_t npcReconcileDeclFlags=0;unsigned npcReconcileDeclReactions[2]{};
std::uint64_t npcVanishedDead=0; // 1214fb (#118)
// 1214fk (#117): a Dead marked ASH1 (kNpcDeadAshed, npc_authority_protocol.h) means the peer's copy turned to ash.
struct NpcAshRequest{std::uint32_t handle=0;std::uintptr_t actor=0;};
std::vector<NpcAshRequest> npcAshPending;std::uint64_t npcAshSent=0,npcAshApplied=0,npcAshRefused=0,npcAshEffects=0;
std::uint64_t npcDeadReconciled=0,npcDeadSent=0,npcCombatSharedDeaths=0,npcClientDeadApplied=0,npcClientDeadAlready=0,npcClientDeadRefused=0;const char* npcClientDeadLast="none";
std::uint64_t npcPairedForced=0; // 1214ij (#160)
std::uint64_t npcPairedHolds=0,npcPairedKnockouts=0,npcPairedRefused=0,npcPairedLocal=0,npcPairedCorpses=0,npcPairedFollows=0,npcHostOwnPaired=0,npcPriorityPoses=0;
NpcAuthorityHeader npcHeader(const NpcAuthorityChannel& c,NpcAuthorityMessage kind){
    NpcAuthorityHeader h{};h.role=localRole;h.kind=kind;h.sequence=c.sequence+1;h.sourceSession=npcStreamSession;h.targetSession=npcStreamPeerSession;
    h.sourceEpoch=npcStreamEpoch;h.targetEpoch=npcStreamPeerEpoch;h.map=npcStreamMap;h.sourceArm=c.arm;h.sourceHandle=c.lease.binding.npc.handle;
    // 1213z: PairedPose is a pose-sized message too (1213x sized it as a control
    // message, so every client victim pose failed validation before sending).
    h.rig=c.rig;h.joints=c.render.joints;h.size=static_cast<unsigned short>(npcPoseKind(kind)?npcPoseBytes(h.joints):sizeof(CoopNpcControl));
    if(kind!=NpcAuthorityMessage::Hello){h.targetArm=c.peerArm;h.targetHandle=c.peerHandle;}return h;
}
bool sendNpcControl(NpcAuthorityChannel& c,NpcAuthorityMessage kind,std::uint32_t reserved=0){
    if(!sockets||!connection||connectionState!=k_ESteamNetworkingConnectionState_Connected)return false;
    CoopNpcControl p{};p.h=npcHeader(c,kind);p.h.reserved=reserved;const auto& n=c.lease.binding.npc;
    std::memcpy(p.spawner,n.spawnerName.c_str(),n.spawnerName.size());std::memcpy(p.definition,n.definition.c_str(),n.definition.size());
    if(!validNpcControl(p,remote,outgoing))return false;
    ++c.sequence;
    if(coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)!=k_EResultOK)return false;
    ++c.sent;return true;
}
// 1214dm (#11 carry_body_lift_idle_move_drop_throw): while the observer attaches the partner's carried body to the
// presented partner (carry_attachment_fixture.h in product mode), the stream leaves that exact body alone: no streamed
// root or bones, no animated hold, no ragdoll request when a channel stops, no relevance eviction. It resumes 300 ms
// after the attachment ends (1500 ms after a throw). In 1277p/r the streamed root and bones kept the observer's copy
// at the carrier's feet (a carried body's root stays at the carrier's root) while the partner presented the carry.
struct NpcCarrySuppression{std::uint32_t handle=0;std::uintptr_t actor=0;ULONGLONG resumeAt=0;
    std::uint64_t claims=0,releases=0,presentSkips=0,holdSkips=0,physicsSkips=0,evictSkips=0;} npcCarrySuppression;
bool npcCarrySuppressedHandle(std::uint32_t handle,std::uintptr_t actor,ULONGLONG tick){
    const auto& s=npcCarrySuppression;return s.handle&&handle==s.handle&&actor==s.actor&&(!s.resumeAt||tick<s.resumeAt);}
bool npcCarrySuppressed(const NpcAuthorityChannel& c,ULONGLONG tick){return npcCarrySuppressedHandle(c.lease.binding.npc.handle,c.lease.binding.npc.actor,tick);}
void npcCarryAttachClaim(std::uint32_t handle,std::uintptr_t actor){
    auto& s=npcCarrySuppression;s.handle=handle;s.actor=actor;s.resumeAt=0;++s.claims;
    for(auto& c:npcChannels)if(c.enabled&&c.lease.binding.npc.handle==handle&&c.lease.binding.npc.actor==actor){clearNpcPresentation(c.arm);c.corpsePhysics=0;}
}
void npcCarryAttachRelease(ULONGLONG tick,ULONGLONG graceMs){auto& s=npcCarrySuppression;if(s.handle&&!s.resumeAt){s.resumeAt=tick+graceMs;++s.releases;}}
// The physics held animated by npcCorpseHoldAnimated falls again: 0x734d90 copies the presented bones into the ragdoll
// bodies, 0x734a10 requests ragdoll mode 0.
std::uint64_t npcCorpseHoldReleases=0; // 1214ff
// 1214fj (#127): after a client's lethal paired takedown the host's corpse stays out of its ragdoll (physics mode 3,
// mission-coverage-v1631a, 1634a) and its animation stands it up once the client's victim poses stop, while the
// client's own copy lies (1630a, 1631a, 1635b). The host keeps presenting the client's last victim pose (its bones;
// the root stays native, so a pull or another move still carries it) until a carry touches the body. Requesting the
// ragdoll instead laid the host's corpse down but stood the client's copy up (1214fg and 1214fi: 1632a, 1635a).
std::uint64_t npcCorpseFreezeFrames=0,npcCorpseFreezeEnds=0;
std::uint64_t npcHostVictimFrames=0,npcHostVictimSampleRefused=0,npcHostVictimRenderRefused=0; // 1214ii (#160)
void npcPhysicsRelease(NpcAuthorityChannel& c){
    if(c.corpsePhysics){std::uintptr_t vt=0;
        if(memoryRead(c.corpsePhysics,&vt,8)&&vt==reinterpret_cast<std::uintptr_t>(gameImage)+0x1c54008){
            reinterpret_cast<void(*)(void*)>(gameImage+0x734d90)(reinterpret_cast<void*>(c.corpsePhysics));
            reinterpret_cast<void(*)(void*)>(gameImage+0x734a10)(reinterpret_cast<void*>(c.corpsePhysics));}
        c.corpsePhysics=0;}
}
// 1214it (#180): the last channel stops (reason, channel age, whether the peer had answered) and the client's
// outcome for each Hello without a channel, kept in the status. Her copy of the host's checkpoint guard never bound in
// mission-partner-powers-v1808a and the v1809b possession case (the host channel stayed negotiating, re-made every
// 5 s), and in v1809b its channel ended before her Blink Assault; the reused channel slots kept no reason.
struct NpcStreamTrace{ULONGLONG at=0;unsigned handle=0,age=0;bool ready=false,peer=false;char what[32]{},spawner[48]{};};
std::array<NpcStreamTrace,32> npcStopTraces{},npcHelloTraces{};unsigned npcStopTraceNext=0,npcHelloTraceNext=0;std::uint64_t npcClientHelloFull=0,npcClientHelloInvalid=0;
std::uint64_t npcClientForcedLeases=0,npcClientForcedReady=0,npcClientForcedStopped=0; // 1214iu (#180)
inline void npcStreamTraceAdd(std::array<NpcStreamTrace,32>& ring,unsigned& next,ULONGLONG tick,unsigned handle,unsigned age,bool ready,bool peer,const char* what,const char* spawner){
    auto& t=ring[next++%ring.size()];t={};t.at=tick;t.handle=handle;t.age=age;t.ready=ready;t.peer=peer;strncpy_s(t.what,what,_TRUNCATE);strncpy_s(t.spawner,spawner,_TRUNCATE);}
void writeNpcStreamTraceRing(FILE* f,const char* name,const std::array<NpcStreamTrace,32>& ring,unsigned next){
    fprintf(f,",\"%s\":[",name);bool first=true;
    for(unsigned i=0;i<ring.size();++i){const auto& t=ring[(next+i)%ring.size()];if(!t.at)continue;
        fprintf(f,"%s{\"at\":%llu,\"handle\":%u,\"age\":%u,\"ready\":%s,\"peer\":%s,\"what\":",first?"":",",t.at,t.handle,t.age,t.ready?"true":"false",t.peer?"true":"false");
        writeWorldText(f,t.what);fputs(",\"spawner\":",f);writeWorldText(f,t.spawner);fputc('}',f);first=false;}
    fputc(']',f);}
void writeNpcStreamTraces(FILE* f){
    fprintf(f,",\"traceNow\":%llu,\"helloFull\":%llu,\"helloInvalid\":%llu,\"forcedLeases\":%llu,\"forcedReady\":%llu,\"forcedStopped\":%llu",GetTickCount64(),npcClientHelloFull,npcClientHelloInvalid,
        npcClientForcedLeases,npcClientForcedReady,npcClientForcedStopped);
    fprintf(f,",\"leaseAcquireStep\":\"%s\",\"leaseAcquireFlags\":[%u,%u,%u]",npcLeaseAcquireStep,npcLeaseAcquireFlags[0],npcLeaseAcquireFlags[1],npcLeaseAcquireFlags[2]); // 1214iv
    writeNpcStreamTraceRing(f,"stopTraces",npcStopTraces,npcStopTraceNext);writeNpcStreamTraceRing(f,"helloTraces",npcHelloTraces,npcHelloTraceNext);}
void stopNpcChannel(NpcAuthorityChannel& c,const char* reason,bool notify=true){
    if(!c.enabled)return;clearNpcPresentation(c.arm);
    if(c.corpsePhysics&&npcCarrySuppressed(c,GetTickCount64())){c.corpsePhysics=0;++npcCarrySuppression.physicsSkips;}
    npcPhysicsRelease(c);
    if(notify&&c.peerArm)sendNpcControl(c,NpcAuthorityMessage::Stop);
    releaseNpcAiLease(c.lease);
    auto& s=npcStreamSessionState;s.released+=c.lease.released;s.retired+=c.lease.retired;s.lost+=c.lease.lost;
    {const ULONGLONG now=GetTickCount64();npcStreamTraceAdd(npcStopTraces,npcStopTraceNext,now,c.lease.binding.npc.handle,static_cast<unsigned>(now>c.started?now-c.started:0),
        c.ready,c.peerArm!=0,reason,c.lease.binding.npc.spawnerName.c_str());} // 1214it (#180)
    c.enabled=c.ready=false;c.result=reason;c.buffer={};
}
// 1214eo: knockedDown also takes a body in the knockdown ragdoll mode 2 (IsOnGround, chapter 100).
bool npcCorpseHoldAnimated(NpcAuthorityChannel& c,bool knockedDown=false){
    if(npcCarrySuppressed(c,GetTickCount64())){++npcCarrySuppression.holdSkips;return false;}
    if(c.corpsePhysics)return true;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t list=0;unsigned flags=0,mode=99,pending=0;std::uintptr_t physics=0;
    if(!memoryRead(c.lease.binding.npc.actor+0x268,&list,8)||!list||!memoryRead(c.lease.binding.npc.actor+0x270,&flags,4)||(flags&0xffffff)>256)return false;
    for(unsigned i=0;i<(flags&0xffffff);++i){std::uintptr_t p=0,vt=0;if(!memoryRead(list+i*8,&p,8)||!p||!memoryRead(p,&vt,8))return false;if(vt==base+0x1c54008){if(physics)return false;physics=p;}}
    if(!physics||!memoryRead(physics+0x238,&mode,4)||(mode!=0&&!(knockedDown&&mode==2))||!memoryRead(physics+0x23c,&pending,4)||pending!=0xffffffff)return false;
    reinterpret_cast<void(*)(void*)>(gameImage+0x736ef0)(reinterpret_cast<void*>(physics));c.corpsePhysics=physics;return true;
}
// 1214gd (#140, npc_corpse_trace.h): the corpse physics mode (+0x238), 255 when unreadable.
unsigned npcCorpsePhysicsMode(const NpcAuthorityChannel& c){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t list=0;unsigned flags=0,mode=255;
    if(!memoryRead(c.lease.binding.npc.actor+0x268,&list,8)||!list||!memoryRead(c.lease.binding.npc.actor+0x270,&flags,4)||(flags&0xffffff)>256)return 255;
    for(unsigned i=0;i<(flags&0xffffff);++i){std::uintptr_t q=0,vt=0;if(!memoryRead(list+i*8,&q,8)||!q||!memoryRead(q,&vt,8))return 255;
        if(vt==base+0x1c54008){if(!memoryRead(q+0x238,&mode,4))return 255;return mode;}}
    return 255;
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
constexpr float kNpcCivilianRelevanceAdd=15.f,kNpcCivilianRelevanceDrop=20.f; // 1214ey (#113)
inline bool npcCivilian(const NativeNpcAuthority& n){return n.ai[0].kind==NativeAiKind::DecisionCivilian;}
ULONGLONG npcRelevanceAt=0;std::uint64_t npcRelevanceScans=0,npcRelevanceAdded=0,npcRelevanceDropped=0,npcRelevanceEvicted=0,npcClientBound=0,npcClientBindRefused=0;
float npcNearestPlayerSq(const float* p){
    float best=1e30f;
    for(const auto* c:{outgoing.position,remote.position}){float d=0;for(unsigned k=0;k<3;++k){const float v=p[k]-c[k];d+=v*v;}best=(std::min)(best,d);}
    return best;
}
unsigned npcEnabledChannels(){unsigned n=0;for(const auto& c:npcChannels)if(c.enabled)++n;return n;}
bool npcHostStreamScope(){return !localRole&&std::strcmp(mapName,kGuardMap)!=0;} // 1214el (#118)
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
// 1213o (#43): native NPC catalog reads leave the engine thread. A catalog read
// is thousands of ReadProcessMemory calls (70-207 ms on Edge of the World, take
// 1235f) and stalled the host's 2 s relevance scan and the client's Hello binds.
// One worker reads it; the engine thread only takes the finished snapshot and
// re-reads the few NPCs it is about to bind (freshNpcCatalogEntry) before
// bindNpcChannel, which re-reads their skeleton and identity again.
struct NpcCatalogSnapshot{ULONGLONG at=0;std::uintptr_t game=0;std::uint64_t epoch=0;bool ok=false;unsigned ineligible=0,unavailable=0,unbound=0;double ms=0;std::vector<NativeNpcAuthority> entries;std::vector<NativeNpcLife> life;};
// 1214el (#118): hostStream: the host's catalog off the lab map, where the mod's own fixtures pause guards.
struct NpcCatalogWorker{SRWLOCK lock=SRWLOCK_INIT;HANDLE wake=nullptr;bool started=false,pending=false,hostStream=false;std::uintptr_t game=0;std::uint64_t epoch=0;
    NpcCatalogSnapshot result;std::uint64_t requests=0,completed=0,taken=0,failed=0;double maxMs=0,lastMs=0;};
NpcCatalogWorker npcCatalogWorker;
DWORD WINAPI npcCatalogWorkerMain(void*){
    auto& w=npcCatalogWorker;
    for(;;){
        WaitForSingleObject(w.wake,INFINITE);
        AcquireSRWLockExclusive(&w.lock);const bool pending=w.pending;const auto game=w.game;const auto epoch=w.epoch;const bool hostStream=w.hostStream;ReleaseSRWLockExclusive(&w.lock);
        if(!pending)continue;
        NpcCatalogSnapshot s{};s.game=game;s.epoch=epoch;const double start=presentationClock();
        s.ok=readNativeNpcAuthorityCatalog(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),game,s.entries,s.ineligible,s.unavailable,s.unbound,&s.life,hostStream);
        s.ms=presentationClock()-start;s.at=GetTickCount64();
        AcquireSRWLockExclusive(&w.lock);w.lastMs=s.ms;w.maxMs=(std::max)(w.maxMs,s.ms);if(!s.ok)++w.failed;
        w.result=std::move(s);w.pending=false;++w.completed;ReleaseSRWLockExclusive(&w.lock);
    }
}
// Engine thread: ask for a catalog of this world unless one is already being read.
void npcCatalogRequest(){
    auto& w=npcCatalogWorker;
    if(!w.started){w.wake=CreateEventW(nullptr,FALSE,FALSE,nullptr);if(!w.wake)return;
        HANDLE t=CreateThread(nullptr,0,npcCatalogWorkerMain,nullptr,0,nullptr);if(!t){CloseHandle(w.wake);w.wake=nullptr;return;}CloseHandle(t);w.started=true;}
    AcquireSRWLockExclusive(&w.lock);const bool idle=!w.pending;if(idle){w.pending=true;w.game=observedGame;w.epoch=outgoing.worldEpoch;w.hostStream=npcHostStreamScope();++w.requests;}ReleaseSRWLockExclusive(&w.lock);
    if(idle)SetEvent(w.wake);
}
void processNpcLife(const NpcCatalogSnapshot& s);
// Engine thread: move out a finished snapshot of the current world not taken before.
bool npcCatalogTake(NpcCatalogSnapshot& out){
    auto& w=npcCatalogWorker;bool fresh=false;
    AcquireSRWLockExclusive(&w.lock);
    if(w.completed>w.taken){w.taken=w.completed;fresh=w.result.ok&&w.result.game==observedGame&&w.result.epoch==outgoing.worldEpoch;if(fresh)out=std::move(w.result);}
    ReleaseSRWLockExclusive(&w.lock);if(fresh)processNpcLife(out);return fresh;
}
// Engine thread: the snapshot entry is still the same eligible living NPC.
bool freshNpcCatalogEntry(const NativeNpcAuthority& n,NativeNpcAuthority& fresh){
    const bool hostStream=npcHostStreamScope();
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);NativeHealth h{};
    if(!readNativeHealth(memoryRead,base,n.npc.handle,h)||h.game!=observedGame||h.actor!=n.npc.actor||h.logic!=1||h.current<=0)return false;
    return readNativeNpcAuthority(memoryRead,base,observedGame,n.npc.handle,fresh)&&(hostStream?npcAuthorityHostStreamEligible(fresh):npcAuthorityLeaseEligible(fresh))&&sameNpcAuthorityIdentity(n,fresh);
}
std::uint64_t npcCatalogStale=0;
void addRelevantNpcs(std::vector<NativeNpcAuthority> catalog,ULONGLONG tick){
    unsigned bound=npcEnabledChannels();
    catalog.erase(std::remove_if(catalog.begin(),catalog.end(),[&](const auto& n){
        if(npcNearestPlayerSq(n.npc.position)>kNpcRelevanceAdd*kNpcRelevanceAdd)return true;
        if(npcCivilian(n)&&npcNearestPlayerSq(n.npc.position)>kNpcCivilianRelevanceAdd*kNpcCivilianRelevanceAdd)return true;
        return std::any_of(npcChannels.begin(),npcChannels.end(),[&](const auto& c){return c.enabled&&(c.lease.binding.npc.handle==n.npc.handle||
            (c.lease.binding.npc.spawnerName==n.npc.spawnerName&&c.lease.binding.npc.definition==n.npc.definition));});}),catalog.end());
    std::stable_sort(catalog.begin(),catalog.end(),[&](const auto& a,const auto& b){return npcNearestPlayerSq(a.npc.position)<npcNearestPlayerSq(b.npc.position);});
    // 1214ac: per-player rank priority over candidates and bound NPCs together.
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    const auto playerSq=[](const float* p,const float* q){float d=0;for(unsigned k=0;k<3;++k){const float v=p[k]-q[k];d+=v*v;}return d;};
    // 1214ag: bound NPCs are tracked by arm, never by pointer: bindNpcChannel appends to
    // npcChannels, and a reallocation left the pointers dangling (take 1279a: the host
    // crashed here with an access violation while many NPCs were bound in one scan).
    std::vector<std::array<float,2>> dist;std::vector<std::uint64_t> owners;std::vector<bool> civilian;
    const auto channelByArm=[](std::uint64_t arm)->NpcAuthorityChannel*{for(auto& c:npcChannels)if(c.enabled&&c.arm==arm)return &c;return nullptr;};
    for(const auto& n:catalog){dist.push_back({playerSq(n.npc.position,outgoing.position),playerSq(n.npc.position,remote.position)});owners.push_back(0);civilian.push_back(npcCivilian(n));}
    for(auto& c:npcChannels){if(!c.enabled)continue;NativeNpcAuthority f{};
        if(!readNativeNpcAuthority(memoryRead,base,observedGame,c.lease.binding.npc.handle,f))continue;
        dist.push_back({playerSq(f.npc.position,outgoing.position),playerSq(f.npc.position,remote.position)});owners.push_back(c.arm);civilian.push_back(npcCivilian(c.lease.binding));}
    auto priority=npcRelevancePriorities(dist,kNpcRelevanceAdd*kNpcRelevanceAdd);
    for(unsigned i=0;i<priority.size();++i)if(civilian[i])priority[i]=npcRelevanceCivilianRank(priority[i]);
    std::vector<unsigned> order;for(unsigned i=0;i<catalog.size();++i)order.push_back(i);
    std::stable_sort(order.begin(),order.end(),[&](unsigned a,unsigned b){return priority[a]<priority[b];});
    for(unsigned i:order){const auto& n=catalog[i];NpcAuthorityChannel* evict=nullptr;
        if(bound>=kNpcRelevanceMax){unsigned worst=0;int worstAt=-1;
            for(unsigned j=catalog.size();j<owners.size();++j){auto* c=owners[j]?channelByArm(owners[j]):nullptr;if(!c||c->localPaired||c->hostPairedLease||c->hostPairedFollow)continue;
                if(npcCarrySuppressed(*c,GetTickCount64())){++npcCarrySuppression.evictSkips;continue;}
                if(worstAt<0||priority[j]>worst){worst=priority[j];worstAt=int(j);}}
            if(worstAt<0||!npcRelevanceDisplaces(priority[i],worst))break;
            evict=channelByArm(owners[worstAt]);owners[worstAt]=0;}
        NativeNpcAuthority fresh{};
        if(!freshNpcCatalogEntry(n,fresh)){++npcCatalogStale;continue;}
        if(evict){stopNpcChannel(*evict,"relevance_evicted");++npcRelevanceEvicted;--bound;}
        if(bindNpcChannel(fresh,tick)){++bound;++npcRelevanceAdded;}}
}
ULONGLONG npcLifeRequestAt=0;extern NpcCatalogSnapshot npcHelloCatalog;
void refreshNpcRelevance(ULONGLONG tick){
    // 1214ad: the client reads its world's catalog every 2 s too (life roster); the
    // snapshot also serves host Hellos.
    if(localRole){NpcCatalogSnapshot s{};if(npcCatalogTake(s))npcHelloCatalog=std::move(s);
        if(tick-npcLifeRequestAt>=2000){npcLifeRequestAt=tick;npcCatalogRequest();}return;}
    NpcCatalogSnapshot snapshot{};if(npcCatalogTake(snapshot))addRelevantNpcs(std::move(snapshot.entries),tick);
    if(tick-npcRelevanceAt<2000)return;npcRelevanceAt=tick;++npcRelevanceScans;
    // Retire channels whose NPC is far from both players (corpses included).
    for(auto& c:npcChannels){if(!c.enabled)continue;NativeNpcAuthority fresh{};
        if(!readNativeNpcAuthority(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,c.lease.binding.npc.handle,fresh))continue;
        const float drop=npcCivilian(fresh)&&!c.localPaired&&!c.hostPairedLease&&!c.hostPairedFollow?kNpcCivilianRelevanceDrop:kNpcRelevanceDrop;
        if(npcNearestPlayerSq(fresh.npc.position)>drop*drop){stopNpcChannel(c,"relevance_left");++npcRelevanceDropped;}}
    npcCatalogRequest(); // 1214aa: also at the cap, so a nearer NPC can displace a far one
}
// Client: bind the NPC named by a host Hello from the current eligible catalog.
// 1213n: every unmatched host Hello re-read the whole native NPC catalog on the
// client's engine thread (80-100 ms on Edge of the World), and a refused bind
// repeated on the next Hello: 82 reads and 46 stalls in ~25 s (take 1235e).
// A catalog snapshot serves Hellos for up to 1 s, and a refused
// spawner/definition pair waits 3 s before it is tried again. 1213o: the
// snapshot comes from the worker; without a current one the Hello is deferred
// (the host repeats it) while the worker reads.
struct NpcHelloRefusal{std::string spawner,definition;ULONGLONG until=0;};
NpcCatalogSnapshot npcHelloCatalog;std::vector<NpcHelloRefusal> npcHelloRefusals;std::uint64_t npcHelloCatalogReads=0,npcClientBindDeferred=0,npcClientBoundLive=0,npcClientBindBusy=0;
// 1214iu (#180): runs of busy Hellos per NPC of her world (first and last busy Hello), and the forced binds.
struct NpcBusyRun{ULONGLONG first=0,last=0;};std::vector<std::pair<unsigned,NpcBusyRun>> npcHelloBusy;
bool bindNpcForHostHello(const CoopNpcControl& p,ULONGLONG tick){
    if(!localRole||p.h.kind!=NpcAuthorityMessage::Hello)return false;
    const std::string spawner(p.spawner,strnlen(p.spawner,sizeof(p.spawner))),definition(p.definition,strnlen(p.definition,sizeof(p.definition)));
    const auto trace=[&](const char* what){npcStreamTraceAdd(npcHelloTraces,npcHelloTraceNext,tick,p.h.sourceHandle,0,false,false,what,spawner.c_str());}; // 1214it (#180)
    if(npcEnabledChannels()>=kNpcRelevanceMax){++npcClientHelloFull;trace("full");return false;}
    for(const auto& r:npcHelloRefusals)if(tick<r.until&&r.spawner==spawner&&r.definition==definition){++npcClientBindDeferred;trace("refusal_wait");return false;}
    auto& c=npcHelloCatalog;
    {NpcCatalogSnapshot next{};if(npcCatalogTake(next)){c=std::move(next);++npcHelloCatalogReads;}}
    if(!c.ok||c.game!=observedGame||c.epoch!=outgoing.worldEpoch||tick-c.at>=1000){npcCatalogRequest();++npcClientBindDeferred;trace("catalog_stale");return false;}
    for(const auto& n:c.entries)if(n.npc.spawnerName==spawner&&n.npc.definition==definition){
        NativeNpcAuthority fresh{};if(!freshNpcCatalogEntry(n,fresh)){++npcCatalogStale;trace("entry_stale");break;}
        if(bindNpcChannel(fresh,tick)){++npcClientBound;trace("bound");return true;}trace("bind_failed");break;}
    // 1214cy (#108): an NPC already attacking in this world (attack AI active) is not acquire-eligible, so it is missing
    // from the eligible catalog and its Hello was refused for 3 s at a time: the guard fighting Emily never got a
    // channel, so its hits were not relayed and its death was not shared (mission-coverage-v1328zzl: the client's guard
    // ineligible, 0 replies to the host's Hellos; zzk). Re-read that one NPC from the life roster on each Hello
    // (every 500 ms) and bind it as soon as its attack is momentarily inactive; no refusal cooldown meanwhile.
    for(const auto& l:c.life)if(l.spawner==spawner&&l.definition==definition&&l.state==0){
        NativeNpcAuthority fresh{};
        if(readNativeNpcAuthority(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,l.handle,fresh)&&fresh.npc.actor==l.actor&&
           fresh.npc.spawnerName==spawner&&fresh.npc.definition==definition){
            // 1214iu (#180): her copy fighting her before the host bound it never had a momentarily inactive attack while
            // she stood in front of it (mission-partner-powers-v1810a: the checkpoint guard's Hellos were busy for 25 s, and
            // the host channel timed out every 5 s). After 2 s of busy Hellos for one NPC its lease is forced, as the host
            // forces it for her Possession (1214ij): decision and reflex AI pause and the host's poses drive the replica. A
            // run continues across the host's re-made channels (busy Hellos less than 4 s apart).
            const bool busy=!npcAuthorityLeaseEligible(fresh);
            if(busy){npcHelloBusy.erase(std::remove_if(npcHelloBusy.begin(),npcHelloBusy.end(),[&](const auto& b){return tick-b.second.last>10000;}),npcHelloBusy.end());
                NpcBusyRun* run=nullptr;for(auto& b:npcHelloBusy)if(b.first==l.handle){run=&b.second;break;}
                if(!run){npcHelloBusy.push_back({l.handle,NpcBusyRun{}});run=&npcHelloBusy.back().second;}
                if(!run->last||tick-run->last>4000)run->first=tick;run->last=tick;
                if(tick-run->first<2000){++npcClientBindBusy;trace("busy");return false;}}
            if(bindNpcChannel(fresh,tick)){++npcClientBound;++npcClientBoundLive;
                if(busy){for(auto& ch:npcChannels)if(ch.enabled&&ch.lease.binding.npc.handle==fresh.npc.handle)ch.forcedLease=true;++npcClientForcedLeases;trace("bound_forced");}
                else trace("bound_live");
                return true;}}
        break;}
    npcHelloRefusals.erase(std::remove_if(npcHelloRefusals.begin(),npcHelloRefusals.end(),[&](const auto& r){return tick>=r.until;}),npcHelloRefusals.end());
    if(npcHelloRefusals.size()<128)npcHelloRefusals.push_back({spawner,definition,tick+3000});
    ++npcClientBindRefused;trace("refused");return false;
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
    clearNpcPresentation();npcChannels.clear();npcHelloCatalog={};npcHelloRefusals.clear();npcHelloBusy.clear();for(auto& c:npcActorBodyApplied)c.store(0);for(auto& c:npcActorBodySequence)c.store(0);
    for(auto& n:catalog)bindNpcChannel(n,GetTickCount64());
    npcRelevanceAt=GetTickCount64();
    npcStreamSession=outgoing.session;npcStreamPeerSession=remote.session;npcStreamEpoch=outgoing.worldEpoch;npcStreamPeerEpoch=remote.worldEpoch;npcStreamMap=outgoing.mapId;
    npcAuthorityStreamActive=true;npcStreamResult="negotiating";
}
// 1214ad: life roster of unbound NPCs.
struct NpcLifeEntry{std::string key;unsigned sent=0,desired=0;bool applied=true;unsigned attempts=0;};
std::vector<NpcLifeEntry> npcLifeEntries;std::uint64_t npcLifeEpoch=~0ull,npcLifeSequence=0,npcLifeSent=0,npcLifeReceived=0,npcLifeApplied=0,npcLifeRefused=0;
bool applyNpcLife(const NativeNpcLife& n,unsigned state);
NpcLifeEntry& npcLifeEntry(const std::string& key){
    for(auto& e:npcLifeEntries)if(e.key==key)return e;
    npcLifeEntries.push_back({key});return npcLifeEntries.back();
}
bool npcLifeBound(const std::string& spawner,const std::string& definition){
    return std::any_of(npcChannels.begin(),npcChannels.end(),[&](const auto& c){return c.enabled&&c.lease.binding.npc.spawnerName==spawner&&c.lease.binding.npc.definition==definition;});
}
bool sendNpcLife(const NativeNpcLife& n){
    if(!sockets||!connection||connectionState!=k_ESteamNetworkingConnectionState_Connected||n.spawner.size()>=192||n.definition.size()>=192||!n.handle)return false;
    CoopNpcControl p{};auto& h=p.h;h.role=localRole;h.kind=NpcAuthorityMessage::Life;h.sequence=npcLifeSequence+1;h.sourceSession=npcStreamSession;h.targetSession=npcStreamPeerSession;
    h.sourceEpoch=npcStreamEpoch;h.targetEpoch=npcStreamPeerEpoch;h.map=npcStreamMap;h.sourceArm=h.sourceHandle=n.handle;h.rig=1;h.joints=1;h.size=sizeof(CoopNpcControl);h.reserved=n.state;
    std::memcpy(p.spawner,n.spawner.c_str(),n.spawner.size());std::memcpy(p.definition,n.definition.c_str(),n.definition.size());
    if(!validNpcControl(p,remote,outgoing))return false;
    ++npcLifeSequence;
    if(coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)!=k_EResultOK)return false;
    ++npcLifeSent;return true;
}
void processNpcLife(const NpcCatalogSnapshot& s){
    if(!npcAuthorityStreamActive||!npcStreamContext(GetTickCount64()))return;
    if(npcLifeEpoch!=outgoing.worldEpoch){npcLifeEntries.clear();npcLifeEpoch=outgoing.worldEpoch;}
    for(const auto& n:s.life){
        if(npcLifeBound(n.spawner,n.definition))continue;
        auto& e=npcLifeEntry(n.spawner+"|"+n.definition);
        if(!e.applied&&e.attempts<5){
            if(!npcLifeApplies(n.state,e.desired)){e.applied=true;}
            else{++e.attempts;if(applyNpcLife(n,e.desired)){e.applied=true;++npcLifeApplied;e.sent=(std::max)(e.sent,e.desired);continue;}++npcLifeRefused;}
        }
        if(n.state>e.sent&&sendNpcLife(n))e.sent=n.state;
    }
}
void receiveNpcLife(const CoopNpcControl& p){
    if(npcLifeEpoch!=outgoing.worldEpoch){npcLifeEntries.clear();npcLifeEpoch=outgoing.worldEpoch;}
    ++npcLifeReceived;auto& e=npcLifeEntry(std::string(p.spawner,strnlen(p.spawner,sizeof(p.spawner)))+"|"+std::string(p.definition,strnlen(p.definition,sizeof(p.definition))));
    // The peer already has this state: never echo it back.
    e.sent=(std::max)(e.sent,p.h.reserved);
    if(p.h.reserved>e.desired){e.desired=p.h.reserved;e.applied=false;e.attempts=0;}
}
void receiveNpcControl(const CoopNpcControl& p,ULONGLONG tick){
    if(npcStreamSessionState.menuSuspended||npcStreamSessionState.reloadPending){++npcStreamSessionState.ignored;return;}
    if(!npcStreamContext(tick)||!validNpcControl(p,outgoing,remote)){++npcPacketsRejected;
        if(localRole&&p.h.kind==NpcAuthorityMessage::Hello){++npcClientHelloInvalid;char name[48]{};std::memcpy(name,p.spawner,(std::min)(sizeof(name)-1,strnlen(p.spawner,sizeof(p.spawner))));
            npcStreamTraceAdd(npcHelloTraces,npcHelloTraceNext,tick,p.h.sourceHandle,0,false,false,"invalid",name);} // 1214it (#180)
        return;}
    if(p.h.kind==NpcAuthorityMessage::Life){receiveNpcLife(p);return;}
    auto it=std::find_if(npcChannels.begin(),npcChannels.end(),[&](const auto& c){return c.enabled&&c.lease.binding.npc.spawnerName==p.spawner&&c.lease.binding.npc.definition==p.definition;});
    if(it==npcChannels.end()&&bindNpcForHostHello(p,tick))
        it=std::find_if(npcChannels.begin(),npcChannels.end(),[&](const auto& c){return c.enabled&&c.lease.binding.npc.spawnerName==p.spawner&&c.lease.binding.npc.definition==p.definition;});
    if(it==npcChannels.end()){++npcPacketsRejected;return;}auto& c=*it;++c.received;
    if(p.h.rig!=c.rig||p.h.joints!=c.render.joints||p.h.sequence<=c.peerSequence){++c.rejected;return;}
    if(p.h.kind==NpcAuthorityMessage::Hello){
        if(c.peerArm&&(c.peerArm!=p.h.sourceArm||c.peerHandle!=p.h.sourceHandle)){stopNpcChannel(c,"peer_incarnation_changed");return;}
        c.peerArm=p.h.sourceArm;c.peerHandle=p.h.sourceHandle;
        if(localRole&&!c.lease.references&&!c.corpse){const bool acquired=acquireNpcAiLease(c.lease,c.forcedLease,c.forcedLease);npcStreamSessionState.acquired+=c.lease.acquired;if(!acquired){stopNpcChannel(c,c.forcedLease?npcLeaseAcquireStep:"ai_acquire_failed");return;}c.result="pausing";}
    }else{
        if(p.h.sourceArm!=c.peerArm||p.h.targetArm!=c.arm||p.h.sourceHandle!=c.peerHandle||p.h.targetHandle!=c.lease.binding.npc.handle){++c.rejected;return;}
        if(p.h.kind==NpcAuthorityMessage::Stop){stopNpcChannel(c,"peer_stopped",false);return;}
        if(p.h.kind==NpcAuthorityMessage::Ready){c.ready=true;c.result="host_authority";}
        if((p.h.kind==NpcAuthorityMessage::PairedHold||p.h.kind==NpcAuthorityMessage::PairedKnockout)&&!localRole)hostNpcPaired(c,p,tick);
        if(p.h.kind==NpcAuthorityMessage::Unconscious&&localRole)clientNpcUnconscious(c);
        if(p.h.kind==NpcAuthorityMessage::Dead&&p.h.reserved==kNpcDeadAshed)npcAshPending.push_back({c.lease.binding.npc.handle,c.lease.binding.npc.actor});
        else if(p.h.kind==NpcAuthorityMessage::Dead&&localRole)clientNpcDead(c);
        if(p.h.kind==NpcAuthorityMessage::Carry&&!localRole){c.carriedUntil=tick+800;++c.carryControls;}
    }
    c.peerSequence=p.h.sequence;c.peerAt=tick;
}
void receiveNpcPose(const CoopNpcPose& p,ULONGLONG tick){
    if(npcStreamSessionState.menuSuspended||npcStreamSessionState.reloadPending){++npcStreamSessionState.ignored;return;}
    if(!npcStreamContext(tick)||!validNpcPose(p,outgoing,remote)){++npcPacketsRejected;++npcPoseScopeRejected;return;}
    auto it=std::find_if(npcChannels.begin(),npcChannels.end(),[&](const auto& c){return c.enabled&&c.arm==p.h.targetArm;});
    if(it==npcChannels.end()){++npcPacketsRejected;return;}auto& c=*it;++c.received;
    const bool reverse=p.h.kind==NpcAuthorityMessage::PairedPose;
    if(reverse?(localRole||!c.ready||(c.corpse&&tick>=c.carriedUntil&&tick>=c.pairedCorpseUntil)||!(c.hostPairedLease||c.hostPairedFollow||(c.unconscious&&tick-c.unconsciousAt<3500)||tick<c.carriedUntil||tick<c.pairedCorpseUntil)):(!localRole||!c.ready||(!c.lease.ready&&!c.corpse))){++c.rejected;++c.poseRejects[!c.ready?0:1];return;}
    if(p.h.sourceArm!=c.peerArm||p.h.sourceHandle!=c.peerHandle||p.h.targetHandle!=c.lease.binding.npc.handle||
       p.h.rig!=c.rig||p.h.joints!=c.render.joints){++c.rejected;++c.poseRejects[2];return;}
    if(!c.buffer.push(p,presentationClock())){++c.rejected;++c.poseRejects[c.buffer.lastReject==1?3:4];
        if(c.buffer.lastReject==2)c.lastPoseLateMs=presentationClock()-(double(p.sampleUs)*.001+c.buffer.clock.offset);return;}
    c.peerAt=tick;if(reverse){c.reversePoseAt=tick;++c.reversePoses;std::memcpy(c.reverseRoot,p.root,12);
        if(c.corpse&&c.corpseAt&&tick-c.corpseAt>=3000&&tick>=c.carriedUntil&&!c.settledHold){c.settledHold=true;++npcSettledCorpseFollows;}} // 1214hi, 1214hj, 1214hl
    else{c.poseAt=tick;++c.poses;if(p.h.reserved&1){c.peerDownAt=tick;++c.downReceived;}}
}
// 1214en: engine thread, client: while the host's poses say the NPC is down (within 600 ms), keep the replica's native
// laying timer (AIPhysicsReaction +0x138) above half of 0.6 s; clear it once the host stops saying so.
void serviceNpcReplicaDown(NpcAuthorityChannel& c,ULONGLONG tick){
    const bool want=c.ready&&!c.corpse&&!c.unconscious&&c.peerDownAt&&tick-c.peerDownAt<600;
    if((!want&&!c.downApplied)||!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    NativeNpcDown d{};if(!readNativeNpcDown(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),c.lease.binding.npc.actor,d)||!d.reaction){c.downApplied=false;return;}
    auto* timer=reinterpret_cast<float*>(d.reaction+0x138);
    if(want){if(npcDownTimerNeedsRefresh(d.timer)){*timer=kNpcDownLayingSeconds;++c.downApplies;}c.downApplied=true;}
    else{if(d.timer>0)*timer=0;c.downApplied=false;}
}
void sendNpcPose(NpcAuthorityChannel& c,NpcAuthorityMessage kind=NpcAuthorityMessage::Pose){
    NativeSkeleton s{};
    const auto refuse=[&](const char* reason){++c.rejected;++c.poseFailures;c.poseFailure=reason;};
    if(!readNativeSkeleton(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),c.lease.binding.npc.handle,s)){refuse("native_skeleton_read");return;}
    if(!s.stable||s.logic!=1||s.game!=c.lease.binding.health.game||s.actor!=c.lease.binding.npc.actor||s.animation!=c.render.animation){refuse("skeleton_identity");return;}
    if(npcAuthorityRig(s)!=c.rig){refuse("rig_identity");return;}
    CoopNpcPose p{};p.h=npcHeader(c,kind);p.h.kind=kind;p.sampleUs=static_cast<std::uint64_t>(presentationClock()*1000.);
    if(kind==NpcAuthorityMessage::Pose&&!localRole){NativeNpcDown d{};
        if(readNativeNpcDown(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),s.actor,d)&&nativeNpcDown(d)){p.h.reserved=1;c.hostDownUntil=GetTickCount64()+1500;++c.downSent;}}
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
// 1214fk (#117): Shadow Kill turns its victim to ash in the killer's world only (carry-body-v1640a: Corvo's guard vanished
// from his world and stayed dead in Emily's; 1640b the reverse). A dead NPC that vanishes within 8 s of its death sends
// Dead with kNpcDeadAshed; the peer hides its copy (native hide 0x8784b0) and plays native ash effects at its body.
bool startNativePowerFx(unsigned kind,const float* origin,ULONGLONG tick,bool network,const float* axis,unsigned* createdId);
void serviceNpcAsh(ULONGLONG tick){
    if(npcAshPending.empty()||!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    for(const auto& r:npcAshPending){const auto actor=worldIdentityEntity(memoryRead,observedGame,r.handle);
        if(!actor||actor!=r.actor){++npcAshRefused;continue;}
        float at[3]{};if(memoryRead(actor+0x300,at,12)){const float lift[4][3]={{0,0,.3f},{0,0,.6f},{.35f,0,.2f},{-.35f,0,.2f}};
            for(unsigned k=0;k<4;++k){const float p[3]={at[0]+lift[k][0],at[1]+lift[k][1],at[2]+lift[k][2]};if(startNativePowerFx(31+k,p,tick,true,nullptr,nullptr))++npcAshEffects;}}
        reinterpret_cast<void(*)(void*)>(gameImage+0x8784b0)(reinterpret_cast<void*>(actor));++npcAshApplied;}
    npcAshPending.clear();
}
void serviceNpcStream(ULONGLONG tick){
    auto& s=npcStreamSessionState;
    serviceNpcAsh(tick);
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
    // 1213u (#46): the client's own native paired takedown (rear choke 1-3,
    // fast-back lethal 5) and the bound replica it targets.
    unsigned pairedHandle=0,pairedVariant=0,possessionTargetHandle=0;
    if(localRole){NativePlayerStates st{};NativePlayerChokeScene scene{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
        if(readNativePlayerStates(memoryRead,base,st)){
            if(readNativePlayerChokeScene(memoryRead,base,st,scene)&&npcPairedVariant(scene.variant)){pairedHandle=scene.target.handle;pairedVariant=scene.variant;}
            // 1214ga (#140, player_scene_probe.h): a takedown state whose scene is not recognized.
            else if(playerTakedownStateActive(st))probePlayerScene(memoryRead,base,st,"takedown_state_unrecognized",tick);
            // 1214if (#160): her Possession of a bound replica holds the host NPC; the host follows her possessed body.
            if(!pairedHandle){std::uint32_t h=0;if(readNativeHumanPossessionOccupation(memoryRead,base,st,&h)&&h){pairedHandle=h;pairedVariant=kNpcPossessionVariant;}}}
        // 1214ig (#160): her Possession target (aimed, travelling or occupied) owns the replica natively; the ownership
        // watchdog stopped its channel at once (partner-corvo-power-v1798a: ai_ownership_changed, 1 reverse pose sent).
        NativePossessionTarget target{};if(readNativePossessionTarget(memoryRead,base,target)&&target.hasTarget)possessionTargetHandle=target.handle;}
    // 1214ef (#11 enemy_body_size_variants): the host's own native paired takedown. The client's
    // replica follows the victim by this stream alone, and the budget above gave 24 bound NPCs one
    // pose per 400 ms each while the client expires render data at 350 ms: in take 1491a (curator
    // map) every client channel was client_pose_stale and the witch's replica got 1 pose, so it stood
    // still through Corvo's choke. The victim's pose now goes every 50 ms, like the client's own
    // victim (PairedPose), from the scene until 3 s after it.
    unsigned hostPairedHandle=0;
    if(!localRole){NativePlayerStates st{};NativePlayerChokeScene scene{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
        if(readNativePlayerStates(memoryRead,base,st)&&readNativePlayerChokeScene(memoryRead,base,st,scene)&&npcPairedVariant(scene.variant))hostPairedHandle=scene.target.handle;}
    // 1214ae: the bound replica the client carries (carry state 45 +0xf8, drop state 46 +0x38).
    unsigned carriedHandle=0;
    if(localRole){NativePlayerStates st{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
        if(readNativePlayerStates(memoryRead,base,st))for(const auto& a:st.active){unsigned h=0;
            if(a.id==45&&a.vtable==base+0x1c77748&&memoryRead(a.pointer+0xf8,&h,4)&&h){carriedHandle=h;break;}
            if(a.id==46&&a.vtable==base+0x1c790c8&&memoryRead(a.pointer+0x38,&h,4)&&h){carriedHandle=h;break;}}}
    // 1214fj (#127): the host's own carry of a body ends its corpse freeze.
    unsigned hostCarriedHandle=0;
    if(!localRole){NativePlayerStates st{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
        if(readNativePlayerStates(memoryRead,base,st))for(const auto& a:st.active){unsigned h=0;
            if(a.id==45&&a.vtable==base+0x1c77748&&memoryRead(a.pointer+0xf8,&h,4)&&h){hostCarriedHandle=h;break;}
            if(a.id==46&&a.vtable==base+0x1c790c8&&memoryRead(a.pointer+0x38,&h,4)&&h){hostCarriedHandle=h;break;}}}
    for(auto& c:npcChannels){if(!c.enabled)continue;
        if(tick<c.started)continue;
        if(npcStreamPeerExpired(tick,c.started,c.peerAt,c.peerArm!=0)){stopNpcChannel(c,"peer_timeout");continue;}
        if(!localRole&&c.ready&&hostPairedHandle&&c.lease.binding.npc.handle==hostPairedHandle){if(tick>=c.hostOwnPairedUntil)++npcHostOwnPaired;c.hostOwnPairedUntil=tick+3000;}
        if(localRole&&c.ready&&pairedHandle&&c.lease.binding.npc.handle==pairedHandle){
            if(!c.localPaired){c.localPaired=true;++npcPairedLocal;c.pairedResult="client_paired";}
            c.pairedVariant=pairedVariant;c.pairedUntil=tick+1500;
            if(tick-c.pairedHoldAt>=200){c.pairedHoldAt=tick;sendNpcControl(c,NpcAuthorityMessage::PairedHold,pairedVariant);}
            if(npcPairedKnockoutVariant(pairedVariant)&&!c.pairedKnockoutAt){c.pairedKnockoutAt=tick;sendNpcControl(c,NpcAuthorityMessage::PairedKnockout,pairedVariant);c.pairedResult="client_knockout_sent";}
        // 1214v: a corpse ends the pairing kept for an unconscious victim, so the host's
        // corpse stream drives it (take 1271l: a client-choked guard killed later lay 8.84 m
        // apart, the client ragdoll never following the host body).
        }else if(localRole&&c.localPaired&&(!c.unconscious||c.corpse)&&tick>=c.pairedUntil){c.localPaired=false;c.pairedResult=c.corpse?"client_paired_corpse":"client_paired_ended";}
        if(!localRole)serviceHostNpcPaired(c,tick);
        if(localRole&&c.ready&&carriedHandle&&c.lease.binding.npc.handle==carriedHandle)c.localCarryUntil=tick+2500;
        if(!localRole&&hostCarriedHandle&&c.lease.binding.npc.handle==hostCarriedHandle&&!c.corpseFreezeEnded){c.corpseFreezeEnded=true;++npcCorpseFreezeEnds;}
        if(!localRole&&c.settledHold&&((hostCarriedHandle&&c.lease.binding.npc.handle==hostCarriedHandle)||tick<c.carriedUntil))c.settledHold=false; // 1214hj: a carry ends it
        if(!localRole&&c.settledHold){const auto r=remoteHeld;if(r.phase&&r.handle==c.lease.binding.npc.handle&&r.at&&tick-r.at<=600){c.settledHold=false;c.corpseFreezeEnded=false;++npcSettledHoldPulls;}} // 1214hk
        if(localRole&&c.ready&&tick<c.localCarryUntil){
            if(tick-c.carryControlAt>=200){c.carryControlAt=tick;if(sendNpcControl(c,NpcAuthorityMessage::Carry))++c.carryControls;}
            if(tick-c.reverseSentAt>=50){c.reverseSentAt=tick;sendNpcPose(c,NpcAuthorityMessage::PairedPose);++c.reversePoses;++c.carryPoses;}}
        // 1214ff: the animated hold ends with its window. Kept after a client's lethal takedown, the host's corpse stood up
        // in its idle animation once the client's victim poses stopped (mission-coverage-v1628a, Corvo's window); only a
        // stopped channel released it. It now falls from the last presented pose when the window ends, or when the
        // client's poses have stopped for 300 ms outside a carry.
        const bool corpseWindow=c.ready&&(tick<c.carriedUntil||(c.corpse&&tick<c.pairedCorpseUntil)||(c.corpse&&c.settledHold));
        if(!localRole&&corpseWindow&&(c.settledHold||!(c.reversePoseAt&&tick-c.reversePoseAt>=300&&tick>=c.carriedUntil)))npcCorpseHoldAnimated(c);
        else if(!localRole&&c.corpsePhysics&&!c.victimHeld&&!npcCarrySuppressed(c,tick)){npcPhysicsRelease(c);++npcCorpseHoldReleases;}
        // 1214eo (#11): a client's knocked-down choke (64-67) knocks the host NPC out at the scene start, and its ragdoll
        // ignored the client's victim pose (take 1535a: 58 victim bodies applied, the host body never moved). The body is
        // held animated while the victim poses arrive and falls again 400 ms after the last one.
        if(!localRole&&c.ready&&!c.corpse&&c.unconscious&&nativeTakedownKnockdownChoke(c.pairedVariant)&&c.reversePoseAt){
            if(tick-c.reversePoseAt<350){if(!c.victimHeld&&npcCorpseHoldAnimated(c,true)){c.victimHeld=true;++c.victimHolds;}}
            else if(c.victimHeld&&tick-c.reversePoseAt>=400){npcPhysicsRelease(c);c.victimHeld=false;++c.victimReleases;}}
        if(localRole&&c.localPaired&&c.ready&&(!c.unconscious||tick-c.unconsciousAt<3000)&&tick-c.reverseSentAt>=50){c.reverseSentAt=tick;sendNpcPose(c,NpcAuthorityMessage::PairedPose);++c.reversePoses;}
        // 1214hh (#140): her own paired kill's native corpse keeps the host corpse following it until it settles.
        if(localRole&&c.ownPairedCorpse&&c.corpse&&c.ready&&!c.localPaired&&tick<c.ownPairedCorpseUntil&&tick>=c.localCarryUntil){
            if(tick-c.pairedHoldAt>=200){c.pairedHoldAt=tick;sendNpcControl(c,NpcAuthorityMessage::PairedHold,c.pairedVariant);}
            if(tick-c.reverseSentAt>=50){c.reverseSentAt=tick;sendNpcPose(c,NpcAuthorityMessage::PairedPose);++c.reversePoses;++npcOwnPairedCorpseStreamPoses;}}
        if(!localRole&&c.unconscious&&c.ready&&c.unconsciousSends<5&&tick-c.unconsciousSentAt>=500){c.unconsciousSentAt=tick;if(sendNpcControl(c,NpcAuthorityMessage::Unconscious))++c.unconsciousSends;}
        if(!localRole&&c.corpse&&c.ready&&c.corpseAt&&c.deadSends<5){const unsigned share=npcCombatDeathShare(c.arm);
            if(share==2&&!std::strcmp(c.deathShare,"none")){c.deathShare="combat";++npcCombatSharedDeaths;}
            if(npcDeadReconcileDue(true,c.corpse,c.ready,c.corpseAt,tick,share,c.deadSends,c.deadSentAt)&&std::strcmp(c.deathShare,"combat")){
                c.deadSentAt=tick;if(sendNpcControl(c,NpcAuthorityMessage::Dead)){if(!c.deadSends++)++npcDeadReconciled;++npcDeadSent;c.deathShare="reconcile_sent";}}}
        if(tick-c.lastCheck>=100){c.lastCheck=tick;NativeNpcAuthority fresh{};
            const bool freshRead=readNativeNpcAuthority(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,c.lease.binding.npc.handle,fresh);
            const bool same=freshRead&&sameNpcAuthorityIdentity(c.lease.binding,fresh,false);
            const bool dead=same&&fresh.health.logic==1&&std::isfinite(fresh.health.current)&&fresh.health.current<=0&&fresh.health.maximum>0;
            if(!same||(!npcAuthorityLiving(fresh)&&!dead)){
                if(localRole){if(auto* t=npcCorpseTraceFor(static_cast<unsigned>(c.arm),c.lease.binding.npc.handle)){if(!t->vanished){t->vanished=true;t->vanishedMs=static_cast<unsigned>(tick-t->startedAt);}}} // 1214gd
                // 1214fb (#118): a killed gravehound's entity is removed (take 1612b: Corvo's native assassination, then the
                // host NPC was absent and the channel ended without a Dead, so Emily's copy lived on). A host NPC that
                // vanishes within 10 s of the host's own takedown on it, or while it follows the client's, is dead: the
                // client's copy gets the native kill too.
                const bool ashed=c.ready&&!freshRead&&(c.corpse||(c.lease.binding.health.current<=0&&c.lease.binding.health.maximum>0))&&(!c.corpseAt||tick-c.corpseAt<8000);
                if(ashed&&sendNpcControl(c,NpcAuthorityMessage::Dead,kNpcDeadAshed)){++npcAshSent;c.deathShare="ash_sent";}
                else if(!localRole&&c.ready&&!freshRead&&!c.corpse&&(tick<c.hostOwnPairedUntil+7000||c.hostPairedLease||c.hostPairedFollow)&&
                   sendNpcControl(c,NpcAuthorityMessage::Dead)){++npcDeadSent;++npcVanishedDead;c.deathShare="vanished_dead_sent";}
                c.lifecycleDiff=!freshRead?0x80000000u:((npcAuthorityIdentityDiff(c.lease.binding,fresh)&~0xe00u)|(!npcAuthorityLiving(fresh)?0x40000000u:0u));
                stopNpcChannel(c,"native_lifecycle_changed");continue;}
            c.lease.binding.health=fresh.health;
            if(!localRole&&!c.unconscious&&(fresh.health.ownerFlags&0x20)&&fresh.health.current>0){c.unconscious=true;c.result="host_unconscious";}
            if(dead&&!c.corpse&&c.ready){c.corpse=true;c.corpseAt=tick;c.corpseUntil=tick+8000;c.result=localRole?"client_corpse":"host_corpse";
                if(localRole&&(c.localPaired||tick<c.pairedUntil)&&!c.ownPairedCorpse){c.ownPairedCorpse=true;c.ownPairedCorpseUntil=tick+kNpcOwnPairedCorpseStreamMs;++npcOwnPairedCorpses;} // 1214hh
                if(localRole)npcCorpseTraceBegin(static_cast<unsigned>(c.arm),c.lease.binding.npc.handle,tick,c.localPaired||tick<c.pairedUntil);} // 1214gd
            if(c.corpse){
                // 1113d: corpses stay host-streamed (budgeted) for the stream lifetime; releasing
                // the client ragdoll mid-session moved it 0.27-0.86 m (1116).
                // 1214ak (#9): not during the client's own paired scene. The host's kill reaches
                // the client mid-finisher, and holding the replica animated while its native
                // ragdoll starts launched the body 7-26 m up (takes 1256f/1256g, 1258l and
                // finisher-follow-v1258a/f; the host now follows that pose, so both bodies ended
                // at z 75). The hold applies when the paired window ends.
                // 1214gd (#140): trace her corpse; optionally leave the corpse of her own paired kill to native physics.
                auto* trace=localRole?npcCorpseTraceFor(static_cast<unsigned>(c.arm),c.lease.binding.npc.handle):nullptr;
                if(trace){float root[3]{};const bool rootRead=memoryRead(c.lease.binding.npc.actor+0x300,root,12);
                    npcCorpseTraceSample(*trace,tick,rootRead?root:nullptr,c.corpsePhysics!=0,c.localPaired,npcCorpsePhysicsMode(c));}
                if(localRole&&!c.localPaired){
                    // 1214hh: no hold while her own paired corpse streams to the host (the lab switch skips it for good).
                    if(c.ownPairedCorpse&&(npcSkipOwnPairedCorpseHold||tick<c.ownPairedCorpseUntil)){if(trace&&!trace->holdSkipped){trace->holdSkipped=true;++npcOwnPairedHoldSkips;}}
                    else npcCorpseHoldAnimated(c);}
            }
            else if(localRole&&(c.unconscious||(c.localPaired&&(fresh.health.ownerFlags&0x20)&&fresh.health.current>0))){
                if(!c.unconscious){c.unconscious=true;c.unconsciousAt=tick;c.pairedResult="client_unconscious_native";}c.result="client_unconscious";}
            else if(localRole&&c.lease.references){const bool paused=npcAuthorityPaused(fresh);
                const bool possessed=(c.localPaired&&c.pairedVariant==kNpcPossessionVariant&&tick<c.pairedUntil)||
                    (possessionTargetHandle&&c.lease.binding.npc.handle==possessionTargetHandle); // 1214ig (#160)
                if(possessed){if(!paused)c.result="client_possessed";}
                // 1214iu: a forced lease waits up to 4 s for the running attack to end.
                else{if(!paused&&(c.lease.ready||tick-c.started>(c.forcedLease?4000u:1500u))){const bool never=c.forcedLease&&!c.lease.ready;if(never)++npcClientForcedStopped;
                        stopNpcChannel(c,never?"forced_lease_never_paused":"ai_ownership_changed");continue;}
                    if(paused&&c.forcedLease&&!c.ready)++npcClientForcedReady;
                    c.lease.ready=paused;if(paused){c.ready=true;c.result="client_replica";}}}
        }
        if(tick-c.helloAt>=500){c.helloAt=tick;sendNpcControl(c,NpcAuthorityMessage::Hello);}
        if(localRole&&c.ready&&tick-c.readyAt>=500){c.readyAt=tick;sendNpcControl(c,NpcAuthorityMessage::Ready);}
        if(localRole)serviceNpcReplicaDown(c,tick);
        const bool priority=!localRole&&(tick<c.hostOwnPairedUntil||tick<c.hostDownUntil);
        if(!localRole&&c.ready&&tick-c.lastSend>=(priority?50:poseInterval)){if(!priority&&bulkTransportCongested(tick))++bulkCongestionSkips;else{c.lastSend=tick;sendNpcPose(c);if(priority){++c.priorityPoses;++npcPriorityPoses;}}}
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
std::uint64_t npcHeldPresentSkips=0;
void presentNpcStream(ULONGLONG tick){
    if(!npcStreamContext(tick)){clearNpcPresentation();return;}
    std::vector<NpcRenderState> next;
    {auto& s=npcCarrySuppression;if(s.handle&&s.resumeAt&&tick>=s.resumeAt){s.handle=0;s.actor=0;s.resumeAt=0;}}
    for(auto& c:npcChannels){
        if(npcCarrySuppressed(c,tick)){++npcCarrySuppression.presentSkips;if(c.corpse&&!c.corpseFreezeEnded){c.corpseFreezeEnded=true;++npcCorpseFreezeEnds;}continue;}
        // 1214fe (#115): a body the client's Far Reach is pulling is reported as its held object (flight and rest); the
        // host's pose must not pull the client's copy back meanwhile.
        if(localRole&&shadowHeld.character&&shadowHeld.phase&&shadowHeld.handle==c.lease.binding.npc.handle){++npcHeldPresentSkips;continue;}
        const bool hostVictim=!localRole&&c.enabled&&c.ready&&c.reversePoseAt&&tick-c.reversePoseAt<350;
        const bool hostCorpseFreeze=!hostVictim&&!localRole&&c.enabled&&c.ready&&c.corpse&&!c.corpseFreezeEnded&&c.reversePoses&&c.reversePoseAt&&!c.corpsePhysics;
        const bool hostSettled=!hostVictim&&!localRole&&c.enabled&&c.ready&&c.corpse&&c.settledHold&&c.corpsePhysics&&c.reversePoses; // 1214hj
        if(!hostVictim&&!hostCorpseFreeze&&!hostSettled&&(!localRole||!c.enabled||!c.ready||c.localPaired||tick<c.localCarryUntil||(!c.lease.ready&&!c.corpse)||!c.poseAt||tick-c.poseAt>=350))continue;
        if(localRole&&c.corpse&&!c.corpsePhysics&&!npcPresentUnheldCorpse){++npcUnheldCorpsePresentSkips;continue;} // 1214hg (#140)
        NpcRenderState p{};p.id=c.render;p.id.corpse=c.corpse;p.id.unconscious=c.unconscious;p.id.possessed=hostVictim&&c.pairedVariant==kNpcPossessionVariant&&tick<c.pairedUntil;p.until=(hostVictim?c.reversePoseAt:c.poseAt)+350;p.arm=c.arm;
        if(hostCorpseFreeze){if(!c.buffer.latest(p.pose)||!npcRenderCurrent(p.id))continue;p.until=tick+350;p.sequence=p.pose.h.sequence;++npcCorpseFreezeFrames;next.push_back(std::move(p));continue;}
        if(hostSettled){if(!c.buffer.latest(p.pose)||!npcRenderCurrent(p.id))continue;p.until=tick+350;p.sequence=p.pose.h.sequence;
            reinterpret_cast<void(*)(void*,const float*,const float*,bool,int)>(gameImage+0x87d200)(reinterpret_cast<void*>(c.lease.binding.npc.actor),p.pose.root,p.pose.root+3,true,0);
            ++npcSettledHoldFrames;next.push_back(std::move(p));continue;}
        // 1214ii (#160): partner-corvo-power-v1800a accepted 191 of her possession reverse poses but never presented one
        // (presentedRoot 0); each refusal of a host victim frame is counted.
        if(hostVictim){++npcHostVictimFrames;if(!c.buffer.sample(presentationClock(),p.pose)){++npcHostVictimSampleRefused;continue;}
            if(!npcRenderCurrent(p.id)){++npcHostVictimRenderRefused;continue;}}
        else if(!c.buffer.sample(presentationClock(),p.pose)||!npcRenderCurrent(p.id))continue;
        p.sequence=p.pose.h.sequence;
        reinterpret_cast<void(*)(void*,const float*,const float*,bool,int)>(gameImage+0x87d200)(reinterpret_cast<void*>(c.lease.binding.npc.actor),p.pose.root,p.pose.root+3,true,0);
        ++c.roots;if(hostVictim)std::memcpy(c.presentedRoot,p.pose.root,12);next.push_back(std::move(p));
    }
    AcquireSRWLockExclusive(&npcPresentationLock);npcPresentation=std::move(next);ReleaseSRWLockExclusive(&npcPresentationLock);
}
void writeNpcStream(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.npc-stream.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;
    fprintf(f,"{\"enabled\":%s,\"result\":\"%s\",\"epoch\":%llu,\"peerEpoch\":%llu,\"rejected\":%llu,\"bodyApplied\":%llu,\"bodyRefused\":%llu,\"framesOwned\":%llu,\"childrenUpdated\":%llu,\"poseIntervalMs\":%u,\"heldSkips\":%llu,\"corpseReleases\":%llu,\"corpseFreezeFrames\":%llu,\"corpseFreezeEnds\":%llu,\"ashSent\":%llu,\"ashApplied\":%llu,\"ashRefused\":%llu,\"ashEffects\":%llu,\"hostVictim\":[%llu,%llu,%llu],\"entries\":[",npcAuthorityStreamActive?"true":"false",npcStreamResult,npcStreamEpoch,npcStreamPeerEpoch,npcPacketsRejected,npcBodiesApplied.load(),npcBodiesRefused.load(),npcFramesOwned.load(),npcChildUpdates.load(),npcPoseIntervalMs,static_cast<unsigned long long>(npcHeldPresentSkips),static_cast<unsigned long long>(npcCorpseHoldReleases),static_cast<unsigned long long>(npcCorpseFreezeFrames),static_cast<unsigned long long>(npcCorpseFreezeEnds),static_cast<unsigned long long>(npcAshSent),static_cast<unsigned long long>(npcAshApplied),static_cast<unsigned long long>(npcAshRefused),static_cast<unsigned long long>(npcAshEffects),static_cast<unsigned long long>(npcHostVictimFrames),static_cast<unsigned long long>(npcHostVictimSampleRefused),static_cast<unsigned long long>(npcHostVictimRenderRefused));
    for(unsigned i=0;i<npcChannels.size();++i){const auto& c=npcChannels[i];fprintf(f,"%s{\"handle\":%u,\"civilian\":%s,\"arm\":%llu,\"peerArm\":%llu,\"peerHandle\":%u,\"enabled\":%s,\"ready\":%s,\"result\":\"%s\",\"joints\":%u,\"rig\":\"%llx\",\"references\":%u,\"released\":%llu,\"lost\":%llu,\"sent\":%llu,\"received\":%llu,\"rejected\":%llu,\"poses\":%llu,\"roots\":%llu,\"lifecycleDiff\":%u,\"localPaired\":%s,\"unconscious\":%s,\"hostPairedLease\":%s,\"pairedVariant\":%u,\"pairedResult\":\"%s\",\"reversePoses\":%llu,\"reverseRoot\":[%.3f,%.3f,%.3f],\"presentedRoot\":[%.3f,%.3f,%.3f],\"corpse\":%s,\"deathShare\":\"%s\",\"deadSends\":%u,\"carryControls\":%llu,\"carryPoses\":%llu,\"priorityPoses\":%llu,\"downSent\":%llu,\"downReceived\":%llu,\"downApplies\":%llu,\"downApplied\":%s,\"victimHolds\":%llu,\"victimReleases\":%llu,\"leaseReady\":%s,\"poseRejects\":[%llu,%llu,%llu,%llu,%llu],\"lastPoseLateMs\":%.1f}",i?",":"",c.lease.binding.npc.handle,npcCivilian(c.lease.binding)?"true":"false",c.arm,c.peerArm,c.peerHandle,c.enabled?"true":"false",c.ready?"true":"false",c.result,c.render.joints,c.rig,c.lease.references,c.lease.released,c.lease.lost,c.sent,c.received,c.rejected,c.poses,c.roots,c.lifecycleDiff,c.localPaired?"true":"false",c.unconscious?"true":"false",c.hostPairedLease?"true":"false",c.pairedVariant,c.pairedResult,static_cast<unsigned long long>(c.reversePoses),c.reverseRoot[0],c.reverseRoot[1],c.reverseRoot[2],c.presentedRoot[0],c.presentedRoot[1],c.presentedRoot[2],c.corpse?"true":"false",c.deathShare,c.deadSends,static_cast<unsigned long long>(c.carryControls),static_cast<unsigned long long>(c.carryPoses),static_cast<unsigned long long>(c.priorityPoses),static_cast<unsigned long long>(c.downSent),static_cast<unsigned long long>(c.downReceived),static_cast<unsigned long long>(c.downApplies),c.downApplied?"true":"false",static_cast<unsigned long long>(c.victimHolds),static_cast<unsigned long long>(c.victimReleases),c.lease.ready?"true":"false",
        static_cast<unsigned long long>(c.poseRejects[0]),static_cast<unsigned long long>(c.poseRejects[1]),static_cast<unsigned long long>(c.poseRejects[2]),static_cast<unsigned long long>(c.poseRejects[3]),static_cast<unsigned long long>(c.poseRejects[4]),c.lastPoseLateMs);}
    fprintf(f,"],\"bodyEntries\":[");for(unsigned i=0;i<npcChannels.size();++i)fprintf(f,"%s{\"arm\":%llu,\"handle\":%u,\"applied\":%llu,\"sequence\":%llu}",i?",":"",npcChannels[i].arm,npcChannels[i].lease.binding.npc.handle,npcActorBodyApplied[i].load(),npcActorBodySequence[i].load());
    fputs("],\"poseDiagnostics\":[",f);for(unsigned i=0;i<npcChannels.size();++i){const auto& c=npcChannels[i];fprintf(f,"%s{\"arm\":%llu,\"handle\":%u,\"stale\":%s,\"gaps\":%llu,\"recoveries\":%llu,\"failures\":%llu,\"lastFailure\":\"%s\",\"joint\":%d,\"matrix\":[",i?",":"",c.arm,c.lease.binding.npc.handle,c.poseStale?"true":"false",c.poseGaps,c.poseRecoveries,c.poseFailures,c.poseFailure,c.poseFailureJoint);
        for(unsigned j=0;j<12;++j)fprintf(f,"%s%.9g",j?",":"",c.poseFailureMatrix[j]);fputs("],\"root\":[",f);
        for(unsigned j=0;j<12;++j)fprintf(f,"%s%.9g",j?",":"",c.poseFailureRoot[j]);fputs("]}",f);
    }
    SteamNetConnectionRealTimeStatus_t status{};const bool qos=sockets&&connection&&sockets->GetConnectionRealTimeStatus(connection,&status,0,nullptr)==k_EResultOK;
    const auto& s=npcStreamSessionState;
    fprintf(f,"],\"relevance\":{\"bound\":%u,\"scans\":%llu,\"added\":%llu,\"dropped\":%llu,\"evicted\":%llu,\"lifeSent\":%llu,\"lifeReceived\":%llu,\"lifeApplied\":%llu,\"lifeRefused\":%llu,\"clientBound\":%llu,\"clientRefused\":%llu,\"clientDeferred\":%llu,\"clientBoundLive\":%llu,\"clientBusy\":%llu,\"helloCatalogReads\":%llu,\"pairedHolds\":%llu,\"pairedKnockouts\":%llu,\"pairedRefused\":%llu,\"pairedLocal\":%llu,\"pairedCorpses\":%llu,\"pairedFollows\":%llu,\"hostOwnPaired\":%llu,\"priorityPoses\":%llu,\"deadReconciled\":%llu,\"deadSent\":%llu,\"vanishedDead\":%llu,\"combatSharedDeaths\":%llu,\"clientDeadApplied\":%llu,\"clientDeadAlready\":%llu,\"clientDeadRefused\":%llu,\"clientDeadLast\":\"%s\",\"reconcileDecl\":[%.3f,%llu,%u,%u],\"catalogStale\":%llu,\"workerRequests\":%llu,\"workerCompleted\":%llu,\"workerFailed\":%llu,\"workerLastMs\":%.3f,\"workerMaxMs\":%.3f",npcEnabledChannels(),npcRelevanceScans,npcRelevanceAdded,npcRelevanceDropped,npcRelevanceEvicted,npcLifeSent,npcLifeReceived,npcLifeApplied,npcLifeRefused,npcClientBound,npcClientBindRefused,npcClientBindDeferred,npcClientBoundLive,npcClientBindBusy,npcHelloCatalogReads,npcPairedHolds,npcPairedKnockouts,npcPairedRefused,npcPairedLocal,npcPairedCorpses,npcPairedFollows,npcHostOwnPaired,npcPriorityPoses,npcDeadReconciled,npcDeadSent,npcVanishedDead,npcCombatSharedDeaths,npcClientDeadApplied,npcClientDeadAlready,npcClientDeadRefused,npcClientDeadLast,npcReconcileDeclAmount,static_cast<unsigned long long>(npcReconcileDeclFlags),npcReconcileDeclReactions[0],npcReconcileDeclReactions[1],npcCatalogStale,npcCatalogWorker.requests,npcCatalogWorker.completed,npcCatalogWorker.failed,npcCatalogWorker.lastMs,npcCatalogWorker.maxMs);
    fprintf(f,"},\"transport\":{\"available\":%s,\"sendRate\":%d,\"pendingUnreliable\":%d,\"pendingReliable\":%d,\"queueUs\":%lld},\"decodeRejected\":%llu,\"poseScopeRejected\":%llu,",qos?"true":"false",status.m_nSendRateBytesPerSecond,status.m_cbPendingUnreliable,status.m_cbPendingReliable,status.m_usecQueueTime,npcDecodeRejected,npcPoseScopeRejected);
    fprintf(f,"\"session\":{\"enabled\":%s,\"result\":\"%s\",\"reloadPending\":%s,\"reloadReady\":%s,\"reloads\":%llu,\"rearms\":%llu,\"menuSuspended\":%s,\"menuSuspensions\":%llu,\"menuResumes\":%llu,\"menuPausedMs\":%llu,\"ignored\":%llu,\"acquired\":%llu,\"released\":%llu,\"retired\":%llu,\"lost\":%llu}",s.enabled?"true":"false",s.result,s.reloadPending?"true":"false",s.reloadReady?"true":"false",s.reloads,s.rearms,s.menuSuspended?"true":"false",s.menuSuspensions,s.menuResumes,s.menuPausedMs,s.ignored,s.acquired,s.released,s.retired,s.lost);
    writeNpcCorpseTraces(f);writeNpcStreamTraces(f);fputs("}\n",f); // 1214gd (#140), 1214it (#180)
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
