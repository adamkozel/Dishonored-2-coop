#pragma once
#include "possession_partner_identity.h"
using NativePossessionPick=std::uintptr_t(*)(std::uintptr_t,unsigned char,unsigned char,unsigned char);
NativePossessionPick realPossessionPick=nullptr;
std::atomic<std::uint64_t> possessionPickCalls{0},possessionPartnerConsidered{0},possessionPartnerBlocked{0},possessionPartnerReadFailed{0},possessionOtherAccepted{0},possessionPartnerReadMaxUs{0};
std::uintptr_t filterPossessionPick(std::uintptr_t mode,unsigned char ranked,unsigned char human,unsigned char corpse){
    ++possessionPickCalls;
    const auto candidate=realPossessionPick(mode,ranked,human,corpse); // Exactly one unchanged native query.
    const auto avatar=shadowDamageIdentity();unsigned handle=0;
    if(!candidate||!avatar.actor||!memoryRead(candidate+0x18,&handle,4)||handle!=avatar.handle){
        if(candidate)++possessionOtherAccepted;return candidate;
    }
    ++possessionPartnerConsidered;const double start=presentationClock();
    const bool blocked=currentPossessionPartnerPick(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),avatar,mode,candidate);
    const auto us=static_cast<std::uint64_t>((std::max)(0.,(presentationClock()-start)*1000.));
    auto maximum=possessionPartnerReadMaxUs.load();while(us>maximum&&!possessionPartnerReadMaxUs.compare_exchange_weak(maximum,us)){}
    if(blocked){++possessionPartnerBlocked;return 0;}
    ++possessionPartnerReadFailed;return candidate;
}
void writePossessionPartnerFilter(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.possession-partner-filter.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;
    fprintf(f,"{\"installed\":%s,\"calls\":%llu,\"considered\":%llu,\"blocked\":%llu,\"readFailed\":%llu,\"otherAccepted\":%llu,\"readMaxUs\":%llu}\n",realPossessionPick?"true":"false",possessionPickCalls.load(),possessionPartnerConsidered.load(),possessionPartnerBlocked.load(),possessionPartnerReadFailed.load(),possessionOtherAccepted.load(),possessionPartnerReadMaxUs.load());
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
bool installPossessionPartnerFilter(){
    // Exact image3c22f0: three whole non-relative instructions,14bytes.
    // mov [rsp+18],rbx; mov [rsp+20],r9b; mov [rsp+10],dl.
    const unsigned char prologue[]={0x48,0x89,0x5c,0x24,0x18,0x44,0x88,0x4c,0x24,0x20,0x88,0x54,0x24,0x10};
    auto* target=gameImage+0x3c22f0;if(std::memcmp(target,prologue,sizeof(prologue)))return false;
    auto* trampoline=static_cast<unsigned char*>(VirtualAlloc(nullptr,64,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));if(!trampoline)return false;
    std::memcpy(trampoline,prologue,sizeof(prologue));unsigned char jump[14]{0xff,0x25,0,0,0,0};void* resume=target+sizeof(prologue);
    std::memcpy(jump+6,&resume,8);std::memcpy(trampoline+sizeof(prologue),jump,sizeof(jump));
    DWORD old=0,unused=0;if(!VirtualProtect(trampoline,64,PAGE_EXECUTE_READ,&old)){VirtualFree(trampoline,0,MEM_RELEASE);return false;}
    FlushInstructionCache(GetCurrentProcess(),trampoline,64);
    void* wrapper=reinterpret_cast<void*>(&filterPossessionPick);std::memcpy(jump+6,&wrapper,8);
    if(!VirtualProtect(target,sizeof(jump),PAGE_EXECUTE_READWRITE,&old)){VirtualFree(trampoline,0,MEM_RELEASE);return false;}
    realPossessionPick=reinterpret_cast<NativePossessionPick>(trampoline);std::memcpy(target,jump,sizeof(jump));FlushInstructionCache(GetCurrentProcess(),target,sizeof(jump));
    return VirtualProtect(target,sizeof(jump),old,&unused)!=0;
}
