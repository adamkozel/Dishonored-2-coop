#pragma once
#include "native_doppelganger_world.h"
#include "native_doppelganger_visual_tests.h"
#include "native_doppelganger_fx_scene_tests.h"
inline void doppelgangerWorldTests(Memory initial,std::uintptr_t base,std::uintptr_t game,std::uintptr_t pc){
    doppelgangerVisualIdentityTests(base,game);
    doppelgangerFxSceneTests(base,game);
    constexpr std::uintptr_t array=0x980000000,actor=array+0x10000,definition=actor+0x20000,name=actor+0x30000,list=actor+0x40000,component=actor+0x50000,decl=actor+0x60000,parent=actor+0x70000;
    constexpr unsigned handle=0x881234;auto m=initial;
    m.q(pc+0xb8,array);m.d(pc+0xc0,0);m.d(pc+0xb0,handle);m.d(pc+0xd4,0x3fff0010);
    NativeDoppelgangerWorld out{};const auto inspect=[&](Memory& mem){return readNativeDoppelgangerWorld([&](auto a,auto b,auto n){return mem.read(a,b,n);},base,out);};
    require(inspect(m)&&!out.present,"preview handle cannot impersonate committed clone");
    m.d(pc+0xc0,1);m.d(array,handle);m.d(pc+0xd4,handle);m.q(game+0x3130b0+(handle>>16)*8,actor);
    m.q(actor,base+0x1c5e108);m.d(actor+0xa8,handle);m.q(actor+0x2e8,definition);m.q(definition,base+0x1ab58f8);m.q(definition+8,name);m.str(name,"models/powers/doppelganger/decoy_entity.def");
    m.q(actor+0x268,list);m.d(actor+0x270,1);m.q(list,component);m.q(component,base+0x1c4fe28);m.q(component+8,decl);m.q(component+0x10,actor);m.d(component+0x18,7);m.d(component+0x1c,0x2000000a);m.d(component+0xa0,1);
    m.q(decl,base+0x1d20068);m.q(decl+8,name+0x200);m.str(name+0x200,"components/characters/player/base/powers/decoy/decoy.cpntdecoy");
    const std::array<float,12> transform{1,2,3,1,0,0,0,1,0,0,0,1};m.put(actor+0x300,transform.data(),sizeof(transform));
    require(inspect(m)&&out.present&&out.handle==handle&&out.actor==actor&&out.generation==7,"committed native clone selected by owned equipment array");
    auto inlineDecl=m;inlineDecl.str(name+0x200,"unnamed");inlineDecl.q(decl+0x48,parent);inlineDecl.q(parent,base+0x1d20068);inlineDecl.q(parent+8,name+0x400);inlineDecl.str(name+0x400,"components/characters/player/base/powers/decoy/decoy.cpntdecoy");
    require(inspect(inlineDecl)&&out.parentDeclaration==parent,"inherited native decoy component remains exact");
    auto bad=inlineDecl;bad.q(parent,base+0x1b52118);require(!inspect(bad),"power declaration cannot impersonate decoy component");
    // 1214eh: Twin Shade keeps two committed clones; each slot reads its own entry.
    bad=m;bad.d(pc+0xc0,3);require(!inspect(bad),"more clones than Twin Shade's two refused");
    bad=m;bad.d(pc+0xc0,2);bad.d(array+4,handle);require(!inspect(bad),"duplicate committed clone entries refused");
    bad=m;bad.d(pc+0xc0,2);bad.d(array+4,handle+0x10000);bad.d(pc+0xd4,handle+0x20000);require(!inspect(bad),"last clone must be one of two committed entries");
    {auto two=m;two.d(pc+0xc0,2);two.d(array+4,handle+0x10000);two.d(pc+0xd4,handle+0x10000);two.q(game+0x3130b0+((handle+0x10000)>>16)*8,0);
     require(inspect(two)&&out.present&&out.handle==handle,"first of two committed clones read in slot 0");
     NativeDoppelgangerWorld second{};require(readNativeDoppelgangerWorld([&](auto a,auto b,auto n){return two.read(a,b,n);},base,second,nullptr,nullptr,1)&&second.handle==handle+0x10000&&!second.present&&second.retired,"second committed clone read in slot 1 (unregistered here, so retired)");
     NativeDoppelgangerWorld third{};require(readNativeDoppelgangerWorld([&](auto a,auto b,auto n){return two.read(a,b,n);},base,third,nullptr,nullptr,2)&&!third.present&&!third.retired,"slot beyond the committed clones reports absence");}
    bad=m;bad.d(pc+0xd4,handle+1);require(!inspect(bad),"last clone must agree with committed array");
    bad=m;bad.d(actor+0xa8,handle+1);require(inspect(bad)&&!out.present&&out.retired,"recycled registry slot proves tracked incarnation retired");
    bad=m;bad.q(game+0x3130b0+(handle>>16)*8,0);require(inspect(bad)&&out.retired&&!out.present,"stable empty registry slot proves tracked incarnation retired");
    bad=m;require(!readNativeDoppelgangerWorld([&](auto a,auto b,auto n){return a!=game+0x3130b0+(handle>>16)*8&&bad.read(a,b,n);},base,out),"failed registry read is unknown, never absence");
    bad=m;require(!readNativeDoppelgangerWorld([&](auto a,auto b,auto n){return a!=actor+0xa8&&bad.read(a,b,n);},base,out),"failed generation read is unknown, never retirement");
    bad=m;bad.str(name,"models/characters/def/d2coop/doppelganger_emily.def");require(!inspect(bad),"partner avatar cannot impersonate native summoned clone");
    bad=m;bad.q(component+0x10,actor+1);require(!inspect(bad),"foreign decoy component owner refused");
    bad=m;bad.d(actor+0x270,2);bad.q(list+8,component);require(!inspect(bad),"duplicate decoy component refused");
    bad=m;bad.d(component+0x1c,0);require(!inspect(bad),"retired decoy component refused");
    bad=m;auto reflected=transform;reflected[3]=-1;bad.put(actor+0x300,reflected.data(),sizeof(reflected));require(!inspect(bad),"reflected clone transform refused");
    bad=m;unsigned reads=0;require(!readNativeDoppelgangerWorld([&](auto a,auto b,auto n){const bool ok=bad.read(a,b,n);if(a==array&&++reads==1)bad.d(pc+0xc0,0);return ok;},base,out),"concurrent committed clone removal refused");
    bad=m;reads=0;require(!readNativeDoppelgangerWorld([&](auto a,auto b,auto n){const bool ok=bad.read(a,b,n);if(a==component+0x18&&++reads==1)bad.d(a,8);return ok;},base,out),"recycled decoy component generation refused");
    bad=m;bad.d(pc+0xc0,0);require(inspect(bad)&&!out.present,"empty committed array reports absence despite stale last handle");
    require(inspect(m)&&out.present,"restore exact live clone");
    const auto known=out.equipment;
    const auto cached=[&](Memory& mem){return readNativeDoppelgangerWorld([&](auto a,auto b,auto n){return mem.read(a,b,n);},base,out,nullptr,&known);};
    require(cached(m)&&out.present,"cached item revalidates exact live clone");
    bad=m;bad.d(known.entry+8,known.handle+1);require(!cached(bad),"cached equipment cannot survive handle reuse");
    bad=m;bad.q(known.entry+0x10,known.component+8);require(!cached(bad),"cached equipment component replacement refused");
    bad=m;for(unsigned i=0;i<known.player.sword.equipmentCount;++i){std::uintptr_t entry=0;const auto at=known.player.sword.equipments+i*8;require(bad.read(at,&entry,8),"test membership readable");if(entry==known.entry)bad.q(at,0);}require(!cached(bad),"cached inventory membership removal refused");
    bad=m;bad.q(known.component+0x10,actor);require(!cached(bad),"cached component foreign owner refused");
    bad=m;bad.d(known.entry+0x20,0);require(cached(bad)&&out.present&&out.equipment.quantity==0,"unequipped quantity-zero item retains committed world ownership");
    bad=m;reads=0;require(!readNativeDoppelgangerWorld([&](auto a,auto b,auto n){const bool ok=bad.read(a,b,n);if(a==actor+0xa8&&++reads==1)bad.d(a,handle+1);return ok;},base,out,nullptr,&known),"live-to-retired generation race refused");
    bad=m;bad.d(component+0xa0,128);require(cached(bad)&&out.present&&out.rawA0==128,"opaque component data never interpreted as lifecycle state");
    std::cout<<"PASS: native Doppelganger committed clone, preview exclusion, exact component, ownership and lifetime guards\n";
}
