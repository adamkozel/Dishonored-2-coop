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
// 2178 (#261): animal possession (d2-ghidra, confirmed static; a helper agent's research, rechecked in part). The
// controller at internal+0x410 is in state 2 (occupied animal) or 3 (leaving it), its target component is an
// arkCpntSwarmAgentPossession (vtable +0x1b63fa8), and its kind at internal+0x670 (set by 0x140823300) is 4 for a rat,
// 0x10 for a fish. Returns the pose bit: 0x800 rat, 0x1000 fish, 0 otherwise.
// 2207 (#278): lab switch coop_corpse_possession on|off, and the frames her corpse possession held a replica.
inline bool coopCorpsePossessionEnabled=true;inline std::uint64_t coopCorpsePossessionFrames=0;
// 2207 (#278): corpse possession (research-corpse-possession-v2188.md, confirmed static; corpse-eotw-v2202c confirmed the
// start at runtime). The controller is in state 4 (occupying a corpse) or 5 (leaving it), kind 2 at internal+0x670, and its
// target is an arkCpntCorpsePossession (vtable +0x1b640a8) owned (+0x10) by the body whose handle is at internal+0x63c.
// It returns that body's handle, which on a partner is her bound replica of the host's NPC.
template<class Read> bool readNativeCorpsePossessionOccupation(Read read,std::uintptr_t base,std::uint32_t* handle=nullptr){
    NativePlayerStates s{};std::uintptr_t internal=0,target=0,vt=0,entity=0;std::uint32_t state=0,kind=0,h=0,eh=0;
    if(!readNativePlayerStates(read,base,s)||!read(s.component+0x2f8,&internal,8)||!internal||!read(internal+0x5d0,&state,4)||(state!=4&&state!=5)||
       !read(internal+0x670,&kind,4)||kind!=2||!read(internal+0x648,&target,8)||!target||!read(target,&vt,8)||vt!=base+0x1b640a8||
       !read(internal+0x63c,&h,4)||!h||!read(target+0x10,&entity,8)||!entity||!read(entity+0xa8,&eh,4)||eh!=h)return false;
    if(handle)*handle=h;return true;
}
template<class Read> std::uint32_t readNativeAnimalPossession(Read read,std::uintptr_t base){
    NativePlayerStates s{};std::uintptr_t internal=0,target=0,vt=0;std::uint32_t state=0,kind=0;
    if(!readNativePlayerStates(read,base,s)||!read(s.component+0x2f8,&internal,8)||!internal||!read(internal+0x5d0,&state,4)||(state!=2&&state!=3)||
       !read(internal+0x648,&target,8)||!target||!read(target,&vt,8)||vt!=base+0x1b63fa8||!read(internal+0x670,&kind,4))return 0;
    return kind==4?0x800u:kind==0x10?0x1000u:0u;
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
