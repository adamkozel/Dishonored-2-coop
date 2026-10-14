#pragma once
#include "checkpoint_protocol.h"
#include "checkpoint_fixture.h"
// Opt-in, fixed-slot Dunwall experiment. All state is owned by the native
// engine thread. Packet reception schedules loads; it never calls LoadGame.
struct CheckpointSync {
    bool enabled=false,queueLoad=false,permitLoad=false,permitContinuation=false,expectBoundary=false;
    bool held=false,readySent=false,completeSent=false,peerComplete=false;
    CheckpointPhase phase=CheckpointPhase::Idle;
    CheckpointPacket key{};
    unsigned char digest[32]{};
    std::uint64_t arm=0,peerArm=0,peerSession=0,peerSequence=0,helloAt=0,helloSent=0;
    std::uint64_t deadline=0,cooldown=0,heldAt=0,releasedAt=0,readyAt=0,completedAt=0;
    std::uint64_t hostReady=0,clientReady=0,loads=0,completed=0,requests=0,duplicates=0,rejected=0,aborted=0;
    std::uint64_t sent=0,received=0,samples=0;double maxDistance=0;
    std::uintptr_t oldGame=0,game=0,player=0,manager=0;
    float position[3]{};unsigned readyDelay=0;
    const char* result="disabled";
};
CheckpointSync checkpointSync{};
bool checkpointOwnsLoading(){return checkpointSync.enabled&&checkpointSync.expectBoundary;}
std::uint64_t checkpointArmSequence=0,checkpointTransaction=0,checkpointPacketSequence=0,checkpointReadySequence=0;
bool checkpointConnected() {return sockets&&connection&&connectionState==k_ESteamNetworkingConnectionState_Connected;}
bool checkpointWorldReady(const CoopPosePacket& p) {
    return (p.flags&1)&&p.worldEpoch&&p.mapId==coopMapId(kGuardMap)&&
        (p.gameLogicState==1||p.gameLogicState==2)&&!p.pendingGameLogicState;
}
CheckpointPacket checkpointScope(bool epochs) {
    auto& s=checkpointSync;CheckpointPacket p{};
    p.hostSession=localRole?remote.session:outgoing.session;p.clientSession=localRole?outgoing.session:remote.session;
    p.hostArm=localRole?s.peerArm:s.arm;p.clientArm=localRole?s.arm:s.peerArm;
    if(epochs){p.hostEpoch=localRole?remote.worldEpoch:outgoing.worldEpoch;p.clientEpoch=localRole?outgoing.worldEpoch:remote.worldEpoch;}
    p.mapId=coopMapId(kGuardMap);std::memcpy(p.digest,s.digest,32);return p;
}
bool checkpointSend(CheckpointMessage kind) {
    auto& s=checkpointSync;if(!checkpointConnected())return false;
    auto p=kind==CheckpointMessage::Hello?checkpointScope(false):s.key;
    p.kind=kind;p.role=localRole;p.sequence=++checkpointPacketSequence;p.character=0;p.hostReady=0;p.clientReady=0;
    if(kind==CheckpointMessage::Ready){p.character=localRole+1;if(localRole)p.clientReady=s.clientReady;else p.hostReady=s.hostReady;}
    if(kind==CheckpointMessage::Release||kind==CheckpointMessage::Complete){p.hostReady=s.hostReady;p.clientReady=s.clientReady;}
    const bool ok=sockets->SendMessageToConnection(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)==k_EResultOK;
    if(ok)++s.sent;return ok;
}
bool checkpointFixtureMatches() {unsigned char digest[32]{};return checkpointDigest(digest)&&!std::memcmp(digest,checkpointSync.digest,32);}
bool checkpointPairReady(std::uint64_t tick) {
    const auto& s=checkpointSync;
    return s.enabled&&checkpointConnected()&&lastReceive&&tick-lastReceive<1500&&s.helloAt&&tick-s.helloAt<1500&&
        s.peerSession==remote.session&&s.peerArm&&checkpointWorldReady(outgoing)&&checkpointWorldReady(remote)&&
        guardLink.phase==GuardPhase::Idle&&!aiDecisionHold.expires&&!cinematicHold.id&&!loadingProbe.held&&!loadingProbe.armed;
}
void checkpointFail(const char* reason,bool notify=true) {
    auto& s=checkpointSync;if(s.phase==CheckpointPhase::Failed)return;
    if(notify&&s.key.transaction)checkpointSend(CheckpointMessage::Abort);
    s.phase=CheckpointPhase::Failed;s.queueLoad=false;s.permitLoad=false;s.result=reason;++s.aborted;
    consoleOutput("COOP: checkpoint failed transaction=%llu reason=%s\n",s.key.transaction,reason);
}
void checkpointEnable(bool on) {
    auto& s=checkpointSync;
    if(!on) {
        if(s.expectBoundary||s.held||s.queueLoad){++s.rejected;return;}
        if(s.phase!=CheckpointPhase::Idle&&s.phase!=CheckpointPhase::Failed)checkpointFail("cancelled");
        s.enabled=false;s.phase=CheckpointPhase::Idle;s.peerArm=0;s.helloAt=0;s.result="disabled";return;
    }
    if(s.enabled){++s.rejected;return;}
    // The pause recovery API is validated before arming, never discovered in
    // the middle of an interrupted native finish-load call.
    const unsigned char commit[]={0x40,0x56,0x57,0x41,0x56,0x48,0x83,0xec,0x30,0x48,0xc7,0x44,0x24,0x20,0xfe,0xff,0xff,0xff};
    if(!realLogicSwitch||!realReplacementUpdate||checkpointHookMask!=7||!checkpointWorldReady(outgoing)||
       std::memcmp(gameImage+0x787510,commit,sizeof(commit))||loadingProbe.armed||loadingProbe.held||
       guardLink.phase!=GuardPhase::Idle||aiDecisionHold.expires||cinematicHold.id||!checkpointDigest(s.digest)) {
        ++s.rejected;s.result="enable_refused";return;
    }
    s.enabled=true;s.phase=CheckpointPhase::Idle;s.arm=++checkpointArmSequence;s.peerArm=0;s.peerSession=0;s.peerSequence=0;
    s.helloAt=0;s.helloSent=0;s.result="awaiting_peer";
}
void checkpointStart(std::uint64_t tick) {
    auto& s=checkpointSync;s.key=checkpointScope(true);s.key.transaction=++checkpointTransaction;
    s.phase=CheckpointPhase::Preparing;s.deadline=tick+45000;s.result="preparing";
    s.heldAt=0;s.releasedAt=0;s.readyAt=0;s.completedAt=0;s.hostReady=0;s.clientReady=0;
    s.readySent=false;s.completeSent=false;s.peerComplete=false;s.samples=0;s.maxDistance=0;
    if(!checkpointSend(CheckpointMessage::Prepare))checkpointFail("prepare_send_failed");
}
void checkpointRequest(std::uint64_t tick) {
    auto& s=checkpointSync;++s.requests;
    if(s.phase!=CheckpointPhase::Idle||tick<s.cooldown){++s.duplicates;return;}
    if(!checkpointPairReady(tick)||!checkpointFixtureMatches()){++s.rejected;s.result="request_refused";return;}
    if(!localRole){checkpointStart(tick);return;}
    s.key=checkpointScope(true);s.phase=CheckpointPhase::Requested;s.deadline=tick+45000;s.result="requesting";
    if(!checkpointSend(CheckpointMessage::Request))checkpointFail("request_send_failed");
}
bool checkpointIntercept(unsigned kind,int slot) {
    auto& s=checkpointSync;if(!s.enabled)return false;
    if(s.permitLoad&&kind==2&&slot==82) {
        s.permitLoad=false;s.permitContinuation=true;++s.loads;return false;
    }
    if(s.permitContinuation&&kind==1&&s.expectBoundary&&s.phase==CheckpointPhase::Loading) {
        // LoadGame reads the file asynchronously and queues this second native
        // command. Permit one continuation only after the engine's cache is
        // marked as our fixed slot. A coincident manual memory-restart has the
        // same effect; the later duplicate is consumed, never a second load.
        int cachedSlot=-1;std::uintptr_t activeGame=0;
        const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
        if(memoryRead(base+0x2318198,&cachedSlot,4)&&cachedSlot==82&&
           memoryRead(base+0x39a5290,&activeGame,8)&&activeGame==s.oldGame) {
            s.permitContinuation=false;return false;
        }
    }
    checkpointRequest(GetTickCount64());return true;
}
void checkpointReceive(const CheckpointPacket& p,std::uint64_t tick) {
    auto& s=checkpointSync;++s.received;
    if(!s.enabled||GetCurrentThreadId()!=engineThread.load()||!checkpointPacketShape(p,localRole)||
       p.hostSession!=(localRole?remote.session:outgoing.session)||p.clientSession!=(localRole?outgoing.session:remote.session)||
       p.mapId!=coopMapId(kGuardMap)||std::memcmp(p.digest,s.digest,32)||
       (s.peerSession==remote.session&&p.sequence<=s.peerSequence)) {++s.rejected;return;}
    if(p.kind==CheckpointMessage::Hello) {
        const auto arm=localRole?p.hostArm:p.clientArm;
        if(s.phase!=CheckpointPhase::Idle&&s.phase!=CheckpointPhase::Requested&&arm!=s.peerArm){checkpointFail("peer_rearmed");return;}
        s.peerSession=remote.session;s.peerSequence=p.sequence;s.peerArm=arm;s.helloAt=tick;
        if(s.phase==CheckpointPhase::Idle)s.result="armed";return;
    }
    if(!checkpointSameSession(p,checkpointScope(false))){++s.rejected;return;}
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
        if(!checkpointPairReady(tick)||p.transaction<=s.key.transaction||p.hostEpoch!=current.hostEpoch||p.clientEpoch!=current.clientEpoch||!checkpointFixtureMatches()){++s.rejected;return;}
        s.key=p;s.phase=CheckpointPhase::Accepted;s.deadline=tick+45000;s.result="accepted";
        s.heldAt=0;s.releasedAt=0;s.readyAt=0;s.completedAt=0;s.hostReady=0;s.clientReady=0;
        s.readySent=false;s.completeSent=false;s.peerComplete=false;s.samples=0;s.maxDistance=0;
        if(!checkpointSend(CheckpointMessage::Accept))checkpointFail("accept_send_failed");
    } else {
        if(!checkpointSameAttempt(p,s.key)){++s.rejected;return;}
        if(p.kind==CheckpointMessage::Accept) {
            if(!checkpointSend(CheckpointMessage::Begin)){checkpointFail("begin_send_failed");return;}
            s.queueLoad=true;s.phase=CheckpointPhase::Loading;s.result="loading";
        }else if(p.kind==CheckpointMessage::Begin){s.queueLoad=true;s.phase=CheckpointPhase::Loading;s.result="loading";}
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
        if(!readNativeCharacter(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),c)||c.character!=static_cast<int>(localRole+1)||c.queued){checkpointFail("postload_role_invalid");return;}
        if(!checkpointSend(CheckpointMessage::Complete)){checkpointFail("complete_send_failed");return;}
        s.completeSent=true;
    }
    if(s.phase==CheckpointPhase::Released&&s.completeSent&&s.peerComplete) {
        s.phase=CheckpointPhase::Idle;s.completedAt=tick;s.cooldown=tick+2000;++s.completed;s.result="complete";
    }
}
void checkpointEngineService(void* system) {
    auto& s=checkpointSync;if(!s.enabled)return;
    if(s.phase==CheckpointPhase::Failed&&!s.expectBoundary&&outgoing.gameLogicState==1&&!outgoing.pendingGameLogicState&&observedGame) {
        // Keep failed sessions in the existing native pause screen until the
        // experiment is explicitly disabled or restarted by the harness.
        realLogicSwitch(reinterpret_cast<void*>(observedGame+0x1f6ec8),2);return;
    }
    if(!s.queueLoad)return;
    if(!checkpointWorldReady(outgoing)||!checkpointFixtureMatches()){checkpointFail("preload_scope_changed");return;}
    stopGuardLink("shared_checkpoint");releaseAiDecisionHold("shared_checkpoint");
    networkFrameOwned.store(false,std::memory_order_release);phaseFollowInvalidated=true;
    s.oldGame=observedGame;s.queueLoad=false;s.permitLoad=true;s.expectBoundary=true;
    if(localRole) {
        loadingProbe.armed=true;loadingProbe.armUntil=s.deadline;loadingProbe.armGame=observedGame;
        loadingProbe.restoreTarget=2;++loadingProbe.armSequence;loadingProbe.result="shared_restore_armed";
    }
    reinterpret_cast<void(*)(void*,const char*)>(gameImage+0xfec70)(system,"LoadGame 82\n");
}
bool checkpointLoadingBoundary(const NativeSwitchEvent& event) {
    auto& s=checkpointSync;if(!s.enabled||!s.expectBoundary)return false;
    if(event.thread!=engineThread.load()||event.caller!=0x7879cd||event.to!=1)return false;
    NativeCharacterState c{};std::uintptr_t name=0;char map[192]{};
    const bool valid=event.from==15&&!event.pending&&event.phase==3&&event.game!=s.oldGame&&
        memoryRead(event.game+0x337138,&name,8)&&nativeName(name,map,sizeof(map))&&!std::strcmp(map,kGuardMap)&&
        readNativeCharacter(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),c,event.game)&&c.character==static_cast<int>(localRole+1)&&
        !c.queued&&!std::strcmp(c.definitionName,c.variantNames[localRole+1])&&checkpointFixtureMatches()&&
        memoryRead(c.player+0x300,s.position,sizeof(s.position));
    loadingProbe.armed=false;s.expectBoundary=false;s.permitContinuation=false;s.game=event.game;s.player=c.player;s.manager=event.manager;
    if(!valid||(localRole&&!loadingRoleRestore.completed))checkpointFail("loaded_world_invalid");
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
    fprintf(f,"{\"enabled\":%s,\"phase\":%u,\"result\":\"%s\",\"arm\":%llu,\"peerArm\":%llu,\"transaction\":%llu,\"digest\":\"%s\",\"held\":%s,\"expectBoundary\":%s,\"loads\":%llu,\"completed\":%llu,\"requests\":%llu,\"duplicates\":%llu,\"rejected\":%llu,\"aborted\":%llu,\"heldAt\":%llu,\"releasedAt\":%llu,\"readyAt\":%llu,\"completedAt\":%llu,\"hostReady\":%llu,\"clientReady\":%llu,\"readyDelay\":%u,\"sent\":%llu,\"received\":%llu,\"samples\":%llu,\"maxDistance\":%.9g,\"game\":\"0x%llx\",\"player\":\"0x%llx\",\"position\":[%.9g,%.9g,%.9g]}\n",
        s.enabled?"true":"false",static_cast<unsigned>(s.phase),s.result,s.arm,s.peerArm,s.key.transaction,digest,s.held?"true":"false",s.expectBoundary?"true":"false",s.loads,s.completed,s.requests,s.duplicates,s.rejected,s.aborted,s.heldAt,s.releasedAt,s.readyAt,s.completedAt,s.hostReady,s.clientReady,s.readyDelay,s.sent,s.received,s.samples,s.maxDistance,s.game,s.player,s.position[0],s.position[1],s.position[2]);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
