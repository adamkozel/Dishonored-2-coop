#pragma once
#include "native_player_action.h"
#include "native_player_sword.h"
#include "native_presentation_fallback.h"
#include "native_player_combat_scene.h"
#include "combat_presentation.h"
#include "movement_semantics.h"
#include "native_player_equipment_scene.h"
#include "equipment_presentation.h"
#include "traversal_presentation.h"
#include "native_player_landing_scene.h"
#include "native_player_keyhole_scene.h"
#include "native_player_power_scene.h"
#include "native_player_domino_scene.h"
#include "native_player_swarm_scene.h"
#include "native_player_doppelganger_scene.h"
#include "native_player_shadowwalk_scene.h"
#include "native_player_possession_scene.h"
#include "native_player_possession_occupation.h"
#include "native_player_ranged_scene.h"
#include "native_player_grenade_scene.h"
#include "native_player_heart_scene.h"
#include "native_player_stun_scene.h"
#include "native_player_carry_scene.h"
#include "carry_identity.h"
#include "ranged_shot_observation.h"
RangedShotObservation nativeRangedShot;
#include "native_crossbow_presentation.h"
void stopShadowEquipment(const char* reason);
bool movementOwnsAirborne();
bool movementOwnsCombat();
// Bounded 15-second probe or explicit continuous opt-in for this incarnation.
// Native Steam transports semantic state;
// the receiver uses native clone clips, never native damage/AI attack calls.
struct ShadowStreamState {
    // Diagnostic history survives brief native loop incarnations which a
    // slower file observer can miss. Reset with this stream's token/world.
    // It observes successful sends / fully accepted states; never drives them.
    struct PowerEventHistory {
        struct Entry {std::uint64_t event=0;unsigned variant=0,duration=0,scene=0;};
        std::array<Entry,32> entries{};unsigned count=0;
        void observe(std::uint64_t event,unsigned variant,unsigned duration,unsigned scene=0){
            if(!variant||(count&&entries[count-1].event==event))return;
            if(count==entries.size()){std::move(entries.begin()+1,entries.end(),entries.begin());--count;}
            entries[count++]={event,variant,duration,scene};
        }
    } powerSentHistory,powerReceivedHistory;
    struct PendingAction {const char* alias=nullptr;double due=0;};
    PendingAction pending[8]{};unsigned pendingCount=0;
    ULONGLONG until=0,lastHello=0,lastSend=0,lastPeer=0,lastState=0;
    std::uintptr_t game=0,actor=0;std::uint32_t handle=0;
    std::uint64_t token=0,peerToken=0,epoch=0,peerEpoch=0,peerSession=0,lastSequence=0,lastEvent=0,sourceEvent=0;
    unsigned sourceState=~0u,remoteState=0;
    unsigned sourceScene=0,sourceVariant=0,sourcePhase=0,sourceTraversal=0,remoteVariant=0,remotePhase=0,remoteDuration=0;
    CombatPresentation combat;EquipmentPresentation equipment;TraversalPresentation traversal;PowerPresentation power;RangedPresentation ranged;MovementSemantics movement;
    NativeKeyholeHandoff keyholeHandoff;
    CarryPresentation carry;GrenadePresentation grenade;HeartPresentation heart;StunPresentation stun;
    unsigned sourceHeartHeld=0,sourceHeart=0,remoteHeartHeld=0,remoteHeart=0,remoteHeartDuration=0;
    unsigned sourceStun=0,remoteStun=0,remoteStunDuration=0;
    unsigned sourceGrenadeHeld=0,sourceGrenade=0,sourceGrenadeTime=0,remoteGrenadeHeld=0,remoteGrenade=0,remoteGrenadeDuration=0;
    CarryPreparationClock carryPreparationClock;
    unsigned sourceCarry=0,sourceCarryIncarnation=0,remoteCarry=0,remoteCarryDuration=0,remoteCarryIncarnation=0,remoteCarryLocalHandle=0;
    std::uint64_t sourceCarrySpawner=0,sourceCarryDefinition=0,remoteCarrySpawner=0,remoteCarryDefinition=0;
    unsigned sourcePower=0,remotePower=0,remotePowerDuration=0;
    unsigned sourceRangedKind=0,sourceRanged=0,remoteRangedKind=0,remoteRanged=0,remoteRangedDuration=0;
    unsigned sourceSword=0,sourceEquipment=0,remoteSword=0,remoteEquipment=0,remoteEquipmentDuration=0;
    float remoteLeanSide=0,remoteLeanUp=0;
    float jumpStartZ=0,jumpTargetZ=0;unsigned airStage=0,airRemaining=0;bool jumpTargetValid=false;
    bool peerReady=false,ownsAction=false,persistent=false;
};
ShadowStreamState shadowStream{};
std::uint64_t shadowStreamNonce=0,shadowStreamSequence=0,shadowStreamSent=0,shadowStreamReceived=0,shadowStreamAccepted=0,
    shadowStreamRejected=0,shadowStreamRefused=0,shadowStreamStarted=0,shadowStreamApplied=0,shadowStreamReleases=0;
