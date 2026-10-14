#pragma once
void swarmWorldTests(Memory initial,std::uintptr_t base,std::uintptr_t game,std::uintptr_t equipment){
    constexpr std::uintptr_t a=0xd00000000,c=a+0x1000,d=a+0x3000,def=a+0x4000,list=a+0x5000,names=a+0x6000,pool=a+0x8000;
    constexpr auto pipeline=a+0xc000,indices=a+0xd000,transforms=a+0xe000,agent=a+0x10000,frame=a+0x11000,anim=a+0x12000,al=a+0x13000;
    constexpr unsigned worldHandle=0x70009,agentHandle=0x80010,index=3;constexpr auto rat=pool+index*0x108;
    auto m=initial;m.d(equipment+0xd4,0x3fff0010);
    NativeSwarmWorld s{};unsigned stage=0;
    const auto inspect=[&](Memory& x){return readNativeSwarmWorld([&](auto a,auto b,auto n){return x.read(a,b,n);},base,s,&stage);};
    require(inspect(m)&&!s.present,"Swarm absence follows native committed handle sentinel");
    m.q(game+0x3130b0+7*8,a);m.q(a,base+0x1c44b98);m.d(a+0xa8,worldHandle);m.d(a+0xac,1);
    m.q(a+0x2e8,def);m.q(def,base+0x1ab58f8);m.q(def+8,names);m.str(names,"models/characters/animals/rat/devouring_swarm.def");
    m.q(a+0x268,list);m.d(a+0x270,1);m.q(list,c);m.q(c,base+0x1c42ab8);m.q(c+8,d);m.q(c+0x10,a);m.d(c+0x1c,0xa800000a);m.q(c+0x20,base+0x1c42c30);
    m.q(d,base+0x1d25df8);m.q(d+8,names+0x100);m.str(names+0x100,"components/characters/animals/rat/devouring_swarm.cpntratswarm");
    m.q(c+0x80,pool);m.d(c+0x88,0x108);m.q(c+0x90,indices);m.d(c+0x98,1);m.d(indices,index);m.q(c+0x138,pipeline);
    m.q(pipeline,base+0x1c44518);m.q(pipeline+0x1a8,pool);m.d(pipeline+0x1b0,0x108);m.q(pipeline+0x138,transforms);m.d(pipeline+0x140,100);m.d(pipeline+0x144,100);
    m.q(rat,base+0x1c442a8);m.q(rat+8,frame);m.q(rat+0x10,agent);m.q(rat+0x18,c);m.d(rat+0x44,17);m.d(rat+0x48,1);m.q(rat+0x78,anim);
    m.q(agent,base+0x1c44ce8);m.d(agent+0xa8,agentHandle);m.d(agent+0xac,worldHandle);m.q(agent+0x360,rat);m.q(agent+0x358,frame);m.q(game+0x3130b0+8*8,agent);
    m.q(agent+0x268,al);m.d(agent+0x270,2);m.q(al,frame);m.q(al+8,anim);
    m.q(frame,base+0x1c573e8);m.q(frame+0x10,agent);m.d(frame+0x18,13);m.d(frame+0x1c,0xa);
    m.q(frame+0xd8,rat);
    m.q(anim,base+0x1b5b938);m.q(anim+0x10,agent);m.d(anim+0x18,14);m.d(anim+0x1c,0xa000000a);
    const float pose[12]={1,2,3,1,0,0,0,1,0,0,0,1},scale=1.1f;m.put(agent+0x300,pose,48);m.put(transforms+index*32+28,&scale,4);
    require(inspect(m)&&!s.present,"uncommitted world never discovered through matching names");
    m.d(equipment+0xd4,worldHandle);
    const bool ok=inspect(m);if(!ok)std::cerr<<"Swarm world stage "<<stage<<'\n';
    require(ok&&s.present&&s.agents.size()==1&&s.agents[0].index==3&&s.agents[0].incarnation==17,"Swarm indexed nonzero pool slot with reciprocal entity/frame/animation and live generation");
    const auto known=s.equipment;
    const auto scoped=[&](Memory& x){return readNativeSwarmWorld([&](auto a,auto b,auto n){return x.read(a,b,n);},base,s,&stage,&known);};
    require(scoped(m)&&s.present,"discovered Swarm item revalidates current ownership without unrelated first-person pose");
    auto invalidOwner=m;invalidOwner.d(game+0x1f7400,1);require(!scoped(invalidOwner),"pending gameplay transition retires scoped Swarm reads");
    invalidOwner=m;invalidOwner.q(known.player.sword.inventory+0x10,a);require(!scoped(invalidOwner),"scoped inventory cannot change player owner");
    invalidOwner=m;invalidOwner.q(known.equipmentDeclaration+0x48,0);require(!scoped(invalidOwner),"scoped inline equipment parent remains exact");
    invalidOwner=m;invalidOwner.d(known.entry+0x24,2);require(!scoped(invalidOwner),"scoped item maximum remains bounded");
    invalidOwner=m;invalidOwner.q(known.player.sword.inventory+0x70,0);invalidOwner.d(equipment+0x1c,0);
    require(scoped(invalidOwner)&&!s.equipment.equipped&&s.present,"native Swarm world can outlive power selection");
    auto bad=m;bad.d(indices,0);require(!inspect(bad),"first physical pool slot is not a substitute for live index");
    bad=m;bad.d(indices,100);require(!inspect(bad),"out-of-capacity rat index refused");
    bad=m;bad.d(c+0x98,2);bad.d(indices+4,index);require(!inspect(bad),"duplicate live rat index refused");
    bad=m;bad.d(c+0x98,101);require(!inspect(bad),"unbounded swarm list refused");
    bad=m;bad.d(c+0x88,0x100);require(!inspect(bad),"wrong rat pool stride refused");
    bad=m;bad.q(pipeline+0x1a8,pool+0x108);require(!inspect(bad),"pipeline pool must match owning component");
    bad=m;bad.q(c+0x10,agent);require(!inspect(bad),"foreign swarm component owner refused");
    bad=m;bad.d(a+0xa8,worldHandle+1);require(!inspect(bad),"recycled swarm entity refused");
    bad=m;bad.d(a+0xac,0x3fff0010);require(!inspect(bad),"unparented swarm cannot impersonate the observed local-player-owned native cast");
    bad=m;bad.q(d,base+0x1b51de8);require(!inspect(bad),"equipment declaration cannot impersonate swarm declaration");
    bad=m;bad.q(rat+0x18,c+1);require(!inspect(bad),"foreign rat owner refused");
    bad=m;bad.q(agent+0x360,rat+0x108);require(!inspect(bad),"rat entity backlink must reciprocate indexed object");
    bad=m;bad.d(agent+0xac,worldHandle+1);require(!inspect(bad),"rat entity parent must be committed swarm");
    bad=m;bad.d(agent+0xa8,agentHandle+0x10000);require(!inspect(bad),"rat handle must resolve through its current entity-table slot");
    bad=m;bad.d(agent+0xa8,agentHandle+1);require(inspect(bad)&&s.agents[0].handle==agentHandle+1,"stable new rat incarnation is discovered as a new identity, not mistaken for the previous handle");
    bad=m;bad.q(frame+0x10,a);require(!inspect(bad),"reference frame must belong to rat entity");
    bad=m;bad.q(agent+0x358,frame+8);require(!inspect(bad),"entity cached frame must identify the same registered reference-frame component");
    bad=m;bad.q(rat+8,0);require(!inspect(bad),"missing rat transform driver is not accepted as generic absence");
    bad=m;bad.q(rat+8,0);bad.d(rat+0x48,3);bad.q(frame+0xd8,frame+0x20);
    require(inspect(bad)&&!s.agents[0].drivingFrame&&s.agents[0].frame==frame,"state3 contextual attachment retains entity frame with native self driver");
    bad.q(frame+0xd8,rat+0x108);require(!inspect(bad),"foreign frame driver is refused even in contextual state3");
    bad=m;bad.q(anim+0x10,a);require(!inspect(bad),"rat animation cannot be borrowed from foreign entity");
    bad=m;bad.q(al+8,frame);require(!inspect(bad),"duplicate frame and absent animation refused");
    bad=m;bad.d(anim+0x1c,0);require(!inspect(bad),"retired rat animation refused");
    bad=m;bad.d(agent+0x300,0x7fc00000);require(!inspect(bad),"nonfinite rat position refused");
    bad=m;bad.d(agent+0x30c,0);require(!inspect(bad),"degenerate rat orientation refused");
    bad=m;bad.d(agent+0x30c,0xbf800000);require(!inspect(bad),"reflected rat orientation refused");
    bad=m;bad.d(transforms+index*32+28,0);require(!inspect(bad),"zero native rat scale refused");
    for(auto where:{equipment+0xd4,rat+0x44,agent+0xa8,anim+0x18,frame+0x18,c+0x98,indices}){
        bad=m;unsigned reads=0;
        require(!readNativeSwarmWorld([&](auto p,auto d,auto n){const bool ok=bad.read(p,d,n);if(p==where&&++reads==1)bad.d(p,1234);return ok;},base,s,&stage),"concurrent Swarm owner/index/generation retirement refused");
    }
    bad=m;unsigned reads=0;
    require(!readNativeSwarmWorld([&](auto p,auto d,auto n){const bool ok=bad.read(p,d,n);if(p==agent+0x300&&++reads==1){const float x=2;bad.put(p,&x,4);}return ok;},base,s,&stage),"rat transform changing inside its read is refused");
    // Native motion is expected across two full passes; identity is stable and
    // the second complete transform is returned. Rejecting all motion would
    // leave only stationary rats eligible for replication.
    bad=m;reads=0;
    require(readNativeSwarmWorld([&](auto p,auto d,auto n){const bool ok=bad.read(p,d,n);if(p==agent+0x300&&++reads==2){const float x=2;bad.put(p,&x,4);}return ok;},base,s,&stage)&&s.agents[0].transform[0]==2,"identity-stable rat movement across full samples remains readable");
    std::cout<<"PASS: Swarm committed ownership, indexed pool, live agent reciprocity, bounded transforms and retirement races\n";
}
