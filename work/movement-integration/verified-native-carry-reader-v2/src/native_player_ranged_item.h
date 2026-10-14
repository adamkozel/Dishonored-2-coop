#pragma once
#include "native_player_sword.h"
// Exact retail gun/crossbow identities, read only. Inventory type 2 and
// distinct gun/crossbow RTTI are confirmed in B671F824 and Void declarations.
// The checkpoint already instantiates these at quantity zero. No grants,
// firing, attachment writes or NPC-equipment assumptions belong in this reader.
struct NativePlayerRangedItem {
    NativePlayerSword player{};
    std::uintptr_t entry=0,declaration=0,component=0,equipmentDeclaration=0,entity=0;
    unsigned kind=0,handle=0,flags=0;int quantity=0,slot=-1;
    bool equipped=false,started=false;std::string name,equipmentName;
};
struct NativeRangedDefaultUpgrade {std::uintptr_t entry=0,declaration=0;int quantity=0;};
struct NativeRangedAmmo {std::uintptr_t entry=0,declaration=0;int quantity=0,maximum=0;};
template<class Read> bool readNativeRangedAmmo(Read read,std::uintptr_t base,const NativePlayerRangedItem& p,NativeRangedAmmo& out){
    if(p.kind!=1&&p.kind!=2)return false;
    const auto& inv=p.player.sword;NativeRangedAmmo found{};
    const std::string expected=p.kind==1?"components/equipment/ammo/bullet.inventoryitem":"components/equipment/ammo/bolt_regular.inventoryitem";
    for(unsigned i=0;i<inv.itemCount;++i){const auto entry=inv.items+i*56;std::uintptr_t d=0,vt=0,n=0;std::string name;
        if(!read(entry,&d,8)||!d||!read(d,&vt,8))return false;
        if(vt!=base+0x1b9ea48)continue;
        if(!read(d+8,&n,8)||!worldIdentityString(read,n,name))return false;
        if(name==expected){if(found.entry)return false;found.entry=entry;found.declaration=d;}
    }
    unsigned handle=0,type=0;std::uintptr_t ptr=0;
    if(!found.entry||!read(found.declaration+0x60,&type,4)||type!=4||
       !read(found.entry+8,&handle,4)||handle!=0x3fff0010||!read(found.entry+0x10,&ptr,8)||ptr||!read(found.entry+0x18,&ptr,8)||ptr||
       !read(found.entry+0x20,&found.quantity,4)||found.quantity<0||found.quantity>10||
       !read(found.entry+0x24,&found.maximum,4)||found.maximum!=10)return false;
    NativePlayerSword final{};int again=0;
    if(!readNativePlayerSword(read,base,final)||!sameSwordEquipmentIdentity(inv,final.sword)||final.character!=p.player.character||
       !read(found.entry,&ptr,8)||ptr!=found.declaration||!read(found.entry+0x20,&again,4)||again!=found.quantity)return false;
    out=found;return true;
}
// Acquiring the native pistol also acquires its role-specific default upgrade.
// Track only that exact declaration, so a bounded fixture can undo that side
// effect through native inventory notifications without touching other upgrades.
template<class Read> bool readNativeRangedDefaultUpgrade(Read read,std::uintptr_t base,const NativePlayerRangedItem& p,NativeRangedDefaultUpgrade& out){
    if(p.kind!=1){out={};return p.kind==2;}
    const auto& inv=p.player.sword;NativeRangedDefaultUpgrade found{};
    const std::string expected=p.player.character==1?"components/characters/player/base/attributes/gun_magazine_0_corvo.equipmentupgrade":
        "components/characters/player/base/attributes/gun_accuracy_0_emily.equipmentupgrade";
    for(unsigned i=0;i<inv.itemCount;++i){const auto entry=inv.items+i*56;std::uintptr_t d=0,vt=0,n=0;std::string name;
        if(!read(entry,&d,8)||!d||!read(d,&vt,8))return false;
        if(vt!=base+0x1b555a8)continue;
        if(!read(d+8,&n,8)||!worldIdentityString(read,n,name))return false;
        if(name==expected){if(found.entry)return false;found.entry=entry;found.declaration=d;}
    }
    unsigned handle=0;int maximum=0;std::uintptr_t ptr=0;
    if(!found.entry||!read(found.entry+8,&handle,4)||handle!=0x3fff0010||
       !read(found.entry+0x10,&ptr,8)||ptr||!read(found.entry+0x18,&ptr,8)||ptr||
       !read(found.entry+0x20,&found.quantity,4)||(found.quantity!=0&&found.quantity!=1)||
       !read(found.entry+0x24,&maximum,4)||maximum!=1)return false;
    NativePlayerSword final{};int again=0;
    if(!readNativePlayerSword(read,base,final)||!sameSwordEquipmentIdentity(inv,final.sword)||final.character!=p.player.character||
       !read(found.entry,&ptr,8)||ptr!=found.declaration||!read(found.entry+0x20,&again,4)||again!=found.quantity)return false;
    out=found;return true;
}
template<class Read> bool readNativePlayerRangedItemFrame(Read read,std::uintptr_t base,unsigned kind,NativePlayerRangedItem& p,bool selectedOnly=false){
    if((kind!=1&&kind!=2)||!readNativePlayerSword(read,base,p.player)||p.player.sword.logic!=1)return false;
    p.kind=kind;const auto& inv=p.player.sword;const bool corvo=p.player.character==1;
    const std::string expected=kind==1?(corvo?"models/weapons/gun_player/gun_player.inventoryitem":"models/weapons/gun_player/gun_emily.inventoryitem"):
        (corvo?"models/weapons/crossbow/crossbow_corvo.inventoryitem":"models/weapons/crossbow/crossbow_emily.inventoryitem");
    if(selectedOnly&&!inv.slots[1])return false;
    for(unsigned i=0;i<(selectedOnly?1u:inv.equipmentCount);++i){std::uintptr_t entry=inv.slots[1],d=0,vt=0,name=0;std::string text;
        if((!selectedOnly&&!read(inv.equipments+i*8,&entry,8))||!read(entry,&d,8)||!d||!read(d,&vt,8)||vt!=base+0x1b9ea48||
           !read(d+8,&name,8)||!worldIdentityString(read,name,text))return false;
        if(text==expected){if(p.entry)return false;p.entry=entry;p.declaration=d;p.name=text;}
    }
    unsigned type=0,parent=0;int maximum=0;std::uintptr_t vt=0,owner=0,name=0;
    if(!p.entry||!read(p.declaration+0x60,&type,4)||type!=2||!read(p.entry+8,&p.handle,4)||
       !read(p.entry+0x10,&p.component,8)||!p.component||!read(p.entry+0x18,&p.equipmentDeclaration,8)||!p.equipmentDeclaration||
       !read(p.entry+0x20,&p.quantity,4)||(p.quantity!=0&&p.quantity!=1)||!read(p.entry+0x24,&maximum,4)||maximum!=1||
       !read(p.component,&vt,8)||vt!=base+(kind==1?0x1b53f58:0x1b9cbf8)||!read(p.component+8,&owner,8)||owner!=p.equipmentDeclaration||
       !read(p.equipmentDeclaration,&vt,8)||vt!=base+(kind==1?0x1b53e18:0x1b9cab8)||
       !read(p.equipmentDeclaration+8,&name,8)||!worldIdentityString(read,name,p.equipmentName)||
       !read(p.equipmentDeclaration+0x68,&p.slot,4)||p.slot!=1||!read(p.component+0x1c,&p.flags,4))return false;
    const std::string component=kind==1?(corvo?"components/characters/player/corvo/weapons/gun.cpntweapongun":"components/characters/player/emily/weapons/gun.cpntweapongun"):
        "components/characters/player/base/weapons/crossbow.cpntweaponcrossbow";
    if(p.equipmentName!=component)return false;
    p.entity=worldIdentityEntity(read,inv.game,p.handle);
    if(!p.entity||!read(p.entity,&vt,8)||vt!=base+0x1ce1e98||!read(p.entity+0xac,&parent,4)||parent!=1||
       !read(p.component+0x10,&owner,8)||owner!=p.entity)return false;
    p.equipped=inv.slots[1]==p.entry;p.started=(p.flags&0xc)==8;
    if(((p.flags&0xc)!=0&&!p.started)||p.started!=p.equipped||(p.equipped&&p.quantity!=1)||
       (selectedOnly&&(!p.equipped||!p.started)))return false;
    std::uintptr_t list=0;unsigned count=0,membership=0;
    if(!read(p.entity+0x268,&list,8)||!list||!read(p.entity+0x270,&count,4)||!(count&0xffffff)||(count&0xffffff)>64)return false;
    for(unsigned i=0;i<(count&0xffffff);++i){std::uintptr_t c=0;if(!read(list+i*8,&c,8))return false;if(c==p.component)++membership;}
    return membership==1;
}
template<class Read> bool readNativePlayerRangedItem(Read read,std::uintptr_t base,unsigned kind,NativePlayerRangedItem& out,bool selectedOnly=false){
    NativePlayerRangedItem a{},b{};
    if(!readNativePlayerRangedItemFrame(read,base,kind,a,selectedOnly)||!readNativePlayerRangedItemFrame(read,base,kind,b,selectedOnly)||
       !sameSwordEquipmentIdentity(a.player.sword,b.player.sword)||a.player.character!=b.player.character||
       a.entry!=b.entry||a.declaration!=b.declaration||a.component!=b.component||a.equipmentDeclaration!=b.equipmentDeclaration||
       a.entity!=b.entity||a.handle!=b.handle||a.quantity!=b.quantity||a.flags!=b.flags||a.equipmentName!=b.equipmentName)return false;
    out=std::move(b);return true;
}
