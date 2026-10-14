from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src'
def change(name,old,new):
 p=S/name;s=p.read_text();assert s.count(old)==1,(name,old,s.count(old));p.write_text(s.replace(old,new),encoding='utf-8')
change('shadow_stream.h','#include "native_player_possession_scene.h"','#include "native_player_possession_scene.h"\n#include "native_player_possession_occupation.h"')
change('shadow_stream.h','    NativePlayerRangedScene ranged{};','    const bool possessionOccupied=nativeStateValid&&!stunValid&&!powerValid&&readNativeHumanPossessionOccupation(memoryRead,base,states);\n    NativePlayerRangedScene ranged{};')
change('shadow_stream.h','if(stunValid)action=16;else if(powerValid)', 'if(possessionOccupied)action=17;else if(stunValid)action=16;else if(powerValid)')
change('shadow_stream.h','(stunValid||powerValid||rangedValid||grenadeValid||carryValid||carryPreparing||nativePlayerShadowAction(states,base,action))','(possessionOccupied||stunValid||powerValid||rangedValid||grenadeValid||carryValid||carryPreparing||nativePlayerShadowAction(states,base,action))')
change('shadow_action_protocol.h','version=49,','version=50,')
change('shadow_action_protocol.h','p.version==49','p.version==50')
change('shadow_action_protocol.h','p.state<=16','p.state<=17')
change('shadow_action_protocol.h','    return stun&&grenade&&carry','    const bool possession=p.state!=17||(p.kind==1&&p.character==1&&p.role==0&&p.swordState==1&&!p.powerVariant&&!p.airStage&&!p.rangedKind&&!p.grenadeHeld&&!p.carryVariant&&!p.stunVariant&&!p.combatVariant&&!p.equipmentVariant);\n    return possession&&stun&&grenade&&carry')
change('movement_semantics.h','s.state>16','s.state>17')
change('movement_playback.h','void requestMovementEquipment(bool drawn);','void requestMovementEquipment(bool drawn);\nvoid requestPossessionBodyVisibility(bool hide);')
change('movement_playback.h','bool equipmentRequest=false,equipmentDrawn=true;','bool equipmentRequest=false,equipmentDrawn=true,possessionHidden=false;')
change('movement_playback.h','const bool semanticValid=shadowStream.movement.sample(pose.time,semantic);','const bool semanticValid=shadowStream.movement.sample(pose.time,semantic);\n        possessionHidden=semanticValid&&semantic.state==17&&p.bank.hasPossessionRelease();')
change('movement_playback.h','    if(equipmentRequest)requestMovementEquipment(equipmentDrawn);','    requestPossessionBodyVisibility(possessionHidden);\n    if(possessionHidden)requestMovementEquipment(false);\n    else if(equipmentRequest)requestMovementEquipment(equipmentDrawn);')
change('engine_bridge.cpp','#include "shadow_equipment.h"','#include "shadow_equipment.h"\n#include "possession_body_visibility.h"')
change('engine_bridge.cpp','writeAvatarSword(tick);writeShadowEquipment(tick);','writeAvatarSword(tick);writeShadowEquipment(tick);writePossessionBodyVisibility();')
change('engine_bridge.cpp','serviceShadowEquipment(tick);serviceShadowStream(tick);','serviceShadowEquipment(tick);serviceShadowStream(tick);servicePossessionBodyVisibility(tick);')
change('shadow_action_protocol_tests.cpp','#include "shadow_action_protocol.h"','#include "shadow_action_protocol.h"\n#include "native_possession_context.h"')
change('shadow_action_protocol_tests.cpp','      for(unsigned variant=26;variant<=27;++variant){','''      {
        auto occupied=power;occupied.powerVariant=occupied.powerDurationMs=occupied.powerTimeMs=0;occupied.state=17;occupied.swordState=1;
        check(validShadowAction(occupied,c,h),"Corvo occupied-host presentation is separate from power animation");
        auto bad=occupied;bad.swordState=2;check(!validShadowAction(bad,c,h),"occupied host cannot retain drawn sword");
        bad=occupied;bad.role=1;bad.character=2;check(!validShadowAction(bad,ec,eh),"Emily cannot claim native human Possession");
        bad=occupied;bad.version=49;check(!validShadowAction(bad,c,h),"old release-only protocol cannot hide body");
        bad=occupied;bad.powerVariant=28;bad.powerDurationMs=400;check(!validShadowAction(bad,c,h),"occupation cannot carry a release animation");
        bad=occupied;bad.targetWorldEpoch++;check(!validShadowAction(bad,c,h),"old occupied world cannot hide new clone");
        MovementSemantics timeline;MovementSemantics::Sample sample{};
        check(timeline.push({100,1,17})&&timeline.sample(200,sample)&&sample.state==17,"occupied visibility uses delayed body timeline");
        check(!timeline.sample(301,sample),"stale occupied sample cannot perpetuate hiding");
        check(timeline.push({310,2,0})&&timeline.sample(310,sample)&&sample.state==0,"native grounded recovery replaces occupancy");
        constexpr std::uintptr_t base=0x140000000;
        NativePlayerStates state{};state.player.logic=1;state.player.current=18;state.active.push_back({0,1,base+0x1c85628});
        check(nativeHumanPossessionContext(state,base,8),"exact occupied host Running0 physics18 admitted");
        check(!nativeHumanPossessionContext(state,base,14),"inactive sentinel is never occupation");
        state.defaultMode=true;check(!nativeHumanPossessionContext(state,base,8),"default movement cannot authorize hide");state.defaultMode=false;
        state.player.current=0;state.active[0]={26,1,base+0x1c79878};check(nativeHumanPossessionContext(state,base,1),"exact post-release entry context admitted");
        check(!nativeHumanPossessionContext(state,base,0),"release context stays separately scene-owned");
        state.active[0].vtable++;check(!nativeHumanPossessionContext(state,base,1),"wrong native class refused");
      }
      for(unsigned variant=26;variant<=27;++variant){''')
print('Prepared cosmetic Corvo occupation channel protocol50 and finite clone visibility lease; no target NPC mutation')
