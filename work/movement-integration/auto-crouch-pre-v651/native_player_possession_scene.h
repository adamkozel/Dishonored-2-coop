#pragma once
#include "native_player_possession_item.h"
#include "native_player_power_scene.h"
#include "native_player_possession_target.h"
#include "native_possession_context.h"
//321b empty-target cast: native Corvo Running0+UsePower63 owns one
//PlayerLeftArm binding. In333ms and loop200ms observed. Reject partially
//initialized/recycled scenes.329 added optional400ms release under exact
//EnterPossessedHost26/internal0 plus generation-validated NPC target.
//330 native evidence: entry mode26 deselects/stops the equipment while
//the reciprocal400ms left-arm scene runs; only that exact target-owned
//context accepts a dormant, still-acquired item.335 verifies runtime release
//playback; shadowSourceAction explicitly opts in. Other callers default to
//entry/aim only. No remote power gameplay or damage events are invoked.
inline unsigned nativePossessionVariant(const std::string& asset,const std::string& sub,unsigned duration,bool observeRelease=false){
    if(sub!="Scene_1")return 0;
    const std::string prefix="midnight/gameplay/player/base/powers/power_possession_cast_";
    if(asset==prefix+"in.midnightscene"&&duration==333)return 26;
    if(asset==prefix+"loop.midnightscene"&&duration==200)return 27;
    if(observeRelease&&asset==prefix+"out.midnightscene"&&duration==400)return 28;
    return 0;
}
template<class Read> bool readNativePlayerPossessionScene(Read read,std::uintptr_t base,const NativePlayerStates& states,NativePlayerPowerScene& out,bool observeRelease=false,unsigned* readerStage=nullptr){
    const auto stage=[&](unsigned value){if(readerStage)*readerStage=value;};stage(1);
    NativePossessionItem item{};NativePlayerPowerScene s{};NativePossessionTarget target{};
    const bool entering=observeRelease&&nativeHumanPossessionEntryContext(states,base)&&
        readNativePossessionTarget(read,base,target)&&target.hasTarget&&target.state==0&&
        target.player.component==states.component&&target.player.mode==states.mode&&target.player.player.game==states.player.game;

    //361 observes the same333/200ms left-arm scenes on Sneaking6. Host-entry
    //370 observes release400ms/internal0 retaining crouched stance1 in mode26.
    if(states.player.logic!=1||states.player.current>1||(!states.defaultMode&&!entering)||states.active.empty()||states.active.size()>4||
       !readNativePossessionItem(read,base,item,!entering)||!item.instantiated||item.quantity!=1||
       (entering?(item.equipped||item.started):!item.started)||
       item.player.sword.game!=states.player.game||item.player.sword.actor!=states.player.player)return false;
    stage(2);unsigned ground=0,casting=0,draw=0,request=0;
    for(const auto& a:states.active){
        if((states.player.current==0&&a.id==0&&a.vtable==base+0x1c85628)||
           (states.player.current==1&&a.id==6&&a.vtable==base+0x1c84258))++ground;
        else if(a.id==63&&a.vtable==base+0x1c80ed8)++casting;
        else if(!entering&&a.id==53&&a.vtable==base+0x1c79cf8)++draw;
        else if(!entering&&a.id==65&&a.vtable==base+0x1c78a68)++request;
        else if(entering&&a.id==26&&a.vtable==base+0x1c79878)continue;
        else return false;
    }
    if(!entering&&(ground!=1||casting!=1||draw>1||request>1||(request&&!draw)))return false;
    if(draw&&(!readNativePlayerEquipmentScene(read,base,states,s.swordDraw,true)||
       !sameSwordEquipmentIdentity(item.player.sword,s.swordDraw.sword.sword)))return false;
    stage(3);const auto game=states.player.game,manager=game+0x1f5338;
    std::array<std::uintptr_t,2> lists{};std::array<unsigned,2> counts{};
    std::uintptr_t declaration=0,scene=0,bindings=0;unsigned bindingFlags=0;
    for(unsigned field=0;field<2;++field){const auto at=manager+(field?0x610:0);
        if(!read(at,&lists[field],8)||!read(at+8,&counts[field],4)||(counts[field]&0xffffff)>512||((counts[field]&0xffffff)&&!lists[field]))return false;
        for(unsigned i=0;i<(counts[field]&0xffffff);++i){
            std::uintptr_t c=0,d=0,vt=0,name=0,sc=0;unsigned state=0,duration=0;std::string asset,sub;
            if(!read(lists[field]+i*8,&c,8)||!c||!read(c+0x27c,&state,4))return false;
            if(state!=3)continue;
            if(!read(c,&d,8)||!d||!read(d,&vt,8)||vt!=base+0x1affbc8||!read(d+8,&name,8)||!worldIdentityString(read,name,asset))return false;
            if(asset.find("midnight/gameplay/player/base/powers/power_possession_cast_")!=0)continue;
            if(!read(c+8,&sc,8)||!sc||!read(sc,&vt,8)||vt!=base+0x1b00198||!read(sc+0x28,&duration,4)||!read(sc+0x30,&name,8)||!worldIdentityString(read,name,sub))return false;
            const auto variant=nativePossessionVariant(asset,sub,duration,observeRelease);if(!variant||(entering?variant!=28:variant==28))return false;
            if(s.instance){if(s.instance==c)continue;return false;}
            s.instance=c;s.variant=variant;s.asset=std::move(asset);s.subscene=std::move(sub);s.durationMs=duration;declaration=d;scene=sc;
        }
    }
    stage(4);if(!s.instance||!read(s.instance+0x158,&s.handle,4)||!s.handle||!read(s.instance+0x160,&s.timeMs,4)||s.timeMs>3600000||
       (s.variant!=27&&s.timeMs>s.durationMs)||
       !read(s.instance+0x10,&bindings,8)||!bindings||!read(s.instance+0x18,&bindingFlags,4)||(bindingFlags&0xffffff)!=1)return false;
    stage(5);std::uintptr_t actor=0;unsigned handle=0;
    if(!read(bindings+8,&actor,8)||!actor||!read(actor+0xa8,&handle,4)||worldIdentityEntity(read,game,handle)!=actor||
       !readNativePlayerArm(read,base,game,handle,s.arm)||s.arm.actor!=actor||s.arm.player!=states.player.player||s.arm.side!=1||s.arm.character!=1)return false;
    stage(6);std::uintptr_t again=0;unsigned number=0;
    if(!read(s.instance,&again,8)||again!=declaration||!read(s.instance+8,&again,8)||again!=scene||!read(s.instance+0x158,&number,4)||number!=s.handle||
       !read(s.instance+0x27c,&number,4)||number!=3||!read(s.instance+0x10,&again,8)||again!=bindings||!read(s.instance+0x18,&number,4)||number!=bindingFlags)return false;
    if(!read(bindings+8,&again,8)||again!=actor)return false;
    for(unsigned i=0;i<2;++i){
        const auto at=manager+(i?0x610:0);if(!read(at,&again,8)||again!=lists[i]||!read(at+8,&number,4)||number!=counts[i])return false;
    }
    stage(7);NativePlayerStates finalStates{};NativePossessionItem finalItem{};NativePlayerArm finalArm{};
    if(!readNativePlayerStates(read,base,finalStates)||finalStates.player.game!=game||finalStates.player.player!=states.player.player||
       finalStates.component!=states.component||finalStates.mode!=states.mode||finalStates.player.current!=states.player.current||finalStates.defaultMode!=states.defaultMode||finalStates.active.size()!=states.active.size()||
       !readNativePossessionItem(read,base,finalItem,!entering)||finalItem.quantity!=1||finalItem.started!=item.started||finalItem.equipped!=item.equipped||!sameSwordEquipmentIdentity(item.player.sword,finalItem.player.sword)||
       finalItem.entry!=item.entry||finalItem.component!=item.component||finalItem.entity!=item.entity||finalItem.handle!=item.handle||
       finalItem.equipmentDeclaration!=item.equipmentDeclaration||finalItem.parentDeclaration!=item.parentDeclaration||
       !readNativePlayerArm(read,base,game,s.arm.handle,finalArm)||!sameNativePlayerArm(s.arm,finalArm))return false;
    for(unsigned i=0;i<states.active.size();++i)if(states.active[i].id!=finalStates.active[i].id||states.active[i].pointer!=finalStates.active[i].pointer||states.active[i].vtable!=finalStates.active[i].vtable)return false;
    if(draw){NativePlayerEquipmentScene finalDraw{};
        if(!readNativePlayerEquipmentScene(read,base,finalStates,finalDraw,true)||
           finalDraw.instance!=s.swordDraw.instance||finalDraw.handle!=s.swordDraw.handle||finalDraw.statePointer!=s.swordDraw.statePointer||
           finalDraw.variant!=s.swordDraw.variant||finalDraw.durationMs!=s.swordDraw.durationMs||
           !sameSwordEquipmentIdentity(finalDraw.sword.sword,s.swordDraw.sword.sword))return false;
    }
    stage(8);if(entering){NativePossessionTarget last{};
        if(!readNativePossessionTarget(read,base,last)||!last.hasTarget||last.state!=0||last.player.component!=states.component||last.player.mode!=states.mode||
           last.internal!=target.internal||last.target!=target.target||last.entity!=target.entity||last.handle!=target.handle||last.generation!=target.generation)return false;
    }
    stage(9);s.power=item;out=std::move(s);return true;
}
