#pragma once
#include "world_identity.h"
#include <array>
#include <vector>
// Exact inventory entry for an ordinary type4 stack or stored type14 potion.
// Native add clamps quantity+20 against capacity+24, returns accepted delta,
// and performs additional category behavior. This reader never writes either.
struct NativeStackItem {
    std::uintptr_t inventory=0,owner=0,items=0,entry=0,item=0;
    unsigned generation=0,count=0,type=0;
    int quantity=0,maximum=0;
    std::string name;
};
template<class Read> bool readNativeStackItem(Read read,std::uintptr_t base,std::uintptr_t inventory,std::uintptr_t owner,std::uintptr_t item,unsigned type,NativeStackItem& out,bool includeFood=false){
    if(!inventory||!owner||!item||(type!=4&&type!=14&&(!includeFood||type!=6)))return false;
    NativeStackItem s{};s.inventory=inventory;s.owner=owner;s.item=item;s.type=type;
    std::uintptr_t p=0,vt=0;unsigned flags=0,kind=0;
    const auto eq=[&](std::uintptr_t at,std::uintptr_t expected){return read(at,&p,8)&&p==expected;};
    if(!eq(inventory,base+0x1b9a878)||!eq(inventory+0x10,owner)||!read(inventory+0x18,&s.generation,4)||s.generation==~0u||
       !read(inventory+0x1c,&flags,4)||(flags&12)!=8||!read(inventory+0x38,&s.items,8)||!s.items||
       !read(inventory+0x40,&s.count,4)||!(s.count&0xffffff)||(s.count&0xffffff)>2048||
       !read(item,&vt,8)||vt!=base+0x1b9ea48||!read(item+0x60,&kind,4)||kind!=type||
       !read(item+8,&p,8)||!worldIdentityString(read,p,s.name))return false;
    std::vector<std::array<unsigned char,56>> entries(s.count&0xffffff);
    if(!read(s.items,entries.data(),entries.size()*56))return false;
    for(unsigned i=0;i<entries.size();++i){std::uintptr_t declaration=0;std::memcpy(&declaration,entries[i].data(),8);if(declaration!=item)continue;
        if(s.entry)return false;s.entry=s.items+i*56;std::memcpy(&s.quantity,entries[i].data()+0x20,4);std::memcpy(&s.maximum,entries[i].data()+0x24,4);
    }
    if(!s.entry||s.maximum<1||s.maximum>1000||s.quantity<0||s.quantity>s.maximum)return false;
    unsigned again=0;int quantity=0,maximum=0;
    if(!eq(inventory,base+0x1b9a878)||!eq(inventory+0x10,owner)||!eq(inventory+0x38,s.items)||
       !read(inventory+0x18,&again,4)||again!=s.generation||!read(inventory+0x1c,&again,4)||(again&12)!=8||!read(inventory+0x40,&again,4)||again!=s.count||
       !eq(s.entry,item)||!eq(item,base+0x1b9ea48)||!read(item+0x60,&again,4)||again!=type||
       !read(s.entry+0x20,&quantity,4)||quantity!=s.quantity||!read(s.entry+0x24,&maximum,4)||maximum!=s.maximum)return false;
    out=std::move(s);return true;
}
inline bool sameNativeStackItem(const NativeStackItem& a,const NativeStackItem& b){return a.inventory==b.inventory&&a.owner==b.owner&&a.items==b.items&&a.entry==b.entry&&a.item==b.item&&a.generation==b.generation&&a.count==b.count&&a.type==b.type&&a.maximum==b.maximum&&a.name==b.name;}
