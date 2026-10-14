#pragma once
#include "native_player_power_item.h"
struct NativeDoppelgangerItem : NativePlayerPowerItem {
    bool instantiated=false;
    std::uintptr_t parentDeclaration=0;
};
// Exact B671 PE RTTI v403: component1b52258 and declaration1b52118.
// Installed Void declarations v404 specify Emily, slot1, and an inherited
// component declaration. Accept only the exact named or inline-parent form.
// Runtime native acquisition is initially experimental. This reader never
// grants, activates, spawns, targets or applies damage.
template<class Read> bool readNativeDoppelgangerItemFrame(Read read,std::uintptr_t base,NativeDoppelgangerItem& p,bool selectedOnly=false){
    if(!readNativePlayerSword(read,base,p.player)||p.player.sword.logic!=1||p.player.character!=2)return false;
    const auto& inv=p.player.sword;
    // Casting reads the already validated selected slot only. Full inventory
    // search is restricted to opt-in fixture/probe acquisition observations.
    if(selectedOnly&&!inv.slots[1])return false;
    for(unsigned i=0;i<(selectedOnly?1u:inv.itemCount);++i){const auto entry=selectedOnly?inv.slots[1]:inv.items+i*56;std::uintptr_t d=0,vt=0,name=0;std::string text;
        if(!read(entry,&d,8)||!d||!read(d,&vt,8))return false;
        if(vt!=base+0x1b9ea48)continue;
        if(!read(d+8,&name,8)||!worldIdentityString(read,name,text))return false;
        if(text=="models/powers/doppelganger/doppelganger.inventoryitem"){if(p.entry)return false;p.entry=entry;p.declaration=d;p.name=text;}
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
       !read(p.component,&vt,8)||vt!=base+0x1b52258||!read(p.component+0x10,&owner,8)||owner!=p.entity||
       !read(p.component+8,&owner,8)||owner!=p.equipmentDeclaration||!read(p.component+0x1c,&p.flags,4)||
       !read(p.equipmentDeclaration,&vt,8)||vt!=base+0x1b52118||!read(p.equipmentDeclaration+8,&name,8)||!worldIdentityString(read,name,text)||
       !read(p.equipmentDeclaration+0x68,&p.slot,4)||p.slot!=1)return false;
    constexpr auto componentName="components/characters/player/base/powers/doppelganger/doppelganger_power.cpntpowerdoppelganger";
    if(text=="unnamed"){
        if(!read(p.equipmentDeclaration+0x48,&p.parentDeclaration,8)||!p.parentDeclaration||p.parentDeclaration==p.equipmentDeclaration||
           !read(p.parentDeclaration,&vt,8)||vt!=base+0x1b52118||!read(p.parentDeclaration+8,&name,8)||
           !worldIdentityString(read,name,text)||text!=componentName)return false;
    }else if(text!=componentName)return false;
    p.started=(p.flags&0xc)==8;if((p.flags&0xc)!=0&&!p.started)return false;
    const auto member=[&](std::uintptr_t actor,std::uintptr_t component,unsigned maximum){
        std::uintptr_t list=0;unsigned count=0,found=0;
        if(!read(actor+0x268,&list,8)||!list||!read(actor+0x270,&count,4)||(count&0xffffff)>maximum)return false;
        for(unsigned i=0;i<(count&0xffffff);++i){std::uintptr_t c=0;if(!read(list+i*8,&c,8))return false;if(c==component)++found;}return found==1;
    };
    // Exact RTTI and asset identity precede any fixture acquisition. The
    // Doppelganger equipment is distinct from its spawned world entity and effects.
    if(!member(p.entity,p.component,64))return false;
    p.instantiated=true;return true;
}
template<class Read> bool readNativeDoppelgangerItem(Read read,std::uintptr_t base,NativeDoppelgangerItem& out,bool selectedOnly=false){
    NativeDoppelgangerItem a{},b{};
    if(!readNativeDoppelgangerItemFrame(read,base,a,selectedOnly)||!readNativeDoppelgangerItemFrame(read,base,b,selectedOnly)||
       !sameSwordEquipmentIdentity(a.player.sword,b.player.sword)||a.player.character!=b.player.character||
       a.entry!=b.entry||a.declaration!=b.declaration||a.component!=b.component||a.equipmentDeclaration!=b.equipmentDeclaration||
       a.entity!=b.entity||a.handle!=b.handle||a.quantity!=b.quantity||a.flags!=b.flags||a.parentDeclaration!=b.parentDeclaration)return false;
    out=std::move(b);return true;
}
