#pragma once
#include "arm_protocol.h"
#include "combat_presentation.h"
#include "traversal_presentation.h"
#include "power_presentation.h"
#include "ranged_presentation.h"
#include "carry_presentation.h"
#include "grenade_presentation.h"
#include "heart_presentation.h"
#include "trap_presentation.h"
#include "choke_presentation.h"
#include "stun_presentation.h"
#include "use_presentation.h"
#pragma pack(push,1)
// Native semantic states and explicit unbound ground presentation18. No pointers, resource names or damage requests.
struct CoopShadowActionPacket {
    std::uint32_t magic=0x48413244;
    std::uint16_t version=77,size=sizeof(CoopShadowActionPacket);
    std::uint32_t build=0x67a50cde,role=0;
    std::uint64_t sequence=0,sourceSession=0,targetSession=0;
    std::uint64_t sourceWorldEpoch=0,targetWorldEpoch=0,mapId=0;
    std::uint64_t sourceToken=0,targetToken=0,event=0;
    std::uint32_t rootSequence=0,character=0,kind=0,state=0; // 0 ground, 1 slash, 2 block, 3 airborne, 4 sprint, 5 lean, 6 slide
    float leanSide=0,leanUp=0;
    float airStartZ=0,airTargetZ=0;std::uint32_t airStage=0,airRemaining=0;
    std::uint64_t sampleTimeUs=0;
    std::uint32_t combatVariant=0,combatPhase=0,combatTimeMs=0,combatDurationMs=0;
    std::uint32_t swordState=0,equipmentVariant=0,equipmentTimeMs=0,equipmentDurationMs=0;
    std::uint32_t traversalVariant=0,traversalTimeMs=0,traversalDurationMs=0,traversalEndStance=0;
    std::array<float,3> traversalStart{},traversalGoal{};
    std::array<float,3> traversalPoint{};float traversalHeight=0;
    std::uint32_t powerVariant=0,powerTimeMs=0,powerDurationMs=0;
    std::uint32_t rangedKind=0,rangedVariant=0,rangedTimeMs=0,rangedDurationMs=0,rangedMechanism=0,rangedShot=0;
    std::uint32_t carryVariant=0,carryTimeMs=0,carryDurationMs=0,carryStance=0,carryIncarnation=0;
    std::uint64_t carrySpawner=0,carryDefinition=0;
    std::array<float,3> carryThrowVelocity{};
    std::uint32_t grenadeHeld=0,grenadeVariant=0,grenadeTimeMs=0,grenadeDurationMs=0;
    std::uint32_t trapHeld=0,trapVariant=0,trapTimeMs=0,trapDurationMs=0,trapStock=0;
    std::uint32_t chokeVariant=0,chokeTimeMs=0,chokeDurationMs=0;
    std::uint64_t chokeSpawner=0,chokeDefinition=0;
    std::uint32_t heartHeld=0,heartVariant=0,heartTimeMs=0,heartDurationMs=0;
    std::uint32_t stunVariant=0,stunTimeMs=0,stunDurationMs=0;
    // 1213 (protocol 74): native use (player state 75) -> authored interaction.
    std::uint32_t useVariant=0,useTimeMs=0,useDurationMs=0,useEvent=0;
    // 1213k (protocol 75, #6): the used mechanism's world position for the partner's hand reach.
    std::array<float,3> useTarget{};std::uint32_t useTargetValid=0;
};
#pragma pack(pop)
static_assert(sizeof(CoopShadowActionPacket)==432);
// Share equipment-context policy between source and wire validation so an
// observed native reader cannot silently outgrow the source transport gate.
inline bool shadowPowerSwordContext(unsigned variant,unsigned swordState){
    // Native successful host entry holsters the sword before release400ms.
    if(variant==28||(variant>=32&&variant<=36))return swordState==1;
    if(variant>=29&&variant<=31)return swordState==1||swordState==2;
    return variant>=1&&variant<=27&&variant!=13&&(swordState==2||(swordState==1&&variant>=10&&variant<=25));
}
inline bool validShadowAction(const CoopShadowActionPacket& p,const CoopPosePacket& local,const CoopPosePacket& peer){
    const bool traversal=TraversalPresentation::valid({0,1,p.traversalVariant,p.traversalTimeMs,p.traversalDurationMs,p.traversalEndStance,p.traversalStart,p.traversalGoal,p.traversalPoint,p.traversalHeight})&&
        (p.state==8?(p.traversalVariant==1||p.traversalVariant==2||p.traversalVariant==4||p.traversalVariant==5):p.state==9?p.traversalVariant==3:p.state==11?p.traversalVariant==6:p.state==12?(p.traversalVariant>=7&&p.traversalVariant<=16&&p.swordState==1):p.traversalVariant==0);
    const bool air=std::isfinite(p.airStartZ)&&std::isfinite(p.airTargetZ)&&std::fabs(p.airStartZ)<=100000.f&&std::fabs(p.airTargetZ)<=100000.f&&p.airRemaining<=3&&
        (p.state==3||(p.state==10&&(p.powerVariant==10||p.powerVariant==11||p.powerVariant==12||p.powerVariant==14||((p.powerVariant>=16&&p.powerVariant<=18||p.powerVariant>=29&&p.powerVariant<=31)&&p.swordState>=1&&p.swordState<=2))&&p.airStage)||(p.state==13&&p.rangedKind&&p.airStage)||(p.state==15&&p.grenadeVariant>=3&&p.airStage)?(p.airStage==1?(p.airTargetZ-p.airStartZ>=.05f&&p.airTargetZ-p.airStartZ<=10.f):
            (p.airStage==2&&!p.airStartZ&&!p.airTargetZ&&!p.airRemaining)):(!p.airStage&&!p.airStartZ&&!p.airTargetZ&&!p.airRemaining));
    // 19/20 native block impact (533 ms) and 21 defender timed parry (500 ms)
    // occur only inside the block state; exact single-phase durations.
    const bool defenseImpact=p.combatVariant>=19&&p.combatVariant<=21;
    // 1214cv (protocol 76): a death collapse belongs only to the dead state (25), single phase, its exact duration.
    const bool deathCombat=CombatPresentation::death(p.combatVariant);
    // 1214cz (protocol 77): a recoil stagger rides the slash state, single phase, at a native recoil duration.
    const bool staggerCombat=CombatPresentation::stagger(p.combatVariant);
    const bool combat=deathCombat?(p.state==25&&p.combatPhase==1&&p.combatDurationMs==CombatPresentation::deathDuration(p.combatVariant)&&p.combatTimeMs<=p.combatDurationMs):
        staggerCombat?(p.state==1&&p.combatPhase==1&&(p.combatDurationMs==666||p.combatDurationMs==733)&&p.combatTimeMs<=p.combatDurationMs):
        p.combatVariant<=23&&p.state!=25&&(!defenseImpact||((p.state==2||p.state==3)&&p.combatPhase==1&&p.combatDurationMs==(p.combatVariant==21?500u:533u)&&p.combatTimeMs<=p.combatDurationMs))&&
        (p.combatVariant<15||defenseImpact||(p.combatVariant>=22&&p.state==1&&p.combatPhase==1&&p.combatDurationMs==733&&p.combatTimeMs<=733)||(p.combatVariant<22&&p.state==1&&p.combatPhase==1&&
        (p.combatVariant==18?(p.combatDurationMs==833||p.combatDurationMs==1000):p.combatDurationMs==(p.combatVariant==17?800u:p.combatVariant==15?866u:666u))&&p.combatTimeMs<=p.combatDurationMs))&&(p.combatVariant?
        ((p.state==1||p.state==2||p.state==3)&&p.combatPhase>=1&&p.combatPhase<=2&&p.combatDurationMs&&p.combatDurationMs<=10000&&p.combatTimeMs<=3600000&&
         (p.combatPhase!=2||p.combatVariant==7||p.combatVariant==11)&&
         (p.state==3||(p.state==1)==(p.combatVariant<=9||(p.combatVariant>=15&&p.combatVariant<=18)||p.combatVariant>=22))):(!p.combatPhase&&!p.combatDurationMs&&!p.combatTimeMs&&p.state!=1&&p.state!=2));
    // Swimming carries no action scene of its own: every scene/action field
    // stays empty; the independent root channel owns position and depth.
    const bool swim=(p.state!=22&&p.state!=23)||(!p.combatVariant&&!p.powerVariant&&!p.traversalVariant&&!p.rangedKind&&!p.grenadeHeld&&!p.grenadeVariant&&!p.carryVariant&&
        !p.chokeVariant&&!p.trapHeld&&!p.trapVariant&&!p.heartHeld&&!p.heartVariant&&!p.stunVariant&&!p.airStage&&!p.equipmentVariant&&!p.leanSide&&!p.leanUp);
    const bool possessionDraw=p.kind==1&&p.state==10&&p.role==0&&p.character==1&&p.swordState==2&&
        (p.powerVariant==26||p.powerVariant==27)&&p.equipmentTimeMs<=p.equipmentDurationMs&&
        ((p.equipmentVariant==1&&p.equipmentDurationMs==1066)||(p.equipmentVariant==3&&p.equipmentDurationMs==1133));
    const bool equipment=(p.equipmentVariant<7||(p.role==1&&p.character==2&&p.equipmentDurationMs==333&&p.equipmentTimeMs<=333))&&(p.kind==0?p.swordState==0:p.swordState>=1&&p.swordState<=2)&&p.equipmentVariant<=8&&
        (p.equipmentVariant?((p.state==7||possessionDraw)&&p.equipmentDurationMs&&p.equipmentDurationMs<=10000&&p.equipmentTimeMs<=3600000):
            (p.state!=7&&!p.equipmentTimeMs&&!p.equipmentDurationMs))&&(!p.combatVariant||p.swordState==2||deathCombat);
    const bool power=PowerPresentation::valid({0,1,p.powerVariant,p.powerTimeMs,p.powerDurationMs})&&
        (p.powerVariant?(p.state==10&&shadowPowerSwordContext(p.powerVariant,p.swordState)&&(p.powerVariant==10||(p.character==1?(p.powerVariant<=3||(p.powerVariant>=11&&p.powerVariant<=15)||(p.powerVariant>=23&&p.powerVariant<=28)):((p.powerVariant>=4&&p.powerVariant<=9)||(p.powerVariant>=16&&p.powerVariant<=22)||(p.powerVariant>=29&&p.powerVariant<=36))))):p.state!=10);
    const bool ranged=RangedPresentation::valid({0,1,p.rangedKind,p.rangedVariant,p.rangedTimeMs,p.rangedDurationMs,p.rangedMechanism,p.rangedShot})&&
        (p.rangedVariant?p.state==13:p.state!=13)&&(!p.rangedKind||p.kind==1)&&
        (!p.rangedKind||(p.state==13||((p.state==0||p.state==3)&&p.swordState==2&&!p.combatVariant)))&&
        (!p.airStage||p.state!=13||(p.rangedVariant==1||p.rangedVariant==2||p.rangedVariant==3||p.rangedVariant==4||p.rangedVariant==7||p.rangedVariant==8));
    const bool carry=CarryPresentation::valid({0,1,p.carryVariant,p.carryTimeMs,p.carryDurationMs,p.carryStance,p.carryIncarnation,p.carrySpawner,p.carryDefinition,p.carryThrowVelocity})&&
        (p.carryVariant?(p.kind==1&&p.state==14&&(p.swordState==1||p.carryVariant==7)):p.state!=14);
    const bool grenade=GrenadePresentation::valid({0,1,p.grenadeHeld,p.grenadeVariant,p.grenadeTimeMs,p.grenadeDurationMs})&&
        (p.grenadeVariant?p.state==15:p.state!=15)&&(!p.grenadeHeld||(p.kind==1&&(p.state==0||p.state==3||p.state==15)&&!p.rangedKind&&!p.powerVariant&&!p.carryVariant&&!p.combatVariant&&(!p.airStage||p.state==3||p.grenadeVariant>=3)));
    const bool stun=StunPresentation::valid({0,1,p.stunVariant,p.stunTimeMs,p.stunDurationMs})&&
        (p.stunVariant?(p.kind==1&&p.state==16&&!p.grenadeHeld&&!p.rangedKind&&!p.powerVariant&&!p.carryVariant&&!p.combatVariant&&!p.airStage):p.state!=16);
    const bool possession=p.state!=17||(p.kind==1&&p.character==1&&p.role==0&&p.swordState==1&&!p.powerVariant&&!p.airStage&&!p.rangedKind&&!p.grenadeHeld&&!p.carryVariant&&!p.stunVariant&&!p.combatVariant&&!p.equipmentVariant);
    // Hold (73): an unclassified native state keeps the stream alive and
    // presents nothing; it can never carry any action, scene or held item.
    const bool hold=p.state!=24||(p.kind==1&&(p.swordState==1||p.swordState==2)&&!p.combatVariant&&!p.combatPhase&&!p.equipmentVariant&&!p.traversalVariant&&
        !p.powerVariant&&!p.rangedKind&&!p.rangedVariant&&!p.grenadeHeld&&!p.grenadeVariant&&!p.carryVariant&&!p.stunVariant&&!p.airStage&&
        !p.heartHeld&&!p.heartVariant&&!p.trapHeld&&!p.trapVariant&&!p.chokeVariant&&!p.leanSide&&!p.leanUp);
    // Unbound presentation can never carry a gesture, target, effect or air event.
    const bool unbound=p.state!=18||(p.kind==1&&!p.combatVariant&&!p.equipmentVariant&&!p.traversalVariant&&
        !p.powerVariant&&!p.rangedKind&&!p.grenadeHeld&&!p.carryVariant&&!p.stunVariant&&!p.airStage);
    const bool heart=HeartPresentation::valid({0,1,p.heartHeld,p.heartVariant,p.heartTimeMs,p.heartDurationMs})&&
        (p.heartVariant?p.state==19:p.state!=19)&&(!p.heartHeld||(p.kind==1&&(p.state==0||p.state==19)&&
        !p.rangedKind&&!p.grenadeHeld&&!p.powerVariant&&!p.combatVariant&&!p.carryVariant&&!p.stunVariant&&!p.airStage));
    const bool trap=TrapPresentation::valid({0,1,p.trapHeld,p.trapVariant,p.trapTimeMs,p.trapDurationMs,p.trapStock})&&
        (p.trapVariant?p.state==20:p.state!=20)&&(!p.trapHeld||(p.kind==1&&(p.state==0||p.state==20)&&
        !p.heartHeld&&!p.rangedKind&&!p.grenadeHeld&&!p.powerVariant&&!p.combatVariant&&!p.carryVariant&&!p.stunVariant&&!p.airStage));
    const bool choke=ChokePresentation::valid({0,1,p.chokeVariant,p.chokeTimeMs,p.chokeDurationMs,p.chokeSpawner,p.chokeDefinition})&&
        (p.chokeVariant?(p.kind==1&&p.state==21&&p.swordState==(nativeTakedownSword(p.chokeVariant)?2u:1u)&&!p.trapHeld&&!p.heartHeld&&!p.grenadeHeld&&!p.rangedKind&&!p.carryVariant&&!p.powerVariant&&!p.stunVariant&&!p.combatVariant&&!p.equipmentVariant&&!p.airStage):p.state!=21);
    // 1213: a native use runs only on the ground action (state 0) and never
    // alongside another scene presentation.
    const bool use=p.useTargetValid<=1&&UsePresentation::valid({1.,p.useEvent,p.useVariant,p.useTimeMs,p.useDurationMs,{p.useTarget[0],p.useTarget[1],p.useTarget[2]},p.useTargetValid==1})&&
        (!p.useVariant||(p.state==0&&!p.combatVariant&&!p.powerVariant&&!p.traversalVariant&&!p.rangedKind&&!p.grenadeHeld&&!p.carryVariant&&!p.chokeVariant&&!p.trapHeld&&!p.heartHeld&&!p.stunVariant));
    // 1214cv: native death (player state 15) -> state 25 with one directional collapse and no other channel.
    const bool dead=p.state!=25||(p.kind==1&&deathCombat&&!p.equipmentVariant&&!p.traversalVariant&&!p.powerVariant&&!p.rangedKind&&!p.rangedVariant&&
        !p.grenadeHeld&&!p.grenadeVariant&&!p.carryVariant&&!p.stunVariant&&!p.airStage&&!p.heartHeld&&!p.heartVariant&&!p.trapHeld&&!p.trapVariant&&
        !p.chokeVariant&&!p.useVariant&&!p.leanSide&&!p.leanUp);
    return dead&&use&&hold&&swim&&choke&&trap&&heart&&unbound&&possession&&stun&&grenade&&carry&&ranged&&power&&traversal&&equipment&&combat&&p.sampleTimeUs&&p.sampleTimeUs<=9007199254740991ull&&air&&p.magic==0x48413244&&p.version==77&&p.size==sizeof(p)&&p.build==0x67a50cde&&p.role<=1&&p.role==peer.role&&
        p.sequence&&p.sourceToken&&p.character==p.role+1&&p.kind<=1&&p.state<=25&&
        std::isfinite(p.leanSide)&&std::isfinite(p.leanUp)&&std::fabs(p.leanSide)<=1.f&&p.leanUp>=0&&p.leanUp<=1.f&&
        (p.state==5||(!p.leanSide&&!p.leanUp))&&
        (p.kind==0?(p.state==0&&!p.event):(p.targetToken&&p.event))&&coopPresentationPairReady(local,peer)&&
        p.sourceSession==peer.session&&p.targetSession==local.session&&p.sourceWorldEpoch==peer.worldEpoch&&
        p.targetWorldEpoch==local.worldEpoch&&p.mapId==local.mapId&&armRootFresh(p.rootSequence,peer.sequence);
}
inline bool shadowActionTokensMatch(const CoopShadowActionPacket& p,std::uint64_t token,std::uint64_t peerToken){
    return token&&peerToken&&p.sourceToken==peerToken&&p.targetToken==token;
}
inline bool shadowActionEventFresh(const CoopShadowActionPacket& p,std::uint64_t sequence,std::uint64_t event,unsigned state){
    return p.sequence>sequence&&p.event>=event&&(p.event!=event||p.state==state);
}
