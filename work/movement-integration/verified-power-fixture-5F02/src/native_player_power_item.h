#pragma once
#include "native_player_sword.h"
// Read-only scope for the two native starting powers already instantiated at
// quantity zero in this exact Dunwall checkpoint. No generic grant API.
struct NativePlayerPowerItem {
    NativePlayerSword player{};std::uintptr_t entry=0,declaration=0,component=0,entity=0;
    std::uintptr_t equipmentDeclaration=0;unsigned handle=0,flags=0;int quantity=0,slot=-1;bool equipped=false,started=false;std::string name;
};
template<class Read> bool readNativePlayerPowerItem(Read read,std::uintptr_t base,NativePlayerPowerItem& out){
    NativePlayerPowerItem p{};
    if(!readNativePlayerSword(read,base,p.player)||p.player.sword.logic!=1)return false;
    const auto& inv=p.player.sword;
    const char* expected=p.player.character==1?"models/powers/blink/blink.inventoryitem":"models/powers/grapple/grapple.inventoryitem";
    for(unsigned i=0;i<inv.itemCount;++i){const auto entry=inv.items+i*56;std::uintptr_t d=0,vt=0,name=0;std::string text;
        if(!read(entry,&d,8)||!d||!read(d,&vt,8))return false;
        if(vt!=base+0x1b9ea48)continue;
        if(!read(d+8,&name,8)||!worldIdentityString(read,name,text))return false;
        if(text==expected){if(p.entry)return false;p.entry=entry;p.declaration=d;p.name=text;}
    }
    unsigned type=0,parent=0;int maximum=0;std::uintptr_t vt=0,owner=0;
    if(!p.entry||!read(p.declaration+0x60,&type,4)||type!=9||!read(p.entry+0x20,&p.quantity,4)||(p.quantity!=0&&p.quantity!=1)||
       !read(p.entry+0x24,&maximum,4)||maximum!=1||!read(p.entry+8,&p.handle,4)||!read(p.entry+0x10,&p.component,8)||!p.component||
       !read(p.component,&vt,8)||vt!=base+(p.player.character==1?0x1b9bb78:0x1b52818))return false;
    p.entity=worldIdentityEntity(read,inv.game,p.handle);
    if(!p.entity||!read(p.entity+0xac,&parent,4)||parent!=1||!read(p.component+0x10,&owner,8)||owner!=p.entity)return false;
    std::uintptr_t name=0;std::string equipmentName;
    if(!read(p.entry+0x18,&p.equipmentDeclaration,8)||!p.equipmentDeclaration||!read(p.component+8,&owner,8)||owner!=p.equipmentDeclaration||
       !read(p.equipmentDeclaration,&vt,8)||vt!=base+(p.player.character==1?0x1b9ba38:0x1b526d8)||
       !read(p.equipmentDeclaration+8,&name,8)||!worldIdentityString(read,name,equipmentName)||
       equipmentName!=(p.player.character==1?"components/characters/player/base/powers/blink/blink.cpntpowerblink":"components/characters/player/base/powers/grapple/grapple.cpntpowergrapple")||
       !read(p.equipmentDeclaration+0x68,&p.slot,4)||p.slot!=1||!read(p.component+0x1c,&p.flags,4))return false;
    p.equipped=inv.slots[1]==p.entry;p.started=(p.flags&0xc)==8;
    if((p.flags&0xc)!=0&&!p.started)return false;
    std::uintptr_t list=0;unsigned count=0,membership=0;
    if(!read(p.entity+0x268,&list,8)||!list||!read(p.entity+0x270,&count,4)||(count&0xffffff)>64)return false;
    for(unsigned i=0;i<(count&0xffffff);++i){std::uintptr_t c=0;if(!read(list+i*8,&c,8))return false;if(c==p.component)++membership;}
    if(membership!=1)return false;
    NativePlayerSword final{};std::uintptr_t again=0;int quantity=0;
    if(!readNativePlayerSword(read,base,final)||!sameSwordEquipmentIdentity(p.player.sword,final.sword)||final.character!=p.player.character||
       !read(p.entry,&again,8)||again!=p.declaration||!read(p.entry+0x10,&again,8)||again!=p.component||!read(p.entry+0x20,&quantity,4)||quantity!=p.quantity||
       !read(p.component+0x1c,&count,4)||count!=p.flags||!read(p.component+8,&again,8)||again!=p.equipmentDeclaration||
       worldIdentityEntity(read,inv.game,p.handle)!=p.entity)return false;
    out=std::move(p);return true;
}
