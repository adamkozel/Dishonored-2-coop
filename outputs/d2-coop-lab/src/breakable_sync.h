#pragma once
#include "native_breakable.h"
#include "breakable_protocol.h"
#include <vector>
// 1214fq (#132): breakables shared between the worlds. A grenade blew the office door's glass apart in one world only
// (grenade-real-v1273b-host), and a barrier broken by one player stayed whole for the other.
// - Each world catalogs its arkBreakable entities (rescanned per world and every 15 s) and polls their steps.
// - A step that advanced locally is sent once, with the hit noted by the damage observer when there is one.
// - A peer step above the local one is applied through the native step advance (1407ed000), one step at a time, with
//   default sword damage params carrying the hit, so the pieces, chunks and FX are the game's own. The breakable's
//   health is lowered to the applied step's threshold, so later hits break the next step as in the other world.
// - Breaks only go forward and an applied step counts as sent, so nothing echoes. Lab and product: coop_breakable_sync.
struct BreakableEntry {std::uint32_t handle=0;std::string name;int stepCount=0,step=0,sent=0;std::uint64_t sends=0,applied=0;ULONGLONG seenAt=0;};
struct BreakableRemoteStep {std::uint32_t handle=0;int step=0,stepCount=0;bool hitValid=false;float hit[3]{},direction[3]{};std::string name;};
struct BreakableSyncState {
    bool enabled=false;std::uint64_t epoch=0,peerEpoch=0,map=0,session=0,peerSession=0,sequence=0;
    std::vector<BreakableEntry> entries;std::vector<BreakableRemoteStep> pending;
    ULONGLONG scanAt=0,pollAt=0;unsigned scanMs=0;
    std::uint64_t scans=0,sent=0,sentWithHit=0,sendFailures=0,received=0,rejected=0,ignored=0,unmatched=0,overflow=0,
        applied=0,appliedSteps=0,applyRefused=0,healthLowered=0,worldResets=0,vanishedSent=0,catalogFull=0;
    const char* result="off";const char* lastApply="none";
};
BreakableSyncState breakableSync;int breakableSyncEnableRequest=-1;
bool breakableSyncContext(){
    return sockets&&connection&&connectionState==k_ESteamNetworkingConnectionState_Connected&&coopWorldPairReady(outgoing,remote)&&!doorCheckpointBarrierActive();
}
void scanBreakables(ULONGLONG tick){
    auto& s=breakableSync;const auto started=GetTickCount64();const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    std::uintptr_t game=0,handles=0;std::uint32_t flags=0;s.scanAt=tick;
    if(!memoryRead(base+0x39a5290,&game,8)||!game||!memoryRead(game+0x601c0,&handles,8)||!handles||!memoryRead(game+0x601c8,&flags,4))return;
    const auto count=flags&0xffffff;if(count>16384)return;
    std::vector<BreakableEntry> next;
    for(std::uint32_t i=0;i<count;++i){std::uint32_t handle=0;NativeBreakable b{};
        if(!memoryRead(handles+i*4,&handle,4)||!handle||!readNativeBreakable(memoryRead,base,game,handle,b,true))continue;
        BreakableEntry e{};e.handle=handle;e.name=b.name;e.stepCount=b.stepCount;e.step=e.sent=b.step;e.seenAt=tick;
        for(const auto& old:s.entries)if(old.handle==handle&&old.name==b.name){e.sent=(std::max)(old.sent,b.step);e.sends=old.sends;e.applied=old.applied;break;}
        // 1214is (#179): 4096, was 1024. With movables Edge of the World has 1269 breakables (mission-breakables-v1774a),
        // and the catalog stopped at 1024 (mission-breakables-v1778a): later entries were never polled.
        next.push_back(std::move(e));if(next.size()>=4096){++s.catalogFull;break;}
    }
    s.entries=std::move(next);++s.scans;s.scanMs=static_cast<unsigned>(GetTickCount64()-started);
}
bool advanceNativeBreakable(const NativeBreakable& b,const BreakableRemoteStep& r){
    static std::uintptr_t decl=0;const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t vt=0;
    if(!decl)decl=reinterpret_cast<std::uintptr_t(*)(void*,const char*,bool,bool)>(gameImage+0x1e030)(gameImage+0x3990b60,"components/characters/player/base/damage/player_sword.damage",false,false);
    if(!decl||!memoryRead(decl,&vt,8)||vt!=base+0x1c5d138)return false;
    alignas(16) std::array<unsigned char,144> params{};
    reinterpret_cast<void(*)(void*,void*)>(gameImage+0x780260)(params.data(),reinterpret_cast<void*>(decl));
    float hit[3]{},direction[3]{0,0,-1};
    if(r.hitValid){std::memcpy(hit,r.hit,12);std::memcpy(direction,r.direction,12);}
    else if(!memoryRead(b.actor+0x300,hit,12))return false;
    const std::uint32_t none=0;std::memcpy(params.data()+0x24,hit,12);std::memcpy(params.data()+0x3c,direction,12);
    std::memcpy(params.data()+0x50,&none,4);std::memcpy(params.data()+0x54,&none,4);
    reinterpret_cast<void(*)(void*,void*)>(gameImage+0x7ed000)(reinterpret_cast<void*>(b.component),params.data());
    return true;
}
void applyBreakableStep(BreakableEntry& e,const BreakableRemoteStep& r){
    auto& s=breakableSync;const auto base=reinterpret_cast<std::uintptr_t>(gameImage);NativeBreakable b{};
    if(!readNativeBreakable(memoryRead,base,observedGame,e.handle,b)||b.stepCount!=r.stepCount){++s.applyRefused;s.lastApply="breakable_changed";return;}
    e.sent=(std::max)(e.sent,r.step);
    const int missing=breakableStepsToApply(b.step,r.step,b.stepCount);if(!missing){s.lastApply="already_at_step";return;}
    for(int k=0;k<missing;++k){const int before=b.step;
        if(!advanceNativeBreakable(b,r)||!readNativeBreakable(memoryRead,base,observedGame,e.handle,b)||b.step!=before+1){++s.applyRefused;s.lastApply="native_step_refused";break;}
        ++s.appliedSteps;
    }
    e.step=b.step;
    float threshold=0;
    if(b.step>0&&b.health&&readNativeBreakableThreshold(memoryRead,b,b.step-1,threshold)&&b.current>threshold&&threshold>=0){
        *reinterpret_cast<float*>(b.health+0x20)=threshold;++s.healthLowered;}
    if(b.step>=r.step){++s.applied;++e.applied;s.lastApply="applied";}
}
void receiveBreakablePacket(const CoopBreakablePacket& p,ULONGLONG){
    auto& s=breakableSync;
    if(!s.enabled){++s.ignored;return;}
    if(!validCoopBreakable(p,outgoing,remote)){++s.rejected;return;}
    ++s.received;if(s.pending.size()>=64){++s.overflow;return;}
    BreakableRemoteStep r{};r.handle=p.handle;r.step=static_cast<int>(p.step);r.stepCount=static_cast<int>(p.steps);r.hitValid=(p.flags&1)!=0;
    std::memcpy(r.hit,p.hit,12);std::memcpy(r.direction,p.direction,12);r.name.assign(p.name,strnlen(p.name,sizeof(p.name)));
    s.pending.push_back(std::move(r));
}
bool sendBreakableStep(BreakableEntry& e,int step,ULONGLONG tick){
    auto& s=breakableSync;CoopBreakablePacket p{};p.role=localRole;p.handle=e.handle;p.step=static_cast<std::uint32_t>(step);
    p.steps=static_cast<std::uint32_t>(e.stepCount);p.sequence=++s.sequence;p.sourceSession=outgoing.session;p.targetSession=remote.session;
    p.sourceEpoch=outgoing.worldEpoch;p.targetEpoch=remote.worldEpoch;p.mapId=outgoing.mapId;
    if(e.name.size()>=sizeof(p.name))return false;std::memcpy(p.name,e.name.c_str(),e.name.size());
    BreakableHit h{};
    if(latestBreakableHit(e.handle,tick>=5000?tick-5000:0,h)&&validBreakableVector(h.hit,100000.f)&&validBreakableVector(h.direction,2.f)){
        p.flags=1;std::memcpy(p.hit,h.hit,12);std::memcpy(p.direction,h.direction,12);}
    if(!validCoopBreakable(p,remote,outgoing)||coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)!=k_EResultOK){++s.sendFailures;return false;}
    ++s.sent;if(p.flags)++s.sentWithHit;++e.sends;return true;
}
void serviceBreakableSync(ULONGLONG tick){
    auto& s=breakableSync;
    if(breakableSyncEnableRequest>=0){s.enabled=breakableSyncEnableRequest!=0;breakableSyncEnableRequest=-1;s.entries.clear();s.pending.clear();s.scanAt=0;s.result=s.enabled?"waiting_world":"off";}
    if(!s.enabled)return;
    if(!breakableSyncContext()||!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load()){s.result="waiting_world";return;}
    if(s.epoch!=outgoing.worldEpoch||s.peerEpoch!=remote.worldEpoch||s.map!=outgoing.mapId||s.session!=outgoing.session||s.peerSession!=remote.session){
        s.epoch=outgoing.worldEpoch;s.peerEpoch=remote.worldEpoch;s.map=outgoing.mapId;s.session=outgoing.session;s.peerSession=remote.session;
        s.entries.clear();s.pending.clear();s.scanAt=0;++s.worldResets;}
    if(!s.scanAt||tick-s.scanAt>=15000)scanBreakables(tick);
    for(const auto& r:s.pending){bool matched=false;
        for(auto& e:s.entries)if(e.handle==r.handle&&e.name==r.name){matched=true;applyBreakableStep(e,r);break;}
        if(!matched)++s.unmatched;}
    s.pending.clear();
    if(tick-s.pollAt>=100){s.pollAt=tick;const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
        for(auto& e:s.entries){NativeBreakable b{};
            if(!readNativeBreakable(memoryRead,base,observedGame,e.handle,b)||b.stepCount!=e.stepCount){
                // 1214io (#178): a movable on its final step is removed from the world at once (a smashed bottle), before a
                // poll reads the step. An entry that vanished within 5 s of a noted hit is sent as broken to its last step.
                BreakableHit h{};
                if(e.sent<e.stepCount&&!worldIdentityEntity(memoryRead,observedGame,e.handle)&&latestBreakableHit(e.handle,tick>=5000?tick-5000:0,h)&&
                   sendBreakableStep(e,e.stepCount,tick)){e.sent=e.step=e.stepCount;++s.vanishedSent;}
                continue;}
            e.step=b.step;if(b.step>e.sent&&sendBreakableStep(e,b.step,tick))e.sent=b.step;}
    }
    s.result="active";
}
void writeBreakableSync(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.breakable-sync.json",statusPath);std::snprintf(temp,sizeof(temp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;const auto& s=breakableSync;
    fprintf(f,"{\"enabled\":%s,\"result\":\"%s\",\"lastApply\":\"%s\",\"catalog\":%zu,\"scans\":%llu,\"scanMs\":%u,\"sent\":%llu,\"sentWithHit\":%llu,\"sendFailures\":%llu,"
        "\"received\":%llu,\"rejected\":%llu,\"ignored\":%llu,\"unmatched\":%llu,\"overflow\":%llu,\"applied\":%llu,\"appliedSteps\":%llu,\"applyRefused\":%llu,"
        "\"healthLowered\":%llu,\"worldResets\":%llu,\"hitsNoted\":%llu,\"vanishedSent\":%llu,\"catalogFull\":%llu,\"broken\":[",
        s.enabled?"true":"false",s.result,s.lastApply,s.entries.size(),s.scans,s.scanMs,s.sent,s.sentWithHit,s.sendFailures,s.received,s.rejected,s.ignored,
        s.unmatched,s.overflow,s.applied,s.appliedSteps,s.applyRefused,s.healthLowered,s.worldResets,breakableHitsNoted.load(),s.vanishedSent,s.catalogFull);
    bool first=true;for(const auto& e:s.entries){if(!e.step&&!e.sent)continue;if(!first)fputc(',',f);first=false;
        fprintf(f,"{\"handle\":%u,\"name\":",e.handle);writeWorldText(f,e.name.c_str());
        fprintf(f,",\"step\":%d,\"steps\":%d,\"sent\":%d,\"sends\":%llu,\"applied\":%llu}",e.step,e.stepCount,e.sent,e.sends,e.applied);}
    fputs("]}\n",f);const bool ok=!ferror(f);const auto closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
