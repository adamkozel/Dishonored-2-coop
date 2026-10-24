#pragma once
#include "use_mirror_protocol.h"
// 1214jn (#187): the use mirror. The native post hook (0x140884920, kiscule_action_mirror.h) hands every "use" event to
// observeUseEvent; one whose _user is this world's player and whose target is an eligible usable is sent by name. The
// other world posts the same event with its own player and runs the native use method (applyNativeUseAsLocalPlayer),
// so the usable's script runs there too (mission-lever-v1861c: a lever's script listens to "use"). Both roles send;
// a mirrored use is never sent back (applyingUseMirror).
struct UseMirrorRecord {std::uint64_t tick=0;unsigned handle=0;bool incoming=false;const char* result="";char name[96]{};};
std::array<UseMirrorRecord,24> useMirrorRecords{};std::uint64_t useMirrorRecordCount=0;
SRWLOCK useMirrorLock=SRWLOCK_INIT;
std::vector<unsigned> useMirrorObserved;
struct UseMirrorItem {std::string name;std::uint64_t epoch=0;};
std::vector<UseMirrorItem> useMirrorIncoming;
bool useMirrorEnabled=false;
std::atomic<bool> applyingUseMirror{false};
std::uint64_t useMirrorSequence=0,useMirrorPeerSequence=0,useMirrorObservedCount=0,useMirrorSent=0,useMirrorReceived=0,useMirrorApplied=0;
std::uint64_t useMirrorIneligible=0,useMirrorNotFound=0,useMirrorRefused=0,useMirrorRejected=0,useMirrorDropped=0;
const char* useMirrorResult="disabled";
void recordUseMirror(const std::string& name,unsigned handle,bool incoming,const char* result){
    AcquireSRWLockExclusive(&useMirrorLock);auto& r=useMirrorRecords[useMirrorRecordCount++%useMirrorRecords.size()];
    r={};r.tick=GetTickCount64();r.handle=handle;r.incoming=incoming;r.result=result;std::snprintf(r.name,sizeof(r.name),"%s",name.c_str());
    ReleaseSRWLockExclusive(&useMirrorLock);
}
// Any thread: the argument is the event's 'e' variant (type byte, then the entity handle at +8).
void observeUseEvent(void* entity,void* argument){
    if(!useMirrorEnabled||applyingUseMirror.load()||!entity||!argument)return;
    unsigned char type=0;unsigned user=0,handle=0;const auto a=reinterpret_cast<std::uintptr_t>(argument);
    if(!memoryRead(a,&type,1)||type!='e'||!memoryRead(a+8,&user,4)||user!=kNativeLocalPlayerHandle||
       !memoryRead(reinterpret_cast<std::uintptr_t>(entity)+0xa8,&handle,4))return;
    AcquireSRWLockExclusive(&useMirrorLock);++useMirrorObservedCount;
    if(useMirrorObserved.size()<32)useMirrorObserved.push_back(handle);else ++useMirrorDropped;
    ReleaseSRWLockExclusive(&useMirrorLock);
}
void receiveUseMirror(const UseMirrorPacket& p){
    ++useMirrorReceived;
    if(!useMirrorEnabled||!validUseMirror(p,localRole)||p.session!=remote.session||p.mapId!=outgoing.mapId||p.epoch!=remote.worldEpoch||p.sequence<=useMirrorPeerSequence){++useMirrorRejected;return;}
    useMirrorPeerSequence=p.sequence;
    AcquireSRWLockExclusive(&useMirrorLock);
    if(useMirrorIncoming.size()<32)useMirrorIncoming.push_back({p.name,p.epoch});else ++useMirrorDropped;
    ReleaseSRWLockExclusive(&useMirrorLock);
}
// The one eligible state-list usable with this authored name in this world.
bool findUseMirrorUsable(const std::string& name,NativeUsable& out){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t list=0;unsigned count=0;
    if(!observedGame||!memoryRead(observedGame+0x601c0,&list,8)||!list||!memoryRead(observedGame+0x601c8,&count,4)||(count&0xffffff)>16384)return false;
    std::vector<unsigned> handles(count&0xffffff);if(!handles.empty()&&!memoryRead(list,handles.data(),handles.size()*4))return false;
    unsigned found=0;
    for(auto h:handles){NativeUsable n{};if(readNativeUsable(memoryRead,base,observedGame,h,n)&&n.name==name){++found;out=std::move(n);}}
    return found==1&&useMirrorEligible(out);
}
void serviceUseMirror(std::uint64_t){
    if(GetCurrentThreadId()!=engineThread.load())return;
    if(!useMirrorEnabled){AcquireSRWLockExclusive(&useMirrorLock);useMirrorObserved.clear();useMirrorIncoming.clear();ReleaseSRWLockExclusive(&useMirrorLock);return;}
    std::vector<unsigned> observed;std::vector<UseMirrorItem> apply;
    AcquireSRWLockExclusive(&useMirrorLock);observed.swap(useMirrorObserved);
    const bool gameplay=outgoing.gameLogicState==1&&!outgoing.pendingGameLogicState&&observedPlayer;
    if(gameplay)apply.swap(useMirrorIncoming);
    ReleaseSRWLockExclusive(&useMirrorLock);
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    for(auto h:observed){NativeUsable n{};
        if(!readNativeUsable(memoryRead,base,observedGame,h,n))continue; // doors, pickups, NPCs: not a state-list usable
        if(!useMirrorEligible(n)){++useMirrorIneligible;continue;}
        if(!sockets||!connection||connectionState!=k_ESteamNetworkingConnectionState_Connected||!coopWorldPairReady(outgoing,remote)||outgoing.mapId!=remote.mapId){
            ++useMirrorDropped;recordUseMirror(n.name,h,false,"dropped_no_pair");continue;}
        UseMirrorPacket p{};p.role=localRole;p.session=outgoing.session;p.epoch=outgoing.worldEpoch;p.mapId=outgoing.mapId;p.sequence=++useMirrorSequence;
        std::snprintf(p.name,sizeof(p.name),"%s",n.name.c_str());
        if(coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)==k_EResultOK){++useMirrorSent;useMirrorResult="sent";recordUseMirror(n.name,h,false,"sent");}
        else{++useMirrorDropped;recordUseMirror(n.name,h,false,"send_failed");}
    }
    for(const auto& item:apply){
        if(item.epoch!=remote.worldEpoch){++useMirrorRejected;continue;}
        NativeUsable n{};if(!findUseMirrorUsable(item.name,n)){++useMirrorNotFound;useMirrorResult="usable_not_found";recordUseMirror(item.name,0,true,"usable_not_found");continue;}
        applyingUseMirror.store(true);const bool ok=applyNativeUseAsLocalPlayer(n);applyingUseMirror.store(false);
        if(ok){++useMirrorApplied;useMirrorResult="applied";}else{++useMirrorRefused;useMirrorResult="native_use_refused";}
        recordUseMirror(item.name,n.handle,true,ok?"applied":"native_use_refused");
    }
}
void writeUseMirror(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.use-mirror.json",statusPath)>=static_cast<int>(sizeof(path))||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=static_cast<int>(sizeof(temp)))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    AcquireSRWLockShared(&useMirrorLock);
    fprintf(f,"{\"role\":%u,\"enabled\":%s,\"result\":\"%s\",\"observed\":%llu,\"sent\":%llu,\"received\":%llu,\"applied\":%llu,\"ineligible\":%llu,\"notFound\":%llu,\"refused\":%llu,\"rejected\":%llu,\"dropped\":%llu,\"records\":[",
        localRole,useMirrorEnabled?"true":"false",useMirrorResult,useMirrorObservedCount,useMirrorSent,useMirrorReceived,useMirrorApplied,useMirrorIneligible,useMirrorNotFound,useMirrorRefused,useMirrorRejected,useMirrorDropped);
    const auto first=useMirrorRecordCount>useMirrorRecords.size()?useMirrorRecordCount-useMirrorRecords.size():0;
    for(auto i=first;i<useMirrorRecordCount;++i){const auto& r=useMirrorRecords[i%useMirrorRecords.size()];
        fprintf(f,"%s{\"tick\":%llu,\"name\":\"%s\",\"handle\":%u,\"incoming\":%s,\"result\":\"%s\"}",i==first?"":",",r.tick,r.name,r.handle,r.incoming?"true":"false",r.result);}
    fprintf(f,"]}\n");
    ReleaseSRWLockShared(&useMirrorLock);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
