#pragma once
// 1171 shared mission progression through native trigger volumes.
// arkCpntTrigger's enter/exit handler (0x140779e00: component, entity, enter)
// posts the trigger's script event to the mission graph (Kiscule), marks
// once-triggers and sends the 0xd2/0xd3 messages (chapter 89, confirmed
// static). Observation is always on. With coop_trigger_mirror on, a scripted
// trigger (script_trigger*, not hideout/noswim volumes) entered or left by the
// LOCAL player is sent to the partner (name rule below, widened in 1214bf),
// whose world runs the same native handler
// for its own player. Once-triggers dedupe natively; mirrored calls never echo.
using NativeTriggerEvent=void(*)(void*,void*,bool);
NativeTriggerEvent realTriggerEvent=nullptr;
const unsigned char triggerEventPrefix[19]={0x48,0x8b,0xc4,0x57,0x48,0x81,0xec,0x80,0,0,0,0x48,0xc7,0x40,0xa8,0xfe,0xff,0xff,0xff};
#pragma pack(push,1)
struct TriggerMirrorPacket {
    std::uint32_t magic=0x4D543244; // "D2TM"
    std::uint16_t version=1,size=sizeof(TriggerMirrorPacket);
    std::uint32_t role=0,enter=0;
    std::uint64_t session=0,epoch=0,mapId=0,sequence=0;
    char name[128]{};
};
#pragma pack(pop)
static_assert(sizeof(TriggerMirrorPacket)==176);
// 1214bf: campaign scripts listen to many trigger name families (ld_trigger
// 428, script_trigger 301, mdnt_trigger 20, obj_trigger, ...; confirmed static
// across the campaign maps), so any volume named *trigger* is mirrored except
// those that describe a player's own position: audio, lighting/environment,
// hideout, no-swim, death (kill volumes), vista and water volumes.
inline bool triggerMirrorEligibleName(const char* name){
    if(!name||!std::strstr(name,"trigger"))return false;
    static const char* const excluded[]={"audio","light","env_","hideout","noswim","death","vista","water"};
    for(const char* x:excluded)if(std::strstr(name,x))return false;
    return true;
}
inline bool validTriggerMirror(const TriggerMirrorPacket& p,unsigned receiver){
    if(p.magic!=0x4D543244||p.version!=1||p.size!=sizeof(p)||p.role>1||p.role==receiver||p.enter>1||!p.session||!p.epoch||!p.mapId||!p.sequence)return false;
    if(!std::memchr(p.name,0,sizeof(p.name)))return false;
    return triggerMirrorEligibleName(p.name);
}
struct TriggerEventRecord {
    std::uint64_t sequence=0,tick=0,epoch=0;std::uint32_t triggerHandle=0,entityHandle=0;
    bool enter=false,local=false,mirrored=false,queued=false;char name[96]{};
};
constexpr unsigned kTriggerEventCapacity=256;
TriggerEventRecord triggerEvents[kTriggerEventCapacity]{};
std::uint64_t triggerEventSequence=0;
SRWLOCK triggerLock=SRWLOCK_INIT;
struct TriggerMirrorItem {std::string name;bool enter=false;std::uint64_t sequence=0,epoch=0;};
std::vector<TriggerMirrorItem> triggerOutgoing,triggerIncoming;
bool triggerMirrorEnabled=false;
std::atomic<bool> applyingTriggerMirror{false};
std::uint64_t triggerMirrorSequence=0,triggerPeerSequence=0,triggerSent=0,triggerReceived=0,triggerApplied=0,triggerNotFound=0,triggerRejected=0,triggerDropped=0;
const char* triggerMirrorResult="disabled";
void observeTriggerEvent(void* component,void* entity,bool enter){
    TriggerEventRecord e{};e.tick=GetTickCount64();e.epoch=outgoing.worldEpoch;e.enter=enter;
    const auto c=reinterpret_cast<std::uintptr_t>(component),en=reinterpret_cast<std::uintptr_t>(entity);
    std::uintptr_t trigger=0,namePointer=0;std::string name;
    if(c&&memoryRead(c+0x10,&trigger,8)&&trigger){memoryRead(trigger+0xa8,&e.triggerHandle,4);if(memoryRead(trigger+0x20,&namePointer,8))worldIdentityString(memoryRead,namePointer,name);}
    if(en)memoryRead(en+0xa8,&e.entityHandle,4);
    e.local=en&&en==observedPlayer;e.mirrored=applyingTriggerMirror.load();
    std::snprintf(e.name,sizeof(e.name),"%s",name.c_str());
    e.queued=triggerMirrorEnabled&&e.local&&!e.mirrored&&triggerMirrorEligibleName(e.name)&&name.size()<127;
    AcquireSRWLockExclusive(&triggerLock);
    e.sequence=++triggerEventSequence;triggerEvents[(e.sequence-1)%kTriggerEventCapacity]=e;
    if(e.queued){if(triggerOutgoing.size()<64)triggerOutgoing.push_back({name,enter,0,e.epoch});else ++triggerDropped;}
    ReleaseSRWLockExclusive(&triggerLock);
    realTriggerEvent(component,entity,enter);
}
bool installTriggerEventObserver(){
    auto* target=gameImage+0x779e00;if(std::memcmp(target,triggerEventPrefix,sizeof(triggerEventPrefix)))return false;
    // First 19 bytes are four complete instructions with no branches or
    // RIP-relative operands; the original continues at entry+19.
    auto* trampoline=static_cast<unsigned char*>(VirtualAlloc(nullptr,64,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));if(!trampoline)return false;
    std::memcpy(trampoline,triggerEventPrefix,19);unsigned char jump[19]{0xff,0x25,0,0,0,0};std::memset(jump+14,0x90,5);
    void* resume=target+19;std::memcpy(jump+6,&resume,8);std::memcpy(trampoline+19,jump,14);
    DWORD old=0,unused=0;if(!VirtualProtect(trampoline,64,PAGE_EXECUTE_READ,&old)){VirtualFree(trampoline,0,MEM_RELEASE);return false;}
    FlushInstructionCache(GetCurrentProcess(),trampoline,64);void* wrapper=reinterpret_cast<void*>(&observeTriggerEvent);std::memcpy(jump+6,&wrapper,8);
    if(!VirtualProtect(target,19,PAGE_EXECUTE_READWRITE,&old)){VirtualFree(trampoline,0,MEM_RELEASE);return false;}
    realTriggerEvent=reinterpret_cast<NativeTriggerEvent>(trampoline);std::memcpy(target,jump,19);FlushInstructionCache(GetCurrentProcess(),target,19);
    return VirtualProtect(target,19,old,&unused)!=0;
}
void receiveTriggerMirror(const TriggerMirrorPacket& p){
    ++triggerReceived;
    if(!triggerMirrorEnabled||!validTriggerMirror(p,localRole)||p.session!=remote.session||p.mapId!=outgoing.mapId||p.epoch!=remote.worldEpoch||p.sequence<=triggerPeerSequence){++triggerRejected;return;}
    triggerPeerSequence=p.sequence;
    AcquireSRWLockExclusive(&triggerLock);
    if(triggerIncoming.size()<64)triggerIncoming.push_back({p.name,p.enter!=0,p.sequence,p.epoch});else ++triggerDropped;
    ReleaseSRWLockExclusive(&triggerLock);
}
// Finds the named arkTrigger entity and its arkCpntTrigger in this world.
std::uintptr_t findTriggerComponent(const std::string& name){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);if(!observedGame)return 0;
    for(unsigned slot=0;slot<16384;++slot){std::uintptr_t actor=0,vt=0,p=0;
        if(!memoryRead(observedGame+0x3130b0+slot*8,&actor,8)||!actor||!memoryRead(actor,&vt,8)||vt!=base+0x1c5eae8)continue;
        std::string n;if(!memoryRead(actor+0x20,&p,8)||!worldIdentityString(memoryRead,p,n)||n!=name)continue;
        std::uintptr_t list=0;unsigned count=0;if(!memoryRead(actor+0x268,&list,8)||!list||!memoryRead(actor+0x270,&count,4)||(count&0xffffff)>256)return 0;
        for(unsigned i=0;i<(count&0xffffff);++i){std::uintptr_t c=0,cv=0,owner=0;
            if(memoryRead(list+i*8,&c,8)&&c&&memoryRead(c,&cv,8)&&cv==base+0x1c5bae8&&memoryRead(c+0x10,&owner,8)&&owner==actor)return c;}
        return 0;
    }
    return 0;
}
void serviceTriggerMirror(std::uint64_t tick){
    if(GetCurrentThreadId()!=engineThread.load())return;
    if(!triggerMirrorEnabled){AcquireSRWLockExclusive(&triggerLock);triggerOutgoing.clear();triggerIncoming.clear();ReleaseSRWLockExclusive(&triggerLock);return;}
    std::vector<TriggerMirrorItem> send,apply;
    AcquireSRWLockExclusive(&triggerLock);send.swap(triggerOutgoing);
    const bool gameplay=outgoing.gameLogicState==1&&!outgoing.pendingGameLogicState&&observedPlayer;
    if(gameplay)apply.swap(triggerIncoming);
    ReleaseSRWLockExclusive(&triggerLock);
    for(const auto& item:send){
        if(item.epoch!=outgoing.worldEpoch||!sockets||!connection||connectionState!=k_ESteamNetworkingConnectionState_Connected){++triggerDropped;continue;}
        TriggerMirrorPacket p{};p.role=localRole;p.enter=item.enter?1u:0u;p.session=outgoing.session;p.epoch=outgoing.worldEpoch;p.mapId=outgoing.mapId;p.sequence=++triggerMirrorSequence;
        std::snprintf(p.name,sizeof(p.name),"%s",item.name.c_str());
        if(coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)==k_EResultOK){++triggerSent;triggerMirrorResult="sent";}
    }
    for(const auto& item:apply){
        if(item.epoch!=remote.worldEpoch){++triggerRejected;continue;}
        const auto component=findTriggerComponent(item.name);
        if(!component){++triggerNotFound;triggerMirrorResult="trigger_not_found";continue;}
        // Same native handler the physics callback calls, for this world's own
        // player; the observer records it as mirrored and never re-sends it.
        applyingTriggerMirror.store(true);
        reinterpret_cast<NativeTriggerEvent>(gameImage+0x779e00)(reinterpret_cast<void*>(component),reinterpret_cast<void*>(observedPlayer),item.enter);
        applyingTriggerMirror.store(false);
        ++triggerApplied;triggerMirrorResult="applied";
    }
}
void writeTriggerTrace(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.triggers.json",statusPath)>=static_cast<int>(sizeof(path))||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=static_cast<int>(sizeof(temp)))return;
    static TriggerEventRecord copy[kTriggerEventCapacity];std::uint64_t last=0;
    AcquireSRWLockShared(&triggerLock);last=triggerEventSequence;std::memcpy(copy,triggerEvents,sizeof(copy));ReleaseSRWLockShared(&triggerLock);
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\"role\":%u,\"mirror\":%s,\"result\":\"%s\",\"sent\":%llu,\"received\":%llu,\"applied\":%llu,\"notFound\":%llu,\"rejected\":%llu,\"dropped\":%llu,\"count\":%llu,\"events\":[",
        localRole,triggerMirrorEnabled?"true":"false",triggerMirrorResult,triggerSent,triggerReceived,triggerApplied,triggerNotFound,triggerRejected,triggerDropped,last);
    const auto first=last>kTriggerEventCapacity?last-kTriggerEventCapacity:0;
    for(auto i=first;i<last;++i){const auto& e=copy[i%kTriggerEventCapacity];
        fprintf(f,"%s{\"sequence\":%llu,\"tick\":%llu,\"epoch\":%llu,\"name\":\"%s\",\"trigger\":\"0x%x\",\"entity\":\"0x%x\",\"enter\":%s,\"local\":%s,\"mirrored\":%s,\"queued\":%s}",
            i==first?"":",",e.sequence,e.tick,e.epoch,e.name,e.triggerHandle,e.entityHandle,e.enter?"true":"false",e.local?"true":"false",e.mirrored?"true":"false",e.queued?"true":"false");}
    fprintf(f,"]}\n");const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
