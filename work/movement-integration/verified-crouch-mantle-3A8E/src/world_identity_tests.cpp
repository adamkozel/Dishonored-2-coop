#include "world_identity.h"
#include "world_protocol.h"
#include "native_ai_state.h"
#include "native_ai_retirement.h"
#include "native_player_stance.h"
#include "native_player_states.h"
#include "native_player_combat_scene.h"
#include "native_player_equipment_scene.h"
#include "native_player_traversal_scene.h"
#include "native_health.h"
#include "native_sword_equipment.h"
#include "native_player_sword.h"
#include "native_sword_retirement.h"
#include "native_child_frames.h"
#include "native_faction.h"
#include "native_ai_targets.h"
#include <unordered_map>
#include <iostream>
#include <limits>
#include <cstdlib>
// Synthetic native layouts deliberately use unrelated local handles in the
// two processes. Test refusal paths before any identity can drive mutations.
struct Memory {
    std::unordered_map<std::uintptr_t,unsigned char> bytes;
    void put(std::uintptr_t p,const void* data,std::size_t n){auto b=static_cast<const unsigned char*>(data);for(std::size_t i=0;i<n;++i)bytes[p+i]=b[i];}
    void q(std::uintptr_t p,std::uint64_t n){put(p,&n,8);}void d(std::uintptr_t p,std::uint32_t n){put(p,&n,4);}
    void str(std::uintptr_t p,const char* s){put(p,s,std::strlen(s)+1);}
    bool read(std::uintptr_t p,void* data,std::size_t n){auto b=static_cast<unsigned char*>(data);for(std::size_t i=0;i<n;++i){auto it=bytes.find(p+i);if(it==bytes.end())return false;b[i]=it->second;}return true;}
};
void require(bool condition,const char* message){if(!condition){std::cerr<<"FAIL: "<<message<<'\n';std::exit(1);}}
void factionTests(){
    constexpr std::uintptr_t base=0x140000000,game=0x200000000,actor=0x300000000,stack=0x400000000,list=stack+0x1000;
    constexpr std::uintptr_t c=0x500000000,decl=c+0x1000,name=c+0x2000,path=c+0x3000,table=c+0x4000,names=c+0x5000,buckets=c+0x6000;
    constexpr std::uintptr_t node=c+0x7000,targetNode=node+0x100,members=node+0x1000,relations=node+0x2000,overrides=node+0x3000,targetActor=actor+0x1000;
    constexpr std::uint32_t target=0x10002;const auto manager=game+0x1f7bb8;
    Memory m;m.q(base+0x39a5290,game);m.q(game,base+0x1c940a8);m.d(game+0x341058,3);m.q(game+0x1f6ed0,game);m.q(game+0x1f73f0,stack);m.d(game+0x1f73f8,1);m.d(stack,1);m.d(game+0x1f7400,0);
    m.q(game+0x3130b0,actor);m.q(actor,base+0x1c5e428);m.d(actor+0xa8,1);m.q(actor+0x268,list);m.d(actor+0x270,1);m.q(list,c);
    m.q(c,base+0x1ce6268);m.q(c+8,decl);m.q(c+0x10,actor);m.d(c+0x1c,0xa);m.d(c+0x20,0);m.q(c+0x28,name);m.str(name,"Player");m.q(decl,base+0x1d1f7e8);m.q(decl+8,path);m.str(path,"factions/faction_player.cpntfaction");
    m.q(manager,table);m.d(manager+8,2);m.q(manager+0x10,names);m.d(manager+0x18,2);m.q(names,path);
    m.q(manager+0x20,buckets);m.d(manager+0x28,2);m.d(manager+0x2c,2);m.d(manager+0x30,1);m.q(buckets,targetNode);m.q(buckets+8,node);
    m.q(table+0x30,decl);m.q(table,members);m.d(table+8,1);m.d(members,1);m.q(table+0x10,relations);m.d(table+0x18,2);m.d(relations,0);m.d(relations+4,2);
    m.d(node,1);m.d(node+8,0);m.q(node+0x10,overrides);m.d(node+0x18,0);m.q(node+0x20,0);
    m.d(targetNode,target);m.d(targetNode+8,1);m.q(targetNode+0x20,0);m.q(game+0x3130b0+8,targetActor);m.d(targetActor+0xa8,target);
    NativeFaction s;const auto inspect=[&](Memory& mem){return readNativeFaction([&](auto p,auto d,auto n){return mem.read(p,d,n);},base,1,s);};
    require(inspect(m)&&s.relations.size()==2&&s.relations[1].effective==2&&!s.relations[1].overridden,"native faction registration and directional base relation");
    auto changed=m;changed.d(node+0x18,1);changed.d(overrides,target);changed.d(overrides+4,0);require(inspect(changed)&&s.relations[1].effective==0&&s.relations[1].overridden,"entity override precedes faction table");
    changed.d(overrides+4,3);require(inspect(changed)&&s.relations[1].effective==2&&s.relations[1].overridden,"undefined override uses faction relation");
    changed=m;changed.q(node+0x20,node);require(!inspect(changed),"cyclic native hash chain refused");
    changed=m;changed.d(manager+0x2c,3);require(!inspect(changed),"registration count mismatch refused");
    changed=m;changed.d(manager+0x28,65536);require(!inspect(changed),"unbounded hash allocation refused");
    changed=m;changed.d(node+8,1);require(!inspect(changed),"component and registered faction disagreement refused");
    changed=m;changed.q(c+0x10,targetActor);require(!inspect(changed),"faction component owner mismatch refused");
    changed=m;changed.d(members,target);require(!inspect(changed),"missing faction membership refused");
    changed=m;changed.d(targetActor+0xa8,target+1);require(!inspect(changed),"stale registered target refused");
    changed=m;changed.d(relations+4,4);require(!inspect(changed),"invalid relationship value refused");
    changed=m;changed.d(stack,15);require(!inspect(changed),"loading faction observation refused");
    changed=m;unsigned reads=0;require(!readNativeFaction([&](auto p,auto d,auto n){const bool ok=changed.read(p,d,n);if(p==relations+4&&++reads==1)changed.d(p,0);return ok;},base,1,s),"concurrent relationship change refused");
    std::cout<<"PASS: native faction registration, directional overrides, bounds, ownership and concurrent change\n";
}
void healthTests(){
    constexpr std::uintptr_t base=0x140000000,game=0x200000000,actor=0x300000000,stack=0x400000000,list=stack+0x1000;
    constexpr std::uintptr_t health=0x500000000,decl=0x600000000,attributes=0x700000000,table=0x800000000;
    Memory m;m.q(base+0x39a5290,game);m.q(game,base+0x1c940a8);m.d(game+0x341058,3);m.q(game+0x1f6ed0,game);
    m.q(game+0x1f73f0,stack);m.d(game+0x1f73f8,1);m.d(stack,1);m.d(game+0x1f7400,0);
    m.q(game+0x3130b0,actor);m.q(actor,base+0x1c5e428);m.d(actor+0xa8,1);m.q(actor+0x268,list);m.d(actor+0x270,2);m.d(actor+0xd1,0);
    m.q(list,health);m.q(list+8,attributes);m.q(health,base+0x1c51588);m.q(health+8,decl);m.q(health+0x10,actor);m.q(decl,base+0x1c51698);
    const auto f=[&](Memory& mem,std::uintptr_t p,float n){mem.put(p,&n,4);};f(m,health+0x20,56);f(m,decl+0x68,70);f(m,decl+0xb4,1);
    m.q(health+0x68,attributes);m.d(health+0xb0,0);m.d(health+0xb4,1);
    m.q(attributes,base+0x1ca7938);m.q(attributes+0x10,actor);m.q(attributes+0x48,table);m.d(attributes+0x50,2);
    f(m,table+8,0);f(m,table+0x24,10);f(m,table+40+8,0);f(m,table+40+0x24,0);
    NativeHealth s;const auto inspect=[&](Memory& mem){return readNativeHealth([&](auto p,auto d,auto n){return mem.read(p,d,n);},base,1,s);};
    require(inspect(m)&&s.current==56&&s.maximum==80,"native base maximum plus current health modifier");
    auto changed=m;f(changed,table+40+0x24,1);require(inspect(changed)&&s.maximum==70,"suppression attribute disables maximum modifier");
    changed=m;changed.d(actor+0xd1,0x20);require(inspect(changed)&&s.maximum==1,"owner suppression selects declaration maximum");
    changed=m;changed.d(health+0xb0,0xffffffff);require(inspect(changed)&&s.maximum==70,"missing maximum attribute contributes zero");
    changed=m;changed.q(health+0x68,0);require(inspect(changed)&&s.maximum==70,"absent attribute component uses base maximum");
    changed=m;changed.q(attributes+0x10,actor+8);require(!inspect(changed),"health attribute owner mismatch");
    changed=m;changed.d(health+0xb4,2);require(!inspect(changed),"out-of-range suppression index");
    changed=m;changed.d(health+0xb0,0xfffffffe);require(!inspect(changed),"unsupported negative attribute index");
    changed=m;changed.q(decl,base+0x1b05218);require(!inspect(changed),"wrong health declaration class");
    changed=m;changed.d(actor+0x270,3);changed.q(list+16,health);require(!inspect(changed),"duplicate health component");
    changed=m;f(changed,health+0x20,std::numeric_limits<float>::quiet_NaN());require(!inspect(changed),"nonfinite health");
    changed=m;f(changed,health+0x20,-1);require(!inspect(changed),"negative health");
    changed=m;changed.d(stack,15);require(!inspect(changed),"health observation refuses loading");
    const auto loadingHealth=[&](Memory& mem,unsigned handle=1){return readNativeHealth([&](auto p,auto d,auto n){return mem.read(p,d,n);},base,handle,s,true);};
    require(loadingHealth(changed)&&s.logic==15&&s.current==56,"explicit loading player health preserves checkpoint value");
    require(!loadingHealth(m),"loading health reader refuses gameplay");
    require(!loadingHealth(changed,0x10003),"loading health reader refuses NPC handles");
    changed.d(game+0x341058,2);require(!loadingHealth(changed),"loading health refuses incomplete world");
    changed=m;changed.d(stack,15);changed.d(game+0x1f7400,1);require(!loadingHealth(changed),"loading health refuses pending transition");
    changed=m;changed.d(stack,13);f(changed,health+0x20,0);require(inspect(changed)&&s.logic==13,"native game-over health remains observable");
    changed=m;unsigned reads=0;
    require(!readNativeHealth([&](auto p,auto d,auto n){bool ok=changed.read(p,d,n);if(p==health+0x20&&++reads==1)f(changed,p,55);return ok;},base,1,s),"health changing between native snapshots refused");
    std::cout<<"PASS: native health ownership, bounded attributes, suppressed maximum, transition and concurrent-change guards\n";
    {
        auto swordMemory=m;
        constexpr std::uintptr_t inv=0x900000000,items=inv+0x1000,equipmentList=inv+0x2000,itemDecl=inv+0x3000;
        constexpr std::uintptr_t equipDecl=inv+0x4000,weapon=inv+0x5000,cpnt=inv+0x6000,frame=inv+0x7000,names=inv+0x8000,weaponList=inv+0x9000;
        constexpr std::uint32_t weaponHandle=0x30005;
        swordMemory.d(actor+0x270,3);swordMemory.q(list+16,inv);swordMemory.q(inv,base+0x1b9a878);swordMemory.q(inv+0x10,actor);
        swordMemory.q(inv+0x38,items);swordMemory.d(inv+0x40,1);swordMemory.q(inv+0x58,equipmentList);swordMemory.d(inv+0x60,1);swordMemory.q(equipmentList,items);
        for(unsigned i=0;i<16;++i)swordMemory.q(inv+0x68+i*8,0);
        swordMemory.q(items,itemDecl);swordMemory.d(items+8,weaponHandle);swordMemory.q(items+0x10,cpnt);swordMemory.q(items+0x18,equipDecl);
        swordMemory.d(items+0x20,1);swordMemory.d(items+0x24,1);swordMemory.q(itemDecl,base+0x1b9ea48);swordMemory.q(itemDecl+8,names);swordMemory.str(names,kGuardSwordItem);
        swordMemory.q(equipDecl,base+0x1b9ce08);swordMemory.q(equipDecl+8,names+0x100);swordMemory.str(names+0x100,kGuardSwordEquipment);
        swordMemory.d(equipDecl+0x68,0);swordMemory.q(equipDecl+0x70,names+0x200);swordMemory.str(names+0x200,"holster_wpn");
        swordMemory.q(equipDecl+0x78,names+0x300);swordMemory.str(names+0x300,"right_hand_wpn");
        swordMemory.q(game+0x3130b0+24,weapon);swordMemory.q(weapon,base+0x1c87d58);swordMemory.d(weapon+0xa8,weaponHandle);swordMemory.d(weapon+0xac,1);
        swordMemory.q(weapon+0x268,weaponList);swordMemory.d(weapon+0x270,2);swordMemory.q(weaponList,cpnt);swordMemory.q(weaponList+8,frame);
        swordMemory.q(cpnt,base+0x1b9cf48);swordMemory.q(cpnt+8,equipDecl);swordMemory.q(cpnt+0x10,weapon);swordMemory.d(cpnt+0x1c,0x20000002);
        swordMemory.q(weapon+0x358,frame);swordMemory.q(frame,base+0x1c573e8);swordMemory.q(frame+0x10,weapon);swordMemory.q(frame+0x3c8,actor);
        swordMemory.q(frame+0x3c0,names+0x200);swordMemory.d(frame+0x3d0,195);
        const float pose[12]={0,0,0,1,0,0,0,1,0,0,0,1};swordMemory.put(weapon+0x300,pose,sizeof(pose));swordMemory.put(weapon+0x78,pose,sizeof(pose));
        NativeSwordEquipment sword{};
        const auto inspectSword=[&](Memory& mem){return readNativeSwordEquipment([&](auto p,auto d,auto n){return mem.read(p,d,n);},base,1,sword);};
        require(inspectSword(swordMemory)&&!sword.equipped&&!sword.started&&sword.attachedTag=="holster_wpn","registered sword inventory and native holster binding");
        auto draw=swordMemory;draw.q(inv+0x68,items);draw.d(cpnt+0x1c,0x2000000a);draw.q(frame+0x3c0,names+0x300);draw.d(frame+0x3d0,196);
        require(inspectSword(draw)&&sword.equipped&&sword.started&&sword.attachedTag=="right_hand_wpn","native equipped sword and right-hand attachment");
        auto gameOver=draw;gameOver.d(stack,13);require(inspectSword(gameOver)&&sword.logic==13,"native game-over retains read-only equipment identity for cleanup");
        auto bad=swordMemory;bad.q(equipmentList,items+1);require(!inspectSword(bad),"unaligned inventory entry refused");
        bad=swordMemory;bad.d(inv+0x40,257);require(!inspectSword(bad),"unbounded item allocation refused");
        bad=swordMemory;bad.d(inv+0x60,2);bad.q(equipmentList+8,items);require(!inspectSword(bad),"duplicate sword identity refused");
        bad=swordMemory;bad.q(inv+0x68,items+56);require(!inspectSword(bad),"unresolved occupant of native slot refused");
        bad=swordMemory;bad.d(weapon+0xa8,weaponHandle+1);require(!inspectSword(bad),"reused weapon entity handle refused");
        bad=swordMemory;bad.d(weapon+0xac,99);require(!inspectSword(bad),"weapon inventory parent mismatch refused");
        bad=swordMemory;bad.q(cpnt+0x10,actor);require(!inspectSword(bad),"weapon component owner mismatch refused");
        bad=swordMemory;bad.q(weaponList,frame);require(!inspectSword(bad),"unregistered weapon component refused");
        bad=swordMemory;bad.q(frame+0x3c8,weapon);require(!inspectSword(bad),"attachment parent mismatch refused");
        bad=swordMemory;bad.d(frame+0x3d0,223);require(!inspectSword(bad),"out-of-range attachment joint refused");
        bad=swordMemory;bad.str(names+0x200,"unknown_tag");require(!inspectSword(bad),"unknown attachment tag refused");
        bad=swordMemory;bad.d(items+0x20,0);require(!inspectSword(bad),"unavailable inventory quantity refused");
        bad=swordMemory;bad.d(stack,15);require(!inspectSword(bad),"loading weapon state refused");
        bad=swordMemory;unsigned n=0;
        require(!readNativeSwordEquipment([&](auto p,auto d,auto size){const bool ok=bad.read(p,d,size);if(p==cpnt+0x1c&&++n==1)bad.d(p,0x2000000a);return ok;},base,1,sword),"concurrent equipment activation refused");
        std::cout<<"PASS: native sword inventory, quantity, component registration, tags, slot and attachment identity\n";
        {
            auto playerSword=swordMemory;
            constexpr auto bodyDefinition=inv+0xc000,armDefinition=inv+0xd000,arm=inv+0xe000,playerChildren=inv+0xf000;
            constexpr std::uint32_t armHandle=0x40005;
            playerSword.q(actor+0x2e8,bodyDefinition);playerSword.q(bodyDefinition,base+0x1ab58f8);playerSword.q(bodyDefinition+8,names+0x400);
            playerSword.str(names+0x400,"models/characters/player/corvo/body.def");
            playerSword.str(names,"models/weapons/ply_sword/ply_sword");playerSword.str(names+0x100,"components/characters/player/base/weapons/sword.cpntweaponmeleeplayer");
            playerSword.q(cpnt,base+0x1b9d508);playerSword.q(equipDecl,base+0x1b9d3c8);playerSword.q(weapon,base+0x1ce1e98);
            playerSword.str(names+0x200,"sword_attach_tag");playerSword.str(names+0x300,"weapon_attach_tag");playerSword.d(frame+0x3d0,2);
            NativePlayerSword observed{};
            const auto inspectPlayerSword=[&](Memory& mem){return readNativePlayerSword([&](auto p,auto d,auto size){return mem.read(p,d,size);},base,observed);};
            require(inspectPlayerSword(playerSword)&&observed.character==1&&!observed.sword.equipped,"native Corvo player sword holstered on main body");
            auto absent=playerSword;absent.d(items+0x20,0);require(inspectPlayerSword(absent)&&!observed.sword.equipped,"pre-acquisition holstered weapon is not drawn");
            playerSword.q(game+0x3130b0+32,arm);playerSword.q(arm,base+0x1c5e428);playerSword.d(arm+0xa8,armHandle);playerSword.d(arm+0xac,1);
            playerSword.q(arm+0x20,names+0x500);playerSword.str(names+0x500,"right_arm_2");playerSword.q(arm+0x2e8,armDefinition);
            playerSword.q(armDefinition,base+0x1ab58f8);playerSword.q(armDefinition+8,names+0x600);playerSword.str(names+0x600,"models/characters/player/corvo/right_arm.def");
            playerSword.q(actor+0xb0,playerChildren);playerSword.d(actor+0xb8,1);playerSword.q(playerChildren,arm);
            playerSword.q(inv+0x68,items);playerSword.d(cpnt+0x1c,0x2000000a);playerSword.q(frame+0x3c0,names+0x300);playerSword.d(frame+0x3d0,59);playerSword.q(frame+0x3c8,arm);
            require(inspectPlayerSword(playerSword)&&observed.sword.equipped&&observed.arm.handle==armHandle,"drawn Corvo sword requires separately owned right arm");
            {
                auto eq=playerSword;
                constexpr auto physics=inv+0x10000,machine=inv+0x11000,mode=inv+0x12000,active=inv+0x13000;
                constexpr auto ground=inv+0x14000,equipState=inv+0x15000,instances=inv+0x16000,instance=inv+0x17000;
                constexpr auto sceneDecl=inv+0x18000,scene=inv+0x19000,bindings=inv+0x1a000,strings=inv+0x1b000;
                eq.d(actor+0x270,5);eq.q(list+24,physics);eq.q(list+32,machine);eq.q(physics,base+0x1c73268);eq.q(physics+0x10,actor);
                eq.d(physics+0x50,0);eq.d(physics+0x54,0);const float position[3]={0,0,0};eq.put(actor+0x300,position,12);
                eq.q(machine,base+0x1c73d58);eq.q(machine+0x10,actor);eq.q(machine+0x90,mode);eq.q(machine+0x28,mode);eq.q(mode,machine);
                eq.q(machine+0xa8,active);eq.d(machine+0xb0,2);eq.d(active,0);eq.d(active+4,53);
                eq.q(mode+0x10,ground);eq.q(ground,base+0x1c85628);eq.d(ground+8,0);eq.q(ground+0x10,machine);
                eq.q(mode+0x10+53*8,equipState);eq.q(equipState,base+0x1c79cf8);eq.d(equipState+8,53);eq.q(equipState+0x10,machine);eq.d(equipState+0x24,123);
                eq.q(game+0x1f5338,instances);eq.d(game+0x1f5340,1);eq.q(game+0x1f5948,0);eq.d(game+0x1f5950,0);eq.q(instances,instance);
                eq.d(instance+0x158,123);eq.d(instance+0x160,400);eq.d(instance+0x27c,3);eq.q(instance,sceneDecl);eq.q(instance+8,scene);
                eq.q(sceneDecl,base+0x1affbc8);eq.q(sceneDecl+8,strings);eq.str(strings,"midnight/gameplay/player/base/combat/sword_equip.midnightscene");
                eq.q(scene,base+0x1b00198);eq.d(scene+0x28,1066);eq.q(scene+0x30,strings+0x100);eq.str(strings+0x100,"Scene_2");
                eq.q(instance+0x10,bindings);eq.d(instance+0x18,2);eq.q(bindings+8,arm);eq.q(bindings+0x28,weapon);
                NativePlayerEquipmentScene result{};
                const auto inspectEquipment=[&](Memory& mem){const auto reader=[&](auto p,auto d,auto n){return mem.read(p,d,n);};NativePlayerStates states{};
                    return readNativePlayerStates(reader,base,states)&&readNativePlayerEquipmentScene(reader,base,states,result);};
                require(inspectEquipment(eq)&&result.character==1&&result.variant==1&&result.timeMs==400,"Corvo equip scene requires actual owned arm and sword");
                auto other=eq;other.str(names+0x400,"models/characters/player/emily/body.def");other.str(names+0x600,"models/characters/player/emily/right_arm.def");other.str(strings+0x100,"Scene_1");
                require(inspectEquipment(other)&&result.character==2,"Emily equip scene has its own subscene and arm");
                auto fast=eq;fast.str(strings,"midnight/gameplay/player/base/combat/sword_equip_fast.midnightscene");fast.d(instance+0x18,1);fast.d(scene+0x28,500);
                require(inspectEquipment(fast)&&result.variant==5,"fast mantle recovery binds only the owned right arm");
                auto fastLow=fast;fastLow.str(strings,"midnight/gameplay/player/base/combat/sword_equip_sneak_fast.midnightscene");fastLow.d(scene+0x28,333);fastLow.d(instance+0x160,200);
                fastLow.d(active,6);fastLow.q(mode+0x10+6*8,ground);fastLow.q(ground,base+0x1c84258);fastLow.d(ground+8,6);
                require(inspectEquipment(fastLow)&&result.variant==6,"Corvo crouched fast recovery binds owned arm");
                auto fastLowEmily=fastLow;fastLowEmily.str(names+0x400,"models/characters/player/emily/body.def");fastLowEmily.str(names+0x600,"models/characters/player/emily/right_arm.def");fastLowEmily.str(strings+0x100,"Scene_1");
                require(inspectEquipment(fastLowEmily)&&result.variant==6&&result.character==2,"Emily crouched fast recovery uses her subscene");
                auto wrongFastLow=fastLow;wrongFastLow.d(scene+0x28,500);require(!inspectEquipment(wrongFastLow),"crouched fast recovery rejects standing duration");
                wrongFastLow=fastLow;wrongFastLow.q(bindings+8,weapon);require(!inspectEquipment(wrongFastLow),"crouched recovery requires arm rather than weapon");
                auto fastBad=fast;fastBad.q(bindings+8,weapon);require(!inspectEquipment(fastBad),"fast recovery cannot bind sword in place of right arm");
                fastBad=fast;fastBad.d(instance+0x18,2);require(!inspectEquipment(fastBad),"fast recovery refuses unobserved extra bindings");
                fastBad=fast;fastBad.d(arm+0xac,2);require(!inspectEquipment(fastBad),"fast recovery refuses foreign arm");
                fastBad=fast;fastBad.str(strings,"midnight/gameplay/player/base/combat/sword_equip.midnightscene");require(!inspectEquipment(fastBad),"ordinary equip still requires the sword binding");
                auto bad=eq;bad.q(bindings+0x28,actor);require(!inspectEquipment(bad),"equipment scene cannot bind body instead of sword");
                bad=eq;bad.q(bindings+0x28,arm);require(!inspectEquipment(bad),"duplicate right-arm scene binding refused");
                bad=eq;bad.q(bindings+8,weapon);require(!inspectEquipment(bad),"duplicate sword scene binding refused");
                bad=eq;bad.str(strings+0x100,"Scene_1");require(!inspectEquipment(bad),"foreign character equipment subscene refused");
                bad=eq;bad.d(weapon+0xa8,weaponHandle+1);require(!inspectEquipment(bad),"stale sword generation refused for equipment scene");
                bad=eq;bad.d(instance+0x27c,2);require(!inspectEquipment(bad),"inactive equipment scene refused");
                bad=eq;bad.d(equipState+0x24,124);require(!inspectEquipment(bad),"retired equipment scene handle refused");
                bad=eq;bad.d(scene+0x28,10001);require(!inspectEquipment(bad),"unbounded equipment scene duration refused");
                bad=eq;bad.q(bindings+8,actor);require(!inspectEquipment(bad),"body is not right arm in equipment scene");
                bad=eq;bad.str(strings,"midnight/gameplay/player/base/combat/unknown.midnightscene");require(!inspectEquipment(bad),"unknown equipment source asset refused");
                bad=eq;NativePlayerStates states{};require(readNativePlayerStates([&](auto p,auto d,auto n){return bad.read(p,d,n);},base,states),"equipment concurrency fixture valid");
                unsigned bindingReads=0;require(!readNativePlayerEquipmentScene([&](auto p,auto d,auto n){const bool ok=bad.read(p,d,n);if(p==bindings+8&&++bindingReads==1)bad.q(p,actor);return ok;},base,states,result),"concurrent equipment scene rebinding refused");
                std::cout<<"PASS: native equipment scene ownership, both character subscenes, lifecycle, generations and concurrent binding guards\n";
            }
            auto emily=playerSword;emily.str(names+0x400,"models/characters/player/emily/body.def");emily.str(names+0x600,"models/characters/player/emily/right_arm.def");
            require(inspectPlayerSword(emily)&&observed.character==2&&observed.arm.character==2,"Emily player sword retains exact role-specific arm ownership");
            auto invalid=playerSword;invalid.q(weapon,base+0x1c87d58);require(!inspectPlayerSword(invalid),"NPC sword entity cannot act as actual player weapon");
            invalid=playerSword;invalid.q(cpnt,base+0x1b9cf48);require(!inspectPlayerSword(invalid),"NPC melee component cannot report actual player draw");
            invalid=playerSword;invalid.d(items+0x20,0);require(!inspectPlayerSword(invalid),"drawn player sword must be owned");
            invalid=playerSword;invalid.d(items+0x20,2);require(!inspectPlayerSword(invalid),"unexpected weapon quantity refused");
            invalid=playerSword;invalid.q(inv+0x68,0);require(!inspectPlayerSword(invalid),"started weapon must occupy its native slot");
            invalid=playerSword;invalid.d(cpnt+0x1c,2);require(!inspectPlayerSword(invalid),"equipped slot must have started native component");
            invalid=playerSword;invalid.q(frame+0x3c8,actor);require(!inspectPlayerSword(invalid),"drawn sword cannot remain attached to main body");
            invalid=playerSword;invalid.d(frame+0x3d0,58);require(!inspectPlayerSword(invalid),"wrong first-person attachment joint refused");
            invalid=playerSword;invalid.d(arm+0xac,2);require(!inspectPlayerSword(invalid),"foreign arm refused");
            invalid=playerSword;invalid.q(playerChildren,weapon);require(!inspectPlayerSword(invalid),"unregistered arm refused");
            invalid=playerSword;invalid.str(names+0x500,"left_arm_1");require(!inspectPlayerSword(invalid),"left arm cannot carry right sword state");
            invalid=playerSword;invalid.str(names+0x600,"models/characters/player/emily/right_arm.def");require(!inspectPlayerSword(invalid),"arm from another character refused");
            invalid=playerSword;invalid.d(inv+0x40,1025);require(!inspectPlayerSword(invalid),"player inventory bound enforced");
            invalid=playerSword;invalid.d(weapon+0xa8,weaponHandle+1);require(!inspectPlayerSword(invalid),"stale weapon generation refused");
            invalid=playerSword;invalid.d(stack,13);require(!inspectPlayerSword(invalid),"death screen cannot source weapon state");
            invalid=playerSword;unsigned attachmentReads=0;
            require(!readNativePlayerSword([&](auto p,auto d,auto size){const bool ok=invalid.read(p,d,size);if(p==frame+0x3c8&&++attachmentReads==1)invalid.q(p,actor);return ok;},base,observed),"concurrent player sword attachment change refused");
            std::cout<<"PASS: native player sword slot/component state, both roles, reciprocal arm/body attachments, quantity, phase and concurrent-change guards\n";
        }
        {
            auto corvo=swordMemory;constexpr std::uint32_t corvoHandle=0x10003;
            constexpr auto actorDecl=inv+0xc000,parentDecl=inv+0xd000;
            corvo.q(game+0x3130b0+8,actor);corvo.q(actor,base+0x1c5e108);corvo.d(actor+0xa8,corvoHandle);corvo.d(weapon+0xac,corvoHandle);
            corvo.q(actor+0x2e8,actorDecl);corvo.q(actorDecl,base+0x1ab58f8);corvo.q(actorDecl+8,names+0x400);corvo.str(names+0x400,"models/characters/def/story/corvo_attano.def");
            corvo.str(names,kCorvoSwordItem);corvo.str(names+0x100,"unnamed");corvo.q(equipDecl+0x48,parentDecl);
            corvo.q(parentDecl,base+0x1b9ce08);corvo.q(parentDecl+8,names+0x500);corvo.str(names+0x500,kGuardSwordEquipment);
            const auto inspectCorvo=[&](Memory& mem){return readNativeSwordEquipment([&](auto p,auto d,auto size){return mem.read(p,d,size);},base,corvoHandle,sword,NativeSwordScope::CorvoTutorial);};
            require(inspectCorvo(corvo)&&sword.itemName==kCorvoSwordItem,"Corvo's exact tutorial sword and inherited native declaration");
            require(!readNativeSwordEquipment([&](auto p,auto d,auto size){return corvo.read(p,d,size);},base,corvoHandle,sword),"default guard scope rejects Corvo equipment");
            auto invalid=corvo;invalid.str(names+0x400,"models/characters/def/story/emily_kaldwin.def");require(!inspectCorvo(invalid),"Corvo scope rejects Emily actor");
            invalid=corvo;invalid.str(names,kGuardSwordItem);require(!inspectCorvo(invalid),"Corvo scope rejects guard equipment");
            invalid=corvo;invalid.str(names+0x500,"unknown");require(!inspectCorvo(invalid),"unnamed declaration needs exact inherited sword asset");
            invalid=corvo;invalid.q(equipDecl+0x48,equipDecl);require(!inspectCorvo(invalid),"self-referential inline declaration rejected");
            invalid=corvo;invalid.d(frame+0x3d0,215);require(!inspectCorvo(invalid),"Corvo skeleton's joint bound is stricter than guard's");
            invalid=corvo;invalid.d(weapon+0xac,1);require(!inspectCorvo(invalid),"Corvo weapon must retain its NPC owner");
            auto emily=corvo;emily.str(names+0x400,"models/characters/def/d2coop/emily_partner.def");
            const auto inspectEmily=[&](Memory& mem){return readNativeSwordEquipment([&](auto p,auto d,auto size){return mem.read(p,d,size);},base,corvoHandle,sword,NativeSwordScope::EmilyPartner);};
            require(inspectEmily(emily)&&!inspectCorvo(emily),"dedicated Emily asset has separate equipment scope");
            require(!inspectEmily(corvo),"Emily scope rejects story Corvo");
            invalid=emily;invalid.str(names+0x400,"models/characters/def/story/emily_kaldwin.def");require(!inspectEmily(invalid),"story Emily is not the independently owned partner asset");
            invalid=emily;invalid.d(frame+0x3d0,218);require(inspectEmily(invalid),"Emily final joint is within her 219 joint rig");
            invalid.d(frame+0x3d0,219);require(!inspectEmily(invalid),"Emily joint upper bound rejected");
            invalid=emily;invalid.q(equipDecl+0x48,equipDecl);require(!inspectEmily(invalid),"Emily inline sword parent cannot refer to itself");
            std::cout<<"PASS: dedicated Emily partner scope, exact asset, inherited sword declaration and 219 joint bounds\n";
            std::cout<<"PASS: separate Corvo tutorial sword scope, exact actor/item/base asset, reciprocal owner and joint bounds\n";
        }
        auto dead=swordMemory;constexpr std::uint32_t deadHandle=0x10003;
        dead.q(game+0x3130b0+8,actor);dead.q(actor,base+0x1c5e108);dead.d(actor+0xa8,deadHandle);dead.q(attributes,base+0x1b99028);
        f(dead,health+0x20,0);f(dead,decl+0x68,30); // Existing maximum modifier adds ten.
        dead.d(inv+0x60,0);dead.d(items+8,0x3fff0010);dead.q(items+0x10,0);dead.q(items+0x18,0);dead.d(items+0x20,0);
        dead.d(weapon+0x270,1);dead.q(weaponList,frame);
        require(inspectSword(swordMemory),"recover original sword identity for death fixture");
        const auto prior=sword;auto deadBefore=prior;deadBefore.handle=deadHandle;
        const auto retired=[&](Memory& mem){return readNativeSwordDeathRetirement([&](auto p,auto d,auto size){return mem.read(p,d,size);},base,deadBefore);};
        require(retired(dead),"native death removes inventory equipment while preserving the physical sword entity");
        auto invalid=dead;f(invalid,health+0x20,1);require(!retired(invalid),"living guard cannot retire equipment through death");
        invalid=dead;invalid.q(inv+0x68,items);require(!retired(invalid),"retained inventory slot prevents retirement");
        invalid=dead;invalid.d(inv+0x60,1);require(!retired(invalid),"retained equipment registration prevents retirement");
        invalid=dead;invalid.q(items+0x10,cpnt);require(!retired(invalid),"retained item component prevents retirement");
        invalid=dead;invalid.d(items+0x20,1);require(!retired(invalid),"retained quantity prevents retirement");
        invalid=dead;invalid.q(weaponList,cpnt);require(!retired(invalid),"registered weapon component prevents retirement");
        invalid=dead;invalid.d(weapon+0xa8,weaponHandle+1);require(!retired(invalid),"replacement sword generation cannot prove retirement");
        invalid=dead;invalid.q(inv+0x38,items+56);require(!retired(invalid),"replacement inventory allocation cannot prove retirement");
        invalid=dead;unsigned deathReads=0;
        require(!readNativeSwordDeathRetirement([&](auto p,auto d,auto size){const bool ok=invalid.read(p,d,size);if(p==items+0x20&&++deathReads==1)invalid.d(p,1);return ok;},base,deadBefore),"changing item state between retirement snapshots refused");
        std::cout<<"PASS: native equipment death retirement requires stable inventory removal and no registered melee component\n";
        constexpr auto parentFrame=inv+0xa000,childrenList=inv+0xb000;
        auto children=swordMemory;children.d(actor+0xa8,1);children.q(actor+0x358,parentFrame);children.q(parentFrame,base+0x1c573e8);children.q(parentFrame+0x10,actor);
        children.q(actor+0xb0,childrenList);children.d(actor+0xb8,1);children.q(childrenList,weapon);
        NativeChildFrames childFrames{};
        const auto inspectChildren=[&](Memory& mem){return readNativeChildFrames([&](auto p,auto d,auto size){return mem.read(p,d,size);},base,parentFrame,childFrames);};
        require(inspectChildren(children)&&childFrames.count==1&&childFrames.children[0].frame==frame,"registered child frame preserved under its native parent");
        auto cbad=children;cbad.d(actor+0xb8,33);require(!inspectChildren(cbad),"unbounded child list refused");
        cbad=children;cbad.d(actor+0xb8,2);cbad.q(childrenList+8,weapon);require(!inspectChildren(cbad),"duplicate child refused");
        cbad=children;cbad.q(childrenList,actor);require(!inspectChildren(cbad),"self-parent recursion refused");
        cbad=children;cbad.d(weapon+0xac,99);require(!inspectChildren(cbad),"foreign child parent refused");
        cbad=children;cbad.q(frame+0x10,actor);require(!inspectChildren(cbad),"child frame owner mismatch refused");
        cbad=children;cbad.q(frame+0x3c8,weapon);require(!inspectChildren(cbad),"bone attachment to another parent refused");
        cbad=children;cbad.d(weapon+0xa8,weaponHandle+0x10000);require(!inspectChildren(cbad),"child handle in another pool slot refused");
        // This reader discovers the child's current generation. A new stable
        // generation is valid; replacement during the snapshot must fail.
        cbad=children;cbad.d(weapon+0xa8,weaponHandle+1);require(inspectChildren(cbad),"current child generation discovered");
        cbad=children;unsigned generationReads=0;
        require(!readNativeChildFrames([&](auto p,auto d,auto size){const bool ok=cbad.read(p,d,size);if(p==weapon+0xa8&&++generationReads==2)cbad.d(p,weaponHandle+1);return ok;},base,parentFrame,childFrames),"concurrent child generation replacement refused");
        cbad=children;cbad.d(stack,2);require(!inspectChildren(cbad),"paused child update refused");
        cbad=children;unsigned childReads=0;
        require(!readNativeChildFrames([&](auto p,auto d,auto size){const bool ok=cbad.read(p,d,size);if(p==childrenList&&++childReads==1)cbad.q(p,actor);return ok;},base,parentFrame,childFrames),"concurrent child list replacement refused");
        std::cout<<"PASS: native child-frame ownership, reciprocal entities, bounded list, cycles, pause and concurrent replacement guards\n";
    }
    constexpr auto knowledge=health+0x2000,targets=knowledge+0x2000,targetActor=actor+0x2000;
    constexpr std::uint32_t npcHandle=0x10003,targetHandle=0x20004;
    auto t=m;t.q(game+0x3130b0+8,actor);t.d(actor+0xa8,npcHandle);t.q(actor,base+0x1c5e108);t.q(attributes,base+0x1b99028);
    t.d(actor+0x270,3);t.q(list+16,knowledge);t.q(knowledge,base+0x1b95308);t.q(knowledge+0x10,actor);t.d(knowledge+0x1c,0xa);
    t.q(knowledge+0x168,targets);t.d(knowledge+0x170,1);t.q(knowledge+0xcd0,base+0x1b44828);t.d(knowledge+0xcd8,1);t.d(knowledge+0xcdc,targetHandle);
    t.q(game+0x3130b0+16,targetActor);t.d(targetActor+0xa8,targetHandle);
    t.q(targets,base+0x1b447c8);t.d(targets+8,1);t.d(targets+12,targetHandle);t.d(targets+0x168,2);t.d(targets+0x16c,2);
    NativeAiTargets ai;const auto inspectAi=[&](Memory& mem){return readNativeAiTargets([&](auto p,auto d,auto n){return mem.read(p,d,n);},base,npcHandle,ai);};
    require(inspectAi(t)&&ai.combat.handle==targetHandle&&ai.combat.resolved&&ai.attention.size()==1&&ai.attention[0].perceived==2,"AI combat and attention refer to registered native target");
    changed=t;changed.d(targetActor+0xa8,targetHandle+1);require(inspectAi(changed)&&!ai.combat.resolved&&!ai.attention[0].resolved,"retained stale AI target is reported without rebinding");
    changed=t;changed.d(knowledge+0x170,129);require(!inspectAi(changed),"oversized native attention list refused");
    changed=t;changed.q(knowledge+0x10,targetActor);require(!inspectAi(changed),"AI knowledge wrong owner refused");
    changed=t;changed.q(targets,base+0x1b44828);require(!inspectAi(changed),"combat record cannot stand in for attention record");
    changed=t;changed.d(targets+0x168,4);require(!inspectAi(changed),"unknown perceived relationship refused");
    changed=t;reads=0;require(!readNativeAiTargets([&](auto p,auto d,auto n){const bool ok=changed.read(p,d,n);if(p==knowledge+0xcdc&&++reads==1)changed.d(p,0x3fff0010);return ok;},base,npcHandle,ai),"concurrent AI target change refused");
    std::cout<<"PASS: AI knowledge ownership, bounded targets, stale target resolution and concurrent change\n";
}
void aiSchedulerTests() {
    constexpr std::uintptr_t base=0x140000000,game=0x200000000,actor=0x300000000,manager=0x400000000;
    constexpr std::uintptr_t registry=manager+0x10000,block=manager+0x20000,component=block+7*0x100;
    constexpr std::uintptr_t blocks=manager+0x30000,type=manager+0x40000,indices=manager+0x50000,list=manager+0x60000;
    constexpr std::uint32_t handle=0x10003;
    Memory m;m.q(game,base+0x1c940a8);m.q(game+0x3130b0+8,actor);m.q(game+0x1e8960,manager);
    m.q(actor,base+0x1c5e108);m.d(actor+0xa8,handle);m.q(actor+0x268,list);m.d(actor+0x270,1);m.q(list,component);
    m.q(component,base+0x1cc4108);m.q(component+0x10,actor);m.d(component+0x1c,0xa000000a);m.q(component+0x30,manager+0x70000);
    m.q(manager+0x2e0,registry);m.d(registry+8,16);m.q(registry+0x40,blocks);m.d(registry+0x48,1);m.q(blocks,block);
    m.q(registry+0x58,type);m.d(type+0x5c,0x100);m.q(registry+0x10,indices);m.d(registry+0x18,3);
    m.q(registry+0x20,indices+6);m.q(registry+0x28,indices+6);m.d(registry+0x30,3);m.d(registry+0x38,3);
    const std::uint16_t active[]={1,7,9};m.put(indices,active,sizeof(active));
    NativeAiDecisionState s{};
    const auto inspect=[&](Memory& source){return readNativeAiDecision([&](auto p,auto d,auto n){return source.read(p,d,n);},base,game,handle,s);};
    require(inspect(m)&&s.schedulerActive&&s.poolIndex==7&&s.activeCount==3,"native AI pool index and active membership");
    auto paused=m;const std::uint16_t partition[]={1,9,7};paused.put(indices,partition,sizeof(partition));
    paused.d(component+0x1c,0xa000006a);paused.d(registry+0x38,2);paused.q(registry+0x28,indices+4);
    require(inspect(paused)&&!s.schedulerActive&&s.liveCount==3&&s.activeCount==2,"paused component remains in inactive partition");
    auto reflex=m;reflex.q(component,base+nativeAiVtable(NativeAiKind::Reflex));reflex.q(manager+0x580,registry);
    require(readNativeAiComponent([&](auto p,auto d,auto n){return reflex.read(p,d,n);},base,game,handle,NativeAiKind::Reflex,s)&&
        s.schedulerActive&&s.kind==NativeAiKind::Reflex&&s.tree==0,"reflex uses its verified independent registry");
    auto attack=paused;attack.q(component,base+nativeAiVtable(NativeAiKind::Attack));attack.q(manager+0x600,registry);attack.d(component+0x1c,2);
    require(readNativeAiComponent([&](auto p,auto d,auto n){return attack.read(p,d,n);},base,game,handle,NativeAiKind::Attack,s)&&
        !s.schedulerActive&&(s.flags&0xc)==0,"inactive attack component can be checked without starting it");
    auto idle=paused;idle.q(component,base+nativeAiVtable(NativeAiKind::IdleBreak));idle.q(manager+0x530,registry);
    idle.d(component+0x20,1);idle.d(component+0x118,3);
    const auto inspectIdle=[&](Memory& source){return readNativeAiComponent([&](auto p,auto d,auto n){return source.read(p,d,n);},base,game,handle,NativeAiKind::IdleBreak,s);};
    require(inspectIdle(idle)&&!s.schedulerActive&&s.idleState==1&&s.idleAutoPlay==3,"idle break owns a distinct scheduler and native autoplay state");
    auto badIdle=idle;badIdle.d(component+0x20,4);require(!inspectIdle(badIdle),"unknown idle-break state refused");
    badIdle=idle;badIdle.q(manager+0x530,0);require(!inspectIdle(badIdle),"idle break does not borrow decision registry");
    badIdle=idle;unsigned idleReads=0;
    require(!readNativeAiComponent([&](auto p,auto d,auto n){const bool ok=badIdle.read(p,d,n);if(p==component+0x118&&++idleReads==1)badIdle.d(p,2);return ok;},base,game,handle,NativeAiKind::IdleBreak,s),"changing native autoplay state refused");
    auto changed=m;changed.q(component+0x10,actor+8);require(!inspect(changed),"AI wrong component owner refused");
    changed=m;changed.d(actor+0xa8,handle+1);require(!inspect(changed),"AI stale native handle refused");
    changed=m;changed.q(component,base+0x1b8a588);require(!inspect(changed),"AI reflex is not decision component");
    changed=m;changed.d(type+0x5c,0x101);require(!inspect(changed),"AI misaligned pool member refused");
    changed=m;changed.d(registry+0x38,4);require(!inspect(changed),"AI active count outside live bounds refused");
    changed=m;changed.q(registry+0x28,indices+4);require(!inspect(changed),"AI inconsistent end pointer refused");
    changed=m;changed.d(registry+0x48,257);require(!inspect(changed),"AI unbounded pool blocks refused");
    changed=m;const std::uint16_t duplicate[]={1,7,7};changed.put(indices,duplicate,sizeof(duplicate));require(!inspect(changed),"AI duplicated target index refused");
    changed=m;const std::uint16_t missing[]={1,8,9};changed.put(indices,missing,sizeof(missing));require(!inspect(changed),"AI missing scheduler member refused");
    changed=m;unsigned reads=0;
    require(!readNativeAiDecision([&](auto p,auto d,auto n){const bool ok=changed.read(p,d,n);if(p==component+0x1c&&++reads==1)changed.d(p,0xa000006a);return ok;},base,game,handle,s),"AI changing pause state refused");
    changed=m;reads=0;
    require(!readNativeAiDecision([&](auto p,auto d,auto n){const bool ok=changed.read(p,d,n);if(p==indices&&++reads==1)changed.put(p,missing,sizeof(missing));return ok;},base,game,handle,s),"AI changing scheduler membership refused");
    std::cout<<"PASS: AI scheduler pool membership, inactive partition, lifetime checks, bounds and concurrent-change refusal\n";
}
void playerStanceTests() {
    constexpr std::uintptr_t base=0x140000000,game=0x200000000,player=0x300000000;
    constexpr std::uintptr_t stack=0x400000000,list=0x500000000,component=0x600000000;
    Memory m;m.q(base+0x39a5290,game);m.q(game,base+0x1c940a8);m.d(game+0x341058,3);
    m.q(game+0x1f6ed0,game);m.q(game+0x1f73f0,stack);m.d(game+0x1f73f8,1);m.d(stack,1);m.d(game+0x1f7400,0);
    m.q(game+0x3130b0,player);m.q(player,base+0x1c5e428);m.d(player+0xa8,1);
    m.q(player+0x268,list);m.d(player+0x270,1);m.q(list,component);
    m.q(component,base+0x1c73268);m.q(component+0x10,player);m.d(component+0x50,2);m.d(component+0x54,1);
    const float position[]={1,2,3};m.put(player+0x300,position,sizeof(position));
    NativePlayerStance s{};
    const auto inspect=[&](Memory& source){return readNativePlayerStance([&](auto p,auto d,auto n){return source.read(p,d,n);},base,s);};
    require(inspect(m)&&s.current==2&&s.previous==1&&s.physics==component,"read stance from the actual registered player");
    auto changed=m;changed.q(component+0x10,player+8);require(!inspect(changed),"wrong stance component owner refused");
    changed=m;changed.q(component,base+0x1b80408);require(!inspect(changed),"animation component is not player physics");
    changed=m;changed.d(player+0xa8,0x10001);require(!inspect(changed),"non-player handle refused");
    changed=m;changed.d(player+0x270,257);require(!inspect(changed),"unbounded component list refused");
    changed=m;changed.d(player+0x270,2);changed.q(list+8,component);require(!inspect(changed),"duplicate physics component refused");
    changed=m;changed.d(stack,15);require(!inspect(changed),"loading world refused");
    changed=m;changed.d(game+0x1f7400,2);require(!inspect(changed),"pending native transition refused");
    changed=m;unsigned reads=0;
    require(!readNativePlayerStance([&](auto p,auto d,auto n){const bool ok=changed.read(p,d,n);if(p==component+0x50&&++reads==1)changed.d(p,3);return ok;},base,s),"mid-read stance change refused");
    changed=m;reads=0;
    require(!readNativePlayerStance([&](auto p,auto d,auto n){const bool ok=changed.read(p,d,n);if(p==player+0x300&&++reads==1)changed.q(game+0x3130b0,player+8);return ok;},base,s),"mid-read player replacement refused");
    std::cout<<"PASS: player stance ownership, bounded lookup, native transition and concurrent replacement refusal\n";
    constexpr std::uintptr_t machine=component+0x1000,mode=0x700000000,active=0x800000000,state=0x900000000;
    auto actions=m;actions.d(player+0x270,2);actions.q(list+8,machine);actions.q(machine,base+0x1c73d58);
    actions.q(machine+0x10,player);actions.q(machine+0x90,mode);actions.q(mode,machine);
    actions.q(machine+0x28,mode);
    actions.q(machine+0xa8,active);actions.d(machine+0xb0,1);actions.d(active,2);actions.q(mode+0x20,state);
    actions.q(state,base+0x1c85028);actions.d(state+8,2);actions.q(state+0x10,machine);
    // Build exact ownership chains, then corrupt each boundary independently.
    {
        auto cm=actions;constexpr auto slash=state+0x1000,arm=player+0x2000,children=player+0x4000;
        constexpr auto decl=player+0x5000,armDecl=player+0x6000,names=player+0x7000;
        constexpr auto instance=0xa00000000ull,scenes=instance+0x1000,sceneDecl=instance+0x2000,scene=instance+0x3000,bindings=instance+0x4000;
        cm.d(machine+0xb0,2);cm.d(active+4,39);cm.q(mode+0x10+39*8,slash);cm.q(slash,base+0x1c85d28);cm.d(slash+8,39);cm.q(slash+0x10,machine);cm.d(slash+0x24,42);
        cm.q(game+0x3130b8,arm);cm.q(arm,base+0x1c5e428);cm.d(arm+0xa8,0x10002);cm.d(arm+0xac,1);cm.q(arm+0x20,names);cm.str(names,"right_arm_2");
        cm.q(player+0xb0,children);cm.d(player+0xb8,1);cm.q(children,arm);cm.q(player+0x2e8,decl);cm.q(arm+0x2e8,armDecl);
        cm.q(decl,base+0x1ab58f8);cm.q(armDecl,base+0x1ab58f8);cm.q(decl+8,names+0x100);cm.q(armDecl+8,names+0x200);
        cm.str(names+0x100,"models/characters/player/corvo/body.def");cm.str(names+0x200,"models/characters/player/corvo/right_arm.def");
        cm.q(game+0x1f5338,scenes);cm.d(game+0x1f5340,1);cm.q(game+0x1f5338+0x610,0);cm.d(game+0x1f5340+0x610,0);cm.q(scenes,instance);
        cm.d(instance+0x158,42);cm.d(instance+0x27c,3);cm.d(instance+0x160,100);cm.q(instance,sceneDecl);cm.q(instance+8,scene);
        cm.q(sceneDecl,base+0x1affbc8);cm.q(sceneDecl+8,names+0x300);cm.str(names+0x300,"midnight/gameplay/player/base/combat/sword_slash_right.midnightscene");
        cm.q(scene,base+0x1b00198);cm.d(scene+0x28,1000);cm.q(scene+0x30,names+0x400);cm.str(names+0x400,"Scene_2");
        cm.q(instance+0x10,bindings);cm.d(instance+0x18,2);cm.q(bindings+8,player);cm.q(bindings+0x28,arm);
        NativePlayerCombatScene result{};
        const auto combat=[&](Memory& mem){auto read=[&](auto p,auto d,auto n){return mem.read(p,d,n);};NativePlayerStates ns{};return readNativePlayerStates(read,base,ns)&&readNativePlayerCombatScene(read,base,ns,result);};
        require(combat(cm)&&result.variant==1&&result.handle==42&&result.character==1,"owned native player and arm bind the current slash scene");
        auto bad=cm;bad.q(bindings+0x28,player);require(!combat(bad),"duplicate player binding cannot impersonate sword arm");
        bad=cm;bad.d(arm+0xac,2);require(!combat(bad),"foreign arm owner rejected");
        bad=cm;bad.q(children,player);require(!combat(bad),"arm requires reciprocal child membership");
        bad=cm;bad.d(instance+0x158,43);require(!combat(bad),"stale scene generation rejected");
        bad=cm;bad.d(instance+0x27c,10);require(!combat(bad),"retiring scene rejected");
        bad=cm;bad.d(instance+0x18,3);require(!combat(bad),"unobserved binding count rejected");
        bad=cm;bad.d(scene+0x28,0);require(!combat(bad),"zero scene duration rejected");
        bad=cm;bad.str(names+0x300,"midnight/gameplay/player/base/combat/sword_parry_idle.midnightscene");require(!combat(bad),"block scene cannot impersonate slash");
        bad=cm;bad.str(names+0x300,"midnight/unknown.midnightscene");require(!combat(bad),"unmapped scene rejected");
        bad=cm;bad.d(game+0x1f5340,513);require(!combat(bad),"unbounded manager rejected");
        for(auto location:{slash+0x24,instance+8,bindings+0x28,children}){
            bad=cm;auto read=[&](auto p,auto d,auto n){const bool ok=bad.read(p,d,n);if(p==location)bad.q(p,0);return ok;};NativePlayerStates ns{};
            require(readNativePlayerStates([&](auto p,auto d,auto n){return cm.read(p,d,n);},base,ns)&&!readNativePlayerCombatScene(read,base,ns,result),"concurrent owner, scene or binding replacement rejected");
        }
        std::cout<<"PASS: native combat scene ownership, bounds, generation and concurrent replacement\n";
    }
    {
        auto tm=actions;constexpr auto instance=0xa00000000ull,scenes=instance+0x1000,decl=instance+0x2000,scene=instance+0x3000,bindings=instance+0x4000,names=instance+0x5000;
        tm.d(active,8);tm.q(mode+0x10+8*8,state);tm.q(state,base+0x1c82f68);tm.d(state+8,8);tm.d(state+0x24,42);tm.d(state+0xd8,1);tm.d(state+0xb8,0);
        const float start[]={-32,-147,66.8f},goal[]={-32,-146,67.8f};tm.put(state+0xe8,start,12);tm.put(state+0xbc,goal,12);
        tm.q(game+0x1f5338,scenes);tm.d(game+0x1f5340,1);tm.q(game+0x1f5338+0x610,0);tm.d(game+0x1f5340+0x610,0);tm.q(scenes,instance);
        tm.d(instance+0x158,42);tm.d(instance+0x27c,3);tm.d(instance+0x160,200);tm.q(instance,decl);tm.q(instance+8,scene);
        tm.q(decl,base+0x1affbc8);tm.q(decl+8,names);tm.str(names,"midnight/gameplay/player/base/mobility/mantle_low_idle.midnightscene");
        tm.q(scene,base+0x1b00198);tm.d(scene+0x28,500);tm.q(scene+0x30,names+0x100);tm.str(names+0x100,"Scene_1");
        tm.q(instance+0x10,bindings);tm.d(instance+0x18,1);tm.q(bindings+8,player);
        NativePlayerTraversalScene result{};
        const auto inspect=[&](Memory& mem){auto reader=[&](auto p,auto d,auto n){return mem.read(p,d,n);};NativePlayerStates ns{};return readNativePlayerStates(reader,base,ns)&&readNativePlayerTraversalScene(reader,base,ns,result);};
        require(inspect(tm)&&result.variant==1&&result.timeMs==200&&result.durationMs==500,"owned low mantle scene and geometry");
        constexpr auto arm=player+0x2000,children=player+0x4000,bodyDecl=player+0x5000,armDecl=player+0x6000,armNames=player+0x7000;
        auto two=tm;two.q(game+0x3130b8,arm);two.q(arm,base+0x1c5e428);two.d(arm+0xa8,0x10002);two.d(arm+0xac,1);two.q(arm+0x20,armNames);two.str(armNames,"right_arm_2");
        two.q(player+0xb0,children);two.d(player+0xb8,1);two.q(children,arm);two.q(player+0x2e8,bodyDecl);two.q(arm+0x2e8,armDecl);
        two.q(bodyDecl,base+0x1ab58f8);two.q(armDecl,base+0x1ab58f8);two.q(bodyDecl+8,armNames+0x100);two.q(armDecl+8,armNames+0x200);
        two.str(armNames+0x100,"models/characters/player/corvo/body.def");two.str(armNames+0x200,"models/characters/player/corvo/right_arm.def");
        two.d(instance+0x18,2);two.q(bindings+0x28,arm);
        require(inspect(two),"low mantle accepts actual player and its owned right arm");
        auto crouch=two;crouch.d(state+0xb8,6);crouch.str(names,"midnight/gameplay/player/base/mobility/mantle_low_sneak.midnightscene");crouch.str(names+0x100,"Scene_2");
        require(inspect(crouch)&&result.variant==2&&result.endStance==1,"Corvo crouched mantle owns its native sneak subscene");
        auto crouchEmily=crouch;crouchEmily.str(armNames+0x100,"models/characters/player/emily/body.def");crouchEmily.str(armNames+0x200,"models/characters/player/emily/right_arm.def");crouchEmily.str(names+0x100,"Scene_1");
        require(inspect(crouchEmily)&&result.variant==2&&result.endStance==1,"Emily crouched mantle owns its native sneak subscene");
        auto wrongCrouch=crouch;wrongCrouch.str(names+0x100,"Scene_1");require(!inspect(wrongCrouch),"crouched mantle refuses other character subscene");
        auto soloCrouch=crouch;soloCrouch.d(instance+0x18,1);require(inspect(soloCrouch)&&result.variant==2,"Corvo crouched mantle accepts actual-player-only scene");
        auto soloEmily=crouchEmily;soloEmily.d(instance+0x18,1);require(inspect(soloEmily)&&result.variant==2,"Emily crouched mantle accepts actual-player-only scene");
        wrongCrouch=soloCrouch;wrongCrouch.str(armNames+0x100,"models/characters/player/other/body.def");require(!inspect(wrongCrouch),"crouched mantle refuses unknown player body");
        wrongCrouch=crouch;wrongCrouch.str(armNames+0x200,"models/characters/player/emily/right_arm.def");require(!inspect(wrongCrouch),"crouched mantle refuses arm/body mismatch");
        for(auto location:{player+0x2e8,bodyDecl+8,bodyDecl}){
            auto changed=soloCrouch;unsigned reads=0;auto reader=[&](auto p,auto d,auto n){const bool ok=changed.read(p,d,n);if(p==location&&++reads==1)changed.q(p,0);return ok;};NativePlayerStates ns{};
            require(readNativePlayerStates([&](auto p,auto d,auto n){return soloCrouch.read(p,d,n);},base,ns)&&!readNativePlayerTraversalScene(reader,base,ns,result),"concurrent crouched-mantle body replacement refused");
        }
        auto reversed=two;reversed.q(bindings+8,arm);reversed.q(bindings+0x28,player);require(inspect(reversed),"low mantle binding order is immaterial");
        auto foreign=two;foreign.d(arm+0xac,2);require(!inspect(foreign),"mantle refuses foreign right arm");
        foreign=two;foreign.q(children,player);require(!inspect(foreign),"mantle arm needs reciprocal ownership");
        foreign=two;foreign.q(bindings+0x28,player);require(!inspect(foreign),"mantle refuses duplicate player binding");
        foreign=two;foreign.q(bindings+8,arm);require(!inspect(foreign),"mantle refuses arm-only bindings");
        foreign=reversed;foreign.d(instance+0x18,1);require(!inspect(foreign),"mantle requires actual player alongside optional arm");
        for(auto location:{bindings+0x28,children,arm+0x2e8}){
            foreign=two;unsigned reads=0;auto reader=[&](auto p,auto d,auto n){const bool ok=foreign.read(p,d,n);if(p==location&&++reads==1)foreign.q(p,0);return ok;};NativePlayerStates ns{};
            require(readNativePlayerStates([&](auto p,auto d,auto n){return two.read(p,d,n);},base,ns)&&!readNativePlayerTraversalScene(reader,base,ns,result),"concurrent mantle arm replacement refused");
        }
        auto bad=tm;bad.q(bindings+8,player+8);require(!inspect(bad),"foreign traversal actor refused");
        bad=tm;bad.d(instance+0x18,3);require(!inspect(bad),"unexpected traversal bindings refused");
        bad=tm;bad.str(names,"midnight/gameplay/player/base/mobility/mantle_high_idle.midnightscene");require(!inspect(bad),"unobserved mantle variant refused");
        bad=tm;bad.d(state+0xb8,6);require(!inspect(bad),"unobserved mantle exit refused");
        bad=tm;bad.d(state+0xd8,2);require(!inspect(bad),"unobserved mantle kind refused");
        bad=tm;bad.d(scene+0x28,600);require(!inspect(bad),"unobserved mantle timing refused");
        bad=tm;bad.d(instance+0x160,501);require(!inspect(bad),"expired mantle scene refused");
        for(auto location:{state+0x24,instance+8,bindings+8,state+0xbc}){
            bad=tm;unsigned reads=0;auto reader=[&](auto p,auto d,auto n){const bool ok=bad.read(p,d,n);if(p==location&&++reads==1)bad.q(p,0);return ok;};NativePlayerStates ns{};
            require(readNativePlayerStates([&](auto p,auto d,auto n){return tm.read(p,d,n);},base,ns)&&!readNativePlayerTraversalScene(reader,base,ns,result),"concurrent mantle scene/target/binding replacement refused");
        }
        std::cout<<"PASS: exact native low mantle ownership, scene bounds, geometry and lifetime\n";
    }
    NativePlayerStates actionState{};
    const auto inspectActions=[&](Memory& source){return readNativePlayerStates([&](auto p,auto d,auto n){return source.read(p,d,n);},base,actionState);};
    require(inspectActions(actions)&&actionState.active.size()==1&&actionState.active[0].id==2,"active state ID resolves through current mode");
    std::uint32_t motion=0;require(nativePlayerMotion(actionState,base,motion)&&motion==2,"verified native jump class maps to semantic jump");
    auto unknown=actionState;unknown.defaultMode=false;require(!nativePlayerMotion(unknown,base,motion),"non-default mode is not assumed to share movement semantics");
    unknown=actionState;unknown.active[0].vtable++;require(!nativePlayerMotion(unknown,base,motion),"known ID with wrong native class refused");
    unknown=actionState;unknown.active.push_back(unknown.active[0]);require(!nativePlayerMotion(unknown,base,motion),"ambiguous simultaneous active states refused");
    for(const auto& move:{NativePlayerActiveState{0,0,base+0x1c85628},NativePlayerActiveState{6,0,base+0x1c84258},NativePlayerActiveState{2,0,base+0x1c85028},NativePlayerActiveState{3,0,base+0x1c845c8}}){
        const unsigned expected=move.id==6?1:move.id;
        for(const auto& combat:{NativePlayerActiveState{39,0,base+0x1c85d28},NativePlayerActiveState{44,0,base+0x1c81a68}}){
            unknown=actionState;unknown.active={move,combat};
            require(nativePlayerMotion(unknown,base,motion)&&motion==expected,"native combat overlay preserves movement semantic");
            unknown.active={combat,move};require(nativePlayerMotion(unknown,base,motion)&&motion==expected,"active-list order does not change movement");
            unknown.active[0].vtable++;require(!nativePlayerMotion(unknown,base,motion),"wrong overlay class refused");
            unknown.active={combat};require(!nativePlayerMotion(unknown,base,motion),"combat alone does not invent grounded movement");
            unknown.active={move,combat,combat};require(!nativePlayerMotion(unknown,base,motion),"extra or duplicate overlays refused");
        }
    }
    changed=actions;changed.q(mode,component);require(!inspectActions(changed),"mode owned by another component refused");
    changed=actions;changed.q(state+0x10,component);require(!inspectActions(changed),"active state owned by another component refused");
    changed=actions;changed.d(state+8,3);require(!inspectActions(changed),"state identity mismatch refused");
    changed=actions;changed.d(active,90);require(!inspectActions(changed),"out-of-bounds authored state ID refused");
    changed=actions;changed.d(machine+0xb0,91);require(!inspectActions(changed),"unbounded active list refused");
    changed=actions;changed.d(machine+0xb0,2);changed.d(active+4,2);require(!inspectActions(changed),"duplicate active state refused");
    changed=actions;reads=0;
    require(!readNativePlayerStates([&](auto p,auto d,auto n){const bool ok=changed.read(p,d,n);if(p==active&&++reads==1)changed.d(active,3);return ok;},base,actionState),"mid-read active-state change refused");
    changed=actions;reads=0;
    require(!readNativePlayerStates([&](auto p,auto d,auto n){const bool ok=changed.read(p,d,n);if(p==active&&++reads==1)changed.q(machine+0x90,mode+8);return ok;},base,actionState),"mid-read player-mode replacement refused");
    std::cout<<"PASS: active player states enforce reciprocal ownership, unique bounded IDs and stable mode/list lifetimes\n";
}
void aiDeathRetirementTests() {
    NativeAiComponentState held{};held.game=1;held.actor=2;held.handle=3;held.component=4;
    held.manager=5;held.registry=6;held.poolIndex=7;held.flags=8;held.schedulerActive=true;
    auto now=held;now.flags=0xa0000002;now.schedulerActive=false;
    NativeHealth health{};health.game=1;health.actor=2;health.handle=3;health.logic=1;health.maximum=40;
    require(nativeDeadAiRetirementMatches(held,now,health),"native death retires the same stopped registered AI object");
    auto changed=now;changed.flags|=0x40;require(!nativeDeadAiRetirementMatches(held,changed,health),"remaining AI reference is not retirement");
    changed=now;changed.schedulerActive=true;require(!nativeDeadAiRetirementMatches(held,changed,health),"scheduled AI is not retirement");
    changed=now;++changed.component;require(!nativeDeadAiRetirementMatches(held,changed,health),"replacement component is not retirement");
    changed=now;++changed.manager;require(!nativeDeadAiRetirementMatches(held,changed,health),"replacement world manager is not retirement");
    changed=now;++changed.registry;require(!nativeDeadAiRetirementMatches(held,changed,health),"replacement registry is not retirement");
    health.current=1;require(!nativeDeadAiRetirementMatches(held,now,health),"living actor is not death retirement");
    health.current=0;++health.actor;require(!nativeDeadAiRetirementMatches(held,now,health),"foreign health object is not retirement");
    std::cout<<"PASS: death retirement requires the same dead actor, registered component, empty pause state and stopped scheduler\n";
}
int main() {
    aiDeathRetirementTests();
    factionTests();
    healthTests();
    playerStanceTests();
    aiSchedulerTests();
    constexpr std::uintptr_t base=0x140000000,game=0x200000000,actor=0x300000000,spawner=0x300001000,component=0x300002000,definition=0x300003000;
    const auto fixture=[&](std::uint32_t handle,std::uint32_t parent) {
        Memory m;m.q(game,base+0x1c940a8);m.q(game+0x601c0,0x400000000);m.d(game+0x601c8,2);
        m.d(0x400000000,handle);m.d(0x400000004,parent);
        m.q(game+0x3130b0+(handle>>16)*8,actor);m.q(game+0x3130b0+(parent>>16)*8,spawner);
        m.q(actor,base+0x1c5e108);m.d(actor+0xa8,handle);m.d(actor+0xac,parent);m.q(actor+0x2e8,definition);
        m.q(definition,base+0x1ab58f8);m.q(definition+8,0x400001000);m.str(0x400001000,"models/characters/test.def");
        m.q(spawner,base+0x1c5e948);m.d(spawner+0xa8,parent);m.q(spawner+0x20,0x400002000);m.str(0x400002000,"ld_test_guard");
        m.q(spawner+0x268,0x400003000);m.d(spawner+0x270,1);m.q(0x400003000,component);
        m.q(component,base+0x1c57d78);m.q(component+0x10,spawner);m.q(component+0x30,0x400004000);m.d(component+0x38,1);m.d(0x400004000,handle);
        const float transform[]={1,2,3,1,0,0,0,1,0,0,0,1};m.put(actor+0x300,transform,sizeof(transform));return m;
    };
    auto a=fixture(0x10001,0x20002),b=fixture(0x70051,0x90062);
    const auto catalog=[&](Memory& m){std::vector<NativeWorldNpc> c;unsigned unbound=0;require(readWorldNpcCatalog([&](auto p,auto d,auto n){return m.read(p,d,n);},base,game,c,unbound),"catalog read");return c;};
    auto ac=catalog(a),bc=catalog(b);
    require(ac.size()==1&&bc.size()==1&&ac[0].unique&&bc[0].unique&&ac[0].handle!=bc[0].handle&&ac[0].spawnerName==bc[0].spawnerName&&ac[0].definition==bc[0].definition,"identity independent of local handle");
    auto changed=a;changed.d(0x400004000,0x40001);require(!catalog(changed)[0].unique,"broken reciprocal ownership refused");
    changed=a;changed.d(component+0x38,2);changed.d(0x400004004,0x40001);require(!catalog(changed)[0].unique,"multiple children refused");
    changed=a;changed.d(actor+0xa8,0x10002);require(catalog(changed).empty(),"stale actor generation refused");
    changed=a;changed.q(component+0x10,actor);require(catalog(changed).empty(),"wrong component owner refused");
    changed=a;changed.q(definition,base+1);require(catalog(changed).empty(),"wrong definition type refused");
    changed=a;changed.q(game+0x3130b0+3*8,spawner+0x10000);changed.d(game+0x601c8,3);changed.d(0x400000008,0x30003);
    changed.q(spawner+0x10000,base+0x1c5e948);changed.d(spawner+0x100a8,0x30003);changed.q(spawner+0x10020,0x400002000);
    require(!catalog(changed)[0].unique,"duplicate authored spawner name refused");
    CoopPosePacket host{},client{};host.role=0;client.role=1;host.sequence=client.sequence=1;host.flags=client.flags=1;
    host.sampleTimeUs=client.sampleTimeUs=1000000;
    host.session=11;client.session=22;host.worldEpoch=3;client.worldEpoch=7;host.mapId=client.mapId=123;
    host.gameLogicState=client.gameLogicState=1;host.cinematicMetadataValid=client.cinematicMetadataValid=1;host.cinematicEpoch=client.cinematicEpoch=1;
    host.orientation[0]=host.orientation[4]=host.orientation[8]=client.orientation[0]=client.orientation[4]=client.orientation[8]=1;
    CoopWorldNpcPacket p{};p.sequence=1;p.sourceSession=11;p.targetSession=22;p.sourceWorldEpoch=3;p.targetWorldEpoch=7;p.mapId=123;p.sourceHandle=0x10001;
    constexpr char spawnerName[]="ld_test_guard",definitionName[]="models/characters/test.def";
    std::memcpy(p.spawner,spawnerName,sizeof(spawnerName));std::memcpy(p.definition,definitionName,sizeof(definitionName));p.orientation[0]=p.orientation[4]=p.orientation[8]=1;
    require(validCoopWorldNpc(p,client,host),"valid host packet");
    auto hp=p;hp.healthValid=1;hp.health=40;hp.maximumHealth=40;
    require(validCoopWorldNpc(hp,client,host),"verified health observation accepted");
    hp.health=0;require(validCoopWorldNpc(hp,client,host),"zero health observation accepted without death inference");
    hp.health=50;require(validCoopWorldNpc(hp,client,host),"current above a changed maximum remains observable");
    hp.healthValid=0;require(!validCoopWorldNpc(hp,client,host),"unavailable health with stale payload refused");
    hp=p;hp.version=1;require(!validCoopWorldNpc(hp,client,host),"old world protocol refused");
    hp=p;hp.healthValid=2;require(!validCoopWorldNpc(hp,client,host),"unknown health validity refused");
    hp=p;hp.healthValid=1;hp.health=-1;require(!validCoopWorldNpc(hp,client,host),"negative health refused");
    hp.health=std::numeric_limits<float>::quiet_NaN();require(!validCoopWorldNpc(hp,client,host),"NaN health refused");
    hp.health=0;hp.maximumHealth=1000001;require(!validCoopWorldNpc(hp,client,host),"unbounded health maximum refused");
    auto bad=p;bad.role=1;require(!validCoopWorldNpc(bad,client,host),"client authority refused");
    bad=p;++bad.sourceWorldEpoch;require(!validCoopWorldNpc(bad,client,host),"old source world refused");
    bad=p;++bad.targetWorldEpoch;require(!validCoopWorldNpc(bad,client,host),"old target world refused");
    bad=p;++bad.sourceSession;require(!validCoopWorldNpc(bad,client,host),"wrong source session refused");
    bad=p;++bad.targetSession;require(!validCoopWorldNpc(bad,client,host),"wrong target session refused");
    bad=p;++bad.mapId;require(!validCoopWorldNpc(bad,client,host),"wrong map refused");
    bad=p;bad.position[0]=std::numeric_limits<float>::quiet_NaN();require(!validCoopWorldNpc(bad,client,host),"NaN refused");
    bad=p;bad.orientation[0]=-1;require(!validCoopWorldNpc(bad,client,host),"reflected basis refused");
    bad=p;std::memset(bad.spawner,'a',sizeof(bad.spawner));require(!validCoopWorldNpc(bad,client,host),"unterminated name refused");
    bad=p;bad.spawner[100]='x';require(!validCoopWorldNpc(bad,client,host),"noncanonical name padding refused");
    auto paused=client;paused.gameLogicState=2;require(!validCoopWorldNpc(p,paused,host),"pause refused");
    std::cout<<"PASS: reciprocal NPC identity, unrelated local handles, stale generations, ambiguous spawners, world packet scope and transform validation\n";
}
