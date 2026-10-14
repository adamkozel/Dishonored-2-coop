#pragma once
#include "domino_partner_identity.h"
// Native447250 chooses an actor;4478c0 subsequently installs target+c8 and
// native aim FX. Reject only our current partner result before those writes.
// Leave native sorting, ray queries and unrelated returned actors unchanged.
using NativeDominoPick=std::uintptr_t(*)(std::uintptr_t,unsigned char);
NativeDominoPick realDominoPick=nullptr;
std::atomic<std::uint64_t> dominoPickCalls{0},dominoPartnerConsidered{0},dominoPartnerBlocked{0},dominoPartnerReadFailed{0},dominoOtherAccepted{0};
std::atomic<std::uint64_t> dominoPartnerCacheHit{0},dominoPartnerCacheMiss{0},dominoPartnerReadMaxUs{0};
std::uintptr_t filterDominoPick(std::uintptr_t component,unsigned char ranked){
    ++dominoPickCalls;const auto candidate=realDominoPick(component,ranked); // Exactly one native query.
    const auto avatar=shadowDamageIdentity();
    if(!candidate||candidate!=avatar.actor){if(candidate)++dominoOtherAccepted;return candidate;}
    ++dominoPartnerConsidered;const auto base=reinterpret_cast<std::uintptr_t>(gameImage);const double start=presentationClock();
    thread_local NativeDominoItem cached{};bool blocked=false;
    if(currentDominoPartnerPick(memoryRead,base,avatar,cached,component,candidate)){++dominoPartnerCacheHit;blocked=true;}
    else{
        ++dominoPartnerCacheMiss;cached={};NativeDominoItem discovered{};
        if(readNativeDominoItem(memoryRead,base,discovered,true)&&currentDominoPartnerPick(memoryRead,base,avatar,discovered,component,candidate)){
            cached=std::move(discovered);blocked=true;
        }else ++dominoPartnerReadFailed;
    }
    const auto us=static_cast<std::uint64_t>((std::max)(0.,(presentationClock()-start)*1000.));
    auto maximum=dominoPartnerReadMaxUs.load();while(us>maximum&&!dominoPartnerReadMaxUs.compare_exchange_weak(maximum,us)){}
    if(blocked){++dominoPartnerBlocked;return 0;}
    return candidate;
}
void writeDominoPartnerFilter(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.domino-partner-filter.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;
    fprintf(f,"{\"installed\":%s,\"calls\":%llu,\"considered\":%llu,\"blocked\":%llu,\"readFailed\":%llu,\"otherAccepted\":%llu,\"cacheHit\":%llu,\"cacheMiss\":%llu,\"readMaxUs\":%llu}\n",realDominoPick?"true":"false",dominoPickCalls.load(),dominoPartnerConsidered.load(),dominoPartnerBlocked.load(),dominoPartnerReadFailed.load(),dominoOtherAccepted.load(),dominoPartnerCacheHit.load(),dominoPartnerCacheMiss.load(),dominoPartnerReadMaxUs.load());
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
bool installDominoPartnerFilter(){
    // Exact disk+Ghidra bytes. Five whole non-relative instructions,18bytes:
    // push rbx;push r15;sub rsp,28;mov r15,rcx;mov rcx,[rcx+b8].
    const unsigned char prologue[]={0x40,0x53,0x41,0x57,0x48,0x83,0xec,0x28,0x4c,0x8b,0xf9,0x48,0x8b,0x89,0xb8,0,0,0};
    auto* target=gameImage+0x447250;if(std::memcmp(target,prologue,sizeof(prologue)))return false;
    auto* trampoline=static_cast<unsigned char*>(VirtualAlloc(nullptr,64,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));if(!trampoline)return false;
    std::memcpy(trampoline,prologue,sizeof(prologue));unsigned char jump[14]{0xff,0x25,0,0,0,0};void* resume=target+sizeof(prologue);
    std::memcpy(jump+6,&resume,8);std::memcpy(trampoline+sizeof(prologue),jump,sizeof(jump));
    DWORD old=0,unused=0;if(!VirtualProtect(trampoline,64,PAGE_EXECUTE_READ,&old)){VirtualFree(trampoline,0,MEM_RELEASE);return false;}
    FlushInstructionCache(GetCurrentProcess(),trampoline,64);
    unsigned char patch[sizeof(prologue)];std::memset(patch,0x90,sizeof(patch));void* wrapper=reinterpret_cast<void*>(&filterDominoPick);std::memcpy(jump+6,&wrapper,8);std::memcpy(patch,jump,sizeof(jump));
    if(!VirtualProtect(target,sizeof(patch),PAGE_EXECUTE_READWRITE,&old)){VirtualFree(trampoline,0,MEM_RELEASE);return false;}
    realDominoPick=reinterpret_cast<NativeDominoPick>(trampoline);std::memcpy(target,patch,sizeof(patch));FlushInstructionCache(GetCurrentProcess(),target,sizeof(patch));
    return VirtualProtect(target,sizeof(patch),old,&unused)!=0;
}
