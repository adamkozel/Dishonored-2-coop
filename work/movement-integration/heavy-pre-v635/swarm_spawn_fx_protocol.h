#pragma once
#include "arm_protocol.h"
#include "swarm_world_protocol.h"
#pragma pack(push,1)
// Cosmetic summon burst only. The source world handle is opaque; the receiver
// never resolves it or invokes the native power, swarm, damage or AI machinery.
struct CoopSwarmSpawnFxPacket {
    std::uint32_t magic=0x53463244;std::uint16_t version=1,size=sizeof(CoopSwarmSpawnFxPacket);
    std::uint32_t build=0x67a50cde,role=0;
    std::uint64_t sequence=0,sourceSession=0,targetSession=0,sourceWorldEpoch=0,targetWorldEpoch=0,mapId=0;
    std::uint64_t sourceToken=0,targetToken=0,event=0,sampleTimeUs=0;
    std::uint32_t rootSequence=0,nativeFxId=0;std::uint64_t powerEvent=0,worldEvent=0;
    std::uint32_t worldHandle=0,castTimeMs=0;std::array<float,3> position{};std::array<float,9> axis{};
};
#pragma pack(pop)
static_assert(sizeof(CoopSwarmSpawnFxPacket)==176);
inline bool validSwarmSpawnFxPacket(const CoopSwarmSpawnFxPacket& p,const CoopPosePacket& local,const CoopPosePacket& peer){
    if(p.magic!=0x53463244||p.version!=1||p.size!=sizeof(p)||p.build!=0x67a50cde||p.role||peer.role||local.role!=1||
       !p.sequence||!p.event||!p.nativeFxId||p.nativeFxId==~0u||!p.powerEvent||!p.worldEvent||!p.sourceToken||!p.targetToken||
       !p.worldHandle||p.worldHandle==1||p.worldHandle==0x3fff0010||p.worldHandle==~0u||!p.sampleTimeUs||p.sampleTimeUs>9007199254740991ull||p.castTimeMs>1000||
       !coopWorldPairReady(local,peer)||p.sourceSession!=peer.session||p.targetSession!=local.session||p.sourceWorldEpoch!=peer.worldEpoch||
       p.targetWorldEpoch!=local.worldEpoch||p.mapId!=local.mapId||!armRootFresh(p.rootSequence,peer.sequence))return false;
    std::array<float,12> transform{};float distance=0;
    for(unsigned k=0;k<3;++k){transform[k]=p.position[k];const float d=p.position[k]-peer.position[k];distance+=d*d;}
    std::copy(p.axis.begin(),p.axis.end(),transform.begin()+3);
    return distance<=900&&mesmerizeRigidTransform(transform);
}
struct SwarmSpawnFxOrder {
    std::uint64_t sequence=0,event=0,time=0,powerEvent=0,worldEvent=0;unsigned nativeFxId=0;
    bool accept(const CoopSwarmSpawnFxPacket& p){
        if(p.sequence<=sequence||p.event<=event||p.sampleTimeUs<=time||p.powerEvent<=powerEvent||p.worldEvent<=worldEvent||p.nativeFxId<=nativeFxId)return false;
        sequence=p.sequence;event=p.event;time=p.sampleTimeUs;powerEvent=p.powerEvent;worldEvent=p.worldEvent;nativeFxId=p.nativeFxId;return true;
    }
};
// Baseline a stream already in progress. A late join must never replay the
// persistent native FX record. Only a subsequently observed world birth opens
// a short discovery window; starting/failed transmission consumes the event.
struct SwarmSpawnDiscovery {
    bool baseline=false,consumed=true;unsigned handle=0,attempts=0;double born=0;
    void observe(unsigned current,double now){
        if(!baseline){baseline=true;handle=current;consumed=true;return;}
        if(current!=handle){handle=current;born=now;attempts=0;consumed=!current;}
    }
    bool mayRead(double now,bool cast){return baseline&&handle&&!consumed&&cast&&std::isfinite(now)&&now>=born&&now-born<=750&&attempts<8;}
};
