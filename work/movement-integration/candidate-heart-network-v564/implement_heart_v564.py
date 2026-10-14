from pathlib import Path
import shutil
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';W=R/'work/movement-integration';B=W/'heart-binding-pre-v564';B.mkdir(exist_ok=False)
files=['native_player_states.h','shadow_stream.h','shadow_action_protocol.h','movement_semantics.h','movement_bank.h','movement_playback.h','ranged_prop.h','engine_bridge.cpp']
for name in files:shutil.copy2(S/name,B/name)
def edit(name,fn):
 p=S/name;p.write_text(fn(p.read_text()))
predicate='''inline bool nativePlayerGroundHeartStates(const NativePlayerStates& s,std::uintptr_t base){
    if(s.player.logic!=1||!s.defaultMode||s.player.current>1||s.active.empty()||s.active.size()>4)return false;
    unsigned ground=0,heart=0,right=0,left=0,draw=0;
    for(const auto& a:s.active){
        if((s.player.current==0&&a.id==0&&a.vtable==base+0x1c85628)||(s.player.current==1&&a.id==6&&a.vtable==base+0x1c84258))++ground;
        else if(a.id==67&&a.vtable==base+0x1c80b78)++heart;
        else if(a.id==53&&a.vtable==base+0x1c79cf8)++right;
        else if(a.id==64&&a.vtable==base+0x1c79cf8)++left;
        else if(a.id==81&&a.vtable==base+0x1c7df88)++draw;
        else return false;
    }
    return ground==1&&heart==1&&right<=1&&left<=1&&draw<=1;
}
'''
edit('native_player_states.h',lambda s:s.replace('inline bool nativePlayerMotion(',predicate+'inline bool nativePlayerMotion(').replace('    if(nativePlayerGroundDrawRequestStates(s,base)){motion=', '    if(nativePlayerGroundHeartStates(s,base)){motion=s.player.current;return true;}\n    if(nativePlayerGroundDrawRequestStates(s,base)){motion='))
g=(S/'native_player_grenade_scene.h').read_text()
a=g.index('inline unsigned nativeGrenadeVariant');b=g.index('template<class Read>',a)
g=g[:a]+'''inline unsigned nativeGrenadeVariant(const std::string& asset,const std::string& sub,unsigned duration){
    if(sub!="Scene_1")return 0;
    if(duration==533&&asset=="midnight/gameplay/player/base/gadgets/heart_equip.midnightscene")return 1;
    if(duration==333&&asset=="midnight/gameplay/player/base/gadgets/heart_unequip.midnightscene")return 2;
    if(duration==1000&&asset=="midnight/gameplay/player/base/gadgets/heart_use.midnightscene")return 3;
    return 0;
}
'''+g[b:]
a=g.index('    if(states.player.logic');b=g.index('    const auto game=',a)
g=g[:a]+'''    unsigned motion=0;
    if(!nativePlayerMotion(states,base,motion)||motion>1||states.player.current!=motion)return false;
    for(const auto& a:states.active)if(a.id!=0&&a.id!=6&&a.id!=53&&a.id!=64&&a.id!=67&&a.id!=81)return false;
'''+g[b:]
a=g.index('    // v54 native action');b=g.index('    const unsigned expectedBindings',a)
g=g[:a]+'''    if(!s.variant||!readNativePlayerGrenade(read,base,s.grenade)||s.grenade.quantity!=1||
       s.grenade.player.sword.game!=states.player.game||s.grenade.player.sword.actor!=states.player.player)return false;
    if(s.variant==3&&(!s.grenade.equipped||!nativePlayerGroundHeartStates(states,base)))return false;
    const unsigned expectedItems=s.variant==3?1u:0u;
    const unsigned expectedPlayers=0;
'''+g[b:]
g=g.replace('grenade','heart').replace('Grenade','Heart').replace('gadget_heart_','heart_')
g=g.replace('||finalItem.lowThrowCandidate!=s.heart.lowThrowCandidate','')
g=g.replace('// v52 actual native X input, both roles: equip 500 ms binds left arm+item;\n// unequip 233 ms binds left arm alone. Scene64 can finish before the scene,\n// while Scene53 refers to the sword. Resolve through exact live bindings.', '// Native563: Heart equip533/unequip333 bind the owned left arm; use1000\n// binds the same arm plus exact Heart item. No gameplay events are replayed.')
(S/'native_player_heart_scene.h').write_text(g)
p=(S/'grenade_presentation.h').read_text().replace('Grenade','Heart').replace('grenade','Heart')
p=p.replace('constexpr unsigned d[]={0,500,233,500,1333,400,566,400,400};return v<=8?d[v]:0;', 'constexpr unsigned d[]={0,533,333,1000};return v<=3?d[v]:0;').replace('return 251+v','return 336+v').replace('s.variant<=8','s.variant<=3')
a=p.index('    static bool visible');b=p.index('    void clear()',a)
p=p[:a]+'''    static bool visible(unsigned held,unsigned variant,float amount){
        if(!held||variant>3||!std::isfinite(amount)||amount<0||amount>1)return false;
        if(variant==1)return amount>=.22f;
        if(variant==2)return amount<=.75f;
        return true;
    }
'''+p[b:];(S/'heart_presentation.h').write_text(p)
def protocol(s):
 s=s.replace('#include "grenade_presentation.h"','#include "grenade_presentation.h"\n#include "heart_presentation.h"').replace('version=59','version=60').replace('p.version==59','p.version==60').replace('p.state<=18','p.state<=19').replace(')==336',')==352')
 s=s.replace('    std::uint32_t stunVariant=', '    std::uint32_t heartHeld=0,heartVariant=0,heartTimeMs=0,heartDurationMs=0;\n    std::uint32_t stunVariant=')
 s=s.replace('    return unbound&&', '''    const bool heart=HeartPresentation::valid({0,1,p.heartHeld,p.heartVariant,p.heartTimeMs,p.heartDurationMs})&&
        (p.heartVariant?p.state==19:p.state!=19)&&(!p.heartHeld||(p.kind==1&&(p.state==0||p.state==19)&&
        !p.rangedKind&&!p.grenadeHeld&&!p.powerVariant&&!p.combatVariant&&!p.carryVariant&&!p.stunVariant&&!p.airStage));
    return heart&&unbound&&''')
 return s
