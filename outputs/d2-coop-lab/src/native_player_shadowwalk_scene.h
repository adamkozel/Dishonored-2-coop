#pragma once
#include "native_player_shadowwalk_state.h"
#include "native_player_power_scene.h"
// Native516b/520 standing and547b crouched sneak entry. Low-ceiling crouch,
// wall, attack and rat-hole contexts remain unverified. The sustained
// form has no active Midnight scene: its owned native phase4 is the authority.
// Variant33's1000ms is a presentation loop period, not native form lifetime.
inline unsigned nativeShadowWalkVariant(unsigned phase,const std::string& asset,const std::string& sub,unsigned duration){
    const std::string prefix="midnight/gameplay/player/base/powers/power_shadow_walk_";
    if(phase==3&&asset==prefix+"trans_in.midnightscene"&&sub=="emily"&&duration==1000)return 32;
    if(phase==3&&asset==prefix+"trans_in_sneak.midnightscene"&&sub=="emily"&&duration==1000)return 35;
    if(phase==5&&asset==prefix+"trans_out.midnightscene"&&sub=="Emily"&&duration==1133)return 34;
    // 1214da: after a shadow-form assassination the form exits through attack_out (1350b: subscene Emily, 1166 ms,
    // ply_body_sw_timeout; confirmed runtime and static).
    if(phase==5&&asset==prefix+"attack_out.midnightscene"&&sub=="Emily"&&duration==1166)return 36;
    return 0;
}
template<class Read> bool readNativePlayerShadowWalkScene(Read read,std::uintptr_t base,const NativePlayerStates& states,NativePlayerPowerScene& out){
    NativeShadowWalkState native{};NativePlayerPowerScene s{};
    if(!readNativeShadowWalkState(read,base,native)||!native.form||native.phase<3||native.phase>5||
       native.item.player.sword.equipped||native.states.player.game!=states.player.game||
       native.states.player.player!=states.player.player||native.states.component!=states.component||native.states.mode!=states.mode||
       native.states.player.current!=states.player.current)return false;
    s.power=native.item;
    if(native.phase==4){s.variant=33;s.handle=native.item.handle;s.durationMs=1000;out=std::move(s);return true;}
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
            const std::string expected=std::string("midnight/gameplay/player/base/powers/power_shadow_walk_trans_")+(native.phase==3?"in":"out")+".midnightscene";
            const bool sneak=native.phase==3&&asset=="midnight/gameplay/player/base/powers/power_shadow_walk_trans_in_sneak.midnightscene";
            const bool attackOut=native.phase==5&&asset=="midnight/gameplay/player/base/powers/power_shadow_walk_attack_out.midnightscene";
            if(asset!=expected&&!sneak&&!attackOut)continue;
            if(!read(c+8,&sc,8)||!sc||!read(sc,&vt,8)||vt!=base+0x1b00198||!read(sc+0x28,&duration,4)||!read(sc+0x30,&name,8)||!worldIdentityString(read,name,sub))return false;
            const auto variant=nativeShadowWalkVariant(native.phase,asset,sub,duration);if(!variant)return false;
            if(s.instance){if(s.instance==c)continue;return false;}
            s.instance=c;s.variant=variant;s.asset=std::move(asset);s.subscene=std::move(sub);s.durationMs=duration;declaration=d;scene=sc;
        }
    }
    if(!s.instance||!read(s.instance+0x158,&s.handle,4)||!s.handle||!read(s.instance+0x160,&s.timeMs,4)||s.timeMs>s.durationMs||
       !read(s.instance+0x10,&bindings,8)||!bindings||!read(s.instance+0x18,&bindingFlags,4)||(bindingFlags&0xffffff)!=7)return false;
    std::array<std::uintptr_t,7> actors{},groups{};NativePlayerArm right{};
    const auto eq=[&](std::uintptr_t at,std::uintptr_t expected){std::uintptr_t value=0;return read(at,&value,8)&&value==expected;};
    constexpr const char* helperNames[]={"fake_ply_emily_leftarm_01.def","fake_ply_emily_rightarm_01.def","fake_ply_corvo_leftarm_01.def","fake_ply_corvo_rightarm_01.def"};
    for(unsigned i=0;i<7;++i){unsigned handle=0;
        if(!read(bindings+i*0x20,&groups[i],8)||!groups[i]||!eq(groups[i],base+0x1c6ca98)||!eq(groups[i]+0xc8,s.instance)||
           !read(bindings+i*0x20+8,&actors[i],8)||!actors[i]||!eq(groups[i]+0xd0,actors[i])||
           !read(actors[i]+0xa8,&handle,4)||worldIdentityEntity(read,game,handle)!=actors[i])return false;
        for(unsigned j=0;j<i;++j)if(actors[j]==actors[i]||groups[j]==groups[i])return false;
        if(i==0){if(handle!=1||actors[i]!=states.player.player)return false;}
        else if(i<3){auto& arm=i==1?s.arm:right;
            if(!readNativePlayerArm(read,base,game,handle,arm)||arm.actor!=actors[i]||arm.player!=states.player.player||arm.character!=2||arm.side!=(i==1?1u:2u))return false;
        }else{std::uintptr_t def=0,name=0;std::string text;
            if(!read(actors[i]+0x2e8,&def,8)||!def||!eq(def,base+0x1ab58f8)||!read(def+8,&name,8)||!worldIdentityString(read,name,text)||
               text!=std::string("models/effects/gameplay/powers/shadow_walk/")+helperNames[i-3])return false;
        }
    }
    unsigned value=0;
    if(!eq(s.instance,declaration)||!eq(s.instance+8,scene)||!eq(s.instance+0x10,bindings)||
       !read(s.instance+0x158,&value,4)||value!=s.handle||!read(s.instance+0x27c,&value,4)||value!=3||
       !read(s.instance+0x18,&value,4)||value!=bindingFlags)return false;
    for(unsigned i=0;i<7;++i)if(!eq(bindings+i*0x20,groups[i])||!eq(bindings+i*0x20+8,actors[i])||!eq(groups[i]+0xc8,s.instance)||!eq(groups[i]+0xd0,actors[i]))return false;
    for(unsigned i=0;i<2;++i){const auto at=manager+(i?0x610:0);if(!eq(at,lists[i])||!read(at+8,&value,4)||value!=counts[i])return false;}
    NativeShadowWalkState final{};
    if(!readNativeShadowWalkState(read,base,final)||final.phase!=native.phase||!final.form||final.item.handle!=native.item.handle||
       final.item.component!=native.item.component||final.item.entity!=native.item.entity||final.states.mode!=native.states.mode||
       final.states.player.current!=native.states.player.current)return false;
    out=std::move(s);return true;
}
