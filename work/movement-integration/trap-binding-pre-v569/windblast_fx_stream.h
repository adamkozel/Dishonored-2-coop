#pragma once
#include "windblast_fx_protocol.h"
#include "native_windblast_fx_records.h"
struct WindblastFxStream {
    struct Pending {CoopWindblastFxPacket p{};double time=0;};
    std::array<Pending,8> pending{},ready{};unsigned pendingCount=0,readyCount=0;
    std::uint64_t token=0,peerToken=0,lastPowerEvent=0;unsigned nativeHighWater=0;
    ULONGLONG lastRead=0;bool baseline=false;WindblastFxOrder order{};CoopWindblastFxPacket last{};
} windblastFxStream;
std::uint64_t windblastFxSequence=0,windblastFxEvent=0,windblastFxSent=0,windblastFxReceived=0,windblastFxRejected=0,windblastFxApplied=0,windblastFxReadFailures=0;
bool windblastFxCurrent(ULONGLONG tick){return shadowStreamCurrent(tick)&&shadowStream.peerReady&&windblastFxStream.token==shadowStream.token&&windblastFxStream.peerToken==shadowStream.peerToken;}
void sampleNativeWindblastFx(ULONGLONG tick){
    if(!nativeMutationAllowed||!shadowStreamCurrent(tick)||!shadowStream.peerReady)return;
    auto& s=windblastFxStream;if(s.token!=shadowStream.token||s.peerToken!=shadowStream.peerToken){s={};s.token=shadowStream.token;s.peerToken=shadowStream.peerToken;}
    // Observe immediately after the corresponding native semantic sample.
    if(localRole||tick!=shadowStream.lastSend||tick-s.lastRead<33)return;s.lastRead=tick;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::vector<NativeWindblastFxRecord> records;
    if(!readNativeWindblastFxRecords(memoryRead,base,observedGame,records)){++windblastFxReadFailures;return;}
    const bool cast=s.baseline&&shadowStream.sourceState==10&&shadowStream.sourcePower==11&&shadowStream.sourceEvent!=s.lastPowerEvent&&
        shadowStreamLastPacket.sourceToken==s.token&&shadowStreamLastPacket.targetToken==s.peerToken&&shadowStreamLastPacket.event==shadowStream.sourceEvent&&
        shadowStreamLastPacket.powerVariant==11&&shadowStreamLastPacket.powerTimeMs<=1000&&tick-shadowStream.lastSend<70;
    const NativeWindblastFxRecord* candidate=nullptr;unsigned count=0,highest=s.nativeHighWater;
    for(const auto& r:records){highest=(std::max)(highest,r.id);
        if(cast&&r.id>s.nativeHighWater&&r.parent==1&&r.ownerActor&&r.ownerDefinition=="models/powers/windblast/windblast_object.def"&&
           r.name=="models/effects/gameplay/powers/windblast/blast/wblast_blast_01.fx"){candidate=&r;++count;}
    }
    // Baseline all already-existing records even if no cast is active. Re-arming
    // cannot replay an earlier native effect; ambiguous new effects fail closed.
    s.nativeHighWater=highest;s.baseline=true;if(count!=1)return;
    CoopWindblastFxPacket p{};p.sequence=++windblastFxSequence;p.event=++windblastFxEvent;p.nativeFxId=candidate->id;p.powerEvent=shadowStream.sourceEvent;p.castTimeMs=shadowStreamLastPacket.powerTimeMs;
    p.sourceSession=outgoing.session;p.targetSession=remote.session;p.sourceWorldEpoch=outgoing.worldEpoch;p.targetWorldEpoch=remote.worldEpoch;p.mapId=outgoing.mapId;
    p.sourceToken=s.token;p.targetToken=s.peerToken;p.sampleTimeUs=static_cast<std::uint64_t>(presentationClock()*1000.);p.rootSequence=outgoing.sequence;
    std::copy(candidate->origin,candidate->origin+3,p.position.begin());std::copy(candidate->axis,candidate->axis+9,p.axis.begin());
    s.lastPowerEvent=p.powerEvent;
    if(!validWindblastFxPacket(p,remote,outgoing)||!sockets||!connection||sockets->SendMessageToConnection(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)!=k_EResultOK){++windblastFxRejected;return;}
    s.last=p;++windblastFxSent;
}
void receiveNativeWindblastFx(const CoopWindblastFxPacket& p,ULONGLONG tick){
    ++windblastFxReceived;auto& s=windblastFxStream;const double time=double(p.sampleTimeUs)*.001+sourcePoseTimeline.offset,now=presentationClock();auto order=s.order;
    if(!nativeMutationAllowed||!windblastFxCurrent(tick)||!movementBankActive()||!validWindblastFxPacket(p,outgoing,remote)||
       p.sourceToken!=shadowStream.peerToken||p.targetToken!=shadowStream.token||sourcePoseTimeline.session!=p.sourceSession||
       sourcePoseTimeline.epoch!=p.sourceWorldEpoch||sourcePoseTimeline.map!=p.mapId||time>now+80||now-time>250||s.pendingCount>=s.pending.size()||!order.accept(p)){++windblastFxRejected;return;}
    s.order=order;s.pending[s.pendingCount++]={p,time};
}
void presentNativeWindblastFx(const PresentationPose& pose,ULONGLONG tick){
    if(!windblastFxCurrent(tick)||!movementBankActive())return;auto& s=windblastFxStream;
    while(s.pendingCount&&s.pending[0].time<=pose.time){
        const auto p=s.pending[0];for(unsigned i=1;i<s.pendingCount;++i)s.pending[i-1]=s.pending[i];--s.pendingCount;
        PowerPresentation::Sample power{};float amount=0;
        if(pose.time-p.time>250||s.readyCount>=s.ready.size()||!shadowStream.power.sample(pose.time,power,amount)||power.variant!=11||power.event!=p.p.powerEvent){++windblastFxRejected;continue;}
        s.ready[s.readyCount++]=p;
    }
}
void serviceNativeWindblastFx(ULONGLONG tick){
    if(!nativeMutationAllowed)return;auto& s=windblastFxStream;if(!windblastFxCurrent(tick)){s={};return;}
    for(unsigned i=0;i<s.readyCount;++i){const auto& e=s.ready[i];
        if(localRole==1&&movementBankActive()&&presentationClock()-e.time<400&&startNativePowerFx(8,e.p.position.data(),tick,true,e.p.axis.data()))++windblastFxApplied;else ++windblastFxRejected;
    }s.readyCount=0;
}
void duplicateNativeWindblastFx(){
    const auto& p=windblastFxStream.last;if(GetCurrentThreadId()==engineThread.load()&&windblastFxCurrent(GetTickCount64())&&p.sequence&&p.sourceToken==shadowStream.token&&p.targetToken==shadowStream.peerToken&&sockets&&connection)
        sockets->SendMessageToConnection(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr);
}
void writeWindblastFxStream(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};if(std::snprintf(path,sizeof(path),"%s.windblast-fx.json",statusPath)>=sizeof(path)||std::snprintf(tmp,sizeof(tmp),"%s.tmp",path)>=sizeof(tmp))return;
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;const auto& s=windblastFxStream;
    fprintf(f,"{\"sent\":%llu,\"received\":%llu,\"rejected\":%llu,\"applied\":%llu,\"readFailures\":%llu,\"pending\":%u,\"ready\":%u,\"baseline\":%s,\"lastNativeFx\":%u,\"sourceEvent\":%llu,\"receivedEvent\":%llu,\"sourcePosition\":[%.8f,%.8f,%.8f]}\n",windblastFxSent,windblastFxReceived,windblastFxRejected,windblastFxApplied,windblastFxReadFailures,s.pendingCount,s.readyCount,s.baseline?"true":"false",s.nativeHighWater,s.lastPowerEvent,s.order.event,s.last.position[0],s.last.position[1],s.last.position[2]);
    fclose(f);MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
