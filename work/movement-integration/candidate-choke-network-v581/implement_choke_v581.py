from pathlib import Path
import shutil
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab/src'
B=W/'choke-pre-v581';B.mkdir(exist_ok=False)
def edit(name,changes):
 p=S/name;s=p.read_text();shutil.copy2(p,B/name)
 for a,b in changes:
  assert s.count(a)==1,(name,a[:100],s.count(a));s=s.replace(a,b)
 p.write_text(s)
edit('native_player_states.h',[
 ('inline bool nativePlayerMotion(','''// Native rear-choke579: exclusive Choke41, default mode and standing root.
// This only carries the independently observed root; owned scenes authorize gestures.
inline bool nativePlayerGroundChokeStates(const NativePlayerStates& s,std::uintptr_t base){
    return s.player.logic==1&&s.defaultMode&&s.player.current==0&&s.active.size()==1&&
        s.active[0].id==41&&s.active[0].vtable==base+0x1c77c18;
}
inline bool nativePlayerMotion('''),
 ('    if(nativePlayerGroundTrapStates(s,base))','    if(nativePlayerGroundChokeStates(s,base)){motion=0;return true;}\n    if(nativePlayerGroundTrapStates(s,base))')])
edit('shadow_action_protocol.h',[
 ('#include "trap_presentation.h"','#include "trap_presentation.h"\n#include "choke_presentation.h"'),
 ('version=62','version=63'),('p.version==62','p.version==63'),('p.state<=20','p.state<=21'),('==372','==400'),
 ('    std::uint32_t heartHeld=', '    std::uint32_t chokeVariant=0,chokeTimeMs=0,chokeDurationMs=0;\n    std::uint64_t chokeSpawner=0,chokeDefinition=0;\n    std::uint32_t heartHeld='),
 ('    return trap&&heart', '''    const bool choke=ChokePresentation::valid({0,1,p.chokeVariant,p.chokeTimeMs,p.chokeDurationMs,p.chokeSpawner,p.chokeDefinition})&&
        (p.chokeVariant?(p.kind==1&&p.state==21&&p.swordState==1&&!p.trapHeld&&!p.heartHeld&&!p.grenadeHeld&&!p.rangedKind&&!p.carryVariant&&!p.powerVariant&&!p.stunVariant&&!p.combatVariant&&!p.equipmentVariant&&!p.airStage):p.state!=21);
    return choke&&trap&&heart''')])
edit('movement_semantics.h',[('s.state>20','s.state>21')])
edit('movement_bank.h',[
 ('32+348*','32+351*'),('    bool hasTrap()','    bool hasChoke()const{return version>=63;}\n    bool hasTrap()'),
 ('340,347,348};return v<=62','340,347,348,351};return v<=63'),('v<=62;++v','v<=63;++v')])
