#pragma once
#include "native_player_sword.h"
struct NativePossessionHumanUpgrade {
    NativePlayerSword player{};std::uintptr_t entry=0,declaration=0;int quantity=0;
};
// Exact installed TYPE_POWER_UPGRADE20: possession_parasite grants
// ARK_ATTRIBUTE_POSSESSION_ALLOW_HUMANS. Read-only, Corvo-only, no target writes.
template<class Read> bool readNativePossessionHumanUpgrade(Read read,std::uintptr_t base,NativePossessionHumanUpgrade& out){
    NativePossessionHumanUpgrade p{};if(!readNativePlayerSword(read,base,p.player)||p.player.sword.logic!=1||p.player.character!=1)return false;
    const auto& inv=p.player.sword;
    for(unsigned i=0;i<inv.itemCount;++i){std::uintptr_t entry=inv.items+i*56,decl=0,vt=0,name=0;std::string text;
        if(!read(entry,&decl,8)||!decl||!read(decl,&vt,8))return false;
        if(vt!=base+0x1b9ea48)continue;
        if(!read(decl+8,&name,8)||!worldIdentityString(read,name,text))return false;
        if(text=="components/characters/player/base/attributes/possession_parasite.inventoryitem"){
            if(p.entry)return false;p.entry=entry;p.declaration=decl;
        }
    }
    unsigned type=0,handle=0;int maximum=0;std::uintptr_t component=0,equipment=0;
    if(!p.entry||!read(p.declaration+0x60,&type,4)||type!=20||!read(p.entry+8,&handle,4)||handle!=0x3fff0010||
       !read(p.entry+0x10,&component,8)||component||!read(p.entry+0x18,&equipment,8)||equipment||
       !read(p.entry+0x20,&p.quantity,4)||(p.quantity!=0&&p.quantity!=1)||!read(p.entry+0x24,&maximum,4)||maximum!=1)return false;
    NativePlayerSword final{};std::uintptr_t again=0;int quantity=0;
    if(!readNativePlayerSword(read,base,final)||!sameSwordEquipmentIdentity(inv,final.sword)||final.character!=p.player.character||
       !read(p.entry,&again,8)||again!=p.declaration||!read(p.declaration,&again,8)||again!=base+0x1b9ea48||
       !read(p.declaration+0x60,&type,4)||type!=20||!read(p.entry+8,&handle,4)||handle!=0x3fff0010||
       !read(p.entry+0x10,&again,8)||again||!read(p.entry+0x18,&again,8)||again||
       !read(p.entry+0x20,&quantity,4)||quantity!=p.quantity)return false;
    out=std::move(p);return true;
}
