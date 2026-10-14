#pragma once
#include "world_protocol.h"

enum class GuardMessage : std::uint32_t {Request=1,Offer,Accept,Commit,Ready,Pose,Stop};
enum class GuardPhase : std::uint32_t {Idle,Armed,Requested,Offered,Accepted,Committed,Pausing,Active};
constexpr const char* kGuardMap="campaign/dunwall/escape/tower/dunwall_escape_tower_p";
constexpr const char* kGuardSpawner="ld_spawner_guard_tutorial_1";
constexpr const char* kGuardDefinition="models/characters/def/generic/dunwall_guard_male_small_01.def";
constexpr const char* kCombatGuardSpawner="ld_spawner_guard_escape_1";
constexpr const char* kCombatGuardDefinition="models/characters/def/generic/guard_male_small_01.def";
inline const char* guardSpawner(unsigned mode){return mode==1?kCombatGuardSpawner:kGuardSpawner;}
inline const char* guardDefinition(unsigned mode){return mode==1?kCombatGuardDefinition:kGuardDefinition;}
inline std::uint64_t guardEntityKey(unsigned mode=0){return coopMapId(mode==1?
    "ld_spawner_guard_escape_1|models/characters/def/generic/guard_male_small_01.def":
    "ld_spawner_guard_tutorial_1|models/characters/def/generic/dunwall_guard_male_small_01.def");}
#pragma pack(push,1)
// Separate from read-only NPC observation packets. Every transfer is bound to
// an explicit client request nonce and both actual native incarnations.
struct CoopGuardPacket {
    std::uint32_t magic=0x47433244;
    std::uint16_t version=3,size=sizeof(CoopGuardPacket);
    std::uint32_t build=0x67a50cde,role=0;
    GuardMessage kind=GuardMessage::Request;
    std::uint32_t mode=0; // 0: inactive tutorial guard, 1: host combat escape guard
    std::uint64_t sequence=0,binding=0,request=0;
    std::uint64_t hostSession=0,clientSession=0,hostEpoch=0,clientEpoch=0,mapId=0,entityKey=0;
    std::uint32_t hostHandle=0,clientHandle=0;
    float position[3]{},orientation[9]{};
    std::uint32_t healthValid=0;
    float health=0,maximumHealth=0;
};
#pragma pack(pop)
static_assert(sizeof(CoopGuardPacket)==164);
inline bool guardSameBinding(const CoopGuardPacket& a,const CoopGuardPacket& b) {
    return a.binding&&a.mode==b.mode&&a.binding==b.binding&&a.request==b.request&&a.hostSession==b.hostSession&&a.clientSession==b.clientSession&&
        a.hostEpoch==b.hostEpoch&&a.clientEpoch==b.clientEpoch&&a.mapId==b.mapId&&a.entityKey==b.entityKey&&
        a.hostHandle==b.hostHandle&&a.clientHandle==b.clientHandle;
}
inline bool validCoopGuard(const CoopGuardPacket& p,const CoopPosePacket& local,const CoopPosePacket& peer) {
    const auto& host=local.role?peer:local;const auto& client=local.role?local:peer;
    if(p.magic!=0x47433244||p.version!=3||p.size!=sizeof(p)||p.build!=0x67a50cde||p.role!=peer.role||p.mode>1||!p.sequence||
       !p.request||!p.clientHandle||!coopWorldPairReady(local,peer)||p.hostSession!=host.session||p.clientSession!=client.session||
       p.hostEpoch!=host.worldEpoch||p.clientEpoch!=client.worldEpoch||p.mapId!=host.mapId||p.mapId!=coopMapId(kGuardMap)||
       p.entityKey!=guardEntityKey(p.mode)||static_cast<unsigned>(p.kind)<1||static_cast<unsigned>(p.kind)>7)return false;
    const bool fromHost=p.kind==GuardMessage::Offer||p.kind==GuardMessage::Commit||p.kind==GuardMessage::Pose;
    const bool fromClient=p.kind==GuardMessage::Request||p.kind==GuardMessage::Accept||p.kind==GuardMessage::Ready;
    if((fromHost&&p.role)||(fromClient&&p.role!=1))return false;
    if(p.kind==GuardMessage::Request){if(p.binding||p.hostHandle)return false;}
    else if(!p.binding||!p.hostHandle)return false;
    if(p.kind==GuardMessage::Pose) {
        // This fixed-guard experiment supports nonfatal health only. Death,
        // healing, modifiers and native hit reactions need separate authority.
        if(p.healthValid!=1||!std::isfinite(p.health)||p.health<1||p.health>40||p.maximumHealth!=40)return false;
        CoopPosePacket pose=host;std::memcpy(pose.position,p.position,sizeof(p.position));std::memcpy(pose.orientation,p.orientation,sizeof(p.orientation));
        return validCoopPose(pose,1);
    }
    if(p.healthValid||p.health!=0||p.maximumHealth!=0)return false;
    for(float f:p.position)if(f!=0)return false;
    for(float f:p.orientation)if(f!=0)return false;
    return true;
}
inline bool guardPhaseAccepts(GuardPhase phase,GuardMessage kind,std::uint32_t localRole) {
    if(kind==GuardMessage::Stop)return phase!=GuardPhase::Idle&&phase!=GuardPhase::Armed&&phase!=GuardPhase::Requested;
    if(!localRole)return (phase==GuardPhase::Armed&&kind==GuardMessage::Request)||
        (phase==GuardPhase::Offered&&kind==GuardMessage::Accept)||(phase==GuardPhase::Committed&&kind==GuardMessage::Ready);
    return (phase==GuardPhase::Requested&&kind==GuardMessage::Offer)||(phase==GuardPhase::Accepted&&kind==GuardMessage::Commit)||
        (phase==GuardPhase::Active&&kind==GuardMessage::Pose);
}
