#pragma once
#include "native_mesmerize_world.h"
#include <algorithm>
// Discovery does not authorize future access. Revalidate this exact local
// Emily equipment, inventory membership, +d4 committed object and generations.
// Does not depend on selected equipment, sword attachments or health state.
template<class Read> bool currentNativeMesmerizeWorldOwner(Read read,std::uintptr_t base,const NativeMesmerizeWorld& w){
 if(!w.present||!w.equipment.instantiated||w.equipment.player.character!=2)return false;
 const auto& e=w.equipment;const auto& inv=e.player.sword;const auto component=w.component;
 std::uintptr_t game=0,vt=0;
 if(!read(base+0x39a5290,&game,8)||!game||game!=inv.game||!read(game,&vt,8)||vt!=base+0x1c940a8)return false;
 const auto eq=[&](std::uintptr_t at,std::uintptr_t value){std::uintptr_t v=0;return read(at,&v,8)&&v==value;};
 const auto decl=[&](std::uintptr_t d,std::uintptr_t vt,const char* expected){std::uintptr_t p=0;std::string name;
  return d&&eq(d,vt)&&read(d+8,&p,8)&&worldIdentityString(read,p,name)&&name==expected;};
 const auto member=[&](std::uintptr_t actor,std::uintptr_t c,unsigned limit){std::uintptr_t list=0;unsigned n=0;
  if(!read(actor+0x268,&list,8)||!list||!read(actor+0x270,&n,4)||!(n&0xffffff)||(n&0xffffff)>limit)return false;
  std::array<std::uintptr_t,256> entries{};n&=0xffffff;
  return read(list,entries.data(),n*8)&&std::count(entries.begin(),entries.begin()+n,c)==1;};
 const auto player=worldIdentityEntity(read,game,1);std::uintptr_t d=0;
 if(!player||player!=inv.actor||!eq(player,base+0x1c5e428)||!read(player+0x2e8,&d,8)||
    !decl(d,base+0x1ab58f8,"models/characters/player/emily/body.def")||
    !member(player,inv.inventory,256)||!eq(inv.inventory,base+0x1b9a878)||!eq(inv.inventory+0x10,player))return false;
 std::uintptr_t items=0,equipments=0;unsigned count=0,equipmentCount=0;
 if(!read(inv.inventory+0x38,&items,8)||!items||!read(inv.inventory+0x40,&count,4)||!(count&0xffffff)||(count&0xffffff)>1024||
    e.entry<items||e.entry-items>=std::uintptr_t(count&0xffffff)*56||(e.entry-items)%56||
    !read(inv.inventory+0x58,&equipments,8)||!equipments||!read(inv.inventory+0x60,&equipmentCount,4)||
    !(equipmentCount&0xffffff)||(equipmentCount&0xffffff)>64)return false;
 std::array<std::uintptr_t,64> entries{};equipmentCount&=0xffffff;
 if(!read(equipments,entries.data(),equipmentCount*8)||std::count(entries.begin(),entries.begin()+equipmentCount,e.entry)!=1)return false;
 unsigned handle=0,parent=0,committed=0,state=0,flags=0;int quantity=0,maximum=0;
 if(!eq(e.entry,e.declaration)||!decl(e.declaration,base+0x1b9ea48,"models/powers/mesmerize/mesmerize.inventoryitem")||
    !read(e.entry+8,&handle,4)||handle!=e.handle||!eq(e.entry+0x10,e.component)||!eq(e.entry+0x18,e.equipmentDeclaration)||
    !read(e.entry+0x20,&quantity,4)||(quantity!=0&&quantity!=1)||!read(e.entry+0x24,&maximum,4)||maximum!=1||
    worldIdentityEntity(read,game,e.handle)!=e.entity||!read(e.entity+0xac,&parent,4)||parent!=1||
    !member(e.entity,e.component,64)||!eq(e.component,base+0x1b9bea8)||!eq(e.component+0x10,e.entity)||!eq(e.component+8,e.equipmentDeclaration)||
    !decl(e.equipmentDeclaration,base+0x1b9bd68,"components/characters/player/base/powers/mesmerize/mesmerize.cpntpowermesmerize")||
    !read(e.component+0x1c,&flags,4)||((flags&0xc)!=0&&(flags&0xc)!=8)||
    !read(e.component+0xd4,&committed,4)||committed!=w.handle||worldIdentityEntity(read,game,w.handle)!=w.actor)return false;
 if(!eq(w.actor,base+0x1c87d58)||!read(w.actor+0xac,&parent,4)||parent!=0x3fff0010||!eq(w.actor+0x2e8,w.definition)||
    !decl(w.definition,base+0x1ab58f8,"models/powers/mesmerize/mesmerize_object.def")||
    !member(w.actor,component,32)||!eq(component,base+0x1c53958)||!eq(component+0x10,w.actor)||!eq(component+0x20,component)||
    !eq(component+8,w.declaration)||!decl(w.declaration,base+0x1c53a38,"components/characters/player/base/powers/mesmerize/mesmerize_object.cpntmesmerize")||
    !read(component+0x2c,&state,4)||state>4)return false;
 unsigned again=0;
 return worldIdentityEntity(read,game,1)==player&&worldIdentityEntity(read,game,e.handle)==e.entity&&
        worldIdentityEntity(read,game,w.handle)==w.actor&&read(e.component+0xd4,&again,4)&&again==committed&&
        eq(e.entry+0x10,e.component)&&eq(component+0x10,w.actor)&&eq(base+0x39a5290,game);
}

