#pragma once
void possessionTargetFxTests(Memory initial,std::uintptr_t base,std::uintptr_t game,std::uintptr_t pc){
    constexpr std::uintptr_t c=0xc00000000,actor=c+0x1000,decl=c+0x2000,fx=c+0x3000,fd=c+0x4000,list=c+0x5000,names=c+0x6000;
    constexpr std::uintptr_t pool=c+0x10000,buckets=c+0x20000,node=c+0x30000;
    constexpr unsigned handle=0x80005,owner=0x90006,id=37,generation=0x1273;
    auto m=initial;
    const auto named=[&](auto d,auto vt,auto p,const char* text){m.q(d,base+vt);m.q(d+8,p);m.str(p,text);};
    m.d(pc+0xb0,handle);m.d(pc+0xb8,generation);m.q(pc+0xc0,c);m.q(pc+0xc8,0);
    m.q(game+0x3130b0+8*8,actor);m.q(actor,base+0x1c5e108);m.d(actor+0xa8,handle);m.q(actor+0x268,list);m.d(actor+0x270,1);m.q(list,c);
    m.q(c,base+0x1b641a8);m.q(c+8,decl);m.q(c+0x10,actor);m.d(c+0x18,generation);m.d(c+0x1c,0x2000000a);m.q(c+0x28,fx);m.d(c+0x30,id);
    named(decl,0x1b63d68,names,"components/characters/npc/base/possession_human.cpntnpcpossession");m.q(decl+0xa8,fd);named(fd,0x1ab3138,names+0x100,kPossessionHumanTargetFx);
    m.q(game+0x3130b0+9*8,fx);m.q(fx,base+0x1c887f8);m.d(fx+0xa8,owner);m.d(fx+0xac,0x3fff0010);m.q(fx+0x2e8,0);
    const auto manager=game+0x1f8430,map=manager+0xeef60;m.q(manager,pool);m.q(map,buckets);m.d(map+0xc,1);m.d(map+0x10,63);m.q(buckets+(id&63)*8,node);
    m.d(node,id);m.d(node+4,0);m.q(node+8,0);m.d(pool,owner);m.d(pool+4,id);m.q(pool+8,fd);
    float root[3]={1,2,3},pose[12]={1,2,4.7f,1,0,0,0,1,0,0,0,1};m.put(actor+0x300,root,12);m.put(pool+0x38,pose,48);
    NativePossessionTargetFx out{};unsigned stage=0;
    const auto inspect=[&](Memory& mem){return readNativePossessionTargetFx([&](auto p,auto d,auto n){return mem.read(p,d,n);},base,out,&stage);};
    const bool valid=inspect(m);if(!valid)std::cerr<<"Possession mark stage "<<stage<<'\n';
    require(valid&&out.selected&&out.present&&out.handle==handle&&out.effect.owner==owner,"Possession selected NPC owns exact target effect through native component and FX ID bucket");
    auto bad=m;bad.d(pc+0xb8,~0u);bad.q(pc+0xc0,0);bad.d(pc+0xb0,0x3fff0010);require(inspect(bad)&&!out.selected&&!out.present,"selected native Possession item can have no target");
    bad.q(pc+0xc0,c);require(!inspect(bad),"invalid generation cannot retain a target pointer");
    bad=m;bad.q(pc+0xc8,c);require(!inspect(bad),"swarm-agent selection is outside verified human scope");
    bad=m;bad.d(pc+0xb0,1);require(!inspect(bad),"local player cannot impersonate selected human NPC");
    bad=m;bad.d(c+0x18,generation+1);require(!inspect(bad),"recycled target component generation refused");
    bad=m;bad.q(c+0x10,fx);require(!inspect(bad),"target component must own reciprocal selected actor");
    bad=m;bad.q(actor,base+0x1c5e428);require(!inspect(bad),"player class cannot impersonate NPC");
    bad=m;bad.d(actor+0x270,2);bad.q(list+8,c);require(!inspect(bad),"duplicate target membership refused");
    bad=m;bad.q(decl,base+0x1b53088);require(!inspect(bad),"power declaration cannot impersonate NPC declaration");
    bad=m;bad.d(c+0x30,~0u);bad.q(c+0x28,0xdead);require(inspect(bad)&&out.selected&&!out.present,"native stopped ID is valid absence without following stale FX actor");
    bad=m;bad.q(c+0x28,actor);require(!inspect(bad),"effect actor cannot be substituted with target NPC");
    bad=m;bad.d(fx+0xa8,owner+0x10000);require(!inspect(bad),"recycled FX actor handle refused");
    bad=m;bad.d(pool+4,id+1);require(!inspect(bad),"recycled FX record refused");
    bad=m;bad.str(names+0x100,"models/effects/gameplay/powers/possession/player/possession_out_01.fx");require(!inspect(bad),"local camera effect cannot impersonate target particles");
    bad=m;bad.d(pool+0x38,0x447a0000);require(!inspect(bad),"target effect must remain near its owning NPC");
    bad=m;bad.d(pool+0x44,0);require(!inspect(bad),"degenerate target basis refused");
    bad=m;bad.d(pool+0x44,0xbf800000);require(!inspect(bad),"reflected target basis refused");
    for(const auto address:{pc+0xb0,c+0x18,c+0x30}){bad=m;unsigned reads=0;require(!readNativePossessionTargetFx([&](auto p,auto d,auto n){const bool ok=bad.read(p,d,n);if(p==address&&++reads==1)bad.d(p,~0u);return ok;},base,out),"concurrent target/FX retirement refused");}
    std::cout<<"PASS: selected Possession target generation, exact native effect ownership, absent/stale states and concurrent retirement guards\n";
}
