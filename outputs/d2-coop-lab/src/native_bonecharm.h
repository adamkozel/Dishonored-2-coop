#pragma once
#include "native_health.h"
#include "world_identity.h"
#include <cstring>
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
// 2112 (#265): crafted charms. A crafted charm is a named entry in the +0x38 list (owned like a found one: equip
// 0x14060f520 checks the owned bit there, 0x1406101f0) and a record in the +0x48 list (key, ints +0x38 and +0x3c, a
// trait id list at +0x40, an int at +0x50; 0x140610f50) that the equip applies through 0x14060e310. The game grows the
// +0x48 list only in its save reader, so both lists are carried in the component serializer's own byte format
// (writers 0x14060dee0 and 0x14060de50, readers 0x14060db30 and 0x14060da30; d2-ghidra, confirmed static):
// - a count is a u16, or 0xffff then a u32;
// - a key is its kind (4 bytes), its name (a count, then that many bytes) and, from version 0x1c, its +0x30 int;
// - a +0x38 entry is a key and a flag byte (the low 4 bits);
// - a crafted record is a key, two ints, a count of trait ids (4 bytes each) and an int.
// The codec below checks saved bytes before the native reader sees them, and builds the lab charm.
struct NativeCharmRecord {NativeBoneCharmKey key;std::uint8_t flags=0;};
struct NativeCraftedRecord {NativeBoneCharmKey key;std::int32_t a=0,b=0,last=0;std::vector<std::int32_t> traits;};
inline void charmPutBytes(std::string& o,const void* p,std::size_t n){o.append(static_cast<const char*>(p),n);}
inline void charmPutCount(std::string& o,std::uint32_t n){if(n<0xffff){const std::uint16_t s=static_cast<std::uint16_t>(n);charmPutBytes(o,&s,2);return;}
    const std::uint16_t m=0xffff;charmPutBytes(o,&m,2);charmPutBytes(o,&n,4);}
inline bool charmGetBytes(const std::string& s,std::size_t& at,void* p,std::size_t n){if(n>s.size()||at>s.size()-n)return false;std::memcpy(p,s.data()+at,n);at+=n;return true;}
inline bool charmGetCount(const std::string& s,std::size_t& at,std::uint32_t& n){std::uint16_t c=0;if(!charmGetBytes(s,at,&c,2))return false;
    if(c!=0xffff){n=c;return true;}return charmGetBytes(s,at,&n,4);}
inline void charmPutKey(std::string& o,const NativeBoneCharmKey& k){charmPutBytes(o,&k.kind,4);charmPutCount(o,static_cast<std::uint32_t>(k.name.size()));
    o+=k.name;charmPutBytes(o,&k.extra,4);}
inline bool charmGetKey(const std::string& s,std::size_t& at,NativeBoneCharmKey& k){std::uint32_t n=0;
    if(!charmGetBytes(s,at,&k.kind,4)||!charmGetCount(s,at,n)||n>190||n>s.size()-at)return false;
    k.name.assign(s.data()+at,n);at+=n;return charmGetBytes(s,at,&k.extra,4);}
inline std::string encodeNativeCharmList(const std::vector<NativeCharmRecord>& v){std::string o;charmPutCount(o,static_cast<std::uint32_t>(v.size()));
    for(const auto& r:v){charmPutKey(o,r.key);const std::uint8_t f=r.flags&0xf;charmPutBytes(o,&f,1);}return o;}
inline bool decodeNativeCharmList(const std::string& s,std::vector<NativeCharmRecord>& v){v.clear();std::size_t at=0;std::uint32_t n=0;
    if(!charmGetCount(s,at,n)||n>1024)return false;
    for(std::uint32_t i=0;i<n;++i){NativeCharmRecord r{};if(!charmGetKey(s,at,r.key)||!charmGetBytes(s,at,&r.flags,1)||(r.flags&0xf0))return false;v.push_back(std::move(r));}
    return at==s.size();}
inline std::string encodeNativeCraftedList(const std::vector<NativeCraftedRecord>& v){std::string o;charmPutCount(o,static_cast<std::uint32_t>(v.size()));
    for(const auto& r:v){charmPutKey(o,r.key);charmPutBytes(o,&r.a,4);charmPutBytes(o,&r.b,4);charmPutCount(o,static_cast<std::uint32_t>(r.traits.size()));
        for(const auto t:r.traits)charmPutBytes(o,&t,4);charmPutBytes(o,&r.last,4);}return o;}
inline bool decodeNativeCraftedList(const std::string& s,std::vector<NativeCraftedRecord>& v){v.clear();std::size_t at=0;std::uint32_t n=0;
    if(!charmGetCount(s,at,n)||n>256)return false;
    for(std::uint32_t i=0;i<n;++i){NativeCraftedRecord r{};std::uint32_t t=0;
        if(!charmGetKey(s,at,r.key)||r.key.name.empty()||!charmGetBytes(s,at,&r.a,4)||!charmGetBytes(s,at,&r.b,4)||!charmGetCount(s,at,t)||t>64)return false;
        r.traits.resize(t);for(auto& x:r.traits)if(!charmGetBytes(s,at,&x,4))return false;
        if(!charmGetBytes(s,at,&r.last,4))return false;v.push_back(std::move(r));}
    return at==s.size();}
// Saved lists fit this world when its found charms are the saved ones in the same order (the same declarations), the
// saved crafted charms each have their named entry, and this world has no crafted charm yet (a join's fresh load).
inline bool nativeCraftedRestoreFits(const NativeBoneCharms& now,const std::vector<NativeCharmRecord>& charms,const std::vector<NativeCraftedRecord>& crafted){
    if(now.craftedCount)return false;std::size_t found=0;
    for(const auto& e:now.entries){if(!e.key.name.empty())return false;
        if(found>=charms.size()||!charms[found].key.name.empty()||charms[found].key.kind!=e.key.kind)return false;++found;}
    for(std::size_t i=found;i<charms.size();++i)if(charms[i].key.name.empty())return false;
    for(const auto& c:crafted){bool named=false;for(std::size_t i=found;i<charms.size();++i)if(charms[i].key.name==c.key.name&&charms[i].key.kind==c.key.kind){named=true;break;}
        if(!named)return false;}
    return charms.size()>found;}
inline std::string charmHex(const std::string& s){static const char* d="0123456789abcdef";std::string o;o.reserve(s.size()*2);
    for(const unsigned char c:s){o+=d[c>>4];o+=d[c&15];}return o;}
inline bool charmUnhex(const std::string& h,std::string& o){o.clear();if(h.size()%2)return false;
    auto v=[](char c)->int{return c>='0'&&c<='9'?c-'0':c>='a'&&c<='f'?c-'a'+10:-1;};
    for(std::size_t i=0;i<h.size();i+=2){const int x=v(h[i]),y=v(h[i+1]);if(x<0||y<0)return false;o+=static_cast<char>(x*16+y);}return true;}
