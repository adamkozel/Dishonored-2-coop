#pragma once
#include "coop_protocol.h"
#include "movement_resume_policy.h"
// Test fixture only: keep existing finite NPC pause references across native
// menu transitions. This is not gameplay readiness or permission to present.
inline bool movementFixtureMenuContext(const CoopPosePacket& p){
    const auto menu=[](unsigned s){return s==1||nativeMovementMenuState(s);};
    return (p.flags&1)&&menu(p.gameLogicState)&&
        (!p.pendingGameLogicState||menu(p.pendingGameLogicState))&&
        !p.cinematicNodeCount&&(p.gameLogicState!=1||p.cinematicMetadataValid==1);
}
inline bool movementFixtureMenuPair(const CoopPosePacket& local,const CoopPosePacket& peer){
    return local.role<=1&&peer.role<=1&&local.role!=peer.role&&local.session&&peer.session&&
        local.worldEpoch&&peer.worldEpoch&&local.mapId&&local.mapId==peer.mapId&&
        movementFixtureMenuContext(local)&&movementFixtureMenuContext(peer);
}
