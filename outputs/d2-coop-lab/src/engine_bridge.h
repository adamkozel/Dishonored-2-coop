#pragma once
bool installEngineBridge(unsigned char* image,const char* role);
struct _EXCEPTION_POINTERS;
long handleCoopPoseTrace(_EXCEPTION_POINTERS* event);
// 1214jy (#195): set while memoryRead copies native memory; a fault inside it is the read's own failure, not a crash.
extern thread_local bool coopGuardedRead;
