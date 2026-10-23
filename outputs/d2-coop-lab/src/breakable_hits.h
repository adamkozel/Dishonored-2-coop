#pragma once
#include <array>
#include <atomic>
#include "native_breakable.h"
// 1214fq (#132): the hit point and direction of recent native damage on breakables, noted in the native damage observer
// (a breakable's arkCpntHealth takes damage through the observed function), so a step sent to the peer carries the
// impulse that broke it. Read-only; the damage itself is never changed here.
struct BreakableHit {std::uint32_t handle=0;float hit[3]{},direction[3]{};ULONGLONG tick=0;};
SRWLOCK breakableHitLock=SRWLOCK_INIT;std::array<BreakableHit,16> breakableHits{};unsigned breakableHitNext=0;
std::atomic<std::uint64_t> breakableHitsNoted{0};
// 1214io (#178): hits on movable props (arkMovable breakables) are noted too.
void noteBreakableDamage(void* component,void* info){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t owner=0,vt=0;std::uint32_t handle=0;
    if(!component||!info||!memoryRead(reinterpret_cast<std::uintptr_t>(component)+0x10,&owner,8)||!owner||!memoryRead(owner,&vt,8)||
       !breakableEntityVtable(vt,base)||!memoryRead(owner+0xa8,&handle,4)||!handle)return;
    BreakableHit h{};h.handle=handle;h.tick=GetTickCount64();
    if(!memoryRead(reinterpret_cast<std::uintptr_t>(info)+0x24,h.hit,12)||!memoryRead(reinterpret_cast<std::uintptr_t>(info)+0x3c,h.direction,12))return;
    AcquireSRWLockExclusive(&breakableHitLock);breakableHits[breakableHitNext++%breakableHits.size()]=h;ReleaseSRWLockExclusive(&breakableHitLock);
    breakableHitsNoted.fetch_add(1);
}
bool latestBreakableHit(std::uint32_t handle,ULONGLONG since,BreakableHit& out){
    bool found=false;AcquireSRWLockShared(&breakableHitLock);
    for(const auto& h:breakableHits)if(h.handle==handle&&h.tick>=since&&(!found||h.tick>out.tick)){out=h;found=true;}
    ReleaseSRWLockShared(&breakableHitLock);return found;
}
