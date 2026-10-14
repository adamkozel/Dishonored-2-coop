#pragma once
#include "native_openable.h"
bool nativeDoorQueueAvailable(){
    const unsigned char start[]={0x48,0x89,0x5c,0x24,0x08,0x48,0x89,0x74,0x24,0x10,0x48,0x89,0x7c,0x24,0x18,0x41,0x56,0x48,0x83,0xec,0x20};
    const unsigned char resume[]={0x48,0x8b,0x92,0x60,0x89,0x1e,0x00,0xe9};
    return !std::memcmp(gameImage+0x74a400,start,sizeof(start))&&!std::memcmp(gameImage+0x7e79a0+7,resume,sizeof(resume));
}
bool queueNativeDoorTransition(const NativeOpenable& s,unsigned target,unsigned side){
    if(!nativeDoorQueueAvailable()||!nativeOpenableTransitionAllowed(s,target,side)||s.current==target)return false;
    const auto parameters=nativeOpenableParameters(s,target,side);
    const auto queued=reinterpret_cast<unsigned char(*)(void*,const unsigned char*)>(gameImage+0x74a400)(reinterpret_cast<void*>(s.component+0x40),parameters.data());
    if(queued){const auto resume=reinterpret_cast<void(*)(void*)>(gameImage+0x7e79a0);resume(reinterpret_cast<void*>(s.animation));resume(reinterpret_cast<void*>(s.component));}
    unsigned pending=~0u;return queued&&memoryRead(s.component+0x4c,&pending,4)&&pending==1;
}
