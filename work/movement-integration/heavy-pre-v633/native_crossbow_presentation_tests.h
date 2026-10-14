#pragma once
void crossbowPresentationTests(){
    constexpr std::uintptr_t base=0x140000000,game=0x200000000,actor=0x300000000,anim=0x400000000;
    constexpr auto list=actor+0x1000,component=actor+0x2000,tree=anim+0x4000,data=anim+0x5000,decl=anim+0x6000,buckets=anim+0x7000,entry=anim+0x8000,resource=anim+0x9000,names=anim+0xa000;
    NativePlayerRangedItem p{};p.kind=2;p.player.character=1;p.player.sword.game=game;p.entity=actor;p.component=component;p.handle=0x10002;p.flags=0xa;
    Memory m;m.q(game+0x3130b0+8,actor);m.d(actor+0xa8,p.handle);m.q(actor+0x268,list);m.d(actor+0x270,2);m.q(list,anim);m.q(list+8,component);
    m.q(anim,base+0x1b80408);m.q(component,base+0x1b9cbf8);m.q(component+0x10,actor);m.d(component+0x1c,p.flags);m.q(anim+0x10,actor);
    m.q(anim+0x338,tree);m.q(tree,data);m.q(anim+0x340,decl);m.q(decl+0x70,buckets);m.d(decl+0x80,0);m.q(buckets,entry);
    m.q(entry,names);m.str(names,"defaultAnim");m.d(entry+8,2);m.d(entry+12,968);m.q(entry+0x970,0);m.q(data+968,resource);
    m.q(resource,base+0x1b0ba38);m.q(resource+8,names+64);m.str(names+64,"models/weapons/crossbow/animations/corvo/crossbow_idle.md6anim");
    NativeCrossbowPresentation out{};const auto inspect=[&](Memory& memory){return readNativeCrossbowPresentation([&](auto a,auto b,auto n){return memory.read(a,b,n);},base,p,out);};
    require(inspect(m)&&out.state==1,"owned native cocked crossbow alias");
    m.str(names+64,"models/weapons/crossbow/animations/corvo/crossbow_outofammo.md6anim");require(inspect(m)&&out.state==2,"native fired default remains after scene end");
    auto bad=m;bad.str(names+64,"models/weapons/crossbow/animations/emily/crossbow_outofammo.md6anim");require(!inspect(bad),"wrong role alias refused");
    bad=m;bad.q(entry+0x970,entry);require(!inspect(bad),"cyclic graph binding table refused");
    bad=m;bad.d(entry+8,1);require(!inspect(bad),"non-alias graph input refused");
    bad=m;bad.q(resource,base+0x1b0ba39);require(!inspect(bad),"foreign animation resource class refused");
    bad=m;bad.q(anim+0x10,actor+1);require(!inspect(bad),"foreign animation owner refused");
    bad=m;bad.d(actor+0xa8,p.handle+1);require(!inspect(bad),"recycled weapon generation refused");
    bad=m;bad.d(decl+0x80,4096);require(!inspect(bad),"unbounded graph declaration refused");
    unsigned reads=0;require(!readNativeCrossbowPresentation([&](auto a,auto b,auto n){if(a==data+968&&++reads==2)m.q(a,0);return m.read(a,b,n);},base,p,out),"concurrent default animation replacement refused");
    std::cout<<"PASS: native crossbow clip state, role, owner, generation, graph bounds and concurrent replacement\n";
}
