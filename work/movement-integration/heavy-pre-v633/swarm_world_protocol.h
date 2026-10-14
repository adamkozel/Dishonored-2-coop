#pragma once
#include "mesmerize_world_protocol.h"
#include <cstddef>
#include <cstring>
// Source native entities are opaque incarnation keys. Never resolve these
// handles in the receiving game. This channel authorizes no damage or AI.
#pragma pack(push,1)
struct CoopSwarmAgent {
    std::uint32_t index=0,handle=0,incarnation=0,state=0;
    std::array<float,12> transform{};
    float scale=0;std::uint32_t directDriver=0;
    std::uint32_t trailId=0,trailAgeMs=0;std::array<float,12> trailTransform{};
};
struct CoopSwarmWorldPacket {
    std::uint32_t magic=0x53573244;std::uint16_t version=2,size=124;
    std::uint32_t build=0x67a50cde,role=0;
    std::uint64_t sequence=0,sourceSession=0,targetSession=0,sourceWorldEpoch=0,targetWorldEpoch=0,mapId=0;
    std::uint64_t sourceToken=0,targetToken=0,event=0,sampleTimeUs=0;
    std::uint32_t rootSequence=0,nativeHandle=0,present=0,count=0;
    std::array<float,3> sourceOrigin{};
    std::array<CoopSwarmAgent,100> agents{};
};
#pragma pack(pop)
static_assert(sizeof(CoopSwarmAgent)==128);
static_assert(offsetof(CoopSwarmWorldPacket,agents)==124&&sizeof(CoopSwarmWorldPacket)==12924);
inline std::size_t swarmWorldWireSize(unsigned count){return 124+count*sizeof(CoopSwarmAgent);}
inline std::uint64_t swarmWorldDigest(const CoopSwarmWorldPacket& p){
    std::uint64_t h=14695981039346656037ull;const auto* bytes=reinterpret_cast<const unsigned char*>(&p);
    if(p.count>100||p.size!=swarmWorldWireSize(p.count))return 0;
    for(unsigned i=0;i<p.size;++i){h^=bytes[i];h*=1099511628211ull;}return h;
}
inline bool decodeSwarmWorldPacket(const void* data,std::size_t bytes,CoopSwarmWorldPacket& out){
    if(!data||bytes<124||bytes>sizeof(out))return false;
    CoopSwarmWorldPacket p{};std::memcpy(&p,data,bytes);
    if(p.count>100||p.size!=bytes||bytes!=swarmWorldWireSize(p.count))return false;
    out=p;return true;
}
inline bool validSwarmWorldPacket(const CoopSwarmWorldPacket& p,const CoopPosePacket& local,const CoopPosePacket& peer){
    if(p.magic!=0x53573244||p.version!=2||p.count>100||p.size!=swarmWorldWireSize(p.count)||p.build!=0x67a50cde||p.role!=0||peer.role!=0||local.role!=1||
       !p.sequence||!p.event||!p.sourceToken||!p.targetToken||!p.sampleTimeUs||p.sampleTimeUs>9007199254740991ull||p.present>1||
       !coopWorldPairReady(local,peer)||p.sourceSession!=peer.session||p.targetSession!=local.session||p.sourceWorldEpoch!=peer.worldEpoch||
       p.targetWorldEpoch!=local.worldEpoch||p.mapId!=local.mapId||!armRootFresh(p.rootSequence,peer.sequence))return false;
    if(p.present){if(!p.nativeHandle||p.nativeHandle==1||p.nativeHandle==0x3fff0010||p.nativeHandle==~0u)return false;}
    else if(p.nativeHandle||p.count)return false;
    float rootDistance=0;for(unsigned k=0;k<3;++k){const float x=p.sourceOrigin[k];if(!std::isfinite(x)||std::fabs(x)>100000)return false;
        const float d=x-peer.position[k];rootDistance+=d*d;}if(rootDistance>100)return false;
    unsigned trails=0;
    for(unsigned i=0;i<p.count;++i){const auto& a=p.agents[i];
        if(a.index>=100||!a.handle||a.handle==1||a.handle==0x3fff0010||a.handle==~0u||a.handle==p.nativeHandle||a.state>6||a.directDriver>1||
           (!a.directDriver&&a.state!=3)||!std::isfinite(a.scale)||a.scale<.1f||a.scale>4.f||!mesmerizeRigidTransform(a.transform))return false;
        for(unsigned j=0;j<i;++j)if(p.agents[j].index==a.index||p.agents[j].handle==a.handle)return false;
        float distance=0;for(unsigned k=0;k<3;++k){const float d=a.transform[k]-p.sourceOrigin[k];distance+=d*d;}if(distance>10000)return false;
        if(a.trailId){
            if(++trails>7||a.trailId==~0u||a.trailAgeMs>2500||!mesmerizeRigidTransform(a.trailTransform))return false;
            float d2=0;for(unsigned k=0;k<3;++k){const auto d=a.trailTransform[k]-a.transform[k];d2+=d*d;}if(d2>4)return false;
            for(unsigned j=0;j<i;++j)if(p.agents[j].trailId==a.trailId)return false;
        }else{
            if(a.trailAgeMs)return false;for(float f:a.trailTransform)if(f!=0)return false;
        }
    }
    return true;
}
struct SwarmWorldOrder {
    std::uint64_t sequence=0,event=0,time=0;unsigned handle=0;
    bool accept(const CoopSwarmWorldPacket& p){
        if(p.sequence<=sequence||p.sampleTimeUs<=time||p.event<event||!p.event)return false;
        if(event&&((p.event==event)!=(p.nativeHandle==handle)))return false;
        sequence=p.sequence;event=p.event;time=p.sampleTimeUs;handle=p.nativeHandle;return true;
    }
};
