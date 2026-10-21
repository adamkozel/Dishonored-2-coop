#pragma once
#include <cstdio>
// 1214gd (#140): the partner's copy of a corpse that vanishes. With the key-target finisher recognized (1214gc), her kill
// of Ramsey was shared, but in all three takes route-dunwall-lead-v1708a-c his corpse then left her world: the search
// found no mortimer_ramsey entity, her world held an ARK_CUE_NPC_DISAPPEARED cue at his spot and an anonymous entity
// 70 m below the floor. This trace samples her corpse from its death (every 100 ms for 4 s, then every 500 ms): the root,
// whether the corpse hold (npcCorpseHoldAnimated) holds it, whether her paired scene still runs and the ragdoll mode,
// and when its native read fails. coop_corpse_hold own_paired_skip leaves the corpse of her own paired kill to its
// native physics instead of holding it on the host's stream (own_paired_keep restores the hold), so takes can compare.
// 1214hb (#140, lab only): which native code removes an NPC. Her copy of Ramsey's corpse leaves her world about 10 s
// after his death in some takes (route-dunwall-lead-v1708a-c, v1710a, v1711b). Entity removal is the event 1439a6f30
// (OnCloneSpawned and the Mesmerize state 2 post it; confirmed static), posted at once by FUN_140884980 (entity, buffer,
// event, args), or after a delay by the event handler FUN_1408759c0 (entity, ..., float delay; it queues the removal
// through the variadic FUN_140884030, which is not hooked). Both are hooked in a lab build (never in product): for an
// arkNPC (vtable 1c5e108) the handle, the path (1 at once, 2 delayed), the thread and the caller stack (image RVAs)
// of the last 24 removals are kept.
struct NpcRemovalRecord {unsigned long long tick=0;unsigned handle=0,path=0,frames=0,thread=0;unsigned rva[12]{};};
std::array<NpcRemovalRecord,24> npcRemovalRecords{};std::atomic<std::uint64_t> npcRemovalCount{0};SRWLOCK npcRemovalLock=SRWLOCK_INIT;
using NativePostNow=void*(*)(void*,void*,void*,void*);using NativeRemoveDelayed=void*(*)(void*,void*,void*,float);
NativePostNow realPostNow=nullptr;NativeRemoveDelayed realRemoveDelayed=nullptr;bool npcRemovalTraceInstalled=false;
inline void noteNpcRemoval(void* entity,unsigned path){
    const auto e=reinterpret_cast<std::uintptr_t>(entity);const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t vt=0;
    if(!e||!memoryRead(e,&vt,8)||vt!=base+0x1c5e108)return;
    NpcRemovalRecord r{};r.tick=GetTickCount64();r.path=path;r.thread=GetCurrentThreadId();memoryRead(e+0xa8,&r.handle,4);
    void* frames[20]{};const auto n=RtlCaptureStackBackTrace(0,20,frames,nullptr);
    for(unsigned i=0;i<n&&r.frames<12;++i){const auto f=reinterpret_cast<std::uintptr_t>(frames[i]);if(f>=base&&f<base+68988928)r.rva[r.frames++]=static_cast<unsigned>(f-base);}
    AcquireSRWLockExclusive(&npcRemovalLock);npcRemovalRecords[npcRemovalCount.fetch_add(1)%24]=r;ReleaseSRWLockExclusive(&npcRemovalLock);}
