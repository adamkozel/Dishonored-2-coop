#pragma once
#include "npc_authority_protocol.h"
#include <cstring>
enum class NpcAttackMessage:unsigned {Hello=1,Ready,Hit,Ack,Stop,Terminal};
// 1214kg (#219): kNpcAttackMelee marks a melee weapon contact, which the receiver resolves natively against its block.
constexpr unsigned kNpcAttackBlockable=1,kNpcAttackHeavy=2,kNpcAttackMelee=4;
constexpr std::size_t kNpcAttackDamageNameSize=96;
// Native 140859980 results 0/1/2 are reported as Blocked/ParryLose/ParryWin.
enum class NpcAttackDefense:unsigned {Damage=0,Blocked,ParryLose,ParryWin};
#pragma pack(push,1)
struct CoopNpcAttack {
    std::uint32_t magic=0x484e3244;std::uint16_t version=4,size=sizeof(CoopNpcAttack);
    NpcAuthorityHeader binding{};
    // Hit: host weapon attack properties read by native 1406357d0
    // (blockable decl+0x180, heavy component+0x3428 bit2). Ack: the
    // receiver's native defense outcome. Never a health or damage value.
    NpcAttackMessage kind=NpcAttackMessage::Hello;unsigned flags=0;
    std::uint64_t sourceNonce=0,targetNonce=0,event=0;
    float offset[3]{},direction[3]{};
    // 1214kg (#219): Hit: the native damage declaration of the host-world contact on the partner's clone, by name.
    char damage[kNpcAttackDamageNameSize]{};
};
#pragma pack(pop)
static_assert(sizeof(CoopNpcAttack)==272);
// 1214kg (#219): an enemy damage declaration the relay may apply to the receiving player: lower-case path characters,
// NUL-terminated and NUL-padded, under the enemy equipment, NPC or animal damage folders. Never a player, base
// (falls, crushing, physics, kills) or code-referenced (generic kill, sleep, remove) declaration.
inline bool npcAttackDamageAllowed(const char* name,std::size_t size){
    const auto n=strnlen(name,size);if(n<8||n>=size)return false;
    for(std::size_t i=0;i<n;++i){const char ch=name[i];if(!((ch>='a'&&ch<='z')||(ch>='0'&&ch<='9')||ch=='_'||ch=='/'||ch=='.'))return false;}
    for(std::size_t i=n;i<size;++i)if(name[i])return false;
    if(std::memcmp(name+n-7,".damage",7)||std::strstr(name,"..")||std::strstr(name,"//"))return false;
    static const char* const folders[]={"components/equipment/damage/","components/characters/npc/","components/characters/animals/"};
    for(const auto* f:folders)if(!std::strncmp(name,f,std::strlen(f)))return true;
    return false;
}
inline bool validNpcAttack(const CoopNpcAttack& p,const CoopPosePacket& local,const CoopPosePacket& peer){
    if(p.magic!=0x484e3244||p.version!=4||p.size!=sizeof(p)||!p.sourceNonce||
       (p.kind==NpcAttackMessage::Hit?p.flags>(kNpcAttackBlockable|kNpcAttackHeavy|kNpcAttackMelee)||((p.flags&3)&&!(p.flags&kNpcAttackMelee)):
        p.kind==NpcAttackMessage::Ack?p.flags>3:p.flags!=0)||
       p.binding.kind!=NpcAuthorityMessage::Stop||!validNpcHeader(p.binding,local,peer)||
       (p.kind==NpcAttackMessage::Hello?p.targetNonce!=0:!p.targetNonce))return false;
    if(p.kind==NpcAttackMessage::Hit){
        if(p.binding.role||!p.event||!npcAttackDamageAllowed(p.damage,sizeof(p.damage)))return false;
        float norm=0;for(unsigned i=0;i<3;++i){
            if(!std::isfinite(p.offset[i])||std::fabs(p.offset[i])>3||!std::isfinite(p.direction[i])||std::fabs(p.direction[i])>1.01f)return false;
            norm+=p.direction[i]*p.direction[i];
        }return norm>=.9f&&norm<=1.1f;
    }
    for(unsigned i=0;i<3;++i)if(p.offset[i]!=0||p.direction[i]!=0)return false;
    for(const char ch:p.damage)if(ch)return false;
    switch(p.kind){
    case NpcAttackMessage::Terminal:case NpcAttackMessage::Ack:return p.binding.role==1&&p.event;
    case NpcAttackMessage::Hello:case NpcAttackMessage::Ready:case NpcAttackMessage::Stop:return !p.event;
    default:return false;
    }
}
