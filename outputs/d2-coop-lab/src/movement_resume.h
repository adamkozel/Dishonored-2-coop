#pragma once
#include "movement_resume_policy.h"
#include "native_readable_pause.h"
struct MovementResume {
    MovementResumePolicy policy;
    std::uint64_t checksum=0,token=0,peerToken=0,armed=0,suspended=0,resumed=0,refused=0;
    ULONGLONG nextAttempt=0;bool nativeFeet=false,currentFeet=false,readNoteVisibleHold=false;
    const char* result="off";
} movementResume;
MovementResumePolicy::Identity movementResumeIdentity(){
    return {observedGame,probeActor,probeHandle,outgoing.worldEpoch,remote.worldEpoch,outgoing.session,remote.session,outgoing.mapId};
}
bool movementMenuPaused(){
    for(auto state:{outgoing.gameLogicState,outgoing.pendingGameLogicState,remote.gameLogicState,remote.pendingGameLogicState})
        if(nativeMovementMenuState(state))return true;
    return false;
}
void stopMovementResume(){movementResume.policy.off();movementResume.readNoteVisibleHold=false;movementResume.result="off";}
bool movementRetainsReadableBody(ULONGLONG tick){
    auto& r=movementResume;
    std::uintptr_t frame=0,frameVtable=0,frameOwner=0;
    const auto image=reinterpret_cast<std::uintptr_t>(gameImage);
    const bool owned=probeDoppelganger&&probeFollow&&avatarAiHeld&&probeGame==observedGame&&
        ownedAvatarAiReference(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),
            shadowDamageIdentity(),avatarAiTarget,avatarAiEpoch,outgoing.worldEpoch)&&
        networkFrameOwned.load(std::memory_order_acquire)&&
        memoryRead(probeActor+0x358,&frame,8)&&frame&&
        controlledFrame.load(std::memory_order_acquire)==reinterpret_cast<void*>(frame)&&
        memoryRead(frame,&frameVtable,8)&&frameVtable==image+0x1c573e8&&
        memoryRead(frame+0x10,&frameOwner,8)&&frameOwner==probeActor;
    return retainNativeReadableBody(outgoing,remote,r.policy.enabled()&&r.policy.identity==movementResumeIdentity(),
        connectionState==k_ESteamNetworkingConnectionState_Connected&&lastReceive&&tick-lastReceive<presentationFreshMs(),
        // Once enableProbeFollow restores fresh native root ownership, let
        // normal follow run while the new semantic handshake/bank starts.
        // The hold keeps ownership of the last root; enableProbeFollow still
        // ends phaseFollowInvalidated before a fresh semantic handshake.
        owned,r.policy.phase!=MovementResumePolicy::Running&&phaseFollowInvalidated,r.readNoteVisibleHold);
}
void beginMovementResume(){
    auto& r=movementResume;const auto tick=GetTickCount64();
    const bool live=movementBankActive()&&shadowStreamCurrent(tick)&&shadowStream.peerReady&&shadowStream.persistent&&
        !movementMenuPaused()&&!shadowEquipment.restoreUntil&&!reviewClipActive()&&!authoringProof.end;
    if(!r.policy.arm(movementResumeIdentity(),live)){++r.refused;r.result="live_owned_presentation_required";return;}
    AcquireSRWLockShared(&movementLock);r.checksum=movementPlayback.bank.checksum;r.nativeFeet=movementPlayback.nativeFeet;r.currentFeet=movementPlayback.currentFeet;ReleaseSRWLockShared(&movementLock);
    r.token=shadowStream.token;r.peerToken=shadowStream.peerToken;r.nextAttempt=0;r.readNoteVisibleHold=false;++r.armed;r.result="armed";
}
void serviceMovementResume(ULONGLONG tick){
    auto& r=movementResume;if(!r.policy.enabled()||!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    const bool paused=movementMenuPaused(),ready=coopGameplayReady(outgoing)&&coopGameplayReady(remote)&&outgoing.mapId==remote.mapId;
    const bool fresh=lastReceive&&tick-lastReceive<presentationFreshMs();
    const auto before=r.policy.phase;
    const auto decision=r.policy.observe(movementResumeIdentity(),paused,ready,fresh,tick);
    if(decision==MovementResumePolicy::Reject){stopMovementBank();stopShadowStream("movement_resume_identity_or_deadline");r.result="identity_changed_or_resume_deadline";++r.refused;return;}
    if(paused){
        if(before!=MovementResumePolicy::Suspended){++r.suspended;r.token=shadowStream.token;r.peerToken=shadowStream.peerToken;}
        stopMovementBank();stopShadowStream("movement_menu_suspended");r.result="native_menu_suspended";r.nextAttempt=0;return;
    }
    if(decision==MovementResumePolicy::Continue){
        if(!r.policy.running(movementBankActive()&&shadowStreamCurrent(tick),tick)){r.result="presentation_stopped_without_pause";++r.refused;}
        return;
    }
    if(decision!=MovementResumePolicy::Restore){r.result="waiting_for_both_native_players";return;}
    if(!ready||!fresh||shadowEquipment.restoreUntil||(shadowEquipment.until&&!movementBankActive())||reviewClipActive()||authoringProof.end||shadowAction.actor){r.result="waiting_for_native_cleanup";return;}
    if(!probeFollow||phaseFollowInvalidated||!avatarAiHeld){
        if(tick<r.nextAttempt)return;r.nextAttempt=tick+500;
        if(!enableProbeFollow(true)){r.result="waiting_for_owned_follow";return;}
    }
    if(!shadowStream.until){
        if(tick<r.nextAttempt)return;r.nextAttempt=tick+500;beginShadowStream(true);r.result="new_native_steam_handshake";return;
    }
    if(!shadowStreamCurrent(tick)||!shadowStream.peerReady||!shadowStream.lastState||tick-shadowStream.lastState>=150){r.result="waiting_for_new_peer_samples";return;}
    if(shadowStream.token==r.token||shadowStream.peerToken==r.peerToken){r.result="waiting_for_new_tokens";return;}
    if(movementBankActive()){
        r.policy.restored();++r.resumed;r.result="same_world_movement_restored";return;
    }
    if(movementLoad.state.load()||tick<r.nextAttempt)return;
    r.nextAttempt=tick+750;movementNativeFeetRequested=r.nativeFeet;movementCurrentFeetRequested=r.currentFeet;
    r.result=beginMovementBank(true,r.checksum)?"loading_pinned_movement_bank":"waiting_for_native_bank_readiness";
}
void writeMovementResumeStatus(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.movement-resume.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;const auto& r=movementResume;
    fprintf(f,"{\"enabled\":%s,\"phase\":%u,\"armed\":%llu,\"suspended\":%llu,\"resumed\":%llu,\"refused\":%llu,\"checksum\":%llu,\"priorToken\":%llu,\"priorPeerToken\":%llu,\"result\":\"%s\",\"scope\":\"Explicit same-world native menu resume; load/death/session replacement remain separate\"}\n",r.policy.enabled()?"true":"false",unsigned(r.policy.phase),r.armed,r.suspended,r.resumed,r.refused,r.checksum,r.token,r.peerToken,r.result);
    const bool ok=!ferror(f);const int close=fclose(f);if(ok&&!close)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
