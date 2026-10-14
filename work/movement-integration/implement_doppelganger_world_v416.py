from pathlib import Path
R=Path(__file__).resolve().parents[2]; S=R/'outputs/d2-coop-lab/src'; W=R/'work/movement-integration'
def edit(path,old,new):
    text=path.read_text(encoding='utf-8-sig'); assert text.count(old)==1,(path,old[:100]); path.write_text(text.replace(old,new),encoding='utf-8')
# The established cached equipment path revalidates ownership every time; it
# avoids scanning every inventory declaration and first-person arm per frame.
t=(S/'native_swarm_equipment_owner.h').read_text(encoding='utf-8-sig')
for a,b in [('native_player_swarm_item','native_player_doppelganger_item'),('NativeSwarm','NativeDoppelganger'),('Swarm worlds','Doppelganger worlds'),('character!=1','character!=2'),('player/corvo/body.def','player/emily/body.def'),('models/powers/devouring_swarm/devouring_swarm.inventoryitem','models/powers/doppelganger/doppelganger.inventoryitem'),('0x1b51f28','0x1b52258'),('0x1b51de8','0x1b52118'),('components/characters/player/base/powers/devouring_swarm/power.cpntpowerdevouringswarm','components/characters/player/base/powers/doppelganger/doppelganger_power.cpntpowerdoppelganger')]:t=t.replace(a,b)
(S/'native_doppelganger_equipment_owner.h').write_text(t,encoding='utf-8')
p=S/'native_doppelganger_world.h'
edit(p,'#include "native_player_doppelganger_item.h"','#include "native_doppelganger_equipment_owner.h"')
edit(p,'// Read-only candidate414.','// Read-only candidate416.')
edit(p,'unsigned componentFlags=0,generation=0,componentCount=0,state=0;','unsigned componentFlags=0,generation=0,componentCount=0,rawA0=0,registryHandle=0;\n    std::uintptr_t registryActor=0;')
edit(p,'bool present=false;','bool present=false,retired=false;')
edit(p,'NativeDoppelgangerWorld& s,unsigned* stage=nullptr){','NativeDoppelgangerWorld& s,unsigned* stage=nullptr,const NativeDoppelgangerItem* known=nullptr){')
edit(p,'if(!readNativeDoppelgangerItem(read,base,s.equipment))return false;','if(!(known?readCurrentNativeDoppelgangerEquipment(read,base,*known,s.equipment):readNativeDoppelgangerItem(read,base,s.equipment)))return false;')
edit(p,'mark(3);s.actor=worldIdentityEntity(read,game,s.handle);','''mark(3);
    // A stale array entry is native behavior:43c950 prunes it on a later cast.
    // Successful registry reads can prove retirement. A read failure cannot.
    const auto slot=static_cast<std::int32_t>(s.handle)>>16;
    if(slot<0||slot>=16384||!read(game+0x3130b0+slot*8,&s.registryActor,8))return false;
    if(s.registryActor&&!read(s.registryActor+0xa8,&s.registryHandle,4))return false;
    if(!s.registryActor||s.registryHandle!=s.handle){s.retired=true;mark(0);return true;}
    s.actor=s.registryActor;''')
edit(p,'for(unsigned i=0;i<(s.componentCount&0xffffff);++i){std::uintptr_t c=0,vt=0;if(!read(s.components+i*8,&c,8)||!c||!read(c,&vt,8))return false;', '''std::array<std::uintptr_t,256> componentEntries{};
    if(!read(s.components,componentEntries.data(),(s.componentCount&0xffffff)*8))return false;
    for(unsigned i=0;i<(s.componentCount&0xffffff);++i){std::uintptr_t c=componentEntries[i],vt=0;if(!c||!read(c,&vt,8))return false;''')
edit(p,'// Raw state is reported without guessing semantics; no action depends on it.','// Opaque data only:39e140 acts on a separate helper, not this Decoy component.\n    // Actual component+a0 is128 in415; do not assign a lifecycle meaning.')
edit(p,'&s.state,4','&s.rawA0,4')
edit(p,'NativeDoppelgangerWorld& out,unsigned* stage=nullptr){','NativeDoppelgangerWorld& out,unsigned* stage=nullptr,const NativeDoppelgangerItem* known=nullptr){')
edit(p,'readNativeDoppelgangerWorldFrame(read,base,a,stage)||!readNativeDoppelgangerWorldFrame(read,base,b,stage)','readNativeDoppelgangerWorldFrame(read,base,a,stage,known)||!readNativeDoppelgangerWorldFrame(read,base,b,stage,known)')
edit(p,'a.present!=b.present||a.handles','a.present!=b.present||a.retired!=b.retired||a.registryActor!=b.registryActor||a.registryHandle!=b.registryHandle||a.handles')
edit(p,'||a.state!=b.state','')
p=S/'runtime_probe.cpp'
edit(p,'''const auto started=GetTickCount64();const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};
        do{NativeDoppelgangerWorld''','''const auto started=GetTickCount64();const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};
        NativeDoppelgangerItem known{};bool haveKnown=false;
        do{NativeDoppelgangerWorld''')
