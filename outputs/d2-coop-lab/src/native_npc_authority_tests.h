#pragma once
#include "native_npc_authority.h"
void nativeNpcAuthorityTests(){
    constexpr std::uintptr_t base=0x140000000,game=0x200000000,actor=0x300000000,spawner=actor+0x1000;
    constexpr std::uintptr_t list=0x400000000,spList=list+0x1000,spComp=list+0x2000,children=list+0x3000,name=list+0x4000;
    constexpr std::uintptr_t def=list+0x5000,path=list+0x6000,stack=list+0x7000,frame=list+0x8000,health=list+0x9000,hdecl=list+0xa000;
    constexpr std::uintptr_t manager=0x500000000;
    constexpr unsigned handle=0x10003,spawnHandle=0x20004;
    Memory m;m.q(base+0x39a5290,game);m.q(game,base+0x1c940a8);m.d(game+0x341058,3);m.q(game+0x1f6ed0,game);
    m.q(game+0x1f73f0,stack);m.d(game+0x1f73f8,1);m.d(stack,1);m.d(game+0x1f7400,0);m.q(game+0x1e8960,manager);
    m.q(game+0x3130b0+8,actor);m.q(game+0x3130b0+16,spawner);m.q(actor,base+0x1c5e108);m.d(actor+0xa8,handle);m.d(actor+0xac,spawnHandle);
    m.q(spawner,base+0x1c5e948);m.d(spawner+0xa8,spawnHandle);m.q(spawner+0x20,name);m.str(name,"any_authored_actor");
    m.q(spawner+0x268,spList);m.d(spawner+0x270,1);m.q(spList,spComp);m.q(spComp,base+0x1c57d78);m.q(spComp+0x10,spawner);
    m.d(spComp+0x18,12);m.q(spComp+0x30,children);m.d(spComp+0x38,1);m.d(children,handle);
    m.q(actor+0x2e8,def);m.q(def,base+0x1ab58f8);m.q(def+8,path);m.str(path,"models/characters/any_native_actor.def");
    const float root[]={1,2,3,1,0,0,0,1,0,0,0,1};m.put(actor+0x300,root,sizeof(root));m.d(actor+0xd1,0);
    m.q(actor+0x268,list);m.d(actor+0x270,5);m.q(list,frame);m.q(list+8,health);m.q(actor+0x358,frame);
    m.q(frame,base+0x1c573e8);m.q(frame+0x10,actor);m.d(frame+0x18,13);
    m.q(health,base+0x1c51588);m.q(health+0x10,actor);m.d(health+0x18,14);m.q(health+8,hdecl);m.q(hdecl,base+0x1c51698);
    float fifty=50,one=1;m.put(health+0x20,&fifty,4);m.put(hdecl+0x68,&fifty,4);m.put(hdecl+0xb4,&one,4);
    m.q(health+0x68,0);m.d(health+0xb0,~0u);m.d(health+0xb4,~0u);
    std::array<std::uintptr_t,3> components{},registries{},indices{};
    for(unsigned i=0;i<3;++i){const auto kind=static_cast<NativeAiKind>(i);
        const auto region=manager+0x10000*(i+1),c=region+0x1000,r=region+0x2000,blocks=region+0x3000,type=region+0x4000,ix=region+0x5000;
        components[i]=c;registries[i]=r;indices[i]=ix;
        m.q(list+(i+2)*8,c);m.q(c,base+nativeAiVtable(kind));m.q(c+0x10,actor);m.d(c+0x18,100+i);m.d(c+0x1c,i==2?2:0xa000000a);
        if(!i)m.q(c+0x30,region+0x6000);
        m.q(manager+nativeAiRegistryOffset(kind),r);m.d(r+8,16);m.q(r+0x40,blocks);m.d(r+0x48,1);m.q(blocks,c);
        m.q(r+0x58,type);m.d(type+0x5c,0x100);m.q(r+0x10,ix);m.d(r+0x18,1);m.q(r+0x20,ix+2);m.q(r+0x28,ix+(i==2?0:2));
        m.d(r+0x30,1);m.d(r+0x38,i==2?0:1);std::uint16_t zero=0;m.put(ix,&zero,2);
    }
    NativeNpcAuthority s{};const auto inspect=[&](Memory& mem){return readNativeNpcAuthority([&](auto p,auto d,auto n){return mem.read(p,d,n);},base,game,handle,s);};
    require(inspect(m)&&npcAuthorityAcquireEligible(s)&&s.health.maximum==50,"generic NPC authority permits non-guard health and names");const auto original=s;
    auto paused=m;for(unsigned i=0;i<2;++i){paused.d(components[i]+0x1c,0xa000006a);paused.d(registries[i]+0x38,0);paused.q(registries[i]+0x28,indices[i]);}
    require(inspect(paused)&&sameNpcAuthorityIdentity(original,s)&&npcAuthorityPaused(s)&&!npcAuthorityAcquireEligible(s),"owned pauses preserve identity and block reacquisition");
    require(npcAuthorityHostStreamEligible(s),"1214el: a paused NPC stays streamable by the host");
    {auto swapped=original;swapped.ai[0].tree^=0x10;require(!sameNpcAuthorityIdentity(original,swapped)&&sameNpcAuthorityIdentity(original,swapped,false),"combat behaviour-tree swap keeps the ongoing incarnation (1104)");
     swapped=original;swapped.ai[0].component^=0x10;require(!sameNpcAuthorityIdentity(original,swapped,false),"a replaced AI component is still a new incarnation");}
    auto b=paused;b.d(components[0]+0x1c,0xa00000aa);require(inspect(b)&&npcAuthorityPaused(s),"additional native pause reference retained");
    // 1214ex (#113): the game's own pause (a scripted post) is lease-eligible; a civilian decision reads as the decision AI.
    require(inspect(paused)&&npcAuthorityGamePaused(s)&&npcAuthorityLeaseEligible(s),"1214ex: a game-paused NPC at a scripted post is lease-eligible");
    require(inspect(m)&&!npcAuthorityGamePaused(s)&&npcAuthorityLeaseEligible(s),"1214ex: a running NPC stays lease-eligible");
    {auto c=paused;c.d(components[2]+0x1c,0xa000000a);c.d(registries[2]+0x38,1);c.q(registries[2]+0x28,indices[2]+2);
     require(inspect(c)&&!npcAuthorityLeaseEligible(s),"1214ex: a paused NPC whose attack AI runs is not lease-eligible");}
    {auto c=m;c.q(components[0],base+nativeAiVtable(NativeAiKind::DecisionCivilian));c.q(manager+nativeAiRegistryOffset(NativeAiKind::DecisionCivilian),registries[0]);
     c.q(manager+nativeAiRegistryOffset(NativeAiKind::Decision),0);
     require(inspect(c)&&s.ai[0].kind==NativeAiKind::DecisionCivilian&&npcAuthorityAcquireEligible(s),"1214ex: a civilian decision (arkCpntAIDecisionCivilian) reads as the decision AI");
     auto w=c;w.q(manager+nativeAiRegistryOffset(NativeAiKind::DecisionCivilian),0);w.q(manager+nativeAiRegistryOffset(NativeAiKind::Decision),registries[0]);
     require(!inspect(w),"1214ex: a civilian decision is found only in its own registry");}
    require(nativeAiVtable(NativeAiKind::DecisionCivilian)==0x1ccb1f8&&nativeAiRegistryOffset(NativeAiKind::DecisionCivilian)==0x458,"1214ex: civilian decision class and registry (1595a)");
    b=m;b.d(registries[0]+0x38,0);b.q(registries[0]+0x28,indices[0]);require(inspect(b)&&!npcAuthorityAcquireEligible(s),"script-inactive scheduler excluded despite started flags");
    b=m;b.d(components[2]+0x1c,0xa000000a);b.d(registries[2]+0x38,1);b.q(registries[2]+0x28,indices[2]+2);require(inspect(b)&&!npcAuthorityAcquireEligible(s),"native active attack excluded");
    require(!npcAuthorityHostStreamEligible(s),"1214el: an attacking NPC is not host-streamable either");
    b=m;float zero=0;b.put(health+0x20,&zero,4);require(inspect(b)&&!npcAuthorityAcquireEligible(s),"zero health cannot become an authority target");
    require(!npcAuthorityHostStreamEligible(s),"1214el: a dead NPC is not host-streamable");
    b=m;b.d(spComp+0x38,2);b.d(children+4,0x30005);require(!inspect(b),"multi-child authored spawner refused");
    b=m;b.q(frame+0x10,spawner);require(!inspect(b),"foreign reference frame refused");
    b=m;b.q(list,health);require(!inspect(b),"frame not a unique owned component refused");
    b=m;b.d(components[0]+0x18,~0u);require(!inspect(b),"retired component generation refused");
    b=m;b.d(stack,15);require(!inspect(b),"loading state cannot acquire world AI");
    for(auto p:{components[0]+0x18,components[1]+0x18,components[2]+0x18,frame+0x18,health+0x18,spComp+0x18}){
        b=m;unsigned reads=0;require(!readNativeNpcAuthority([&](auto a,auto d,auto n){const bool ok=b.read(a,d,n);if(a==p&&++reads==1)b.d(p,500);return ok;},base,game,handle,s),"component incarnation change between snapshots refused");
    }
    b=m;b.d(components[1]+0x18,1010);require(inspect(b)&&!sameNpcAuthorityIdentity(original,s),"same-address recycled AI invalidates saved lease");
    {// 1214en: the knocked-down state read the way IsOnGround evaluates it.
     constexpr std::uintptr_t down=0x600000000,dlist=down+0x100,phys=down+0x1000,react=down+0x2000,other=down+0x3000,code=base+0x3000000; // lea reach: within 2 GB of the types
     Memory k;k.q(down+0x268,dlist);k.d(down+0x270,3);k.q(dlist,other);k.q(dlist+8,phys);k.q(dlist+16,react);
     const auto getter=[&](std::uintptr_t at,std::uintptr_t type){const std::int32_t rel=static_cast<std::int32_t>(type-(at+7));unsigned char c[8]{0x48,0x8d,0x05,0,0,0,0,0xc3};std::memcpy(c+3,&rel,4);k.put(at,c,8);};
     const auto physType=base+kNativePhysicsComponentType,reactType=base+kNativeAiPhysicsReactionType,physClass=code+0x800,otherClass=code+0x900;
     k.d(physType+0x60,40);k.d(physType+0x64,44);k.d(reactType+0x60,90);k.d(reactType+0x64,90);k.d(physClass+0x60,42);k.d(otherClass+0x60,7);
     k.q(phys,code+0x100);k.q(code+0x100,code+0x10);getter(code+0x10,physClass);
     k.q(react,base+0x1b88928);k.q(base+0x1b88928,code+0x20);getter(code+0x20,reactType);
     k.q(other,code+0x200);k.q(code+0x200,code+0x30);getter(code+0x30,otherClass);
     k.d(phys+0x238,3);k.d(react+0x13c,3);float zero=0;k.put(react+0x138,&zero,4);unsigned char none=0;k.put(react+0x150,&none,1);k.put(react+0x152,&none,1);
     NativeNpcDown d{};const auto readDown=[&](Memory& mem){return readNativeNpcDown([&](auto a,auto p,auto n){return mem.read(a,p,n);},base,down,d);};
     require(readDown(k)&&d.physics==phys&&d.reaction==react&&!nativeNpcDown(d),"1214en: a standing NPC (mode 3, pose 3) is not down");
     auto b=k;b.d(phys+0x238,2);require(readDown(b)&&nativeNpcRagdoll(d)&&nativeNpcDown(d),"1214en: ragdoll mode 2 is down");
     b=k;b.d(phys+0x238,0);require(readDown(b)&&nativeNpcDown(d),"1214en: ragdoll mode 0 is down");
     b=k;b.d(react+0x13c,1);require(readDown(b)&&nativeNpcDown(d),"1214en: recovery pose 1 (getting up) is down");
     b=k;b.d(react+0x13c,0);require(readDown(b)&&nativeNpcDown(d),"1214en: pose 0 is down");
     b=k;float laying=.4f;b.put(react+0x138,&laying,4);require(readDown(b)&&nativeNpcDown(d),"1214en: a running laying timer is down");
     b=k;unsigned char bit=8;b.put(react+0x152,&bit,1);require(readDown(b)&&nativeNpcDown(d),"1214en: +0x152 bit 3 is down");
     b=k;bit=2;b.put(react+0x150,&bit,1);require(readDown(b)&&nativeNpcDown(d),"1214en: +0x150 bit 1 is down");
     b=k;b.d(physClass+0x60,45);require(readDown(b)&&!d.physics&&d.reaction,"1214en: a class outside the type range is not the physics component");
     b=k;unsigned char ret=0xc3;b.put(code+0x10,&ret,1);require(readDown(b)&&!d.physics,"1214en: a type getter that is not lea/ret is not matched");
     b=k;b.q(react,base+0x1b88930);b.q(base+0x1b88930,code+0x20);require(!readDown(b),"1214en: a reaction-range component with a foreign vtable is refused");
     require(npcDownTimerNeedsRefresh(0)&&npcDownTimerNeedsRefresh(.2f)&&!npcDownTimerNeedsRefresh(.5f)&&npcDownTimerNeedsRefresh(std::nanf("")),"1214en: the replica laying timer is refreshed below half");}
    std::cout<<"PASS: generic NPC binding, native generations, active/script-held selection, attack exclusion and pause lifetime\n";
}
