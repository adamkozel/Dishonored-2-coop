#pragma once
#include "world_identity.h"
#include <array>

// Exact retail Simple pickup declaration. Money is inventory+16c, not a
// quantity in the normal 56-byte inventory-entry list (6156e0/615620).
struct NativeCoin {
    std::uintptr_t game=0,actor=0,component=0,declaration=0,item=0;
    unsigned handle=0,generation=0,componentFlags=0;
    unsigned char flags=0,ownerFlags=0;
    int quantity=0;unsigned type=1;
    std::string name,itemName;
    std::array<float,3> position{};
};
template<class Read> bool readNativeCoin(Read read,std::uintptr_t base,std::uintptr_t game,unsigned handle,NativeCoin& out,bool includeStacks=false){
    NativeCoin s{};s.game=game;s.handle=handle;std::uintptr_t vt=0,p=0,list=0;unsigned count=0;
    const auto eq=[&](std::uintptr_t at,std::uintptr_t value){return read(at,&vt,8)&&vt==value;};
    if(!game||!eq(game,base+0x1c940a8)||!(s.actor=worldIdentityEntity(read,game,handle))||!eq(s.actor,base+0x1c5e258)||
       !read(s.actor+0x20,&p,8)||!worldIdentityString(read,p,s.name)||
       !read(s.actor+0x268,&list,8)||!list||!read(s.actor+0x270,&count,4)||!(count&0xffffff)||(count&0xffffff)>256)return false;
    std::array<std::uintptr_t,256> members{};if(!read(list,members.data(),(count&0xffffff)*8))return false;
    for(unsigned i=0;i<(count&0xffffff);++i){if(!members[i]||!read(members[i],&vt,8))return false;if(vt==base+0x1c5a3b8){if(s.component)return false;s.component=members[i];}}
    int special=0;unsigned char carried=0;
    if(!s.component||!eq(s.component+0x10,s.actor)||!read(s.component+0x18,&s.generation,4)||s.generation==~0u||
       !read(s.component+0x1c,&s.componentFlags,4)||(s.componentFlags&12)!=8||
       !read(s.component+8,&s.declaration,8)||!s.declaration||!eq(s.declaration,base+0x1c5a278)||
       !read(s.declaration+0xa0,&carried,1)||carried||!read(s.declaration+0xf8,&s.item,8)||!s.item||!eq(s.item,base+0x1b9ea48)||
       !read(s.item+8,&p,8)||!worldIdentityString(read,p,s.itemName)||!read(s.item+0x60,&s.type,4)||
       (s.type!=1&&(!includeStacks||(s.type!=4&&s.type!=14&&s.type!=6)))||
       !read(s.item+0xc4,&special,4)||special!=-1||!read(s.declaration+0x100,&s.quantity,4)||s.quantity<1||s.quantity>1000||
       !read(s.component+0xd0,&s.flags,1)||s.flags&0xf0||!read(s.actor+0xd0,&s.ownerFlags,1)||
       !read(s.actor+0x300,s.position.data(),12))return false;
    for(float f:s.position)if(!std::isfinite(f))return false;
    if(s.type==6){int health=0,mana=0;if(!read(s.item+0x78,&health,4)||!read(s.item+0x7c,&mana,4)||health<0||mana<0||(!health&&!mana))return false;}
    unsigned again=0;
    if(worldIdentityEntity(read,game,handle)!=s.actor||!eq(s.component,base+0x1c5a3b8)||!eq(s.component+0x10,s.actor)||
       !read(s.component+0x18,&again,4)||again!=s.generation||!eq(s.component+8,s.declaration)||!eq(s.declaration+0xf8,s.item))return false;
    out=std::move(s);return true;
}
inline bool sameNativeCoin(const NativeCoin& a,const NativeCoin& b){
    return a.game==b.game&&a.actor==b.actor&&a.handle==b.handle&&a.component==b.component&&a.generation==b.generation&&
        a.declaration==b.declaration&&a.item==b.item&&a.name==b.name&&a.itemName==b.itemName&&a.quantity==b.quantity&&a.type==b.type;
}