// 1214hc: her copy of Ramsey was removed by the event queue (route-dunwall-lead-v1745d: its dispatch 1408853e8 posts a
// queued record's event to its entity, 10.2 s after the death), so the removal was queued earlier. The queue insert
// FUN_1408851f0 (queue, record, entity, delay; the record's event at +8) is hooked too: a queued removal of an arkNPC
// keeps its delay and caller stack in its own ring (the 24 posted ones were overwritten by the restore's teardown).
struct NpcRemovalQueued {unsigned long long tick=0;unsigned handle=0,frames=0,thread=0;float delay=0;unsigned rva[12]{};};
std::array<NpcRemovalQueued,24> npcRemovalQueuedRecords{};std::atomic<std::uint64_t> npcRemovalQueuedCount{0};
using NativeQueueInsert=void(*)(void*,void*,void*,std::uint32_t);NativeQueueInsert realQueueInsert=nullptr;
inline void hookQueueInsert(void* queue,void* record,void* entity,std::uint32_t delayBits){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t def=0,vt=0;const auto e=reinterpret_cast<std::uintptr_t>(entity);
    if(record&&memoryRead(reinterpret_cast<std::uintptr_t>(record)+8,&def,8)&&def==base+0x39a6f30&&e&&memoryRead(e,&vt,8)&&vt==base+0x1c5e108){
        NpcRemovalQueued r{};r.tick=GetTickCount64();r.thread=GetCurrentThreadId();memoryRead(e+0xa8,&r.handle,4);std::memcpy(&r.delay,&delayBits,4);
        void* frames[20]{};const auto n=RtlCaptureStackBackTrace(0,20,frames,nullptr);
        for(unsigned i=0;i<n&&r.frames<12;++i){const auto f=reinterpret_cast<std::uintptr_t>(frames[i]);if(f>=base&&f<base+68988928)r.rva[r.frames++]=static_cast<unsigned>(f-base);}
        AcquireSRWLockExclusive(&npcRemovalLock);npcRemovalQueuedRecords[npcRemovalQueuedCount.fetch_add(1)%24]=r;ReleaseSRWLockExclusive(&npcRemovalLock);}
    realQueueInsert(queue,record,entity,delayBits);}
inline void* hookPostNow(void* e,void* buf,void* def,void* args){if(def==gameImage+0x39a6f30)noteNpcRemoval(e,1);return realPostNow(e,buf,def,args);}
inline void* hookRemoveDelayed(void* e,void* p2,void* p3,float delay){noteNpcRemoval(e,2);return realRemoveDelayed(e,p2,p3,delay);}
template<class T> bool installNpcRemovalHook(std::uintptr_t rva,const unsigned char* prologue,unsigned len,void* hook,T& real){
    auto* target=gameImage+rva;if(len<14||len>32||std::memcmp(target,prologue,len))return false;
    auto* tramp=static_cast<unsigned char*>(VirtualAlloc(nullptr,64,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));if(!tramp)return false;
    unsigned char jump[14]{0xff,0x25,0,0,0,0};void* resume=target+len;std::memcpy(jump+6,&resume,8);std::memcpy(tramp,prologue,len);std::memcpy(tramp+len,jump,14);
    DWORD old=0,unused=0;if(!VirtualProtect(tramp,64,PAGE_EXECUTE_READ,&old)){VirtualFree(tramp,0,MEM_RELEASE);return false;}FlushInstructionCache(GetCurrentProcess(),tramp,64);
    real=reinterpret_cast<T>(tramp);unsigned char patch[32];std::memset(patch,0x90,sizeof(patch));std::memcpy(jump+6,&hook,8);std::memcpy(patch,jump,14);
    if(!VirtualProtect(target,len,PAGE_EXECUTE_READWRITE,&old))return false;std::memcpy(target,patch,len);FlushInstructionCache(GetCurrentProcess(),target,len);
    return VirtualProtect(target,len,old,&unused)!=0;}
inline bool installNpcRemovalTrace(){
    static const unsigned char now[]={0x48,0x89,0x54,0x24,0x10,0x57,0x41,0x56,0x41,0x57,0x48,0x83,0xec,0x30};
    static const unsigned char queueInsert[]={0x44,0x89,0x4c,0x24,0x20,0x48,0x89,0x54,0x24,0x10,0x53,0x57,0x48,0x83,0xec,0x38};
    static const unsigned char delayed[]={0x40,0x53,0x48,0x83,0xec,0x20,0xf3,0x0f,0x11,0x5c,0x24,0x48,0x44,0x8b,0x44,0x24,0x48};
    npcRemovalTraceInstalled=installNpcRemovalHook(0x884980,now,sizeof(now),reinterpret_cast<void*>(&hookPostNow),realPostNow)&&
        installNpcRemovalHook(0x8759c0,delayed,sizeof(delayed),reinterpret_cast<void*>(&hookRemoveDelayed),realRemoveDelayed)&&
        installNpcRemovalHook(0x8851f0,queueInsert,sizeof(queueInsert),reinterpret_cast<void*>(&hookQueueInsert),realQueueInsert);
    return npcRemovalTraceInstalled;}
