#pragma once
#include "friendly_fire.h"
// Exact-image3b00a0 checks native assassination eligibility. Its helper3b0000
// resolves the source PlayerStates from context+20. Reject only the current
// registered partner with a reciprocal current local PlayerStates context.
// Native movement collision, other NPCs and world interactions are untouched.
template<class Read> bool currentPartnerAssassinationTarget(Read read,std::uintptr_t base,
        const CoopDamageAvatar& avatar,std::uintptr_t context,std::uintptr_t candidate){
    if(!context||candidate!=avatar.actor||!validCoopDamageAvatar(read,base,avatar))return false;
    const auto player=worldIdentityEntity(read,avatar.game,1);
    std::uintptr_t states=0,list=0,v=0;unsigned count=0,phase=0,pending=0;
    const auto eq=[&](std::uintptr_t at,std::uintptr_t expected){return read(at,&v,8)&&v==expected;};
    if(!player||!eq(player,base+0x1c5e428)||!read(avatar.game+0x341058,&phase,4)||phase!=3||
       !read(avatar.game+0x1f7400,&pending,4)||pending||!read(context+0x20,&states,8)||!states||
       !eq(states,base+0x1c73d58)||!eq(states+0x10,player)||
       !read(player+0x268,&list,8)||!list||!read(player+0x270,&count,4)||
       !(count&0xffffff)||(count&0xffffff)>256)return false;
    std::array<std::uintptr_t,256> members{};unsigned matches=0,index=0;
    if(!read(list,members.data(),(count&0xffffff)*8))return false;
    for(unsigned i=0;i<(count&0xffffff);++i)if(members[i]==states){++matches;index=i;}
    unsigned again=0;
    return matches==1&&eq(context+0x20,states)&&eq(states,base+0x1c73d58)&&eq(states+0x10,player)&&
        eq(player+0x268,list)&&read(player+0x270,&again,4)&&again==count&&eq(list+index*8,states)&&
        read(avatar.game+0x341058,&again,4)&&again==phase&&read(avatar.game+0x1f7400,&again,4)&&!again&&
        worldIdentityEntity(read,avatar.game,1)==player&&validCoopDamageAvatar(read,base,avatar);
}
