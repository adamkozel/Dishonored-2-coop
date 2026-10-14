#pragma once
#include "native_player_stance.h"
#include "world_identity.h"
#include <array>

// Read-only single-target observation of arkCpntRetargeting. Native setup
// 755870, activation757830 and release757550 were checked in Ghidra and
// against normal Corvo carry/release. This is NOT authority to mutate a body.
struct NativeRetargeting {
    std::uintptr_t game=0,actor=0,component=0,declaration=0,records=0;
    std::uintptr_t target=0,targetFrame=0,targetPhysics=0;
    unsigned handle=0,count=0,mode=0,targetHandle=0,flags=0;
    std::uint16_t ownerJoint=0xffff,targetJoint=0xffff; // process-local interned name IDs, not bone indices
    std::array<float,7> ownerOffset{},targetOffset{}; // xyz + xyzw
};
template<class Read> bool readNativeRetargeting(Read read,std::uintptr_t base,unsigned handle,NativeRetargeting& out){
    NativePlayerStance world{};if(!readNativePlayerStance(read,base,world))return false;
    NativeRetargeting s{};s.game=world.game;s.handle=handle;
    s.actor=worldIdentityEntity(read,s.game,handle);
    std::uintptr_t vt=0,list=0,owner=0,again=0;unsigned flags=0,index=0,recordFlags=0,n=0;
    if(!s.actor||!read(s.actor,&vt,8)||(vt!=base+0x1c5e108&&vt!=base+0x1c5e428)||
       !read(s.actor+0x268,&list,8)||!list||!read(s.actor+0x270,&flags,4)||
       !(flags&0xffffff)||(flags&0xffffff)>256)return false;
    for(unsigned i=0;i<(flags&0xffffff);++i){
        std::uintptr_t c=0;
        if(!read(list+i*8,&c,8)||!c||!read(c,&vt,8))return false;
        if(vt!=base+0x1c57778)continue;
        if(s.component||!read(c+0x10,&owner,8)||owner!=s.actor)return false;
        s.component=c;index=i;
    }
    if(!s.component||!read(s.component+8,&s.declaration,8)||!s.declaration||
       !read(s.declaration,&vt,8)||vt!=base+0x1c57638||
       !read(s.component+0x20,&s.records,8)||!read(s.component+0x28,&recordFlags,4)||
       (recordFlags&0xffffff)>1||!read(s.component+0x3c,&s.mode,4)||
       !read(s.component+0x40,&s.flags,4))return false;
    s.count=recordFlags&0xffffff;
    if(s.count){
        if(!s.records||s.mode!=7||!read(s.records+0x90,&s.targetHandle,4)||!s.targetHandle||s.targetHandle==handle||
           !(s.target=worldIdentityEntity(read,s.game,s.targetHandle))||!read(s.target,&vt,8)||vt!=base+0x1c5e108||
           !read(s.records+0x98,&s.targetFrame,8)||!s.targetFrame||!read(s.targetFrame,&vt,8)||vt!=base+0x1c573e8||
           !read(s.targetFrame+0x10,&owner,8)||owner!=s.target||!read(s.target+0x358,&again,8)||again!=s.targetFrame||
           !read(s.targetFrame+0x3f0,&owner,8)||owner!=s.actor||
           !read(s.records+8,&s.targetPhysics,8)||!s.targetPhysics||!read(s.targetPhysics,&vt,8)||vt!=base+0x1c54008||
           !read(s.targetPhysics+0x10,&owner,8)||owner!=s.target||
           !read(s.records+0x50,s.targetOffset.data(),28)||!read(s.records+0x70,s.ownerOffset.data(),28)||
           !read(s.records+0x6c,&s.targetJoint,2)||!read(s.records+0x8c,&s.ownerJoint,2)||
           s.targetJoint==0xffff||s.ownerJoint==0xffff)return false;
        for(const auto& offset:{s.targetOffset,s.ownerOffset}){
            float norm=0;for(unsigned i=0;i<7;++i){if(!std::isfinite(offset[i])||std::abs(offset[i])>100)return false;if(i>=3)norm+=offset[i]*offset[i];}
            if(norm<.95f||norm>1.05f)return false;
        }
    }
    // Recheck ownership after reading. Concurrent native release invalidates
    // this sample rather than returning an old target or a reusable pointer.
    if(!read(base+0x39a5290,&again,8)||again!=s.game||worldIdentityEntity(read,s.game,handle)!=s.actor||
       !read(s.actor+0x268,&again,8)||again!=list||!read(s.actor+0x270,&n,4)||n!=flags||
       !read(list+index*8,&again,8)||again!=s.component||!read(s.component,&again,8)||again!=base+0x1c57778||
       !read(s.component+8,&again,8)||again!=s.declaration||!read(s.component+0x10,&again,8)||again!=s.actor||
       !read(s.component+0x20,&again,8)||again!=s.records||!read(s.component+0x28,&n,4)||n!=recordFlags||
       !read(s.component+0x3c,&n,4)||n!=s.mode||!read(s.component+0x40,&n,4)||n!=s.flags)return false;
    if(s.count){
        std::array<float,7> a{},b{};std::uint16_t j=0;
        if(worldIdentityEntity(read,s.game,s.targetHandle)!=s.target||!read(s.records+0x90,&n,4)||n!=s.targetHandle||
           !read(s.records+0x98,&again,8)||again!=s.targetFrame||!read(s.target+0x358,&again,8)||again!=s.targetFrame||
           !read(s.targetFrame+0x10,&again,8)||again!=s.target||!read(s.targetFrame+0x3f0,&again,8)||again!=s.actor||
           !read(s.records+8,&again,8)||again!=s.targetPhysics||!read(s.targetPhysics+0x10,&again,8)||again!=s.target||
           !read(s.records+0x50,a.data(),28)||a!=s.targetOffset||!read(s.records+0x70,b.data(),28)||b!=s.ownerOffset||
           !read(s.records+0x6c,&j,2)||j!=s.targetJoint||!read(s.records+0x8c,&j,2)||j!=s.ownerJoint)return false;
    }
    NativePlayerStance finalWorld{};
    if(!readNativePlayerStance(read,base,finalWorld)||finalWorld.game!=s.game||finalWorld.player!=world.player)return false;
    out=s;return true;
}
