#pragma once
#include "native_player_states.h"
#include "native_player_arm.h"
#include <array>
#include <algorithm>
#include <cmath>
// Exact native state9, independently activated by ordinary sprint+jump input
// in native-vault-trigger-*-v4 and native-vault-unarmed-*-v5. Read only.
struct NativePlayerVault {
    std::uintptr_t state=0,instance=0;unsigned phase=0,handle=0,character=0,sceneVariant=0,clockMs=0,durationMs=0;
    std::array<float,3> start{},point{},goal{},direction{};float height=0,progress=0;
};
inline bool validVaultGeometry(const std::array<float,3>& start,const std::array<float,3>& point,const std::array<float,3>& goal,float height){
    if(!std::isfinite(height)||height<.5f||height>1.35f)return false;
    float length=0,along=0;
    for(unsigned k=0;k<3;++k){
        if(!std::isfinite(start[k])||!std::isfinite(point[k])||!std::isfinite(goal[k])||std::fabs(start[k])>100000||std::fabs(point[k])>100000||std::fabs(goal[k])>100000)return false;
        length+=(goal[k]-start[k])*(goal[k]-start[k]);along+=(point[k]-start[k])*(goal[k]-start[k]);
    }
    return length>=.25f&&length<=36&&along>=0&&along<=length&&std::fabs(goal[2]-start[2])<2;
}
template<class Read> bool readNativePlayerVault(Read read,std::uintptr_t base,const NativePlayerStates& states,NativePlayerVault& out){
    if(states.player.logic!=1||!states.defaultMode||states.active.size()!=1||states.active[0].id!=9||states.active[0].vtable!=base+0x1c86e18)return false;
    NativePlayerVault s{};s.state=states.active[0].pointer;
    if(!read(s.state+0x38,&s.phase,4)||s.phase>2||!read(s.state+0x24,&s.handle,4)||
       !read(s.state+0x58,s.start.data(),12)||!read(s.state+0x64,s.point.data(),12)||!read(s.state+0x70,s.goal.data(),12)||
       !read(s.state+0x4c,s.direction.data(),12)||!read(s.state+0x7c,&s.height,4)||!validVaultGeometry(s.start,s.point,s.goal,s.height))return false;
    float norm=0,span=0,travel=0;
    for(unsigned k=0;k<3;++k){if(!std::isfinite(s.direction[k])||!std::isfinite(states.player.position[k]))return false;
        norm+=s.direction[k]*s.direction[k];span+=(s.goal[k]-s.start[k])*s.direction[k];travel+=(states.player.position[k]-s.start[k])*s.direction[k];}
    if(norm<.99f||norm>1.01f||span<.5f||span>6)return false;
    s.progress=s.phase==0?0.f:s.phase==2?1.f:(std::max)(0.f,(std::min)(1.f,travel/span));
    std::uintptr_t body=0,name=0,vt=0;std::string bodyName;
    if(!read(states.player.player+0x2e8,&body,8)||!body||!read(body,&vt,8)||vt!=base+0x1ab58f8||
       !read(body+8,&name,8)||!worldIdentityString(read,name,bodyName))return false;
    if(bodyName=="models/characters/player/corvo/body.def")s.character=1;
    else if(bodyName=="models/characters/player/emily/body.def")s.character=2;else return false;
    const auto game=states.player.game,manager=game+0x1f5338;
    std::array<std::uintptr_t,2> lists{};std::array<unsigned,2> flags{};std::array<NativePlayerArm,2> arms{};
    std::array<std::uintptr_t,3> actors{};std::uintptr_t declaration=0,scene=0,bindings=0;unsigned bindingFlags=0;
    if(s.phase==1){
        if(!s.handle)return false;
        for(unsigned f=0;f<2;++f){const auto at=manager+(f?0x610:0);
            if(!read(at,&lists[f],8)||!read(at+8,&flags[f],4)||(flags[f]&0xffffff)>512||((flags[f]&0xffffff)&&!lists[f]))return false;
            for(unsigned i=0;i<(flags[f]&0xffffff);++i){std::uintptr_t c=0;unsigned id=0;if(!read(lists[f]+i*8,&c,8)||!c||!read(c+0x158,&id,4))return false;
                if(id==s.handle){if(s.instance&&s.instance!=c)return false;s.instance=c;}}
        }
        unsigned phase=0;std::uintptr_t assetName=0,subName=0;std::string asset,subscene;
        if(!s.instance||!read(s.instance+0x27c,&phase,4)||phase!=3||!read(s.instance,&declaration,8)||!read(declaration,&vt,8)||vt!=base+0x1affbc8||
           !read(declaration+8,&assetName,8)||!worldIdentityString(read,assetName,asset)||!read(s.instance+8,&scene,8)||!read(scene,&vt,8)||vt!=base+0x1b00198||
           !read(scene+0x28,&s.durationMs,4)||s.durationMs!=933||!read(scene+0x30,&subName,8)||!worldIdentityString(read,subName,subscene)||subscene!=(s.character==1?"Scene_2":"Scene_1")||
           !read(s.instance+0x160,&s.clockMs,4)||s.clockMs>s.durationMs||!read(s.instance+0x10,&bindings,8)||!bindings||!read(s.instance+0x18,&bindingFlags,4)||(bindingFlags&0xffffff)!=3)return false;
        if(asset=="midnight/gameplay/player/base/mobility/vault_1.midnightscene")s.sceneVariant=1;
        else if(asset=="midnight/gameplay/player/base/mobility/vault_2.midnightscene")s.sceneVariant=2;else return false;
        unsigned playerCount=0;
        for(unsigned i=0;i<3;++i){unsigned handle=0;
            if(!read(bindings+i*0x20+8,&actors[i],8)||!actors[i]||!read(actors[i]+0xa8,&handle,4)||worldIdentityEntity(read,game,handle)!=actors[i])return false;
            if(actors[i]==states.player.player){if(++playerCount>1||handle!=1)return false;}
            else{NativePlayerArm arm{};if(!readNativePlayerArm(read,base,game,handle,arm)||arm.actor!=actors[i]||arm.player!=states.player.player||arm.character!=s.character||arm.side<1||arm.side>2||arms[arm.side-1].actor)return false;arms[arm.side-1]=arm;}
        }
        if(playerCount!=1||!arms[0].actor||!arms[1].actor)return false;
    }else if(s.handle)return false; // Native approach/exit do not own a scene.
    std::uintptr_t again=0;unsigned value=0;float height=0;std::array<float,3> vector{};
    if(worldIdentityEntity(read,game,1)!=states.player.player||!read(states.player.player+0x2e8,&again,8)||again!=body||
       !read(body,&again,8)||again!=base+0x1ab58f8||!read(body+8,&again,8)||again!=name||
       !read(s.state+0x38,&value,4)||value!=s.phase||!read(s.state+0x24,&value,4)||value!=s.handle||
       !read(s.state+0x58,vector.data(),12)||vector!=s.start||!read(s.state+0x64,vector.data(),12)||vector!=s.point||
       !read(s.state+0x70,vector.data(),12)||vector!=s.goal||!read(s.state+0x4c,vector.data(),12)||vector!=s.direction||!read(s.state+0x7c,&height,4)||height!=s.height)return false;
    if(s.phase==1){
        if(!read(s.instance+0x158,&value,4)||value!=s.handle||!read(s.instance+0x27c,&value,4)||value!=3||
           !read(s.instance,&again,8)||again!=declaration||!read(s.instance+8,&again,8)||again!=scene||!read(s.instance+0x10,&again,8)||again!=bindings||!read(s.instance+0x18,&value,4)||value!=bindingFlags)return false;
        for(unsigned f=0;f<2;++f){const auto at=manager+(f?0x610:0);if(!read(at,&again,8)||again!=lists[f]||!read(at+8,&value,4)||value!=flags[f])return false;}
        for(unsigned i=0;i<3;++i)if(!read(bindings+i*0x20+8,&again,8)||again!=actors[i])return false;
        for(const auto& arm:arms){NativePlayerArm final{};if(!readNativePlayerArm(read,base,game,arm.handle,final)||!sameNativePlayerArm(arm,final))return false;}
    }
    NativePlayerStates final{};if(!readNativePlayerStates(read,base,final)||!final.defaultMode||final.player.game!=game||final.player.player!=states.player.player||final.component!=states.component||final.mode!=states.mode||
       final.active.size()!=1||final.active[0].id!=9||final.active[0].pointer!=s.state||final.active[0].vtable!=base+0x1c86e18)return false;
    out=s;return true;
}
