from pathlib import Path
R=Path(__file__).resolve().parents[2]; S=R/'outputs/d2-coop-lab/src'; W=R/'work/movement-integration'
def read(p): return p.read_text(encoding='utf-8-sig')
def write(p,s): p.write_text(s,encoding='utf-8')
def once(s,a,b):
    assert s.count(a)==1,(a,s.count(a))
    return s.replace(a,b)
p=S/'native_player_doppelganger_item.h'; assert not p.exists()
s=read(S/'native_player_swarm_item.h')
for a,b in [('Swarm','Doppelganger'),('swarm','doppelganger'),('0x1b51f28','0x1b52258'),('0x1b51de8','0x1b52118'),('p.player.character!=1','p.player.character!=2'),('devouring_doppelganger/devouring_doppelganger.inventoryitem','doppelganger/doppelganger.inventoryitem'),('devouring_doppelganger/power.cpntpowerdevouringsdoppelganger','doppelganger/doppelganger_power.cpntpowerdoppelganger')]: s=s.replace(a,b)
# Replace the full exact asset path; do not infer spelling from class names.
s=s.replace('components/characters/player/base/powers/devouring_doppelganger/power.cpntpowerdevouringswarm','components/characters/player/base/powers/doppelganger/doppelganger_power.cpntpowerdoppelganger')
s=s.replace('components/characters/player/base/powers/devouring_doppelganger/power.cpntpowerdevouringdoppelganger','components/characters/player/base/powers/doppelganger/doppelganger_power.cpntpowerdoppelganger')
a=s.index('// Static v265'); b=s.index('template<class Read>')
s=s[:a]+'''// Exact B671 PE RTTI v403: component1b52258 and declaration1b52118.
// Installed Void declarations v404 specify Emily, slot1, and an inherited
// component declaration. Accept only the exact named or inline-parent form.
// Runtime native acquisition is initially experimental. This reader never
// grants, activates, spawns, targets or applies damage.
'''+s[b:]
assert 'devouring' not in s and 'swarm' not in s.lower()
assert 'doppelganger/doppelganger_power.cpntpowerdoppelganger' in s
write(p,s)
s=read(S/'mesmerize_fixture.h').replace('Mesmerize','Doppelganger').replace('mesmerize','doppelganger')
write(S/'doppelganger_fixture.h',s)
p=S/'engine_bridge.cpp';s=read(p)
s=once(s,'bool movementPossessionFixtureBusy();','bool movementPossessionFixtureBusy();\nbool movementDoppelgangerFixtureBusy();')
s=once(s,'#include "possession_fixture.h"','#include "possession_fixture.h"\n#include "doppelganger_fixture.h"')
line='    if(!std::strcmp(text,"coop_mesmerize_fixture 20000")||!std::strcmp(text,"coop_mesmerize_fixture 40000")||!std::strcmp(text,"coop_mesmerize_fixture off"))return true;'
s=once(s,line,line+'\n'+line.replace('mesmerize','doppelganger'))
line='    if(!std::strcmp(request.command,"coop_mesmerize_fixture 20000")){mesmerizeFixtureRequest=1;return;}'
new='\n'.join('    if(!std::strcmp(request.command,"coop_doppelganger_fixture '+arg+'")){doppelgangerFixtureRequest='+str(n)+';return;}' for arg,n in [('20000',1),('40000',3),('off',2)])
s=once(s,line,line+'\n'+new)
s=once(s,'writePossessionFixture(tick);','writePossessionFixture(tick);writeDoppelgangerFixture(tick);')
s=once(s,'servicePossessionFixture(tick);','servicePossessionFixture(tick);serviceDoppelgangerFixture(tick);')
write(p,s)
p=S/'potion_fixture.h';s=read(p);s=once(s,'movementPossessionFixtureBusy();','movementPossessionFixtureBusy()||movementDoppelgangerFixtureBusy();');write(p,s)
p=S/'runtime_probe.cpp';s=read(p);s=once(s,'#include "native_player_mesmerize_item.h"','#include "native_player_mesmerize_item.h"\n#include "native_player_doppelganger_item.h"')
a=s.index('    if(argc==3&&!std::strcmp(argv[2],"--player-mesmerize"))');b=s.index('\n    }',a)+6
block=s[a:b].replace('Mesmerize','Doppelganger').replace('mesmerize','doppelganger')
s=s[:b]+'\n'+block+s[b:]
s=once(s,'||!std::strcmp(argv[2],"--domino-effects"))){','||!std::strcmp(argv[2],"--domino-effects")||!std::strcmp(argv[2],"--doppelganger-effects"))){')
a=':readNativeWindblastFxRecords(reader,base,game,records,&failureLine))'
b=':std::strcmp(argv[2],"--doppelganger-effects")==0?readNativeFilteredFxRecords(reader,base,game,records,[](const std::string& name){return name.find("models/effects/gameplay/powers/doppelganger/")==0;},&failureLine,&diagnostic):readNativeWindblastFxRecords(reader,base,game,records,&failureLine))'
s=once(s,a,b);write(p,s)
p=S/'world_identity_tests.cpp';s=read(p);s=once(s,'#include "native_player_swarm_item.h"','#include "native_player_swarm_item.h"\n#include "native_player_doppelganger_item.h"')
a=s.index('                        {\n                            auto domino=mesmer;');b=s.index('                        NativeMesmerizeItem observed',a)
block=s[a:b].replace('Domino','Doppelganger').replace('domino','doppelganger').replace('phantasmallink/phantasmal_link.inventoryitem','doppelganger/doppelganger.inventoryitem').replace('phantasmallink/phantasmal_link.cpntpowerphantasmallink','doppelganger/doppelganger_power.cpntpowerdoppelganger').replace('0x1b52ec8','0x1b52258').replace('0x1b52d88','0x1b52118').replace('v185 native inline','v404 candidate inline')
block='\n'.join(line for line in block.split('\n') if 'doppelgangerTargetsTests(' not in line and 'doppelgangerPartnerIdentityTests(' not in line)
block=block.replace('does not imply a target network','does not imply a spawned clone')
s=s[:b]+block+s[b:];write(p,s)
s=read(W/'Build-PartnerInteraction-v396.ps1').replace('partner-interaction','doppelganger-fixture').replace('v396','v405').replace('Build396','Build405')
write(W/'Build-DoppelgangerFixture-v405.ps1',s)
print('Implemented isolated Doppelganger item reader, fixture, native probe and ABI identity tests405')
