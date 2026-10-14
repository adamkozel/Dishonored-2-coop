#pragma once
// Lab-only observer camera for reviewing remote presentation from a host
// window while the host player stays parked. Native render-view setter
// 140890c50(view, position[3], basis[9]) writes view+A8/+C0 position, +CC
// basis and +3EB0 view matrices (cf. IGCS Dishonored 2 notes, same build
// family). "hold" freezes the most active view at its current native pose;
// nothing is synthesized, no gameplay entity or player transform is changed.
// Holds expire after 120 s and on "off". Not network-visible.
struct ObserverViewRecord {std::uintptr_t view=0;float position[3]{},basis[9]{};ULONGLONG tick=0;std::uint64_t count=0;};
using NativeRenderViewSet=void(*)(std::uintptr_t,const float*,const float*);
NativeRenderViewSet realNativeRenderViewSet=nullptr;
const unsigned char kRenderViewSetPrologue[18]={0x48,0x89,0x5c,0x24,0x08,0x48,0x89,0x74,0x24,0x10,0x57,0x48,0x81,0xec,0xf0,0x00,0x00,0x00};
SRWLOCK observerCameraLock=SRWLOCK_INIT;
std::array<ObserverViewRecord,8> observerViews{};
struct ObserverCameraHold {bool active=false;std::uintptr_t view=0;float position[3]{},basis[9]{};ULONGLONG until=0,started=0;std::uint64_t overrides=0;const char* result="off";};
ObserverCameraHold observerCamera{};
int observerCameraRequest=-1;
void observeNativeRenderViewSet(std::uintptr_t view,const float* position,const float* basis){
    const auto tick=GetTickCount64();float p[3]{},b[9]{};bool override=false;
    AcquireSRWLockExclusive(&observerCameraLock);
    if(observerCamera.active&&view==observerCamera.view&&tick<observerCamera.until){
        std::memcpy(p,observerCamera.position,sizeof(p));std::memcpy(b,observerCamera.basis,sizeof(b));override=true;++observerCamera.overrides;
    }else if(position&&basis){
        unsigned slot=0;bool found=false;
        for(unsigned i=0;i<observerViews.size();++i)if(observerViews[i].view==view){slot=i;found=true;break;}
        if(!found){ULONGLONG oldest=~0ull;for(unsigned i=0;i<observerViews.size();++i)if(observerViews[i].tick<oldest){oldest=observerViews[i].tick;slot=i;}observerViews[slot]={};observerViews[slot].view=view;}
        auto& r=observerViews[slot];std::memcpy(r.position,position,12);std::memcpy(r.basis,basis,36);r.tick=tick;++r.count;
    }
    ReleaseSRWLockExclusive(&observerCameraLock);
    realNativeRenderViewSet(view,override?p:position,override?b:basis);
}
bool installObserverCamera(){
    auto* target=gameImage+0x890c50;if(std::memcmp(target,kRenderViewSetPrologue,sizeof(kRenderViewSetPrologue)))return false;
    auto* trampoline=static_cast<unsigned char*>(VirtualAlloc(nullptr,64,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));if(!trampoline)return false;
    std::memcpy(trampoline,kRenderViewSetPrologue,sizeof(kRenderViewSetPrologue));
    unsigned char jump[14]{0xff,0x25,0,0,0,0};void* resume=target+sizeof(kRenderViewSetPrologue);std::memcpy(jump+6,&resume,8);
    std::memcpy(trampoline+sizeof(kRenderViewSetPrologue),jump,sizeof(jump));DWORD old=0,unused=0;
    if(!VirtualProtect(trampoline,64,PAGE_EXECUTE_READ,&old)){VirtualFree(trampoline,0,MEM_RELEASE);return false;}
    FlushInstructionCache(GetCurrentProcess(),trampoline,64);realNativeRenderViewSet=reinterpret_cast<NativeRenderViewSet>(trampoline);
    void* wrapper=reinterpret_cast<void*>(&observeNativeRenderViewSet);std::memcpy(jump+6,&wrapper,8);
    if(!VirtualProtect(target,sizeof(jump),PAGE_EXECUTE_READWRITE,&old))return false;
    std::memcpy(target,jump,sizeof(jump));FlushInstructionCache(GetCurrentProcess(),target,sizeof(jump));
    return VirtualProtect(target,sizeof(jump),old,&unused)!=0;
}
void serviceObserverCamera(ULONGLONG tick){
    const int request=observerCameraRequest;observerCameraRequest=-1;
    AcquireSRWLockExclusive(&observerCameraLock);
    if(request==0){observerCamera.active=false;observerCamera.result="off";}
    else if(request==1){
        // The main rendered view is the one updated most often recently.
        const ObserverViewRecord* best=nullptr;
        for(const auto& r:observerViews)if(r.view&&tick-r.tick<250&&(!best||r.count>best->count))best=&r;
        bool finite=best!=nullptr;
        if(best){for(float f:best->position)finite=finite&&std::isfinite(f);for(float f:best->basis)finite=finite&&std::isfinite(f);}
        if(!finite){observerCamera.active=false;observerCamera.result="no_active_view";}
        else{observerCamera={};observerCamera.active=true;observerCamera.view=best->view;std::memcpy(observerCamera.position,best->position,12);
            std::memcpy(observerCamera.basis,best->basis,36);observerCamera.started=tick;observerCamera.until=tick+120000;observerCamera.result="holding";}
    }
    if(observerCamera.active&&tick>=observerCamera.until){observerCamera.active=false;observerCamera.result="expired";}
    ReleaseSRWLockExclusive(&observerCameraLock);
}
void writeObserverCamera(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.observer-camera.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    ObserverCameraHold h{};std::array<ObserverViewRecord,8> views{};
    AcquireSRWLockShared(&observerCameraLock);h=observerCamera;views=observerViews;ReleaseSRWLockShared(&observerCameraLock);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;
    fprintf(f,"{\"labOnly\":true,\"installed\":%s,\"active\":%s,\"result\":\"%s\",\"view\":\"%llx\",\"overrides\":%llu,\"position\":[%.6f,%.6f,%.6f],\"views\":[",
        realNativeRenderViewSet?"true":"false",h.active?"true":"false",h.result,static_cast<unsigned long long>(h.view),h.overrides,h.position[0],h.position[1],h.position[2]);
    bool first=true;for(const auto& r:views)if(r.view){fprintf(f,"%s{\"view\":\"%llx\",\"count\":%llu,\"tick\":%llu,\"position\":[%.6f,%.6f,%.6f]}",first?"":",",static_cast<unsigned long long>(r.view),r.count,r.tick,r.position[0],r.position[1],r.position[2]);first=false;}
    fputs("]}\n",f);const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
