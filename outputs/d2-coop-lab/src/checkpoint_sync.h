#pragma once
#include "checkpoint_protocol.h"
#include "checkpoint_barrier_policy.h"
#include "checkpoint_fixture.h"
#include "checkpoint_mission.h"
#include "autosave_pair.h"
// Opt-in, fixed-slot Dunwall experiment. All state is owned by the native
// engine thread. Packet reception schedules loads; it never calls LoadGame.
struct CheckpointSync {
    bool enabled=false,queueLoad=false,permitLoad=false,permitContinuation=false,expectBoundary=false;
    bool held=false,readySent=false,completeSent=false,peerComplete=false;
    bool deathActive=false,deathPending=false,deathReady=false,peerDeathReady=false,deathRetry=false;
    bool avatarRequested=false,avatarPending=false,avatarReady=false;
    bool avatarShadow=false,avatarExact=false; // Captured at explicit session arm; no native pointers survive the load.
    std::uint64_t avatarRestored=0,avatarRefused=0,avatarSpawnedAt=0;
    CheckpointPacket deathKey{};
    std::uint64_t deathDeadline=0,deathCount=0,deathApplied=0,deathReceived=0;
    CheckpointPhase phase=CheckpointPhase::Idle;
    CheckpointPacket key{};
    unsigned char digest[32]{};
    std::uint64_t arm=0,peerArm=0,peerSession=0,peerSequence=0,helloAt=0,helloSent=0;
    std::uint64_t deadline=0,cooldown=0,heldAt=0,releasedAt=0,readyAt=0,completedAt=0;
    std::uint64_t hostReady=0,clientReady=0,loads=0,completed=0,requests=0,duplicates=0,rejected=0,aborted=0;
    std::uint64_t sent=0,received=0,samples=0;double maxDistance=0;
    std::uintptr_t oldGame=0,game=0,player=0,manager=0;
    std::uintptr_t loadingGame=0;std::uint64_t loadingEntryAt=0;
    std::uint64_t slotRetryCalls=0,continuationCalls=0,continuationDeferred=0,continuationAccepted=0;
    std::uint64_t retryProbeUntil=0,retryProbeArm=0,retryProbeEpoch=0,retryProbeInjected=0;
    float position[3]{};unsigned readyDelay=0;
    const char* result="disabled";
};
CheckpointSync checkpointSync{};
// 1214hy (#159): the character the restored world must hold (the player's own, captured when the restore starts;
// any pairing), instead of the role's default.
int checkpointExpectCharacter=0;
bool doorCheckpointBarrierActive(){
    return checkpointBlocksWorldActions(checkpointSync.enabled,checkpointSync.phase,checkpointSync.deathActive,
        outgoing.gameLogicState,outgoing.pendingGameLogicState,remote.gameLogicState,remote.pendingGameLogicState);
}
bool checkpointOwnsLoading(){return checkpointSync.enabled&&checkpointSync.expectBoundary;}
void checkpointObserveLoadingEntry(const NativeSwitchEvent& e) {
    auto& s=checkpointSync;
    if(s.enabled&&s.expectBoundary&&!s.permitLoad&&!s.loadingEntryAt) {
        // A cached LoadGame can enter directly without the asynchronous
        // RestartFromMemoryCheckpoint continuation used on a cold load.
        s.permitContinuation=false;s.loadingGame=e.game;s.loadingEntryAt=e.tick;
    }
}
std::uint64_t checkpointArmSequence=0,checkpointTransaction=0,checkpointPacketSequence=0,checkpointReadySequence=0;
bool checkpointConnected() {return sockets&&connection&&connectionState==k_ESteamNetworkingConnectionState_Connected;}
std::uint64_t checkpointMapId(){return checkpointMission.mapId?checkpointMission.mapId:coopMapId(kGuardMap);}
// 1147: mission sessions identify the per-character autosave pair; Dunwall
// keeps the fixed SLOT82 file digest.
bool checkpointCurrentDigest(unsigned char digest[32]){return checkpointMission.mission?missionCheckpointDigest(digest):checkpointDigest(digest);}
bool checkpointWorldReady(const CoopPosePacket& p) {
    return (p.flags&1)&&p.worldEpoch&&p.mapId==checkpointMapId()&&
        (p.gameLogicState==1||p.gameLogicState==2||p.gameLogicState==13)&&!p.pendingGameLogicState;
}
CheckpointPacket checkpointScope(bool epochs) {
    auto& s=checkpointSync;CheckpointPacket p{};
    p.hostSession=localRole?remote.session:outgoing.session;p.clientSession=localRole?outgoing.session:remote.session;
    p.hostArm=localRole?s.peerArm:s.arm;p.clientArm=localRole?s.arm:s.peerArm;
    if(epochs){p.hostEpoch=localRole?remote.worldEpoch:outgoing.worldEpoch;p.clientEpoch=localRole?outgoing.worldEpoch:remote.worldEpoch;}
    p.mapId=checkpointMapId();std::memcpy(p.digest,s.digest,32);return p;
}
bool checkpointSend(CheckpointMessage kind) {
    auto& s=checkpointSync;if(!checkpointConnected())return false;
    auto p=kind==CheckpointMessage::Hello?checkpointScope(false):s.key;
    if(kind==CheckpointMessage::Death||kind==CheckpointMessage::DeathReady)p=s.deathKey;
    p.kind=kind;p.role=localRole;p.sequence=++checkpointPacketSequence;p.character=0;p.hostReady=0;p.clientReady=0;
    if(kind==CheckpointMessage::Ready){p.character=localCharacter();if(localRole)p.clientReady=s.clientReady;else p.hostReady=s.hostReady;}
    if(kind==CheckpointMessage::DeathReady)p.character=localCharacter();
    if(kind==CheckpointMessage::Release||kind==CheckpointMessage::Complete){p.hostReady=s.hostReady;p.clientReady=s.clientReady;}
    const bool ok=coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)==k_EResultOK;
    if(ok)++s.sent;return ok;
}
bool checkpointFixtureMatches() {
    unsigned char digest[32]{};
    if(!checkpointCurrentDigest(digest)||std::memcmp(digest,checkpointSync.digest,32))return false;
    // 1153: on missions both peers must also hold paired autosaves.
    return !checkpointMission.mission||autosavePairReady();
}
bool checkpointPeerFresh(std::uint64_t tick) {
    const auto& s=checkpointSync;
    return s.enabled&&checkpointConnected()&&lastReceive&&tick-lastReceive<1500&&s.helloAt&&tick-s.helloAt<1500&&
        s.peerSession==remote.session&&s.peerArm;
}
bool checkpointPairReady(std::uint64_t tick) {
    return checkpointPeerFresh(tick)&&checkpointWorldReady(outgoing)&&checkpointWorldReady(remote)&&
        guardLink.phase==GuardPhase::Idle&&!aiDecisionHold.expires&&!cinematicHold.id&&!loadingProbe.held&&!loadingProbe.armed;
}
std::uint64_t checkpointFailedAt=0,checkpointRecoveries=0;
void checkpointFail(const char* reason,bool notify=true) {
    auto& s=checkpointSync;if(s.phase==CheckpointPhase::Failed)return;
    if(doorSession.enabled)stopDoorSession("checkpoint_failed");
    if(coinSync.enabled)stopCoinSync("checkpoint_failed");
    if(notify&&s.key.transaction)checkpointSend(CheckpointMessage::Abort);
    s.phase=CheckpointPhase::Failed;s.queueLoad=false;s.avatarPending=false;if(!s.expectBoundary)s.permitLoad=false;s.result=reason;++s.aborted;checkpointFailedAt=GetTickCount64();
    consoleOutput("COOP: checkpoint failed transaction=%llu reason=%s\n",s.key.transaction,reason);
}
void checkpointEnable(bool on) {
    auto& s=checkpointSync;
    if(!on) {
        if(s.expectBoundary||s.held||s.queueLoad){++s.rejected;return;}
        if(s.phase!=CheckpointPhase::Idle&&s.phase!=CheckpointPhase::Failed)checkpointFail("cancelled");
        s.enabled=false;s.phase=CheckpointPhase::Idle;s.peerArm=0;s.helloAt=0;s.result="disabled";s.retryProbeUntil=0;stopAutosavePair("disabled");
        s.avatarRequested=s.avatarPending=s.avatarReady=false;
        s.deathActive=s.deathPending=s.deathReady=s.peerDeathReady=s.deathRetry=false;return;
    }
    if(s.enabled){++s.rejected;return;}
    if(!armCheckpointMission()){++s.rejected;s.result="map_scope_unavailable";return;}
    // The pause recovery API is validated before arming, never discovered in
    // the middle of an interrupted native finish-load call.
    const unsigned char commit[]={0x40,0x56,0x57,0x41,0x56,0x48,0x83,0xec,0x30,0x48,0xc7,0x44,0x24,0x20,0xfe,0xff,0xff,0xff};
    const unsigned char memoryStart[]={0x48,0x83,0xec,0x28,0x48,0x8b,0x05,0x85,0x77,0x34,0x03};
    const unsigned char retryReturn[]={0x32,0xc0,0x4c,0x8d,0x9c,0x24,0x20,0x9f,0,0};
    const unsigned char acceptedReturn[]={0x0f,0xb6,0xc3,0xeb,0x6e};
    if(!realLogicSwitch||!realReplacementUpdate||checkpointHookMask!=7||!checkpointWorldReady(outgoing)||
       std::memcmp(gameImage+0x787510,commit,sizeof(commit))||loadingProbe.armed||loadingProbe.held||
       guardLink.phase!=GuardPhase::Idle||aiDecisionHold.expires||cinematicHold.id||!checkpointCurrentDigest(s.digest)) {
        ++s.rejected;s.result="enable_refused";return;
    }
    if(std::memcmp(gameImage+0x65db00,memoryStart,sizeof(memoryStart))||
       std::memcmp(gameImage+0x65997c,retryReturn,sizeof(retryReturn))||
       std::memcmp(gameImage+0x65990b,acceptedReturn,sizeof(acceptedReturn))) {
        ++s.rejected;s.result="native_continuation_signature_changed";return;
    }
    bool shadow=false,exact=false;
    const bool wantsAvatar=probeActor&&probeFollow;
    if(wantsAvatar&&!captureCheckpointPresentation(shadow,exact)){++s.rejected;s.result="avatar_scope_unavailable";return;}
    s.avatarShadow=shadow;s.avatarExact=exact;
    s.enabled=true;s.phase=CheckpointPhase::Idle;s.arm=++checkpointArmSequence;s.peerArm=0;s.peerSession=0;s.peerSequence=0;
    // A fresh arm has a new packet scope. A restarted host's transaction
    // counter may begin at one; do not compare it with the previous scope.
    s.key={};s.deathKey={};
    s.helloAt=0;s.helloSent=0;s.result="awaiting_peer";
    if(checkpointMission.mission)armAutosavePair();
    s.avatarRequested=wantsAvatar;s.avatarPending=s.avatarReady=false;
}
void checkpointStart(std::uint64_t tick) {
    auto& s=checkpointSync;s.key=checkpointScope(true);s.key.transaction=++checkpointTransaction;
    // 1214bn: an attempt the peer refuses ends in 8 s (was 45 s with both on
    // GameOver, take route-eotw-boatdeath-v1302l); loading keeps 45 s.
    s.phase=CheckpointPhase::Preparing;s.deadline=tick+8000;s.result="preparing";s.avatarReady=false;
    s.heldAt=0;s.releasedAt=0;s.readyAt=0;s.completedAt=0;s.hostReady=0;s.clientReady=0;
    s.readySent=false;s.completeSent=false;s.peerComplete=false;s.samples=0;s.maxDistance=0;
    if(!checkpointSend(CheckpointMessage::Prepare))checkpointFail("prepare_send_failed");
}
void checkpointRequest(std::uint64_t tick) {
    auto& s=checkpointSync;++s.requests;
    if(s.phase!=CheckpointPhase::Idle||tick<s.cooldown){++s.duplicates;return;}
    if(s.deathActive&&(!s.deathReady||!s.peerDeathReady)){s.deathRetry=true;s.result="waiting_for_both_death_screens";return;}
    if(!checkpointPairReady(tick)||!checkpointFixtureMatches()){++s.rejected;s.result="request_refused";return;}
    if(!localRole){checkpointStart(tick);return;}
    s.key=checkpointScope(true);s.phase=CheckpointPhase::Requested;s.deadline=tick+8000;s.result="requesting";
    if(!checkpointSend(CheckpointMessage::Request))checkpointFail("request_send_failed");
}
std::uint64_t checkpointMissionContinues=0;
bool checkpointIntercept(unsigned kind,int slot,bool& continuation) {
    continuation=false;
    auto& s=checkpointSync;if(!s.enabled)return false;
    if(checkpointMission.mission&&s.permitLoad&&kind==0) {
        // Our own dispatched RestartFromCheckpoint: the native routine caches
        // this peer's last autosave and queues the memory continuation.
        s.permitLoad=false;s.permitContinuation=true;++s.loads;return false;
    }
    if(s.permitLoad&&kind==2&&slot==82&&!checkpointMission.mission) {
        s.permitLoad=false;s.permitContinuation=true;++s.loads;return false;
    }
    if(kind==2&&slot==82){
        std::uintptr_t currentGame=0;
        if(memoryRead(reinterpret_cast<std::uintptr_t>(gameImage)+0x39a5290,&currentGame,8)&&
           checkpointFixedSlotRetry(s.enabled,s.permitContinuation,s.expectBoundary,s.loadingEntryAt!=0,s.phase,kind,slot,
               s.oldGame,currentGame,GetTickCount64()<=s.deadline)&&checkpointFixtureMatches()){
            // Native659190 operation-busy mask0x1 calls65acb0 to requeue the
            // same LoadGame82. Do not consume it as another user reload.
            // Logical loads remain one; retain permission until actual entry
            // or the existing memory continuation reports acceptance.
            continuation=true;++s.slotRetryCalls;return false;
        }
    }
    if(s.permitContinuation&&kind==1&&s.expectBoundary&&(s.phase==CheckpointPhase::Loading||s.phase==CheckpointPhase::Failed)) {
        // Native 659190 may return AL=0 and requeue RestartFromMemoryCheckpoint
        // plus "wait 1" while save I/O is busy. Dispatch is not acceptance.
        // Keep this fixed-slot permission until AL=1 or actual loading entry;
        // otherwise a coincident manual retry can consume the needed callback.
        int cachedSlot=-1;std::uintptr_t activeGame=0;
        const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
        const int expectedSlot=checkpointMission.mission?checkpointMission.slot:82;
        if(memoryRead(base+0x2318198,&cachedSlot,4)&&cachedSlot==expectedSlot&&
           memoryRead(base+0x39a5290,&activeGame,8)&&activeGame==s.oldGame) {
            continuation=true;++s.continuationCalls;return false;
        }
    }
    // 1214bd: after a mission's result screen, Continue loads the next mission
    // with RestartFromMemoryCheckpoint (take mission-end-product-v1291h). 1214bh:
    // a script LevelTransition (logic 15, MapTransition) continues the same way
    // (take route-dunwall-v1300m: the Dunwall vault exit). Both are native map
    // transitions, not user reloads: pass them through unless this checkpoint
    // sync is itself mid-reload.
    if(kind==1&&(outgoing.gameLogicState==11||(outgoing.gameLogicState==15&&!s.expectBoundary&&!s.permitContinuation))){++checkpointMissionContinues;return false;}
    checkpointRequest(GetTickCount64());return true;
}
void checkpointContinuationReturned(unsigned char result) {
    auto& s=checkpointSync;
    if(result==1){++s.continuationAccepted;s.permitContinuation=false;}
    else if(result==0)++s.continuationDeferred;
    else{ s.permitContinuation=false;checkpointFail("native_continuation_result_invalid"); }
}
void checkpointArmRetryProbe() {
    auto& s=checkpointSync;const auto tick=GetTickCount64();
    if(!s.enabled||s.phase!=CheckpointPhase::Idle||s.queueLoad||s.expectBoundary||s.held||
       !checkpointPairReady(tick)||!checkpointFixtureMatches()||!observerIsolation.ready||
       !observerIsolation.movementFixture||observerIsolation.until<tick+10000){++s.rejected;return;}
    // Explicit one-shot fixture: exercise native busy retry without changing
    // save-manager memory, slowing I/O or accepting a packet-provided command.
    s.retryProbeUntil=tick+10000;s.retryProbeArm=s.arm;s.retryProbeEpoch=outgoing.worldEpoch;
}
void checkpointReceive(const CheckpointPacket& p,std::uint64_t tick) {
    auto& s=checkpointSync;++s.received;
    if(!s.enabled||GetCurrentThreadId()!=engineThread.load()||!checkpointPacketShape(p,localRole)||
       p.hostSession!=(localRole?remote.session:outgoing.session)||p.clientSession!=(localRole?outgoing.session:remote.session)||
       p.mapId!=checkpointMapId()||std::memcmp(p.digest,s.digest,32)||
       (s.peerSession==remote.session&&p.sequence<=s.peerSequence)) {++s.rejected;return;}
    if(p.kind==CheckpointMessage::Hello) {
        const auto arm=localRole?p.hostArm:p.clientArm;
        if(s.peerArm&&(arm!=s.peerArm||s.peerSession!=remote.session)){checkpointFail("peer_rearmed");return;}
        s.peerSession=remote.session;s.peerSequence=p.sequence;s.peerArm=arm;s.helloAt=tick;
        if(s.phase==CheckpointPhase::Idle)s.result="armed";return;
    }
    if(!checkpointSameSession(p,checkpointScope(false))){++s.rejected;return;}
    if(p.kind==CheckpointMessage::Death||p.kind==CheckpointMessage::DeathReady) {
        if(s.phase!=CheckpointPhase::Idle||!checkpointSameWorld(p,checkpointScope(true))||!checkpointPeerFresh(tick)){++s.rejected;return;}
        if(p.kind==CheckpointMessage::Death) {
            if(!s.deathActive) {
                s.deathActive=true;s.deathKey=p;s.deathDeadline=tick+8000;s.deathReady=false;s.peerDeathReady=false;
                s.deathPending=outgoing.gameLogicState!=13;++s.deathCount;s.result="shared_death";
            }
            ++s.deathReceived;
        }else {
            if(!s.deathActive||!checkpointSameWorld(p,s.deathKey)){++s.rejected;return;}
            s.peerDeathReady=true;
        }
        s.peerSequence=p.sequence;return;
    }
    if(p.kind==CheckpointMessage::Request) {
        s.peerSequence=p.sequence;
        if(s.phase!=CheckpointPhase::Idle||tick<s.cooldown){++s.duplicates;return;}
        const auto current=checkpointScope(true);
        if(!checkpointPairReady(tick)||p.hostEpoch!=current.hostEpoch||p.clientEpoch!=current.clientEpoch||!checkpointFixtureMatches()){++s.rejected;return;}
        ++s.requests;checkpointStart(tick);return;
    }
    if(!checkpointPhaseAccepts(s.phase,p.kind,localRole)){++s.rejected;return;}
    if(p.kind==CheckpointMessage::Prepare) {
        const auto current=checkpointScope(true);
        if(!checkpointPairReady(tick)||(s.deathActive&&(!s.deathReady||!s.peerDeathReady))||p.transaction<=s.key.transaction||p.hostEpoch!=current.hostEpoch||p.clientEpoch!=current.clientEpoch||!checkpointFixtureMatches()){++s.rejected;return;}
        s.key=p;s.phase=CheckpointPhase::Accepted;s.deadline=tick+45000;s.result="accepted";s.avatarReady=false;
        s.heldAt=0;s.releasedAt=0;s.readyAt=0;s.completedAt=0;s.hostReady=0;s.clientReady=0;
        s.readySent=false;s.completeSent=false;s.peerComplete=false;s.samples=0;s.maxDistance=0;
        if(!checkpointSend(CheckpointMessage::Accept))checkpointFail("accept_send_failed");
    } else {
        if(!checkpointSameAttempt(p,s.key)){++s.rejected;return;}
        if(p.kind==CheckpointMessage::Accept) {
            if(!checkpointSend(CheckpointMessage::Begin)){checkpointFail("begin_send_failed");return;}
            s.queueLoad=true;s.phase=CheckpointPhase::Loading;s.result="loading";s.deadline=tick+45000;
        }else if(p.kind==CheckpointMessage::Begin){s.queueLoad=true;s.phase=CheckpointPhase::Loading;s.result="loading";s.deadline=tick+45000;}
        else if(p.kind==CheckpointMessage::Ready) {
            auto& token=localRole?s.hostReady:s.clientReady;const auto incoming=localRole?p.hostReady:p.clientReady;
            if(token&&token!=incoming){++s.rejected;return;}token=incoming;
        }else if(p.kind==CheckpointMessage::Release) {
            if(!s.readySent||p.hostReady!=s.hostReady||p.clientReady!=s.clientReady){++s.rejected;return;}
            s.phase=CheckpointPhase::Released;s.result="released";
        }else if(p.kind==CheckpointMessage::Complete) {
            if(p.hostReady!=s.hostReady||p.clientReady!=s.clientReady){++s.rejected;return;}s.peerComplete=true;
        }else if(p.kind==CheckpointMessage::Abort){checkpointFail("peer_aborted",false);}
    }
    s.peerSequence=p.sequence;
}
void checkpointService(std::uint64_t tick) {
    auto& s=checkpointSync;if(!s.enabled)return;
    if(s.phase==CheckpointPhase::Idle||s.phase==CheckpointPhase::Requested) {
        if(tick-s.helloSent>=250){s.helloSent=tick;checkpointSend(CheckpointMessage::Hello);}
    }
    if(s.phase==CheckpointPhase::Idle&&s.peerArm&&
       (!checkpointConnected()||s.peerSession!=remote.session||
        (lastReceive&&tick-lastReceive>5000)||(s.helloAt&&tick-s.helloAt>5000&&tick-s.completedAt>5000))) {
        checkpointFail("session_peer_disconnected");return;
    }
    if(s.phase==CheckpointPhase::Idle&&outgoing.gameLogicState==13&&!s.deathActive) {
        if(!checkpointPeerFresh(tick)){checkpointFail("death_peer_unavailable");return;}
        s.deathActive=true;s.deathKey=checkpointScope(true);s.deathDeadline=tick+8000;
        s.deathReady=false;s.peerDeathReady=false;s.deathPending=false;++s.deathCount;s.result="shared_death";
        if(!checkpointSend(CheckpointMessage::Death)){checkpointFail("death_send_failed");return;}
    }
    if(s.phase==CheckpointPhase::Idle&&s.deathActive) {
        if(!checkpointSameWorld(s.deathKey,checkpointScope(true))||!checkpointConnected()){checkpointFail("death_scope_changed");return;}
        if(!s.deathReady&&outgoing.gameLogicState==13&&!outgoing.pendingGameLogicState) {
            NativeGameOverState g{};NativeCharacterState c{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
            if(readNativeGameOver(memoryRead,base,g)&&g.logic==13&&!g.pending&&g.flash&&g.showFunction[0]&&g.showFunction[1]&&
               readNativeCharacter(memoryRead,base,c)&&c.character==localCharacter()&&!c.queued) {
                if(!checkpointSend(CheckpointMessage::DeathReady)){checkpointFail("death_ready_send_failed");return;}
                s.deathReady=true;
            }
        }
        if(s.deathReady&&s.peerDeathReady) {
            s.result="both_death_screens_ready";
            if(s.deathRetry){s.deathRetry=false;checkpointRequest(tick);}
        }else if(tick>=s.deathDeadline){checkpointFail("death_screen_timeout");return;}
    }
    if(s.phase==CheckpointPhase::Idle||s.phase==CheckpointPhase::Failed)return;
    if(tick>=s.deadline){checkpointFail("timeout");return;}
    if(!checkpointConnected()||s.peerSession!=remote.session||(lastReceive&&tick-lastReceive>5000)){checkpointFail("peer_disconnected");return;}
    if(s.phase==CheckpointPhase::Held&&!s.readySent&&tick-s.heldAt>=s.readyDelay) {
        if(!checkpointSend(CheckpointMessage::Ready)){checkpointFail("ready_send_failed");return;}
        s.readySent=true;s.readyAt=tick;
    }
    if(!localRole&&s.phase==CheckpointPhase::Held&&s.readySent&&s.hostReady&&s.clientReady) {
        if(!checkpointSend(CheckpointMessage::Release)){checkpointFail("release_send_failed");return;}
        s.phase=CheckpointPhase::Released;s.result="released";
    }
    if(s.phase==CheckpointPhase::Released&&!s.held&&!s.completeSent&&coopGameplayReady(outgoing)) {
        NativeCharacterState c{};
        if(!readNativeCharacter(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),c)||c.character!=checkpointExpectCharacter||c.queued){checkpointFail("postload_role_invalid");return;}
        if(!checkpointSend(CheckpointMessage::Complete)){checkpointFail("complete_send_failed");return;}
        s.completeSent=true;
    }
    if(s.phase==CheckpointPhase::Released&&s.completeSent&&s.peerComplete) {
        s.phase=CheckpointPhase::Idle;s.completedAt=tick;s.cooldown=tick+2000;++s.completed;s.result="complete";
        s.deathActive=s.deathPending=s.deathReady=s.peerDeathReady=s.deathRetry=false;
        s.avatarPending=s.avatarRequested;s.avatarSpawnedAt=0;
    }
}
bool autosaveProbeRequested=false;std::uint64_t autosaveProbeCalls=0,autosaveProbeRefused=0;
std::uint64_t checkpointRemaps=0;
void checkpointEngineService(void* system) {
    serviceAutosavePair(GetTickCount64(),checkpointSync.enabled&&checkpointMission.mission);
    if(autosaveProbeRequested) {
        // Developer fixture: a native autosave on this peer only, standing in
        // for a mission-script autosave trigger crossed by one player.
        autosaveProbeRequested=false;
        if(checkpointSync.enabled&&checkpointMission.mission&&outgoing.gameLogicState==1&&!outgoing.pendingGameLogicState&&requestNativeAutosave())++autosaveProbeCalls;
        else ++autosaveProbeRefused;
    }
    auto& s=checkpointSync;if(!s.enabled)return;
    // 1214bl: the session's map scope is fixed when it is armed. After a level
    // transition or a mission end the world is on another map, so no restart
    // could pass checkpointWorldReady (takes route-eotw-v1302f, campaign-eotw-vm-
    // v1302h and route-eotw-boatdeath-v1302i: each death after the boat was
    // refused). An idle session re-arms on the new map; if arming is refused,
    // the product maintenance re-issues coop_checkpoint_sync on.
    if((s.phase==CheckpointPhase::Idle||s.phase==CheckpointPhase::Failed)&&!s.expectBoundary&&!s.held&&!s.queueLoad&&
       outgoing.mapId&&outgoing.mapId!=checkpointMapId()&&outgoing.gameLogicState==1&&!outgoing.pendingGameLogicState){
        ++checkpointRemaps;checkpointEnable(false);checkpointEnable(true);
        if(!s.enabled)return;
    }
    // 1214bn: a failed session on the same map re-arms after 3 s, so the next
    // restart press (or a later death) is shared again instead of refused.
    if(s.phase==CheckpointPhase::Failed&&!s.expectBoundary&&!s.held&&!s.queueLoad&&GetTickCount64()-checkpointFailedAt>=3000&&
       (outgoing.gameLogicState==1||outgoing.gameLogicState==2||outgoing.gameLogicState==13)&&!outgoing.pendingGameLogicState){
        ++checkpointRecoveries;checkpointEnable(false);checkpointEnable(true);
        if(!s.enabled)return;
    }
    if(s.avatarPending&&s.phase==CheckpointPhase::Idle) {
        const auto tick=GetTickCount64();
        // Recreate only a presentation that was explicitly following when
        // this session was armed, once both native loads have completed.
        if(checkpointPairReady(tick)) {
            if(!s.avatarSpawnedAt) {
                if(prepareCheckpointPresentation(s.avatarShadow))s.avatarSpawnedAt=tick;
                else{s.avatarPending=false;++s.avatarRefused;}
            }else if(tick-s.avatarSpawnedAt>=800) {
                // Match the tested native spawn/initialize/follow sequence:
                // allow ordinary actor/component updates before owning its
                // reference frame and overriding animation evaluation.
                s.avatarPending=false;s.avatarReady=restoreCheckpointPresentation(s.avatarShadow,s.avatarExact);
                if(s.avatarReady)++s.avatarRestored;else ++s.avatarRefused;
            }
        }else if(tick-s.completedAt>5000){s.avatarPending=false;++s.avatarRefused;}
    }
    // 1153b: world-authority renewal waits only until the presentation step has
    // finished; a refused partner avatar must not hold doors/coins/NPCs back.
    if(s.phase==CheckpointPhase::Idle&&s.completedAt&&(!s.avatarRequested||s.avatarReady||!s.avatarPending)){finishDoorCheckpointRecovery();finishCoinCheckpointRecovery();finishUsableCheckpointRecovery();finishNpcCheckpointRecovery();}
    if(s.deathPending&&s.phase==CheckpointPhase::Idle) {
        NativeGameOverState g{};
        if(!readNativeGameOver(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),g)||
           (g.logic!=1&&g.logic!=2&&g.logic!=13)){checkpointFail("death_transition_refused");return;}
        if(g.pending)return;
        s.deathPending=false;
        if(g.logic!=13) {
            stopNpcStream("shared_death");stopGuardLink("shared_death");stopObserverIsolation("shared_death");releaseAiDecisionHold("shared_death");
            networkFrameOwned.store(false,std::memory_order_release);phaseFollowInvalidated=true;
            realLogicSwitch(reinterpret_cast<void*>(g.manager),13);++s.deathApplied;
        }
        return;
    }
    // 1214kh (#221): a lost partner is not a failed restart, and the player plays on alone (peer-loss-dunwall-vm-v1909a:
    // the host's pause screen opened when the VM's game was ended).
    if(s.phase==CheckpointPhase::Failed&&outgoing.gameLogicState==1&&!outgoing.pendingGameLogicState&&observedGame&&
       std::strcmp(s.result,"session_peer_disconnected")&&std::strcmp(s.result,"peer_disconnected")&&std::strcmp(s.result,"death_peer_unavailable")) {
        // Keep failed sessions in the existing native pause screen until the
        // experiment is explicitly disabled or restarted by the harness.
        realLogicSwitch(reinterpret_cast<void*>(observedGame+0x1f6ec8),2);return;
    }
    if(!s.queueLoad)return;
    if(!checkpointWorldReady(outgoing)||!checkpointFixtureMatches()){checkpointFail("preload_scope_changed");return;}
    checkpointExpectCharacter=localCharacter(); // 1214hy (#159)
    const bool retryProbe=s.retryProbeUntil>GetTickCount64()&&s.retryProbeArm==s.arm&&
        s.retryProbeEpoch==outgoing.worldEpoch&&observerIsolation.ready&&observerIsolation.movementFixture;
    s.retryProbeUntil=0;
    beginDoorCheckpointRecovery();beginCoinCheckpointRecovery();beginUsableCheckpointRecovery();beginNpcCheckpointRecovery();
    stopGuardLink("shared_checkpoint");stopObserverIsolation("shared_checkpoint");releaseAiDecisionHold("shared_checkpoint");
    networkFrameOwned.store(false,std::memory_order_release);phaseFollowInvalidated=true;
    s.oldGame=observedGame;s.queueLoad=false;s.permitLoad=true;s.expectBoundary=true;s.loadingGame=0;s.loadingEntryAt=0;
    if(localRole&&!checkpointMission.mission) {
        loadingProbe.armed=true;loadingProbe.armUntil=s.deadline;loadingProbe.armGame=observedGame;
        loadingProbe.loadingGame=0;loadingProbe.loadingEntryAt=0;
        loadingProbe.restoreTarget=2;++loadingProbe.armSequence;loadingProbe.result="shared_restore_armed";
    }
    const char* command=checkpointMission.mission?"RestartFromCheckpoint\n":"LoadGame 82\n";
    if(retryProbe){command="LoadGame 82\nRestartFromMemoryCheckpoint\nRestartFromMemoryCheckpoint\n";s.retryProbeInjected+=2;}
    reinterpret_cast<void(*)(void*,const char*)>(gameImage+0xfec70)(system,command);
}
bool checkpointLoadingBoundary(const NativeSwitchEvent& event) {
    auto& s=checkpointSync;if(!s.enabled||!s.expectBoundary)return false;
    if(event.thread!=engineThread.load()||event.caller!=0x7879cd||event.to!=1)return false;
    NativeCharacterState c{};std::uintptr_t name=0;char map[192]{};
    const bool valid=event.from==15&&!event.pending&&event.phase==3&&s.loadingEntryAt&&event.tick>=s.loadingEntryAt&&event.game==s.loadingGame&&
        memoryRead(event.game+0x337138,&name,8)&&nativeName(name,map,sizeof(map))&&!std::strcmp(map,checkpointMission.map)&&
        readNativeCharacter(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),c,event.game)&&c.character==checkpointExpectCharacter&&
        !c.queued&&!std::strcmp(c.definitionName,c.variantNames[checkpointExpectCharacter])&&(checkpointMission.mission||checkpointFixtureMatches())&&
        memoryRead(c.player+0x300,s.position,sizeof(s.position));
    loadingProbe.armed=false;s.expectBoundary=false;s.permitContinuation=false;s.game=event.game;s.player=c.player;s.manager=event.manager;
    if(!valid||(localRole&&!checkpointMission.mission&&!loadingRoleRestore.completed))checkpointFail("loaded_world_invalid");
    for(float x:s.position)if(!std::isfinite(x))checkpointFail("loaded_position_invalid");
    s.held=true;s.heldAt=GetTickCount64();s.deadline=s.heldAt+15000;
    if(s.phase!=CheckpointPhase::Failed) {
        s.phase=CheckpointPhase::Held;s.result="held";
        if(localRole)s.clientReady=++checkpointReadySequence;else s.hostReady=++checkpointReadySequence;
    }
    const bool previousMutation=nativeMutationAllowed;nativeMutationAllowed=false;
    while(s.phase==CheckpointPhase::Held) {
        NativeSwitchEvent e{};float position[3]{};
        if(!nativeSwitchContext(s.manager,e)||e.game!=s.game||e.player!=s.player||e.from!=15||e.pending||e.phase!=3||
           !memoryRead(s.player+0x300,position,sizeof(position))){checkpointFail("held_world_changed");break;}
        double d=0;for(unsigned i=0;i<3;++i){const double delta=position[i]-s.position[i];d+=delta*delta;}
        if(!std::isfinite(d)||d>0.000001){checkpointFail("held_player_moved");break;}
        if(std::sqrt(d)>s.maxDistance)s.maxDistance=std::sqrt(d);++s.samples;
        const auto tick=GetTickCount64();pumpTransport(tick);writeStatus(tick);Sleep(10);
    }
    nativeMutationAllowed=previousMutation;s.held=false;s.releasedAt=GetTickCount64();
    if(s.phase==CheckpointPhase::Failed) {
        // Match the game's own FinishLoad branch: commit gameplay entry, then
        // request a second native state before the caller commits it. No world
        // simulation or player-input frame runs between these native calls.
        realLogicSwitch(reinterpret_cast<void*>(event.manager),1);
        reinterpret_cast<void(*)(void*)>(gameImage+0x787510)(reinterpret_cast<void*>(event.manager));
        return true; // wrapper forwards native pause (2), caller commits it
    }
    return false;
}
void writeCheckpointSync() {
    const auto& s=checkpointSync;char path[MAX_PATH]{},temp[MAX_PATH]{},digest[65]{};
    for(unsigned i=0;i<32;++i)std::snprintf(digest+i*2,3,"%02x",s.digest[i]);
    if(std::snprintf(path,sizeof(path),"%s.reset.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\"enabled\":%s,\"phase\":%u,\"result\":\"%s\",\"arm\":%llu,\"peerArm\":%llu,\"transaction\":%llu,\"digest\":\"%s\",\"held\":%s,\"expectBoundary\":%s,\"loads\":%llu,\"completed\":%llu,\"requests\":%llu,\"duplicates\":%llu,\"rejected\":%llu,\"aborted\":%llu,\"remaps\":%llu,\"recoveries\":%llu,\"heldAt\":%llu,\"releasedAt\":%llu,\"readyAt\":%llu,\"completedAt\":%llu,\"hostReady\":%llu,\"clientReady\":%llu,\"readyDelay\":%u,\"sent\":%llu,\"received\":%llu,\"samples\":%llu,\"maxDistance\":%.9g,\"game\":\"0x%llx\",\"player\":\"0x%llx\",\"position\":[%.9g,%.9g,%.9g],",
        s.enabled?"true":"false",static_cast<unsigned>(s.phase),s.result,s.arm,s.peerArm,s.key.transaction,digest,s.held?"true":"false",s.expectBoundary?"true":"false",s.loads,s.completed,s.requests,s.duplicates,s.rejected,s.aborted,checkpointRemaps,checkpointRecoveries,s.heldAt,s.releasedAt,s.readyAt,s.completedAt,s.hostReady,s.clientReady,s.readyDelay,s.sent,s.received,s.samples,s.maxDistance,s.game,s.player,s.position[0],s.position[1],s.position[2]);
    fprintf(f,"\"loadingEntryAt\":%llu,\"oldGame\":\"0x%llx\",",s.loadingEntryAt,s.oldGame);
    {const auto& a=autosavePair;fprintf(f,"\"autosavePair\":{\"active\":%s,\"ready\":%s,\"saves\":%u,\"originated\":%u,\"mirrored\":%u,\"pending\":%s,\"peerSaves\":%u,\"peerOriginated\":%u,\"peerMirrored\":%u,\"peerPending\":%s,\"requests\":%llu,\"refused\":%llu,\"resets\":%llu,\"received\":%llu,\"rejected\":%llu,\"peerRearms\":%llu,\"slot\":%d,\"result\":\"%s\",\"probeCalls\":%llu,\"probeRefused\":%llu},",
        a.active?"true":"false",autosavePairReady()?"true":"false",a.saves,a.originated,a.mirrored,a.pending?"true":"false",a.peerSaves,a.peerOriginated,a.peerMirrored,a.peerPending?"true":"false",a.requests,a.refused,a.resets,a.received,a.rejected,a.peerRearms,a.slot,a.result,autosaveProbeCalls,autosaveProbeRefused);}
    fprintf(f,"\"mission\":%s,\"missionMap\":\"%s\",\"missionSlot\":%d,\"missionGeneration\":%u,\"missionDetails\":\"%s\",",checkpointMission.mission?"true":"false",checkpointMission.map,checkpointMission.slot,checkpointMission.generation,checkpointMission.details);
    // 1214im (#171): the native last autosave slot and save root as read now, for a digest that does not arm.
    {int nativeSlot=-1;std::uintptr_t game=0,rootName=0;std::string rootText;const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
     if(memoryRead(base+0x39a5290,&game,8)&&game)memoryRead(game+0x2e75d8+0x1c,&nativeSlot,4);
     if(!memoryRead(base+0x2286f40,&rootName,8)||!worldIdentityString(memoryRead,rootName,rootText)||rootText.size()>=32)rootText.clear();
     fprintf(f,"\"nativeAutosaveSlot\":%d,\"saveRoot\":\"%s\",",nativeSlot,rootText.c_str());}
    fprintf(f,"\"slotRetryCalls\":%llu,\"continuationCalls\":%llu,\"continuationDeferred\":%llu,\"continuationAccepted\":%llu,",s.slotRetryCalls,s.continuationCalls,s.continuationDeferred,s.continuationAccepted);
    fprintf(f,"\"retryProbeUntil\":%llu,\"retryProbeInjected\":%llu,",s.retryProbeUntil,s.retryProbeInjected);
    fprintf(f,"\"avatarRequested\":%s,\"avatarPending\":%s,\"avatarReady\":%s,\"avatarRestored\":%llu,\"avatarRefused\":%llu,",
        s.avatarRequested?"true":"false",s.avatarPending?"true":"false",s.avatarReady?"true":"false",s.avatarRestored,s.avatarRefused);
    fprintf(f,"\"avatarProfile\":\"%s\",\"avatarExact\":%s,",s.avatarShadow?"doppelganger":"story",s.avatarExact?"true":"false");
    fprintf(f,"\"avatarAiHeld\":%s,\"avatarAiAcquired\":%llu,\"avatarAiReleased\":%llu,\"avatarAiLost\":%llu,\"avatarAiRefused\":%llu,",
        avatarAiHeld?"true":"false",avatarAiAcquired,avatarAiReleased,avatarAiLost,avatarAiRefused);
    fprintf(f,"\"avatarIdleOwned\":%s,\"avatarIdleAcquired\":%llu,\"avatarIdleReleased\":%llu,\"avatarIdleLost\":%llu,\"avatarIdleRefused\":%llu,",
        avatarIdleOwned?"true":"false",avatarIdleAcquired,avatarIdleReleased,avatarIdleLost,avatarIdleRefused);
    fprintf(f,"\"deathActive\":%s,\"deathReady\":%s,\"peerDeathReady\":%s,\"deathRetryQueued\":%s,\"deathCount\":%llu,\"deathApplied\":%llu,\"deathReceived\":%llu}\n",
        s.deathActive?"true":"false",s.deathReady?"true":"false",s.peerDeathReady?"true":"false",s.deathRetry?"true":"false",s.deathCount,s.deathApplied,s.deathReceived);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
