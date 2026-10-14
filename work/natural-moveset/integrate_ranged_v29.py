"""One-time source migration, assertions keep edits scoped to expected text."""
from pathlib import Path
S=Path(__file__).resolve().parents[2]/'outputs/d2-coop-lab/src'
def update(name,changes):
 p=S/name;t=p.read_text()
 for old,new in changes:
  assert old in t,(name,old[:100]);t=t.replace(old,new)
 p.write_text(t)
update('shadow_stream.h',[
 ('#include "native_player_power_scene.h"','#include "native_player_power_scene.h"\n#include "native_player_ranged_scene.h"'),
 ('PowerPresentation power;MovementSemantics movement;','PowerPresentation power;RangedPresentation ranged;MovementSemantics movement;'),
 ('unsigned sourcePower=0,remotePower=0,remotePowerDuration=0;','unsigned sourcePower=0,remotePower=0,remotePowerDuration=0;\n    unsigned sourceRangedKind=0,sourceRanged=0,remoteRangedKind=0,remoteRanged=0,remoteRangedDuration=0;'),
 ('if(powerValid)action=10;','NativePlayerRangedScene ranged{};\n    const bool rangedValid=nativeStateValid&&!powerValid&&readNativePlayerRangedScene(memoryRead,base,states,ranged);\n    if(powerValid)action=10;else if(rangedValid)action=13;'),
 ('(powerValid||nativePlayerShadowAction(states,base,action))','(powerValid||rangedValid||nativePlayerShadowAction(states,base,action))'),
 ('if(powerValid){\n        if(!sword.sword.equipped', '''if(rangedValid){
        if(ranged.weapon.player.character!=localRole+1)return false;
        if(sceneHandle)*sceneHandle=ranged.handle;
        if(airPacket){airPacket->rangedKind=ranged.weapon.kind;airPacket->rangedVariant=ranged.variant;airPacket->rangedTimeMs=ranged.timeMs;airPacket->rangedDurationMs=ranged.durationMs;}
    }else if(action==0&&states.player.current==0&&states.active.size()==1&&sword.sword.equipped){
        NativePlayerRangedItem held{};
        if(readNativePlayerRangedItem(memoryRead,base,1,held,true)||readNativePlayerRangedItem(memoryRead,base,2,held,true))
            if(airPacket)airPacket->rangedKind=held.kind;
    }
    if(powerValid){
        if(!sword.sword.equipped'''),
 ('shadowStream.power.clear();shadowStream.movement.clear();','shadowStream.power.clear();shadowStream.ranged.clear();shadowStream.movement.clear();'),
 ('p.traversalVariant!=shadowStream.sourceTraversal){','p.traversalVariant!=shadowStream.sourceTraversal||p.rangedKind!=shadowStream.sourceRangedKind||p.rangedVariant!=shadowStream.sourceRanged){'),
 ('shadowStream.sourceTraversal=p.traversalVariant;++shadowStream.sourceEvent;','shadowStream.sourceTraversal=p.traversalVariant;shadowStream.sourceRangedKind=p.rangedKind;shadowStream.sourceRanged=p.rangedVariant;++shadowStream.sourceEvent;'),
 ('p.powerDurationMs!=shadowStream.remotePowerDuration))','p.powerDurationMs!=shadowStream.remotePowerDuration||p.rangedKind!=shadowStream.remoteRangedKind||p.rangedVariant!=shadowStream.remoteRanged||p.rangedDurationMs!=shadowStream.remoteRangedDuration))'),
 ('auto powerTimeline=shadowStream.power;auto movementTimeline','auto powerTimeline=shadowStream.power;auto rangedTimeline=shadowStream.ranged;auto movementTimeline'),
 ('!movementTimeline.push({mapped,p.event,p.state,','!rangedTimeline.push({mapped,p.event,p.rangedKind,p.rangedVariant,p.rangedTimeMs,p.rangedDurationMs})||\n       !movementTimeline.push({mapped,p.event,p.state,'),
 ('shadowStream.power=std::move(powerTimeline);shadowStream.movement','shadowStream.power=std::move(powerTimeline);shadowStream.ranged=std::move(rangedTimeline);shadowStream.movement'),
 ('shadowStream.remotePowerDuration=p.powerDurationMs;','shadowStream.remotePowerDuration=p.powerDurationMs;shadowStream.remoteRangedKind=p.rangedKind;shadowStream.remoteRanged=p.rangedVariant;shadowStream.remoteRangedDuration=p.rangedDurationMs;'),
 ])
