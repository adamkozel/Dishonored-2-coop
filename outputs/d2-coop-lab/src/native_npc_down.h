#pragma once
#include <cstdint>
#include <cstring>
// 1214en (#11 enemy_body_size_variants, knocked down): whether an NPC is "on the ground" for the native takedown rules.
// IsOnGround 0x14082cc80 (chapter 100, confirmed static) is true when the NPC's physics component (type 0x39b4630) is in
// mode 0 or 2 (0x1405bf440), or its AIPhysicsReaction (type 0x39e2340, vtable 0x1b88928, recovery pose +0x13c = 3 at
// construction 0x140590990) is in recovery pose 1 (0x1405bf480), or is laying: +0x152 bit 3, +0x150 bit 1, the laying
// timer +0x138 above 0, or pose 0 (0x1405bf4c0). Components are matched the way D2_Entity_GetComponent 0x140877720 does:
// the class index that the component's type getter returns (vtable slot 0: lea rax,[type]; ret) lies within the requested
// type's index range (+0x60..+0x64).
// The host reads it for each streamed NPC and flags its poses; the client's replica is animated by those poses and its
// own physics never leaves mode 3 (take 1529b), so the client keeps the replica's laying timer above 0 while the host
// reports it down, which is what lets the client's own native rules offer the knocked-down takedowns.
constexpr std::uintptr_t kNativePhysicsComponentType=0x39b4630,kNativeAiPhysicsReactionType=0x39e2340;
constexpr float kNpcDownLayingSeconds=0.6f;
struct NativeNpcDown {
    std::uintptr_t physics=0,reaction=0;
    std::uint32_t mode=0xffffffffu;std::int32_t pose=3;float timer=0;unsigned char flags150=0,flags152=0;
};
template<class Read> std::uintptr_t nativeComponentTypeOf(Read read,std::uintptr_t component){
    std::uintptr_t vt=0,getter=0;unsigned char code[8]{};std::int32_t rel=0;
    if(!component||!read(component,&vt,8)||!vt||!read(vt,&getter,8)||!getter||!read(getter,code,8))return 0;
    if(code[0]!=0x48||code[1]!=0x8d||code[2]!=0x05||code[7]!=0xc3)return 0;
    std::memcpy(&rel,code+3,4);return getter+7+static_cast<std::intptr_t>(rel);
}
template<class Read> std::uintptr_t findNativeComponentOfType(Read read,std::uintptr_t actor,std::uintptr_t type){
    std::uintptr_t list=0;std::uint32_t count=0;std::int32_t lo=0,hi=0;
    if(!actor||!read(type+0x60,&lo,4)||!read(type+0x64,&hi,4)||lo>hi||!read(actor+0x268,&list,8)||!list||!read(actor+0x270,&count,4))return 0;
    count&=0xffffff;if(!count||count>256)return 0;
    for(std::uint32_t i=0;i<count;++i){std::uintptr_t component=0;std::int32_t index=0;
        if(!read(list+i*8,&component,8)||!component)continue;
        const auto t=nativeComponentTypeOf(read,component);
        if(t&&read(t+0x60,&index,4)&&lo<=index&&index<=hi)return component;}
    return 0;
}
template<class Read> bool readNativeNpcDown(Read read,std::uintptr_t base,std::uintptr_t actor,NativeNpcDown& d){
    d={};d.physics=findNativeComponentOfType(read,actor,base+kNativePhysicsComponentType);
    d.reaction=findNativeComponentOfType(read,actor,base+kNativeAiPhysicsReactionType);
    if(d.physics&&!read(d.physics+0x238,&d.mode,4))return false;
    if(d.reaction){std::uintptr_t vt=0;
        if(!read(d.reaction,&vt,8)||vt!=base+0x1b88928||!read(d.reaction+0x138,&d.timer,4)||!read(d.reaction+0x13c,&d.pose,4)||
           !read(d.reaction+0x150,&d.flags150,1)||!read(d.reaction+0x152,&d.flags152,1))return false;}
    return d.physics||d.reaction;
}
inline bool nativeNpcRagdoll(const NativeNpcDown& d){return d.physics&&(d.mode&0xfffffffdu)==0;}
inline bool nativeNpcDown(const NativeNpcDown& d){
    return nativeNpcRagdoll(d)||(d.reaction&&(d.pose==1||d.pose==0||(d.flags152&8)||d.timer>0||(d.flags150&2)));
}
// The client keeps the replica laying while host poses say it is down: refresh the timer when it falls below half.
inline bool npcDownTimerNeedsRefresh(float timer){return !(timer>=kNpcDownLayingSeconds*.5f);}
