#pragma once
void assassinationPartnerIdentityTests(){
    constexpr std::uintptr_t base=0x140000000,game=0x200000000,player=0x300000000,actor=0x400000000;
    constexpr std::uintptr_t decl=actor+0x1000,name=actor+0x2000,context=0x500000000,states=context+0x1000,list=context+0x2000;
    constexpr unsigned handle=0xa0002;const CoopDamageAvatar avatar{game,actor,handle};
    Memory m;m.q(base+0x39a5290,game);m.q(game,base+0x1c940a8);m.d(game+0x341058,3);m.d(game+0x1f7400,0);
    m.q(game+0x3130b0,player);m.q(player,base+0x1c5e428);m.d(player+0xa8,1);
    m.q(game+0x3130b0+10*8,actor);m.q(actor,base+0x1c5e108);m.d(actor+0xa8,handle);m.q(actor+0x2e8,decl);
    m.q(decl,base+0x1ab58f8);m.q(decl+8,name);m.str(name,"models/characters/def/d2coop/doppelganger_emily.def");
    m.q(context+0x20,states);m.q(states,base+0x1c73d58);m.q(states+0x10,player);m.q(player+0x268,list);m.d(player+0x270,1);m.q(list,states);
    const auto test=[&](Memory& x){return currentPartnerAssassinationTarget([&](auto p,auto d,auto n){return x.read(p,d,n);},base,avatar,context,actor);};
    require(test(m),"exact registered Emily partner with local assassination context excluded");
    auto bad=m;bad.str(name,"models/characters/def/d2coop/doppelganger_corvo.def");require(test(bad),"exact registered Corvo partner excluded");
    bad=m;bad.str(name,"models/characters/def/generic/dunwall_guard_male_small_01.def");require(!test(bad),"ordinary NPC retains native eligibility");
    require(!currentPartnerAssassinationTarget([&](auto p,auto d,auto n){return m.read(p,d,n);},base,avatar,context,player),"nonpartner candidate untouched");
    bad=m;bad.d(actor+0xa8,handle+1);require(!test(bad),"stale partner generation refused");
    bad=m;bad.q(game+0x3130b0+10*8,player);require(!test(bad),"replaced partner table slot refused");
    bad=m;bad.q(context+0x20,player);require(!test(bad),"wrong context component refused");
    bad=m;bad.q(states+0x10,actor);require(!test(bad),"foreign source owner refused");
    bad=m;bad.q(states,base+0x1c73268);require(!test(bad),"wrong source component type refused");
    bad=m;bad.q(list,actor);require(!test(bad),"unregistered source component refused");
    bad=m;bad.d(player+0x270,2);bad.q(list+8,states);require(!test(bad),"duplicate source membership refused");
    bad=m;bad.d(player+0x270,257);require(!test(bad),"unbounded component list refused");
    bad=m;bad.d(game+0x1f7400,1);require(!test(bad),"pending load refused");
    bad=m;bad.d(game+0x341058,2);require(!test(bad),"non-gameplay phase refused");
    bad=m;unsigned reads=0;
    require(!currentPartnerAssassinationTarget([&](auto p,auto d,auto n){const bool ok=bad.read(p,d,n);if(p==context+0x20&&++reads==1)bad.q(p,player);return ok;},base,avatar,context,actor),"context replacement during read refused");
    std::cout<<"PASS: partner assassination eligibility identity, source ownership, membership, lifecycle and generation guards\n";
}
