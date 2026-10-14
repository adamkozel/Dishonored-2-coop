#pragma once
#include "native_player_ranged_item.h"
#include "native_player_states.h"
struct NativePlayerRangedScene {
    NativePlayerRangedItem weapon{};NativePlayerArm arm{};
    std::uintptr_t instance=0;unsigned handle=0,variant=0,timeMs=0,durationMs=0;
    std::string asset,subscene;
};
// Exact scene/phase/duration identities observed under normal controller input.
// No trigger-edge inference and no arbitrary Midnight playback on the source.
inline unsigned nativeRangedVariant(unsigned kind,const std::string& asset,const std::string& sub,unsigned duration){
    const std::string prefix="midnight/gameplay/player/base/guns/";
    if(sub=="Scene_1"&&duration==500&&asset==prefix+(kind==1?"gun_ooa.midnightscene":"crossbow_ooa.midnightscene"))return kind==1?1:kind==2?2:0;
    if(kind==1){
        if(asset==prefix+"gun_fire.midnightscene"&&sub=="Fire"&&duration==666)return 3;
        if(asset==prefix+"gun_reload.midnightscene"&&sub=="DefaultReload"&&duration==1500)return 4;
        if(asset==prefix+"gun_equip.midnightscene"&&sub=="Scene_1"&&duration==533)return 5;
        if(asset==prefix+"gun_unequip.midnightscene"&&sub=="Scene_1"&&duration==333)return 6;
    }
    if(kind==2){
        if(asset==prefix+"crossbow_fire.midnightscene"&&sub=="Fire"&&duration==666)return 7;
        if(asset==prefix+"crossbow_reload.midnightscene"&&sub=="Reload"&&duration==900)return 8;
        if(asset==prefix+"crossbow_equip.midnightscene"&&sub=="Scene_1"&&duration==533)return 9;
        if(asset==prefix+"crossbow_unequip.midnightscene"&&sub=="Scene_1"&&duration==333)return 10;
    }
    return 0;
}
template<class Read> bool readNativePlayerRangedScene(Read read,std::uintptr_t base,const NativePlayerStates& states,NativePlayerRangedScene& out){
    NativePlayerRangedScene s{};
    const bool air=nativePlayerAirRangedStates(states,base);
    if(states.player.logic!=1||(!nativePlayerGroundRangedStates(states,base)&&!air))return false;
    if((!readNativePlayerRangedItem(read,base,1,s.weapon,true)&&!readNativePlayerRangedItem(read,base,2,s.weapon,true))||
       s.weapon.player.sword.game!=states.player.game||s.weapon.player.sword.actor!=states.player.player)return false;
    const auto game=states.player.game,manager=game+0x1f5338;
    std::array<std::uintptr_t,2> lists{};std::array<unsigned,2> counts{};
    std::uintptr_t declaration=0,scene=0,bindings=0;unsigned bindingFlags=0;
    for(unsigned field=0;field<2;++field){const auto at=manager+(field?0x610:0);
        if(!read(at,&lists[field],8)||!read(at+8,&counts[field],4)||(counts[field]&0xffffff)>512||((counts[field]&0xffffff)&&!lists[field]))return false;
        for(unsigned i=0;i<(counts[field]&0xffffff);++i){
            std::uintptr_t c=0,d=0,vt=0,name=0,sc=0;unsigned state=0,duration=0;std::string asset,sub;
            if(!read(lists[field]+i*8,&c,8)||!c||!read(c+0x27c,&state,4))return false;
            if(state!=3)continue;
            if(!read(c,&d,8)||!d||!read(d,&vt,8)||vt!=base+0x1affbc8||!read(d+8,&name,8)||!worldIdentityString(read,name,asset))return false;
            if(asset.find("midnight/gameplay/player/base/guns/")!=0)continue;
            if(!read(c+8,&sc,8)||!sc||!read(sc,&vt,8)||vt!=base+0x1b00198||!read(sc+0x28,&duration,4)||!read(sc+0x30,&name,8)||!worldIdentityString(read,name,sub))return false;
            const unsigned variant=nativeRangedVariant(s.weapon.kind,asset,sub,duration);if(!variant)continue;
            if(s.instance){if(s.instance==c)continue;return false;}
            s.instance=c;s.variant=variant;s.asset=std::move(asset);s.subscene=std::move(sub);s.durationMs=duration;declaration=d;scene=sc;
        }
    }
    // Only observed fire/empty/reload actions are admitted in the air. Draw/holster
    // and other equipment transitions remain grounded until independently traced.
    if(air&&s.variant!=1&&s.variant!=2&&s.variant!=3&&s.variant!=4&&s.variant!=7&&s.variant!=8)return false;
    if(!s.instance||!read(s.instance+0x158,&s.handle,4)||!s.handle||!read(s.instance+0x160,&s.timeMs,4)||s.timeMs>3600000||
       !read(s.instance+0x10,&bindings,8)||!bindings||!read(s.instance+0x18,&bindingFlags,4)||(bindingFlags&0xffffff)!=2)return false;
    std::array<std::uintptr_t,2> actors{};unsigned armCount=0,weaponCount=0;
    for(unsigned i=0;i<2;++i){unsigned handle=0;
        if(!read(bindings+i*0x20+8,&actors[i],8)||!actors[i]||!read(actors[i]+0xa8,&handle,4)||worldIdentityEntity(read,game,handle)!=actors[i])return false;
        if(actors[i]==s.weapon.entity){if(++weaponCount>1||handle!=s.weapon.handle)return false;}
        else if(++armCount>1||!readNativePlayerArm(read,base,game,handle,s.arm)||s.arm.actor!=actors[i]||s.arm.player!=states.player.player||s.arm.side!=1||s.arm.character!=s.weapon.player.character)return false;
    }
    if(armCount!=1||weaponCount!=1)return false;
    std::uintptr_t again=0;unsigned number=0;
    if(!read(s.instance,&again,8)||again!=declaration||!read(s.instance+8,&again,8)||again!=scene||!read(s.instance+0x158,&number,4)||number!=s.handle||
       !read(s.instance+0x27c,&number,4)||number!=3||!read(s.instance+0x10,&again,8)||again!=bindings||!read(s.instance+0x18,&number,4)||number!=bindingFlags)return false;
    for(unsigned i=0;i<2;++i)if(!read(bindings+i*0x20+8,&again,8)||again!=actors[i])return false;
    for(unsigned i=0;i<2;++i){const auto at=manager+(i?0x610:0);if(!read(at,&again,8)||again!=lists[i]||!read(at+8,&number,4)||number!=counts[i])return false;}
    NativePlayerStates finalStates{};NativePlayerRangedItem finalWeapon{};NativePlayerArm finalArm{};
    if(!readNativePlayerStates(read,base,finalStates)||finalStates.player.game!=game||finalStates.player.player!=states.player.player||finalStates.component!=states.component||
       finalStates.mode!=states.mode||!finalStates.defaultMode||finalStates.player.current!=states.player.current||finalStates.active.size()!=states.active.size()||
       !readNativePlayerRangedItem(read,base,s.weapon.kind,finalWeapon,true)||finalWeapon.entry!=s.weapon.entry||finalWeapon.component!=s.weapon.component||finalWeapon.entity!=s.weapon.entity||finalWeapon.handle!=s.weapon.handle||
       !readNativePlayerArm(read,base,game,s.arm.handle,finalArm)||!sameNativePlayerArm(s.arm,finalArm))return false;
    for(unsigned i=0;i<states.active.size();++i)if(states.active[i].id!=finalStates.active[i].id||states.active[i].pointer!=finalStates.active[i].pointer||states.active[i].vtable!=finalStates.active[i].vtable)return false;
    out=std::move(s);return true;
}
