from pathlib import Path
P=Path(__file__).resolve().parents[2]/'outputs/d2-coop-lab/src'
def edit(name,old,new,count=1):
    p=P/name;s=p.read_text();assert s.count(old)==count,(name,old,s.count(old));p.write_text(s.replace(old,new))
edit('shadow_action_protocol.h','#include "ranged_presentation.h"','#include "ranged_presentation.h"\n#include "carry_presentation.h"')
edit('shadow_action_protocol.h','version=28','version=29')
edit('shadow_action_protocol.h','sizeof(CoopShadowActionPacket)==260','sizeof(CoopShadowActionPacket)==296')
edit('shadow_action_protocol.h','p.version==28','p.version==29')
edit('shadow_action_protocol.h','p.state<=13','p.state<=14')
edit('shadow_action_protocol.h','rangedMechanism=0,rangedShot=0;','rangedMechanism=0,rangedShot=0;\n    std::uint32_t carryVariant=0,carryTimeMs=0,carryDurationMs=0,carryStance=0,carryIncarnation=0;\n    std::uint64_t carrySpawner=0,carryDefinition=0;')
edit('shadow_action_protocol.h','    return ranged&&power&&traversal', '''    const bool carry=CarryPresentation::valid({0,1,p.carryVariant,p.carryTimeMs,p.carryDurationMs,p.carryStance,p.carryIncarnation,p.carrySpawner,p.carryDefinition})&&
        (p.carryVariant?(p.kind==1&&p.state==14&&p.swordState==1):p.state!=14);
    return carry&&ranged&&power&&traversal''')
edit('movement_semantics.h','s.state>13','s.state>14')
edit('shadow_stream.h','#include "native_player_ranged_scene.h"','#include "native_player_ranged_scene.h"\n#include "native_player_carry_scene.h"\n#include "carry_identity.h"')
edit('shadow_stream.h','    NativeKeyholeHandoff keyholeHandoff;','''    NativeKeyholeHandoff keyholeHandoff;
    CarryPresentation carry;
    unsigned sourceCarry=0,sourceCarryIncarnation=0,remoteCarry=0,remoteCarryDuration=0,remoteCarryIncarnation=0,remoteCarryLocalHandle=0;
    std::uint64_t sourceCarrySpawner=0,sourceCarryDefinition=0,remoteCarrySpawner=0,remoteCarryDefinition=0;''')
edit('shadow_stream.h','    if(powerValid)action=10;else if(rangedValid)action=13;','''    NativePlayerCarryScene carry{};
    const bool carryValid=nativeStateValid&&!powerValid&&!rangedValid&&readNativePlayerCarryScene(memoryRead,base,states,carry);
    if(powerValid)action=10;else if(rangedValid)action=13;else if(carryValid)action=14;''')
edit('shadow_stream.h','(powerValid||rangedValid||nativePlayerShadowAction','(powerValid||rangedValid||carryValid||nativePlayerShadowAction')
edit('shadow_stream.h','    if(rangedValid){','''    if(carryValid){
        NativeWorldNpc native{},target{};const auto tick=GetTickCount64();
        if(sword.sword.equipped||carry.character!=localRole+1||!worldObservationReady(tick)||!worldCatalogTick||tick-worldCatalogTick>750||
           !readWorldNpcIdentity(memoryRead,base,observedGame,carry.corpseHandle,native)||native.actor!=carry.corpse||!native.unique||
           !resolveCarryIdentity(memoryRead,base,observedGame,worldCatalog,carryIdentityKey(native.spawnerName),carryIdentityKey(native.definition),target)||target.actor!=carry.corpse)return false;
        if(sceneHandle)*sceneHandle=carry.handle;
        if(airPacket){airPacket->carryVariant=carry.variant;airPacket->carryTimeMs=carry.timeMs;airPacket->carryDurationMs=carry.durationMs;
            airPacket->carryStance=carry.stance;airPacket->carryIncarnation=carry.corpseHandle;
            airPacket->carrySpawner=carryIdentityKey(target.spawnerName);airPacket->carryDefinition=carryIdentityKey(target.definition);}
    }
    if(rangedValid){''')
