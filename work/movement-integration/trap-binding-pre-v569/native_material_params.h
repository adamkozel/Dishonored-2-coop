#pragma once
#include "world_identity.h"
#include <array>
#include <vector>
#include <unordered_set>
// Confirmed static538 and read-only native542 layout. Values are raw words:
// renderParm declarations choose signed integer versus float interpretation.
struct NativeMaterialParams {
    std::uintptr_t actor=0,render=0;
    std::vector<unsigned short> ids;
    std::vector<std::array<unsigned,4>> words;
    int find(unsigned id)const{for(unsigned i=0;i<ids.size();++i)if(ids[i]==id)return static_cast<int>(i);return -1;}
};
template<class Read> bool readNativeMaterialParams(Read read,std::uintptr_t base,std::uintptr_t actor,std::uintptr_t render,NativeMaterialParams& out){
    NativeMaterialParams s{};s.actor=actor;s.render=render;
    std::uintptr_t values=0,ids=0,p=0;unsigned nf=0,ni=0,n=0;
    if(!actor||!render||!read(actor+0xf0,&p,8)||p!=render||!read(render,&p,8)||p!=base+0x1b0d628||
       !read(render+0x1950,&values,8)||!read(render+0x1958,&nf,4)||!read(render+0x1960,&ids,8)||!read(render+0x1968,&ni,4)||
       (nf&0xffffff)!=(ni&0xffffff)||(ni&0xffffff)>256||((ni&0xffffff)&&(!values||!ids)))return false;
    s.ids.resize(ni&0xffffff);s.words.resize(s.ids.size());
    if(!s.ids.empty()&&(!read(ids,s.ids.data(),s.ids.size()*2)||!read(values,s.words.data(),s.words.size()*16)))return false;
    std::unordered_set<unsigned> unique;for(auto id:s.ids)if(!unique.insert(id).second)return false;
    auto againIds=s.ids;auto again=s.words;
    if(!read(actor+0xf0,&p,8)||p!=render||!read(render,&p,8)||p!=base+0x1b0d628||
       !read(render+0x1950,&p,8)||p!=values||!read(render+0x1958,&n,4)||n!=nf||
       !read(render+0x1960,&p,8)||p!=ids||!read(render+0x1968,&n,4)||n!=ni||
       (!s.ids.empty()&&(!read(ids,againIds.data(),againIds.size()*2)||againIds!=s.ids||!read(values,again.data(),again.size()*16)||again!=s.words)))return false;
    out=std::move(s);return true;
}
