#pragma once
#include "native_swarm_world.h"
#include "native_windblast_fx_records.h"
constexpr const char* kNativeSwarmSpawnFx="models/effects/gameplay/powers/rat_swarm/spawn/rswarm_spawn_01.fx";
// Requires a freshly validated native world. Its equipment +d4, reciprocal
// component owner and declaration+A0 must remain unchanged around discovery.
// Static:1406a7a30; runtime research:v312. No native function is invoked.
template<class Read> bool readNativeSwarmSpawnFx(Read read,std::uintptr_t base,const NativeSwarmWorld& world,NativeWindblastFxRecord& out){
    if(!world.present||!world.actor||!world.component||!world.declaration||!world.equipment.instantiated)return false;
    const auto game=world.equipment.player.sword.game;unsigned handle=0;
    std::uintptr_t declaration=0,owner=0;float origin[3]{};
    if(!read(world.equipment.component+0xd4,&handle,4)||handle!=world.handle||worldIdentityEntity(read,game,handle)!=world.actor||
       !read(world.component+0x10,&owner,8)||owner!=world.actor||!read(world.declaration+0xa0,&declaration,8)||!declaration||
       !read(world.actor+0x300,origin,12))return false;
    std::vector<NativeWindblastFxRecord> records;
    if(!readNativeFilteredFxRecords(read,base,game,records,[](const std::string& n){return n==kNativeSwarmSpawnFx;}))return false;
    unsigned matches=0;NativeWindblastFxRecord candidate{};
    for(const auto& r:records){
        if(r.owner!=world.handle||r.ownerActor!=world.actor||r.parent!=1||r.declaration!=declaration||
           r.ownerDefinition!="models/characters/animals/rat/devouring_swarm.def")continue;
        std::array<float,12> t{};float distance=0;
        for(unsigned k=0;k<3;++k){t[k]=r.origin[k];const float d=r.origin[k]-origin[k];distance+=d*d;}
        std::copy(r.axis,r.axis+9,t.begin()+3);
        if(!validSwarmTransform(t)||distance>.01f)return false;
        candidate=r;++matches;
    }
    std::uintptr_t again=0;unsigned handleAgain=0;
    if(matches!=1||!read(world.equipment.component+0xd4,&handleAgain,4)||handleAgain!=handle||worldIdentityEntity(read,game,handle)!=world.actor||
       !read(world.component+0x10,&again,8)||again!=world.actor||!read(world.component+8,&again,8)||again!=world.declaration||
       !read(world.declaration+0xa0,&again,8)||again!=declaration)return false;
    out=std::move(candidate);return true;
}
