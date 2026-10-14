#pragma once
#include "native_player_states.h"
//335/370: internal0 release and internal1 transfer retain the original
// standing/crouched physics0/1 with exact EnterPossessedHost26.
inline bool nativeHumanPossessionEntryContext(const NativePlayerStates& s,std::uintptr_t base){
    return s.player.logic==1&&!s.defaultMode&&s.player.current<=1&&s.active.size()==1&&
        s.active[0].id==26&&s.active[0].vtable==base+0x1c79878;
}
//335: internal1 is transfer with EnterPossessedHost26;
// internal8 is occupied human host with Running0/physics18. Neither is
// ordinary player locomotion. Unknown target types and exit states stay out.
inline bool nativeHumanPossessionContext(const NativePlayerStates& s,std::uintptr_t base,unsigned internal){
    if(s.player.logic!=1||s.defaultMode||s.active.size()!=1)return false;
    const auto& a=s.active[0];
    return (internal==1&&nativeHumanPossessionEntryContext(s,base))||
        (internal==8&&s.player.current==18&&a.id==0&&a.vtable==base+0x1c85628);
}
//337 runtime: native controller restarts the unselected power component
// once human occupation is established. Transfer retains the stopped item.
inline bool nativeHumanPossessionEquipment(unsigned internal,bool equipped,bool started){
    return !equipped&&((internal==1&&!started)||(internal==8&&started));
}
