#pragma once
#include "swarm_world_protocol.h"
#include "pose_interpolation.h"

// Visual interpolation of already validated world snapshots. Never creates
// native gameplay, predicts missing rats or blends different incarnations.
inline bool swarmRatPresentation(const CoopSwarmWorldPacket& a,double at,
    const CoopSwarmWorldPacket* b,double bt,double target,unsigned index,
    CoopSwarmAgent& out,float& alpha){
    alpha=0;out={};
    if(!a.present||a.count>100||index>=100||!std::isfinite(at)||!std::isfinite(target))return false;
    const CoopSwarmAgent* first=nullptr;for(unsigned i=0;i<a.count;++i)if(a.agents[i].index==index)first=&a.agents[i];
    if(!first)return false;out=*first;
    if(!b||!b->present||b->count>100||!std::isfinite(bt)||bt<=at||bt-at>250.||target<=at||
       b->sequence<=a.sequence||b->sampleTimeUs<=a.sampleTimeUs||b->nativeHandle!=a.nativeHandle||b->event!=a.event||
       b->sourceSession!=a.sourceSession||b->targetSession!=a.targetSession||b->mapId!=a.mapId||
       b->sourceWorldEpoch!=a.sourceWorldEpoch||b->targetWorldEpoch!=a.targetWorldEpoch||
       b->sourceToken!=a.sourceToken||b->targetToken!=a.targetToken)return true;
    const CoopSwarmAgent* next=nullptr;for(unsigned i=0;i<b->count;++i)if(b->agents[i].index==index)next=&b->agents[i];
    if(!next||next->handle!=first->handle||next->incarnation!=first->incarnation)return true;
    float distance=0;for(unsigned i=0;i<3;++i){const auto d=next->transform[i]-first->transform[i];distance+=d*d;}
    if(!std::isfinite(distance)||distance>9.f)return true;
    alpha=static_cast<float>((std::min)(1.,(target-at)/(bt-at)));
    for(unsigned i=0;i<3;++i)out.transform[i]=first->transform[i]+alpha*(next->transform[i]-first->transform[i]);
    presentationRotation(first->transform.data()+3,next->transform.data()+3,alpha,out.transform.data()+3);
    out.scale=first->scale+alpha*(next->scale-first->scale);
    if(first->trailId&&next->trailId==first->trailId&&next->trailAgeMs>=first->trailAgeMs){
        for(unsigned i=0;i<3;++i)out.trailTransform[i]=first->trailTransform[i]+alpha*(next->trailTransform[i]-first->trailTransform[i]);
        presentationRotation(first->trailTransform.data()+3,next->trailTransform.data()+3,alpha,out.trailTransform.data()+3);
        out.trailAgeMs=first->trailAgeMs+static_cast<unsigned>(alpha*(next->trailAgeMs-first->trailAgeMs));
    }
    return true;
}
