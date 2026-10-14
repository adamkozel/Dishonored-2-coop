#pragma once
#include "native_player_power_item.h"
#include "native_player_states.h"
struct NativePlayerGrapple {
    std::uintptr_t body=0,record=0,declaration=0;unsigned id=0,flags=0;
    bool present=false,pulling=false;std::array<float,3> hand{},end{},goal{},impact{},normal{};
};
// Read native PlayerBody's owned Far Reach beam. Never invokes the first-person
// body callbacks or transfers native handles to the other process.
template<class Read> bool readNativePlayerGrapple(Read read,std::uintptr_t base,NativePlayerGrapple& out){
    NativePlayerStates states{};NativePlayerPowerItem power{};NativePlayerGrapple g{};
    if(!readNativePlayerStates(read,base,states)||!states.defaultMode||states.player.logic!=1||
       !readNativePlayerPowerItem(read,base,power)||power.player.character!=2||power.quantity!=1||!power.equipped||!power.started||
       power.player.sword.game!=states.player.game||power.player.sword.actor!=states.player.player)return false;
    for(const auto& a:states.active){
        if(a.id==27&&a.vtable==base+0x1c7a328)g.pulling=true;
        else if(!((a.id==0&&a.vtable==base+0x1c85628)||(a.id==63&&a.vtable==base+0x1c80ed8)))return false;
    }
    std::uintptr_t vt=0,owner=0,bodyDecl=0,callback=0,name=0;std::string text;
    if(!read(states.component+0x300,&g.body,8)||!g.body||!read(g.body,&vt,8)||vt!=base+0x1c5a8d8||
       !read(g.body+0x10,&owner,8)||owner!=states.player.player||!read(g.body+8,&bodyDecl,8)||!bodyDecl||
       !read(bodyDecl+0x1e0,&g.declaration,8)||!g.declaration||!read(g.declaration,&vt,8)||vt!=base+0x1ab3138||
       !read(g.declaration+8,&name,8)||!worldIdentityString(read,name,text)||text!="models/effects/gameplay/powers/shadow_grapple/grapple/sgrapple_body_01.fx"||
       !read(g.body+0xe90,&g.record,8))return false;
    if(g.record){unsigned entity=0;std::uintptr_t decl=0;
        if(!read(g.record,&entity,4)||entity!=1||!read(g.record+4,&g.id,4)||g.id==~0u||
           !read(g.record+8,&decl,8)||decl!=g.declaration||!read(g.record+0x128,&callback,8)||callback!=g.body+0x20||
           !read(g.record+0x138,&g.flags,4)||(g.flags&4)||!read(g.record+0x38,g.hand.data(),12)||
           !read(g.record+0x80,g.end.data(),12)||!read(g.body+0xe74,g.goal.data(),12)||!read(power.component+0x114,g.impact.data(),12)||!read(power.component+0x12c,g.normal.data(),12))return false;
        float distance=0;for(unsigned k=0;k<3;++k){for(float v:{g.hand[k],g.end[k],g.goal[k],g.impact[k],g.normal[k]})if(!std::isfinite(v)||std::fabs(v)>100000)return false;
            const float d=g.goal[k]-states.player.position[k];distance+=d*d;}
        if(distance>900)return false;g.present=true;
    }
    std::uintptr_t again=0;unsigned id=0;
    if(!read(base+0x39a5290,&again,8)||again!=states.player.game||worldIdentityEntity(read,states.player.game,1)!=states.player.player||
       !read(states.component+0x300,&again,8)||again!=g.body||!read(g.body+0xe90,&again,8)||again!=g.record||
       (g.record&&(!read(g.record+4,&id,4)||id!=g.id)))return false;
    out=g;return true;
}