inline void writeNpcRemovals(FILE* f){
    fprintf(f,",\"npcRemovalTrace\":%s,\"npcRemovalCount\":%llu,\"npcRemovals\":[",npcRemovalTraceInstalled?"true":"false",static_cast<unsigned long long>(npcRemovalCount.load()));
    AcquireSRWLockShared(&npcRemovalLock);const auto n=npcRemovalCount.load();bool first=true;
    for(std::uint64_t i=n>24?n-24:0;i<n;++i){const auto& r=npcRemovalRecords[i%24];
        fprintf(f,"%s{\"tick\":%llu,\"handle\":%u,\"path\":%u,\"thread\":%u,\"rva\":[",first?"":",",r.tick,r.handle,r.path,r.thread);first=false;
        for(unsigned k=0;k<r.frames;++k)fprintf(f,"%s\"%x\"",k?",":"",r.rva[k]);fputs("]}",f);}
    fprintf(f,"],\"npcRemovalQueuedCount\":%llu,\"npcRemovalsQueued\":[",static_cast<unsigned long long>(npcRemovalQueuedCount.load()));
    {const auto q=npcRemovalQueuedCount.load();bool firstQ=true;
     for(std::uint64_t i=q>24?q-24:0;i<q;++i){const auto& r=npcRemovalQueuedRecords[i%24];
        fprintf(f,"%s{\"tick\":%llu,\"handle\":%u,\"delay\":%.3f,\"thread\":%u,\"rva\":[",firstQ?"":",",r.tick,r.handle,r.delay,r.thread);firstQ=false;
        for(unsigned k=0;k<r.frames;++k)fprintf(f,"%s\"%x\"",k?",":"",r.rva[k]);fputs("]}",f);}}
    ReleaseSRWLockShared(&npcRemovalLock);fputs("]",f);}
struct NpcCorpseSample {unsigned ms=0;float root[3]{};unsigned char held=0,paired=0,mode=0xff;};
struct NpcCorpseTrace {unsigned arm=0,handle=0,count=0,vanishedMs=0;bool vanished=false,ownPaired=false,holdSkipped=false;unsigned long long startedAt=0;NpcCorpseSample samples[80]{};};
NpcCorpseTrace npcCorpseTraces[4]{};unsigned npcCorpseTraceCount=0;
bool npcSkipOwnPairedCorpseHold=false;unsigned long long npcOwnPairedHoldSkips=0;
inline NpcCorpseTrace* npcCorpseTraceFor(unsigned arm,unsigned handle){for(auto& t:npcCorpseTraces)if(t.startedAt&&t.arm==arm&&t.handle==handle)return &t;return nullptr;}
inline void npcCorpseTraceBegin(unsigned arm,unsigned handle,unsigned long long tick,bool ownPaired){
    auto& t=npcCorpseTraces[npcCorpseTraceCount++%4];t=NpcCorpseTrace{};t.arm=arm;t.handle=handle;t.startedAt=tick?tick:1;t.ownPaired=ownPaired;}
inline void npcCorpseTraceSample(NpcCorpseTrace& t,unsigned long long tick,const float* root,bool held,bool paired,unsigned mode){
    if(t.vanished||t.count>=80)return;const unsigned ms=static_cast<unsigned>(tick-t.startedAt);
    if(t.count&&ms>4000&&ms-t.samples[t.count-1].ms<500)return;
    auto& s=t.samples[t.count++];s.ms=ms;if(root){s.root[0]=root[0];s.root[1]=root[1];s.root[2]=root[2];}
    s.held=held?1:0;s.paired=paired?1:0;s.mode=static_cast<unsigned char>(mode>255?255:mode);}
inline void writeNpcCorpseTraces(FILE* f){
    writeNpcRemovals(f);
    fprintf(f,",\"ownPairedHoldSkip\":%s,\"ownPairedHoldSkips\":%llu,\"corpseTraces\":[",npcSkipOwnPairedCorpseHold?"true":"false",npcOwnPairedHoldSkips);
    bool first=true;
    for(const auto& t:npcCorpseTraces){if(!t.startedAt)continue;
        fprintf(f,"%s{\"arm\":%u,\"handle\":%u,\"ownPaired\":%s,\"holdSkipped\":%s,\"vanished\":%s,\"vanishedMs\":%u,\"samples\":[",first?"":",",t.arm,t.handle,
            t.ownPaired?"true":"false",t.holdSkipped?"true":"false",t.vanished?"true":"false",t.vanishedMs);first=false;
        for(unsigned i=0;i<t.count;++i){const auto& s=t.samples[i];fprintf(f,"%s[%u,%.2f,%.2f,%.2f,%u,%u,%u]",i?",":"",s.ms,s.root[0],s.root[1],s.root[2],s.held,s.paired,s.mode);}
        fputs("]}",f);}
    fputs("]",f);
}
