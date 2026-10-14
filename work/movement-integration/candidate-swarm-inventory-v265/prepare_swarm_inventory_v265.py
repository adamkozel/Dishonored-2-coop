"""Strict Corvo inventory reader and finite native acquisition experiment.

Static RTTI/installed assets are known; runtime instantiated ownership remains
a candidate until recorded. No remote swarm, spawn call or damage call added.
"""
from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src'
item='models/powers/devouring_swarm/devouring_swarm.inventoryitem'
component='components/characters/player/base/powers/devouring_swarm/power.cpntpowerdevouringswarm'
def convert(s):
 return s.replace('models/powers/phantasmallink/phantasmal_link.inventoryitem',item).replace('components/characters/player/base/powers/phantasmallink/phantasmal_link.cpntpowerphantasmallink',component).replace('Domino','Swarm').replace('domino','swarm').replace('0x1b52ec8','0x1b51f28').replace('0x1b52d88','0x1b51de8')
s=convert((S/'native_player_domino_item.h').read_text(encoding='utf-8-sig'))
a=s.index('// Exact installed');b=s.index('template<class Read>',a)
s=s[:a]+'''// Static v265 exact PE RTTI: component1b51f28 and declaration1b51de8.
// Installed Void item/component definitions specify Corvo and slot1. Saved
// baseline inventory contains a zero-quantity uninstantiated entry. Native
// grant/inline declaration ownership is a candidate until the isolated test.
// Both named and inline-parent forms require the exact component name/type.
// This reader never grants, activates, spawns or applies damage.
'''+s[b:]
s=s.replace('p.player.character!=2','p.player.character!=1')
(S/'native_player_swarm_item.h').write_text(s,encoding='utf-8')
s=convert((S/'domino_fixture.h').read_text(encoding='utf-8-sig')).replace('isolated Emily','isolated Corvo').replace('p.player.character==2','p.player.character==1')
(S/'swarm_fixture.h').write_text(s,encoding='utf-8')
f=S/'runtime_probe.cpp';s=f.read_text(encoding='utf-8-sig')
if '#include "native_player_swarm_item.h"' not in s:
 s=s.replace('#include "native_player_domino_item.h"','#include "native_player_domino_item.h"\n#include "native_player_swarm_item.h"')
 a=s.index('    if(argc==3&&!std::strcmp(argv[2],"--player-domino"))');b=s.index('    if(argc==3&&!std::strcmp(argv[2],"--player-mesmerize"))',a)
 s=s[:a]+convert(s[a:b])+s[a:];f.write_text(s,encoding='utf-8')
f=S/'world_identity_tests.cpp';s=f.read_text(encoding='utf-8-sig')
if '#include "native_player_swarm_item.h"' not in s:
 s=s.replace('#include "native_player_domino_item.h"','#include "native_player_domino_item.h"\n#include "native_player_swarm_item.h"')
 a=s.index('                        {\n                            auto domino=mesmer;');b=s.index('                        NativeMesmerizeItem observed{};',a)
 block=convert(s[a:b])
 block='\n'.join(line for line in block.split('\n')if 'swarmTargetsTests(' not in line and 'swarmPartnerIdentityTests(' not in line)
 block=block.replace('v185 native inline Swarm declaration resolves its exact named parent','candidate inline Swarm declaration requires its exact named parent')
 block=block.replace('auto swarm=mesmer;','auto swarm=mesmer;\n                            swarm.str(names+0x400,"models/characters/player/corvo/body.def");swarm.str(names+0x600,"models/characters/player/corvo/right_arm.def");')
 block=block.replace('item.player.character==2','item.player.character==1')
 block=block.replace('invalid.str(names+0x400,"models/characters/player/corvo/body.def");invalid.str(names+0x600,"models/characters/player/corvo/right_arm.def");','invalid.str(names+0x400,"models/characters/player/emily/body.def");invalid.str(names+0x600,"models/characters/player/emily/right_arm.def");')
 block=block.replace('fixture refuses Corvo','fixture refuses Emily').replace('target network','spawned swarm')
 s=s[:a]+block+s[a:];f.write_text(s,encoding='utf-8')
f=S/'engine_bridge.cpp';s=f.read_text(encoding='utf-8-sig')
if '#include "swarm_fixture.h"' not in s:
 for a,b in [('bool movementDominoFixtureBusy();','bool movementDominoFixtureBusy();\nbool movementSwarmFixtureBusy();'),('#include "domino_fixture.h"','#include "domino_fixture.h"\n#include "swarm_fixture.h"'),('writeDominoFixture(tick);','writeDominoFixture(tick);writeSwarmFixture(tick);'),('serviceDominoFixture(tick);','serviceDominoFixture(tick);serviceSwarmFixture(tick);')]:
  assert s.count(a)==1,a;s=s.replace(a,b)
 a='    if(!std::strcmp(text,"coop_domino_fixture 20000")||!std::strcmp(text,"coop_domino_fixture 40000")||!std::strcmp(text,"coop_domino_fixture off"))return true;'
 assert s.count(a)==1;s=s.replace(a,a+'\n'+convert(a))
 for ending in ['20000")){dominoFixtureRequest=1;return;}','40000")){dominoFixtureRequest=3;return;}','off")){dominoFixtureRequest=2;return;}']:
  a='    if(!std::strcmp(request.command,"coop_domino_fixture '+ending
  assert s.count(a)==1,a;s=s.replace(a,a+'\n'+convert(a))
 f.write_text(s,encoding='utf-8')
f=S/'potion_fixture.h';s=f.read_text(encoding='utf-8-sig');a='||movementDominoFixtureBusy();'
if 'movementSwarmFixtureBusy()' not in s:
 assert s.count(a)==1;s=s.replace(a,'||movementDominoFixtureBusy()||movementSwarmFixtureBusy();');f.write_text(s,encoding='utf-8')
print('Prepared Swarm read-only identity/probe and finite opt-in acquisition fixture; native runtime ownership still unverified.')
