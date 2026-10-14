#pragma once
#include "native_player_power_item.h"
#include "native_player_states.h"
struct NativePlayerBlink {
    std::uintptr_t state=0;std::array<float,3> start{},goal{},eyeGoal{},foot{};unsigned flags=0;
    bool completed=false;
};
inline bool nativeBlinkGeometry(const float* values,const float* origin,bool completed,NativePlayerBlink& b){
    for(unsigned i=0;i<9;++i)if(!std::isfinite(values[i])||std::fabs(values[i])>100000)return false;
    for(unsigned i=0;i<3;++i)if(!std::isfinite(origin[i])||std::fabs(origin[i])>100000)return false;
    const float height=values[5]-values[2];
    if(height<.5f||height>2.2f||std::fabs(values[3]-values[0])>.05f||std::fabs(values[4]-values[1])>.05f)return false;
    float distance=0;for(unsigned i=0;i<3;++i){const float offset=values[3+i]-values[i];
        b.goal[i]=values[i];b.eyeGoal[i]=values[3+i];b.start[i]=values[6+i]-offset;b.foot[i]=origin[i]-(completed?0:offset);
        const float d=b.goal[i]-b.start[i];distance+=d*d;}
    return distance>=.0001f&&distance<=625.f;
}
// Native Blinking uses an eye-height kinematic origin until 858010 restores
// the destination foot position and sets state+a4 bit0. This reader changes
// no local physics, first-person transform, camera or power component.
template<class Read> bool readNativePlayerBlink(Read read,std::uintptr_t base,const NativePlayerStates& states,NativePlayerBlink& out){
    if(!states.defaultMode||states.active.empty()||states.active.size()>2||states.player.logic!=1)return false;
    NativePlayerBlink b{};for(const auto& a:states.active){
        if(a.id==12&&a.vtable==base+0x1c815d8){if(b.state)return false;b.state=a.pointer;}
        else if(!(a.id==63&&a.vtable==base+0x1c80ed8))return false;
    }
    if(!b.state)return false;NativePlayerPowerItem power{};std::uintptr_t component=0;unsigned mode=0,priorStance=0;unsigned char flags=0;float values[9]{},targets[6]{};
    if(!readNativePlayerPowerItem(read,base,power)||power.player.character!=1||power.quantity!=1||!power.equipped||!power.started||
       power.player.sword.game!=states.player.game||power.player.sword.actor!=states.player.player||
       !read(states.player.player+0x380,&component,8)||component!=power.component||!read(b.state+0x50,&mode,4)||mode||
       !read(b.state+0x78,&priorStance,4)||priorStance||!read(b.state+0xa4,&flags,1)||
       !read(b.state+0x54,values,sizeof(values))||!read(component+0x1f4,targets,12)||!read(component+0x208,targets+3,12)||std::memcmp(values,targets,24))return false;
    b.flags=flags;b.completed=(flags&1)!=0;
    if((!b.completed&&states.player.current!=4)||(b.completed&&states.player.current!=0&&states.player.current!=4)||
       !nativeBlinkGeometry(values,states.player.position,b.completed,b))return false;
    NativePlayerStates final{};unsigned char finalFlags=0;float again[9]{};
    if(!readNativePlayerStates(read,base,final)||final.player.game!=states.player.game||final.player.player!=states.player.player||final.component!=states.component||
       final.mode!=states.mode||final.active.size()!=states.active.size()||final.player.current!=states.player.current||
       !read(b.state+0xa4,&finalFlags,1)||finalFlags!=flags||!read(b.state+0x54,again,sizeof(again))||std::memcmp(values,again,sizeof(values)))return false;
    for(unsigned i=0;i<states.active.size();++i)if(final.active[i].id!=states.active[i].id||final.active[i].pointer!=states.active[i].pointer||final.active[i].vtable!=states.active[i].vtable)return false;
    out=b;return true;
}
