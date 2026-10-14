#pragma once
#include "interaction_partner_identity.h"
using NativeInteractionPick=std::uintptr_t(*)(std::uintptr_t,unsigned char);
NativeInteractionPick realInteractionSwordPick=nullptr,realInteractionFatalityPick=nullptr;
std::atomic<std::uint64_t> interactionCalls{0},interactionConsidered{0},interactionSwordBlocked{0},interactionFatalityBlocked{0},interactionRefused{0},interactionOtherReturned{0},interactionMaxUs{0};
std::uintptr_t filterInteractionPick(std::uintptr_t sensor,unsigned char force,NativeInteractionPick original,unsigned field){
    ++interactionCalls;const auto candidate=original(sensor,force);const auto avatar=shadowDamageIdentity();
    if(candidate&&candidate==avatar.actor){
        ++interactionConsidered;const auto start=presentationClock();
        const bool block=currentPartnerInteractionPick(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),avatar,sensor,candidate,field);
        const auto us=static_cast<std::uint64_t>((std::max)(0.,(presentationClock()-start)*1000.));
        auto max=interactionMaxUs.load();while(us>max&&!interactionMaxUs.compare_exchange_weak(max,us)){}
        if(block){if(field==0x88)++interactionFatalityBlocked;else ++interactionSwordBlocked;return 0;}
        ++interactionRefused;
    }else if(candidate)++interactionOtherReturned;
    return candidate;
}
std::uintptr_t filterInteractionSwordPick(std::uintptr_t sensor,unsigned char force){return filterInteractionPick(sensor,force,realInteractionSwordPick,0x8c);}
std::uintptr_t filterInteractionFatalityPick(std::uintptr_t sensor,unsigned char force){return filterInteractionPick(sensor,force,realInteractionFatalityPick,0x88);}
void writeInteractionPartnerFilter(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.interaction-partner-filter.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;
    fprintf(f,"{\"installed\":%s,\"calls\":%llu,\"considered\":%llu,\"swordBlocked\":%llu,\"fatalityBlocked\":%llu,\"readFailed\":%llu,\"otherReturned\":%llu,\"readMaxUs\":%llu}\n",realInteractionSwordPick&&realInteractionFatalityPick?"true":"false",interactionCalls.load(),interactionConsidered.load(),interactionSwordBlocked.load(),interactionFatalityBlocked.load(),interactionRefused.load(),interactionOtherReturned.load(),interactionMaxUs.load());
    const bool ok=!ferror(f);const int close=fclose(f);if(ok&&!close)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
bool installInteractionPickAt(std::size_t rva,void* wrapper,NativeInteractionPick& original){
    // Both getters start with these16bytes. Both short branches land exactly
    // at+16, so the copied branches reach the appended absolute resume jump.
    // False branch returns directly from the original trampoline. No relative
    // call or RIP-relative access is copied. Native handle resolution remains.
    const unsigned char prologue[]={0x84,0xd2,0x75,0x0c,0xf6,0x81,0xb8,0,0,0,0x01,0x74,0x03,0x33,0xc0,0xc3};
    auto* target=gameImage+rva;if(std::memcmp(target,prologue,sizeof(prologue)))return false;
    auto* trampoline=static_cast<unsigned char*>(VirtualAlloc(nullptr,64,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));if(!trampoline)return false;
    std::memcpy(trampoline,prologue,sizeof(prologue));unsigned char jump[16]{0xff,0x25,0,0,0,0};jump[14]=jump[15]=0x90;void* resume=target+sizeof(prologue);
    std::memcpy(jump+6,&resume,8);std::memcpy(trampoline+sizeof(prologue),jump,14);
    DWORD old=0,unused=0;if(!VirtualProtect(trampoline,64,PAGE_EXECUTE_READ,&old)){VirtualFree(trampoline,0,MEM_RELEASE);return false;}
    FlushInstructionCache(GetCurrentProcess(),trampoline,64);std::memcpy(jump+6,&wrapper,8);
    if(!VirtualProtect(target,sizeof(jump),PAGE_EXECUTE_READWRITE,&old)){VirtualFree(trampoline,0,MEM_RELEASE);return false;}
    original=reinterpret_cast<NativeInteractionPick>(trampoline);std::memcpy(target,jump,sizeof(jump));FlushInstructionCache(GetCurrentProcess(),target,sizeof(jump));
    return VirtualProtect(target,sizeof(jump),old,&unused)!=0;
}
bool installInteractionPartnerFilter(){
    return installInteractionPickAt(0x3b8020,reinterpret_cast<void*>(&filterInteractionSwordPick),realInteractionSwordPick)&&
        installInteractionPickAt(0x3b8080,reinterpret_cast<void*>(&filterInteractionFatalityPick),realInteractionFatalityPick);
}
