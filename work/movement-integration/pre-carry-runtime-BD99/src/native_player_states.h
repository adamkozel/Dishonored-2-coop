#pragma once
#include "native_player_stance.h"
#include <vector>
#include <cstring>

struct NativePlayerActiveState {std::uint32_t id=0;std::uintptr_t pointer=0,vtable=0;};
struct NativePlayerStates {
    NativePlayerStance player;
    std::uintptr_t component=0,mode=0;
    bool defaultMode=false;
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
    std::uintptr_t defaultMode=0;
    if(!read(s.component+0x28,&defaultMode,8))return false;
    s.defaultMode=defaultMode==s.mode;
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
       !read(s.component+0x28,&again,8)||again!=defaultMode||
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
// Exact grounded ranged/equipment overlays. The four-state Draw 65 +
// EquipChange 64/53 overlap was observed during Emily's native crouched
// crossbow draw (ranged-emily-empty-crossbow-cache-d58d-v1, sample 87).
// It changes equipment, not physical stance; weapon/scene ownership remains
// independently required by the ranged gesture reader.
inline bool nativePlayerGroundRangedStates(const NativePlayerStates& s,std::uintptr_t base) {
    if(!s.defaultMode||s.player.current>1||s.active.empty()||s.active.size()>4)return false;
    bool ground=false,draw=false,equip53=false,equip64=false;bool seen[90]{};
    for(const auto& a:s.active){
        if(a.id>=90||seen[a.id])return false;seen[a.id]=true;
        if((s.player.current==0&&a.id==0&&a.vtable==base+0x1c85628)||
           (s.player.current==1&&a.id==6&&a.vtable==base+0x1c84258)){if(ground)return false;ground=true;continue;}
        if(a.id==65&&a.vtable==base+0x1c78a68){draw=true;continue;}
        if((a.id==53||a.id==64)&&a.vtable==base+0x1c79cf8){equip53|=a.id==53;equip64|=a.id==64;continue;}
        if((a.id==56&&a.vtable==base+0x1c849d8)||(a.id==59&&a.vtable==base+0x1c7d608)||
           (a.id==81&&a.vtable==base+0x1c7df88))continue;
        return false;
    }
    return ground&&(draw?(s.active.size()==4&&equip53&&equip64):s.active.size()<=3);
}
// Corvo gun and Emily crossbow air-v1 observations: the same native
// FireWeapon56 can accompany exact Jump2/Fall3 while physics stays standing.
// Corvo gun air-v2 additionally observes Reload59 with both airborne states.
inline bool nativePlayerAirRangedStates(const NativePlayerStates& s,std::uintptr_t base) {
    if(!s.defaultMode||s.player.current!=0||s.active.empty()||s.active.size()>2)return false;
    bool air=false,overlay=false;
    for(const auto& a:s.active){
        if((a.id==2&&a.vtable==base+0x1c85028)||(a.id==3&&a.vtable==base+0x1c845c8)){if(air)return false;air=true;}
        else if((a.id==56&&a.vtable==base+0x1c849d8)||(a.id==59&&a.vtable==base+0x1c7d608)){if(overlay)return false;overlay=true;}
        else return false;
    }
    return air;
}
// Semantic motion enum for the observed default player mode only:
// 0 ground locomotion, 1 sneaking, 2 jumping, 3 falling. Native IDs and
// class tables were independently observed for both Corvo and Emily.
inline bool nativePlayerMotion(const NativePlayerStates& s,std::uintptr_t base,std::uint32_t& motion) {
    // Observed base gun/crossbow fire, reload and equipment overlays retain
    // standing/sneaking native locomotion. Gesture ownership is checked separately.
    // Crouched observations retain Sneaking 6 with the same ranged overlays.
    if(s.active.size()>=2&&nativePlayerGroundRangedStates(s,base)){motion=s.player.current;return true;}
    if(nativePlayerAirRangedStates(s,base)){
        for(const auto& a:s.active)if(a.id==2||a.id==3){motion=a.id;return true;}
    }
    if(!s.defaultMode||s.active.empty()||s.active.size()>2)return false;
    struct Entry {unsigned id;std::uintptr_t table;unsigned motion;};
    bool found=false,overlay=false,equipmentOverlay=false;unsigned resolved=0;
    for(const auto& a:s.active){
        bool known=false;
        // Sprint is still grounded root motion; its distinct presentation
        // travels in the versioned semantic action channel. Observed in
        // native-sprint-observation-v1 on both roles, forward and diagonals.
        for(const auto& e:{Entry{0,0x1c85628,0},Entry{1,0x1c85948,0},Entry{6,0x1c84258,1},Entry{2,0x1c85028,2},Entry{3,0x1c845c8,3}}) {
            if(a.id==e.id&&a.vtable==base+e.table){
                if(found)return false;found=true;known=true;resolved=e.motion;break;
            }
        }
        if(!known){
            // A native Slash/Block can coexist with its movement state. Keep
            // that movement channel valid without guessing from controller input.
            if(overlay||!((a.id==39&&a.vtable==base+0x1c85d28)||(a.id==44&&a.vtable==base+0x1c81a68)||
                (a.id==18&&a.vtable==base+0x1c82b28)||(a.id==63&&a.vtable==base+0x1c80ed8)||
                ((resolved==0||resolved==1)&&a.id==75&&a.vtable==base+0x1c86ab8)||
                ((resolved==0||resolved==1)&&((a.id==53&&a.vtable==base+0x1c79cf8)||(a.id==54&&a.vtable==base+0x1c78a68)||(a.id==81&&a.vtable==base+0x1c7df88)))))return false;
            // Native Windblast observation air-v2 confirms UsePower with
            // both Jump and Fall. Preserve the independent movement state;
            // an owned power scene is still required to publish its gesture.
            overlay=true;equipmentOverlay=a.id==53||a.id==54||a.id==81||a.id==75;
        }
    }
    if(!found||(equipmentOverlay&&resolved>1))return false;motion=resolved;return true;
}
