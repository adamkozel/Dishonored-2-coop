#pragma once
#include "native_food.h"
inline void nativeFoodTests(){
    PickupFoodValue before{56,0,100,100,100,100,5,0,0,1,false,true},after=before;after.healthTarget=61;
    require(validFoodOutcome(before,after,1),"queued healing is a native food award before current health moves");
    auto filled=before;filled.health=filled.healthTarget=100;require(validFoodOutcome(filled,filled,1),"full health may consume food with zero heal");
    require(!validFoodOutcome(before,before,1),"missing queued and current heal does not certify food");
    auto current=after;current.health=61;current.healthTarget=0;require(validFoodOutcome(before,current,1),"completed native healing remains observable after queue clears");
    auto bad=after;bad.health=55;require(!validFoodOutcome(before,bad,1),"concurrent damage is not a food outcome");
    bad=after;bad.healthTarget=101;require(!validFoodOutcome(before,bad,1),"food cannot exceed native maximum");
    bad=after;bad.stored=1;require(!validFoodOutcome(before,bad,1),"healthy food must not invent a stored item");
    auto suppressed=before;suppressed.healthSuppressed=true;require(validFoodOutcome(suppressed,suppressed,1),"native healing suppression preserves zero outcome");
    auto mana=before;mana.healthGrant=0;mana.manaGrant=20;mana.mana=mana.manaTarget=50;auto recovered=mana;recovered.manaTarget=70;
    require(validFoodOutcome(mana,recovered,1),"mana food queues its native target");
    mana.manaEnabled=false;recovered=mana;recovered.stored=1;require(validFoodOutcome(mana,recovered,1),"native disabled-mana path can fall back to storage");
    before.healthGrant=-1;require(!validFoodValue(before),"damaging rotten food is not admitted as positive restoration");

    constexpr std::uintptr_t base=0x140000000,game=0x200000000,actor=0x300000000,stack=0x400000000,list=stack+0x1000;
    constexpr std::uintptr_t health=0x500000000,hd=health+0x1000,attributes=health+0x2000,table=health+0x3000,physics=health+0x4000,manaC=health+0x5000,md=health+0x6000,inv=health+0x7000,items=health+0x8000,item=health+0x9000,name=health+0xa000;
    Memory m;const auto f=[](Memory& mm,std::uintptr_t p,float n){mm.put(p,&n,4);};
    m.q(base+0x39a5290,game);m.q(game,base+0x1c940a8);m.d(game+0x341058,3);m.q(game+0x1f6ed0,game);m.q(game+0x1f73f0,stack);m.d(game+0x1f73f8,1);m.d(stack,1);m.d(game+0x1f7400,0);
    m.q(game+0x3130b0,actor);m.q(actor,base+0x1c5e428);m.d(actor+0xa8,1);m.q(actor+0x268,list);m.d(actor+0x270,5);m.d(actor+0xd1,0);float xyz[3]{};m.put(actor+0x300,xyz,12);
    const std::uintptr_t components[]={health,attributes,physics,manaC,inv};m.put(list,components,sizeof(components));
    m.q(health,base+0x1c51588);m.q(health+8,hd);m.q(health+0x10,actor);m.q(hd,base+0x1c51698);m.d(health+0x18,17);m.d(health+0x1c,8);f(m,health+0x20,56);f(m,health+0xf4,0);f(m,hd+0x68,100);f(m,hd+0xb4,1);m.q(health+0x68,attributes);m.d(health+0xb0,0);m.d(health+0xb4,1);
    m.q(attributes,base+0x1ca7938);m.q(attributes+0x10,actor);m.q(attributes+0x48,table);m.d(attributes+0x50,4);
    for(unsigned i=0;i<4;++i){f(m,table+i*40+8,0);f(m,table+i*40+0x24,0);}
    m.q(physics,base+0x1c73268);m.q(physics+0x10,actor);m.d(physics+0x50,0);m.d(physics+0x54,0);
    m.q(manaC,base+0x1c55ed8);m.q(manaC+8,md);m.q(manaC+0x10,actor);m.q(md,base+0x1c55d98);m.d(manaC+0x18,18);m.d(manaC+0x1c,8);f(m,manaC+0x20,100);f(m,manaC+0x2c,100);f(m,md+0x68,100);m.q(manaC+0x48,attributes);m.d(manaC+0x68,2);
    m.q(inv,base+0x1b9a878);m.q(inv+0x10,actor);m.d(inv+0x18,19);m.d(inv+0x1c,8);m.q(inv+0x38,items);m.d(inv+0x40,1);m.q(inv+0xf0,attributes);m.d(inv+0x174,3);
    std::array<unsigned char,56> entry{};m.put(items,entry.data(),entry.size());m.q(items,item);m.d(items+0x20,0);m.d(items+0x24,1);m.q(item,base+0x1b9ea48);m.q(item+8,name);m.str(name,"components/interactive/food.inventoryitem");m.d(item+0x60,6);m.d(item+0x78,5);m.d(item+0x7c,0);
    const unsigned char disabled[]={0x32,0xc0,0xc3};m.put(base+0x399620,disabled,3);m.d(base+0x398dbd8,1);
    NativeFood observed{};const auto inspect=[&](Memory& source){return readNativeFood([&](auto a,auto d,auto n){return source.read(a,d,n);},base,inv,actor,item,observed);};
    require(inspect(m)&&observed.value.health==56&&observed.value.healthGrant==5&&observed.value.manaMaximum==100,"owned native food/vitals with exact pending fields");
    auto changed=m;changed.q(manaC+0x10,actor+8);require(!inspect(changed),"food refuses foreign mana owner");
    changed=m;changed.d(manaC+0x68,4);require(!inspect(changed),"food refuses out-of-bounds mana maximum attribute");
    changed=m;changed.q(inv+0xf0,attributes+8);require(!inspect(changed),"food modifier must belong to same player");
    changed=m;changed.d(item+0x78,0xffffffffu);require(!inspect(changed),"negative native food health amount excluded");
    changed=m;changed.d(health+0x1c,0);require(!inspect(changed),"unstarted health cannot prove food effect");
    changed=m;changed.d(base+0x399620,0x90);require(!inspect(changed),"modified overflow policy invalidates exact-build proof");
    changed=m;unsigned reads=0;require(!readNativeFood([&](auto a,auto d,auto n){const bool ok=changed.read(a,d,n);if(a==health+0xf4&&++reads==1)f(changed,a,61);return ok;},base,inv,actor,item,observed),"concurrent healing target change refused");
    std::cout<<"PASS: native food ownership, queued healing, full health, mana fallback, capacity and concurrent-change checks\n";
}
