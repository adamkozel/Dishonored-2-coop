#pragma once
#include "native_openable.h"
// One reversible native API experiment. Checkpoint restoration owns cleanup.
unsigned doorFixtureRequest=~0u;
struct DoorFixture {unsigned calls=0,refused=0,noops=0,target=0,before=0,after=0,handle=0;std::uint64_t epoch=0;const char* result="idle";} doorFixture;
void writeDoorFixture(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.door-fixture.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;const auto& d=doorFixture;
    fprintf(f,"{\"localExperiment\":true,\"calls\":%u,\"refused\":%u,\"noops\":%u,\"target\":%u,\"before\":%u,\"pendingAfter\":%u,\"handle\":%u,\"epoch\":%llu,\"result\":\"%s\"}\n",d.calls,d.refused,d.noops,d.target,d.before,d.after,d.handle,d.epoch,d.result);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
void serviceDoorFixture(ULONGLONG tick){
    if(doorFixtureRequest==~0u||!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    const auto target=doorFixtureRequest;doorFixtureRequest=~0u;auto& d=doorFixture;
    const auto refuse=[&](const char* reason){++d.refused;d.result=reason;writeDoorFixture();};
    const unsigned char prologue[]={0x48,0x89,0x5c,0x24,0x08,0x48,0x89,0x74,0x24,0x10,0x48,0x89,0x7c,0x24,0x18,0x41,0x56,0x48,0x83,0xec,0x20};
    const unsigned char resume[]={0x48,0x8b,0x92,0x60,0x89,0x1e,0x00,0xe9};
    if(std::memcmp(gameImage+0x74a400,prologue,sizeof(prologue))||std::memcmp(gameImage+0x7e79a0+7,resume,sizeof(resume))||std::strcmp(mapName,kGuardMap)||
       !coopGameplayReady(outgoing)||!coopGameplayReady(remote)||!lastReceive||tick-lastReceive>=250||
       !observerIsolation.ready||!observerIsolation.movementFixture||observerIsolation.until<tick+5000){refuse("native_scope_required");return;}
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t list=0;unsigned count=0;
    if(!memoryRead(observedGame+0x601c0,&list,8)||!list||!memoryRead(observedGame+0x601c8,&count,4)||(count&0xffffff)>16384){refuse("registry_unavailable");return;}
    std::vector<unsigned> handles(count&0xffffff);if(!handles.empty()&&!memoryRead(list,handles.data(),handles.size()*4)){refuse("registry_unreadable");return;}
    NativeOpenable selected{};unsigned matches=0;
    for(auto handle:handles){NativeOpenable s{};if(readNativeOpenable(memoryRead,base,observedGame,handle,s)&&s.name=="ld_door_350cm_int_rich_wood_01_3"){selected=std::move(s);++matches;}}
    const unsigned side=target==2?0:selected.openSide;
    if(matches!=1||!nativeOpenableTransitionAllowed(selected,target,side)){refuse("exact_unlocked_unlinked_door_required");return;}
    d.target=target;d.before=selected.current;d.handle=selected.handle;d.epoch=outgoing.worldEpoch;
    if(selected.current==target){++d.noops;d.result="already_at_target";writeDoorFixture();return;}
    NativeOpenable fresh{};
    if(!readNativeOpenable(memoryRead,base,observedGame,selected.handle,fresh)||fresh.component!=selected.component||fresh.generation!=selected.generation||!nativeOpenableTransitionAllowed(fresh,target,side)){refuse("native_identity_changed");return;}
    // Do not use74d7a0 for a fresh stable door: it first finalizes stale
    // Moving state, whose saved target is initially -1 (711 first-open bug).
    // Queue Moving through the same native template used by ordinary use.
    const auto parameters=nativeOpenableParameters(fresh,target,side);
    ++d.calls;d.result="calling_native_transition";writeDoorFixture();
    const auto queued=reinterpret_cast<unsigned char(*)(void*,const unsigned char*)>(gameImage+0x74a400)(reinterpret_cast<void*>(fresh.component+0x40),parameters.data());
    if(queued){
        const auto resumeComponent=reinterpret_cast<void(*)(void*)>(gameImage+0x7e79a0);
        resumeComponent(reinterpret_cast<void*>(fresh.animation));resumeComponent(reinterpret_cast<void*>(fresh.component));
    }
    unsigned pending=~0u;if(memoryRead(fresh.component+0x4c,&pending,4))d.after=pending;
    d.result=queued&&pending==1?"native_transition_queued":"native_result_unconfirmed";writeDoorFixture();
}
