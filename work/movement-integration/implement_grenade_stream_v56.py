"""Apply checked grenade presentation extension; no gameplay outcome messages."""
from pathlib import Path
P=Path(__file__).resolve().parents[2]/'outputs/d2-coop-lab/src'
def change(name, edits):
 p=P/name;s=p.read_text()
 for a,b in edits:
  assert s.count(a)==1,(name,a[:100],s.count(a));s=s.replace(a,b)
 p.write_text(s)
change('shadow_action_protocol.h',[
 ('#include "carry_presentation.h"','#include "carry_presentation.h"\n#include "grenade_presentation.h"'),
 ('version=36','version=37'),('p.version==36','p.version==37'),
 ('std::array<float,3> carryThrowVelocity{};','std::array<float,3> carryThrowVelocity{};\n    std::uint32_t grenadeHeld=0,grenadeVariant=0,grenadeTimeMs=0,grenadeDurationMs=0;'),
 ('sizeof(CoopShadowActionPacket)==308','sizeof(CoopShadowActionPacket)==324'),
 ('    return carry&&ranged&&power', '''    const bool grenade=GrenadePresentation::valid({0,1,p.grenadeHeld,p.grenadeVariant,p.grenadeTimeMs,p.grenadeDurationMs})&&
        (p.grenadeVariant?p.state==15:p.state!=15)&&(!p.grenadeHeld||(p.kind==1&&(p.state==0||p.state==15)&&!p.rangedKind&&!p.powerVariant&&!p.carryVariant&&!p.combatVariant&&!p.airStage));
    return grenade&&carry&&ranged&&power'''),('p.state<=14','p.state<=15')])
change('movement_semantics.h',[('s.state>14','s.state>15')])
change('movement_bank.h',[
 ('32+251*','32+260*'),('    bool hasCrouch()','    bool hasGrenade()const{return version>=42;}\n    bool hasCrouch()'),
 ('248,251};return v<=41','248,251,260};return v<=42'),('v<=41;++v','v<=42;++v')])
change('shadow_stream.h',[
 ('#include "native_player_ranged_scene.h"','#include "native_player_ranged_scene.h"\n#include "native_player_grenade_scene.h"'),
 ('    CarryPresentation carry;','    CarryPresentation carry;GrenadePresentation grenade;\n    unsigned sourceGrenadeHeld=0,sourceGrenade=0,sourceGrenadeTime=0,remoteGrenadeHeld=0,remoteGrenade=0,remoteGrenadeDuration=0;'),
 ('    NativePlayerCarryScene carry{};', '''    NativePlayerGrenadeScene grenade{};
    const bool grenadeValid=nativeStateValid&&!powerValid&&!rangedValid&&readNativePlayerGrenadeScene(memoryRead,base,states,grenade);
    NativePlayerCarryScene carry{};'''),
 ('nativeStateValid&&!powerValid&&!rangedValid&&readNativePlayerCarryScene','nativeStateValid&&!powerValid&&!rangedValid&&!grenadeValid&&readNativePlayerCarryScene'),
 ('nativeStateValid&&!powerValid&&!rangedValid&&!carryValid&&','nativeStateValid&&!powerValid&&!rangedValid&&!grenadeValid&&!carryValid&&'),
 ('else if(carryValid||carryPreparing)action=14;','else if(grenadeValid)action=15;else if(carryValid||carryPreparing)action=14;'),
 ('powerValid||rangedValid||carryValid||carryPreparing||','powerValid||rangedValid||grenadeValid||carryValid||carryPreparing||'),
 ('    if(powerValid){', '''    if(grenadeValid){
        if(grenade.grenade.player.character!=localRole+1)return false;
        if(sceneHandle)*sceneHandle=grenade.handle;
        if(airPacket){airPacket->grenadeHeld=1;airPacket->grenadeVariant=grenade.variant;airPacket->grenadeTimeMs=grenade.timeMs;airPacket->grenadeDurationMs=grenade.durationMs;}
    }else if(action==0&&states.player.current==0&&states.active.size()==1&&states.active[0].id==0&&states.active[0].vtable==base+0x1c85628&&sword.sword.equipped){
        NativePlayerGrenade held{};
        if(readNativePlayerGrenade(memoryRead,base,held)&&held.player.character==localRole+1&&held.equipped&&held.started&&held.quantity>0&&airPacket)airPacket->grenadeHeld=1;
    }
    if(powerValid){'''),
 ('shadowStream.carry.clear();','shadowStream.carry.clear();shadowStream.grenade.clear();'),
 ('p.carryDefinition!=shadowStream.sourceCarryDefinition){','p.carryDefinition!=shadowStream.sourceCarryDefinition||p.grenadeHeld!=shadowStream.sourceGrenadeHeld||p.grenadeVariant!=shadowStream.sourceGrenade||(p.grenadeVariant==4&&p.grenadeTimeMs<shadowStream.sourceGrenadeTime)){'),
 ('++shadowStream.sourceEvent;}','shadowStream.sourceGrenadeHeld=p.grenadeHeld;shadowStream.sourceGrenade=p.grenadeVariant;++shadowStream.sourceEvent;}\n    shadowStream.sourceGrenadeTime=p.grenadeTimeMs;'),
 ('p.carryDefinition!=shadowStream.remoteCarryDefinition)){','p.carryDefinition!=shadowStream.remoteCarryDefinition||p.grenadeHeld!=shadowStream.remoteGrenadeHeld||p.grenadeVariant!=shadowStream.remoteGrenade||p.grenadeDurationMs!=shadowStream.remoteGrenadeDuration)){'),
 ('auto carryTimeline=shadowStream.carry;','auto carryTimeline=shadowStream.carry;auto grenadeTimeline=shadowStream.grenade;'),
 ('       !carryTimeline.push(', '       !grenadeTimeline.push({mapped,p.event,p.grenadeHeld,p.grenadeVariant,p.grenadeTimeMs,p.grenadeDurationMs})||\n       !carryTimeline.push('),
 ('shadowStream.carry=std::move(carryTimeline);','shadowStream.carry=std::move(carryTimeline);shadowStream.grenade=std::move(grenadeTimeline);'),
 ('    shadowStream.airStage=p.airStage;', '    shadowStream.remoteGrenadeHeld=p.grenadeHeld;shadowStream.remoteGrenade=p.grenadeVariant;shadowStream.remoteGrenadeDuration=p.grenadeDurationMs;\n    shadowStream.airStage=p.airStage;')])
