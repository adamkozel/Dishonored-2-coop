from pathlib import Path
import shutil
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';W=R/'work/movement-integration'
B=W/'trap-binding-pre-v569';B.mkdir(exist_ok=False)
for p in S.iterdir():
 if p.is_file() and p.suffix in ('.h','.cpp'):shutil.copy2(p,B/p.name)
def edit(n,f):
 p=S/n;p.write_text(f(p.read_text()))

predicate='''// Exact native568 observations for both roles and both placeable gadgets.
inline bool nativePlayerGroundTrapStates(const NativePlayerStates& s,std::uintptr_t base){
    if(s.player.logic!=1||!s.defaultMode||s.player.current>1||s.active.empty()||s.active.size()>5)return false;
    unsigned ground=0,place=0,ready=0,right=0,left=0,draw=0;
    for(const auto& a:s.active){
        if((s.player.current==0&&a.id==0&&a.vtable==base+0x1c85628)||(s.player.current==1&&a.id==6&&a.vtable==base+0x1c84258))++ground;
        else if(a.id==60&&a.vtable==base+0x1c7c098)++place;
        else if(a.id==61&&a.vtable==base+0x1c3db88)++ready;
        else if(a.id==53&&a.vtable==base+0x1c79cf8)++right;
        else if(a.id==64&&a.vtable==base+0x1c79cf8)++left;
        else if(a.id==81&&a.vtable==base+0x1c7df88)++draw;
        else return false;
    }
    return ground==1&&place+ready==1&&right<=1&&left<=1&&draw<=1;
}
'''
edit('native_player_states.h',lambda s:s.replace('inline bool nativePlayerMotion(',predicate+'inline bool nativePlayerMotion(').replace('    if(nativePlayerGroundHeartStates(s,base))','    if(nativePlayerGroundTrapStates(s,base)){motion=s.player.current;return true;}\n    if(nativePlayerGroundHeartStates(s,base))'))

s=(S/'native_player_heart_scene.h').read_text().replace('Heart','Trap').replace('heart','trap').replace('native_player_trap.h','native_player_placeable.h').replace('NativePlayerTrap trap','NativePlayerPlaceable trap').replace('NativePlayerTrap finalItem','NativePlayerPlaceable finalItem')
a=s.index('inline unsigned nativeTrapVariant');b=s.index('template<class Read>',a)
s=s[:a]+'''inline unsigned nativeTrapVariant(const std::string& asset,const std::string& sub,unsigned duration){
    const std::string prefix="midnight/gameplay/player/base/gadgets/gadget_stunmine_";
    if(sub=="Scene_1"){
        if(duration==500&&asset==prefix+"equip.midnightscene")return 1;
        if(duration==500&&asset==prefix+"unequip.midnightscene")return 2;
        if(duration==300&&asset==prefix+"place.midnightscene")return 5;
        if(duration==333&&asset==prefix+"ooa.midnightscene")return 6;
    }
    if(asset==prefix+"ready.midnightscene"){
        if(duration==200&&sub=="In")return 3;
        if(duration==666&&sub=="Looping")return 4;
    }
    return 0; // Reload/ready-out/slide are not inferred from unobserved scenes.
}
'''+s[b:]
s=s.replace('a.id!=67','a.id!=60&&a.id!=61').replace('gadgets/trap_','gadgets/gadget_stunmine_')
a=s.index('    if(!s.variant||');b=s.index('    const unsigned expectedBindings',a)
s=s[:a]+'''    if(!s.variant)return false;
    // Prefer the actual selected left-slot item. An unselected holster scene
    // is accepted only with one unique acquired trap; ambiguous inventories
    // fall back to grounded presentation instead of guessing an item kind.
    NativePlayerPlaceable selected{},unique{};unsigned selectedCount=0,acquiredCount=0;
    for(unsigned kind=1;kind<=2;++kind){NativePlayerPlaceable p{};
        if(!readNativePlayerPlaceable(read,base,kind,p))return false;
        if(p.component){unique=p;++acquiredCount;if(p.equipped){selected=p;++selectedCount;}}
    }
    if(selectedCount==1)s.trap=selected;
    else if(!selectedCount&&s.variant==2&&acquiredCount==1)s.trap=unique;
    else return false;
    if(s.trap.player.sword.game!=states.player.game||s.trap.player.sword.actor!=states.player.player)return false;
    const unsigned expectedItems=s.variant==5?1u:0u;
    const unsigned expectedPlayers=(s.variant==5||s.variant==6)?1u:0u;
'''+s[b:]
s=s.replace('readNativePlayerTrap(read,base,finalItem)','readNativePlayerPlaceable(read,base,s.trap.kind,finalItem)')
s=s.replace('// Native563: Trap equip533/unequip333 bind the owned left arm; use1000\n// binds the same arm plus exact Trap item. No gameplay events are replayed.', '// Native568: owned left-arm scenes; placement adds item+player, empty adds\n// player. Both gadget types share clocks. No placement/damage events replayed.')
(S/'native_player_trap_scene.h').write_text(s)

