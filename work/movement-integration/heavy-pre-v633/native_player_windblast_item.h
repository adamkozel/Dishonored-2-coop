#pragma once
#include "native_player_power_item.h"
struct NativeWindblastItem : NativePlayerPowerItem {
    bool instantiated=false;
};
// Exact installed Windblast item. Its initial zero-quantity entry has no
// entity; native 618DE0 creates the equipment through 619380 when acquired.
// This reader never grants, activates or starts a screen/world effect.
template<class Read> bool readNativeWindblastItemFrame(Read read,std::uintptr_t base,NativeWindblastItem& p,bool selectedOnly=false){
    if(!readNativePlayerSword(read,base,p.player)||p.player.sword.logic!=1||p.player.character!=1)return false;
    const auto& inv=p.player.sword;
    // Casting reads the already validated selected slot only. Full inventory
    // search is restricted to opt-in fixture/probe acquisition observations.
    if(selectedOnly&&!inv.slots[1])return false;
    for(unsigned i=0;i<(selectedOnly?1u:inv.itemCount);++i){const auto entry=selectedOnly?inv.slots[1]:inv.items+i*56;std::uintptr_t d=0,vt=0,name=0;std::string text;
        if(!read(entry,&d,8)||!d||!read(d,&vt,8))return false;
        if(vt!=base+0x1b9ea48)continue;
        if(!read(d+8,&name,8)||!worldIdentityString(read,name,text))return false;
        if(text=="models/powers/windblast/windblast.inventoryitem"){if(p.entry)return false;p.entry=entry;p.declaration=d;p.name=text;}
    }
    unsigned type=0;int maximum=0;
    if(!p.entry||!read(p.declaration+0x60,&type,4)||type!=9||!read(p.entry+8,&p.handle,4)||
       !read(p.entry+0x10,&p.component,8)||!read(p.entry+0x18,&p.equipmentDeclaration,8)||
       !read(p.entry+0x20,&p.quantity,4)||(p.quantity!=0&&p.quantity!=1)||!read(p.entry+0x24,&maximum,4)||maximum!=1)return false;
    unsigned membership=0;for(unsigned i=0;i<inv.equipmentCount;++i){std::uintptr_t entry=0;if(!read(inv.equipments+i*8,&entry,8))return false;if(entry==p.entry)++membership;}
    p.equipped=inv.slots[1]==p.entry;
    if(selectedOnly&&(!p.equipped||p.quantity!=1))return false;
    if(!p.component)return !p.quantity&&p.handle==0x3fff0010&&!p.equipmentDeclaration&&!p.equipped&&!membership;
    std::uintptr_t vt=0,owner=0,name=0;std::string text;unsigned parent=0;
    p.entity=worldIdentityEntity(read,inv.game,p.handle);
    if(membership!=1||!p.entity||!read(p.entity+0xac,&parent,4)||parent!=1||
       !read(p.component,&vt,8)||vt!=base+0x1b53488||!read(p.component+0x10,&owner,8)||owner!=p.entity||
       !read(p.component+8,&owner,8)||owner!=p.equipmentDeclaration||!read(p.component+0x1c,&p.flags,4)||
       !read(p.equipmentDeclaration,&vt,8)||vt!=base+0x1b53348||!read(p.equipmentDeclaration+8,&name,8)||!worldIdentityString(read,name,text)||
       text!="components/characters/player/base/powers/windblast/windblast.cpntpowerwindblast"||
       !read(p.equipmentDeclaration+0x68,&p.slot,4)||p.slot!=1)return false;
    p.started=(p.flags&0xc)==8;if((p.flags&0xc)!=0&&!p.started)return false;
    const auto member=[&](std::uintptr_t actor,std::uintptr_t component,unsigned maximum){
        std::uintptr_t list=0;unsigned count=0,found=0;
        if(!read(actor+0x268,&list,8)||!list||!read(actor+0x270,&count,4)||(count&0xffffff)>maximum)return false;
        for(unsigned i=0;i<(count&0xffffff);++i){std::uintptr_t c=0;if(!read(list+i*8,&c,8))return false;if(c==component)++found;}return found==1;
    };
    // Exact RTTI and asset identity precede any fixture acquisition. The
    // Windblast equipment has its own native entity; no player vision manager.
    if(!member(p.entity,p.component,64))return false;
    p.instantiated=true;return true;
}
template<class Read> bool readNativeWindblastItem(Read read,std::uintptr_t base,NativeWindblastItem& out,bool selectedOnly=false){
    NativeWindblastItem a{},b{};
    if(!readNativeWindblastItemFrame(read,base,a,selectedOnly)||!readNativeWindblastItemFrame(read,base,b,selectedOnly)||
       !sameSwordEquipmentIdentity(a.player.sword,b.player.sword)||a.player.character!=b.player.character||
       a.entry!=b.entry||a.declaration!=b.declaration||a.component!=b.component||a.equipmentDeclaration!=b.equipmentDeclaration||
       a.entity!=b.entity||a.handle!=b.handle||a.quantity!=b.quantity||a.flags!=b.flags)return false;
    out=std::move(b);return true;
}
