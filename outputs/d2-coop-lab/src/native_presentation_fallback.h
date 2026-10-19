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
// 1054d: a native state that no binding classifies (and that is not default
// ground locomotion) must not starve the action stream into permanent
// retirement. The hold proves only the same live player in gameplay with a
// stable state list and equipment; it carries no action, so the receiver
// presents nothing authored (bank weight 0, network-owned root).
template<class Read> bool readNativePresentationHold(Read read,std::uintptr_t base,
        std::uintptr_t game,std::uintptr_t player,unsigned character,unsigned& swordState,unsigned lastSword=0){
    NativePlayerStates first{},last{};NativePlayerSword sword{},finalSword{};
    const auto valid=[&](const NativePlayerStates& s,const NativePlayerSword& w){
        return s.player.logic==1&&s.player.game==game&&s.player.player==player&&
            w.character==character&&w.sword.game==game&&w.sword.actor==player&&w.sword.logic==1&&
            w.sword.quantity==1&&w.sword.equipped==w.sword.started;
    };
    if(!game||!player||(character!=1&&character!=2))return false;
    // 1074: the native item wheel is logic8 (confirmed 1073). Native health and
    // sword readers refuse it, so the hold carries the last equipment state the
    // source validated in gameplay, after two stable reads of the same player.
    // 1080: while the wheel opens the native logic stack is 1 with pending 8
    // and the health/sword readers refuse the pending transition; the same
    // proof (two stable reads of the same live player) applies to logic 1 or 8.
    // 1085: while the wheel is open the player state machine is not readable
    // (states read fails, stance read succeeds with logic8), so the identity
    // proof is two stable stance reads of the same live player and physics.
    NativePlayerStance wheelFirst{},wheelLast{};
    const auto wheelHold=[&](){return (lastSword==1||lastSword==2)&&readNativePlayerStance(read,base,wheelFirst,true)&&(wheelFirst.logic==8||wheelFirst.logic==10||wheelFirst.logic==1)&&
       wheelFirst.game==game&&wheelFirst.player==player&&readNativePlayerStance(read,base,wheelLast,true)&&wheelLast.logic==wheelFirst.logic&&
       wheelLast.game==game&&wheelLast.player==player&&wheelLast.physics==wheelFirst.physics;};
    // 1214dr: the black market (logic 10) holds the same way: the stream keeps the partner on its replicated root, so it
    // walks to the counter and stands there instead of vanishing for the whole shop (1399a).
    if(wheelHold()&&(wheelFirst.logic==8||wheelFirst.logic==10)){swordState=lastSword;return true;}
    if(!readNativePlayerStates(read,base,first)||!readNativePlayerSword(read,base,sword)||!valid(first,sword)||
       !readNativePlayerStates(read,base,last)||!readNativePlayerSword(read,base,finalSword)||!valid(last,finalSword)||
       first.component!=last.component||first.mode!=last.mode||first.active.size()!=last.active.size()||
       !sameSwordEquipmentIdentity(sword.sword,finalSword.sword)){if(wheelHold()){swordState=lastSword;return true;}return false;}
    for(unsigned i=0;i<first.active.size();++i)
        if(first.active[i].id!=last.active[i].id||first.active[i].pointer!=last.active[i].pointer||first.active[i].vtable!=last.active[i].vtable)return false;
    swordState=sword.sword.equipped?2u:1u;return true;
}
