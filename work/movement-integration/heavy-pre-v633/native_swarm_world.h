#pragma once
#include "native_swarm_equipment_owner.h"
#include <algorithm>
#include <array>

// Read-only, exact retail executable. The equipment +d4 relationship selects
// the committed swarm. Pool indices (not the first N slots) select agents.
// Evidence: native v282; 1406bab70/1406b4a20; init 1406baf20; cleanup 1406b33e0.
// No spawn, animation, AI, effect or damage function is invoked here.
struct NativeSwarmAgent {
    std::uintptr_t rat=0,actor=0,frame=0,animation=0,list=0,drivingFrame=0,frameDriver=0;
    unsigned index=0,handle=0,incarnation=0,animationGeneration=0,frameGeneration=0,count=0;
    unsigned state=0,animationFlags=0,frameFlags=0;
    float scale=0;
    std::array<float,12> transform{};
};
struct NativeSwarmWorld {
    NativeSwarmItem equipment{};
    std::uintptr_t actor=0,definition=0,component=0,declaration=0,list=0;
    std::uintptr_t pool=0,pipeline=0,indicesPointer=0,transforms=0;
    unsigned handle=0x3fff0010,flags=0,count=0,indexCount=0,capacity=0;
    std::vector<unsigned> indices;
    std::vector<NativeSwarmAgent> agents;
    bool present=false;
};
inline bool validSwarmTransform(const std::array<float,12>& t){
    for(unsigned i=0;i<12;++i)if(!std::isfinite(t[i])||std::fabs(t[i])>(i<3?100000.f:1.01f))return false;
    for(unsigned i=0;i<3;++i)for(unsigned j=i;j<3;++j){float dot=0;
        for(unsigned k=0;k<3;++k)dot+=t[3+i*3+k]*t[3+j*3+k];
        if(std::fabs(dot-(i==j?1.f:0.f))>.02f)return false;
    }
    const float det=t[3]*(t[7]*t[11]-t[10]*t[8])-t[6]*(t[4]*t[11]-t[10]*t[5])+t[9]*(t[4]*t[8]-t[7]*t[5]);
    return det>.98f&&det<1.02f;
}
inline bool sameSwarmAgentIdentity(const NativeSwarmAgent& a,const NativeSwarmAgent& b){
    return a.rat==b.rat&&a.actor==b.actor&&a.frame==b.frame&&a.animation==b.animation&&a.list==b.list&&
        a.index==b.index&&a.handle==b.handle&&a.incarnation==b.incarnation&&a.count==b.count&&
        a.animationGeneration==b.animationGeneration&&a.frameGeneration==b.frameGeneration&&
        a.animationFlags==b.animationFlags&&a.frameFlags==b.frameFlags&&a.drivingFrame==b.drivingFrame&&a.frameDriver==b.frameDriver;
}
template<class Read> bool readNativeSwarmWorldFrame(Read read,std::uintptr_t base,NativeSwarmWorld& s,unsigned* stage=nullptr,const NativeSwarmItem* known=nullptr){
    const auto mark=[&](unsigned n){if(stage)*stage=n;};mark(1);
    if(!(known?readCurrentNativeSwarmEquipment(read,base,*known,s.equipment):readNativeSwarmItem(read,base,s.equipment)))return false;
    if(!s.equipment.instantiated){mark(0);return true;}
    if(!read(s.equipment.component+0xd4,&s.handle,4))return false;
    if(s.handle==0x3fff0010){mark(0);return true;}
    const auto game=s.equipment.player.sword.game;
    const auto eq=[&](std::uintptr_t p,std::uintptr_t expected){std::uintptr_t x=0;return read(p,&x,8)&&x==expected;};
    const auto named=[&](std::uintptr_t d,std::uintptr_t vt,const char* name){std::uintptr_t p=0;std::string text;
        return d&&eq(d,base+vt)&&read(d+8,&p,8)&&worldIdentityString(read,p,text)&&text==name;};
    mark(2);s.actor=worldIdentityEntity(read,game,s.handle);
    unsigned parent=0;
    if(!s.actor||s.actor==s.equipment.entity||!eq(s.actor,base+0x1c44b98)||
       !read(s.actor+0xac,&parent,4)||parent!=1||
       !read(s.actor+0x2e8,&s.definition,8)||!named(s.definition,0x1ab58f8,"models/characters/animals/rat/devouring_swarm.def")||
       !read(s.actor+0x268,&s.list,8)||!s.list||!read(s.actor+0x270,&s.count,4)||!(s.count&0xffffff)||(s.count&0xffffff)>64)return false;
    for(unsigned i=0;i<(s.count&0xffffff);++i){std::uintptr_t c=0,vt=0;
        if(!read(s.list+i*8,&c,8)||!c||!read(c,&vt,8))return false;
        if(vt==base+0x1c42ab8){if(s.component)return false;s.component=c;}
    }
    mark(3);
    if(!s.component||!eq(s.component+0x10,s.actor)||!eq(s.component+0x20,base+0x1c42c30)||
       !read(s.component+8,&s.declaration,8)||!named(s.declaration,0x1d25df8,"components/characters/animals/rat/devouring_swarm.cpntratswarm")||
       !read(s.component+0x1c,&s.flags,4)||(s.flags&0xc)!=8||
       !read(s.component+0x80,&s.pool,8)||!s.pool||!read(s.component+0x90,&s.indicesPointer,8)||
       !read(s.component+0x98,&s.indexCount,4)||(s.indexCount&0xffffff)>100||
       !read(s.component+0x138,&s.pipeline,8)||!s.pipeline||!eq(s.pipeline,base+0x1c44518))return false;
    mark(4);unsigned stride=0,pipelineStride=0,cap=0;
    if(!read(s.component+0x88,&stride,4)||stride!=0x108||!eq(s.pipeline+0x1a8,s.pool)||
       !read(s.pipeline+0x1b0,&pipelineStride,4)||pipelineStride!=stride||
       !read(s.pipeline+0x138,&s.transforms,8)||!s.transforms||
       !read(s.pipeline+0x140,&s.capacity,4)||!s.capacity||s.capacity>100||
       !read(s.pipeline+0x144,&cap,4)||cap<s.capacity||cap>100||
       s.pool>UINTPTR_MAX-s.capacity*0x108||s.transforms>UINTPTR_MAX-s.capacity*0x20)return false;
    const auto n=s.indexCount&0xffffff;
    if(n>s.capacity||(n&&!s.indicesPointer))return false;
    s.indices.resize(n);if(n&&!read(s.indicesPointer,s.indices.data(),n*4))return false;
    for(unsigned i=0;i<n;++i){
        mark(5);NativeSwarmAgent a{};a.index=s.indices[i];
        if(a.index>=s.capacity||std::count(s.indices.begin(),s.indices.end(),a.index)!=1)return false;
        a.rat=s.pool+a.index*0x108;
        mark(51);
        if(!eq(a.rat,base+0x1c442a8)||!eq(a.rat+0x18,s.component)||
           !read(a.rat+8,&a.drivingFrame,8)||
           !read(a.rat+0x10,&a.actor,8)||!a.actor||!eq(a.actor,base+0x1c44ce8)||
           !read(a.actor+0x358,&a.frame,8)||!a.frame||!eq(a.frame,base+0x1c573e8)||
           !read(a.rat+0x78,&a.animation,8)||!a.animation||!eq(a.animation,base+0x1b5b938))return false;
        mark(52);
        if(
           !read(a.actor+0xa8,&a.handle,4)||worldIdentityEntity(read,game,a.handle)!=a.actor||
           !read(a.actor+0xac,&parent,4)||parent!=s.handle||!eq(a.actor+0x360,a.rat)||
           !eq(a.frame+0x10,a.actor)||!eq(a.animation+0x10,a.actor)||
           !read(a.rat+0x44,&a.incarnation,4)||!read(a.rat+0x48,&a.state,4)||a.state>6||
           !read(a.frame+0x18,&a.frameGeneration,4)||!read(a.animation+0x18,&a.animationGeneration,4)||
           !read(a.frame+0x1c,&a.frameFlags,4)||(a.frameFlags&0xc)!=8||
           !read(a.animation+0x1c,&a.animationFlags,4)||(a.animationFlags&0xc)!=8)return false;
        // rat+8 is the frame this rat currently drives, not the only route to
        // its entity frame. Native 6b8fc0 restores that driver after a contextual
        // attachment. During state3 the frame can use its own +20 driver.
        mark(54);
        if(!read(a.frame+0xd8,&a.frameDriver,8)||
           !((a.drivingFrame==a.frame&&a.frameDriver==a.rat)||
             (!a.drivingFrame&&a.state==3&&a.frameDriver==a.frame+0x20)))return false;
        mark(6);
        if(!read(a.actor+0x268,&a.list,8)||!a.list||!read(a.actor+0x270,&a.count,4)||!(a.count&0xffffff)||(a.count&0xffffff)>16)return false;
        std::array<std::uintptr_t,16> members{};const auto count=a.count&0xffffff;
        if(!read(a.list,members.data(),count*8)||std::count(members.begin(),members.begin()+count,a.frame)!=1||
           std::count(members.begin(),members.begin()+count,a.animation)!=1)return false;
        // Read actual entity presentation, not the unsynchronized simulation
        // job array. Its reciprocal component supplies identity, not a second
        // coordinate snapshot to mix with this one.
        mark(7);std::array<float,12> again{};
        if(!read(a.actor+0x300,a.transform.data(),48)||!validSwarmTransform(a.transform)||
           !read(s.transforms+a.index*0x20+0x1c,&a.scale,4)||!std::isfinite(a.scale)||a.scale<.1f||a.scale>4.f||
           !read(a.actor+0x300,again.data(),48)||again!=a.transform)return false;
        if(std::any_of(s.agents.begin(),s.agents.end(),[&](const auto& old){return old.actor==a.actor||old.handle==a.handle||old.frame==a.frame||old.animation==a.animation;}))return false;
        s.agents.push_back(a);
    }
    mark(8);std::vector<unsigned> indices(n);unsigned countAgain=0,handleAgain=0;
    if(!eq(s.component+0x90,s.indicesPointer)||!read(s.component+0x98,&countAgain,4)||countAgain!=s.indexCount||
       (n&&(!read(s.indicesPointer,indices.data(),n*4)||indices!=s.indices))||
       !eq(s.component+0x80,s.pool)||!eq(s.component+0x138,s.pipeline)||!eq(s.pipeline+0x1a8,s.pool)||
       !eq(s.pipeline+0x138,s.transforms)||!eq(s.component+0x10,s.actor)||
       !read(s.equipment.component+0xd4,&handleAgain,4)||handleAgain!=s.handle||
       worldIdentityEntity(read,game,s.handle)!=s.actor)return false;
    s.present=true;mark(0);return true;
}
template<class Read> bool readNativeSwarmWorld(Read read,std::uintptr_t base,NativeSwarmWorld& out,unsigned* stage=nullptr,const NativeSwarmItem* known=nullptr){
    NativeSwarmWorld a{},b{};
    if(!readNativeSwarmWorldFrame(read,base,a,stage,known)||!readNativeSwarmWorldFrame(read,base,b,stage,known))return false;
    if(stage)*stage=9;
    if(!sameSwordEquipmentIdentity(a.equipment.player.sword,b.equipment.player.sword)||
       a.equipment.entry!=b.equipment.entry||a.equipment.handle!=b.equipment.handle||a.equipment.component!=b.equipment.component||
       a.equipment.declaration!=b.equipment.declaration||a.equipment.equipmentDeclaration!=b.equipment.equipmentDeclaration||
       a.equipment.quantity!=b.equipment.quantity||a.equipment.flags!=b.equipment.flags||a.equipment.parentDeclaration!=b.equipment.parentDeclaration||
       a.present!=b.present||a.handle!=b.handle||a.actor!=b.actor||a.definition!=b.definition||a.component!=b.component||
       a.declaration!=b.declaration||a.list!=b.list||a.flags!=b.flags||a.count!=b.count||a.indexCount!=b.indexCount||
       a.pool!=b.pool||a.pipeline!=b.pipeline||a.indicesPointer!=b.indicesPointer||a.transforms!=b.transforms||
       a.capacity!=b.capacity||a.indices!=b.indices||a.agents.size()!=b.agents.size())return false;
    for(unsigned i=0;i<a.agents.size();++i)if(!sameSwarmAgentIdentity(a.agents[i],b.agents[i]))return false;
    if(stage)*stage=0;out=std::move(b);return true;
}