s=(S/'heart_presentation.h').read_text().replace('Heart','Trap').replace('heart','trap')
s=s.replace('unsigned held=0,variant=0,elapsed=0,duration=0','unsigned held=0,variant=0,elapsed=0,duration=0,stock=0')
s=s.replace('constexpr unsigned d[]={0,533,333,1000};return v<=3?d[v]:0;','constexpr unsigned d[]={0,500,500,200,666,300,333};return v<=6?d[v]:0;').replace('return 336+v','return 340+v')
s=s.replace('s.held<=1&&s.variant<=3','s.held<=2&&s.stock<=3&&(s.held||!s.stock)&&s.variant<=6')
s=s.replace('unsigned held,unsigned variant,float amount','unsigned held,unsigned variant,float amount,unsigned stock').replace('variant>3','variant>6')
s=s.replace('        if(variant==1)', '        if(variant==6||(!stock&&variant!=5))return false;\n        if(variant==5)return amount<236.f/300.f;\n        if(variant==1)')
s=s.replace('s.held!=p.held||','s.held!=p.held||s.stock!=p.stock||')
(S/'trap_presentation.h').write_text(s)

def protocol(s):
 s=s.replace('#include "heart_presentation.h"','#include "heart_presentation.h"\n#include "trap_presentation.h"').replace('version=60','version=61').replace('p.version==60','p.version==61').replace('p.state<=19','p.state<=20').replace(')==352',')==372')
 s=s.replace('    std::uint32_t heartHeld=', '    std::uint32_t trapHeld=0,trapVariant=0,trapTimeMs=0,trapDurationMs=0,trapStock=0;\n    std::uint32_t heartHeld=')
 s=s.replace('    return heart&&', '''    const bool trap=TrapPresentation::valid({0,1,p.trapHeld,p.trapVariant,p.trapTimeMs,p.trapDurationMs,p.trapStock})&&
        (p.trapVariant?p.state==20:p.state!=20)&&(!p.trapHeld||(p.kind==1&&(p.state==0||p.state==20)&&
        !p.heartHeld&&!p.rangedKind&&!p.grenadeHeld&&!p.powerVariant&&!p.combatVariant&&!p.carryVariant&&!p.stunVariant&&!p.airStage));
    return trap&&heart&&''')
 return s
edit('shadow_action_protocol.h',protocol)
edit('movement_semantics.h',lambda s:s.replace('s.state>19','s.state>20'))
edit('movement_bank.h',lambda s:s.replace('32+340*','32+347*').replace('336,340};return v<=60?','336,340,347};return v<=61?').replace('v<=60;++v','v<=61;++v').replace('    bool hasHeart()', '    bool hasTrap()const{return version>=61;}\n    bool hasHeart()'))

