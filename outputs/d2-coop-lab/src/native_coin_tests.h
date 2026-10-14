#pragma once
#include "native_coin.h"
void nativeCoinTests(){
    constexpr std::uintptr_t base=0x140000000,game=0x200000000,actor=0x300000000,list=actor+0x1000,c=actor+0x2000,decl=actor+0x3000,item=actor+0x4000,name=actor+0x5000,itemName=actor+0x6000;
    constexpr unsigned handle=0x50001;Memory m;
    m.q(game,base+0x1c940a8);m.q(game+0x3130b0+5*8,actor);m.q(actor,base+0x1c5e258);m.d(actor+0xa8,handle);m.q(actor+0x20,name);
    const char n[]="pickups_coin_emily_lowvalue_4",it[]="models/interactive/valuables/coins/coin1_lowvalue.inventoryitem";
    m.put(name,n,sizeof(n));m.put(itemName,it,sizeof(it));m.q(actor+0x268,list);m.d(actor+0x270,1);m.q(list,c);
    m.q(c,base+0x1c5a3b8);m.q(c+0x10,actor);m.d(c+0x18,123);m.d(c+0x1c,8);m.q(c+8,decl);m.q(decl,base+0x1c5a278);m.d(decl+0xa0,0);m.q(decl+0xf8,item);m.d(decl+0x100,1);
    m.q(item,base+0x1b9ea48);m.q(item+8,itemName);m.d(item+0x60,1);m.d(item+0xc4,~0u);m.d(c+0xd0,0);m.d(actor+0xd0,0);float xyz[3]={-42.25f,-152.69f,67.6f};m.put(actor+0x300,xyz,12);
    NativeCoin out{};const auto inspect=[&](Memory& mem){return readNativeCoin([&](auto p,auto b,auto size){return mem.read(p,b,size);},base,game,handle,out);};
    require(inspect(m)&&out.generation==123&&out.quantity==1&&out.flags==0,"exact native coin identity");
    auto changed=m;changed.d(c+0xd0,13);changed.d(actor+0xd0,1);require(inspect(changed)&&out.flags==13,"collected native coin remains observable");
    changed=m;changed.q(c+0x10,actor+8);require(!inspect(changed),"coin foreign owner refused");
    changed=m;changed.d(actor+0xa8,handle+1);require(!inspect(changed),"coin recycled handle refused");
    changed=m;changed.d(item+0x60,12);require(!inspect(changed),"note is not a currency coin");
    changed=m;changed.d(item+0x60,4);require(!inspect(changed),"currency-only reader does not silently broaden its scope");
    require(readNativeCoin([&](auto p,auto b,auto n){return changed.read(p,b,n);},base,game,handle,out,true)&&out.type==4,"explicit stack-family reader supports native ammunition");
    changed=m;changed.d(item+0xc4,0);require(!inspect(changed),"special pickup cannot use simple reward path");
    changed=m;changed.d(decl+0xa0,1);require(!inspect(changed),"carried object cannot use coin path");
    changed=m;changed.d(c+0x1c,0);require(!inspect(changed),"unstarted coin component refused");
    changed=m;changed.d(actor+0x270,2);changed.q(list+8,c);require(!inspect(changed),"duplicate pickable component refused");
    changed=m;changed.d(decl+0x100,0);require(!inspect(changed),"invalid coin quantity refused");
    changed=m;changed.d(c+0xd0,0x80);require(!inspect(changed),"unknown coin flag refused");
    changed=m;unsigned reads=0;require(!readNativeCoin([&](auto p,auto b,auto size){const bool ok=changed.read(p,b,size);if(p==c+0x18&&++reads==1)changed.d(c+0x18,124);return ok;},base,game,handle,out),"coin generation change during read refused");
    std::cout<<"PASS: native coin identity, foreign ownership, stale lifetime, pickup kinds and concurrent change\n";
}
