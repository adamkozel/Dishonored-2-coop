#pragma once
// Local developer experiment using the shipped character-choice script's API.
// No network packet can invoke it. Completion is checked by independent reads.
void probeNativeCharacter(int target) {
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    const unsigned char signature[]={0x48,0x89,0x5c,0x24,0x08,0x48,0x89,0x74,0x24,0x10,0x57,0x48,0x83,0xec,0x50,0x8b,0xfa,0x48,0x8b,0xd9};
    NativeCharacterState s{};
    if(localRole!=1||GetCurrentThreadId()!=engineThread.load()||
       !coopGameplayReady(outgoing)||std::strcmp(mapName,kGuardMap)||probeActor||cinematicHold.id||
       guardLink.phase!=GuardPhase::Idle||aiDecisionHold.expires||target<1||target>2||
       std::memcmp(gameImage+0x8a7650,signature,sizeof(signature))||!readNativeCharacter(memoryRead,base,s)||
       s.game!=observedGame||s.player!=observedPlayer||s.queued||s.character==target||
       std::strcmp(s.definitionName,s.variantNames[s.character])) {
        consoleOutput("COOP: native character experiment refused target=%d\n",target);return;
    }
    networkFrameOwned.store(false,std::memory_order_release);phaseFollowInvalidated=true;
    reinterpret_cast<void(*)(void*,int)>(gameImage+0x8a7650)(reinterpret_cast<void*>(s.game),target);
    consoleOutput("COOP: native character experiment queued %d -> %d through shipped script API\n",s.character,target);
}
