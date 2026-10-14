#pragma once
#include "guard_protocol.h"
#include "native_sword_equipment.h"

#pragma pack(push,1)
struct CoopGuardWeaponPacket {
    std::uint32_t magic=0x45473244;
    std::uint16_t version=1,size=sizeof(CoopGuardWeaponPacket);
    CoopGuardPacket binding{};
    std::uint64_t item=0,rootSequence=0;
    std::uint32_t drawn=0,reserved=0;
};
#pragma pack(pop)
static_assert(sizeof(CoopGuardWeaponPacket)==196);
inline bool validGuardWeapon(const CoopGuardWeaponPacket& p,const CoopPosePacket& local,const CoopPosePacket& peer){
    return p.magic==0x45473244&&p.version==1&&p.size==sizeof(p)&&p.item==coopMapId(kGuardSwordItem)&&p.rootSequence&&
        p.drawn<=1&&!p.reserved&&local.role==1&&!p.binding.role&&p.binding.mode==1&&p.binding.kind==GuardMessage::Pose&&
        validCoopGuard(p.binding,local,peer);
}
inline bool guardWeaponSequenceFresh(const CoopGuardWeaponPacket& p,std::uint64_t session,std::uint64_t lastSession,std::uint64_t lastSequence){
    return p.binding.sequence&&(session!=lastSession||p.binding.sequence>lastSequence);
}
inline bool guardWeaponRootFresh(std::uint64_t source,std::uint64_t received){
    // At 30 root poses/second, eight sequence steps bound the equipment to
    // approximately the same 250 ms root lease. Future roots are not trusted.
    return source&&received>=source&&received-source<=8;
}
