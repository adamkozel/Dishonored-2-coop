#pragma once
#include "npc_attack_protocol.h"
void npcAttackProtocolTests(){
    CoopPosePacket h{},c{};h.role=0;c.role=1;h.session=11;c.session=22;h.worldEpoch=3;c.worldEpoch=5;h.mapId=c.mapId=9;
    h.flags=c.flags=1;h.gameLogicState=c.gameLogicState=1;h.cinematicMetadataValid=c.cinematicMetadataValid=1;
    CoopNpcAttack p{};auto& b=p.binding;b.kind=NpcAuthorityMessage::Stop;b.size=sizeof(CoopNpcControl);b.sequence=1;
    b.sourceSession=11;b.targetSession=22;b.sourceEpoch=3;b.targetEpoch=5;b.map=9;b.sourceArm=7;b.targetArm=8;b.sourceHandle=0x10003;b.targetHandle=0x50009;b.rig=456;b.joints=223;
    p.sourceNonce=99;require(validNpcAttack(p,c,h),"native enemy handshake uses separately bound local handles");
    p.kind=NpcAttackMessage::Hit;p.targetNonce=88;p.event=1;p.offset[2]=1;p.direction[1]=-1;
    std::strcpy(p.damage,"components/equipment/damage/npc_sword.damage");p.flags=kNpcAttackMelee;
    require(validNpcAttack(p,c,h),"host native sword contact accepted without remote health or damage amount");
    for(unsigned i=0;i<17;++i){auto q=p;switch(i){
        case 0:q.binding.role=1;break;case 1:++q.binding.sourceEpoch;break;case 2:++q.binding.targetEpoch;break;
        case 3:++q.binding.sourceSession;break;case 4:++q.binding.targetSession;break;case 5:++q.binding.map;break;
        case 6:q.targetNonce=0;break;case 7:q.sourceNonce=0;break;case 8:q.event=0;break;
        case 9:q.offset[0]=NAN;break;case 10:q.offset[2]=3.1f;break;case 11:q.direction[1]=0;break;
        case 12:q.direction[0]=INFINITY;break;case 13:q.flags=8;break;case 14:q.version=2;break;
        case 15:--q.size;break;case 16:q.kind=static_cast<NpcAttackMessage>(20);break;
    }require(!validNpcAttack(q,c,h),"malformed or stale enemy contact refused");}
    auto paused=c;paused.gameLogicState=2;require(!validNpcAttack(p,paused,h),"native menu cannot accept enemy-hit application");
    auto dead=c;dead.gameLogicState=8;require(!validNpcAttack(p,dead,h),"death state cannot accept enemy-hit application");
    auto ack=p;ack.kind=NpcAttackMessage::Ack;ack.binding.role=1;
    std::swap(ack.binding.sourceSession,ack.binding.targetSession);std::swap(ack.binding.sourceEpoch,ack.binding.targetEpoch);
    std::swap(ack.binding.sourceArm,ack.binding.targetArm);std::swap(ack.binding.sourceHandle,ack.binding.targetHandle);
    for(unsigned i=0;i<3;++i)ack.offset[i]=ack.direction[i]=0;ack.flags=0;
    {auto q=ack;require(!validNpcAttack(q,h,c),"ack cannot carry a damage declaration");}
    std::memset(ack.damage,0,sizeof(ack.damage));
    require(validNpcAttack(ack,h,c),"client acknowledges a consumed native contact");
    {auto q=p;q.flags=kNpcAttackBlockable|kNpcAttackHeavy|kNpcAttackMelee;require(validNpcAttack(q,c,h),"host carries native blockable and heavy attack bits");}
    {auto q=p;q.flags=kNpcAttackBlockable;require(!validNpcAttack(q,c,h),"block bits only on a melee weapon contact");}
    // 1214kg (#219): enemy declarations by name; never a player, base or code-referenced one, never malformed.
    for(const char* ok:{"components/equipment/damage/bullet_regular_npc.damage","components/equipment/damage/npc_sword_mech.damage",
                        "components/characters/npc/witch/damage_shriek.damage","components/characters/animals/rat/damage_periodic.damage"}){
        auto q=p;q.flags=0;std::memset(q.damage,0,sizeof(q.damage));std::strcpy(q.damage,ok);require(validNpcAttack(q,c,h),"enemy damage declaration accepted by name");}
    for(const char* bad:{"","components/characters/player/base/damage/assassination_damage.damage","components/base/damage/fall_damage.damage",
                         "code_referenced/damage/generic_kill_damage.damage","components/equipment/damage/../base/kill.damage",
                         "components/equipment/damage/NPC_SWORD.damage","components/equipment/damage/npc_sword","components/equipment/damage//x.damage"}){
        auto q=p;std::memset(q.damage,0,sizeof(q.damage));std::strcpy(q.damage,bad);require(!validNpcAttack(q,c,h),"player, base, malformed or empty declaration refused");}
    {auto q=p;std::memset(q.damage,'a',sizeof(q.damage));require(!validNpcAttack(q,c,h),"unterminated declaration refused");}
    {auto q=p;q.damage[sizeof(q.damage)-1]='x';require(!validNpcAttack(q,c,h),"bytes after the declaration name refused");}
    for(unsigned outcome=1;outcome<=3;++outcome){auto q=ack;q.flags=outcome;require(validNpcAttack(q,h,c),"client reports native block, parry-lose or parry-win outcome");}
    {auto q=ack;q.flags=4;require(!validNpcAttack(q,h,c),"unknown native defense outcome refused");}
    {auto q=p;q.kind=NpcAttackMessage::Hello;q.targetNonce=0;q.event=0;for(unsigned i=0;i<3;++i)q.offset[i]=q.direction[i]=0;q.flags=1;std::memset(q.damage,0,sizeof(q.damage));
     require(!validNpcAttack(q,c,h),"handshake cannot carry attack or defense bits");}
    auto terminal=ack;terminal.kind=NpcAttackMessage::Terminal;
    require(validNpcAttack(terminal,h,c),"client terminal outcome names the consumed native contact");
    {auto q=terminal;q.flags=1;require(!validNpcAttack(q,h,c),"terminal outcome cannot carry defense bits");}
    terminal.event=0;require(!validNpcAttack(terminal,h,c),"unbound terminal outcome refused");
    terminal=ack;terminal.kind=NpcAttackMessage::Terminal;terminal.direction[0]=1;
    require(!validNpcAttack(terminal,h,c),"terminal outcome cannot carry extra damage geometry");
    terminal=p;terminal.kind=NpcAttackMessage::Terminal;for(unsigned i=0;i<3;++i)terminal.offset[i]=terminal.direction[i]=0;
    require(!validNpcAttack(terminal,c,h),"host cannot report remote-player terminal outcome");
    ack.offset[0]=.1f;require(!validNpcAttack(ack,h,c),"ack cannot carry gameplay geometry");
}
