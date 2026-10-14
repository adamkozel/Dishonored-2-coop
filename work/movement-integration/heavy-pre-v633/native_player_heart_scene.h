#pragma once
#include "native_player_heart.h"
#include "native_player_states.h"
// Native563: Heart equip533/unequip333 bind the owned left arm; use1000
// binds the same arm plus exact Heart item. No gameplay events are replayed.
struct NativePlayerHeartScene {
    NativePlayerHeart heart{};NativePlayerArm arm{};
    std::uintptr_t instance=0;unsigned handle=0,variant=0,timeMs=0,durationMs=0;
    std::string asset,subscene;
};
inline unsigned nativeHeartVariant(const std::string& asset,const std::string& sub,unsigned duration){
    if(sub!="Scene_1")return 0;
    if(duration==533&&asset=="midnight/gameplay/player/base/gadgets/heart_equip.midnightscene")return 1;
    if(duration==333&&asset=="midnight/gameplay/player/base/gadgets/heart_unequip.midnightscene")return 2;
    if(duration==1000&&asset=="midnight/gameplay/player/base/gadgets/heart_use.midnightscene")return 3;
    return 0;
}
template<class Read> bool readNativePlayerHeartScene(Read read,std::uintptr_t base,const NativePlayerStates& states,NativePlayerHeartScene& out){
    NativePlayerHeartScene s{};
    unsigned motion=0;
    if(!nativePlayerMotion(states,base,motion)||motion>1||states.player.current!=motion)return false;
    for(const auto& a:states.active)if(a.id!=0&&a.id!=6&&a.id!=53&&a.id!=64&&a.id!=67&&a.id!=81)return false;
    const auto game=states.player.game,manager=game+0x1f5338;
    std::array<std::uintptr_t,2> lists{};std::array<unsigned,2> counts{};
    std::uintptr_t declaration=0,scene=0,bindings=0;unsigned bindingFlags=0;
    for(unsigned field=0;field<2;++field){const auto at=manager+(field?0x610:0);
        if(!read(at,&lists[field],8)||!read(at+8,&counts[field],4)||(counts[field]&0xffffff)>512||((counts[field]&0xffffff)&&!lists[field]))return false;
        for(unsigned i=0;i<(counts[field]&0xffffff);++i){
            std::uintptr_t c=0,d=0,vt=0,name=0,sc=0;unsigned phase=0,duration=0;std::string asset,sub;
            if(!read(lists[field]+i*8,&c,8)||!c||!read(c+0x27c,&phase,4))return false;
            if(phase!=3)continue;
            if(!read(c,&d,8)||!d||!read(d,&vt,8)||vt!=base+0x1affbc8||!read(d+8,&name,8)||!worldIdentityString(read,name,asset))return false;
            if(asset.find("midnight/gameplay/player/base/gadgets/heart_")!=0)continue;
            if(!read(c+8,&sc,8)||!sc||!read(sc,&vt,8)||vt!=base+0x1b00198||!read(sc+0x28,&duration,4)||
               !read(sc+0x30,&name,8)||!worldIdentityString(read,name,sub))return false;
            const unsigned variant=nativeHeartVariant(asset,sub,duration);if(!variant)return false;
            if(s.instance){if(s.instance==c)continue;return false;}
            s.instance=c;s.variant=variant;s.asset=std::move(asset);s.subscene=std::move(sub);s.durationMs=duration;declaration=d;scene=sc;
        }
    }
    if(!s.variant||!readNativePlayerHeart(read,base,s.heart)||s.heart.quantity!=1||
       s.heart.player.sword.game!=states.player.game||s.heart.player.sword.actor!=states.player.player)return false;
    if(s.variant==3&&(!s.heart.equipped||!nativePlayerGroundHeartStates(states,base)))return false;
    const unsigned expectedItems=s.variant==3?1u:0u;
    const unsigned expectedPlayers=0;
    const unsigned expectedBindings=1+expectedItems+expectedPlayers;
    if(!s.instance||!read(s.instance+0x158,&s.handle,4)||!s.handle||!read(s.instance+0x160,&s.timeMs,4)||s.timeMs>s.durationMs||
       !read(s.instance+0x10,&bindings,8)||!bindings||!read(s.instance+0x18,&bindingFlags,4)||(bindingFlags&0xffffff)!=expectedBindings)return false;
    std::array<std::uintptr_t,3> actors{};unsigned armCount=0,itemCount=0,playerCount=0;
    for(unsigned i=0;i<expectedBindings;++i){unsigned handle=0;
        if(!read(bindings+i*0x20+8,&actors[i],8)||!actors[i]||!read(actors[i]+0xa8,&handle,4)||worldIdentityEntity(read,game,handle)!=actors[i])return false;
        if(actors[i]==s.heart.entity){if(++itemCount>1||handle!=s.heart.handle)return false;}
        else if(actors[i]==states.player.player){if(++playerCount>1||handle!=1)return false;}
        else if(++armCount>1||!readNativePlayerArm(read,base,game,handle,s.arm)||s.arm.actor!=actors[i]||s.arm.player!=states.player.player||s.arm.side!=1||s.arm.character!=s.heart.player.character)return false;
    }
    if(armCount!=1||itemCount!=expectedItems||playerCount!=expectedPlayers)return false;
    std::uintptr_t again=0;unsigned number=0;
    if(!read(s.instance,&again,8)||again!=declaration||!read(s.instance+8,&again,8)||again!=scene||!read(s.instance+0x158,&number,4)||number!=s.handle||
       !read(s.instance+0x27c,&number,4)||number!=3||!read(s.instance+0x10,&again,8)||again!=bindings||!read(s.instance+0x18,&number,4)||number!=bindingFlags)return false;
    for(unsigned i=0;i<expectedBindings;++i)if(!read(bindings+i*0x20+8,&again,8)||again!=actors[i])return false;
    for(unsigned i=0;i<2;++i){const auto at=manager+(i?0x610:0);if(!read(at,&again,8)||again!=lists[i]||!read(at+8,&number,4)||number!=counts[i])return false;}
    NativePlayerStates finalStates{};NativePlayerHeart finalItem{};NativePlayerArm finalArm{};
    if(!readNativePlayerStates(read,base,finalStates)||finalStates.player.game!=game||finalStates.player.player!=states.player.player||
       finalStates.player.current!=states.player.current||finalStates.component!=states.component||finalStates.mode!=states.mode||
       !finalStates.defaultMode||finalStates.active.size()!=states.active.size()||!readNativePlayerHeart(read,base,finalItem)||
       finalItem.entry!=s.heart.entry||finalItem.component!=s.heart.component||finalItem.entity!=s.heart.entity||finalItem.handle!=s.heart.handle||
       finalItem.quantity!=s.heart.quantity||finalItem.flags!=s.heart.flags||finalItem.equipped!=s.heart.equipped||
       !readNativePlayerArm(read,base,game,s.arm.handle,finalArm)||!sameNativePlayerArm(s.arm,finalArm))return false;
    for(unsigned i=0;i<states.active.size();++i)if(states.active[i].id!=finalStates.active[i].id||states.active[i].pointer!=finalStates.active[i].pointer||states.active[i].vtable!=finalStates.active[i].vtable)return false;
    out=std::move(s);return true;
}
