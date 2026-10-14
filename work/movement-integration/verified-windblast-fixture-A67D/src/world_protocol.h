#pragma once
#include "coop_protocol.h"
#include <cstring>

#pragma pack(push,1)
// Observation only: no receiver may mutate an entity based on this packet.
// Native sourceHandle is an incarnation marker, never a remote lookup key.
struct CoopWorldNpcPacket {
    std::uint32_t magic=0x57433244; // D2CW
    std::uint16_t version=2,size=sizeof(CoopWorldNpcPacket);
    std::uint32_t build=0x67a50cde,role=0;
    std::uint64_t sequence=0,sourceSession=0,targetSession=0;
    std::uint64_t sourceWorldEpoch=0,targetWorldEpoch=0,mapId=0;
    std::uint32_t sourceHandle=0,reserved=0;
    char spawner[192]{},definition[192]{};
    float position[3]{},orientation[9]{};
    std::uint32_t healthValid=0;
    float health=0,maximumHealth=0;
};
#pragma pack(pop)
static_assert(sizeof(CoopWorldNpcPacket)==516);
inline bool coopWorldName(const char (&name)[192]) {
    bool ended=false;
    if(!name[0])return false;
    for(unsigned char c:name) {
        if(!c)ended=true;
        else if(ended||c<32||c>126)return false;
    }
    return ended;
}
inline bool coopWorldPairReady(const CoopPosePacket& local,const CoopPosePacket& peer) {
    return local.role!=peer.role&&local.session&&peer.session&&local.worldEpoch&&peer.worldEpoch&&
        local.mapId&&local.mapId==peer.mapId&&coopGameplayReady(local)&&coopGameplayReady(peer);
}
inline bool validCoopWorldNpc(const CoopWorldNpcPacket& p,const CoopPosePacket& local,const CoopPosePacket& peer) {
    if(p.magic!=0x57433244||p.version!=2||p.size!=sizeof(p)||p.build!=0x67a50cde||p.role||peer.role||local.role!=1||
       !p.sequence||p.reserved||!p.sourceHandle||!coopWorldPairReady(local,peer)||
       p.sourceSession!=peer.session||p.targetSession!=local.session||p.sourceWorldEpoch!=peer.worldEpoch||
       p.targetWorldEpoch!=local.worldEpoch||p.mapId!=local.mapId||!coopWorldName(p.spawner)||!coopWorldName(p.definition))return false;
    if(p.healthValid>1||!std::isfinite(p.health)||!std::isfinite(p.maximumHealth)||
       p.health<0||p.maximumHealth<0||p.health>1000000.f||p.maximumHealth>1000000.f||
       (!p.healthValid&&(p.health!=0||p.maximumHealth!=0)))return false;
    // A changed maximum can temporarily be below current health. Neither zero
    // health nor this observation packet establishes a native death event.
    // Reuse the existing pose basis checks, including determinant and NaNs.
    CoopPosePacket pose=peer;std::memcpy(pose.position,p.position,sizeof(p.position));
    std::memcpy(pose.orientation,p.orientation,sizeof(p.orientation));
    return validCoopPose(pose,local.role);
}
