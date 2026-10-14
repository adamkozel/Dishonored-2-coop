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
    constexpr const char* names[]={"sword_equip","sword_unequip","sword_equip_sneak","sword_unequip_sneak","sword_equip_fast","sword_equip_sneak_fast","sword_unequip_fast"};
    for(unsigned i=0;i<7;++i)if(asset==std::string("midnight/gameplay/player/base/combat/")+names[i]+".midnightscene")return i+1;return 0;
}

// v154 Emily/v279 Corvo holster and v339/342 Corvo possession exit:
// state64 owns a separate reciprocal left-arm equipment scene while state53
// owns the sword scene. Both scene identities and arm owners must validate.
struct NativePowerEquipmentOverlay {
    std::uintptr_t instance=0,declaration=0,scene=0,bindings=0;
    unsigned handle=0,bindingFlags=0;NativePlayerArm arm{};
};
template<class Read> bool readNativePowerEquipmentOverlay(Read read,std::uintptr_t base,const NativePlayerStates& states,
        std::uintptr_t statePointer,const std::array<std::uintptr_t,2>& lists,const std::array<unsigned,2>& flags,
        NativePowerEquipmentOverlay& out,bool draw=false){
    NativePowerEquipmentOverlay s{};const auto game=states.player.game;
    if(!statePointer||!read(statePointer+0x24,&s.handle,4)||!s.handle)return false;
    for(unsigned f=0;f<2;++f)for(unsigned i=0;i<(flags[f]&0xffffff);++i){
        std::uintptr_t c=0;unsigned id=0;
        if(!read(lists[f]+i*8,&c,8)||!c||!read(c+0x158,&id,4))return false;
        if(id==s.handle){if(s.instance&&s.instance!=c)return false;s.instance=c;}
    }
    std::uintptr_t vt=0,name=0,actor=0;unsigned phase=0,duration=0,time=0,armHandle=0;std::string asset,sub;
    if(!s.instance||!read(s.instance+0x27c,&phase,4)||phase!=3||!read(s.instance,&s.declaration,8)||
       !read(s.declaration,&vt,8)||vt!=base+0x1affbc8||!read(s.declaration+8,&name,8)||!worldIdentityString(read,name,asset)||
       asset!=(draw?"midnight/gameplay/player/base/powers/power_equip.midnightscene":"midnight/gameplay/player/base/powers/power_unequip.midnightscene")||
       !read(s.instance+8,&s.scene,8)||!read(s.scene,&vt,8)||vt!=base+0x1b00198||
       !read(s.scene+0x28,&duration,4)||duration!=333||!read(s.scene+0x30,&name,8)||!worldIdentityString(read,name,sub)||sub!="Scene_1"||
       !read(s.instance+0x160,&time,4)||time>333||!read(s.instance+0x10,&s.bindings,8)||!s.bindings||
       !read(s.instance+0x18,&s.bindingFlags,4)||(s.bindingFlags&0xffffff)!=1||
       !read(s.bindings+8,&actor,8)||!actor||!read(actor+0xa8,&armHandle,4)||
       !readNativePlayerArm(read,base,game,armHandle,s.arm)||s.arm.actor!=actor||s.arm.player!=states.player.player||
       s.arm.side!=1||(s.arm.character!=1&&s.arm.character!=2))return false;
    out=s;return true;
}
template<class Read> bool readNativePlayerEquipmentScene(Read read,std::uintptr_t base,const NativePlayerStates& states,NativePlayerEquipmentScene& out,bool possessionCast=false){
    NativePlayerEquipmentScene s{};
    if(states.player.logic!=1||!states.defaultMode||states.active.size()<2||states.active.size()>(possessionCast?4u:3u))return false;
    std::uintptr_t powerState=0;unsigned casting=0,drawRequest=0;
    bool ground=false;for(const auto& a:states.active){
        if((a.id==0&&a.vtable==base+0x1c85628)||(a.id==6&&a.vtable==base+0x1c84258))ground=true;
        else if(a.id==53&&a.vtable==base+0x1c79cf8)s.statePointer=a.pointer;
        else if(a.id==64&&a.vtable==base+0x1c79cf8&&!powerState)powerState=a.pointer;
        else if(possessionCast&&a.id==63&&a.vtable==base+0x1c80ed8)++casting;
        else if(a.id==65&&a.vtable==base+0x1c78a68)++drawRequest;
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
    //525 observed Emily preparation: standing default mode0+53, exact
    //333ms fast holster with its owned right arm alone. Preserve actual
    //native sword contact; this is not permission to activate Shadow Walk.
    if(s.variant==7&&(s.character!=2||states.player.current!=0||states.active.size()!=2||powerState||drawRequest||possessionCast||s.durationMs!=333||s.timeMs>333))return false;
    if((s.variant==5&&s.durationMs!=500)||(s.variant==6&&s.durationMs!=333))return false;
    //366 observed independent owned sword draw before the power scene starts.
    //The request alone never authorizes a pose or changes sword contact.
    if(drawRequest&&(!(possessionCast?nativePlayerGroundPowerDrawStates(states,base):nativePlayerGroundDrawRequestStates(states,base))||powerState||s.character!=1||
       !s.sword.sword.equipped||!s.sword.sword.started||s.timeMs>s.durationMs||
       !((states.player.current==0&&s.variant==1&&s.durationMs==1066)||
         (states.player.current==1&&s.variant==3&&s.durationMs==1133))))return false;
    //361 native holstered Possession starts the independent right-arm draw
    // while its left-arm cast already runs. This opt-in is used only by the
    // exact Possession scene reader, which validates that second scene too.
    if(possessionCast&&(casting!=1||powerState||!nativePlayerGroundPowerDrawStates(states,base)||s.character!=1||
       !s.sword.sword.equipped||!s.sword.sword.started||s.timeMs>s.durationMs||
       !((states.player.current==0&&s.variant==1&&s.durationMs==1066)||
         (states.player.current==1&&s.variant==3&&s.durationMs==1133))))return false;
    NativePlayerArm arm{};unsigned armCount=0,weaponCount=0;std::array<std::uintptr_t,2> actors{};
    for(unsigned i=0;i<(bindingFlags&0xffffff);++i){unsigned handle=0;
        if(!read(bindings+i*0x20+8,&actors[i],8)||!actors[i]||!read(actors[i]+0xa8,&handle,4)||worldIdentityEntity(read,game,handle)!=actors[i])return false;
        if(actors[i]==s.sword.sword.entity){if(++weaponCount>1||handle!=s.sword.sword.weaponHandle)return false;}
        else if(++armCount>1||!readNativePlayerArm(read,base,game,handle,arm)||arm.actor!=actors[i]||arm.player!=states.player.player||arm.side!=2||arm.character!=s.character)return false;
    }
    if(armCount!=1||weaponCount!=(s.variant>=5?0u:1u))return false;
    //339/342 standing and370 crouched Corvo exit: independent sword
    // Scene_2/1066 or1133ms plus owned left-power Scene_1/333ms.
    const bool powerDraw=s.character==1&&((s.variant==1&&states.player.current==0&&s.durationMs==1066)||
        (s.variant==3&&states.player.current==1&&s.durationMs==1133));
    NativePowerEquipmentOverlay powerOverlay{};
    if(powerState&&((!powerDraw&&s.variant!=2&&s.variant!=4)||s.handle==0||
       !readNativePowerEquipmentOverlay(read,base,states,powerState,lists,flags,powerOverlay,powerDraw)||
       powerOverlay.arm.character!=s.character||powerOverlay.handle==s.handle))return false;
    std::uintptr_t again=0;unsigned number=0;
    if(!read(s.instance+0x158,&number,4)||number!=s.handle||!read(s.instance+0x27c,&number,4)||number!=3||
       !read(s.instance,&again,8)||again!=declaration||!read(s.instance+8,&again,8)||again!=scene||
       !read(s.instance+0x10,&again,8)||again!=bindings||!read(s.instance+0x18,&number,4)||number!=bindingFlags||
       !read(s.statePointer+0x24,&number,4)||number!=s.handle)return false;
    for(unsigned i=0;i<2;++i){const auto at=manager+(i?0x610:0);if(!read(at,&again,8)||again!=lists[i]||!read(at+8,&number,4)||number!=flags[i])return false;}
    for(unsigned i=0;i<(bindingFlags&0xffffff);++i)if(!read(bindings+i*0x20+8,&again,8)||again!=actors[i])return false;
    NativePlayerStates finalStates{};NativePlayerSword finalSword{};NativePlayerArm finalArm{};
    if(!readNativePlayerStates(read,base,finalStates)||finalStates.player.game!=game||finalStates.player.player!=states.player.player||
       finalStates.component!=states.component||finalStates.mode!=states.mode||finalStates.player.current!=states.player.current||!finalStates.defaultMode||finalStates.active.size()!=states.active.size()||
       !readNativePlayerSword(read,base,finalSword)||finalSword.character!=s.character||!sameSwordEquipmentIdentity(s.sword.sword,finalSword.sword)||
       !readNativePlayerArm(read,base,game,arm.handle,finalArm)||!sameNativePlayerArm(arm,finalArm))return false;
    for(unsigned i=0;i<states.active.size();++i)if(states.active[i].id!=finalStates.active[i].id||states.active[i].pointer!=finalStates.active[i].pointer||states.active[i].vtable!=finalStates.active[i].vtable)return false;
    if(powerState){NativePowerEquipmentOverlay finalPower{};
        if(!readNativePowerEquipmentOverlay(read,base,finalStates,powerState,lists,flags,finalPower,powerDraw)||
           finalPower.instance!=powerOverlay.instance||finalPower.declaration!=powerOverlay.declaration||finalPower.scene!=powerOverlay.scene||
           finalPower.handle!=powerOverlay.handle||finalPower.bindings!=powerOverlay.bindings||finalPower.bindingFlags!=powerOverlay.bindingFlags||
           !sameNativePlayerArm(finalPower.arm,powerOverlay.arm))return false;
    }
    out=std::move(s);return true;
}
