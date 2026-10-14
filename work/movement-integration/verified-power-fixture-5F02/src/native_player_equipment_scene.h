#pragma once
#include "native_player_states.h"
#include "native_player_sword.h"
// Confirmed under actual X-hold input on both roles/stances in
// native-equipment-timing-v2. Draw/Undraw are request states; EquipChange53
// owns the scene. Ordinary scenes bind arm+sword. Native mantle recovery
// sword_equip_fast binds only the owned right arm (low-mantle-v5-contact-test).
struct NativePlayerEquipmentScene {
    std::uintptr_t instance=0,statePointer=0;unsigned handle=0,variant=0,timeMs=0,durationMs=0,character=0;
    std::string asset,subscene;NativePlayerSword sword;
};
inline unsigned nativeEquipmentVariant(const std::string& asset){
    constexpr const char* names[]={"sword_equip","sword_unequip","sword_equip_sneak","sword_unequip_sneak","sword_equip_fast","sword_equip_sneak_fast"};
    for(unsigned i=0;i<6;++i)if(asset==std::string("midnight/gameplay/player/base/combat/")+names[i]+".midnightscene")return i+1;return 0;
}
template<class Read> bool readNativePlayerEquipmentScene(Read read,std::uintptr_t base,const NativePlayerStates& states,NativePlayerEquipmentScene& out){
    NativePlayerEquipmentScene s{};
    if(states.player.logic!=1||!states.defaultMode||states.active.size()!=2)return false;
    bool ground=false;for(const auto& a:states.active){
        if((a.id==0&&a.vtable==base+0x1c85628)||(a.id==6&&a.vtable==base+0x1c84258))ground=true;
        else if(a.id==53&&a.vtable==base+0x1c79cf8)s.statePointer=a.pointer;
        else return false;
    }
    if(!ground||!s.statePointer||!read(s.statePointer+0x24,&s.handle,4)||!s.handle||
       !readNativePlayerSword(read,base,s.sword)||s.sword.sword.actor!=states.player.player||s.sword.sword.game!=states.player.game||s.sword.sword.logic!=1)return false;
    s.character=s.sword.character;const auto game=states.player.game,manager=game+0x1f5338;
    std::array<std::uintptr_t,2> lists{};std::array<unsigned,2> flags{};
    for(unsigned field=0;field<2;++field){const auto at=manager+(field?0x610:0);
        if(!read(at,&lists[field],8)||!read(at+8,&flags[field],4)||(flags[field]&0xffffff)>512||((flags[field]&0xffffff)&&!lists[field]))return false;
        for(unsigned i=0;i<(flags[field]&0xffffff);++i){std::uintptr_t c=0;unsigned id=0;
            if(!read(lists[field]+i*8,&c,8)||!c||!read(c+0x158,&id,4))return false;
            if(id==s.handle){if(s.instance&&s.instance!=c)return false;s.instance=c;}}
    }
    std::uintptr_t declaration=0,scene=0,vt=0,name=0,subName=0,bindings=0;unsigned phase=0,bindingFlags=0;
    if(!s.instance||!read(s.instance+0x27c,&phase,4)||phase!=3||!read(s.instance,&declaration,8)||
       !read(declaration,&vt,8)||vt!=base+0x1affbc8||!read(declaration+8,&name,8)||!worldIdentityString(read,name,s.asset)||
       !(s.variant=nativeEquipmentVariant(s.asset))||!read(s.instance+8,&scene,8)||!read(scene,&vt,8)||vt!=base+0x1b00198||
       !read(scene+0x28,&s.durationMs,4)||!s.durationMs||s.durationMs>10000||!read(scene+0x30,&subName,8)||
       !worldIdentityString(read,subName,s.subscene)||s.subscene!=(s.character==1?"Scene_2":"Scene_1")||
       !read(s.instance+0x160,&s.timeMs,4)||s.timeMs>3600000||!read(s.instance+0x10,&bindings,8)||!bindings||
       !read(s.instance+0x18,&bindingFlags,4)||(bindingFlags&0xffffff)!=(s.variant>=5?1u:2u))return false;
    if((s.variant==5&&s.durationMs!=500)||(s.variant==6&&s.durationMs!=333))return false;
    NativePlayerArm arm{};unsigned armCount=0,weaponCount=0;std::array<std::uintptr_t,2> actors{};
    for(unsigned i=0;i<(bindingFlags&0xffffff);++i){unsigned handle=0;
        if(!read(bindings+i*0x20+8,&actors[i],8)||!actors[i]||!read(actors[i]+0xa8,&handle,4)||worldIdentityEntity(read,game,handle)!=actors[i])return false;
        if(actors[i]==s.sword.sword.entity){if(++weaponCount>1||handle!=s.sword.sword.weaponHandle)return false;}
        else if(++armCount>1||!readNativePlayerArm(read,base,game,handle,arm)||arm.actor!=actors[i]||arm.player!=states.player.player||arm.side!=2||arm.character!=s.character)return false;
    }
    if(armCount!=1||weaponCount!=(s.variant>=5?0u:1u))return false;
    std::uintptr_t again=0;unsigned number=0;
    if(!read(s.instance+0x158,&number,4)||number!=s.handle||!read(s.instance+0x27c,&number,4)||number!=3||
       !read(s.instance,&again,8)||again!=declaration||!read(s.instance+8,&again,8)||again!=scene||
       !read(s.instance+0x10,&again,8)||again!=bindings||!read(s.instance+0x18,&number,4)||number!=bindingFlags||
       !read(s.statePointer+0x24,&number,4)||number!=s.handle)return false;
    for(unsigned i=0;i<2;++i){const auto at=manager+(i?0x610:0);if(!read(at,&again,8)||again!=lists[i]||!read(at+8,&number,4)||number!=flags[i])return false;}
    for(unsigned i=0;i<(bindingFlags&0xffffff);++i)if(!read(bindings+i*0x20+8,&again,8)||again!=actors[i])return false;
    NativePlayerStates finalStates{};NativePlayerSword finalSword{};NativePlayerArm finalArm{};
    if(!readNativePlayerStates(read,base,finalStates)||finalStates.player.game!=game||finalStates.player.player!=states.player.player||
       finalStates.component!=states.component||finalStates.mode!=states.mode||!finalStates.defaultMode||finalStates.active.size()!=states.active.size()||
       !readNativePlayerSword(read,base,finalSword)||finalSword.character!=s.character||!sameSwordEquipmentIdentity(s.sword.sword,finalSword.sword)||
       !readNativePlayerArm(read,base,game,arm.handle,finalArm)||!sameNativePlayerArm(arm,finalArm))return false;
    for(unsigned i=0;i<states.active.size();++i)if(states.active[i].id!=finalStates.active[i].id||states.active[i].pointer!=finalStates.active[i].pointer||states.active[i].vtable!=finalStates.active[i].vtable)return false;
    out=std::move(s);return true;
}
