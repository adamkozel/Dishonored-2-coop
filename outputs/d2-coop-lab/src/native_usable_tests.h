#pragma once
#include "native_usable.h"
inline void nativeUsableTests(){
    constexpr std::uintptr_t base=0x140000000,game=0x200000000,actor=0x300000000,cp=actor+0x1000,anim=actor+0x2000,list=actor+0x3000,
        def=actor+0x4000,decl=actor+0x5000,states=actor+0x6000,name=actor+0x7000,defname=name+0x200,declname=name+0x400;
    constexpr unsigned handle=0x510023;Memory m;
    m.q(game,base+0x1c940a8);m.q(game+0x3130b0+(handle>>16)*8,actor);m.q(actor,base+0x1c5ec38);m.d(actor+0xa8,handle);
    m.q(actor+0x20,name);m.str(name,"authored_usable");m.q(actor+0x2e8,def);m.q(def,base+0x1ab58f8);m.q(def+8,defname);m.str(defname,"models/environment/usable.def");
    m.q(actor+0x268,list);m.d(actor+0x270,2);m.q(list,cp);m.q(list+8,anim);float xyz[3]{};m.put(actor+0x300,xyz,12);
    m.q(cp,base+0x1c5bfc8);m.q(cp+8,decl);m.q(cp+0x10,actor);m.d(cp+0x18,17);m.d(cp+0x1c,8);m.d(cp+0x20,1);m.d(cp+0x28,0);m.d(cp+0x2c,~0u);m.d(cp+0x30,0);m.d(cp+0xd0,0x3fff0010);m.q(cp+0xd8,anim);
    m.q(decl,base+0x1c5c0c8);m.q(decl+8,declname);m.str(declname,"map_usable_decl");m.q(decl+0x70,states);m.d(decl+0x78,2);m.d(decl+0x80,0);
    std::array<unsigned char,256> entries{};m.put(states,entries.data(),entries.size());m.d(states+8,0x1ff);m.d(states+0x88,0xff);
    m.q(anim,base+0x1b7f408);m.q(anim+0x10,actor);m.d(anim+0x18,18);m.d(anim+0x1c,8);
    NativeUsable out{};const auto inspect=[&](Memory& mm){return readNativeUsable([&](auto a,auto d,auto n){return mm.read(a,d,n);},base,game,handle,out);};
    require(inspect(m)&&out.stateCount==2&&out.current==0&&nativeUsableAtTarget(out,0),"owned two-state usable with animation membership");
    auto bad=m;bad.q(anim+0x10,actor+8);require(!inspect(bad),"foreign animation owner cannot drive usable replication");
    bad=m;bad.q(list+8,cp);require(!inspect(bad),"duplicate usable component rejected");
    bad=m;bad.d(decl+0x78,33);require(!inspect(bad),"unbounded state array rejected");
    bad=m;bad.d(cp+0x28,2);require(!inspect(bad),"out-of-range current state rejected");
    bad=m;bad.d(cp+0x2c,2);require(!inspect(bad),"out-of-range queued state rejected");
    bad=m;bad.d(cp+0x1c,0);require(!inspect(bad),"stopped component rejected");
    bad=m;bad.d(states+0xc,0x7fc00000);require(!inspect(bad),"nonfinite native transition duration rejected");
    bad=m;bad.d(cp+0x30,2);require(inspect(bad)&&!nativeUsableAtTarget(out,0),"moving state cannot acknowledge convergence");
    bad=m;bad.d(cp+0x2c,1);require(inspect(bad)&&!nativeUsableAtTarget(out,0),"queued state cannot acknowledge old target");
    bad=m;bad.d(cp+0x20,0);require(inspect(bad)&&!nativeUsableTargetValid(out,1),"disabled native usable cannot be forced by target helper");
    require(inspect(m),"restore usable fixture");auto old=out;++old.animationGeneration;require(!sameNativeUsableIdentity(old,out),"recycled animation invalidates cached usable binding");
    unsigned reads=0;bad=m;require(!readNativeUsable([&](auto a,auto d,auto n){const bool ok=bad.read(a,d,n);if(a==cp+0x28&&++reads==1)bad.d(cp+0x28,1);return ok;},base,game,handle,out),"concurrent native state change rejected");
    std::cout<<"PASS: usable state-list ownership, bounded states, animation lifetime and pending transition checks\n";
}
