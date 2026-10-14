#pragma once
#include "guard_protocol.h"
constexpr char kPlayerSwordDamageDecl[]="components/characters/player/base/damage/player_sword.damage";
struct EnemyMeleeGeometry {
    float offset[3]{},normal[3]{},direction[3]{};
    std::uint32_t region=0,regions=0;
};
inline bool validEnemyMeleeGeometry(const EnemyMeleeGeometry& g){
    if((g.region!=2&&g.region!=5)||g.regions!=(1u<<g.region))return false;
    float distance=0,normal=0,direction=0;
    for(unsigned i=0;i<3;++i){
        if(!std::isfinite(g.offset[i])||!std::isfinite(g.normal[i])||!std::isfinite(g.direction[i])||
           std::fabs(g.offset[i])>3||std::fabs(g.normal[i])>1.01f||std::fabs(g.direction[i])>1.01f)return false;
        distance+=g.offset[i]*g.offset[i];normal+=g.normal[i]*g.normal[i];direction+=g.direction[i]*g.direction[i];
    }
    return distance<=9&&normal>=.99f&&normal<=1.01f&&direction>=.99f&&direction<=1.01f;
}
inline bool emptyEnemyMeleeGeometry(const EnemyMeleeGeometry& g){
    if(g.region||g.regions)return false;
    for(unsigned i=0;i<3;++i)if(g.offset[i]!=0||g.normal[i]!=0||g.direction[i]!=0)return false;
    return true;
}
enum class EnemyMeleeMessage:std::uint32_t {Arm=1,Ready,Hit,Ack,Stop};
#pragma pack(push,1)
struct CoopEnemyMeleePacket {
    std::uint32_t magic=0x4d453244;
    std::uint16_t version=1,size=sizeof(CoopEnemyMeleePacket);
    CoopGuardPacket binding{};
    std::uint64_t hostArm=0,clientArm=0;
    EnemyMeleeMessage kind=EnemyMeleeMessage::Arm;
    std::uint32_t sourceRole=2;
    std::uint64_t event=0,declaration=0;
    EnemyMeleeGeometry geometry{};
};
#pragma pack(pop)
static_assert(sizeof(CoopEnemyMeleePacket)==256);
inline bool validEnemyMelee(const CoopEnemyMeleePacket& p,const CoopPosePacket& local,const CoopPosePacket& peer){
    if(p.magic!=0x4d453244||p.version!=1||p.size!=sizeof(p)||p.binding.kind!=GuardMessage::Stop||p.binding.mode!=1||
       !validCoopGuard(p.binding,local,peer)||static_cast<unsigned>(p.kind)<1||static_cast<unsigned>(p.kind)>5)return false;
    if(p.kind==EnemyMeleeMessage::Arm){
        if(p.binding.role?(!p.clientArm||p.hostArm):(!p.hostArm||p.clientArm))return false;
    }else if(!p.hostArm||!p.clientArm)return false;
    if(p.kind==EnemyMeleeMessage::Hit||p.kind==EnemyMeleeMessage::Ack){
        if(p.event!=1||p.declaration!=coopMapId(kPlayerSwordDamageDecl)||
           p.sourceRole!=(p.kind==EnemyMeleeMessage::Hit?peer.role:local.role))return false;
        return p.kind==EnemyMeleeMessage::Hit?validEnemyMeleeGeometry(p.geometry):emptyEnemyMeleeGeometry(p.geometry);
    }
    return !p.event&&!p.declaration&&p.sourceRole==2&&emptyEnemyMeleeGeometry(p.geometry);
}
inline bool enemyMeleeSameBinding(const CoopEnemyMeleePacket& a,const CoopEnemyMeleePacket& b){
    return a.hostArm&&a.clientArm&&a.hostArm==b.hostArm&&a.clientArm==b.clientArm&&guardSameBinding(a.binding,b.binding);
}
inline bool enemyMeleeConsume(bool ready,bool& consumed,const CoopEnemyMeleePacket& armed,const CoopEnemyMeleePacket& p){
    if(!ready||consumed||p.kind!=EnemyMeleeMessage::Hit||!enemyMeleeSameBinding(armed,p)||p.event!=1)return false;
    consumed=true;return true;
}
