#pragma once
#include "world_protocol.h"
#include "arm_retarget.h"

// Fixed semantic order: five arm/weapon joints followed by sixteen finger
// joints in kArmStreamJoints order. Version 2 refuses the earlier five-joint packet.
// Native pointers, entity handles and interned joint IDs are never wire IDs.
#pragma pack(push,1)
struct CoopArmPacket {
    std::uint32_t magic=0x41433244;
    std::uint16_t version=2,size=sizeof(CoopArmPacket);
    std::uint32_t build=0x67a50cde,role=0;
    std::uint64_t sequence=0,sourceSession=0,targetSession=0;
    std::uint64_t sourceWorldEpoch=0,targetWorldEpoch=0,mapId=0,rig=0;
    std::uint32_t rootSequence=0,character=0,side=2,reserved=0;
    float global[kArmStreamJointCount][12]{};
};
#pragma pack(pop)
static_assert(sizeof(CoopArmPacket)==1096);
inline bool armSequenceFresh(std::uint64_t sequence,std::uint64_t session,std::uint64_t lastSession,std::uint64_t lastSequence){
    return sequence&&(session!=lastSession||sequence>lastSequence);
}
inline bool armRootFresh(std::uint32_t source,std::uint32_t received){
    // Unsigned subtraction also handles the source root sequence wrapping.
    return source&&received&&static_cast<std::uint32_t>(received-source)<=8;
}
inline bool validCoopArm(const CoopArmPacket& p,const CoopPosePacket& local,const CoopPosePacket& peer){
    if(p.magic!=0x41433244||p.version!=2||p.size!=sizeof(p)||p.build!=0x67a50cde||p.role>1||p.role!=peer.role||
       !p.sequence||!p.rig||p.character!=p.role+1||p.side!=2||p.reserved||!coopWorldPairReady(local,peer)||
       p.sourceSession!=peer.session||p.targetSession!=local.session||p.sourceWorldEpoch!=peer.worldEpoch||
       p.targetWorldEpoch!=local.worldEpoch||p.mapId!=local.mapId||!armRootFresh(p.rootSequence,peer.sequence))return false;
    for(const auto& joint:p.global){BodyMatrix m{};
        for(unsigned c=0;c<4;++c)for(unsigned r=0;r<3;++r)m.v[c*4+r]=joint[c*3+r];
        if(!bodyRigid(m,3.f))return false;
    }
    return true;
}
inline std::uint64_t nativeArmRig(const NativeSkeleton& s){
    if(s.playerArm.side!=2||std::strcmp(s.skeletonName,"models/characters/player/emily/player_rightarm_emily.md6skl")||
       s.parents.size()!=69||s.bindLocal.size()!=69||s.parents[0]!=-1||s.parents[1]!=0||s.parents[2]!=1||
       s.parents[3]!=2||s.parents[4]!=3||s.parents[59]!=4)return 0;
    auto hash=coopMapId(s.skeletonName);
    const auto byte=[&](unsigned char b){hash^=b;hash*=1099511628211ULL;};
    for(unsigned j=0;j<69;++j){
        const auto parent=s.parents[j];if(parent<-1||parent>=static_cast<int>(j))return 0;
        byte(static_cast<unsigned char>(parent));byte(static_cast<unsigned char>(parent>>8));
        BodyMatrix bind{};if(!bodyFromQts(s.bindLocal[j],bind))return 0;
        for(unsigned k=0;k<11;++k)if(k!=7){std::uint32_t bits=0;std::memcpy(&bits,&s.bindLocal[j][k],4);for(unsigned b=0;b<4;++b)byte(static_cast<unsigned char>(bits>>(b*8)));}
    }
    return hash;
}
