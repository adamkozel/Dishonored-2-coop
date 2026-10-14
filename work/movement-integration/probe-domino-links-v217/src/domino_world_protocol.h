#pragma once
#include "mesmerize_world_protocol.h"
// Native selected-target mark snapshots. Foreign handles/FX IDs are opaque
// incarnation keys, never addresses or receiver-side NPC lookup arguments.
#pragma pack(push,1)
struct CoopDominoMark {std::uint32_t id=0,target=0;std::array<float,12> transform{};};
struct CoopDominoWorldPacket {
    std::uint32_t magic=0x44573244;std::uint16_t version=1,size=sizeof(CoopDominoWorldPacket);
    std::uint32_t build=0x67a50cde,role=1;
    std::uint64_t sequence=0,sourceSession=0,targetSession=0,sourceWorldEpoch=0,targetWorldEpoch=0,mapId=0;
    std::uint64_t sourceToken=0,targetToken=0,event=0,sampleTimeUs=0;
    std::uint32_t rootSequence=0,nativeHandle=0,present=0,count=0;
    std::array<float,3> sourceOrigin{};
    std::array<CoopDominoMark,4> marks{};
};
#pragma pack(pop)
static_assert(sizeof(CoopDominoWorldPacket)==348);
inline bool validDominoWorldPacket(const CoopDominoWorldPacket& p,const CoopPosePacket& local,const CoopPosePacket& peer){
    if(p.magic!=0x44573244||p.version!=1||p.size!=sizeof(p)||p.build!=0x67a50cde||p.role!=1||peer.role!=1||local.role!=0||
       !p.sequence||!p.event||!p.sourceToken||!p.targetToken||!p.sampleTimeUs||p.sampleTimeUs>9007199254740991ull||p.present>1||p.count>4||
       !coopWorldPairReady(local,peer)||p.sourceSession!=peer.session||p.targetSession!=local.session||p.sourceWorldEpoch!=peer.worldEpoch||
       p.targetWorldEpoch!=local.worldEpoch||p.mapId!=local.mapId||!armRootFresh(p.rootSequence,peer.sequence))return false;
    if(p.present){if(!p.nativeHandle||p.nativeHandle==1||p.nativeHandle==0x3fff0010||p.nativeHandle==~0u)return false;}
    else if(p.nativeHandle||p.count)return false;
    for(float f:p.sourceOrigin)if(!std::isfinite(f)||std::fabs(f)>100000)return false;
    float rootDistance=0;for(unsigned i=0;i<3;++i){const float d=p.sourceOrigin[i]-peer.position[i];rootDistance+=d*d;}if(rootDistance>100)return false;
    for(unsigned i=0;i<4;++i){const auto& m=p.marks[i];
        if(i>=p.count){if(m.id||m.target)return false;for(float f:m.transform)if(f!=0)return false;continue;}
        if(!m.id||m.id==~0u||!m.target||m.target==1||m.target==0x3fff0010||m.target==~0u||m.target==p.nativeHandle||!mesmerizeRigidTransform(m.transform))return false;
        for(unsigned j=0;j<i;++j)if(p.marks[j].id==m.id||p.marks[j].target==m.target)return false;
        float d=0;for(unsigned k=0;k<3;++k){const float v=m.transform[k]-p.sourceOrigin[k];d+=v*v;}if(d>10000)return false;
    }
    return true;
}
struct DominoWorldOrder {
    std::uint64_t sequence=0,event=0,time=0;unsigned handle=0;
    bool accept(const CoopDominoWorldPacket& p){
        if(p.sequence<=sequence||p.sampleTimeUs<=time||p.event<event||!p.event)return false;
        if(event&&((p.event==event)!=(p.nativeHandle==handle)))return false;
        sequence=p.sequence;event=p.event;time=p.sampleTimeUs;handle=p.nativeHandle;return true;
    }
};
