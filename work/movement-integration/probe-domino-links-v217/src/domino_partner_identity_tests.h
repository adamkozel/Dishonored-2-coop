#pragma once
void dominoPartnerIdentityTests(Memory initial,std::uintptr_t base,std::uintptr_t game){
    constexpr std::uintptr_t actor=0xb00000000,decl=actor+0x1000,name=actor+0x2000;constexpr unsigned handle=0xa0002;
    auto m=initial;m.q(game+0x3130b0+10*8,actor);m.d(actor+0xa8,handle);m.q(actor,base+0x1c5e108);m.q(actor+0x2e8,decl);m.q(decl,base+0x1ab58f8);m.q(decl+8,name);m.str(name,"models/characters/def/d2coop/doppelganger_corvo.def");
    NativeDominoItem known{};const CoopDamageAvatar a{game,actor,handle};
    require(readNativeDominoItem([&](auto p,auto d,auto n){return m.read(p,d,n);},base,known,true),"Domino partner fixture exact selected native equipment");
    const auto test=[&](Memory& x,const CoopDamageAvatar& v,std::uintptr_t component,std::uintptr_t candidate){return currentDominoPartnerPick([&](auto p,auto d,auto n){return x.read(p,d,n);},base,v,known,component,candidate);};
    require(test(m,a,known.component,actor),"current native Emily Domino may exclude exact registered partner");
    require(!test(m,a,known.component,known.player.sword.actor),"local player not confused with partner");
    require(!test(m,a,known.component+8,actor),"unrelated power component cannot trigger partner filter");
    auto bad=m;bad.str(name,"models/characters/def/generic/dunwall_guard_male_small_01.def");require(!test(bad,a,known.component,actor),"ordinary guard is never excluded by partner identity");
    bad=m;bad.d(actor+0xa8,handle+1);require(!test(bad,a,known.component,actor),"recycled partner handle not blocked");
    bad=m;bad.q(known.player.sword.inventory+0x70,0);require(!test(bad,a,known.component,actor),"deselected Domino cannot claim active aim pick");
    bad=m;bad.d(known.component+0x1c,0);require(!test(bad,a,known.component,actor),"stopped equipment cannot claim active aim pick");
    bad=m;bad.d(known.entry+0x20,0);require(!test(bad,a,known.component,actor),"unacquired native item cannot exclude partner");
    bad=m;bad.q(known.component+0x10,actor);require(!test(bad,a,known.component,actor),"foreign equipment owner refused");
    bad=m;bad.d(game+0x1f7400,1);require(!test(bad,a,known.component,actor),"pending checkpoint or world transition invalidates exclusion scope");
    bad=m;unsigned checks=0;require(!currentDominoPartnerPick([&](auto p,auto d,auto n){const bool ok=bad.read(p,d,n);if(p==actor+0xa8&&++checks==1)bad.d(p,handle+1);return ok;},base,a,known,known.component,actor),"partner incarnation rechecked after equipment read");
    std::cout<<"PASS: native Domino partner aim selection ownership and current identity guards\n";
}
