#pragma once
#include "assassination_partner_identity.h"
using NativeAssassinationEligibility=unsigned char(*)(std::uintptr_t,std::uintptr_t);
NativeAssassinationEligibility realAssassinationEligibility=nullptr;
std::atomic<std::uint64_t> assassinationCalls{0},assassinationPartnerConsidered{0},assassinationPartnerBlocked{0},assassinationPartnerRefused{0},assassinationOtherAccepted{0},assassinationMaxUs{0};
unsigned char filterAssassinationEligibility(std::uintptr_t context,std::uintptr_t candidate){
    ++assassinationCalls;const auto avatar=shadowDamageIdentity();
    if(avatar.actor&&candidate==avatar.actor){
        ++assassinationPartnerConsidered;const auto start=presentationClock();
        const bool blocked=currentPartnerAssassinationTarget(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),avatar,context,candidate);
        const auto us=static_cast<std::uint64_t>((std::max)(0.,(presentationClock()-start)*1000.));
        auto max=assassinationMaxUs.load();while(us>max&&!assassinationMaxUs.compare_exchange_weak(max,us)){}
        if(blocked){++assassinationPartnerBlocked;return 0;}
        ++assassinationPartnerRefused;
    }
    const auto native=realAssassinationEligibility(context,candidate);
    if(native&&candidate!=avatar.actor)++assassinationOtherAccepted;
    return native;
}
void writeAssassinationPartnerFilter(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.assassination-partner-filter.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;
    fprintf(f,"{\"installed\":%s,\"calls\":%llu,\"considered\":%llu,\"blocked\":%llu,\"readFailed\":%llu,\"otherAccepted\":%llu,\"readMaxUs\":%llu}\n",realAssassinationEligibility?"true":"false",assassinationCalls.load(),assassinationPartnerConsidered.load(),assassinationPartnerBlocked.load(),assassinationPartnerRefused.load(),assassinationOtherAccepted.load(),assassinationMaxUs.load());
    const bool ok=!ferror(f);const int close=fclose(f);if(ok&&!close)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
bool installAssassinationPartnerFilter(){
    //3b00a0: six complete non-relative instructions,16bytes. The last TEST
    //sets flags consumed by the untouched JNZ at+16; FF25 preserves flags.
    const unsigned char prologue[]={0x40,0x53,0x57,0x48,0x83,0xec,0x38,0x48,0x8b,0xfa,0x48,0x8b,0xd9,0x48,0x85,0xd2};
    auto* target=gameImage+0x3b00a0;if(std::memcmp(target,prologue,sizeof(prologue)))return false;
    auto* trampoline=static_cast<unsigned char*>(VirtualAlloc(nullptr,64,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));if(!trampoline)return false;
    std::memcpy(trampoline,prologue,sizeof(prologue));unsigned char jump[16]{0xff,0x25,0,0,0,0};jump[14]=jump[15]=0x90;void* resume=target+sizeof(prologue);
    std::memcpy(jump+6,&resume,8);std::memcpy(trampoline+sizeof(prologue),jump,14);
    DWORD old=0,unused=0;if(!VirtualProtect(trampoline,64,PAGE_EXECUTE_READ,&old)){VirtualFree(trampoline,0,MEM_RELEASE);return false;}
    FlushInstructionCache(GetCurrentProcess(),trampoline,64);
    void* wrapper=reinterpret_cast<void*>(&filterAssassinationEligibility);std::memcpy(jump+6,&wrapper,8);
    if(!VirtualProtect(target,sizeof(jump),PAGE_EXECUTE_READWRITE,&old)){VirtualFree(trampoline,0,MEM_RELEASE);return false;}
    realAssassinationEligibility=reinterpret_cast<NativeAssassinationEligibility>(trampoline);std::memcpy(target,jump,sizeof(jump));FlushInstructionCache(GetCurrentProcess(),target,sizeof(jump));
    return VirtualProtect(target,sizeof(jump),old,&unused)!=0;
}
