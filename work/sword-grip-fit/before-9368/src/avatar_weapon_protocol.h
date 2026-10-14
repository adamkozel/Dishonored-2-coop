#pragma once
#include "arm_protocol.h"

// Explicit local opt-in handshake and observed Corvo/Emily player draw state.
// Character/item assets are fixed by code, never supplied over the network.
#pragma pack(push,1)
struct CoopAvatarWeaponPacket {
    std::uint32_t magic=0x57413244;
    std::uint16_t version=2,size=sizeof(CoopAvatarWeaponPacket);
    std::uint32_t build=0x67a50cde,role=0;
    std::uint64_t sequence=0,sourceSession=0,targetSession=0;
    std::uint64_t sourceWorldEpoch=0,targetWorldEpoch=0,mapId=0;
    std::uint64_t sourceToken=0,targetToken=0;
    std::uint32_t rootSequence=0,character=0,kind=0,state=0; // kind 0=hello, 1=state; state 0=holstered, 1=drawn
};
#pragma pack(pop)
static_assert(sizeof(CoopAvatarWeaponPacket)==96);
inline bool validAvatarWeapon(const CoopAvatarWeaponPacket& p,const CoopPosePacket& local,const CoopPosePacket& peer){
    return p.magic==0x57413244&&p.version==2&&p.size==sizeof(p)&&p.build==0x67a50cde&&p.role<=1&&p.role==peer.role&&
        p.sequence&&p.sourceToken&&p.character==p.role+1&&p.kind<=1&&p.state<=1&&
        (p.kind==0?p.state==0:p.targetToken!=0)&&coopWorldPairReady(local,peer)&&
        p.sourceSession==peer.session&&p.targetSession==local.session&&p.sourceWorldEpoch==peer.worldEpoch&&
        p.targetWorldEpoch==local.worldEpoch&&p.mapId==local.mapId&&armRootFresh(p.rootSequence,peer.sequence);
}
inline bool avatarWeaponTokensMatch(const CoopAvatarWeaponPacket& p,std::uint64_t token,std::uint64_t peerToken){
    return token&&peerToken&&p.sourceToken==peerToken&&p.targetToken==token;
}
