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
#include "native_player_trap_scene.h"
#include "native_player_choke_scene.h"
#include "native_player_potion.h"
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
    CarryPresentation carry;GrenadePresentation grenade;HeartPresentation heart;TrapPresentation trap;ChokePresentation choke;StunPresentation stun;UsePresentation use;
    unsigned sourceChoke=0,sourceChokeTime=0,remoteChoke=0,remoteChokeDuration=0;
    std::uint64_t sourceChokeSpawner=0,sourceChokeDefinition=0,remoteChokeSpawner=0,remoteChokeDefinition=0;
    unsigned sourceTrapHeld=0,sourceTrap=0,sourceTrapTime=0,sourceTrapStock=0,remoteTrapHeld=0,remoteTrap=0,remoteTrapDuration=0,remoteTrapStock=0;
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
std::uint64_t shadowUnboundSamples=0,shadowUnboundRefused=0,shadowHoldSamples=0;
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
// 1203c (product controller only): a peer hello that arrives while no local
// stream runs is remembered so the controller restarts the stream at once,
// and a hello from a NEWER peer incarnation (higher token: the peer restarted
// unilaterally) re-pins the handshake instead of being rejected forever.
ULONGLONG shadowPeerHelloAt=0;std::uint64_t shadowStreamRepins=0;
bool shadowStreamPairReady(ULONGLONG tick){
    return GetCurrentThreadId()==engineThread.load()&&probeFollowExact&&armProbePairReady(tick)&&avatarAiHeld&&
        avatarAiTarget.actor==probeActor&&locomotionEnabled&&coopPresentationReady(outgoing)&&coopPresentationReady(remote);
}
bool shadowStreamCurrent(ULONGLONG tick){
    return shadowStream.until>tick&&shadowStreamPairReady(tick)&&shadowStream.game==observedGame&&
        shadowStream.actor==probeActor&&shadowStream.handle==probeHandle&&shadowStream.epoch==outgoing.worldEpoch&&
        shadowStream.peerEpoch==remote.worldEpoch&&shadowStream.peerSession==remote.session;
}
// 1213: native use source. Player state 75 (vtable 0x1c86ab8) lasts one ~15 ms
// sample per successful use (1210c); its rising edge starts one authored
// interaction whose variant comes from the entity the interaction sensor
// (arkCpntPlayerSensorInteraction, vtable 0x1b403b8; +0x80 high dword = focused
// handle, 1133) points at. Moving away (>0.6 m) or any other action ends it.
struct ShadowUse {bool wasActive=false;unsigned variant=0,duration=0;ULONGLONG start=0;std::uint32_t events=0;float origin[3]{};char name[96]{};std::uint64_t uses=0,unfocused=0,frames=0,edges=0;unsigned endReason=0,endAction=0,lastVariant=0;float target[3]{};bool targetValid=false;} shadowUse;
unsigned shadowFocusedUseVariant(std::uintptr_t base){
    shadowUse.targetValid=false;
    const auto player=worldIdentityEntity(memoryRead,observedGame,1);std::uintptr_t list=0,vt=0,sensor=0;unsigned count=0;std::uint64_t word=0;
    if(!player||!memoryRead(player+0x268,&list,8)||!list||!memoryRead(player+0x270,&count,4)||!(count&0xffffff)||(count&0xffffff)>256)return 0;
    std::array<std::uintptr_t,256> members{};if(!memoryRead(list,members.data(),(count&0xffffff)*8))return 0;
    for(unsigned i=0;i<(count&0xffffff);++i)if(members[i]&&memoryRead(members[i],&vt,8)&&vt==base+0x1b403b8){sensor=members[i];break;}
    if(!sensor||!memoryRead(sensor+0x80,&word,8))return 0;
    const auto handle=static_cast<std::uint32_t>(word>>32);if((handle>>16)==0x3fff){++shadowUse.unfocused;return 1;}
    const auto actor=worldIdentityEntity(memoryRead,observedGame,handle);std::uintptr_t name=0;std::string text;
    if(!actor||!memoryRead(actor+0x20,&name,8)||!worldIdentityString(memoryRead,name,text)){++shadowUse.unfocused;return 1;}
    std::snprintf(shadowUse.name,sizeof(shadowUse.name),"%s",text.c_str());
    // #6: the focused entity's origin (actor+0x300, as readNativeUsable) is the hand target.
    float origin[3]{};if(memoryRead(actor+0x300,origin,12)&&std::isfinite(origin[0])&&std::isfinite(origin[1])&&std::isfinite(origin[2])){std::memcpy(shadowUse.target,origin,12);shadowUse.targetValid=true;}
    return useVariantForName(text.c_str());
}
// 1214j (#11, visual sweep 1260): the native game plays no first-person gesture for pickups, food
// or elixirs, so the partner showed none. A pickup or food (native LootObject47 over Running0/
// Sneaking6) presents the generic use reach toward the focused item; a consumed elixir (health or
// mana potion quantity drop, last valid count kept across the item wheel) presents the same reach
// to the mouth. Presentation only: no item, inventory or health outcome is synthesized.
struct ShadowGesture {bool wasLoot=false,wasHold=false;std::uint64_t holds=0;int potions[2]{-1,-1};std::uint64_t epoch=0;unsigned nextKind=1;ULONGLONG nextPotionRead=0;std::uint64_t loots=0,drinks=0;} shadowGesture;
// 1214al (#11): use-path diagnostics. Takes 1228m-1228q: the client's lock and door uses register at the
// source but the host never presents them, while client taps and host locks/doors do.
std::uint64_t shadowUseSent=0,shadowUseReceived=0,shadowUseAccepted=0,shadowUseRejected=0;unsigned shadowUseRejectReason=0,shadowUseLastVariant=0,shadowUseLastEvent=0;
void startShadowUse(unsigned variant){
    shadowUse.lastVariant=variant;shadowUse.endReason=0;shadowUse.variant=variant;shadowUse.duration=UsePresentation::duration(variant);
    shadowUse.start=GetTickCount64();++shadowUse.events;++shadowUse.uses;std::memcpy(shadowUse.origin,outgoing.position,12);
}
void serviceShadowGestures(const NativePlayerStates& states,std::uintptr_t base,bool useActive){
    bool loot=false;for(const auto& a:states.active)if(a.id==47&&a.vtable==base+0x1c7bcc8){loot=true;break;}
    if(loot&&!shadowGesture.wasLoot&&!useActive&&!shadowUse.variant){shadowFocusedUseVariant(base);startShadowUse(1);++shadowGesture.loots;}
    shadowGesture.wasLoot=loot;
    // 1214m: a held native movable (HoldObject49) holds the reach; its end finishes it.
    const bool hold=nativePlayerGroundHoldObjectStates(states,base);
    if(hold&&!shadowGesture.wasHold&&!useActive){shadowUse.targetValid=false;startShadowUse(6);++shadowGesture.holds;}
    else if(!hold&&shadowGesture.wasHold&&shadowUse.variant==6){shadowUse.targetValid=false;startShadowUse(7);}
    shadowGesture.wasHold=hold;
    if(shadowGesture.epoch!=outgoing.worldEpoch){shadowGesture.epoch=outgoing.worldEpoch;shadowGesture.potions[0]=shadowGesture.potions[1]=-1;}
    const auto tick=GetTickCount64();if(tick<shadowGesture.nextPotionRead)return;
    shadowGesture.nextPotionRead=tick+600;const unsigned kind=shadowGesture.nextKind;shadowGesture.nextKind=kind==1?2:1;
    NativePlayerPotion p{};if(!readNativePlayerPotion(memoryRead,base,kind,p))return;
    auto& last=shadowGesture.potions[kind-1];
    if(last>=0&&p.quantity<last&&!useActive&&!shadowUse.variant){
        shadowUse.target[0]=outgoing.position[0];shadowUse.target[1]=outgoing.position[1];shadowUse.target[2]=outgoing.position[2]+1.5f;shadowUse.targetValid=true;
        startShadowUse(1);++shadowGesture.drinks;}
    last=p.quantity;
}
void serviceShadowUse(const NativePlayerStates& states,std::uintptr_t base){
    bool active=false;for(const auto& a:states.active)if(a.id==75&&a.vtable==base+0x1c86ab8){active=true;break;}
    serviceShadowGestures(states,base,active);
    if(active&&!shadowUse.wasActive){++shadowUse.edges;const unsigned variant=shadowFocusedUseVariant(base);
        if(variant){shadowUse.lastVariant=variant;shadowUse.endReason=0;shadowUse.variant=variant;shadowUse.duration=UsePresentation::duration(variant);shadowUse.start=GetTickCount64();++shadowUse.events;++shadowUse.uses;std::memcpy(shadowUse.origin,outgoing.position,12);}}
    shadowUse.wasActive=active;
    if(shadowUse.variant){
        if(shadowUse.variant<6&&useMovedAway(shadowUse.origin,outgoing.position)){shadowUse.variant=0;shadowUse.endReason=1;}else if(GetTickCount64()-shadowUse.start>=shadowUse.duration){shadowUse.variant=0;shadowUse.endReason=3;}}
}
// 1214cv (#11 death_variants): the native Dead state (player state 15, arkPlayerStateDead, vtable RVA 0x1c82478 from
// its RTTI; confirmed static) plays no scene (probe 1361a). The source read failed closed on it, the stream stopped
// and the partner vanished. It is now action 25 with one directional collapse chosen once at the death edge: the
// nearest living catalogued NPC within 10 m relative to the facing (24 front, 25 back, 26 left, 27 right); a death
// without one (a fall) collapses as front.
struct ShadowDeath {std::uint64_t epoch=0,deaths=0,fatalities=0;ULONGLONG start=0;unsigned variant=0,leadMs=0,sceneMs=0,sceneStage=0;float bearing=0;bool nearby=false,fatality=false,sceneRead=false;std::string scene;};
ShadowDeath shadowDeath{};
inline bool nativePlayerDead(const NativePlayerStates& s,std::uintptr_t base){
    return s.active.size()==1&&s.active[0].id==15&&s.active[0].vtable==base+0x1c82478;
}
// 1214cw: an NPC finisher on the player is native state 38 (arkPlayerStateNpcFatality, vtable RVA 0x1c3d878 from its
// RTTI; confirmed static) beside Running, playing midnight/gameplay/player/base/death/npc_fatality_<npc>.midnightscene
// (probe 1361c: guards, subscene npc, 4000 ms; it binds PlayerBody to ply_body_generic_npc_<npc>_fatality). The body
// is held, then dropped: its pelvis reaches half height at guards 2.9 s, elite 1.4, howler 1.5, overseer 2.3, witch
// 0.73, clockwork 3.47 (offline native samples, npc-fatality-timing-v1362.json). The partner's collapse reaches half
// height about 0.9 s in, so it starts that much earlier on the native scene clock and holds its first frame before.
// 1214cx: 0.4 s earlier still: native GameOver (about 3.8 s into the guards' finisher) retires the dead source's
// stream, and in 1361o Emily vanished while still kneeling.
inline bool nativePlayerNpcFatality(const NativePlayerStates& s,std::uintptr_t base,std::uintptr_t& pointer){
    for(const auto& a:s.active)if(a.id==38&&a.vtable==base+0x1c3d878){pointer=a.pointer;return true;}
    return false;
}
inline unsigned npcFatalityCollapseLeadMs(const std::string& asset){
    static const std::pair<const char*,unsigned> leads[]={{"guards",1600},{"elite",100},{"howler",200},{"overseer",1000},{"witch",0},{"clockwork",2170}};
    for(const auto& e:leads)if(asset==std::string("midnight/gameplay/player/base/death/npc_fatality_")+e.first+".midnightscene")return e.second;
    return 1600;
}
// The state's scene handle (+0x24) names the playing instance in the midnight manager lists, as for sword scenes.
// stage (1214cx): 1 no scene handle, 2 manager list unreadable, 3 no instance, 4 declaration/name/time; 0 read.
template<class Read> bool readNativeNpcFatalityScene(Read read,std::uintptr_t base,std::uintptr_t game,std::uintptr_t pointer,std::string& asset,unsigned& timeMs,unsigned* stage=nullptr){
    unsigned handle=0;std::uintptr_t instance=0;unsigned local=0;unsigned& at=stage?*stage:local;at=1;
    if(!pointer||!read(pointer+0x24,&handle,4)||!handle)return false;
    at=2;
    for(unsigned field=0;field<2;++field){std::uintptr_t list=0;unsigned flags=0;const auto at=game+0x1f5338+(field?0x610:0);
        if(!read(at,&list,8)||!read(at+8,&flags,4)||(flags&0xffffff)>512||((flags&0xffffff)&&!list))return false;
        for(unsigned i=0;i<(flags&0xffffff);++i){std::uintptr_t candidate=0;unsigned id=0;
            if(!read(list+i*8,&candidate,8)||!candidate||!read(candidate+0x158,&id,4))return false;
            if(id==handle){if(instance&&instance!=candidate)return false;instance=candidate;}}
    }
    at=instance?4:3;
    std::uintptr_t declaration=0,vt=0,name=0;std::string text;unsigned t=0;
    if(!instance||!read(instance,&declaration,8)||!read(declaration,&vt,8)||vt!=base+0x1affbc8||!read(declaration+8,&name,8)||
       !worldIdentityString(read,name,text)||text.rfind("midnight/gameplay/player/base/death/npc_fatality_",0)!=0||!read(instance+0x160,&t,4)||t>60000)return false;
    asset=std::move(text);timeMs=t;at=0;return true;
}
unsigned shadowDeathVariant(ULONGLONG tick){
    shadowDeath.nearby=false;shadowDeath.bearing=0;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    const float fx=outgoing.orientation[0],fy=outgoing.orientation[1],fl=std::sqrt(fx*fx+fy*fy);
    if(!(fl>.1f)||!worldObservationReady(tick)||!worldCatalogTick||tick-worldCatalogTick>750)return 24;
    float best=100.f,bx=0,by=0;
    for(const auto& n:worldCatalog){float root[3]{};NativeHealth h{};
        if(!n.actor||n.actor==probeActor||!memoryRead(n.actor+0x300,root,12)||!readNativeHealth(memoryRead,base,n.handle,h)||h.actor!=n.actor||h.current<=0)continue;
        const float dx=root[0]-outgoing.position[0],dy=root[1]-outgoing.position[1],dz=root[2]-outgoing.position[2],d=dx*dx+dy*dy+dz*dz;
        if(std::isfinite(d)&&d>.01f&&d<best){best=d;bx=dx;by=dy;}
    }
    if(best>=100.f)return 24;
    const float a=std::atan2((by*fx-bx*fy)/fl,(bx*fx+by*fy)/fl);shadowDeath.nearby=true;shadowDeath.bearing=a;
    return std::fabs(a)<=.7853981634f?24u:std::fabs(a)>=2.356194490f?25u:a>0?26u:27u;
}
bool shadowSourceAction(unsigned& action,float* leanSide=nullptr,float* leanUp=nullptr,CoopShadowActionPacket* airPacket=nullptr,unsigned* sceneHandle=nullptr){
    shadowSourceReadStage=1;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);NativePlayerStates states{};NativePlayerSword sword{};
    NativePlayerPowerScene power{};NativePlayerStunScene stun{};
    const bool nativeStateValid=shadowTimedRead(0,[&]{return readNativePlayerStates(memoryRead,base,states);})&&states.player.game==observedGame&&states.player.player==observedPlayer;
    if(nativeStateValid)serviceShadowUse(states,base);
    std::uintptr_t fatalityState=0;
    const bool fatality=nativeStateValid&&nativePlayerNpcFatality(states,base,fatalityState);
    if(nativeStateValid&&(fatality||nativePlayerDead(states,base))){
        const auto tick=GetTickCount64();
        if(!shadowDeath.start||shadowDeath.epoch!=outgoing.worldEpoch){shadowDeath.epoch=outgoing.worldEpoch;shadowDeath.start=tick;shadowDeath.variant=shadowDeathVariant(tick);++shadowDeath.deaths;
            shadowDeath.fatality=fatality;shadowDeath.leadMs=fatality?1600:0;shadowDeath.sceneRead=false;shadowDeath.sceneMs=0;shadowDeath.scene.clear();if(fatality)++shadowDeath.fatalities;}
        // A finisher's clock is its native scene time; a plain death (or a read miss) uses the time since the edge.
        std::string scene;unsigned sceneMs=0;
        const bool sceneRead=fatality&&readNativeNpcFatalityScene(memoryRead,base,states.player.game,fatalityState,scene,sceneMs,&shadowDeath.sceneStage);
        if(sceneRead){shadowDeath.sceneRead=true;shadowDeath.sceneMs=sceneMs;shadowDeath.leadMs=npcFatalityCollapseLeadMs(scene);shadowDeath.scene=std::move(scene);}
        const ULONGLONG clock=sceneRead?sceneMs:tick-shadowDeath.start;
        action=25;
        if(airPacket){const unsigned duration=CombatPresentation::deathDuration(shadowDeath.variant);
            airPacket->combatVariant=shadowDeath.variant;airPacket->combatPhase=1;airPacket->combatDurationMs=duration;
            airPacket->combatTimeMs=unsigned((std::min<ULONGLONG>)(duration,clock>shadowDeath.leadMs?clock-shadowDeath.leadMs:0));
            airPacket->swordState=shadowStreamLastPacket.swordState==2?2u:1u;}
        if(leanSide)*leanSide=0;if(leanUp)*leanUp=0;shadowSourceReadStage=0;return true;
    }
    if(nativeStateValid)shadowDeath.start=0;
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
    NativePlayerTrapScene trap{};NativePlayerPlaceable heldTrap{};
    const bool trapValid=nativeStateValid&&!powerValid&&!rangedValid&&!grenadeValid&&!heartValid&&!heartIdle&&readNativePlayerTrapScene(memoryRead,base,states,trap);
    bool trapIdle=false;
    if(nativeStateValid&&!trapValid&&!heartValid&&!heartIdle&&states.active.size()==1&&states.player.current<=1){
        for(unsigned kind=1;kind<=2;++kind)if(readNativePlayerPlaceable(memoryRead,base,kind,heldTrap,nullptr,true)){trapIdle=true;break;}
    }
    NativePlayerCarryScene carry{};
    const bool carryValid=nativeStateValid&&!powerValid&&!rangedValid&&!grenadeValid&&shadowTimedRead(5,[&]{return readNativePlayerCarryScene(memoryRead,base,states,carry,true);});
    NativePlayerCarryPreparation carryPreparation{};
    const bool carryPreparing=nativeStateValid&&!powerValid&&!rangedValid&&!grenadeValid&&!carryValid&&
        shadowTimedRead(6,[&]{return readNativePlayerCarryPreparation(memoryRead,base,states,carryPreparation);});
    if(!carryPreparing)shadowStream.carryPreparationClock.clear();
    NativePlayerChokeScene choke{};
    const bool chokeValid=nativeStateValid&&readNativePlayerChokeScene(memoryRead,base,states,choke);
    if(chokeValid)action=21;else if(trapValid)action=20;else if(trapIdle)action=0;else if(heartValid)action=19;else if(heartIdle)action=0;else if(possessionOccupied)action=17;else if(stunValid)action=16;else if(powerValid)action=10;else if(rangedValid)action=13;else if(grenadeValid)action=15;else if(carryValid||carryPreparing)action=14;
    shadowSourceReadStage=2;
    const bool valid=nativeStateValid&&(chokeValid||trapValid||trapIdle||heartValid||heartIdle||possessionOccupied||stunValid||powerValid||rangedValid||grenadeValid||carryValid||carryPreparing||nativePlayerShadowAction(states,base,action))&&
        shadowTimedRead(7,[&]{return readNativePlayerSword(memoryRead,base,sword);})&&sword.character==localRole+1&&sword.sword.game==states.player.game&&
        sword.sword.actor==states.player.player&&sword.sword.logic==1&&sword.sword.quantity==1&&sword.sword.equipped==sword.sword.started;
    float side=0,up=0;
    if(!valid||(action==5&&!nativePlayerLeanAmount(memoryRead,states,base,side,up)))return false;
    shadowSourceReadStage=3;
    if(chokeValid){
        if(sword.sword.equipped!=nativeTakedownSword(choke.variant)||choke.character!=localRole+1||choke.target.handle==probeHandle||choke.target.actor==probeActor)return false;
        if(sceneHandle)*sceneHandle=choke.handle;
        if(airPacket){airPacket->chokeVariant=choke.variant;airPacket->chokeTimeMs=choke.timeMs;airPacket->chokeDurationMs=choke.durationMs;
            airPacket->chokeSpawner=carryIdentityKey(choke.target.spawnerName);airPacket->chokeDefinition=carryIdentityKey(choke.target.definition);}
    }
    if(stunValid){
        if(sceneHandle)*sceneHandle=stun.handle;
        if(airPacket){airPacket->stunVariant=stun.variant;airPacket->stunTimeMs=stun.timeMs;airPacket->stunDurationMs=stun.durationMs;}
    }
    if(shadowUse.variant&&action!=0){shadowUse.variant=0;shadowUse.endReason=2;shadowUse.endAction=action;} // any other action ends the interaction
    if(airPacket&&shadowUse.variant){const auto elapsed=static_cast<unsigned>(GetTickCount64()-shadowUse.start);
        if(elapsed<shadowUse.duration){airPacket->useVariant=shadowUse.variant;airPacket->useTimeMs=elapsed;airPacket->useDurationMs=shadowUse.duration;airPacket->useEvent=shadowUse.events;
            if(shadowUse.targetValid){for(unsigned k=0;k<3;++k)airPacket->useTarget[k]=shadowUse.target[k];airPacket->useTargetValid=1;}}}
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
    if(trapValid||trapIdle){
        const auto& item=trapValid?trap.trap:heldTrap;
        if(item.player.character!=localRole+1||item.player.sword.game!=observedGame||item.player.sword.actor!=observedPlayer)return false;
        if(trapValid&&sceneHandle)*sceneHandle=trap.handle;
        if(airPacket){airPacket->trapHeld=item.kind;airPacket->trapStock=item.quantity;airPacket->trapVariant=trap.variant;airPacket->trapTimeMs=trap.timeMs;airPacket->trapDurationMs=trap.durationMs;}
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
    // 1214g: a generic sword finisher (census-1255 fatality, right arm only) runs in the
    // sword attack state but is owned by the takedown channel; it has no combat scene
    // (take 1256b: the client finisher was admitted and never sent).
    if(swordAction&&!chokeValid){if(!sword.sword.equipped)return false;NativePlayerCombatScene combat{};
        if(!readNativePlayerCombatScene(memoryRead,base,states,combat)||combat.character!=localRole+1||
           (combat.target.actor&&(combat.target.actor==probeActor||combat.target.handle==probeHandle)))return false;
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
    shadowUnboundProbeUntil=0;shadowStream.until=0;shadowStream.peerReady=false;shadowStream.pendingCount=0;shadowStream.keyholeHandoff={};shadowStream.combat.clear();shadowStream.equipment.clear();shadowStream.traversal.clear();shadowStream.power.clear();shadowStream.ranged.clear();shadowStream.carry.clear();shadowStream.grenade.clear();shadowStream.heart.clear();shadowStream.trap.clear();shadowStream.stun.clear();shadowStream.use.clear();shadowStream.remoteCarryLocalHandle=0;shadowStream.movement.clear();stopShadowEquipment(reason);shadowStreamResult=reason;shadowLocoUntil=0;
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
    shadowSourceReadStage=shadowSourceLastFailure=0;shadowSourceFailureCounts={};shadowUnboundSamples=shadowUnboundRefused=shadowHoldSamples=0;
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
    // 1213b: native use (state 75) is visible for about one engine frame, so
    // the use edge is sampled every frame here, not only at the 33 ms packet
    // cadence (1214b: the client's use was missed at packet cadence).
    {const auto base=reinterpret_cast<std::uintptr_t>(gameImage);NativePlayerStates frameStates{};
     if(readNativePlayerStates(memoryRead,base,frameStates)&&frameStates.player.game==observedGame&&frameStates.player.player==observedPlayer){++shadowUse.frames;serviceShadowUse(frameStates,base);}}
    if(shadowPreviousSendService)shadowSendGapMax=(std::max)(shadowSendGapMax,tick-shadowPreviousSendService);shadowPreviousSendService=tick;
    if(tick-shadowStream.lastHello>=100){
        shadowStream.lastHello=tick;const auto p=shadowStreamPacket(0);
        if(validShadowAction(p,remote,outgoing)&&coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_UnreliableNoNagle,nullptr)==k_EResultOK)++shadowStreamSent;
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
        if(readNativePresentationFallback(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,observedPlayer,localRole+1,swordState)){
            // Discard every partially populated action field. Only independently
            // revalidated equipment and ground locomotion cross this path.
            p=shadowStreamPacket(1);p.swordState=swordState;action=18;scene=0;leanSide=leanUp=0;++shadowUnboundSamples;
        }else if(readNativePresentationHold(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,observedPlayer,localRole+1,swordState,shadowStreamLastPacket.swordState)){
            p=shadowStreamPacket(1);p.swordState=swordState;action=24;scene=0;leanSide=leanUp=0;++shadowHoldSamples;
        }else{++shadowUnboundRefused;return;}
    }
    if(p.chokeVariant!=shadowStream.sourceChoke||p.chokeSpawner!=shadowStream.sourceChokeSpawner||p.chokeDefinition!=shadowStream.sourceChokeDefinition||(p.chokeVariant==2&&p.chokeTimeMs<shadowStream.sourceChokeTime)||p.trapHeld!=shadowStream.sourceTrapHeld||p.trapStock!=shadowStream.sourceTrapStock||p.trapVariant!=shadowStream.sourceTrap||(p.trapVariant==4&&p.trapTimeMs<shadowStream.sourceTrapTime)||p.heartHeld!=shadowStream.sourceHeartHeld||p.heartVariant!=shadowStream.sourceHeart||action!=shadowStream.sourceState||scene!=shadowStream.sourceScene||p.stunVariant!=shadowStream.sourceStun||p.combatVariant!=shadowStream.sourceVariant||p.combatPhase!=shadowStream.sourcePhase||p.swordState!=shadowStream.sourceSword||p.equipmentVariant!=shadowStream.sourceEquipment||p.powerVariant!=shadowStream.sourcePower||p.traversalVariant!=shadowStream.sourceTraversal||p.rangedKind!=shadowStream.sourceRangedKind||p.rangedVariant!=shadowStream.sourceRanged||p.carryVariant!=shadowStream.sourceCarry||p.carryIncarnation!=shadowStream.sourceCarryIncarnation||p.carrySpawner!=shadowStream.sourceCarrySpawner||p.carryDefinition!=shadowStream.sourceCarryDefinition||p.grenadeHeld!=shadowStream.sourceGrenadeHeld||p.grenadeVariant!=shadowStream.sourceGrenade||(p.grenadeVariant==4&&p.grenadeTimeMs<shadowStream.sourceGrenadeTime)){
        shadowStream.sourceChoke=p.chokeVariant;shadowStream.sourceChokeSpawner=p.chokeSpawner;shadowStream.sourceChokeDefinition=p.chokeDefinition;
        shadowStream.sourceTrapHeld=p.trapHeld;shadowStream.sourceTrapStock=p.trapStock;shadowStream.sourceTrap=p.trapVariant;shadowStream.sourceHeartHeld=p.heartHeld;shadowStream.sourceHeart=p.heartVariant;shadowStream.sourceState=action;shadowStream.sourceScene=scene;shadowStream.sourceStun=p.stunVariant;shadowStream.sourceVariant=p.combatVariant;shadowStream.sourcePhase=p.combatPhase;shadowStream.sourceSword=p.swordState;shadowStream.sourceEquipment=p.equipmentVariant;shadowStream.sourcePower=p.powerVariant;shadowStream.sourceTraversal=p.traversalVariant;shadowStream.sourceRangedKind=p.rangedKind;shadowStream.sourceRanged=p.rangedVariant;shadowStream.sourceCarry=p.carryVariant;shadowStream.sourceCarryIncarnation=p.carryIncarnation;shadowStream.sourceCarrySpawner=p.carrySpawner;shadowStream.sourceCarryDefinition=p.carryDefinition;shadowStream.sourceGrenadeHeld=p.grenadeHeld;shadowStream.sourceGrenade=p.grenadeVariant;++shadowStream.sourceEvent;}
    shadowStream.sourceChokeTime=p.chokeTimeMs;shadowStream.sourceTrapTime=p.trapTimeMs;shadowStream.sourceGrenadeTime=p.grenadeTimeMs;
    p.state=action;p.event=shadowStream.sourceEvent;p.leanSide=leanSide;p.leanUp=leanUp;
    std::vector<NativeWindblastFxRecord> muzzleRecords;
    if(shadowTimedRead(10,[&]{return readNativeRangedFxRecords(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,muzzleRecords);}))
        p.rangedShot=nativeRangedShot.observe(shadowStream.token,shadowStream.peerToken,p.event,localRole,
            action==13&&p.rangedKind==1&&p.rangedVariant==3&&p.rangedTimeMs<=666,outgoing.position,muzzleRecords);
    if(validShadowAction(p,remote,outgoing)&&coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_UnreliableNoNagle,nullptr)==k_EResultOK){
        ++shadowStreamSent;shadowStreamLastPacket=p;if(p.useVariant)++shadowUseSent;
        shadowStream.powerSentHistory.observe(p.event,p.powerVariant,p.powerDurationMs,scene);
    }else ++shadowWireRefused;
}
void duplicateShadowStream(){
    if(shadowStreamCurrent(GetTickCount64())&&shadowStreamLastPacket.sequence&&validShadowAction(shadowStreamLastPacket,remote,outgoing))
        coopSendMessage(connection,&shadowStreamLastPacket,sizeof(shadowStreamLastPacket),k_nSteamNetworkingSend_ReliableNoNagle,nullptr);
}
void receiveShadowStream(const CoopShadowActionPacket& p,ULONGLONG tick){
    ++shadowStreamReceived;
    if(productMode||productAutoArm){
        if(p.kind==0&&!shadowStreamCurrent(tick)&&validShadowAction(p,outgoing,remote))shadowPeerHelloAt=tick;
        if(p.kind==0&&nativeMutationAllowed&&shadowStreamCurrent(tick)&&shadowStream.peerToken&&p.sourceToken>shadowStream.peerToken&&
           p.sequence>shadowStream.lastSequence&&validShadowAction(p,outgoing,remote)){
            shadowStream.peerToken=p.sourceToken;shadowStream.peerReady=false;++shadowStreamRepins;shadowStreamResult="peer_restarted_repinned";consoleOutput("COOP: shadow stream re-pinned to restarted peer token %llu\n",p.sourceToken);
        }
    }
    const unsigned reject=!nativeMutationAllowed?1:!shadowStreamCurrent(tick)?2:!validShadowAction(p,outgoing,remote)?3:p.sequence<=shadowStream.lastSequence?4:
        (shadowStream.peerToken&&p.sourceToken!=shadowStream.peerToken)?5:(p.targetToken&&p.targetToken!=shadowStream.token)?6:0;
    if(p.useVariant){++shadowUseReceived;shadowUseLastVariant=p.useVariant;shadowUseLastEvent=p.useEvent;}
    if(reject){shadowStreamRejectReason=reject;if(p.kind==1&&reject!=2)shadowStreamStateRejectReason=reject;++shadowStreamRejected;if(p.useVariant){++shadowUseRejected;shadowUseRejectReason=reject;}return;}
    if(p.kind==0){
        shadowStream.peerToken=p.sourceToken;shadowStream.lastSequence=p.sequence;shadowStream.lastPeer=tick;
        if(p.targetToken==shadowStream.token){shadowStream.peerReady=true;shadowStreamResult="ready";}return;
    }
    if(!shadowStream.peerReady||!shadowActionTokensMatch(p,shadowStream.token,shadowStream.peerToken)||
       !shadowActionEventFresh(p,shadowStream.lastSequence,shadowStream.lastEvent,shadowStream.remoteState)){
        shadowStreamStateRejectReason=shadowStreamRejectReason=!shadowStream.peerReady?7:!shadowActionTokensMatch(p,shadowStream.token,shadowStream.peerToken)?20:21;
        if(p.useVariant){++shadowUseRejected;shadowUseRejectReason=shadowStreamRejectReason;}
        shadowStreamRejectedEvent=p.event;shadowStreamExpectedEvent=shadowStream.lastEvent;shadowStreamRejectedSequence=p.sequence;shadowStreamExpectedSequence=shadowStream.lastSequence;shadowStreamRejectedState=p.state;shadowStreamExpectedState=shadowStream.remoteState;
        ++shadowStreamRejected;return;
    }
    if(p.event==shadowStream.lastEvent&&(p.chokeVariant!=shadowStream.remoteChoke||p.chokeDurationMs!=shadowStream.remoteChokeDuration||p.chokeSpawner!=shadowStream.remoteChokeSpawner||p.chokeDefinition!=shadowStream.remoteChokeDefinition||p.trapHeld!=shadowStream.remoteTrapHeld||p.trapVariant!=shadowStream.remoteTrap||p.trapStock!=shadowStream.remoteTrapStock||p.trapDurationMs!=shadowStream.remoteTrapDuration||p.heartHeld!=shadowStream.remoteHeartHeld||p.heartVariant!=shadowStream.remoteHeart||p.heartDurationMs!=shadowStream.remoteHeartDuration||p.stunVariant!=shadowStream.remoteStun||p.stunDurationMs!=shadowStream.remoteStunDuration||p.combatVariant!=shadowStream.remoteVariant||p.combatPhase!=shadowStream.remotePhase||p.combatDurationMs!=shadowStream.remoteDuration||p.swordState!=shadowStream.remoteSword||p.equipmentVariant!=shadowStream.remoteEquipment||p.equipmentDurationMs!=shadowStream.remoteEquipmentDuration||p.powerVariant!=shadowStream.remotePower||p.powerDurationMs!=shadowStream.remotePowerDuration||p.rangedKind!=shadowStream.remoteRangedKind||p.rangedVariant!=shadowStream.remoteRanged||p.rangedDurationMs!=shadowStream.remoteRangedDuration||p.carryVariant!=shadowStream.remoteCarry||p.carryDurationMs!=shadowStream.remoteCarryDuration||p.carryIncarnation!=shadowStream.remoteCarryIncarnation||p.carrySpawner!=shadowStream.remoteCarrySpawner||p.carryDefinition!=shadowStream.remoteCarryDefinition||p.grenadeHeld!=shadowStream.remoteGrenadeHeld||p.grenadeVariant!=shadowStream.remoteGrenade||p.grenadeDurationMs!=shadowStream.remoteGrenadeDuration)){
        shadowStreamStateRejectReason=8;++shadowStreamRejected;if(p.useVariant){++shadowUseRejected;shadowUseRejectReason=8;}return;
    }
    NativeWorldNpc chokeTarget{};
    if(p.chokeVariant&&(!worldObservationReady(tick)||!worldCatalogTick||tick-worldCatalogTick>750||
        !resolveCarryIdentity(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,worldCatalog,p.chokeSpawner,p.chokeDefinition,chokeTarget)||
        chokeTarget.handle==probeHandle||chokeTarget.actor==probeActor)){++shadowStreamRejected;return;}
    NativeWorldNpc carryTarget{};
    if(p.carryVariant&&(!worldObservationReady(tick)||!worldCatalogTick||tick-worldCatalogTick>750||
        !resolveCarryIdentity(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,worldCatalog,p.carrySpawner,p.carryDefinition,carryTarget))){
        ++shadowStreamRejected;return;
    }
    // Use the already-established root clock mapping, without modifying it.
    const double mapped=double(p.sampleTimeUs)*.001+sourcePoseTimeline.offset,now=presentationClock();
    auto combatTimeline=shadowStream.combat;auto equipmentTimeline=shadowStream.equipment;auto traversalTimeline=shadowStream.traversal;auto powerTimeline=shadowStream.power;auto rangedTimeline=shadowStream.ranged;auto carryTimeline=shadowStream.carry;auto grenadeTimeline=shadowStream.grenade;auto heartTimeline=shadowStream.heart;auto trapTimeline=shadowStream.trap;auto chokeTimeline=shadowStream.choke;auto stunTimeline=shadowStream.stun;auto useTimeline=shadowStream.use;auto movementTimeline=shadowStream.movement;
    const unsigned timelineReject=(sourcePoseTimeline.session!=p.sourceSession||sourcePoseTimeline.epoch!=p.sourceWorldEpoch||sourcePoseTimeline.map!=p.mapId)?10:
       (mapped>now+80.||now-mapped>250.)?11:
       !combatTimeline.push({mapped,p.event,p.combatVariant,p.combatPhase,p.combatTimeMs,p.combatDurationMs})?12:
       !equipmentTimeline.push({mapped,p.event,p.swordState,p.equipmentVariant,p.equipmentTimeMs,p.equipmentDurationMs})?13:
       !traversalTimeline.push({mapped,p.event,p.traversalVariant,p.traversalTimeMs,p.traversalDurationMs,p.traversalEndStance,p.traversalStart,p.traversalGoal,p.traversalPoint,p.traversalHeight})?14:
       !powerTimeline.push({mapped,p.event,p.powerVariant,p.powerTimeMs,p.powerDurationMs})?15:
       !rangedTimeline.push({mapped,p.event,p.rangedKind,p.rangedVariant,p.rangedTimeMs,p.rangedDurationMs,p.rangedMechanism,p.rangedShot})?16:
       !chokeTimeline.push({mapped,p.event,p.chokeVariant,p.chokeTimeMs,p.chokeDurationMs,p.chokeSpawner,p.chokeDefinition})?23:
       !trapTimeline.push({mapped,p.event,p.trapHeld,p.trapVariant,p.trapTimeMs,p.trapDurationMs,p.trapStock})?22:
       !heartTimeline.push({mapped,p.event,p.heartHeld,p.heartVariant,p.heartTimeMs,p.heartDurationMs})?19:
       !grenadeTimeline.push({mapped,p.event,p.grenadeHeld,p.grenadeVariant,p.grenadeTimeMs,p.grenadeDurationMs})?17:
       !carryTimeline.push({mapped,p.event,p.carryVariant,p.carryTimeMs,p.carryDurationMs,p.carryStance,p.carryIncarnation,p.carrySpawner,p.carryDefinition,p.carryThrowVelocity})?18:
       !stunTimeline.push({mapped,p.event,p.stunVariant,p.stunTimeMs,p.stunDurationMs})?22:
       !useTimeline.push({mapped,p.useEvent,p.useVariant,p.useTimeMs,p.useDurationMs,{p.useTarget[0],p.useTarget[1],p.useTarget[2]},p.useTargetValid==1})?24:
       !movementTimeline.push({mapped,p.event,p.state,p.leanSide,p.leanUp,p.airStartZ,p.airTargetZ,p.airStage,p.airRemaining})?19:0;
    if(timelineReject){shadowStreamStateRejectReason=shadowStreamRejectReason=timelineReject;shadowStreamRejectedAge=now-mapped;++shadowStreamRejected;if(p.useVariant){++shadowUseRejected;shadowUseRejectReason=100+timelineReject;}return;}
    if(p.useVariant)++shadowUseAccepted;
    shadowStream.combat=std::move(combatTimeline);shadowStream.equipment=std::move(equipmentTimeline);shadowStream.traversal=std::move(traversalTimeline);shadowStream.power=std::move(powerTimeline);shadowStream.ranged=std::move(rangedTimeline);shadowStream.carry=std::move(carryTimeline);shadowStream.grenade=std::move(grenadeTimeline);shadowStream.heart=std::move(heartTimeline);shadowStream.trap=std::move(trapTimeline);shadowStream.choke=std::move(chokeTimeline);shadowStream.stun=std::move(stunTimeline);shadowStream.use=std::move(useTimeline);shadowStream.remoteCarryLocalHandle=carryTarget.handle;shadowStream.movement=std::move(movementTimeline);
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
    shadowStream.remoteChoke=p.chokeVariant;shadowStream.remoteChokeDuration=p.chokeDurationMs;shadowStream.remoteChokeSpawner=p.chokeSpawner;shadowStream.remoteChokeDefinition=p.chokeDefinition;
    shadowStream.remoteTrapHeld=p.trapHeld;shadowStream.remoteTrapStock=p.trapStock;shadowStream.remoteTrap=p.trapVariant;shadowStream.remoteTrapDuration=p.trapDurationMs;
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
    if(!shadowStreamCurrent(tick)||(shadowStream.peerReady&&tick-shadowStream.lastPeer>presentationHelloMs())){
        shadowStreamRetireRootAge=tick-lastReceive;shadowStreamRetireHelloAge=tick-shadowStream.lastPeer;
        shadowStreamRetireReason=tick>=shadowStream.until?1:!shadowStreamPairReady(tick)?2:!shadowStreamCurrent(tick)?3:4;
        stopShadowStream(tick>=shadowStream.until?"expired":"scope_or_peer_changed");return;
    }
    if(shadowStream.lastState&&tick-shadowStream.lastState>presentationFreshMs()){shadowStopServiceTick=tick;shadowStopLastState=shadowStream.lastState;stopShadowStream("native_state_stale");return;}
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
    fprintf(f,"\"death\":{\"deaths\":%llu,\"fatalities\":%llu,\"active\":%s,\"fatality\":%s,\"variant\":%u,\"nearby\":%s,\"bearing\":%.4f,\"sceneRead\":%s,\"sceneMs\":%u,\"leadMs\":%u,\"sceneStage\":%u,\"scene\":\"%s\"},",
        shadowDeath.deaths,shadowDeath.fatalities,shadowDeath.start?"true":"false",shadowDeath.fatality?"true":"false",shadowDeath.variant,shadowDeath.nearby?"true":"false",shadowDeath.bearing,
        shadowDeath.sceneRead?"true":"false",shadowDeath.sceneMs,shadowDeath.leadMs,shadowDeath.sceneStage,shadowDeath.scene.c_str());
    fprintf(f,"\"useDiag\":{\"sent\":%llu,\"received\":%llu,\"accepted\":%llu,\"rejected\":%llu,\"rejectReason\":%u,\"lastVariant\":%u,\"lastEvent\":%u},",
        static_cast<unsigned long long>(shadowUseSent),static_cast<unsigned long long>(shadowUseReceived),static_cast<unsigned long long>(shadowUseAccepted),static_cast<unsigned long long>(shadowUseRejected),shadowUseRejectReason,shadowUseLastVariant,shadowUseLastEvent);
    fprintf(f,"\"sourceLastFailure\":%u,\"sourceFailureCounts\":[",shadowSourceLastFailure);
    for(unsigned i=0;i<shadowSourceFailureCounts.size();++i)fprintf(f,"%s%llu",i?",":"",shadowSourceFailureCounts[i]);fprintf(f,"],");
    fprintf(f,"\"unboundSamples\":%llu,\"unboundRefused\":%llu,\"holdSamples\":%llu,\"unboundProbeActive\":%s,",shadowUnboundSamples,shadowUnboundRefused,shadowHoldSamples,tick<shadowUnboundProbeUntil?"true":"false");
    // 1074h diagnostic: individual pair-readiness terms (bit set = term false).
    fprintf(f,"\"pairBits\":%u,",(probeFollowExact?0u:1u)|(armProbePairReady(tick)?0u:2u)|(avatarAiHeld?0u:4u)|(avatarAiTarget.actor==probeActor?0u:8u)|(locomotionEnabled?0u:16u)|
        (coopPresentationReady(outgoing)?0u:32u)|(coopPresentationReady(remote)?0u:64u)|(probeFollow?0u:128u)|(networkFrameOwned.load()?0u:256u)|(lastReceive&&tick-lastReceive<presentationFreshMs()?0u:512u)|(coopPresentationPairReady(outgoing,remote)?0u:1024u));
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
