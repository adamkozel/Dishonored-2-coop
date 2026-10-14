#pragma once
#include "native_player_power_item.h"
#include "native_player_darkvision_item.h"
#include "native_player_windblast_item.h"
#include "native_player_bendtime_item.h"
#include "native_player_mesmerize_item.h"
#include "native_player_states.h"
struct NativePlayerPowerScene {
    NativePlayerPowerItem power{};NativePlayerArm arm{};
    std::uintptr_t instance=0;unsigned handle=0,variant=0,timeMs=0,durationMs=0;
    std::string asset,subscene;
};
// Exact native phase names and durations from assets and actual Blink/Far
// Reach casts. PullLoop is an asset-backed candidate until observed live.
inline unsigned nativePowerVariant(unsigned character,const std::string& asset,const std::string& sub,unsigned duration){
    const std::string prefix="midnight/gameplay/player/base/powers/";
    if((character==1||character==2)&&sub=="Scene_1"&&duration==933&&asset==prefix+"power_darkvision_cast.midnightscene")return 10;
    if(character==1&&sub=="Cast_Ou_Shockwave"&&duration==1000&&asset==prefix+"power_windblast_cast.midnightscene")return 11;
    if(character==1&&sub=="Scene_1"){
        if(asset==prefix+"power_outofmana.midnightscene"&&duration==333)return 15;
        if(asset==prefix+"power_bendtime_cast_in_test.midnightscene"&&duration==533)return 12;
        // Hold clip exists in assets but was not observed during native v39
        // short/long presses. Reserve variant 13 until its lifecycle is proven.
        if(asset==prefix+"power_bendtime_cast_out_test.midnightscene"&&duration==433)return 14;
    }
    if(character==1&&sub=="Scene_1"&&duration==333){
        if(asset==prefix+"power_blink_cast_in.midnightscene")return 1;
        if(asset==prefix+"power_blink_cast_loop.midnightscene")return 2;
        if(asset==prefix+"power_blink_cast_out.midnightscene")return 3;
    }
    if(character==2){
        if(sub=="Scene_1"){
            if(asset==prefix+"power_mesmerize_cast_in.midnightscene"&&duration==466)return 16;
            if(asset==prefix+"power_mesmerize_cast_loop.midnightscene"&&duration==1200)return 17;
            if(asset==prefix+"power_mesmerize_cast_out.midnightscene"&&duration==666)return 18;
            if(asset==prefix+"power_grapple_cast_in.midnightscene"&&duration==200)return 4;
            if(asset==prefix+"power_grapple_cast_in_loop.midnightscene"&&duration==200)return 5;
            if(asset==prefix+"power_grapple_cast_out.midnightscene"&&duration==133)return 6;
        }
        if(asset==prefix+"power_grapple_pull.midnightscene"){
            if(sub=="pullIn"&&duration==266)return 7;
            if(sub=="pullLoop"&&duration==100)return 8;
            if(sub=="pullOut"&&duration==366)return 9;
        }
    }
    return 0;
}
template<class Read> bool readNativePlayerPowerScene(Read read,std::uintptr_t base,const NativePlayerStates& states,NativePlayerPowerScene& out,bool allowMesmerize=false){
    NativePlayerPowerScene s{};NativeDarkVisionItem dark{};NativeWindblastItem wind{};NativeBendTimeItem bend{};NativeMesmerizeItem mesmer{};
    if(states.player.logic!=1||!states.defaultMode||states.active.empty()||states.active.size()>2)return false;
    const bool darkVision=readNativeDarkVisionItem(read,base,dark,true);
    const bool windblast=!darkVision&&readNativeWindblastItem(read,base,wind,true);
    const bool bendTime=!darkVision&&!windblast&&readNativeBendTimeItem(read,base,bend,true);
    const bool mesmerize=allowMesmerize&&!darkVision&&!windblast&&!bendTime&&readNativeMesmerizeItem(read,base,mesmer,true);
    // v131 observes all three exact arm-bound phases in native crouch.
    // v151/v161/v163 observe armed Jump/Fall with entry, loop and release;
    // v175/v177 confirm the same reciprocal left arm with the sword holstered.
    // UsePower can be absent during the native release scene.
    if(mesmerize&&states.player.current>1)return false;
    if(mesmerize)s.power=mesmer;
    else if(bendTime)s.power=bend;
    else if(darkVision)s.power=dark;
    else if(windblast)s.power=wind;
    else if(!readNativePlayerPowerItem(read,base,s.power))return false;
    if(s.power.quantity!=1||!s.power.equipped||!s.power.started||
       s.power.player.sword.game!=states.player.game||s.power.player.sword.actor!=states.player.player)return false;
    if(bendTime&&states.player.current>1)return false; // Native ground/crouch and standing Jump/Fall proven through v43.
    bool blinking=false,grappling=false;
    for(const auto& a:states.active){
        if(a.id==0&&a.vtable==base+0x1c85628){if(mesmerize&&states.player.current!=0)return false;continue;}
        if((darkVision||windblast||bendTime||mesmerize)&&a.id==6&&a.vtable==base+0x1c84258){if(mesmerize&&states.player.current!=1)return false;continue;}
        // Exact owned scenes persist through Jump/Fall. Bend Time v43 also
        // observes Jump/Fall plus UsePower, with unchanged arm/player bindings.
        if((darkVision||windblast||bendTime||mesmerize)&&states.player.current==0&&
           ((a.id==2&&a.vtable==base+0x1c85028)||(a.id==3&&a.vtable==base+0x1c845c8)))continue;
        if(a.id==63&&a.vtable==base+0x1c80ed8)continue;
        if(!darkVision&&!windblast&&!bendTime&&!mesmerize&&s.power.player.character==1&&a.id==12&&a.vtable==base+0x1c815d8){blinking=true;continue;}
        if(!darkVision&&!windblast&&!bendTime&&!mesmerize&&s.power.player.character==2&&a.id==27&&a.vtable==base+0x1c7a328){grappling=true;continue;}
        return false;
    }
    if(states.player.current!=0&&!((darkVision||windblast||bendTime||mesmerize)&&states.player.current==1)&&!(blinking&&states.player.current==4)&&!(grappling&&states.player.current==8))return false;
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
            if(asset.find("midnight/gameplay/player/base/powers/power_")!=0)continue;
            if(!read(c+8,&sc,8)||!sc||!read(sc,&vt,8)||vt!=base+0x1b00198||!read(sc+0x28,&duration,4)||!read(sc+0x30,&name,8)||!worldIdentityString(read,name,sub))return false;
            const unsigned variant=nativePowerVariant(s.power.player.character,asset,sub,duration);if(!variant||(variant==10)!=darkVision||(variant==11)!=windblast||(variant>=12&&variant<=15)!=bendTime||(variant>=16)!=mesmerize)continue;
            // Exact v44/v45 failed casts confirm Corvo standing/crouched,
            // drawn/holstered, inactive selected Bend Time and sole ground state.
            if(variant==15&&(states.player.current>1||bend.powerActive||
                states.active.size()!=1||states.active[0].id!=(states.player.current==1?6u:0u)))continue;
            if(s.instance){if(s.instance==c)continue;return false;} // No guessing across overlapping native scenes.
            s.instance=c;s.variant=variant;s.asset=std::move(asset);s.subscene=std::move(sub);s.durationMs=duration;declaration=d;scene=sc;
        }
    }
    const unsigned expectedBindings=bendTime?(s.variant==12?2:1):windblast?2:darkVision||mesmerize||s.power.player.character==1?1:s.variant==5?2:3;
    if(!s.instance||!read(s.instance+0x158,&s.handle,4)||!s.handle||!read(s.instance+0x160,&s.timeMs,4)||s.timeMs>3600000||
       !read(s.instance+0x10,&bindings,8)||!bindings||!read(s.instance+0x18,&bindingFlags,4)||(bindingFlags&0xffffff)!=expectedBindings)return false;
    std::array<std::uintptr_t,3> actors{};unsigned armCount=0,powerCount=0,playerCount=0;
    for(unsigned i=0;i<expectedBindings;++i){unsigned handle=0;
        if(!read(bindings+i*0x20+8,&actors[i],8)||!actors[i]||!read(actors[i]+0xa8,&handle,4)||worldIdentityEntity(read,game,handle)!=actors[i])return false;
        if(actors[i]==s.power.entity){if(++powerCount>1||handle!=s.power.handle)return false;}
        else if(actors[i]==states.player.player){if(++playerCount>1||handle!=1)return false;}
        else if(++armCount>1||!readNativePlayerArm(read,base,game,handle,s.arm)||s.arm.actor!=actors[i]||s.arm.player!=states.player.player||s.arm.side!=1||s.arm.character!=s.power.player.character)return false;
    }
    if(armCount!=1||powerCount!=(!darkVision&&!windblast&&!mesmerize&&s.power.player.character==2?1u:0u)||playerCount!=(windblast||(bendTime&&s.variant==12)||expectedBindings==3?1u:0u))return false;
    std::uintptr_t again=0;unsigned number=0;
    if(!read(s.instance,&again,8)||again!=declaration||!read(s.instance+8,&again,8)||again!=scene||!read(s.instance+0x158,&number,4)||number!=s.handle||
       !read(s.instance+0x27c,&number,4)||number!=3||!read(s.instance+0x10,&again,8)||again!=bindings||!read(s.instance+0x18,&number,4)||number!=bindingFlags)return false;
    for(unsigned i=0;i<expectedBindings;++i)if(!read(bindings+i*0x20+8,&again,8)||again!=actors[i])return false;
    for(unsigned i=0;i<2;++i){const auto at=manager+(i?0x610:0);if(!read(at,&again,8)||again!=lists[i]||!read(at+8,&number,4)||number!=counts[i])return false;}
    NativePlayerStates finalStates{};NativePlayerPowerItem finalPower{};NativePlayerArm finalArm{};
    if(mesmerize){NativeMesmerizeItem end{};if(!readNativeMesmerizeItem(read,base,end,true))return false;finalPower=end;}
    else if(bendTime){NativeBendTimeItem end{};if(!readNativeBendTimeItem(read,base,end,true)||(s.variant==15&&end.powerActive))return false;finalPower=end;}
    else if(darkVision){NativeDarkVisionItem end{};
        if(!readNativeDarkVisionItem(read,base,end,true)||end.manager!=dark.manager||end.managerFlags!=dark.managerFlags)return false;finalPower=end;
    }else if(windblast){NativeWindblastItem end{};if(!readNativeWindblastItem(read,base,end,true))return false;finalPower=end;}
    else if(!readNativePlayerPowerItem(read,base,finalPower))return false;
    if(!readNativePlayerStates(read,base,finalStates)||finalStates.player.game!=game||finalStates.player.player!=states.player.player||finalStates.component!=states.component||
       finalStates.mode!=states.mode||finalStates.player.current!=states.player.current||!finalStates.defaultMode||finalStates.active.size()!=states.active.size()||
       finalPower.entry!=s.power.entry||finalPower.component!=s.power.component||finalPower.entity!=s.power.entity||finalPower.handle!=s.power.handle||
       finalPower.quantity!=1||!finalPower.equipped||!finalPower.started||!readNativePlayerArm(read,base,game,s.arm.handle,finalArm)||!sameNativePlayerArm(s.arm,finalArm))return false;
    for(unsigned i=0;i<states.active.size();++i)if(states.active[i].id!=finalStates.active[i].id||states.active[i].pointer!=finalStates.active[i].pointer||states.active[i].vtable!=finalStates.active[i].vtable)return false;
    out=std::move(s);return true;
}
