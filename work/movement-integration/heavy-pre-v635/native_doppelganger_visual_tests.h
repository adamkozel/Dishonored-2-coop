#pragma once
#include "native_doppelganger_visual.h"
inline void doppelgangerVisualIdentityTests(std::uintptr_t base,std::uintptr_t game){
    Memory m;constexpr std::uintptr_t a=0xa10000000,d=a+0x10000,n=a+0x20000,list=a+0x30000,render=a+0x40000,model=a+0x50000;constexpr unsigned handle=0x991234;
    m.q(game+0x3130b0+(handle>>16)*8,a);m.q(a,base+0x1c87d58);m.d(a+0xa8,handle);m.d(a+0xac,0x3fff0010);
    m.q(a+0x2e8,d);m.q(d,base+0x1ab58f8);m.q(d+8,n);m.str(n,kDoppelgangerVisualDefinition);m.q(a+0x268,list);m.d(a+0x270,4);
    constexpr std::uintptr_t types[]={0x1b80408,0x1c59ac8,0x1c4e588,0x1c573e8};
    constexpr const char* names[]={"components/d2coop/doppelganger_visual.cpntanimevolved","components/base/midnightproxy_default.cpntmidnightproxy","components/base/audio_default.cpntaudio","components/base/referenceframe_default.cpntreferenceframe"};
    for(unsigned i=0;i<4;++i){const auto c=a+0x60000+i*0x1000,decl=c+0x500,name=n+0x1000+i*0x200;
        m.q(list+i*8,c);m.q(c,base+types[i]);m.q(c+0x10,a);m.q(c+8,decl);m.q(decl+8,name);m.str(name,names[i]);m.d(c+0x18,80+i);
        if(i==0){m.q(c+0x38,render);m.q(render,base+0x1b0d628);m.q(render+0x1fe8,model);m.q(model,base+0x1b05218);m.q(model+8,n+0x3000);m.str(n+0x3000,"models/characters/female/emily/head01/emily_head01.md6");}
        if(i==3)m.q(a+0x358,c);
    }
    NativeDoppelgangerVisual out{};const auto inspect=[&](Memory& memory){return readNativeDoppelgangerVisual([&](auto p,auto b,auto z){return memory.read(p,b,z);},base,game,handle,out);};
    require(inspect(m)&&out.actor==a&&out.animationGeneration==80,"exact four-component cosmetic Emily model accepted");
    auto bad=m;bad.q(a,base+0x1c5e108);require(!inspect(bad),"native NPC never treated as disposable cosmetic model");
    bad=m;bad.str(n,"models/characters/def/d2coop/doppelganger_emily.def");require(!inspect(bad),"partner avatar cannot impersonate cosmetic summoned clone");
    bad=m;bad.d(a+0xac,1);require(!inspect(bad),"player-owned equipment model cannot be clone visual");
    bad=m;bad.d(a+0x270,5);require(!inspect(bad),"extra gameplay component is refused");
    bad=m;bad.q(list+8,a+0x60000);require(!inspect(bad),"duplicate animation component refused");
    bad=m;bad.q(a+0x60000+0x10,a+8);require(!inspect(bad),"foreign animation owner refused");
    bad=m;bad.str(n+0x3000,"models/characters/animals/rat/rat.md6");require(!inspect(bad),"foreign model refused");
    bad=m;bad.q(a+0x358,a+0x60000);require(!inspect(bad),"frame must be owned reference-frame component");
    bad=m;bad.d(a+0xa8,handle+1);require(!inspect(bad),"recycled cosmetic handle refused");
    std::cout<<"PASS: exact cosmetic clone definition, component whitelist, owner and incarnation checks\n";
}
