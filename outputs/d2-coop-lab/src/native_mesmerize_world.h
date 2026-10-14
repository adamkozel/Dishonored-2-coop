#pragma once
#include "native_player_mesmerize_item.h"
// Observational only. Preview entities are deliberately not searched by name.
// The equipment's native +d4 relationship establishes committed ownership.
struct NativeMesmerizeWorld {
    NativeMesmerizeItem equipment{};
    std::uintptr_t actor=0,definition=0,component=0,declaration=0,list=0;
    unsigned handle=0x3fff0010,state=4,flags=0,count=0;
    std::array<float,12> transform{};
    bool present=false;
};
template<class Read> bool readNativeMesmerizeWorldFrame(Read read,std::uintptr_t base,NativeMesmerizeWorld& s){
    if(!readNativeMesmerizeItem(read,base,s.equipment))return false;
    if(!s.equipment.instantiated)return true;
    if(!read(s.equipment.component+0xd4,&s.handle,4))return false;
    if(s.handle==0x3fff0010)return true;
    const auto game=s.equipment.player.sword.game;
    s.actor=worldIdentityEntity(read,game,s.handle);
    std::uintptr_t vt=0,name=0,owner=0;unsigned parent=0;std::string text;
    if(!s.actor||s.actor==s.equipment.entity||!read(s.actor,&vt,8)||vt!=base+0x1c87d58||
       !read(s.actor+0xac,&parent,4)||parent!=0x3fff0010||
       !read(s.actor+0x2e8,&s.definition,8)||!s.definition||!read(s.definition,&vt,8)||vt!=base+0x1ab58f8||
       !read(s.definition+8,&name,8)||!worldIdentityString(read,name,text)||text!="models/powers/mesmerize/mesmerize_object.def"||
       !read(s.actor+0x268,&s.list,8)||!s.list||!read(s.actor+0x270,&s.count,4)||!(s.count&0xffffff)||(s.count&0xffffff)>32)return false;
    for(unsigned i=0;i<(s.count&0xffffff);++i){std::uintptr_t c=0;
        if(!read(s.list+i*8,&c,8)||!c||!read(c,&vt,8))return false;
        if(vt==base+0x1c53958){if(s.component)return false;s.component=c;}
    }
    if(!s.component||!read(s.component+0x10,&owner,8)||owner!=s.actor||!read(s.component+0x20,&owner,8)||owner!=s.component||
       !read(s.component+8,&s.declaration,8)||!s.declaration||!read(s.declaration,&vt,8)||vt!=base+0x1c53a38||
       !read(s.declaration+8,&name,8)||!worldIdentityString(read,name,text)||text!="components/characters/player/base/powers/mesmerize/mesmerize_object.cpntmesmerize"||
       !read(s.component+0x1c,&s.flags,4)||!read(s.component+0x2c,&s.state,4)||s.state>4||
       !read(s.actor+0x300,s.transform.data(),48))return false;
    for(unsigned i=0;i<12;++i)if(!std::isfinite(s.transform[i])||std::fabs(s.transform[i])>(i<3?100000.f:1.01f))return false;
    for(unsigned i=0;i<3;++i)for(unsigned j=i;j<3;++j){float dot=0;
        for(unsigned k=0;k<3;++k)dot+=s.transform[3+i*3+k]*s.transform[3+j*3+k];
        if(std::fabs(dot-(i==j?1.f:0.f))>.02f)return false;
    }
    s.present=true;return true;
}
template<class Read> bool readNativeMesmerizeWorld(Read read,std::uintptr_t base,NativeMesmerizeWorld& out){
    NativeMesmerizeWorld a{},b{};
    if(!readNativeMesmerizeWorldFrame(read,base,a)||!readNativeMesmerizeWorldFrame(read,base,b)||
       !sameSwordEquipmentIdentity(a.equipment.player.sword,b.equipment.player.sword)||
       a.equipment.entry!=b.equipment.entry||a.equipment.component!=b.equipment.component||a.equipment.handle!=b.equipment.handle||
       a.equipment.quantity!=b.equipment.quantity||a.equipment.flags!=b.equipment.flags||a.equipment.declaration!=b.equipment.declaration||
       a.equipment.equipmentDeclaration!=b.equipment.equipmentDeclaration||a.present!=b.present||a.handle!=b.handle||
       a.actor!=b.actor||a.definition!=b.definition||a.component!=b.component||a.declaration!=b.declaration||
       a.list!=b.list||a.count!=b.count||a.state!=b.state||a.flags!=b.flags||a.transform!=b.transform)return false;
    out=std::move(b);return true;
}
