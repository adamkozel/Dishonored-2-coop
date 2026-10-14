#pragma once
#include "guard_protocol.h"
#include "body_retarget.h"
constexpr unsigned kGuardBodyJoints=223;
constexpr const char* kGuardBodySkeleton="models/characters/small/guards/gs_guards/gs_guard_01/head07/gs_guard_small01_head07.md6skl";
#pragma pack(push,1)
struct CoopGuardBodyPacket {
    std::uint32_t magic=0x41473244;
    std::uint16_t version=1,size=sizeof(CoopGuardBodyPacket);
    std::uint64_t rig=0;
    CoopGuardPacket binding{};
    float local[kGuardBodyJoints][12]{};
};
#pragma pack(pop)
static_assert(sizeof(CoopGuardBodyPacket)==10884);
inline BodyMatrix guardBodyMatrix(const float* source){
    BodyMatrix m{};for(unsigned c=0;c<4;++c)for(unsigned r=0;r<3;++r)m.v[c*4+r]=source[c*3+r];return m;
}
inline bool validGuardBody(const CoopGuardBodyPacket& p,const CoopPosePacket& local,const CoopPosePacket& peer){
    if(p.magic!=0x41473244||p.version!=1||p.size!=sizeof(p)||!p.rig||local.role!=1||p.binding.role||
       p.binding.mode!=1||p.binding.kind!=GuardMessage::Pose||!validCoopGuard(p.binding,local,peer))return false;
    for(const auto& joint:p.local)if(!bodyRigid(guardBodyMatrix(joint),3.f))return false;
    return true;
}
inline std::uint64_t guardBodyRig(const NativeSkeleton& s){
    if(std::strcmp(s.skeletonName,kGuardBodySkeleton)||s.parents.size()!=kGuardBodyJoints||s.bindLocal.size()!=kGuardBodyJoints)return 0;
    auto hash=coopMapId(s.skeletonName);
    const auto byte=[&](unsigned char b){hash^=b;hash*=1099511628211ULL;};
    // Asset layout and bind data identify the wire rig. Interned native name
    // IDs are local to each game process and never become wire identities.
    for(unsigned j=0;j<kGuardBodyJoints;++j){
        const auto parent=s.parents[j];if(parent<-1||parent>=static_cast<int>(j))return 0;
        byte(static_cast<unsigned char>(parent));byte(static_cast<unsigned char>(parent>>8));
        BodyMatrix bind{};if(!bodyFromQts(s.bindLocal[j],bind))return 0;
        for(unsigned i=0;i<11;++i)if(i!=7){std::uint32_t bits=0;std::memcpy(&bits,&s.bindLocal[j][i],4);for(unsigned n=0;n<4;++n)byte(static_cast<unsigned char>(bits>>(n*8)));}
    }
    return hash;
}
