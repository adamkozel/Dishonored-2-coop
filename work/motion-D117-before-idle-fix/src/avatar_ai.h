#pragma once
// The owned story-character presentation must not run independent NPC
// decisions while its movement is supplied by the other player. Animation,
// physics and all ordinary world NPCs retain their native update paths.
NativeAiDecisionState avatarAiTarget{};
std::uint64_t avatarAiEpoch=0,avatarAiAcquired=0,avatarAiReleased=0,avatarAiLost=0,avatarAiRefused=0;
bool avatarAiHeld=false;
void releaseAvatarAi(const char* reason) {
    if(!avatarAiHeld)return;
    avatarAiHeld=false;
    if(releaseNativeAiReference(avatarAiTarget,avatarAiEpoch))++avatarAiReleased;else ++avatarAiLost;
    avatarAiTarget={};avatarAiEpoch=0;
    consoleOutput("COOP: counterpart AI reference ended reason=%s released=%llu lost=%llu\n",reason,avatarAiReleased,avatarAiLost);
}
bool acquireAvatarAi() {
    if(avatarAiHeld) {
        if(avatarAiTarget.game==observedGame&&avatarAiTarget.actor==probeActor&&
           avatarAiTarget.handle==probeHandle&&avatarAiEpoch==outgoing.worldEpoch)return true;
        releaseAvatarAi("binding_changed");
    }
    NativeAiDecisionState s{};
    if(GetCurrentThreadId()!=engineThread.load()||!nativeAiRequestsMatch()||!probeActor||probeGame!=observedGame||
       !readNativeAiDecision(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,probeHandle,s)||
       s.actor!=probeActor||!s.tree||(s.flags&0xc)!=8||(s.flags&0x7fffff0)||!s.schedulerActive) {
        ++avatarAiRefused;return false;
    }
    avatarAiTarget=s;avatarAiEpoch=outgoing.worldEpoch;avatarAiHeld=true;
    reinterpret_cast<void(*)(void*,void*)>(gameImage+0x7e7800)(reinterpret_cast<void*>(s.component),reinterpret_cast<void*>(s.manager));
    unsigned flags=0;
    if(!memoryRead(s.component+0x1c,&flags,4)||((flags>>6)&0x1fffff)!=1) {
        releaseAvatarAi("pause_not_verified");++avatarAiRefused;return false;
    }
    ++avatarAiAcquired;return true;
}