edit('shadow_stream.h','shadowStream.ranged.clear();','shadowStream.ranged.clear();shadowStream.carry.clear();shadowStream.remoteCarryLocalHandle=0;')
edit('shadow_stream.h','p.rangedVariant!=shadowStream.sourceRanged){','p.rangedVariant!=shadowStream.sourceRanged||p.carryVariant!=shadowStream.sourceCarry||p.carryIncarnation!=shadowStream.sourceCarryIncarnation||p.carrySpawner!=shadowStream.sourceCarrySpawner||p.carryDefinition!=shadowStream.sourceCarryDefinition){')
edit('shadow_stream.h','shadowStream.sourceRanged=p.rangedVariant;++shadowStream.sourceEvent;','shadowStream.sourceRanged=p.rangedVariant;shadowStream.sourceCarry=p.carryVariant;shadowStream.sourceCarryIncarnation=p.carryIncarnation;shadowStream.sourceCarrySpawner=p.carrySpawner;shadowStream.sourceCarryDefinition=p.carryDefinition;++shadowStream.sourceEvent;')
edit('shadow_stream.h','p.rangedDurationMs!=shadowStream.remoteRangedDuration)){','p.rangedDurationMs!=shadowStream.remoteRangedDuration||p.carryVariant!=shadowStream.remoteCarry||p.carryDurationMs!=shadowStream.remoteCarryDuration||p.carryIncarnation!=shadowStream.remoteCarryIncarnation||p.carrySpawner!=shadowStream.remoteCarrySpawner||p.carryDefinition!=shadowStream.remoteCarryDefinition)){')
edit('shadow_stream.h','    // Use the already-established root clock mapping, without modifying it.','''    NativeWorldNpc carryTarget{};
    if(p.carryVariant&&(!worldObservationReady(tick)||!worldCatalogTick||tick-worldCatalogTick>750||
        !resolveCarryIdentity(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,worldCatalog,p.carrySpawner,p.carryDefinition,carryTarget))){
        ++shadowStreamRejected;return;
    }
    // Use the already-established root clock mapping, without modifying it.''')
edit('shadow_stream.h','auto rangedTimeline=shadowStream.ranged;','auto rangedTimeline=shadowStream.ranged;auto carryTimeline=shadowStream.carry;')
edit('shadow_stream.h','       !movementTimeline.push','       !carryTimeline.push({mapped,p.event,p.carryVariant,p.carryTimeMs,p.carryDurationMs,p.carryStance,p.carryIncarnation,p.carrySpawner,p.carryDefinition})||\n       !movementTimeline.push')
edit('shadow_stream.h','shadowStream.ranged=std::move(rangedTimeline);','shadowStream.ranged=std::move(rangedTimeline);shadowStream.carry=std::move(carryTimeline);shadowStream.remoteCarryLocalHandle=carryTarget.handle;')
edit('shadow_stream.h','shadowStream.remoteRangedDuration=p.rangedDurationMs;','shadowStream.remoteRangedDuration=p.rangedDurationMs;shadowStream.remoteCarry=p.carryVariant;shadowStream.remoteCarryDuration=p.carryDurationMs;shadowStream.remoteCarryIncarnation=p.carryIncarnation;shadowStream.remoteCarrySpawner=p.carrySpawner;shadowStream.remoteCarryDefinition=p.carryDefinition;')
# Carry motion remains native grounded locomotion; the owned scene is still
# required independently for the gesture and partner identity.
edit('native_player_states.h','inline bool nativePlayerMotion(','''inline bool nativePlayerGroundCarryStates(const NativePlayerStates& s,std::uintptr_t base){
    if(!s.defaultMode||s.player.current>1||s.active.size()<2||s.active.size()>3)return false;
    unsigned grounded=0,carrying=0,overlay=0;
    for(const auto& a:s.active){
        if((s.player.current==0&&a.id==0&&a.vtable==base+0x1c85628)||(s.player.current==1&&a.id==6&&a.vtable==base+0x1c84258)){++grounded;continue;}
        if((a.id==45&&a.vtable==base+0x1c77748)||(a.id==46&&a.vtable==base+0x1c790c8)){++carrying;continue;}
        if((a.id==53&&a.vtable==base+0x1c79cf8)||(a.id==81&&a.vtable==base+0x1c7df88)){++overlay;continue;}
        return false;
    }
    return grounded==1&&carrying==1&&overlay<=1;
}
inline bool nativePlayerMotion(''')
edit('native_player_states.h','    // Observed base gun/crossbow fire','    if(nativePlayerGroundCarryStates(s,base)){motion=s.player.current;return true;}\n    // Observed base gun/crossbow fire')
print('Carry protocol 29 / 296 bytes and read-only peer identity connected')
