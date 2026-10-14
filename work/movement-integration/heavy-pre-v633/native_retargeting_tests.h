#pragma once
#include "native_retargeting.h"
inline void nativeRetargetingTests(){
    constexpr std::uintptr_t base=0x140000000,game=0x200000000,player=0x300000000,stack=0x400000000;
    constexpr auto list=stack+0x1000,physics=stack+0x2000,c=stack+0x3000,decl=stack+0x4000,records=stack+0x5000;
    constexpr auto target=stack+0x6000,frame=stack+0x7000,targetPhysics=stack+0x8000;
    Memory m;m.q(base+0x39a5290,game);m.q(game,base+0x1c940a8);m.d(game+0x341058,3);m.q(game+0x1f6ed0,game);
    m.q(game+0x1f73f0,stack);m.d(game+0x1f73f8,1);m.d(stack,1);m.d(game+0x1f7400,0);
    m.q(game+0x3130b0,player);m.q(player,base+0x1c5e428);m.d(player+0xa8,1);
    m.q(player+0x268,list);m.d(player+0x270,2);m.q(list,physics);m.q(list+8,c);
    m.q(physics,base+0x1c73268);m.q(physics+0x10,player);m.d(physics+0x50,0);m.d(physics+0x54,0);
    float xyz[3]{};m.put(player+0x300,xyz,sizeof(xyz));
    m.q(c,base+0x1c57778);m.q(c+8,decl);m.q(decl,base+0x1c57638);m.q(c+0x10,player);
    m.q(c+0x20,records);m.d(c+0x28,1);m.d(c+0x3c,7);m.d(c+0x40,0x100);
    m.d(records+0x90,0x10007);m.q(game+0x3130b8,target);m.q(target,base+0x1c5e108);m.d(target+0xa8,0x10007);
    m.q(target+0x358,frame);m.q(records+0x98,frame);m.q(frame,base+0x1c573e8);m.q(frame+0x10,target);m.q(frame+0x3f0,player);
    m.q(records+8,targetPhysics);m.q(targetPhysics,base+0x1c54008);m.q(targetPhysics+0x10,target);
    float offset[7]={0,0,0,0,0,0,1};m.put(records+0x50,offset,28);m.put(records+0x70,offset,28);
    m.d(records+0x6c,31);m.d(records+0x8c,69);
    NativeRetargeting out;const auto inspect=[&](Memory& mem){return readNativeRetargeting([&](auto p,auto b,auto n){return mem.read(p,b,n);},base,1,out);};
    require(inspect(m)&&out.count==1&&out.targetHandle==0x10007&&out.ownerJoint==69&&out.targetJoint==31,"retargeting reciprocal single-body ownership");
    auto bad=m;bad.q(frame+0x3f0,target);require(!inspect(bad),"competing frame owner refused");
    bad=m;bad.q(frame+0x10,player);require(!inspect(bad),"frame points to wrong target refused");
    bad=m;bad.d(target+0xa8,0x10008);require(!inspect(bad),"reused target generation refused");
    bad=m;bad.d(c+0x28,2);require(!inspect(bad),"multiple retarget targets out of scope");
    bad=m;bad.d(c+0x3c,0);require(!inspect(bad),"inactive attachment mode refused");
    bad=m;bad.q(targetPhysics+0x10,player);require(!inspect(bad),"wrong target physics owner refused");
    bad=m;bad.d(records+0x8c,0xffff);require(!inspect(bad),"missing owner joint refused");
    bad=m;bad.d(records+0x60,0x7fc00000);require(!inspect(bad),"nonfinite attachment transform refused");
    bad=m;bad.d(records+0x68,0);require(!inspect(bad),"invalid attachment quaternion refused");
    bad=m;bad.q(list+16,c);bad.d(player+0x270,3);require(!inspect(bad),"duplicated component registration refused");
    bad=m;bad.d(stack,15);require(!inspect(bad),"loading world refused");
    bad=m;bad.q(c+0x20,0);bad.d(c+0x28,0);require(inspect(bad)&&!out.count&&!out.target,"native release observes empty list without stale target");
    bad=m;unsigned reads=0;
    require(!readNativeRetargeting([&](auto p,auto b,auto n){const bool ok=bad.read(p,b,n);if(p==frame+0x3f0&&++reads==1)bad.q(p,0);return ok;},base,1,out),"release racing observation refused");
    std::cout<<"PASS: native retarget attachment ownership, generations, transforms, native release and read races\n";
}
