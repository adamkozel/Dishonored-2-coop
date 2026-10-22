#pragma once
#include <cstdint>
#include <cstring>
enum class CheckpointMessage:std::uint32_t {Hello=1,Request,Prepare,Accept,Begin,Ready,Release,Complete,Abort,Death,DeathReady};
enum class CheckpointPhase:std::uint32_t {Idle,Requested,Preparing,Accepted,Loading,Held,Released,Failed};
#pragma pack(push,1)
struct CheckpointPacket {
    std::uint32_t magic=0x52433244;
    std::uint16_t version=2,size=sizeof(CheckpointPacket);
    std::uint32_t build=0x67a50cde,role=0;
    CheckpointMessage kind=CheckpointMessage::Hello;
    std::uint32_t character=0;
    std::uint64_t sequence=0,hostSession=0,clientSession=0,hostArm=0,clientArm=0,transaction=0;
    std::uint64_t hostEpoch=0,clientEpoch=0,mapId=0,hostReady=0,clientReady=0;
    unsigned char digest[32]{};
};
#pragma pack(pop)
static_assert(sizeof(CheckpointPacket)==144);
inline bool checkpointPacketShape(const CheckpointPacket& p,unsigned receiver) {
    if(p.magic!=0x52433244||p.version!=2||p.size!=sizeof(p)||p.build!=0x67a50cde||p.role>1||p.role==receiver||
       !p.sequence||!p.hostSession||!p.clientSession||!p.mapId||p.character>2)return false;
    const auto k=static_cast<unsigned>(p.kind);if(k<1||k>11)return false;
    unsigned char any=0;for(auto b:p.digest)any|=b;if(!any)return false;
    if((p.kind==CheckpointMessage::Request||p.kind==CheckpointMessage::Accept)&&p.role!=1)return false;
    if((p.kind==CheckpointMessage::Prepare||p.kind==CheckpointMessage::Begin||p.kind==CheckpointMessage::Release)&&p.role)return false;
    if(p.kind==CheckpointMessage::Hello)return (p.role?p.clientArm:p.hostArm)&&!p.transaction&&!p.hostEpoch&&!p.clientEpoch&&!p.hostReady&&!p.clientReady&&!p.character;
    if(!p.hostArm||!p.clientArm||!p.hostEpoch||!p.clientEpoch)return false;
    if(p.kind==CheckpointMessage::Death||p.kind==CheckpointMessage::DeathReady)return !p.transaction&&!p.hostReady&&!p.clientReady&&
        (p.kind==CheckpointMessage::DeathReady?(p.character==1||p.character==2):!p.character); // 1214hy (#159): any pairing
    if(p.kind==CheckpointMessage::Request)return !p.transaction&&!p.hostReady&&!p.clientReady&&!p.character;
    if(!p.transaction)return false;
    if(p.kind==CheckpointMessage::Ready)return (p.character==1||p.character==2)&&(p.role?p.clientReady:p.hostReady)&&!(p.role?p.hostReady:p.clientReady);
    if(p.character)return false;
    if(p.kind==CheckpointMessage::Release||p.kind==CheckpointMessage::Complete)return p.hostReady&&p.clientReady;
    return !p.hostReady&&!p.clientReady;
}
inline bool checkpointSameSession(const CheckpointPacket& a,const CheckpointPacket& b) {
    return a.hostSession==b.hostSession&&a.clientSession==b.clientSession&&a.hostArm==b.hostArm&&a.clientArm==b.clientArm&&
        a.mapId==b.mapId&&!std::memcmp(a.digest,b.digest,32);
}
inline bool checkpointSameAttempt(const CheckpointPacket& a,const CheckpointPacket& b) {
    return a.transaction&&a.transaction==b.transaction&&a.hostEpoch==b.hostEpoch&&a.clientEpoch==b.clientEpoch&&checkpointSameSession(a,b);
}
inline bool checkpointSameWorld(const CheckpointPacket& a,const CheckpointPacket& b) {
    return a.hostEpoch&&a.clientEpoch&&a.hostEpoch==b.hostEpoch&&a.clientEpoch==b.clientEpoch&&checkpointSameSession(a,b);
}
inline bool checkpointPhaseAccepts(CheckpointPhase phase,CheckpointMessage kind,unsigned role) {
    if(kind==CheckpointMessage::Abort)return phase!=CheckpointPhase::Idle&&phase!=CheckpointPhase::Failed;
    if(!role)return (phase==CheckpointPhase::Preparing&&kind==CheckpointMessage::Accept)||
        ((phase==CheckpointPhase::Loading||phase==CheckpointPhase::Held)&&kind==CheckpointMessage::Ready)||
        (phase==CheckpointPhase::Released&&kind==CheckpointMessage::Complete);
    return ((phase==CheckpointPhase::Idle||phase==CheckpointPhase::Requested)&&kind==CheckpointMessage::Prepare)||
        (phase==CheckpointPhase::Accepted&&kind==CheckpointMessage::Begin)||
        ((phase==CheckpointPhase::Loading||phase==CheckpointPhase::Held)&&kind==CheckpointMessage::Ready)||
        (phase==CheckpointPhase::Held&&kind==CheckpointMessage::Release)||
        (phase==CheckpointPhase::Released&&kind==CheckpointMessage::Complete);
}
