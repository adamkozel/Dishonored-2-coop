#pragma once
// 1153 paired autosaves (mission checkpoint mode). Mission autosaves are
// per-player script events (arkKisculeNodeActionAutosave), so a peer that
// crosses a trigger alone would hold a later checkpoint than the other. Each
// peer observes its own native autosaves; a save it did not request is
// "originated" and the other peer mirrors it with the game's own RequestSave
// (slotType 3, next rotating autosave slot). Counters are monotonic per world
// epoch and exchanged periodically, so a lost packet only delays mirroring.
// A shared restart is permitted only while both peers are settled.
#include "autosave_pair_protocol.h"
struct AutosavePair {
    bool active=false,pending=false,peerSeen=false,peerPending=false;
    std::uint64_t arm=0,epoch=0,peerEpoch=0,peerArm=0,peerSession=0,peerRearms=0,sentAt=0,receivedAt=0,requestedAt=0,observedAt=0;
    int slot=-1;std::uint64_t stamp=0;
    unsigned saves=0,originated=0,mirrored=0,peerSaves=0,peerOriginated=0,peerMirrored=0;
    std::uint64_t requests=0,refused=0,sent=0,received=0,rejected=0,resets=0;
    const char* result="idle";
};
AutosavePair autosavePair{};
std::uint64_t autosavePairArmSequence=0;
bool missionAutosaveStamp(int& slot,std::uint64_t& stamp);
void resetAutosavePair(const char* why){
    auto& a=autosavePair;const auto arm=a.arm;const auto peerArm=a.peerArm;const auto peerSession=a.peerSession,peerRearms=a.peerRearms;
    const auto keepRequests=a.requests,keepRefused=a.refused,keepSent=a.sent,keepReceived=a.received,keepRejected=a.rejected,keepResets=a.resets+1;
    a={};a.arm=arm;a.peerArm=peerArm;a.peerSession=peerSession;a.peerRearms=peerRearms;a.requests=keepRequests;a.refused=keepRefused;a.sent=keepSent;a.received=keepReceived;a.rejected=keepRejected;a.resets=keepResets;
    a.active=true;a.epoch=outgoing.worldEpoch;a.result=why;
    missionAutosaveStamp(a.slot,a.stamp); // baseline: the save present now is the shared starting point
}
void armAutosavePair(){
    autosavePair={};autosavePair.arm=++autosavePairArmSequence;resetAutosavePair("armed");autosavePair.resets=0;
}
void stopAutosavePair(const char* why){autosavePair.active=false;autosavePair.pending=false;autosavePair.result=why;}
bool autosavePairReady(){
    const auto& a=autosavePair;
    return a.active&&a.peerSeen&&a.peerEpoch==remote.worldEpoch&&a.epoch==outgoing.worldEpoch&&
        autosavePairSettled(a.saves,a.originated,a.mirrored,a.pending,a.peerSaves,a.peerOriginated,a.peerMirrored,a.peerPending);
}
void receiveAutosavePair(const AutosavePairPacket& p){
    auto& a=autosavePair;++a.received;
    if(!a.active||!validAutosavePair(p,localRole)||p.session!=remote.session||p.mapId!=outgoing.mapId){++a.rejected;return;}
    // 1214bo: a peer that re-armed (checkpoint re-arm on a new map or after a
    // failure) restarts its counts; counting on from the old baseline could
    // never settle (take campaign-eotw-vm-v1302o: the client rejected every
    // host packet after the host's re-arm, so the shared restart was refused).
    // Both sides now count from the autosave each holds.
    if(a.peerArm&&p.session==a.peerSession&&p.arm<a.peerArm){++a.rejected;return;}
    if(a.peerArm&&(p.arm!=a.peerArm||p.session!=a.peerSession)){resetAutosavePair("peer_rearmed");++a.peerRearms;}
    a.peerSession=p.session;
    if(p.epoch!=remote.worldEpoch){++a.rejected;return;} // stale world: ignore until the peer reports its current epoch
    if(a.peerSeen&&a.peerEpoch==p.epoch&&(p.saves<a.peerSaves||p.originated<a.peerOriginated||p.mirrored<a.peerMirrored)){++a.rejected;return;}
    a.peerArm=p.arm;a.peerEpoch=p.epoch;a.peerSeen=true;a.peerSaves=p.saves;a.peerOriginated=p.originated;a.peerMirrored=p.mirrored;a.peerPending=p.pending!=0;
    a.receivedAt=GetTickCount64();
}
bool requestNativeAutosave(){
    // Exact RequestSave prologue (0x14065cc50) before calling it.
    const unsigned char prologue[]={0x48,0x8b,0xc4,0x44,0x89,0x40,0x18,0x55,0x56,0x57,0x41,0x54,0x41,0x55,0x41,0x56,0x41,0x57,0x48,0x83,0xec,0x70};
    std::uintptr_t game=0;const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(std::memcmp(gameImage+0x65cc50,prologue,sizeof(prologue))||!memoryRead(base+0x39a5290,&game,8)||!game||game!=observedGame)return false;
    using RequestSave=int(*)(void*,int,int,bool,bool,int,int);
    const int request=reinterpret_cast<RequestSave>(gameImage+0x65cc50)(reinterpret_cast<void*>(game+0x2e75d8),3,-1,false,true,0,0);
    return request>=0;
}
void serviceAutosavePair(std::uint64_t tick,bool missionCheckpoint){
    auto& a=autosavePair;
    if(!missionCheckpoint){if(a.active)stopAutosavePair("checkpoint_scope_inactive");return;}
    if(!a.active)return;
    if(GetCurrentThreadId()!=engineThread.load())return;
    if(outgoing.worldEpoch!=a.epoch){resetAutosavePair("world_epoch_renewed");return;}
    const bool gameplay=outgoing.gameLogicState==1&&!outgoing.pendingGameLogicState;
    if(gameplay&&tick-a.observedAt>=250){
        a.observedAt=tick;int slot=-1;std::uint64_t stamp=0;
        if(missionAutosaveStamp(slot,stamp)&&(slot!=a.slot||stamp!=a.stamp)) {
            a.slot=slot;a.stamp=stamp;++a.saves;
            if(a.pending){a.pending=false;++a.mirrored;a.result="mirrored_save_landed";}
            else{++a.originated;a.result="local_save_originated";}
        }
    }
    // Mirror one peer-originated save at a time, in ordinary gameplay only.
    if(gameplay&&!a.pending&&a.peerSeen&&a.peerEpoch==remote.worldEpoch&&a.mirrored<a.peerOriginated&&tick-a.requestedAt>=2000) {
        a.requestedAt=tick;
        if(requestNativeAutosave()){a.pending=true;++a.requests;a.result="mirror_requested";}
        else{++a.refused;a.result="mirror_request_refused";}
    }
    if(a.pending&&tick-a.requestedAt>20000){a.pending=false;a.result="mirror_timeout_retry";} // native save never landed: ask again
    if(sockets&&connection&&connectionState==k_ESteamNetworkingConnectionState_Connected&&tick-a.sentAt>=250) {
        AutosavePairPacket p{};p.role=localRole;p.session=outgoing.session;p.arm=a.arm;p.epoch=outgoing.worldEpoch;p.mapId=outgoing.mapId;
        p.saves=a.saves;p.originated=a.originated;p.mirrored=a.mirrored;p.pending=a.pending?1u:0u;
        if(coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_UnreliableNoNagle,nullptr)==k_EResultOK){++a.sent;a.sentAt=tick;}
    }
}
