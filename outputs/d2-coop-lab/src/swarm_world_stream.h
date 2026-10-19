#pragma once
#include "swarm_world_protocol.h"
#include "swarm_world_presentation.h"
#include "native_swarm_world.h"
#include "swarm_trail_source.h"
void sampleSwarmSpawnFx(const NativeSwarmWorld&,const CoopSwarmWorldPacket&,ULONGLONG);
// Two-instance native measurements:80ms yielded84.4-86.8% bracketing.
// Give cosmetic rat presentation one extra engine frame of jitter margin.
constexpr double kSwarmPresentationDelayMs=120.;
struct SwarmWorldStream {
    struct Pending {CoopSwarmWorldPacket packet{};double time=0;};
    std::array<Pending,8> pending{};unsigned count=0,sourceHandle=0;
    std::uint64_t token=0,peerToken=0,sourceEvent=0;ULONGLONG nextSample=0,lease=0;
    NativeSwarmItem known{};bool discovered=false;SwarmWorldOrder order{};
    CoopSwarmWorldPacket sent{},presented{};double presentedTime=0;
} swarmWorldStream;
std::uint64_t swarmWorldSequence=0,swarmWorldEvent=0,swarmWorldSent=0,swarmWorldReceived=0,swarmWorldRejected=0,swarmWorldUnknown=0,swarmWorldPresented=0;
std::uint64_t swarmOlderSamples=0;unsigned swarmOlderAgents=0,swarmOlderHandle=0; // 1214ek: Double Swarm's older swarm
double swarmReadMaxMs=0,swarmScopedReadMaxMs=0;std::uint64_t swarmReadCount=0,swarmScopedReadCount=0;unsigned swarmReadStage=0;
struct SwarmWorldRecord {std::uint64_t sequence=0,event=0,digest=0;unsigned handle=0,count=0,bytes=0;};
std::array<SwarmWorldRecord,128> swarmWorldHistory{};std::uint64_t swarmWorldHistoryCount=0;
void recordSwarmWorld(const CoopSwarmWorldPacket& p){swarmWorldHistory[swarmWorldHistoryCount++%128]={p.sequence,p.event,swarmWorldDigest(p),p.nativeHandle,p.count,p.size};}
bool swarmWorldCurrent(ULONGLONG tick){return shadowStreamCurrent(tick)&&shadowStream.peerReady&&swarmWorldStream.token==shadowStream.token&&swarmWorldStream.peerToken==shadowStream.peerToken;}
void sampleSwarmWorld(ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;auto& s=swarmWorldStream;
    if(!shadowStreamCurrent(tick)||!shadowStream.peerReady){s={};return;}
    if(s.token!=shadowStream.token||s.peerToken!=shadowStream.peerToken){s={};s.token=shadowStream.token;s.peerToken=shadowStream.peerToken;}
    // At most ~30Hz once ownership is known, so the presentation buffer
    // normally contains a following sample even with one delayed engine frame.
    if(localRole!=0||tick<s.nextSample)return;s.nextSample=tick+(s.discovered?33:500);
    NativeSwarmWorld world{};const bool cached=s.discovered;const double start=presentationClock();
    const bool valid=readNativeSwarmWorld(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),world,&swarmReadStage,cached?&s.known:nullptr);
    const double elapsed=presentationClock()-start;swarmReadMaxMs=(std::max)(swarmReadMaxMs,elapsed);++swarmReadCount;
    if(cached){swarmScopedReadMaxMs=(std::max)(swarmScopedReadMaxMs,elapsed);++swarmScopedReadCount;}
    if(!valid){if(swarmReadStage==1)s.discovered=false;++swarmWorldUnknown;return;}
    if(world.equipment.instantiated){s.known=world.equipment;s.discovered=true;}
    const auto handle=world.present?world.handle:0u;
    if(!s.sourceEvent&&!handle){sampleSwarmSpawnFx(world,{},tick);CoopSwarmWorldPacket empty{};sampleSwarmTrails(world,empty);return;}
    if(handle!=s.sourceHandle||!s.sourceEvent){s.sourceHandle=handle;s.sourceEvent=++swarmWorldEvent;}
    CoopSwarmWorldPacket p{};p.sequence=++swarmWorldSequence;p.sourceSession=outgoing.session;p.targetSession=remote.session;
    p.sourceWorldEpoch=outgoing.worldEpoch;p.targetWorldEpoch=remote.worldEpoch;p.mapId=outgoing.mapId;p.sourceToken=s.token;p.targetToken=s.peerToken;
    p.event=s.sourceEvent;p.sampleTimeUs=static_cast<std::uint64_t>(presentationClock()*1000.);p.rootSequence=outgoing.sequence;p.nativeHandle=handle;p.present=world.present?1u:0u;
    if(!memoryRead(world.equipment.player.sword.actor+0x300,p.sourceOrigin.data(),12))return;
    for(const auto& a:world.agents){if(p.count>=100)return;p.agents[p.count++]={a.index,a.handle,a.incarnation,a.state,a.transform,a.scale,a.drivingFrame?1u:0u};}
    sampleSwarmTrails(world,p);
    // 1214ek: Double Swarm's older swarm rides in the same packet with its pool indices moved by 50 (mod 100); the newest
    // keeps its own (its trails and spawn effect stay keyed to it). An older rat whose moved index is taken is left out.
    swarmOlderAgents=0;swarmOlderHandle=0;
    if(world.present){NativeSwarmWorld older{};
        if(readNativeSwarmWorld(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),older,nullptr,&s.known,true)&&older.present&&older.handle!=world.handle){
            const unsigned newest=p.count;for(const auto& a:older.agents){if(p.count>=100)break;const unsigned index=(a.index+50)%100;
                if(std::any_of(p.agents.begin(),p.agents.begin()+newest,[&](const auto& n){return n.index==index;}))continue;
                p.agents[p.count++]={index,a.handle,a.incarnation,a.state,a.transform,a.scale,a.drivingFrame?1u:0u};++swarmOlderAgents;}
            ++swarmOlderSamples;swarmOlderHandle=older.handle;}}
    p.size=static_cast<std::uint16_t>(swarmWorldWireSize(p.count));
    if(!validSwarmWorldPacket(p,remote,outgoing)||!sockets||!connection)return;
    if(coopSendMessage(connection,&p,p.size,k_nSteamNetworkingSend_UnreliableNoDelay,nullptr)==k_EResultOK){s.sent=p;recordSwarmWorld(p);++swarmWorldSent;sampleSwarmSpawnFx(world,p,tick);}
}
void receiveSwarmWorld(const CoopSwarmWorldPacket& p,ULONGLONG tick){
    ++swarmWorldReceived;auto& s=swarmWorldStream;
    if(!nativeMutationAllowed||!swarmWorldCurrent(tick)||!validSwarmWorldPacket(p,outgoing,remote)||p.sourceToken!=shadowStream.peerToken||
       p.targetToken!=shadowStream.token||!movementBankActive()||s.count>=8){++swarmWorldRejected;return;}
    const double now=presentationClock(),mapped=double(p.sampleTimeUs)*.001+sourcePoseTimeline.offset;auto order=s.order;
    if(sourcePoseTimeline.session!=p.sourceSession||sourcePoseTimeline.epoch!=p.sourceWorldEpoch||sourcePoseTimeline.map!=p.mapId||
       mapped>now+80||now-mapped>250||!order.accept(p)){++swarmWorldRejected;return;}
    s.order=order;s.pending[s.count++]={p,mapped};recordSwarmWorld(p);
}
void presentSwarmWorld(ULONGLONG tick){
    auto& s=swarmWorldStream;if(!swarmWorldCurrent(tick)||!movementBankActive()||!coopGameplayReady(outgoing)||!coopGameplayReady(remote)){s.lease=0;return;}
    const double now=presentationClock()-kSwarmPresentationDelayMs;while(s.count&&s.pending[0].time<=now){auto p=s.pending[0];for(unsigned i=1;i<s.count;++i)s.pending[i-1]=s.pending[i];--s.count;
        if(now-p.time>250){++swarmWorldRejected;continue;}s.presented=p.packet;s.presentedTime=p.time;s.lease=tick+300;++swarmWorldPresented;
    }
}
void writeSwarmWorld(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.swarm-world.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;const auto& s=swarmWorldStream;const auto& p=localRole==0?s.sent:s.presented;
    fprintf(f,"{\"sent\":%llu,\"received\":%llu,\"rejected\":%llu,\"unknown\":%llu,\"presented\":%llu,\"sequence\":%llu,\"event\":%llu,\"nativeHandle\":%u,\"present\":%u,\"count\":%u,\"bytes\":%u,\"leased\":%s,\"readCount\":%llu,\"scopedReadCount\":%llu,\"readMaxMs\":%.4f,\"scopedReadMaxMs\":%.4f,\"readStage\":%u,\"olderSamples\":%llu,\"olderAgents\":%u,\"olderHandle\":%u,\"agents\":[",swarmWorldSent,swarmWorldReceived,swarmWorldRejected,swarmWorldUnknown,swarmWorldPresented,p.sequence,p.event,p.nativeHandle,p.present,p.count,p.size,s.lease>GetTickCount64()?"true":"false",swarmReadCount,swarmScopedReadCount,swarmReadMaxMs,swarmScopedReadMaxMs,swarmReadStage,swarmOlderSamples,swarmOlderAgents,swarmOlderHandle);
    for(unsigned i=0;i<p.count;++i){const auto& a=p.agents[i];fprintf(f,"%s{\"index\":%u,\"handle\":%u,\"incarnation\":%u,\"state\":%u,\"directDriver\":%u,\"scale\":%.6f,\"transform\":[",i?",":"",a.index,a.handle,a.incarnation,a.state,a.directDriver,a.scale);
        for(unsigned k=0;k<12;++k)fprintf(f,"%s%.6f",k?",":"",a.transform[k]);fputs("]}",f);}
    fputs("],\"history\":[",f);const auto n=(std::min)(swarmWorldHistoryCount,128ull);
    for(std::uint64_t i=0;i<n;++i){const auto& r=swarmWorldHistory[(swarmWorldHistoryCount-n+i)%128];
        fprintf(f,"%s{\"sequence\":%llu,\"event\":%llu,\"digest\":\"%016llx\",\"handle\":%u,\"count\":%u,\"bytes\":%u}",i?",":"",r.sequence,r.event,r.digest,r.handle,r.count,r.bytes);}
    fputs("]}\n",f);const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
