#pragma once
#include "native_player_stance.h"
// Exact retail component, verified against native power expenditure and potion
// recovery. Current value only: do not infer maximum, regeneration or cost.
struct NativePlayerMana {
    std::uintptr_t game=0,player=0,physics=0,list=0,component=0,declaration=0;
    unsigned count=0;float current=0;
};
template<class Read> bool readNativePlayerMana(Read read,std::uintptr_t base,NativePlayerMana& out){
    NativePlayerStance first{},last{};NativePlayerMana s{};std::uintptr_t vt=0,owner=0;
    if(!readNativePlayerStance(read,base,first))return false;
    s.game=first.game;s.player=first.player;s.physics=first.physics;
    if(!read(s.player+0x268,&s.list,8)||!s.list||!read(s.player+0x270,&s.count,4)||!(s.count&0xffffff)||(s.count&0xffffff)>256)return false;
    unsigned index=0;
    for(unsigned i=0;i<(s.count&0xffffff);++i){std::uintptr_t c=0;
        if(!read(s.list+i*8,&c,8)||!c||!read(c,&vt,8))return false;
        if(vt!=base+0x1c55ed8)continue;
        if(s.component)return false;s.component=c;index=i;
    }
    if(!s.component||!read(s.component+0x10,&owner,8)||owner!=s.player||
       !read(s.component+8,&s.declaration,8)||!s.declaration||!read(s.declaration,&vt,8)||vt!=base+0x1c55d98||
       !read(s.component+0x20,&s.current,4)||!std::isfinite(s.current)||s.current<0||s.current>10000)return false;
    std::uintptr_t again=0;unsigned count=0;float current=0;
    if(!readNativePlayerStance(read,base,last)||last.game!=s.game||last.player!=s.player||last.physics!=s.physics||
       !read(s.player+0x268,&again,8)||again!=s.list||!read(s.player+0x270,&count,4)||count!=s.count||
       !read(s.list+index*8,&again,8)||again!=s.component||!read(s.component,&vt,8)||vt!=base+0x1c55ed8||
       !read(s.component+0x10,&owner,8)||owner!=s.player||!read(s.component+8,&again,8)||again!=s.declaration||
       !read(s.declaration,&vt,8)||vt!=base+0x1c55d98||!read(s.component+0x20,&current,4)||current!=s.current)return false;
    out=s;return true;
}
