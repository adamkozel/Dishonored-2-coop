#pragma once
#include "world_protocol.h"
#include <cmath>
// 1214fq (#132): one breakable's step, sent by the world where it advanced. Breaks only go forward, so either role sends
// it and the peer applies a step above its own; a step is never sent back. Map entities share handles between the two
// worlds, and the name must match too. flags bit 0: hit and direction are the native hit that broke it.
#pragma pack(push,1)
struct CoopBreakablePacket {
    std::uint32_t magic=0x4b423244; // 'D2BK'
    std::uint16_t version=1,size=sizeof(CoopBreakablePacket);
    std::uint32_t build=0x67a50cde,role=0,handle=0,step=0,steps=0,flags=0;
    std::uint64_t sequence=0,sourceSession=0,targetSession=0,sourceEpoch=0,targetEpoch=0,mapId=0;
    float hit[3]{},direction[3]{};
    char name[192]{};
};
#pragma pack(pop)
static_assert(sizeof(CoopBreakablePacket)==296);
inline bool validBreakableVector(const float (&v)[3],float limit){
    for(float x:v)if(!std::isfinite(x)||std::fabs(x)>limit)return false;
    return true;
}
inline bool validCoopBreakable(const CoopBreakablePacket& p,const CoopPosePacket& local,const CoopPosePacket& peer){
    if(p.magic!=0x4b423244||p.version!=1||p.size!=sizeof(p)||p.build!=0x67a50cde||p.role>1||p.role!=peer.role||
       !p.sequence||!p.handle||p.step<1||p.step>p.steps||p.steps>64||p.flags>1||!coopWorldPairReady(local,peer)||
       p.sourceSession!=peer.session||p.targetSession!=local.session||p.sourceEpoch!=peer.worldEpoch||
       p.targetEpoch!=local.worldEpoch||p.mapId!=local.mapId||!coopWorldName(p.name))return false;
    if(!(p.flags&1))return !p.hit[0]&&!p.hit[1]&&!p.hit[2]&&!p.direction[0]&&!p.direction[1]&&!p.direction[2];
    return validBreakableVector(p.hit,100000.f)&&validBreakableVector(p.direction,2.f);
}
// Steps the receiver still has to advance: only above its own step, never past the breakable's count.
inline int breakableStepsToApply(int local,int remote,int steps){
    return (local>=0&&remote>local&&remote<=steps&&steps<=64)?remote-local:0;
}
