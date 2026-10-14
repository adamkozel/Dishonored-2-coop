#pragma once
#include "mesmerize_world_protocol.h"
#include "pose_interpolation.h"
#include "doppelganger_rig.h"
// The source's clone/equipment handles are opaque keys, never receiver handles.
// This presentation channel cannot authorize AI, interactions or damage.
#pragma pack(push,1)
struct CoopDoppelgangerWorldPacket {
    std::uint32_t magic=0x43443244;std::uint16_t version=4,size=10712;
    std::uint32_t build=0x67a50cde,role=1;
    std::uint64_t sequence=0,sourceSession=0,targetSession=0,sourceWorldEpoch=0,targetWorldEpoch=0,mapId=0;
    std::uint64_t sourceToken=0,targetToken=0,event=0,sampleTimeUs=0;
    std::uint32_t rootSequence=0,nativeHandle=0,present=0,generation=0;
    std::array<float,3> sourceOrigin{};
    std::array<float,12> transform{};
    std::uint32_t equipmentHandle=0;
    std::uint64_t rig=0;
    std::array<std::array<float,12>,kDoppelgangerJoints> local{};
    std::uint32_t fxKnown=0,fxScene=0,fxVariant=0,fxTimeMs=0;
};
#pragma pack(pop)
static_assert(sizeof(CoopDoppelgangerWorldPacket)==10712);
inline unsigned doppelgangerFxDuration(unsigned variant){return variant==1?1000:variant==2?466:0;}
inline std::uint64_t doppelgangerWorldDigest(const CoopDoppelgangerWorldPacket& p){
    std::uint64_t h=14695981039346656037ull;const auto* bytes=reinterpret_cast<const unsigned char*>(&p);
    for(unsigned i=0;i<sizeof(p);++i){h^=bytes[i];h*=1099511628211ull;}return h;
}
inline bool validDoppelgangerWorldPacket(const CoopDoppelgangerWorldPacket& p,const CoopPosePacket& local,const CoopPosePacket& peer,unsigned* failure=nullptr){
    if(failure)*failure=0;const auto fail=[&](unsigned line){if(failure)*failure=line;return false;};
    if(p.magic!=0x43443244||p.version!=4||p.size!=sizeof(p)||p.build!=0x67a50cde||p.role!=1||peer.role!=1||local.role!=0||
       !p.sequence||!p.event||!p.sourceToken||!p.targetToken||!p.sampleTimeUs||p.sampleTimeUs>9007199254740991ull||p.present>1||
       !coopWorldPairReady(local,peer)||p.sourceSession!=peer.session||p.targetSession!=local.session||p.sourceWorldEpoch!=peer.worldEpoch||
       p.targetWorldEpoch!=local.worldEpoch||p.mapId!=local.mapId||!armRootFresh(p.rootSequence,peer.sequence))return fail(__LINE__);
    float rootDistance=0;for(unsigned k=0;k<3;++k){const auto x=p.sourceOrigin[k];if(!std::isfinite(x)||std::fabs(x)>100000)return fail(__LINE__);const auto d=x-peer.position[k];rootDistance+=d*d;}
    if(rootDistance>100)return fail(__LINE__);
    if(p.fxKnown>1||p.fxVariant>2)return fail(__LINE__);
    if(!p.fxKnown||!p.fxVariant){if(p.fxScene||p.fxVariant||p.fxTimeMs)return fail(__LINE__);}
    else if(!p.fxScene||p.fxTimeMs>doppelgangerFxDuration(p.fxVariant))return fail(__LINE__);
    if(p.present){
        const auto handle=[](unsigned v){return v>1&&v!=0x3fff0010&&v!=~0u&&(v>>16)<16384;};
        if(!handle(p.nativeHandle)||!handle(p.equipmentHandle)||p.nativeHandle==p.equipmentHandle||!mesmerizeRigidTransform(p.transform))return fail(__LINE__);
        if(p.rig!=kDoppelgangerRig)return fail(__LINE__);for(unsigned j=0;j<kDoppelgangerJoints;++j)if(!doppelgangerJointValid(j,doppelgangerLocalMatrix(p.local[j])))return fail(__LINE__);
        float d2=0;for(unsigned k=0;k<3;++k){const auto d=p.transform[k]-p.sourceOrigin[k];d2+=d*d;}if(d2>10000)return fail(__LINE__);
    }else{
        if(p.nativeHandle||p.equipmentHandle||p.generation||p.rig||p.fxKnown)return fail(__LINE__);for(float x:p.transform)if(x!=0)return fail(__LINE__);for(const auto& j:p.local)for(float x:j)if(x!=0)return fail(__LINE__);
    }
    return true;
}
inline bool sameDoppelgangerIncarnation(const CoopDoppelgangerWorldPacket& a,const CoopDoppelgangerWorldPacket& b){
    return a.present==b.present&&a.nativeHandle==b.nativeHandle&&a.equipmentHandle==b.equipmentHandle&&a.generation==b.generation;
}
struct DoppelgangerWorldOrder {
    CoopDoppelgangerWorldPacket last{};
    bool accept(const CoopDoppelgangerWorldPacket& p){
        if(p.sequence<=last.sequence||p.sampleTimeUs<=last.sampleTimeUs||!p.event||p.event<last.event)return false;
        if(last.event&&((p.event==last.event)!=sameDoppelgangerIncarnation(p,last)))return false;
        last=p;return true;
    }
};
inline bool doppelgangerWorldPresentation(const CoopDoppelgangerWorldPacket& a,double at,const CoopDoppelgangerWorldPacket* b,double bt,double time,std::array<float,12>& out,float& alpha){
    alpha=0;out={};if(!a.present||!std::isfinite(at)||!std::isfinite(time))return false;out=a.transform;
    if(!b||!sameDoppelgangerIncarnation(a,*b)||a.event!=b->event||a.sourceSession!=b->sourceSession||a.targetSession!=b->targetSession||
       a.sourceWorldEpoch!=b->sourceWorldEpoch||a.targetWorldEpoch!=b->targetWorldEpoch||a.mapId!=b->mapId||
       a.sourceToken!=b->sourceToken||a.targetToken!=b->targetToken||b->sequence<=a.sequence||b->sampleTimeUs<=a.sampleTimeUs||
       !std::isfinite(bt)||bt<=at||bt-at>250||time<=at)return true;
    float d2=0;for(unsigned i=0;i<3;++i){const auto d=b->transform[i]-a.transform[i];d2+=d*d;}if(!std::isfinite(d2)||d2>9)return true;
    alpha=static_cast<float>((std::min)(1.,(time-at)/(bt-at)));
    for(unsigned i=0;i<3;++i)out[i]=a.transform[i]+alpha*(b->transform[i]-a.transform[i]);
    presentationRotation(a.transform.data()+3,b->transform.data()+3,alpha,out.data()+3);return true;
}
