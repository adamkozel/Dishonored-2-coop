#pragma once
#include "native_ai_state.h"
#include "native_health.h"

// Native death can stop a component and discard its pause references before
// our lease cleanup runs. This is retirement, not a successful native Resume.
// Require the same registered object, stopped scheduler and zero references.
inline bool nativeDeadAiRetirementMatches(const NativeAiComponentState& held,
                                         const NativeAiComponentState& now,const NativeHealth& health) {
    return (held.flags&0xc)==8&&held.game==now.game&&held.actor==now.actor&&
        held.handle==now.handle&&held.kind==now.kind&&held.component==now.component&&
        held.manager==now.manager&&held.registry==now.registry&&held.poolIndex==now.poolIndex&&
        health.game==held.game&&health.actor==held.actor&&health.handle==held.handle&&
        health.logic==1&&health.current==0&&health.maximum>0&&
        !(now.flags&0xc)&&!(now.flags&0x7fffff0)&&!now.schedulerActive;
}
template<class Read> bool readNativeDeadAiRetirement(Read read,std::uintptr_t base,const NativeAiComponentState& held) {
    NativeHealth health{};NativeAiComponentState now{};
    return readNativeHealth(read,base,held.handle,health)&&
        readNativeAiComponent(read,base,held.game,held.handle,held.kind,now)&&
        nativeDeadAiRetirementMatches(held,now,health);
}
