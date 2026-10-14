#pragma once
#include "assassination_partner_identity.h"
// Exact image3b8020/3b8080 resolve interaction-sensor handles+8c/+88.
// Native sword state uses these independently of assassination eligibility.
// Only suppress a returned current partner, with reciprocal live sensor and
// local PlayerStates ownership; leave raw target records and all NPCs intact.
template<class Read> bool currentPartnerInteractionPick(Read read,std::uintptr_t base,
        const CoopDamageAvatar& avatar,std::uintptr_t sensor,std::uintptr_t candidate,unsigned field){
    if((field!=0x88&&field!=0x8c)||!currentPartnerAssassinationTarget(read,base,avatar,sensor,candidate))return false;
    const auto player=worldIdentityEntity(read,avatar.game,1);
    std::uintptr_t v=0,list=0;unsigned count=0,generation=0,handle=0;
    const auto eq=[&](std::uintptr_t at,std::uintptr_t expected){return read(at,&v,8)&&v==expected;};
    if(!eq(sensor,base+0x1b403b8)||!eq(sensor+0x10,player)||!read(sensor+0x18,&generation,4)||generation==~0u||
       !read(sensor+field,&handle,4)||handle!=avatar.handle||!read(player+0x268,&list,8)||!list||
       !read(player+0x270,&count,4)||!(count&0xffffff)||(count&0xffffff)>256)return false;
    std::array<std::uintptr_t,256> members{};unsigned matches=0,index=0;
    if(!read(list,members.data(),(count&0xffffff)*8))return false;
    for(unsigned i=0;i<(count&0xffffff);++i)if(members[i]==sensor){++matches;index=i;}
    unsigned again=0;
    return matches==1&&eq(sensor,base+0x1b403b8)&&eq(sensor+0x10,player)&&
        read(sensor+0x18,&again,4)&&again==generation&&read(sensor+field,&again,4)&&again==handle&&
        eq(player+0x268,list)&&read(player+0x270,&again,4)&&again==count&&eq(list+index*8,sensor)&&
        currentPartnerAssassinationTarget(read,base,avatar,sensor,candidate);
}
