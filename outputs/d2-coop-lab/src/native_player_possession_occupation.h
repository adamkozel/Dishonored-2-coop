#pragma once
#include "native_player_possession_target.h"
#include "native_player_possession_item.h"
#include "native_possession_context.h"
// Presentation observation only: no host NPC state, input, AI or damage call.
// 1214if (#160): handle, when given, receives the possessed body (the bound NPC replica on a partner).
template<class Read> bool readNativeHumanPossessionOccupation(Read read,std::uintptr_t base,const NativePlayerStates& states,std::uint32_t* handle=nullptr){
    if(states.defaultMode||states.active.size()!=1)return false;
    NativePossessionTarget a{},b{};NativePossessionItem item{};
    if(!readNativePossessionTarget(read,base,a)||!a.hasTarget||!nativeHumanPossessionContext(a.player,base,a.state)||
       a.player.component!=states.component||a.player.mode!=states.mode||a.player.player.game!=states.player.game||
       a.player.player.player!=states.player.player||a.player.player.current!=states.player.current||
       a.player.active[0].pointer!=states.active[0].pointer||
       !readNativePossessionItem(read,base,item)||!item.instantiated||item.quantity!=1||!nativeHumanPossessionEquipment(a.state,item.equipped,item.started)||
       item.player.character!=1||item.player.sword.equipped||item.player.sword.started||
       item.player.sword.game!=states.player.game||item.player.sword.actor!=states.player.player||
       !readNativePossessionTarget(read,base,b)||!nativeHumanPossessionContext(b.player,base,b.state))return false;
    const bool same=b.hasTarget&&a.internal==b.internal&&a.target==b.target&&a.entity==b.entity&&a.handle==b.handle&&
        a.generation==b.generation&&a.state==b.state&&a.player.component==b.player.component&&a.player.mode==b.player.mode&&
        a.player.player.game==b.player.player.game&&a.player.player.player==b.player.player.player;
    if(same&&handle)*handle=b.handle;
    return same;
}
