#pragma once
#include "native_health.h"
#include "native_player_mana.h"
#include "native_stack_item.h"
#include "food_outcome.h"
// Exact retail player health/mana/food identity and queued native restoration.
struct NativeFood {
    NativeHealth health{};NativePlayerMana mana{};NativeStackItem stack{};PickupFoodValue value{};
    std::uintptr_t manaAttributes=0,foodAttributes=0;
    int manaIndex=-1,foodIndex=-1,declaredHealth=0,declaredMana=0;
    unsigned healthGeneration=0,manaGeneration=0;
};
inline bool sameNativeFood(const NativeFood& a,const NativeFood& b){
    return sameNativeStackItem(a.stack,b.stack)&&a.health.game==b.health.game&&a.health.actor==b.health.actor&&a.health.component==b.health.component&&
        a.health.declaration==b.health.declaration&&a.health.attributes==b.health.attributes&&a.health.table==b.health.table&&a.health.attributeCount==b.health.attributeCount&&
        a.healthGeneration==b.healthGeneration&&a.mana.component==b.mana.component&&a.mana.declaration==b.mana.declaration&&a.manaGeneration==b.manaGeneration&&
        a.manaAttributes==b.manaAttributes&&a.foodAttributes==b.foodAttributes&&a.manaIndex==b.manaIndex&&a.foodIndex==b.foodIndex&&
        a.declaredHealth==b.declaredHealth&&a.declaredMana==b.declaredMana&&sameFoodRules(a.value,b.value);
}
template<class Read> bool readNativeFoodFrame(Read read,std::uintptr_t base,std::uintptr_t inventory,std::uintptr_t owner,std::uintptr_t item,NativeFood& out){
    NativeFood f{};auto& v=f.value;unsigned flags=0;unsigned char enabled=0;float manaBase=0,foodBonus=0,manaBonus=0;
    const unsigned char overflowDisabled[]={0x32,0xc0,0xc3};unsigned char bytes[3]{};
    if(!read(base+0x399620,bytes,3)||std::memcmp(bytes,overflowDisabled,3)||
       !readNativeStackItem(read,base,inventory,owner,item,6,f.stack,true)||!readNativeHealth(read,base,1,f.health)||f.health.logic!=1||f.health.actor!=owner||
       !readNativePlayerMana(read,base,f.mana)||f.mana.player!=owner||f.mana.game!=f.health.game||
       !read(f.health.component+0x18,&f.healthGeneration,4)||f.healthGeneration==~0u||!read(f.health.component+0x1c,&flags,4)||(flags&12)!=8||
       !read(f.mana.component+0x18,&f.manaGeneration,4)||f.manaGeneration==~0u||!read(f.mana.component+0x1c,&flags,4)||(flags&12)!=8||
       !read(f.health.component+0xf4,&v.healthTarget,4)||!read(f.mana.component+0x2c,&v.manaTarget,4)||!read(f.mana.declaration+0x68,&manaBase,4)||
       !read(f.mana.component+0x48,&f.manaAttributes,8)||!read(f.mana.component+0x68,&f.manaIndex,4)||
       !read(inventory+0xf0,&f.foodAttributes,8)||!read(inventory+0x174,&f.foodIndex,4)||
       !read(item+0x78,&f.declaredHealth,4)||!read(item+0x7c,&f.declaredMana,4)||f.declaredHealth<0||f.declaredMana<0||
       !read(base+0x398dbd8,&enabled,1))return false;
    const auto attribute=[&](std::uintptr_t attributes,int index,float& value){
        if(index<-1)return false;
        if(!attributes)return index<0;
        if(attributes!=f.health.attributes)return false;
        if(index<0)return true;
        return f.health.table&&index<static_cast<int>(f.health.attributeCount&0xffffff)&&read(f.health.table+static_cast<std::uintptr_t>(index)*40+0x24,&value,4)&&std::isfinite(value);
    };
    if(!attribute(f.manaAttributes,f.manaIndex,manaBonus)||!attribute(f.foodAttributes,f.foodIndex,foodBonus))return false;
    v.health=f.health.current;v.healthMaximum=f.health.maximum;v.mana=f.mana.current;v.manaMaximum=manaBase+manaBonus;
    v.healthGrant=f.declaredHealth+foodBonus;v.manaGrant=static_cast<float>(f.declaredMana);v.healthSuppressed=(f.health.ownerFlags&8)!=0;v.manaEnabled=enabled!=0;
    v.stored=f.stack.quantity;v.storageMaximum=f.stack.maximum;
    if(!validFoodValue(v))return false;out=std::move(f);return true;
}
template<class Read> bool readNativeFood(Read read,std::uintptr_t base,std::uintptr_t inventory,std::uintptr_t owner,std::uintptr_t item,NativeFood& out){
    NativeFood a{},b{};
    if(!readNativeFoodFrame(read,base,inventory,owner,item,a)||!readNativeFoodFrame(read,base,inventory,owner,item,b)||!sameNativeFood(a,b)||
       a.value.health!=b.value.health||a.value.healthTarget!=b.value.healthTarget||a.value.mana!=b.value.mana||a.value.manaTarget!=b.value.manaTarget||a.value.stored!=b.value.stored)return false;
    out=std::move(a);return true;
}
