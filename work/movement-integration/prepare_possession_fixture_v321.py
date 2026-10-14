from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';W=R/'work/movement-integration';P=R/'work/natural-moveset'
def change(p,old,new):
 s=p.read_text();assert s.count(old)==1,(p,old[:90],s.count(old));p.write_text(s.replace(old,new))
# Reuse the structurally verified power equipment contract, with exact new RTTI/assets.
p=S/'native_player_possession_item.h';assert not p.exists()
s=(S/'native_player_swarm_item.h').read_text().replace('Swarm','Possession').replace('swarm','possession').replace('devouring_possession','possession').replace('1b51f28','1b531c8').replace('1b51de8','1b53088')
s=s.replace('components/characters/player/base/powers/possession/power.cpntpowerdevouringswarm','components/characters/player/base/powers/possession/possession.cpntpowerpossession')
# The general replacement changes the cpnt name to devouringpossession.
s=s.replace('components/characters/player/base/powers/possession/power.cpntpowerdevouringpossession','components/characters/player/base/powers/possession/possession.cpntpowerpossession')
s=s.replace('// Static v265 exact PE RTTI:', '// Static v320 exact PE RTTI:')
s=s.replace('    if(selectedOnly&&!inv.slots[1])return false;', '''    if(selectedOnly&&(inv.slots[1]<inv.items||inv.slots[1]>=inv.items+inv.itemCount*56||(inv.slots[1]-inv.items)%56))return false;''')
assert 'components/characters/player/base/powers/possession/possession.cpntpowerpossession' in s and 'devouring' not in s
p.write_text(s)
p=S/'possession_fixture.h';assert not p.exists()
s=(S/'swarm_fixture.h').read_text().replace('Swarm','Possession').replace('swarm','possession')
s=s.replace('// may fire the native power. Never call native spawn, AI or damage functions here. World effects are observed\n// independently and reset by the mandatory isolated checkpoint reload.', '// may exercise the native power. No direct transfer, AI, damage or target\n// control invocation. First experiment observes empty-target casting only;\n// mandatory isolated checkpoint reload restores all native state.')
s=s.replace('darkVisionFixture.until||bendtimeFixture.until||agilityFixture.until||','darkVisionFixture.until||windblastFixture.until||bendtimeFixture.until||agilityFixture.until||')
p.write_text(s)
change(S/'engine_bridge.cpp','bool movementSwarmFixtureBusy();','bool movementSwarmFixtureBusy();\nbool movementPossessionFixtureBusy();')
change(S/'engine_bridge.cpp','#include "swarm_fixture.h"','#include "swarm_fixture.h"\n#include "possession_fixture.h"')
change(S/'potion_fixture.h','movementSwarmFixtureBusy();','movementSwarmFixtureBusy()||movementPossessionFixtureBusy();')
change(S/'engine_bridge.cpp','    if(!std::strcmp(text,"coop_swarm_fixture 20000")', '    if(!std::strcmp(text,"coop_possession_fixture 20000")||!std::strcmp(text,"coop_possession_fixture 40000")||!std::strcmp(text,"coop_possession_fixture off"))return true;\n    if(!std::strcmp(text,"coop_swarm_fixture 20000")')
change(S/'engine_bridge.cpp','    if(!std::strcmp(request.command,"coop_swarm_fixture 20000"))', '''    if(!std::strcmp(request.command,"coop_possession_fixture 20000")){possessionFixtureRequest=1;return;}
    if(!std::strcmp(request.command,"coop_possession_fixture 40000")){possessionFixtureRequest=3;return;}
    if(!std::strcmp(request.command,"coop_possession_fixture off")){possessionFixtureRequest=2;return;}
    if(!std::strcmp(request.command,"coop_swarm_fixture 20000"))''')
change(S/'engine_bridge.cpp','writeDominoFixture(tick);writeSwarmFixture(tick);','writeDominoFixture(tick);writeSwarmFixture(tick);writePossessionFixture(tick);')
change(S/'engine_bridge.cpp','serviceSwarmFixture(tick);serviceRangedProp(tick);','serviceSwarmFixture(tick);servicePossessionFixture(tick);serviceRangedProp(tick);')
change(S/'runtime_probe.cpp','#include "native_player_swarm_item.h"','#include "native_player_swarm_item.h"\n#include "native_player_possession_item.h"')
p=S/'runtime_probe.cpp';s=p.read_text();start=s.index('    if(argc==3&&!std::strcmp(argv[2],"--player-swarm"))');end=s.index('    if(argc==3&&!std::strcmp(argv[2],"--player-domino"))',start)
block=s[start:end].replace('Swarm','Possession').replace('swarm','possession');s=s[:start]+block+s[start:];p.write_text(s)
# Discovery harness records raw scenes before enforcing only known identity/cleanup gates.
s=(P/'Observe-NativeSwarm-v265.ps1').read_text().replace('Swarm','Possession').replace('swarm','possession').replace('devouring_possession','possession')
s=s.replace("'coop_possession_fixture off','coop_movement_fixture off'", "'coop_swarm_fixture off','coop_possession_fixture off','coop_movement_fixture off'")
s=s.replace("entities=@($entities.entities|Where-Object {$_.definitionName-match'(?i)(possession|rat_possession)'})", "entities=@($entities.entities|Where-Object {$_.definitionName-match'(?i)(possession|rat)'})")
start=s.index(" foreach($part in 'in','loop','out')");end=s.index(' if(@($scenes|Where-Object',start)
s=s[:start]+" if(!$scenes.Count){throw 'No native Possession cast scene observed'}\n"+s[end:]
s=s.replace('Native input produces reciprocal Possession entry/loop/out scenes with observed clocks and state sets','Native input produces reciprocal Possession cast scenes; observed phase set recorded without assuming complete coverage')
s=s.replace("  $entities=Q host '--entities';if(@($entities.entities|Where-Object {$_.definitionName-match'^models/characters/animals/rat/possession(_big)?\\.def$'}).Count){throw 'Spawned possession remains after checkpoint'}\n",'')
s=s.replace('retire spawned Possession entities','clear the temporary Possession acquisition')
s=s.replace('and spawned entity observations','and candidate target/entity observations')
s=s.replace('neither general possession behavior nor remote movement/VFX/gameplay verified','empty-target cast only; valid-target possession, remote movement/VFX/gameplay remain unverified')
s=s.replace('PASS_NATIVE_POSSESSION_OBSERVATION', 'PASS_NATIVE_POSSESSION_OBSERVATION')
p=P/'Observe-NativePossession-v321.ps1';assert not p.exists();p.write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text();assert s.count("'SwarmObserve'")>=3
s=s.replace("'SwarmObserve','SwarmNetwork'", "'PossessionObserve','SwarmObserve','SwarmNetwork'")
anchor="  'SwarmObserve'{";assert s.count(anchor)==1
s=s.replace(anchor,"  'PossessionObserve'{& \"$root/work/natural-moveset/Observe-NativePossession-v321.ps1\" -Take $Take -Pitch $Pitch}\n"+anchor)
p.write_text(s)
print('Prepared321 strict Possession item/fixture/probe and empty-target discovery harness; no remote semantics integrated')
