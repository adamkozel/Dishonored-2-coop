#pragma once
#include "world_identity.h"
#include <array>

// Exact executable: arkCpntUsableWithStateList, not ArkDoor/Openable.
// Observe native state and animation ownership before any state request.
struct NativeUsableEntry {
    unsigned char enter=255,exit=255,applyOnFinish=0,customPrompt=0;
    float transitionSeconds=0;
    bool operator==(const NativeUsableEntry& b)const{return enter==b.enter&&exit==b.exit&&applyOnFinish==b.applyOnFinish&&customPrompt==b.customPrompt&&transitionSeconds==b.transitionSeconds;}
};
struct NativeUsable {
    std::uintptr_t game=0,actor=0,component=0,declaration=0,definition=0,animation=0,stateArray=0;
    unsigned handle=0,generation=0,animationGeneration=0,stateCount=0,instigator=0;
    int current=-1,pending=-1,initial=-1;
    unsigned char enabled=0,flags=0;
    std::string name,definitionName,declarationName;
    std::array<float,3> position{};
    std::array<NativeUsableEntry,32> entries{};
    bool transitioning()const{return (flags&2)!=0;}
};
inline bool sameNativeUsableIdentity(const NativeUsable& a,const NativeUsable& b){
    return a.game==b.game&&a.actor==b.actor&&a.handle==b.handle&&a.component==b.component&&a.generation==b.generation&&
        a.declaration==b.declaration&&a.definition==b.definition&&a.animation==b.animation&&a.animationGeneration==b.animationGeneration&&
        a.stateArray==b.stateArray&&a.stateCount==b.stateCount&&a.initial==b.initial&&a.name==b.name&&
        a.definitionName==b.definitionName&&a.declarationName==b.declarationName&&a.entries==b.entries;
}
template<class Read> bool readNativeUsableFrame(Read read,std::uintptr_t base,std::uintptr_t game,unsigned handle,NativeUsable& out,unsigned* stage=nullptr){
    NativeUsable s{};s.game=game;s.handle=handle;std::uintptr_t v=0,p=0,list=0;unsigned count=0,flags=0;
    const auto eq=[&](std::uintptr_t at,std::uintptr_t expected){return read(at,&v,8)&&v==expected;};
    if(stage)*stage=1;
    if(!game||!eq(game,base+0x1c940a8)||!(s.actor=worldIdentityEntity(read,game,handle))||!eq(s.actor,base+0x1c5ec38)||
       !read(s.actor+0x20,&p,8)||!worldIdentityString(read,p,s.name)||!read(s.actor+0x2e8,&s.definition,8)||!s.definition||!eq(s.definition,base+0x1ab58f8)||
       !read(s.definition+8,&p,8)||!worldIdentityString(read,p,s.definitionName)||!read(s.actor+0x268,&list,8)||!list||
       !read(s.actor+0x270,&count,4)||!(count&0xffffff)||(count&0xffffff)>256)return false;
    if(stage)*stage=2;
    std::array<std::uintptr_t,256> members{};if(!read(list,members.data(),(count&0xffffff)*8))return false;
    for(unsigned i=0;i<(count&0xffffff);++i){if(!members[i]||!read(members[i],&v,8))return false;if(v==base+0x1c5bfc8){if(s.component)return false;s.component=members[i];}}
    if(!s.component||!eq(s.component+0x10,s.actor)||!read(s.component+0x18,&s.generation,4)||s.generation==~0u||
       !read(s.component+0x1c,&flags,4)||(flags&12)!=8||!read(s.component+8,&s.declaration,8)||!s.declaration||!eq(s.declaration,base+0x1c5c0c8)||
       !read(s.declaration+8,&p,8)||!worldIdentityString(read,p,s.declarationName))return false;
    if(stage)*stage=3;
    if(!read(s.declaration+0x70,&s.stateArray,8)||!s.stateArray||!read(s.declaration+0x78,&s.stateCount,4)||
       !(s.stateCount&=0xffffff)||s.stateCount>s.entries.size()||!read(s.declaration+0x80,&s.initial,4)||s.initial<0||s.initial>=static_cast<int>(s.stateCount)||
       !read(s.component+0x20,&s.enabled,1)||s.enabled>1||!read(s.component+0x28,&s.current,4)||s.current<0||s.current>=static_cast<int>(s.stateCount)||
       !read(s.component+0x2c,&s.pending,4)||s.pending< -1||s.pending>=static_cast<int>(s.stateCount)||
       !read(s.component+0x30,&s.flags,1)||!read(s.component+0xd0,&s.instigator,4))return false;
    if(stage)*stage=4;
    for(unsigned i=0;i<s.stateCount;++i){auto& e=s.entries[i];const auto at=s.stateArray+i*0x80;
        if(!read(at+8,&e.enter,1)||!read(at+9,&e.exit,1)||!read(at+0xc,&e.transitionSeconds,4)||
           !read(at+0x50,&e.applyOnFinish,1)||e.applyOnFinish>1||!read(at+0x51,&e.customPrompt,1)||e.customPrompt>1||
           !std::isfinite(e.transitionSeconds)||e.transitionSeconds<0||e.transitionSeconds>60)return false;
    }
    if(stage)*stage=5;
    if(!read(s.component+0xd8,&s.animation,8)||!s.animation||!eq(s.animation,base+0x1b7f408)||!eq(s.animation+0x10,s.actor)||
       !read(s.animation+0x18,&s.animationGeneration,4)||s.animationGeneration==~0u||!read(s.animation+0x1c,&flags,4)||(flags&12)!=8)return false;
    unsigned matches=0;for(unsigned i=0;i<(count&0xffffff);++i)if(members[i]==s.animation)++matches;if(matches!=1)return false;
    if(stage)*stage=6;
    if(!read(s.actor+0x300,s.position.data(),12))return false;for(float f:s.position)if(!std::isfinite(f))return false;
    if(worldIdentityEntity(read,game,handle)!=s.actor)return false;
    out=std::move(s);if(stage)*stage=0;return true;
}
template<class Read> bool readNativeUsable(Read read,std::uintptr_t base,std::uintptr_t game,unsigned handle,NativeUsable& out,unsigned* stage=nullptr){
    NativeUsable a{},b{};
    if(!readNativeUsableFrame(read,base,game,handle,a,stage)||!readNativeUsableFrame(read,base,game,handle,b,stage))return false;
    if(stage)*stage=7;
    if(!sameNativeUsableIdentity(a,b)||a.current!=b.current||a.pending!=b.pending||a.enabled!=b.enabled||a.flags!=b.flags||a.instigator!=b.instigator)return false;
    out=std::move(b);if(stage)*stage=0;return true;
}
inline bool nativeUsableTargetValid(const NativeUsable& s,unsigned target){return s.enabled&&s.stateCount>0&&target<s.stateCount;}
inline bool nativeUsableAtTarget(const NativeUsable& s,unsigned target){return nativeUsableTargetValid(s,target)&&!s.transitioning()&&s.pending== -1&&s.current==static_cast<int>(target);}
