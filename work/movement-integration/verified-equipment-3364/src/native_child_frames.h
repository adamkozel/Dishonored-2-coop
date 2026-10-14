#pragma once
#include "native_health.h"
struct NativeChildFrame {std::uintptr_t entity=0,frame=0;std::uint32_t handle=0;};
struct NativeChildFrames {
    std::uintptr_t game=0,actor=0,list=0,frame=0;
    std::uint32_t handle=0,count=0;
    std::array<NativeChildFrame,32> children{};
};
template<class Read> bool readNativeChildFrames(Read read,std::uintptr_t base,std::uintptr_t frame,NativeChildFrames& out){
    NativeChildFrames s{};s.frame=frame;std::uintptr_t vt=0,owner=0,again=0;std::uint32_t count=0;
    if(!frame||!read(frame,&vt,8)||vt!=base+0x1c573e8||!read(frame+0x10,&s.actor,8)||!s.actor||
       !read(s.actor+0xa8,&s.handle,4)||!read(s.actor+0x358,&again,8)||again!=frame)return false;
    NativeHealth health{};
    if(!readNativeHealth(read,base,s.handle,health)||health.logic!=1||health.actor!=s.actor)return false;s.game=health.game;
    // Native 16fec0 returns entity+B0; 753c10 visits this direct-child list
    // and calls itself for every child's reference frame after updating root.
    if(!read(s.actor+0xb0,&s.list,8)||!read(s.actor+0xb8,&count,4)||(count&0xffffff)>s.children.size())return false;
    s.count=count&0xffffff;if(s.count&&!s.list)return false;
    for(unsigned i=0;i<s.count;++i){
        auto& c=s.children[i];std::uint32_t parent=0;
        if(!read(s.list+i*8,&c.entity,8)||!c.entity||c.entity==s.actor||!read(c.entity+0xa8,&c.handle,4)||
           worldIdentityEntity(read,s.game,c.handle)!=c.entity||!read(c.entity+0xac,&parent,4)||parent!=s.handle||
           !read(c.entity+0x358,&c.frame,8)||!c.frame||c.frame==frame||!read(c.frame,&vt,8)||vt!=base+0x1c573e8||
           !read(c.frame+0x10,&owner,8)||owner!=c.entity||!read(c.frame+0x3c8,&owner,8)||owner!=s.actor)return false;
        for(unsigned j=0;j<i;++j)if(s.children[j].entity==c.entity||s.children[j].frame==c.frame)return false;
    }
    if(!read(s.actor+0xb0,&again,8)||again!=s.list||!read(s.actor+0xb8,&count,4)||(count&0xffffff)!=s.count||
       !read(frame+0x10,&again,8)||again!=s.actor||worldIdentityEntity(read,s.game,s.handle)!=s.actor)return false;
    for(unsigned i=0;i<s.count;++i){const auto& c=s.children[i];std::uint32_t parent=0;
        if(!read(s.list+i*8,&again,8)||again!=c.entity||worldIdentityEntity(read,s.game,c.handle)!=c.entity||
           !read(c.entity+0xac,&parent,4)||parent!=s.handle||!read(c.entity+0x358,&again,8)||again!=c.frame)return false;
    }
    out=s;return true;
}
