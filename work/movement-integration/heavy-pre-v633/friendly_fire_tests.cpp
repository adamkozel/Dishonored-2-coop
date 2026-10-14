#include "friendly_fire.h"
#include "avatar_ai_ownership.h"
#include "mesmerize_partner_identity.h"
#include "mesmerize_partner_cached_identity.h"
#include "carry_damage_identity.h"
#include <unordered_map>
#include <cstring>
#include <iostream>
#include <cstdlib>
struct Memory {
    std::unordered_map<std::uintptr_t,unsigned char> bytes;
    void put(std::uintptr_t p,const void* data,std::size_t n){auto b=static_cast<const unsigned char*>(data);for(std::size_t i=0;i<n;++i)bytes[p+i]=b[i];}
    void q(std::uintptr_t p,std::uint64_t v){put(p,&v,8);}void d(std::uintptr_t p,std::uint32_t v){put(p,&v,4);}
    void str(std::uintptr_t p,const char* s){put(p,s,std::strlen(s)+1);}
    bool read(std::uintptr_t p,void* out,std::size_t n){auto b=static_cast<unsigned char*>(out);for(std::size_t i=0;i<n;++i){auto it=bytes.find(p+i);if(it==bytes.end())return false;b[i]=it->second;}return true;}
};
void require(bool ok,const char* message){if(!ok){std::cerr<<"FAIL: "<<message<<'\n';std::exit(1);}}
constexpr std::uintptr_t base=0x140000000,game=0x200000000,player=0x300000000,avatar=player+0x1000;
constexpr std::uintptr_t playerHealth=0x400000000,avatarHealth=playerHealth+0x1000,enemyHealth=playerHealth+0x2000,definition=0x500000000,name=definition+0x1000;
constexpr std::uint32_t avatarHandle=0x10007,playerWeapon=0x20008,avatarWeapon=0x30009,enemyHandle=0x4000a,projectile=0x5000b;
void entity(Memory& m,std::uint32_t h,std::uintptr_t p,std::uint32_t parent=0){m.q(game+0x3130b0+(h>>16)*8,p);m.d(p+0xa8,h);m.d(p+0xac,parent);}
Memory fixture(){
    Memory m;m.q(base+0x39a5290,game);m.q(game,base+0x1c940a8);
    entity(m,1,player);entity(m,avatarHandle,avatar);entity(m,playerWeapon,player+0x2000,1);entity(m,avatarWeapon,player+0x3000,avatarHandle);
    entity(m,enemyHandle,player+0x4000);entity(m,projectile,player+0x5000,playerWeapon);
    m.q(player,base+0x1c5e428);m.q(avatar,base+0x1c5e108);m.q(avatar+0x2e8,definition);
    m.q(definition,base+0x1ab58f8);m.q(definition+8,name);m.str(name,"models/characters/def/d2coop/doppelganger_emily.def");
    m.q(playerHealth,base+0x1c51588);m.q(playerHealth+0x10,player);
    m.q(avatarHealth,base+0x1c51588);m.q(avatarHealth+0x10,avatar);
    m.q(enemyHealth,base+0x1c51588);m.q(enemyHealth+0x10,player+0x4000);return m;
}
int main(){
    {
        auto m=fixture();const CoopDamageAvatar a{game,avatar,avatarHandle};
        NativeAiComponentState held{};held.game=game;held.actor=avatar;held.handle=avatarHandle;
        held.component=0x610000000;held.manager=0x620000000;held.kind=NativeAiKind::Decision;
        constexpr std::uintptr_t list=0x630000000;
        m.q(game+0x1e8960,held.manager);m.q(avatar+0x268,list);m.d(avatar+0x270,1);m.q(list,held.component);
        m.q(held.component,base+nativeAiVtable(held.kind));m.q(held.component+0x10,avatar);m.d(held.component+0x1c,0x48);
        auto check=[&](Memory& mem,const NativeAiComponentState& h,std::uint64_t epoch=7){return ownedAvatarAiReference([&](auto p,auto d,auto n){return mem.read(p,d,n);},base,a,h,7,epoch);};
        require(check(m,held),"owned clone retains its one pause reference independently of peer/menu state");
        require(!check(m,held,8),"changed local world never retains old ownership");
        auto bad=held;bad.actor=player;require(!check(m,bad),"local player never gains clone AI ownership");
        bad=held;bad.kind=NativeAiKind::Reflex;require(!check(m,bad),"unowned AI component kind rejected");
        auto original=m;
        m.str(name,"models/characters/def/generic/dunwall_guard_male_small_01.def");require(!check(m,held),"ordinary world NPC is never retained");m=original;
        m.d(avatar+0xa8,avatarHandle+1);require(!check(m,held),"recycled clone generation rejected");m=original;
        m.q(game+0x1e8960,held.manager+8);require(!check(m,held),"replaced manager rejected");m=original;
        m.q(held.component+0x10,player);require(!check(m,held),"foreign component owner rejected");m=original;
        m.q(list,held.component+8);require(!check(m,held),"detached AI component rejected");m=original;
        m.d(avatar+0x270,2);m.q(list+8,held.component);require(!check(m,held),"duplicate component membership rejected");m=original;
        m.d(held.component+0x1c,8);require(!check(m,held),"no native pause ref means nothing to retain");m=original;
        m.d(held.component+0x1c,0x40);require(!check(m,held),"stopped component may retire");m=original;
        m.str(name,"models/characters/def/d2coop/doppelganger_corvo.def");require(check(m,held),"both custom clone definitions retain ownership");
        unsigned flagReads=0;
        auto racing=[&](auto p,auto d,auto n){if(p==held.component+0x1c&&++flagReads==2)m.d(p,8);return m.read(p,d,n);};
        require(!ownedAvatarAiReference(racing,base,a,held,7,7),"concurrently retired pause reference rejected");
        std::cout<<"Presentation AI lifetime identity checks passed\n";
    }
    {
        auto m=fixture();const CoopDamageAvatar a{game,avatar,avatarHandle};NativeMesmerizeWorld w{};
        constexpr auto power=player+0x7000;constexpr auto component=power+0x1000;constexpr unsigned handle=0x7000d;
        entity(m,handle,power);w.present=true;w.actor=power;w.handle=handle;w.component=component;w.state=3;
        w.equipment.player.character=2;w.equipment.player.sword.game=game;w.equipment.player.sword.actor=player;
        w.equipment.instantiated=w.equipment.started=w.equipment.equipped=true;
        const auto check=[&](Memory& mem,const NativeMesmerizeWorld& snapshot,std::uintptr_t target,std::uintptr_t c){return localMesmerizePartnerTarget([&](auto p,auto d,auto n){return mem.read(p,d,n);},base,a,snapshot,c,target);};
        require(check(m,w,avatar,component),"local committed Emily power excludes exact partner");
        auto unequipped=w;unequipped.equipment.equipped=false;unequipped.equipment.started=false;
        require(check(m,unequipped,avatar,component),"committed power still excludes partner after equipment stop and deselection");
        unequipped.equipment.instantiated=false;
        require(!check(m,unequipped,avatar,component),"missing equipment ownership still refuses exclusion");
        require(!check(m,w,player+0x4000,component),"ordinary NPC eligibility remains native");
        require(!check(m,w,avatar,component+8),"foreign Mesmerize component remains native");
        auto bad=w;bad.equipment.player.sword.actor=player+0x4000;require(!check(m,bad,avatar,component),"foreign caster remains native");
        bad=w;bad.present=false;require(!check(m,bad,avatar,component),"preview or missing world cannot authorize exclusion");
        bad=w;bad.state=2;require(!check(m,bad,avatar,component),"retired native power excluded from filter scope");
        auto mem=m;mem.d(avatar+0xa8,avatarHandle+1);require(!check(mem,w,avatar,component),"recycled partner refused");
        mem=m;mem.d(power+0xa8,handle+1);require(!check(mem,w,avatar,component),"recycled committed power refused");
        mem=m;mem.str(name,"models/powers/doppelganger/decoy_entity.def");require(!check(mem,w,avatar,component),"ordinary clone remains native");
        mem=m;unsigned reads=0;require(!localMesmerizePartnerTarget([&](auto p,auto d,auto n){bool ok=mem.read(p,d,n);if(p==power+0xa8&&++reads==1)mem.q(base+0x39a5290,0);return ok;},base,a,w,component,avatar),"world retirement during target decision refused");
        std::cout<<"PASS: local Mesmerize partner exclusion, unrelated NPC/caster preservation and stale identity refusal\n";
        // Populate the smaller current ownership graph. The cache is discovery
        // data, never authority for an actor/component that has since changed.
        auto& e=w.equipment;auto& inv=e.player.sword;
        constexpr std::uintptr_t storage=0x600000000;
        inv.inventory=storage;inv.items=storage+0x1000;inv.itemCount=1;inv.equipments=storage+0x2000;inv.equipmentCount=1;
        e.entry=inv.items;e.declaration=storage+0x3000;e.component=storage+0x4000;e.equipmentDeclaration=storage+0x5000;
        e.entity=storage+0x6000;e.handle=0x8000e;w.definition=storage+0x7000;w.declaration=storage+0x8000;
        auto componentList=[&](std::uintptr_t actor,std::uintptr_t c,std::uintptr_t list){m.q(actor+0x268,list);m.d(actor+0x270,1);m.q(list,c);};
        auto declaration=[&](std::uintptr_t d,std::uintptr_t vt,const char* text){m.q(d,vt);m.q(d+8,d+0x100);m.str(d+0x100,text);};
        m.q(player+0x2e8,storage+0x9000);declaration(storage+0x9000,base+0x1ab58f8,"models/characters/player/emily/body.def");
        componentList(player,inv.inventory,storage+0xa000);m.q(inv.inventory,base+0x1b9a878);m.q(inv.inventory+0x10,player);
        m.q(inv.inventory+0x38,inv.items);m.d(inv.inventory+0x40,1);m.q(inv.inventory+0x58,inv.equipments);m.d(inv.inventory+0x60,1);m.q(inv.equipments,e.entry);
        m.q(e.entry,e.declaration);m.d(e.entry+8,e.handle);m.q(e.entry+0x10,e.component);m.q(e.entry+0x18,e.equipmentDeclaration);m.d(e.entry+0x20,1);m.d(e.entry+0x24,1);
        declaration(e.declaration,base+0x1b9ea48,"models/powers/mesmerize/mesmerize.inventoryitem");
        declaration(e.equipmentDeclaration,base+0x1b9bd68,"components/characters/player/base/powers/mesmerize/mesmerize.cpntpowermesmerize");
        entity(m,e.handle,e.entity,1);componentList(e.entity,e.component,storage+0xb000);
        m.q(e.component,base+0x1b9bea8);m.q(e.component+8,e.equipmentDeclaration);m.q(e.component+0x10,e.entity);m.d(e.component+0x1c,8);m.d(e.component+0xd4,w.handle);
        m.q(w.actor,base+0x1c87d58);m.d(w.actor+0xac,0x3fff0010);m.q(w.actor+0x2e8,w.definition);componentList(w.actor,w.component,storage+0xc000);
        declaration(w.definition,base+0x1ab58f8,"models/powers/mesmerize/mesmerize_object.def");
        declaration(w.declaration,base+0x1c53a38,"components/characters/player/base/powers/mesmerize/mesmerize_object.cpntmesmerize");
        m.q(component,base+0x1c53958);m.q(component+8,w.declaration);m.q(component+0x10,w.actor);m.q(component+0x20,component);m.d(component+0x2c,3);
        const auto cached=[&](Memory& mem){return currentMesmerizePartnerOwner([&](auto p,auto d,auto n){return mem.read(p,d,n);},base,a,w,component,avatar);};
        require(cached(m),"discovered current ownership graph excludes partner");
        mem=m;mem.d(e.entry+0x20,0);mem.d(e.component+0x1c,0);require(cached(mem),"zero quantity and stopped equipment retain committed ownership");
        mem=m;mem.d(e.component+0xd4,w.handle+1);require(!cached(mem),"changed committed relationship invalidates discovery");
        mem=m;mem.d(w.actor+0xa8,w.handle+1);require(!cached(mem),"recycled world invalidates discovery");
        mem=m;mem.d(e.entity+0xa8,e.handle+1);require(!cached(mem),"recycled equipment invalidates discovery");
        mem=m;mem.q(inv.equipments,0);require(!cached(mem),"equipment no longer registered invalidates discovery");
        mem=m;mem.q(e.entry+0x10,e.component+8);require(!cached(mem),"replaced equipment component invalidates discovery");
        mem=m;mem.q(component+0x10,player);require(!cached(mem),"foreign world owner invalidates discovery");
        mem=m;mem.d(component+0x2c,2);require(!cached(mem),"retiring world is outside active exclusion scope");
        mem=m;mem.q(storage+0xc000,component+8);require(!cached(mem),"world component must remain a member");
        mem=m;mem.str(storage+0x9100,"models/characters/player/corvo/body.def");require(!cached(mem),"changed caster role invalidates discovery");
        mem=m;mem.str(w.definition+0x100,"models/powers/other/object.def");require(!cached(mem),"different native world declaration invalidates discovery");
        mem=m;mem.d(avatar+0xa8,avatarHandle+1);require(!cached(mem),"recycled partner invalidates discovery");
        mem=m;unsigned cachedReads=0;
        require(!currentMesmerizePartnerOwner([&](auto p,auto d,auto n){bool ok=mem.read(p,d,n);if(p==e.component+0xd4&&++cachedReads==1)mem.d(e.component+0xd4,w.handle+1);return ok;},base,a,w,component,avatar),"ownership changing during validation is refused");
        std::cout<<"PASS: cached Mesmerize discovery revalidates ownership, memberships, declarations and current generations\n";
        m.d(game+0x341058,3);m.d(game+0x1f7400,0);m.q(game+0x1f73f0,storage+0xd000);m.d(game+0x1f73f8,1);m.d(storage+0xd000,1);
        m.d(component+0x1c,8);std::array<float,12> transform{-33,-155,66,1,0,0,0,1,0,0,0,1};m.put(w.actor+0x300,transform.data(),48);
        NativeMesmerizeWorld sampled{};
        const auto sample=[&](Memory& mem){return readCurrentNativeMesmerizeWorld([&](auto p,auto d,auto n){return mem.read(p,d,n);},base,w,sampled);};
        require(sample(m)&&sampled.state==3&&sampled.transform==transform,"scoped resample refreshes native committed pose and state");
        mem=m;mem.d(component+0x2c,2);require(sample(mem)&&sampled.state==2,"scoped resample preserves native retirement state");
        mem=m;mem.d(storage+0xd000,2);require(!sample(mem),"native pause refuses scoped sampling");
        mem=m;mem.d(game+0x1f7400,1);require(!sample(mem),"native pending transition refuses scoped sampling");
        mem=m;mem.d(e.component+0xd4,w.handle+1);require(!sample(mem),"changed committed relationship refuses scoped sampling");
        mem=m;auto invalidTransform=transform;invalidTransform[0]=NAN;mem.put(w.actor+0x300,invalidTransform.data(),48);require(!sample(mem),"nonfinite scoped pose refused");
        mem=m;cachedReads=0;
        require(!readCurrentNativeMesmerizeWorld([&](auto p,auto d,auto n){bool ok=mem.read(p,d,n);if(p==w.actor+0x300&&++cachedReads==1)mem.d(component+0x2c,2);return ok;},base,w,sampled),"state change during scoped sampling refused");
        mem=m;cachedReads=0;
        require(!readCurrentNativeMesmerizeWorld([&](auto p,auto d,auto n){bool ok=mem.read(p,d,n);if(p==w.actor+0x300&&++cachedReads==1)mem.q(game+0x1f73f0,storage+0xe000);return ok;},base,w,sampled),"logic stack replacement during scoped sampling refused");
        std::cout<<"PASS: scoped Mesmerize resampling preserves native retirement, pause, transition and concurrent-change rejection\n";
    }
    const CoopDamageAvatar a{game,avatar,avatarHandle};const auto none=CoopFriendlyHit::None,local=CoopFriendlyHit::LocalToPartner,remote=CoopFriendlyHit::PartnerToLocal;
    auto m=fixture();const auto hit=[&](Memory& mem,std::uintptr_t health,std::uint32_t inflictor,std::uint32_t attacker){return classifyCoopFriendlyHit([&](auto p,auto d,auto n){return mem.read(p,d,n);},base,a,health,inflictor,attacker);};
    require(hit(m,avatarHealth,0,1)==local,"player attack against partner");
    require(hit(m,playerHealth,0,avatarHandle)==remote,"partner attack against player");
    require(hit(m,avatarHealth,playerWeapon,0)==local,"player attached sword as inflictor");
    require(hit(m,playerHealth,avatarWeapon,0)==remote,"partner attached sword as inflictor");
    require(hit(m,avatarHealth,projectile,0)==local,"nested owned source");
    require(hit(m,avatarHealth,0,projectile)==local,"owned child as attacker");
    require(hit(m,avatarHealth,0,enemyHandle)==none,"enemy still damages partner");
    require(hit(m,playerHealth,0,enemyHandle)==none,"enemy still damages player");
    require(hit(m,enemyHealth,0,1)==none,"player still damages enemies");
    require(hit(m,enemyHealth,0,avatarHandle)==none,"friendly filter does not absorb authoritative enemy outcomes");
    require(hit(m,avatarHealth,0,0)==none&&hit(m,playerHealth,0,0)==none,"environment damage unaffected");
    require(hit(m,playerHealth,0,1)==none&&hit(m,avatarHealth,0,avatarHandle)==none,"self damage outside friendly filter");
    require(hit(m,avatarHealth,0,playerWeapon+1)==none,"stale source handle refused");
    require(hit(m,avatarHealth,0,0xffff0001)==none,"invalid source slot refused");
    auto bad=m;bad.str(name,"models/characters/def/d2coop/doppelganger_corvo.def");require(hit(bad,avatarHealth,0,1)==local,"both owned character definitions supported");
    bad=m;bad.str(name,"models/powers/doppelganger/decoy_entity.def");require(hit(bad,avatarHealth,0,1)==none,"ordinary power clone is not partner");
    bad=m;bad.d(avatar+0xa8,avatarHandle+1);require(hit(bad,avatarHealth,0,1)==none,"recycled avatar generation refused");
    bad=m;bad.q(base+0x39a5290,game+0x1000);require(hit(bad,avatarHealth,0,1)==none,"replaced world refused");
    bad=m;bad.q(avatarHealth+0x10,player+0x4000);require(hit(bad,avatarHealth,0,1)==none,"foreign health owner refused");
    bad=m;bad.q(avatarHealth,base+0x1c51590);require(hit(bad,avatarHealth,0,1)==none,"wrong component class refused");
    bad=m;bad.q(player,base+0x1c5e108);require(hit(bad,avatarHealth,0,1)==none,"local player class validated");
    bad=m;bad.q(definition,0);require(hit(bad,avatarHealth,0,1)==none,"declaration class validated");
    bad=m;bad.d(player+0x2000+0xac,projectile);require(hit(bad,avatarHealth,projectile,0)==none,"cyclic source ownership refused");
    bad=m;bad.d(player+0x2000+0xa8,playerWeapon+1);require(hit(bad,avatarHealth,projectile,0)==none,"stale ancestor refused");
    bad=m;bad.bytes.erase(player+0x2000+0xac);require(hit(bad,avatarHealth,playerWeapon,0)==none,"unreadable source ownership refused");
    bad=m;unsigned reads=0;
    require(classifyCoopFriendlyHit([&](auto p,auto d,auto n){bool ok=bad.read(p,d,n);if(p==player+0xa8&&++reads==2)bad.q(avatarHealth+0x10,player);return ok;},base,a,avatarHealth,0,1)==none,"concurrent victim change refused");
    bad=m;reads=0;
    require(classifyCoopFriendlyHit([&](auto p,auto d,auto n){bool ok=bad.read(p,d,n);if(p==avatar+0xa8&&++reads==1)bad.q(base+0x39a5290,0);return ok;},base,a,avatarHealth,0,1)==none,"world retirement during classification refused");
    // Reproduce the v15 native damage attribution: a released replicated body
    // is the inflictor while the attacker is an unrelated native sentinel.
    const std::uintptr_t corpse=player+0x6000,corpseDecl=definition+0x2000,corpseName=definition+0x3000;
    const std::uint32_t corpseHandle=0x6000c;
    entity(m,corpseHandle,corpse);m.q(corpse,base+0x1c5e108);m.q(corpse+0x2e8,corpseDecl);
    m.q(corpseDecl,base+0x1ab58f8);m.q(corpseDecl+8,corpseName);m.str(corpseName,"models/characters/def/story/alexi_mayhew.def");
    const auto corpseFrame=corpse+0x1000;m.q(corpse+0x358,corpseFrame);m.q(corpseFrame,base+0x1c573e8);m.q(corpseFrame+0x10,corpse);m.q(corpseFrame+0x3f0,0);
    CarryDamageLease lease{a,corpse,corpseHandle,5000};
    const auto carry=[&](Memory& mem,CarryDamageLease s,std::uint64_t tick,std::uint32_t source){
        return isCarryPresentationDamage([&](auto p,auto d,auto n){return mem.read(p,d,n);},base,s,tick,source,0x3fff0010);
    };
    require(carry(m,lease,1000,corpseHandle),"replicated body impact with unattributed attacker blocked");
    bad=m;bad.q(corpseFrame+0x3f0,avatar);require(carry(bad,lease,1000,corpseHandle),"attached presentation body impact blocked");
    bad=m;bad.q(corpseFrame+0x3f0,player);require(!carry(bad,lease,1000,corpseHandle),"local native pickup immediately ends presentation exclusion");
    require(!carry(m,lease,5000,corpseHandle),"release tail expires at deadline");
    require(!carry(m,lease,1000,enemyHandle),"unrelated NPC damage remains native");
    require(!carry(m,lease,1000,1),"native player damage remains native");
    require(!carry(m,{},1000,corpseHandle),"unregistered body impact remains native");
    auto invalid=lease;invalid.until=20001;require(!carry(m,invalid,1000,corpseHandle),"unbounded lease refused");
    bad=m;bad.d(corpse+0xa8,corpseHandle+1);require(!carry(bad,lease,1000,corpseHandle),"recycled body generation refused");
    bad=m;bad.str(corpseName,"models/characters/def/generic/guard.def");require(!carry(bad,lease,1000,corpseHandle),"different body definition refused");
    bad=m;bad.q(base+0x39a5290,game+0x1000);require(!carry(bad,lease,1000,corpseHandle),"retired carry world refused");
    bad=m;bad.d(avatar+0xa8,avatarHandle+1);require(!carry(bad,lease,1000,corpseHandle),"retired carrier incarnation refused");
    std::cout<<"PASS: friendly-fire attribution and scoped carry presentation impact; unrelated damage, stale generations and lease expiry preserved\n";
}
