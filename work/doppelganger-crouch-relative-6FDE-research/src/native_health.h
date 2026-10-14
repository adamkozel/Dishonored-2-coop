#pragma once
#include "world_identity.h"
#include <array>
// Read-only retail 1.77.9.0 health reconstruction. Native 717d50 reads +20
// and divides by the virtual maximum; 7197f0 derives that maximum from the
// declaration, owner suppression flag, and bounded attribute entries.
// A zero value alone does not establish a native death/game-over transition.
struct NativeHealth {
    std::uintptr_t game=0,actor=0,component=0,declaration=0,attributes=0,table=0,list=0,logicStack=0;
    std::uint32_t handle=0,componentCount=0,attributeCount=0,logicCount=0,logic=0;
    std::int32_t maximumIndex=-1,suppressedIndex=-1;
    unsigned char ownerFlags=0;
    float current=0,maximum=0,baseMaximum=0,suppressedMaximum=0;
    std::array<float,4> attributeValues{}; // max base/current, suppression base/current
};
template<class Read> bool readNativeHealthFrame(Read read,std::uintptr_t base,std::uint32_t handle,NativeHealth& out,bool loadingPlayer=false){
    NativeHealth s{};s.handle=handle;std::uintptr_t vt=0,owner=0;unsigned phase=0,pending=0;
    if(!read(base+0x39a5290,&s.game,8)||!s.game||!read(s.game,&vt,8)||vt!=base+0x1c940a8||
       !read(s.game+0x341058,&phase,4)||phase!=3||!read(s.game+0x1f6ed0,&owner,8)||owner!=s.game||
       !read(s.game+0x1f73f0,&s.logicStack,8)||!s.logicStack||!read(s.game+0x1f73f8,&s.logicCount,4)||
       !(s.logicCount&0xffffff)||(s.logicCount&0xffffff)>32||!read(s.logicStack+((s.logicCount&0xffffff)-1)*4,&s.logic,4)||
       (loadingPlayer?(handle!=1||s.logic!=15):(s.logic!=1&&s.logic!=2&&s.logic!=13))||!read(s.game+0x1f7400,&pending,4)||pending)return false;
    s.actor=worldIdentityEntity(read,s.game,handle);
    if(!s.actor||!read(s.actor,&vt,8)||vt!=base+(handle==1?0x1c5e428:0x1c5e108)||
       !read(s.actor+0x268,&s.list,8)||!s.list||!read(s.actor+0x270,&s.componentCount,4)||
       !(s.componentCount&0xffffff)||(s.componentCount&0xffffff)>256||!read(s.actor+0xd1,&s.ownerFlags,1))return false;
    std::uintptr_t foundAttributes=0;
    for(unsigned i=0;i<(s.componentCount&0xffffff);++i){
        std::uintptr_t c=0;if(!read(s.list+i*8,&c,8)||!c||!read(c,&vt,8))return false;
        if(vt==base+0x1c51588){if(s.component)return false;s.component=c;}
        if(vt==base+0x1b99028||vt==base+0x1ca7938){if(foundAttributes)return false;foundAttributes=c;}
    }
    if(!s.component||!read(s.component+0x10,&owner,8)||owner!=s.actor||
       !read(s.component+8,&s.declaration,8)||!s.declaration||!read(s.declaration,&vt,8)||vt!=base+0x1c51698||
       !read(s.component+0x20,&s.current,4)||!read(s.declaration+0x68,&s.baseMaximum,4)||
       !read(s.declaration+0xb4,&s.suppressedMaximum,4)||!read(s.component+0x68,&s.attributes,8)||
       !read(s.component+0xb0,&s.maximumIndex,4)||!read(s.component+0xb4,&s.suppressedIndex,4)||
       s.maximumIndex<-1||s.suppressedIndex<-1)return false;
    if(s.attributes){
        if(s.attributes!=foundAttributes||!read(s.attributes+0x10,&owner,8)||owner!=s.actor||
           !read(s.attributes+0x48,&s.table,8)||!read(s.attributes+0x50,&s.attributeCount,4)||(s.attributeCount&0xffffff)>4096)return false;
        for(unsigned i=0;i<2;++i){const int index=i?s.suppressedIndex:s.maximumIndex;if(index<0)continue;
            if(!s.table||index>=static_cast<int>(s.attributeCount&0xffffff)||
               !read(s.table+static_cast<std::uintptr_t>(index)*40+8,&s.attributeValues[i*2],4)||
               !read(s.table+static_cast<std::uintptr_t>(index)*40+0x24,&s.attributeValues[i*2+1],4))return false;
        }
    }
    for(float f:s.attributeValues)if(!std::isfinite(f)||std::fabs(f)>1000000.f)return false;
    for(float f:{s.current,s.baseMaximum,s.suppressedMaximum})if(!std::isfinite(f)||f<0||f>1000000.f)return false;
    s.maximum=s.baseMaximum;
    if(s.ownerFlags&0x20)s.maximum=s.suppressedMaximum;
    else if(s.attributes&&(s.suppressedIndex<0||s.attributeValues[2]==s.attributeValues[3])&&s.maximumIndex>=0)s.maximum+=s.attributeValues[1];
    if(!std::isfinite(s.maximum)||s.maximum<0||s.maximum>1000000.f)return false;
    out=s;return true;
}
template<class Read> bool readNativeHealth(Read read,std::uintptr_t base,std::uint32_t handle,NativeHealth& out,bool loadingPlayer=false){
    // Loading reads are explicitly scoped to the player in native state 15.
    // Gameplay damage/AI callers retain their existing transition refusal.
    NativeHealth a{},b{};if(!readNativeHealthFrame(read,base,handle,a,loadingPlayer)||!readNativeHealthFrame(read,base,handle,b,loadingPlayer))return false;
    if(a.game!=b.game||a.actor!=b.actor||a.component!=b.component||a.declaration!=b.declaration||a.attributes!=b.attributes||a.table!=b.table||
       a.list!=b.list||a.logicStack!=b.logicStack||a.componentCount!=b.componentCount||a.attributeCount!=b.attributeCount||a.logicCount!=b.logicCount||
       a.logic!=b.logic||a.ownerFlags!=b.ownerFlags||a.current!=b.current||a.maximum!=b.maximum||a.baseMaximum!=b.baseMaximum||
       a.suppressedMaximum!=b.suppressedMaximum||a.maximumIndex!=b.maximumIndex||a.suppressedIndex!=b.suppressedIndex||a.attributeValues!=b.attributeValues)return false;
    out=a;return true;
}