edit('shadow_action_protocol.h',protocol)
edit('movement_semantics.h',lambda s:s.replace('s.state>18','s.state>19'))
def bank(s):
 s=s.replace('32+336*','32+340*').replace('335,336};return v<=59?', '335,336,340};return v<=60?').replace('v<=59;++v','v<=60;++v')
 s=s.replace('    bool hasShadowWalkSneak()', '    bool hasHeart()const{return version>=60;}\n    bool hasShadowWalkSneak()')
 return s
edit('movement_bank.h',bank)
def stream(s):
 s=s.replace('#include "native_player_grenade_scene.h"','#include "native_player_grenade_scene.h"\n#include "native_player_heart_scene.h"')
 s=s.replace('CarryPresentation carry;GrenadePresentation grenade;', 'CarryPresentation carry;GrenadePresentation grenade;HeartPresentation heart;')
 s=s.replace('    unsigned sourceStun=', '    unsigned sourceHeartHeld=0,sourceHeart=0,remoteHeartHeld=0,remoteHeart=0,remoteHeartDuration=0;\n    unsigned sourceStun=')
 pos=s.index('    NativePlayerCarryScene carry{};')
 s=s[:pos]+'''    NativePlayerHeartScene heart{};NativePlayerHeart heldHeart{};
    const bool heartValid=nativeStateValid&&!powerValid&&!rangedValid&&!grenadeValid&&readNativePlayerHeartScene(memoryRead,base,states,heart);
    const bool heartIdle=nativeStateValid&&!heartValid&&nativePlayerGroundHeartStates(states,base)&&states.active.size()==2&&
        readNativePlayerHeart(memoryRead,base,heldHeart,nullptr,true)&&heldHeart.quantity==1;
'''+s[pos:]
 s=s.replace('if(possessionOccupied)action=17;', 'if(heartValid)action=19;else if(heartIdle)action=0;else if(possessionOccupied)action=17;')
 s=s.replace('(possessionOccupied||stunValid||powerValid||', '(heartValid||heartIdle||possessionOccupied||stunValid||powerValid||')
 pos=s.index('    if(grenadeValid){')
 s=s[:pos]+'''    if(heartValid||heartIdle){
        const auto& item=heartValid?heart.heart:heldHeart;
        if(item.player.character!=localRole+1||item.player.sword.game!=observedGame||item.player.sword.actor!=observedPlayer)return false;
        if(heartValid&&sceneHandle)*sceneHandle=heart.handle;
        if(airPacket){airPacket->heartHeld=1;airPacket->heartVariant=heart.variant;airPacket->heartTimeMs=heart.timeMs;airPacket->heartDurationMs=heart.durationMs;}
    }
'''+s[pos:]
 s=s.replace('shadowStream.grenade.clear();', 'shadowStream.grenade.clear();shadowStream.heart.clear();')
 s=s.replace('if(action!=shadowStream.sourceState||', 'if(p.heartHeld!=shadowStream.sourceHeartHeld||p.heartVariant!=shadowStream.sourceHeart||action!=shadowStream.sourceState||')
 s=s.replace('        shadowStream.sourceState=action;', '        shadowStream.sourceHeartHeld=p.heartHeld;shadowStream.sourceHeart=p.heartVariant;shadowStream.sourceState=action;')
 s=s.replace('if(p.event==shadowStream.lastEvent&&(', 'if(p.event==shadowStream.lastEvent&&(p.heartHeld!=shadowStream.remoteHeartHeld||p.heartVariant!=shadowStream.remoteHeart||p.heartDurationMs!=shadowStream.remoteHeartDuration||')
 s=s.replace('auto grenadeTimeline=shadowStream.grenade;', 'auto grenadeTimeline=shadowStream.grenade;auto heartTimeline=shadowStream.heart;')
 s=s.replace('       !grenadeTimeline.push(', '       !heartTimeline.push({mapped,p.event,p.heartHeld,p.heartVariant,p.heartTimeMs,p.heartDurationMs})?19:\n       !grenadeTimeline.push(')
 s=s.replace('shadowStream.grenade=std::move(grenadeTimeline);', 'shadowStream.grenade=std::move(grenadeTimeline);shadowStream.heart=std::move(heartTimeline);')
 s=s.replace('    shadowStream.remoteGrenadeHeld=', '    shadowStream.remoteHeartHeld=p.heartHeld;shadowStream.remoteHeart=p.heartVariant;shadowStream.remoteHeartDuration=p.heartDurationMs;\n    shadowStream.remoteGrenadeHeld=')
 return s
