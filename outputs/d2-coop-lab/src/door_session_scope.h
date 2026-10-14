#pragma once
#include "world_protocol.h"
#include "movement_resume_policy.h"
// Session intent retains IDs only. Native door bindings never survive a load.
struct DoorSessionScope {
    std::uint64_t session=0,peerSession=0,epoch=0,peerEpoch=0,map=0;
};
inline DoorSessionScope doorSessionScope(const CoopPosePacket& local,const CoopPosePacket& peer){
    return {local.session,peer.session,local.worldEpoch,peer.worldEpoch,local.mapId};
}
inline bool doorSessionPairMatches(const DoorSessionScope& s,const CoopPosePacket& local,const CoopPosePacket& peer){
    return s.session&&s.peerSession&&s.map&&s.session==local.session&&s.peerSession==peer.session&&
        s.map==local.mapId&&s.map==peer.mapId&&local.role<=1&&peer.role<=1&&local.role!=peer.role;
}
inline bool doorSessionWorldMatches(const DoorSessionScope& s,const CoopPosePacket& local,const CoopPosePacket& peer){
    return doorSessionPairMatches(s,local,peer)&&s.epoch&&s.peerEpoch&&s.epoch==local.worldEpoch&&s.peerEpoch==peer.worldEpoch;
}
inline bool doorSessionRenewalReady(const DoorSessionScope& s,const CoopPosePacket& local,const CoopPosePacket& peer){
    return s.epoch&&s.peerEpoch&&doorSessionPairMatches(s,local,peer)&&coopWorldPairReady(local,peer)&&
        s.epoch!=local.worldEpoch&&s.peerEpoch!=peer.worldEpoch;
}
// Retaining a binding during a known native menu does not authorize mutation.
// Death, cutscene, load and unknown states must use their own lifecycle.
// 1123e: native UI overlays that pause input without changing the world:
// menus (2/4), the item wheel (arkGameLogicStatePowerwheel, 8) and tutorial
// modals (12, "Play Your Way"). They suspend sessions instead of ending them.
inline bool doorSessionUiState(unsigned state){return nativeMovementMenuState(state)||state==8||state==12;}
inline bool doorSessionMenuContext(const CoopPosePacket& p){
    const auto allowed=[](unsigned state){return state==1||doorSessionUiState(state);};
    // Opening an overlay samples as logic 1 with the overlay pending; native
    // cinematic metadata is unavailable there (1074 wheel finding).
    const bool opening=p.gameLogicState==1&&p.pendingGameLogicState&&doorSessionUiState(p.pendingGameLogicState);
    return (p.flags&1)&&allowed(p.gameLogicState)&&(!p.pendingGameLogicState||allowed(p.pendingGameLogicState))&&
        !p.cinematicNodeCount&&(p.gameLogicState!=1||p.cinematicMetadataValid==1||opening);
}
inline bool doorSessionCanRetainMenu(const DoorSessionScope& s,const CoopPosePacket& local,const CoopPosePacket& peer){
    return doorSessionWorldMatches(s,local,peer)&&doorSessionMenuContext(local)&&doorSessionMenuContext(peer);
}
// 1166b: a transport stall (no partner pose for 1-10 s, e.g. a streaming hitch
// after a long move) suspends sessions like a native menu instead of ending
// them; longer silence still ends the session.
constexpr std::uint64_t kSessionStallLimitMs=10000;
inline std::uint64_t doorResumeDeadline(std::uint64_t deadline,std::uint64_t pausedMs){
    if(!deadline)return 0;
    return pausedMs>UINT64_MAX-deadline?UINT64_MAX:deadline+pausedMs;
}
