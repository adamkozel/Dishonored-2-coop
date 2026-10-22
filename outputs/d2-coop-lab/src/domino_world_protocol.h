#pragma once
#include "mesmerize_world_protocol.h"
// Native selected-target mark snapshots. Foreign handles/FX IDs are opaque
// incarnation keys, never addresses or receiver-side NPC lookup arguments.
#pragma pack(push,1)
struct CoopDominoMark {std::uint32_t id=0,target=0;std::array<float,12> transform{};};
struct CoopDominoBeam {std::uint32_t id=0,from=0,to=0;std::array<float,12> start{},end{};};
struct CoopDominoWorldPacket {
    std::uint32_t magic=0x44573244;std::uint16_t version=2,size=sizeof(CoopDominoWorldPacket);
    std::uint32_t build=0x67a50cde,role=1;
    std::uint64_t sequence=0,sourceSession=0,targetSession=0,sourceWorldEpoch=0,targetWorldEpoch=0,mapId=0;
    std::uint64_t sourceToken=0,targetToken=0,event=0,sampleTimeUs=0;
    std::uint32_t rootSequence=0,nativeHandle=0,present=0,count=0;
    std::array<float,3> sourceOrigin{};
    std::array<CoopDominoMark,4> marks{};
    std::uint32_t beamCount=0;
    std::array<CoopDominoBeam,6> beams{};
};
#pragma pack(pop)
static_assert(sizeof(CoopDominoWorldPacket)==1000);
inline bool validDominoWorldPacket(const CoopDominoWorldPacket& p,const CoopPosePacket& local,const CoopPosePacket& peer){
    if(p.magic!=0x44573244||p.version!=2||p.size!=sizeof(p)||p.build!=0x67a50cde||p.role!=peer.role||p.role==local.role||
       !p.sequence||!p.event||!p.sourceToken||!p.targetToken||!p.sampleTimeUs||p.sampleTimeUs>9007199254740991ull||p.present>1||p.count>4||p.beamCount>6||
       !coopWorldPairReady(local,peer)||p.sourceSession!=peer.session||p.targetSession!=local.session||p.sourceWorldEpoch!=peer.worldEpoch||
       p.targetWorldEpoch!=local.worldEpoch||p.mapId!=local.mapId||!armRootFresh(p.rootSequence,peer.sequence))return false;
    if(p.present){if(!p.nativeHandle||p.nativeHandle==1||p.nativeHandle==0x3fff0010||p.nativeHandle==~0u)return false;}
    else if(p.nativeHandle||p.count||p.beamCount)return false;
    for(float f:p.sourceOrigin)if(!std::isfinite(f)||std::fabs(f)>100000)return false;
    float rootDistance=0;for(unsigned i=0;i<3;++i){const float d=p.sourceOrigin[i]-peer.position[i];rootDistance+=d*d;}if(rootDistance>100)return false;
    for(unsigned i=0;i<4;++i){const auto& m=p.marks[i];
        if(i>=p.count){if(m.id||m.target)return false;for(float f:m.transform)if(f!=0)return false;continue;}
        if(!m.id||m.id==~0u||!m.target||m.target==1||m.target==0x3fff0010||m.target==~0u||m.target==p.nativeHandle||!mesmerizeRigidTransform(m.transform))return false;
        for(unsigned j=0;j<i;++j)if(p.marks[j].id==m.id||p.marks[j].target==m.target)return false;
        float d=0;for(unsigned k=0;k<3;++k){const float v=m.transform[k]-p.sourceOrigin[k];d+=v*v;}if(d>10000)return false;
    }
    for(unsigned i=0;i<6;++i){const auto& b=p.beams[i];
        if(i>=p.beamCount){if(b.id||b.from||b.to)return false;for(float f:b.start)if(f!=0)return false;for(float f:b.end)if(f!=0)return false;continue;}
        if(!b.id||b.id==~0u||b.from==b.to||!mesmerizeRigidTransform(b.start)||!mesmerizeRigidTransform(b.end))return false;
        const CoopDominoMark* from=nullptr;const CoopDominoMark* to=nullptr;
        for(unsigned j=0;j<p.count;++j){const auto& m=p.marks[j];if(m.id==b.id)return false;if(m.target==b.from)from=&m;if(m.target==b.to)to=&m;}
        if(!from||!to)return false;
        for(unsigned j=0;j<i;++j){const auto& v=p.beams[j];if(v.id==b.id||(v.from==b.from&&v.to==b.to)||(v.from==b.to&&v.to==b.from))return false;}
        float startDistance=0,span=0,dot=0;std::array<float,3> direction{},endDelta{};
        for(unsigned k=0;k<3;++k){const float delta=b.start[k]-from->transform[k];startDistance+=delta*delta;direction[k]=to->transform[k]-from->transform[k];endDelta[k]=b.end[k]-from->transform[k];span+=direction[k]*direction[k];dot+=direction[k]*endDelta[k];}
        // Native opening interpolates between selected nodes. Mark offsets can
        // differ slightly; two metres bounds that observed presentation offset.
        if(startDistance>4||span>2500)return false;
        const float t=span>.0001f?(std::max)(0.f,(std::min)(1.f,dot/span)):0.f;float distance=0;
        for(unsigned k=0;k<3;++k){const float delta=endDelta[k]-t*direction[k];distance+=delta*delta;}if(distance>4)return false;
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
