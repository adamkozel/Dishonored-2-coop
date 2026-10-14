#pragma once
#include "arm_protocol.h"
#pragma pack(push,1)
// Cosmetic basic Corvo Windblast only. No entity handles, pointers, resource
// names, damage, impulses, mana, spawn or screen-effect requests on the wire.
struct CoopWindblastFxPacket {
    std::uint32_t magic=0x57463244;std::uint16_t version=1,size=sizeof(CoopWindblastFxPacket);
    std::uint32_t build=0x67a50cde,role=0;
    std::uint64_t sequence=0,sourceSession=0,targetSession=0,sourceWorldEpoch=0,targetWorldEpoch=0,mapId=0;
    std::uint64_t sourceToken=0,targetToken=0,event=0,sampleTimeUs=0;
    std::uint32_t rootSequence=0,nativeFxId=0;std::uint64_t powerEvent=0;
    std::uint32_t castTimeMs=0;std::array<float,3> position{};std::array<float,9> axis{};
};
#pragma pack(pop)
static_assert(sizeof(CoopWindblastFxPacket)==164);
inline bool validWindblastFxPacket(const CoopWindblastFxPacket& p,const CoopPosePacket& local,const CoopPosePacket& peer){
    if(p.magic!=0x57463244||p.version!=1||p.size!=sizeof(p)||p.build!=0x67a50cde||p.role||peer.role||local.role!=1||
       !p.sequence||!p.event||!p.nativeFxId||!p.powerEvent||!p.sourceToken||!p.targetToken||!p.sampleTimeUs||p.sampleTimeUs>9007199254740991ull||p.castTimeMs>1000||
       !coopWorldPairReady(local,peer)||p.sourceSession!=peer.session||p.targetSession!=local.session||p.sourceWorldEpoch!=peer.worldEpoch||
       p.targetWorldEpoch!=local.worldEpoch||p.mapId!=local.mapId||!armRootFresh(p.rootSequence,peer.sequence))return false;
    float distance=0;for(unsigned k=0;k<3;++k){if(!std::isfinite(p.position[k])||std::fabs(p.position[k])>100000)return false;const float d=p.position[k]-peer.position[k];distance+=d*d;}
    if(distance>16)return false;
    for(float f:p.axis)if(!std::isfinite(f)||std::fabs(f)>1.001f)return false;
    for(unsigned a=0;a<3;++a)for(unsigned b=a;b<3;++b){float dot=0;for(unsigned k=0;k<3;++k)dot+=p.axis[a*3+k]*p.axis[b*3+k];if(std::fabs(dot-(a==b?1.f:0.f))>.02f)return false;}
    const auto& m=p.axis;const float determinant=m[0]*(m[4]*m[8]-m[5]*m[7])-m[3]*(m[1]*m[8]-m[2]*m[7])+m[6]*(m[1]*m[5]-m[2]*m[4]);
    return determinant>.98f&&determinant<1.02f;
}
struct WindblastFxOrder {
    std::uint64_t sequence=0,event=0,time=0,powerEvent=0;unsigned nativeFxId=0;
    bool accept(const CoopWindblastFxPacket& p){
        if(p.sequence<=sequence||p.event<=event||p.sampleTimeUs<=time||p.powerEvent<=powerEvent||p.nativeFxId<=nativeFxId)return false;
        sequence=p.sequence;event=p.event;time=p.sampleTimeUs;powerEvent=p.powerEvent;nativeFxId=p.nativeFxId;return true;
    }
};