// Fast resampling from a full-reader discovery. Only ownership, native state,
// flags and transform are refreshed; unrelated equipment pose data is a hint.
template<class Read> bool readCurrentNativeMesmerizeWorld(Read read,std::uintptr_t base,const NativeMesmerizeWorld& known,NativeMesmerizeWorld& out){
 if(!currentNativeMesmerizeWorldOwner(read,base,known))return false;
 const auto game=known.equipment.player.sword.game;std::uintptr_t stack=0;unsigned count=0,logic=0,phase=0,pending=0;
 if(!read(game+0x341058,&phase,4)||phase!=3||!read(game+0x1f7400,&pending,4)||pending||
    !read(game+0x1f73f0,&stack,8)||!stack||!read(game+0x1f73f8,&count,4)||!(count&0xffffff)||(count&0xffffff)>32||
    !read(stack+std::uintptr_t((count&0xffffff)-1)*4,&logic,4)||logic!=1)return false;
 NativeMesmerizeWorld s=known;unsigned state=0,flags=0,countAgain=0;std::uintptr_t stackAgain=0;std::array<float,12> pose{};
 if(!read(s.component+0x2c,&s.state,4)||s.state>4||!read(s.component+0x1c,&s.flags,4)||!read(s.actor+0x300,s.transform.data(),48))return false;
 for(unsigned i=0;i<12;++i)if(!std::isfinite(s.transform[i])||std::fabs(s.transform[i])>(i<3?100000.f:1.01f))return false;
 for(unsigned i=0;i<3;++i)for(unsigned j=i;j<3;++j){float dot=0;
  for(unsigned k=0;k<3;++k)dot+=s.transform[3+i*3+k]*s.transform[3+j*3+k];
  if(std::fabs(dot-(i==j?1.f:0.f))>.02f)return false;
 }
 if(!currentNativeMesmerizeWorldOwner(read,base,s)||!read(s.component+0x2c,&state,4)||state!=s.state||
    !read(s.component+0x1c,&flags,4)||flags!=s.flags||!read(s.actor+0x300,pose.data(),48)||pose!=s.transform||
    !read(game+0x341058,&phase,4)||phase!=3||!read(game+0x1f7400,&pending,4)||pending||
    !read(game+0x1f73f0,&stackAgain,8)||stackAgain!=stack||!read(game+0x1f73f8,&countAgain,4)||countAgain!=count||
    !read(stack+std::uintptr_t((count&0xffffff)-1)*4,&logic,4)||logic!=1)return false;
 out=std::move(s);return true;
}