def stream(s):
 s=s.replace('#include "native_player_heart_scene.h"','#include "native_player_heart_scene.h"\n#include "native_player_trap_scene.h"')
 s=s.replace('HeartPresentation heart;', 'HeartPresentation heart;TrapPresentation trap;')
 s=s.replace('    unsigned sourceHeartHeld=', '    unsigned sourceTrapHeld=0,sourceTrap=0,sourceTrapTime=0,sourceTrapStock=0,remoteTrapHeld=0,remoteTrap=0,remoteTrapDuration=0,remoteTrapStock=0;\n    unsigned sourceHeartHeld=')
 pos=s.index('    NativePlayerCarryScene carry{};')
 s=s[:pos]+'''    NativePlayerTrapScene trap{};NativePlayerPlaceable heldTrap{};
    const bool trapValid=nativeStateValid&&!powerValid&&!rangedValid&&!grenadeValid&&!heartValid&&!heartIdle&&readNativePlayerTrapScene(memoryRead,base,states,trap);
    bool trapIdle=false;
    if(nativeStateValid&&!trapValid&&!heartValid&&!heartIdle&&states.active.size()==1&&states.player.current<=1){
        for(unsigned kind=1;kind<=2;++kind)if(readNativePlayerPlaceable(memoryRead,base,kind,heldTrap,nullptr,true)){trapIdle=true;break;}
    }
'''+s[pos:]
 s=s.replace('if(heartValid)action=19;', 'if(trapValid)action=20;else if(trapIdle)action=0;else if(heartValid)action=19;')
 s=s.replace('(heartValid||heartIdle||possessionOccupied', '(trapValid||trapIdle||heartValid||heartIdle||possessionOccupied')
 pos=s.index('    if(heartValid||heartIdle){')
 s=s[:pos]+'''    if(trapValid||trapIdle){
        const auto& item=trapValid?trap.trap:heldTrap;
        if(item.player.character!=localRole+1||item.player.sword.game!=observedGame||item.player.sword.actor!=observedPlayer)return false;
        if(trapValid&&sceneHandle)*sceneHandle=trap.handle;
        if(airPacket){airPacket->trapHeld=item.kind;airPacket->trapStock=item.quantity;airPacket->trapVariant=trap.variant;airPacket->trapTimeMs=trap.timeMs;airPacket->trapDurationMs=trap.durationMs;}
    }
'''+s[pos:]
 s=s.replace('shadowStream.heart.clear();','shadowStream.heart.clear();shadowStream.trap.clear();')
 s=s.replace('if(p.heartHeld!=shadowStream.sourceHeartHeld||','if(p.trapHeld!=shadowStream.sourceTrapHeld||p.trapStock!=shadowStream.sourceTrapStock||p.trapVariant!=shadowStream.sourceTrap||(p.trapVariant==4&&p.trapTimeMs<shadowStream.sourceTrapTime)||p.heartHeld!=shadowStream.sourceHeartHeld||')
 s=s.replace('        shadowStream.sourceHeartHeld=', '        shadowStream.sourceTrapHeld=p.trapHeld;shadowStream.sourceTrapStock=p.trapStock;shadowStream.sourceTrap=p.trapVariant;shadowStream.sourceHeartHeld=')
 s=s.replace('    shadowStream.sourceGrenadeTime=', '    shadowStream.sourceTrapTime=p.trapTimeMs;shadowStream.sourceGrenadeTime=')
 s=s.replace('if(p.event==shadowStream.lastEvent&&(', 'if(p.event==shadowStream.lastEvent&&(p.trapHeld!=shadowStream.remoteTrapHeld||p.trapVariant!=shadowStream.remoteTrap||p.trapStock!=shadowStream.remoteTrapStock||p.trapDurationMs!=shadowStream.remoteTrapDuration||')
 s=s.replace('auto heartTimeline=shadowStream.heart;', 'auto heartTimeline=shadowStream.heart;auto trapTimeline=shadowStream.trap;')
 s=s.replace('       !heartTimeline.push(', '       !trapTimeline.push({mapped,p.event,p.trapHeld,p.trapVariant,p.trapTimeMs,p.trapDurationMs,p.trapStock})?22:\n       !heartTimeline.push(')
 s=s.replace('shadowStream.heart=std::move(heartTimeline);','shadowStream.heart=std::move(heartTimeline);shadowStream.trap=std::move(trapTimeline);')
 s=s.replace('    shadowStream.remoteHeartHeld=', '    shadowStream.remoteTrapHeld=p.trapHeld;shadowStream.remoteTrapStock=p.trapStock;shadowStream.remoteTrap=p.trapVariant;shadowStream.remoteTrapDuration=p.trapDurationMs;\n    shadowStream.remoteHeartHeld=')
 return s
