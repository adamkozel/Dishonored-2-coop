#pragma once
#include "world_identity.h"
constexpr const char* kDoppelgangerVisualDefinition="models/powers/d2coop/doppelganger_visual.def";
struct NativeDoppelgangerVisual {
    std::uintptr_t actor=0,definition=0,animation=0,frame=0,render=0,model=0;
    unsigned handle=0,animationGeneration=0,frameGeneration=0;
};
template<class Read> bool readNativeDoppelgangerVisual(Read read,std::uintptr_t base,std::uintptr_t game,unsigned handle,NativeDoppelgangerVisual& out){
    NativeDoppelgangerVisual s{};s.handle=handle;s.actor=worldIdentityEntity(read,game,handle);
    const auto eq=[&](std::uintptr_t at,std::uintptr_t wanted){std::uintptr_t x=0;return read(at,&x,8)&&x==wanted;};
    const auto name=[&](std::uintptr_t d,const char* wanted){std::uintptr_t p=0;std::string text;return d&&read(d+8,&p,8)&&worldIdentityString(read,p,text)&&text==wanted;};
    unsigned parent=0,count=0;std::uintptr_t list=0;
    if(handle==1||!s.actor||!eq(s.actor,base+0x1c87d58)||!read(s.actor+0xac,&parent,4)||parent!=0x3fff0010||
       !read(s.actor+0x2e8,&s.definition,8)||!eq(s.definition,base+0x1ab58f8)||!name(s.definition,kDoppelgangerVisualDefinition)||
       !read(s.actor+0x268,&list,8)||!list||!read(s.actor+0x270,&count,4)||(count&0xffffff)!=4)return false;
    constexpr std::uintptr_t types[]={0x1b80408,0x1c59ac8,0x1c4e588,0x1c573e8};
    constexpr const char* names[]={"components/d2coop/doppelganger_visual.cpntanimevolved","components/base/midnightproxy_default.cpntmidnightproxy","components/base/audio_default.cpntaudio","components/base/referenceframe_default.cpntreferenceframe"};
    unsigned seen=0;
    for(unsigned i=0;i<4;++i){std::uintptr_t c=0,vt=0,d=0;if(!read(list+i*8,&c,8)||!c||!read(c,&vt,8)||!eq(c+0x10,s.actor)||!read(c+8,&d,8))return false;
        unsigned k=0;while(k<4&&vt!=base+types[k])++k;if(k==4||(seen&(1u<<k))||!name(d,names[k]))return false;seen|=1u<<k;
        if(k==0){s.animation=c;if(!read(c+0x18,&s.animationGeneration,4))return false;}
        if(k==3){s.frame=c;if(!read(c+0x18,&s.frameGeneration,4))return false;}
    }
    if(seen!=15||!eq(s.actor+0x358,s.frame)||!read(s.animation+0x38,&s.render,8)||!s.render||!eq(s.render,base+0x1b0d628)||
       !read(s.render+0x1fe8,&s.model,8)||!s.model||!eq(s.model,base+0x1b05218)||!name(s.model,"models/characters/female/emily/head01/emily_head01.md6")||
       worldIdentityEntity(read,game,handle)!=s.actor||!eq(s.animation+0x10,s.actor)||!eq(s.frame+0x10,s.actor))return false;
    out=s;return true;
}
