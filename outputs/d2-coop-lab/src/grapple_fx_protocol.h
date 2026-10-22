#pragma once
#include "arm_protocol.h"
#include <array>
#pragma pack(push,1)
struct CoopGrappleFxPacket {
    std::uint32_t magic=0x47503244;std::uint16_t version=1,size=sizeof(CoopGrappleFxPacket);
    std::uint32_t build=0x67a50cde,role=1;
    std::uint64_t sequence=0,sourceSession=0,targetSession=0,sourceWorldEpoch=0,targetWorldEpoch=0,mapId=0;
    std::uint64_t sourceToken=0,targetToken=0,event=0,sampleTimeUs=0;
    std::uint32_t rootSequence=0,phase=0; // 1 extending, 2 native pull, 3 native end.
    float progress=0;std::array<float,3> goal{},impact{},normal{};
};
#pragma pack(pop)
static_assert(sizeof(CoopGrappleFxPacket)==144);
inline bool validGrappleFxPacket(const CoopGrappleFxPacket& p,const CoopPosePacket& local,const CoopPosePacket& peer){
    if(p.magic!=0x47503244||p.version!=1||p.size!=sizeof(p)||p.build!=0x67a50cde||p.role!=peer.role||p.role==local.role||
       !p.sequence||!p.event||!p.sourceToken||!p.targetToken||!p.sampleTimeUs||p.sampleTimeUs>9007199254740991ull||p.phase<1||p.phase>3||
       !std::isfinite(p.progress)||p.progress<0||p.progress>1||!coopWorldPairReady(local,peer)||p.sourceSession!=peer.session||
       p.targetSession!=local.session||p.sourceWorldEpoch!=peer.worldEpoch||p.targetWorldEpoch!=local.worldEpoch||p.mapId!=local.mapId||!armRootFresh(p.rootSequence,peer.sequence))return false;
    float normal=0,distance=0;for(unsigned k=0;k<3;++k){for(float v:{p.goal[k],p.impact[k],p.normal[k]})if(!std::isfinite(v)||std::fabs(v)>100000)return false;
        normal+=p.normal[k]*p.normal[k];const float d=p.goal[k]-peer.position[k];distance+=d*d;}
    return distance<=900&&normal>.8f&&normal<1.2f;
}
struct GrappleFxTimeline {
    struct Sample {CoopGrappleFxPacket p{};double time=0;};
    std::array<Sample,32> values{};unsigned count=0;
    bool push(const CoopGrappleFxPacket& p,double time){
        if(!std::isfinite(time))return false;
        if(count){const auto& last=values[count-1];
            if(p.sequence<=last.p.sequence||p.sampleTimeUs<=last.p.sampleTimeUs||time<=last.time||p.event<last.p.event)return false;
            if(p.event==last.p.event){if(p.phase<last.p.phase||last.p.phase==3)return false;
                float change=0;for(unsigned k=0;k<3;++k){const float d=p.goal[k]-last.p.goal[k];change+=d*d;}if(change>.0025f)return false;
            }else if(p.phase!=1||last.p.phase!=3)return false;
        }else if(p.phase!=1)return false;
        if(count==values.size()){for(unsigned i=1;i<count;++i)values[i-1]=values[i];--count;}values[count++]={p,time};return true;
    }
    bool sample(double now,CoopGrappleFxPacket& out)const{
        if(!count||now<values[0].time||now-values[count-1].time>180)return false;
        unsigned a=0;while(a+1<count&&values[a+1].time<=now)++a;out=values[a].p;
        if(a+1<count&&values[a+1].p.event==out.event&&values[a+1].p.phase==out.phase&&out.phase!=3){
            const auto& b=values[a+1];const float mix=float((now-values[a].time)/(b.time-values[a].time));
            out.progress+=(b.p.progress-out.progress)*mix;
        }return true;
    }
};
