#pragma once
#include "world_identity.h"
#include <algorithm>

// Inspected retail build only. The native OnPause/OnResume callbacks remove/add
// the component's pool index in the registry's active prefix (48ff60/48fe10).
// Reading that prefix checks scheduler membership independently of pause bits.
enum class NativeAiKind { Decision,Reflex,Attack,IdleBreak };
inline std::uintptr_t nativeAiVtable(NativeAiKind kind) {
    return kind==NativeAiKind::Decision?0x1cc4108:kind==NativeAiKind::Reflex?0x1b8a588:kind==NativeAiKind::Attack?0x1b93a88:0x1b4cec8;
}
inline std::uintptr_t nativeAiRegistryOffset(NativeAiKind kind) {
    return kind==NativeAiKind::Decision?0x2e0:kind==NativeAiKind::Reflex?0x580:kind==NativeAiKind::Attack?0x600:0x530;
}
struct NativeAiComponentState {
    std::uintptr_t game=0,actor=0,component=0,manager=0,registry=0,tree=0;
    std::uint32_t handle=0,flags=0,poolIndex=0,liveCount=0,activeCount=0;
    std::uint32_t idleState=0;std::uint16_t idleAutoPlay=0;
    NativeAiKind kind=NativeAiKind::Decision;
    bool schedulerActive=false;
    std::vector<std::uint16_t> activeIndices;
};
using NativeAiDecisionState=NativeAiComponentState;
template<class Read> bool readNativeAiComponent(Read read,std::uintptr_t base,std::uintptr_t game,
                                              std::uint32_t handle,NativeAiKind kind,NativeAiComponentState& out) {
    NativeAiComponentState s{};s.game=game;s.handle=handle;s.kind=kind;
    const auto expectedVtable=base+nativeAiVtable(kind);
    std::uintptr_t vt=0,list=0;std::uint32_t flags=0;
    if(!game||!read(game,&vt,8)||vt!=base+0x1c940a8)return false;
    s.actor=worldIdentityEntity(read,game,handle);
    if(!s.actor||!read(s.actor,&vt,8)||vt!=base+0x1c5e108||
       !read(s.actor+0x268,&list,8)||!list||!read(s.actor+0x270,&flags,4)||(flags&0xffffff)>256)return false;
    for(unsigned i=0;i<(flags&0xffffff);++i) {
        std::uintptr_t component=0,owner=0;
        if(!read(list+i*8,&component,8)||!component||!read(component,&vt,8))return false;
        if(vt!=expectedVtable)continue;
        if(s.component||!read(component+0x10,&owner,8)||owner!=s.actor)return false;
        s.component=component;
    }
    if(!s.component||!read(s.component+0x1c,&s.flags,4)||
       (kind==NativeAiKind::Decision&&!read(s.component+0x30,&s.tree,8))||
       !read(game+0x1e8960,&s.manager,8)||!s.manager)return false;
    // Verified native accessors: 8f6610 decision, 5a3100 reflex, 5cc230 attack,
    // 425020 idle-break. Idle state and autoplay follow 425c90/426070.
    if(kind==NativeAiKind::IdleBreak&&(!read(s.component+0x20,&s.idleState,4)||s.idleState>3||
       !read(s.component+0x118,&s.idleAutoPlay,2)))return false;
    if(!read(s.manager+nativeAiRegistryOffset(kind),&s.registry,8)||!s.registry)return false;
    std::uintptr_t blocks=0,type=0,indices=0,liveEnd=0,activeEnd=0;
    std::uint32_t blockSize=0,blockFlags=0,stride=0,capacityFlags=0;
    if(!read(s.registry+8,&blockSize,4)||!blockSize||blockSize>65536||
       !read(s.registry+0x40,&blocks,8)||!blocks||!read(s.registry+0x48,&blockFlags,4)||
       !(blockFlags&0xffffff)||(blockFlags&0xffffff)>256||
       !read(s.registry+0x58,&type,8)||!type||!read(type+0x5c,&stride,4)||!stride||stride>65536)return false;
    unsigned matches=0;
    for(unsigned i=0;i<(blockFlags&0xffffff);++i) {
        std::uintptr_t block=0;if(!read(blocks+i*8,&block,8)||!block)return false;
        if(s.component<block||s.component-block>=static_cast<std::uint64_t>(stride)*blockSize)continue;
        if((s.component-block)%stride)return false;
        const auto index=static_cast<std::uint64_t>(i)*blockSize+(s.component-block)/stride;
        if(index>=65536)return false;
        s.poolIndex=static_cast<std::uint32_t>(index);++matches;
    }
    if(matches!=1||!read(s.registry+0x10,&indices,8)||!indices||
       !read(s.registry+0x18,&capacityFlags,4)||!read(s.registry+0x20,&liveEnd,8)||
       !read(s.registry+0x28,&activeEnd,8)||!read(s.registry+0x30,&s.liveCount,4)||
       !read(s.registry+0x38,&s.activeCount,4)||s.liveCount>65536||s.liveCount>(capacityFlags&0xffffff)||
       s.activeCount>s.liveCount||liveEnd!=indices+s.liveCount*2||activeEnd!=indices+s.activeCount*2)return false;
    std::vector<std::uint16_t> live(s.liveCount);
    if(s.liveCount&&!read(indices,live.data(),live.size()*2))return false;
    if(!std::is_sorted(live.begin(),live.begin()+s.activeCount)||
       !std::is_sorted(live.begin()+s.activeCount,live.end())||
       std::count(live.begin(),live.end(),static_cast<std::uint16_t>(s.poolIndex))!=1)return false;
    s.activeIndices.assign(live.begin(),live.begin()+s.activeCount);
    s.schedulerActive=std::binary_search(s.activeIndices.begin(),s.activeIndices.end(),static_cast<std::uint16_t>(s.poolIndex));
    // External probes race native updates. Reject changed snapshots rather than
    // publishing a mixture of pre-pause and post-pause scheduler state.
    std::uintptr_t check=0;std::uint32_t word=0;
    std::vector<std::uint16_t> again(s.liveCount);
    if(worldIdentityEntity(read,game,handle)!=s.actor||
       !read(s.component,&check,8)||check!=expectedVtable||
       !read(s.component+0x10,&check,8)||check!=s.actor||!read(s.component+0x1c,&word,4)||word!=s.flags||
       !read(game+0x1e8960,&check,8)||check!=s.manager||
       !read(s.registry+0x10,&check,8)||check!=indices||
       !read(s.registry+0x30,&word,4)||word!=s.liveCount||!read(s.registry+0x38,&word,4)||word!=s.activeCount||
       (s.liveCount&&!read(indices,again.data(),again.size()*2))||again!=live)return false;
    if(kind==NativeAiKind::IdleBreak) {
        std::uint16_t autoPlay=0;
        if(!read(s.component+0x20,&word,4)||word!=s.idleState||!read(s.component+0x118,&autoPlay,2)||autoPlay!=s.idleAutoPlay)return false;
    }
    out=std::move(s);return true;
}
template<class Read> bool readNativeAiDecision(Read read,std::uintptr_t base,std::uintptr_t game,
                                             std::uint32_t handle,NativeAiDecisionState& out) {
    return readNativeAiComponent(read,base,game,handle,NativeAiKind::Decision,out);
}
