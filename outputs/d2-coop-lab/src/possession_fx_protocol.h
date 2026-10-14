#pragma once
#include "mesmerize_world_protocol.h"
// Cosmetic target FX snapshots only. Foreign target handles/IDs are opaque
// incarnation keys and never addresses or receiver-side NPC lookup arguments.
#pragma pack(push,1)
struct CoopPossessionFxPacket {
    std::uint32_t magic=0x50463244;std::uint16_t version=1,size=sizeof(CoopPossessionFxPacket);
    std::uint32_t build=0x67a50cde,role=0;
    std::uint64_t sequence=0,sourceSession=0,targetSession=0,sourceWorldEpoch=0,targetWorldEpoch=0,mapId=0;
    std::uint64_t sourceToken=0,targetToken=0,event=0,sampleTimeUs=0;
    std::uint32_t rootSequence=0,target=0,id=0,present=0;
    std::array<float,3> sourceOrigin{};std::array<float,12> transform{};
};
#pragma pack(pop)
static_assert(sizeof(CoopPossessionFxPacket)==172);
inline bool validPossessionFxPacket(const CoopPossessionFxPacket& p,const CoopPosePacket& local,const CoopPosePacket& peer){
    if(p.magic!=0x50463244||p.version!=1||p.size!=sizeof(p)||p.build!=0x67a50cde||p.role!=0||peer.role!=0||local.role!=1||
       !p.sequence||!p.event||!p.sourceToken||!p.targetToken||!p.sampleTimeUs||p.sampleTimeUs>9007199254740991ull||p.present>1||
       !coopWorldPairReady(local,peer)||p.sourceSession!=peer.session||p.targetSession!=local.session||p.sourceWorldEpoch!=peer.worldEpoch||
       p.targetWorldEpoch!=local.worldEpoch||p.mapId!=local.mapId||!armRootFresh(p.rootSequence,peer.sequence))return false;
    float distance=0;for(unsigned i=0;i<3;++i){const auto x=p.sourceOrigin[i];if(!std::isfinite(x)||std::fabs(x)>100000)return false;const auto d=x-peer.position[i];distance+=d*d;}if(distance>100)return false;
    if(!p.present){if(p.id||p.target)return false;for(float f:p.transform)if(f!=0)return false;return true;}
    if(!p.id||p.id==~0u||!p.target||p.target==1||p.target==0x3fff0010||p.target==~0u||!mesmerizeRigidTransform(p.transform))return false;
    distance=0;for(unsigned i=0;i<3;++i){const auto d=p.transform[i]-p.sourceOrigin[i];distance+=d*d;}return distance<=400.f;
}
struct PossessionFxOrder {
    std::uint64_t sequence=0,event=0,time=0;unsigned target=0,id=0;
    bool accept(const CoopPossessionFxPacket& p){
        if(p.sequence<=sequence||p.sampleTimeUs<=time||!p.event||p.event<event)return false;
        if(event&&((p.event==event)!=(p.target==target&&p.id==id)))return false;
        sequence=p.sequence;event=p.event;time=p.sampleTimeUs;target=p.target;id=p.id;return true;
    }
};
