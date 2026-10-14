#pragma once
#include "checkpoint_protocol.h"
// Native save I/O can defer LoadGame itself before it caches the slot and
// schedules RestartFromMemoryCheckpoint. A repeated fixed-slot command is
// part of the existing attempt only while its original world is still live.
inline bool checkpointFixedSlotRetry(bool enabled,bool permitted,bool expected,bool loadingEntered,
        CheckpointPhase phase,unsigned kind,int slot,std::uintptr_t oldGame,std::uintptr_t currentGame,bool inTime){
    return enabled&&permitted&&expected&&!loadingEntered&&inTime&&kind==2&&slot==82&&oldGame&&oldGame==currentGame&&
        (phase==CheckpointPhase::Loading||phase==CheckpointPhase::Failed);
}
// The native pending GameOver state can be sampled before the checkpoint
// protocol observes committed GameOver and broadcasts Death. Retire world
// bindings immediately but retain session intent across that ordering gap.
// This never authorizes gameplay, a native load, or reuse of old bindings.
inline bool checkpointBlocksWorldActions(bool enabled,CheckpointPhase phase,bool deathActive,
        unsigned localState,unsigned localPending,unsigned peerState,unsigned peerPending){
    if(!enabled||phase==CheckpointPhase::Failed)return false;
    return deathActive||phase!=CheckpointPhase::Idle||localState==13||localPending==13||peerState==13||peerPending==13;
}
