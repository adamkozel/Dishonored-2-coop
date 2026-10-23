#pragma once
#include "world_protocol.h"
// 1214jf (#185): Unlock is either player's key unlock, applied once in the other world.
enum class DoorMessage:std::uint32_t {Hello=0,Request=1,State=2,Ack=3,Unlock=4};
#pragma pack(push,1)
struct CoopDoorPacket {
    std::uint32_t magic=0x4f443244;
    std::uint16_t version=1,size=sizeof(CoopDoorPacket);
    std::uint32_t build=0x67a50cde,role=0;
    DoorMessage kind=DoorMessage::Hello;std::uint32_t reserved=0;
    std::uint64_t sequence=0,sourceSession=0,targetSession=0,sourceEpoch=0,targetEpoch=0,mapId=0,sourceArm=0,targetArm=0,revision=0,requestAck=0;
    std::uint32_t target=0,side=0;
    char name[192]{},definition[192]{};
};
#pragma pack(pop)
static_assert(sizeof(CoopDoorPacket)==496);
inline bool validCoopDoor(const CoopDoorPacket& p,const CoopPosePacket& local,const CoopPosePacket& peer){
    if(p.magic!=0x4f443244||p.version!=1||p.size!=sizeof(p)||p.build!=0x67a50cde||p.role>1||p.role!=peer.role||
       p.reserved||!p.sequence||!p.sourceArm||!coopWorldPairReady(local,peer)||p.sourceSession!=peer.session||
       p.targetSession!=local.session||p.sourceEpoch!=peer.worldEpoch||p.targetEpoch!=local.worldEpoch||
       p.mapId!=local.mapId||!coopWorldName(p.name)||!coopWorldName(p.definition))return false;
    if(p.kind==DoorMessage::Hello)return !p.targetArm&&!p.revision&&!p.requestAck&&!p.target&&!p.side;
    // An unlock comes from either role and carries no desired state.
    if(p.kind==DoorMessage::Unlock)return p.targetArm&&!p.revision&&!p.requestAck&&!p.target&&!p.side;
    if(!p.targetArm||!p.revision||(p.target!=0&&p.target!=2)||p.side>1)return false;
    return (p.kind==DoorMessage::State&&p.role==0)||((p.kind==DoorMessage::Request||p.kind==DoorMessage::Ack)&&p.role==1&&!p.requestAck);
}
// The host orders desired states, never toggles. Revision is authoritative;
// request sequence and arm/session replay protection belong to the envelope.
struct DoorOrder {
    std::uint64_t revision=0;unsigned target=0,side=0;
    bool set(unsigned t,unsigned s){
        if((t!=0&&t!=2)||s>1||revision==UINT64_MAX)return false;
        if(revision&&target==t&&(t==0||side==s))return false;
        ++revision;target=t;side=s;return true;
    }
    bool accept(std::uint64_t r,unsigned t,unsigned s){
        if(!r||r<=revision||(t!=0&&t!=2)||s>1)return false;
        revision=r;target=t;side=s;return true;
    }
};
