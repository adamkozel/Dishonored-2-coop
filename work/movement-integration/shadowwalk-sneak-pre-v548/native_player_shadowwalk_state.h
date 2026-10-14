#pragma once
#include "native_player_shadowwalk_item.h"
#include "native_player_states.h"
struct NativeShadowWalkState {
    NativeShadowWalkItem item{};NativePlayerStates states{};
    unsigned phase=0;bool form=false;
};
// Native516b: phase2 preparation,3 transition-in,4 form,5 transition-out,
// then0 equipment recovery. These are observed correlations, not a complete
// enum. State slot0 belongs to ShadowWalk only in its alternate native mode.
inline bool nativeShadowWalkStateContext(const NativePlayerStates& s,std::uintptr_t base,unsigned phase){
    if(s.player.logic!=1||s.active.empty()||s.active.size()>2)return false;
    if(!s.defaultMode){
        if(s.active.size()!=1||s.active[0].id!=0||s.active[0].vtable!=base+0x1c7ed28)return false;
        return (s.player.current==2&&(phase==3||phase==4||phase==5))||
               (s.player.current==1&&(phase==0||phase==5));
    }
    if((s.player.current==0&&(phase==0||phase==2||phase==3))||
       (s.player.current==1&&phase==0)){
        unsigned ground=0,equipment=0;
        for(const auto& a:s.active){
            if((s.player.current==0&&a.id==0&&a.vtable==base+0x1c85628)||
               (s.player.current==1&&a.id==6&&a.vtable==base+0x1c84258))++ground;
            else if(a.id==53&&a.vtable==base+0x1c79cf8)++equipment;
            else return false;
        }
        return ground==1&&equipment<=1;
    }
    return false;
}
template<class Read> bool readNativeShadowWalkStateFrame(Read read,std::uintptr_t base,NativeShadowWalkState& s){
    if(!readNativeShadowWalkItem(read,base,s.item,true)||!s.item.started||!s.item.instantiated||
       !readNativePlayerStates(read,base,s.states)||s.item.player.sword.game!=s.states.player.game||
       s.item.player.sword.actor!=s.states.player.player)return false;
    std::uintptr_t machine=0;
    if(!read(s.item.component+0xc0,&machine,8)||machine!=s.states.component||
       !read(s.item.component+0xe4,&s.phase,4)||!nativeShadowWalkStateContext(s.states,base,s.phase))return false;
    s.form=!s.states.defaultMode;return true;
}
template<class Read> bool readNativeShadowWalkState(Read read,std::uintptr_t base,NativeShadowWalkState& out){
    NativeShadowWalkState a{},b{};
    if(!readNativeShadowWalkStateFrame(read,base,a)||!readNativeShadowWalkStateFrame(read,base,b)||
       a.phase!=b.phase||a.form!=b.form||a.item.entry!=b.item.entry||a.item.handle!=b.item.handle||
       a.item.component!=b.item.component||a.item.entity!=b.item.entity||a.item.equipmentDeclaration!=b.item.equipmentDeclaration||
       a.item.parentDeclaration!=b.item.parentDeclaration||!sameSwordEquipmentIdentity(a.item.player.sword,b.item.player.sword)||
       a.states.component!=b.states.component||a.states.mode!=b.states.mode||a.states.player.physics!=b.states.player.physics||
       a.states.player.current!=b.states.player.current||a.states.active.size()!=b.states.active.size())return false;
    for(unsigned i=0;i<a.states.active.size();++i){const auto& x=a.states.active[i];const auto& y=b.states.active[i];
        if(x.id!=y.id||x.pointer!=y.pointer||x.vtable!=y.vtable)return false;
    }
    out=std::move(b);return true;
}
