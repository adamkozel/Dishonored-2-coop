"""Create a separate exact-build Bend Time probe and finite acquisition fixture."""
from pathlib import Path
P=Path(__file__).resolve().parents[2];S=P/'outputs/d2-coop-lab/src'
reader=(S/'native_player_windblast_item.h').read_text()
reader=reader.replace('Windblast','BendTime').replace('windblast','bendtime').replace('0x1b53488','0x1b9b748').replace('0x1b53348','0x1b9b608')
reader=reader.replace('bool instantiated=false;','bool instantiated=false,powerActive=false;')
reader=reader.replace('p.instantiated=true;return true;', '''unsigned char active=0;
    if(!read(p.component+0xd0,&active,1)||active>1)return false;
    p.powerActive=active!=0;p.instantiated=true;return true;''')
reader=reader.replace('a.quantity!=b.quantity||a.flags!=b.flags','a.quantity!=b.quantity||a.flags!=b.flags||a.powerActive!=b.powerActive')
reader=reader.replace('// BendTime equipment has its own native entity; no player vision manager.','// Bend Time has its own native equipment entity.')
(S/'native_player_bendtime_item.h').write_text(reader)
fixture=(S/'windblast_fixture.h').read_text().replace('Windblast','BendTime').replace('windblast','bendtime')
fixture=fixture.replace('powerFixture.until||darkVisionFixture.until||','powerFixture.until||darkVisionFixture.until||windblastFixture.until||')
fixture=fixture.replace('p.quantity||p.equipped||p.started||p.player.sword.slots[1]','p.quantity||p.equipped||p.started||p.powerActive||p.player.sword.slots[1]')
fixture=fixture.replace('}else if(p.equipped&&!f.unequipSent){','''}else if(p.powerActive){
            // Do not remove the equipment while its native time request lives.
            // Ordinary controller toggle / native timer must end the power.
            // The fixed cleanup deadline still applies; caller then reloads
            // its isolated checkpoint if native cleanup cannot be verified.
            bendtimeFixtureResult="waiting_for_native_bendtime_end";
        }else if(p.equipped&&!f.unequipSent){''')
fixture=fixture.replace('// may fire the native power. Never call native blast/spawn/damage functions here.',
 '// may toggle the native power. Never call time-scale, FX or gameplay functions here.')
(S/'bendtime_fixture.h').write_text(fixture)
bridge=(S/'engine_bridge.cpp').read_text()
def replace_once(text,old,new):
    assert text.count(old)==1,(old,text.count(old));return text.replace(old,new)
bridge=replace_once(bridge,'#include "windblast_fixture.h"','#include "windblast_fixture.h"\n#include "bendtime_fixture.h"')
needle='    if(!std::strcmp(text,"coop_windblast_fixture 20000")||!std::strcmp(text,"coop_windblast_fixture off"))return true;'
bridge=replace_once(bridge,needle,needle+'\n'+needle.replace('windblast','bendtime'))
needle='    if(!std::strcmp(request.command,"coop_windblast_fixture off")){windblastFixtureRequest=2;return;}'
bridge=replace_once(bridge,needle,needle+'\n    if(!std::strcmp(request.command,"coop_bendtime_fixture 20000")){bendtimeFixtureRequest=1;return;}\n    if(!std::strcmp(request.command,"coop_bendtime_fixture off")){bendtimeFixtureRequest=2;return;}')
bridge=replace_once(bridge,'    writeWindblastFixture(tick);','    writeWindblastFixture(tick);\n    writeBendTimeFixture(tick);')
bridge=replace_once(bridge,'serviceWindblastFixture(tick);','serviceWindblastFixture(tick);serviceBendTimeFixture(tick);')
(S/'engine_bridge.cpp').write_text(bridge)
probe=(S/'runtime_probe.cpp').read_text()
probe=replace_once(probe,'#include "native_player_windblast_item.h"','#include "native_player_windblast_item.h"\n#include "native_player_bendtime_item.h"')
start=probe.index('    if(argc==3&&!std::strcmp(argv[2],"--player-windblast"))');end=probe.index('    if(argc==3&&',start+10)
section=probe[start:end].replace('Windblast','BendTime').replace('windblast','bendtime')
section=section.replace('<<",\\\"instantiated\\\":','<<",\\\"powerActive\\\":"<<(p.powerActive?"true":"false")<<",\\\"instantiated\\\":')
assert 'powerActive' in section
probe=probe[:end]+section+probe[end:];(S/'runtime_probe.cpp').write_text(probe)
sender=P/'outputs/d2-coop-lab/Send-D2Command.ps1';text=sender.read_text(encoding='utf-8-sig')
sender.write_text(replace_once(text,'|coop_windblast_fixture|','|coop_windblast_fixture|coop_bendtime_fixture|'),encoding='utf-8-sig')
observe=(P/'work/natural-moveset/Observe-Windblast.ps1').read_text().replace('Windblast','BendTime').replace('WINDBLAST','BENDTIME').replace('windblast','bendtime')
observe=observe.replace('if($after.quantity-or$after.equipped-or$after.started)','if($after.quantity-or$after.equipped-or$after.started-or$after.powerActive)')
observe=observe.replace('Native gameplay may move destructible objects','Native time dilation remains source-local in this research take')
(P/'work/natural-moveset/Observe-BendTime.ps1').write_text(observe)
print('Created exact-build Bend Time reader/fixture/observation harness; no remote integration claim')
