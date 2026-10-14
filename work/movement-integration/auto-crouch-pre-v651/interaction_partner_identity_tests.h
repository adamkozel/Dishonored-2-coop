#pragma once
void interactionPartnerIdentityTests(){
    constexpr std::uintptr_t base=0x140000000,game=0x200000000,player=0x300000000,actor=0x400000000;
    constexpr std::uintptr_t decl=actor+0x1000,name=actor+0x2000,sensor=0x500000000,states=sensor+0x1000,list=sensor+0x2000;
    constexpr unsigned handle=0xa0002;const CoopDamageAvatar avatar{game,actor,handle};
    Memory m;m.q(base+0x39a5290,game);m.q(game,base+0x1c940a8);m.d(game+0x341058,3);m.d(game+0x1f7400,0);
    m.q(game+0x3130b0,player);m.q(player,base+0x1c5e428);m.d(player+0xa8,1);
    m.q(game+0x3130b0+10*8,actor);m.q(actor,base+0x1c5e108);m.d(actor+0xa8,handle);m.q(actor+0x2e8,decl);
    m.q(decl,base+0x1ab58f8);m.q(decl+8,name);m.str(name,"models/characters/def/d2coop/doppelganger_emily.def");
    m.q(sensor,base+0x1b403b8);m.q(sensor+0x10,player);m.d(sensor+0x18,62);m.d(sensor+0x88,handle);m.d(sensor+0x8c,handle);
    m.q(sensor+0x20,states);m.q(states,base+0x1c73d58);m.q(states+0x10,player);
    m.q(player+0x268,list);m.d(player+0x270,2);m.q(list,states);m.q(list+8,sensor);
    const auto test=[&](Memory& x,unsigned field){return currentPartnerInteractionPick([&](auto p,auto d,auto n){return x.read(p,d,n);},base,avatar,sensor,actor,field);};
    for(const auto field:{0x88u,0x8cu}){
        require(test(m,field),"exact Emily partner interaction pick excluded");
        auto bad=m;bad.str(name,"models/characters/def/d2coop/doppelganger_corvo.def");require(test(bad,field),"exact Corvo interaction pick excluded");
        bad=m;bad.str(name,"models/characters/def/generic/dunwall_guard_male_small_01.def");require(!test(bad,field),"ordinary guard pick untouched");
        bad=m;bad.d(sensor+field,handle+1);require(!test(bad,field),"different selected handle untouched");
        bad=m;bad.d(actor+0xa8,handle+1);require(!test(bad,field),"recycled partner refused");
        bad=m;bad.q(sensor,base+0x1b3f998);require(!test(bad,field),"wrong sensor type refused");
        bad=m;bad.q(sensor+0x10,actor);require(!test(bad,field),"foreign sensor owner refused");
        bad=m;bad.d(sensor+0x18,~0u);require(!test(bad,field),"retired sensor refused");
        bad=m;bad.q(list+8,actor);require(!test(bad,field),"unregistered sensor refused");
        bad=m;bad.d(player+0x270,3);bad.q(list+16,sensor);require(!test(bad,field),"duplicate sensor refused");
        bad=m;bad.d(game+0x1f7400,1);require(!test(bad,field),"pending load refused");
        bad=m;unsigned reads=0;
        require(!currentPartnerInteractionPick([&](auto p,auto d,auto n){const bool ok=bad.read(p,d,n);if(p==sensor+0x18&&++reads==1)bad.d(p,63);return ok;},base,avatar,sensor,actor,field),"mid-read sensor generation change refused");
        bad=m;reads=0;
        require(!currentPartnerInteractionPick([&](auto p,auto d,auto n){const bool ok=bad.read(p,d,n);if(p==sensor+field&&++reads==1)bad.d(p,handle+1);return ok;},base,avatar,sensor,actor,field),"mid-read target change refused");
    }
    require(!test(m,0x84),"untraced interaction field refused");
    std::cout<<"PASS: partner interaction picks, both roles, reciprocal ownership and lifecycle refusal\n";
}
