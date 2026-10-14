#pragma once
#include "world_identity.h"
// Narrow read-only identity for our four isolated, cosmetic-only entities.
// It cannot recognize a native player weapon or any arbitrary idEntity.
struct NativeCosmeticRanged {
    std::uintptr_t actor=0,owner=0,definition=0,frame=0,animation=0;
    unsigned handle=0,ownerHandle=0,character=0,kind=0;
};
template<class Read> bool readNativeCosmeticRanged(Read read,std::uintptr_t base,std::uintptr_t game,unsigned handle,NativeCosmeticRanged& out){
    NativeCosmeticRanged s{};s.handle=handle;s.actor=worldIdentityEntity(read,game,handle);
    std::uintptr_t vt=0,n=0,d=0,list=0,c=0,owner=0;unsigned count=0;std::string name;
    if(!s.actor||handle==1||!read(s.actor,&vt,8)||vt!=base+0x1c87d58||!read(s.actor+0x2e8,&s.definition,8)||!s.definition||
       !read(s.definition,&vt,8)||vt!=base+0x1ab58f8||!read(s.definition+8,&n,8)||!worldIdentityString(read,n,name))return false;
    if(name=="models/weapons/d2coop/ranged_prop_corvo_gun.def"){s.character=1;s.kind=1;}
    else if(name=="models/weapons/d2coop/ranged_prop_corvo_crossbow.def"){s.character=1;s.kind=2;}
    else if(name=="models/weapons/d2coop/ranged_prop_emily_gun.def"){s.character=2;s.kind=1;}
    else if(name=="models/weapons/d2coop/ranged_prop_emily_crossbow.def"){s.character=2;s.kind=2;}
    else return false;
    if(!read(s.actor+0xac,&s.ownerHandle,4)||s.ownerHandle==1||s.ownerHandle==handle||
       !(s.owner=worldIdentityEntity(read,game,s.ownerHandle))||!read(s.owner,&vt,8)||vt!=base+0x1c5e108||
       !read(s.owner+0x2e8,&d,8)||!d||!read(d,&vt,8)||vt!=base+0x1ab58f8||!read(d+8,&n,8)||!worldIdentityString(read,n,name)||
       name!=(s.character==1?"models/characters/def/d2coop/doppelganger_corvo.def":"models/characters/def/d2coop/doppelganger_emily.def")||
       !read(s.actor+0x268,&list,8)||!list||!read(s.actor+0x270,&count,4)||(count&0xffffff)!=2)return false;
    for(unsigned i=0;i<2;++i){
        if(!read(list+i*8,&c,8)||!c||!read(c,&vt,8)||!read(c+0x10,&owner,8)||owner!=s.actor)return false;
        if(vt==base+0x1b80408){if(s.animation)return false;s.animation=c;}
        else if(vt==base+0x1c573e8){if(s.frame)return false;s.frame=c;}
        else return false;
    }
    if(!s.animation||!s.frame||!read(s.actor+0x358,&c,8)||c!=s.frame||!read(s.frame+0x3c8,&owner,8)||owner!=s.owner)return false;
    // Re-read generation-bearing ownership and declaration after the walk.
    unsigned check=0;
    if(worldIdentityEntity(read,game,handle)!=s.actor||worldIdentityEntity(read,game,s.ownerHandle)!=s.owner||
       !read(s.actor+0xac,&check,4)||check!=s.ownerHandle||!read(s.actor+0x2e8,&d,8)||d!=s.definition)return false;
    out=s;return true;
}
inline bool cosmeticRangedModelMatches(const NativeCosmeticRanged& p,const char* model,const char* skeleton){
    const std::string stem=p.kind==1?(p.character==1?"models/weapons/gun_player/gun_player":"models/weapons/gun_player/gun_emily"):
        (p.character==1?"models/weapons/crossbow/crossbow_corvo":"models/weapons/crossbow/crossbow_emily");
    // The Corvo model's native skeleton resource omits the final 'w'.
    // Keep the exact model and skeleton identities distinct.
    const std::string rig=p.kind==2&&p.character==1?"models/weapons/crossbow/crossbo_corvo.md6skl":stem+".md6skl";
    return model==stem+".md6"&&skeleton==rig;
}
