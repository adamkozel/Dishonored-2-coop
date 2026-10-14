#pragma once
#include "native_player_states.h"
#include "native_player_arm.h"
#include "world_identity.h"
#include <array>
#include <cmath>
// Native low mantle observed on both roles in native-traversal-*-v1.
// Exact scene binds the actual player, optionally with its owned right arm.
// Both forms were observed at the same Dunwall low-mantle fixture.
struct NativePlayerTraversalScene {
    std::uintptr_t instance=0,statePointer=0;unsigned handle=0,variant=0,timeMs=0,durationMs=0,endStance=0;
    std::array<float,3> start{},goal{};std::string asset,subscene;
};
inline bool validTraversalGeometry(const std::array<float,3>& start,const std::array<float,3>& goal){
    float horizontal=0;for(unsigned k=0;k<3;++k){if(!std::isfinite(start[k])||!std::isfinite(goal[k])||std::fabs(start[k])>100000||std::fabs(goal[k])>100000)return false;if(k<2)horizontal+=(goal[k]-start[k])*(goal[k]-start[k]);}
    return horizontal<=16&&goal[2]-start[2]>=.05f&&goal[2]-start[2]<=3.f;
}
template<class Read> bool readNativePlayerTraversalScene(Read read,std::uintptr_t base,const NativePlayerStates& states,NativePlayerTraversalScene& out,unsigned* researchStage=nullptr){
    if(researchStage)*researchStage=1;
    NativePlayerTraversalScene s{};
    if(states.player.logic!=1||!states.defaultMode||states.active.size()!=1||states.active[0].id!=8||states.active[0].vtable!=base+0x1c82f68)return false;
    s.statePointer=states.active[0].pointer;unsigned kind=0,next=0;
    if(researchStage)*researchStage=2;
    if(!read(s.statePointer+0x24,&s.handle,4)||!s.handle||!read(s.statePointer+0xd8,&kind,4)||kind!=1||
       !read(s.statePointer+0xb8,&next,4)||next!=0||!read(s.statePointer+0xe8,s.start.data(),12)||!read(s.statePointer+0xbc,s.goal.data(),12)||!validTraversalGeometry(s.start,s.goal))return false;
    const auto game=states.player.game,manager=game+0x1f5338;std::array<std::uintptr_t,2> lists{};std::array<unsigned,2> flags{};
    if(researchStage)*researchStage=3;
    for(unsigned field=0;field<2;++field){const auto at=manager+(field?0x610:0);
        if(!read(at,&lists[field],8)||!read(at+8,&flags[field],4)||(flags[field]&0xffffff)>512||((flags[field]&0xffffff)&&!lists[field]))return false;
        for(unsigned i=0;i<(flags[field]&0xffffff);++i){std::uintptr_t c=0;unsigned id=0;if(!read(lists[field]+i*8,&c,8)||!c||!read(c+0x158,&id,4))return false;
            if(id==s.handle){if(s.instance&&s.instance!=c)return false;s.instance=c;}}
    }
    std::uintptr_t declaration=0,scene=0,vt=0,name=0,subName=0,bindings=0;unsigned phase=0,bindingFlags=0;
    if(researchStage)*researchStage=4;
    if(!s.instance||!read(s.instance+0x27c,&phase,4)||phase!=3||!read(s.instance,&declaration,8)||
       !read(declaration,&vt,8)||vt!=base+0x1affbc8||!read(declaration+8,&name,8)||!worldIdentityString(read,name,s.asset)||
       s.asset!="midnight/gameplay/player/base/mobility/mantle_low_idle.midnightscene"||
       !read(s.instance+8,&scene,8)||!read(scene,&vt,8)||vt!=base+0x1b00198||!read(scene+0x28,&s.durationMs,4)||s.durationMs!=500||
       !read(scene+0x30,&subName,8)||!worldIdentityString(read,subName,s.subscene)||s.subscene!="Scene_1"||
       !read(s.instance+0x160,&s.timeMs,4)||s.timeMs>s.durationMs||!read(s.instance+0x10,&bindings,8)||!bindings||
       !read(s.instance+0x18,&bindingFlags,4)||(bindingFlags&0xffffff)<1||(bindingFlags&0xffffff)>2)return false;
    unsigned playerCount=0,armCount=0;NativePlayerArm arm{};std::array<std::uintptr_t,2> actors{};
    for(unsigned i=0;i<(bindingFlags&0xffffff);++i){
        unsigned handle=0;
        if(!read(bindings+i*0x20+8,&actors[i],8)||!actors[i]||!read(actors[i]+0xa8,&handle,4)||worldIdentityEntity(read,game,handle)!=actors[i])return false;
        if(actors[i]==states.player.player){if(++playerCount>1||handle!=1)return false;}
        else{NativePlayerArm candidate{};if(++armCount>1||!readNativePlayerArm(read,base,game,handle,candidate)||candidate.side!=2||candidate.actor!=actors[i]||candidate.player!=states.player.player)return false;arm=candidate;}
    }
    if(playerCount!=1)return false;
    std::uintptr_t again=0;unsigned number=0;std::array<float,3> vector{};
    if(researchStage)*researchStage=5;
    if(!read(s.instance+0x158,&number,4)||number!=s.handle||!read(s.instance+0x27c,&number,4)||number!=3||
       !read(s.instance,&again,8)||again!=declaration||!read(s.instance+8,&again,8)||again!=scene||!read(s.instance+0x10,&again,8)||again!=bindings||
       !read(s.instance+0x18,&number,4)||number!=bindingFlags||
       !read(s.statePointer+0x24,&number,4)||number!=s.handle||!read(s.statePointer+0xd8,&number,4)||number!=kind||!read(s.statePointer+0xb8,&number,4)||number!=next||
       !read(s.statePointer+0xe8,vector.data(),12)||vector!=s.start||!read(s.statePointer+0xbc,vector.data(),12)||vector!=s.goal)return false;
    for(unsigned i=0;i<2;++i){const auto at=manager+(i?0x610:0);if(!read(at,&again,8)||again!=lists[i]||!read(at+8,&number,4)||number!=flags[i])return false;}
    for(unsigned i=0;i<(bindingFlags&0xffffff);++i)if(!read(bindings+i*0x20+8,&again,8)||again!=actors[i])return false;
    if(armCount){NativePlayerArm finalArm{};if(!readNativePlayerArm(read,base,game,arm.handle,finalArm)||!sameNativePlayerArm(arm,finalArm))return false;}
    if(researchStage)*researchStage=6;
    NativePlayerStates final{};if(!readNativePlayerStates(read,base,final)||final.player.game!=game||final.player.player!=states.player.player||final.component!=states.component||final.mode!=states.mode||
       !final.defaultMode||final.active.size()!=1||final.active[0].id!=8||final.active[0].pointer!=s.statePointer||final.active[0].vtable!=base+0x1c82f68)return false;
    s.variant=1;out=std::move(s);if(researchStage)*researchStage=0;return true;
}
