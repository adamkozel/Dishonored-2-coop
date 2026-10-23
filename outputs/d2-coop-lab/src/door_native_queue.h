#pragma once
#include "native_openable.h"
bool nativeDoorQueueAvailable(){
    const unsigned char start[]={0x48,0x89,0x5c,0x24,0x08,0x48,0x89,0x74,0x24,0x10,0x48,0x89,0x7c,0x24,0x18,0x41,0x56,0x48,0x83,0xec,0x20};
    const unsigned char resume[]={0x48,0x8b,0x92,0x60,0x89,0x1e,0x00,0xe9};
    return !std::memcmp(gameImage+0x74a400,start,sizeof(start))&&!std::memcmp(gameImage+0x7e79a0+7,resume,sizeof(resume));
}
// 1214jf (#185): the native lock routine 0x14074e4c0 (component, lock, recurse into the linked door). The key branch of
// the interaction state routine 0x14074c5c0 calls it with (component, 0, 0) after its key check; the other world replays
// that call for the partner's key. It writes the lock byte (+0x1e8), emits the native unlock event and dirties the owner
// and visual child (chapter 47, decompile 800). Linked doors stay excluded, so it never recurses.
bool nativeDoorUnlockAvailable(){
    const unsigned char start[]={0x48,0x8b,0xc4,0x48,0x89,0x70,0x18,0x57,0x48,0x83,0xec,0x70,0x48,0x89,0x68,0x10,0x0f,0xb6,0xf2,0x48,0x8b,0xf9,0x41,0x0f,0xb6,0xe8,0x3a,0x91,0xe8,0x01,0x00,0x00};
    return !std::memcmp(gameImage+0x74e4c0,start,sizeof(start));
}
bool unlockNativeDoor(const NativeOpenable& s){
    if(!nativeDoorUnlockAvailable()||!s.locked||s.blocked||s.broken||s.linked)return false;
    reinterpret_cast<void(*)(void*,char,char)>(gameImage+0x74e4c0)(reinterpret_cast<void*>(s.component),0,0);
    unsigned char locked=1;return memoryRead(s.component+0x1e8,&locked,1)&&!locked;
}
bool queueNativeDoorTransition(const NativeOpenable& s,unsigned target,unsigned side){
    if(!nativeDoorQueueAvailable()||!nativeOpenableTransitionAllowed(s,target,side)||s.current==target)return false;
    const auto parameters=nativeOpenableParameters(s,target,side);
    const auto queued=reinterpret_cast<unsigned char(*)(void*,const unsigned char*)>(gameImage+0x74a400)(reinterpret_cast<void*>(s.component+0x40),parameters.data());
    if(queued){const auto resume=reinterpret_cast<void(*)(void*)>(gameImage+0x7e79a0);resume(reinterpret_cast<void*>(s.animation));resume(reinterpret_cast<void*>(s.component));}
    unsigned pending=~0u;return queued&&memoryRead(s.component+0x4c,&pending,4)&&pending==1;
}
