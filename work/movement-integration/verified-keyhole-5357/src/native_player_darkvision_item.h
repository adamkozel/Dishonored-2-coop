#pragma once
#include "native_player_power_item.h"
struct NativeDarkVisionItem : NativePlayerPowerItem {
    std::uintptr_t manager=0;unsigned managerFlags=0;bool instantiated=false,managerStarted=false;
};
// Exact installed Dark Vision item. Its initial zero-quantity entry has no
// entity; native 618DE0 creates the equipment through 619380 when acquired.
// This reader never grants, activates or starts a screen/world effect.
template<class Read> bool readNativeDarkVisionItemFrame(Read read,std::uintptr_t base,NativeDarkVisionItem& p){
    if(!readNativePlayerSword(read,base,p.player)||p.player.sword.logic!=1)return false;
    const auto& inv=p.player.sword;
    for(unsigned i=0;i<inv.itemCount;++i){const auto entry=inv.items+i*56;std::uintptr_t d=0,vt=0,name=0;std::string text;
        if(!read(entry,&d,8)||!d||!read(d,&vt,8))return false;
        if(vt!=base+0x1b9ea48)continue;
        if(!read(d+8,&name,8)||!worldIdentityString(read,name,text))return false;
        if(text=="models/powers/darkvision/darkvision.inventoryitem"){if(p.entry)return false;p.entry=entry;p.declaration=d;p.name=text;}
    }
    unsigned type=0;int maximum=0;
    if(!p.entry||!read(p.declaration+0x60,&type,4)||type!=9||!read(p.entry+8,&p.handle,4)||
       !read(p.entry+0x10,&p.component,8)||!read(p.entry+0x18,&p.equipmentDeclaration,8)||
       !read(p.entry+0x20,&p.quantity,4)||(p.quantity!=0&&p.quantity!=1)||!read(p.entry+0x24,&maximum,4)||maximum!=1)return false;
    unsigned membership=0;for(unsigned i=0;i<inv.equipmentCount;++i){std::uintptr_t entry=0;if(!read(inv.equipments+i*8,&entry,8))return false;if(entry==p.entry)++membership;}
    p.equipped=inv.slots[1]==p.entry;
    if(!p.component)return !p.quantity&&p.handle==0x3fff0010&&!p.equipmentDeclaration&&!p.equipped&&!membership;
    std::uintptr_t vt=0,owner=0,name=0;std::string text;unsigned parent=0;
    p.entity=worldIdentityEntity(read,inv.game,p.handle);
    if(membership!=1||!p.entity||!read(p.entity+0xac,&parent,4)||parent!=1||
       !read(p.component,&vt,8)||vt!=base+0x1b51c18||!read(p.component+0x10,&owner,8)||owner!=p.entity||
       !read(p.component+8,&owner,8)||owner!=p.equipmentDeclaration||!read(p.component+0x1c,&p.flags,4)||
       !read(p.equipmentDeclaration,&vt,8)||vt!=base+0x1b51ad8||!read(p.equipmentDeclaration+8,&name,8)||!worldIdentityString(read,name,text)||
       text!="components/characters/player/base/powers/darkvision/darkvision.cpntpowerdarkvision"||
       !read(p.equipmentDeclaration+0x68,&p.slot,4)||p.slot!=1)return false;
    p.started=(p.flags&0xc)==8;if((p.flags&0xc)!=0&&!p.started)return false;
    const auto member=[&](std::uintptr_t actor,std::uintptr_t component){
        std::uintptr_t list=0;unsigned count=0,found=0;
        if(!read(actor+0x268,&list,8)||!list||!read(actor+0x270,&count,4)||(count&0xffffff)>64)return false;
        for(unsigned i=0;i<(count&0xffffff);++i){std::uintptr_t c=0;if(!read(list+i*8,&c,8))return false;if(c==component)++found;}return found==1;
    };
    if(!member(p.entity,p.component)||!read(p.component+0xb8,&p.manager,8)||!p.manager||!read(p.manager,&vt,8)||vt!=base+0x1b5eb68||
       !read(p.manager+0x10,&owner,8)||owner!=inv.actor||!member(inv.actor,p.manager)||!read(p.manager+0x1c,&p.managerFlags,4))return false;
    p.managerStarted=(p.managerFlags&0xc)==8;p.instantiated=true;return true;
}
template<class Read> bool readNativeDarkVisionItem(Read read,std::uintptr_t base,NativeDarkVisionItem& out){
    NativeDarkVisionItem a{},b{};
    if(!readNativeDarkVisionItemFrame(read,base,a)||!readNativeDarkVisionItemFrame(read,base,b)||
       !sameSwordEquipmentIdentity(a.player.sword,b.player.sword)||a.player.character!=b.player.character||
       a.entry!=b.entry||a.declaration!=b.declaration||a.component!=b.component||a.equipmentDeclaration!=b.equipmentDeclaration||
       a.entity!=b.entity||a.handle!=b.handle||a.quantity!=b.quantity||a.flags!=b.flags||a.manager!=b.manager||a.managerFlags!=b.managerFlags)return false;
    out=std::move(b);return true;
}
