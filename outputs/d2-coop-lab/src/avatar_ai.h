#pragma once
#include "avatar_ai_ownership.h"
// The owned story-character presentation must not run independent NPC
// decisions while its movement is supplied by the other player. Animation,
// physics and all ordinary world NPCs retain their native update paths.
NativeAiDecisionState avatarAiTarget{};
std::uint64_t avatarAiEpoch=0,avatarAiAcquired=0,avatarAiReleased=0,avatarAiLost=0,avatarAiRefused=0;
bool avatarAiHeld=false;
NativeAiComponentState avatarIdleTarget{};
std::uint64_t avatarIdleEpoch=0,avatarIdleAcquired=0,avatarIdleReleased=0,avatarIdleLost=0,avatarIdleRefused=0;
bool avatarIdleOwned=false,avatarIdleWasEnabled=false;
bool nativeIdleControlMatches() {
    const unsigned char bytes[]={0x40,0x53,0x48,0x83,0xec,0x20,0x0f,0xb7,0x81,0x18,0x01,0x00,0x00};
    return gameImage&&!std::memcmp(gameImage+0x426070,bytes,sizeof(bytes));
}
void releaseAvatarIdle() {
    if(!avatarIdleOwned)return;
    avatarIdleOwned=false;
    NativeAiComponentState now{};std::uintptr_t activeGame=0;
    const bool valid=GetCurrentThreadId()==engineThread.load()&&nativeIdleControlMatches()&&
        avatarIdleEpoch==outgoing.worldEpoch&&observedGame==avatarIdleTarget.game&&
        memoryRead(reinterpret_cast<std::uintptr_t>(gameImage)+0x39a5290,&activeGame,8)&&activeGame==avatarIdleTarget.game&&
        readNativeAiComponent(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,
            avatarIdleTarget.handle,NativeAiKind::IdleBreak,now)&&now.actor==avatarIdleTarget.actor&&
        now.component==avatarIdleTarget.component&&now.manager==avatarIdleTarget.manager;
    if(valid) {
        // Restore only the autoplay bit we changed. LOD/debug bits and the
        // component's own scheduler pause reference remain native-owned.
        if(avatarIdleWasEnabled&&!(now.idleAutoPlay&1))
            reinterpret_cast<void(*)(void*,unsigned short,bool)>(gameImage+0x426070)(reinterpret_cast<void*>(now.component),1,true);
        ++avatarIdleReleased;
    }else ++avatarIdleLost;
    avatarIdleTarget={};avatarIdleEpoch=0;avatarIdleWasEnabled=false;
}
bool acquireAvatarIdle() {
    if(avatarIdleOwned) {
        if(avatarIdleTarget.game==observedGame&&avatarIdleTarget.actor==probeActor&&
           avatarIdleTarget.handle==probeHandle&&avatarIdleEpoch==outgoing.worldEpoch)return true;
        releaseAvatarIdle();
    }
    NativeAiComponentState s{};
    if(GetCurrentThreadId()!=engineThread.load()||!nativeIdleControlMatches()||
       !readNativeAiComponent(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,
          probeHandle,NativeAiKind::IdleBreak,s)||s.actor!=probeActor||(s.flags&0xc)!=8) {
        ++avatarIdleRefused;return false;
    }
    avatarIdleTarget=s;avatarIdleEpoch=outgoing.worldEpoch;avatarIdleWasEnabled=(s.idleAutoPlay&1)!=0;avatarIdleOwned=true;
    // Native 426070 clears the autoplay bit and invokes 425c90, which stops
    // an already playing idle break through its own Midnight/timer APIs.
    if(avatarIdleWasEnabled)
        reinterpret_cast<void(*)(void*,unsigned short,bool)>(gameImage+0x426070)(reinterpret_cast<void*>(s.component),1,false);
    unsigned short flags=0;unsigned state=0;
    if(!memoryRead(s.component+0x118,&flags,2)||(flags&1)||!memoryRead(s.component+0x20,&state,4)||state==3) {
        releaseAvatarIdle();++avatarIdleRefused;return false;
    }
    ++avatarIdleAcquired;return true;
}
void releaseAvatarAi(const char* reason) {
    stopBodyProbe();
    // v109 crash: releasing our Corvo clone on a peer menu resumed its
    // CombatGiveUp task with a null context. Presentation ownership outlives
    // the movement stream. Keep this one reference until local retirement;
    // all ordinary NPCs and invalid/replaced identities retain prior cleanup.
    if(avatarAiHeld&&probeDoppelganger&&
       avatarAiTarget.actor==probeActor&&avatarAiTarget.game==probeGame&&avatarAiTarget.handle==probeHandle&&
       ownedAvatarAiReference(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),
           shadowDamageIdentity(),avatarAiTarget,avatarAiEpoch,outgoing.worldEpoch))return;
    releaseAvatarIdle();
    if(!avatarAiHeld)return;
    avatarAiHeld=false;
    if(releaseNativeAiReference(avatarAiTarget,avatarAiEpoch))++avatarAiReleased;else ++avatarAiLost;
    avatarAiTarget={};avatarAiEpoch=0;
    consoleOutput("COOP: counterpart AI reference ended reason=%s released=%llu lost=%llu\n",reason,avatarAiReleased,avatarAiLost);
}
bool acquireAvatarAi() {
    if(avatarAiHeld) {
        if(avatarAiTarget.game==observedGame&&avatarAiTarget.actor==probeActor&&
           avatarAiTarget.handle==probeHandle&&avatarAiEpoch==outgoing.worldEpoch)return acquireAvatarIdle();
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
    ++avatarAiAcquired;
    if(!acquireAvatarIdle()){releaseAvatarAi("idle_control_refused");return false;}
    return true;
}
