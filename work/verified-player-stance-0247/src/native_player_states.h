#pragma once
#include "native_player_stance.h"
#include <vector>
#include <cstring>

struct NativePlayerActiveState {std::uint32_t id=0;std::uintptr_t pointer=0,vtable=0;};
struct NativePlayerStates {
    NativePlayerStance player;
    std::uintptr_t component=0,mode=0;
    std::vector<NativePlayerActiveState> active;
};
// Native 0x803cb7 / 0x803ff7 iterate component+0xa8 IDs and resolve each
// through mode+0x10+ID*8. A mode contains 90 authored state slots. Read only.
template<class Read> bool readNativePlayerStates(Read read,std::uintptr_t base,NativePlayerStates& out) {
    NativePlayerStates s{};
    if(!readNativePlayerStance(read,base,s.player))return false;
    std::uintptr_t list=0,vt=0,owner=0;std::uint32_t componentCount=0;
    if(!read(s.player.player+0x268,&list,8)||!list||!read(s.player.player+0x270,&componentCount,4)||
       !(componentCount&0xffffff)||(componentCount&0xffffff)>256)return false;
    unsigned index=0;
    for(unsigned i=0;i<(componentCount&0xffffff);++i) {
        std::uintptr_t c=0;if(!read(list+i*8,&c,8)||!c||!read(c,&vt,8))return false;
        if(vt!=base+0x1c73d58)continue;
        if(s.component||!read(c+0x10,&owner,8)||owner!=s.player.player)return false;
        s.component=c;index=i;
    }
    std::uintptr_t active=0;std::uint32_t count=0;
    if(!s.component||!read(s.component+0x90,&s.mode,8)||!s.mode||!read(s.mode,&owner,8)||owner!=s.component||
       !read(s.component+0xa8,&active,8)||!active||!read(s.component+0xb0,&count,4)||
       !(count&0xffffff)||(count&0xffffff)>90)return false;
    std::vector<std::uint32_t> ids(count&0xffffff);
    if(!read(active,ids.data(),ids.size()*4))return false;
    bool seen[90]{};
    for(auto id:ids) {
        NativePlayerActiveState state{};state.id=id;std::uint32_t actual=0;
        if(id>=90||seen[id]||!read(s.mode+0x10+id*8,&state.pointer,8)||!state.pointer||
           !read(state.pointer,&state.vtable,8)||state.vtable<base+0x1a7d000||state.vtable>=base+68988928||
           !read(state.pointer+8,&actual,4)||actual!=id||!read(state.pointer+0x10,&owner,8)||owner!=s.component)return false;
        seen[id]=true;s.active.push_back(state);
    }
    std::uintptr_t again=0;std::uint32_t number=0;
    std::vector<std::uint32_t> finalIds(ids.size());
    if(!read(base+0x39a5290,&again,8)||again!=s.player.game||!read(s.player.game+0x3130b0,&again,8)||again!=s.player.player||
       !read(s.player.player+0x268,&again,8)||again!=list||!read(s.player.player+0x270,&number,4)||number!=componentCount||
       !read(list+index*8,&again,8)||again!=s.component||!read(s.component,&again,8)||again!=base+0x1c73d58||
       !read(s.component+0x10,&again,8)||again!=s.player.player||!read(s.component+0x90,&again,8)||again!=s.mode||
       !read(s.mode,&again,8)||again!=s.component||!read(s.component+0xa8,&again,8)||again!=active||
       !read(s.component+0xb0,&number,4)||number!=count||!read(active,finalIds.data(),finalIds.size()*4)||ids!=finalIds)return false;
    for(const auto& state:s.active) {
        if(!read(s.mode+0x10+state.id*8,&again,8)||again!=state.pointer||!read(state.pointer,&again,8)||again!=state.vtable||
           !read(state.pointer+8,&number,4)||number!=state.id||!read(state.pointer+0x10,&again,8)||again!=s.component)return false;
    }
    NativePlayerStance last{};
    if(!readNativePlayerStance(read,base,last)||last.game!=s.player.game||last.player!=s.player.player||last.physics!=s.player.physics)return false;
    out=s;return true;
}