edit(p,'const bool valid=readNativeDoppelgangerWorld(reader,base,s,&stage);QueryPerformanceCounter(&end);','''const bool cached=haveKnown;
            const bool valid=readNativeDoppelgangerWorld(reader,base,s,&stage,haveKnown?&known:nullptr);QueryPerformanceCounter(&end);
            if(valid&&s.equipment.instantiated){known=s.equipment;haveKnown=true;}else if(!valid&&stage==1){haveKnown=false;}''')
edit(p,'<<",\\\"equipmentHandle\\\":"<<s.equipment.handle;', '''<<",\\\"equipmentHandle\\\":"<<s.equipment.handle<<",\\\"cached\\\":"<<(cached?"true":"false")<<",\\\"retired\\\":"<<(s.retired?"true":"false")<<",\\\"registryActor\\\":\\\"0x"<<std::hex<<s.registryActor<<"\\\",\\\"registryHandle\\\":"<<std::dec<<s.registryHandle;''')
edit(p,'<<",\\\"stateCandidate\\\":"<<s.state','<<",\\\"rawA0\\\":"<<s.rawA0')
p=S/'native_doppelganger_world_tests.h'
edit(p,'bad=m;bad.d(actor+0xa8,handle+1);require(!inspect(bad),"recycled clone entity handle refused");','''bad=m;bad.d(actor+0xa8,handle+1);require(inspect(bad)&&!out.present&&out.retired,"recycled registry slot proves tracked incarnation retired");
    bad=m;bad.q(game+0x3130b0+(handle>>16)*8,0);require(inspect(bad)&&out.retired&&!out.present,"stable empty registry slot proves tracked incarnation retired");
    bad=m;require(!readNativeDoppelgangerWorld([&](auto a,auto b,auto n){return a!=game+0x3130b0+(handle>>16)*8&&bad.read(a,b,n);},base,out),"failed registry read is unknown, never absence");
    bad=m;require(!readNativeDoppelgangerWorld([&](auto a,auto b,auto n){return a!=actor+0xa8&&bad.read(a,b,n);},base,out),"failed generation read is unknown, never retirement");''')
edit(p,'    std::cout<<"PASS: native Doppelganger', '''    require(inspect(m)&&out.present,"restore exact live clone");
    const auto known=out.equipment;
    const auto cached=[&](Memory& mem){return readNativeDoppelgangerWorld([&](auto a,auto b,auto n){return mem.read(a,b,n);},base,out,nullptr,&known);};
    require(cached(m)&&out.present,"cached item revalidates exact live clone");
    bad=m;bad.d(known.entry+8,known.handle+1);require(!cached(bad),"cached equipment cannot survive handle reuse");
    bad=m;bad.q(known.entry+0x10,known.component+8);require(!cached(bad),"cached equipment component replacement refused");
    bad=m;bad.q(known.player.sword.equipments,known.entry+56);require(!cached(bad),"cached inventory membership removal refused");
    bad=m;bad.q(known.component+0x10,actor);require(!cached(bad),"cached component foreign owner refused");
    bad=m;bad.d(known.entry+0x20,0);require(cached(bad)&&out.present&&out.equipment.quantity==0,"unequipped quantity-zero item retains committed world ownership");
    bad=m;reads=0;require(!readNativeDoppelgangerWorld([&](auto a,auto b,auto n){const bool ok=bad.read(a,b,n);if(a==actor+0xa8&&++reads==1)bad.d(a,handle+1);return ok;},base,out,nullptr,&known),"live-to-retired generation race refused");
    bad=m;bad.d(component+0xa0,128);require(cached(bad)&&out.present&&out.rawA0==128,"opaque component data never interpreted as lifecycle state");
    std::cout<<"PASS: native Doppelganger''')
t=(W/'Build-DoppelgangerWorldProbe-v414.ps1').read_text(encoding='utf-8-sig').replace('v414','v416').replace('probe414','probe416')
(W/'Build-DoppelgangerWorldProbe-v416.ps1').write_text(t,encoding='utf-8')
print('Prepared read-only416 cached ownership, authoritative registry retirement, and negative/race tests')
