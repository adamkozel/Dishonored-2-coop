#pragma once
#include "native_trigger_identity.h"
void nativeTriggerIdentityTests(){
    constexpr std::uintptr_t base=0x140000000,game=0x200000000,actor=0x300000000,component=0x400000000,list=0x500000000,name=0x600000000,live=0x700000000;
    Memory m;m.q(game,base+0x1c940a8);m.q(game+0x601c0,live);m.d(game+0x601c8,2);m.d(live,0x10005);m.d(live+4,0x70009);
    for(unsigned i=0;i<2;++i){auto a=actor+i*4096,c=component+i*4096,l=list+i*4096,n=name+i*4096;unsigned h=i?0x70009:0x10005;
        m.q(game+0x3130b0+(h>>16)*8,a);m.q(a,base+0x1c5eae8);m.d(a+0xa8,h);m.d(a+0xac,0x3fff0010);m.q(a+0x20,n);m.str(n,i?"other_trigger":"damage_trigger");
        m.q(a+0x268,l);m.d(a+0x270,1);m.q(l,c);m.q(c,base+0x1c5bae8);m.q(c+0x10,a);m.d(c+0x18,17+i);
    }
    auto read=[&](auto at,void* dst,auto size){return m.read(at,dst,size);};std::vector<NativeDamageTrigger> catalog;
    require(readNativeDamageTriggers(read,base,game,catalog)&&catalog.size()==2&&catalog[0].key&&catalog[1].key,"authored trigger catalog binds name, native type, owner and incarnation");
    auto saved=catalog[0];NativeDamageTrigger fresh{};
    require(readNativeDamageTrigger(read,base,game,0x10005,fresh)&&sameNativeDamageTrigger(saved,fresh),"current exact trigger identity retained");
    m.d(component+0x18,18);require(readNativeDamageTrigger(read,base,game,0x10005,fresh)&&!sameNativeDamageTrigger(saved,fresh),"changed trigger incarnation cannot replay an old source");m.d(component+0x18,17);
    m.d(actor+0xa8,0x10006);require(!readNativeDamageTrigger(read,base,game,0x10005,fresh),"reused native slot does not resolve old handle");m.d(actor+0xa8,0x10005);
    m.q(component+0x10,actor+4096);require(!readNativeDamageTrigger(read,base,game,0x10005,fresh),"wrong component owner refused");m.q(component+0x10,actor);
    m.str(name+4096,"damage_trigger");require(readNativeDamageTriggers(read,base,game,catalog)&&!catalog[0].key&&!catalog[1].key,"every duplicate authored trigger name refused, not just first");
    m.q(actor,base+0x1c5e108);require(!readNativeDamageTrigger(read,base,game,0x10005,fresh),"NPC cannot be accepted as authored map trigger");
}
