#pragma once
#include "native_health.h"
#include <algorithm>
#include <cstring>

// Read-only retail sword inventory/attachment reconstruction. Inventory
// reflection 1599460: items +38 (56-byte entries), equipment pointers +58,
// sixteen slot pointers +68. Native 614b20 equips, 619640 unequips; their
// component callbacks bind the existing entity to declaration tags +70/+78.
constexpr char kGuardSwordItem[]="models/characters/inventory/items/gs_guard_sword_item.inventoryitem";
constexpr char kGuardSwordEquipment[]="components/characters/base/weaponmeleenpc_sword.cpntweaponmeleenpc";
struct NativeSwordEquipment {
    std::uintptr_t game=0,actor=0,inventory=0,items=0,equipments=0,entry=0,itemDeclaration=0;
    std::uintptr_t entity=0,component=0,declaration=0,frame=0,parent=0;
    std::uint32_t handle=0,logic=0,itemCount=0,equipmentCount=0,weaponHandle=0,componentFlags=0;
    std::int32_t slot=-1,quantity=0,maximumQuantity=0;
    std::uint16_t joint=0xffff;
    std::array<std::uintptr_t,16> slots{};
    std::string itemName,equipmentName,holsterTag,drawTag,attachedTag;
    std::array<float,12> root{},cached{};
    bool equipped=false,started=false;
};
template<class Read> bool readNativeSwordEquipmentFrame(Read read,std::uintptr_t base,std::uint32_t handle,NativeSwordEquipment& out){
    NativeHealth actor{};
    if(!readNativeHealth(read,base,handle,actor)||(actor.logic!=1&&actor.logic!=2&&actor.logic!=13))return false;
    NativeSwordEquipment s{};s.game=actor.game;s.actor=actor.actor;s.handle=handle;s.logic=actor.logic;
    std::uintptr_t vt=0,owner=0,name=0;
    for(unsigned i=0;i<(actor.componentCount&0xffffff);++i){
        std::uintptr_t c=0;if(!read(actor.list+i*8,&c,8)||!c||!read(c,&vt,8))return false;
        if(vt==base+0x1b9a878){if(s.inventory)return false;s.inventory=c;}
    }
    std::uint32_t flags=0;
    if(!s.inventory||!read(s.inventory+0x10,&owner,8)||owner!=s.actor||
       !read(s.inventory+0x38,&s.items,8)||!s.items||!read(s.inventory+0x40,&flags,4))return false;
    s.itemCount=flags&0xffffff;if(!s.itemCount||s.itemCount>256)return false;
    if(!read(s.inventory+0x58,&s.equipments,8)||!s.equipments||!read(s.inventory+0x60,&flags,4))return false;
    s.equipmentCount=flags&0xffffff;if(!s.equipmentCount||s.equipmentCount>64)return false;
    if(!read(s.inventory+0x68,s.slots.data(),sizeof(s.slots)))return false;
    std::vector<std::uintptr_t> entries(s.equipmentCount);
    if(!read(s.equipments,entries.data(),entries.size()*8))return false;
    for(auto entry:entries){
        if(entry<s.items||entry>=s.items+s.itemCount*56||(entry-s.items)%56)return false;
        std::uintptr_t decl=0;std::string text;
        if(!read(entry,&decl,8)||!decl||!read(decl,&vt,8)||vt!=base+0x1b9ea48||
           !read(decl+8,&name,8)||!worldIdentityString(read,name,text))return false;
        if(text==kGuardSwordItem){if(s.entry)return false;s.entry=entry;s.itemDeclaration=decl;s.itemName=std::move(text);}
    }
    if(!s.entry||!read(s.entry+8,&s.weaponHandle,4)||!read(s.entry+0x10,&s.component,8)||!s.component||
       !read(s.entry+0x18,&s.declaration,8)||!s.declaration||!read(s.entry+0x20,&s.quantity,4)||
       !read(s.entry+0x24,&s.maximumQuantity,4)||s.quantity!=1||s.maximumQuantity!=1||
       !read(s.component,&vt,8)||vt!=base+0x1b9cf48||!read(s.component+8,&owner,8)||owner!=s.declaration||
       !read(s.declaration,&vt,8)||vt!=base+0x1b9ce08||!read(s.declaration+8,&name,8)||
       !worldIdentityString(read,name,s.equipmentName)||s.equipmentName!=kGuardSwordEquipment||
       !read(s.declaration+0x68,&s.slot,4)||s.slot!=0||
       !read(s.declaration+0x70,&name,8)||!worldIdentityString(read,name,s.holsterTag)||s.holsterTag!="holster_wpn"||
       !read(s.declaration+0x78,&name,8)||!worldIdentityString(read,name,s.drawTag)||s.drawTag!="right_hand_wpn")return false;
    s.entity=worldIdentityEntity(read,s.game,s.weaponHandle);
    std::uint32_t parentHandle=0;
    if(!s.entity||!read(s.entity,&vt,8)||vt!=base+0x1c87d58||!read(s.entity+0xac,&parentHandle,4)||parentHandle!=handle||
       !read(s.component+0x10,&owner,8)||owner!=s.entity||!read(s.component+0x1c,&s.componentFlags,4))return false;
    s.started=(s.componentFlags&0xc)==8;s.equipped=s.slots[0]==s.entry;
    if((s.componentFlags&0xc)!=0&&!s.started)return false;
    // A third item occupying the sword's slot is not part of this fixed case.
    if(s.slots[0]&&!s.equipped)return false;
    for(auto slot:s.slots)if(slot&&std::find(entries.begin(),entries.end(),slot)==entries.end())return false;
    std::uintptr_t list=0;std::uint32_t count=0;unsigned membership=0;
    if(!read(s.entity+0x268,&list,8)||!list||!read(s.entity+0x270,&count,4)||(count&0xffffff)>64)return false;
    for(unsigned i=0;i<(count&0xffffff);++i){std::uintptr_t c=0;if(!read(list+i*8,&c,8))return false;if(c==s.component)++membership;}
    if(membership!=1||!read(s.entity+0x358,&s.frame,8)||!s.frame||!read(s.frame,&vt,8)||vt!=base+0x1c573e8||
       !read(s.frame+0x10,&owner,8)||owner!=s.entity||!read(s.frame+0x3c8,&s.parent,8)||s.parent!=s.actor||
       !read(s.frame+0x3c0,&name,8)||!worldIdentityString(read,name,s.attachedTag)||
       (s.attachedTag!=s.holsterTag&&s.attachedTag!=s.drawTag)||!read(s.frame+0x3d0,&s.joint,2)||s.joint>=223||
       !read(s.entity+0x300,s.root.data(),48)||!read(s.entity+0x78,s.cached.data(),48))return false;
    for(float f:s.root)if(!std::isfinite(f))return false;
    for(float f:s.cached)if(!std::isfinite(f))return false;
    out=std::move(s);return true;
}
inline bool sameSwordEquipmentIdentity(const NativeSwordEquipment& a,const NativeSwordEquipment& b){
    return a.game==b.game&&a.actor==b.actor&&a.inventory==b.inventory&&a.items==b.items&&a.equipments==b.equipments&&
        a.entry==b.entry&&a.itemDeclaration==b.itemDeclaration&&a.entity==b.entity&&a.component==b.component&&a.declaration==b.declaration&&
        a.frame==b.frame&&a.parent==b.parent&&a.handle==b.handle&&a.logic==b.logic&&a.itemCount==b.itemCount&&a.equipmentCount==b.equipmentCount&&
        a.weaponHandle==b.weaponHandle&&a.componentFlags==b.componentFlags&&a.slot==b.slot&&a.quantity==b.quantity&&a.maximumQuantity==b.maximumQuantity&&
        a.joint==b.joint&&a.slots==b.slots&&a.itemName==b.itemName&&a.equipmentName==b.equipmentName&&a.holsterTag==b.holsterTag&&
        a.drawTag==b.drawTag&&a.attachedTag==b.attachedTag&&a.equipped==b.equipped&&a.started==b.started;
}
template<class Read> bool readNativeSwordEquipment(Read read,std::uintptr_t base,std::uint32_t handle,NativeSwordEquipment& out){
    NativeSwordEquipment a{},b{};
    if(!readNativeSwordEquipmentFrame(read,base,handle,a)||!readNativeSwordEquipmentFrame(read,base,handle,b)||!sameSwordEquipmentIdentity(a,b))return false;
    out=std::move(b);return true;
}
