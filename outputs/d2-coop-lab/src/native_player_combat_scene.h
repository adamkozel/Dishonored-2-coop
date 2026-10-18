#pragma once
#include "native_player_action.h"
#include "native_player_arm.h"
#include "world_identity.h"
#include <array>

// Read-only source semantics. Ghidra8549c0 stores the current animation
// scene at state+24. Native 7ce7e0 builds the instance's target bindings.
// Both were correlated on Corvo and Emily in native-combat-timing-v2.
struct NativePlayerCombatScene {
    std::uintptr_t instance=0,statePointer=0,stateVtable=0;
    unsigned handle=0,action=0,variant=0,timeMs=0,durationMs=0,character=0,stateId=0;
    std::string asset,subscene;
    NativeWorldNpc target{}; // Optional native hit target; presentation never replays its events.
};
inline bool nativeDefenseImpact(unsigned variant){return variant>=19&&variant<=21;}
inline bool nativeDefenseImpactScene(unsigned variant,const std::string& sub,unsigned duration,unsigned stance){
    return stance<=1&&sub=="Scene_1"&&(((variant==19||variant==20)&&duration==533)||(variant==21&&duration==500));
}
// Native607, both roles: ordinary sword hits bind player, owned right arm,
// and one reciprocal guard. Keep unobserved target-bearing scenes closed.
inline bool nativeCombatTargetScene(unsigned variant,const std::string& sub,unsigned duration,unsigned stance=0){
    return ((variant==1||variant==2)&&(sub=="Scene_1"||sub=="Scene_2")&&duration==1000&&stance==0)||
        ((variant==5||variant==6)&&sub=="Scene_1"&&duration==933&&stance<=1)||
        // Native1021 block impact (hit_1, asset-equal hit_2 alternative) and
        // defender-side timed parry: player camera, owned right arm and the
        // one attacking NPC. Correlated with relayed 140859980 results 0/1.
        nativeDefenseImpactScene(variant,sub,duration,stance);
}
inline bool nativeCombatHitRecovery(unsigned variant,const std::string& sub,unsigned duration){
    return (variant==15&&sub=="Scene_1"&&duration==866)||(variant==16&&sub=="Scene_2"&&duration==666);
}
inline unsigned nativeCombatVariant(const std::string& asset){
    constexpr const char* names[]={"sword_slash_right","sword_slash_rightb","sword_slash_left","sword_slash_leftb",
        "sword_slash_sneak_right","sword_slash_sneak_left","sword_attack_focus_in","sword_attack_focus_out",
        "sword_attack_focus_cancel","sword_parry_in","sword_parry_idle","sword_parry_out",
        "sword_parry_in_sneak","sword_parry_out_sneak","sword_sneak_attack_hit","sword_sneak_attack_left_hit","throwhit","sword_attack_focus_sneak_out",
        "sword_parry_hit_1","sword_parry_hit_2","sword_parry_win",
        // Takes 1256a/1258b: a second (b) swing that connects plays its own short hit scene
        // (Scene_1, 733 ms, owned player and right arm only).
        "sword_attack_right_b_hit","sword_attack_left_b_hit"};
    for(unsigned i=0;i<23;++i)if(asset==std::string("midnight/gameplay/player/base/combat/")+names[i]+".midnightscene")return i+1;
    // 1214ct (#11): Blink Assault plays its kick through blink_attack_focus (subscene kick, 800 ms, the native
    // ply_body_throw_hit_kick animations; confirmed static and runtime, blink-assault-probe-v1360a): the same motion as 17.
    if(asset=="midnight/gameplay/player/base/combat/blink_attack_focus.midnightscene")return 17;
    // 1214cz (#11 stagger): a parried or blocked swing recoils (player state 40) with one of these first-person scenes,
    // binding only the camera and the right arm (native-stagger-scenes-v1365, confirmed static): 28 left, 29 right.
    for(const char* n:{"sword_parry_ll","sword_parry_rl","sword_recoil_left","sword_recoil_leftb","sword_sneak_recoil_left"})
        if(asset==std::string("midnight/gameplay/player/base/combat/")+n+".midnightscene")return 28;
    for(const char* n:{"sword_parry_lr","sword_parry_rr","sword_recoil_right","sword_recoil_rightb","sword_sneak_recoil_right"})
        if(asset==std::string("midnight/gameplay/player/base/combat/")+n+".midnightscene")return 29;
    return 0;
}
template<class Read> bool readNativePlayerCombatScene(Read read,std::uintptr_t base,const NativePlayerStates& states,NativePlayerCombatScene& out){
    NativePlayerCombatScene s{};
    if(states.player.logic!=1||!nativePlayerSwordAction(states,base,s.action)||!s.action)return false;
    for(const auto& a:states.active)if(a.id==39||a.id==44||a.id==40){s.statePointer=a.pointer;s.stateId=a.id;s.stateVtable=a.vtable;}
    if(!s.statePointer||!read(s.statePointer+0x24,&s.handle,4)||!s.handle)return false;
    const auto game=states.player.game,manager=game+0x1f5338;
    std::array<std::uintptr_t,2> lists{};std::array<unsigned,2> flags{};
    for(unsigned field=0;field<2;++field){
        const auto at=manager+(field?0x610:0);
        if(!read(at,&lists[field],8)||!read(at+8,&flags[field],4)||(flags[field]&0xffffff)>512||((flags[field]&0xffffff)&&!lists[field]))return false;
        for(unsigned i=0;i<(flags[field]&0xffffff);++i){
            std::uintptr_t candidate=0;unsigned id=0;
            if(!read(lists[field]+i*8,&candidate,8)||!candidate||!read(candidate+0x158,&id,4))return false;
            if(id==s.handle){if(s.instance&&s.instance!=candidate)return false;s.instance=candidate;}
        }
    }
    std::uintptr_t declaration=0,scene=0,vt=0,name=0,subName=0,bindings=0;unsigned phase=0,bindingFlags=0;
    if(!s.instance||!read(s.instance+0x27c,&phase,4)||phase!=3||!read(s.instance,&declaration,8)||
       !read(declaration,&vt,8)||vt!=base+0x1affbc8||!read(declaration+8,&name,8)||!worldIdentityString(read,name,s.asset)||
       !(s.variant=nativeCombatVariant(s.asset))||!read(s.instance+8,&scene,8)||!read(scene,&vt,8)||vt!=base+0x1b00198||
       !read(scene+0x28,&s.durationMs,4)||!s.durationMs||s.durationMs>10000||!read(scene+0x30,&subName,8)||
       !worldIdentityString(read,subName,s.subscene)||s.subscene.empty()||s.subscene.size()>64||
       !read(s.instance+0x160,&s.timeMs,4)||s.timeMs>3600000||!read(s.instance+0x10,&bindings,8)||!bindings||
       !read(s.instance+0x18,&bindingFlags,4)||(bindingFlags&0xffffff)<1||(bindingFlags&0xffffff)>3)return false;
    if((s.action==1)!=(s.variant<=9||(s.variant>=15&&s.variant<=18)||s.variant>=22))return false;
    if(s.variant>=22&&s.variant<=23&&(s.subscene!="Scene_1"||s.durationMs!=733||(bindingFlags&0xffffff)!=2))return false;
    // A recoil runs only under Recoil40, at its observed native durations, with the camera and right arm bound.
    const bool stagger=s.variant==28||s.variant==29;
    if(stagger!=(s.stateId==40)||(stagger&&((s.durationMs!=666&&s.durationMs!=733)||s.timeMs>s.durationMs||(bindingFlags&0xffffff)!=2)))return false;
    // Exact native611 kick, corroborated by the exported624 declaration:
    // PlayerRightArm, PlayerCamera and PlayerBody. Both latter groups bind
    // the SAME owned player. No NPC/partner is admitted by this exception.
    const bool kick=s.variant==17;
    // Blink Assault (1214ct): standing (Running0 + Slash39, physical 0 or 1), binding the owned player body, the right arm
    // and the one kicked NPC (AIGroup1) instead of the player twice.
    const bool blinkKick=kick&&s.asset=="midnight/gameplay/player/base/combat/blink_attack_focus.midnightscene";
    if(blinkKick&&(s.subscene!="kick"||s.durationMs!=800||s.timeMs>s.durationMs||(bindingFlags&0xffffff)!=3||states.player.current>1||
       !std::any_of(states.active.begin(),states.active.end(),[](const auto& a){return a.id==0||a.id==6;})))return false;
    if(kick&&!blinkKick&&(s.subscene!="kick"||s.durationMs!=800||s.timeMs>s.durationMs||
       (bindingFlags&0xffffff)!=3||states.player.current!=1||
       !std::any_of(states.active.begin(),states.active.end(),[](const auto& a){return a.id==6;})))return false;
    if((bindingFlags&0xffffff)==3&&!kick&&!nativeCombatTargetScene(s.variant,s.subscene,s.durationMs,states.player.current))return false;
    if((bindingFlags&0xffffff)==3&&!std::any_of(states.active.begin(),states.active.end(),[](const auto& a){return a.id==0||a.id==6;}))return false;
    // Native613: contact switches to a separate owned player/right-arm scene.
    // Corvo's left hit also changes physical stance to standing. Never replay
    // its source damage events; only read the scene's presentation clock.
    if((s.variant==15||s.variant==16)&&(!nativeCombatHitRecovery(s.variant,s.subscene,s.durationMs)||(bindingFlags&0xffffff)!=2||
       states.player.current>1||!std::any_of(states.active.begin(),states.active.end(),[](const auto& a){return a.id==0||a.id==6;})))return false;
    // Native629/630: crouched release binds only owned player and right arm.
    if(s.variant==18&&((bindingFlags&0xffffff)!=2||s.timeMs>s.durationMs||states.player.current!=1||
       !std::any_of(states.active.begin(),states.active.end(),[](const auto& a){return a.id==6;})))return false;
    unsigned playerCount=0,armCount=0,targetCount=0;NativePlayerArm arm{};
    std::array<std::uintptr_t,3> actors{};
    for(unsigned i=0;i<(bindingFlags&0xffffff);++i){
        unsigned handle=0;std::uintptr_t actorVt=0;
        if(!read(bindings+i*0x20+8,&actors[i],8)||!actors[i]||!read(actors[i]+0xa8,&handle,4)||
           worldIdentityEntity(read,game,handle)!=actors[i]||!read(actors[i],&actorVt,8))return false;
        if(actors[i]==states.player.player){if(++playerCount>(kick&&!blinkKick?2u:1u)||handle!=1)return false;}
        else if(actorVt==base+0x1c5e108){
            if((kick&&!blinkKick)||++targetCount>1||(bindingFlags&0xffffff)!=3||!readWorldNpcIdentity(read,base,game,handle,s.target)||
               !s.target.unique||!s.target.reciprocal||s.target.actor!=actors[i])return false;
        }
        else{NativePlayerArm candidate{};if(++armCount>1||!readNativePlayerArm(read,base,game,handle,candidate)||candidate.side!=2||candidate.actor!=actors[i]||candidate.player!=states.player.player)return false;arm=candidate;}
    }
    // Parry idle targets the right arm only. Other observed scenes bind both
    // the native player and arm, plus an optional validated native hit target.
    if(armCount!=1||(s.variant!=11&&playerCount!=(kick&&!blinkKick?2u:1u))||((!kick||blinkKick)&&(bindingFlags&0xffffff)==3&&targetCount!=1))return false;s.character=arm.character;
    // Corvo634 selected Scene_1 too: these are attack variants, not roles.
    if(s.variant==18&&!((s.subscene=="Scene_1"&&s.durationMs==833)||(s.subscene=="Scene_2"&&s.durationMs==1000)))return false;
    std::uintptr_t again=0;unsigned number=0;
    if(!read(base+0x39a5290,&again,8)||again!=game||worldIdentityEntity(read,game,1)!=states.player.player||
       !read(s.statePointer+0x24,&number,4)||number!=s.handle||!read(s.statePointer+0x10,&again,8)||again!=states.component||
       !read(s.statePointer,&again,8)||again!=s.stateVtable||
       !read(states.component+0x90,&again,8)||again!=states.mode||!read(states.mode+0x10+s.stateId*8,&again,8)||again!=s.statePointer||
       !read(s.instance+0x158,&number,4)||number!=s.handle||!read(s.instance+0x27c,&number,4)||number!=3||
       !read(s.instance,&again,8)||again!=declaration||!read(s.instance+8,&again,8)||again!=scene||
       !read(s.instance+0x10,&again,8)||again!=bindings||!read(s.instance+0x18,&number,4)||number!=bindingFlags)return false;
    for(unsigned i=0;i<2;++i){const auto at=manager+(i?0x610:0);if(!read(at,&again,8)||again!=lists[i]||!read(at+8,&number,4)||number!=flags[i])return false;}
    for(unsigned i=0;i<(bindingFlags&0xffffff);++i)if(!read(bindings+i*0x20+8,&again,8)||again!=actors[i])return false;
    NativePlayerArm finalArm{};if(!readNativePlayerArm(read,base,game,arm.handle,finalArm)||!sameNativePlayerArm(arm,finalArm))return false;
    if(targetCount){NativeWorldNpc finalTarget{};
        if(!readWorldNpcIdentity(read,base,game,s.target.handle,finalTarget)||!finalTarget.unique||!finalTarget.reciprocal||
           finalTarget.actor!=s.target.actor||finalTarget.spawner!=s.target.spawner||finalTarget.component!=s.target.component||
           finalTarget.spawnerHandle!=s.target.spawnerHandle||finalTarget.definition!=s.target.definition||finalTarget.spawnerName!=s.target.spawnerName)return false;
    }
    NativePlayerStates finalStates{};
    if(!readNativePlayerStates(read,base,finalStates)||finalStates.player.logic!=1||finalStates.player.game!=game||
       finalStates.player.player!=states.player.player||finalStates.player.current!=states.player.current||finalStates.component!=states.component||finalStates.mode!=states.mode)return false;
    unsigned finalAction=0;if(!nativePlayerSwordAction(finalStates,base,finalAction)||finalAction!=s.action)return false;
    bool sameState=false;for(const auto& a:finalStates.active)if(a.id==s.stateId&&a.pointer==s.statePointer)sameState=true;
    if(!sameState)return false;
    out=std::move(s);return true;
}
