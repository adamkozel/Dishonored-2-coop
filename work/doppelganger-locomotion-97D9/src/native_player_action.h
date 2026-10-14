#pragma once
#include "native_player_states.h"
// Observed on both roles in B671F824: Slash 39 / VT1c85d28 and
// Block 44 / VT1c81a68, alongside ordinary Running. Unknown states fail closed.
inline bool nativePlayerSwordAction(const NativePlayerStates& s,std::uintptr_t base,unsigned& action){
    if(!s.defaultMode||s.active.empty()||s.active.size()>2)return false;
    bool motion=false,slash=false,block=false;
    for(const auto& a:s.active){
        if(a.id==39&&a.vtable==base+0x1c85d28){if(slash)return false;slash=true;}
        else if(a.id==44&&a.vtable==base+0x1c81a68){if(block)return false;block=true;}
        else {
            bool known=false;
            for(const auto& e:{NativePlayerActiveState{0,0,0x1c85628},NativePlayerActiveState{6,0,0x1c84258},
                 NativePlayerActiveState{2,0,0x1c85028},NativePlayerActiveState{3,0,0x1c845c8}})
                if(a.id==e.id&&a.vtable==base+e.vtable)known=true;
            if(!known||motion)return false;motion=true;
        }
    }
    if(!motion||(slash&&block))return false;
    action=slash?1u:block?2u:0u;return true;
}
