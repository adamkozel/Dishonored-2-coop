#pragma once
#include "npc_authority_protocol.h"
#include "enemy_melee_protocol.h"
#include "npc_damage_declarations.h"
// Reliable native melee damage events, independently scoped from pose
// sequence numbers. No remote pointer, raw damage block or health setter.
enum class NpcCombatMessage:unsigned {Hello=1,Ready,Request,Commit,Stop};
// Kick (1106): native player kick, player_kick.damage (amount10, type0x80,
// reactions 1/1, inflictor = the kicking player itself; confirmed 1105).
enum class NpcCombatDamage:unsigned {None=0,Sword,Fatality,Assassination,Corpse,MapTrigger,RangedSpread,RangedContact,Kick};
#pragma pack(push,1)
struct NpcCombatScriptDamage {std::uint64_t trigger=0,declaration=0;float amount=0;unsigned reserved=0;};
struct NpcCombatRangedDamage {std::uint64_t declaration=0;float amount=0,distance=0;unsigned weapon=0;};
#pragma pack(pop)
inline bool emptyNpcScript(const NpcCombatScriptDamage& s){return !s.trigger&&!s.declaration&&s.amount==0&&!s.reserved;}
inline bool validNpcScript(const NpcCombatScriptDamage& s){return s.trigger&&npcDamageDeclarationName(s.declaration)&&std::isfinite(s.amount)&&s.amount>=0&&s.amount<=1000000&&!s.reserved;}
inline bool npcCombatWorldDamage(NpcCombatDamage d){return d==NpcCombatDamage::Corpse||d==NpcCombatDamage::MapTrigger;}
inline bool npcCombatRanged(NpcCombatDamage d){return d==NpcCombatDamage::RangedSpread||d==NpcCombatDamage::RangedContact;}
inline bool emptyNpcRanged(const NpcCombatRangedDamage& r){return !r.declaration&&r.amount==0&&r.distance==0&&!r.weapon;}
// Exact declaration profiles share capture, geometry, ordering and native
// playback. A profile is admitted only after native callback/declaration proof.
struct NpcCombatRangedProfile {NpcCombatDamage damage;std::uint64_t key,flags;unsigned weapon;float minimum,maximum;unsigned reaction;};
inline constexpr NpcCombatRangedProfile npcCombatRangedProfiles[]={
    {NpcCombatDamage::RangedSpread,0xfcb8900ee15d2295ull,0x100000000ull,1,0,20,4},
    {NpcCombatDamage::RangedContact,0xc9aab9596e9c2cc7ull,0x200ull,1,5,20,2},
    {NpcCombatDamage::RangedContact,0x7b198783ddf206e3ull,0x100ull,2,0,15,2}
};
inline const NpcCombatRangedProfile* npcCombatRangedProfile(NpcCombatDamage d,const NpcCombatRangedDamage& r){
    for(const auto& p:npcCombatRangedProfiles)if(p.damage==d&&p.key==r.declaration&&p.weapon==r.weapon)return &p;
    return nullptr;
}
inline unsigned npcCombatRangedWeapon(std::uint64_t flags){
    for(const auto& p:npcCombatRangedProfiles)if(p.flags==flags)return p.weapon;
    return 0;
}
inline bool validNpcRanged(NpcCombatDamage d,const NpcCombatRangedDamage& r){
    const auto* p=npcCombatRangedProfile(d,r);
    return p&&std::isfinite(r.amount)&&r.amount>0&&r.amount>=p->minimum&&r.amount<=p->maximum&&
        std::isfinite(r.distance)&&r.distance>=0&&r.distance<=32;
}
#pragma pack(push,1)
struct CoopNpcCombat {
    unsigned magic=0x434e3244;
    unsigned short version=7,size=sizeof(CoopNpcCombat);
    NpcAuthorityHeader binding{};
    NpcCombatMessage kind=NpcCombatMessage::Hello;
    unsigned sourceRole=2;
    NpcCombatDamage damage=NpcCombatDamage::None;
    std::uint64_t sourceNonce=0,targetNonce=0,event=0,request=0;
    float before=0,after=0,maximum=0,inflicted=0;
    EnemyMeleeGeometry geometry{};
    NpcCombatScriptDamage script{};
    std::uint64_t sourceActorArm=0; // Host-owned world source; never a native handle/pointer.
    NpcCombatRangedDamage ranged{};
};
#pragma pack(pop)
static_assert(sizeof(CoopNpcCombat)==276);
inline bool npcSwordGeometry(const EnemyMeleeGeometry& g){
    if(g.region>=32||g.regions!=(1u<<g.region))return false;
    auto probe=g;probe.region=2;probe.regions=4;return validEnemyMeleeGeometry(probe);
}
// 1108: the native player kick hit carries region1/regions2, a unit direction
// and a zero normal (confirmed 1107).
inline bool npcKickGeometry(const EnemyMeleeGeometry& g){
    if(g.region!=1||g.regions!=2||g.normal[0]!=0||g.normal[1]!=0||g.normal[2]!=0)return false;
    auto probe=g;probe.region=2;probe.regions=4;probe.normal[0]=1;return validEnemyMeleeGeometry(probe);
}
inline bool npcCombatGeometry(NpcCombatDamage damage,const EnemyMeleeGeometry& g){
    if(damage==NpcCombatDamage::Kick)return npcKickGeometry(g);
    if(damage==NpcCombatDamage::RangedContact)return npcSwordGeometry(g);
    if(damage==NpcCombatDamage::RangedSpread){
        if(g.region!=1||!g.regions||(g.regions&~255u)||g.normal[0]!=0||g.normal[1]!=0||g.normal[2]!=0)return false;
        auto probe=g;probe.region=2;probe.regions=4;probe.normal[0]=1;return validEnemyMeleeGeometry(probe);
    }
    if(damage==NpcCombatDamage::Assassination)return emptyEnemyMeleeGeometry(g);
    if(damage==NpcCombatDamage::MapTrigger&&emptyEnemyMeleeGeometry(g))return true;
    if(damage==NpcCombatDamage::Corpse||damage==NpcCombatDamage::MapTrigger){
        if(g.region!=1||g.regions)return false;
        auto probe=g;probe.region=2;probe.regions=4;return validEnemyMeleeGeometry(probe);
    }
    return (damage==NpcCombatDamage::Sword||damage==NpcCombatDamage::Fatality)&&npcSwordGeometry(g);
}
// IEEE float subtraction can round differently from the native damage value.
// Require conservation to within two rounding units, never exact equality of
// (before-after) and a separately computed native inflicted value.
inline bool npcCombatClose(float a,float b){
    return std::isfinite(a)&&std::isfinite(b)&&std::fabs(a-b)<=0.000001f+0.00000024f*(std::max)(std::fabs(a),std::fabs(b));
}
inline bool npcCombatNativeLethal(float before,float inflicted){
    return std::isfinite(before)&&before>0&&std::isfinite(inflicted)&&inflicted>=0&&before-inflicted<0.5f;
}
inline bool npcCombatReplicaWorldEvent(unsigned localRole,bool localPlayerOwned){return localRole==1&&!localPlayerOwned;}
inline bool npcCombatHealth(float current,float maximum){return std::isfinite(current)&&std::isfinite(maximum)&&current>0&&maximum>0&&current<=maximum&&maximum<=1000000;}
inline bool validNpcCombat(const CoopNpcCombat& p,const CoopPosePacket& local,const CoopPosePacket& peer){
    if(p.magic!=0x434e3244||p.version!=7||p.size!=sizeof(p)||p.binding.kind!=NpcAuthorityMessage::Stop||
       !validNpcHeader(p.binding,local,peer)||!p.sourceNonce||static_cast<unsigned>(p.kind)<1||static_cast<unsigned>(p.kind)>5)return false;
    if(p.damage==NpcCombatDamage::MapTrigger?!validNpcScript(p.script):!emptyNpcScript(p.script))return false;
    if(npcCombatRanged(p.damage)?!validNpcRanged(p.damage,p.ranged):!emptyNpcRanged(p.ranged))return false;
    if(p.kind==NpcCombatMessage::Hello)return !p.sourceActorArm&&!p.targetNonce&&!p.event&&!p.request&&p.sourceRole==2&&p.damage==NpcCombatDamage::None&&npcCombatHealth(p.before,p.maximum)&&!p.after&&!p.inflicted&&emptyEnemyMeleeGeometry(p.geometry);
    if(!p.targetNonce)return false;
    if(p.kind==NpcCombatMessage::Ready||p.kind==NpcCombatMessage::Stop)return !p.sourceActorArm&&!p.event&&!p.request&&p.sourceRole==2&&p.damage==NpcCombatDamage::None&&!p.before&&!p.after&&!p.maximum&&!p.inflicted&&emptyEnemyMeleeGeometry(p.geometry);
    if(!npcCombatGeometry(p.damage,p.geometry))return false;
    const bool world=npcCombatWorldDamage(p.damage);
    if(world?(p.sourceRole!=2||(p.damage==NpcCombatDamage::Corpse?!p.sourceActorArm:p.sourceActorArm!=0)):(p.sourceRole>1||p.sourceActorArm))return false;
    if(p.kind==NpcCombatMessage::Request)return !world&&peer.role==1&&p.sourceRole==1&&p.request&&!p.event&&!p.before&&!p.after&&!p.maximum&&!p.inflicted;
    // Terminal native outcomes retain a scoped tombstone after pose/AI retirement.
    return peer.role==0&&p.event&&(p.sourceRole==1?p.request!=0:p.request==0)&&npcCombatHealth(p.before,p.maximum)&&
        std::isfinite(p.after)&&p.after>=0&&(world?p.after<=p.before:p.after<p.before)&&std::isfinite(p.inflicted)&&(world?p.inflicted>=0:p.inflicted>0)&&
        (p.after>0 ? (p.damage==NpcCombatDamage::Sword||p.damage==NpcCombatDamage::Kick||npcCombatRanged(p.damage)||world)&&npcCombatClose(p.inflicted,p.before-p.after) :
        p.inflicted<=p.before&&npcCombatNativeLethal(p.before,p.inflicted));
}
enum class NpcCombatOrder {Duplicate,Next,Gap};
inline NpcCombatOrder npcCombatOrder(std::uint64_t previous,std::uint64_t event){return event<=previous?NpcCombatOrder::Duplicate:(event==previous+1?NpcCombatOrder::Next:NpcCombatOrder::Gap);}
// One native action can report several damage contacts before its first
// network acknowledgement. Retain each ordered request instead of dropping
// every callback after a single outstanding hit (pistol observation903).
struct NpcCombatPendingRequests {
    std::array<std::uint64_t,32> ids{};unsigned head=0,count=0;
    std::uint64_t front()const{return count?ids[head]:0;}
    bool push(std::uint64_t id){
        if(!id||count==ids.size()||(count&&id!=ids[(head+count-1)%ids.size()]+1))return false;
        ids[(head+count)%ids.size()]=id;++count;return true;
    }
    bool acknowledge(std::uint64_t id){
        if(!count||id!=front())return false;
        ids[head]=0;head=(head+1)%ids.size();--count;return true;
    }
    void clear(){*this={};}
};

// 1214i (take 1258d): a retired, non-terminal combat channel no longer blocks its NPC forever.
// Once the NPC is bound and alive again, a fresh channel re-handshakes (new peer nonce, fresh
// health comparison) 2 s after the retirement; a death or terminal outcome ends the NPC for good.
inline bool npcCombatChannelBlocksRebind(bool enabled,bool terminal,bool deathPending,std::uint64_t retiredAt,std::uint64_t tick){
    return enabled||terminal||deathPending||(tick>=retiredAt&&tick-retiredAt<2000);
}
