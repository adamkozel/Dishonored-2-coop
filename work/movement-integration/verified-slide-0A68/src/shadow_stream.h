#pragma once
#include "native_player_action.h"
#include "native_player_sword.h"
// Bounded 15-second probe or explicit continuous opt-in for this incarnation.
// Native Steam transports semantic state;
// the receiver uses native clone clips, never native damage/AI attack calls.
struct ShadowStreamState {
    struct PendingAction {const char* alias=nullptr;double due=0;};
    PendingAction pending[8]{};unsigned pendingCount=0;
    ULONGLONG until=0,lastHello=0,lastSend=0,lastPeer=0,lastState=0;
    std::uintptr_t game=0,actor=0;std::uint32_t handle=0;
    std::uint64_t token=0,peerToken=0,epoch=0,peerEpoch=0,peerSession=0,lastSequence=0,lastEvent=0,sourceEvent=0;
    unsigned sourceState=~0u,remoteState=0;
    float remoteLeanSide=0,remoteLeanUp=0;
    bool peerReady=false,ownsAction=false,persistent=false;
};
ShadowStreamState shadowStream{};
std::uint64_t shadowStreamNonce=0,shadowStreamSequence=0,shadowStreamSent=0,shadowStreamReceived=0,shadowStreamAccepted=0,
    shadowStreamRejected=0,shadowStreamRefused=0,shadowStreamStarted=0,shadowStreamApplied=0,shadowStreamReleases=0;
