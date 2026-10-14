#include "friendly_fire.h"
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
    std::cout<<"PASS: friendly-fire direct/owned-child attribution, both roles, stale generations, unrelated damage and retirement guards\n";
}
