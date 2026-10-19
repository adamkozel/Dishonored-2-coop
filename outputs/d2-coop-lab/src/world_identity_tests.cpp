#include "native_mesmerize_targets.h"
#include "native_possession_target_fx.h"
#include "native_mesmerize_eye.h"
#include "world_identity.h"
#include "map_path.h"
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
#include "native_player_mesmerize_item.h"
#include "native_player_domino_item.h"
#include "native_player_swarm_item.h"
#include "native_player_doppelganger_item.h"
#include "native_player_shadowwalk_item.h"
#include "native_player_shadowwalk_state.h"
#include "native_player_shadowwalk_scene.h"
#include "native_swarm_world.h"
#include "native_domino_targets.h"
#include "native_domino_links.h"
#include "domino_partner_identity.h"
#include "possession_partner_identity.h"
#include "assassination_partner_identity.h"
#include "interaction_partner_identity.h"
#include "native_player_domino_scene.h"
#include "native_player_swarm_scene.h"
#include "native_player_doppelganger_scene.h"
#include "native_player_possession_scene.h"
#include "native_mesmerize_world.h"
#include "native_player_ranged_item.h"
#include "native_crossbow_presentation.h"
#include "native_player_ranged_scene.h"
#include "native_windblast_fx_records.h"
#include "native_player_power_scene.h"
#include "native_player_agility.h"
#include "native_player_potion.h"
#include "native_potion_statistics.h"
#include "native_player_mana.h"
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
#include "native_domino_links_tests.h"
#include "native_domino_targets_tests.h"
#include "native_possession_target_fx_tests.h"
#include "native_swarm_world_tests.h"
#include "native_doppelganger_world_tests.h"
#include "domino_partner_identity_tests.h"
#include "possession_partner_identity_tests.h"
#include "assassination_partner_identity_tests.h"
#include "interaction_partner_identity_tests.h"
#include "native_crossbow_presentation_tests.h"
#include "native_retargeting_tests.h"
#include "native_cosmetic_grenade_tests.h"
#include "native_player_stun_tests.h"
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
                for(unsigned kind=1;kind<=2;++kind){
                    auto potion=agility;potion.str(an,kind==1?"components/interactive/health_potion.inventoryitem":"components/interactive/mana_potion.inventoryitem");potion.d(ad+0x60,14);potion.d(ae+0x24,10);
                    NativePlayerPotion observedPotion{};const auto inspectPotion=[&](Memory& mem){return readNativePlayerPotion([&](auto p,auto d,auto n){return mem.read(p,d,n);},base,kind,observedPotion);};
                    require(inspectPotion(potion)&&observedPotion.quantity==0&&observedPotion.kind==kind,"exact native scalar potion identified without grant");
                    auto bad=potion;bad.d(ae+0x20,1);require(inspectPotion(bad)&&observedPotion.quantity==1,"native owned potion quantity observed");
                    bad.str(names+0x400,"models/characters/player/emily/body.def");bad.str(names+0x600,"models/characters/player/emily/right_arm.def");require(inspectPotion(bad)&&observedPotion.player.character==2,"native potion identity works on Emily");
                    bad=potion;bad.d(ad+0x60,18);require(!inspectPotion(bad),"enhancement cannot impersonate potion");
                    bad=potion;bad.q(ae+0x10,cpnt);require(!inspectPotion(bad),"potion cannot own unexpected equipment");
                    bad=potion;bad.d(ae+8,0x20003);require(!inspectPotion(bad),"scalar potion cannot be a world entity");
                    bad=potion;bad.d(ae+0x20,11);require(!inspectPotion(bad),"out-of-range potion quantity refused");
                    bad=potion;bad.d(ae+0x24,99);require(!inspectPotion(bad),"unknown native potion maximum refused");
                    bad=potion;unsigned reads=0;require(!readNativePlayerPotion([&](auto p,auto d,auto n){const bool ok=bad.read(p,d,n);if(p==ae+0x20&&++reads==1)bad.d(p,1);return ok;},base,kind,observedPotion),"concurrent potion consumption refused for identity snapshot");
                }
                std::cout<<"PASS: native health/mana scalar potion identities, both roles, bounds and concurrent consumption refusal\n";
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
                    {
                        auto possession=activeDark;
                        possession.str(strings,"models/powers/possession/possession.inventoryitem");
                        possession.str(strings+0x100,"components/characters/player/base/powers/possession/possession.cpntpowerpossession");
                        possession.q(pc,base+0x1b531c8);possession.q(ped,base+0x1b53088);
                        possessionTargetFxTests(possession,base,game,pc);
                    }
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
                    {
                        auto mesmer=wind;
                        mesmer.str(names+0x400,"models/characters/player/emily/body.def");mesmer.str(names+0x600,"models/characters/player/emily/right_arm.def");
                        mesmer.str(strings,"models/powers/mesmerize/mesmerize.inventoryitem");
                        mesmer.str(strings+0x100,"components/characters/player/base/powers/mesmerize/mesmerize.cpntpowermesmerize");
                        mesmer.q(pc,base+0x1b9bea8);mesmer.q(ped,base+0x1b9bd68);
                        {
                            auto swarm=mesmer;
                            swarm.str(names+0x400,"models/characters/player/corvo/body.def");swarm.str(names+0x600,"models/characters/player/corvo/right_arm.def");
                            swarm.str(strings,"models/powers/devouring_swarm/devouring_swarm.inventoryitem");
                            swarm.str(strings+0x100,"components/characters/player/base/powers/devouring_swarm/power.cpntpowerdevouringswarm");
                            swarm.q(pc,base+0x1b51f28);swarm.q(ped,base+0x1b51de8);
                            NativeSwarmItem item{};const auto inspectSwarm=[&](Memory& mem){return readNativeSwarmItem([&](auto a,auto d,auto n){return mem.read(a,d,n);},base,item);};
                            require(inspectSwarm(swarm)&&item.player.character==1&&item.instantiated&&item.started&&item.equipped,"Swarm static RTTI and exact native asset identities are coherent");
                            auto inlineSwarm=swarm;constexpr auto swarmParent=inv+0x78000,swarmParentName=inv+0x79000;
                            inlineSwarm.str(strings+0x100,"unnamed");inlineSwarm.q(ped+0x48,swarmParent);
                            inlineSwarm.q(swarmParent,base+0x1b51de8);inlineSwarm.q(swarmParent+8,swarmParentName);
                            inlineSwarm.str(swarmParentName,"components/characters/player/base/powers/devouring_swarm/power.cpntpowerdevouringswarm");
                            require(inspectSwarm(inlineSwarm)&&item.parentDeclaration==swarmParent,"candidate inline Swarm declaration requires its exact named parent");
                            swarmWorldTests(inlineSwarm,base,game,pc);
                            auto badParent=inlineSwarm;badParent.q(ped+0x48,0);require(!inspectSwarm(badParent),"unnamed Swarm declaration requires a parent");
                            badParent=inlineSwarm;badParent.q(ped+0x48,ped);require(!inspectSwarm(badParent),"self-parented Swarm declaration rejected");
                            badParent=inlineSwarm;badParent.q(swarmParent,base+0x1b9bd68);require(!inspectSwarm(badParent),"wrong native parent RTTI rejected");
                            badParent=inlineSwarm;badParent.str(swarmParentName,"components/characters/player/base/powers/mesmerize/mesmerize.cpntpowermesmerize");require(!inspectSwarm(badParent),"wrong named parent rejected despite matching outer Swarm RTTI");
                            badParent=inlineSwarm;unsigned parentReads=0;require(!readNativeSwarmItem([&](auto a,auto d,auto n){const bool ok=badParent.read(a,d,n);if(a==ped+0x48&&++parentReads==1)badParent.q(a,0);return ok;},base,item),"concurrent native parent replacement rejected");
                            auto unowned=swarm;unowned.d(inv+0x60,1);unowned.q(inv+0x70,0);unowned.d(pe+8,0x3fff0010);unowned.q(pe+0x10,0);unowned.q(pe+0x18,0);unowned.d(pe+0x20,0);
                            require(inspectSwarm(unowned)&&!item.instantiated&&!item.quantity,"recorded unowned Swarm inventory does not imply a spawned swarm");
                            auto invalid=swarm;invalid.str(names+0x400,"models/characters/player/emily/body.def");invalid.str(names+0x600,"models/characters/player/emily/right_arm.def");require(!inspectSwarm(invalid),"Swarm initial native fixture refuses Emily");
                            invalid=swarm;invalid.q(pc,base+0x1b9bea8);require(!inspectSwarm(invalid),"Mesmerize cannot impersonate Swarm equipment");
                            invalid=swarm;invalid.q(pc+0x10,actor);require(!inspectSwarm(invalid),"Swarm rejects a foreign component owner");
                            invalid=swarm;invalid.d(entity+0xa8,handle+1);require(!inspectSwarm(invalid),"Swarm rejects a recycled equipment entity");
                            invalid=swarm;invalid.d(pe+0x20,2);require(!inspectSwarm(invalid),"Swarm rejects impossible quantity");
                            invalid=swarm;unsigned changed=0;require(!readNativeSwarmItem([&](auto a,auto d,auto n){const bool ok=invalid.read(a,d,n);if(a==pc+0x1c&&++changed==1)invalid.d(a,0);return ok;},base,item),"Swarm detects concurrent equipment retirement");
                            std::cout<<"PASS: Swarm exact asset/RTTI, native unowned inventory, character, ownership and race guards\n";
                        }
                        {
                            auto domino=mesmer;
                            domino.str(strings,"models/powers/phantasmallink/phantasmal_link.inventoryitem");
                            domino.str(strings+0x100,"components/characters/player/base/powers/phantasmallink/phantasmal_link.cpntpowerphantasmallink");
                            domino.q(pc,base+0x1b52ec8);domino.q(ped,base+0x1b52d88);
                            NativeDominoItem item{};const auto inspectDomino=[&](Memory& mem){return readNativeDominoItem([&](auto a,auto d,auto n){return mem.read(a,d,n);},base,item);};
                            require(inspectDomino(domino)&&item.player.character==2&&item.instantiated&&item.started&&item.equipped,"Domino static RTTI and exact native asset identities are coherent");
                            auto inlineDomino=domino;constexpr auto dominoParent=inv+0x78000,dominoParentName=inv+0x79000;
                            inlineDomino.str(strings+0x100,"unnamed");inlineDomino.q(ped+0x48,dominoParent);
                            inlineDomino.q(dominoParent,base+0x1b52d88);inlineDomino.q(dominoParent+8,dominoParentName);
                            inlineDomino.str(dominoParentName,"components/characters/player/base/powers/phantasmallink/phantasmal_link.cpntpowerphantasmallink");
                            require(inspectDomino(inlineDomino)&&item.parentDeclaration==dominoParent,"v185 native inline Domino declaration resolves its exact named parent");
                            auto badParent=inlineDomino;badParent.q(ped+0x48,0);require(!inspectDomino(badParent),"unnamed Domino declaration requires a parent");
                            badParent=inlineDomino;badParent.q(ped+0x48,ped);require(!inspectDomino(badParent),"self-parented Domino declaration rejected");
                            badParent=inlineDomino;badParent.q(dominoParent,base+0x1b9bd68);require(!inspectDomino(badParent),"wrong native parent RTTI rejected");
                            badParent=inlineDomino;badParent.str(dominoParentName,"components/characters/player/base/powers/mesmerize/mesmerize.cpntpowermesmerize");require(!inspectDomino(badParent),"wrong named parent rejected despite matching outer Domino RTTI");
                            badParent=inlineDomino;unsigned parentReads=0;require(!readNativeDominoItem([&](auto a,auto d,auto n){const bool ok=badParent.read(a,d,n);if(a==ped+0x48&&++parentReads==1)badParent.q(a,0);return ok;},base,item),"concurrent native parent replacement rejected");
                            dominoTargetsTests(inlineDomino,base,game,pc);
                            dominoPartnerIdentityTests(inlineDomino,base,game);
                            auto unowned=domino;unowned.d(inv+0x60,1);unowned.q(inv+0x70,0);unowned.d(pe+8,0x3fff0010);unowned.q(pe+0x10,0);unowned.q(pe+0x18,0);unowned.d(pe+0x20,0);
                            require(inspectDomino(unowned)&&!item.instantiated&&!item.quantity,"recorded unowned Domino inventory does not imply a target network");
                            auto invalid=domino;invalid.str(names+0x400,"models/characters/player/corvo/body.def");invalid.str(names+0x600,"models/characters/player/corvo/right_arm.def");require(!inspectDomino(invalid),"Domino initial native fixture refuses Corvo");
                            invalid=domino;invalid.q(pc,base+0x1b9bea8);require(!inspectDomino(invalid),"Mesmerize cannot impersonate Domino equipment");
                            invalid=domino;invalid.q(pc+0x10,actor);require(!inspectDomino(invalid),"Domino rejects a foreign component owner");
                            invalid=domino;invalid.d(entity+0xa8,handle+1);require(!inspectDomino(invalid),"Domino rejects a recycled equipment entity");
                            invalid=domino;invalid.d(pe+0x20,2);require(!inspectDomino(invalid),"Domino rejects impossible quantity");
                            invalid=domino;unsigned changed=0;require(!readNativeDominoItem([&](auto a,auto d,auto n){const bool ok=invalid.read(a,d,n);if(a==pc+0x1c&&++changed==1)invalid.d(a,0);return ok;},base,item),"Domino detects concurrent equipment retirement");
                            std::cout<<"PASS: Domino exact asset/RTTI, native unowned inventory, character, ownership and race guards\n";
                        }
                        {
                            auto doppelganger=mesmer;
                            doppelganger.str(strings,"models/powers/doppelganger/doppelganger.inventoryitem");
                            doppelganger.str(strings+0x100,"components/characters/player/base/powers/doppelganger/doppelganger_power.cpntpowerdoppelganger");
                            doppelganger.q(pc,base+0x1b52258);doppelganger.q(ped,base+0x1b52118);
                            NativeDoppelgangerItem item{};const auto inspectDoppelganger=[&](Memory& mem){return readNativeDoppelgangerItem([&](auto a,auto d,auto n){return mem.read(a,d,n);},base,item);};
                            doppelgangerWorldTests(doppelganger,base,game,pc);
                            require(inspectDoppelganger(doppelganger)&&item.player.character==2&&item.instantiated&&item.started&&item.equipped,"Doppelganger static RTTI and exact native asset identities are coherent");
                            auto inlineDoppelganger=doppelganger;constexpr auto doppelgangerParent=inv+0x78000,doppelgangerParentName=inv+0x79000;
                            inlineDoppelganger.str(strings+0x100,"unnamed");inlineDoppelganger.q(ped+0x48,doppelgangerParent);
                            inlineDoppelganger.q(doppelgangerParent,base+0x1b52118);inlineDoppelganger.q(doppelgangerParent+8,doppelgangerParentName);
                            inlineDoppelganger.str(doppelgangerParentName,"components/characters/player/base/powers/doppelganger/doppelganger_power.cpntpowerdoppelganger");
                            require(inspectDoppelganger(inlineDoppelganger)&&item.parentDeclaration==doppelgangerParent,"v404 candidate inline Doppelganger declaration resolves its exact named parent");
                            auto badParent=inlineDoppelganger;badParent.q(ped+0x48,0);require(!inspectDoppelganger(badParent),"unnamed Doppelganger declaration requires a parent");
                            badParent=inlineDoppelganger;badParent.q(ped+0x48,ped);require(!inspectDoppelganger(badParent),"self-parented Doppelganger declaration rejected");
                            badParent=inlineDoppelganger;badParent.q(doppelgangerParent,base+0x1b9bd68);require(!inspectDoppelganger(badParent),"wrong native parent RTTI rejected");
                            badParent=inlineDoppelganger;badParent.str(doppelgangerParentName,"components/characters/player/base/powers/mesmerize/mesmerize.cpntpowermesmerize");require(!inspectDoppelganger(badParent),"wrong named parent rejected despite matching outer Doppelganger RTTI");
                            badParent=inlineDoppelganger;unsigned parentReads=0;require(!readNativeDoppelgangerItem([&](auto a,auto d,auto n){const bool ok=badParent.read(a,d,n);if(a==ped+0x48&&++parentReads==1)badParent.q(a,0);return ok;},base,item),"concurrent native parent replacement rejected");
                            auto unowned=doppelganger;unowned.d(inv+0x60,1);unowned.q(inv+0x70,0);unowned.d(pe+8,0x3fff0010);unowned.q(pe+0x10,0);unowned.q(pe+0x18,0);unowned.d(pe+0x20,0);
                            require(inspectDoppelganger(unowned)&&!item.instantiated&&!item.quantity,"recorded unowned Doppelganger inventory does not imply a spawned clone");
                            auto invalid=doppelganger;invalid.str(names+0x400,"models/characters/player/corvo/body.def");invalid.str(names+0x600,"models/characters/player/corvo/right_arm.def");require(!inspectDoppelganger(invalid),"Doppelganger initial native fixture refuses Corvo");
                            invalid=doppelganger;invalid.q(pc,base+0x1b9bea8);require(!inspectDoppelganger(invalid),"Mesmerize cannot impersonate Doppelganger equipment");
                            invalid=doppelganger;invalid.q(pc+0x10,actor);require(!inspectDoppelganger(invalid),"Doppelganger rejects a foreign component owner");
                            invalid=doppelganger;invalid.d(entity+0xa8,handle+1);require(!inspectDoppelganger(invalid),"Doppelganger rejects a recycled equipment entity");
                            invalid=doppelganger;invalid.d(pe+0x20,2);require(!inspectDoppelganger(invalid),"Doppelganger rejects impossible quantity");
                            invalid=doppelganger;unsigned changed=0;require(!readNativeDoppelgangerItem([&](auto a,auto d,auto n){const bool ok=invalid.read(a,d,n);if(a==pc+0x1c&&++changed==1)invalid.d(a,0);return ok;},base,item),"Doppelganger detects concurrent equipment retirement");
                            std::cout<<"PASS: Doppelganger exact asset/RTTI, native unowned inventory, character, ownership and race guards\n";
                        }
                        {
                            auto shadowwalk=mesmer;
                            shadowwalk.str(strings,"models/powers/shadow_walk/shadow_walk.inventoryitem");
                            shadowwalk.str(strings+0x100,"components/characters/player/base/powers/shadow_walk/shadow_walk.cpntpowershadowwalk");
                            shadowwalk.q(pc,base+0x1b9c248);shadowwalk.q(ped,base+0x1b9c108);
                            NativeShadowWalkItem item{};const auto inspectShadowWalk=[&](Memory& mem){return readNativeShadowWalkItem([&](auto a,auto d,auto n){return mem.read(a,d,n);},base,item);};
                            require(inspectShadowWalk(shadowwalk)&&item.player.character==2&&item.instantiated&&item.started&&item.equipped,"ShadowWalk static RTTI and exact native asset identities are coherent");
                            auto inlineShadowWalk=shadowwalk;constexpr auto shadowwalkParent=inv+0x78000,shadowwalkParentName=inv+0x79000;
                            inlineShadowWalk.str(strings+0x100,"unnamed");inlineShadowWalk.q(ped+0x48,shadowwalkParent);
                            inlineShadowWalk.q(shadowwalkParent,base+0x1b9c108);inlineShadowWalk.q(shadowwalkParent+8,shadowwalkParentName);
                            inlineShadowWalk.str(shadowwalkParentName,"components/characters/player/base/powers/shadow_walk/shadow_walk.cpntpowershadowwalk");
                            require(inspectShadowWalk(inlineShadowWalk)&&item.parentDeclaration==shadowwalkParent,"v516 native inline ShadowWalk declaration resolves its exact named parent");
                            auto badParent=inlineShadowWalk;badParent.q(ped+0x48,0);require(!inspectShadowWalk(badParent),"unnamed ShadowWalk declaration requires a parent");
                            badParent=inlineShadowWalk;badParent.q(ped+0x48,ped);require(!inspectShadowWalk(badParent),"self-parented ShadowWalk declaration rejected");
                            badParent=inlineShadowWalk;badParent.q(shadowwalkParent,base+0x1b9bd68);require(!inspectShadowWalk(badParent),"wrong native parent RTTI rejected");
                            badParent=inlineShadowWalk;badParent.str(shadowwalkParentName,"components/characters/player/base/powers/mesmerize/mesmerize.cpntpowermesmerize");require(!inspectShadowWalk(badParent),"wrong named parent rejected despite matching outer ShadowWalk RTTI");
                            badParent=inlineShadowWalk;unsigned parentReads=0;require(!readNativeShadowWalkItem([&](auto a,auto d,auto n){const bool ok=badParent.read(a,d,n);if(a==ped+0x48&&++parentReads==1)badParent.q(a,0);return ok;},base,item),"concurrent native parent replacement rejected");
                            auto unowned=shadowwalk;unowned.d(inv+0x60,1);unowned.q(inv+0x70,0);unowned.d(pe+8,0x3fff0010);unowned.q(pe+0x10,0);unowned.q(pe+0x18,0);unowned.d(pe+0x20,0);
                            require(inspectShadowWalk(unowned)&&!item.instantiated&&!item.quantity,"recorded unowned ShadowWalk inventory does not imply active form");
                            auto invalid=shadowwalk;invalid.str(names+0x400,"models/characters/player/corvo/body.def");invalid.str(names+0x600,"models/characters/player/corvo/right_arm.def");require(!inspectShadowWalk(invalid),"ShadowWalk initial native fixture refuses Corvo");
                            invalid=shadowwalk;invalid.q(pc,base+0x1b9bea8);require(!inspectShadowWalk(invalid),"Mesmerize cannot impersonate ShadowWalk equipment");
                            invalid=shadowwalk;invalid.q(pc+0x10,actor);require(!inspectShadowWalk(invalid),"ShadowWalk rejects a foreign component owner");
                            invalid=shadowwalk;invalid.d(entity+0xa8,handle+1);require(!inspectShadowWalk(invalid),"ShadowWalk rejects a recycled equipment entity");
                            invalid=shadowwalk;invalid.d(pe+0x20,2);require(!inspectShadowWalk(invalid),"ShadowWalk rejects impossible quantity");
                            invalid=shadowwalk;unsigned changed=0;require(!readNativeShadowWalkItem([&](auto a,auto d,auto n){const bool ok=invalid.read(a,d,n);if(a==pc+0x1c&&++changed==1)invalid.d(a,0);return ok;},base,item),"ShadowWalk detects concurrent equipment retirement");
                            std::cout<<"PASS: ShadowWalk exact asset/RTTI, native unowned inventory, character, ownership and race guards\n";
                        }
                        NativeMesmerizeItem observed{};const auto inspectMesmer=[&](Memory& mem){return readNativeMesmerizeItem([&](auto a,auto d,auto n){return mem.read(a,d,n);},base,observed);};
                        require(inspectMesmer(mesmer)&&observed.player.character==2&&observed.instantiated&&observed.started&&observed.equipped,"exact native Emily Mesmerize equipment identity");
                        auto missing=mesmer;missing.d(inv+0x60,1);missing.q(inv+0x70,0);missing.d(pe+8,0x3fff0010);missing.q(pe+0x10,0);missing.q(pe+0x18,0);missing.d(pe+0x20,0);
                        require(inspectMesmer(missing)&&!observed.instantiated&&!observed.quantity,"unowned Mesmerize has no inferred world entity");
                        auto bad=mesmer;bad.str(names+0x400,"models/characters/player/corvo/body.def");bad.str(names+0x600,"models/characters/player/corvo/right_arm.def");require(!inspectMesmer(bad),"Mesmerize isolated fixture refuses Corvo");
                        bad=mesmer;bad.q(pc,base+0x1b53488);require(!inspectMesmer(bad),"Windblast cannot impersonate Mesmerize");
                        bad=mesmer;bad.q(pc+0x10,actor);require(!inspectMesmer(bad),"foreign Mesmerize component owner refused");
                        bad=mesmer;bad.d(entity+0xa8,handle+1);require(!inspectMesmer(bad),"recycled Mesmerize entity refused");
                        bad=mesmer;bad.d(pe+0x20,2);require(!inspectMesmer(bad),"invalid Mesmerize quantity refused");
                        bad=mesmer;unsigned reads=0;require(!readNativeMesmerizeItem([&](auto a,auto d,auto n){const bool ok=bad.read(a,d,n);if(a==pc+0x1c&&++reads==1)bad.d(a,0);return ok;},base,observed),"concurrent Mesmerize retirement refused");
                        {
                            auto world=mesmer;world.d(pc+0xd4,0x3fff0010);
                            NativeMesmerizeWorld result{};const auto inspectWorld=[&](Memory& m){return readNativeMesmerizeWorld([&](auto a,auto d,auto n){return m.read(a,d,n);},base,result);};
                            require(inspectWorld(world)&&!result.present,"equipment sentinel is absence, not permission to search preview entities");
                            constexpr auto wa=inv+0x53000,wd=inv+0x54000,wc=inv+0x55000,wl=inv+0x56000,wcd=inv+0x57000,wn=inv+0x58000;
                            constexpr unsigned wh=0x70008;
                            world.q(game+0x3130b0+7*8,wa);world.q(wa,base+0x1c87d58);world.d(wa+0xa8,wh);world.d(wa+0xac,0x3fff0010);
                            world.q(wa+0x2e8,wd);world.q(wd,base+0x1ab58f8);world.q(wd+8,wn);world.str(wn,"models/powers/mesmerize/mesmerize_object.def");
                            world.q(wa+0x268,wl);world.d(wa+0x270,1);world.q(wl,wc);world.q(wc,base+0x1c53958);world.q(wc+8,wcd);world.q(wc+0x10,wa);world.q(wc+0x20,wc);world.d(wc+0x1c,0x2000000a);world.d(wc+0x2c,1);
                            world.q(wcd,base+0x1c53a38);world.q(wcd+8,wn+0x100);world.str(wn+0x100,"components/characters/player/base/powers/mesmerize/mesmerize_object.cpntmesmerize");
                            const float tr[12]={1,2,3,1,0,0,0,1,0,0,0,1};world.put(wa+0x300,tr,sizeof(tr));
                            require(inspectWorld(world)&&!result.present,"existing preview is ignored until native equipment commits its handle");
                            world.d(pc+0xd4,wh);require(inspectWorld(world)&&result.present&&result.handle==wh&&result.state==1,"exact committed world relationship resolves independent native component");
                            auto b=world;b.q(wc+0x10,actor);require(!inspectWorld(b),"world component with foreign owner refused");
                            b=world;b.d(wa+0xa8,wh+0x10000);require(!inspectWorld(b),"recycled committed world incarnation refused");
                            b=world;b.q(wcd,base+0x1b9bd68);require(!inspectWorld(b),"equipment declaration cannot replace world declaration");
                            b=world;b.d(wa+0x270,2);b.q(wl+8,wc);require(!inspectWorld(b),"duplicate world component refused");
                            b=world;b.d(wc+0x2c,5);require(!inspectWorld(b),"unknown world FSM state refused");
                            b=world;b.d(wa+0xac,1);require(!inspectWorld(b),"unobserved parented world entity refused");
                            b=world;b.q(wc+0x20,actor);require(!inspectWorld(b),"invalid state-machine self pointer refused");
                            b=world;b.d(wa+0x30c,0);require(!inspectWorld(b),"degenerate world orientation refused");
                            b=world;unsigned count=0;require(!readNativeMesmerizeWorld([&](auto a,auto d,auto n){const bool ok=b.read(a,d,n);if(a==pc+0xd4&&++count==1)b.d(a,0x3fff0010);return ok;},base,result),"concurrent world relationship retirement refused");
                            b=world;count=0;require(!readNativeMesmerizeWorld([&](auto a,auto d,auto n){const bool ok=b.read(a,d,n);if(a==wc+0x2c&&++count==1)b.d(a,2);return ok;},base,result),"concurrent world FSM transition refused");
                            std::cout<<"PASS: Mesmerize committed world ownership, ignored preview, exact component, state, transform and retirement guards\n";
                        }
                        std::cout<<"PASS: Mesmerize native item, Emily scope, ownership, quantity, stale handle and concurrent retirement guards\n";
                    }
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
                    {
                        auto sw=cast;
                        sw.str(names+0x400,"models/characters/player/emily/body.def");sw.str(names+0x600,"models/characters/player/emily/right_arm.def");
                        sw.str(texts+0x100,"models/characters/player/emily/left_arm.def");
                        sw.str(strings,"models/powers/shadow_walk/shadow_walk.inventoryitem");sw.str(strings+0x100,"components/characters/player/base/powers/shadow_walk/shadow_walk.cpntpowershadowwalk");
                        sw.q(pc,base+0x1b9c248);sw.q(ped,base+0x1b9c108);sw.q(pc+0xc0,machine);sw.d(pc+0xe4,0);
                        NativeShadowWalkState observed{};
                        const auto inspect=[&](Memory& m){return readNativeShadowWalkState([&](auto a,auto b,auto n){return m.read(a,b,n);},base,observed);};
                        require(inspect(sw)&&!observed.form&&!observed.phase,"Shadow Walk exact selected native ready context");
                        for(unsigned phase:{2u,3u}){auto m=sw;m.d(pc+0xe4,phase);require(inspect(m)&&!observed.form,"native standing Shadow Walk preparation before mode switch");}
                        {auto low=sw;low.d(physics+0x50,1);low.d(physics+0x54,1);low.d(active,6);
                         low.q(mode+0x10+6*8,ground);low.q(ground,base+0x1c84258);low.d(ground+8,6);low.d(pc+0xe4,2);
                         require(inspect(low)&&!observed.form&&observed.phase==2,"observed547b native sneaking preparation accepted");
                         low.d(pc+0xe4,3);require(!inspect(low),"unobserved default-mode crouched transition not inferred");
                         low.d(pc+0xe4,2);low.q(ground,base+0x1c85628);require(!inspect(low),"crouched phase cannot borrow standing class");}
                        constexpr auto alternate=inv+0x7d000;
                        auto form=sw;form.q(machine+0x90,alternate);form.q(alternate,machine);form.q(alternate+0x10,ground);form.q(ground,base+0x1c7ed28);
                        form.d(physics+0x50,2);form.d(physics+0x54,2);
                        for(unsigned phase:{3u,4u,5u}){form.d(pc+0xe4,phase);require(inspect(form)&&observed.form&&observed.phase==phase,"native alternate Shadow Walk mode accepts observed transition/form phases");}
                        form.d(pc+0xe4,4);
                        {const auto rd=[&](auto a,auto b,auto n){return form.read(a,b,n);};NativePlayerStates states{};NativePlayerPowerScene scene{};
                         require(readNativePlayerStates(rd,base,states),"Shadow Walk form fixture state readable");
                         require(!readNativePlayerShadowWalkScene(rd,base,states,scene),"unobserved armed Shadow Walk form rejected");
                        }
                        require(nativeShadowWalkVariant(3,"midnight/gameplay/player/base/powers/power_shadow_walk_trans_in.midnightscene","emily",1000)==32,"exact native Shadow Walk entry resource and clock");
                        require(nativeShadowWalkVariant(5,"midnight/gameplay/player/base/powers/power_shadow_walk_trans_out.midnightscene","Emily",1133)==34,"exact native Shadow Walk exit resource and clock");
                        require(nativeShadowWalkVariant(3,"midnight/gameplay/player/base/powers/power_shadow_walk_trans_in_sneak.midnightscene","emily",1000)==35,"547b exact native sneaking entry resource and clock");
                        require(nativeShadowWalkVariant(5,"midnight/gameplay/player/base/powers/power_shadow_walk_attack_out.midnightscene","Emily",1166)==36,"1214da exact native attack exit resource and clock");
                        require(!nativeShadowWalkVariant(5,"midnight/gameplay/player/base/powers/power_shadow_walk_attack_out.midnightscene","Emily",1133),"attack exit requires its own clock");
                        require(!nativeShadowWalkVariant(3,"midnight/gameplay/player/base/powers/power_shadow_walk_attack_out.midnightscene","Emily",1166),"attack exit cannot impersonate entry");
                        require(!nativeShadowWalkVariant(3,"midnight/gameplay/player/base/powers/power_shadow_walk_trans_in_sneak.midnightscene","emily",999),"sneaking scene requires recorded clock");
                        require(!nativeShadowWalkVariant(3,"midnight/gameplay/player/base/powers/power_shadow_walk_trans_in_sneak.midnightscene","Corvo",1000),"Corvo cannot borrow Emily sneaking form");
                        require(!nativeShadowWalkVariant(4,"midnight/gameplay/player/base/powers/power_shadow_walk_trans_in.midnightscene","emily",1000),"entry resource cannot impersonate sustained form");
                        require(!nativeShadowWalkVariant(3,"midnight/gameplay/player/base/powers/power_shadow_walk_trans_in_crouch.midnightscene","emily",1000),"unobserved crouched entry not silently admitted");

                        auto bad=form;bad.q(ground,base+0x1c85628);require(!inspect(bad),"slot0 does not identify Shadow Walk without exact class");
                        bad=form;bad.q(machine+0x28,alternate);require(!inspect(bad),"Shadow Walk form cannot masquerade as default native mode");
                        bad=form;bad.q(pc+0xc0,machine+8);require(!inspect(bad),"selected power must refer to same native state machine");
                        bad=form;bad.d(pc+0xe4,1);require(!inspect(bad),"unobserved intermediate phase rejected");
                        bad=form;bad.d(physics+0x50,1);require(!inspect(bad),"sneaking does not imply active Shadow Walk form");
                        for(unsigned phase:{0u,5u}){auto recovery=form;recovery.d(pc+0xe4,phase);recovery.d(physics+0x50,1);require(inspect(recovery)&&observed.form,"native alternate mode persists briefly through crouched exit");}
                        bad=form;unsigned reads=0;require(!readNativeShadowWalkState([&](auto a,auto b,auto n){const bool ok=bad.read(a,b,n);if(a==pc+0xe4&&++reads==1)bad.d(a,5);return ok;},base,observed),"native phase change during read rejected");
                        bad=form;reads=0;require(!readNativeShadowWalkState([&](auto a,auto b,auto n){const bool ok=bad.read(a,b,n);if(a==pc+0xe4&&++reads==1)bad.q(machine+0x90,mode);return ok;},base,observed),"native mode retirement during read rejected");
                        std::cout<<"PASS: native Shadow Walk phase, alternate mode, exact class, owner and concurrent retirement\n";
                    }

                    {
                        auto d=cast;
                        d.str(strings,"models/powers/possession/possession.inventoryitem");d.str(strings+0x100,"components/characters/player/base/powers/possession/possession.cpntpowerpossession");
                        d.q(pc,base+0x1b531c8);d.q(ped,base+0x1b53088);
                        possessionPartnerIdentityTests(d,base,game);
                        constexpr auto use=inv+0x7b000;d.d(machine+0xb0,2);d.d(active+4,63);d.q(mode+0x10+63*8,use);d.q(use,base+0x1c80ed8);d.d(use+8,63);d.q(use+0x10,machine);
                        const auto inspect=[&](Memory& m){const auto rd=[&](auto a,auto b,auto n){return m.read(a,b,n);};NativePlayerStates st{};return readNativePlayerStates(rd,base,st)&&readNativePlayerPossessionScene(rd,base,st,observedCast);};
                        for(unsigned low=0;low<2;++low)for(unsigned phase=0;phase<2;++phase){auto m=d;
                            m.str(texts+0x200,(std::string("midnight/gameplay/player/base/powers/power_possession_cast_")+(phase?"loop":"in")+".midnightscene").c_str());m.d(scene+0x28,phase?200:333);m.d(instance+0x160,80);
                            if(low){m.d(physics+0x50,1);m.d(physics+0x54,1);m.d(active,6);m.q(mode+0x10+6*8,ground);m.q(ground,base+0x1c84258);m.d(ground+8,6);}
                            require(inspect(m)&&observedCast.variant==26+phase,"Possession observed standing/crouched phase owns native arm/equipment");
                            {
                                auto dual=m;constexpr auto ds=inv+0x80000,di=ds+0x1000,dd=ds+0x2000,dc=ds+0x3000,db=ds+0x4000,dt=ds+0x5000;
                                dual.d(machine+0xb0,3);dual.d(active+8,53);dual.q(mode+0x10+53*8,ds);dual.q(ds,base+0x1c79cf8);dual.d(ds+8,53);dual.q(ds+0x10,machine);dual.d(ds+0x24,220);
                                dual.d(game+0x1f5340,2);dual.q(instances+8,di);dual.q(di,dd);dual.q(di+8,dc);dual.d(di+0x158,220);dual.d(di+0x160,80);dual.d(di+0x27c,3);
                                dual.q(dd,base+0x1affbc8);dual.q(dd+8,dt);dual.str(dt,low?"midnight/gameplay/player/base/combat/sword_equip_sneak.midnightscene":"midnight/gameplay/player/base/combat/sword_equip.midnightscene");
                                dual.q(dc,base+0x1b00198);dual.d(dc+0x28,low?1133:1066);dual.q(dc+0x30,dt+0x100);dual.str(dt+0x100,"Scene_2");dual.q(di+0x10,db);dual.d(di+0x18,2);dual.q(db+8,arm);dual.q(db+0x28,weapon);
                                require(inspect(dual)&&observedCast.swordDraw.variant==(low?3u:1u)&&observedCast.swordDraw.handle==220,"Possession independently owns simultaneous native right-arm draw and left-arm cast");
                                {NativePlayerStates states{};unsigned motion=99;
                                 require(readNativePlayerStates([&](auto a,auto b,auto n){return dual.read(a,b,n);},base,states)&&nativePlayerMotion(states,base,motion)&&motion==low,"simultaneous Possession and sword draw preserve native root motion classification");
                                 auto bad=states;bad.active[2].id=64;require(!nativePlayerGroundPowerDrawStates(bad,base),"power draw motion does not accept another equipment slot");
                                 bad=states;bad.active[1].vtable++;require(!nativePlayerGroundPowerDrawStates(bad,base),"power draw motion requires exact UsePower class");
                                 bad=states;bad.player.current=1-low;require(!nativePlayerGroundPowerDrawStates(bad,base),"power draw motion requires matching physical stance");
                                 bad=states;bad.active[2]=bad.active[1];require(!nativePlayerGroundPowerDrawStates(bad,base),"power draw motion refuses duplicate active states");
                                }
                                auto wrong=dual;wrong.q(db+8,leftArm);require(!inspect(wrong),"Possession sword draw cannot borrow casting arm");
                                {
                                    auto overlap=dual;constexpr auto request=ds+0x7000;
                                    overlap.d(machine+0xb0,4);overlap.d(active+12,65);overlap.q(mode+0x10+65*8,request);overlap.q(request,base+0x1c78a68);overlap.d(request+8,65);overlap.q(request+0x10,machine);
                                    const auto rd=[&](auto a,auto b,auto n){return overlap.read(a,b,n);};NativePlayerStates st{};unsigned motion=99;
                                    require(readNativePlayerStates(rd,base,st)&&nativePlayerMotion(st,base,motion)&&motion==low,"four-state native draw request/cast overlap preserves physical motion");
                                    require(inspect(overlap)&&observedCast.swordDraw.handle==220,"four-state overlap still requires both independently owned scenes");
                                    auto bad=st;bad.active[3].vtable++;require(!nativePlayerGroundPowerDrawStates(bad,base),"four-state motion refuses wrong request class");
                                    bad=st;bad.active[3]=bad.active[2];require(!nativePlayerGroundPowerDrawStates(bad,base),"four-state motion refuses duplicated equip scene");
                                    overlap.q(db+8,leftArm);require(!inspect(overlap),"four-state cast cannot borrow the casting arm for sword draw");overlap.q(db+8,arm);
                                    overlap.d(active+12,64);require(!inspect(overlap),"four-state cast refuses unrelated left equipment slot");
                                }
                                {
                                    auto request=dual;
                                    request.d(active+4,65);request.q(mode+0x10+65*8,use);request.q(use,base+0x1c78a68);request.d(use+8,65);
                                    const auto rd=[&](auto a,auto b,auto n){return request.read(a,b,n);};NativePlayerStates st{};NativePlayerEquipmentScene eq{};unsigned motion=99,action=99;
                                    require(readNativePlayerStates(rd,base,st)&&nativePlayerMotion(st,base,motion)&&motion==low&&nativePlayerShadowAction(st,base,action)&&action==7,"native draw request with owned sword scene preserves physical motion and equipment classification");
                                    require(readNativePlayerEquipmentScene(rd,base,st,eq)&&eq.handle==220&&eq.variant==(low?3u:1u),"precast native draw request resolves exact right arm and sword scene");
                                    require(!inspect(request),"draw request does not invent a Possession cast");
                                    auto bad=st;bad.active[1].vtable++;require(!nativePlayerGroundDrawRequestStates(bad,base),"draw request requires exact native class");
                                    bad=st;bad.active[2].id=64;require(!nativePlayerGroundDrawRequestStates(bad,base),"draw request refuses different equipment slot");
                                    bad=st;bad.active[2]=bad.active[1];require(!nativePlayerGroundDrawRequestStates(bad,base),"draw request refuses duplicate state");
                                    bad=st;bad.player.current=1-low;require(!nativePlayerGroundDrawRequestStates(bad,base),"draw request requires matching native stance");
                                    request.q(db+8,leftArm);require(!readNativePlayerEquipmentScene(rd,base,st,eq),"draw request cannot borrow casting arm");request.q(db+8,arm);
                                    request.d(dc+0x28,low?1066:1133);require(!readNativePlayerEquipmentScene(rd,base,st,eq),"draw request requires observed duration");request.d(dc+0x28,low?1133:1066);
                                    unsigned reads=0;require(!readNativePlayerEquipmentScene([&](auto a,auto b,auto n){const bool ok=request.read(a,b,n);if(a==ds+0x24&&++reads==1)request.d(a,221);return ok;},base,st,eq),"draw request refuses concurrent scene rebinding");
                                }
                                wrong=dual;wrong.q(db+0x28,entity);require(!inspect(wrong),"Possession power equipment cannot replace drawn sword");
                                wrong=dual;wrong.d(ds+0x24,219);require(!inspect(wrong),"Possession draw cannot alias casting scene");
                                wrong=dual;wrong.d(dc+0x28,low?1066:1133);require(!inspect(wrong),"Possession draw requires observed stance-specific duration");
                                wrong=dual;wrong.d(di+0x27c,5);require(!inspect(wrong),"Possession does not accept retired draw overlay");
                                wrong=dual;wrong.d(di+0x160,2000);require(!inspect(wrong),"Possession draw cannot outlive its native clip");
                                const auto rd=[&](auto a,auto b,auto n){return dual.read(a,b,n);};NativePlayerStates st{};require(readNativePlayerStates(rd,base,st),"Possession draw concurrency fixture valid");unsigned reads=0;
                                require(!readNativePlayerPossessionScene([&](auto a,auto b,auto n){const bool ok=dual.read(a,b,n);if(a==ds+0x24&&++reads==1)dual.d(a,221);return ok;},base,st,observedCast),"Possession refuses concurrently rebound sword draw");
                            }
                            auto bad=m;bad.q(ground,base+(low?0x1c85628:0x1c84258));require(!inspect(bad),"Possession refuses locomotion ID/class mismatch");
                            bad=m;bad.d(leftArm+0xac,2);require(!inspect(bad),"Possession refuses foreign arm");
                            bad=m;bad.d(instance+0x18,0);require(!inspect(bad),"Possession refuses incomplete native binding");
                            bad=m;bad.d(scene+0x28,phase?201:334);require(!inspect(bad),"Possession refuses unknown scene timing");
                            bad=m;bad.d(machine+0xb0,1);require(!inspect(bad),"Possession aim requires UsePower63");
                            bad=m;bad.d(instance+0x160,334);require(inspect(bad)==bool(phase),"Only native aim loop may exceed one cycle");
                            bad=m;const auto rd=[&](auto a,auto b,auto n){return bad.read(a,b,n);};NativePlayerStates st{};require(readNativePlayerStates(rd,base,st),"Possession concurrent fixture valid");unsigned reads=0;
                            require(!readNativePlayerPossessionScene([&](auto a,auto b,auto n){const bool ok=bad.read(a,b,n);if(a==instance+0x158&&++reads==1)bad.d(a,220);return ok;},base,st,observedCast),"Possession refuses recycled scene incarnation");
                        }
                        std::cout<<"PASS: Possession standing/crouched phases, arm/equipment, incomplete binding and concurrent retirement guards\n";
                    }
                    {
                        auto d=cast;
                        d.str(strings,"models/powers/devouring_swarm/devouring_swarm.inventoryitem");d.str(strings+0x100,"components/characters/player/base/powers/devouring_swarm/power.cpntpowerdevouringswarm");
                        d.q(pc,base+0x1b51f28);d.q(ped,base+0x1b51de8);d.d(instance+0x18,2);d.q(bindings+0x28,actor);
                        constexpr auto use=inv+0x7b000;d.d(machine+0xb0,2);d.d(active+4,63);d.q(mode+0x10+63*8,use);d.q(use,base+0x1c80ed8);d.d(use+8,63);d.q(use+0x10,machine);
                        const auto inspect=[&](Memory& m){const auto rd=[&](auto a,auto b,auto n){return m.read(a,b,n);};NativePlayerStates st{};return readNativePlayerStates(rd,base,st)&&readNativePlayerSwarmScene(rd,base,st,observedCast);};
                        const char* parts[]={"in","loop","out"};const unsigned durations[]={400,333,1600};
                        for(unsigned k=0;k<3;++k){
                            d.str(texts+0x200,(std::string("midnight/gameplay/player/base/powers/power_swarm_cast_")+parts[k]+".midnightscene").c_str());d.d(scene+0x28,durations[k]);d.d(instance+0x160,80);
                            require(inspect(d)&&observedCast.variant==23+k&&observedCast.arm.character==1,"Swarm phase requires exact duration, Corvo body and owned left arm");
                            for(unsigned low=0;low<2;++low)for(unsigned empty=0;empty<2;++empty){
                                auto context=d;
                                if(low){context.d(physics+0x50,1);context.d(physics+0x54,1);context.d(active,6);context.q(mode+0x10+6*8,ground);context.q(ground,base+0x1c84258);context.d(ground+8,6);}
                                if(empty){context.q(inv+0x68,0);context.d(cpnt+0x1c,0x20000002);context.q(frame+0x3c8,actor);context.q(frame+0x3c0,names+0x200);context.d(frame+0x3d0,2);}
                                require(inspect(context)&&observedCast.variant==23+k&&observedCast.power.player.sword.equipped==!empty,"observed Swarm stance/equipment matrix retains phase and matching hand state");
                                auto bad=context;bad.d(leftArm+0xac,2);require(!inspect(bad),"all Swarm contexts reject foreign arms");
                                bad=context;bad.d(machine+0xb0,1);require(inspect(bad)==(k==2),"only Swarm release may recover on exact standing/crouched ground state alone");
                                if(low){bad=context;bad.q(ground,base+0x1c85628);require(!inspect(bad),"crouched Swarm cannot use Running vtable with Sneaking ID");}
                            }

                            auto bad=d;bad.d(scene+0x28,durations[k]+1);require(!inspect(bad),"Swarm wrong duration refused");
                            bad=d;bad.q(bindings+0x28,leftArm);require(!inspect(bad),"Swarm duplicate arm cannot replace body");
                            bad=d;bad.d(leftArm+0xac,2);require(!inspect(bad),"Swarm foreign arm refused");
                            bad=d;bad.d(instance+0x18,1);require(!inspect(bad),"Swarm requires two bindings");
                            bad=d;bad.q(use,base+0x1c3eeb8);require(!inspect(bad),"Swarm requires exact UsePower63 class");
                            bad=d;bad.d(instance+0x27c,5);require(!inspect(bad),"retired Swarm cannot continue posing");
                            bad=d;bad.str(texts+0x300,"Scene_1_unknown");require(!inspect(bad),"Swarm subscene prefix is not authority");
                            bad=d;bad.d(physics+0x50,1);require(!inspect(bad),"unobserved crouched Swarm refused");
                            bad=d;bad.q(inv+0x68,0);bad.d(cpnt+0x1c,0x20000002);bad.q(frame+0x3c8,actor);bad.q(frame+0x3c0,names+0x200);bad.d(frame+0x3d0,2);require(inspect(bad)&&!observedCast.power.player.sword.equipped,"v273 native holstered Swarm preserves exact ownership");
                            bad=d;bad.d(machine+0xb0,1);require(inspect(bad)==(k==2),"only observed Swarm release may recover on Running0 alone");
                            bad=d;bad.d(instance+0x160,durations[k]+1);require(inspect(bad)==(k==1),"only native Swarm loop can exceed its cycle clock");
                            bad=d;NativePlayerStates st{};const auto rd=[&](auto a,auto b,auto n){return bad.read(a,b,n);};require(readNativePlayerStates(rd,base,st),"Swarm race fixture state valid");unsigned reads=0;
                            require(!readNativePlayerSwarmScene([&](auto a,auto b,auto n){const bool ok=bad.read(a,b,n);if(a==instance+0x158&&++reads==1)bad.d(a,220);return ok;},base,st,observedCast),"recycled Swarm scene incarnation refused");
                        }
                        std::cout<<"PASS: Swarm exact native phases, body/arm ownership, equipment, state and concurrent retirement guards\n";
                    }
                    {
                        auto d=cast;
                        d.str(names+0x400,"models/characters/player/emily/body.def");d.str(names+0x600,"models/characters/player/emily/right_arm.def");d.str(texts+0x100,"models/characters/player/emily/left_arm.def");
                        d.str(strings,"models/powers/doppelganger/doppelganger.inventoryitem");d.str(strings+0x100,"components/characters/player/base/powers/doppelganger/doppelganger_power.cpntpowerdoppelganger");
                        d.q(pc,base+0x1b52258);d.q(ped,base+0x1b52118);d.d(instance+0x18,1);
                        constexpr auto use=inv+0x7b000;d.d(machine+0xb0,2);d.d(active+4,63);d.q(mode+0x10+63*8,use);d.q(use,base+0x1c80ed8);d.d(use+8,63);d.q(use+0x10,machine);
                        const auto inspect=[&](Memory& m){const auto rd=[&](auto a,auto b,auto n){return m.read(a,b,n);};NativePlayerStates st{};return readNativePlayerStates(rd,base,st)&&readNativePlayerDoppelgangerScene(rd,base,st,observedCast);};
                        const char* parts[]={"in","loop","out"};const unsigned durations[]={466,1200,666};
                        for(unsigned k=0;k<3;++k){
                            d.str(texts+0x200,(std::string("midnight/gameplay/player/base/powers/power_decoy_cast_")+parts[k]+".midnightscene").c_str());d.d(scene+0x28,durations[k]);d.d(instance+0x160,80);
                            require(inspect(d)&&observedCast.variant==29+k&&observedCast.arm.character==2,"Doppelganger phase requires exact duration, Emily body and owned left arm");
                            auto bad=d;bad.d(scene+0x28,durations[k]+1);require(!inspect(bad),"Doppelganger wrong duration refused");
                            bad=d;bad.q(bindings+8,actor);require(!inspect(bad),"Doppelganger body cannot impersonate casting arm");
                            bad=d;bad.d(leftArm+0xac,2);require(!inspect(bad),"Doppelganger foreign arm refused");
                            bad=d;bad.d(instance+0x18,2);require(!inspect(bad),"Doppelganger requires one binding");
                            bad=d;bad.q(use,base+0x1c3eeb8);require(!inspect(bad),"Doppelganger requires exact UsePower63 class");
                            bad=d;bad.d(instance+0x27c,5);require(!inspect(bad),"retired Doppelganger cannot continue posing");
                            bad=d;bad.str(texts+0x300,"Scene_1_unknown");require(!inspect(bad),"Doppelganger subscene prefix is not authority");
                            bad=d;bad.d(physics+0x50,1);require(!inspect(bad),"Doppelganger stance must match its exact locomotion state");
                            auto low=bad;low.d(active,6);low.q(mode+0x10+6*8,ground);low.q(ground,base+0x1c84258);low.d(ground+8,6);
                            require(inspect(low)&&observedCast.variant==29+k,"489 crouched Doppelganger retains exact owned phase and native timing");
                            bad=low;bad.d(physics+0x50,0);require(!inspect(bad),"standing physics cannot claim Sneaking6");
                            bad=low;bad.q(ground,base+0x1c85628);require(!inspect(bad),"crouched Doppelganger requires exact Sneaking class");
                            bad=low;bad.d(machine+0xb0,1);require(inspect(bad)==(k==2),"only release may recover on Sneaking6 without UsePower");
                            for(auto context:{d,low}){
                                context.q(inv+0x68,0);context.d(cpnt+0x1c,0x20000002);context.q(frame+0x3c8,actor);context.q(frame+0x3c0,names+0x200);context.d(frame+0x3d0,2);
                                require(inspect(context)&&!observedCast.power.player.sword.equipped,"493/494 native holstered ground contexts retain exact owned phases");
                                bad=context;bad.d(machine+0xb0,1);require(inspect(bad)==(k==2),"holstered recovery accepts only release");
                                bad=context;bad.q(frame+0x3c8,leftArm);require(!inspect(bad),"holstered sword attachment cannot change parent");
                                bad=context;bad.q(inv+0x70,0);require(!inspect(bad),"holstered cast still needs selected power slot");
                            }
                            for(unsigned motion:{2u,3u})for(bool empty:{false,true}){
                                auto air=d;air.d(active,motion);air.q(mode+0x10+motion*8,ground);air.q(ground,base+(motion==2?0x1c85028:0x1c845c8));air.d(ground+8,motion);
                                if(empty){air.q(inv+0x68,0);air.d(cpnt+0x1c,0x20000002);air.q(frame+0x3c8,actor);air.q(frame+0x3c0,names+0x200);air.d(frame+0x3d0,2);}
                                require(inspect(air)&&observedCast.variant==29+k&&observedCast.power.player.sword.equipped==!empty,"airborne Doppelganger retains owned phase and native equipment");
                                bad=air;bad.d(machine+0xb0,1);require(inspect(bad)==(k==2),"only airborne release may outlive UsePower");
                                bad=air;bad.d(physics+0x50,1);require(!inspect(bad),"Sneaking physics cannot claim Jump or Fall");
                                bad=air;bad.q(ground,base+0x1c85628);require(!inspect(bad),"airborne ID needs exact native Jump/Fall class");
                                bad=air;bad.d(leftArm+0xac,2);require(!inspect(bad),"airborne cast rejects foreign arm");
                                bad=air;bad.d(machine+0xb0,3);bad.d(active+8,0);require(!inspect(bad),"airborne cast cannot duplicate locomotion state");
                            }
                            bad=d;bad.d(machine+0xb0,1);require(inspect(bad)==(k==2),"only observed Doppelganger release may recover on Running0 alone");
                            bad=d;bad.d(instance+0x160,durations[k]+1);require(inspect(bad)==(k==1),"only native Doppelganger loop can exceed its cycle clock");
                            bad=d;NativePlayerStates st{};const auto rd=[&](auto a,auto b,auto n){return bad.read(a,b,n);};require(readNativePlayerStates(rd,base,st),"Doppelganger race fixture state valid");unsigned reads=0;
                            require(!readNativePlayerDoppelgangerScene([&](auto a,auto b,auto n){const bool ok=bad.read(a,b,n);if(a==instance+0x158&&++reads==1)bad.d(a,220);return ok;},base,st,observedCast),"recycled Doppelganger scene incarnation refused");
                        }
                        std::cout<<"PASS: Doppelganger exact native phases, body/arm ownership, equipment, state and concurrent retirement guards\n";
                    }
                    {
                        auto d=cast;
                        d.str(names+0x400,"models/characters/player/emily/body.def");d.str(names+0x600,"models/characters/player/emily/right_arm.def");d.str(texts+0x100,"models/characters/player/emily/left_arm.def");
                        d.str(strings,"models/powers/phantasmallink/phantasmal_link.inventoryitem");d.str(strings+0x100,"components/characters/player/base/powers/phantasmallink/phantasmal_link.cpntpowerphantasmallink");
                        d.q(pc,base+0x1b52ec8);d.q(ped,base+0x1b52d88);d.str(texts+0x200,"midnight/gameplay/player/base/powers/power_phantasmal_cast.midnightscene");
                        d.d(instance+0x18,2);d.q(bindings+0x28,actor);
                        constexpr auto overlayState=inv+0x7b000;d.d(machine+0xb0,2);d.d(active+4,72);d.q(mode+0x10+72*8,overlayState);d.q(overlayState,base+0x1c3eeb8);d.d(overlayState+8,72);d.q(overlayState+0x10,machine);
                        const auto inspect=[&](Memory& m){const auto rd=[&](auto a,auto b,auto n){return m.read(a,b,n);};NativePlayerStates st{};return readNativePlayerStates(rd,base,st)&&readNativePlayerDominoScene(rd,base,st,observedCast);};
                        const char* subs[]={"Cast_In","Cast_Loop","Cast_Select","Cast_Select_out","Cast_final_Select"};const unsigned durations[]={266,1333,400,266,400},variants[]={19,20,21,22,21};
                        for(unsigned k=0;k<5;++k){d.str(texts+0x300,subs[k]);d.d(scene+0x28,durations[k]);
                            require(inspect(d)&&observedCast.variant==variants[k]&&observedCast.arm.side==1,"observed native Domino phase requires exact scene, duration, player body and owned left arm");
                            auto bad=d;bad.d(scene+0x28,durations[k]+1);require(!inspect(bad),"Domino phase duration mismatch refused");
                            bad=d;bad.q(bindings+0x28,leftArm);require(!inspect(bad),"duplicate arm cannot replace Domino player body");
                            bad=d;bad.d(instance+0x18,1);require(!inspect(bad),"Domino requires both native bindings");
                            bad=d;bad.q(overlayState,base+0x1c80ed8);require(!inspect(bad),"generic UsePower cannot impersonate Domino state72");
                            bad=d;bad.d(physics+0x50,1);require(!inspect(bad),"Domino stance/state mismatch refused");
                            bad=d;bad.d(instance+0x27c,5);require(!inspect(bad),"retired Domino scene refused");
                            bad=d;unsigned reads=0;require(!readNativePlayerDominoScene([&](auto a,auto b,auto n){return bad.read(a,b,n);},base,NativePlayerStates{},observedCast),"missing source states refused");
                            const auto rd=[&](auto a,auto b,auto n){return bad.read(a,b,n);};NativePlayerStates st{};require(readNativePlayerStates(rd,base,st),"Domino race fixture states valid");
                            unsigned motion=99;require(nativePlayerMotion(st,base,motion)&&motion==0,"Domino state72 preserves native grounded root presentation");
                            auto reversed=st;std::reverse(reversed.active.begin(),reversed.active.end());require(nativePlayerMotion(reversed,base,motion)&&motion==0,"Domino root classification does not depend on active-state order");
                            auto unknown=st;unknown.active.back().vtable=base+0x1c80ed8;require(!nativePlayerMotion(unknown,base,motion),"wrong state72 class cannot authorize root motion");
                            unknown=st;unknown.player.current=1;require(!nativePlayerMotion(unknown,base,motion),"crouched Domino stance cannot retain Running0 instead of Sneaking6");
                            require(!readNativePlayerDominoScene([&](auto a,auto b,auto n){const bool ok=bad.read(a,b,n);if(a==instance+0x158&&++reads==1)bad.d(a,220);return ok;},base,st,observedCast),"recycled Domino scene incarnation refused");
                        }
                        require(!nativeDominoVariant("midnight/gameplay/player/base/powers/power_phantasmal_cast.midnightscene","Cast_fire",500),"unobserved final-link gesture remains unsupported");
                        auto ended=d;ended.d(machine+0xb0,1);require(inspect(ended)&&observedCast.variant==21,"observed exact final selection recovery retains owned scene after native state72 retires");
                        auto invalidRecovery=ended;invalidRecovery.str(texts+0x300,"Cast_Select");require(!inspect(invalidRecovery),"ordinary selection without native state72 remains refused");
                        invalidRecovery=ended;invalidRecovery.d(instance+0x160,401);require(!inspect(invalidRecovery),"final selection clock beyond exact duration refused");
                        invalidRecovery=ended;invalidRecovery.d(instance+0x27c,5);require(!inspect(invalidRecovery),"retired final selection scene cannot keep playing during grounded recovery");
                        invalidRecovery=ended;invalidRecovery.q(bindings+0x28,leftArm);require(!inspect(invalidRecovery),"final recovery still requires reciprocal body and arm bindings");
                        require(!nativeDominoVariant("midnight/gameplay/player/base/powers/power_phantasmal_cast.midnightscene","Cast_final_Select_unknown",400),"final-select prefix does not authorize unknown subscenes");
                        auto low=d;constexpr auto sneakingState=inv+0x7b100;low.d(physics+0x50,1);low.d(active,6);low.q(mode+0x10+6*8,sneakingState);low.q(sneakingState,base+0x1c84258);low.d(sneakingState+8,6);low.q(sneakingState+0x10,machine);
                        for(unsigned k:{0u,1u,2u,3u,4u}){
                            low.str(texts+0x300,subs[k]);low.d(scene+0x28,durations[k]);
                            require(inspect(low)&&observedCast.variant==variants[k],"observed crouched Domino phase requires matching Sneaking6 and dedicated casting72");
                            NativePlayerStates st{};const auto rd=[&](auto a,auto b,auto n){return low.read(a,b,n);};unsigned motion=99;
                            require(readNativePlayerStates(rd,base,st)&&nativePlayerMotion(st,base,motion)&&motion==1,"crouched Domino keeps native crouched root motion");
                            auto bad=low;bad.d(physics+0x50,0);require(!inspect(bad),"standing stance cannot reuse crouched Domino state");
                            bad=low;bad.q(sneakingState,base+0x1c85628);require(!inspect(bad),"wrong native sneaking class cannot authorize crouched cast");
                        }
                        low.str(texts+0x300,"Cast_Select_out");low.d(scene+0x28,266);low.d(machine+0xb0,1);require(inspect(low)&&observedCast.variant==22,"observed crouched native exit recovers on Sneaking6 alone");
                        low.str(texts+0x300,"Cast_Select");low.d(scene+0x28,400);low.d(machine+0xb0,1);require(!inspect(low),"ordinary crouched selection still requires native casting72");
                        low.str(texts+0x300,"Cast_final_Select");low.d(instance+0x160,375);require(inspect(low)&&observedCast.variant==21,"observed crouched final scene recovers on exact Sneaking6 after casting72 retires");
                        auto lowBad=low;lowBad.d(instance+0x160,401);require(!inspect(lowBad),"crouched final recovery rejects time beyond native duration");
                        lowBad=low;lowBad.d(instance+0x27c,5);require(!inspect(lowBad),"crouched final recovery rejects retired scene");
                        lowBad=low;lowBad.q(bindings+0x28,leftArm);require(!inspect(lowBad),"crouched final recovery requires reciprocal body as well as arm");
                        for(unsigned stance=0;stance<2;++stance){
                            auto empty=stance?low:d;empty.d(machine+0xb0,2);empty.q(inv+0x68,0);empty.d(cpnt+0x1c,0x20000002);empty.q(frame+0x3c8,actor);empty.q(frame+0x3c0,names+0x200);empty.d(frame+0x3d0,2);empty.d(instance+0x160,80);
                            for(unsigned k=0;k<4;++k){empty.str(texts+0x300,subs[k]);empty.d(scene+0x28,durations[k]);
                                require(inspect(empty)&&observedCast.variant==variants[k]&&!observedCast.power.player.sword.equipped,"native holstered ordinary Domino phase retains exact stance, equipment and arm/body identity");
                                auto bad=empty;bad.d(leftArm+0xac,2);require(!inspect(bad),"holstered Domino rejects foreign arm");
                                bad=empty;bad.d(instance+0x27c,5);require(!inspect(bad),"holstered Domino rejects retired scene");
                            }
                            empty.str(texts+0x300,"Cast_Select");empty.d(scene+0x28,400);empty.d(machine+0xb0,1);require(!inspect(empty),"holstered ordinary selection requires active casting72");
                            empty.str(texts+0x300,"Cast_final_Select");empty.d(machine+0xb0,2);require(inspect(empty)&&observedCast.variant==21&&!observedCast.power.player.sword.equipped,"v263 holstered final selection retains native cast and exact empty-hand context");
                            empty.d(machine+0xb0,1);empty.d(instance+0x160,375);require(inspect(empty)&&observedCast.variant==21,"holstered final selection recovers on matching ground state through exact400ms owned scene");
                            auto bad=empty;bad.d(instance+0x160,401);require(!inspect(bad),"holstered final recovery rejects out-of-duration clock");
                            bad=empty;bad.d(instance+0x27c,5);require(!inspect(bad),"holstered final recovery rejects retired scene");
                            bad=empty;bad.q(bindings+0x28,leftArm);require(!inspect(bad),"holstered final recovery still requires reciprocal player body");
                            bad=empty;bad.d(leftArm+0xac,2);require(!inspect(bad),"holstered final recovery rejects foreign arm");
                        }
                        std::cout<<"PASS: Domino five observed subscenes, exact state72, arm/body ownership, duration and scene incarnation guards\n";
                    }
                    {
                        auto mesmer=cast;
                        mesmer.str(names+0x400,"models/characters/player/emily/body.def");mesmer.str(names+0x600,"models/characters/player/emily/right_arm.def");mesmer.str(texts+0x100,"models/characters/player/emily/left_arm.def");
                        mesmer.str(strings,"models/powers/mesmerize/mesmerize.inventoryitem");mesmer.str(strings+0x100,"components/characters/player/base/powers/mesmerize/mesmerize.cpntpowermesmerize");
                        mesmer.q(pc,base+0x1b9bea8);mesmer.q(ped,base+0x1b9bd68);
                        const auto inspect=[&](Memory& mem,bool allow=true){const auto rd=[&](auto a,auto d,auto n){return mem.read(a,d,n);};NativePlayerStates st{};return readNativePlayerStates(rd,base,st)&&readNativePlayerPowerScene(rd,base,st,observedCast,allow);};
                        const char* phases[]={"in","loop","out"};const unsigned durations[]={466,1200,666};
                        for(unsigned k=0;k<3;++k){
                            auto m=mesmer;m.str(texts+0x200,(std::string("midnight/gameplay/player/base/powers/power_mesmerize_cast_")+phases[k]+".midnightscene").c_str());m.d(scene+0x28,durations[k]);
                            require(inspect(m)&&observedCast.variant==16+k&&observedCast.arm.character==2&&observedCast.arm.side==1,"Mesmerize exact phase binds only the native owned Emily left arm");
                            require(!inspect(m,false),"Mesmerize remains opt-in until production transport/playback validation");
                            auto bad=m;bad.q(bindings+8,actor);require(!inspect(bad),"body cannot impersonate Mesmerize first-person arm binding");
                            bad=m;bad.q(bindings+8,entity);require(!inspect(bad),"Mesmerize equipment cannot impersonate arm binding");
                            bad=m;bad.d(leftArm+0xac,2);require(!inspect(bad),"foreign Mesmerize arm refused");
                            bad=m;bad.d(scene+0x28,durations[k]+1);require(!inspect(bad),"unobserved Mesmerize scene duration refused");
                            bad=m;bad.d(instance+0x18,2);require(!inspect(bad),"extra Mesmerize binding refused");
                            bad=m;bad.d(instance+0x27c,5);require(!inspect(bad),"retired Mesmerize scene refused");
                            bad=m;bad.d(physics+0x50,1);require(!inspect(bad),"Mesmerize crouch physics with Running state remains refused");
                            auto low=m;low.d(physics+0x50,1);low.d(active,6);low.q(mode+0x10+6*8,ground);low.d(ground+8,6);low.q(ground,base+0x1c84258);
                            require(inspect(low)&&observedCast.variant==16+k,"observed crouched Mesmerize keeps exact Emily arm and native phase");
                            for(unsigned id:{2u,3u}){
                                auto air=m;air.d(active,id);air.q(mode+0x10+id*8,ground);air.d(ground+8,id);air.q(ground,base+(id==2?0x1c85028:0x1c845c8));
                                require(inspect(air)&&observedCast.variant==16+k,"armed native airborne Mesmerize retains exact phase ownership");
                                constexpr auto use=inv+0x4d000;air.d(machine+0xb0,2);air.d(active+4,63);air.q(mode+0x10+63*8,use);air.q(use,base+0x1c80ed8);air.d(use+8,63);air.q(use+0x10,machine);
                                require(inspect(air),"airborne Mesmerize accepts observed UsePower overlay");
                                auto wrong=air;wrong.d(physics+0x50,1);require(!inspect(wrong),"airborne Mesmerize rejects crouched physics");
                                wrong=air;wrong.q(bindings+8,arm);require(!inspect(wrong),"airborne Mesmerize rejects sword arm binding");
                                wrong=air;wrong.q(inv+0x68,0);wrong.d(cpnt+0x1c,0x20000002);wrong.q(frame+0x3c8,actor);wrong.q(frame+0x3c0,names+0x200);wrong.d(frame+0x3d0,2);
                                require(inspect(wrong)&&observedCast.variant==16+k,"v175/v177 native holstered airborne Mesmerize retains exact scene and arm ownership");
                                wrong.d(leftArm+0xac,2);require(!inspect(wrong),"holstered airborne cast still rejects a foreign left arm");
                            }
                            bad=low;bad.d(physics+0x50,0);require(!inspect(bad),"standing physics cannot impersonate native Sneaking Mesmerize");
                            bad=low;bad.d(physics+0x50,2);require(!inspect(bad),"unobserved Mesmerize stance remains refused");
                            bad=m;bad.d(pe+0x20,0);require(!inspect(bad),"unowned Mesmerize cannot source scene");
                            bad=m;unsigned reads=0;NativePlayerStates st{};require(readNativePlayerStates([&](auto a,auto d,auto n){return bad.read(a,d,n);},base,st),"Mesmerize concurrent test initial state");
                            require(!readNativePlayerPowerScene([&](auto a,auto d,auto n){const bool ok=bad.read(a,d,n);if(a==instance+0x158&&++reads==1)bad.d(a,220);return ok;},base,st,observedCast,true),"recycled Mesmerize scene incarnation refused");
                        }
                        {
                            // 1214dk: Emily's failed Mesmerize cast (1390a): power_outofmana, Scene_1, 333 ms, only her left arm bound.
                            auto fail=mesmer;fail.str(texts+0x200,"midnight/gameplay/player/base/powers/power_outofmana.midnightscene");fail.d(scene+0x28,333);
                            require(inspect(fail)&&observedCast.variant==15&&observedCast.arm.character==2&&observedCast.arm.side==1,"Emily's out-of-mana response binds only her owned left arm");
                            auto bad=fail;bad.d(scene+0x28,334);require(!inspect(bad),"Emily's failure requires the native scene duration");
                            bad=fail;bad.d(instance+0x18,2);require(!inspect(bad),"Emily's failure refuses an extra binding");
                            bad=fail;bad.q(bindings+8,actor);require(!inspect(bad),"Emily's failure refuses a body binding");
                            auto low=fail;low.d(physics+0x50,1);low.d(active,6);low.q(mode+0x10+6*8,ground);low.d(ground+8,6);low.q(ground,base+0x1c84258);
                            require(inspect(low)&&observedCast.variant==15,"Emily's crouched failure keeps the exact arm");
                            auto air=fail;air.d(active,3);air.q(mode+0x10+3*8,ground);air.d(ground+8,3);air.q(ground,base+0x1c845c8);
                            require(!inspect(air),"Emily's airborne failure stays refused");
                        }
                        std::cout<<"PASS: Mesmerize three exact phase identities, reciprocal Emily arm, scope, stale scene and concurrent incarnation guards\n";
                    }
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
                    {
                        auto bend=cast;bend.str(strings,"models/powers/bendtime/bendtime.inventoryitem");bend.str(strings+0x100,"components/characters/player/base/powers/bendtime/bendtime.cpntpowerbendtime");bend.q(pc,base+0x1b9b748);bend.q(ped,base+0x1b9b608);const unsigned char inactive=0;bend.put(pc+0xd0,&inactive,1);
                        bend.str(texts+0x200,"midnight/gameplay/player/base/powers/power_bendtime_cast_in_test.midnightscene");bend.str(texts+0x300,"Scene_1");bend.d(scene+0x28,533);bend.d(instance+0x18,2);bend.q(bindings+0x28,actor);
                        require(inspectCast(bend)&&observedCast.variant==12,"Bend Time starts with exact owned left arm plus native player binding");
                        for(unsigned id:{2u,3u}){
                            auto flying=bend;flying.d(active,id);flying.q(mode+0x10+id*8,ground);flying.d(ground+8,id);flying.q(ground,base+(id==2?0x1c85028:0x1c845c8));
                            const float apex[2]={67.f,68.5f};flying.put(ground+0xa0,apex,sizeof(apex));flying.d(ground+0x94,0);
                            require(inspectCast(flying)&&observedCast.variant==12&&observedCast.handle==219,"observed airborne Bend Time retains exact scene ownership");
                            const auto use=inv+0x4d000;flying.d(machine+0xb0,2);flying.d(active+4,63);flying.q(mode+0x10+63*8,use);flying.q(use,base+0x1c80ed8);flying.d(use+8,63);flying.q(use+0x10,machine);
                            require(inspectCast(flying),"Bend Time accepts observed Jump/Fall with UsePower only after exact binding validation");
                            const auto rd=[&](auto p,auto d,auto n){return flying.read(p,d,n);};NativePlayerStates ps{};unsigned stage=0,remaining=0;float start=0,target=0;
                            require(readNativePlayerStates(rd,base,ps)&&nativePlayerAir(rd,ps,base,stage,start,target,remaining,true)&&stage==(id==2?1u:2u),"Bend Time overlay preserves actual native jump/fall metadata");
                            require(!nativePlayerAir(rd,ps,base,stage,start,target,remaining),"an unconfirmed power overlay cannot authorize airborne metadata");
                            auto wrong=flying;wrong.q(bindings+8,arm);require(!inspectCast(wrong),"airborne Bend Time still rejects the wrong native arm");
                            flying.d(physics+0x50,1);require(!inspectCast(flying),"unobserved crouched airborne Bend Time remains refused");
                        }
                        auto low=bend;low.d(physics+0x50,1);low.d(physics+0x54,1);low.d(active,6);low.q(mode+0x10+6*8,ground);low.q(ground,base+0x1c84258);low.d(ground+8,6);
                        low.q(inv+0x68,0);low.d(cpnt+0x1c,0x20000002);low.q(frame+0x3c8,actor);low.q(frame+0x3c0,names+0x200);low.d(frame+0x3d0,2);
                        require(inspectCast(low)&&observedCast.variant==12&&!observedCast.power.player.sword.equipped,"native crouched empty-hand Bend Time retains owned player/left-arm bindings");
                        auto bad=bend;bad.d(instance+0x18,1);require(!inspectCast(bad),"Bend Time start cannot omit player binding");
                        bad=bend;bad.q(bindings+0x28,entity);require(!inspectCast(bad),"Bend Time equipment cannot impersonate player camera actor");
                        bad=bend;bad.str(strings,"models/powers/darkvision/darkvision.inventoryitem");require(!inspectCast(bad),"Bend Time rejects wrong selected power");
                        bad=bend;bad.d(entity+0xa8,handle+1);require(!inspectCast(bad),"Bend Time rejects recycled equipment");
                        bend.str(texts+0x200,"midnight/gameplay/player/base/powers/power_bendtime_cast_out_test.midnightscene");bend.d(scene+0x28,433);bend.d(instance+0x18,1);
                        require(inspectCast(bend)&&observedCast.variant==14,"Bend Time release has only the exact left-arm binding");
                        auto falling=bend;falling.d(active,3);falling.q(mode+0x10+3*8,ground);falling.d(ground+8,3);falling.q(ground,base+0x1c845c8);
                        require(inspectCast(falling)&&observedCast.variant==14,"observed falling Bend Time release keeps only owned left-arm binding");
                        auto empty=bend;empty.str(texts+0x200,"midnight/gameplay/player/base/powers/power_outofmana.midnightscene");empty.d(scene+0x28,333);
                        require(inspectCast(empty)&&observedCast.variant==15,"native out-of-mana response with exact selected power and owned arm");
                        auto invalidEmpty=empty;const unsigned char on=1;invalidEmpty.put(pc+0xd0,&on,1);require(!inspectCast(invalidEmpty),"active power cannot impersonate failed cast");
                        auto lowEmpty=low;lowEmpty.str(texts+0x200,"midnight/gameplay/player/base/powers/power_outofmana.midnightscene");lowEmpty.d(scene+0x28,333);lowEmpty.d(instance+0x18,1);
                        require(inspectCast(lowEmpty)&&observedCast.variant==15&&!observedCast.power.player.sword.equipped,"observed crouched empty-hand failure keeps exact owned arm");
                        invalidEmpty=empty;invalidEmpty.d(physics+0x50,1);require(!inspectCast(invalidEmpty),"failure refuses mismatched stance and active native state");
                        invalidEmpty=falling;invalidEmpty.str(texts+0x200,"midnight/gameplay/player/base/powers/power_outofmana.midnightscene");invalidEmpty.d(scene+0x28,333);require(!inspectCast(invalidEmpty),"unobserved airborne failure remains refused");
                        invalidEmpty=empty;invalidEmpty.q(bindings+8,arm);require(!inspectCast(invalidEmpty),"failed cast rejects wrong arm");
                        invalidEmpty=empty;invalidEmpty.d(scene+0x28,334);require(!inspectCast(invalidEmpty),"failed cast requires native scene duration");
                        bad=bend;bad.d(instance+0x18,2);require(!inspectCast(bad),"Bend Time release rejects extra player binding");
                        bad=bend;bad.d(instance+0x27c,5);require(!inspectCast(bad),"completed Bend Time phase cannot masquerade as active");
                        bad=bend;bad.str(texts+0x200,"midnight/gameplay/player/base/powers/power_bendtime_cast_loop_test.midnightscene");bad.d(scene+0x28,300);require(!inspectCast(bad),"unobserved Bend Time hold remains unbound");
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
                {
                    auto both=other;
                    constexpr auto ps=inv+0x20000,pi=inv+0x21000,pd=inv+0x22000,sc=inv+0x23000,pb=inv+0x24000;
                    constexpr auto text=inv+0x25000,left=inv+0x26000,ld=inv+0x27000;
                    both.d(machine+0xb0,3);both.d(active+8,64);both.q(mode+0x10+64*8,ps);
                    both.q(ps,base+0x1c79cf8);both.d(ps+8,64);both.q(ps+0x10,machine);both.d(ps+0x24,124);
                    both.str(strings,"midnight/gameplay/player/base/combat/sword_unequip.midnightscene");both.d(scene+0x28,866);
                    both.d(game+0x1f5340,2);both.q(instances+8,pi);both.q(pi,pd);both.q(pi+8,sc);both.d(pi+0x158,124);both.d(pi+0x160,150);both.d(pi+0x27c,3);
                    both.q(pd,base+0x1affbc8);both.q(pd+8,text);both.str(text,"midnight/gameplay/player/base/powers/power_unequip.midnightscene");
                    both.q(sc,base+0x1b00198);both.d(sc+0x28,333);both.q(sc+0x30,text+0x100);both.str(text+0x100,"Scene_1");
                    both.q(pi+0x10,pb);both.d(pi+0x18,1);both.q(pb+8,left);
                    both.q(game+0x3130b0+40,left);both.q(left,base+0x1c5e428);both.d(left+0xa8,0x50006);both.d(left+0xac,1);
                    both.q(left+0x20,text+0x200);both.str(text+0x200,"left_arm_1");both.q(left+0x2e8,ld);
                    both.q(ld,base+0x1ab58f8);both.q(ld+8,text+0x300);both.str(text+0x300,"models/characters/player/emily/left_arm.def");
                    both.d(actor+0xb8,2);both.q(playerChildren+8,left);
                    require(inspectEquipment(both)&&result.variant==2,"native simultaneous power and sword holster validates both separate owned scenes");
                    // v279 native Corvo state64/53: left Scene_1/333ms and
                    // right Scene_2/1266ms, distinct reciprocal arm bindings.
                    for(unsigned low=0;low<2;++low){
                        auto corvo=both;
                        corvo.str(names+0x400,"models/characters/player/corvo/body.def");
                        corvo.str(names+0x600,"models/characters/player/corvo/right_arm.def");
                        corvo.str(text+0x300,"models/characters/player/corvo/left_arm.def");
                        corvo.str(strings+0x100,"Scene_2");corvo.d(scene+0x28,1266);
                        if(low){corvo.d(physics+0x50,1);corvo.d(physics+0x54,1);corvo.d(active,6);corvo.q(mode+0x10+6*8,ground);corvo.q(ground,base+0x1c84258);corvo.d(ground+8,6);corvo.str(strings,"midnight/gameplay/player/base/combat/sword_unequip_sneak.midnightscene");}
                        require(inspectEquipment(corvo)&&result.character==1&&result.variant==(low?4u:2u),"Corvo dual holster keeps both owned character-specific arms and separate clocks");
                        auto bad=corvo;bad.str(text+0x300,"models/characters/player/emily/left_arm.def");require(!inspectEquipment(bad),"Corvo dual holster cannot borrow Emily's left arm");
                        bad=corvo;bad.d(ps+0x24,123);require(!inspectEquipment(bad),"Corvo left-power scene cannot alias sword scene");
                        bad=corvo;bad.d(left+0xac,2);require(!inspectEquipment(bad),"Corvo dual holster rejects foreign arm owner");
                        bad=corvo;bad.d(pi+0x160,334);require(!inspectEquipment(bad),"Corvo dual holster refuses expired power clock");
                        bad=corvo;bad.d(pi+0x27c,5);require(!inspectEquipment(bad),"Corvo dual holster refuses retired power scene");
                        for(auto location:{ps+0x24,pb+8,left+0xac,playerChildren+8}){
                            bad=corvo;unsigned n=0;NativePlayerStates ns{};
                            require(readNativePlayerStates([&](auto p,auto d,auto sz){return corvo.read(p,d,sz);},base,ns),"Corvo dual holster race setup");
                            require(!readNativePlayerEquipmentScene([&](auto p,auto d,auto sz){const bool ok=bad.read(p,d,sz);if(p==location&&++n==1)bad.q(p,0);return ok;},base,ns,result),"Corvo dual holster rejects concurrent scene/arm retirement");
                        }
                    }
                    {
                        auto draw=both;
                        draw.str(names+0x400,"models/characters/player/corvo/body.def");
                        draw.str(names+0x600,"models/characters/player/corvo/right_arm.def");
                        draw.str(text+0x300,"models/characters/player/corvo/left_arm.def");
                        draw.str(strings+0x100,"Scene_2");draw.d(scene+0x28,1066);
                        draw.str(strings,"midnight/gameplay/player/base/combat/sword_equip.midnightscene");
                        draw.str(text,"midnight/gameplay/player/base/powers/power_equip.midnightscene");
                        require(inspectEquipment(draw)&&result.variant==1&&result.character==1,"Corvo native possession exit validates simultaneous sword1066 and power333 draw");
                        auto bad=draw;bad.d(scene+0x28,1067);require(!inspectEquipment(bad),"dual draw rejects unobserved sword duration");
                        bad=draw;bad.str(text,"midnight/gameplay/player/base/powers/power_unequip.midnightscene");require(!inspectEquipment(bad),"power holster cannot authorize a sword draw");
                        bad=draw;bad.q(pb+8,arm);require(!inspectEquipment(bad),"dual draw requires its own left arm");
                        bad=draw;bad.d(sc+0x28,334);require(!inspectEquipment(bad),"dual draw requires exact left scene333");
                        bad=draw;bad.d(pi+0x160,334);require(!inspectEquipment(bad),"dual draw rejects expired left clock");
                        bad=draw;bad.d(ps+0x24,123);require(!inspectEquipment(bad),"dual draw refuses aliased scene ownership");
                        bad=draw;bad.str(names+0x400,"models/characters/player/emily/body.def");bad.str(names+0x600,"models/characters/player/emily/right_arm.def");bad.str(text+0x300,"models/characters/player/emily/left_arm.def");bad.str(strings+0x100,"Scene_1");require(!inspectEquipment(bad),"unobserved Emily dual draw remains refused");
                        bad=draw;bad.d(physics+0x50,1);bad.d(physics+0x54,1);bad.d(active,6);bad.q(mode+0x10+6*8,ground);bad.q(ground,base+0x1c84258);bad.d(ground+8,6);require(!inspectEquipment(bad),"standing draw declaration cannot impersonate crouched exit");
                        bad.str(strings,"midnight/gameplay/player/base/combat/sword_equip_sneak.midnightscene");bad.d(scene+0x28,1133);
                        require(inspectEquipment(bad)&&result.variant==3,"crouched Corvo exit validates independent sword1133 and power333");
                        auto low=bad;low.d(scene+0x28,1066);require(!inspectEquipment(low),"crouched exit refuses standing timing");
                        low=bad;low.q(pb+8,arm);require(!inspectEquipment(low),"crouched exit refuses borrowed right arm");
                        low=bad;low.d(ps+0x24,123);require(!inspectEquipment(low),"crouched exit refuses aliased power scene");
                        for(auto location:{ps+0x24,pi+8,pb+8,left+0xac,playerChildren+8}){
                            bad=draw;unsigned n=0;NativePlayerStates ns{};
                            require(readNativePlayerStates([&](auto p,auto d,auto sz){return draw.read(p,d,sz);},base,ns),"dual draw concurrency fixture");
                            require(!readNativePlayerEquipmentScene([&](auto p,auto d,auto sz){const bool ok=bad.read(p,d,sz);if(p==location&&++n==1)bad.q(p,0);return ok;},base,ns,result),"dual draw rejects concurrent state scene or arm retirement");
                        }
                        std::cout<<"PASS: Corvo simultaneous native draw and exact overlay ownership, role, clock and concurrency guards\n";
                    }
                    auto invalid=both;invalid.q(pb+8,arm);require(!inspectEquipment(invalid),"power holster cannot borrow right sword arm");
                    invalid=both;invalid.d(left+0xac,2);require(!inspectEquipment(invalid),"power holster refuses foreign arm");
                    invalid=both;invalid.q(playerChildren+8,weapon);require(!inspectEquipment(invalid),"power holster needs reciprocal child membership");
                    invalid=both;invalid.d(ps+0x24,123);require(!inspectEquipment(invalid),"power and sword state handles cannot alias");
                    invalid=both;invalid.d(pi+0x18,2);require(!inspectEquipment(invalid),"power holster refuses extra bindings");
                    invalid=both;invalid.d(sc+0x28,334);require(!inspectEquipment(invalid),"power holster exact duration required");
                    invalid=both;invalid.d(pi+0x160,334);require(!inspectEquipment(invalid),"expired power holster scene refused");
                    invalid=both;invalid.d(pi+0x27c,5);require(!inspectEquipment(invalid),"completed power scene cannot authorize active overlap");
                    invalid=both;invalid.str(text,"midnight/gameplay/player/base/powers/power_equip.midnightscene");require(!inspectEquipment(invalid),"unobserved simultaneous power draw refused");
                    invalid=both;invalid.str(strings,"midnight/gameplay/player/base/combat/sword_equip.midnightscene");require(!inspectEquipment(invalid),"power holster cannot authorize sword draw");
                    for(auto location:{ps+0x24,pi+8,pb+8,left+0xac,playerChildren+8}){
                        invalid=both;unsigned n=0;NativePlayerStates ns{};
                        require(readNativePlayerStates([&](auto p,auto d,auto sz){return both.read(p,d,sz);},base,ns),"dual holster race setup");
                        require(!readNativePlayerEquipmentScene([&](auto p,auto d,auto sz){const bool ok=invalid.read(p,d,sz);if(p==location&&++n==1)invalid.q(p,0);return ok;},base,ns,result),"dual holster rejects concurrent state, scene and arm replacement");
                    }
                    std::cout<<"PASS: dual native Emily holster, exact left/right ownership and concurrent replacement refusal\n";
                }
                {auto quick=other;quick.str(strings,"midnight/gameplay/player/base/combat/sword_unequip_fast.midnightscene");
                 quick.d(instance+0x18,1);quick.d(scene+0x28,333);quick.d(instance+0x160,120);
                 require(inspectEquipment(quick)&&result.variant==7&&result.character==2,"observed Emily333ms fast holster uses owned right arm");
                 auto bad=quick;bad.d(scene+0x28,334);require(!inspectEquipment(bad),"fast holster exact source clock required");
                 bad=quick;bad.q(bindings+8,weapon);require(!inspectEquipment(bad),"fast holster cannot bind sword instead of arm");
                 bad=quick;bad.d(instance+0x160,334);require(!inspectEquipment(bad),"expired fast holster refused");
                 bad=quick;bad.str(names+0x400,"models/characters/player/corvo/body.def");require(!inspectEquipment(bad),"foreign character cannot borrow Emily holster");
                 bad=quick;bad.d(instance+0x27c,2);require(!inspectEquipment(bad),"retired fast holster refused");}
                {auto quick=other;quick.str(strings,"midnight/gameplay/player/base/combat/sword_unequip_sneak_fast.midnightscene");
                 quick.d(physics+0x50,1);quick.d(physics+0x54,1);quick.d(active,6);quick.q(mode+0x10+6*8,ground);quick.q(ground,base+0x1c84258);quick.d(ground+8,6);
                 quick.d(instance+0x18,1);quick.d(scene+0x28,333);quick.d(instance+0x160,120);
                 require(inspectEquipment(quick)&&result.variant==8&&result.character==2,"549 crouched fast holster owns native Emily arm");
                 auto bad=quick;bad.d(scene+0x28,334);require(!inspectEquipment(bad),"crouched fast holster exact clock required");
                 bad=quick;bad.q(bindings+8,weapon);require(!inspectEquipment(bad),"crouched fast holster requires arm not sword");
                 bad=quick;bad.d(physics+0x50,0);require(!inspectEquipment(bad),"crouched fast holster cannot borrow standing stance");
                 bad=quick;bad.d(instance+0x160,334);require(!inspectEquipment(bad),"expired crouched holster refused");
                 bad=quick;bad.d(arm+0xac,2);require(!inspectEquipment(bad),"foreign crouched holster arm refused");}
                auto fast=eq;fast.str(strings,"midnight/gameplay/player/base/combat/sword_equip_fast.midnightscene");fast.d(instance+0x18,1);fast.d(scene+0x28,500);
                require(inspectEquipment(fast)&&result.variant==5,"fast mantle recovery binds only the owned right arm");
                auto fastLow=fast;fastLow.str(strings,"midnight/gameplay/player/base/combat/sword_equip_sneak_fast.midnightscene");fastLow.d(scene+0x28,333);fastLow.d(instance+0x160,200);
                fastLow.d(active,6);fastLow.q(mode+0x10+6*8,ground);fastLow.q(ground,base+0x1c84258);fastLow.d(ground+8,6);
                require(inspectEquipment(fastLow)&&result.variant==6,"Corvo crouched fast recovery binds owned arm");
                auto fastLowEmily=fastLow;fastLowEmily.str(names+0x400,"models/characters/player/emily/body.def");fastLowEmily.str(names+0x600,"models/characters/player/emily/right_arm.def");fastLowEmily.str(strings+0x100,"Scene_1");
                require(inspectEquipment(fastLowEmily)&&result.variant==6&&result.character==2,"Emily crouched fast recovery uses her subscene");
                for(auto confined:{fastLow,fastLowEmily}){
                    confined.d(physics+0x50,2);confined.d(physics+0x54,2);confined.d(active,10);
                    confined.q(mode+0x10+10*8,ground);confined.q(ground,base+0x1c812b8);confined.d(ground+8,10);
                    require(inspectEquipment(confined)&&result.variant==6,"649/650 automatic crouch owns fast right-arm recovery on both roles");
                    auto bad=confined;bad.q(ground,base+0x1c84258);require(!inspectEquipment(bad),"automatic crouch needs its actual native class");
                    bad=confined;bad.d(physics+0x50,1);require(!inspectEquipment(bad),"automatic crouch cannot borrow manual crouch stance");
                    bad=confined;bad.str(strings,"midnight/gameplay/player/base/combat/sword_equip_fast.midnightscene");bad.d(scene+0x28,500);require(!inspectEquipment(bad),"automatic crouch cannot authorize unobserved standing draw");
                    bad=confined;bad.d(instance+0x160,334);require(!inspectEquipment(bad),"automatic crouch recovery cannot outlive its scene");
                    bad=confined;unsigned reads=0;NativePlayerStates ns{};
                    require(readNativePlayerStates([&](auto p,auto d,auto n){return bad.read(p,d,n);},base,ns)&&
                        !readNativePlayerEquipmentScene([&](auto p,auto d,auto n){const bool ok=bad.read(p,d,n);if(p==equipState+0x24&&++reads==1)bad.d(p,999);return ok;},base,ns,result),"automatic recovery refuses concurrent scene replacement");
                }
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
    {
        auto mana=m;const auto c=component+0x1000,decl=c+0x1000;
        mana.d(player+0x270,2);mana.q(list+8,c);mana.q(c,base+0x1c55ed8);mana.q(c+8,decl);mana.q(c+0x10,player);mana.q(decl,base+0x1c55d98);
        const auto putFloat=[](Memory& mem,std::uintptr_t at,float value){mem.put(at,&value,4);};putFloat(mana,c+0x20,40.f);
        NativePlayerMana observed{};const auto inspectMana=[&](Memory& mem){return readNativePlayerMana([&](auto a,auto d,auto n){return mem.read(a,d,n);},base,observed);};
        require(inspectMana(mana)&&observed.current==40&&observed.component==c,"native mana belongs to current player");
        auto bad=mana;bad.q(c+0x10,player+8);require(!inspectMana(bad),"foreign mana owner refused");
        bad=mana;bad.q(decl,base+0x1c51698);require(!inspectMana(bad),"health declaration cannot impersonate mana");
        bad=mana;bad.d(player+0x270,3);bad.q(list+16,c);require(!inspectMana(bad),"duplicate mana component refused");
        bad=mana;putFloat(bad,c+0x20,-1);require(!inspectMana(bad),"negative mana refused");
        bad=mana;putFloat(bad,c+0x20,std::numeric_limits<float>::quiet_NaN());require(!inspectMana(bad),"nonfinite mana refused");
        bad=mana;bad.d(stack,8);require(!inspectMana(bad),"wheel logic does not authorize normal-gameplay mana callback");
        bad=mana;unsigned reads=0;require(!readNativePlayerMana([&](auto a,auto d,auto n){const bool ok=bad.read(a,d,n);if(a==c+0x20&&++reads==1)putFloat(bad,a,41.f);return ok;},base,observed),"concurrent native mana regeneration refused");
        bad=mana;reads=0;require(!readNativePlayerMana([&](auto a,auto d,auto n){const bool ok=bad.read(a,d,n);if(a==c+0x20&&++reads==1)bad.q(game+0x3130b0,player+8);return ok;},base,observed),"player replacement invalidates native mana snapshot");
        std::cout<<"PASS: native mana identity, declaration, bounds, duplicate, transition and concurrent-change guards\n";
    }
    {
        auto counters=m;const auto stats=game+0x1f77c8;
        counters.d(stats+0x9c,3);counters.d(stats+0xa0,5);counters.d(stats+0x158,13);counters.d(stats+0x15c,15);
        NativePotionStatistics p{};
        const auto inspectCounters=[&](Memory& mem){return readNativePotionStatistics([&](auto a,auto d,auto n){return mem.read(a,d,n);},base,p);};
        require(inspectCounters(counters)&&p.a[0]==3&&p.a[1]==5&&p.b[0]==13&&p.b[1]==15,"native potion IDs read from both independent statistic arrays");
        auto bad=counters;bad.d(stats+0x9c,0xffffffff);require(!inspectCounters(bad),"negative potion statistic refused");
        bad=counters;bad.d(stats+0x15c,1000000001);require(!inspectCounters(bad),"unbounded potion statistic refused");
        bad=counters;bad.d(stack,8);require(!inspectCounters(bad),"diagnostic statistics do not relax native wheel refusal");
        bad=counters;bad.d(game+0x1f7400,15);require(!inspectCounters(bad),"pending load refuses diagnostic statistics");
        bad=counters;unsigned n=0;require(!readNativePotionStatistics([&](auto a,auto d,auto size){const bool ok=bad.read(a,d,size);if(a==stats+0x9c&&++n==1)bad.d(a,4);return ok;},base,p),"concurrent native consumption refuses torn statistic snapshot");
        bad=counters;n=0;require(!readNativePotionStatistics([&](auto a,auto d,auto size){const bool ok=bad.read(a,d,size);if(a==stats+0x9c&&++n==1)bad.q(game+0x3130b0,player+8);return ok;},base,p),"player replacement refuses old statistic ownership");
        std::cout<<"PASS: native potion statistic arrays, IDs, bounds, native transition and torn-read refusal\n";
    }
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
        bad=cm;bad.d(instance+0x18,3);require(!combat(bad),"three bindings require a validated native target");
        bad=cm;bad.d(scene+0x28,0);require(!combat(bad),"zero scene duration rejected");
        bad=cm;bad.str(names+0x300,"midnight/gameplay/player/base/combat/sword_parry_idle.midnightscene");require(!combat(bad),"block scene cannot impersonate slash");
        bad=cm;bad.str(names+0x300,"midnight/unknown.midnightscene");require(!combat(bad),"unmapped scene rejected");
        bad=cm;bad.d(game+0x1f5340,513);require(!combat(bad),"unbounded manager rejected");
        for(auto location:{slash+0x24,instance+8,bindings+0x28,children}){
            bad=cm;auto read=[&](auto p,auto d,auto n){const bool ok=bad.read(p,d,n);if(p==location)bad.q(p,0);return ok;};NativePlayerStates ns{};
            require(readNativePlayerStates([&](auto p,auto d,auto n){return cm.read(p,d,n);},base,ns)&&!readNativePlayerCombatScene(read,base,ns,result),"concurrent owner, scene or binding replacement rejected");
        }
        {
            auto kick=cm;kick.d(component+0x50,1);kick.d(active,6);kick.q(mode+0x10+6*8,state);kick.q(state,base+0x1c84258);kick.d(state+8,6);
            kick.str(names+0x300,"midnight/gameplay/player/base/combat/throwhit.midnightscene");kick.str(names+0x400,"kick");kick.d(scene+0x28,800);
            kick.d(instance+0x18,3);kick.q(bindings+8,arm);kick.q(bindings+0x28,player);kick.q(bindings+0x48,player);
            require(combat(kick)&&result.variant==17&&!result.target.actor,"native kick has one owned arm and two camera/body references to the same player");
            auto wrong=kick;wrong.q(bindings+0x48,arm);require(!combat(wrong),"kick cannot duplicate arm instead of player");
            wrong=kick;wrong.d(instance+0x18,2);require(!combat(wrong),"kick requires both player camera and body groups");
            wrong=kick;wrong.str(names+0x400,"Scene_1");require(!combat(wrong),"kick subscene identity required");
            wrong=kick;wrong.d(scene+0x28,801);require(!combat(wrong),"kick duration must be native800");
            wrong=kick;wrong.d(instance+0x160,801);require(!combat(wrong),"expired kick scene refused");
            wrong=kick;wrong.d(component+0x50,0);require(!combat(wrong),"unobserved standing kick source remains refused");
            wrong=kick;wrong.d(arm+0xac,2);require(!combat(wrong),"kick arm cannot belong to partner");
            wrong=kick;unsigned reads=0;NativePlayerStates ns{};
            require(readNativePlayerStates([&](auto p,auto d,auto n){return kick.read(p,d,n);},base,ns),"kick state fixture");
            require(!readNativePlayerCombatScene([&](auto p,auto d,auto n){const bool ok=wrong.read(p,d,n);if(p==bindings+0x48&&++reads==1)wrong.q(p,arm);return ok;},base,ns,result),"kick actor replacement during read refused");
        }
        {
            auto heavy=cm;heavy.d(component+0x50,1);heavy.d(active,6);heavy.q(mode+0x10+6*8,state);heavy.q(state,base+0x1c84258);heavy.d(state+8,6);
            heavy.str(names+0x300,"midnight/gameplay/player/base/combat/sword_attack_focus_sneak_out.midnightscene");
            heavy.str(names+0x400,"Scene_2");heavy.d(scene+0x28,1000);
            require(combat(heavy)&&result.variant==18&&result.character==1,"Corvo owned crouched heavy release");
            auto em=heavy;em.str(names+0x100,"models/characters/player/emily/body.def");em.str(names+0x200,"models/characters/player/emily/right_arm.def");em.str(names+0x400,"Scene_1");em.d(scene+0x28,833);
            require(combat(em)&&result.variant==18&&result.character==2,"Emily owned crouched heavy release");
            auto other=heavy;other.str(names+0x400,"Scene_1");other.d(scene+0x28,833);require(combat(other)&&result.character==1,"Corvo634 selects native Scene_1 too");
            auto wrong=heavy;wrong.str(names+0x400,"Scene_1");require(!combat(wrong),"mismatched heavy subscene timing refused");
            wrong=heavy;wrong.d(scene+0x28,833);require(!combat(wrong),"wrong heavy subscene duration refused");
            wrong=heavy;wrong.d(component+0x50,0);require(!combat(wrong),"unobserved standing sneak release refused");
            wrong=heavy;wrong.d(instance+0x18,3);wrong.q(bindings+0x48,player);require(!combat(wrong),"heavy release cannot acquire kick duplicate binding");
            wrong=heavy;wrong.d(instance+0x160,1001);require(!combat(wrong),"expired heavy release refused");
        }
        std::cout<<"PASS: native combat scene ownership, bounds, generation and concurrent replacement\n";
        {
            auto hit=cm;constexpr auto npc=player+0x10000,parent=npc+0x1000,spawner=npc+0x2000;
            constexpr auto parentComponents=npc+0x3000,spawned=npc+0x4000,npcDecl=npc+0x5000,npcNames=npc+0x6000;
            constexpr unsigned npcHandle=0x20003,parentHandle=0x30004;
            hit.d(component+0x50,0);hit.d(active,0);hit.q(mode+0x10,state);hit.q(state,base+0x1c85628);hit.d(state+8,0);
            hit.q(game+0x3130c0,npc);hit.q(npc,base+0x1c5e108);hit.d(npc+0xa8,npcHandle);hit.d(npc+0xac,parentHandle);
            hit.q(game+0x3130c8,parent);hit.q(parent,base+0x1c5e948);hit.d(parent+0xa8,parentHandle);hit.q(parent+0x20,npcNames);hit.str(npcNames,"ld_spawner_guard_tutorial_2");
            hit.q(npc+0x2e8,npcDecl);hit.q(npcDecl,base+0x1ab58f8);hit.q(npcDecl+8,npcNames+0x100);hit.str(npcNames+0x100,"models/characters/def/generic/dunwall_guard_male_small_01.def");
            hit.q(parent+0x268,parentComponents);hit.d(parent+0x270,1);hit.q(parentComponents,spawner);hit.q(spawner,base+0x1c57d78);hit.q(spawner+0x10,parent);
            hit.q(spawner+0x30,spawned);hit.d(spawner+0x38,1);hit.d(spawned,npcHandle);
            const float transform[12]={0,0,0,1,0,0,0,1,0,0,0,1};hit.put(npc+0x300,transform,sizeof(transform));
            hit.d(instance+0x18,3);hit.q(bindings+0x48,npc);
            require(combat(hit)&&result.variant==1&&result.target.handle==npcHandle&&result.target.reciprocal,"native targeted slash retains owned arm and reciprocal NPC");
            auto alternate=hit;alternate.str(names+0x300,"midnight/gameplay/player/base/combat/sword_slash_rightb.midnightscene");alternate.str(names+0x400,"Scene_1");
            require(combat(alternate)&&result.variant==2,"both observed targeted slash variants and subscenes accepted");
            auto low=hit;low.d(component+0x50,1);low.d(active,6);low.q(mode+0x10+6*8,state);low.q(state,base+0x1c84258);low.d(state+8,6);
            low.str(names+0x400,"Scene_1");low.d(scene+0x28,933);
            for(auto name:{"sword_slash_sneak_right","sword_slash_sneak_left"}){
                low.str(names+0x300,(std::string("midnight/gameplay/player/base/combat/")+name+".midnightscene").c_str());
                require(combat(low)&&(result.variant==5||result.variant==6)&&result.target.handle==npcHandle,"native crouched strike validates exact target-bearing scene");
            }
            auto rise=low;rise.d(component+0x50,0);rise.d(active,0);rise.q(state,base+0x1c85628);rise.d(state+8,0);
            require(combat(rise)&&result.variant==6,"native crouched strike can retain its scene while rising to standing");
            rise.d(active,2);rise.q(mode+0x20,state);rise.q(state,base+0x1c85028);rise.d(state+8,2);
            require(!combat(rise),"unobserved airborne target-bearing strike remains refused");
            low.d(instance+0x18,2);low.str(names+0x300,"midnight/gameplay/player/base/combat/sword_sneak_attack_hit.midnightscene");low.d(scene+0x28,866);
            require(combat(low)&&result.variant==15&&!result.target.actor,"native right hit recovery retains player and owned right arm");
            auto wrong=low;wrong.d(instance+0x18,3);require(!combat(wrong),"recovery cannot impersonate target-bearing scene");
            wrong=low;wrong.d(scene+0x28,933);require(!combat(wrong),"recovery must have exact native duration");
            low.str(names+0x300,"midnight/gameplay/player/base/combat/sword_sneak_attack_left_hit.midnightscene");low.str(names+0x400,"Scene_2");low.d(scene+0x28,666);
            low.d(component+0x50,0);low.d(active,0);low.q(state,base+0x1c85628);low.d(state+8,0);
            require(combat(low)&&result.variant==16,"native left hit recovery can change from crouch to standing");
            auto invalid=hit;invalid.q(bindings+0x48,arm);require(!combat(invalid),"duplicate arm cannot impersonate hit target");
            invalid=hit;invalid.d(spawned,npcHandle+1);require(!combat(invalid),"nonreciprocal hit target refused");
            invalid=hit;invalid.d(spawner+0x38,2);invalid.d(spawned+4,npcHandle+1);require(!combat(invalid),"ambiguous target spawner refused");
            invalid=hit;invalid.q(npc,base+0x1c5e428);require(!combat(invalid),"another player cannot become target NPC");
            invalid=hit;invalid.d(component+0x50,1);require(!combat(invalid),"unobserved crouched target scene refused");
            invalid=hit;invalid.str(names+0x300,"midnight/gameplay/player/base/combat/sword_attack_focus_out.midnightscene");require(!combat(invalid),"unobserved target-bearing asset refused");invalid=hit;invalid.str(names+0x300,"midnight/gameplay/player/base/combat/sword_slash_left.midnightscene");require(combat(invalid),"1214de: the left slash binding its guard (1371a/b) admitted");
            invalid=hit;invalid.str(names+0x400,"Unknown");require(!combat(invalid),"unobserved target subscene refused");
            invalid=hit;invalid.d(scene+0x28,999);require(!combat(invalid),"unobserved target duration refused");
            invalid=hit;invalid.d(instance+0x18,4);require(!combat(invalid),"four-actor scene cannot enter sword hit path");
            invalid=hit;unsigned n=0;NativePlayerStates ns{};require(readNativePlayerStates([&](auto p,auto d,auto size){return hit.read(p,d,size);},base,ns),"target fixture native states");
            require(!readNativePlayerCombatScene([&](auto p,auto d,auto size){const bool ok=invalid.read(p,d,size);if(p==npc+0x300&&++n==1)invalid.d(spawned,npcHandle+1);return ok;},base,ns,result),"target replacement during read invalidates whole scene");
            std::cout<<"PASS: targeted combat ownership, reciprocal NPC, context boundaries and concurrent replacement\n";
        }
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
                constexpr auto throwDecl=player+0xb000,camera=player+0xc000;
                const float strength=4.f,direction[]={1.f,0.f,0.f};
                high.q(slash+0x18,throwDecl);high.q(throwDecl,base+0x1c78f98);high.put(throwDecl+0xa0,&strength,4);
                high.q(machine+0x310,camera);high.q(camera,base+0x1c72218);high.q(camera+0x10,player);high.put(camera+0x25288,direction,12);
                const auto research=[&](Memory& memory){auto read=[&](auto p,auto d,auto n){return memory.read(p,d,n);};NativePlayerStates ns{};
                    return readNativePlayerStates(read,base,ns)&&readNativePlayerCarryScene(read,base,ns,observed,true);};
                require(!inspect(high),"production carry path still refuses observation-only throw");
                require(research(high)&&observed.variant==8&&observed.phase==2&&observed.character==1&&observed.throwVelocity==std::array<float,3>{4,0,0},"Corvo high throw binds exact native drop state, owned arm, corpse and owned camera impulse");
                auto highEmily=high;highEmily.str(names+0x100,"models/characters/player/emily/body.def");highEmily.str(names+0x200,"models/characters/player/emily/right_arm.def");
                require(research(highEmily)&&observed.character==2,"Emily high throw uses the same observed duration and Scene_1");
                for(auto location:{slash+0x54,slash+0x50,slash+0x24,scene+0x28,instance+0x18,corpse+0xa8,component+0x50}){
                    auto bad=high;bad.d(location,99);require(!research(bad),"throw rejects unknown selector, flags, scene, duration, bindings, incarnation or stance");
                }
                auto bad=high;bad.d(instance+0x160,667);require(!research(bad),"throw refuses expired one-shot clock");
                for(auto location:{throwDecl,throwDecl+0xa0,camera,camera+0x10,camera+0x25288,machine+0x310}){bad=high;bad.q(location,0);require(!research(bad),"throw requires exact configuration and reciprocal finite camera direction");}
                for(auto location:{slash+0x54,slash+0x50,slash+0x24,bindings+0x48,game+0x3130c0,instance+0x160,throwDecl+0xa0,camera+0x25288,machine+0x310}){
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
        bad=lm;bad.str(names,"midnight/gameplay/player/base/mobility/cat_fall_strong.midnightscene");require(!inspect(bad),"damaging landing refused at the light landing duration");
        bad.d(scene+0x28,1066);require(!inspect(bad),"damaging landing refused with only the body bound (its arms share the scene, 1214co)");
        constexpr auto right=instance+0x8000,left=instance+0x9000,rd=instance+0xa000,ld=instance+0xb000,children=instance+0xc000;
        auto strong=lm;strong.str(names,"midnight/gameplay/player/base/mobility/cat_fall_strong.midnightscene");strong.d(scene+0x28,1066);strong.d(instance+0x160,1040);
        strong.d(instance+0x18,3);strong.q(bindings+0x28,left);strong.q(bindings+0x48,right);strong.q(game+0x3130b8,right);strong.q(game+0x3130c0,left);
        strong.q(player+0xb0,children);strong.d(player+0xb8,2);strong.q(children,right);strong.q(children+8,left);
        for(unsigned i=0;i<2;++i){const auto arm=i?left:right,ad=i?ld:rd;strong.q(arm,base+0x1c5e428);strong.d(arm+0xa8,i?0x20003:0x10002);strong.d(arm+0xac,1);strong.q(arm+0x20,names+0x300+i*0x100);strong.str(names+0x300+i*0x100,i?"left_arm_1":"right_arm_2");strong.q(arm+0x2e8,ad);strong.q(ad,base+0x1ab58f8);strong.q(ad+8,names+0x500+i*0x100);strong.str(names+0x500+i*0x100,i?"models/characters/player/corvo/left_arm.def":"models/characters/player/corvo/right_arm.def");}
        require(inspect(strong)&&result.strong&&result.character==1&&result.durationMs==1066&&result.timeMs==1040&&result.rightArm.side==2&&result.leftArm.side==1,"Corvo damaging landing binds the body, then the left and right native arms (1214cq, observed 1066 ms)");
        auto strongEmily=strong;strongEmily.str(names+0x100,"Scene_1");strongEmily.str(names+0x200,"models/characters/player/emily/body.def");
        strongEmily.str(names+0x500,"models/characters/player/emily/right_arm.def");strongEmily.str(names+0x600,"models/characters/player/emily/left_arm.def");
        require(inspect(strongEmily)&&result.strong&&result.character==2,"Emily damaging landing binds her body and arms");
        bad=lm;bad.d(scene+0x28,1066);require(!inspect(bad),"light landing refused at the strong landing duration");
        bad=strong;bad.str(names,"midnight/gameplay/player/base/mobility/cat_fall_light.midnightscene");bad.d(scene+0x28,1000);require(!inspect(bad),"light landing refused with three bindings");
        bad=strong;bad.d(instance+0x160,1067);require(!inspect(bad),"expired damaging landing refused");
        bad=strong;bad.q(bindings+0x28,right);bad.q(bindings+0x48,left);require(!inspect(bad),"damaging landing arms in the observed order (left, then right)");
        bad=strong;bad.str(names+0x500,"models/characters/player/emily/right_arm.def");require(!inspect(bad),"other character's arm refused");
        bad=strong;bad.q(bindings+0x48,left);require(!inspect(bad),"one arm bound twice refused");
        bad=strong;bad.str(names,"midnight/gameplay/player/base/mobility/cat_fall_light_leftarm.midnightscene");require(!inspect(bad),"first-person arm landing scene refused");
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
        for(auto confined:{crouch,crouchEmily}){
            confined.d(component+0x50,2);confined.d(component+0x54,2);confined.d(state+0xb8,10);
            confined.str(names,"midnight/gameplay/player/base/mobility/mantle_low_crouch.midnightscene");
            require(inspect(confined)&&result.variant==2&&result.endStance==1,"649/650 owned low mantle into automatic crouch reuses crouch presentation");
            NativePlayerStates ns{};unsigned presentation=99;auto rd=[&](auto p,auto d,auto n){return confined.read(p,d,n);};
            require(readNativePlayerStates(rd,base,ns)&&readNativePlayerPresentationStance(rd,base,ns,presentation)&&presentation==1&&ns.player.current==2&&ns.active[0].id==8,"auto mantle changes only presentation stance and preserves native identity");
            auto bad=confined;bad.d(component+0x50,0);require(!inspect(bad),"automatic mantle requires observed physical crouch2");
            bad=confined;bad.d(state+0xd8,2);require(!inspect(bad),"unobserved high automatic mantle stays rejected");
            bad=confined;bad.str(names,"midnight/gameplay/player/base/mobility/mantle_low_sneak.midnightscene");require(!inspect(bad),"automatic mantle requires confined rather than manual sneak scene");
            bad=confined;bad.d(state+0xb8,6);require(!inspect(bad),"manual crouch cannot borrow automatic mantle scene");
            bad=confined;bad.d(arm+0xac,2);require(!inspect(bad),"automatic mantle still refuses foreign support arm");
            {// 1213l (#7, 1237c): the EotW window's confined low mantle binds the LEFT arm.
             auto leftLow=confined;std::string body;{const auto at=armNames+0x100;char text[64]{};for(unsigned k=0;k<63;++k){char c=0;leftLow.read(at+k,&c,1);if(!c)break;text[k]=c;}body=text;}
             const bool emily=body.find("emily")!=std::string::npos;
             leftLow.str(armNames,"left_arm_1");leftLow.str(armNames+0x200,emily?"models/characters/player/emily/left_arm.def":"models/characters/player/corvo/left_arm.def");
             require(inspect(leftLow)&&result.variant==2&&result.endStance==1,"1213l a confined low mantle accepts a single LEFT support arm (EotW window)");}
            bad=confined;unsigned reads=0;
            require(!readNativePlayerTraversalScene([&](auto p,auto d,auto n){const bool ok=bad.read(p,d,n);if(p==state+0xb8&&++reads==1)bad.d(p,6);return ok;},base,ns,result),"automatic mantle rejects next-state changes while reading");
        }
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
    {
        auto loot=actionState;loot.player.current=0;loot.active={{0,0,base+0x1c85628},{47,0,base+0x1c7bcc8},{81,0,base+0x1c7df88}};
        unsigned semantic=99;
        require(nativePlayerMotion(loot,base,motion)&&motion==0&&nativePlayerShadowAction(loot,base,semantic)&&semantic==0,"observed standing loot retains movement without inventing an item gesture");
        std::reverse(loot.active.begin(),loot.active.end());require(nativePlayerGroundLootStates(loot,base),"loot state order independent");
        auto bad=loot;bad.defaultMode=false;require(!nativePlayerGroundLootStates(bad,base),"loot cannot borrow another mode");
        bad=loot;bad.player.current=1;require(!nativePlayerMotion(bad,base,motion),"standing class cannot authorize crouched loot");
        bad=loot;bad.player.logic=5;require(!nativePlayerGroundLootStates(bad,base),"cinematic loot cannot authorize grounded control");
        bad=loot;bad.active[1].vtable++;require(!nativePlayerMotion(bad,base,motion),"loot ID alone cannot authorize another native class");
        bad=loot;bad.active[1]=bad.active[0];require(!nativePlayerGroundLootStates(bad,base),"duplicate undraw cannot replace loot");
        bad=loot;bad.active.back()={2,0,base+0x1c85028};require(!nativePlayerGroundLootStates(bad,base),"air loot is not inferred from standing evidence");
        bad=loot;bad.active[0]={39,0,base+0x1c85d28};require(!nativePlayerShadowAction(bad,base,semantic),"combat cannot alias the optional undraw state");
        {auto blockLeft=loot;blockLeft.active={{0,0,base+0x1c85628},{44,0,base+0x1c81a68},{64,0,base+0x1c79cf8}};unsigned sa=9;require(nativePlayerSwordAction(blockLeft,base,sa)&&sa==2&&nativePlayerShadowAction(blockLeft,base,semantic)&&semantic==2,"1214cu: a left-hand power EquipChange beside a block keeps the block");auto slashLeft=blockLeft;slashLeft.active[1]={39,0,base+0x1c85d28};require(nativePlayerShadowAction(slashLeft,base,semantic)&&semantic==1,"1214cu: and beside a slash keeps the slash");auto runLeft=blockLeft;runLeft.active.pop_back();runLeft.active[1]={64,0,base+0x1c79cf8};require(nativePlayerShadowAction(runLeft,base,semantic)&&semantic==0,"1214cu: alone beside movement keeps the movement");auto twoLeft=blockLeft;twoLeft.active[1]={64,0,base+0x1c79cf8};require(!nativePlayerSwordAction(twoLeft,base,sa),"1214cu: two left-hand EquipChange states still fail closed");}
        bad=loot;bad.active.erase(bad.active.begin());require(nativePlayerGroundLootStates(bad,base),"standing loot does not require an undraw overlay");
        bad.active.pop_back();require(!nativePlayerGroundLootStates(bad,base),"loot alone cannot invent grounded movement");
        auto crouch=loot;crouch.player.current=1;crouch.active.back()={6,0,base+0x1c84258};
        require(nativePlayerMotion(crouch,base,motion)&&motion==1&&nativePlayerShadowAction(crouch,base,semantic)&&semantic==0,"observed native crouched loot preserves crouch while item handling stays native");
        crouch.active.erase(crouch.active.begin());require(nativePlayerMotion(crouch,base,motion)&&motion==1,"crouched loot without undraw remains crouched");
        bad=crouch;bad.player.current=0;require(!nativePlayerGroundLootStates(bad,base),"sneaking class cannot invent standing loot");
        bad=crouch;bad.player.current=2;require(!nativePlayerGroundLootStates(bad,base),"automatic crouch loot is not inferred from manual crouch");
        bad=crouch;bad.active.back().vtable++;require(!nativePlayerGroundLootStates(bad,base),"crouched loot requires exact Sneaking class");
    }
    {
        auto confined=actionState;confined.player.current=2;confined.active={{10,0,base+0x1c812b8}};
        unsigned semantic=99,presentation=99;const auto noRead=[](auto,auto,auto){return false;};
        require(nativePlayerMotion(confined,base,motion)&&motion==1&&nativePlayerShadowAction(confined,base,semantic)&&semantic==0&&
            readNativePlayerPresentationStance(noRead,base,confined,presentation)&&presentation==1&&confined.player.current==2,"native automatic crouch maps idle and motion without changing capsule");
        confined.active.push_back({53,0,base+0x1c79cf8});
        require(nativePlayerMotion(confined,base,motion)&&motion==1&&nativePlayerShadowAction(confined,base,semantic)&&semantic==7,"automatic crouch equipment overlap retains movement");
        std::reverse(confined.active.begin(),confined.active.end());require(nativePlayerAutoCrouchStates(confined,base),"automatic crouch active-list order independent");
        auto bad=confined;bad.defaultMode=false;require(!nativePlayerAutoCrouchStates(bad,base),"automatic crouch requires default player mode");
        bad=confined;bad.player.current=1;require(!nativePlayerAutoCrouchStates(bad,base),"automatic crouch requires physical stance2");
        bad=confined;bad.active.back().vtable++;require(!nativePlayerMotion(bad,base,motion),"automatic crouch refuses wrong native class");
        bad=confined;bad.active[0]={39,0,base+0x1c85d28};require(!nativePlayerShadowAction(bad,base,semantic),"unobserved auto crouch combat overlay remains rejected");
        bad=confined;bad.active[0]=bad.active[1];require(!nativePlayerAutoCrouchStates(bad,base),"automatic crouch duplicates refused");
        bad=confined;bad.active.erase(bad.active.begin()+1);require(!readNativePlayerPresentationStance(noRead,base,bad,presentation),"raw stance2 and equipment alone cannot invent crouch");
    }
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
    {NativeWindblastFxRecord one{};require(readNativeFxRecordById(read,base,game,7,one)&&one.id==7&&one.record==record,"targeted FX lookup finds exact native ID");
     require(!readNativeFxRecordById(read,base,game,8,one),"targeted lookup refuses absent ID");
     m.d(record+4,8);require(!readNativeFxRecordById(read,base,game,7,one),"targeted FX lookup refuses recycled pool slot");m.d(record+4,7);
     m.q(node+8,node);require(!readNativeFxRecordById(read,base,game,7,one),"targeted bucket cycle refused");m.q(node+8,0);
     unsigned seen=0;auto changing=[&](auto a,auto b,auto n){if(a==record&&++seen==2)m.d(record+4,8);return m.read(a,b,n);};
     require(!readNativeFxRecordById(changing,base,game,7,one),"targeted ID replacement mid-read refused");m.d(record+4,7);}
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
void identityStringReadTests(){
    Memory mem;const std::uintptr_t address=0x10000;std::string text;unsigned calls=0;
    const auto read=[&](auto p,auto out,auto n){++calls;return mem.read(p,out,n);};
    unsigned char page[192]{};std::fill(page,page+191,'a');mem.put(address,page,sizeof(page));
    require(worldIdentityString(read,address,text)&&text==std::string(191,'a')&&calls==6,"bounded strings batch six memory reads instead of 192");
    page[191]='a';mem.put(address,page,sizeof(page));require(!worldIdentityString(read,address,text),"unterminated 192-byte string refused");
    page[33]=31;mem.put(address,page,sizeof(page));require(!worldIdentityString(read,address,text),"control character in later block refused");
    page[33]=127;mem.put(address,page,sizeof(page));require(!worldIdentityString(read,address,text),"nonprintable character refused");
    page[3]=0;mem.put(address,page,sizeof(page));require(worldIdentityString(read,address,text)&&text=="aaa","bytes after terminator do not invalidate a string");
    Memory boundary;boundary.str(address,"end");calls=0;
    const auto bounded=[&](auto p,auto out,auto n){++calls;return boundary.read(p,out,n);};
    require(worldIdentityString(bounded,address,text)&&text=="end","terminator before unreadable boundary remains readable");
    boundary.bytes.erase(address+3);require(!worldIdentityString(bounded,address,text),"unreadable terminator refused");
    boundary.str(address,"");require(!worldIdentityString(bounded,address,text),"empty identity refused");
    require(!worldIdentityString(bounded,0,text)&&!worldIdentityString(bounded,UINTPTR_MAX-10,text),"null and overflowing pointers refused");
}
void mesmerizeEyeIdentityTests(){
    Memory m;constexpr std::uintptr_t component=0x700000000,list=0x700010000;
    m.q(component+0x140,list);m.d(component+0x148,5);for(unsigned i=0;i<5;++i){m.d(list+i*8,~0u);m.d(list+i*8+4,0);}
    NativeMesmerizeEyeSlots slots{};auto inspect=[&](Memory& x){return readMesmerizeEyeSlots([&](auto a,auto b,auto n){return x.read(a,b,n);},component,slots);};
    require(inspect(m),"empty exact five-eye table readable");auto b=m;b.d(component+0x148,6);require(!inspect(b),"unknown eye table size refused");
    b=m;b.d(list+4,1);require(!inspect(b),"reference cannot point at absent effect ID");
    m.d(list,99);m.d(list+4,1);require(inspect(m),"one referenced native eye slot");b=m;b.d(list+8,99);require(!inspect(b),"duplicate eye incarnation refused");
    b=m;b.d(list+4,~0u);require(!inspect(b),"negative or unbounded reference count refused");require(inspect(m),"restore canonical eye slots");
    NativeMesmerizeWorld w{};w.present=true;w.actor=0x700020000;w.component=component;w.handle=0x880042;w.transform={1,2,3,1,0,0,0,1,0,0,0,1};
    NativeWindblastFxRecord e{};e.id=99;e.owner=w.handle;e.ownerActor=w.actor;e.parent=0x3fff0010;e.declaration=0x700030000;e.name=kNativeMesmerizeEye;e.ownerDefinition="models/powers/mesmerize/mesmerize_object.def";
    e.origin[0]=1;e.origin[1]=2;e.origin[2]=5;e.axis[0]=e.axis[4]=e.axis[8]=1;std::vector<NativeWindblastFxRecord> out;
    auto select=[&](std::vector<NativeWindblastFxRecord> r){return selectNativeMesmerizeEyes(w,slots,e.declaration,r,out);};
    require(select({e})&&out.size()==1,"exact committed world eye selected by live reference ID");
    auto wrong=e;wrong.owner++;require(!select({wrong}),"foreign effect owner refused");wrong=e;wrong.ownerActor++;require(!select({wrong}),"recycled world actor refused");
    wrong=e;wrong.id++;require(!select({wrong}),"missing referenced eye cannot be guessed by name");wrong=e;wrong.declaration++;require(!select({wrong}),"wrong eye declaration refused");
    wrong=e;wrong.name="models/effects/gameplay/powers/mesmerize_v2/link/mesmerize_v2_link_01.fx";require(!select({wrong}),"beam cannot impersonate eye");
    wrong=e;wrong.axis[0]=-1;require(!select({wrong}),"reflected eye basis refused");wrong=e;wrong.origin[0]=NAN;require(!select({wrong}),"invalid eye origin refused");
    require(!select({e,e}),"ambiguous effect incarnation refused");slots.entries[0][1]=0;require(select({e})&&out.empty(),"unreferenced native effect tail is not a live eye");
    std::cout<<"PASS: exact Mesmerize eye references, owner/declaration, finite pose, stale/foreign effects and tail exclusion\n";
}
void mesmerizeTargetTests(){
    Memory m;constexpr std::uintptr_t c=0x700000000,list=c+0x1000;
    m.q(c+0x1a8,0);m.d(c+0x1b0,0);NativeMesmerizeTargetTable out{};
    auto inspect=[&](Memory& v){return readMesmerizeTargetTable([&](auto a,auto b,auto n){return v.read(a,b,n);},c,out);};
    require(inspect(m)&&out.entries.empty(),"empty native target list is explicit absence");
    std::array<unsigned char,0x70> raw{};m.put(list,raw.data(),raw.size());m.q(c+0x1a8,list);m.d(c+0x1b0,1);m.d(list,0x120014);m.d(list+0x30,55);m.d(list+0x34,~0u);m.d(list+0x38,56);m.d(list+0x60,2);unsigned char one=1;m.put(list+0x6d,&one,1);
    require(inspect(m)&&out.entries.size()==1&&out.entries[0].beam==55&&out.entries[0].head==56,"native target/effect IDs and anchor decoded");
    auto b=m;b.d(c+0x1b0,33);require(!inspect(b),"unbounded target array refused");b=m;b.d(list+0x60,5);require(!inspect(b),"unknown anchor refused");
    b=m;b.d(list+0x38,55);require(!inspect(b),"duplicate effect assigned to different target slots refused");b=m;b.d(list,0x3fff0010);require(!inspect(b),"retired target handle refused");
    b=m;b.d(c+0x1b0,2);std::array<unsigned char,0x70> copy{};m.read(list,copy.data(),copy.size());b.put(list+0x70,copy.data(),copy.size());require(!inspect(b),"duplicate target incarnation refused");
    std::array<float,12> origin={1,2,3,1,0,0,0,1,0,0,0,1},pose=origin;pose[0]+=5;
    require(mesmerizeTargetTransform(pose.data(),origin,10),"finite rigid beam endpoint within scope");pose[0]+=10;require(!mesmerizeTargetTransform(pose.data(),origin,10),"unbounded beam endpoint refused");
    pose=origin;pose[3]=-1;require(!mesmerizeTargetTransform(pose.data(),origin,10),"reflected beam basis refused");pose=origin;pose[2]=NAN;require(!mesmerizeTargetTransform(pose.data(),origin,10),"nonfinite beam endpoint refused");
    std::cout<<"PASS: read-only Mesmerize target identity bounds, duplicate/retired references and rigid beam endpoint validation\n";
}
#include "native_material_params_tests.h"
#include "native_openable_tests.h"
#include "door_protocol_tests.h"
#include "usable_protocol_tests.h"
#include "native_coin_tests.h"
#include "coin_protocol_tests.h"
#include "native_food_tests.h"
#include "native_usable_tests.h"
#include "native_npc_authority_tests.h"
#include "npc_authority_protocol_tests.h"
#include "npc_combat_protocol_tests.h"
#include "npc_attack_protocol_tests.h"
void mapPathTests(){
    char a[]="campaign\\dunwall\\return\\streets\\dunwall_return_streets_p.map";
    require(normalizeCampaignMapPath(a)&&!std::strcmp(a,"campaign/dunwall/return/streets/dunwall_return_streets_p.map"),"1214by a backslash level-transition map normalizes to a campaign path");
    char c[]="campaign/dust/dust_return_p.map";require(normalizeCampaignMapPath(c)&&!std::strcmp(c,"campaign/dust/dust_return_p.map"),"1214by a slash path is unchanged");
    char m[]="menu\\main.map";require(!normalizeCampaignMapPath(m),"1214by a non-campaign map is still refused");}
int main() {
    mapPathTests();
    npcAuthorityProtocolTests();
    npcCombatProtocolTests();
    npcAttackProtocolTests();
    nativeNpcAuthorityTests();
    nativeUsableTests();
    nativeCoinTests();coinProtocolTests();nativeFoodTests();
    nativeOpenableTests();doorProtocolTests();usableProtocolTests();
    nativeMaterialParamsTests();
    assassinationPartnerIdentityTests();
    interactionPartnerIdentityTests();
    mesmerizeTargetTests();
    mesmerizeEyeIdentityTests();
    nativePlayerStunTests();
    identityStringReadTests();
    cosmeticGrenadeIdentityTests();
    {
        const std::uintptr_t base=0x140000000;NativePlayerStates s{};s.defaultMode=true;s.player.current=0;s.player.logic=1;unsigned motion=99;
        const NativePlayerActiveState ground{0,0,base+0x1c85628},throwing{62,0,base+0x1c7f118};
        s.active={ground,throwing};require(nativePlayerMotion(s,base,motion)&&motion==0,"native grenade overlay preserves standing movement");
        s.active={throwing,ground};require(nativePlayerMotion(s,base,motion)&&motion==0,"grenade locomotion independent of state order");
        s.active[0].vtable++;require(!nativePlayerMotion(s,base,motion),"wrong throw class refused");
        s.active={throwing};require(!nativePlayerMotion(s,base,motion),"throw alone cannot invent ground motion");
        s.active={ground,throwing,throwing};require(!nativePlayerMotion(s,base,motion),"duplicate throw overlay refused");
        s.active={ground,throwing};s.player.current=1;require(!nativePlayerMotion(s,base,motion),"standing native state cannot impersonate crouched grenade motion");
        const NativePlayerActiveState sneaking{6,0,base+0x1c84258};
        s.active={sneaking,throwing};require(nativePlayerMotion(s,base,motion)&&motion==1,"observed native crouched grenade motion");
        s.active={throwing,sneaking};require(nativePlayerMotion(s,base,motion)&&motion==1,"crouched grenade state order independent");
        s.active[1].vtable++;require(!nativePlayerGroundGrenadeStates(s,base),"wrong crouched class refused");
        s.active={sneaking,throwing};s.player.current=0;require(!nativePlayerGroundGrenadeStates(s,base),"crouched native state cannot impersonate standing");
        s.player.current=1;s.player.logic=2;require(!nativePlayerGroundGrenadeStates(s,base),"paused grenade ownership refused");s.player.logic=1;
        s.active={sneaking};require(nativePlayerGroundIdleStates(s,base),"crouched native idle permits owned held-item inspection");
        s.player.current=0;require(!nativePlayerGroundIdleStates(s,base),"held idle stance must match native class");
        s.active={ground};require(nativePlayerGroundIdleStates(s,base),"standing native idle still recognized");
        s.active={ground,throwing};require(!nativePlayerGroundIdleStates(s,base),"throwing requires separate owned scene not idle fallback");
        for(const auto& air:{NativePlayerActiveState{2,0x500000000,base+0x1c85028},NativePlayerActiveState{3,0x500000000,base+0x1c845c8}}){
            s.active={air,throwing};s.component=0x500001000;
            require(nativePlayerMotion(s,base,motion)&&motion==air.id,"observed native grenade overlay preserves exact jump/fall motion");
            s.active={throwing,air};require(nativePlayerAirGrenadeStates(s,base),"air grenade ignores state ordering");
            auto bad=s;bad.player.current=1;require(!nativePlayerAirGrenadeStates(bad,base),"air grenade cannot invent crouched physics");
            bad=s;bad.active[1].vtable++;require(!nativePlayerAirGrenadeStates(bad,base),"wrong native air class refused");
            bad=s;bad.active.push_back(air);require(!nativePlayerAirGrenadeStates(bad,base),"duplicate air activation refused");
            bad=s;bad.player.logic=2;require(!nativePlayerAirGrenadeStates(bad,base),"paused air grenade refused");
            bad=s;bad.active={air};require(nativePlayerAirIdleStates(bad,base)&&!nativePlayerAirGrenadeStates(bad,base),"idle jump is not an owned throw");
            require(!nativePlayerAirIdleStates(s,base),"throw cannot use idle held-item fallback");
            Memory m;m.q(air.pointer,air.vtable);m.d(air.pointer+8,air.id);m.q(air.pointer+0x10,s.component);m.d(air.pointer+0x94,0);
            const float z=66,apex=67.5f;m.put(air.pointer+0xa0,&z,4);m.put(air.pointer+0xa4,&apex,4);
            auto read=[&](auto p,auto d,auto n){return m.read(p,d,n);};unsigned stage=0,remaining=0;float start=0,target=0;
            require(!nativePlayerAir(read,s,base,stage,start,target,remaining),"grenade air metadata requires an independently owned scene");
            require(!nativePlayerAir(read,s,base,stage,start,target,remaining,false,true),"ranged ownership does not authorize grenade air");
            require(nativePlayerAir(read,s,base,stage,start,target,remaining,false,false,true)&&stage==(air.id==2?1u:2u),"owned grenade reads original native jump metadata");
            if(air.id==2){const float invalid=100;m.put(air.pointer+0xa4,&invalid,4);require(!nativePlayerAir(read,s,base,stage,start,target,remaining,false,false,true),"grenade cannot publish malformed native apex");}
        }
        s.active={ground,throwing};
        s.player.current=0;s.defaultMode=false;require(!nativePlayerMotion(s,base,motion),"grenade nondefault mode refused");
    }
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