change('movement_playback.h',[
 ('    unsigned carryVariant=0,carryStance=0;', '''    unsigned grenadeHeld=0,grenadeVariant=0;float grenadeAmount=0,grenadeWeight=0,grenadeTransition=0,grenadeMoveMix=0;
    std::uint64_t grenadeEvent=0;std::vector<MovementBank::Joint> grenadeFrom,grenadeLast;
    unsigned carryVariant=0,carryStance=0;'''),
 ('        PowerPresentation::Sample power{};', '''        GrenadePresentation::Sample grenade{};float grenadeAmount=0;
        const bool grenadeValid=p.bank.hasGrenade()&&shadowStream.grenade.sample(pose.time,grenade,grenadeAmount);
        const bool grenadeActive=grenadeValid&&grenade.held&&ground&&pose.motionValid;
        if(combatDt>=0&&combatDt<.25f){
            if(grenadeActive){
                if(grenade.event!=p.grenadeEvent){p.grenadeFrom=p.grenadeLast;p.grenadeTransition=0;p.grenadeEvent=grenade.event;}
                p.grenadeHeld=grenade.held;p.grenadeVariant=grenade.variant;
                p.grenadeAmount=grenade.variant?grenadeAmount:std::fmod(float(pose.time*.001)/p.bank.clips[251].angle,1.f);
                p.grenadeTransition=(std::min)(1.f,p.grenadeTransition+combatDt/.06f);
            }else p.grenadeHeld=0;
            p.grenadeWeight+=((grenadeActive?1.f:0.f)-p.grenadeWeight)*(1-std::exp(-combatDt/(grenadeActive?.025f:.10f)));
            const float moving=(std::max)(0.f,(std::min)(1.f,(p.clock.speed-.1f)/.5f));
            p.grenadeMoveMix+=(moving-p.grenadeMoveMix)*(1-std::exp(-combatDt/.07f));
        }else p.grenadeWeight=p.grenadeHeld=0;
        PowerPresentation::Sample power{};'''),
 ('||powerActive||rangedActive||carryActive||carryUnbound','||powerActive||rangedActive||grenadeActive||carryActive||carryUnbound'),
 ('        if(p.bank.hasCarry()&&p.carryWeight>.001f){', '''        if(p.bank.hasGrenade()&&p.grenadeWeight>.001f){
            auto grenade=p.bank.sampleParameter(GrenadePresentation::clip(p.grenadeVariant),j,p.grenadeAmount);
            if(p.grenadeFrom.size()==count)grenade=MovementBank::blend(p.grenadeFrom[j],grenade,p.grenadeTransition*p.grenadeTransition*(3-2*p.grenadeTransition));
            if(p.grenadeLast.size()!=count)p.grenadeLast.resize(count);p.grenadeLast[j]=grenade;
            if(MovementBank::combatLocomotionJoint(j,p.parents))grenade=MovementBank::blend(grenade,v,p.grenadeMoveMix);
            v=MovementBank::blend(v,grenade,p.grenadeWeight);
        }
        if(p.bank.hasCarry()&&p.carryWeight>.001f){''')])
print('Grenade state, semantic clock and bank/body path patched; tests and runtime pending')
