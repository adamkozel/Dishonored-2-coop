#pragma once
#include "guard_protocol.h"
constexpr char kEnemyAssassinationDecl[]="components/characters/player/base/damage/assassination_damage.damage";
enum class EnemyOutcomeMessage:std::uint32_t {Arm=1,Ready,Kill,Ack,Stop};
#pragma pack(push,1)
struct CoopEnemyOutcomePacket {
    std::uint32_t magic=0x4f453244;
    std::uint16_t version=1,size=sizeof(CoopEnemyOutcomePacket);
    // Reuse the validated guard identity envelope; kind is always Stop so
    // either role is allowed and every unused pose/health field is zero.
    CoopGuardPacket binding{};
    std::uint64_t hostArm=0,clientArm=0;
    EnemyOutcomeMessage kind=EnemyOutcomeMessage::Arm;
    std::uint32_t sourceRole=2;
    std::uint64_t event=0,declaration=0;
};
#pragma pack(pop)
static_assert(sizeof(CoopEnemyOutcomePacket)==212);
inline bool validEnemyOutcome(const CoopEnemyOutcomePacket& p,const CoopPosePacket& local,const CoopPosePacket& peer){
    if(p.magic!=0x4f453244||p.version!=1||p.size!=sizeof(p)||p.binding.kind!=GuardMessage::Stop||p.binding.mode!=1||
       !validCoopGuard(p.binding,local,peer)||static_cast<unsigned>(p.kind)<1||static_cast<unsigned>(p.kind)>5)return false;
    if(p.kind==EnemyOutcomeMessage::Arm){
        if(p.binding.role?(!p.clientArm||p.hostArm):(!p.hostArm||p.clientArm))return false;
    }else if(!p.hostArm||!p.clientArm)return false;
    if(p.kind==EnemyOutcomeMessage::Kill||p.kind==EnemyOutcomeMessage::Ack)
        return p.event==1&&p.declaration==coopMapId(kEnemyAssassinationDecl)&&
            p.sourceRole==(p.kind==EnemyOutcomeMessage::Kill?peer.role:local.role);
    return !p.event&&!p.declaration&&p.sourceRole==2;
}
inline bool enemySameBinding(const CoopEnemyOutcomePacket& a,const CoopEnemyOutcomePacket& b){
    return a.hostArm&&a.clientArm&&a.hostArm==b.hostArm&&a.clientArm==b.clientArm&&guardSameBinding(a.binding,b.binding);
}
inline bool enemyConsume(bool ready,bool& consumed,const CoopEnemyOutcomePacket& armed,const CoopEnemyOutcomePacket& p){
    if(!ready||consumed||p.kind!=EnemyOutcomeMessage::Kill||!enemySameBinding(armed,p)||p.event!=1)return false;
    consumed=true;return true;
}
