from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';P=R/'work/natural-moveset';W=R/'work/movement-integration'
p=S/'native_player_possession_scene.h';s=p.read_text()
s=s.replace('#include "native_player_power_scene.h"','#include "native_player_power_scene.h"\n#include "native_player_possession_target.h"')
s=s.replace('unsigned duration){','unsigned duration,bool observeRelease=false){',1)
s=s.replace('    return 0;\n}', '    if(observeRelease&&asset==prefix+"out.midnightscene"&&duration==400)return 28;\n    return 0;\n}',1)
s=s.replace('NativePlayerPowerScene& out){','NativePlayerPowerScene& out,bool observeRelease=false){',1)
s=s.replace('    NativePossessionItem item{};NativePlayerPowerScene s{};', '''    NativePossessionItem item{};NativePlayerPowerScene s{};NativePossessionTarget target{};
    const bool entering=observeRelease&&!states.defaultMode&&states.player.current==0&&states.active.size()==1&&
        states.active[0].id==26&&states.active[0].vtable==base+0x1c79878&&
        readNativePossessionTarget(read,base,target)&&target.hasTarget&&target.state==0&&
        target.player.component==states.component&&target.player.mode==states.mode&&target.player.player.game==states.player.game;
''')
s=s.replace('states.player.current!=0||!states.defaultMode||','states.player.current!=0||(!states.defaultMode&&!entering)||',1)
s=s.replace('        else return false;','        else if(entering&&a.id==26&&a.vtable==base+0x1c79878)continue;\n        else return false;',1)
s=s.replace('    if(ground!=1||casting!=1)return false;','    if(!entering&&(ground!=1||casting!=1))return false;')
s=s.replace('nativePossessionVariant(asset,sub,duration);if(!variant)', 'nativePossessionVariant(asset,sub,duration,observeRelease);if(!variant||(entering?variant!=28:variant==28))')
s=s.replace('||!finalStates.defaultMode||finalStates.active.size()', '||finalStates.defaultMode!=states.defaultMode||finalStates.active.size()')
anchor='    s.power=item;out=std::move(s);return true;'
s=s.replace(anchor,'''    if(entering){NativePossessionTarget last{};
        if(!readNativePossessionTarget(read,base,last)||!last.hasTarget||last.state!=0||last.player.component!=states.component||last.player.mode!=states.mode||
           last.internal!=target.internal||last.target!=target.target||last.entity!=target.entity||last.handle!=target.handle||last.generation!=target.generation)return false;
    }
'''+anchor)
s=s.replace('//until separately observed. Read-only; no remote power or damage events.', '//until separately observed.329 adds probe-only400ms release under exact\n//EnterPossessedHost26/internal0 plus generation-validated NPC target. Default\n//runtime calls keep release disabled; no remote power or damage events.')
p.write_text(s)
p=S/'runtime_probe.cpp';s=p.read_text();s=s.replace('readNativePlayerPossessionScene(reader,base,states,s);','readNativePlayerPossessionScene(reader,base,states,s,true);');p.write_text(s)
s=(P/'Observe-NativePossessionTarget-v328.ps1').read_text();a=" $accepted|ConvertTo-Json -Depth 8|Set-Content \"$out/accepted-scenes.json\""
assert a in s;s=s.replace(a," if(@($accepted|Where-Object {$_.variant-eq28-and$_.durationMs-eq400-and($_.activeStates-join',')-eq'26'}).Count-lt2){throw 'Strict successful release with exact target and EnterPossessedHost26 not repeatedly observed'}\n"+a)
s=s.replace('Strict reader observes pre-transfer cast; unsupported target-transfer contexts remain excluded from presentation','Strict probe observes entry/aim and successful release400ms with EnterPossessedHost26/internal0 and exact NPC target; runtime transport remains entry/aim only')
(P/'Observe-NativePossessionTarget-v329.ps1').write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text().replace('Observe-NativePossessionTarget-v328.ps1','Observe-NativePossessionTarget-v329.ps1');p.write_text(s)
print('Prepared probe-only release reader; runtime default keeps variant28 excluded')
