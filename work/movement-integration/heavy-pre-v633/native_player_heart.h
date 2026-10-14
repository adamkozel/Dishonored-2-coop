#pragma once
#include "native_player_sword.h"
// B671F824 native Heart. Exact component, declaration, entity and slot
// identities observed in both roles in native-gadgets-research-v561.
// No item grants, equip calls, projectile creation or gameplay writes here.
struct NativePlayerHeart {
    NativePlayerSword player{};
    std::uintptr_t entry=0,declaration=0,component=0,equipmentDeclaration=0,entity=0;
    unsigned handle=0,flags=0;int quantity=0,maximum=0,slot=-1;
    bool equipped=false,started=false;
};
template<class Read> bool readNativePlayerHeartFrame(Read read,std::uintptr_t base,NativePlayerHeart& p,unsigned* stage=nullptr,bool selectedOnly=false){
    if(stage)*stage=1;
    if(!readNativePlayerSword(read,base,p.player)||p.player.sword.logic!=1)return false;
    const auto& inv=p.player.sword;
    // Idle presentation needs only the selected left-hand item. The complete
    // inventory scan remains available for acquisition and unequip scenes.
    unsigned first=0,end=inv.itemCount;
    if(selectedOnly){
        const auto selected=inv.slots[1];
        if(selected<inv.items||selected>=inv.items+inv.itemCount*56||(selected-inv.items)%56)return false;
        unsigned membership=0;
        for(unsigned i=0;i<inv.equipmentCount;++i){std::uintptr_t entry=0;
            if(!read(inv.equipments+i*8,&entry,8))return false;if(entry==selected)++membership;}
        if(membership!=1)return false;
        first=static_cast<unsigned>((selected-inv.items)/56);end=first+1;
    }
    for(unsigned i=first;i<end;++i){std::uintptr_t entry=inv.items+i*56,d=0,vt=0,text=0;std::string name;
        if(!read(entry,&d,8)||!d||!read(d,&vt,8))return false;
        if(vt!=base+0x1b9ea48)continue;
        if(!read(d+8,&text,8)||!worldIdentityString(read,text,name))return false;
        if(name=="models/items/heart/heart.inventoryitem"){
            if(p.entry)return false;p.entry=entry;p.declaration=d;
        }
    }
    if(stage)*stage=2;
    unsigned type=0,parent=0;std::uintptr_t vt=0,owner=0,text=0;std::string name;
    if(!p.entry||!read(p.declaration+0x60,&type,4)||type!=2||!read(p.entry+8,&p.handle,4)||
       !read(p.entry+0x10,&p.component,8)||!p.component||!read(p.entry+0x18,&p.equipmentDeclaration,8)||!p.equipmentDeclaration||
       !read(p.entry+0x20,&p.quantity,4)||!read(p.entry+0x24,&p.maximum,4)||p.maximum!=1||p.quantity<0||p.quantity>p.maximum||
       !read(p.component,&vt,8)||vt!=base+0x1b99c68||!read(p.component+8,&owner,8)||owner!=p.equipmentDeclaration||
       !read(p.equipmentDeclaration,&vt,8)||vt!=base+0x1b99b28||!read(p.equipmentDeclaration+8,&text,8)||
       !worldIdentityString(read,text,name)||name!="components/characters/player/base/items/heart.cpntheart"||
       !read(p.equipmentDeclaration+0x68,&p.slot,4)||p.slot!=1||!read(p.component+0x1c,&p.flags,4))return false;
    if(stage)*stage=3;
    p.entity=worldIdentityEntity(read,inv.game,p.handle);
    // This native idEntity has a different vtable from the gun/crossbow entity.
    // Both role snapshots confirm RVA 1c87d58 and parent handle 1.
    if(!p.entity||!read(p.entity,&vt,8)||vt!=base+0x1c87d58||!read(p.entity+0xac,&parent,4)||parent!=1||
       !read(p.component+0x10,&owner,8)||owner!=p.entity)return false;
    if(stage)*stage=4;
    p.equipped=inv.slots[1]==p.entry;p.started=(p.flags&0xc)==8;
    if(((p.flags&0xc)!=0&&!p.started)||p.started!=p.equipped||(selectedOnly&&!p.equipped))return false;
    if(stage)*stage=5;
    std::uintptr_t list=0;unsigned count=0,membership=0;
    if(!read(p.entity+0x268,&list,8)||!list||!read(p.entity+0x270,&count,4)||!(count&0xffffff)||(count&0xffffff)>64)return false;
    for(unsigned i=0;i<(count&0xffffff);++i){std::uintptr_t c=0;if(!read(list+i*8,&c,8))return false;if(c==p.component)++membership;}
    return membership==1;
}
template<class Read> bool readNativePlayerHeart(Read read,std::uintptr_t base,NativePlayerHeart& out,unsigned* stage=nullptr,bool selectedOnly=false){
    NativePlayerHeart a{},b{};
    if(!readNativePlayerHeartFrame(read,base,a,stage,selectedOnly)||!readNativePlayerHeartFrame(read,base,b,stage,selectedOnly)||
       !sameSwordEquipmentIdentity(a.player.sword,b.player.sword)||a.player.character!=b.player.character||
       a.entry!=b.entry||a.declaration!=b.declaration||a.component!=b.component||a.equipmentDeclaration!=b.equipmentDeclaration||
       a.entity!=b.entity||a.handle!=b.handle||a.quantity!=b.quantity||a.flags!=b.flags)return false;
    out=std::move(b);return true;
}
