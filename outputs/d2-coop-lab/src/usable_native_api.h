#pragma once
#include "native_usable.h"
inline bool nativeUsableIndexAvailable(){
    const unsigned char prefix[]={0x85,0xd2,0x78,0x75,0x53,0x48,0x83,0xec,0x20,0x48,0x8b,0x41,0x08,0x48,0x8b,0xd9,0x8b,0x48,0x78,0x81,0xe1,0xff,0xff,0xff,0x00,0x3b,0xd1,0x7d,0x57,0x45,0x84,0xc0};
    return !std::memcmp(gameImage+0x77b920,prefix,sizeof(prefix));
}
// Native indexed request preserves transition queuing, animation, collision
// servicing and state notifications. Callers own gameplay/session eligibility.
bool requestNativeUsableIndex(const NativeUsable& binding,unsigned target,NativeUsable& after){
    if(!nativeUsableIndexAvailable()||!nativeUsableTargetValid(binding,target))return false;
    NativeUsable fresh{};
    if(!readNativeUsable(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,binding.handle,fresh)||
       !sameNativeUsableIdentity(binding,fresh)||!nativeUsableTargetValid(fresh,target))return false;
    reinterpret_cast<void(*)(void*,int,unsigned char)>(gameImage+0x77b920)(reinterpret_cast<void*>(fresh.component),static_cast<int>(target),0);
    return readNativeUsable(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,binding.handle,after)&&
        sameNativeUsableIdentity(fresh,after)&&(after.current==static_cast<int>(target)||after.pending==static_cast<int>(target));
}