edit('shadow_stream.h',stream)
def playback(s):
 a=s.index('    unsigned grenadeHeld=');b=s.index('\n',s.index('std::uint64_t grenadeEvent=',a))
 fields=s[a:b].replace('grenade','heart').replace('Grenade','Heart');s=s[:b]+'\n'+fields+s[b:]
 a=s.index('        GrenadePresentation::Sample');b=s.index('        EquipmentPresentation::Sample',a)
 code=s[a:b].replace('grenade','heart').replace('Grenade','Heart').replace('p.bank.hasCrouchedHeart()&&crouched','crouched').replace('||combatAir','').replace('clips[251]','clips[336]')
 s=s[:b]+code+s[b:]
 s=s.replace('||grenadeActive||carryActive','||grenadeActive||heartActive||carryActive')
 a=s.index('        if(p.bank.hasGrenade()&&p.grenadeWeight');b=s.index('\n        }',a)+10
 code=s[a:b].replace('grenade','heart').replace('Grenade','Heart')
 code='\n'.join(line for line in code.splitlines() if 'hasCrouchedHeart' not in line)
 code=code.replace('(std::max)(p.heartMoveMix,p.clock.airWeight)','(std::max)(p.heartMoveMix,p.clock.stanceMix)')
 s=s[:b]+'\n'+code+s[b:]
 line=next(line for line in s.splitlines() if 'fprintf(f,' in line and 'p.grenadeHeld,p.grenadeVariant' in line)
 s=s.replace(line,line+'\n'+line.replace('grenade','heart').replace('Grenade','Heart'))
 return s
edit('movement_playback.h',playback)
print('Heart state19, exact owned-scene reader, bank60 and presentation timeline prepared.')
