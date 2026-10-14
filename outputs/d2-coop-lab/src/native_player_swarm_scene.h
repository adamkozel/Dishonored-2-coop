#pragma once
#include "native_player_swarm_item.h"
#include "native_player_power_scene.h"
// v266 standing armed, v271 crouched armed, v273/v275 holstered standing/crouch.
// Exact Corvo input; stance must match Running0 or Sneaking6. Body and left arm are
// reciprocal bindings; UsePower63 retires during the 1600ms release scene.
// No rat spawn, damage or animation events are executed by this reader.
inline unsigned nativeSwarmVariant(const std::string& asset,const std::string& sub,unsigned duration){
    if(sub!="Scene_1")return 0;
    const std::string prefix="midnight/gameplay/player/base/powers/power_swarm_cast_";
    if(asset==prefix+"in.midnightscene"&&duration==400)return 23;
    if(asset==prefix+"loop.midnightscene"&&duration==333)return 24;
    if(asset==prefix+"out.midnightscene"&&duration==1600)return 25;
    return 0;
}
template<class Read> bool readNativePlayerSwarmScene(Read read,std::uintptr_t base,const NativePlayerStates& states,NativePlayerPowerScene& out){
    NativeSwarmItem item{};NativePlayerPowerScene s{};
    if(states.player.logic!=1||states.player.current>1||!states.defaultMode||states.active.empty()||states.active.size()>2||
       !readNativeSwarmItem(read,base,item,true)||!item.instantiated||!item.started||
       item.player.sword.game!=states.player.game||item.player.sword.actor!=states.player.player)return false;
    unsigned ground=0,casting=0;
    for(const auto& a:states.active){
        if((states.player.current==0&&a.id==0&&a.vtable==base+0x1c85628)||(states.player.current==1&&a.id==6&&a.vtable==base+0x1c84258))++ground;
        else if(a.id==63&&a.vtable==base+0x1c80ed8)++casting;
        else return false;
    }
    if(ground!=1||casting>1)return false;
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
            if(asset.find("midnight/gameplay/player/base/powers/power_swarm_cast_")!=0)continue;
            if(!read(c+8,&sc,8)||!sc||!read(sc,&vt,8)||vt!=base+0x1b00198||!read(sc+0x28,&duration,4)||!read(sc+0x30,&name,8)||!worldIdentityString(read,name,sub))return false;
            const auto variant=nativeSwarmVariant(asset,sub,duration);if(!variant)return false;
            if(s.instance){if(s.instance==c)continue;return false;}
            s.instance=c;s.variant=variant;s.asset=std::move(asset);s.subscene=std::move(sub);s.durationMs=duration;declaration=d;scene=sc;
        }
    }
    // v266 source body returns to Running0 while the owned release scene
    // continues (Sneaking6 when crouched). Entry/aim still require exact UsePower63, and release cannot
    // survive its bounded native clock or any equipment/scene retirement.
    if(!s.instance||(!casting&&s.variant!=25)||!read(s.instance+0x158,&s.handle,4)||!s.handle||!read(s.instance+0x160,&s.timeMs,4)||s.timeMs>3600000||
       (s.variant!=24&&s.timeMs>s.durationMs)||
       !read(s.instance+0x10,&bindings,8)||!bindings||!read(s.instance+0x18,&bindingFlags,4)||(bindingFlags&0xffffff)!=2)return false;
    std::array<std::uintptr_t,2> actors{};unsigned armCount=0,bodyCount=0;
    for(unsigned i=0;i<2;++i){unsigned handle=0;
        if(!read(bindings+i*0x20+8,&actors[i],8)||!actors[i]||!read(actors[i]+0xa8,&handle,4)||worldIdentityEntity(read,game,handle)!=actors[i])return false;
        if(actors[i]==states.player.player){if(++bodyCount>1||handle!=1)return false;}
        else if(++armCount>1||!readNativePlayerArm(read,base,game,handle,s.arm)||s.arm.actor!=actors[i]||s.arm.player!=states.player.player||s.arm.side!=1||s.arm.character!=1)return false;
    }
    if(armCount!=1||bodyCount!=1)return false;
    std::uintptr_t again=0;unsigned number=0;
    if(!read(s.instance,&again,8)||again!=declaration||!read(s.instance+8,&again,8)||again!=scene||!read(s.instance+0x158,&number,4)||number!=s.handle||
       !read(s.instance+0x27c,&number,4)||number!=3||!read(s.instance+0x10,&again,8)||again!=bindings||!read(s.instance+0x18,&number,4)||number!=bindingFlags)return false;
    for(unsigned i=0;i<2;++i){
        if(!read(bindings+i*0x20+8,&again,8)||again!=actors[i])return false;
        const auto at=manager+(i?0x610:0);if(!read(at,&again,8)||again!=lists[i]||!read(at+8,&number,4)||number!=counts[i])return false;
    }
    NativePlayerStates finalStates{};NativeSwarmItem finalItem{};NativePlayerArm finalArm{};
    if(!readNativePlayerStates(read,base,finalStates)||finalStates.player.game!=game||finalStates.player.player!=states.player.player||
       finalStates.component!=states.component||finalStates.mode!=states.mode||finalStates.player.current!=states.player.current||!finalStates.defaultMode||finalStates.active.size()!=states.active.size()||
       !readNativeSwarmItem(read,base,finalItem,true)||!finalItem.started||!sameSwordEquipmentIdentity(item.player.sword,finalItem.player.sword)||
       finalItem.entry!=item.entry||finalItem.component!=item.component||finalItem.entity!=item.entity||finalItem.handle!=item.handle||
       finalItem.equipmentDeclaration!=item.equipmentDeclaration||finalItem.parentDeclaration!=item.parentDeclaration||
       !readNativePlayerArm(read,base,game,s.arm.handle,finalArm)||!sameNativePlayerArm(s.arm,finalArm))return false;
    for(unsigned i=0;i<states.active.size();++i)if(states.active[i].id!=finalStates.active[i].id||states.active[i].pointer!=finalStates.active[i].pointer||states.active[i].vtable!=finalStates.active[i].vtable)return false;
    s.power=item;out=std::move(s);return true;
}
