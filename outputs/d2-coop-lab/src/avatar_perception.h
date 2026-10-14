#pragma once
// Exact-executable experiment: native SetTransform treats each call as a
// teleport and refreshes observable membership (1BF/1BE) even in one area.
// Network interpolation instead supplies continuous movement. Retain the
// owned clone's membership only inside that validated, same-area callback.
// Area changes use native875fc0, outside this hook. Large jumps still reset.
using NativeObservableRefresh=void(*)(void*);
NativeObservableRefresh realObservableRefresh=nullptr;
bool avatarPerceptionEnabled=false;
thread_local std::uintptr_t continuousAvatarTransformOwner=0;
std::atomic<std::uint64_t> avatarObservableRetained{0},avatarObservableForwarded{0},avatarObservableRefused{0};
std::uint64_t avatarContinuousTransforms=0,avatarTeleportTransforms=0;
void refreshObservableForTransform(void* observable){
    if(continuousAvatarTransformOwner){
        const auto p=reinterpret_cast<std::uintptr_t>(observable);
        std::uintptr_t owner=0,vt=0;
        if(memoryRead(p,&vt,8)&&vt==reinterpret_cast<std::uintptr_t>(gameImage)+0x1b98078&&
           memoryRead(p+0x10,&owner,8)&&owner==continuousAvatarTransformOwner){
            ++avatarObservableRetained;return;
        }
        ++avatarObservableRefused;
    }
    ++avatarObservableForwarded;realObservableRefresh(observable);
}
void dispatchAvatarTransform(const float* position,const float* orientation){
    float before[3]{};double distance2=0;
    bool continuous=avatarPerceptionEnabled&&probeDoppelganger&&avatarAiHeld&&
        GetCurrentThreadId()==engineThread.load()&&probeGame==observedGame&&
        memoryRead(probeActor+0x300,before,sizeof(before));
    for(unsigned i=0;i<3&&continuous;++i){
        const double delta=double(position[i])-before[i];
        continuous=std::isfinite(delta);distance2+=delta*delta;
    }
    continuous=continuous&&distance2<=9.;
    if(continuous)++avatarContinuousTransforms;else ++avatarTeleportTransforms;
    struct Scope {
        std::uintptr_t previous;
        explicit Scope(std::uintptr_t owner):previous(continuousAvatarTransformOwner){continuousAvatarTransformOwner=owner;}
        ~Scope(){continuousAvatarTransformOwner=previous;}
    } scope(continuous?probeActor:0);
    reinterpret_cast<void(*)(void*,const float*,const float*,bool,int)>(gameImage+0x87d200)(
        reinterpret_cast<void*>(probeActor),position,orientation,true,0);
}
void writeAvatarPerception(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};
    std::snprintf(path,sizeof(path),"%s.avatar-perception.json",statusPath);
    std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;
    fprintf(f,"{\"experimental\":true,\"installed\":%s,\"enabled\":%s,\"retained\":%llu,\"forwarded\":%llu,\"refused\":%llu,\"continuousTransforms\":%llu,\"teleportTransforms\":%llu,\"avatarAiHeld\":%s,\"avatarHandle\":%u}\n",
        realObservableRefresh?"true":"false",avatarPerceptionEnabled?"true":"false",avatarObservableRetained.load(),avatarObservableForwarded.load(),avatarObservableRefused.load(),avatarContinuousTransforms,avatarTeleportTransforms,avatarAiHeld?"true":"false",probeHandle);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
bool installAvatarPerception(){
    // Exact19byte prologue. First13bytes have no PC-relative operands. The
    // following JLE is relocated as JG over an absolute jump to native exit.
    const unsigned char prologue[]={0x40,0x57,0x48,0x83,0xec,0x20,0x83,0x79,0x40,0,0x48,0x8b,0xf9,0x0f,0x8e,0x9e,0,0,0};
    auto* target=gameImage+0x600220;if(std::memcmp(target,prologue,sizeof(prologue)))return false;
    auto* trampoline=static_cast<unsigned char*>(VirtualAlloc(nullptr,64,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));if(!trampoline)return false;
    std::memcpy(trampoline,prologue,13);trampoline[13]=0x7f;trampoline[14]=14;
    unsigned char jump[14]{0xff,0x25,0,0,0,0};void* destination=gameImage+0x6002d1;
    std::memcpy(jump+6,&destination,8);std::memcpy(trampoline+15,jump,14);
    destination=target+sizeof(prologue);std::memcpy(jump+6,&destination,8);std::memcpy(trampoline+29,jump,14);
    DWORD old=0,unused=0;if(!VirtualProtect(trampoline,64,PAGE_EXECUTE_READ,&old)){VirtualFree(trampoline,0,MEM_RELEASE);return false;}
    FlushInstructionCache(GetCurrentProcess(),trampoline,64);
    unsigned char patch[sizeof(prologue)];std::memset(patch,0x90,sizeof(patch));
    destination=reinterpret_cast<void*>(&refreshObservableForTransform);std::memcpy(jump+6,&destination,8);std::memcpy(patch,jump,14);
    if(!VirtualProtect(target,sizeof(patch),PAGE_EXECUTE_READWRITE,&old)){VirtualFree(trampoline,0,MEM_RELEASE);return false;}
    realObservableRefresh=reinterpret_cast<NativeObservableRefresh>(trampoline);
    std::memcpy(target,patch,sizeof(patch));FlushInstructionCache(GetCurrentProcess(),target,sizeof(patch));
    return VirtualProtect(target,sizeof(patch),old,&unused)!=0;
}
