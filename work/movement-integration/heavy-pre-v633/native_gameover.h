#pragma once
#include <cstdint>
struct NativeGameOverState {
    std::uintptr_t game=0,manager=0,state=0,flash=0,showFunction[2]{};
    unsigned logic=0,pending=0;
};
template<class Reader> bool readNativeGameOver(Reader read,std::uintptr_t base,NativeGameOverState& s) {
    s={};std::uintptr_t vt=0,owner=0,stack=0;unsigned count=0,phase=0;
    if(!read(base+0x25af110+0x19a8,&s.game,8)||!s.game||!read(s.game,&vt,8)||vt!=base+0x1c940a8||
       !read(s.game+0x341058,&phase,4)||phase!=3)return false;
    s.manager=s.game+0x1f6ec8;
    if(!read(s.manager+8,&owner,8)||owner!=s.game||!read(s.manager+0x80,&s.state,8)||!s.state||
       !read(s.state,&vt,8)||vt!=base+0x1b6aab8||!read(s.state+8,&owner,8)||owner!=s.manager||
       !read(s.manager+0x528,&stack,8)||!stack||!read(s.manager+0x530,&count,4)||!(count&0xffffff)||(count&0xffffff)>32||
       !read(stack+((count&0xffffff)-1)*4,&s.logic,4)||s.logic>16||!read(s.manager+0x538,&s.pending,4)||s.pending>16||
       !read(s.state+0x20,&s.flash,8)||!read(s.state+0x60,s.showFunction,sizeof(s.showFunction)))return false;
    return true;
}
