#pragma once
#include "usable_protocol.h"
#include "usable_native_api.h"
#include "usable_catalog.h"
#include "door_session_scope.h"
// Opt-in native two-state toggle family (1123: map-agnostic; first coverage was
// Dunwall windows). Quest devices, vaults and non-uniform prompts stay ineligible.
struct UsableSync {
    bool enabled=false,ready=false,awaiting=false,retryAfterMenu=false;
    NativeUsable binding{};UsableOrder order{};
    std::uint64_t arm=0,peerArm=0,session=0,peerSession=0,epoch=0,peerEpoch=0,map=0,peerSequence=0;
    std::uint64_t helloAt=0,helloSentAt=0,stateSentAt=0,deadline=0,ack=0,requestAck=0,requestSequence=0;
    std::uint64_t inFlight=0,inFlightUntil=0,applied=0;
    unsigned observedTarget=0,requestTarget=0,inFlightTarget=0;
    std::uint64_t sent=0,received=0,rejected=0,duplicates=0,requests=0,commits=0,calls=0,converged=0;
    const char* result="disabled";
};
std::vector<UsableSync> usableChannels;
bool usableCatalogEnabled=false;
unsigned usableCatalogAmbiguous=0,usableCatalogIneligible=0,usableCatalogCursor=0,usableCatalogScanMs=0,usableServiceMs=0,usableServiceMaxMs=0;
std::uint64_t usableCatalogIgnored=0,usableCatalogUnmatched=0,usableCatalogRejected=0;
const char* usableCatalogResult="disabled";
std::uint64_t usableSyncArm=0,usableSyncSequence=0;
int usableSyncEnableRequest=-1;
bool usableSyncDuplicateRequest=false;
CoopUsablePacket usableSyncLastAction{};
struct UsableSession {
    bool enabled=false,reloadPending=false,reloadReady=false,menuSuspended=false;
    DoorSessionScope scope{};
    std::uint64_t deadline=0,reloads=0,rearms=0,retiredChannels=0,retiredNativeCalls=0;
    std::uint64_t menuSince=0,menuSuspensions=0,menuResumes=0,menuPausedMs=0,menuPacketsIgnored=0;
    std::uint64_t transientPauses=0;
    const char* result="disabled";
};
UsableSession usableSession{};
bool doorCheckpointBarrierActive();
void retireUsableCatalog(const char* reason){
    if(usableCatalogEnabled){
        for(auto& s:usableChannels){++usableSession.retiredChannels;usableSession.retiredNativeCalls+=s.calls;s.enabled=s.ready=false;s.result=reason;}
    }
    usableCatalogEnabled=false;usableSyncDuplicateRequest=false;usableSyncLastAction={};usableCatalogResult=reason;
}
void stopUsableSession(const char* reason){
    usableSession.enabled=usableSession.reloadPending=usableSession.reloadReady=usableSession.menuSuspended=false;usableSession.menuSince=0;usableSession.result=reason;
    retireUsableCatalog(reason);
}
void beginUsableCheckpointRecovery(){
    auto& s=usableSession;if(!s.enabled)return;
    // The other native world may already be inside loading. Its map/epoch
    // will be checked at renewal; never require an old peer world here.
    if(outgoing.session!=s.scope.session||remote.session!=s.scope.peerSession||
       outgoing.mapId!=s.scope.map||outgoing.worldEpoch!=s.scope.epoch){stopUsableSession("checkpoint_session_changed");return;}
    s.reloadPending=true;s.reloadReady=false;s.menuSuspended=false;s.menuSince=0;s.deadline=GetTickCount64()+60000;++s.reloads;s.result="waiting_shared_checkpoint";
    retireUsableCatalog("shared_checkpoint");
}
void finishUsableCheckpointRecovery(){
    auto& s=usableSession;if(!s.enabled||!s.reloadPending)return;
    // Called only after the checkpoint protocol's both-complete boundary and
    // local role/counterpart restoration. Each peer must have a new epoch.
    if(doorSessionRenewalReady(s.scope,outgoing,remote))s.reloadReady=true;
}
bool usableSyncContext(const UsableSync& s,ULONGLONG tick){
    return s.enabled&&observedGame==s.binding.game&&nativeMutationAllowed&&GetCurrentThreadId()==engineThread.load()&&
        sockets&&connection&&connectionState==k_ESteamNetworkingConnectionState_Connected&&lastReceive&&tick-lastReceive<250&&
        outgoing.session==s.session&&remote.session==s.peerSession&&outgoing.worldEpoch==s.epoch&&remote.worldEpoch==s.peerEpoch&&
        coopWorldPairReady(outgoing,remote)&&outgoing.mapId==s.map&&
        ((usableSession.enabled&&!usableSession.reloadPending&&!usableSession.menuSuspended&&doorSessionWorldMatches(usableSession.scope,outgoing,remote))||
         (!usableSession.enabled&&observerIsolation.ready&&observerIsolation.movementFixture&&observerIsolation.until>tick+1000));
}
// 1123c: identity scope only (session/epoch/map/game/thread). Inside an armed
// session, transient gaps (pose stall <1 s, native mutation gate, pair
// readiness blips) pause the channel; the session enforces the hard limits.
bool usableSyncScope(const UsableSync& s){
    return s.enabled&&observedGame==s.binding.game&&GetCurrentThreadId()==engineThread.load()&&sockets&&connection&&
        connectionState==k_ESteamNetworkingConnectionState_Connected&&outgoing.session==s.session&&remote.session==s.peerSession&&
        outgoing.worldEpoch==s.epoch&&remote.worldEpoch==s.peerEpoch&&outgoing.mapId==s.map&&
        usableSession.enabled&&!usableSession.reloadPending&&doorSessionWorldMatches(usableSession.scope,outgoing,remote);
}
void failUsableSync(UsableSync& s,const char* reason){s.enabled=s.ready=false;s.result=reason;++s.rejected;}
bool readBoundUsable(const UsableSync& s,NativeUsable& current){
    const auto& b=s.binding;
    return readNativeUsable(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,b.handle,current)&&sameNativeUsableIdentity(current,b);
}
bool usableNativeDesired(const NativeUsable& n,unsigned& target){
    if(!nativeUsableToggleFamily(n))return false;
    target=static_cast<unsigned>(n.pending>=0?n.pending:n.current);
    return nativeUsableTargetValid(n,target);
}
void enableUsableSync(bool on){
    if(!on){stopUsableSession("disabled");return;}
    if(usableCatalogEnabled){++usableCatalogRejected;return;}
    const auto started=GetTickCount64();usableChannels.clear();usableCatalogCursor=0;usableSyncLastAction={};usableSyncDuplicateRequest=false;
    usableCatalogIgnored=usableCatalogUnmatched=usableCatalogRejected=0;usableCatalogAmbiguous=usableCatalogIneligible=0;usableServiceMs=usableServiceMaxMs=0;
    const auto refuse=[](const char* reason){usableCatalogResult=reason;++usableCatalogRejected;};
    std::uintptr_t list=0;unsigned count=0;
    // 1123: map-agnostic (per-channel pairing intersects per-character worlds).
    if(!coopWorldPairReady(outgoing,remote)||!nativeUsableIndexAvailable()||!observedGame||!memoryRead(observedGame+0x601c0,&list,8)||!list||
       !memoryRead(observedGame+0x601c8,&count,4)||(count&0xffffff)>16384){refuse("catalog_unavailable");return;}
    std::vector<unsigned> handles(count&0xffffff);if(!handles.empty()&&!memoryRead(list,handles.data(),handles.size()*4)){refuse("catalog_unreadable");return;}
    std::vector<NativeUsable> all;
    for(auto h:handles){NativeUsable n{};if(readNativeUsable(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,h,n))all.push_back(std::move(n));}
    auto catalog=selectNativeUsableCatalog(std::move(all));usableCatalogAmbiguous=catalog.ambiguous;usableCatalogIneligible=catalog.ineligible;
    if(catalog.entries.empty()||catalog.entries.size()>256){refuse("eligible_catalog_empty_or_overflow");return;}
    for(auto& n:catalog.entries){UsableSync fresh{};fresh.enabled=true;fresh.session=outgoing.session;fresh.peerSession=remote.session;
        fresh.epoch=outgoing.worldEpoch;fresh.peerEpoch=remote.worldEpoch;fresh.map=outgoing.mapId;fresh.binding=std::move(n);
        fresh.observedTarget=fresh.binding.current;fresh.arm=++usableSyncArm;fresh.result="awaiting_peer";
        // 1123d: a session arm checks identity scope only; transient freshness
        // (e.g. right after an intro cinematic) pauses channels instead.
        if(!(usableSession.enabled?usableSyncScope(fresh):usableSyncContext(fresh,GetTickCount64()))){usableChannels.clear();refuse("isolated_pair_context_required");return;}
        usableChannels.push_back(std::move(fresh));
    }
    usableCatalogScanMs=static_cast<unsigned>(GetTickCount64()-started);usableCatalogEnabled=true;usableCatalogResult="native_toggle_family_armed";
}
void enableUsableSession(){
    if(usableSession.enabled||usableCatalogEnabled){++usableCatalogRejected;return;}
    // Explicit opt-in for the current paired map. Native AI remains active;
    // ordinary-usable ownership/eligibility and exact API checks still apply.
    usableSession={};usableSession.scope=doorSessionScope(outgoing,remote);
    if(!coopWorldPairReady(outgoing,remote)||doorCheckpointBarrierActive()){usableSession.result="pair_not_ready";return;}
    usableSession.enabled=true;enableUsableSync(true);
    if(!usableCatalogEnabled){stopUsableSession("initial_catalog_refused");return;}
    usableSession.result="active";
}
bool sendUsablePacket(UsableSync& s,UsableMessage kind,ULONGLONG tick){
    if(!usableSyncContext(s,tick))return false;
    CoopUsablePacket p{};p.role=localRole;p.kind=kind;p.sequence=++usableSyncSequence;p.sourceSession=s.session;p.targetSession=s.peerSession;
    p.sourceEpoch=s.epoch;p.targetEpoch=s.peerEpoch;p.mapId=outgoing.mapId;p.sourceArm=s.arm;p.stateCount=s.binding.stateCount;p.rulesHash=nativeUsableRulesHash(s.binding);
    std::memcpy(p.name,s.binding.name.c_str(),s.binding.name.size());std::memcpy(p.definition,s.binding.definitionName.c_str(),s.binding.definitionName.size());
    if(kind!=UsableMessage::Hello){p.targetArm=s.peerArm;p.revision=s.order.revision;p.target=s.order.target;}
    if(kind==UsableMessage::Request){p.target=s.requestTarget;}
    if(kind==UsableMessage::State)p.requestAck=s.requestAck;
    if(!validCoopUsable(p,remote,outgoing)||coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)!=k_EResultOK)return false;
    ++s.sent;
    if(kind==UsableMessage::State||kind==UsableMessage::Request)usableSyncLastAction=p;
    if(kind==UsableMessage::Request){s.requestSequence=p.sequence;s.awaiting=true;s.retryAfterMenu=false;++s.requests;s.result="awaiting_host_order";}
    return true;
}
void receiveUsableChannel(UsableSync& s,const CoopUsablePacket& p,ULONGLONG tick){
    ++s.received;
    if(!usableSyncContext(s,tick)||!validCoopUsable(p,outgoing,remote)||s.binding.name!=p.name||s.binding.definitionName!=p.definition||p.stateCount!=s.binding.stateCount||p.rulesHash!=nativeUsableRulesHash(s.binding)){++s.rejected;return;}
    if(p.kind==UsableMessage::Hello){
        if(s.peerArm&&p.sourceArm!=s.peerArm){failUsableSync(s,"peer_rearmed_requires_new_pair");return;}
        if(p.sequence<=s.peerSequence){++s.duplicates;return;}
        s.peerArm=p.sourceArm;s.peerSequence=p.sequence;s.helloAt=tick;s.ready=true;return;
    }
    if(!s.ready||p.targetArm!=s.arm||p.sourceArm!=s.peerArm){++s.rejected;return;}
    if(p.sequence<=s.peerSequence){++s.duplicates;return;}
    s.peerSequence=p.sequence;
    if(p.kind==UsableMessage::Request){
        if(p.revision>s.order.revision){++s.rejected;return;}
        s.requestAck=p.sequence;++s.requests;
        if(s.order.set(p.target)){++s.commits;s.deadline=tick+5000;s.applied=0;}
        else ++s.duplicates;
        s.stateSentAt=0;return;
    }
    if(p.kind==UsableMessage::State){
        if(p.revision<s.order.revision){++s.rejected;return;}
        if(p.revision==s.order.revision&&p.target!=s.order.target){failUsableSync(s,"conflicting_same_revision");return;}
        if(s.order.accept(p.revision,p.target)){++s.commits;s.deadline=tick+5000;s.applied=0;}
        else ++s.duplicates;
        if(s.awaiting&&p.requestAck>=s.requestSequence){s.awaiting=false;s.requestSequence=0;}
        s.ack=0;return;
    }
    if(p.revision==s.order.revision&&p.target==s.order.target)s.ack=p.revision;
    else ++s.rejected;
}
void serviceUsableChannel(UsableSync& s,ULONGLONG tick){
    if(!s.enabled)return;
    if(!usableSyncContext(s,tick)){
        if(usableSyncScope(s)){s.ready=false;++usableSession.transientPauses;return;}
        failUsableSync(s,"world_input_or_connection_scope_changed");return;}
    if(tick-s.helloSentAt>=channelHelloIntervalMs(s.peerArm&&s.helloAt)&&sendUsablePacket(s,UsableMessage::Hello,tick))s.helloSentAt=tick;
    s.ready=s.peerArm&&s.helloAt&&tick-s.helloAt<channelHelloReadyMs();
    if(!s.ready)return;
    NativeUsable n{};unsigned target=0;
    if(!readBoundUsable(s,n)||!usableNativeDesired(n,target)){failUsableSync(s,"native_usable_identity_or_eligibility_changed");return;}
    if(!localRole&&!s.order.revision){s.order.set(target);++s.commits;s.deadline=tick+5000;}
    if(s.inFlight){
        if(tick>s.inFlightUntil){failUsableSync(s,"native_transition_timeout");return;}
        if(!n.transitioning()&&n.pending== -1){if(n.current!=s.inFlightTarget){failUsableSync(s,"native_transition_did_not_reach_target");return;}s.inFlight=0;}
        s.observedTarget=target;
    }else if(target!=s.observedTarget){
        s.observedTarget=target;
        if(!localRole){if(s.order.set(target)){++s.commits;s.applied=0;s.deadline=tick+5000;s.stateSentAt=0;}}
        else if(s.order.revision){s.requestTarget=target;if(!sendUsablePacket(s,UsableMessage::Request,tick)){failUsableSync(s,"request_send_failed");return;}}
    }
    // The receiver can discard a reliable request at the menu boundary. A
    // fresh Hello on the same arms permits one new envelope for the retained
    // desired state. A newer local use above supersedes that old intent.
    if(localRole&&s.retryAfterMenu){
        if(s.awaiting&&!sendUsablePacket(s,UsableMessage::Request,tick)){failUsableSync(s,"resume_request_send_failed");return;}
        s.retryAfterMenu=false;
    }
    if(!localRole&&s.order.revision&&(s.ack!=s.order.revision||!s.stateSentAt)&&(!s.stateSentAt||tick-s.stateSentAt>=250)){
        if(sendUsablePacket(s,UsableMessage::State,tick))s.stateSentAt=tick;
    }
    if(!s.order.revision||s.awaiting)return;
    if(s.applied!=s.order.revision&&s.deadline&&tick>s.deadline){failUsableSync(s,"desired_state_timeout");return;}
    if(nativeUsableAtTarget(n,s.order.target)){
        if(s.applied!=s.order.revision){s.applied=s.order.revision;++s.converged;s.result="native_state_converged";}
        if(localRole&&s.ack!=s.order.revision&&sendUsablePacket(s,UsableMessage::Ack,tick))s.ack=s.order.revision;
        return;
    }
    if(s.inFlight||n.transitioning()||n.pending!= -1)return;
    if(!nativeUsableTargetValid(n,s.order.target)){failUsableSync(s,"native_transition_not_eligible");return;}
    NativeUsable fresh{},after{};
    if(!readBoundUsable(s,fresh)||fresh.current!=n.current||fresh.pending!= -1||fresh.transitioning()||
       !nativeUsableToggleFamily(fresh)||!nativeUsableTargetValid(fresh,s.order.target)){failUsableSync(s,"native_lifetime_changed_before_apply");return;}
    ++s.calls;
    // 1214jh: the replica's script events name this world's player as the user, so a lever's script runs here too.
    // 1214ji: a two-state usable goes through the native use method instead, as a player's use does.
    if(fresh.stateCount==2?!useNativeUsableAsLocalPlayer(fresh,s.order.target,after):!requestNativeUsableIndex(fresh,s.order.target,after,true)){failUsableSync(s,"native_index_refused");return;}
    s.inFlight=s.order.revision;s.inFlightTarget=s.order.target;s.inFlightUntil=tick+4000;
    s.observedTarget=s.order.target;s.result="native_transition_requested";

}
void receiveUsablePacket(const CoopUsablePacket& p,ULONGLONG tick){
    if(!usableCatalogEnabled){++usableCatalogIgnored;return;}
    // Nothing from a menu is applied or cached as a native mutation. On
    // resume, both sides refresh Hello; host state and outstanding client
    // intent are sent again with fresh envelopes and the existing arms.
    if(usableSession.enabled&&(usableSession.menuSuspended||
       (doorSessionCanRetainMenu(usableSession.scope,outgoing,remote)&&!coopWorldPairReady(outgoing,remote)))){++usableSession.menuPacketsIgnored;return;}
    if(!validCoopUsable(p,outgoing,remote)){++usableCatalogRejected;return;}
    for(auto& s:usableChannels)if(s.binding.name==p.name&&s.binding.definitionName==p.definition){receiveUsableChannel(s,p,tick);return;}
    ++usableCatalogUnmatched;
}
void serviceUsableSync(ULONGLONG tick){
    if(usableSyncEnableRequest>=0){const auto mode=usableSyncEnableRequest;usableSyncEnableRequest=-1;if(mode==2)enableUsableSession();else enableUsableSync(mode!=0);}
    if(usableSession.enabled){
        auto& s=usableSession;
        if(!sockets||!connection||connectionState!=k_ESteamNetworkingConnectionState_Connected||
           outgoing.session!=s.scope.session||remote.session!=s.scope.peerSession){stopUsableSession("session_disconnected");return;}
        if(doorCheckpointBarrierActive()){retireUsableCatalog("shared_checkpoint");return;}
        if(s.reloadPending){
            if(tick>s.deadline){stopUsableSession("checkpoint_renewal_timeout");return;}
            if(!s.reloadReady||!doorSessionRenewalReady(s.scope,outgoing,remote)||!lastReceive||tick-lastReceive>=250)return;
            s.scope=doorSessionScope(outgoing,remote);s.reloadPending=s.reloadReady=false;
            enableUsableSync(true);
            if(!usableCatalogEnabled){stopUsableSession("renewed_catalog_refused");return;}
            ++s.rearms;s.result="renewed_after_shared_checkpoint";
        }else if(!doorSessionWorldMatches(s.scope,outgoing,remote)){stopUsableSession("uncoordinated_world_change");return;}
        if(!doorSessionCanRetainMenu(s.scope,outgoing,remote)){stopUsableSession("unsupported_native_phase");return;}
        if(!lastReceive||tick-lastReceive>=kSessionStallLimitMs){stopUsableSession("session_pose_stale");return;}
        if(tick-lastReceive>=1000||!coopWorldPairReady(outgoing,remote)){
            if(!s.menuSuspended){s.menuSuspended=true;s.menuSince=tick;++s.menuSuspensions;for(auto& c:usableChannels)c.ready=false;}
            s.result="native_menu_suspended";return;
        }
        if(s.menuSuspended){
            if(tick-lastReceive>=250)return;
            const auto paused=tick>=s.menuSince?tick-s.menuSince:0;
            for(auto& c:usableChannels)if(c.enabled){
                c.deadline=doorResumeDeadline(c.deadline,paused);c.inFlightUntil=doorResumeDeadline(c.inFlightUntil,paused);
                c.ready=false;c.helloAt=c.helloSentAt=c.stateSentAt=c.ack=0;c.retryAfterMenu=c.awaiting;
            }
            s.menuSuspended=false;s.menuSince=0;++s.menuResumes;s.menuPausedMs+=paused;s.result="resumed_after_native_menu";
        }
    }
    if(!usableCatalogEnabled)return;
    if(usableSyncDuplicateRequest){usableSyncDuplicateRequest=false;bool sent=false;const auto& p=usableSyncLastAction;
        for(auto& s:usableChannels)if(s.binding.name==p.name&&s.binding.definitionName==p.definition&&usableSyncContext(s,tick)&&
            p.sequence&&p.sourceArm==s.arm&&p.targetArm==s.peerArm&&validCoopUsable(p,remote,outgoing)){
            if(coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)==k_EResultOK){++s.sent;sent=true;}break;}
        if(!sent)++usableCatalogRejected;
    }
    const auto started=GetTickCount64();
    for(unsigned work=0;work<usableChannels.size();++work){
        auto& s=usableChannels[usableCatalogCursor];usableCatalogCursor=(usableCatalogCursor+1)%static_cast<unsigned>(usableChannels.size());
        serviceUsableChannel(s,tick);if(GetTickCount64()-started>=3)break;
    }
    usableServiceMs=static_cast<unsigned>(GetTickCount64()-started);usableServiceMaxMs=(std::max)(usableServiceMaxMs,usableServiceMs);
    bool live=false;for(const auto& s:usableChannels)live=live||s.enabled;
    if(!live){usableCatalogEnabled=false;usableCatalogResult="all_channels_retired";if(usableSession.enabled)stopUsableSession("all_channels_retired");}
}
void writeUsableSync(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.usable-sync.json",statusPath);std::snprintf(temp,sizeof(temp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\"session\":{\"enabled\":%s,\"reloadPending\":%s,\"reloadReady\":%s,\"localSession\":%llu,\"peerSession\":%llu,\"epoch\":%llu,\"peerEpoch\":%llu,\"map\":%llu,\"reloads\":%llu,\"rearms\":%llu,\"retiredChannels\":%llu,\"retiredNativeCalls\":%llu,\"transientPauses\":%llu,\"menuSuspensions\":%llu,\"menuResumes\":%llu,\"menuPausedMs\":%llu,\"result\":\"%s\"},",usableSession.enabled?"true":"false",usableSession.reloadPending?"true":"false",usableSession.reloadReady?"true":"false",usableSession.scope.session,usableSession.scope.peerSession,usableSession.scope.epoch,usableSession.scope.peerEpoch,usableSession.scope.map,usableSession.reloads,usableSession.rearms,usableSession.retiredChannels,usableSession.retiredNativeCalls,usableSession.transientPauses,usableSession.menuSuspensions,usableSession.menuResumes,usableSession.menuPausedMs,usableSession.result);
    fprintf(f,"\"menu\":{\"suspended\":%s,\"since\":%llu,\"suspensions\":%llu,\"resumes\":%llu,\"pausedMs\":%llu,\"packetsIgnored\":%llu},",usableSession.menuSuspended?"true":"false",usableSession.menuSince,usableSession.menuSuspensions,usableSession.menuResumes,usableSession.menuPausedMs,usableSession.menuPacketsIgnored);
    fprintf(f,"\"experimental\":true,\"enabled\":%s,\"catalogCount\":%zu,\"ambiguous\":%u,\"ineligible\":%u,\"ignoredWhileDisabled\":%llu,\"unmatched\":%llu,\"rejected\":%llu,\"scanMs\":%u,\"serviceMs\":%u,\"serviceMaxMs\":%u,\"result\":\"%s\",\"entries\":[",usableCatalogEnabled?"true":"false",usableChannels.size(),usableCatalogAmbiguous,usableCatalogIneligible,usableCatalogIgnored,usableCatalogUnmatched,usableCatalogRejected,usableCatalogScanMs,usableServiceMs,usableServiceMaxMs,usableCatalogResult);
    bool first=true;for(const auto& s:usableChannels){if(!first)fputc(',',f);first=false;fputs("{\"name\":",f);writeWorldText(f,s.binding.name.c_str());fputs(",\"definition\":",f);writeWorldText(f,s.binding.definitionName.c_str());
    fprintf(f,",\"role\":%u,\"enabled\":%s,\"ready\":%s,\"arm\":%llu,\"peerArm\":%llu,\"revision\":%llu,\"target\":%u,\"applied\":%llu,\"ack\":%llu,\"awaiting\":%s,\"inFlight\":%llu,\"sent\":%llu,\"received\":%llu,\"rejected\":%llu,\"duplicates\":%llu,\"requests\":%llu,\"commits\":%llu,\"nativeCalls\":%llu,\"converged\":%llu,\"result\":\"%s\"}",localRole,s.enabled?"true":"false",s.ready?"true":"false",s.arm,s.peerArm,s.order.revision,s.order.target,s.applied,s.ack,s.awaiting?"true":"false",s.inFlight,s.sent,s.received,s.rejected,s.duplicates,s.requests,s.commits,s.calls,s.converged,s.result);
    }
    fputs("]}\n",f);const bool ok=!ferror(f);const auto closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
