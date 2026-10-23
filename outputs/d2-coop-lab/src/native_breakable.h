#pragma once
#include "world_identity.h"
#include <cmath>
#include <string>
// 1214fq (#132): read-only breakable reader. A breakable is an arkBreakable entity (vtable RVA 1b67bc8; 51 on the
// Dunwall lab map: door, display-case and cupboard glass) with one arkCpntBreakable (vtable RVA 1c71a88; confirmed
// runtime by --components). Component layout (confirmed static, 1407ed000/1407ed920): +8 declaration (steps at +0x68,
// stride 0x60, health threshold at +0; count at +0x70, low 24 bits), +0x10 owner, +0x20 its arkCpntHealth (current at
// +0x20), +0x38 the current step. 1407ed920 advances a step when health falls to the step threshold; 1407ed000
// (component, damage params) advances one step: detaches the step's pieces, spawns its FX and chunks with the hit
// impulse (params +0x24 hit point, +0x3c direction, +0x54 attacker) and notifies scripts.
// 1214io (#178): movable props break the same way. Bottles, glasses and bales are arkMovable entities (vtable RVA 1c5de18,
// confirmed runtime) with the same arkCpntBreakable (one step) and arkCpntHealth; a Windblast broke six of them in the
// host's world only (physics-props-v1769a).
inline bool breakableEntityVtable(std::uintptr_t vt,std::uintptr_t base){return vt==base+0x1b67bc8||vt==base+0x1c5de18;}
struct NativeBreakable {
    std::uint32_t handle=0;std::uintptr_t actor=0,component=0,declaration=0,steps=0,health=0;
    int step=0,stepCount=0;float current=0;std::string name;
};
template<class Read> bool readNativeBreakable(Read read,std::uintptr_t base,std::uintptr_t game,std::uint32_t handle,NativeBreakable& out,bool withName=false){
    NativeBreakable b{};b.handle=handle;b.actor=worldIdentityEntity(read,game,handle);
    std::uintptr_t vt=0,list=0,owner=0;std::uint32_t count=0,steps=0;
    if(!b.actor||!read(b.actor,&vt,8)||!breakableEntityVtable(vt,base)||!read(b.actor+0x268,&list,8)||!list||!read(b.actor+0x270,&count,4))return false;
    count&=0xffffff;if(!count||count>64)return false;
    for(std::uint32_t i=0;i<count;++i){std::uintptr_t c=0;if(!read(list+i*8,&c,8)||!c||!read(c,&vt,8))return false;
        if(vt==base+0x1c71a88){if(b.component)return false;b.component=c;}}
    if(!b.component||!read(b.component+0x10,&owner,8)||owner!=b.actor||!read(b.component+8,&b.declaration,8)||!b.declaration||
       !read(b.declaration+0x68,&b.steps,8)||!read(b.declaration+0x70,&steps,4)||!read(b.component+0x38,&b.step,4))return false;
    b.stepCount=static_cast<int>(steps&0xffffff);if(b.stepCount<1||b.stepCount>64||!b.steps||b.step<0||b.step>b.stepCount)return false;
    if(read(b.component+0x20,&b.health,8)&&b.health)read(b.health+0x20,&b.current,4);
    if(withName){std::uintptr_t p=0;if(!read(b.actor+0x20,&p,8)||!worldIdentityString(read,p,b.name)||b.name.empty()||b.name.size()>=192)return false;}
    out=b;return true;
}
// The health threshold that breaks step i (the breakable's health at or below it advances to step i+1).
template<class Read> bool readNativeBreakableThreshold(Read read,const NativeBreakable& b,int i,float& threshold){
    return i>=0&&i<b.stepCount&&read(b.steps+static_cast<std::uintptr_t>(i)*0x60,&threshold,4)&&std::isfinite(threshold);
}
