#pragma once
#include "native_player_sword.h"
// Exact scalar consumables observed in both native Dunwall inventories.
// No weapon entity, equip call or invented animation ownership.
struct NativePlayerPotion {
    NativePlayerSword player{};std::uintptr_t entry=0,declaration=0;
    unsigned kind=0;int quantity=0,maximum=0;
};
template<class Read> bool readNativePlayerPotion(Read read,std::uintptr_t base,unsigned kind,NativePlayerPotion& out){
    if(kind!=1&&kind!=2)return false;
    NativePlayerPotion p{};p.kind=kind;
    if(!readNativePlayerSword(read,base,p.player)||p.player.sword.logic!=1)return false;
    const auto& inv=p.player.sword;
    const char* wanted=kind==1?"components/interactive/health_potion.inventoryitem":"components/interactive/mana_potion.inventoryitem";
    for(unsigned i=0;i<inv.itemCount;++i){std::uintptr_t entry=inv.items+i*56,decl=0,vt=0,name=0;std::string text;
        if(!read(entry,&decl,8)||!decl||!read(decl,&vt,8))return false;
        if(vt!=base+0x1b9ea48)continue;
        if(!read(decl+8,&name,8)||!worldIdentityString(read,name,text))return false;
        if(text==wanted){if(p.entry)return false;p.entry=entry;p.declaration=decl;}
    }
    unsigned type=0,handle=0;std::uintptr_t component=0,equipment=0;
    if(!p.entry||!read(p.declaration+0x60,&type,4)||type!=14||!read(p.entry+8,&handle,4)||handle!=0x3fff0010||
       !read(p.entry+0x10,&component,8)||component||!read(p.entry+0x18,&equipment,8)||equipment||
       !read(p.entry+0x20,&p.quantity,4)||!read(p.entry+0x24,&p.maximum,4)||p.maximum!=10||p.quantity<0||p.quantity>p.maximum)return false;
    NativePlayerSword final{};std::uintptr_t again=0;int quantity=0,maximum=0;
    if(!readNativePlayerSword(read,base,final)||!sameSwordEquipmentIdentity(inv,final.sword)||final.character!=p.player.character||
       !read(p.entry,&again,8)||again!=p.declaration||!read(p.declaration,&again,8)||again!=base+0x1b9ea48||
       !read(p.declaration+0x60,&type,4)||type!=14||!read(p.entry+8,&handle,4)||handle!=0x3fff0010||
       !read(p.entry+0x10,&again,8)||again||!read(p.entry+0x18,&again,8)||again||
       !read(p.entry+0x20,&quantity,4)||quantity!=p.quantity||!read(p.entry+0x24,&maximum,4)||maximum!=p.maximum)return false;
    out=std::move(p);return true;
}
inline bool samePotionObjects(const NativePlayerPotion& a,const NativePlayerPotion& b){
    return a.kind==b.kind&&a.player.character==b.player.character&&a.player.sword.game==b.player.sword.game&&a.player.sword.actor==b.player.sword.actor&&
        a.player.sword.inventory==b.player.sword.inventory&&a.player.sword.items==b.player.sword.items&&a.player.sword.itemCount==b.player.sword.itemCount&&
        a.entry==b.entry&&a.declaration==b.declaration&&a.maximum==b.maximum;
}
