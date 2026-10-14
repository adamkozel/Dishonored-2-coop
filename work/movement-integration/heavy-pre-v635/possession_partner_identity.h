#pragma once
#include "friendly_fire.h"
#include "native_player_possession_item.h"
// Native3c22f0 returns a borrowed0x30-byte ranked sensor record.448200
// subsequently resolves its handle and publishes the native target/FX.
//373 selected the exact registered Emily avatar and entered its native host.
//374 scopes exclusion to the selected Corvo power's reciprocal sensor mode.
template<class Read> bool currentPossessionPartnerPick(Read read,std::uintptr_t base,
        const CoopDamageAvatar& avatar,std::uintptr_t mode,std::uintptr_t record){
    unsigned handle=0;std::uintptr_t proxy=0;
    if(!mode||!record||!avatar.actor||!read(record+0x18,&handle,4)||handle!=avatar.handle||
       !read(record+0x20,&proxy,8)||proxy||!validCoopDamageAvatar(read,base,avatar))return false;
    NativePossessionItem item{};
    if(!readNativePossessionItem(read,base,item,true)||!item.instantiated||!item.started||
       item.player.character!=1||item.player.sword.game!=avatar.game)return false;
    const auto player=item.player.sword.actor;
    const auto eq=[&](std::uintptr_t at,std::uintptr_t value){std::uintptr_t x=0;return read(at,&x,8)&&x==value;};
    unsigned phase=0,pending=0,generation=0,actual=0;
    std::uintptr_t sensor=0,list=0;unsigned count=0;
    if(!read(avatar.game+0x341058,&phase,4)||phase!=3||!read(avatar.game+0x1f7400,&pending,4)||pending||
       !read(item.component+0xe0,&sensor,8)||!sensor||!eq(item.component+0x40,sensor)||
       !read(item.component+0xd8,&generation,4)||generation==~0u||!read(sensor+0x18,&actual,4)||actual!=generation||
       !eq(sensor,base+0x1c5b108)||!eq(sensor+0x10,player)||!eq(sensor+0xe8,mode)||
       !eq(mode,base+0x1b41918)||!eq(mode+0x10,player)||
       !read(player+0x268,&list,8)||!list||!read(player+0x270,&count,4)||!(count&0xffffff)||(count&0xffffff)>256)return false;
    std::array<std::uintptr_t,256> members{};count&=0xffffff;
    if(!read(list,members.data(),count*8))return false;
    unsigned sensors=0,modes=0;
    for(unsigned i=0;i<count;++i){sensors+=members[i]==sensor;modes+=members[i]==mode;}
    if(sensors!=1||modes!=1)return false;
    std::uintptr_t records=0,ownerField=0;unsigned flags=0;
    for(unsigned kind=0;kind<2;++kind){ // Regular entity lists; proxy targets remain untouched.
        const auto field=mode+0x110+kind*0x58;std::uintptr_t entries=0;unsigned n=0;
        if(!read(field,&entries,8)||!read(field+8,&n,4)||(n&0xffffff)>512||((n&0xffffff)&&!entries))return false;
        if(entries&&record>=entries&&record-entries<std::uintptr_t(n&0xffffff)*0x30&&(record-entries)%0x30==0){
            if(ownerField)return false;records=entries;flags=n;ownerField=field;
        }
    }
    if(!ownerField)return false;
    unsigned flagsAgain=0;
    return eq(ownerField,records)&&read(ownerField+8,&flagsAgain,4)&&flagsAgain==flags&&
        read(record+0x18,&actual,4)&&actual==handle&&eq(record+0x20,0)&&
        eq(item.component+0xe0,sensor)&&eq(sensor+0xe8,mode)&&eq(mode+0x10,player)&&
        read(sensor+0x18,&actual,4)&&actual==generation&&validCoopDamageAvatar(read,base,avatar);
}
