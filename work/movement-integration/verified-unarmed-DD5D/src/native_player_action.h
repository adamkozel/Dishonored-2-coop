#pragma once
#include "native_player_states.h"
#include <algorithm>
#include <cmath>
// Observed on both roles in B671F824: Slash 39 / VT1c85d28 and
// Block 44 / VT1c81a68, alongside ordinary Running. Unknown states fail closed.
inline bool nativePlayerSwordAction(const NativePlayerStates& s,std::uintptr_t base,unsigned& action){
    if(!s.defaultMode||s.active.empty()||s.active.size()>2)return false;
    bool motion=false,slash=false,block=false;
    for(const auto& a:s.active){
        if(a.id==39&&a.vtable==base+0x1c85d28){if(slash)return false;slash=true;}
        else if(a.id==44&&a.vtable==base+0x1c81a68){if(block)return false;block=true;}
        else {
            bool known=false;
            for(const auto& e:{NativePlayerActiveState{0,0,0x1c85628},NativePlayerActiveState{6,0,0x1c84258},
                 NativePlayerActiveState{2,0,0x1c85028},NativePlayerActiveState{3,0,0x1c845c8}})
                if(a.id==e.id&&a.vtable==base+e.vtable)known=true;
            if(!known||motion)return false;motion=true;
        }
    }
    if(!motion||(slash&&block))return false;
    action=slash?1u:block?2u:0u;return true;
}

// Presentation-only airborne priority. Both exact native Jump/Fall classes
// were observed on each role; no raw input or root-height inference.
inline bool nativePlayerShadowAction(const NativePlayerStates& s,std::uintptr_t base,unsigned& action){
    if(s.defaultMode&&s.active.size()==2){
        bool ground=false,lean=false,equipment=false,request=false;
        for(const auto& a:s.active){
            ground=ground||(a.id==0&&a.vtable==base+0x1c85628)||(a.id==6&&a.vtable==base+0x1c84258);
            lean=lean||(a.id==18&&a.vtable==base+0x1c82b28);
            equipment=equipment||(a.id==53&&a.vtable==base+0x1c79cf8);
            request=request||(a.id==54&&a.vtable==base+0x1c78a68)||(a.id==81&&a.vtable==base+0x1c7df88);
        }
        if(ground&&lean){action=5;return true;}
        if(ground&&(equipment||request)){action=equipment?7u:0u;return true;}
    }
    // Distinct native sprint activation observed on both roles. Do not infer
    // sprint from root speed, and do not invent unobserved overlay semantics.
    if(s.defaultMode&&s.active.size()==1&&s.active[0].id==1&&s.active[0].vtable==base+0x1c85948){action=4;return true;}
    // Sliding begins in raw physics stance 5; native exit changes capsule
    // stance before retiring ID7 (observed during slide-to-jump). Therefore
    // the exact active class owns this action through its exit as well.
    if(s.defaultMode&&(s.player.current==0||s.player.current==1||s.player.current==5)&&s.active.size()==1&&s.active[0].id==7&&s.active[0].vtable==base+0x1c86108){action=6;return true;}
    if(!nativePlayerSwordAction(s,base,action))return false;
    for(const auto& a:s.active)if(a.id==2||a.id==3){action=3;break;}
    return true;
}
// Read-only native lean amount. Ghidra 14085dcc0 and both-role runtime input
// observations establish these fields; local camera data is never modified.
template<class Read> bool nativePlayerLeanAmount(Read read,const NativePlayerStates& s,std::uintptr_t base,float& side,float& up){
    unsigned action=0;if(!nativePlayerShadowAction(s,base,action)||action!=5)return false;
    for(const auto& a:s.active)if(a.id==18){
        float scale=0;std::uintptr_t vt=0,owner=0;unsigned id=0;
        if(!read(a.pointer+0x40,&side,4)||!read(a.pointer+0x44,&up,4)||!read(a.pointer+0x50,&scale,4)||
           !std::isfinite(side)||!std::isfinite(up)||!std::isfinite(scale)||std::fabs(side)>1.001f||up<0||up>1.001f||scale<0||scale>1.001f||
           !read(a.pointer,&vt,8)||vt!=a.vtable||!read(a.pointer+8,&id,4)||id!=18||!read(a.pointer+0x10,&owner,8)||owner!=s.component)return false;
        side=(std::max)(-1.f,(std::min)(1.f,side*scale));up=(std::min)(1.f,up);return true;
    }return false;
}
// Correlated in native-jump-timing-v1 on both roles: entry stores original
// foot/root Z at A0 and absolute target apex at A4. Read only, no physics call.
template<class Read> bool nativePlayerAir(Read read,const NativePlayerStates& s,std::uintptr_t base,unsigned& stage,float& startZ,float& targetZ,unsigned& remaining){
    unsigned action=0;if(!nativePlayerShadowAction(s,base,action)||action!=3)return false;
    for(const auto& a:s.active){
        if(a.id==3&&a.vtable==base+0x1c845c8){stage=2;startZ=targetZ=0;remaining=0;return true;}
        if(a.id!=2||a.vtable!=base+0x1c85028)continue;
        std::uintptr_t vt=0,owner=0;unsigned id=0;
        if(!read(a.pointer+0xa0,&startZ,4)||!read(a.pointer+0xa4,&targetZ,4)||!read(a.pointer+0x94,&remaining,4)||
           !std::isfinite(startZ)||!std::isfinite(targetZ)||std::fabs(startZ)>100000.f||std::fabs(targetZ)>100000.f||
           targetZ-startZ<.05f||targetZ-startZ>10.f||remaining>3||
           !read(a.pointer,&vt,8)||vt!=a.vtable||!read(a.pointer+8,&id,4)||id!=2||!read(a.pointer+0x10,&owner,8)||owner!=s.component)return false;
        stage=1;return true;
    }return false;
}
