from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';P=R/'work/natural-moveset';W=R/'work/movement-integration'
t=(S/'native_domino_equipment_owner.h').read_text().replace('native_player_domino_item.h','native_player_swarm_item.h').replace('NativeDomino','NativeSwarm')
t=t.replace('known.player.character!=2','known.player.character!=1').replace('models/characters/player/emily/body.def','models/characters/player/corvo/body.def')
t=t.replace('models/powers/phantasmallink/phantasmal_link.inventoryitem','models/powers/devouring_swarm/devouring_swarm.inventoryitem')
t=t.replace('components/characters/player/base/powers/phantasmallink/phantasmal_link.cpntpowerphantasmallink','components/characters/player/base/powers/devouring_swarm/power.cpntpowerdevouringswarm')
t=t.replace('0x1b52ec8','0x1b51f28').replace('0x1b52d88','0x1b51de8').replace('Persistent target marks','Committed Swarm worlds')
t=t.replace('#include "native_player_swarm_item.h"','#include "native_player_swarm_item.h"\n#include <array>')
(S/'native_swarm_equipment_owner.h').write_text(t)
p=S/'native_swarm_world.h';t=p.read_text().replace('#include "native_player_swarm_item.h"','#include "native_swarm_equipment_owner.h"')
old='NativeSwarmWorld& s,unsigned* stage=nullptr){';assert t.count(old)==1;t=t.replace(old,'NativeSwarmWorld& s,unsigned* stage=nullptr,const NativeSwarmItem* known=nullptr){')
old='if(!readNativeSwarmItem(read,base,s.equipment))return false;';assert t.count(old)==1
t=t.replace(old,'if(!(known?readCurrentNativeSwarmEquipment(read,base,*known,s.equipment):readNativeSwarmItem(read,base,s.equipment)))return false;')
old='NativeSwarmWorld& out,unsigned* stage=nullptr){';assert t.count(old)==1;t=t.replace(old,'NativeSwarmWorld& out,unsigned* stage=nullptr,const NativeSwarmItem* known=nullptr){')
t=t.replace('readNativeSwarmWorldFrame(read,base,a,stage)||!readNativeSwarmWorldFrame(read,base,b,stage)','readNativeSwarmWorldFrame(read,base,a,stage,known)||!readNativeSwarmWorldFrame(read,base,b,stage,known)')
old='        s.agents.push_back(a);';assert t.count(old)==1
t=t.replace(old,'        if(std::any_of(s.agents.begin(),s.agents.end(),[&](const auto& old){return old.actor==a.actor||old.handle==a.handle||old.frame==a.frame||old.animation==a.animation;}))return false;\n'+old)
p.write_text(t)
p=S/'runtime_probe.cpp';t=p.read_text()
start=t.index('    if((argc==3&&!std::strcmp(argv[2],"--swarm-world"))');end=t.index('    if(argc==4&&!std::strcmp(argv[2],"--watch-swarm-scenes"))',start);body=t[start:end]
body=body.replace('std::cout.precision(9);','std::cout.precision(9);NativeSwarmItem known{};bool discovered=false;')
body=body.replace('const bool valid=readNativeSwarmWorld(reader,base,w,&stage);','const bool cached=discovered;const bool valid=readNativeSwarmWorld(reader,base,w,&stage,cached?&known:nullptr);\n            if(valid&&w.equipment.instantiated){known=w.equipment;discovered=true;}else if(!valid&&stage==1)discovered=false;')
body=body.replace('<<",\\\"readMs\\\":"','<<",\\\"cached\\\":"<<(cached?"true":"false")<<",\\\"readMs\\\":"')
body=body.replace('<<",\\\"frameGeneration\\\":"','<<",\\\"directFrameDriver\\\":"<<(a.drivingFrame?"true":"false")<<",\\\"frameGeneration\\\":"')
t=t[:start]+body+t[end:];p.write_text(t)
p=S/'native_swarm_world_tests.h';t=p.read_text();old='    auto bad=m;bad.d(indices,0);';assert old in t
new='''    const auto known=s.equipment;
    const auto scoped=[&](Memory& x){return readNativeSwarmWorld([&](auto a,auto b,auto n){return x.read(a,b,n);},base,s,&stage,&known);};
    require(scoped(m)&&s.present,"discovered Swarm item revalidates current ownership without unrelated first-person pose");
    auto invalidOwner=m;invalidOwner.d(game+0x1f7400,1);require(!scoped(invalidOwner),"pending gameplay transition retires scoped Swarm reads");
    invalidOwner=m;invalidOwner.q(known.player.sword.inventory+0x10,a);require(!scoped(invalidOwner),"scoped inventory cannot change player owner");
    invalidOwner=m;invalidOwner.q(known.equipmentDeclaration+0x48,0);require(!scoped(invalidOwner),"scoped inline equipment parent remains exact");
    invalidOwner=m;invalidOwner.d(known.entry+0x24,2);require(!scoped(invalidOwner),"scoped item maximum remains bounded");
    invalidOwner=m;invalidOwner.q(known.player.sword.inventory+0x70,0);invalidOwner.d(equipment+0x1c,0);
    require(scoped(invalidOwner)&&!s.equipment.equipped&&s.present,"native Swarm world can outlive power selection");
'''
t=t.replace(old,new+old);p.write_text(t)
t=(P/'Observe-NativeSwarmWorld-v288.ps1').read_text().replace("if($liveWorld.Count-lt20){throw 'Fewer than20 strict full native Swarm snapshots'}","if(!$liveWorld.Count){throw 'No strict initial full25-rat snapshot'}")
t=t.replace("At least20 strict snapshots validate all25 indexed native rats, reciprocal entities/frames/animations and stable generations","Initial full25-rat snapshot validates native indexed identities; sustained gate separately covers surviving and contextual agents")
(P/'Observe-NativeSwarmWorld-v289.ps1').write_text(t)
p=W/'Invoke-NativeLabOperation.ps1';t=p.read_text();assert 'Observe-NativeSwarmWorld-v288.ps1' in t;p.write_text(t.replace('Observe-NativeSwarmWorld-v288.ps1','Observe-NativeSwarmWorld-v289.ps1'))
print('Scoped Swarm reader and tests prepared; original288 receipt will retain its full25-count assumption failure.')
