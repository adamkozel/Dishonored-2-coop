#pragma once
#include "door_protocol.h"
#include "door_native_queue.h"
#include "door_session_scope.h"
// Opt-in authored ordinary-door catalogue for the current paired world.
// Linked, blocked, broken and ambiguous doors need separate authority. A locked door is cataloged, and its key
// unlock is shared (1214jf).
struct DoorSync {
    bool enabled=false,ready=false,awaiting=false,retryAfterMenu=false;
    NativeOpenable binding{};DoorOrder order{};
    std::uint64_t arm=0,peerArm=0,session=0,peerSession=0,epoch=0,peerEpoch=0,map=0,peerSequence=0;
    std::uint64_t helloAt=0,helloSentAt=0,stateSentAt=0,deadline=0,ack=0,requestAck=0,requestSequence=0;
    std::uint64_t inFlight=0,inFlightUntil=0,applied=0;
    unsigned observedTarget=0,observedSide=0,requestTarget=0,requestSide=0,inFlightTarget=0;
    std::uint64_t sent=0,received=0,rejected=0,duplicates=0,requests=0,commits=0,calls=0,converged=0,sideCorrections=0;
    // 1214jf (#185): the shared key unlock. observedLocked is the last native lock byte seen here; unlockPending sends
    // this world's unlock, unlockOrdered applies the peer's.
    bool unlockPending=false,unlockOrdered=false;unsigned char observedLocked=0;
    std::uint64_t unlocksSent=0,unlocksReceived=0,unlocksApplied=0;
    const char* result="disabled";
};
std::vector<DoorSync> doorChannels;
bool doorCatalogEnabled=false;
unsigned doorCatalogAmbiguous=0,doorCatalogIneligible=0,doorCatalogCursor=0,doorCatalogScanMs=0,doorServiceMs=0,doorServiceMaxMs=0;
std::uint64_t doorCatalogIgnored=0,doorCatalogUnmatched=0,doorCatalogRejected=0;
const char* doorCatalogResult="disabled";
std::uint64_t doorSyncArm=0,doorSyncSequence=0;
int doorSyncEnableRequest=-1;
bool doorSyncDuplicateRequest=false;
CoopDoorPacket doorSyncLastAction{};
struct DoorSession {
    bool enabled=false,reloadPending=false,reloadReady=false,menuSuspended=false;
    DoorSessionScope scope{};
    std::uint64_t deadline=0,reloads=0,rearms=0,retiredChannels=0,retiredNativeCalls=0;
    std::uint64_t menuSince=0,menuSuspensions=0,menuResumes=0,menuPausedMs=0,menuPacketsIgnored=0;
    const char* result="disabled";
};
DoorSession doorSession{};
bool doorCheckpointBarrierActive();
void retireDoorCatalog(const char* reason){
    if(doorCatalogEnabled){
        for(auto& s:doorChannels){++doorSession.retiredChannels;doorSession.retiredNativeCalls+=s.calls;s.enabled=s.ready=false;s.result=reason;}
    }
    doorCatalogEnabled=false;doorSyncDuplicateRequest=false;doorSyncLastAction={};doorCatalogResult=reason;
}
void stopDoorSession(const char* reason){
    doorSession.enabled=doorSession.reloadPending=doorSession.reloadReady=doorSession.menuSuspended=false;doorSession.menuSince=0;doorSession.result=reason;
    retireDoorCatalog(reason);
}
void beginDoorCheckpointRecovery(){
    auto& s=doorSession;if(!s.enabled)return;
    // The other native world may already be inside loading. Its map/epoch
    // will be checked at renewal; never require an old peer world here.
    if(outgoing.session!=s.scope.session||remote.session!=s.scope.peerSession||
       outgoing.mapId!=s.scope.map||outgoing.worldEpoch!=s.scope.epoch){stopDoorSession("checkpoint_session_changed");return;}
    s.reloadPending=true;s.reloadReady=false;s.menuSuspended=false;s.menuSince=0;s.deadline=GetTickCount64()+60000;++s.reloads;s.result="waiting_shared_checkpoint";
    retireDoorCatalog("shared_checkpoint");
}
void finishDoorCheckpointRecovery(){
    auto& s=doorSession;if(!s.enabled||!s.reloadPending)return;
    // Called only after the checkpoint protocol's both-complete boundary and
    // local role/counterpart restoration. Each peer must have a new epoch.
    if(doorSessionRenewalReady(s.scope,outgoing,remote))s.reloadReady=true;
}
bool doorSyncContext(const DoorSync& s,ULONGLONG tick){
    return s.enabled&&observedGame==s.binding.game&&nativeMutationAllowed&&GetCurrentThreadId()==engineThread.load()&&
        sockets&&connection&&connectionState==k_ESteamNetworkingConnectionState_Connected&&lastReceive&&tick-lastReceive<250&&
        outgoing.session==s.session&&remote.session==s.peerSession&&outgoing.worldEpoch==s.epoch&&remote.worldEpoch==s.peerEpoch&&
        coopWorldPairReady(outgoing,remote)&&outgoing.mapId==s.map&&
        ((doorSession.enabled&&!doorSession.reloadPending&&!doorSession.menuSuspended&&doorSessionWorldMatches(doorSession.scope,outgoing,remote))||
         (!doorSession.enabled&&observerIsolation.ready&&observerIsolation.movementFixture&&observerIsolation.until>tick+1000));
}
void failDoorSync(DoorSync& s,const char* reason){s.enabled=s.ready=false;s.result=reason;++s.rejected;}
// 1153c (as usables 1123c/d): identity scope only. Inside an armed session,
// transient gaps (pose stall under 1 s, native mutation gate, pair-readiness
// blips) pause a channel; identity changes still retire it. 1159: one blip
// retired all 16 client door channels on Edge of the World.
std::uint64_t doorTransientPauses=0;
bool doorSyncScope(const DoorSync& s){
    return s.enabled&&observedGame==s.binding.game&&GetCurrentThreadId()==engineThread.load()&&sockets&&connection&&
        connectionState==k_ESteamNetworkingConnectionState_Connected&&outgoing.session==s.session&&remote.session==s.peerSession&&
        outgoing.worldEpoch==s.epoch&&remote.worldEpoch==s.peerEpoch&&outgoing.mapId==s.map&&
        doorSession.enabled&&!doorSession.reloadPending&&doorSessionWorldMatches(doorSession.scope,outgoing,remote);
}
bool readBoundDoor(const DoorSync& s,NativeOpenable& current){
    const auto& b=s.binding;
    return readNativeOpenable(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,b.handle,current)&&sameNativeOpenableIdentity(current,b);
}
bool doorNativeDesired(const NativeOpenable& n,unsigned& target,unsigned& side){
    if(n.current==1||n.pending==1){unsigned char p[4]{};if(!memoryRead(n.component+0x90,p,4)||p[0]>1||p[1]||p[2]>1)return false;target=p[0]?2:0;side=p[2];}
    else{if(n.current!=n.pending)return false;target=n.current;side=n.openSide;}
    // 1214jf (#185): the lock is its own shared state (serviceDoorChannel), not an eligibility change.
    return !n.blocked&&!n.broken&&!n.linked;
}
void enableDoorSync(bool on){
    if(!on){stopDoorSession("disabled");return;}
    if(doorCatalogEnabled){++doorCatalogRejected;return;}
    const auto started=GetTickCount64();doorChannels.clear();doorCatalogCursor=0;doorSyncLastAction={};doorSyncDuplicateRequest=false;
    doorCatalogIgnored=doorCatalogUnmatched=doorCatalogRejected=0;doorCatalogAmbiguous=doorCatalogIneligible=0;doorServiceMs=doorServiceMaxMs=0;
    const auto refuse=[](const char* reason){doorCatalogResult=reason;++doorCatalogRejected;};
    std::uintptr_t list=0;unsigned count=0;
    if(!nativeDoorQueueAvailable()||!observedGame||!memoryRead(observedGame+0x601c0,&list,8)||!list||
       !memoryRead(observedGame+0x601c8,&count,4)||(count&0xffffff)>16384){refuse("catalog_unavailable");return;}
    std::vector<unsigned> handles(count&0xffffff);if(!handles.empty()&&!memoryRead(list,handles.data(),handles.size()*4)){refuse("catalog_unreadable");return;}
    std::vector<NativeOpenable> all;
    for(auto h:handles){NativeOpenable n{};if(readNativeOpenable(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,h,n))all.push_back(std::move(n));}
    auto catalog=selectNativeOpenableCatalog(std::move(all));doorCatalogAmbiguous=catalog.ambiguous;doorCatalogIneligible=catalog.ineligible;
    if(catalog.entries.empty()||catalog.entries.size()>128){refuse("eligible_catalog_empty_or_overflow");return;}
    for(auto& n:catalog.entries){DoorSync fresh{};fresh.enabled=true;fresh.session=outgoing.session;fresh.peerSession=remote.session;
        fresh.epoch=outgoing.worldEpoch;fresh.peerEpoch=remote.worldEpoch;fresh.map=outgoing.mapId;fresh.binding=std::move(n);
        fresh.observedTarget=fresh.binding.current;fresh.observedSide=fresh.binding.openSide;fresh.observedLocked=fresh.binding.locked;fresh.arm=++doorSyncArm;fresh.result="awaiting_peer";
        if(!(doorSession.enabled?doorSyncScope(fresh):doorSyncContext(fresh,GetTickCount64()))){doorChannels.clear();refuse("isolated_pair_context_required");return;}
        doorChannels.push_back(std::move(fresh));
    }
    doorCatalogScanMs=static_cast<unsigned>(GetTickCount64()-started);doorCatalogEnabled=true;doorCatalogResult="ordinary_door_catalog_armed";
}
void enableDoorSession(){
    if(doorSession.enabled||doorCatalogEnabled){++doorCatalogRejected;return;}
    // Explicit opt-in for the current paired map. Native AI remains active;
    // ordinary-door ownership/eligibility and exact API checks still apply.
    doorSession={};doorSession.scope=doorSessionScope(outgoing,remote);
    if(!coopWorldPairReady(outgoing,remote)||doorCheckpointBarrierActive()){doorSession.result="pair_not_ready";return;}
    doorSession.enabled=true;enableDoorSync(true);
    if(!doorCatalogEnabled){stopDoorSession("initial_catalog_refused");return;}
    doorSession.result="active";
}
bool sendDoorPacket(DoorSync& s,DoorMessage kind,ULONGLONG tick){
    if(!doorSyncContext(s,tick))return false;
    CoopDoorPacket p{};p.role=localRole;p.kind=kind;p.sequence=++doorSyncSequence;p.sourceSession=s.session;p.targetSession=s.peerSession;
    p.sourceEpoch=s.epoch;p.targetEpoch=s.peerEpoch;p.mapId=outgoing.mapId;p.sourceArm=s.arm;
    std::memcpy(p.name,s.binding.name.c_str(),s.binding.name.size());std::memcpy(p.definition,s.binding.definitionName.c_str(),s.binding.definitionName.size());
    if(kind!=DoorMessage::Hello){p.targetArm=s.peerArm;p.revision=s.order.revision;p.target=s.order.target;p.side=s.order.side;}
    if(kind==DoorMessage::Request){p.target=s.requestTarget;p.side=s.requestSide;}
    if(kind==DoorMessage::Unlock){p.revision=0;p.target=p.side=0;}
    if(kind==DoorMessage::State)p.requestAck=s.requestAck;
    if(!validCoopDoor(p,remote,outgoing)||coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)!=k_EResultOK)return false;
    ++s.sent;
    if(kind==DoorMessage::State||kind==DoorMessage::Request)doorSyncLastAction=p;
    if(kind==DoorMessage::Request){s.requestSequence=p.sequence;s.awaiting=true;s.retryAfterMenu=false;++s.requests;s.result="awaiting_host_order";}
    return true;
}
void receiveDoorChannel(DoorSync& s,const CoopDoorPacket& p,ULONGLONG tick){
    ++s.received;
    if(!doorSyncContext(s,tick)||!validCoopDoor(p,outgoing,remote)||s.binding.name!=p.name||s.binding.definitionName!=p.definition){++s.rejected;return;}
    if(p.kind==DoorMessage::Hello){
        if(s.peerArm&&p.sourceArm!=s.peerArm){failDoorSync(s,"peer_rearmed_requires_new_pair");return;}
        if(p.sequence<=s.peerSequence){++s.duplicates;return;}
        s.peerArm=p.sourceArm;s.peerSequence=p.sequence;s.helloAt=tick;s.ready=true;return;
    }
    // 1214jf: an unlock needs only the paired arms. It is kept until this world can apply it.
    if(p.kind==DoorMessage::Unlock){
        if(!s.peerArm||p.targetArm!=s.arm||p.sourceArm!=s.peerArm){++s.rejected;return;}
        if(p.sequence<=s.peerSequence){++s.duplicates;return;}
        s.peerSequence=p.sequence;++s.unlocksReceived;s.unlockOrdered=true;return;
    }
    if(!s.ready||p.targetArm!=s.arm||p.sourceArm!=s.peerArm){++s.rejected;return;}
    if(p.sequence<=s.peerSequence){++s.duplicates;return;}
    s.peerSequence=p.sequence;
    if(p.kind==DoorMessage::Request){
        if(p.revision>s.order.revision){++s.rejected;return;}
        s.requestAck=p.sequence;++s.requests;
        if(s.order.set(p.target,p.side)){++s.commits;s.deadline=tick+5000;s.applied=0;}
        else ++s.duplicates;
        s.stateSentAt=0;return;
    }
    if(p.kind==DoorMessage::State){
        if(p.revision<s.order.revision){++s.rejected;return;}
        if(p.revision==s.order.revision&&(p.target!=s.order.target||p.side!=s.order.side)){failDoorSync(s,"conflicting_same_revision");return;}
        if(s.order.accept(p.revision,p.target,p.side)){++s.commits;s.deadline=tick+5000;s.applied=0;}
        else ++s.duplicates;
        if(s.awaiting&&p.requestAck>=s.requestSequence){s.awaiting=false;s.requestSequence=0;}
        s.ack=0;return;
    }
    if(p.revision==s.order.revision&&p.target==s.order.target&&p.side==s.order.side)s.ack=p.revision;
    else ++s.rejected;
}
void serviceDoorChannel(DoorSync& s,ULONGLONG tick){
    if(!s.enabled)return;
    if(!doorSyncContext(s,tick)){
        if(doorSyncScope(s)){s.ready=false;++doorTransientPauses;return;}
        failDoorSync(s,"world_input_or_connection_scope_changed");return;}
    if(tick-s.helloSentAt>=channelHelloIntervalMs(s.peerArm&&s.helloAt)&&sendDoorPacket(s,DoorMessage::Hello,tick))s.helloSentAt=tick;
    s.ready=s.peerArm&&s.helloAt&&tick-s.helloAt<channelHelloReadyMs();
    if(!s.ready)return;
    NativeOpenable n{};unsigned target=0,side=0;
    if(!readBoundDoor(s,n)||!doorNativeDesired(n,target,side)){failDoorSync(s,"native_door_identity_or_eligibility_changed");return;}
    // 1214jf (#185): a key unlock in either world is applied once in the other through the native lock routine. Keys are
    // personal pickups, so the other player could never open that door (mission-locked-door-v1841b). An unlock applied
    // here is not sent back. A later native relock is not shared: door scripts run in both worlds.
    if(n.locked!=s.observedLocked){if(s.observedLocked&&!n.locked&&!s.unlockOrdered)s.unlockPending=true;s.observedLocked=n.locked;}
    if(s.unlockPending){if(!sendDoorPacket(s,DoorMessage::Unlock,tick)){failDoorSync(s,"unlock_send_failed");return;}s.unlockPending=false;++s.unlocksSent;s.result="unlock_sent";}
    if(s.unlockOrdered){
        if(!n.locked)s.unlockOrdered=false;
        else{
            NativeOpenable fresh{};
            if(!readBoundDoor(s,fresh)||!fresh.locked||fresh.current!=n.current||fresh.pending!=n.pending){failDoorSync(s,"native_lifetime_changed_before_unlock");return;}
            ++s.calls;if(!unlockNativeDoor(fresh)){failDoorSync(s,"native_unlock_refused");return;}
            s.unlockOrdered=false;s.observedLocked=0;++s.unlocksApplied;s.result="native_unlock_applied";return;
        }
    }
    if(!localRole&&!s.order.revision){s.order.set(target,side);++s.commits;s.deadline=tick+5000;}
    if(s.inFlight){
        if(tick>s.inFlightUntil){failDoorSync(s,"native_transition_timeout");return;}
        if(n.current==n.pending&&n.current!=1){if(n.current!=s.inFlightTarget){failDoorSync(s,"native_transition_did_not_reach_target");return;}s.inFlight=0;}
        s.observedTarget=target;s.observedSide=side;
    }else if(target!=s.observedTarget||(target==2&&side!=s.observedSide)){
        s.observedTarget=target;s.observedSide=side;
        if(!localRole){if(s.order.set(target,side)){++s.commits;s.applied=0;s.deadline=tick+5000;s.stateSentAt=0;}}
        else if(s.order.revision){s.requestTarget=target;s.requestSide=side;if(!sendDoorPacket(s,DoorMessage::Request,tick)){failDoorSync(s,"request_send_failed");return;}}
    }
    // The receiver can discard a reliable request at the menu boundary. A
    // fresh Hello on the same arms permits one new envelope for the retained
    // desired state. A newer local use above supersedes that old intent.
    if(localRole&&s.retryAfterMenu){
        if(s.awaiting&&!sendDoorPacket(s,DoorMessage::Request,tick)){failDoorSync(s,"resume_request_send_failed");return;}
        s.retryAfterMenu=false;
    }
    if(!localRole&&s.order.revision&&(s.ack!=s.order.revision||!s.stateSentAt)&&(!s.stateSentAt||tick-s.stateSentAt>=250)){
        if(sendDoorPacket(s,DoorMessage::State,tick))s.stateSentAt=tick;
    }
    if(!s.order.revision||s.awaiting)return;
    if(s.applied!=s.order.revision&&s.deadline&&tick>s.deadline){failDoorSync(s,"desired_state_timeout");return;}
    if(nativeOpenableMatchesTarget(n,s.order.target,s.order.side)){
        if(s.applied!=s.order.revision){s.applied=s.order.revision;++s.converged;s.result="native_state_converged";}
        if(localRole&&s.ack!=s.order.revision&&sendDoorPacket(s,DoorMessage::Ack,tick))s.ack=s.order.revision;
        return;
    }
    if(s.inFlight||n.current==1||n.pending!=n.current)return;
    // Simultaneous use from opposite sides can leave both native machines
    // Open while the actual door leaves disagree. Finish existing motion,
    // close from the actual side, then reopen to the latest host order.
    // The owned intermediate close must not become a new player request.
    const bool correctSide=nativeOpenableNeedsSideCorrection(n,s.order.target,s.order.side);
    const auto applyTarget=correctSide?0u:s.order.target;
    const auto applySide=applyTarget==0?n.openSide:s.order.side;
    // 1214jf: an order to open a door still locked here waits for the shared unlock (bounded by the desired-state deadline).
    if(n.locked){s.result="waiting_native_unlock";return;}
    if(!nativeOpenableTransitionAllowed(n,applyTarget,applySide)){failDoorSync(s,"native_transition_not_eligible");return;}
    NativeOpenable fresh{};
    if(!readBoundDoor(s,fresh)||fresh.current!=n.current||fresh.pending!=n.pending||fresh.openSide!=n.openSide||!nativeOpenableTransitionAllowed(fresh,applyTarget,applySide)){failDoorSync(s,"native_lifetime_changed_before_apply");return;}
    ++s.calls;
    if(!queueNativeDoorTransition(fresh,applyTarget,applySide)){failDoorSync(s,"native_queue_refused");return;}
    if(correctSide)++s.sideCorrections;
    s.inFlight=s.order.revision;s.inFlightTarget=applyTarget;s.inFlightUntil=tick+4000;
    s.observedTarget=applyTarget;s.observedSide=applySide;s.result=correctSide?"native_side_correction_queued":"native_transition_queued";
}
void receiveDoorPacket(const CoopDoorPacket& p,ULONGLONG tick){
    if(!doorCatalogEnabled){++doorCatalogIgnored;return;}
    // Nothing from a menu is applied or cached as a native mutation. On
    // resume, both sides refresh Hello; host state and outstanding client
    // intent are sent again with fresh envelopes and the existing arms.
    if(doorSession.enabled&&(doorSession.menuSuspended||
       (doorSessionCanRetainMenu(doorSession.scope,outgoing,remote)&&!coopWorldPairReady(outgoing,remote)))){++doorSession.menuPacketsIgnored;return;}
    if(!validCoopDoor(p,outgoing,remote)){++doorCatalogRejected;return;}
    for(auto& s:doorChannels)if(s.binding.name==p.name&&s.binding.definitionName==p.definition){receiveDoorChannel(s,p,tick);return;}
    ++doorCatalogUnmatched;
}
void serviceDoorSync(ULONGLONG tick){
    if(doorSyncEnableRequest>=0){const auto mode=doorSyncEnableRequest;doorSyncEnableRequest=-1;if(mode==2)enableDoorSession();else enableDoorSync(mode!=0);}
    if(doorSession.enabled){
        auto& s=doorSession;
        if(!sockets||!connection||connectionState!=k_ESteamNetworkingConnectionState_Connected||
           outgoing.session!=s.scope.session||remote.session!=s.scope.peerSession){stopDoorSession("session_disconnected");return;}
        if(doorCheckpointBarrierActive()){retireDoorCatalog("shared_checkpoint");return;}
        if(s.reloadPending){
            if(tick>s.deadline){stopDoorSession("checkpoint_renewal_timeout");return;}
            if(!s.reloadReady||!doorSessionRenewalReady(s.scope,outgoing,remote)||!lastReceive||tick-lastReceive>=250)return;
            s.scope=doorSessionScope(outgoing,remote);s.reloadPending=s.reloadReady=false;
            enableDoorSync(true);
            if(!doorCatalogEnabled){stopDoorSession("renewed_catalog_refused");return;}
            ++s.rearms;s.result="renewed_after_shared_checkpoint";
        }else if(!doorSessionWorldMatches(s.scope,outgoing,remote)){stopDoorSession("uncoordinated_world_change");return;}
        if(!doorSessionCanRetainMenu(s.scope,outgoing,remote)){stopDoorSession("unsupported_native_phase");return;}
        if(!lastReceive||tick-lastReceive>=kSessionStallLimitMs){stopDoorSession("session_pose_stale");return;}
        if(tick-lastReceive>=1000||!coopWorldPairReady(outgoing,remote)){
            if(!s.menuSuspended){s.menuSuspended=true;s.menuSince=tick;++s.menuSuspensions;for(auto& c:doorChannels)c.ready=false;}
            s.result="native_menu_suspended";return;
        }
        if(s.menuSuspended){
            if(tick-lastReceive>=250)return;
            const auto paused=tick>=s.menuSince?tick-s.menuSince:0;
            for(auto& c:doorChannels)if(c.enabled){
                c.deadline=doorResumeDeadline(c.deadline,paused);c.inFlightUntil=doorResumeDeadline(c.inFlightUntil,paused);
                c.ready=false;c.helloAt=c.helloSentAt=c.stateSentAt=c.ack=0;c.retryAfterMenu=c.awaiting;
                // 1214jf: the peer may have dropped an unlock at its menu boundary; send it again (a duplicate is a no-op).
                c.unlockPending=c.unlockPending||(c.unlocksSent&&!c.observedLocked);
            }
            s.menuSuspended=false;s.menuSince=0;++s.menuResumes;s.menuPausedMs+=paused;s.result="resumed_after_native_menu";
        }
    }
    if(!doorCatalogEnabled)return;
    if(doorSyncDuplicateRequest){doorSyncDuplicateRequest=false;bool sent=false;const auto& p=doorSyncLastAction;
        for(auto& s:doorChannels)if(s.binding.name==p.name&&s.binding.definitionName==p.definition&&doorSyncContext(s,tick)&&
            p.sequence&&p.sourceArm==s.arm&&p.targetArm==s.peerArm&&validCoopDoor(p,remote,outgoing)){
            if(coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)==k_EResultOK){++s.sent;sent=true;}break;}
        if(!sent)++doorCatalogRejected;
    }
    const auto started=GetTickCount64();
    for(unsigned work=0;work<doorChannels.size();++work){
        auto& s=doorChannels[doorCatalogCursor];doorCatalogCursor=(doorCatalogCursor+1)%static_cast<unsigned>(doorChannels.size());
        serviceDoorChannel(s,tick);if(GetTickCount64()-started>=3)break;
    }
    doorServiceMs=static_cast<unsigned>(GetTickCount64()-started);doorServiceMaxMs=(std::max)(doorServiceMaxMs,doorServiceMs);
    bool live=false;for(const auto& s:doorChannels)live=live||s.enabled;
    if(!live){doorCatalogEnabled=false;doorCatalogResult="all_channels_retired";if(doorSession.enabled)stopDoorSession("all_channels_retired");}
}
void writeDoorSync(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.door-sync.json",statusPath);std::snprintf(temp,sizeof(temp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\"session\":{\"enabled\":%s,\"reloadPending\":%s,\"reloadReady\":%s,\"localSession\":%llu,\"peerSession\":%llu,\"epoch\":%llu,\"peerEpoch\":%llu,\"map\":%llu,\"reloads\":%llu,\"rearms\":%llu,\"retiredChannels\":%llu,\"retiredNativeCalls\":%llu,\"transientPauses\":%llu,\"result\":\"%s\"},",doorSession.enabled?"true":"false",doorSession.reloadPending?"true":"false",doorSession.reloadReady?"true":"false",doorSession.scope.session,doorSession.scope.peerSession,doorSession.scope.epoch,doorSession.scope.peerEpoch,doorSession.scope.map,doorSession.reloads,doorSession.rearms,doorSession.retiredChannels,doorSession.retiredNativeCalls,doorTransientPauses,doorSession.result);
    fprintf(f,"\"menu\":{\"suspended\":%s,\"since\":%llu,\"suspensions\":%llu,\"resumes\":%llu,\"pausedMs\":%llu,\"packetsIgnored\":%llu},",doorSession.menuSuspended?"true":"false",doorSession.menuSince,doorSession.menuSuspensions,doorSession.menuResumes,doorSession.menuPausedMs,doorSession.menuPacketsIgnored);
    fprintf(f,"\"experimental\":true,\"enabled\":%s,\"catalogCount\":%zu,\"ambiguous\":%u,\"ineligible\":%u,\"ignoredWhileDisabled\":%llu,\"unmatched\":%llu,\"rejected\":%llu,\"scanMs\":%u,\"serviceMs\":%u,\"serviceMaxMs\":%u,\"result\":\"%s\",\"entries\":[",doorCatalogEnabled?"true":"false",doorChannels.size(),doorCatalogAmbiguous,doorCatalogIneligible,doorCatalogIgnored,doorCatalogUnmatched,doorCatalogRejected,doorCatalogScanMs,doorServiceMs,doorServiceMaxMs,doorCatalogResult);
    bool first=true;for(const auto& s:doorChannels){if(!first)fputc(',',f);first=false;fputs("{\"name\":",f);writeWorldText(f,s.binding.name.c_str());fputs(",\"definition\":",f);writeWorldText(f,s.binding.definitionName.c_str());
    fprintf(f,",\"role\":%u,\"enabled\":%s,\"ready\":%s,\"arm\":%llu,\"peerArm\":%llu,\"revision\":%llu,\"target\":%u,\"side\":%u,\"applied\":%llu,\"ack\":%llu,\"awaiting\":%s,\"inFlight\":%llu,\"sent\":%llu,\"received\":%llu,\"rejected\":%llu,\"duplicates\":%llu,\"requests\":%llu,\"commits\":%llu,\"nativeCalls\":%llu,\"converged\":%llu,\"sideCorrections\":%llu,\"locked\":%u,\"unlocksSent\":%llu,\"unlocksReceived\":%llu,\"unlocksApplied\":%llu,\"result\":\"%s\"}",localRole,s.enabled?"true":"false",s.ready?"true":"false",s.arm,s.peerArm,s.order.revision,s.order.target,s.order.side,s.applied,s.ack,s.awaiting?"true":"false",s.inFlight,s.sent,s.received,s.rejected,s.duplicates,s.requests,s.commits,s.calls,s.converged,s.sideCorrections,unsigned(s.observedLocked),s.unlocksSent,s.unlocksReceived,s.unlocksApplied,s.result);
    }
    fputs("]}\n",f);const bool ok=!ferror(f);const auto closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