edit('shadow_stream.h',[
 ('#include "native_player_trap_scene.h"','#include "native_player_trap_scene.h"\n#include "native_player_choke_scene.h"'),
 ('TrapPresentation trap;','TrapPresentation trap;ChokePresentation choke;'),
 ('    unsigned sourceTrapHeld=', '    unsigned sourceChoke=0,sourceChokeTime=0,remoteChoke=0,remoteChokeDuration=0;\n    std::uint64_t sourceChokeSpawner=0,sourceChokeDefinition=0,remoteChokeSpawner=0,remoteChokeDefinition=0;\n    unsigned sourceTrapHeld='),
 ('    if(trapValid)action=20;', '    NativePlayerChokeScene choke{};\n    const bool chokeValid=nativeStateValid&&readNativePlayerChokeScene(memoryRead,base,states,choke);\n    if(chokeValid)action=21;else if(trapValid)action=20;'),
 ('&&(trapValid||trapIdle', '&&(chokeValid||trapValid||trapIdle'),
 ('    if(stunValid){\n', '''    if(chokeValid){
        if(sword.sword.equipped||choke.character!=localRole+1||choke.target.handle==probeHandle||choke.target.actor==probeActor)return false;
        if(sceneHandle)*sceneHandle=choke.handle;
        if(airPacket){airPacket->chokeVariant=choke.variant;airPacket->chokeTimeMs=choke.timeMs;airPacket->chokeDurationMs=choke.durationMs;
            airPacket->chokeSpawner=carryIdentityKey(choke.target.spawnerName);airPacket->chokeDefinition=carryIdentityKey(choke.target.definition);}
    }
    if(stunValid){
'''),
 ('    if(p.trapHeld!=shadowStream.sourceTrapHeld', '    if(p.chokeVariant!=shadowStream.sourceChoke||p.chokeSpawner!=shadowStream.sourceChokeSpawner||p.chokeDefinition!=shadowStream.sourceChokeDefinition||(p.chokeVariant==2&&p.chokeTimeMs<shadowStream.sourceChokeTime)||p.trapHeld!=shadowStream.sourceTrapHeld'),
 ('        shadowStream.sourceTrapHeld=', '        shadowStream.sourceChoke=p.chokeVariant;shadowStream.sourceChokeSpawner=p.chokeSpawner;shadowStream.sourceChokeDefinition=p.chokeDefinition;\n        shadowStream.sourceTrapHeld='),
 ('    shadowStream.sourceTrapTime=', '    shadowStream.sourceChokeTime=p.chokeTimeMs;shadowStream.sourceTrapTime='),
 ('if(p.event==shadowStream.lastEvent&&(p.trapHeld', 'if(p.event==shadowStream.lastEvent&&(p.chokeVariant!=shadowStream.remoteChoke||p.chokeDurationMs!=shadowStream.remoteChokeDuration||p.chokeSpawner!=shadowStream.remoteChokeSpawner||p.chokeDefinition!=shadowStream.remoteChokeDefinition||p.trapHeld'),
 ('    NativeWorldNpc carryTarget{};', '''    NativeWorldNpc chokeTarget{};
    if(p.chokeVariant&&(!worldObservationReady(tick)||!worldCatalogTick||tick-worldCatalogTick>750||
        !resolveCarryIdentity(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,worldCatalog,p.chokeSpawner,p.chokeDefinition,chokeTarget)||
        chokeTarget.handle==probeHandle||chokeTarget.actor==probeActor)){++shadowStreamRejected;return;}
    NativeWorldNpc carryTarget{};'''),
 ('auto trapTimeline=shadowStream.trap;', 'auto trapTimeline=shadowStream.trap;auto chokeTimeline=shadowStream.choke;'),
 ('       !trapTimeline.push(', '       !chokeTimeline.push({mapped,p.event,p.chokeVariant,p.chokeTimeMs,p.chokeDurationMs,p.chokeSpawner,p.chokeDefinition})?23:\n       !trapTimeline.push('),
 ('shadowStream.trap=std::move(trapTimeline);', 'shadowStream.trap=std::move(trapTimeline);shadowStream.choke=std::move(chokeTimeline);'),
 ('    shadowStream.remoteTrapHeld=', '    shadowStream.remoteChoke=p.chokeVariant;shadowStream.remoteChokeDuration=p.chokeDurationMs;shadowStream.remoteChokeSpawner=p.chokeSpawner;shadowStream.remoteChokeDefinition=p.chokeDefinition;\n    shadowStream.remoteTrapHeld=')])
edit('movement_playback.h',[
 ('    unsigned trapStock=', '    unsigned chokeVariant=0;float chokeAmount=0,chokeWeight=0,chokeTransition=0;\n    std::uint64_t chokeEvent=0;std::vector<MovementBank::Joint> chokeFrom,chokeLast;\n    unsigned trapStock='),
 ('        TrapPresentation::Sample trap{};', '''        ChokePresentation::Sample choke{};float chokeAmount=0;
        const bool chokeValid=p.bank.hasChoke()&&shadowStream.choke.sample(pose.time,choke,chokeAmount);
        const bool chokeActive=chokeValid&&choke.variant&&ground&&pose.motionValid&&semantic.state==21;
        if(p.bank.hasChoke()){
            if(chokeActive){
                if(choke.event!=p.chokeEvent){p.chokeFrom=p.chokeLast;p.chokeTransition=0;p.chokeEvent=choke.event;}
                p.chokeVariant=choke.variant;p.chokeAmount=chokeAmount;
                p.chokeTransition=(std::min)(1.f,p.chokeTransition+combatDt/.06f);
            }
            p.chokeWeight+=((chokeActive?1.f:0.f)-p.chokeWeight)*(1-std::exp(-combatDt/(chokeActive?.025f:.10f)));
        }else p.chokeWeight=0;
        TrapPresentation::Sample trap{};'''),
 ('||heartActive||trapActive||carryActive', '||heartActive||trapActive||chokeActive||carryActive'),
 ('        if(p.bank.hasTrap()&&p.trapWeight>.001f){', '''        if(p.bank.hasChoke()&&p.chokeWeight>.001f&&p.chokeVariant){
            auto choke=p.bank.sampleParameter(ChokePresentation::clip(p.chokeVariant),j,p.chokeAmount);
            if(p.chokeFrom.size()==count)choke=MovementBank::blend(p.chokeFrom[j],choke,p.chokeTransition*p.chokeTransition*(3-2*p.chokeTransition));
            if(p.chokeLast.size()!=count)p.chokeLast.resize(count);p.chokeLast[j]=choke;
            v=MovementBank::blend(v,choke,p.chokeWeight);
        }
        if(p.bank.hasTrap()&&p.trapWeight>.001f){'''),
 ('    fprintf(f,"\\\"trapStock', '    fprintf(f,"\\\"chokeVariant\\\":%u,\\\"chokeAmount\\\":%.7f,\\\"chokeWeight\\\":%.7f,\\\"chokeEvent\\\":%llu,",p.chokeVariant,p.chokeAmount,p.chokeWeight,p.chokeEvent);\n    fprintf(f,"\\\"trapStock')])
print('Choke581 source binding prepared; native ownership/runtime verification still required.')
