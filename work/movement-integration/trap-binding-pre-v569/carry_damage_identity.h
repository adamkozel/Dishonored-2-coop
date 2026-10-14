#pragma once
#include "friendly_fire.h"
// A local presentation lease, created only after the carry fixture validates
// the exact dead authored body. No network packet can register a damage source.
struct CarryDamageLease {
    CoopDamageAvatar owner{};
    std::uintptr_t actor=0;
    std::uint32_t handle=0;
    std::uint64_t until=0;
};
template<class Read> bool isCarryPresentationDamage(Read read,std::uintptr_t base,const CarryDamageLease& s,
    std::uint64_t tick,std::uint32_t inflictor,std::uint32_t attacker){
    if(!s.until||tick>=s.until||s.until-tick>18000||!s.actor||!s.handle||s.handle==1||
       s.actor==s.owner.actor||s.handle==s.owner.handle||(inflictor!=s.handle&&attacker!=s.handle)||
       !validCoopDamageAvatar(read,base,s.owner)||worldIdentityEntity(read,s.owner.game,s.handle)!=s.actor)return false;
    std::uintptr_t vt=0,decl=0,name=0,frame=0,frameOwner=0,again=0;std::string path;
    if(!read(s.actor,&vt,8)||vt!=base+0x1c5e108||!read(s.actor+0x2e8,&decl,8)||!decl||
       !read(decl,&vt,8)||vt!=base+0x1ab58f8||!read(decl+8,&name,8)||!worldIdentityString(read,name,path)||
       path!="models/characters/def/story/alexi_mayhew.def"||
       !read(s.actor+0x358,&frame,8)||!frame||!read(frame,&vt,8)||vt!=base+0x1c573e8||
       !read(frame+0x10,&again,8)||again!=s.actor||!read(frame+0x3f0,&frameOwner,8)||
       (frameOwner&&frameOwner!=s.owner.actor))return false;
    // A local player/native actor reclaiming the dropped body ends this
    // presentation exclusion immediately, even inside the short release tail.
    return validCoopDamageAvatar(read,base,s.owner)&&worldIdentityEntity(read,s.owner.game,s.handle)==s.actor&&
        read(s.actor+0x358,&again,8)&&again==frame&&read(frame+0x3f0,&again,8)&&again==frameOwner;
}
