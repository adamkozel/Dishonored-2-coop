#include "world_identity.h"
#include "carry_identity.h"
#include "world_protocol.h"
#include "native_ai_state.h"
#include "native_ai_retirement.h"
#include "native_player_stance.h"
#include "native_player_states.h"
#include "native_player_action.h"
#include "native_player_landing_scene.h"
#include "native_player_keyhole_scene.h"
#include "native_player_combat_scene.h"
#include "native_player_carry_scene.h"
#include "native_player_equipment_scene.h"
#include "native_player_traversal_scene.h"
#include "native_player_vault.h"
#include "native_health.h"
#include "native_sword_equipment.h"
#include "native_player_sword.h"
#include "native_player_power_item.h"
#include "native_player_darkvision_item.h"
#include "native_player_windblast_item.h"
#include "native_player_ranged_item.h"
#include "native_crossbow_presentation.h"
#include "native_player_ranged_scene.h"
#include "native_windblast_fx_records.h"
#include "native_player_power_scene.h"
#include "native_player_agility.h"
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
#include "native_crossbow_presentation_tests.h"
#include "native_retargeting_tests.h"
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
                auto agility=playerSword;constexpr auto ae=items+56,ad=inv+0x20000,an=inv+0x21000;
                agility.d(inv+0x40,2);agility.q(ae,ad);agility.q(ad,base+0x1b9ea48);agility.q(ad+8,an);
                agility.str(an,"components/characters/player/base/attributes/super_jump.inventoryitem");agility.d(ad+0x60,18);
                agility.d(ae+8,0x3fff0010);agility.q(ae+0x10,0);agility.q(ae+0x18,0);agility.d(ae+0x20,0);agility.d(ae+0x24,1);
                NativePlayerAgility a{};const auto inspect=[&](Memory& m){return readNativePlayerAgility([&](auto p,auto d,auto n){return m.read(p,d,n);},base,a);};
                require(inspect(agility)&&!a.quantity,"native Agility enhancement identified without granting");
                auto b=agility;b.d(ae+0x20,1);require(inspect(b)&&a.quantity==1,"native owned Agility quantity observed");
                b=agility;b.str(names+0x400,"models/characters/player/emily/body.def");b.str(names+0x600,"models/characters/player/emily/right_arm.def");require(inspect(b)&&a.player.character==2,"Agility identity works on Emily");
                b=agility;b.d(ad+0x60,9);require(!inspect(b),"equipment declaration cannot masquerade as enhancement");
                b=agility;b.q(ae+0x10,cpnt);require(!inspect(b),"unexpected enhancement equipment refused");
                b=agility;b.d(ae+0x20,2);require(!inspect(b),"out-of-scope enhancement quantity refused");
                b=agility;b.str(an,"components/characters/player/base/attributes/unrelated.inventoryitem");require(!inspect(b),"unrelated enhancement refused");
                b=agility;unsigned reads=0;require(!readNativePlayerAgility([&](auto p,auto d,auto n){const bool ok=b.read(p,d,n);if(p==ae+0x20&&++reads==1)b.d(p,1);return ok;},base,a),"concurrent enhancement change refused");
                std::cout<<"PASS: native Agility item identity, both roles, enhancement type, absent equipment and concurrent-change guards\n";
            }
            {
                auto powers=playerSword;constexpr auto pe=items+56,pd=inv+0x20000,pc=inv+0x21000,ped=inv+0x22000,entity=inv+0x23000,components=inv+0x24000;
                constexpr auto strings=inv+0x25000;constexpr unsigned handle=0x50006;
                powers.d(inv+0x40,2);powers.d(inv+0x60,2);powers.q(equipmentList+8,pe);
                powers.q(pe,pd);powers.q(pd,base+0x1b9ea48);powers.q(pd+8,strings);powers.str(strings,"models/powers/blink/blink.inventoryitem");powers.d(pd+0x60,9);
                powers.d(pe+8,handle);powers.q(pe+0x10,pc);powers.q(pe+0x18,ped);powers.d(pe+0x20,0);powers.d(pe+0x24,1);
                powers.q(pc,base+0x1b9bb78);powers.q(pc+8,ped);powers.q(pc+0x10,entity);powers.d(pc+0x1c,0x20000002);
                powers.q(ped,base+0x1b9ba38);powers.q(ped+8,strings+0x100);powers.str(strings+0x100,"components/characters/player/base/powers/blink/blink.cpntpowerblink");powers.d(ped+0x68,1);
                powers.q(game+0x3130b0+40,entity);powers.d(entity+0xa8,handle);powers.d(entity+0xac,1);powers.q(entity+0x268,components);powers.d(entity+0x270,1);powers.q(components,pc);
                NativePlayerPowerItem power{};const auto inspect=[&](Memory& m){return readNativePlayerPowerItem([&](auto p,auto d,auto n){return m.read(p,d,n);},base,power);};
                require(inspect(powers)&&!power.quantity&&!power.equipped,"unavailable owned Corvo Blink component is identifiable without granting");
                auto equipped=powers;equipped.d(pe+0x20,1);equipped.q(inv+0x70,pe);equipped.d(pc+0x1c,0x2000000a);
                require(inspect(equipped)&&power.quantity==1&&power.equipped&&power.started,"native power quantity, selection and component activation read separately");
                auto e=powers;e.str(names+0x400,"models/characters/player/emily/body.def");e.str(names+0x600,"models/characters/player/emily/right_arm.def");
                e.str(strings,"models/powers/grapple/grapple.inventoryitem");e.str(strings+0x100,"components/characters/player/base/powers/grapple/grapple.cpntpowergrapple");e.q(pc,base+0x1b52818);e.q(ped,base+0x1b526d8);
                require(inspect(e)&&power.player.character==2,"Emily Far Reach has distinct native component and declaration");
                auto b=powers;b.d(entity+0xac,2);require(!inspect(b),"foreign power entity parent refused");
                b=powers;b.q(pc+0x10,actor);require(!inspect(b),"foreign power component owner refused");
                b=powers;b.d(entity+0xa8,handle+1);require(!inspect(b),"recycled power handle refused");
                b=powers;b.q(components,cpnt);require(!inspect(b),"unregistered power component refused");
                b=powers;b.q(pc,base+0x1b52818);require(!inspect(b),"cross-character power component refused");
                b=powers;b.d(ped+0x68,0);require(!inspect(b),"power cannot replace sword slot");
                b=powers;b.d(pe+0x20,2);require(!inspect(b),"out-of-scope power quantity refused");
                b=powers;unsigned reads=0;require(!readNativePlayerPowerItem([&](auto p,auto d,auto n){const bool ok=b.read(p,d,n);if(p==pc+0x1c&&++reads==1)b.d(p,0x2000000a);return ok;},base,power),"concurrent power activation refused");
                std::cout<<"PASS: both native starting power identities, quantity/selection, ownership, slot, generation and concurrent activation guards\n";
                for(unsigned role=1;role<=2;++role)for(unsigned kind=1;kind<=2;++kind){
                    auto ranged=powers;const bool corvo=role==1;
                    if(!corvo){ranged.str(names+0x400,"models/characters/player/emily/body.def");ranged.str(names+0x600,"models/characters/player/emily/right_arm.def");}
                    ranged.str(strings,kind==1?(corvo?"models/weapons/gun_player/gun_player.inventoryitem":"models/weapons/gun_player/gun_emily.inventoryitem"):
                        (corvo?"models/weapons/crossbow/crossbow_corvo.inventoryitem":"models/weapons/crossbow/crossbow_emily.inventoryitem"));
                    ranged.str(strings+0x100,kind==1?(corvo?"components/characters/player/corvo/weapons/gun.cpntweapongun":"components/characters/player/emily/weapons/gun.cpntweapongun"):
                        "components/characters/player/base/weapons/crossbow.cpntweaponcrossbow");
                    ranged.d(pd+0x60,2);ranged.q(pc,base+(kind==1?0x1b53f58:0x1b9cbf8));ranged.q(ped,base+(kind==1?0x1b53e18:0x1b9cab8));ranged.q(entity,base+0x1ce1e98);
                    NativePlayerRangedItem ri{};
                    const auto inspectRanged=[&](Memory& m,bool selected=false){return readNativePlayerRangedItem([&](auto p,auto d,auto n){return m.read(p,d,n);},base,kind,ri,selected);};
                    require(inspectRanged(ranged)&&ri.player.character==role&&ri.kind==kind&&!ri.quantity&&!ri.equipped,"unacquired native ranged equipment readable on either role");
                    require(!inspectRanged(ranged,true),"unselected ranged weapon cannot source an action");
                    auto selected=ranged;selected.d(pe+0x20,1);selected.q(inv+0x70,pe);selected.d(pc+0x1c,0x2000000a);
                    require(inspectRanged(selected,true)&&ri.equipped&&ri.started,"exact selected ranged weapon independently validates activation");
                    auto invalid=selected;invalid.d(pd+0x60,9);require(!inspectRanged(invalid),"power item cannot masquerade as ranged weapon");
                    invalid=selected;invalid.q(pc,base+0x1b9d508);require(!inspectRanged(invalid),"sword component refused as ranged weapon");
                    invalid=selected;invalid.q(pc+0x10,actor);require(!inspectRanged(invalid),"foreign ranged component owner refused");
                    invalid=selected;invalid.d(entity+0xa8,handle+1);require(!inspectRanged(invalid),"recycled ranged entity refused");
                    invalid=selected;invalid.d(pe+0x20,0);require(!inspectRanged(invalid),"equipped zero-quantity weapon refused");
                    invalid=selected;invalid.d(ped+0x68,0);require(!inspectRanged(invalid),"ranged weapon cannot occupy sword slot");
                    invalid=selected;invalid.q(components,cpnt);require(!inspectRanged(invalid),"unregistered ranged component refused");
                    invalid=selected;invalid.str(strings,"models/weapons/gun_player/unrelated.inventoryitem");require(!inspectRanged(invalid),"unrelated or foreign-role item refused");
                    invalid=selected;unsigned reads=0;
                    require(!readNativePlayerRangedItem([&](auto p,auto d,auto n){const bool ok=invalid.read(p,d,n);if(p==pe+0x20&&++reads==1)invalid.d(p,0);return ok;},base,kind,ri),"concurrent ranged quantity change refused");
                    {
                        constexpr auto ae=items+112,ad=inv+0x29000,an=inv+0x2a000;
                        auto ammo=ranged;ammo.d(inv+0x40,3);ammo.q(ae,ad);ammo.q(ad,base+0x1b9ea48);ammo.q(ad+8,an);ammo.d(ad+0x60,4);
                        ammo.str(an,kind==1?"components/equipment/ammo/bullet.inventoryitem":"components/equipment/ammo/bolt_regular.inventoryitem");
                        ammo.d(ae+8,0x3fff0010);ammo.q(ae+0x10,0);ammo.q(ae+0x18,0);ammo.d(ae+0x20,0);ammo.d(ae+0x24,10);
                        NativeRangedAmmo a{};const auto inspectAmmo=[&](Memory& m){return inspectRanged(m)&&readNativeRangedAmmo([&](auto p,auto d,auto n){return m.read(p,d,n);},base,ri,a);};
                        require(inspectAmmo(ammo)&&a.quantity==0&&a.maximum==10,"native regular ammunition scoped to exact ranged kind");
                        auto bad=ammo;bad.d(ae+0x20,2);require(inspectAmmo(bad)&&a.quantity==2,"two-round native observation fixture quantity");
                        bad=ammo;bad.str(an,"components/equipment/ammo/bolt_explosive.inventoryitem");require(!inspectAmmo(bad),"special ammunition refused");
                        bad=ammo;bad.d(ad+0x60,2);require(!inspectAmmo(bad),"equipment cannot masquerade as ammunition");
                        bad=ammo;bad.d(ae+0x24,20);require(!inspectAmmo(bad),"unobserved capacity refused");
                        bad=ammo;bad.d(ae+0x20,11);require(!inspectAmmo(bad),"excessive ammunition refused");
                        require(inspectRanged(ammo),"ammo fixture player scope valid");reads=0;
                        require(!readNativeRangedAmmo([&](auto p,auto d,auto n){const bool ok=ammo.read(p,d,n);if(p==ae+0x20&&++reads==1)ammo.d(p,1);return ok;},base,ri,a),"concurrent native ammo change refused");
                    }
                    if(kind==1){
                        constexpr auto ue=items+112,ud=inv+0x29000,un=inv+0x2a000;
                        auto defaults=ranged;defaults.d(inv+0x40,3);defaults.q(ue,ud);defaults.q(ud,base+0x1b555a8);defaults.q(ud+8,un);
                        defaults.str(un,corvo?"components/characters/player/base/attributes/gun_magazine_0_corvo.equipmentupgrade":"components/characters/player/base/attributes/gun_accuracy_0_emily.equipmentupgrade");
                        defaults.d(ue+8,0x3fff0010);defaults.q(ue+0x10,0);defaults.q(ue+0x18,0);defaults.d(ue+0x20,0);defaults.d(ue+0x24,1);
                        NativeRangedDefaultUpgrade upgrade{};
                        const auto checkUpgrade=[&](Memory& m){return inspectRanged(m)&&readNativeRangedDefaultUpgrade([&](auto p,auto d,auto n){return m.read(p,d,n);},base,ri,upgrade);};
                        require(checkUpgrade(defaults)&&!upgrade.quantity,"role-specific pistol default upgrade captured before native acquisition");
                        auto broken=defaults;broken.d(ue+0x20,1);require(checkUpgrade(broken)&&upgrade.quantity==1,"acquired default upgrade is separately observable");
                        broken=defaults;broken.str(un,"components/characters/player/base/attributes/unrelated.equipmentupgrade");require(!checkUpgrade(broken),"unrelated upgrades cannot be restored by ranged fixture");
                        broken=defaults;broken.q(ue+0x10,pc);require(!checkUpgrade(broken),"unexpected upgrade component refused");
                        broken=defaults;broken.d(ue+0x20,2);require(!checkUpgrade(broken),"out-of-scope default upgrade quantity refused");
                        require(inspectRanged(defaults),"default upgrade test inventory scope valid");reads=0;
                        require(!readNativeRangedDefaultUpgrade([&](auto p,auto d,auto n){const bool ok=defaults.read(p,d,n);if(p==ue+0x20&&++reads==1)defaults.d(p,1);return ok;},base,ri,upgrade),"concurrent default-upgrade change refused");
                    }
                }
                std::cout<<"PASS: native ranged identities on both roles, selection, quantity, slot, generation, ownership and concurrent-read guards\n";
                auto dark=powers;constexpr auto manager=inv+0x26000;
                dark.str(strings,"models/powers/darkvision/darkvision.inventoryitem");
                dark.str(strings+0x100,"components/characters/player/base/powers/darkvision/darkvision.cpntpowerdarkvision");
                dark.q(pc,base+0x1b51c18);dark.q(ped,base+0x1b51ad8);dark.q(pc+0xb8,manager);
                dark.d(actor+0x270,4);dark.q(list+24,manager);dark.q(manager,base+0x1b5eb68);dark.q(manager+0x10,actor);dark.d(manager+0x1c,0x20000002);
                NativeDarkVisionItem dv{};const auto readDark=[&](Memory& m){return readNativeDarkVisionItem([&](auto p,auto d,auto n){return m.read(p,d,n);},base,dv);};
                require(readDark(dark)&&dv.instantiated&&!dv.managerStarted,"native Dark Vision equipment and owned player manager identified");
                auto fullPlayer=dark;fullPlayer.d(actor+0x270,88);for(unsigned i=4;i<88;++i){const auto otherComponent=inv+0x30000+i*0x100;fullPlayer.q(list+i*8,otherComponent);fullPlayer.q(otherComponent,base+0x1234);}
                require(readDark(fullPlayer),"actual 88-component player list is valid; manager still uniquely registered");
                fullPlayer.q(list+87*8,manager);require(!readDark(fullPlayer),"large player list cannot duplicate the vision manager");
                auto missing=dark;missing.d(inv+0x60,1);missing.d(pe+8,0x3fff0010);missing.q(pe+0x10,0);missing.q(pe+0x18,0);
                require(readDark(missing)&&!dv.instantiated&&!dv.quantity,"unacquired Dark Vision declaration does not imply an existing equipment entity");
                auto activeDark=dark;activeDark.d(pe+0x20,1);activeDark.q(inv+0x70,pe);activeDark.d(pc+0x1c,0x2000000a);activeDark.d(manager+0x1c,0x2000000a);
                require(readDark(activeDark)&&dv.equipped&&dv.started&&dv.managerStarted,"Dark Vision equipment and vision manager activation are separate native facts");
                {
                    auto wind=activeDark;
                    wind.str(strings,"models/powers/windblast/windblast.inventoryitem");
                    wind.str(strings+0x100,"components/characters/player/base/powers/windblast/windblast.cpntpowerwindblast");
                    wind.q(pc,base+0x1b53488);wind.q(ped,base+0x1b53348);
                    NativeWindblastItem wb{};const auto inspectWind=[&](Memory& m){return readNativeWindblastItem([&](auto p,auto d,auto n){return m.read(p,d,n);},base,wb);};
                    require(inspectWind(wind)&&wb.instantiated&&wb.equipped&&wb.started,"exact Corvo Windblast equipment identity");
                    auto absent=wind;absent.d(inv+0x60,1);absent.q(inv+0x70,0);absent.d(pe+8,0x3fff0010);absent.q(pe+0x10,0);absent.q(pe+0x18,0);absent.d(pe+0x20,0);
                    require(inspectWind(absent)&&!wb.instantiated&&!wb.quantity,"unacquired Windblast native item is readable without a spawned entity");
                    auto invalid=wind;invalid.str(names+0x400,"models/characters/player/emily/body.def");invalid.str(names+0x600,"models/characters/player/emily/right_arm.def");require(!inspectWind(invalid),"Windblast fixture rejects unsupported native character");
                    invalid=wind;invalid.q(pc,base+0x1b51c18);require(!inspectWind(invalid),"Windblast rejects another power component");
                    invalid=wind;invalid.q(pc+0x10,actor);require(!inspectWind(invalid),"Windblast rejects foreign component ownership");
                    invalid=wind;invalid.d(entity+0xa8,handle+1);require(!inspectWind(invalid),"Windblast rejects recycled entity handle");
                    std::cout<<"PASS: native Windblast exact item, Corvo scope, unacquired and equipped identity guards\n";
                }
                require(readNativeDarkVisionItem([&](auto p,auto d,auto n){return activeDark.read(p,d,n);},base,dv,true),"selected Dark Vision cast lookup does not require a full inventory scan");
                require(!readNativeDarkVisionItem([&](auto p,auto d,auto n){return dark.read(p,d,n);},base,dv,true),"unselected Dark Vision cannot source a cast");
                auto emilyDark=activeDark;emilyDark.str(names+0x400,"models/characters/player/emily/body.def");emilyDark.str(names+0x600,"models/characters/player/emily/right_arm.def");
                require(readDark(emilyDark)&&dv.player.character==2,"Dark Vision exact equipment is shared by both native characters");
                {
                    auto cast=activeDark;
                    constexpr auto physics=inv+0x40000,machine=inv+0x41000,mode=inv+0x42000,active=inv+0x43000,ground=inv+0x44000;
                    constexpr auto leftArm=inv+0x45000,leftDecl=inv+0x46000,instances=inv+0x47000,instance=inv+0x48000,sceneDecl=inv+0x49000,scene=inv+0x4a000,bindings=inv+0x4b000,texts=inv+0x4c000;
                    constexpr unsigned leftHandle=0x60007;
                    cast.d(actor+0x270,6);cast.q(list+32,physics);cast.q(list+40,machine);cast.q(physics,base+0x1c73268);cast.q(physics+0x10,actor);
                    cast.d(physics+0x50,0);cast.d(physics+0x54,0);const float pos[3]={0,0,0};cast.put(actor+0x300,pos,sizeof(pos));
                    cast.q(machine,base+0x1c73d58);cast.q(machine+0x10,actor);cast.q(machine+0x90,mode);cast.q(machine+0x28,mode);cast.q(mode,machine);
                    cast.q(machine+0xa8,active);cast.d(machine+0xb0,1);cast.d(active,0);cast.q(mode+0x10,ground);cast.q(ground,base+0x1c85628);cast.d(ground+8,0);cast.q(ground+0x10,machine);
                    cast.q(game+0x3130b0+48,leftArm);cast.q(leftArm,base+0x1c5e428);cast.d(leftArm+0xa8,leftHandle);cast.d(leftArm+0xac,1);
                    cast.q(leftArm+0x20,texts);cast.str(texts,"left_arm_1");cast.q(leftArm+0x2e8,leftDecl);cast.q(leftDecl,base+0x1ab58f8);cast.q(leftDecl+8,texts+0x100);cast.str(texts+0x100,"models/characters/player/corvo/left_arm.def");
                    cast.d(actor+0xb8,2);cast.q(playerChildren+8,leftArm);
                    cast.q(game+0x1f5338,instances);cast.d(game+0x1f5340,1);cast.q(game+0x1f5948,0);cast.d(game+0x1f5950,0);cast.q(instances,instance);
                    cast.q(instance,sceneDecl);cast.q(sceneDecl,base+0x1affbc8);cast.q(sceneDecl+8,texts+0x200);cast.str(texts+0x200,"midnight/gameplay/player/base/powers/power_darkvision_cast.midnightscene");
                    cast.q(instance+8,scene);cast.q(scene,base+0x1b00198);cast.d(scene+0x28,933);cast.q(scene+0x30,texts+0x300);cast.str(texts+0x300,"Scene_1");
                    cast.d(instance+0x27c,3);cast.d(instance+0x158,219);cast.d(instance+0x160,358);cast.q(instance+0x10,bindings);cast.d(instance+0x18,1);cast.q(bindings+8,leftArm);
                    NativePlayerPowerScene observedCast{};
                    const auto inspectCast=[&](Memory& mem){const auto rd=[&](auto p,auto d,auto n){return mem.read(p,d,n);};NativePlayerStates state{};return readNativePlayerStates(rd,base,state)&&readNativePlayerPowerScene(rd,base,state,observedCast);};
                    require(inspectCast(cast)&&observedCast.variant==10&&observedCast.timeMs==358&&observedCast.arm.side==1,"native Dark Vision cast requires selected equipment and reciprocal left-arm binding");
                    for(unsigned role=1;role<=2;++role)for(unsigned kind=1;kind<=2;++kind){
                        auto ranged=cast;const bool corvo=role==1;
                        if(!corvo){ranged.str(names+0x400,"models/characters/player/emily/body.def");ranged.str(names+0x600,"models/characters/player/emily/right_arm.def");ranged.str(texts+0x100,"models/characters/player/emily/left_arm.def");}
                        ranged.str(strings,kind==1?(corvo?"models/weapons/gun_player/gun_player.inventoryitem":"models/weapons/gun_player/gun_emily.inventoryitem"):(corvo?"models/weapons/crossbow/crossbow_corvo.inventoryitem":"models/weapons/crossbow/crossbow_emily.inventoryitem"));
                        ranged.str(strings+0x100,kind==1?(corvo?"components/characters/player/corvo/weapons/gun.cpntweapongun":"components/characters/player/emily/weapons/gun.cpntweapongun"):"components/characters/player/base/weapons/crossbow.cpntweaponcrossbow");
                        ranged.d(pd+0x60,2);ranged.q(pc,base+(kind==1?0x1b53f58:0x1b9cbf8));ranged.q(ped,base+(kind==1?0x1b53e18:0x1b9cab8));ranged.q(entity,base+0x1ce1e98);
                        ranged.d(instance+0x18,2);ranged.q(bindings+0x28,entity);ranged.str(texts+0x200,kind==1?"midnight/gameplay/player/base/guns/gun_ooa.midnightscene":"midnight/gameplay/player/base/guns/crossbow_ooa.midnightscene");ranged.d(scene+0x28,500);
                        NativePlayerRangedScene result{};
                        const auto inspectRangedScene=[&](Memory& mem){const auto rd=[&](auto p,auto d,auto n){return mem.read(p,d,n);};NativePlayerStates st{};return readNativePlayerStates(rd,base,st)&&readNativePlayerRangedScene(rd,base,st,result);};
                        require(inspectRangedScene(ranged)&&result.variant==kind&&result.timeMs==358&&result.weapon.player.character==role,"native empty trigger requires exact role weapon and owned left arm");
                        for(unsigned id:{2u,3u}){
                            auto flying=ranged;flying.d(active,id);flying.q(mode+0x10+id*8,ground);flying.d(ground+8,id);flying.q(ground,base+(id==2?0x1c85028:0x1c845c8));
                            const float apex[2]={67.f,68.5f};flying.put(ground+0xa0,apex,sizeof(apex));flying.d(ground+0x94,0);
                            require(inspectRangedScene(flying)&&result.variant==kind,"owned native empty trigger scene remains readable while airborne");
                            const auto fire=inv+0x4d000;flying.d(machine+0xb0,2);flying.d(active+4,56);flying.q(mode+0x10+56*8,fire);
                            flying.q(fire,base+0x1c849d8);flying.d(fire+8,56);flying.q(fire+0x10,machine);
                            require(inspectRangedScene(flying),"observed native Jump/Fall plus FireWeapon overlay retains exact empty trigger binding");
                            const auto rd=[&](auto p,auto d,auto n){return flying.read(p,d,n);};NativePlayerStates st{};unsigned stage=0,remaining=0;float start=0,target=0;
                            require(readNativePlayerStates(rd,base,st)&&nativePlayerAir(rd,st,base,stage,start,target,remaining,false,true)&&stage==(id==2?1u:2u),"owned ranged scene allows independently validated native air metadata");
                            require(!nativePlayerAir(rd,st,base,stage,start,target,remaining),"unowned ranged overlay cannot publish air metadata");
                            auto invalid=flying;invalid.q(fire,base+0x1c79cf8);require(!inspectRangedScene(invalid),"airborne ranged overlay requires exact native class");
                            invalid=flying;invalid.q(bindings+8,arm);require(!inspectRangedScene(invalid),"airborne scene still refuses a right-arm substitute");
                            invalid=flying;invalid.d(physics+0x50,1);require(!inspectRangedScene(invalid),"unobserved crouched airborne ranged state refused");
                        }
                        struct Case{const char* clip;const char* sub;unsigned duration,variant;};
                        for(const auto& c:{Case{"fire","Fire",666,kind==1?3u:7u},Case{"reload",kind==1?"DefaultReload":"Reload",kind==1?1500u:900u,kind==1?4u:8u},Case{"equip","Scene_1",533,kind==1?5u:9u},Case{"unequip","Scene_1",333,kind==1?6u:10u}}){
                            auto m=ranged;m.str(texts+0x200,(std::string("midnight/gameplay/player/base/guns/")+(kind==1?"gun_":"crossbow_")+c.clip+".midnightscene").c_str());m.str(texts+0x300,c.sub);m.d(scene+0x28,c.duration);
                            require(inspectRangedScene(m)&&result.variant==c.variant,"observed native ranged clip phase and exact duration");
                            for(unsigned id:{2u,3u}){
                                auto flying=m;flying.d(active,id);flying.q(mode+0x10+id*8,ground);flying.d(ground+8,id);flying.q(ground,base+(id==2?0x1c85028:0x1c845c8));
                                const bool supported=c.variant==3||c.variant==4||c.variant==7||c.variant==8;
                                require(inspectRangedScene(flying)==supported,"airborne ranged scene scope admits observed firing and refuses unobserved equipment phases");
                                if(c.variant==4||c.variant==8){
                                    const auto reload=inv+0x4d000;flying.d(machine+0xb0,2);flying.d(active+4,59);flying.q(mode+0x10+59*8,reload);
                                    flying.q(reload,base+0x1c7d608);flying.d(reload+8,59);flying.q(reload+0x10,machine);
                                    const float apex[2]={67.f,68.5f};flying.put(ground+0xa0,apex,sizeof(apex));flying.d(ground+0x94,0);
                                    require(inspectRangedScene(flying)&&result.variant==c.variant,"owned native reload retains its phase while jumping or falling");
                                    const auto rd=[&](auto p,auto d,auto n){return flying.read(p,d,n);};NativePlayerStates st{};unsigned stage=0,remaining=0;float start=0,target=0;
                                    require(readNativePlayerStates(rd,base,st)&&nativePlayerAir(rd,st,base,stage,start,target,remaining,false,true)&&stage==(id==2?1u:2u),"owned Reload59 can supply bounded native jump metadata");
                                    require(!nativePlayerAir(rd,st,base,stage,start,target,remaining),"reload metadata still requires independently owned ranged scene");
                                }
                            }
                            auto low=m;low.d(physics+0x50,1);low.d(physics+0x54,1);low.d(active,6);low.q(mode+0x10+6*8,ground);low.q(ground,base+0x1c84258);low.d(ground+8,6);
                            require(inspectRangedScene(low)&&result.variant==c.variant,"crouched ranged phases retain owned native scene clock and bindings");
                            low.d(physics+0x50,0);require(!inspectRangedScene(low),"inconsistent crouched physics and native locomotion refused");
                        }
                        for(const auto state: {std::pair<unsigned,std::uintptr_t>{56,0x1c849d8},{59,0x1c7d608},{81,0x1c7df88}}){
                            const auto overlay=inv+0x4d000;auto m=ranged;m.d(machine+0xb0,2);m.d(active+4,state.first);m.q(mode+0x10+state.first*8,overlay);m.q(overlay,base+state.second);m.d(overlay+8,state.first);m.q(overlay+0x10,machine);
                            require(inspectRangedScene(m),"owned ranged scene retains native fire/reload/undraw overlay");
                            m.d(physics+0x50,1);m.d(physics+0x54,1);m.d(active,6);m.q(mode+0x10+6*8,ground);m.q(ground,base+0x1c84258);m.d(ground+8,6);
                            require(inspectRangedScene(m),"crouched ranged overlay retains weapon ownership guards");
                        }
                        {
                            auto overlap=ranged;overlap.d(machine+0xb0,4);
                            unsigned index=1;
                            for(const auto state:{std::pair<unsigned,std::uintptr_t>{65,0x1c78a68},{64,0x1c79cf8},{53,0x1c79cf8}}){
                                const auto overlay=inv+0x4d000+index*0x100;
                                overlap.d(active+index*4,state.first);overlap.q(mode+0x10+state.first*8,overlay);
                                overlap.q(overlay,base+state.second);overlap.d(overlay+8,state.first);overlap.q(overlay+0x10,machine);++index;
                            }
                            require(inspectRangedScene(overlap),"exact native Draw 65 and both equipment overlays retain owned ranged scene");
                            overlap.d(physics+0x50,1);overlap.d(physics+0x54,1);overlap.d(active,6);
                            overlap.q(mode+0x10+6*8,ground);overlap.q(ground,base+0x1c84258);overlap.d(ground+8,6);
                            require(inspectRangedScene(overlap),"observed four-state crouched draw retains ranged scene ownership");
                            auto bad=overlap;bad.q(inv+0x4d100,base+0x1c79cf8);require(!inspectRangedScene(bad),"Draw ID with equipment class refused");
                            bad=overlap;bad.q(inv+0x4d100+0x10,actor);require(!inspectRangedScene(bad),"foreign Draw owner refused");
                            bad=overlap;bad.d(active+12,64);require(!inspectRangedScene(bad),"duplicate equipment state refused during draw");
                            bad=overlap;bad.d(machine+0xb0,3);require(!inspectRangedScene(bad),"unobserved partial Draw 65 combination refused");
                            bad=overlap;bad.d(physics+0x50,0);require(!inspectRangedScene(bad),"four-state overlap does not override physical stance");
                            bad=overlap;bad.q(bindings+8,arm);require(!inspectRangedScene(bad),"four-state overlap still requires owned left arm");
                            bad=overlap;bad.d(entity+0xa8,handle+1);require(!inspectRangedScene(bad),"four-state overlap still rejects recycled weapon");
                            unsigned overlapReads=0;const auto rd=[&](auto p,auto d,auto n){return overlap.read(p,d,n);};NativePlayerStates st{};
                            require(readNativePlayerStates(rd,base,st),"four-state concurrency source valid");
                            require(!readNativePlayerRangedScene([&](auto p,auto d,auto n){const bool ok=overlap.read(p,d,n);if(p==bindings+8&&++overlapReads==1)overlap.d(active+4,64);return ok;},base,st,result),"concurrent four-state change refused");
                        }
                        auto wrong=ranged;wrong.q(bindings+8,arm);require(!inspectRangedScene(wrong),"ranged scene refuses right arm");
                        wrong=ranged;wrong.q(bindings+0x28,actor);require(!inspectRangedScene(wrong),"player body cannot substitute for ranged weapon");
                        wrong=ranged;wrong.d(entity+0xa8,handle+1);require(!inspectRangedScene(wrong),"ranged scene refuses recycled weapon generation");
                        wrong=ranged;wrong.d(scene+0x28,501);require(!inspectRangedScene(wrong),"unknown ranged scene duration refused");
                        wrong=ranged;wrong.q(inv+0x70,0);require(!inspectRangedScene(wrong),"unselected ranged item cannot source trigger action");
                        wrong=ranged;wrong.d(instance+0x27c,5);require(!inspectRangedScene(wrong),"completed ranged scene cannot source live action");
                        wrong=ranged;wrong.d(instance+0x18,1);require(!inspectRangedScene(wrong),"missing ranged scene weapon binding refused");
                        wrong=ranged;wrong.d(leftArm+0xac,2);require(!inspectRangedScene(wrong),"foreign left arm refused for ranged action");
                        NativePlayerStates st{};const auto rd=[&](auto p,auto d,auto n){return ranged.read(p,d,n);};require(readNativePlayerStates(rd,base,st),"ranged concurrency source valid");unsigned bindingReads=0;
                        require(!readNativePlayerRangedScene([&](auto p,auto d,auto n){const bool ok=ranged.read(p,d,n);if(p==bindings+8&&++bindingReads==1)ranged.q(p,actor);return ok;},base,st,result),"concurrent native ranged rebinding refused");
                    }
                    std::cout<<"PASS: native ranged action timing, both role/kind identities, owned weapon/left arm, lifecycle and concurrent scene guards\n";
                    {
                        auto wind=cast;wind.str(strings,"models/powers/windblast/windblast.inventoryitem");wind.str(strings+0x100,"components/characters/player/base/powers/windblast/windblast.cpntpowerwindblast");wind.q(pc,base+0x1b53488);wind.q(ped,base+0x1b53348);
                        wind.str(texts+0x200,"midnight/gameplay/player/base/powers/power_windblast_cast.midnightscene");wind.str(texts+0x300,"Cast_Ou_Shockwave");wind.d(scene+0x28,1000);wind.d(instance+0x18,2);wind.q(bindings+0x28,actor);
                        require(inspectCast(wind)&&observedCast.variant==11&&observedCast.durationMs==1000,"observed Windblast exact scene, native player and owned left arm");
                        auto lowWind=wind;lowWind.d(physics+0x50,1);lowWind.d(physics+0x54,1);lowWind.d(active,6);lowWind.q(mode+0x10+6*8,ground);lowWind.q(ground,base+0x1c84258);lowWind.d(ground+8,6);
                        require(inspectCast(lowWind)&&observedCast.variant==11,"observed crouched Windblast retains owned scene and native Sneaking stance");
                        auto emptyWind=wind;emptyWind.q(inv+0x68,0);emptyWind.d(cpnt+0x1c,0x20000002);emptyWind.q(frame+0x3c8,actor);emptyWind.q(frame+0x3c0,names+0x200);emptyWind.d(frame+0x3d0,2);
                        require(inspectCast(emptyWind)&&!observedCast.power.player.sword.equipped,"observed Windblast holstered sword keeps native left-arm/player scene bindings");
                        for(unsigned id:{2u,3u}){
                            auto flying=wind;flying.d(active,id);flying.q(mode+0x10+id*8,ground);flying.d(ground+8,id);flying.q(ground,base+(id==2?0x1c85028:0x1c845c8));
                            const float apex[2]={67.f,68.5f};flying.put(ground+0xa0,apex,sizeof(apex));flying.d(ground+0x94,0);
                            require(inspectCast(flying)&&observedCast.variant==11&&observedCast.handle==219&&observedCast.timeMs==358,"native airborne Windblast retains exact scene event and time");
                            const auto use=inv+0x4d000;flying.d(machine+0xb0,2);flying.d(active+4,63);flying.q(mode+0x10+63*8,use);flying.q(use,base+0x1c80ed8);flying.d(use+8,63);flying.q(use+0x10,machine);
                            require(inspectCast(flying),"observed Jump/Fall plus UsePower overlay retains the same owned Windblast scene");
                            const auto rd=[&](auto p,auto d,auto n){return flying.read(p,d,n);};NativePlayerStates ps{};unsigned stage=0,remaining=0;float start=0,target=0;
                            require(readNativePlayerStates(rd,base,ps)&&nativePlayerAir(rd,ps,base,stage,start,target,remaining,true)&&stage==(id==2?1u:2u),"validated power overlay reads native air metadata");
                            require(!nativePlayerAir(rd,ps,base,stage,start,target,remaining),"power overlay requires an independently confirmed owned scene");
                            flying.d(physics+0x50,1);require(!inspectCast(flying),"unobserved crouched airborne Windblast refused");
                        }
                        auto badWind=wind;badWind.d(instance+0x18,1);require(!inspectCast(badWind),"Windblast cannot omit native player binding");
                        badWind=wind;badWind.q(bindings+0x28,entity);require(!inspectCast(badWind),"Windblast equipment cannot impersonate player camera binding");
                        badWind=wind;badWind.str(texts+0x300,"Scene_1");require(!inspectCast(badWind),"unobserved Windblast subscene refused");
                        badWind=wind;badWind.str(strings,"models/powers/darkvision/darkvision.inventoryitem");require(!inspectCast(badWind),"another selected power cannot source Windblast cast");
                    }
                    auto crouchedCast=cast;crouchedCast.d(physics+0x50,1);crouchedCast.d(physics+0x54,1);crouchedCast.d(active,6);crouchedCast.q(mode+0x10+6*8,ground);crouchedCast.q(ground,base+0x1c84258);crouchedCast.d(ground+8,6);
                    require(inspectCast(crouchedCast),"observed native Sneaking state retains the same Dark Vision cast");
                    auto emptyCast=cast;emptyCast.q(inv+0x68,0);emptyCast.d(cpnt+0x1c,0x20000002);emptyCast.q(frame+0x3c8,actor);emptyCast.q(frame+0x3c0,names+0x200);emptyCast.d(frame+0x3d0,2);
                    require(inspectCast(emptyCast)&&!observedCast.power.player.sword.equipped,"native Dark Vision does not require drawing the holstered sword");
                    auto offCast=cast;offCast.d(manager+0x1c,0x20000002);require(inspectCast(offCast),"native toggle-off retains cast while vision manager has stopped");
                    auto female=cast;female.str(names+0x400,"models/characters/player/emily/body.def");female.str(names+0x600,"models/characters/player/emily/right_arm.def");female.str(texts+0x100,"models/characters/player/emily/left_arm.def");require(inspectCast(female)&&observedCast.power.player.character==2,"Emily uses the same native Dark Vision scene with her own arm");
                    auto wrong=cast;wrong.q(bindings+8,arm);require(!inspectCast(wrong),"Dark Vision cannot bind the sword arm");
                    wrong=cast;wrong.d(leftArm+0xac,2);require(!inspectCast(wrong),"foreign cast arm refused");
                    wrong=cast;wrong.q(game+0x3130b0+48,arm);require(!inspectCast(wrong),"cast arm whose entity slot resolves elsewhere is refused");
                    wrong=cast;wrong.d(instance+0x18,2);require(!inspectCast(wrong),"unknown Dark Vision binding count refused");
                    auto jumpCast=cast;jumpCast.d(ground+8,2);jumpCast.q(ground,base+0x1c85028);jumpCast.d(active,2);jumpCast.q(mode+0x10+2*8,ground);
                    require(inspectCast(jumpCast)&&observedCast.handle==219&&observedCast.timeMs==358,"observed native jump preserves Dark Vision event and clock");
                    auto fallCast=cast;fallCast.d(ground+8,3);fallCast.q(ground,base+0x1c845c8);fallCast.d(active,3);fallCast.q(mode+0x10+3*8,ground);
                    require(inspectCast(fallCast),"observed native fall retains reciprocal Dark Vision cast");
                    fallCast.str(names+0x400,"models/characters/player/emily/body.def");fallCast.str(names+0x600,"models/characters/player/emily/right_arm.def");fallCast.str(texts+0x100,"models/characters/player/emily/left_arm.def");
                    require(inspectCast(fallCast)&&observedCast.power.player.character==2,"Emily airborne cast retains her own native arm");
                    wrong=jumpCast;wrong.d(physics+0x50,1);require(!inspectCast(wrong),"airborne Dark Vision rejects unobserved crouched physics");
                    wrong=jumpCast;wrong.q(ground,base+0x1c845c8);require(!inspectCast(wrong),"airborne Dark Vision rejects a mismatched native state class");
                    wrong=jumpCast;wrong.q(bindings+8,arm);require(!inspectCast(wrong),"airborne power still requires the owned left arm");
                    wrong=cast;wrong.d(scene+0x28,934);require(!inspectCast(wrong),"unknown native cast duration refused");
                    wrong=cast;wrong.q(inv+0x70,0);require(!inspectCast(wrong),"unselected power cannot impersonate an active scene");
                    wrong=cast;wrong.str(texts+0x200,"midnight/gameplay/player/base/powers/power_blink_cast_in.midnightscene");wrong.d(scene+0x28,333);require(!inspectCast(wrong),"Dark Vision equipment cannot source Blink");
                    wrong=cast;unsigned checks=0;require(!readNativeDarkVisionItem([&](auto p,auto d,auto n){const bool ok=wrong.read(p,d,n);if(p==manager+0x1c&&++checks==1)wrong.d(p,0x20000002);return ok;},base,dv,true),"selected cast read rejects a concurrently changing vision manager");
                    std::cout<<"PASS: native Dark Vision phase, selected equipment, both reciprocal arms, toggle-off and foreign scene guards\n";
                }
                b=dark;b.q(manager+0x10,entity);require(!readDark(b),"foreign Dark Vision manager owner refused");
                b=dark;b.q(list+24,pc);require(!readDark(b),"unregistered Dark Vision manager refused");
                b=dark;b.d(entity+0xa8,handle+1);require(!readDark(b),"recycled Dark Vision equipment handle refused");
                b=missing;b.d(pe+0x20,1);require(!readDark(b),"quantity cannot substitute for the missing native equipment");
                b=dark;unsigned managerReads=0;require(!readNativeDarkVisionItem([&](auto p,auto d,auto n){const bool ok=b.read(p,d,n);if(p==manager+0x1c&&++managerReads==1)b.d(p,0x2000000a);return ok;},base,dv),"concurrent Dark Vision activation refused");
                std::cout<<"PASS: Dark Vision missing/acquired equipment, manager ownership, native activation, both roles and concurrent changes\n";
            }
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
        {
            auto carry=cm;constexpr auto corpse=player+0x8000,left=player+0x9000,leftDecl=player+0xa000;
            carry.d(component+0x50,0);carry.d(active,0);carry.q(mode+0x10,state);carry.q(state,base+0x1c85628);carry.d(state+8,0);
            carry.d(active+4,45);carry.q(mode+0x10+45*8,slash);carry.q(slash,base+0x1c77748);carry.d(slash+8,45);
            carry.d(slash+0xf8,0x20003);carry.d(slash+0x110,3);
            carry.q(game+0x3130c0,corpse);carry.q(corpse,base+0x1c5e108);carry.d(corpse+0xa8,0x20003);
            carry.d(instance+0x18,3);carry.q(bindings+0x48,corpse);carry.d(scene+0x28,3000);
            carry.str(names+0x300,"midnight/gameplay/player/base/carrycorpse/carry_corpse_loop.midnightscene");
            NativePlayerCarryScene observed{};
            const auto inspect=[&](Memory& memory){const auto read=[&](auto p,auto d,auto n){return memory.read(p,d,n);};NativePlayerStates ns{};return readNativePlayerStates(read,base,ns)&&readNativePlayerCarryScene(read,base,ns,observed);};
            require(inspect(carry)&&observed.variant==2&&observed.corpseHandle==0x20003&&observed.character==1,"owned Corvo carry loop binds exact corpse and native right arm");
            {
                auto prep=carry;constexpr auto equip=state+0xb000;
                prep.d(slash+0x24,0);prep.d(slash+0x110,1);prep.d(machine+0xb0,3);prep.d(active+8,53);
                prep.q(mode+0x10+53*8,equip);prep.q(equip,base+0x1c79cf8);prep.d(equip+8,53);prep.q(equip+0x10,machine);
                NativePlayerCarryPreparation preparation{};
                const auto preparing=[&](Memory& memory){auto read=[&](auto p,auto d,auto n){return memory.read(p,d,n);};NativePlayerStates ns{};
                    return readNativePlayerStates(read,base,ns)&&readNativePlayerCarryPreparation(read,base,ns,preparation);};
                require(preparing(prep)&&preparation.corpse==corpse&&preparation.corpseHandle==0x20003,"pre-scene pickup keeps exact grounded state and corpse identity");
                require(!inspect(prep),"pickup preparation does not impersonate an active carry animation");
                for(auto location:{slash+0x24,slash+0x110,slash+0xf8,corpse+0xa8,equip+8}){
                    auto bad=prep;bad.d(location,99);require(!preparing(bad),"unknown preparation phase, scene, target or overlay refused");
                }
                auto bad=prep;bad.d(active+8,45);require(!preparing(bad),"duplicate preparation states refused");
                bad=prep;bad.d(component+0x50,1);require(!preparing(bad),"standing state cannot impersonate crouched preparation");
                auto low=prep;low.d(component+0x50,1);low.d(active,6);low.q(mode+0x10+6*8,state);low.d(state+8,6);low.q(state,base+0x1c84258);
                require(preparing(low)&&preparation.corpseHandle==0x20003,"observed Sneaking6 Carry45 Equip53 preparation preserves exact corpse identity");
                bad=low;bad.d(component+0x50,0);require(!preparing(bad),"crouched state cannot impersonate standing preparation");
                bad=low;bad.q(state,base+0x1c85628);require(!preparing(bad),"crouched preparation requires native sneaking vtable");
                bad=low;bad.d(slash+0x24,42);require(!preparing(bad),"crouched preparation refuses active native scene");
                bad=low;bad.d(corpse+0xa8,0x30003);require(!preparing(bad),"crouched preparation refuses recycled target");
                {
                    NativePlayerStates ns{};require(readNativePlayerStates([&](auto p,auto d,auto n){return low.read(p,d,n);},base,ns),"valid crouched preparation fixture");
                    bad=low;unsigned reads=0;
                    auto read=[&](auto p,auto d,auto n){const bool ok=bad.read(p,d,n);if(p==component+0x50&&++reads==1)bad.d(p,0);return ok;};
                    require(!readNativePlayerCarryPreparation(read,base,ns,preparation),"concurrent crouch-to-stand transition refused");
                }
                bad=prep;bad.d(stack,15);require(!preparing(bad),"preparation cannot cross native load boundary");
                for(auto location:{slash+0x24,slash+0x110,slash+0xf8,game+0x3130c0,mode+0x10+53*8}){
                    bad=prep;NativePlayerStates ns{};require(readNativePlayerStates([&](auto p,auto d,auto n){return prep.read(p,d,n);},base,ns),"valid preparation fixture");
                    unsigned count=0;auto read=[&](auto p,auto d,auto n){const bool ok=bad.read(p,d,n);if(p==location&&++count==1)bad.q(p,0xdead);return ok;};
                    require(!readNativePlayerCarryPreparation(read,base,ns,preparation),"concurrent preparation or corpse replacement refused");
                }
                std::cout<<"PASS: exact native pickup preparation, no invented scene, transition and concurrent replacement refusal\n";
            }
            auto invalid=carry;invalid.q(bindings+0x48,player);require(!inspect(invalid),"player cannot impersonate carried NPC");
            invalid=carry;invalid.d(corpse+0xa8,0x30003);require(!inspect(invalid),"recycled corpse generation refused");
            invalid=carry;invalid.d(slash+0xf8,0x10002);require(!inspect(invalid),"native first-person arm cannot be carried corpse");
            invalid=carry;invalid.str(names+0x400,"Scene_1");require(!inspect(invalid),"wrong character carry subscene refused");
            invalid=carry;invalid.d(slash+0x110,2);require(!inspect(invalid),"pickup phase cannot impersonate carry loop");
            invalid=carry;invalid.d(instance+0x27c,4);require(!inspect(invalid),"retired native carry scene refused");
            invalid=carry;invalid.d(scene+0x28,1000);require(!inspect(invalid),"carry loop duration must match observed asset");
            invalid=carry;invalid.d(machine+0xb0,3);invalid.d(active+8,45);require(!inspect(invalid),"duplicate carry activation refused");
            invalid=carry;invalid.d(stack,15);require(!inspect(invalid),"carry cannot cross native loading boundary");
            auto emily=carry;emily.str(names+0x100,"models/characters/player/emily/body.def");emily.str(names+0x200,"models/characters/player/emily/right_arm.def");emily.str(names+0x400,"Scene_1");
            emily.d(component+0x50,1);emily.d(active,6);emily.q(mode+0x10+6*8,state);emily.d(state+8,6);emily.q(state,base+0x1c84258);
            require(inspect(emily)&&observed.character==2&&observed.stance==1,"owned Emily crouched carry uses observed Scene_1");
            auto walk=emily;walk.str(names+0x300,"midnight/gameplay/player/base/carrycorpse/carry_corpse_walk.midnightscene");walk.d(scene+0x28,1600);
            require(inspect(walk)&&observed.variant==3,"native carry walk clock is distinct from idle");
            auto pickup=carry;pickup.d(slash+0x110,2);pickup.d(scene+0x28,1000);pickup.str(names+0x400,"Scene_1");pickup.str(names+0x300,"midnight/gameplay/player/base/carrycorpse/carry_corpse_in.midnightscene");
            pickup.q(game+0x3130c8,left);pickup.q(left,base+0x1c5e428);pickup.d(left+0xa8,0x30004);pickup.d(left+0xac,1);pickup.q(left+0x20,names+0x500);pickup.str(names+0x500,"left_arm_1");
            pickup.q(left+0x2e8,leftDecl);pickup.q(leftDecl,base+0x1ab58f8);pickup.q(leftDecl+8,names+0x600);pickup.str(names+0x600,"models/characters/player/corvo/left_arm.def");
            pickup.d(player+0xb8,2);pickup.q(children+8,left);pickup.d(instance+0x18,4);pickup.q(bindings+0x68,left);
            require(inspect(pickup)&&observed.variant==1,"native pickup owns both arms and corpse");
            invalid=pickup;invalid.q(bindings+0x68,arm);require(!inspect(invalid),"duplicate arm cannot satisfy both pickup hands");
            auto drop=carry;drop.d(active+4,46);drop.q(mode+0x10+46*8,slash);drop.q(slash,base+0x1c790c8);drop.d(slash+8,46);drop.d(slash+0x38,0x20003);drop.d(slash+0x54,0);
            drop.str(names+0x300,"midnight/gameplay/player/base/carrycorpse/carry_corpse_drop_low_stand.midnightscene");drop.d(scene+0x28,1000);drop.str(names+0x400,"Scene_1");
            require(inspect(drop)&&observed.variant==4&&observed.stateId==46,"drop state owns release scene independently of inactive carry state");
            drop.str(names+0x300,"midnight/gameplay/player/base/carrycorpse/carry_corpse_drop_fetal_pose.midnightscene");drop.d(scene+0x28,666);drop.d(slash+0x54,4);
            require(inspect(drop)&&observed.variant==5,"native fetal release scene observed in constrained crouched location");
            drop.str(names+0x300,"midnight/gameplay/player/base/carrycorpse/carry_corpse_drop_low_sneak.midnightscene");drop.d(scene+0x28,1000);drop.d(slash+0x54,1);
            require(inspect(drop)&&observed.variant==6,"native open-space low sneak release uses observed release kind1");
            invalid=drop;invalid.d(slash+0x54,4);require(!inspect(invalid),"release kind must match independently observed scene");
            invalid=drop;invalid.str(names+0x300,"midnight/gameplay/player/base/carrycorpse/carry_corpse_drop_high.midnightscene");require(!inspect(invalid),"unobserved release variants remain refused");
            {
                auto high=drop;high.str(names+0x300,"midnight/gameplay/player/base/carrycorpse/carry_corpse_drop_high.midnightscene");
                high.d(scene+0x28,666);high.d(slash+0x54,2);
                const unsigned char flags[]={1,0,1};high.put(slash+0x50,flags,sizeof(flags));
                const auto research=[&](Memory& memory){auto read=[&](auto p,auto d,auto n){return memory.read(p,d,n);};NativePlayerStates ns{};
                    return readNativePlayerStates(read,base,ns)&&readNativePlayerCarryScene(read,base,ns,observed,true);};
                require(!inspect(high),"production carry path still refuses observation-only throw");
                require(research(high)&&observed.variant==8&&observed.phase==2&&observed.character==1,"Corvo high throw binds exact native drop state, owned arm and corpse");
                auto highEmily=high;highEmily.str(names+0x100,"models/characters/player/emily/body.def");highEmily.str(names+0x200,"models/characters/player/emily/right_arm.def");
                require(research(highEmily)&&observed.character==2,"Emily high throw uses the same observed duration and Scene_1");
                for(auto location:{slash+0x54,slash+0x50,slash+0x24,scene+0x28,instance+0x18,corpse+0xa8,component+0x50}){
                    auto bad=high;bad.d(location,99);require(!research(bad),"throw rejects unknown selector, flags, scene, duration, bindings, incarnation or stance");
                }
                auto bad=high;bad.d(instance+0x160,667);require(!research(bad),"throw refuses expired one-shot clock");
                for(auto location:{slash+0x54,slash+0x50,slash+0x24,bindings+0x48,game+0x3130c0,instance+0x160}){
                    bad=high;NativePlayerStates ns{};require(readNativePlayerStates([&](auto p,auto d,auto n){return high.read(p,d,n);},base,ns),"valid native throw test fixture");
                    unsigned count=0;auto read=[&](auto p,auto d,auto n){const bool ok=bad.read(p,d,n);if(p==location&&++count==1)bad.d(p,0);return ok;};
                    require(!readNativePlayerCarryScene(read,base,ns,observed,true),"throw brackets selector, intent flags, clock and owned scene/body against replacement");
                }
                std::cout<<"PASS: observation-only native high throw, both characters, explicit opt-in and mid-read replacement refusal\n";
            }
            for(auto location:{slash+0xf8,instance+8,bindings+0x48,game+0x3130c0}){
                invalid=carry;auto read=[&](auto p,auto d,auto n){const bool ok=invalid.read(p,d,n);if(p==location)invalid.q(p,0);return ok;};NativePlayerStates ns{};
                require(readNativePlayerStates([&](auto p,auto d,auto n){return carry.read(p,d,n);},base,ns)&&!readNativePlayerCarryScene(read,base,ns,observed),"concurrent corpse, scene or binding replacement refused");
            }
            std::cout<<"PASS: carry/drop native ownership, both characters/stances, pickup arms, release variants and replacement refusal\n";
        }
    }
    {
        auto lm=actions;constexpr auto instance=0xa00000000ull,scenes=instance+0x1000,decl=instance+0x2000,scene=instance+0x3000,bindings=instance+0x4000,names=instance+0x5000,body=instance+0x6000;
        lm.d(active,4);lm.q(mode+0x10+4*8,state);lm.q(state,base+0x1c82158);lm.d(state+8,4);lm.d(state+0x24,42);
        lm.q(game+0x1f5338,scenes);lm.d(game+0x1f5340,1);lm.q(game+0x1f5338+0x610,0);lm.d(game+0x1f5340+0x610,0);lm.q(scenes,instance);
        lm.d(instance+0x158,42);lm.d(instance+0x27c,3);lm.d(instance+0x160,200);lm.q(instance,decl);lm.q(instance+8,scene);
        lm.q(decl,base+0x1affbc8);lm.q(decl+8,names);lm.str(names,"midnight/gameplay/player/base/mobility/cat_fall_light.midnightscene");
        lm.q(scene,base+0x1b00198);lm.d(scene+0x28,1000);lm.q(scene+0x30,names+0x100);lm.str(names+0x100,"Scene_2");
        lm.q(instance+0x10,bindings);lm.d(instance+0x18,1);lm.q(bindings+8,player);
        lm.q(player+0x2e8,body);lm.q(body,base+0x1ab58f8);lm.q(body+8,names+0x200);lm.str(names+0x200,"models/characters/player/corvo/body.def");
        constexpr auto landingInput=instance+0x7000;lm.q(machine+0x2f8,landingInput);lm.d(landingInput+0x6c,16);lm.q(landingInput+0xa0,0);
        NativePlayerLandingScene result{};
        const auto inspect=[&](Memory& mem){const auto rd=[&](auto p,auto d,auto n){return mem.read(p,d,n);};NativePlayerStates ns{};return readNativePlayerStates(rd,base,ns)&&readNativePlayerLandingScene(rd,base,ns,result);};
        require(inspect(lm)&&result.character==1&&result.durationMs==1000&&result.timeMs==200,"Corvo native body landing clock");
        auto em=lm;em.str(names+0x100,"Scene_1");em.str(names+0x200,"models/characters/player/emily/body.def");require(inspect(em)&&result.character==2,"Emily native body landing clock");
        em.q(landingInput+0xa0,1ull<<16);require(inspect(em)&&result.endStance==1,"native landing crouch latch selects recovery without first-person anchors");
        auto bad=lm;bad.q(bindings+8,player+8);require(!inspect(bad),"landing refuses foreign body");
        bad=lm;bad.d(instance+0x18,2);require(!inspect(bad),"landing refuses arms or extra bindings");
        bad=lm;bad.d(instance+0x158,43);require(!inspect(bad),"landing scene generation required");
        bad=lm;bad.d(instance+0x160,1001);require(!inspect(bad),"expired landing scene refused");
        bad=lm;bad.d(scene+0x28,999);require(!inspect(bad),"unobserved landing duration refused");
        bad=lm;bad.str(names,"midnight/gameplay/player/base/mobility/cat_fall_strong.midnightscene");require(!inspect(bad),"damaging landing requires separate evidence");
        bad=lm;bad.str(names+0x100,"Scene_1");require(!inspect(bad),"other character landing subscene refused");
        bad=lm;bad.d(landingInput+0x6c,64);require(!inspect(bad),"unobserved native landing input mapping refused");
        for(auto location:{state+0x24,instance+8,scene,bindings+8,player+0x2e8,body,body+8,machine+0x2f8,landingInput+0x6c}){
            bad=lm;unsigned count=0;auto rd=[&](auto p,auto d,auto n){const bool ok=bad.read(p,d,n);if(p==location&&++count==1)bad.q(p,0);return ok;};NativePlayerStates ns{};
            require(readNativePlayerStates([&](auto p,auto d,auto n){return lm.read(p,d,n);},base,ns)&&!readNativePlayerLandingScene(rd,base,ns,result),"concurrent landing ownership change refused");
        }
        bad=lm;unsigned bitReads=0;NativePlayerStates beforeLatch{};require(readNativePlayerStates([&](auto p,auto d,auto n){return lm.read(p,d,n);},base,beforeLatch),"landing input ownership setup");
        require(!readNativePlayerLandingScene([&](auto p,auto d,auto n){const bool ok=bad.read(p,d,n);if(p==landingInput+0xa0&&++bitReads==1)bad.q(p,1ull<<16);return ok;},base,beforeLatch,result),"concurrent crouch decision refused");
        std::cout<<"PASS: native hard-landing body scene, character, generation and ownership\n";
    }
    {
        auto km=actions;constexpr auto instance=0xa00000000ull,scenes=instance+0x1000,decl=instance+0x2000,scene=instance+0x3000,bindings=instance+0x4000,names=instance+0x5000;
        constexpr auto right=instance+0x6000,left=instance+0x7000,door=instance+0x8000,child=instance+0x9000,children=instance+0xa000,body=instance+0xb000,rd=instance+0xc000,ld=instance+0xd000;
        km.d(active,24);km.q(mode+0x10+24*8,state);km.q(state,base+0x1c7c4a8);km.d(state+8,24);km.d(state+0x24,42);km.d(state+0x1f0,0x30004);km.d(state+0x220,0);
        km.q(game+0x1f5338,scenes);km.d(game+0x1f5340,1);km.q(game+0x1f5338+0x610,0);km.d(game+0x1f5340+0x610,0);km.q(scenes,instance);
        km.d(instance+0x158,42);km.d(instance+0x27c,3);km.d(instance+0x15c,200);km.d(instance+0x160,200);km.q(instance,decl);km.q(instance+8,scene);
        km.q(decl,base+0x1affbc8);km.q(decl+8,names);km.str(names,"midnight/gameplay/player/base/interaction/keyhole_left.midnightscene");
        km.q(scene,base+0x1b00198);km.d(scene+0x28,1500);km.q(scene+0x30,names+0x100);km.str(names+0x100,"Keyhole_left_in");
        km.q(instance+0x10,bindings);km.d(instance+0x18,4);km.q(bindings+8,player);km.q(bindings+0x28,right);km.q(bindings+0x48,left);km.q(bindings+0x68,child);
        km.q(game+0x3130b8,right);km.q(game+0x3130c0,left);km.q(game+0x3130c8,door);km.q(game+0x3130d0,child);
        km.d(door+0xa8,0x30004);km.q(door,base+0x1cb7c68);km.d(child+0xa8,0x40005);km.d(child+0xac,0x30004);
        km.q(player+0xb0,children);km.d(player+0xb8,2);km.q(children,right);km.q(children+8,left);km.q(player+0x2e8,body);km.q(body,base+0x1ab58f8);km.q(body+8,names+0x200);
        km.str(names+0x200,"models/characters/player/corvo/body.def");
        for(unsigned i=0;i<2;++i){const auto arm=i?left:right,ad=i?ld:rd;km.q(arm,base+0x1c5e428);km.d(arm+0xa8,i?0x20003:0x10002);km.d(arm+0xac,1);km.q(arm+0x20,names+0x300+i*0x100);km.str(names+0x300+i*0x100,i?"left_arm_1":"right_arm_2");km.q(arm+0x2e8,ad);km.q(ad,base+0x1ab58f8);km.q(ad+8,names+0x500+i*0x100);km.str(names+0x500+i*0x100,i?"models/characters/player/corvo/left_arm.def":"models/characters/player/corvo/right_arm.def");}
        NativePlayerKeyholeScene result{};const auto inspect=[&](Memory& mem){auto rd=[&](auto p,auto d,auto n){return mem.read(p,d,n);};NativePlayerStates ns{};return readNativePlayerStates(rd,base,ns)&&readNativePlayerKeyholeScene(rd,base,ns,result);};
        require(inspect(km)&&result.variant==7&&result.timeMs==200&&result.character==1,"keyhole binds player, both native arms and generation-owned door child");
        for(unsigned role=0;role<2;++role)for(unsigned side=0;side<2;++side)for(unsigned crouch=0;crouch<2;++crouch)for(unsigned phase=0;phase<3;++phase){
            auto m=km;const std::string direction=side?"right":"left",character=role?"emily":"corvo";
            m.str(names,("midnight/gameplay/player/base/interaction/keyhole_"+direction+".midnightscene").c_str());
            m.str(names+0x100,((crouch&&phase!=1?"Sneak_":"")+std::string("Keyhole_")+direction+(phase==0?"_in":phase==1?"_loop":"_out")).c_str());
            m.str(names+0x200,("models/characters/player/"+character+"/body.def").c_str());m.str(names+0x500,("models/characters/player/"+character+"/right_arm.def").c_str());m.str(names+0x600,("models/characters/player/"+character+"/left_arm.def").c_str());
            m.d(state+0x220,(crouch<<8)|(side<<24));m.d(scene+0x28,phase==0?1500:phase==1?500:1000);m.d(instance+0x15c,phase==0?200:phase==1?2200:4200);m.d(instance+0x160,200);
            require(inspect(m)&&result.character==role+1&&result.endStance==crouch&&result.timeMs==(phase==1?700u:200u),"both roles, mirrored and crouched phase clocks");
        }
        auto bad=km;bad.d(child+0xac,0x30005);require(!inspect(bad),"unrelated door child refused");
        bad=km;bad.d(right+0xac,2);require(!inspect(bad),"foreign first-person arm refused");
        bad=km;bad.q(bindings+0x48,right);require(!inspect(bad),"duplicated right arm cannot replace left arm");
        bad=km;bad.d(instance+0x160,1501);require(!inspect(bad),"expired local phase refused");
        bad=km;bad.d(instance+0x15c,3600001);require(!inspect(bad),"unbounded hold refused");
        bad=km;bad.d(instance+0x158,43);require(!inspect(bad),"stale scene generation refused");
        bad=km;bad.d(state+0x220,1u<<24);require(!inspect(bad),"wrong door side scene refused");
        for(auto location:{state+0x24,state+0x1f0,instance+8,bindings+0x68,child+0xac,door,children}){bad=km;unsigned reads=0;NativePlayerStates ns{};require(readNativePlayerStates([&](auto p,auto d,auto n){return km.read(p,d,n);},base,ns),"keyhole replacement setup");
            require(!readNativePlayerKeyholeScene([&](auto p,auto d,auto n){const bool ok=bad.read(p,d,n);if(p==location&&++reads==1)bad.q(p,0);return ok;},base,ns,result),"concurrent keyhole ownership replacement refused");}
        auto exit=km;exit.str(names+0x100,"Keyhole_left_out");exit.d(scene+0x28,1000);exit.d(instance+0x15c,3900);exit.d(instance+0x160,900);exit.d(state+0xec,3);
        require(inspect(exit),"native exit establishes handoff provenance");
        NativePlayerStates exitOwner{};require(readNativePlayerStates([&](auto p,auto d,auto n){return exit.read(p,d,n);},base,exitOwner),"keyhole exit owner");
        NativeKeyholeHandoff handoff;handoff.observe(exitOwner,result,10000);
        auto retired=exit;retired.d(game+0x1f5340,0);
        const auto continueExit=[&](Memory& mem,std::uint64_t now){const auto rd=[&](auto p,auto d,auto n){return mem.read(p,d,n);};NativePlayerStates current{};return readNativePlayerStates(rd,base,current)&&readNativeKeyholeHandoff(rd,base,current,handoff,now,result);};
        require(continueExit(retired,10200)&&result.timeMs==1000&&result.variant==9,"retired native scene bridges only its confirmed exit endpoint");
        require(!continueExit(retired,10050),"handoff cannot finish the native scene early");
        require(!continueExit(retired,10501),"handoff has a nonrenewing deadline");
        require(!continueExit(exit,10200),"present scene cannot be replaced with endpoint inference");
        bad=retired;bad.d(state+0xec,2);require(!continueExit(bad,10200),"native hold never qualifies as exit handoff");
        bad=retired;bad.d(state+0x24,43);require(!continueExit(bad,10200),"different scene incarnation cannot use old exit");
        bad=retired;bad.d(door+0xa8,0x40004);require(!continueExit(bad,10200),"destroyed door cannot use exit continuation");
        bad=retired;bad.d(right+0xac,2);require(!continueExit(bad,10200),"exit continuation requires the same owned arms");
        bad=retired;bad.d(state+0x220,1u<<8);require(!continueExit(bad,10200),"exit stance cannot change during handoff");
        bad=retired;bad.d(state+0xec,0);require(continueExit(bad,10200),"native exit listener reset before state retirement is supported");
        handoff={};require(!continueExit(retired,10200),"a fresh or retired stream cannot invent keyhole provenance");
        require(inspect(km),"entry cancellation provenance");handoff.observe(exitOwner,result,20000);
        auto cancelled=km;cancelled.d(state+0x24,0);cancelled.d(state+0xec,3);cancelled.d(game+0x1f5340,0);
        const auto continueCancel=[&](Memory& mem,std::uint64_t now){const auto rd=[&](auto p,auto d,auto n){return mem.read(p,d,n);};NativePlayerStates current{};return readNativePlayerStates(rd,base,current)&&readNativeKeyholeHandoff(rd,base,current,handoff,now,result,true);};
        require(continueCancel(cancelled,20200)&&result.variant==7&&result.timeMs==200,"early native cancellation preserves last confirmed entry pose without inventing an exit");
        require(!continueCancel(cancelled,20401),"early cancellation cannot renew provenance");
        bad=cancelled;bad.d(state+0x24,43);require(!continueCancel(bad,20200),"replacement scene is not cancellation");
        bad=cancelled;bad.d(state+0xec,2);require(!continueCancel(bad,20200),"hold is not cancellation");
        bad=cancelled;bad.d(game+0x1f5340,1);require(!continueCancel(bad,20200),"cancelled scene must leave both manager lists");
        bad=cancelled;bad.d(left+0xac,2);require(!continueCancel(bad,20200),"cancellation requires original owned arms");
        handoff={};require(!continueCancel(cancelled,20200),"fresh stream cannot manufacture native cancellation");
        std::cout<<"PASS: native keyhole phases, body/arms/door ownership, mirrored crouch, clocks and bounded exit handoff\n";
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
        for(unsigned role=1;role<=2;++role)for(unsigned lowExit=0;lowExit<=1;++lowExit){
            auto high=two;high.d(state+0xd8,2);high.d(state+0xb8,lowExit?6:0);high.d(scene+0x28,833);
            high.str(names,lowExit?"midnight/gameplay/player/base/mobility/mantle_sneak.midnightscene":"midnight/gameplay/player/base/mobility/mantle_idle.midnightscene");
            high.str(names+0x100,role==1?"Scene_2":"Scene_1");
            high.str(armNames,"left_arm_1");high.str(armNames+0x200,"models/characters/player/corvo/left_arm.def");
            if(role==2){high.str(armNames+0x100,"models/characters/player/emily/body.def");high.str(armNames+0x200,"models/characters/player/emily/left_arm.def");}
            require(inspect(high)&&result.variant==4+lowExit&&result.endStance==lowExit&&result.durationMs==833,"observed high mantle validates each character, scene clock and exit stance");
            auto bad=high;bad.d(scene+0x28,500);require(!inspect(bad),"high mantle rejects low-mantle scene clock");
            bad=high;bad.str(names+0x100,role==1?"Scene_1":"Scene_2");require(!inspect(bad),"high mantle rejects other character subscene");
            bad=high;bad.d(state+0xd8,1);require(!inspect(bad),"low mantle cannot borrow high scene");
            bad=high;bad.d(state+0xb8,10);require(!inspect(bad),"unobserved confined crouch mantle exit remains rejected");
            bad=high;bad.d(instance+0x160,834);require(!inspect(bad),"expired high scene refused");
            bad=high;bad.str(armNames,"right_arm_2");bad.str(armNames+0x200,role==1?"models/characters/player/corvo/right_arm.def":"models/characters/player/emily/right_arm.def");require(!inspect(bad),"high mantle's observed single support arm must be left");
        }
        {
            auto three=two;constexpr auto left=player+0x9000,leftDecl=player+0xa000,leftNames=player+0xb000;
            three.q(game+0x3130c0,left);three.q(left,base+0x1c5e428);three.d(left+0xa8,0x20003);three.d(left+0xac,1);three.q(left+0x20,leftNames);three.str(leftNames,"left_arm_1");
            three.q(left+0x2e8,leftDecl);three.q(leftDecl,base+0x1ab58f8);three.q(leftDecl+8,leftNames+0x100);three.str(leftNames+0x100,"models/characters/player/corvo/left_arm.def");
            three.d(player+0xb8,2);three.q(children+8,left);three.d(instance+0x18,3);
            three.q(bindings+8,left);three.q(bindings+0x28,player);three.q(bindings+0x48,arm);
            require(inspect(three),"observed canopy mantle accepts left arm, body and right arm in native order");
            auto reordered=three;reordered.q(bindings+8,arm);reordered.q(bindings+0x48,left);require(inspect(reordered),"three mantle bindings are order independent");
            auto invalid=three;invalid.q(bindings+0x48,left);require(!inspect(invalid),"duplicate left mantle arm refused");
            invalid=three;invalid.q(bindings+8,arm);require(!inspect(invalid),"duplicate right mantle arm refused");
            invalid=three;invalid.d(left+0xac,2);require(!inspect(invalid),"foreign left mantle arm refused");
            invalid=three;invalid.q(children+8,player);require(!inspect(invalid),"left mantle arm requires reciprocal child membership");
            invalid=three;invalid.str(leftNames+0x100,"models/characters/player/emily/left_arm.def");require(!inspect(invalid),"other character left mantle arm refused");
            invalid=three;invalid.d(instance+0x18,4);require(!inspect(invalid),"more than player and two mantle arms refused");
            invalid=three;invalid.d(instance+0x18,2);require(!inspect(invalid),"unobserved body and left-only mantle refused");
            for(auto location:{bindings+0x48,children+8,left+0x2e8}){
                invalid=three;unsigned reads=0;auto reader=[&](auto p,auto d,auto n){const bool ok=invalid.read(p,d,n);if(p==location&&++reads==1)invalid.q(p,0);return ok;};NativePlayerStates ns{};
                require(readNativePlayerStates([&](auto p,auto d,auto n){return three.read(p,d,n);},base,ns)&&!readNativePlayerTraversalScene(reader,base,ns,result),"concurrent three-binding mantle owner replacement refused");
            }
        }
        {
            auto vm=two;vm.d(active,9);vm.q(mode+0x10+9*8,state);vm.q(state,base+0x1c86e18);vm.d(state+8,9);vm.d(state+0x38,0);vm.d(state+0x24,0);
            const float vs[]={0,0,0},vp[]={0,1.5f,0},vg[]={0,2.6f,0},vd[]={0,1,0},location[]={0,1,0},height=1;
            vm.put(state+0x58,vs,12);vm.put(state+0x64,vp,12);vm.put(state+0x70,vg,12);vm.put(state+0x4c,vd,12);vm.put(state+0x7c,&height,4);vm.put(player+0x300,location,12);
            NativePlayerVault vault{};const auto inspectVault=[&](Memory& mem){const auto rd=[&](auto p,auto d,auto n){return mem.read(p,d,n);};NativePlayerStates ns{};return readNativePlayerStates(rd,base,ns)&&readNativePlayerVault(rd,base,ns,vault);};
            require(inspectVault(vm)&&vault.phase==0&&vault.progress==0,"native vault approach is scene-free preparation");
            auto invalid=vm;invalid.d(state+0x38,3);require(!inspectVault(invalid),"unobserved vault phase refused");
            invalid=vm;invalid.d(state+0x24,42);require(!inspectVault(invalid),"vault approach refuses stray scene handle");
            vm.d(state+0x38,1);vm.d(state+0x24,42);vm.str(names,"midnight/gameplay/player/base/mobility/vault_2.midnightscene");vm.str(names+0x100,"Scene_2");vm.d(scene+0x28,933);vm.d(instance+0x18,3);
            constexpr auto left=player+0x9000,leftDecl=player+0xa000,leftNames=player+0xb000;
            vm.q(game+0x3130c0,left);vm.q(left,base+0x1c5e428);vm.d(left+0xa8,0x20003);vm.d(left+0xac,1);vm.q(left+0x20,leftNames);vm.str(leftNames,"left_arm_1");
            vm.q(left+0x2e8,leftDecl);vm.q(leftDecl,base+0x1ab58f8);vm.q(leftDecl+8,leftNames+0x100);vm.str(leftNames+0x100,"models/characters/player/corvo/left_arm.def");
            vm.d(player+0xb8,2);vm.q(children+8,left);vm.q(bindings+0x48,left);
            require(inspectVault(vm)&&vault.sceneVariant==2&&vault.character==1&&std::fabs(vault.progress-1.f/2.6f)<.0001f,"Corvo vault validates body and both owned arms, progress from geometry");
            auto em=vm;em.str(armNames+0x100,"models/characters/player/emily/body.def");em.str(armNames+0x200,"models/characters/player/emily/right_arm.def");em.str(leftNames+0x100,"models/characters/player/emily/left_arm.def");em.str(names+0x100,"Scene_1");em.str(names,"midnight/gameplay/player/base/mobility/vault_1.midnightscene");
            require(inspectVault(em)&&vault.sceneVariant==1&&vault.character==2,"Emily native vault variant uses her body/arms/subscene");
            invalid=vm;invalid.q(bindings+0x48,arm);require(!inspectVault(invalid),"duplicate vault arm refused");
            invalid=vm;invalid.d(left+0xac,2);require(!inspectVault(invalid),"foreign left arm refused");
            invalid=vm;invalid.d(instance+0x18,2);require(!inspectVault(invalid),"partial vault binding refused");
            invalid=vm;invalid.str(names+0x100,"Scene_1");require(!inspectVault(invalid),"foreign vault character subscene refused");
            invalid=vm;invalid.str(names,"midnight/gameplay/player/base/mobility/vault_window.midnightscene");require(!inspectVault(invalid),"unobserved window vault refused");
            invalid=vm;invalid.d(scene+0x28,1000);require(!inspectVault(invalid),"unexpected native vault duration refused");
            invalid=vm;invalid.d(state+0x7c,0x7fc00000);require(!inspectVault(invalid),"nonfinite vault height refused");
            for(auto at:{state+0x38,state+0x64,state+0x70,bindings+0x48,children+8,player+0x2e8}){
                invalid=vm;unsigned reads=0;auto rd=[&](auto p,auto d,auto n){const bool ok=invalid.read(p,d,n);if(p==at&&++reads==1)invalid.q(p,0);return ok;};NativePlayerStates ns{};
                require(readNativePlayerStates([&](auto p,auto d,auto n){return vm.read(p,d,n);},base,ns)&&!readNativePlayerVault(rd,base,ns,vault),"concurrent vault phase, geometry or ownership replacement refused");
            }
            auto exit=vm;exit.d(state+0x38,2);exit.d(state+0x24,0);require(inspectVault(exit)&&vault.progress==1,"native vault exit preserves completed progress without a scene");
        }
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
    for(unsigned stance=0;stance<2;++stance){
        const NativePlayerActiveState move{stance?6u:0u,0,base+(stance?0x1c84258:0x1c85628)};
        for(const auto overlay:{NativePlayerActiveState{56,0,base+0x1c849d8},NativePlayerActiveState{59,0,base+0x1c7d608},NativePlayerActiveState{81,0,base+0x1c7df88}}){
            unknown=actionState;unknown.player.current=stance;unknown.active={move,overlay};
            require(nativePlayerMotion(unknown,base,motion)&&motion==stance,"ranged overlays retain the exact standing or crouched movement channel");
            unknown.active={overlay,move};require(nativePlayerMotion(unknown,base,motion)&&motion==stance,"ranged stance is independent of native active-list ordering");
            unknown.active[0].vtable++;require(!nativePlayerMotion(unknown,base,motion),"unrecognized ranged overlay class refused in either stance");
        }
        unknown=actionState;unknown.player.current=stance;unknown.active={move,{64,0,base+0x1c79cf8},{53,0,base+0x1c79cf8}};
        require(nativePlayerMotion(unknown,base,motion)&&motion==stance,"overlapping native ranged equipment scenes retain locomotion");
        unknown.active={move,{65,0,base+0x1c78a68},{64,0,base+0x1c79cf8},{53,0,base+0x1c79cf8}};
        require(nativePlayerMotion(unknown,base,motion)&&motion==stance,"exact four-state Draw equipment overlap preserves physical locomotion");
        std::reverse(unknown.active.begin(),unknown.active.end());
        require(nativePlayerMotion(unknown,base,motion)&&motion==stance,"four-state draw overlap is active-list order independent");
        const auto validOverlap=unknown;
        unknown.active[2].vtable++;require(!nativePlayerMotion(unknown,base,motion),"unknown Draw class cannot supply locomotion");
        unknown=validOverlap;unknown.active[0]=unknown.active[1];require(!nativePlayerMotion(unknown,base,motion),"duplicate four-state equipment overlay refused");
        unknown=validOverlap;unknown.active.pop_back();require(!nativePlayerMotion(unknown,base,motion),"Draw and equipment alone do not invent grounded locomotion");
        unknown=validOverlap;unknown.active.erase(unknown.active.begin());require(!nativePlayerMotion(unknown,base,motion),"unobserved partial Draw overlap refused");
        unknown=validOverlap;unknown.player.current=1-stance;require(!nativePlayerMotion(unknown,base,motion),"four-state draw respects physical stance");
    }
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
    for(const auto& move:{NativePlayerActiveState{0,0,base+0x1c85628},NativePlayerActiveState{6,0,base+0x1c84258},NativePlayerActiveState{2,0,base+0x1c85028},NativePlayerActiveState{3,0,base+0x1c845c8}}){
        const NativePlayerActiveState power{63,0,base+0x1c80ed8};unknown=actionState;unknown.active={move,power};
        const unsigned expected=move.id==6?1:move.id;
        require(nativePlayerMotion(unknown,base,motion)&&motion==expected,"observed power overlay preserves grounded and airborne movement");
        unknown.active={power,move};require(nativePlayerMotion(unknown,base,motion)&&motion==expected,"power overlay movement is order independent");
        unknown.active[0].vtable++;require(!nativePlayerMotion(unknown,base,motion),"power overlay wrong class refused");
        unknown.active={power};require(!nativePlayerMotion(unknown,base,motion),"power alone does not invent locomotion");
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
void windblastFxRecordTests(){
    constexpr std::uintptr_t base=0x140000000,game=0x200000000,pool=0x300000000,buckets=0x400000000,node=0x500000000,decl=0x600000000,name=decl+0x1000;
    const auto manager=game+0x1f8430,map=manager+0xeef60,record=pool+0x140;
    Memory m;m.q(base+0x39a5290,game);m.q(game,base+0x1c940a8);m.q(manager,pool);m.q(map,buckets);m.d(map+0xc,1);m.d(map+0x10,1);
    m.q(buckets,0);m.q(buckets+8,node);m.d(node,7);m.d(node+4,1);m.q(node+8,0);m.d(record,0x3fff0010);m.d(record+4,7);m.q(record+8,decl);
    m.q(decl,base+0x1ab3138);m.q(decl+8,name);m.str(name,"models/effects/gameplay/powers/windblast/blast/wblast_blast_01.fx");
    float transform[12]={1,2,3,1,0,0,0,1,0,0,0,1};m.put(record+0x38,transform,sizeof(transform));
    auto read=[&](auto a,auto b,auto n){return m.read(a,b,n);};std::vector<NativeWindblastFxRecord> out;
    require(readNativeWindblastFxRecords(read,base,game,out)&&out.size()==1&&out[0].id==7&&out[0].origin[2]==3,"active native Windblast FX map");
    m.d(record+4,8);require(!readNativeWindblastFxRecords(read,base,game,out)&&out.empty(),"recycled FX ID refused");m.d(record+4,7);
    m.q(node+8,node);require(!readNativeWindblastFxRecords(read,base,game,out),"FX map cycle refused");m.q(node+8,0);
    m.d(node+4,1024);require(!readNativeWindblastFxRecords(read,base,game,out),"FX pool overrun refused");m.d(node+4,1);
    m.d(map+0xc,2);require(!readNativeWindblastFxRecords(read,base,game,out),"inconsistent FX count refused");m.d(map+0xc,1);
    m.str(name,"models/effects/gameplay/powers/windblast/unrecognized.fx");require(readNativeWindblastFxRecords(read,base,game,out)&&out.empty(),"only exact Windblast declarations returned");
    m.str(name,"models/effects/gameplay/powers/windblast/blast/wblast_blast_01.fx");unsigned reads=0;
    auto changing=[&](auto a,auto b,auto n){if(a==record&&++reads==2)m.d(record+4,8);return m.read(a,b,n);};
    require(!readNativeWindblastFxRecords(changing,base,game,out),"FX recycling during snapshot refused");
    constexpr std::uintptr_t actor=0x700000000;const unsigned handle=0x40001;
    m.d(record+4,7);m.d(record,handle);m.q(game+0x3130b0+4*8,actor);m.d(actor+0xa8,handle);m.d(actor+0xac,0x3fff0010);m.q(actor+0x2e8,0);
    m.str(name,"models/effects/weapons/gun/muzzle_1p_corvo.fx");
    require(readNativeRangedFxRecords(read,base,game,out)&&out.size()==1&&out[0].ownerActor==actor&&out[0].ownerDefinition.empty(),"temporary native FX owner may have no asset definition");
    for(const char* remoteName:{"models/effects/d2coop/muzzle_remote_corvo.fx","models/effects/d2coop/muzzle_remote_emily.fx"}){
        m.str(name,remoteName);require(readNativeRangedFxRecords(read,base,game,out)&&out.size()==1&&out[0].name==remoteName,"exact remote cosmetic muzzle definition can be independently inspected");
    }
    m.str(name,"models/effects/weapons/gun/muzzle_1p_corvo.fx");
    unsigned defReads=0;auto replaced=[&](auto a,auto b,auto n){if(a==actor+0x2e8&&++defReads==2)m.q(actor+0x2e8,decl);return m.read(a,b,n);};
    require(!readNativeRangedFxRecords(replaced,base,game,out),"FX owner definition changing during read refused");
    m.q(actor+0x2e8,0);unsigned ownerReads=0;auto retired=[&](auto a,auto b,auto n){if(a==actor+0xa8&&++ownerReads==2)m.d(actor+0xa8,handle+1);return m.read(a,b,n);};
    require(!readNativeRangedFxRecords(retired,base,game,out),"temporary FX owner retirement during read refused");
}
int main() {
    nativeRetargetingTests();
    crossbowPresentationTests();
    windblastFxRecordTests();
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
    {
      const auto sk=carryIdentityKey(ac[0].spawnerName),dk=carryIdentityKey(ac[0].definition);NativeWorldNpc target{};
      auto read=[&](auto p,auto d,auto n){return b.read(p,d,n);};
      require(resolveCarryIdentity(read,base,game,bc,sk,dk,target)&&target.handle==0x70051,"carry target resolves different local generation by authored identity");
      require(!resolveCarryIdentity(read,base,game,bc,sk+1,dk,target),"unknown carry key refused");
      auto ambiguous=bc;ambiguous.push_back(bc[0]);require(!resolveCarryIdentity(read,base,game,ambiguous,sk,dk,target),"ambiguous carry target refused");
      auto wrong=bc;wrong[0].component++;require(!resolveCarryIdentity(read,base,game,wrong,sk,dk,target),"changed spawner component refused");
      b.d(actor+0xa8,0x70052);require(!resolveCarryIdentity(read,base,game,bc,sk,dk,target),"stale cached corpse generation refused");b.d(actor+0xa8,0x70051);
    }
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
