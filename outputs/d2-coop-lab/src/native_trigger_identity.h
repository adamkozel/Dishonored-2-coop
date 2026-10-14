#pragma once
#include "world_identity.h"
inline std::uint64_t npcDamageKey(const std::string& text){
    std::uint64_t h=14695981039346656037ull;for(unsigned char c:text){h^=c;h*=1099511628211ull;}return h;
}
struct NativeDamageTrigger {
    std::uintptr_t actor=0,component=0;unsigned handle=0,generation=0;
    std::string name;std::uint64_t key=0;
};
template<class Read> bool readNativeDamageTrigger(Read read,std::uintptr_t base,std::uintptr_t game,unsigned handle,NativeDamageTrigger& out){
    NativeDamageTrigger n{};n.handle=handle;n.actor=worldIdentityEntity(read,game,handle);
    std::uintptr_t vt=0,name=0,list=0;unsigned flags=0,parent=0;
    if(!n.actor||!read(n.actor,&vt,8)||vt!=base+0x1c5eae8||!read(n.actor+0xac,&parent,4)||parent!=0x3fff0010||
       !read(n.actor+0x20,&name,8)||!worldIdentityString(read,name,n.name)||
       !read(n.actor+0x268,&list,8)||!list||!read(n.actor+0x270,&flags,4)||(flags&0xffffff)>256)return false;
    for(unsigned i=0;i<(flags&0xffffff);++i){std::uintptr_t cp=0,owner=0;
        if(!read(list+i*8,&cp,8)||!cp||!read(cp,&vt,8))return false;
        if(vt!=base+0x1c5bae8)continue;
        if(n.component||!read(cp+0x10,&owner,8)||owner!=n.actor||!read(cp+0x18,&n.generation,4))return false;
        n.component=cp;
    }
    if(!n.component||worldIdentityEntity(read,game,handle)!=n.actor)return false;
    n.key=npcDamageKey(n.name);out=std::move(n);return true;
}
template<class Read> bool readNativeDamageTriggers(Read read,std::uintptr_t base,std::uintptr_t game,std::vector<NativeDamageTrigger>& out){
    out.clear();std::uintptr_t vt=0,handles=0;unsigned flags=0;
    if(!game||!read(game,&vt,8)||vt!=base+0x1c940a8||!read(game+0x601c0,&handles,8)||!handles||
       !read(game+0x601c8,&flags,4)||(flags&0xffffff)>16384)return false;
    std::vector<unsigned> live(flags&0xffffff);if(!live.empty()&&!read(handles,live.data(),live.size()*4))return false;
    for(auto handle:live){const auto actor=worldIdentityEntity(read,game,handle);
        if(!actor||!read(actor,&vt,8)||vt!=base+0x1c5eae8)continue;
        NativeDamageTrigger n{};if(!readNativeDamageTrigger(read,base,game,handle,n))continue;
        out.push_back(std::move(n));
    }
    // Hash collisions and duplicated authored names are both ambiguous.
    std::vector<std::uint64_t> keys;for(const auto& n:out)keys.push_back(n.key);
    for(auto& n:out){unsigned matches=0;for(auto key:keys)if(key==n.key)++matches;if(matches!=1)n.key=0;}
    std::uintptr_t again=0;unsigned count=0;
    return read(game+0x601c0,&again,8)&&again==handles&&read(game+0x601c8,&count,4)&&count==flags;
}
inline bool sameNativeDamageTrigger(const NativeDamageTrigger& a,const NativeDamageTrigger& b){
    return a.key&&a.key==b.key&&a.name==b.name&&a.actor==b.actor&&a.component==b.component&&a.handle==b.handle&&a.generation==b.generation;
}