update('movement_bank.h',[
 ('32+201*','32+213*'),
 ('bool hasWindblastContexts()const{return version>=28;}','bool hasWindblastContexts()const{return version>=28;}\n    bool hasRanged()const{return version>=29;}'),
 ('198,201};return v<=28?','198,201,213};return v<=29?'),
 ('v<=28;++v','v<=29;++v'),
 ])
update('movement_bank_tests.cpp', [('(argc==1?1:28);format<=28','(argc==1?1:29);format<=29')])
update('movement_playback.h',[
 ('unsigned turnSerial=0;', '''unsigned rangedKind=0,rangedVariant=0;float rangedAmount=0,rangedWeight=0,rangedTransition=0,rangedMoveMix=0;
    std::uint64_t rangedEvent=0;std::vector<MovementBank::Joint> rangedFrom,rangedLast;
    unsigned turnSerial=0;'''),
 ('PowerPresentation::Sample power{};float powerAmount=0;', '''RangedPresentation::Sample ranged{};float rangedAmount=0;
        const bool rangedValid=p.bank.hasRanged()&&shadowStream.ranged.sample(pose.time,ranged,rangedAmount);
        const bool rangedActive=rangedValid&&ranged.kind&&ground&&pose.motionValid;
        if(combatDt>=0&&combatDt<.25f){
            if(rangedActive){
                if(ranged.event!=p.rangedEvent){p.rangedFrom=p.rangedLast;p.rangedTransition=0;p.rangedEvent=ranged.event;}
                p.rangedKind=ranged.kind;p.rangedVariant=ranged.variant;p.rangedAmount=ranged.variant?rangedAmount:std::fmod(float(pose.time*.001)/p.bank.clips[RangedPresentation::clip(ranged.kind,0)].angle,1.f);
                p.rangedTransition=(std::min)(1.f,p.rangedTransition+combatDt/.06f);
            }
            p.rangedWeight+=((rangedActive?1.f:0.f)-p.rangedWeight)*(1-std::exp(-combatDt/(rangedActive?.025f:.08f)));
            const float moving=(std::max)(0.f,(std::min)(1.f,(p.clock.speed-.1f)/.5f));
            p.rangedMoveMix+=(moving-p.rangedMoveMix)*(1-std::exp(-combatDt/.07f));
        }else p.rangedWeight=0;
        PowerPresentation::Sample power{};float powerAmount=0;'''),
 ('||powerActive||(p.bank.hasPowers()&&semantic.state==10))','||powerActive||rangedActive||(p.bank.hasRanged()&&semantic.state==13)||(p.bank.hasPowers()&&semantic.state==10))'),
 ('p.equipmentWeight<.001f&&!powerActive)','p.equipmentWeight<.001f&&!powerActive&&!rangedActive)'),
 ('        std::array<float,12> q{};std::copy(v.begin(),v.end(),q.begin());', '''        if(p.bank.hasRanged()&&p.rangedWeight>.001f&&p.rangedKind){
            auto ranged=p.bank.sampleParameter(RangedPresentation::clip(p.rangedKind,p.rangedVariant),j,p.rangedAmount);
            if(p.rangedFrom.size()==count)ranged=MovementBank::blend(p.rangedFrom[j],ranged,p.rangedTransition*p.rangedTransition*(3-2*p.rangedTransition));
            if(p.rangedLast.size()!=count)p.rangedLast.resize(count);p.rangedLast[j]=ranged;
            if(MovementBank::combatLocomotionJoint(j,p.parents))ranged=MovementBank::blend(ranged,v,p.rangedMoveMix);
            v=MovementBank::blend(v,ranged,p.rangedWeight);
        }
        std::array<float,12> q{};std::copy(v.begin(),v.end(),q.begin());'''),
 ('\\"landingCrouchMix\\":%.6f}', '\\"landingCrouchMix\\":%.6f,\\"rangedKind\\":%u,\\"rangedVariant\\":%u,\\"rangedAmount\\":%.6f,\\"rangedWeight\\":%.6f,\\"rangedEvent\\":%llu}'),
 ('p.secondJump.transition,p.landingCrouchMix);','p.secondJump.transition,p.landingCrouchMix,p.rangedKind,p.rangedVariant,p.rangedAmount,p.rangedWeight,p.rangedEvent);'),
 ])
print('Ranged v29 source migration applied')
