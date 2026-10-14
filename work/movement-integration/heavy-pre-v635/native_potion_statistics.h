#pragma once
#include "native_player_stance.h"
#include <array>

// Read-only diagnostic. Exact retail 1407194a0/140745b20 increment IDs
// 0x16/0x17 through 14067aea0(game+0x1f77c8,id,1,...). That routine updates
// arrays at +44 and +100. Their campaign/reset semantics are not yet a
// network event contract. Never infer use from an inventory decrement alone.
struct NativePotionStatistics {
    std::uintptr_t game=0,player=0;
    std::array<std::int32_t,2> a{},b{}; // health, mana in each native array
};
template<class Read> bool readNativePotionStatistics(Read read,std::uintptr_t base,NativePotionStatistics& out){
    NativePlayerStance first{},last{};NativePotionStatistics s{};
    if(!readNativePlayerStance(read,base,first))return false;
    s.game=first.game;s.player=first.player;
    const auto stats=s.game+0x1f77c8;
    if(!read(stats+0x44+0x16*4,s.a.data(),8)||!read(stats+0x100+0x16*4,s.b.data(),8))return false;
    for(auto v:s.a)if(v<0||v>1000000000)return false;
    for(auto v:s.b)if(v<0||v>1000000000)return false;
    std::array<std::int32_t,2> a{},b{};
    if(!readNativePlayerStance(read,base,last)||last.game!=s.game||last.player!=s.player||last.physics!=first.physics||
       !read(stats+0x44+0x16*4,a.data(),8)||a!=s.a||!read(stats+0x100+0x16*4,b.data(),8)||b!=s.b)return false;
    out=s;return true;
}
