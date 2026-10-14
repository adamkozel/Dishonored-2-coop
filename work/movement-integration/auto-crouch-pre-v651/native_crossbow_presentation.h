#pragma once
#include "native_player_ranged_item.h"
#include <unordered_set>
// Read the native equipment graph's default clip identity. This is a visual
// state (cocked / fired), not ammunition authority or a request to fire.
struct NativeCrossbowPresentation {
    std::uintptr_t animation=0,tree=0,data=0,declaration=0,descriptor=0,slot=0,resource=0;
    unsigned state=0; // 1 native crossbow_idle, 2 native crossbow_outofammo
};
template<class Read> bool readNativeCrossbowPresentation(Read read,std::uintptr_t base,const NativePlayerRangedItem& p,NativeCrossbowPresentation& out){
    if(p.kind!=2||p.player.character<1||p.player.character>2||!p.entity||!p.component)return false;
    NativeCrossbowPresentation s{};std::uintptr_t list=0,c=0,vt=0,owner=0,buckets=0;unsigned flags=0,mask=0;
    if(!read(p.entity+0x268,&list,8)||!list||!read(p.entity+0x270,&flags,4)||!(flags&0xffffff)||(flags&0xffffff)>64)return false;
    for(unsigned i=0;i<(flags&0xffffff);++i){
        if(!read(list+i*8,&c,8)||!c||!read(c,&vt,8))return false;
        if(vt==base+0x1b80408){if(s.animation)return false;s.animation=c;}
    }
    if(!s.animation||!read(s.animation+0x10,&owner,8)||owner!=p.entity||
       !read(s.animation+0x338,&s.tree,8)||!s.tree||!read(s.tree,&s.data,8)||!s.data||
       !read(s.animation+0x340,&s.declaration,8)||!s.declaration||
       !read(s.declaration+0x70,&buckets,8)||!buckets||!read(s.declaration+0x80,&mask,4)||mask>4095)return false;
    unsigned offset=0;std::uintptr_t n=0;std::string name;std::unordered_set<std::uintptr_t> seen;
    for(unsigned b=0;b<=mask;++b){std::uintptr_t entry=0;
        if(!read(buckets+b*8,&entry,8))return false;
        for(unsigned depth=0;entry;++depth){
            if(depth>=128||!seen.insert(entry).second)return false;
            std::uintptr_t next=0;unsigned type=0,at=0;
            if(!read(entry,&n,8)||!worldIdentityString(read,n,name)||!read(entry+8,&type,4)||
               !read(entry+12,&at,4)||at>0x100000||!read(entry+0x970,&next,8))return false;
            if(name=="defaultAnim"){if(s.descriptor||type!=2)return false;s.descriptor=entry;offset=at;}
            entry=next;
        }
    }
    s.slot=s.data+offset;
    if(!s.descriptor||!read(s.slot,&s.resource,8)||!s.resource||!read(s.resource,&vt,8)||vt!=base+0x1b0ba38||
       !read(s.resource+8,&n,8)||!worldIdentityString(read,n,name))return false;
    const auto prefix=std::string("models/weapons/crossbow/animations/")+(p.player.character==1?"corvo/":"emily/");
    if(name==prefix+"crossbow_idle.md6anim")s.state=1;
    else if(name==prefix+"crossbow_outofammo.md6anim")s.state=2;
    else return false;
    // Concurrent replacement or a changed actor generation invalidates the read.
    std::uintptr_t q=0;unsigned number=0;
    if(worldIdentityEntity(read,p.player.sword.game,p.handle)!=p.entity||
       !read(p.entity+0x268,&q,8)||q!=list||!read(p.entity+0x270,&number,4)||number!=flags||
       !read(s.animation+0x10,&q,8)||q!=p.entity||!read(s.animation+0x338,&q,8)||q!=s.tree||
       !read(s.tree,&q,8)||q!=s.data||!read(s.animation+0x340,&q,8)||q!=s.declaration||
       !read(s.declaration+0x70,&q,8)||q!=buckets||!read(s.declaration+0x80,&number,4)||number!=mask||
       !read(s.descriptor+8,&number,4)||number!=2||!read(s.descriptor+12,&number,4)||number!=offset||
       !read(s.slot,&q,8)||q!=s.resource||!read(p.component+0x10,&q,8)||q!=p.entity||
       !read(p.component+0x1c,&number,4)||number!=p.flags)return false;
    unsigned matches=0;
    for(unsigned i=0;i<(flags&0xffffff);++i){if(!read(list+i*8,&q,8))return false;if(q==s.animation)++matches;}
    if(matches!=1)return false;
    out=s;return true;
}
