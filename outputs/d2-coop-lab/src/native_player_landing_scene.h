#pragma once
#include "native_player_states.h"
#include "world_identity.h"
#include "native_player_arm.h"
#include <array>
#include <string>
// Confirmed native CatFall4 on both characters after a non-damaging >6m fall.
// The last scene stored at state+24 is the BODY scene, not either FP arm.
// 1214cn: a damaging ~11 m fall plays the strong body scene in the same CatFall4 (confirmed runtime,
// mission-coverage-v1328e midnight watch: cat_fall_strong, 1066 ms, phase 3, Corvo Scene_2, Emily Scene_1).
// 1214co: unlike the light body scene (its arms run separate scenes), the strong scene binds three actors: the
// player first, then both first-person arms (1328e: handles 1, then two arm entities; 1328f refused it at stage 6).
constexpr unsigned kNativeLightLandingMs=1000,kNativeStrongLandingMs=1066;
struct NativePlayerLandingScene {
    std::uintptr_t instance=0,statePointer=0;unsigned handle=0,timeMs=0,durationMs=0,character=0,endStance=0;
    std::string asset,subscene;bool strong=false;NativePlayerArm rightArm{},leftArm{};
};
template<class Read> bool readNativePlayerLandingScene(Read read,std::uintptr_t base,const NativePlayerStates& states,NativePlayerLandingScene& out){
    NativePlayerLandingScene s{};
    if(states.player.logic!=1||!states.defaultMode||states.active.size()!=1||states.active[0].id!=4||states.active[0].vtable!=base+0x1c82158)return false;
    s.statePointer=states.active[0].pointer;
    if(!read(s.statePointer+0x24,&s.handle,4)||!s.handle)return false;
    const auto game=states.player.game,manager=game+0x1f5338;
    std::array<std::uintptr_t,2> lists{};std::array<unsigned,2> flags{};
    for(unsigned field=0;field<2;++field){const auto at=manager+(field?0x610:0);
        if(!read(at,&lists[field],8)||!read(at+8,&flags[field],4)||(flags[field]&0xffffff)>512||((flags[field]&0xffffff)&&!lists[field]))return false;
        for(unsigned i=0;i<(flags[field]&0xffffff);++i){std::uintptr_t c=0;unsigned id=0;
            if(!read(lists[field]+i*8,&c,8)||!c||!read(c+0x158,&id,4))return false;
            if(id==s.handle){if(s.instance&&s.instance!=c)return false;s.instance=c;}}
    }
    std::uintptr_t declaration=0,scene=0,vt=0,name=0,subName=0,bindings=0,bound=0,body=0,bodyName=0;unsigned phase=0,bindingFlags=0;std::string bodyAsset;
    if(!s.instance||!read(s.instance+0x27c,&phase,4)||phase!=3||!read(s.instance,&declaration,8)||
        !read(declaration,&vt,8)||vt!=base+0x1affbc8||!read(declaration+8,&name,8)||!worldIdentityString(read,name,s.asset)||
        (s.asset!="midnight/gameplay/player/base/mobility/cat_fall_light.midnightscene"&&s.asset!="midnight/gameplay/player/base/mobility/cat_fall_strong.midnightscene")||
        !read(s.instance+8,&scene,8)||!read(scene,&vt,8)||vt!=base+0x1b00198||!read(scene+0x28,&s.durationMs,4)||
        s.durationMs!=((s.strong=s.asset=="midnight/gameplay/player/base/mobility/cat_fall_strong.midnightscene")?kNativeStrongLandingMs:kNativeLightLandingMs)||
        !read(scene+0x30,&subName,8)||!worldIdentityString(read,subName,s.subscene)||
        !read(s.instance+0x160,&s.timeMs,4)||s.timeMs>s.durationMs||!read(s.instance+0x10,&bindings,8)||!bindings||
        !read(s.instance+0x18,&bindingFlags,4)||(bindingFlags&0xffffff)!=(s.strong?3u:1u)||!read(bindings+8,&bound,8)||bound!=states.player.player||
        worldIdentityEntity(read,game,1)!=bound||!read(bound+0x2e8,&body,8)||!body||!read(body,&vt,8)||vt!=base+0x1ab58f8||
        !read(body+8,&bodyName,8)||!worldIdentityString(read,bodyName,bodyAsset))return false;
    if(bodyAsset=="models/characters/player/corvo/body.def")s.character=1;
    else if(bodyAsset=="models/characters/player/emily/body.def")s.character=2;else return false;
    if(s.subscene!=(s.character==1?"Scene_2":"Scene_1"))return false;
    std::array<std::uintptr_t,2> armActors{};std::array<unsigned,2> armHandles{};
    if(s.strong){
        for(unsigned i=0;i<2;++i)if(!read(bindings+(i+1)*0x20+8,&armActors[i],8)||!armActors[i]||!read(armActors[i]+0xa8,&armHandles[i],4)||worldIdentityEntity(read,game,armHandles[i])!=armActors[i])return false;
        if(!readNativePlayerArm(read,base,game,armHandles[0],s.rightArm)||s.rightArm.actor!=armActors[0]||s.rightArm.side!=2||s.rightArm.player!=bound||s.rightArm.character!=s.character||
           !readNativePlayerArm(read,base,game,armHandles[1],s.leftArm)||s.leftArm.actor!=armActors[1]||s.leftArm.side!=1||s.leftArm.player!=bound||s.leftArm.character!=s.character)return false;
    }
    // Native CatFall exit 14085b1e0 uses this owned input processor's crouch
    // latch, not the capsule's temporary landing stance. Read-only.
    std::uintptr_t input=0;unsigned crouchIndex=0;std::uint64_t inputBits=0,lastBits=0;
    if(!read(states.component+0x2f8,&input,8)||!input||!read(input+0x6c,&crouchIndex,4)||crouchIndex!=16||!read(input+0xa0,&inputBits,8))return false;
    s.endStance=unsigned((inputBits>>crouchIndex)&1);
    std::uintptr_t again=0;unsigned number=0;std::string text;
    if(!read(s.instance+0x158,&number,4)||number!=s.handle||!read(s.instance+0x27c,&number,4)||number!=3||
        !read(s.instance,&again,8)||again!=declaration||!read(declaration,&again,8)||again!=base+0x1affbc8||
        !read(declaration+8,&again,8)||again!=name||!worldIdentityString(read,name,text)||text!=s.asset||
        !read(s.instance+8,&again,8)||again!=scene||!read(scene,&again,8)||again!=base+0x1b00198||!read(scene+0x28,&number,4)||number!=s.durationMs||
        !read(scene+0x30,&again,8)||again!=subName||!worldIdentityString(read,subName,text)||text!=s.subscene||
        !read(s.instance+0x10,&again,8)||again!=bindings||!read(s.instance+0x18,&number,4)||number!=bindingFlags||
        !read(bindings+8,&again,8)||again!=bound||!read(s.statePointer+0x24,&number,4)||number!=s.handle||
        !read(bound+0x2e8,&again,8)||again!=body||!read(body,&again,8)||again!=base+0x1ab58f8||!read(body+8,&again,8)||again!=bodyName||!worldIdentityString(read,bodyName,text)||text!=bodyAsset||
        !read(states.component+0x2f8,&again,8)||again!=input||!read(input+0x6c,&number,4)||number!=crouchIndex||
        !read(input+0xa0,&lastBits,8)||((lastBits^inputBits)&(1ull<<crouchIndex)))return false;
    for(unsigned i=0;i<2;++i){const auto at=manager+(i?0x610:0);if(!read(at,&again,8)||again!=lists[i]||!read(at+8,&number,4)||number!=flags[i])return false;}
    if(s.strong){NativePlayerArm arm{};
        for(unsigned i=0;i<2;++i)if(!read(bindings+(i+1)*0x20+8,&again,8)||again!=armActors[i]||worldIdentityEntity(read,game,armHandles[i])!=armActors[i])return false;
        if(!readNativePlayerArm(read,base,game,armHandles[0],arm)||!sameNativePlayerArm(arm,s.rightArm)||!readNativePlayerArm(read,base,game,armHandles[1],arm)||!sameNativePlayerArm(arm,s.leftArm))return false;
    }
    NativePlayerStates final{};
    if(!readNativePlayerStates(read,base,final)||final.player.game!=game||final.player.player!=states.player.player||final.component!=states.component||final.mode!=states.mode||
        !final.defaultMode||final.active.size()!=1||final.active[0].id!=4||final.active[0].pointer!=s.statePointer||final.active[0].vtable!=base+0x1c82158||worldIdentityEntity(read,game,1)!=bound)return false;
    out=std::move(s);return true;
}
