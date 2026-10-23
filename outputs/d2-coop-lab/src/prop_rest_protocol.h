#pragma once
#include "world_protocol.h"
#include <cmath>
// 1214ip (#178): where a map prop came to rest after something moved it in one world (a blast, a kick, a bump), so the
// other world puts its copy there too. Props are arkMovable (bottles, glasses, bales) and arkPickup (fruit, coins, keys)
// entities; map entities share handles between the two worlds, and the name must match. Either role sends; the
// receiver places its copy through the native entity transform setter and stays quiet about that prop for a while, so
// nothing echoes. physics-props-v1770a: a Windblast threw the fruit off the table up to 18 m in the host's world only.
#pragma pack(push,1)
struct CoopPropRestPacket {
    std::uint32_t magic=0x52503244; // 'D2PR'
    std::uint16_t version=1,size=sizeof(CoopPropRestPacket);
    std::uint32_t build=0x67a50cde,role=0,handle=0,flags=0;
    std::uint64_t sequence=0,sourceSession=0,targetSession=0,sourceEpoch=0,targetEpoch=0,mapId=0;
    float position[3]{},axis[9]{};
    char name[192]{};
};
#pragma pack(pop)
static_assert(sizeof(CoopPropRestPacket)==312);
inline bool validPropRestTransform(const float (&position)[3],const float (&axis)[9]){
    for(float v:position)if(!std::isfinite(v)||std::fabs(v)>100000.f)return false;
    for(float v:axis)if(!std::isfinite(v)||std::fabs(v)>1.01f)return false;
    const float* m=axis;
    const float determinant=m[0]*(m[4]*m[8]-m[5]*m[7])-m[1]*(m[3]*m[8]-m[5]*m[6])+m[2]*(m[3]*m[7]-m[4]*m[6]);
    return determinant>.9f&&determinant<1.1f;
}
inline bool validCoopPropRest(const CoopPropRestPacket& p,const CoopPosePacket& local,const CoopPosePacket& peer){
    return p.magic==0x52503244&&p.version==1&&p.size==sizeof(p)&&p.build==0x67a50cde&&p.role<=1&&p.role==peer.role&&
        p.sequence&&p.handle&&!p.flags&&coopWorldPairReady(local,peer)&&p.sourceSession==peer.session&&p.targetSession==local.session&&
        p.sourceEpoch==peer.worldEpoch&&p.targetEpoch==local.worldEpoch&&p.mapId==local.mapId&&coopWorldName(p.name)&&
        validPropRestTransform(p.position,p.axis);
}
// The rest rule: a prop that came more than 5 cm from its last rest and has been still for 1 s is sent, unless a player
// holds it (the held object stream carries it), it is attached to another entity (a key on a guard's belt, fruit on a
// plate: its native reference frame has a parent; that world moves its own copy), or it is inside the quiet time after
// a rest applied from the other world.
inline bool propRestShouldSend(float movedFromRest,unsigned stillMs,bool held,bool attached,bool quiet){
    return movedFromRest>.05f&&stillMs>=1000&&!held&&!attached&&!quiet;
}
// 1214ir: the native reference frame (arkCpntReferenceFrame, vtable RVA 1c573e8). +0x84 names what the prop stands on or
// is bound to (-1 none); a prop bound to a joint (a key on a guard's belt) also has its local offset at +0x70/+0x78, while
// one that only stands on something (fruit on a plate, or anything a blast threw onto another body) keeps them zero
// (confirmed runtime: of 98 props on the Dunwall tower map only the guard's key had them; physics-props-v1772a: blasted
// fruit read a +0x84 parent with zero offsets). Bound is a non-zero offset.
inline bool propRestBound(std::uint64_t offsetA,std::uint64_t offsetB){return offsetA||offsetB;}
