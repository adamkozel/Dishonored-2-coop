#pragma once
#include "native_player_states.h"
#include "world_identity.h"
// Read-only research: the internal Possession mode state is independent of
// physics stance and active-state IDs. Exact image822a70 stores target at
// internal mode+410+230 and its entity handle at+410+22c. No control/VFX calls.
struct NativePossessionTarget {
    NativePlayerStates player{};
    std::uintptr_t internal=0,target=0,entity=0;
    std::uint32_t state=0,phase=0,handle=0,generation=0,flags=0;
    bool hasTarget=false;
};
template<class Read> bool readNativePossessionTargetFrame(Read read,std::uintptr_t base,NativePossessionTarget& p){
    if(!readNativePlayerStates(read,base,p.player)||
       !read(p.player.component+0x2f8,&p.internal,8)||!p.internal||
       !read(p.internal+0x5d0,&p.state,4)||
       !read(p.internal+0x5d4,&p.phase,4)||
       !read(p.internal+0x63c,&p.handle,4)||
       !read(p.internal+0x640,&p.generation,4)||
       !read(p.internal+0x648,&p.target,8))return false;
    if(p.generation==0xffffffff)return p.target==0;
    std::uintptr_t vt=0;std::uint32_t generation=0,handle=0;
    if(!p.target||!read(p.target,&vt,8)||vt!=base+0x1b641a8||
       !read(p.target+0x18,&generation,4)||generation!=p.generation||
       !read(p.target+0x1c,&p.flags,4)||!read(p.target+0x10,&p.entity,8)||!p.entity||
       !read(p.entity+0xa8,&handle,4)||handle!=p.handle||
       worldIdentityEntity(read,p.player.player.game,p.handle)!=p.entity)return false;
    std::uintptr_t list=0;std::uint32_t count=0;unsigned members=0;
    if(!read(p.entity+0x268,&list,8)||!list||!read(p.entity+0x270,&count,4)||!(count&0xffffff)||(count&0xffffff)>256)return false;
    for(unsigned i=0;i<(count&0xffffff);++i){std::uintptr_t c=0;if(!read(list+i*8,&c,8))return false;if(c==p.target)++members;}
    if(members!=1)return false;
    p.hasTarget=true;return true;
}
template<class Read> bool readNativePossessionTarget(Read read,std::uintptr_t base,NativePossessionTarget& out){
    NativePossessionTarget a{},b{};
    if(!readNativePossessionTargetFrame(read,base,a)||!readNativePossessionTargetFrame(read,base,b)||
       a.player.player.game!=b.player.player.game||a.player.player.player!=b.player.player.player||
       a.player.component!=b.player.component||a.player.mode!=b.player.mode||
       a.internal!=b.internal||a.state!=b.state||a.phase!=b.phase||a.target!=b.target||
       a.entity!=b.entity||a.handle!=b.handle||a.generation!=b.generation||a.flags!=b.flags||a.hasTarget!=b.hasTarget)return false;
    out=std::move(b);return true;
}
