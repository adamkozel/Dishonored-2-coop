#pragma once
#include "native_swarm_world.h"
#include "native_windblast_fx_records.h"
// Research reader only. Native6baf20 ->875e80 ->752900 attaches the
// temporary effect owner to a rat through its reference frame, not actor+AC.
// No function calls or writes. A changing relation is rejected.
struct NativeSwarmTrailAttachment {
    std::uintptr_t frame=0,parent=0,binding=0;
    unsigned parentHandle=0,index=0,incarnation=0;
    std::uint16_t joint=0xffff;
    std::string bindingName;
};
template<class Read> bool readNativeSwarmTrailAttachment(Read read,std::uintptr_t base,
    const NativeSwarmWorld& world,const NativeWindblastFxRecord& fx,NativeSwarmTrailAttachment& out){
    out={};if(!world.present||!fx.ownerActor||!fx.ownerDefinition.empty()||
        fx.name!="models/effects/gameplay/powers/rat_swarm/rat/rat_spawn_trail_01.fx")return false;
    const auto game=world.equipment.player.sword.game;
    const auto eq=[&](std::uintptr_t p,std::uintptr_t wanted){std::uintptr_t v=0;return read(p,&v,8)&&v==wanted;};
    NativeSwarmTrailAttachment s{};
    if(worldIdentityEntity(read,game,fx.owner)!=fx.ownerActor||
       !read(fx.ownerActor+0x358,&s.frame,8)||!s.frame||!eq(s.frame,base+0x1c573e8)||!eq(s.frame+0x10,fx.ownerActor)||
       !read(s.frame+0x3c8,&s.parent,8)||!s.parent||!read(s.frame+0x3c0,&s.binding,8)||!s.binding||
       !worldIdentityString(read,s.binding,s.bindingName)||s.bindingName!="center_spine_jnt"||
       !read(s.frame+0x3d0,&s.joint,2)||s.joint>=0x8000||!read(s.parent+0xa8,&s.parentHandle,4)||
       worldIdentityEntity(read,game,s.parentHandle)!=s.parent)return false;
    const NativeSwarmAgent* rat=nullptr;
    for(const auto& a:world.agents)if(a.actor==s.parent&&a.handle==s.parentHandle){if(rat)return false;rat=&a;}
    if(!rat||!eq(rat->rat+0x10,s.parent)||!eq(rat->rat+0x18,world.component)||!eq(s.parent+0x360,rat->rat))return false;
    unsigned incarnation=0,worldHandle=0;std::uint16_t joint=0;std::string name;
    if(!read(rat->rat+0x44,&incarnation,4)||incarnation!=rat->incarnation||
       !read(world.equipment.component+0xd4,&worldHandle,4)||worldHandle!=world.handle||
       !eq(world.component+0x10,world.actor)||worldIdentityEntity(read,game,world.handle)!=world.actor||
       !eq(fx.ownerActor+0x358,s.frame)||!eq(s.frame+0x10,fx.ownerActor)||!eq(s.frame+0x3c8,s.parent)||
       !eq(s.frame+0x3c0,s.binding)||!read(s.frame+0x3d0,&joint,2)||joint!=s.joint||
       !worldIdentityString(read,s.binding,name)||name!=s.bindingName||worldIdentityEntity(read,game,fx.owner)!=fx.ownerActor)return false;
    s.index=rat->index;s.incarnation=rat->incarnation;out=std::move(s);return true;
}
