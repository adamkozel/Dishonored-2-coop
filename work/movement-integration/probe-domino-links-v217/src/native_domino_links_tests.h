#pragma once
void dominoLinksTests(Memory m,std::uintptr_t base,std::uintptr_t game,const NativeDominoItem& known){
    constexpr std::uintptr_t a=0xa00000000,c=a+0x1000,d=a+0x5000,list=a+0x7000,names=a+0x8000,pool=a+0x9000;
    constexpr auto target=a+0x30000,anim=a+0x31000,members=a+0x32000,connections=a+0x33000,table=a+0x35000,fxDecl=a+0x36000;
    constexpr auto fxPool=a+0x12000,fxBuckets=a+0x20000,fxNode=a+0x21040;
    constexpr unsigned targetHandle=0xb0007,beamId=38;const auto node=pool+0x150;
    m.d(c+0x30,2);m.q(list+0x108,node);m.d(node,targetHandle);m.d(node+8,0x1234);m.q(node+0x10,anim);m.d(node+0x90,~0u);m.d(node+0x94,0x3fff0010);
    m.q(game+0x3130b0+11*8,target);m.d(target+0xa8,targetHandle);m.q(target,base+0x1c5e108);m.q(target+0x2e8,a+0xf000);
    m.q(target+0x268,members);m.d(target+0x270,1);m.q(members,anim);m.q(anim,base+0x1b80408);m.q(anim+0x10,target);m.d(anim+0x18,0x1234);m.d(anim+0x1c,2);
    float root[3]={6,2,3},start[12]={1,2,4,1,0,0,0,1,0,0,0,1},end[12]={6,2,4,1,0,0,0,1,0,0,0,1};
    m.put(target+0x300,root,12);m.put(node+0x38,end,48);
    m.q(c+0x15b0,table);m.d(c+0x15b8,2);m.q(c+0x15c0,connections);m.d(c+0x15c8,32);m.q(table,connections);m.q(table+8,connections+0x80);
    m.q(d+0xb0,fxDecl);m.q(fxDecl,base+0x1ab3138);m.q(fxDecl+8,names+0x600);m.str(names+0x600,kDominoLinkFx);
    for(unsigned i=0;i<2;++i){const auto x=connections+i*0x80;m.d(x,1);m.q(x+8,i?node:pool);m.q(x+0x10,i?pool:node);m.d(x+0x28,0x3f800000);m.d(x+0x2c,i?~0u:beamId);m.d(x+0x30,i?0x3fff0010:1);}
    const auto manager=game+0x1f8430,map=manager+0xeef60,record=fxPool+0x140;
    m.d(map+0xc,2);m.q(fxBuckets+(beamId&63)*8,fxNode);m.d(fxNode,beamId);m.d(fxNode+4,1);m.q(fxNode+8,0);
    m.d(record,1);m.d(record+4,beamId);m.q(record+8,fxDecl);m.put(record+0x38,start,48);m.put(record+0x80,end,48);m.q(record+0x128,0);
    m.d(known.player.sword.actor+0xac,0x3fff0010);
    NativeDominoLinks result{};unsigned stage=0;
    const auto inspect=[&](Memory& x){return readNativeDominoLinks([&](auto p,auto b,auto n){return x.read(p,b,n);},base,result,&stage,&known);};
    const bool valid=inspect(m);if(!valid)std::cerr<<"Domino link fixture stage "<<stage<<'\n';
    require(valid&&result.connections.size()==2&&result.connections[0].hasBeam&&!result.connections[1].hasBeam,"two directed Domino connections retain exactly one owned native beam");
    auto bad=m;bad.d(c+0x15b8,13);require(!inspect(bad),"connection count bounded by four selected targets");
    bad=m;bad.q(table,connections+1);require(!inspect(bad),"unaligned connection refused");
    bad=m;bad.q(table,connections+32*0x80);require(!inspect(bad),"connection outside pool refused");
    bad=m;bad.q(table+8,connections);require(!inspect(bad),"duplicate connection object refused");
    bad=m;bad.q(connections+0x10,pool);require(!inspect(bad),"self link refused");
    bad=m;bad.q(connections+0x10,pool+0x2a0);require(!inspect(bad),"unselected endpoint refused");
    bad=m;bad.q(connections+0x88,pool);bad.q(connections+0x90,node);require(!inspect(bad),"duplicate directed pair refused");
    bad=m;bad.d(connections,2);require(!inspect(bad),"unplugging connection cannot claim active beam");
    bad=m;bad.d(connections+0x28,0x7fc00000);require(!inspect(bad),"nonfinite opening progress refused");
    bad=m;bad.d(connections+0x30,0xb0007);require(!inspect(bad),"foreign FX owner refused");
    bad=m;bad.d(record+4,beamId+1);require(!inspect(bad),"recycled beam record refused");
    bad=m;bad.q(record+0x128,target);require(!inspect(bad),"unobserved FX callback refused");
    bad=m;bad.d(record+0x80,0x447a0000);require(!inspect(bad),"beam end displaced from native interpolation refused");
    bad=m;bad.d(record+0x8c,0xbf800000);require(!inspect(bad),"reflected endpoint basis refused");
    bad=m;bad.d(connections+0x28,0x3f000000);end[0]=3.5f;bad.put(record+0x80,end,48);require(inspect(bad),"opening beam follows native connection progress");
    bad=m;unsigned reads=0;require(!readNativeDominoLinks([&](auto p,auto b,auto n){const bool ok=bad.read(p,b,n);if(p==connections+0x2c&&++reads==1)bad.d(p,~0u);return ok;},base,result,&stage,&known),"beam replacement across frames refused");
    bad=m;reads=0;require(!readNativeDominoLinks([&](auto p,auto b,auto n){const bool ok=bad.read(p,b,n);if(p==connections+8&&++reads==1)bad.q(p,node);return ok;},base,result,&stage,&known),"endpoint identity race refused");
    std::cout<<"PASS: native Domino directed connection identity, beam owner, endpoints, progress and race guards\n";
}
