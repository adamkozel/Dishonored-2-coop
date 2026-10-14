from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';W=R/'work/movement-integration'
def new(p,s):
    assert not p.exists(),p
    p.write_text(s,encoding='utf-8')
def change(p,a,b):
    s=p.read_text(encoding='utf-8-sig');assert s.count(a)==1,(p,a)
    p.write_text(s.replace(a,b),encoding='utf-8')
def adapt(s):
    s=s.replace('models/powers/doppelganger/doppelganger.inventoryitem','models/powers/shadow_walk/shadow_walk.inventoryitem')
    s=s.replace('components/characters/player/base/powers/doppelganger/doppelganger_power.cpntpowerdoppelganger','components/characters/player/base/powers/shadow_walk/shadow_walk.cpntpowershadowwalk')
    return s.replace('Doppelganger','ShadowWalk').replace('doppelganger','shadowwalk').replace('0x1b52258','0x1b9c248').replace('0x1b52118','0x1b9c108')
s=adapt((S/'native_player_doppelganger_item.h').read_text(encoding='utf-8-sig'))
a=s.index('// Exact B671');b=s.index('template<class Read>',a)
s=s[:a]+'''// Exact B671 PE RTTI514 and installed native Shadow Walk declarations.
// Baseline inventory510 confirms this exact unowned Emily item. The entity
// inherits its native component; named and exact inline-parent forms require
// reciprocal ownership. Reader only; no activation or gameplay writes.

'''+s[b:]
s=s.replace('ShadowWalk equipment is distinct from its spawned world entity and effects.', 'Shadow Walk equipment is distinct from its player state and presentation effects.')
new(S/'native_player_shadowwalk_item.h',s)
s=adapt((S/'doppelganger_fixture.h').read_text(encoding='utf-8-sig'))
s=s.replace('if(f.until||movementRangedFixtureBusy()', 'if(f.until||movementDoppelgangerFixtureBusy()||movementRangedFixtureBusy()')
s=s.replace('may fire the native power. Never call native spawn, AI or damage functions here. World effects are observed','may activate the native power. Never invoke its state transitions or attack functions here. Effects are observed')
new(S/'shadowwalk_fixture.h',s)
p=S/'engine_bridge.cpp'
change(p,'bool movementDoppelgangerFixtureBusy();','bool movementDoppelgangerFixtureBusy();\nbool movementShadowWalkFixtureBusy();')
change(p,'#include "doppelganger_fixture.h"','#include "doppelganger_fixture.h"\n#include "shadowwalk_fixture.h"')
s=p.read_text(encoding='utf-8-sig');a='    if(!std::strcmp(text,"coop_doppelganger_fixture 20000")';start=s.index(a);end=s.index('\n',start)
s=s[:end]+ '\n'+adapt(s[start:end])+s[end:]
start=s.index('    if(!std::strcmp(request.command,"coop_doppelganger_fixture 20000")');end=s.index('\n',s.index('    if(!std::strcmp(request.command,"coop_doppelganger_fixture off")',start))
s=s[:end]+'\n'+adapt(s[start:end])+s[end:]
s=s.replace('writeDoppelgangerFixture(tick);','writeDoppelgangerFixture(tick);writeShadowWalkFixture(tick);').replace('serviceDoppelgangerFixture(tick);','serviceDoppelgangerFixture(tick);serviceShadowWalkFixture(tick);')
p.write_text(s,encoding='utf-8')
change(S/'potion_fixture.h','||movementDoppelgangerFixtureBusy();','||movementDoppelgangerFixtureBusy()||movementShadowWalkFixtureBusy();')
# Explicit mutual exclusion as this fixture is served after its predecessor.
change(S/'doppelganger_fixture.h','if(f.until||movementRangedFixtureBusy()', 'if(f.until||movementShadowWalkFixtureBusy()||movementRangedFixtureBusy()')
p=S/'runtime_probe.cpp';change(p,'#include "native_player_doppelganger_item.h"','#include "native_player_doppelganger_item.h"\n#include "native_player_shadowwalk_item.h"')
s=p.read_text(encoding='utf-8-sig');a=s.index('    if(argc==3&&!std::strcmp(argv[2],"--player-doppelganger"))');b=s.index('    if(argc==3&&',a+20)
block=adapt(s[a:b]);s=s[:b]+block+s[b:];p.write_text(s,encoding='utf-8')
p=S/'world_identity_tests.cpp';change(p,'#include "native_player_doppelganger_item.h"','#include "native_player_doppelganger_item.h"\n#include "native_player_shadowwalk_item.h"')
s=p.read_text(encoding='utf-8-sig');a=s.index('                        {\n                            auto doppelganger=mesmer;');b=s.index('                        NativeMesmerizeItem observed{};',a)
block=adapt(s[a:b]);block=block.replace('                            shadowwalkWorldTests(shadowwalk,base,game,pc);\n','')
s=s[:b]+block+s[b:];p.write_text(s,encoding='utf-8')
s=(W/'Build-DoppelgangerAir-v512.ps1').read_text().replace('doppelganger-air-','shadowwalk-inventory-').replace('-v512.log','-v515.log').replace('Build512','Build515')
new(W/'Build-ShadowWalkInventory-v515.ps1',s)
print('Added exact Shadow Walk inventory reader, isolated reversible fixture, read-only probe and ownership regression tests. No power activation or remote gesture binding.')
