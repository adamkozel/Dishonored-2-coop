#pragma once
#include "native_sword_equipment.h"
#include "native_player_arm.h"

// Actual player equipment is an arkWeapon, attached to the main body when
// holstered and to its separately owned right arm when drawn. Never accept
// an NPC inventory or infer a draw from controller input or an animation.
struct NativePlayerSword {
    NativeSwordEquipment sword{};
    NativePlayerArm arm{};
    std::uint32_t character=0;
};
template<class Read> bool readNativePlayerSwordFrame(Read read,std::uintptr_t base,NativePlayerSword& out){
    NativePlayerSword n{};auto& s=n.sword;NativeHealth actor{};
    if(!readNativeHealth(read,base,1,actor)||(actor.logic!=1&&actor.logic!=2))return false;
    s.game=actor.game;s.actor=actor.actor;s.handle=1;s.logic=actor.logic;
    std::uintptr_t vt=0,owner=0,name=0,definition=0;std::string body;
    if(!read(s.actor+0x2e8,&definition,8)||!definition||!read(definition,&vt,8)||vt!=base+0x1ab58f8||
       !read(definition+8,&name,8)||!worldIdentityString(read,name,body))return false;
    if(body=="models/characters/player/corvo/body.def")n.character=1;
    else if(body=="models/characters/player/emily/body.def")n.character=2;else return false;
    for(unsigned i=0;i<(actor.componentCount&0xffffff);++i){
        std::uintptr_t c=0;if(!read(actor.list+i*8,&c,8)||!c||!read(c,&vt,8))return false;
        if(vt==base+0x1b9a878){if(s.inventory)return false;s.inventory=c;}
    }
    std::uint32_t flags=0;
    if(!s.inventory||!read(s.inventory+0x10,&owner,8)||owner!=s.actor||!read(s.inventory+0x38,&s.items,8)||!s.items||
       !read(s.inventory+0x40,&flags,4))return false;
    s.itemCount=flags&0xffffff;if(!s.itemCount||s.itemCount>1024)return false;
    if(!read(s.inventory+0x58,&s.equipments,8)||!s.equipments||!read(s.inventory+0x60,&flags,4))return false;
    s.equipmentCount=flags&0xffffff;if(!s.equipmentCount||s.equipmentCount>64||!read(s.inventory+0x68,s.slots.data(),sizeof(s.slots)))return false;
    std::vector<std::uintptr_t> entries(s.equipmentCount);
    if(!read(s.equipments,entries.data(),entries.size()*8))return false;
    for(auto entry:entries){
        if(entry<s.items||entry>=s.items+s.itemCount*56||(entry-s.items)%56)return false;
        std::uintptr_t decl=0;std::string text;
        if(!read(entry,&decl,8)||!decl||!read(decl,&vt,8)||vt!=base+0x1b9ea48||!read(decl+8,&name,8)||!worldIdentityString(read,name,text))return false;
        if(text=="models/weapons/ply_sword/ply_sword"){if(s.entry)return false;s.entry=entry;s.itemDeclaration=decl;s.itemName=std::move(text);}
    }
    if(!s.entry||!read(s.entry+8,&s.weaponHandle,4)||!read(s.entry+0x10,&s.component,8)||!s.component||
       !read(s.entry+0x18,&s.declaration,8)||!s.declaration||!read(s.entry+0x20,&s.quantity,4)||s.quantity<0||s.quantity>1||
       !read(s.entry+0x24,&s.maximumQuantity,4)||s.maximumQuantity!=1||!read(s.component,&vt,8)||vt!=base+0x1b9d508||
       !read(s.component+8,&owner,8)||owner!=s.declaration||!read(s.declaration,&vt,8)||vt!=base+0x1b9d3c8||
       !read(s.declaration+8,&name,8)||!worldIdentityString(read,name,s.equipmentName)||s.equipmentName!="components/characters/player/base/weapons/sword.cpntweaponmeleeplayer"||
       !read(s.declaration+0x68,&s.slot,4)||s.slot!=0||!read(s.declaration+0x70,&name,8)||!worldIdentityString(read,name,s.holsterTag)||s.holsterTag!="sword_attach_tag"||
       !read(s.declaration+0x78,&name,8)||!worldIdentityString(read,name,s.drawTag)||s.drawTag!="weapon_attach_tag")return false;
    s.entity=worldIdentityEntity(read,s.game,s.weaponHandle);std::uint32_t parentHandle=0;
    if(!s.entity||!read(s.entity,&vt,8)||vt!=base+0x1ce1e98||!read(s.entity+0xac,&parentHandle,4)||parentHandle!=1||
       !read(s.component+0x10,&owner,8)||owner!=s.entity||!read(s.component+0x1c,&s.componentFlags,4))return false;
    s.started=(s.componentFlags&0xc)==8;s.equipped=s.slots[0]==s.entry;
    if(((s.componentFlags&0xc)!=0&&!s.started)||(s.slots[0]&&!s.equipped)||s.started!=s.equipped||(s.equipped&&s.quantity!=1))return false;
    for(auto slot:s.slots)if(slot&&std::find(entries.begin(),entries.end(),slot)==entries.end())return false;
    std::uintptr_t list=0;std::uint32_t count=0;unsigned membership=0;
    if(!read(s.entity+0x268,&list,8)||!list||!read(s.entity+0x270,&count,4)||(count&0xffffff)>64)return false;
    for(unsigned i=0;i<(count&0xffffff);++i){std::uintptr_t c=0;if(!read(list+i*8,&c,8))return false;if(c==s.component)++membership;}
    if(membership!=1||!read(s.entity+0x358,&s.frame,8)||!s.frame||!read(s.frame,&vt,8)||vt!=base+0x1c573e8||
       !read(s.frame+0x10,&owner,8)||owner!=s.entity||!read(s.frame+0x3c8,&s.parent,8)||!s.parent||
       !read(s.frame+0x3c0,&name,8)||!worldIdentityString(read,name,s.attachedTag)||!read(s.frame+0x3d0,&s.joint,2))return false;
    if(s.equipped){
        std::uint32_t armHandle=0;
        if(s.attachedTag!=s.drawTag||s.joint!=59||!read(s.parent+0xa8,&armHandle,4)||!readNativePlayerArm(read,base,s.game,armHandle,n.arm)||
           n.arm.actor!=s.parent||n.arm.player!=s.actor||n.arm.character!=n.character||n.arm.side!=2)return false;
    }else if(s.parent!=s.actor||s.attachedTag!=s.holsterTag||s.joint!=2)return false;
    out=std::move(n);return true;
}
template<class Read> bool readNativePlayerSword(Read read,std::uintptr_t base,NativePlayerSword& out){
    NativePlayerSword a{},b{};
    if(!readNativePlayerSwordFrame(read,base,a)||!readNativePlayerSwordFrame(read,base,b)||
       !sameSwordEquipmentIdentity(a.sword,b.sword)||a.character!=b.character||!sameNativePlayerArm(a.arm,b.arm))return false;
    out=std::move(b);return true;
}
