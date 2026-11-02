#pragma once
#include "world_protocol.h"
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>
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
// 2107 (#260): the host's breakable presence. A movable breakable that breaks while the host's world loads (Edge of the
// World's env_bottle_04_19 in progress-eotw-v2104b and v2107a, and #260's spawn diffs) is removed before the host's
// first catalog scan, so no step or vanish is ever sent and the partner's bottle stays whole. After each complete scan
// the host sends the sorted, unique name hashes (FNV-1a) of every breakable in its catalog; the partner breaks its own
// unbroken breakables that the host's world lacks.
#pragma pack(push,1)
struct CoopBreakablePresenceHeader {
    std::uint32_t magic=0x56423244; // 'D2BV'
    std::uint16_t version=1,size=sizeof(CoopBreakablePresenceHeader);
    std::uint32_t build=0x67a50cde,role=0,count=0,reserved=0;
    std::uint64_t sequence=0,sourceSession=0,targetSession=0,sourceEpoch=0,targetEpoch=0,mapId=0;
};
#pragma pack(pop)
static_assert(sizeof(CoopBreakablePresenceHeader)==72);
constexpr std::uint32_t kBreakablePresenceMax=4096;
struct CoopBreakablePresencePacket {CoopBreakablePresenceHeader header{};std::uint32_t hashes[kBreakablePresenceMax]{};};
inline std::uint32_t breakableNameHash(const std::string& s){std::uint32_t h=2166136261u;for(const unsigned char c:s){h^=c;h*=16777619u;}return h;}
// Only the host sends it, to the partner, for the partner's own world epoch and map; the hashes rise strictly.
inline bool validCoopBreakablePresence(const CoopBreakablePresencePacket& p,std::size_t bytes,const CoopPosePacket& local,const CoopPosePacket& peer){
    const auto& h=p.header;
    if(bytes<sizeof(h)||h.magic!=0x56423244||h.version!=1||h.size!=sizeof(h)||h.build!=0x67a50cde||h.role!=0||peer.role!=0||local.role!=1||
       h.reserved||h.count>kBreakablePresenceMax||bytes!=sizeof(h)+h.count*4ull||!h.sequence||!coopWorldPairReady(local,peer)||
       h.sourceSession!=peer.session||h.targetSession!=local.session||h.sourceEpoch!=peer.worldEpoch||h.targetEpoch!=local.worldEpoch||h.mapId!=local.mapId)return false;
    for(std::uint32_t i=1;i<h.count;++i)if(p.hashes[i]<=p.hashes[i-1])return false;
    return true;
}
// A partner breakable the host's world lacks, still unbroken here, and known here for 10 s before the host's list was
// received (so a breakable the partner's world has only just streamed in is not judged on an older list).
constexpr std::size_t kBreakablePresenceMaxBreaks=16;
inline bool breakablePresenceTarget(const std::vector<std::uint32_t>& host,const std::string& name,int done,int steps,
    std::uint64_t firstSeen,std::uint64_t presenceAt){
    return !name.empty()&&steps>0&&done<steps&&presenceAt>=firstSeen+10000&&!std::binary_search(host.begin(),host.end(),breakableNameHash(name));
}
