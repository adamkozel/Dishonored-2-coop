#pragma once
#include "native_health.h"
#include <array>
// Read-only arkCpntAIKnowledge inline arkBlackboard_Knowledge. Reflected
// attention list +148 and combat record +cb0 are relative to component+20.
// Perception records share the native entity handle at +c. A retained target
// can outlive its entity; report resolution separately instead of rebinding it.
struct NativeAiTargetRecord {
    std::uint32_t handle=0,flags=0,perceived=3,cached=3;
    bool resolved=false;
    bool operator==(const NativeAiTargetRecord& b)const{return handle==b.handle&&flags==b.flags&&perceived==b.perceived&&cached==b.cached&&resolved==b.resolved;}
};
struct NativeAiTargets {
    std::uintptr_t game=0,actor=0,component=0,list=0;
    std::uint32_t handle=0,logic=0,flags=0,count=0;
    NativeAiTargetRecord combat{};
    std::vector<NativeAiTargetRecord> attention;
};
template<class Read> bool readNativeAiTargetsFrame(Read read,std::uintptr_t base,std::uint32_t handle,NativeAiTargets& out){
    NativeHealth h{};if(handle==1||!readNativeHealth(read,base,handle,h))return false;
    NativeAiTargets s{};s.handle=handle;s.game=h.game;s.actor=h.actor;s.logic=h.logic;
    std::uintptr_t vt=0,owner=0;
    for(unsigned i=0;i<(h.componentCount&0xffffff);++i){
        std::uintptr_t c=0;if(!read(h.list+i*8,&c,8)||!c||!read(c,&vt,8))return false;
        if(vt==base+0x1b95308){if(s.component)return false;s.component=c;}
    }
    if(!s.component||!read(s.component+0x10,&owner,8)||owner!=s.actor||!read(s.component+0x1c,&s.flags,4)||(s.flags&0xc)!=8||
       !read(s.component+0x168,&s.list,8)||!read(s.component+0x170,&s.count,4)||(s.count&0xffffff)>128)return false;
    const auto record=[&](std::uintptr_t at,std::uintptr_t expected,NativeAiTargetRecord& r){
        if(!read(at,&vt,8)||vt!=base+expected||!read(at+8,&r.flags,4)||!read(at+12,&r.handle,4))return false;
        r.resolved=worldIdentityEntity(read,s.game,r.handle)!=0;return true;
    };
    if(!record(s.component+0xcd0,0x1b44828,s.combat))return false;
    for(unsigned i=0;i<(s.count&0xffffff);++i){
        NativeAiTargetRecord r{};const auto at=s.list+i*0x178;
        if(!s.list||!record(at,0x1b447c8,r)||!read(at+0x168,&r.perceived,4)||!read(at+0x16c,&r.cached,4)||r.perceived>3||r.cached>3)return false;
        s.attention.push_back(r);
    }
    out=std::move(s);return true;
}
template<class Read> bool readNativeAiTargets(Read read,std::uintptr_t base,std::uint32_t handle,NativeAiTargets& out){
    NativeAiTargets a{},b{};
    if(!readNativeAiTargetsFrame(read,base,handle,a)||!readNativeAiTargetsFrame(read,base,handle,b)||a.game!=b.game||a.actor!=b.actor||a.component!=b.component||a.list!=b.list||
       a.logic!=b.logic||a.flags!=b.flags||a.count!=b.count||!(a.combat==b.combat)||a.attention!=b.attention)return false;
    out=std::move(a);return true;
}
