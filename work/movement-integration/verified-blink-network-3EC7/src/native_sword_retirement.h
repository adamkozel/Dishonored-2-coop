#pragma once
#include "native_sword_equipment.h"

// In the observed native death path the inventory removes the equipment
// component while leaving a physical sword entity. Confirm removal from the
// same inventory and live entity without reading the destroyed component.
template<class Read> bool nativeSwordDeathRetirementFrame(Read read,std::uintptr_t base,const NativeSwordEquipment& before) {
    NativeHealth health{};
    if(before.handle==1||!readNativeHealth(read,base,before.handle,health)||health.logic!=1||health.current!=0||health.maximum!=40||
       health.game!=before.game||health.actor!=before.actor)return false;
    std::uintptr_t vt=0,owner=0,items=0,equipments=0;std::uint32_t itemCount=0,equipmentCount=0;
    unsigned inventories=0;
    for(unsigned i=0;i<(health.componentCount&0xffffff);++i){
        std::uintptr_t c=0;if(!read(health.list+i*8,&c,8)||!c||!read(c,&vt,8))return false;
        if(vt==base+0x1b9a878){if(c!=before.inventory||++inventories!=1)return false;}
    }
    if(inventories!=1||!read(before.inventory+0x10,&owner,8)||owner!=health.actor||
       !read(before.inventory+0x38,&items,8)||items!=before.items||!read(before.inventory+0x40,&itemCount,4)||
       !(itemCount&0xffffff)||(itemCount&0xffffff)>256||before.entry<items||
       before.entry>=items+(itemCount&0xffffff)*56ull||(before.entry-items)%56||
       !read(before.inventory+0x58,&equipments,8)||equipments!=before.equipments||
       !read(before.inventory+0x60,&equipmentCount,4)||(equipmentCount&0xffffff)>64)return false;
    std::uintptr_t decl=0,name=0,component=0,equipmentDecl=0;std::string itemName;
    std::uint32_t handle=0;std::int32_t quantity=0,maximum=0;
    if(!read(before.entry,&decl,8)||decl!=before.itemDeclaration||!read(decl,&vt,8)||vt!=base+0x1b9ea48||
       !read(decl+8,&name,8)||!worldIdentityString(read,name,itemName)||itemName!=kGuardSwordItem||
       !read(before.entry+8,&handle,4)||handle!=0x3fff0010||!read(before.entry+0x10,&component,8)||component||
       !read(before.entry+0x18,&equipmentDecl,8)||equipmentDecl||!read(before.entry+0x20,&quantity,4)||quantity||
       !read(before.entry+0x24,&maximum,4)||maximum!=1)return false;
    std::vector<std::uintptr_t> entries(equipmentCount&0xffffff);std::array<std::uintptr_t,16> slots{};
    if((!entries.empty()&&!read(equipments,entries.data(),entries.size()*8))||!read(before.inventory+0x68,slots.data(),sizeof(slots)))return false;
    for(auto entry:entries)if(entry==before.entry||entry<items||entry>=items+(itemCount&0xffffff)*56ull||(entry-items)%56)return false;
    for(auto slot:slots)if(slot&&(slot==before.entry||std::find(entries.begin(),entries.end(),slot)==entries.end()))return false;
    // The observed dropped sword remains a live idEntity with a rigid body,
    // but no melee equipment component. A replaced or absent entity is not
    // classified by this narrow retirement reader.
    std::uintptr_t list=0;std::uint32_t count=0;
    if(worldIdentityEntity(read,health.game,before.weaponHandle)!=before.entity||!read(before.entity,&vt,8)||vt!=base+0x1c87d58||
       !read(before.entity+0x268,&list,8)||!list||!read(before.entity+0x270,&count,4)||!(count&0xffffff)||(count&0xffffff)>64)return false;
    for(unsigned i=0;i<(count&0xffffff);++i){
        std::uintptr_t c=0;if(!read(list+i*8,&c,8)||!c||c==before.component||!read(c,&vt,8)||vt==base+0x1b9cf48||
            !read(c+0x10,&owner,8)||owner!=before.entity)return false;
    }
    return true;
}
template<class Read> bool readNativeSwordDeathRetirement(Read read,std::uintptr_t base,const NativeSwordEquipment& before) {
    return nativeSwordDeathRetirementFrame(read,base,before)&&nativeSwordDeathRetirementFrame(read,base,before);
}
