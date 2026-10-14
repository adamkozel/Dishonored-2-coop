#pragma once
#include "friendly_fire.h"
#include "native_ai_state.h"

// A locally owned presentation clone never becomes an autonomous NPC when
// the remote player pauses, disconnects, dies, or stops movement playback.
// Keep the existing native pause reference while its exact local owner lives.
// This grants no movement permission and never acquires an additional ref.
template<class Read> bool ownedAvatarAiReference(Read read,std::uintptr_t base,
    const CoopDamageAvatar& avatar,const NativeAiComponentState& held,
    std::uint64_t heldEpoch,std::uint64_t currentEpoch) {
    if(!heldEpoch||heldEpoch!=currentEpoch||held.kind!=NativeAiKind::Decision||
       held.game!=avatar.game||held.actor!=avatar.actor||held.handle!=avatar.handle||
       !held.component||!held.manager||!validCoopDamageAvatar(read,base,avatar))return false;
    std::uintptr_t manager=0,list=0,vt=0,owner=0;unsigned count=0,flags=0,matches=0;
    if(!read(avatar.game+0x1e8960,&manager,8)||manager!=held.manager||
       !read(avatar.actor+0x268,&list,8)||!list||!read(avatar.actor+0x270,&count,4)||
       !(count&0xffffff)||(count&0xffffff)>256)return false;
    for(unsigned i=0;i<(count&0xffffff);++i){
        std::uintptr_t component=0;if(!read(list+i*8,&component,8))return false;
        if(component==held.component)++matches;
    }
    if(matches!=1||!read(held.component,&vt,8)||vt!=base+nativeAiVtable(NativeAiKind::Decision)||
       !read(held.component+0x10,&owner,8)||owner!=avatar.actor||
       !read(held.component+0x1c,&flags,4)||(flags&0xc)!=8||!((flags>>6)&0x1fffff))return false;
    std::uintptr_t again=0;unsigned countAgain=0,flagsAgain=0;
    return read(avatar.actor+0x268,&again,8)&&again==list&&
        read(avatar.actor+0x270,&countAgain,4)&&countAgain==count&&
        read(held.component+0x10,&again,8)&&again==owner&&
        read(held.component+0x1c,&flagsAgain,4)&&flagsAgain==flags&&
        validCoopDamageAvatar(read,base,avatar);
}
