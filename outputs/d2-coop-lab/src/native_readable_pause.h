#pragma once
#include "coop_protocol.h"
// A readable is a local UI pause, not a new gameplay/animation permission.
// Preserve only an already visible, owned actor while this exact pair remains
// in gameplay/ReadNote. Loads, death, cutscenes, stale peers and other menus
// retain normal retirement. Recovery may keep the body until fresh playback.
inline bool nativeReadablePair(const CoopPosePacket& a,const CoopPosePacket& b,bool& reading){
    reading=false;
    if(a.role>1||b.role>1||a.role==b.role||!a.session||!b.session||!a.worldEpoch||!b.worldEpoch||!a.mapId||a.mapId!=b.mapId)return false;
    for(const auto* p:{&a,&b}){
        if(!(p->flags&1)||(p->gameLogicState!=1&&p->gameLogicState!=4)||
           (p->pendingGameLogicState&&p->pendingGameLogicState!=4)||p->cinematicNodeCount||
           (p->gameLogicState==1&&p->cinematicMetadataValid!=1))return false;
        reading=reading||p->gameLogicState==4||p->pendingGameLogicState==4;
    }
    return true;
}
inline bool retainNativeReadableBody(const CoopPosePacket& a,const CoopPosePacket& b,
    bool sameArmedIdentity,bool fresh,bool owned,bool recovering,bool& held){
    bool reading=false;
    held=sameArmedIdentity&&fresh&&owned&&nativeReadablePair(a,b,reading)&&(reading||(held&&recovering));
    return held;
}
