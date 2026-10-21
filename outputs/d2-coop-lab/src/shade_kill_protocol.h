#pragma once
// 1214gv (#131): the host-to-partner shade kill message (native_shade_host.h sends and applies it).
#pragma pack(push,1)
struct CoopShadeKillPacket {
    std::uint32_t magic=0x4b533244;std::uint16_t version=1,size=0;
    std::uint64_t sequence=0,sourceSession=0,targetSession=0,sourceWorldEpoch=0,targetWorldEpoch=0,mapId=0,event=0,declaration=0;
    std::uint32_t slot=0,clone=0,attacker=0,inflictor=0;float amount=0;
};
#pragma pack(pop)
std::uint64_t shadeHostHitCount=0,shadeKillsSent=0,shadeKillsReceived=0,shadeKillsApplied=0,shadeKillsRefused=0,shadeKillSequence=0;
const char* shadeKillResult="idle";CoopShadeKillPacket shadeKillPending{};bool shadeKillPendingValid=false;
void receiveShadeKill(const CoopShadeKillPacket& q){
    ++shadeKillsReceived;
    if(localRole!=1||q.magic!=0x4b533244||q.version!=1||q.size!=sizeof(q)||q.slot>=kDoppelgangerWireSlots||!q.event||!q.clone||
       q.sourceSession!=remote.session||q.targetSession!=outgoing.session||q.sourceWorldEpoch!=remote.worldEpoch||q.targetWorldEpoch!=outgoing.worldEpoch||q.mapId!=outgoing.mapId){
        ++shadeKillsRefused;shadeKillResult="kill_invalid";return;}
    shadeKillPending=q;shadeKillPendingValid=true;
}
