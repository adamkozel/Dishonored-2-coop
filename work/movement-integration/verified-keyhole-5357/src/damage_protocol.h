#pragma once
#include "guard_protocol.h"
enum class SwordMessage:std::uint32_t {Arm=1,Ready,Hit,Stop};
constexpr const char* kSwordGuardSpawner="ld_spawner_guard_escape_1";
constexpr const char* kSwordGuardDefinition="models/characters/def/generic/guard_male_small_01.def";
inline std::uint64_t swordGuardKey(){return coopMapId("ld_spawner_guard_escape_1|models/characters/def/generic/guard_male_small_01.def");}
#pragma pack(push,1)
// Bounded observed sword hits per explicitly armed pair. No pointers, remote actor
// handles, arbitrary declarations, health values or damage amounts cross here.
struct CoopSwordPacket {
    std::uint32_t magic=0x44433244;std::uint16_t version=2,size=sizeof(CoopSwordPacket);
    std::uint32_t build=0x67a50cde,role=0;SwordMessage kind=SwordMessage::Arm;std::uint32_t limit=1;
    std::uint64_t sequence=0,hostSession=0,clientSession=0,hostEpoch=0,clientEpoch=0,mapId=0,hostArm=0,clientArm=0,guardKey=0,event=0;
    float hitOffset[3]{},direction[3]{};
    std::uint64_t reservedTail[4]{};
};
#pragma pack(pop)
static_assert(sizeof(CoopSwordPacket)==160);
inline bool swordSameBinding(const CoopSwordPacket& a,const CoopSwordPacket& b){
    return a.hostArm&&a.clientArm&&a.limit==b.limit&&a.hostArm==b.hostArm&&a.clientArm==b.clientArm&&a.hostSession==b.hostSession&&a.clientSession==b.clientSession&&
        a.hostEpoch==b.hostEpoch&&a.clientEpoch==b.clientEpoch&&a.mapId==b.mapId&&a.guardKey==b.guardKey;
}
inline bool validCoopSword(const CoopSwordPacket& p,const CoopPosePacket& local,const CoopPosePacket& peer){
    const auto& h=local.role?peer:local;const auto& c=local.role?local:peer;
    if(p.magic!=0x44433244||p.version!=2||p.size!=sizeof(p)||p.build!=0x67a50cde||p.role!=peer.role||(p.limit!=1&&p.limit!=12)||!p.sequence||
       !coopWorldPairReady(local,peer)||p.hostSession!=h.session||p.clientSession!=c.session||p.hostEpoch!=h.worldEpoch||p.clientEpoch!=c.worldEpoch||
       p.mapId!=h.mapId||p.mapId!=coopMapId(kGuardMap)||p.guardKey!=swordGuardKey()||static_cast<unsigned>(p.kind)<1||static_cast<unsigned>(p.kind)>4)return false;
    for(auto v:p.reservedTail)if(v)return false;
    if(p.kind==SwordMessage::Arm){if(p.role?(!p.clientArm||p.hostArm):(!p.hostArm||p.clientArm))return false;}
    else if(!p.hostArm||!p.clientArm)return false;
    if(p.kind==SwordMessage::Hit){
        if(p.role||!p.event||p.event>p.limit)return false;
        float norm=0;for(float v:p.direction){if(!std::isfinite(v)||std::fabs(v)>1.01f)return false;norm+=v*v;}
        if(norm<.90f||norm>1.10f)return false;
        for(float v:p.hitOffset)if(!std::isfinite(v)||std::fabs(v)>3)return false;
    }else{
        if(p.event)return false;for(float v:p.hitOffset)if(v!=0)return false;for(float v:p.direction)if(v!=0)return false;
    }
    return true;
}
inline bool swordConsumeHit(bool active,std::uint64_t& consumed,const CoopSwordPacket& key,const CoopSwordPacket& p){
    if(!active||consumed>=key.limit||p.kind!=SwordMessage::Hit||p.role||p.event!=consumed+1||!swordSameBinding(key,p))return false;
    consumed=p.event;return true;
}
inline bool swordPlayerHealthReady(float current,float expected,bool cumulative){
    // Native player recovery may increase health between spaced sword hits.
    // A decrease not accounted for by our last native hit is another damage
    // source and ends this isolated experiment. No health comes from packets.
    return std::isfinite(current)&&std::isfinite(expected)&&current>0&&current<=56&&expected>0&&expected<=56&&
        (cumulative?current>=expected:current==expected);
}
