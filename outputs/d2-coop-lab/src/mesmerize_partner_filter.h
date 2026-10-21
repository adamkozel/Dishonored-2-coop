#pragma once
#include "mesmerize_partner_cached_identity.h"
// Native target eligibility only, before 72a280 queues NPC behaviour/events.
// All unrelated actors/powers receive the original call once, unchanged.
using NativeMesmerizeEligibility=std::uint64_t(*)(std::uintptr_t,std::uintptr_t,unsigned char);
NativeMesmerizeEligibility realMesmerizeEligibility=nullptr;
// 1214gh (#131, native_mesmerize_host.h): the host-owned native Mesmerize object never takes the partner avatar.
std::atomic<std::uintptr_t> mesmerizeHostOwnedComponent{0};std::atomic<std::uint64_t> mesmerizeHostPartnerBlocked{0};
std::atomic<std::uint64_t> mesmerizePartnerConsidered{0},mesmerizePartnerBlocked{0},mesmerizePartnerReadFailed{0},mesmerizeEligibilityForwarded{0},mesmerizeOtherAccepted{0};
std::atomic<std::uint64_t> mesmerizePartnerCacheHit{0},mesmerizePartnerCacheMiss{0},mesmerizePartnerReadTotalUs{0},mesmerizePartnerReadMaxUs{0};
std::uint64_t filterMesmerizeEligibility(std::uintptr_t component,std::uintptr_t candidate,unsigned char range){
 const auto a=shadowDamageIdentity();
 if(a.actor&&candidate==a.actor&&component&&component==mesmerizeHostOwnedComponent.load()){++mesmerizeHostPartnerBlocked;return 0;}
 if(a.actor&&candidate==a.actor){
  ++mesmerizePartnerConsidered;const auto base=reinterpret_cast<std::uintptr_t>(gameImage);const double start=presentationClock();
  thread_local NativeMesmerizeWorld discovered{};bool blocked=false;
  if(currentMesmerizePartnerOwner(memoryRead,base,a,discovered,component,candidate)){++mesmerizePartnerCacheHit;blocked=true;}
  else{
   ++mesmerizePartnerCacheMiss;discovered={};NativeMesmerizeWorld world{};
   if(readNativeMesmerizeWorld(memoryRead,base,world)){
    if(localMesmerizePartnerTarget(memoryRead,base,a,world,component,candidate)){discovered=std::move(world);blocked=true;}
   }else ++mesmerizePartnerReadFailed;
  }
  const auto us=static_cast<std::uint64_t>((std::max)(0.,(presentationClock()-start)*1000.));mesmerizePartnerReadTotalUs+=us;
  auto maximum=mesmerizePartnerReadMaxUs.load();while(us>maximum&&!mesmerizePartnerReadMaxUs.compare_exchange_weak(maximum,us)){}
  if(blocked){++mesmerizePartnerBlocked;return 0;}
 }
 ++mesmerizeEligibilityForwarded;const auto result=realMesmerizeEligibility(component,candidate,range);
 if(result&&candidate!=a.actor)++mesmerizeOtherAccepted;return result;
}
void writeMesmerizePartnerFilter(){
 char path[MAX_PATH]{},temp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.mesmerize-partner-filter.json",statusPath);std::snprintf(temp,sizeof(temp),"%s.tmp",path);
 FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
 fprintf(f,"{\"installed\":%s,\"considered\":%llu,\"blocked\":%llu,\"readFailed\":%llu,\"forwarded\":%llu,\"otherAccepted\":%llu,\"cacheHit\":%llu,\"cacheMiss\":%llu,\"readTotalUs\":%llu,\"readMaxUs\":%llu}\n",realMesmerizeEligibility?"true":"false",mesmerizePartnerConsidered.load(),mesmerizePartnerBlocked.load(),mesmerizePartnerReadFailed.load(),mesmerizeEligibilityForwarded.load(),mesmerizeOtherAccepted.load(),mesmerizePartnerCacheHit.load(),mesmerizePartnerCacheMiss.load(),mesmerizePartnerReadTotalUs.load(),mesmerizePartnerReadMaxUs.load());
 const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
bool installMesmerizePartnerFilter(){
 // Exact retail disk disassembly: 14 bytes of complete non-relative
 // instructions. Includes stack home-space write, pushes and allocation.
 const unsigned char prologue[]={0x44,0x88,0x44,0x24,0x18,0x55,0x56,0x57,0x41,0x56,0x48,0x83,0xec,0x48};
 auto* target=gameImage+0x72a400;if(std::memcmp(target,prologue,sizeof(prologue)))return false;
 auto* trampoline=static_cast<unsigned char*>(VirtualAlloc(nullptr,64,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));if(!trampoline)return false;
 std::memcpy(trampoline,prologue,sizeof(prologue));unsigned char jump[14]{0xff,0x25,0,0,0,0};void* resume=target+sizeof(prologue);std::memcpy(jump+6,&resume,8);std::memcpy(trampoline+sizeof(prologue),jump,sizeof(jump));
 DWORD old=0,unused=0;if(!VirtualProtect(trampoline,64,PAGE_EXECUTE_READ,&old)){VirtualFree(trampoline,0,MEM_RELEASE);return false;}FlushInstructionCache(GetCurrentProcess(),trampoline,64);
 realMesmerizeEligibility=reinterpret_cast<NativeMesmerizeEligibility>(trampoline);void* wrapper=reinterpret_cast<void*>(&filterMesmerizeEligibility);std::memcpy(jump+6,&wrapper,8);
 if(!VirtualProtect(target,sizeof(jump),PAGE_EXECUTE_READWRITE,&old))return false;std::memcpy(target,jump,sizeof(jump));FlushInstructionCache(GetCurrentProcess(),target,sizeof(jump));return VirtualProtect(target,sizeof(jump),old,&unused)!=0;
}
