#pragma once
#include "arm_protocol.h"
#pragma pack(push,1)
struct CoopPowerFxPacket {
    std::uint32_t magic=0x46503244;std::uint16_t version=1,size=sizeof(CoopPowerFxPacket);
    std::uint32_t build=0x67a50cde,role=0;
    std::uint64_t sequence=0,sourceSession=0,targetSession=0,sourceWorldEpoch=0,targetWorldEpoch=0,mapId=0;
    std::uint64_t sourceToken=0,targetToken=0,event=0,sampleTimeUs=0;
    std::uint32_t rootSequence=0,kind=0;std::array<float,3> position{}; // 1 Blink departure, 2 confirmed arrival.
};
#pragma pack(pop)
static_assert(sizeof(CoopPowerFxPacket)==116);
inline bool validPowerFxPacket(const CoopPowerFxPacket& p,const CoopPosePacket& local,const CoopPosePacket& peer){
    if(p.magic!=0x46503244||p.version!=1||p.size!=sizeof(p)||p.build!=0x67a50cde||p.role!=peer.role||p.role==local.role||
       !p.sequence||!p.event||!p.sourceToken||!p.targetToken||!p.sampleTimeUs||p.sampleTimeUs>9007199254740991ull||p.kind<1||p.kind>2||
       !coopWorldPairReady(local,peer)||p.sourceSession!=peer.session||p.targetSession!=local.session||p.sourceWorldEpoch!=peer.worldEpoch||
       p.targetWorldEpoch!=local.worldEpoch||p.mapId!=local.mapId||!armRootFresh(p.rootSequence,peer.sequence))return false;
    for(float x:p.position)if(!std::isfinite(x)||std::fabs(x)>100000)return false;return true;
}
struct PowerFxOrder {
    std::uint64_t sequence=0,event=0,time=0;unsigned phase=0;std::array<float,3> start{};
    bool accept(const CoopPowerFxPacket& p){
        if(p.sequence<=sequence||p.sampleTimeUs<=time||p.event<event)return false;
        if(p.kind==1){if(p.event==event||(phase==1&&p.sampleTimeUs-time<2000000))return false;start=p.position;}
        else if(p.kind==2){if(p.event!=event||phase!=1||p.sampleTimeUs-time>2000000)return false;
            float distance=0;for(unsigned k=0;k<3;++k){const float d=p.position[k]-start[k];distance+=d*d;}if(distance>625.f)return false;
        }else return false;
        sequence=p.sequence;event=p.event;time=p.sampleTimeUs;phase=p.kind;return true;
    }
};
