#pragma once
#include "native_player_states.h"
#include "native_player_sword.h"
// Unbound presentation is not an inferred gameplay action. It preserves only
// independently verified default standing/crouching locomotion and equipment.
// No new native state classes, air modes, scene pointers or outcomes are accepted.
inline bool nativePresentationFallbackGround(const NativePlayerStates& s,std::uintptr_t base){
    unsigned motion=99;
    if(s.player.logic!=1||!s.defaultMode||s.player.current>1||
       !nativePlayerMotion(s,base,motion)||motion!=s.player.current)return false;
    unsigned ground=0;
    for(const auto& a:s.active)
        ground+=(s.player.current==0&&a.id==0&&a.vtable==base+0x1c85628)||
                (s.player.current==1&&a.id==6&&a.vtable==base+0x1c84258);
    return ground==1;
}
template<class Read> bool readNativePresentationFallback(Read read,std::uintptr_t base,
        std::uintptr_t game,std::uintptr_t player,unsigned character,unsigned& swordState){
    NativePlayerStates first{},last{};NativePlayerSword sword{},finalSword{};
    const auto valid=[&](const NativePlayerStates& s,const NativePlayerSword& w){
        return nativePresentationFallbackGround(s,base)&&s.player.game==game&&s.player.player==player&&
            w.character==character&&w.sword.game==game&&w.sword.actor==player&&w.sword.logic==1&&
            w.sword.quantity==1&&w.sword.equipped==w.sword.started;
    };
    if(!game||!player||(character!=1&&character!=2)||
       !readNativePlayerStates(read,base,first)||!readNativePlayerSword(read,base,sword)||!valid(first,sword)||
       !readNativePlayerStates(read,base,last)||!readNativePlayerSword(read,base,finalSword)||!valid(last,finalSword)||
       first.component!=last.component||first.mode!=last.mode||first.player.current!=last.player.current||
       first.active.size()!=last.active.size()||!sameSwordEquipmentIdentity(sword.sword,finalSword.sword))return false;
    for(unsigned i=0;i<first.active.size();++i)
        if(first.active[i].id!=last.active[i].id||first.active[i].pointer!=last.active[i].pointer||first.active[i].vtable!=last.active[i].vtable)return false;
    swordState=sword.sword.equipped?2u:1u;return true;
}
