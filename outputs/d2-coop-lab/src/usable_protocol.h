#pragma once
#include "world_protocol.h"

enum class UsableMessage:std::uint32_t {Hello=0,Request=1,State=2,Ack=3};
#pragma pack(push,1)
struct CoopUsablePacket {
    std::uint32_t magic=0x53553244;
    std::uint16_t version=1,size=sizeof(CoopUsablePacket);
    std::uint32_t build=0x67a50cde,role=0;
    UsableMessage kind=UsableMessage::Hello;std::uint32_t reserved=0;
    std::uint64_t sequence=0,sourceSession=0,targetSession=0,sourceEpoch=0,targetEpoch=0,mapId=0,sourceArm=0,targetArm=0,revision=0,requestAck=0;
    std::uint32_t target=0,stateCount=0;std::uint64_t rulesHash=0;
    char name[192]{},definition[192]{};
};
#pragma pack(pop)
static_assert(sizeof(CoopUsablePacket)==504);
inline bool validCoopUsable(const CoopUsablePacket& p,const CoopPosePacket& local,const CoopPosePacket& peer){
    if(p.magic!=0x53553244||p.version!=1||p.size!=sizeof(p)||p.build!=0x67a50cde||p.role>1||p.role!=peer.role||
       p.reserved||!p.sequence||!p.sourceArm||!coopWorldPairReady(local,peer)||p.sourceSession!=peer.session||
       p.targetSession!=local.session||p.sourceEpoch!=peer.worldEpoch||p.targetEpoch!=local.worldEpoch||
       p.mapId!=local.mapId||!coopWorldName(p.name)||!coopWorldName(p.definition)||
       !p.rulesHash||!p.stateCount||p.stateCount>32||p.target>=p.stateCount)return false;
    if(p.kind==UsableMessage::Hello)return !p.targetArm&&!p.revision&&!p.requestAck&&!p.target;
    if(!p.targetArm||!p.revision)return false;
    return (p.kind==UsableMessage::State&&p.role==0)||((p.kind==UsableMessage::Request||p.kind==UsableMessage::Ack)&&p.role==1&&!p.requestAck);
}
// Only the host creates revisions; targets are indices, never toggles.
struct UsableOrder {
    std::uint64_t revision=0;unsigned target=0;
    bool set(unsigned t){
        if(t>=32||revision==UINT64_MAX||(revision&&target==t))return false;
        ++revision;target=t;return true;
    }
    bool accept(std::uint64_t r,unsigned t){
        if(!r||r<=revision||t>=32)return false;
        revision=r;target=t;return true;
    }
};
