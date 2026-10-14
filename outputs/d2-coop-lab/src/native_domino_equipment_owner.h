#pragma once
#include "native_player_domino_item.h"
// A discovered item is only a hint. Revalidate its current player, inventory,
// entry range/membership, entity generation and inherited declaration each time.
// Persistent target marks do not depend on the current first-person sword pose.
template<class Read> bool readCurrentNativeDominoEquipment(Read read,std::uintptr_t base,const NativeDominoItem& known,NativeDominoItem& out){
    if(!known.instantiated||known.player.character!=2)return false;
    NativeDominoItem s=known;const auto& inv=s.player.sword;std::uintptr_t game=0,vt=0,stack=0;unsigned count=0,phase=0,pending=0,logic=0;
    if(!read(base+0x39a5290,&game,8)||!game||game!=inv.game||!read(game,&vt,8)||vt!=base+0x1c940a8||
       !read(game+0x341058,&phase,4)||phase!=3||!read(game+0x1f7400,&pending,4)||pending||
       !read(game+0x1f73f0,&stack,8)||!stack||!read(game+0x1f73f8,&count,4)||!(count&0xffffff)||(count&0xffffff)>32||
       !read(stack+((count&0xffffff)-1)*4,&logic,4)||logic!=1)return false;
    const auto eq=[&](std::uintptr_t at,std::uintptr_t wanted){std::uintptr_t p=0;return read(at,&p,8)&&p==wanted;};
    const auto decl=[&](std::uintptr_t d,std::uintptr_t vt,const char* expected){std::uintptr_t p=0;std::string name;return d&&eq(d,vt)&&read(d+8,&p,8)&&worldIdentityString(read,p,name)&&name==expected;};
    const auto member=[&](std::uintptr_t a,std::uintptr_t c,unsigned limit){std::uintptr_t list=0;unsigned n=0;
        if(!read(a+0x268,&list,8)||!list||!read(a+0x270,&n,4)||!(n&0xffffff)||(n&0xffffff)>limit)return false;
        std::array<std::uintptr_t,256> entries{};n&=0xffffff;if(!read(list,entries.data(),n*8))return false;
        unsigned matches=0;for(unsigned i=0;i<n;++i)if(entries[i]==c)++matches;return matches==1;
    };
    const auto player=worldIdentityEntity(read,game,1);std::uintptr_t definition=0;
    if(!player||player!=inv.actor||!eq(player,base+0x1c5e428)||!read(player+0x2e8,&definition,8)||
       !decl(definition,base+0x1ab58f8,"models/characters/player/emily/body.def")||
       !eq(inv.inventory,base+0x1b9a878)||!eq(inv.inventory+0x10,player)||!member(player,inv.inventory,256))return false;
    std::uintptr_t items=0,equipments=0,selected=0;unsigned n=0,equipmentCount=0;
    if(!read(inv.inventory+0x38,&items,8)||!items||!read(inv.inventory+0x40,&n,4)||!(n&0xffffff)||(n&0xffffff)>1024||
       s.entry<items||s.entry-items>=std::uintptr_t(n&0xffffff)*56||(s.entry-items)%56||
       !read(inv.inventory+0x58,&equipments,8)||!equipments||!read(inv.inventory+0x60,&equipmentCount,4)||!(equipmentCount&0xffffff)||(equipmentCount&0xffffff)>64)return false;
    std::array<std::uintptr_t,64> entries{};equipmentCount&=0xffffff;if(!read(equipments,entries.data(),equipmentCount*8))return false;
    unsigned matches=0;for(unsigned i=0;i<equipmentCount;++i)if(entries[i]==s.entry)++matches;if(matches!=1)return false;
    unsigned handle=0,parent=0,type=0,slot=0;int maximum=0;
    if(!eq(s.entry,s.declaration)||!decl(s.declaration,base+0x1b9ea48,"models/powers/phantasmallink/phantasmal_link.inventoryitem")||
       !read(s.declaration+0x60,&type,4)||type!=9||!read(s.entry+8,&handle,4)||handle!=s.handle||
       !eq(s.entry+0x10,s.component)||!eq(s.entry+0x18,s.equipmentDeclaration)||!read(s.entry+0x20,&s.quantity,4)||(s.quantity!=0&&s.quantity!=1)||
       !read(s.entry+0x24,&maximum,4)||maximum!=1||worldIdentityEntity(read,game,s.handle)!=s.entity||!read(s.entity+0xac,&parent,4)||parent!=1||
       !member(s.entity,s.component,64)||!eq(s.component,base+0x1b52ec8)||!eq(s.component+0x10,s.entity)||!eq(s.component+8,s.equipmentDeclaration)||
       !read(s.component+0x1c,&s.flags,4)||((s.flags&0xc)!=0&&(s.flags&0xc)!=8)||
       !read(s.equipmentDeclaration+0x68,&slot,4)||slot!=1)return false;
    constexpr auto name="components/characters/player/base/powers/phantasmallink/phantasmal_link.cpntpowerphantasmallink";
    if(s.parentDeclaration){
        if(s.parentDeclaration==s.equipmentDeclaration||!decl(s.equipmentDeclaration,base+0x1b52d88,"unnamed")||
           !eq(s.equipmentDeclaration+0x48,s.parentDeclaration)||!decl(s.parentDeclaration,base+0x1b52d88,name))return false;
    }else if(!decl(s.equipmentDeclaration,base+0x1b52d88,name))return false;
    if(!read(inv.inventory+0x70,&selected,8))return false;s.equipped=selected==s.entry;s.started=(s.flags&0xc)==8;
    unsigned countAgain=0;std::uintptr_t stackAgain=0;
    if(worldIdentityEntity(read,game,1)!=player||worldIdentityEntity(read,game,s.handle)!=s.entity||!eq(s.entry+0x10,s.component)||
       !eq(base+0x39a5290,game)||!read(game+0x341058,&phase,4)||phase!=3||!read(game+0x1f7400,&pending,4)||pending||
       !read(game+0x1f73f0,&stackAgain,8)||stackAgain!=stack||!read(game+0x1f73f8,&countAgain,4)||countAgain!=count||
       !read(stack+((count&0xffffff)-1)*4,&logic,4)||logic!=1)return false;
    out=std::move(s);return true;
}
