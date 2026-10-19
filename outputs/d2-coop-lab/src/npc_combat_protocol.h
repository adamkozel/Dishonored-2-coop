#pragma once
#include "npc_authority_protocol.h"
#include "enemy_melee_protocol.h"
#include "npc_damage_declarations.h"
// Reliable native melee damage events, independently scoped from pose
// sequence numbers. No remote pointer, raw damage block or health setter.
enum class NpcCombatMessage:unsigned {Hello=1,Ready,Request,Commit,Stop};
// Kick (1106): native player kick, player_kick.damage (amount10, type0x80,
// reactions 1/1, inflictor = the kicking player itself; confirmed 1105).
// HostWorld (1214o, #14): host-world damage the client cannot replay (a creature or
// NPC source, take 1270e: bloodflies on a checkpoint guard). The host lets it apply
// natively and commits its observed native loss in event order; the client removes
// exactly that loss by the native health subtraction. Never lethal, never a gain.
// Linked (1214fm, #117): a client hit the native Domino network repeated on a linked
// NPC (the link flag set). The host applies it natively with the partner avatar as
// attacker, its declaration, native amount and flags; HostWorld and Dead share the
// outcome. Never a commit.
// PeerNative (1214y, #14): a client hit the replay cannot reproduce (grenade,
// power, tranquilizer or variant bolt), sent as a request carrying only its known
// declaration and native amount. The host applies it natively with the partner
// avatar as attacker; HostWorld and Dead share the outcome. Never a commit.
enum class NpcCombatDamage:unsigned {None=0,Sword,Fatality,Assassination,Corpse,MapTrigger,RangedSpread,RangedContact,Kick,HostWorld,PeerNative,Linked};
#pragma pack(push,1)
struct NpcCombatScriptDamage {std::uint64_t trigger=0,declaration=0;float amount=0;unsigned reserved=0;};
struct NpcCombatRangedDamage {std::uint64_t declaration=0;float amount=0,distance=0;unsigned weapon=0;};
#pragma pack(pop)
inline bool emptyNpcScript(const NpcCombatScriptDamage& s){return !s.trigger&&!s.declaration&&s.amount==0&&!s.reserved;}
inline bool validNpcScript(const NpcCombatScriptDamage& s){return s.trigger&&npcDamageDeclarationName(s.declaration)&&std::isfinite(s.amount)&&s.amount>=0&&s.amount<=1000000&&!s.reserved;}
inline bool npcCombatWorldDamage(NpcCombatDamage d){return d==NpcCombatDamage::Corpse||d==NpcCombatDamage::MapTrigger||d==NpcCombatDamage::HostWorld;}
// Host-world damage passes the host capture natively: neither the attacker nor the
// inflictor is the local player, a player-owned entity or a world sentinel (corpse
// contact and map triggers stay captured and replayed).
inline bool npcCombatHostWorldSource(unsigned localRole,unsigned attacker,unsigned inflictor,bool attackerOwned,bool inflictorOwned){
    return localRole==0&&attacker!=1&&inflictor!=1&&attacker!=0x3fff0010u&&inflictor!=0x3fff0010u&&!attackerOwned&&!inflictorOwned;
}
// Hello health (1214o): 0 equal; 1 the client follows the host's lower health; 2 the
// host waits for the client's corrected Hello; -1 diverged (a replica never gains).
inline int npcCombatHelloHealth(unsigned localRole,float local,float peer){
    if(local==peer)return 0;
    if(localRole==1&&peer>0&&peer<local)return 1;
    if(localRole==0&&local>0&&peer>local)return 2;
    return -1;
}
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
// 1214w (#14): declarations the combat replay reproduces exactly: player sword,
// kick, fatality, assassination, corpse contact and the ranged profiles.
inline bool npcCombatReplayableDeclaration(std::uint64_t key){
    constexpr std::uint64_t keys[]={0x1b4f75938aa0c437ull,0xfe8a6479049dd8c8ull,0x2fe1234fd9ffa6aull,0xc277b040bc6873ccull,0xace60ad990b96dbdull};
    for(auto k:keys)if(k==key)return true;
    for(const auto& p:npcCombatRangedProfiles)if(p.key==key)return true;
    return false;
}
// On the host, any other damage (grenade explosions, powers, thrown props, bolt
// variants) applies natively; HostWorld and Dead share its outcome. World
// sentinels (corpse contact, map triggers) stay captured.
inline bool npcCombatHostNativeDeclaration(unsigned localRole,unsigned attacker,unsigned inflictor,std::uint64_t key){
    return localRole==0&&attacker!=0x3fff0010u&&inflictor!=0x3fff0010u&&!npcCombatReplayableDeclaration(key);
}
inline bool validNpcPeerNative(const NpcCombatRangedDamage& r){
    return npcDamageDeclarationName(r.declaration)&&!npcCombatReplayableDeclaration(r.declaration)&&std::isfinite(r.amount)&&r.amount>=0&&r.amount<=1000&&r.distance==0&&!r.weapon;
}
// 1214fm (#117): the native Domino network repeats a hit on each linked NPC with bit 40 of the damage flags set
// (takes 1647a fatality and 1648a assassination; confirmed runtime). A Linked request carries a replayable player
// declaration, its native amount, and its native flags without the link flag in weapon.
// 1214fn: or a linked knockout (sleep damage), which used to pass through the client natively (1653b).
constexpr std::uint64_t kNpcCombatLinkedFlag=0x10000000000ull;
inline bool npcCombatLinkedSleepDeclaration(std::uint64_t key){
    return key==0xba6f17ec4e0523d7ull||key==0xa0b9d817837f7916ull||key==0x23cd8d2d89e9665aull;
}
inline bool validNpcLinked(const NpcCombatRangedDamage& r){
    return npcDamageDeclarationName(r.declaration)&&(npcCombatReplayableDeclaration(r.declaration)||npcCombatLinkedSleepDeclaration(r.declaration))&&std::isfinite(r.amount)&&r.amount>=0&&r.amount<=1000&&r.distance==0&&r.weapon&&!(r.weapon&~0xffu);
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
    if(damage==NpcCombatDamage::Assassination||damage==NpcCombatDamage::HostWorld||damage==NpcCombatDamage::PeerNative||damage==NpcCombatDamage::Linked)return emptyEnemyMeleeGeometry(g);
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
// 1214fp (#117): a knockout sets the owner suppression flag, and the native maximum drops to the suppressed maximum
// (40 to 10 for the tutorial guard; native_health.h). A HostWorld baseline above the new maximum is no loss to
// commit: it failed validation and stopped the host's whole combat session after every choke (1652b, 1654a, 1655a).
inline bool npcCombatHostWorldRebase(float baseline,float current,float maximum){
    return std::isfinite(baseline)&&std::isfinite(maximum)&&baseline>maximum&&std::isfinite(current)&&current<=maximum;
}
inline bool npcCombatHealth(float current,float maximum){return std::isfinite(current)&&std::isfinite(maximum)&&current>0&&maximum>0&&current<=maximum&&maximum<=1000000;}
inline bool validNpcCombat(const CoopNpcCombat& p,const CoopPosePacket& local,const CoopPosePacket& peer){
    if(p.magic!=0x434e3244||p.version!=7||p.size!=sizeof(p)||p.binding.kind!=NpcAuthorityMessage::Stop||
       !validNpcHeader(p.binding,local,peer)||!p.sourceNonce||static_cast<unsigned>(p.kind)<1||static_cast<unsigned>(p.kind)>5)return false;
    if(p.damage==NpcCombatDamage::MapTrigger?!validNpcScript(p.script):!emptyNpcScript(p.script))return false;
    if(p.damage==NpcCombatDamage::PeerNative?!validNpcPeerNative(p.ranged):p.damage==NpcCombatDamage::Linked?!validNpcLinked(p.ranged):npcCombatRanged(p.damage)?!validNpcRanged(p.damage,p.ranged):!emptyNpcRanged(p.ranged))return false;
    if(p.kind==NpcCombatMessage::Hello)return !p.sourceActorArm&&!p.targetNonce&&!p.event&&!p.request&&p.sourceRole==2&&p.damage==NpcCombatDamage::None&&npcCombatHealth(p.before,p.maximum)&&!p.after&&!p.inflicted&&emptyEnemyMeleeGeometry(p.geometry);
    if(!p.targetNonce)return false;
    if(p.kind==NpcCombatMessage::Ready||p.kind==NpcCombatMessage::Stop)return !p.sourceActorArm&&!p.event&&!p.request&&p.sourceRole==2&&p.damage==NpcCombatDamage::None&&!p.before&&!p.after&&!p.maximum&&!p.inflicted&&emptyEnemyMeleeGeometry(p.geometry);
    if(!npcCombatGeometry(p.damage,p.geometry))return false;
    const bool world=npcCombatWorldDamage(p.damage);
    if(world?(p.sourceRole!=2||(p.damage==NpcCombatDamage::Corpse?!p.sourceActorArm:p.sourceActorArm!=0)):(p.sourceRole>1||p.sourceActorArm))return false;
    if(p.damage==NpcCombatDamage::HostWorld&&(p.kind!=NpcCombatMessage::Commit||!(p.after>0)||!(p.after<p.before)))return false;
    if((p.damage==NpcCombatDamage::PeerNative||p.damage==NpcCombatDamage::Linked)&&p.kind!=NpcCombatMessage::Request)return false;
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
