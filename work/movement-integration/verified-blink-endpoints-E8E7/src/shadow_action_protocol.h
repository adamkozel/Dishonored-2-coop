#pragma once
#include "arm_protocol.h"
#include "traversal_presentation.h"
#include "power_presentation.h"
#pragma pack(push,1)
// Native semantic states only. No pointers, resource names or damage requests.
struct CoopShadowActionPacket {
    std::uint32_t magic=0x48413244;
    std::uint16_t version=20,size=sizeof(CoopShadowActionPacket);
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
};
#pragma pack(pop)
static_assert(sizeof(CoopShadowActionPacket)==236);
inline bool validShadowAction(const CoopShadowActionPacket& p,const CoopPosePacket& local,const CoopPosePacket& peer){
    const bool traversal=TraversalPresentation::valid({0,1,p.traversalVariant,p.traversalTimeMs,p.traversalDurationMs,p.traversalEndStance,p.traversalStart,p.traversalGoal,p.traversalPoint,p.traversalHeight})&&
        (p.state==8?(p.traversalVariant==1||p.traversalVariant==2||p.traversalVariant==4||p.traversalVariant==5):p.state==9?p.traversalVariant==3:p.state==11?p.traversalVariant==6:p.state==12?(p.traversalVariant>=7&&p.traversalVariant<=16&&p.swordState==1):p.traversalVariant==0);
    const bool air=std::isfinite(p.airStartZ)&&std::isfinite(p.airTargetZ)&&std::fabs(p.airStartZ)<=100000.f&&std::fabs(p.airTargetZ)<=100000.f&&p.airRemaining<=3&&
        (p.state==3?(p.airStage==1?(p.airTargetZ-p.airStartZ>=.05f&&p.airTargetZ-p.airStartZ<=10.f):
            (p.airStage==2&&!p.airStartZ&&!p.airTargetZ&&!p.airRemaining)):(!p.airStage&&!p.airStartZ&&!p.airTargetZ&&!p.airRemaining));
    const bool combat=p.combatVariant<=14&&(p.combatVariant?
        ((p.state==1||p.state==2||p.state==3)&&p.combatPhase>=1&&p.combatPhase<=2&&p.combatDurationMs&&p.combatDurationMs<=10000&&p.combatTimeMs<=3600000&&
         (p.combatPhase!=2||p.combatVariant==7||p.combatVariant==11)&&
         (p.state==3||(p.state==1)==(p.combatVariant<=9))):(!p.combatPhase&&!p.combatDurationMs&&!p.combatTimeMs&&p.state!=1&&p.state!=2));
    const bool equipment=(p.kind==0?p.swordState==0:p.swordState>=1&&p.swordState<=2)&&p.equipmentVariant<=6&&
        (p.equipmentVariant?(p.state==7&&p.equipmentDurationMs&&p.equipmentDurationMs<=10000&&p.equipmentTimeMs<=3600000):
            (p.state!=7&&!p.equipmentTimeMs&&!p.equipmentDurationMs))&&(!p.combatVariant||p.swordState==2);
    const bool power=PowerPresentation::valid({0,1,p.powerVariant,p.powerTimeMs,p.powerDurationMs})&&
        (p.powerVariant?(p.state==10&&(p.swordState==2||(p.powerVariant==10&&p.swordState==1))&&(p.powerVariant==10||(p.character==1?p.powerVariant<=3:p.powerVariant>=4&&p.powerVariant<=9))):p.state!=10);
    return power&&traversal&&equipment&&combat&&p.sampleTimeUs&&p.sampleTimeUs<=9007199254740991ull&&air&&p.magic==0x48413244&&p.version==20&&p.size==sizeof(p)&&p.build==0x67a50cde&&p.role<=1&&p.role==peer.role&&
        p.sequence&&p.sourceToken&&p.character==p.role+1&&p.kind<=1&&p.state<=12&&
        std::isfinite(p.leanSide)&&std::isfinite(p.leanUp)&&std::fabs(p.leanSide)<=1.f&&p.leanUp>=0&&p.leanUp<=1.f&&
        (p.state==5||(!p.leanSide&&!p.leanUp))&&
        (p.kind==0?(p.state==0&&!p.event):(p.targetToken&&p.event))&&coopWorldPairReady(local,peer)&&
        p.sourceSession==peer.session&&p.targetSession==local.session&&p.sourceWorldEpoch==peer.worldEpoch&&
        p.targetWorldEpoch==local.worldEpoch&&p.mapId==local.mapId&&armRootFresh(p.rootSequence,peer.sequence);
}
inline bool shadowActionTokensMatch(const CoopShadowActionPacket& p,std::uint64_t token,std::uint64_t peerToken){
    return token&&peerToken&&p.sourceToken==peerToken&&p.targetToken==token;
}
inline bool shadowActionEventFresh(const CoopShadowActionPacket& p,std::uint64_t sequence,std::uint64_t event,unsigned state){
    return p.sequence>sequence&&p.event>=event&&(p.event!=event||p.state==state);
}
