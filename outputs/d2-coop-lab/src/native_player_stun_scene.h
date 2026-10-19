#pragma once
#include "native_player_states.h"
#include "world_identity.h"
#include <array>
struct NativePlayerStunScene {
    std::uintptr_t instance=0;unsigned handle=0,variant=0,timeMs=0,durationMs=0;
    std::string asset;
};
inline unsigned nativeStunVariant(const std::string& asset){
    constexpr const char* directions[]={"right","front","left","back"};
    for(unsigned i=0;i<4;++i)if(asset==std::string("midnight/gameplay/player/base/hitreact/hit_react_stun_")+directions[i]+".midnightscene")return i+1;
    // 1214dn: the knockdown (KnockedDown, state 16): Scene_1 2599 ms binding a first-person arm then the player (1397f/g).
    if(asset=="midnight/gameplay/player/base/hitreact/hit_react_knocked_down.midnightscene")return 5;
    return 0;
}
template<class Read> bool readNativePlayerStunScene(Read read,std::uintptr_t base,const NativePlayerStates& states,NativePlayerStunScene& out){
    const bool knocked=nativePlayerKnockdownStates(states,base);
    if(!knocked&&!nativePlayerStunStates(states,base))return false;
    NativePlayerStunScene s{};const auto game=states.player.game,manager=game+0x1f5338;
    std::array<std::uintptr_t,2> lists{};std::array<unsigned,2> flags{};
    std::uintptr_t declaration=0,scene=0,bindings=0;unsigned bindingFlags=0;
    for(unsigned field=0;field<2;++field){const auto at=manager+(field?0x610:0);
        if(!read(at,&lists[field],8)||!read(at+8,&flags[field],4)||(flags[field]&0xffffff)>512||((flags[field]&0xffffff)&&!lists[field]))return false;
        for(unsigned i=0;i<(flags[field]&0xffffff);++i){
            std::uintptr_t c=0,d=0,vt=0,name=0;unsigned phase=0;std::string asset;
            if(!read(lists[field]+i*8,&c,8)||!c||!read(c+0x27c,&phase,4))return false;
            if(phase!=3)continue;
            if(!read(c,&d,8)||!d||!read(d,&vt,8)||vt!=base+0x1affbc8||!read(d+8,&name,8)||!worldIdentityString(read,name,asset))return false;
            const auto variant=nativeStunVariant(asset);if(!variant)continue;
            if(s.instance){if(s.instance==c)continue;return false;}
            s.instance=c;s.variant=variant;s.asset=std::move(asset);declaration=d;
        }
    }
    std::uintptr_t vt=0,name=0,actor=0;std::string sub;
    if(s.instance&&(s.variant==5)!=knocked)return false;
    const unsigned playerBinding=s.variant==5?0x28:8,bindingCount=s.variant==5?2:1;
    if(!s.instance||!read(s.instance+8,&scene,8)||!scene||!read(scene,&vt,8)||vt!=base+0x1b00198||
       !read(scene+0x28,&s.durationMs,4)||s.durationMs!=(s.variant==5?2599u:800u)||!read(scene+0x30,&name,8)||!worldIdentityString(read,name,sub)||sub!="Scene_1"||
       !read(s.instance+0x158,&s.handle,4)||!s.handle||!read(s.instance+0x160,&s.timeMs,4)||s.timeMs>s.durationMs||
       !read(s.instance+0x10,&bindings,8)||!bindings||!read(s.instance+0x18,&bindingFlags,4)||(bindingFlags&0xffffff)!=bindingCount||
       !read(bindings+playerBinding,&actor,8)||actor!=states.player.player||worldIdentityEntity(read,game,1)!=actor)return false;
    std::uintptr_t arm=0;if(s.variant==5&&(!read(bindings+8,&arm,8)||!arm||arm==actor))return false;
    std::uintptr_t again=0;unsigned number=0;
    if(!read(s.instance,&again,8)||again!=declaration||!read(s.instance+8,&again,8)||again!=scene||
       !read(s.instance+0x158,&number,4)||number!=s.handle||!read(s.instance+0x27c,&number,4)||number!=3||
       !read(s.instance+0x10,&again,8)||again!=bindings||!read(s.instance+0x18,&number,4)||number!=bindingFlags||
       !read(bindings+playerBinding,&again,8)||again!=actor||worldIdentityEntity(read,game,1)!=actor||
       (s.variant==5&&(!read(bindings+8,&again,8)||again!=arm)))return false;
    for(unsigned i=0;i<2;++i){const auto at=manager+(i?0x610:0);if(!read(at,&again,8)||again!=lists[i]||!read(at+8,&number,4)||number!=flags[i])return false;}
    NativePlayerStates final{};
    if(!readNativePlayerStates(read,base,final)||!(knocked?nativePlayerKnockdownStates(final,base):nativePlayerStunStates(final,base))||final.player.game!=game||final.player.player!=actor||
       final.component!=states.component||final.mode!=states.mode||final.player.physics!=states.player.physics||final.active.size()!=states.active.size())return false;
    for(unsigned i=0;i<states.active.size();++i)if(states.active[i].id!=final.active[i].id||states.active[i].pointer!=final.active[i].pointer||states.active[i].vtable!=final.active[i].vtable)return false;
    out=std::move(s);return true;
}