CoopShadowActionPacket shadowStreamLastPacket{};
unsigned shadowStreamStateRejectReason=0;double shadowStreamRejectedAge=0;
std::uint64_t shadowStreamRejectedEvent=0,shadowStreamExpectedEvent=0,shadowStreamRejectedSequence=0,shadowStreamExpectedSequence=0;
unsigned shadowStreamRejectedState=0,shadowStreamExpectedState=0;
unsigned shadowStreamRejectReason=0,shadowStreamRetireReason=0;ULONGLONG shadowStreamRetireRootAge=0,shadowStreamRetireHelloAge=0;
std::uint64_t shadowSourceAttempts=0,shadowSourceMisses=0,shadowWireRefused=0;
unsigned shadowSourceReadStage=0,shadowSourceLastFailure=0;
std::uint64_t shadowUnboundSamples=0,shadowUnboundRefused=0;
ULONGLONG shadowUnboundProbeUntil=0; // Explicit bounded missing-binding test, off by default.
std::array<std::uint64_t,8> shadowSourceFailureCounts{};
double shadowSourceReadMaxMs=0;ULONGLONG shadowSendGapMax=0,shadowPreviousSendService=0,shadowStoppedStateAge=0,shadowStoppedRootAge=0,shadowStopServiceTick=0,shadowStopLastState=0;
// Read-only profiling on the native engine thread; reset with stream startup.
const char* shadowReadTimingNames[]={"states","stun","power","ranged","grenadeScene","carry","carryPreparation","sword","heldRanged","heldGrenade","muzzle"};
std::array<double,11> shadowReadTimingMax{};
std::array<std::uint64_t,11> shadowReadTimingCount{};
template<class Read> bool shadowTimedRead(unsigned stage,Read read){
    const auto start=presentationClock();const bool valid=read();
    shadowReadTimingMax[stage]=(std::max)(shadowReadTimingMax[stage],presentationClock()-start);
    ++shadowReadTimingCount[stage];return valid;
}
const char* shadowStreamResult="idle";
bool shadowStreamPairReady(ULONGLONG tick){
    return GetCurrentThreadId()==engineThread.load()&&probeFollowExact&&armProbePairReady(tick)&&avatarAiHeld&&
        avatarAiTarget.actor==probeActor&&locomotionEnabled&&coopGameplayReady(outgoing)&&coopGameplayReady(remote);
}
bool shadowStreamCurrent(ULONGLONG tick){
    return shadowStream.until>tick&&shadowStreamPairReady(tick)&&shadowStream.game==observedGame&&
        shadowStream.actor==probeActor&&shadowStream.handle==probeHandle&&shadowStream.epoch==outgoing.worldEpoch&&
        shadowStream.peerEpoch==remote.worldEpoch&&shadowStream.peerSession==remote.session;
}
bool shadowSourceAction(unsigned& action,float* leanSide=nullptr,float* leanUp=nullptr,CoopShadowActionPacket* airPacket=nullptr,unsigned* sceneHandle=nullptr){
    shadowSourceReadStage=1;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);NativePlayerStates states{};NativePlayerSword sword{};
    NativePlayerPowerScene power{};NativePlayerStunScene stun{};
    const bool nativeStateValid=shadowTimedRead(0,[&]{return readNativePlayerStates(memoryRead,base,states);})&&states.player.game==observedGame&&states.player.player==observedPlayer;
    const bool stunValid=nativeStateValid&&shadowTimedRead(1,[&]{return readNativePlayerStunScene(memoryRead,base,states,stun);});
    const bool powerValid=nativeStateValid&&!stunValid&&shadowTimedRead(2,[&]{return (readNativePlayerPowerScene(memoryRead,base,states,power,true)||readNativePlayerDominoScene(memoryRead,base,states,power)||readNativePlayerDoppelgangerScene(memoryRead,base,states,power)||readNativePlayerSwarmScene(memoryRead,base,states,power)||readNativePlayerPossessionScene(memoryRead,base,states,power,true)||readNativePlayerShadowWalkScene(memoryRead,base,states,power));});
    const bool possessionOccupied=nativeStateValid&&!stunValid&&!powerValid&&readNativeHumanPossessionOccupation(memoryRead,base,states);
    NativePlayerRangedScene ranged{};
    const bool rangedValid=nativeStateValid&&!powerValid&&shadowTimedRead(3,[&]{return readNativePlayerRangedScene(memoryRead,base,states,ranged);});
    NativePlayerGrenadeScene grenade{};
    const bool grenadeValid=nativeStateValid&&!powerValid&&!rangedValid&&shadowTimedRead(4,[&]{return readNativePlayerGrenadeScene(memoryRead,base,states,grenade);});
    NativePlayerHeartScene heart{};NativePlayerHeart heldHeart{};
    const bool heartValid=nativeStateValid&&!powerValid&&!rangedValid&&!grenadeValid&&readNativePlayerHeartScene(memoryRead,base,states,heart);
    const bool heartIdle=nativeStateValid&&!heartValid&&nativePlayerGroundHeartStates(states,base)&&states.active.size()==2&&
        readNativePlayerHeart(memoryRead,base,heldHeart,nullptr,true)&&heldHeart.quantity==1;
    NativePlayerCarryScene carry{};
    const bool carryValid=nativeStateValid&&!powerValid&&!rangedValid&&!grenadeValid&&shadowTimedRead(5,[&]{return readNativePlayerCarryScene(memoryRead,base,states,carry,true);});
    NativePlayerCarryPreparation carryPreparation{};
    const bool carryPreparing=nativeStateValid&&!powerValid&&!rangedValid&&!grenadeValid&&!carryValid&&
        shadowTimedRead(6,[&]{return readNativePlayerCarryPreparation(memoryRead,base,states,carryPreparation);});
    if(!carryPreparing)shadowStream.carryPreparationClock.clear();
    if(heartValid)action=19;else if(heartIdle)action=0;else if(possessionOccupied)action=17;else if(stunValid)action=16;else if(powerValid)action=10;else if(rangedValid)action=13;else if(grenadeValid)action=15;else if(carryValid||carryPreparing)action=14;
    shadowSourceReadStage=2;
    const bool valid=nativeStateValid&&(heartValid||heartIdle||possessionOccupied||stunValid||powerValid||rangedValid||grenadeValid||carryValid||carryPreparing||nativePlayerShadowAction(states,base,action))&&
        shadowTimedRead(7,[&]{return readNativePlayerSword(memoryRead,base,sword);})&&sword.character==localRole+1&&sword.sword.game==states.player.game&&
        sword.sword.actor==states.player.player&&sword.sword.logic==1&&sword.sword.quantity==1&&sword.sword.equipped==sword.sword.started;
    float side=0,up=0;
    if(!valid||(action==5&&!nativePlayerLeanAmount(memoryRead,states,base,side,up)))return false;
    shadowSourceReadStage=3;
    if(stunValid){
        if(sceneHandle)*sceneHandle=stun.handle;
        if(airPacket){airPacket->stunVariant=stun.variant;airPacket->stunTimeMs=stun.timeMs;airPacket->stunDurationMs=stun.durationMs;}
    }
    if(carryValid||carryPreparing){
        NativeWorldNpc native{},target{};const auto tick=GetTickCount64();
        const auto corpse=carryValid?carry.corpse:carryPreparation.corpse;
        const auto corpseHandle=carryValid?carry.corpseHandle:carryPreparation.corpseHandle;
        if((carryValid&&(sword.sword.equipped||carry.character!=localRole+1))||!worldObservationReady(tick)||!worldCatalogTick||tick-worldCatalogTick>750||
           !readWorldNpcIdentity(memoryRead,base,observedGame,corpseHandle,native)||native.actor!=corpse||!native.unique||
           !resolveCarryIdentity(memoryRead,base,observedGame,worldCatalog,carryIdentityKey(native.spawnerName),carryIdentityKey(native.definition),target)||target.actor!=corpse)return false;
        if(carryValid&&sceneHandle)*sceneHandle=carry.handle;
        if(airPacket){airPacket->carryVariant=carryValid?carry.variant:7;
            airPacket->carryTimeMs=carryValid?carry.timeMs:shadowStream.carryPreparationClock.observe(tick,carryPreparation.statePointer,corpseHandle);
            airPacket->carryDurationMs=carryValid?carry.durationMs:CarryPresentation::duration(7);
            airPacket->carryStance=states.player.current;airPacket->carryIncarnation=corpseHandle;
            airPacket->carrySpawner=carryIdentityKey(target.spawnerName);airPacket->carryDefinition=carryIdentityKey(target.definition);
            if(carryValid)airPacket->carryThrowVelocity=carry.throwVelocity;}
    }
    if(rangedValid){
        if(ranged.weapon.player.character!=localRole+1)return false;
        if(sceneHandle)*sceneHandle=ranged.handle;
        if(airPacket){airPacket->rangedKind=ranged.weapon.kind;airPacket->rangedVariant=ranged.variant;airPacket->rangedTimeMs=ranged.timeMs;airPacket->rangedDurationMs=ranged.durationMs;
            NativeCrossbowPresentation mechanical{};if(readNativeCrossbowPresentation(memoryRead,base,ranged.weapon,mechanical))airPacket->rangedMechanism=mechanical.state;}
    }else if((action==0||action==3)&&states.active.size()==1&&sword.sword.equipped&&
        ((states.player.current==0&&states.active[0].id==0&&states.active[0].vtable==base+0x1c85628)||
         (states.player.current==1&&states.active[0].id==6&&states.active[0].vtable==base+0x1c84258)||
         (action==3&&nativePlayerAirRangedStates(states,base)))){
        NativePlayerRangedItem held{};
        if(shadowTimedRead(8,[&]{return readNativePlayerRangedItem(memoryRead,base,1,held,true)||readNativePlayerRangedItem(memoryRead,base,2,held,true);}))
            if(airPacket){airPacket->rangedKind=held.kind;NativeCrossbowPresentation mechanical{};
                if(readNativeCrossbowPresentation(memoryRead,base,held,mechanical))airPacket->rangedMechanism=mechanical.state;}
    }
    if(heartValid||heartIdle){
        const auto& item=heartValid?heart.heart:heldHeart;
        if(item.player.character!=localRole+1||item.player.sword.game!=observedGame||item.player.sword.actor!=observedPlayer)return false;
        if(heartValid&&sceneHandle)*sceneHandle=heart.handle;
        if(airPacket){airPacket->heartHeld=1;airPacket->heartVariant=heart.variant;airPacket->heartTimeMs=heart.timeMs;airPacket->heartDurationMs=heart.durationMs;}
    }
    if(grenadeValid){
        if(grenade.grenade.player.character!=localRole+1)return false;
        if(sceneHandle)*sceneHandle=grenade.handle;
        if(airPacket){airPacket->grenadeHeld=1;airPacket->grenadeVariant=grenade.variant;airPacket->grenadeTimeMs=grenade.timeMs;airPacket->grenadeDurationMs=grenade.durationMs;}
    }else if(((action==0&&nativePlayerGroundIdleStates(states,base))||(action==3&&nativePlayerAirIdleStates(states,base)))&&sword.sword.equipped){
        NativePlayerGrenade held{};
        if(shadowTimedRead(9,[&]{return readNativePlayerGrenade(memoryRead,base,held,nullptr,true);})&&held.player.character==localRole+1&&held.equipped&&held.started&&held.quantity>0&&airPacket)airPacket->grenadeHeld=1;
    }
    if(powerValid){
        shadowSourceReadStage=4;
        if(!shadowPowerSwordContext(power.variant,sword.sword.equipped?2u:1u))return false;
        if(sceneHandle)*sceneHandle=power.handle;
        if(airPacket){airPacket->powerVariant=power.variant;airPacket->powerTimeMs=power.timeMs;airPacket->powerDurationMs=power.durationMs;}
        if(airPacket&&power.swordDraw.variant){airPacket->equipmentVariant=power.swordDraw.variant;
            airPacket->equipmentTimeMs=power.swordDraw.timeMs;airPacket->equipmentDurationMs=power.swordDraw.durationMs;}
    }
    if(airPacket){airPacket->swordState=sword.sword.equipped?2u:1u;
        for(const auto& a:states.active)if(a.id==54&&a.vtable==base+0x1c78a68)airPacket->swordState=1; // Draw request precedes its contact scene.
    }
    if(action==7){NativePlayerEquipmentScene equipment{};
        shadowSourceReadStage=5;
        if(!readNativePlayerEquipmentScene(memoryRead,base,states,equipment)||equipment.character!=localRole+1)return false;
        if(sceneHandle)*sceneHandle=equipment.handle;
        if(airPacket){airPacket->equipmentVariant=equipment.variant;airPacket->equipmentTimeMs=equipment.timeMs;airPacket->equipmentDurationMs=equipment.durationMs;}
    }
    shadowSourceReadStage=6;
    if(action==12){NativePlayerKeyholeScene keyhole{};
        if(sword.sword.equipped)return false;
        if(readNativePlayerKeyholeScene(memoryRead,base,states,keyhole))shadowStream.keyholeHandoff.observe(states,keyhole,GetTickCount64());
        else if(!readNativeKeyholeHandoff(memoryRead,base,states,shadowStream.keyholeHandoff,GetTickCount64(),keyhole)&&
                !readNativeKeyholeHandoff(memoryRead,base,states,shadowStream.keyholeHandoff,GetTickCount64(),keyhole,true))return false;
        if(keyhole.character!=localRole+1)return false;
        if(sceneHandle)*sceneHandle=keyhole.handle;
        if(airPacket){airPacket->traversalVariant=keyhole.variant;airPacket->traversalTimeMs=keyhole.timeMs;airPacket->traversalDurationMs=keyhole.durationMs;airPacket->traversalEndStance=keyhole.endStance;}
    }
    else shadowStream.keyholeHandoff={};
    if(action==11){NativePlayerLandingScene landing{};
        if(!readNativePlayerLandingScene(memoryRead,base,states,landing)||landing.character!=localRole+1)return false;
        if(sceneHandle)*sceneHandle=landing.handle;
        if(airPacket){airPacket->traversalVariant=6;airPacket->traversalTimeMs=landing.timeMs;airPacket->traversalDurationMs=landing.durationMs;airPacket->traversalEndStance=landing.endStance;}
    }
    if(action==8){NativePlayerTraversalScene traversal{};
        if(!readNativePlayerTraversalScene(memoryRead,base,states,traversal))return false;
        if(sceneHandle)*sceneHandle=traversal.handle;
        if(airPacket){airPacket->traversalVariant=traversal.variant;airPacket->traversalTimeMs=traversal.timeMs;airPacket->traversalDurationMs=traversal.durationMs;
            airPacket->traversalEndStance=traversal.endStance;airPacket->traversalStart=traversal.start;airPacket->traversalGoal=traversal.goal;}
    }
    if(action==9){NativePlayerVault vault{};
        if(!readNativePlayerVault(memoryRead,base,states,vault)||vault.character!=localRole+1)return false;
        if(sceneHandle)*sceneHandle=vault.handle;
        if(airPacket){airPacket->traversalVariant=3;airPacket->traversalTimeMs=unsigned(std::lround(vault.progress*1000));airPacket->traversalDurationMs=1000;
            airPacket->traversalStart=vault.start;airPacket->traversalGoal=vault.goal;airPacket->traversalPoint=vault.point;airPacket->traversalHeight=vault.height;}
    }
    unsigned swordAction=0;nativePlayerSwordAction(states,base,swordAction);
    if(swordAction){if(!sword.sword.equipped)return false;NativePlayerCombatScene combat{};
        if(!readNativePlayerCombatScene(memoryRead,base,states,combat)||combat.character!=localRole+1)return false;
        if(sceneHandle)*sceneHandle=combat.handle;
        if(airPacket){airPacket->combatVariant=combat.variant;airPacket->combatPhase=combat.subscene=="Loop"||combat.variant==11?2u:1u;
            airPacket->combatTimeMs=combat.timeMs;airPacket->combatDurationMs=combat.durationMs;}}
    const bool powerAir=powerValid&&(power.variant==10||power.variant==11||power.variant==12||power.variant==14||(power.variant>=16&&power.variant<=18)||(power.variant>=29&&power.variant<=31))&&std::any_of(states.active.begin(),states.active.end(),[](const auto& a){return a.id==2||a.id==3;});
    const bool rangedAir=rangedValid&&nativePlayerAirRangedStates(states,base);
    const bool grenadeAir=grenadeValid&&nativePlayerAirGrenadeStates(states,base);
    if(action==3||powerAir||rangedAir||grenadeAir){unsigned stage=0,remaining=0;float startZ=0,targetZ=0;if(!nativePlayerAir(memoryRead,states,base,stage,startZ,targetZ,remaining,powerAir,rangedAir,grenadeAir))return false;
        if(airPacket){airPacket->airStage=stage;airPacket->airRemaining=remaining;airPacket->airStartZ=startZ;airPacket->airTargetZ=targetZ;}}
    if(leanSide)*leanSide=side;if(leanUp)*leanUp=up;shadowSourceReadStage=0;return true;
}
void stopShadowStream(const char* reason){
    if(!shadowStream.until)return;
    const auto stopped=GetTickCount64();shadowStoppedStateAge=shadowStream.lastState?stopped-shadowStream.lastState:0;shadowStoppedRootAge=lastReceive?stopped-lastReceive:0;
    shadowUnboundProbeUntil=0;shadowStream.until=0;shadowStream.peerReady=false;shadowStream.pendingCount=0;shadowStream.keyholeHandoff={};shadowStream.combat.clear();shadowStream.equipment.clear();shadowStream.traversal.clear();shadowStream.power.clear();shadowStream.ranged.clear();shadowStream.carry.clear();shadowStream.grenade.clear();shadowStream.heart.clear();shadowStream.stun.clear();shadowStream.remoteCarryLocalHandle=0;shadowStream.movement.clear();stopShadowEquipment(reason);shadowStreamResult=reason;shadowLocoUntil=0;
    if(shadowStream.ownsAction){stopShadowAction(reason);shadowStream.ownsAction=false;++shadowStreamReleases;}
}
void beginShadowStream(bool persistent=false){
    const auto tick=GetTickCount64();unsigned action=0;
    if((bodyStreamEnabled&&!mixamoBodyCompatible())||shadowStream.until||shadowAction.actor||!shadowStreamPairReady(tick)||!shadowSourceAction(action)){
        ++shadowStreamRefused;shadowStreamResult="unavailable_or_busy";return;
    }
    NativeSwordEquipment s{};NativeHealth h{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(!readNativeSwordEquipment(memoryRead,base,probeHandle,s,localRole?NativeSwordScope::DoppelgangerCorvo:NativeSwordScope::DoppelgangerEmily)||s.actor!=probeActor){
        ++shadowStreamRefused;shadowStreamResult="clone_equipment_unreadable";return;
    }
    // Whole-body presentation already supports both stable native inventory
    // states. Requiring a drawn sword here prevents a fresh stream handshake
    // after an interruption leaves the live player/clone holstered. Continue
    // to reject queued transitions or an attachment on the wrong joint.
    if(s.equipped!=s.started||s.attachedTag!=(s.equipped?s.drawTag:s.holsterTag)){
        ++shadowStreamRefused;shadowStreamResult="clone_equipment_transition";return;
    }
    // The scoped native clone may have taken nonlethal world damage. Full
    // health is not an identity proof and must not prevent a new handshake.
    // Preserve its generation/owner and expected health declaration checks;
    // never restart a dead or suppressed presentation, or write its health.
    if(!readNativeHealth(memoryRead,base,probeHandle,h)||h.actor!=probeActor||h.game!=observedGame||
       h.baseMaximum!=40||h.maximum!=40||(h.ownerFlags&0x20)||h.current<=0||h.current>h.maximum){
        ++shadowStreamRefused;shadowStreamResult="clone_health_unavailable";return;
    }
    shadowStream={};shadowStream.persistent=persistent;shadowStream.until=tick+15000;shadowStream.token=++shadowStreamNonce;shadowStream.game=observedGame;
    shadowSourceAttempts=shadowSourceMisses=shadowWireRefused=0;shadowSourceReadMaxMs=0;shadowReadTimingMax.fill(0);shadowReadTimingCount.fill(0);shadowSendGapMax=shadowPreviousSendService=shadowStoppedStateAge=shadowStoppedRootAge=shadowStopServiceTick=shadowStopLastState=0;
    shadowSourceReadStage=shadowSourceLastFailure=0;shadowSourceFailureCounts={};shadowUnboundSamples=shadowUnboundRefused=0;
    shadowStream.actor=probeActor;shadowStream.handle=probeHandle;shadowStream.epoch=outgoing.worldEpoch;
    shadowStream.peerEpoch=remote.worldEpoch;shadowStream.peerSession=remote.session;
    ++shadowStreamStarted;shadowStreamResult="awaiting_peer";
}
CoopShadowActionPacket shadowStreamPacket(unsigned kind){
    CoopShadowActionPacket p{};p.sequence=++shadowStreamSequence;p.role=localRole;p.character=localRole+1;p.kind=kind;
    p.sampleTimeUs=static_cast<std::uint64_t>(presentationClock()*1000.);
    p.sourceSession=outgoing.session;p.targetSession=remote.session;p.sourceWorldEpoch=outgoing.worldEpoch;p.targetWorldEpoch=remote.worldEpoch;
    p.mapId=outgoing.mapId;p.rootSequence=outgoing.sequence;p.sourceToken=shadowStream.token;p.targetToken=shadowStream.peerToken;return p;
}
void sendShadowStream(ULONGLONG tick){
    if(!nativeMutationAllowed||!shadowStreamCurrent(tick))return;
    if(shadowPreviousSendService)shadowSendGapMax=(std::max)(shadowSendGapMax,tick-shadowPreviousSendService);shadowPreviousSendService=tick;
    if(tick-shadowStream.lastHello>=100){
        shadowStream.lastHello=tick;const auto p=shadowStreamPacket(0);
        if(validShadowAction(p,remote,outgoing)&&sockets->SendMessageToConnection(connection,&p,sizeof(p),k_nSteamNetworkingSend_UnreliableNoNagle,nullptr)==k_EResultOK)++shadowStreamSent;
    }
    if(!shadowStream.peerReady||tick-shadowStream.lastSend<33)return;
    shadowStream.lastSend=tick;unsigned action=0;float leanSide=0,leanUp=0;auto p=shadowStreamPacket(1);
    unsigned scene=0;
    ++shadowSourceAttempts;const auto readStart=presentationClock();bool sourceValid=shadowSourceAction(action,&leanSide,&leanUp,&p,&scene);
    // Fault injection suppresses only this cosmetic equipment binding; the
    // actual native action, controls, root and inventory remain untouched.
    if(sourceValid&&action==7&&tick<shadowUnboundProbeUntil){sourceValid=false;shadowSourceReadStage=5;}
    shadowSourceReadMaxMs=(std::max)(shadowSourceReadMaxMs,presentationClock()-readStart);
    if(!sourceValid){++shadowSourceMisses;shadowSourceLastFailure=shadowSourceReadStage;
        ++shadowSourceFailureCounts[(std::min)(7u,shadowSourceReadStage)];
        unsigned swordState=0;
        if(!readNativePresentationFallback(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,observedPlayer,localRole+1,swordState)){
            ++shadowUnboundRefused;return;
        }
        // Discard every partially populated action field. Only independently
        // revalidated equipment and ground locomotion cross this path.
        p=shadowStreamPacket(1);p.swordState=swordState;action=18;scene=0;leanSide=leanUp=0;++shadowUnboundSamples;
    }
    if(p.heartHeld!=shadowStream.sourceHeartHeld||p.heartVariant!=shadowStream.sourceHeart||action!=shadowStream.sourceState||scene!=shadowStream.sourceScene||p.stunVariant!=shadowStream.sourceStun||p.combatVariant!=shadowStream.sourceVariant||p.combatPhase!=shadowStream.sourcePhase||p.swordState!=shadowStream.sourceSword||p.equipmentVariant!=shadowStream.sourceEquipment||p.powerVariant!=shadowStream.sourcePower||p.traversalVariant!=shadowStream.sourceTraversal||p.rangedKind!=shadowStream.sourceRangedKind||p.rangedVariant!=shadowStream.sourceRanged||p.carryVariant!=shadowStream.sourceCarry||p.carryIncarnation!=shadowStream.sourceCarryIncarnation||p.carrySpawner!=shadowStream.sourceCarrySpawner||p.carryDefinition!=shadowStream.sourceCarryDefinition||p.grenadeHeld!=shadowStream.sourceGrenadeHeld||p.grenadeVariant!=shadowStream.sourceGrenade||(p.grenadeVariant==4&&p.grenadeTimeMs<shadowStream.sourceGrenadeTime)){
        shadowStream.sourceHeartHeld=p.heartHeld;shadowStream.sourceHeart=p.heartVariant;shadowStream.sourceState=action;shadowStream.sourceScene=scene;shadowStream.sourceStun=p.stunVariant;shadowStream.sourceVariant=p.combatVariant;shadowStream.sourcePhase=p.combatPhase;shadowStream.sourceSword=p.swordState;shadowStream.sourceEquipment=p.equipmentVariant;shadowStream.sourcePower=p.powerVariant;shadowStream.sourceTraversal=p.traversalVariant;shadowStream.sourceRangedKind=p.rangedKind;shadowStream.sourceRanged=p.rangedVariant;shadowStream.sourceCarry=p.carryVariant;shadowStream.sourceCarryIncarnation=p.carryIncarnation;shadowStream.sourceCarrySpawner=p.carrySpawner;shadowStream.sourceCarryDefinition=p.carryDefinition;shadowStream.sourceGrenadeHeld=p.grenadeHeld;shadowStream.sourceGrenade=p.grenadeVariant;++shadowStream.sourceEvent;}
    shadowStream.sourceGrenadeTime=p.grenadeTimeMs;
    p.state=action;p.event=shadowStream.sourceEvent;p.leanSide=leanSide;p.leanUp=leanUp;
    std::vector<NativeWindblastFxRecord> muzzleRecords;
    if(shadowTimedRead(10,[&]{return readNativeRangedFxRecords(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,muzzleRecords);}))
        p.rangedShot=nativeRangedShot.observe(shadowStream.token,shadowStream.peerToken,p.event,localRole,
            action==13&&p.rangedKind==1&&p.rangedVariant==3&&p.rangedTimeMs<=666,outgoing.position,muzzleRecords);
    if(validShadowAction(p,remote,outgoing)&&sockets->SendMessageToConnection(connection,&p,sizeof(p),k_nSteamNetworkingSend_UnreliableNoNagle,nullptr)==k_EResultOK){
        ++shadowStreamSent;shadowStreamLastPacket=p;
        shadowStream.powerSentHistory.observe(p.event,p.powerVariant,p.powerDurationMs,scene);
    }else ++shadowWireRefused;
}
void duplicateShadowStream(){
    if(shadowStreamCurrent(GetTickCount64())&&shadowStreamLastPacket.sequence&&validShadowAction(shadowStreamLastPacket,remote,outgoing))
        sockets->SendMessageToConnection(connection,&shadowStreamLastPacket,sizeof(shadowStreamLastPacket),k_nSteamNetworkingSend_ReliableNoNagle,nullptr);
}
void receiveShadowStream(const CoopShadowActionPacket& p,ULONGLONG tick){
    ++shadowStreamReceived;
    const unsigned reject=!nativeMutationAllowed?1:!shadowStreamCurrent(tick)?2:!validShadowAction(p,outgoing,remote)?3:p.sequence<=shadowStream.lastSequence?4:
        (shadowStream.peerToken&&p.sourceToken!=shadowStream.peerToken)?5:(p.targetToken&&p.targetToken!=shadowStream.token)?6:0;
    if(reject){shadowStreamRejectReason=reject;if(p.kind==1&&reject!=2)shadowStreamStateRejectReason=reject;++shadowStreamRejected;return;}
    if(p.kind==0){
        shadowStream.peerToken=p.sourceToken;shadowStream.lastSequence=p.sequence;shadowStream.lastPeer=tick;
        if(p.targetToken==shadowStream.token){shadowStream.peerReady=true;shadowStreamResult="ready";}return;
    }
    if(!shadowStream.peerReady||!shadowActionTokensMatch(p,shadowStream.token,shadowStream.peerToken)||
       !shadowActionEventFresh(p,shadowStream.lastSequence,shadowStream.lastEvent,shadowStream.remoteState)){
        shadowStreamStateRejectReason=shadowStreamRejectReason=!shadowStream.peerReady?7:!shadowActionTokensMatch(p,shadowStream.token,shadowStream.peerToken)?20:21;
        shadowStreamRejectedEvent=p.event;shadowStreamExpectedEvent=shadowStream.lastEvent;shadowStreamRejectedSequence=p.sequence;shadowStreamExpectedSequence=shadowStream.lastSequence;shadowStreamRejectedState=p.state;shadowStreamExpectedState=shadowStream.remoteState;
        ++shadowStreamRejected;return;
    }
    if(p.event==shadowStream.lastEvent&&(p.heartHeld!=shadowStream.remoteHeartHeld||p.heartVariant!=shadowStream.remoteHeart||p.heartDurationMs!=shadowStream.remoteHeartDuration||p.stunVariant!=shadowStream.remoteStun||p.stunDurationMs!=shadowStream.remoteStunDuration||p.combatVariant!=shadowStream.remoteVariant||p.combatPhase!=shadowStream.remotePhase||p.combatDurationMs!=shadowStream.remoteDuration||p.swordState!=shadowStream.remoteSword||p.equipmentVariant!=shadowStream.remoteEquipment||p.equipmentDurationMs!=shadowStream.remoteEquipmentDuration||p.powerVariant!=shadowStream.remotePower||p.powerDurationMs!=shadowStream.remotePowerDuration||p.rangedKind!=shadowStream.remoteRangedKind||p.rangedVariant!=shadowStream.remoteRanged||p.rangedDurationMs!=shadowStream.remoteRangedDuration||p.carryVariant!=shadowStream.remoteCarry||p.carryDurationMs!=shadowStream.remoteCarryDuration||p.carryIncarnation!=shadowStream.remoteCarryIncarnation||p.carrySpawner!=shadowStream.remoteCarrySpawner||p.carryDefinition!=shadowStream.remoteCarryDefinition||p.grenadeHeld!=shadowStream.remoteGrenadeHeld||p.grenadeVariant!=shadowStream.remoteGrenade||p.grenadeDurationMs!=shadowStream.remoteGrenadeDuration)){
        shadowStreamStateRejectReason=8;++shadowStreamRejected;return;
    }
    NativeWorldNpc carryTarget{};
    if(p.carryVariant&&(!worldObservationReady(tick)||!worldCatalogTick||tick-worldCatalogTick>750||
        !resolveCarryIdentity(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,worldCatalog,p.carrySpawner,p.carryDefinition,carryTarget))){
        ++shadowStreamRejected;return;
    }
    // Use the already-established root clock mapping, without modifying it.
    const double mapped=double(p.sampleTimeUs)*.001+sourcePoseTimeline.offset,now=presentationClock();
    auto combatTimeline=shadowStream.combat;auto equipmentTimeline=shadowStream.equipment;auto traversalTimeline=shadowStream.traversal;auto powerTimeline=shadowStream.power;auto rangedTimeline=shadowStream.ranged;auto carryTimeline=shadowStream.carry;auto grenadeTimeline=shadowStream.grenade;auto heartTimeline=shadowStream.heart;auto stunTimeline=shadowStream.stun;auto movementTimeline=shadowStream.movement;
    const unsigned timelineReject=(sourcePoseTimeline.session!=p.sourceSession||sourcePoseTimeline.epoch!=p.sourceWorldEpoch||sourcePoseTimeline.map!=p.mapId)?10:
       (mapped>now+80.||now-mapped>250.)?11:
       !combatTimeline.push({mapped,p.event,p.combatVariant,p.combatPhase,p.combatTimeMs,p.combatDurationMs})?12:
       !equipmentTimeline.push({mapped,p.event,p.swordState,p.equipmentVariant,p.equipmentTimeMs,p.equipmentDurationMs})?13:
       !traversalTimeline.push({mapped,p.event,p.traversalVariant,p.traversalTimeMs,p.traversalDurationMs,p.traversalEndStance,p.traversalStart,p.traversalGoal,p.traversalPoint,p.traversalHeight})?14:
       !powerTimeline.push({mapped,p.event,p.powerVariant,p.powerTimeMs,p.powerDurationMs})?15:
       !rangedTimeline.push({mapped,p.event,p.rangedKind,p.rangedVariant,p.rangedTimeMs,p.rangedDurationMs,p.rangedMechanism,p.rangedShot})?16:
       !heartTimeline.push({mapped,p.event,p.heartHeld,p.heartVariant,p.heartTimeMs,p.heartDurationMs})?19:
       !grenadeTimeline.push({mapped,p.event,p.grenadeHeld,p.grenadeVariant,p.grenadeTimeMs,p.grenadeDurationMs})?17:
       !carryTimeline.push({mapped,p.event,p.carryVariant,p.carryTimeMs,p.carryDurationMs,p.carryStance,p.carryIncarnation,p.carrySpawner,p.carryDefinition,p.carryThrowVelocity})?18:
       !stunTimeline.push({mapped,p.event,p.stunVariant,p.stunTimeMs,p.stunDurationMs})?22:
       !movementTimeline.push({mapped,p.event,p.state,p.leanSide,p.leanUp,p.airStartZ,p.airTargetZ,p.airStage,p.airRemaining})?19:0;
    if(timelineReject){shadowStreamStateRejectReason=shadowStreamRejectReason=timelineReject;shadowStreamRejectedAge=now-mapped;++shadowStreamRejected;return;}
    shadowStream.combat=std::move(combatTimeline);shadowStream.equipment=std::move(equipmentTimeline);shadowStream.traversal=std::move(traversalTimeline);shadowStream.power=std::move(powerTimeline);shadowStream.ranged=std::move(rangedTimeline);shadowStream.carry=std::move(carryTimeline);shadowStream.grenade=std::move(grenadeTimeline);shadowStream.heart=std::move(heartTimeline);shadowStream.stun=std::move(stunTimeline);shadowStream.remoteCarryLocalHandle=carryTarget.handle;shadowStream.movement=std::move(movementTimeline);
    shadowStream.lastSequence=p.sequence;
    const bool changed=p.event!=shadowStream.lastEvent;
    if(changed){
        const bool ground=p.state==0||p.state==4||p.state==5||p.state==18;
        const bool release=ground&&shadowStream.remoteState==2;
        const bool landing=ground&&shadowStream.remoteState==3;
        if((p.state>=1&&p.state<=3)||release||landing){
            if(shadowStream.pendingCount==8){++shadowStreamRefused;stopShadowStream("presentation_queue_full");return;}
            // Match root presentation delay. Otherwise landing can play while
            // the interpolated body is still above the ground. This queue
            // contains local whitelisted clips only, never gameplay outcomes.
            auto& pending=shadowStream.pending[shadowStream.pendingCount++];
            pending.alias=p.state==1?"attack":p.state==2?"parry":p.state==3?"airborne":landing?"land":"recover";
            pending.due=presentationClock()+(poseSmoothingEnabled?PoseInterpolation::delayMs:0.);
        }
        shadowStream.lastEvent=p.event;shadowStream.remoteState=p.state;
    }
    shadowStream.remoteLeanSide=p.leanSide;shadowStream.remoteLeanUp=p.leanUp;
    shadowStream.remoteVariant=p.combatVariant;shadowStream.remotePhase=p.combatPhase;shadowStream.remoteDuration=p.combatDurationMs;
    shadowStream.remoteSword=p.swordState;shadowStream.remoteEquipment=p.equipmentVariant;shadowStream.remoteEquipmentDuration=p.equipmentDurationMs;shadowStream.remotePower=p.powerVariant;shadowStream.remotePowerDuration=p.powerDurationMs;shadowStream.remoteRangedKind=p.rangedKind;shadowStream.remoteRanged=p.rangedVariant;shadowStream.remoteRangedDuration=p.rangedDurationMs;shadowStream.remoteCarry=p.carryVariant;shadowStream.remoteCarryDuration=p.carryDurationMs;shadowStream.remoteCarryIncarnation=p.carryIncarnation;shadowStream.remoteCarrySpawner=p.carrySpawner;shadowStream.remoteCarryDefinition=p.carryDefinition;
    shadowStream.remoteStun=p.stunVariant;shadowStream.remoteStunDuration=p.stunDurationMs;
    shadowStream.remoteHeartHeld=p.heartHeld;shadowStream.remoteHeart=p.heartVariant;shadowStream.remoteHeartDuration=p.heartDurationMs;
    shadowStream.remoteGrenadeHeld=p.grenadeHeld;shadowStream.remoteGrenade=p.grenadeVariant;shadowStream.remoteGrenadeDuration=p.grenadeDurationMs;
    shadowStream.airStage=p.airStage;shadowStream.airRemaining=p.airRemaining;
    if(p.airStage==1){shadowStream.jumpStartZ=p.airStartZ;shadowStream.jumpTargetZ=p.airTargetZ;shadowStream.jumpTargetValid=true;}
    // A fully validated state proves this exact token/session/world peer is
    // alive, just as its hello does. Unreliable hellos can arrive behind a
    // newer state sequence and be rejected; they must not retire an otherwise
    // fresh action stream. Rejected/duplicate states never reach this renewal.
    shadowStream.lastPeer=shadowStream.lastState=tick;++shadowStreamAccepted;
    shadowStream.powerReceivedHistory.observe(p.event,p.powerVariant,p.powerDurationMs);
    // The native stance uses the same exact incarnation and freshness lease.
    shadowLocoGame=probeGame;shadowLocoActor=probeActor;shadowLocoHandle=probeHandle;shadowLocoEpoch=outgoing.worldEpoch;
    shadowLocoRemoteEpoch=remote.worldEpoch;shadowLocoRemoteSession=remote.session;shadowLocoCombat=true;
    shadowLocoUntil=(std::min)(tick+250,shadowStream.until);
    if((p.state==2||p.state==3)&&shadowAction.actor&&shadowStream.ownsAction){
        shadowAction.until=(std::min)(tick+250,shadowStream.until);shadowActionDeadline.store(shadowAction.until);
    }
}
void serviceShadowStream(ULONGLONG tick){
    if(!shadowStream.until)return;
    if(!shadowStreamCurrent(tick)||(shadowStream.peerReady&&tick-shadowStream.lastPeer>350)){
        shadowStreamRetireRootAge=tick-lastReceive;shadowStreamRetireHelloAge=tick-shadowStream.lastPeer;
        shadowStreamRetireReason=tick>=shadowStream.until?1:!shadowStreamPairReady(tick)?2:!shadowStreamCurrent(tick)?3:4;
        stopShadowStream(tick>=shadowStream.until?"expired":"scope_or_peer_changed");return;
    }
    if(shadowStream.lastState&&tick-shadowStream.lastState>250){shadowStopServiceTick=tick;shadowStopLastState=shadowStream.lastState;stopShadowStream("native_state_stale");return;}
    // Only a current, mutually ready pair renews. No timeout, load, pause,
    // disconnect or identity change can implicitly recreate a retired stream.
    if(shadowStream.persistent&&shadowStream.peerReady)shadowStream.until=tick+15000;
    while(shadowStream.pendingCount&&presentationClock()>=shadowStream.pending[0].due){
        const auto command=shadowStream.pending[0];
        for(unsigned i=1;i<shadowStream.pendingCount;++i)shadowStream.pending[i-1]=shadowStream.pending[i];
        --shadowStream.pendingCount;stopShadowAction("remote_transition");
        if(movementOwnsCombat()&&(!std::strcmp(command.alias,"attack")||!std::strcmp(command.alias,"parry")||!std::strcmp(command.alias,"recover"))){
            shadowStream.ownsAction=false;++shadowStreamApplied;continue;
        }
        if(movementOwnsAirborne()&&(!std::strcmp(command.alias,"airborne")||!std::strcmp(command.alias,"land"))){
            shadowStream.ownsAction=false;++shadowStreamApplied;continue;
        }
        if(!playShadowAction(command.alias)){++shadowStreamRefused;stopShadowStream("native_action_refused");return;}
        shadowStream.ownsAction=true;++shadowStreamApplied;
        if(!std::strcmp(command.alias,"airborne")||!std::strcmp(command.alias,"parry")){
            shadowAction.until=(std::min)(tick+250,shadowStream.until);shadowActionDeadline.store(shadowAction.until);
        }
    }
}
void writeShadowStream(ULONGLONG tick){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.shadow-stream.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{");
    for(unsigned side=0;side<2;++side){
        const auto& history=side?shadowStream.powerReceivedHistory:shadowStream.powerSentHistory;
        fprintf(f,"\"%s\":[",side?"powerReceivedHistory":"powerSentHistory");
        for(unsigned i=0;i<history.count;++i){const auto& e=history.entries[i];
            fprintf(f,"%s{\"event\":%llu,\"variant\":%u,\"durationMs\":%u,\"nativeScene\":%u}",i?",":"",e.event,e.variant,e.duration,e.scene);
        }fprintf(f,"],");
    }
    fprintf(f,"\"wireRefused\":%llu,\"stopServiceTick\":%llu,\"stopLastState\":%llu,",shadowWireRefused,shadowStopServiceTick,shadowStopLastState);
    fprintf(f,"\"sourceLastFailure\":%u,\"sourceFailureCounts\":[",shadowSourceLastFailure);
    for(unsigned i=0;i<shadowSourceFailureCounts.size();++i)fprintf(f,"%s%llu",i?",":"",shadowSourceFailureCounts[i]);fprintf(f,"],");
    fprintf(f,"\"unboundSamples\":%llu,\"unboundRefused\":%llu,\"unboundProbeActive\":%s,",shadowUnboundSamples,shadowUnboundRefused,tick<shadowUnboundProbeUntil?"true":"false");
    fputs("\"sourceReadTimings\":{",f);
    for(unsigned i=0;i<shadowReadTimingMax.size();++i)fprintf(f,"%s\"%s\":{\"count\":%llu,\"maxMs\":%.6f}",i?",":"",shadowReadTimingNames[i],shadowReadTimingCount[i],shadowReadTimingMax[i]);
    fputs("},",f);
    fprintf(f,"\"sourceAttempts\":%llu,\"sourceMisses\":%llu,\"sourceReadMaxMs\":%.4f,\"sendGapMaxMs\":%llu,\"stoppedStateAgeMs\":%llu,\"stoppedRootAgeMs\":%llu,",shadowSourceAttempts,shadowSourceMisses,shadowSourceReadMaxMs,shadowSendGapMax,shadowStoppedStateAge,shadowStoppedRootAge);
    fprintf(f,"\"rejectedEvent\":%llu,\"expectedEvent\":%llu,\"rejectedSequence\":%llu,\"expectedSequence\":%llu,\"rejectedState\":%u,\"expectedState\":%u,",shadowStreamRejectedEvent,shadowStreamExpectedEvent,shadowStreamRejectedSequence,shadowStreamExpectedSequence,shadowStreamRejectedState,shadowStreamExpectedState);
    fprintf(f,"\"stateRejectReason\":%u,\"rejectedAgeMs\":%.5f,",shadowStreamStateRejectReason,shadowStreamRejectedAge);
    fprintf(f,"\"rejectReason\":%u,\"retireReason\":%u,\"retireRootAge\":%llu,\"retireHelloAge\":%llu,\"sourceGrenadeHeld\":%u,\"sourceGrenade\":%u,\"remoteGrenadeHeld\":%u,\"remoteGrenade\":%u,",shadowStreamRejectReason,shadowStreamRetireReason,shadowStreamRetireRootAge,shadowStreamRetireHelloAge,shadowStream.sourceGrenadeHeld,shadowStream.sourceGrenade,shadowStream.remoteGrenadeHeld,shadowStream.remoteGrenade);
    fprintf(f,"\"armed\":%s,\"ready\":%s,\"remainingMs\":%llu,\"token\":%llu,\"peerToken\":%llu,\"epoch\":%llu,\"peerEpoch\":%llu,\"handle\":%u,\"sourceState\":%u,\"remoteState\":%u,\"sourceEvent\":%llu,\"remoteEvent\":%llu,\"lastSequence\":%llu,\"started\":%llu,\"sent\":%llu,\"received\":%llu,\"accepted\":%llu,\"applied\":%llu,\"rejected\":%llu,\"refused\":%llu,\"released\":%llu,\"actionActive\":%s,\"result\":\"%s\",\"leanSide\":%.6f,\"leanUp\":%.6f,\"sourceVariant\":%u,\"sourcePhase\":%u,\"remoteVariant\":%u,\"remotePhase\":%u,\"carrySource\":%u,\"carryRemote\":%u,\"carrySourceIncarnation\":%u,\"carryRemoteIncarnation\":%u,\"carryLocalHandle\":%u,\"carrySpawner\":%llu,\"carryDefinition\":%llu,\"corpseMutations\":false}\n",
        shadowStream.until>tick?"true":"false",shadowStream.peerReady?"true":"false",shadowStream.until>tick?shadowStream.until-tick:0,
        shadowStream.token,shadowStream.peerToken,shadowStream.epoch,shadowStream.peerEpoch,shadowStream.handle,shadowStream.sourceState,shadowStream.remoteState,
        shadowStream.sourceEvent,shadowStream.lastEvent,shadowStream.lastSequence,shadowStreamStarted,shadowStreamSent,shadowStreamReceived,
        shadowStreamAccepted,shadowStreamApplied,shadowStreamRejected,shadowStreamRefused,shadowStreamReleases,shadowAction.actor?"true":"false",shadowStreamResult,shadowStream.remoteLeanSide,shadowStream.remoteLeanUp,shadowStream.sourceVariant,shadowStream.sourcePhase,shadowStream.remoteVariant,shadowStream.remotePhase,shadowStream.sourceCarry,shadowStream.remoteCarry,shadowStream.sourceCarryIncarnation,shadowStream.remoteCarryIncarnation,shadowStream.remoteCarryLocalHandle,shadowStream.remoteCarrySpawner,shadowStream.remoteCarryDefinition);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
