#pragma once
#include "native_cosmetic_ranged.h"
// Uses the synthetic Memory and require helpers in world_identity_tests.cpp.
inline void cosmeticGrenadeIdentityTests(){
    const std::uintptr_t base=0x140000000,game=0x200000000,actor=0x300000000,owner=actor+0x1000,
        definition=actor+0x2000,ownerDef=actor+0x3000,name=actor+0x4000,ownerName=actor+0x5000,
        list=actor+0x6000,animation=actor+0x7000,frame=actor+0x8000,tag=actor+0x9000,children=actor+0xa000;
    const unsigned handle=0x10007,parent=0x20009;
    for(unsigned character=1;character<=2;++character){
        Memory m;m.q(game+0x3130b0+8,actor);m.q(game+0x3130b0+16,owner);m.d(actor+0xa8,handle);m.d(owner+0xa8,parent);
        m.q(actor,base+0x1c87d58);m.q(owner,base+0x1c5e108);m.q(actor+0x2e8,definition);m.q(owner+0x2e8,ownerDef);
        m.q(definition,base+0x1ab58f8);m.q(ownerDef,base+0x1ab58f8);m.q(definition+8,name);m.q(ownerDef+8,ownerName);
        m.str(name,"models/weapons/d2coop/grenade_prop.def");m.str(ownerName,character==1?"models/characters/def/d2coop/doppelganger_corvo.def":"models/characters/def/d2coop/doppelganger_emily.def");
        m.d(actor+0xac,parent);m.q(actor+0x268,list);m.d(actor+0x270,2);m.q(list,animation);m.q(list+8,frame);
        m.q(animation,base+0x1b80408);m.q(animation+0x10,actor);m.q(frame,base+0x1c573e8);m.q(frame+0x10,actor);
        m.q(actor+0x358,frame);m.q(frame+0x3c8,owner);std::uint16_t joint=9;m.put(frame+0x3d0,&joint,2);
        m.q(frame+0x3c0,tag);m.str(tag,"left_wrist_jnt");m.q(owner+0xb0,children);m.d(owner+0xb8,1);m.q(children,actor);
        NativeCosmeticRanged p{};const auto inspect=[&](Memory& mem){return readNativeCosmeticRanged([&](auto at,auto out,auto n){return mem.read(at,out,n);},base,game,handle,p);};
        require(inspect(m)&&p.kind==3&&p.character==character&&p.owner==owner,"exact cosmetic grenade recognizes both native partner owners");
        require(cosmeticRangedModelMatches(p,"models/weapons/grenade/grenade.md6","models/weapons/grenade/grenade.md6skl"),"exact grenade model and six-joint resource identity");
        require(!cosmeticRangedModelMatches(p,"models/weapons/grenade/sticky/grenade.md6","models/weapons/grenade/grenade.md6skl"),"other grenade model refused");
        auto bad=m;bad.str(name,"models/weapons/grenade/grenade.def");require(!inspect(bad),"real gameplay grenade declaration excluded");
        bad=m;bad.d(actor+0xac,1);require(!inspect(bad),"player-owned weapon excluded");
        bad=m;bad.str(ownerName,"models/characters/player/corvo/body.def");require(!inspect(bad),"player body cannot be cosmetic parent");
        bad=m;bad.q(animation+0x10,owner);require(!inspect(bad),"animation component ownership enforced");
        bad=m;bad.q(animation,base+0x1b9d8b8);require(!inspect(bad),"gameplay weapon component excluded");
        bad=m;bad.d(actor+0x270,3);require(!inspect(bad),"additional gameplay component excluded");
        bad=m;bad.q(list+8,animation);require(!inspect(bad),"duplicate animation component refused");
        bad=m;bad.q(frame+0x3c8,actor);require(!inspect(bad),"reference-frame parent must match native owner");
        bad=m;joint=13;bad.put(frame+0x3d0,&joint,2);require(!inspect(bad),"wrong wrist joint refused");
        bad=m;bad.str(tag,"right_wrist_jnt");require(!inspect(bad),"wrong attachment tag refused");
        bad=m;bad.q(children,owner);require(!inspect(bad),"missing reciprocal child refused");
        bad=m;bad.d(owner+0xb8,2);bad.q(children+8,actor);require(!inspect(bad),"duplicate reciprocal child refused");
        bad=m;bad.d(actor+0xa8,handle+1);require(!inspect(bad),"stale prop generation refused");
        unsigned reads=0;const auto race=[&](auto at,auto out,auto n){if(at==actor+0xac&&++reads==2)m.d(actor+0xac,parent+1);return m.read(at,out,n);};
        require(!readNativeCosmeticRanged(race,base,game,handle,p),"concurrent parent replacement refused");
    }
    NativeCosmeticRanged invalid{};require(!cosmeticRangedModelMatches(invalid,"models/weapons/crossbow/crossbow_emily.md6","models/weapons/crossbow/crossbow_emily.md6skl"),"unknown cosmetic kind cannot inherit crossbow identity");
    std::cout<<"PASS: cosmetic grenade exact owners, attachment, component whitelist, generations and concurrent replacement\n";
}
