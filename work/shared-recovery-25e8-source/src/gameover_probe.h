#pragma once
std::uint64_t gameOverProbes=0,gameOverRetries=0,gameOverRefused=0;
void probeGameOver(bool retry) {
    NativeGameOverState s{};
    if(GetCurrentThreadId()!=engineThread.load()||std::strcmp(mapName,kGuardMap)||
       !readNativeGameOver(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),s)||s.pending||
       guardLink.phase!=GuardPhase::Idle||aiDecisionHold.expires||cinematicHold.id||loadingProbe.armed||loadingProbe.held||
       checkpointSync.phase!=CheckpointPhase::Idle){++gameOverRefused;return;}
    if(!retry) {
        if(!coopGameplayReady(outgoing)||s.logic!=1){++gameOverRefused;return;}
        ++gameOverProbes;realLogicSwitch(reinterpret_cast<void*>(s.manager),13);
    }else {
        const unsigned char expected[]={0x48,0x83,0xec,0x28,0xe8,0x87,0x6d,0x29,0x00,0x48,0x8b,0xc8,0x48,0x83,0xc4,0x28,0xe9,0xbb,0x3c,0xf7,0xff};
        if(s.logic!=13||!s.flash||std::memcmp(gameImage+0x4f2980,expected,sizeof(expected))){++gameOverRefused;return;}
        ++gameOverRetries;
        reinterpret_cast<void(*)(void*)>(gameImage+0x4f2980)(reinterpret_cast<void*>(s.state));
    }
    consoleOutput("COOP: native game-over probe retry=%u state=%llx flash=%llx show=%llx/%llx\n",retry?1:0,s.state,s.flash,s.showFunction[0],s.showFunction[1]);
}
