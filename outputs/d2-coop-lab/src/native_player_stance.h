#pragma once
#include <cstdint>
#include <cmath>

// Read-only, inspected retail build. Native 0x7fff02 compares these two
// fields and emits playerStanceChanged. Values are raw until live mapping.
struct NativePlayerStance {
    std::uintptr_t game=0,player=0,physics=0;
    std::uint32_t current=0,previous=0,logic=0;
    float position[3]{};
};
// allowWheel (1074c): the action-stream hold alone may read the native item
// wheel (logic8); every other caller keeps the gameplay-only contract.
template<class Read> bool readNativePlayerStance(Read read,std::uintptr_t base,NativePlayerStance& out,bool allowWheel=false) {
    NativePlayerStance s{};
    std::uintptr_t vt=0,list=0,owner=0,stack=0;std::uint32_t flags=0,handle=0,count=0,pending=0,phase=0;
    if(!read(base+0x39a5290,&s.game,8)||!s.game||!read(s.game,&vt,8)||vt!=base+0x1c940a8||
       !read(s.game+0x341058,&phase,4)||phase!=3||
       !read(s.game+0x1f6ed0,&owner,8)||owner!=s.game||
       !read(s.game+0x1f73f0,&stack,8)||!stack||!read(s.game+0x1f73f8,&count,4)||
       !(count&0xffffff)||(count&0xffffff)>32||!read(stack+((count&0xffffff)-1)*4,&s.logic,4)||(s.logic!=1&&!(allowWheel&&s.logic==8))||
       !read(s.game+0x1f7400,&pending,4)||pending||
       !read(s.game+0x3130b0,&s.player,8)||!s.player||!read(s.player,&vt,8)||vt!=base+0x1c5e428||
       !read(s.player+0xa8,&handle,4)||handle!=1||
       !read(s.player+0x268,&list,8)||!list||!read(s.player+0x270,&flags,4)||
       !(flags&0xffffff)||(flags&0xffffff)>256)return false;
    unsigned index=0;
    for(unsigned i=0;i<(flags&0xffffff);++i) {
        std::uintptr_t component=0;
        if(!read(list+i*8,&component,8)||!component||!read(component,&vt,8))return false;
        if(vt!=base+0x1c73268)continue;
        if(s.physics||!read(component+0x10,&owner,8)||owner!=s.player)return false;
        s.physics=component;index=i;
    }
    if(!s.physics||!read(s.physics+0x50,&s.current,4)||!read(s.physics+0x54,&s.previous,4)||
       !read(s.player+0x300,s.position,sizeof(s.position)))return false;
    for(float value:s.position)if(!std::isfinite(value))return false;
    // A fresh lookup on every call, then recheck identity and state. Never
    // carry a component pointer across a load or accept a mid-read transition.
    std::uintptr_t again=0;std::uint32_t value=0;
    if(!read(base+0x39a5290,&again,8)||again!=s.game||
       !read(s.game+0x3130b0,&again,8)||again!=s.player||
       !read(s.player+0x268,&again,8)||again!=list||!read(s.player+0x270,&value,4)||value!=flags||
       !read(list+index*8,&again,8)||again!=s.physics||
       !read(s.physics,&again,8)||again!=base+0x1c73268||!read(s.physics+0x10,&again,8)||again!=s.player||
       !read(s.physics+0x50,&value,4)||value!=s.current||!read(s.physics+0x54,&value,4)||value!=s.previous||
       !read(s.game+0x1f73f0,&again,8)||again!=stack||!read(s.game+0x1f73f8,&value,4)||value!=count||
       !read(stack+((count&0xffffff)-1)*4,&value,4)||value!=s.logic||
       !read(s.game+0x1f7400,&value,4)||value)return false;
    out=s;return true;
}
