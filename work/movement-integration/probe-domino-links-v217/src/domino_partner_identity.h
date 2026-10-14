#pragma once
#include "friendly_fire.h"
#include "native_domino_equipment_owner.h"
// Filter the native aim pick before it becomes equipment+c8 or a target FX.
// A cached item is only a hint; all current ownership is revalidated.
template<class Read> bool currentDominoPartnerPick(Read read,std::uintptr_t base,const CoopDamageAvatar& avatar,
    const NativeDominoItem& known,std::uintptr_t component,std::uintptr_t candidate){
    if(!candidate||candidate!=avatar.actor||!component||component!=known.component||
       known.player.sword.game!=avatar.game||!validCoopDamageAvatar(read,base,avatar))return false;
    NativeDominoItem current{};
    return readCurrentNativeDominoEquipment(read,base,known,current)&&current.component==component&&
        current.equipped&&current.started&&current.quantity==1&&validCoopDamageAvatar(read,base,avatar);
}