CoopShadowActionPacket shadowStreamLastPacket{};
const char* shadowStreamResult="idle";
bool shadowStreamPairReady(ULONGLONG tick){
    return GetCurrentThreadId()==engineThread.load()&&probeFollowExact&&armProbePairReady(tick)&&avatarAiHeld&&
        avatarAiTarget.actor==probeActor&&locomotionEnabled&&coopGameplayReady(outgoing)&&coopGameplayReady(remote);
}
bool shadowStreamCurrent(ULONGLONG tick){
    return shadowStream.until>tick&&shadowStreamPairReady(tick)&&shadowStream.game==observedGame&&
        shadowStream.actor==probeActor&&shadowStream.handle==probeHandle&&shadowStream.epoch==outgoing.worldEpoch&&
        shadowStream.peerEpoch==remote.worldEpoch&&shadowStream.peerSession==remote.session;
}
bool shadowSourceAction(unsigned& action,float* leanSide=nullptr,float* leanUp=nullptr){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);NativePlayerStates states{};NativePlayerSword sword{};
    const bool valid=readNativePlayerStates(memoryRead,base,states)&&states.player.game==observedGame&&
        states.player.player==observedPlayer&&nativePlayerShadowAction(states,base,action)&&
        readNativePlayerSword(memoryRead,base,sword)&&sword.character==localRole+1&&sword.sword.game==states.player.game&&
        sword.sword.actor==states.player.player&&sword.sword.logic==1&&sword.sword.equipped&&sword.sword.started;
    float side=0,up=0;
    if(!valid||(action==5&&!nativePlayerLeanAmount(memoryRead,states,base,side,up)))return false;
    if(leanSide)*leanSide=side;if(leanUp)*leanUp=up;return true;
}
void stopShadowStream(const char* reason){
    if(!shadowStream.until)return;
    shadowStream.until=0;shadowStream.peerReady=false;shadowStream.pendingCount=0;shadowStreamResult=reason;shadowLocoUntil=0;
    if(shadowStream.ownsAction){stopShadowAction(reason);shadowStream.ownsAction=false;++shadowStreamReleases;}
}
void beginShadowStream(bool persistent=false){
    const auto tick=GetTickCount64();unsigned action=0;
    if((bodyStreamEnabled&&!mixamoBodyCompatible())||shadowStream.until||shadowAction.actor||!shadowStreamPairReady(tick)||!shadowSourceAction(action)){
        ++shadowStreamRefused;shadowStreamResult="unavailable_or_busy";return;
    }
    NativeSwordEquipment s{};NativeHealth h{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(!readNativeSwordEquipment(memoryRead,base,probeHandle,s,localRole?NativeSwordScope::DoppelgangerCorvo:NativeSwordScope::DoppelgangerEmily)||
       s.actor!=probeActor||!s.equipped||!s.started||!readNativeHealth(memoryRead,base,probeHandle,h)||h.current!=40){
        ++shadowStreamRefused;shadowStreamResult="clone_unavailable";return;
    }
    shadowStream={};shadowStream.persistent=persistent;shadowStream.until=tick+15000;shadowStream.token=++shadowStreamNonce;shadowStream.game=observedGame;
    shadowStream.actor=probeActor;shadowStream.handle=probeHandle;shadowStream.epoch=outgoing.worldEpoch;
    shadowStream.peerEpoch=remote.worldEpoch;shadowStream.peerSession=remote.session;
    ++shadowStreamStarted;shadowStreamResult="awaiting_peer";
}
CoopShadowActionPacket shadowStreamPacket(unsigned kind){
    CoopShadowActionPacket p{};p.sequence=++shadowStreamSequence;p.role=localRole;p.character=localRole+1;p.kind=kind;
    p.sourceSession=outgoing.session;p.targetSession=remote.session;p.sourceWorldEpoch=outgoing.worldEpoch;p.targetWorldEpoch=remote.worldEpoch;
    p.mapId=outgoing.mapId;p.rootSequence=outgoing.sequence;p.sourceToken=shadowStream.token;p.targetToken=shadowStream.peerToken;return p;
}
void sendShadowStream(ULONGLONG tick){
    if(!nativeMutationAllowed||!shadowStreamCurrent(tick))return;
    if(tick-shadowStream.lastHello>=100){
        shadowStream.lastHello=tick;const auto p=shadowStreamPacket(0);
        if(validShadowAction(p,remote,outgoing)&&sockets->SendMessageToConnection(connection,&p,sizeof(p),k_nSteamNetworkingSend_UnreliableNoNagle,nullptr)==k_EResultOK)++shadowStreamSent;
    }
    if(!shadowStream.peerReady||tick-shadowStream.lastSend<33)return;
    shadowStream.lastSend=tick;unsigned action=0;float leanSide=0,leanUp=0;
    if(!shadowSourceAction(action,&leanSide,&leanUp))return; // Missing native evidence never becomes an inferred action.
    if(action!=shadowStream.sourceState){shadowStream.sourceState=action;++shadowStream.sourceEvent;}
    auto p=shadowStreamPacket(1);p.state=action;p.event=shadowStream.sourceEvent;p.leanSide=leanSide;p.leanUp=leanUp;
    if(validShadowAction(p,remote,outgoing)&&sockets->SendMessageToConnection(connection,&p,sizeof(p),k_nSteamNetworkingSend_UnreliableNoNagle,nullptr)==k_EResultOK){
        ++shadowStreamSent;shadowStreamLastPacket=p;
    }
}
void duplicateShadowStream(){
    if(shadowStreamCurrent(GetTickCount64())&&shadowStreamLastPacket.sequence&&validShadowAction(shadowStreamLastPacket,remote,outgoing))
        sockets->SendMessageToConnection(connection,&shadowStreamLastPacket,sizeof(shadowStreamLastPacket),k_nSteamNetworkingSend_ReliableNoNagle,nullptr);
}
void receiveShadowStream(const CoopShadowActionPacket& p,ULONGLONG tick){
    ++shadowStreamReceived;
    if(!nativeMutationAllowed||!shadowStreamCurrent(tick)||!validShadowAction(p,outgoing,remote)||p.sequence<=shadowStream.lastSequence||
       (shadowStream.peerToken&&p.sourceToken!=shadowStream.peerToken)||(p.targetToken&&p.targetToken!=shadowStream.token)){
        ++shadowStreamRejected;return;
    }
    if(p.kind==0){
        shadowStream.peerToken=p.sourceToken;shadowStream.lastSequence=p.sequence;shadowStream.lastPeer=tick;
        if(p.targetToken==shadowStream.token){shadowStream.peerReady=true;shadowStreamResult="ready";}return;
    }
    if(!shadowStream.peerReady||!shadowActionTokensMatch(p,shadowStream.token,shadowStream.peerToken)||
       !shadowActionEventFresh(p,shadowStream.lastSequence,shadowStream.lastEvent,shadowStream.remoteState)){
        ++shadowStreamRejected;return;
    }
    shadowStream.lastSequence=p.sequence;
    const bool changed=p.event!=shadowStream.lastEvent;
    if(changed){
        const bool ground=p.state==0||p.state==4||p.state==5;
        const bool release=ground&&shadowStream.remoteState==2;
        const bool landing=ground&&shadowStream.remoteState==3;
        if((p.state>=1&&p.state<=3)||release||landing){
            if(shadowStream.pendingCount==8){++shadowStreamRefused;stopShadowStream("presentation_queue_full");return;}
            // Match root presentation delay. Otherwise landing can play while
            // the interpolated body is still above the ground. This queue
            // contains local whitelisted clips only, never gameplay outcomes.
            auto& pending=shadowStream.pending[shadowStream.pendingCount++];
            pending.alias=p.state==1?"attack":p.state==2?"parry":p.state==3?"airborne":landing?"land":"recover";
            pending.due=presentationClock()+(poseSmoothingEnabled?PoseInterpolation::delayMs:0.);
        }
        shadowStream.lastEvent=p.event;shadowStream.remoteState=p.state;
    }
    shadowStream.remoteLeanSide=p.leanSide;shadowStream.remoteLeanUp=p.leanUp;
    shadowStream.lastState=tick;++shadowStreamAccepted;
    // The native stance uses the same exact incarnation and freshness lease.
    shadowLocoGame=probeGame;shadowLocoActor=probeActor;shadowLocoHandle=probeHandle;shadowLocoEpoch=outgoing.worldEpoch;
    shadowLocoRemoteEpoch=remote.worldEpoch;shadowLocoRemoteSession=remote.session;shadowLocoCombat=true;
    shadowLocoUntil=(std::min)(tick+250,shadowStream.until);
    if((p.state==2||p.state==3)&&shadowAction.actor&&shadowStream.ownsAction){
        shadowAction.until=(std::min)(tick+250,shadowStream.until);shadowActionDeadline.store(shadowAction.until);
    }
}
void serviceShadowStream(ULONGLONG tick){
    if(!shadowStream.until)return;
    if(!shadowStreamCurrent(tick)||(shadowStream.peerReady&&tick-shadowStream.lastPeer>350)){
        stopShadowStream(tick>=shadowStream.until?"expired":"scope_or_peer_changed");return;
    }
    if(shadowStream.lastState&&tick-shadowStream.lastState>250){stopShadowStream("native_state_stale");return;}
    // Only a current, mutually ready pair renews. No timeout, load, pause,
    // disconnect or identity change can implicitly recreate a retired stream.
    if(shadowStream.persistent&&shadowStream.peerReady)shadowStream.until=tick+15000;
    while(shadowStream.pendingCount&&presentationClock()>=shadowStream.pending[0].due){
        const auto command=shadowStream.pending[0];
        for(unsigned i=1;i<shadowStream.pendingCount;++i)shadowStream.pending[i-1]=shadowStream.pending[i];
        --shadowStream.pendingCount;stopShadowAction("remote_transition");
        if(!playShadowAction(command.alias)){++shadowStreamRefused;stopShadowStream("native_action_refused");return;}
        shadowStream.ownsAction=true;++shadowStreamApplied;
        if(!std::strcmp(command.alias,"airborne")||!std::strcmp(command.alias,"parry")){
            shadowAction.until=(std::min)(tick+250,shadowStream.until);shadowActionDeadline.store(shadowAction.until);
        }
    }
}
void writeShadowStream(ULONGLONG tick){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.shadow-stream.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\"armed\":%s,\"ready\":%s,\"remainingMs\":%llu,\"token\":%llu,\"peerToken\":%llu,\"epoch\":%llu,\"peerEpoch\":%llu,\"handle\":%u,\"sourceState\":%u,\"remoteState\":%u,\"sourceEvent\":%llu,\"remoteEvent\":%llu,\"lastSequence\":%llu,\"started\":%llu,\"sent\":%llu,\"received\":%llu,\"accepted\":%llu,\"applied\":%llu,\"rejected\":%llu,\"refused\":%llu,\"released\":%llu,\"actionActive\":%s,\"result\":\"%s\",\"leanSide\":%.6f,\"leanUp\":%.6f}\n",
        shadowStream.until>tick?"true":"false",shadowStream.peerReady?"true":"false",shadowStream.until>tick?shadowStream.until-tick:0,
        shadowStream.token,shadowStream.peerToken,shadowStream.epoch,shadowStream.peerEpoch,shadowStream.handle,shadowStream.sourceState,shadowStream.remoteState,
        shadowStream.sourceEvent,shadowStream.lastEvent,shadowStream.lastSequence,shadowStreamStarted,shadowStreamSent,shadowStreamReceived,
        shadowStreamAccepted,shadowStreamApplied,shadowStreamRejected,shadowStreamRefused,shadowStreamReleases,shadowAction.actor?"true":"false",shadowStreamResult,shadowStream.remoteLeanSide,shadowStream.remoteLeanUp);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
