"""Apply the bounded native stun presentation candidate to the current source."""
from pathlib import Path
S=Path(__file__).resolve().parents[2]/'outputs/d2-coop-lab/src'
def edit(name,changes):
 p=S/name;s=p.read_text(encoding='utf-8-sig')
 for a,b in changes:
  assert s.count(a)==1,(name,a,s.count(a));s=s.replace(a,b)
 p.write_text(s,encoding='utf-8')
p=S/'native_player_stun_scene.h';s=p.read_text();a=s.index('// Exact native');b=s.index('struct NativePlayerStunScene');helper=s[a:b];p.write_text(s[:a]+s[b:])
edit('native_player_states.h', [('inline bool nativePlayerMotion(',helper+'inline bool nativePlayerMotion('), ('    // Exact v54/v56h source captures:', '    if(nativePlayerStunStates(s,base)){motion=0;return true;} // Native Stun inherits Navigation; exact standing class only.\n    // Exact v54/v56h source captures:')])
edit('shadow_action_protocol.h', [
 ('#include "grenade_presentation.h"','#include "grenade_presentation.h"\n#include "stun_presentation.h"'),
 ('version=37,size=', 'version=38,size='),
 ('    std::uint32_t grenadeHeld=0,grenadeVariant=0,grenadeTimeMs=0,grenadeDurationMs=0;', '    std::uint32_t grenadeHeld=0,grenadeVariant=0,grenadeTimeMs=0,grenadeDurationMs=0;\n    std::uint32_t stunVariant=0,stunTimeMs=0,stunDurationMs=0;'),
 ('==324','==336'),
 ('    return grenade&&carry', '''    const bool stun=StunPresentation::valid({0,1,p.stunVariant,p.stunTimeMs,p.stunDurationMs})&&
        (p.stunVariant?(p.kind==1&&p.state==16&&!p.grenadeHeld&&!p.rangedKind&&!p.powerVariant&&!p.carryVariant&&!p.combatVariant&&!p.airStage):p.state!=16);
    return stun&&grenade&&carry'''),
 ('p.version==37','p.version==38'),('p.state<=15','p.state<=16')])
edit('movement_semantics.h',[('s.state>15','s.state>16')])
edit('shadow_stream.h',[
 ('#include "native_player_grenade_scene.h"','#include "native_player_grenade_scene.h"\n#include "native_player_stun_scene.h"'),
 ('    CarryPresentation carry;GrenadePresentation grenade;', '    CarryPresentation carry;GrenadePresentation grenade;StunPresentation stun;\n    unsigned sourceStun=0,remoteStun=0,remoteStunDuration=0;'),
 ('    NativePlayerPowerScene power{};', '    NativePlayerPowerScene power{};NativePlayerStunScene stun{};'),
 ('    const bool powerValid=nativeStateValid&&', '    const bool stunValid=nativeStateValid&&readNativePlayerStunScene(memoryRead,base,states,stun);\n    const bool powerValid=nativeStateValid&&!stunValid&&'),
 ('    if(powerValid)action=10;', '    if(stunValid)action=16;else if(powerValid)action=10;'),
 ('(powerValid||rangedValid||grenadeValid||carryValid||carryPreparing||nativePlayerShadowAction', '(stunValid||powerValid||rangedValid||grenadeValid||carryValid||carryPreparing||nativePlayerShadowAction'),
 ('    if(carryValid||carryPreparing){', '''    if(stunValid){
        if(sceneHandle)*sceneHandle=stun.handle;
        if(airPacket){airPacket->stunVariant=stun.variant;airPacket->stunTimeMs=stun.timeMs;airPacket->stunDurationMs=stun.durationMs;}
    }
    if(carryValid||carryPreparing){'''),
 ('shadowStream.grenade.clear();', 'shadowStream.grenade.clear();shadowStream.stun.clear();'),
 ('if(action!=shadowStream.sourceState||scene!=shadowStream.sourceScene||', 'if(action!=shadowStream.sourceState||scene!=shadowStream.sourceScene||p.stunVariant!=shadowStream.sourceStun||'),
 ('shadowStream.sourceState=action;shadowStream.sourceScene=scene;', 'shadowStream.sourceState=action;shadowStream.sourceScene=scene;shadowStream.sourceStun=p.stunVariant;'),
 ('if(p.event==shadowStream.lastEvent&&(p.combatVariant', 'if(p.event==shadowStream.lastEvent&&(p.stunVariant!=shadowStream.remoteStun||p.stunDurationMs!=shadowStream.remoteStunDuration||p.combatVariant'),
 ('auto grenadeTimeline=shadowStream.grenade;', 'auto grenadeTimeline=shadowStream.grenade;auto stunTimeline=shadowStream.stun;'),
 ('       !movementTimeline.push(', '       !stunTimeline.push({mapped,p.event,p.stunVariant,p.stunTimeMs,p.stunDurationMs})?22:\n       !movementTimeline.push('),
 ('shadowStream.grenade=std::move(grenadeTimeline);', 'shadowStream.grenade=std::move(grenadeTimeline);shadowStream.stun=std::move(stunTimeline);'),
 ('    shadowStream.remoteGrenadeHeld=p.grenadeHeld;', '    shadowStream.remoteStun=p.stunVariant;shadowStream.remoteStunDuration=p.stunDurationMs;\n    shadowStream.remoteGrenadeHeld=p.grenadeHeld;')])
