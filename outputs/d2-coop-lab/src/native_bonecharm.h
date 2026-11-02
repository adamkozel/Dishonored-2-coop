#pragma once
#include "native_health.h"
#include "world_identity.h"
#include <string>
#include <vector>
// 2107 (#265): the player's arkCpntBoneCharmInventory (vtable 0x141b99928; d2-ghidra, confirmed static).
// - +0x38/+0x40: every charm the game knows, stride 0x40. Each starts with its key (int kind at +0, idStr name at +8,
//   int at +0x30; keys compare on kind and name, 0x14060b1d0), and flag byte +0x38 bit 1 is owned (give 0x14060e4c0
//   sets it, and equips too when the declaration's +0x78 flag is set).
// - +0x48/+0x50: crafted charms, stride 0x58.
// - +0x58/+0x60: equipped slots, stride 0x38, each a key; an empty slot has kind 0 and an empty name (0x14060b3e0).
//   Equip 0x14060f520 fills the first empty slot of an owned charm; unequip 0x140611350 empties the matching slot.
// An idStr is 0x28 bytes: char* data at +0, the length in the low 29 bits of +8 (allocator bits above), capacity
// and flags at +0xc, an inline buffer at +0x10 (0x14013f490, 0x140143bb0; confirmed static).
struct NativeBoneCharmKey {std::int32_t kind=0,extra=0;std::string name;};
struct NativeBoneCharmEntry {NativeBoneCharmKey key;unsigned char flags=0;bool owned=false;};
struct NativeBoneCharms {
    std::uintptr_t actor=0,component=0,charms=0,crafted=0,slots=0;const char* why="";
    std::uint32_t charmCount=0,craftedCount=0,slotCount=0;
    std::vector<NativeBoneCharmEntry> entries;std::vector<NativeBoneCharmKey> equipped; // equipped: one per slot, empty name when free
};
template<class Read> bool readNativeBoneCharmKey(Read read,std::uintptr_t at,NativeBoneCharmKey& out){
    std::uintptr_t data=0;std::uint32_t lengthBits=0;
    if(!read(at,&out.kind,4)||!read(at+8,&data,8)||!read(at+0x10,&lengthBits,4)||!read(at+0x30,&out.extra,4))return false;
    const std::uint32_t length=lengthBits&0x1fffffff;if(length>190)return false;
    out.name.clear();if(!length)return true;
    return data&&worldIdentityString(read,data,out.name)&&out.name.size()==static_cast<std::size_t>(length);
}
template<class Read> bool readNativeBoneCharms(Read read,std::uintptr_t base,NativeBoneCharms& out){
    out=NativeBoneCharms{};NativeHealth actor{};
    if(!readNativeHealth(read,base,1,actor)||(actor.logic!=1&&actor.logic!=2)){out.why="player";return false;}
    out.actor=actor.actor;
    for(unsigned i=0;i<(actor.componentCount&0xffffff);++i){std::uintptr_t c=0,vt=0;
        if(!read(actor.list+i*8,&c,8)||!c||!read(c,&vt,8)){out.why="components";return false;}
        if(vt==base+0x1b99928){if(out.component){out.why="two_components";return false;}out.component=c;}}
    std::uintptr_t owner=0;std::uint32_t flags=0;
    if(!out.component||!read(out.component+0x10,&owner,8)||owner!=out.actor||
       !read(out.component+0x38,&out.charms,8)||!read(out.component+0x40,&flags,4)){out.why="component";return false;}
    out.charmCount=flags&0xffffff;if(out.charmCount>1024||(out.charmCount&&!out.charms)){out.why="charm_list";return false;}
    if(!read(out.component+0x48,&out.crafted,8)||!read(out.component+0x50,&flags,4)){out.why="crafted_list";return false;}
    out.craftedCount=flags&0xffffff;if(out.craftedCount>256){out.why="crafted_count";return false;}
    if(!read(out.component+0x58,&out.slots,8)||!read(out.component+0x60,&flags,4)){out.why="slot_list";return false;}
    out.slotCount=flags&0xffffff;if(out.slotCount>16||(out.slotCount&&!out.slots)){out.why="slot_count";return false;}
    for(std::uint32_t i=0;i<out.charmCount;++i){NativeBoneCharmEntry e{};const auto at=out.charms+static_cast<std::uintptr_t>(i)*0x40;
        if(!readNativeBoneCharmKey(read,at,e.key)||!read(at+0x38,&e.flags,1)){out.why="charm_key";return false;}
        e.owned=(e.flags&2)!=0;out.entries.push_back(std::move(e));}
    for(std::uint32_t i=0;i<out.slotCount;++i){NativeBoneCharmKey k{};
        if(!readNativeBoneCharmKey(read,out.slots+static_cast<std::uintptr_t>(i)*0x38,k)){out.why="slot_key";return false;}out.equipped.push_back(std::move(k));}
    return true;
}
