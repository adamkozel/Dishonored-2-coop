#pragma once
#include "native_bonecharm.h"
// 2112 (#265): the crafted charm codec (native_bonecharm.h) against hand-built bytes in the serializer's format.
void nativeBoneCharmTests(){
    std::vector<NativeCharmRecord> cr;NativeCharmRecord found{};found.key={13858082,-1,""};found.flags=3;cr.push_back(found);
    NativeCharmRecord named{};named.key={0,-1,"coop_lab_crafted"};named.flags=2;cr.push_back(named);
    const std::string cb=encodeNativeCharmList(cr);
    require(cb.size()==2+(4+2+0+4+1)+(4+2+16+4+1),"2112: a charm list is a u16 count, then key and flag byte per entry");
    std::vector<NativeCharmRecord> cr2;require(decodeNativeCharmList(cb,cr2)&&cr2.size()==2&&cr2[1].key.name=="coop_lab_crafted"&&cr2[0].key.kind==13858082&&cr2[0].flags==3,"2112: a charm list round-trips");
    require(!decodeNativeCharmList(cb.substr(0,cb.size()-1),cr2),"2112: a short charm list is refused");
    require(!decodeNativeCharmList(cb+'x',cr2),"2112: trailing bytes are refused");
    std::string big;charmPutCount(big,70000);require(big.size()==6&&static_cast<unsigned char>(big[0])==0xff&&static_cast<unsigned char>(big[1])==0xff,"2112: a count of 0xffff or more is 0xffff then a u32");
    require(!decodeNativeCharmList(big,cr2),"2112: a charm count above 1024 is refused");
    auto flagged=cb;flagged[2+4+2+4]=static_cast<char>(0x13);require(!decodeNativeCharmList(flagged,cr2),"2112: flag bits above the low four are refused");
    std::vector<NativeCraftedRecord> kr;NativeCraftedRecord k{};k.key=named.key;k.a=1;k.b=2;k.traits={11,12};k.last=13;kr.push_back(k);
    const std::string kb=encodeNativeCraftedList(kr);
    require(kb.size()==2+(4+2+16+4)+4+4+2+8+4,"2112: a crafted record is a key, two ints, the trait ids and an int");
    std::vector<NativeCraftedRecord> kr2;require(decodeNativeCraftedList(kb,kr2)&&kr2.size()==1&&kr2[0].traits.size()==2&&kr2[0].traits[1]==12&&kr2[0].last==13&&kr2[0].b==2,"2112: a crafted list round-trips");
    auto unnamed=kr;unnamed[0].key.name.clear();require(!decodeNativeCraftedList(encodeNativeCraftedList(unnamed),kr2),"2112: a crafted record needs a name");
    std::string h;require(charmUnhex(charmHex(kb),h)&&h==kb,"2112: the hex lines round-trip");
    require(!charmUnhex("abc",h)&&!charmUnhex("zz",h),"2112: odd or non-hex text is refused");
    NativeBoneCharms now{};NativeBoneCharmEntry e{};e.key=found.key;now.entries.push_back(e);now.charmCount=1;
    require(nativeCraftedRestoreFits(now,cr,kr),"2112: saved lists fit a fresh world with the same found charms");
    auto other=now;other.entries[0].key.kind=5;require(!nativeCraftedRestoreFits(other,cr,kr),"2112: another found charm list does not fit");
    auto crafted=now;crafted.craftedCount=1;require(!nativeCraftedRestoreFits(crafted,cr,kr),"2112: a world that has a crafted charm is left alone");
    std::vector<NativeCharmRecord> onlyFound{found};require(!nativeCraftedRestoreFits(now,onlyFound,kr),"2112: a crafted record needs its named entry");
    require(!nativeCraftedRestoreFits(now,onlyFound,{}),"2112: lists without a crafted charm are not restored");
}
