#pragma once
#include "native_openable.h"
inline void nativeOpenableTests(){
    const std::uintptr_t base=0x140000000,game=0x200000000,actor=0x300000000,cp=actor+0x1000,child=actor+0x2000,
        anim=actor+0x3000,list=actor+0x4000,def=actor+0x5000,name=actor+0x6000,path=name+0x200;
    const unsigned handle=0x510023,childHandle=0x520024;Memory m;
    m.q(game,base+0x1c940a8);m.q(game+0x3130b0+(handle>>16)*8,actor);m.q(game+0x3130b0+(childHandle>>16)*8,child);
    m.q(actor,base+0x1cb7c68);m.d(actor+0xa8,handle);m.d(child+0xa8,childHandle);m.q(actor+0x20,name);m.str(name,"authored_door");
    m.q(actor+0x2e8,def);m.q(def,base+0x1ab58f8);m.q(def+8,path);m.str(path,"models/environment/door.def");
    m.q(actor+0x370,cp);m.q(actor+0x268,list);m.d(actor+0x270,1);m.q(list,cp);
    m.q(cp,base+0x1c569c8);m.q(cp+0x10,actor);m.d(cp+0x18,17);m.d(cp+0x1c,0x2800006a);
    m.q(cp+0x40,base+0x1b5e208);m.q(cp+0x60,base+0x1c56908);m.q(cp+0x80,base+0x1c56948);m.q(cp+0xb8,base+0x1c56988);
    for(auto offset:{0x68,0x88,0xc0})m.q(cp+offset,cp+0x40);
    for(auto offset:{0x78,0x98,0xd0})m.q(cp+offset,cp);
    m.d(cp+0x48,0);m.d(cp+0x4c,0);m.d(cp+0xdc,childHandle);m.q(cp+0xf8,anim);
    m.q(anim,base+0x1b7f408);m.q(anim+0x10,child);m.d(anim+0x18,18);m.q(cp+0xf0,0);
    m.d(cp+0x118,0);m.d(cp+0x1e8,0);m.d(cp+0x1ed,0);m.d(cp+0xc8,0);m.q(cp+0x1e0,0x0a0b080902030607);
    NativeOpenable out;const auto inspect=[&](Memory& mem){return readNativeOpenable([&](auto a,auto b,auto n){return mem.read(a,b,n);},base,game,handle,out);};
    require(inspect(m)&&out.name=="authored_door"&&nativeOpenableTransitionAllowed(out,2,0),"native owned unlocked door eligible");
    auto bad=m;bad.d(actor+0xa8,handle+1);require(!inspect(bad),"recycled door registry handle rejected");
    bad=m;bad.d(child+0xa8,childHandle+1);require(!inspect(bad),"recycled visual child rejected");
    bad=m;bad.q(cp+0x10,child);require(!inspect(bad),"foreign Openable owner rejected");
    bad=m;bad.q(anim+0x10,actor);require(!inspect(bad),"wrong cached animation owner rejected");
    bad=m;bad.q(anim,base+0x1b80408);require(!inspect(bad),"character animation type cannot impersonate native door animation");
    bad=m;bad.d(cp+0x1c,0);require(!inspect(bad),"retired Openable rejected");
    bad=m;bad.d(actor+0x270,2);bad.q(list+8,cp);require(!inspect(bad),"duplicate component membership rejected");
    bad=m;bad.q(cp+0x88,cp+0x41);require(!inspect(bad),"foreign moving-state machine rejected");
    bad=m;bad.d(cp+0x48,3);require(!inspect(bad),"unknown door state rejected");
    bad=m;bad.d(cp+0xc8,0xf0);require(inspect(bad)&&out.openSide==0,"inactive Open-state side does not reject a closed native door");
    bad.d(cp+0x48,2);bad.d(cp+0x4c,2);require(!inspect(bad),"invalid side remains rejected in active Open state");
    bad.d(cp+0xc8,1);require(inspect(bad)&&out.openSide==1,"active Open state retains native closing side");
    bad.d(cp+0x48,1);bad.d(cp+0x4c,1);bad.d(cp+0xc8,0xf0);require(inspect(bad)&&out.openSide==0,"Moving uses its own params and ignores inactive Open data");
    for(auto offset:{0x118,0x1e8,0x1ed}){bad=m;bad.d(cp+offset,1);require(inspect(bad)&&!nativeOpenableTransitionAllowed(out,2,0),"blocked locked or broken doors cannot bypass native preconditions");}
    bad=m;bad.q(cp+0xf0,child);require(inspect(bad)&&out.linked&&!nativeOpenableTransitionAllowed(out,2,0),"linked door requires later group authority");
    bad=m;bad.d(cp+0x4c,1);require(inspect(bad)&&!nativeOpenableTransitionAllowed(out,2,0),"queued native transition is not overwritten");
    bad=m;bad.d(cp+0x48,1);bad.d(cp+0x4c,1);require(inspect(bad)&&!nativeOpenableTransitionAllowed(out,0,0),"moving native door is not interrupted");
    require(inspect(m),"restore reader fixture");
    require(nativeOpenableMatchesTarget(out,0,0)&&nativeOpenableMatchesTarget(out,0,1),"closed convergence ignores inactive opening side");
    require(!nativeOpenableMatchesTarget(out,1,0)&&!nativeOpenableMatchesTarget(out,0,2),"invalid convergence target or side rejected");
    auto sideCase=out;sideCase.current=sideCase.pending=2;sideCase.openSide=0;
    require(nativeOpenableMatchesTarget(sideCase,2,0)&&!nativeOpenableMatchesTarget(sideCase,2,1),"open convergence requires actual native opening side");
    require(nativeOpenableNeedsSideCorrection(sideCase,2,1)&&!nativeOpenableNeedsSideCorrection(sideCase,2,0)&&!nativeOpenableNeedsSideCorrection(sideCase,0,1),"opposite opening sides need native close then reopen, latest close order does not");
    sideCase.openSide=1;require(nativeOpenableMatchesTarget(sideCase,2,1)&&nativeOpenableNeedsSideCorrection(sideCase,2,0),"side correction works in both directions");
    sideCase.pending=1;require(!nativeOpenableMatchesTarget(sideCase,2,1)&&!nativeOpenableNeedsSideCorrection(sideCase,2,0),"pending native motion cannot be acknowledged or corrected yet");
    sideCase.current=1;require(!nativeOpenableMatchesTarget(sideCase,1,1)&&!nativeOpenableNeedsSideCorrection(sideCase,2,0),"moving state waits for native completion");
    auto identity=out;require(identity.animationGeneration==18&&sameNativeOpenableIdentity(identity,out),"door binding retains cached animation generation");
    ++identity.animationGeneration;require(!sameNativeOpenableIdentity(identity,out),"recycled animation invalidates a cached door binding");
    auto second=out;second.name="another_door";second.current=second.pending=2;
    auto catalog=selectNativeOpenableCatalog({out,second});require(catalog.entries.size()==2&&catalog.entries[0].name=="another_door","catalog includes independently named closed and open doors");
    auto duplicate=out;duplicate.definitionName="different_definition";catalog=selectNativeOpenableCatalog({out,duplicate,second});require(catalog.entries.size()==1&&catalog.ambiguous==2,"ambiguous authored name never becomes a remote lookup target");
    duplicate.locked=1;catalog=selectNativeOpenableCatalog({out,duplicate,second});require(catalog.entries.size()==1&&catalog.ambiguous==2,"locked duplicate cannot hide an ambiguous identity");
    second.entries[1]=255;catalog=selectNativeOpenableCatalog({out,second});require(catalog.entries.size()==1&&catalog.ineligible==1,"missing reverse-side animation excluded from ordinary door catalog");
    auto lockedDoor=out;lockedDoor.name="locked_door";lockedDoor.locked=1;catalog=selectNativeOpenableCatalog({out,lockedDoor});
    require(catalog.entries.size()==2&&catalog.entries[1].name=="locked_door"&&catalog.entries[1].locked&&!nativeOpenableTransitionAllowed(lockedDoor,2,0),"locked ordinary door is cataloged and its native transition waits for the unlock");
    lockedDoor.blocked=1;catalog=selectNativeOpenableCatalog({out,lockedDoor});require(catalog.entries.size()==1&&catalog.ineligible==1,"a locked and blocked door stays excluded");
    require(nativeOpenableParameters(out,2,0)==std::array<unsigned char,4>{1,0,0,7}&&nativeOpenableParameters(out,2,1)==std::array<unsigned char,4>{1,0,1,6},"native open parameter ordering");
    require(nativeOpenableParameters(out,0,0)==std::array<unsigned char,4>{0,0,0,2}&&nativeOpenableParameters(out,0,1)==std::array<unsigned char,4>{0,0,1,3},"native close retains opening side and selects opposite clip");
    require(!nativeOpenableTransitionAllowed(out,1,0)&&!nativeOpenableTransitionAllowed(out,2,2),"invalid target or side rejected");
    out.entries[0]=255;require(!nativeOpenableTransitionAllowed(out,2,0),"missing authored door animation rejected");
    unsigned reads=0;require(!readNativeOpenable([&](auto a,auto b,auto n){const bool ok=m.read(a,b,n);if(a==cp+0x18&&++reads==1)m.d(a,18);return ok;},base,game,handle,out),"concurrent component replacement rejected");
    std::cout<<"PASS: Openable identity, reciprocal machine/visual ownership and guarded native requests\n";
}
