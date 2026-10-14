#pragma once
#include "usable_native_api.h"
#include "usable_catalog.h"
// Finite developer experiment only. The harness restores the checkpoint.
std::string usableFixtureName;unsigned usableFixtureTarget=~0u;
struct UsableFixture {unsigned calls=0,refused=0,noops=0,target=0,handle=0;int before=-1,after=-1,pending=-1;unsigned flags=0;std::uint64_t epoch=0;std::string name;const char* result="idle";} usableFixture;
void writeUsableFixture(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.usable-fixture.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;const auto& u=usableFixture;
    fprintf(f,"{\"localExperiment\":true,\"calls\":%u,\"refused\":%u,\"noops\":%u,\"target\":%u,\"before\":%d,\"after\":%d,\"pending\":%d,\"flags\":%u,\"handle\":%u,\"epoch\":%llu,\"name\":\"%s\",\"result\":\"%s\"}\n",u.calls,u.refused,u.noops,u.target,u.before,u.after,u.pending,u.flags,u.handle,u.epoch,u.name.c_str(),u.result);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
void serviceUsableFixture(ULONGLONG tick){
    if(usableFixtureTarget==~0u||!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    const auto target=usableFixtureTarget;const auto name=std::move(usableFixtureName);usableFixtureTarget=~0u;auto& u=usableFixture;
    const auto refuse=[&](const char* why){++u.refused;u.result=why;writeUsableFixture();};
    if(!nativeUsableIndexAvailable()||!coopGameplayReady(outgoing)||!coopGameplayReady(remote)||
       !lastReceive||tick-lastReceive>=250||!observerIsolation.ready||!observerIsolation.movementFixture||observerIsolation.until<tick+5000){refuse("finite_native_scope_required");return;}
    // 1123b: any map; the selected usable must be in the structural two-state
    // toggle family (vaults and flagged quest devices stay excluded).
    if(target>1){refuse("toggle_family_required");return;}
    std::uintptr_t list=0;unsigned count=0;
    if(!observedGame||!memoryRead(observedGame+0x601c0,&list,8)||!list||!memoryRead(observedGame+0x601c8,&count,4)||(count&0xffffff)>16384){refuse("registry_unavailable");return;}
    std::vector<unsigned> handles(count&0xffffff);if(!handles.empty()&&!memoryRead(list,handles.data(),handles.size()*4)){refuse("registry_unreadable");return;}
    NativeUsable selected{};unsigned matches=0;
    for(auto h:handles){NativeUsable candidate{};if(readNativeUsable(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,h,candidate)&&candidate.name==name){selected=std::move(candidate);++matches;}}
    if(matches!=1||!nativeUsableToggleFamily(selected)||!nativeUsableTargetValid(selected,target)){refuse("owned_native_toggle_required");return;}
    u.name=name;u.target=target;u.before=selected.current;u.handle=selected.handle;u.epoch=outgoing.worldEpoch;
    if(nativeUsableAtTarget(selected,target)){++u.noops;u.after=selected.current;u.pending=selected.pending;u.flags=selected.flags;u.result="already_at_target";writeUsableFixture();return;}
    ++u.calls;u.result="calling_native_index_request";writeUsableFixture();NativeUsable after{};
    if(requestNativeUsableIndex(selected,target,after)){u.after=after.current;u.pending=after.pending;u.flags=after.flags;u.result="native_index_observed";}
    else u.result="native_result_unconfirmed";
    writeUsableFixture();
}
