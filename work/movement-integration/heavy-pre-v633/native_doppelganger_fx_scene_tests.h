#pragma once
#include "native_doppelganger_fx_scene.h"
inline void doppelgangerFxSceneTests(std::uintptr_t base,std::uintptr_t game){
    Memory m;constexpr std::uintptr_t p=0x910000000,actor=p+0x1000,list=p+0x2000,instance=p+0x3000,decl=p+0x4000,scene=p+0x5000,
        bindings=p+0x6000,group=p+0x7000,tracks=p+0x8000,fx=p+0x9000,records=p+0xa000,keys=p+0xb000,text=p+0xc000;
    constexpr unsigned handle=0x880007;
    NativeDoppelgangerWorld world{};world.actor=actor;world.handle=handle;world.present=true;world.equipment.player.sword.game=game;
    m.q(base+0x39a5290,game);m.q(game,base+0x1c940a8);m.q(game+0x3130b0+(handle>>16)*8,actor);m.d(actor+0xa8,handle);
    m.q(game+0x1f5338,list);m.d(game+0x1f5340,1);m.q(list,instance);m.q(game+0x1f5948,0);m.d(game+0x1f5950,0);
    m.q(instance,decl);m.q(instance+8,scene);m.q(instance+0x10,bindings);m.d(instance+0x18,1);m.d(instance+0x158,41);m.d(instance+0x160,100);m.d(instance+0x27c,3);
    m.q(decl,base+0x1affbc8);m.q(decl+8,text);m.str(text,"midnight/gameplay/human/common/doppelganger/doppelganger_spawn.midnightscene");
    m.q(scene,base+0x1b00198);m.d(scene+0x28,1000);m.q(scene+0x30,text+0x200);m.str(text+0x200,"Scene_1");
    m.q(bindings,group);m.q(bindings+8,actor);m.q(group,base+0x1c6ca98);m.q(group+0xc8,instance);m.q(group+0xd0,actor);m.q(group+0x20,tracks);m.d(group+0x28,5);
    for(unsigned i=0;i<5;++i){auto t=fx+i*0x100;m.q(tracks+i*8,t);m.q(t,base+(i?0x1c7f100:0x1c7f5a8));m.q(t+0x10,group);}
    m.q(fx+0xc0,records);m.d(fx+0xc8,5);m.q(fx+0x18,keys);m.d(fx+0x20,5);
    constexpr const char* sockets[]={"decoy_root","sl_l_arm","sl_r_arm","decoy_spawn_l_hand","decoy_spawn_r_hand"};
    const auto setPhase=[&](Memory& mem,unsigned variant){
        mem.str(text,variant==1?"midnight/gameplay/human/common/doppelganger/doppelganger_spawn.midnightscene":"midnight/gameplay/human/common/doppelganger/doppelganger_dispawn.midnightscene");
        mem.d(scene+0x28,variant==1?1000:466);mem.d(group+0x28,variant==1?5:4);
        for(unsigned i=0;i<5;++i){const auto k=p+0x10000+i*0x1000,prop=k+0x100,d=k+0x200,name=k+0x400,helper=k+0x800;const unsigned h=0x990010+i*0x10000;
            mem.d(records+i*12,67428353+i);mem.d(records+i*12+4,h);mem.d(records+i*12+8,1);
            mem.q(keys+i*24,k);mem.d(keys+i*24+8,67428353+i);for(unsigned j=12;j<24;j+=4)mem.d(keys+i*24+j,0);
            mem.d(k+8,67428353+i);mem.q(k+0x28,prop);mem.q(prop,base+0x1afdfd0);mem.q(prop+8,d);mem.q(prop+0x10,name+0x200);
            mem.q(d,base+0x1ab3138);mem.q(d+8,name);mem.str(name,(std::string("models/effects/gameplay/powers/doppelganger/decoy/spawn_despawn/decoy_")+(variant==1?"spawn":"despawn")+(i?"_body":"")+"_01.fx").c_str());mem.str(name+0x200,sockets[i]);
            mem.q(game+0x3130b0+(h>>16)*8,helper);mem.q(helper,base+0x1c87d58);mem.d(helper+0xa8,h);
        }
    };
    setPhase(m,1);NativeDoppelgangerFxScene out{};
    const auto inspect=[&](Memory& mem){return readNativeDoppelgangerFxScene([&](auto a,auto b,auto n){return mem.read(a,b,n);},base,world,out);};
    require(inspect(m)&&out.variant==1&&out.id==41&&out.timeMs==100,"native spawn scene clock and five exact helpers resolve");
    auto b=m;setPhase(b,2);b.d(instance+0x158,42);require(inspect(b)&&out.variant==2&&out.id==42&&out.instance==instance,"reused scene pointer has distinct disappearance incarnation");
    b=m;b.q(bindings+8,actor+8);require(inspect(b)&&!out.instance,"foreign actor's scene does not become this clone's event");
    b=m;b.d(game+0x1f5340,0);require(inspect(b)&&!out.instance,"stable list absence differs from unknown read");
    for(unsigned i=0;i<8;++i){b=m;switch(i){case 0:b.q(group+0xd0,actor+8);break;case 1:b.q(fx+0x10,group+8);break;case 2:b.d(records,67428352);break;
        case 3:b.d(records+4,handle);break;case 4:b.d(records+12+4,0x990010);break;case 5:b.d(instance+0x160,1001);break;case 6:b.d(scene+0x28,1001);break;case 7:b.str(p+0x10600,"wrong_socket");break;}
        require(!inspect(b),"foreign group/track/helper, duplicate helper, wrong socket and bad clock rejected");}
    b=m;unsigned reads=0;require(!readNativeDoppelgangerFxScene([&](auto a,auto dst,auto n){const auto ok=b.read(a,dst,n);if(a==instance+0x158&&++reads==1)b.d(a,42);return ok;},base,world,out),"scene pointer reuse during snapshot is unknown");
    b=m;require(!readNativeDoppelgangerFxScene([&](auto a,auto dst,auto n){return a!=game+0x1f5338&&b.read(a,dst,n);},base,world,out),"unreadable scene list is never absence");
    std::cout<<"PASS: native clone FX scene identity, helpers, clocks and pooled pointer reuse\n";
}
