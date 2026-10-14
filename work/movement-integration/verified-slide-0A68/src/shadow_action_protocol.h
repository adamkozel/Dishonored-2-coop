#pragma once
#include "arm_protocol.h"
#pragma pack(push,1)
// Native semantic states only. No pointers, resource names or damage requests.
struct CoopShadowActionPacket {
    std::uint32_t magic=0x48413244;
    std::uint16_t version=5,size=sizeof(CoopShadowActionPacket);
    std::uint32_t build=0x67a50cde,role=0;
    std::uint64_t sequence=0,sourceSession=0,targetSession=0;
    std::uint64_t sourceWorldEpoch=0,targetWorldEpoch=0,mapId=0;
    std::uint64_t sourceToken=0,targetToken=0,event=0;
    std::uint32_t rootSequence=0,character=0,kind=0,state=0; // 0 ground, 1 slash, 2 block, 3 airborne, 4 sprint, 5 lean, 6 slide
    float leanSide=0,leanUp=0;
};
#pragma pack(pop)
static_assert(sizeof(CoopShadowActionPacket)==112);
inline bool validShadowAction(const CoopShadowActionPacket& p,const CoopPosePacket& local,const CoopPosePacket& peer){
    return p.magic==0x48413244&&p.version==5&&p.size==sizeof(p)&&p.build==0x67a50cde&&p.role<=1&&p.role==peer.role&&
        p.sequence&&p.sourceToken&&p.character==p.role+1&&p.kind<=1&&p.state<=6&&
        std::isfinite(p.leanSide)&&std::isfinite(p.leanUp)&&std::fabs(p.leanSide)<=1.f&&p.leanUp>=0&&p.leanUp<=1.f&&
        (p.state==5||(!p.leanSide&&!p.leanUp))&&
        (p.kind==0?(p.state==0&&!p.event):(p.targetToken&&p.event))&&coopWorldPairReady(local,peer)&&
        p.sourceSession==peer.session&&p.targetSession==local.session&&p.sourceWorldEpoch==peer.worldEpoch&&
        p.targetWorldEpoch==local.worldEpoch&&p.mapId==local.mapId&&armRootFresh(p.rootSequence,peer.sequence);
}
inline bool shadowActionTokensMatch(const CoopShadowActionPacket& p,std::uint64_t token,std::uint64_t peerToken){
    return token&&peerToken&&p.sourceToken==peerToken&&p.targetToken==token;
}
inline bool shadowActionEventFresh(const CoopShadowActionPacket& p,std::uint64_t sequence,std::uint64_t event,unsigned state){
    return p.sequence>sequence&&p.event>=event&&(p.event!=event||p.state==state);
}
