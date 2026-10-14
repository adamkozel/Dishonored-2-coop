#pragma once
void possessionPartnerIdentityTests(Memory m,std::uintptr_t base,std::uintptr_t game){
    constexpr std::uintptr_t actor=0xd00000000,decl=actor+0x1000,name=actor+0x2000;
    constexpr std::uintptr_t sensor=actor+0x3000,mode=actor+0x4000,records=actor+0x5000,members=actor+0x6000;
    constexpr unsigned handle=0xa0002;
    m.q(game+0x3130b0+10*8,actor);m.d(actor+0xa8,handle);m.q(actor,base+0x1c5e108);
    m.q(actor+0x2e8,decl);m.q(decl,base+0x1ab58f8);m.q(decl+8,name);m.str(name,"models/characters/def/d2coop/doppelganger_emily.def");
    NativePossessionItem item{};
    require(readNativePossessionItem([&](auto p,auto d,auto n){return m.read(p,d,n);},base,item,true),"Possession partner fixture owns selected Corvo power");
    const auto player=item.player.sword.actor;
    std::uintptr_t list=0;unsigned count=0;
    require(m.read(player+0x268,&list,8)&&m.read(player+0x270,&count,4),"player component fixture readable");count&=0xffffff;
    for(unsigned i=0;i<count;++i){std::uintptr_t p=0;require(m.read(list+i*8,&p,8),"component copy readable");m.q(members+i*8,p);}
    m.q(members+count*8,sensor);m.q(members+(count+1)*8,mode);m.q(player+0x268,members);m.d(player+0x270,count+2);
    m.q(item.component+0xe0,sensor);m.q(item.component+0x40,sensor);m.d(item.component+0xd8,9);
    m.q(sensor,base+0x1c5b108);m.q(sensor+0x10,player);m.d(sensor+0x18,9);m.q(sensor+0xe8,mode);
    m.q(mode,base+0x1b41918);m.q(mode+0x10,player);m.q(mode+0x110,records);m.d(mode+0x118,1);m.q(mode+0x168,0);m.d(mode+0x170,0);
    m.d(records+0x18,handle);m.q(records+0x20,0);
    const CoopDamageAvatar avatar{game,actor,handle};
    const auto test=[&](Memory& x){return currentPossessionPartnerPick([&](auto p,auto d,auto n){return x.read(p,d,n);},base,avatar,mode,records);};
    require(test(m),"selected Corvo Possession excludes exact registered partner sensor record");
    auto bad=m;bad.str(name,"models/characters/def/generic/dunwall_guard_male_small_01.def");require(!test(bad),"ordinary guard remains native target");
    bad=m;bad.d(actor+0xa8,handle+1);require(!test(bad),"recycled partner generation not blocked");
    bad=m;bad.q(mode+0x10,actor);require(!test(bad),"foreign sensor owner not blocked");
    bad=m;bad.q(sensor+0xe8,mode+8);require(!test(bad),"unrelated sensor mode not blocked");
    bad=m;bad.d(sensor+0x18,10);require(!test(bad),"recycled sensor weak reference refused");
    bad=m;bad.q(members+count*8,0);require(!test(bad),"sensor must be player member");
    bad=m;bad.q(records+0x20,actor);require(!test(bad),"proxy record outside ordinary entity scope");
    bad=m;bad.q(mode+0x110,records+8);require(!test(bad),"foreign ranked record refused");
    bad=m;bad.d(mode+0x118,513);require(!test(bad),"unbounded sensor list refused");
    bad=m;bad.d(game+0x1f7400,1);require(!test(bad),"world load invalidates partner exclusion scope");
    bad=m;bad.q(item.player.sword.inventory+0x70,0);require(!test(bad),"deselected power not authorized to exclude");
    bad=m;bad.d(item.component+0x1c,0);require(!test(bad),"stopped power not authorized to exclude");
    bad=m;unsigned reads=0;
    require(!currentPossessionPartnerPick([&](auto p,auto d,auto n){const bool ok=bad.read(p,d,n);if(p==records+0x18&&++reads==1)bad.d(p,handle+1);return ok;},base,avatar,mode,records),"ranked handle identity rechecked before exclusion");
    std::cout<<"PASS: native Possession partner record, sensor/power ownership and generation guards\n";
}
