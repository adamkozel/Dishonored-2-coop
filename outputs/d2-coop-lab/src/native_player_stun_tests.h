#pragma once
#include "native_player_stun_scene.h"
#include "stun_presentation.h"
void nativePlayerStunTests(){
    constexpr std::uintptr_t base=0x140000000,game=0x200000000,actor=0x300000000,stack=0x400000000;
    constexpr auto list=stack+0x1000,physics=list+0x1000,machine=physics+0x1000,mode=machine+0x1000,active=mode+0x1000,state=active+0x1000;
    constexpr auto instances=state+0x1000,instance=instances+0x1000,decl=instance+0x1000,scene=decl+0x1000,name=scene+0x1000,sub=name+0x1000,bindings=sub+0x1000;
    const auto manager=game+0x1f5338;Memory m;
    m.q(base+0x39a5290,game);m.q(game,base+0x1c940a8);m.d(game+0x341058,3);m.q(game+0x1f6ed0,game);m.q(game+0x1f73f0,stack);m.d(game+0x1f73f8,1);m.d(stack,1);m.d(game+0x1f7400,0);
    m.q(game+0x3130b0,actor);m.q(actor,base+0x1c5e428);m.d(actor+0xa8,1);m.q(actor+0x268,list);m.d(actor+0x270,2);m.q(list,physics);m.q(list+8,machine);
    m.q(physics,base+0x1c73268);m.q(physics+0x10,actor);m.d(physics+0x50,0);m.d(physics+0x54,0);float pos[3]={1,2,3};m.put(actor+0x300,pos,sizeof(pos));
    m.q(machine,base+0x1c73d58);m.q(machine+0x10,actor);m.q(machine+0x90,mode);m.q(machine+0x28,mode);m.q(machine+0xa8,active);m.d(machine+0xb0,1);
    m.q(mode,machine);m.d(active,14);m.q(mode+0x10+14*8,state);m.q(state,base+0x1c86768);m.d(state+8,14);m.q(state+0x10,machine);
    m.q(manager,instances);m.d(manager+8,1);m.q(manager+0x610,0);m.d(manager+0x618,0);m.q(instances,instance);
    m.q(instance,decl);m.q(instance+8,scene);m.d(instance+0x27c,3);m.d(instance+0x158,77);m.d(instance+0x160,180);m.q(instance+0x10,bindings);m.d(instance+0x18,1);m.q(bindings+8,actor);
    m.q(decl,base+0x1affbc8);m.q(decl+8,name);m.str(name,"midnight/gameplay/player/base/hitreact/hit_react_stun_right.midnightscene");
    m.q(scene,base+0x1b00198);m.d(scene+0x28,800);m.q(scene+0x30,sub);m.str(sub,"Scene_1");
    const auto inspect=[&](Memory& mem){auto read=[&](auto p,auto d,auto n){return mem.read(p,d,n);};NativePlayerStates states{};NativePlayerStunScene out{};
        return readNativePlayerStates(read,base,states)&&readNativePlayerStunScene(read,base,states,out)&&out.timeMs==180&&out.durationMs==800&&out.handle==77;};
    require(inspect(m),"exact native owned stun scene and phase");
    for(const auto* direction:{"right","front","left","back"}){auto v=m;v.str(name,(std::string("midnight/gameplay/player/base/hitreact/hit_react_stun_")+direction+".midnightscene").c_str());require(inspect(v),"exact authored stun direction");}
    auto bad=m;bad.q(bindings+8,actor+0x1000);require(!inspect(bad),"stun cannot bind a foreign actor");
    bad=m;bad.d(instance+0x18,2);require(!inspect(bad),"extra stun scene binding refused");
    bad=m;bad.d(scene+0x28,799);require(!inspect(bad),"unexpected native stun duration refused");
    bad=m;bad.d(instance+0x160,801);require(!inspect(bad),"stun time outside native scene refused");
    bad=m;bad.d(instance+0x27c,2);require(!inspect(bad),"inactive stun scene refused");
    bad=m;bad.d(physics+0x50,1);require(!inspect(bad),"unobserved crouched stun refused");
    bad=m;bad.q(state,base+0x1c86770);require(!inspect(bad),"wrong class cannot claim stun");
    bad=m;bad.q(machine+0x28,mode+1);require(!inspect(bad),"nondefault mode refused");
    bad=m;bad.str(name,"midnight/gameplay/player/base/hitreact/hit_react_unknown.midnightscene");require(!inspect(bad),"unknown hit reaction remains unknown");
    auto overlay=m;const auto eq=state+0x20000;overlay.d(machine+0xb0,2);overlay.d(active+4,53);overlay.q(mode+0x10+53*8,eq);overlay.q(eq,base+0x1c79cf8);overlay.d(eq+8,53);overlay.q(eq+0x10,machine);require(inspect(overlay),"observed fast equipment overlay during stun");
    auto read=[&](auto p,auto d,auto n){return m.read(p,d,n);};NativePlayerStates states{};NativePlayerStunScene out{};require(readNativePlayerStates(read,base,states),"stun fixture state");
    unsigned bindingReads=0;auto concurrent=m;require(!readNativePlayerStunScene([&](auto p,auto d,auto n){const bool ok=concurrent.read(p,d,n);if(p==bindings+8&&++bindingReads==1)concurrent.q(p,actor+1);return ok;},base,states,out),"concurrent scene ownership change refused");
    {   // 1214dn: the knockdown (KnockedDown 16, hit_react_knocked_down Scene_1 2599 ms, an arm then the player bound).
        auto kd=m;const auto arm=actor+0x8000;kd.d(active,16);kd.q(mode+0x10+16*8,state);kd.q(state,base+0x1c827b8);kd.d(state+8,16);
        kd.str(name,"midnight/gameplay/player/base/hitreact/hit_react_knocked_down.midnightscene");kd.d(scene+0x28,2599);kd.d(instance+0x160,1200);
        kd.d(instance+0x18,2);kd.q(bindings+8,arm);kd.q(bindings+0x28,actor);
        const auto inspectKd=[&](Memory& mem){auto read=[&](auto p,auto d,auto n){return mem.read(p,d,n);};NativePlayerStates states{};NativePlayerStunScene out{};
            return readNativePlayerStates(read,base,states)&&readNativePlayerStunScene(read,base,states,out)&&out.variant==5&&out.durationMs==2599&&out.timeMs==1200;};
        require(inspectKd(kd),"native knockdown scene with the arm and the player bound");
        auto b=kd;b.q(state,base+0x1c86768);b.d(state+8,16);require(!inspectKd(b),"knockdown requires the KnockedDown class");
        b=kd;b.d(active,14);b.q(mode+0x10+14*8,state);b.q(state,base+0x1c86768);b.d(state+8,14);require(!inspectKd(b),"a stun state cannot carry the knockdown scene");
        b=kd;b.d(scene+0x28,800);require(!inspectKd(b),"knockdown requires its native duration");
        b=kd;b.d(instance+0x18,1);require(!inspectKd(b),"knockdown requires both bindings");
        b=kd;b.q(bindings+0x28,actor+0x1000);require(!inspectKd(b),"knockdown cannot bind a foreign player");
        b=m;b.str(name,"midnight/gameplay/player/base/hitreact/hit_react_knocked_down.midnightscene");b.d(scene+0x28,2599);require(!inspect(b),"the stun state cannot claim a knockdown");
        require(StunPresentation::valid({1,1,5,1200,2599})&&!StunPresentation::valid({1,1,5,1200,800})&&StunPresentation::clip(5)==380&&StunPresentation::clip(1)==260,"knockdown presentation variant 5, clip 380");
    }
    std::cout<<"PASS: native stun class, scene, clock, binding, world identity and concurrent retirement\n";
}
