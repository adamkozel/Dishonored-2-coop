#pragma once
#include "swarm_spawn_fx_protocol.h"
#include "native_swarm_spawn_fx.h"
struct SwarmSpawnFxStream {
    struct Pending {CoopSwarmSpawnFxPacket packet{};double time=0;};
    std::array<Pending,8> pending{};unsigned count=0;
    std::uint64_t token=0,peerToken=0,appliedWorldEvent=0;unsigned appliedWorldHandle=0;
    SwarmSpawnDiscovery discovery{};SwarmSpawnFxOrder order{};
    CoopSwarmSpawnFxPacket last{},applied{};
} swarmSpawnFxStream;
std::uint64_t swarmSpawnFxSequence=0,swarmSpawnFxEvent=0,swarmSpawnFxSent=0,swarmSpawnFxReceived=0,swarmSpawnFxRejected=0,swarmSpawnFxApplied=0;
std::uint64_t swarmSpawnFxReads=0,swarmSpawnFxReadFailures=0;double swarmSpawnFxReadMaxMs=0;
bool swarmSpawnFxCurrent(ULONGLONG tick){return shadowStreamCurrent(tick)&&shadowStream.peerReady&&swarmSpawnFxStream.token==shadowStream.token&&swarmSpawnFxStream.peerToken==shadowStream.peerToken;}
void sampleSwarmSpawnFx(const NativeSwarmWorld& world,const CoopSwarmWorldPacket& packet,ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load()||localRole||!shadowStreamCurrent(tick)||!shadowStream.peerReady)return;
    auto& s=swarmSpawnFxStream;
    if(s.token!=shadowStream.token||s.peerToken!=shadowStream.peerToken){s={};s.token=shadowStream.token;s.peerToken=shadowStream.peerToken;}
    const double now=presentationClock();s.discovery.observe(world.present?world.handle:0,now);
    const auto& action=shadowStreamLastPacket;
    const bool cast=packet.present&&packet.event&&packet.nativeHandle==world.handle&&shadowStream.sourceState==10&&shadowStream.sourcePower==25&&
        action.sourceToken==s.token&&action.targetToken==s.peerToken&&action.event==shadowStream.sourceEvent&&
        action.powerVariant==25&&action.powerTimeMs<=1000&&tick-shadowStream.lastSend<70;
    if(!s.discovery.mayRead(now,cast))return;++s.discovery.attempts;++swarmSpawnFxReads;
    NativeWindblastFxRecord fx{};const double began=presentationClock();
    const bool valid=readNativeSwarmSpawnFx(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),world,fx);
    swarmSpawnFxReadMaxMs=(std::max)(swarmSpawnFxReadMaxMs,presentationClock()-began);
    if(!valid){++swarmSpawnFxReadFailures;return;}
    s.discovery.consumed=true;
    CoopSwarmSpawnFxPacket p{};p.sequence=++swarmSpawnFxSequence;p.event=++swarmSpawnFxEvent;p.nativeFxId=fx.id;p.powerEvent=action.event;
    p.worldEvent=packet.event;p.worldHandle=world.handle;p.castTimeMs=action.powerTimeMs;
    p.sourceSession=outgoing.session;p.targetSession=remote.session;p.sourceWorldEpoch=outgoing.worldEpoch;p.targetWorldEpoch=remote.worldEpoch;p.mapId=outgoing.mapId;
    p.sourceToken=s.token;p.targetToken=s.peerToken;p.sampleTimeUs=static_cast<std::uint64_t>(presentationClock()*1000.);p.rootSequence=outgoing.sequence;
    std::copy(fx.origin,fx.origin+3,p.position.begin());std::copy(fx.axis,fx.axis+9,p.axis.begin());
    if(!validSwarmSpawnFxPacket(p,remote,outgoing)||!sockets||!connection||sockets->SendMessageToConnection(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)!=k_EResultOK){++swarmSpawnFxRejected;return;}
    s.last=p;++swarmSpawnFxSent;
}
void receiveSwarmSpawnFx(const CoopSwarmSpawnFxPacket& p,ULONGLONG tick){
    ++swarmSpawnFxReceived;auto& s=swarmSpawnFxStream;
    if(s.token!=shadowStream.token||s.peerToken!=shadowStream.peerToken){s={};s.token=shadowStream.token;s.peerToken=shadowStream.peerToken;}
    const double mapped=double(p.sampleTimeUs)*.001+sourcePoseTimeline.offset,now=presentationClock();auto order=s.order;
    if(!nativeMutationAllowed||!swarmSpawnFxCurrent(tick)||!movementBankActive()||!validSwarmSpawnFxPacket(p,outgoing,remote)||
       p.sourceToken!=shadowStream.peerToken||p.targetToken!=shadowStream.token||sourcePoseTimeline.session!=p.sourceSession||
       sourcePoseTimeline.epoch!=p.sourceWorldEpoch||sourcePoseTimeline.map!=p.mapId||mapped>now+80||now-mapped>250||s.count>=s.pending.size()||!order.accept(p)){++swarmSpawnFxRejected;return;}
    s.order=order;s.pending[s.count++]={p,mapped};
}
void serviceSwarmSpawnFx(ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    auto& s=swarmSpawnFxStream;if(!swarmSpawnFxCurrent(tick)){s={};return;}if(localRole!=1)return;
    const auto& world=swarmWorldStream.presented;
    const bool worldLive=swarmWorldCurrent(tick)&&swarmWorldStream.lease>tick&&world.present;
    if(s.appliedWorldEvent&&(!worldLive||world.event!=s.appliedWorldEvent||world.nativeHandle!=s.appliedWorldHandle)){
        for(auto& fx:nativePowerEffects)if(fx.expires&&fx.kind==20){
            if(nativePowerFxRecord(fx))reinterpret_cast<void(*)(std::uintptr_t,unsigned)>(gameImage+0x1a31f0)(nativePowerFxManager(),fx.id);
            fx={};++powerFxStopped;
        }
        s.appliedWorldEvent=0;s.appliedWorldHandle=0;
    }
    const double now=presentationClock(),at=now-kSwarmPresentationDelayMs;
    while(s.count&&s.pending[0].time<=at){
        const auto next=s.pending[0];const auto& p=next.packet;
        // Reliable FX can arrive before the unreliable world frame. Wait only
        // inside the same finite presentation lease, without resolving its ID.
        if(now-next.time<350&&(!worldLive||world.event<p.worldEvent))break;
        for(unsigned i=1;i<s.count;++i)s.pending[i-1]=s.pending[i];--s.count;
        PowerPresentation::Sample power{};float amount=0;
        if(now-next.time>=400||!movementBankActive()||!worldLive||world.event!=p.worldEvent||world.nativeHandle!=p.worldHandle||
           !shadowStream.power.sample(next.time,power,amount)||power.variant!=25||power.event!=p.powerEvent){++swarmSpawnFxRejected;continue;}
        // Consume once even if the native cosmetic start fails. No retry can
        // turn a lingering source record into a repeated emitter/sound.
        if(startNativePowerFx(20,p.position.data(),tick,true,p.axis.data())){
            ++swarmSpawnFxApplied;s.applied=p;s.appliedWorldEvent=p.worldEvent;s.appliedWorldHandle=p.worldHandle;
        }else ++swarmSpawnFxRejected;
    }
}
void duplicateSwarmSpawnFx(){const auto& p=swarmSpawnFxStream.last;
    if(GetCurrentThreadId()==engineThread.load()&&swarmSpawnFxCurrent(GetTickCount64())&&p.sequence&&sockets&&connection)
        sockets->SendMessageToConnection(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr);
}
void writeSwarmSpawnFx(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.swarm-spawn-fx.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;const auto& s=swarmSpawnFxStream;const auto& p=localRole?s.applied:s.last;
    fprintf(f,"{\"sent\":%llu,\"received\":%llu,\"applied\":%llu,\"rejected\":%llu,\"reads\":%llu,\"readFailures\":%llu,\"readMaxMs\":%.6f,\"pending\":%u,\"baseline\":%s,\"consumed\":%s,\"sequence\":%llu,\"event\":%llu,\"nativeFxId\":%u,\"worldEvent\":%llu,\"worldHandle\":%u,\"powerEvent\":%llu,\"castTimeMs\":%u,\"sampleTimeUs\":%llu,\"position\":[",swarmSpawnFxSent,swarmSpawnFxReceived,swarmSpawnFxApplied,swarmSpawnFxRejected,swarmSpawnFxReads,swarmSpawnFxReadFailures,swarmSpawnFxReadMaxMs,s.count,s.discovery.baseline?"true":"false",s.discovery.consumed?"true":"false",p.sequence,p.event,p.nativeFxId,p.worldEvent,p.worldHandle,p.powerEvent,p.castTimeMs,p.sampleTimeUs);
    for(unsigned i=0;i<3;++i)fprintf(f,"%s%.8f",i?",":"",p.position[i]);fputs("],\"axis\":[",f);
    for(unsigned i=0;i<9;++i)fprintf(f,"%s%.8f",i?",":"",p.axis[i]);fputs("]}\n",f);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