edit('movement_bank.h',[
 ('32+260*','32+264*'),
 ('    bool hasGrenade()', '    bool hasStun()const{return version>=43;}\n    bool hasGrenade()'),
 ('248,251,260};return v<=42?', '248,251,260,264};return v<=43?'),
 ('for(unsigned v=1;v<=42;++v)', 'for(unsigned v=1;v<=43;++v)')])
edit('movement_playback.h',[
 ('    unsigned grenadeHeld=0,', '    unsigned stunVariant=0;float stunAmount=0,stunWeight=0,stunTransition=0;std::uint64_t stunEvent=0;\n    std::vector<MovementBank::Joint> stunFrom,stunLast;\n    unsigned grenadeHeld=0,'),
 ('        GrenadePresentation::Sample grenade{};', '''        StunPresentation::Sample stun{};float stunAmount=0;
        const bool stunActive=p.bank.hasStun()&&shadowStream.stun.sample(pose.time,stun,stunAmount)&&stun.variant&&ground&&pose.motionValid;
        if(combatDt>=0&&combatDt<.25f){
            if(stunActive){if(stun.event!=p.stunEvent){p.stunFrom=p.stunLast;p.stunTransition=0;p.stunEvent=stun.event;}
                p.stunVariant=stun.variant;p.stunAmount=stunAmount;p.stunTransition=(std::min)(1.f,p.stunTransition+combatDt/.035f);}
            p.stunWeight+=((stunActive?1.f:0.f)-p.stunWeight)*(1-std::exp(-combatDt/(stunActive?.025f:.10f)));
        }else p.stunWeight=0;
        GrenadePresentation::Sample grenade{};'''),
 ('        if(p.bank.hasCarry()&&p.carryWeight>.001f){', '''        if(p.bank.hasStun()&&p.stunWeight>.001f&&p.stunVariant){
            auto stun=p.bank.sampleParameter(StunPresentation::clip(p.stunVariant),j,p.stunAmount);
            if(p.stunFrom.size()==count)stun=MovementBank::blend(p.stunFrom[j],stun,p.stunTransition*p.stunTransition*(3-2*p.stunTransition));
            if(p.stunLast.size()!=count)p.stunLast.resize(count);p.stunLast[j]=stun;
            v=MovementBank::blend(v,stun,p.stunWeight);
        }
        if(p.bank.hasCarry()&&p.carryWeight>.001f){'''),
 ('    fprintf(f,"\\\"grenadeHeld\\\":', '    fprintf(f,"\\\"stunVariant\\\":%u,\\\"stunAmount\\\":%.7f,\\\"stunWeight\\\":%.7f,\\\"stunEvent\\\":%llu,",p.stunVariant,p.stunAmount,p.stunWeight,p.stunEvent);\n    fprintf(f,"\\\"grenadeHeld\\\":')])
print('Integrated native scene-based stun candidate; build/runtime acceptance pending')
