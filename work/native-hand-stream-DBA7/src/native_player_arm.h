#pragma once
#include "world_identity.h"

// Read-only identity for the separate first-person arm entities. They share
// arkPlayer's vtable, but are not the local player and must never pass ordinary
// player/NPC ownership checks merely because that vtable matches.
struct NativePlayerArm {
    std::uintptr_t game=0,actor=0,player=0,children=0,definition=0,playerDefinition=0;
    std::uint32_t handle=0,childCount=0,character=0,side=0; // 1=left, 2=right
};
inline bool sameNativePlayerArm(const NativePlayerArm& a,const NativePlayerArm& b){
    return a.game==b.game&&a.actor==b.actor&&a.player==b.player&&a.children==b.children&&a.definition==b.definition&&
        a.playerDefinition==b.playerDefinition&&a.handle==b.handle&&a.childCount==b.childCount&&a.character==b.character&&a.side==b.side;
}
template<class Read> bool readNativePlayerArm(Read read,std::uintptr_t base,std::uintptr_t game,std::uint32_t handle,NativePlayerArm& out){
    NativePlayerArm s{};s.game=game;s.handle=handle;
    if(handle==1)return false;
    s.actor=worldIdentityEntity(read,game,handle);s.player=worldIdentityEntity(read,game,1);
    std::uintptr_t vt=0,name=0;std::uint32_t parent=0,count=0;std::string label,definition,body;
    if(!s.actor||!s.player||s.actor==s.player||!read(s.actor,&vt,8)||vt!=base+0x1c5e428||
       !read(s.player,&vt,8)||vt!=base+0x1c5e428||!read(s.actor+0xac,&parent,4)||parent!=1||
       !read(s.actor+0x20,&name,8)||!worldIdentityString(read,name,label))return false;
    if(label=="left_arm_1")s.side=1;else if(label=="right_arm_2")s.side=2;else return false;
    if(!read(s.actor+0x2e8,&s.definition,8)||!s.definition||!read(s.definition,&vt,8)||vt!=base+0x1ab58f8||
       !read(s.definition+8,&name,8)||!worldIdentityString(read,name,definition)||
       !read(s.player+0x2e8,&s.playerDefinition,8)||!s.playerDefinition||!read(s.playerDefinition,&vt,8)||vt!=base+0x1ab58f8||
       !read(s.playerDefinition+8,&name,8)||!worldIdentityString(read,name,body))return false;
    const char* role=nullptr;
    if(body=="models/characters/player/corvo/body.def"){s.character=1;role="corvo";}
    else if(body=="models/characters/player/emily/body.def"){s.character=2;role="emily";}else return false;
    const std::string expected=std::string("models/characters/player/")+role+(s.side==1?"/left_arm.def":"/right_arm.def");
    if(definition!=expected||!read(s.player+0xb0,&s.children,8)||!s.children||!read(s.player+0xb8,&count,4)||
       !(count&0xffffff)||(count&0xffffff)>32)return false;
    s.childCount=count&0xffffff;unsigned matches=0;
    for(unsigned i=0;i<s.childCount;++i){std::uintptr_t child=0;if(!read(s.children+i*8,&child,8))return false;if(child==s.actor)++matches;}
    if(matches!=1)return false;
    std::uintptr_t again=0;std::uint32_t number=0;
    if(worldIdentityEntity(read,game,handle)!=s.actor||worldIdentityEntity(read,game,1)!=s.player||
       !read(s.actor+0xac,&number,4)||number!=1||!read(s.player+0xb0,&again,8)||again!=s.children||
       !read(s.player+0xb8,&number,4)||number!=count||!read(s.actor+0x2e8,&again,8)||again!=s.definition||
       !read(s.player+0x2e8,&again,8)||again!=s.playerDefinition)return false;
    matches=0;for(unsigned i=0;i<s.childCount;++i){if(!read(s.children+i*8,&again,8))return false;if(again==s.actor)++matches;}
    if(matches!=1)return false;out=s;return true;
}
