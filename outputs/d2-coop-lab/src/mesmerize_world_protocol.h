#pragma once
#include "arm_protocol.h"
// Source-confirmed cosmetic world lifecycle. Handles are opaque incarnation
// identifiers here: a receiver must never resolve a foreign entity handle.
#pragma pack(push,1)
struct CoopMesmerizeEye {std::uint32_t id=0;std::array<float,12> transform{};};
struct CoopMesmerizeLink {std::uint32_t id=0,target=0,kind=0;std::array<float,12> transform{},other{};};
struct CoopMesmerizeWorldPacket {
    std::uint32_t magic=0x4d573244;std::uint16_t version=4,size=sizeof(CoopMesmerizeWorldPacket);
    std::uint32_t build=0x67a50cde,role=1;
    std::uint64_t sequence=0,sourceSession=0,targetSession=0,sourceWorldEpoch=0,targetWorldEpoch=0,mapId=0;
    std::uint64_t sourceToken=0,targetToken=0,event=0,sampleTimeUs=0;
    std::uint32_t rootSequence=0,nativeHandle=0,state=0,snapshot=0;
    std::array<float,12> transform{}; // Native states: 1 spawn, 3 active, 2 retire.
    std::uint32_t eyesValid=0,eyeCount=0;std::array<CoopMesmerizeEye,5> eyes{};
    std::uint32_t linksValid=0,linkCount=0;std::array<CoopMesmerizeLink,15> links{};
};
#pragma pack(pop)
static_assert(sizeof(CoopMesmerizeWorldPacket)==2056);
inline bool mesmerizeRigidTransform(const std::array<float,12>& m){
    for(unsigned i=0;i<12;++i)if(!std::isfinite(m[i])||std::fabs(m[i])>(i<3?100000.f:1.01f))return false;
    for(unsigned i=0;i<3;++i)for(unsigned j=i;j<3;++j){float dot=0;for(unsigned k=0;k<3;++k)dot+=m[3+i*3+k]*m[3+j*3+k];if(std::fabs(dot-(i==j?1.f:0.f))>.02f)return false;}
    const float det=m[3]*(m[7]*m[11]-m[10]*m[8])-m[6]*(m[4]*m[11]-m[10]*m[5])+m[9]*(m[4]*m[8]-m[7]*m[5]);
    return det>.98f&&det<1.02f;
}
inline bool validMesmerizeWorldPacket(const CoopMesmerizeWorldPacket& p,const CoopPosePacket& local,const CoopPosePacket& peer){
    if(p.magic!=0x4d573244||p.version!=4||p.size!=sizeof(p)||p.build!=0x67a50cde||p.role!=peer.role||p.role==local.role||
       !p.sequence||!p.event||!p.sourceToken||!p.targetToken||!p.sampleTimeUs||p.sampleTimeUs>9007199254740991ull||p.snapshot>1||(p.snapshot&&p.state!=3)||
       !p.nativeHandle||p.nativeHandle==1||p.nativeHandle==0x3fff0010||(p.state!=1&&p.state!=2&&p.state!=3)||
       !coopWorldPairReady(local,peer)||p.sourceSession!=peer.session||p.targetSession!=local.session||
       p.sourceWorldEpoch!=peer.worldEpoch||p.targetWorldEpoch!=local.worldEpoch||p.mapId!=local.mapId||!armRootFresh(p.rootSequence,peer.sequence))return false;
    if(!mesmerizeRigidTransform(p.transform)||p.eyesValid>1||p.eyeCount>5||(!p.eyesValid&&p.eyeCount))return false;
    for(unsigned i=0;i<5;++i){const auto& e=p.eyes[i];
        if(i>=p.eyeCount){if(e.id)return false;for(float f:e.transform)if(f!=0)return false;continue;}
        if(!e.id||e.id==~0u||!mesmerizeRigidTransform(e.transform))return false;
        for(unsigned j=0;j<i;++j)if(p.eyes[j].id==e.id)return false;
        float d=0;for(unsigned k=0;k<3;++k){const float v=e.transform[k]-p.transform[k];d+=v*v;}if(d>100)return false;
    }
    if(p.linksValid>1||p.linkCount>15||(!p.linksValid&&p.linkCount))return false;
    std::array<unsigned,5> targets{};unsigned targetCount=0;
    for(unsigned i=0;i<15;++i){const auto& e=p.links[i];
        if(i>=p.linkCount){if(e.id||e.target||e.kind)return false;for(float f:e.transform)if(f!=0)return false;for(float f:e.other)if(f!=0)return false;continue;}
        if(!e.id||e.id==~0u||!e.target||e.target==1||e.target==0x3fff0010||e.target==~0u||e.target==p.nativeHandle||e.kind<1||e.kind>3||!mesmerizeRigidTransform(e.transform))return false;
        for(unsigned j=0;j<i;++j)if(p.links[j].id==e.id||(p.links[j].target==e.target&&p.links[j].kind==e.kind))return false;
        for(unsigned j=0;j<p.eyeCount;++j)if(p.eyes[j].id==e.id)return false;
        bool found=false;for(unsigned j=0;j<targetCount;++j)if(targets[j]==e.target)found=true;
        if(!found){if(targetCount>=targets.size())return false;targets[targetCount++]=e.target;}
        float d=0;for(unsigned k=0;k<3;++k){float v=e.transform[k]-p.transform[k];d+=v*v;}if(d>10000)return false;
        if(e.kind<3){if(!mesmerizeRigidTransform(e.other))return false;d=0;for(unsigned k=0;k<3;++k){float v=e.other[k]-p.transform[k];d+=v*v;}if(d>100)return false;}
        else for(float f:e.other)if(f!=0)return false;
    }
    return true;
}
struct MesmerizeWorldOrder {
    std::uint64_t sequence=0,event=0,time=0;unsigned handle=0,state=0;
    bool snapshot=false;
    std::array<float,12> anchor{};
    bool accept(const CoopMesmerizeWorldPacket& p){
        if((p.state!=1&&p.state!=2&&p.state!=3)||p.snapshot>1||(p.snapshot&&p.state!=3))return false;
        if(p.sequence<=sequence||p.sampleTimeUs<=time||p.event<event)return false;
        if(p.event!=event){
            // Only a fresh, separately token-scoped stream may begin with a
            // confirmed active snapshot. It is not a new native cast. Once
            // this order has an event, snapshot packets cannot replace it.
            const bool initialSnapshot=!event&&!state&&p.state==3&&p.snapshot==1;
            if((!initialSnapshot&&p.state!=1)||(p.snapshot&&!initialSnapshot)||
               p.nativeHandle==handle||(state&&state!=2&&p.sampleTimeUs-time<=500000))return false;
            anchor=p.transform;
            snapshot=initialSnapshot;
        }else{
            if(p.nativeHandle!=handle||!state||(state==2&&p.state!=2)||(state==3&&p.state==1))return false;
            if(p.state==3&&bool(p.snapshot)!=snapshot)return false;
            // Observed committed object is static. Keep the original anchor,
            // so many small hostile deltas cannot accumulate into movement.
            for(unsigned i=0;i<12;++i)if(std::fabs(p.transform[i]-anchor[i])>.02f)return false;
        }
        sequence=p.sequence;event=p.event;time=p.sampleTimeUs;handle=p.nativeHandle;state=p.state;return true;
    }
};