edit('shadow_stream.h',stream)

def playback(s):
 a=s.index('    unsigned heartHeld=');b=s.index('\n',s.index('std::uint64_t heartEvent=',a))
 s=s[:b]+'\n'+s[a:b].replace('heart','trap').replace('Heart','Trap').replace('unsigned trapHeld=0','unsigned trapStock=0,trapHeld=0')+s[b:]
 a=s.index('        HeartPresentation::Sample');b=s.index('        EquipmentPresentation::Sample',a)
 s=s[:b]+s[a:b].replace('heart','trap').replace('Heart','Trap').replace('clips[336]','clips[340]').replace('p.trapHeld=trap.held;', 'p.trapHeld=trap.held;p.trapStock=trap.stock;')+s[b:]
 s=s.replace('||heartActive||carryActive','||heartActive||trapActive||carryActive')
 a=s.index('        if(p.bank.hasHeart()&&p.heartWeight');b=s.index('\n        }',a)+10
 s=s[:b]+'\n'+s[a:b].replace('heart','trap').replace('Heart','Trap')+s[b:]
 line=next(l for l in s.splitlines() if 'fprintf(f,' in l and 'p.heartHeld,p.heartVariant' in l)
 s=s.replace(line,line+'\n'+line.replace('heart','trap').replace('Heart','Trap')+'\n    fprintf(f,"\\\"trapStock\\\":%u,",p.trapStock);')
 return s
edit('movement_playback.h',playback)

# Independently parked visual entities permit item-kind switches without ever
# substituting a native gameplay component. Each is bound to the remote wrist.
s=(S/'heart_prop.h').read_text().replace('heart','trap').replace('Heart','Trap')
s=s.replace('RangedProp trapProp;', 'RangedProp trapProps[2];RangedProp* currentTrapProp=&trapProps[0];')
s=s.replace('auto& p=trapProp;', 'auto& p=*currentTrapProp;').replace('const auto& p=trapProp;', 'const auto& p=*currentTrapProp;')
s=s.replace('variant>3','variant>6').replace('rangedPropDefinition(4)','rangedPropDefinition(unsigned(currentTrapProp-trapProps)+5)').replace('p.kind=4','p.kind=unsigned(currentTrapProp-trapProps)+5')
s=s.replace('unsigned variant=0;float amount=0;', 'unsigned kind=0,stock=0,variant=0;float amount=0;')
s=s.replace('variant=movement.trapVariant;', 'kind=movement.trapHeld;stock=movement.trapStock;variant=movement.trapVariant;')
s=s.replace('visible(movement.trapHeld,variant,amount)', 'visible(movement.trapHeld,variant,amount,stock)')
# Acquire the active prop reference only after choosing its native gadget kind.
s=s.replace('    auto& p=*currentTrapProp;auto request=', '    auto request=')
s=s.replace('    const bool automatic=presented&&!request;', '''    const bool automatic=presented&&!request;
    if(automatic&&kind>=1&&kind<=2&&currentTrapProp!=&trapProps[kind-1]){
        hideTrapProp();if(currentTrapProp->pendingHide)return;currentTrapProp=&trapProps[kind-1];
    }
    auto& p=*currentTrapProp;''')
s=s.replace('p.handle,p.parentHandle,p.joint,', 'p.handle,p.parentHandle,p.joint,')
(S/'trap_prop.h').write_text(s)
edit('ranged_prop.h',lambda s:s.replace('    return "";','    if(kind==5)return "models/weapons/d2coop/stunmine_prop.def";\n    if(kind==6)return "models/weapons/d2coop/springrazor_prop.def";\n    return "";',1))
edit('engine_bridge.cpp',lambda s:s.replace('#include "heart_prop.h"','#include "heart_prop.h"\n#include "trap_prop.h"').replace('serviceHeartProp(tick);','serviceHeartProp(tick);serviceTrapProp(tick);').replace('writeHeartProp();','writeHeartProp();writeTrapProp();'))
print('Prepared trap state20 and owned scene/timeline/body/visual prop integration; no native placement events replayed.')
