#pragma once
#include "windblast_fx_protocol.h"
#include "native_windblast_fx_records.h"
struct WindblastFxStream {
    struct Pending {CoopWindblastFxPacket p{};double time=0;};
    std::array<Pending,8> pending{},ready{};unsigned pendingCount=0,readyCount=0;
    std::uint64_t token=0,peerToken=0,lastPowerEvent=0;unsigned nativeHighWater=0;
    ULONGLONG lastRead=0;bool baseline=false;WindblastFxOrder order{};CoopWindblastFxPacket last{};
} windblastFxStream;
std::uint64_t windblastFxSequence=0,windblastFxEvent=0,windblastFxSent=0,windblastFxReceived=0,windblastFxRejected=0,windblastFxApplied=0,windblastFxReadFailures=0,windblastFxWaves=0;
// 1214ei: new windblast-named effects the source saw while variant 11 ran, with its action and power.
struct WindblastFxSeen{unsigned id=0,parent=0,action=0,power=0;char name[112]{},owner[72]{};float origin[3]{};};
std::array<WindblastFxSeen,16> windblastFxSeen{};std::uint64_t windblastFxSeenCount=0;unsigned windblastFxSeenHigh=0;
bool windblastFxCurrent(ULONGLONG tick){return shadowStreamCurrent(tick)&&shadowStream.peerReady&&windblastFxStream.token==shadowStream.token&&windblastFxStream.peerToken==shadowStream.peerToken;}
void sampleNativeWindblastFx(ULONGLONG tick){
    if(!nativeMutationAllowed||!shadowStreamCurrent(tick)||!shadowStream.peerReady)return;
    auto& s=windblastFxStream;if(s.token!=shadowStream.token||s.peerToken!=shadowStream.peerToken){s={};s.token=shadowStream.token;s.peerToken=shadowStream.peerToken;}
    // Observe immediately after the corresponding native semantic sample.
    if(localCharacter()!=1||tick!=shadowStream.lastSend||tick-s.lastRead<33)return;s.lastRead=tick; // 1214ia (#159)
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::vector<NativeWindblastFxRecord> records;
    if(!readNativeWindblastFxRecords(memoryRead,base,observedGame,records)){++windblastFxReadFailures;return;}
    if(shadowStream.sourcePower==11){std::vector<NativeWindblastFxRecord> named;unsigned high=windblastFxSeenHigh;
        if(readNativeFilteredFxRecords(memoryRead,base,observedGame,named,[](const std::string& n){return n.find("wblast")!=std::string::npos||n.find("windblast")!=std::string::npos||n.find("shockwave")!=std::string::npos;}))
            for(const auto& r:named){high=(std::max)(high,r.id);if(r.id<=windblastFxSeenHigh)continue;auto& w=windblastFxSeen[windblastFxSeenCount++%16];w={};
                w.id=r.id;w.parent=r.parent;w.action=shadowStream.sourceState;w.power=shadowStream.sourcePower;std::copy(r.origin,r.origin+3,w.origin);
                strncpy_s(w.name,r.name.c_str(),_TRUNCATE);strncpy_s(w.owner,r.ownerDefinition.c_str(),_TRUNCATE);}
        windblastFxSeenHigh=high;}
    const bool cast=s.baseline&&shadowStream.sourceState==10&&shadowStream.sourcePower==11&&shadowStream.sourceEvent!=s.lastPowerEvent&&
        shadowStreamLastPacket.sourceToken==s.token&&shadowStreamLastPacket.targetToken==s.peerToken&&shadowStreamLastPacket.event==shadowStream.sourceEvent&&
        shadowStreamLastPacket.powerVariant==11&&shadowStreamLastPacket.powerTimeMs<=1000&&tick-shadowStream.lastSend<70;
    // 1214ej (#11 power_upgrade_variants): the blast of each Windblast object (plain, Greater, Shockwave; in 1500a the
    // upgrades own windblast_greater_object and windblast_shockwave_object, and Greater draws wblast_stronger_blast_01)
    // and the Shockwave's ground wave (wblast_wave_blast_01 of the same object).
    const auto variantOf=[](const std::string& owner){return owner=="models/powers/windblast/windblast_object.def"?1u:owner=="models/powers/windblast/windblast_greater_object.def"?2u:
        owner=="models/powers/windblast/windblast_shockwave_object.def"?3u:0u;};
    const NativeWindblastFxRecord* candidate=nullptr;const NativeWindblastFxRecord* wave=nullptr;unsigned count=0,waves=0,highest=s.nativeHighWater;
    for(const auto& r:records){highest=(std::max)(highest,r.id);
        if(!cast||r.id<=s.nativeHighWater||r.parent!=1||!r.ownerActor)continue;const unsigned v=variantOf(r.ownerDefinition);
        if(v&&r.name==(v==2?"models/effects/gameplay/powers/windblast/blast/wblast_stronger_blast_01.fx":"models/effects/gameplay/powers/windblast/blast/wblast_blast_01.fx")){candidate=&r;++count;}
        if(v==3&&r.name=="models/effects/gameplay/powers/windblast/blast/wblast_wave_blast_01.fx"){wave=&r;++waves;}
    }
    // Baseline all already-existing records even if no cast is active. Re-arming
    // cannot replay an earlier native effect; ambiguous new effects fail closed.
    s.nativeHighWater=highest;s.baseline=true;if(count!=1)return;
    CoopWindblastFxPacket p{};p.role=localRole;p.sequence=++windblastFxSequence;p.event=++windblastFxEvent;p.nativeFxId=candidate->id;p.powerEvent=shadowStream.sourceEvent;p.castTimeMs=shadowStreamLastPacket.powerTimeMs;
    p.sourceSession=outgoing.session;p.targetSession=remote.session;p.sourceWorldEpoch=outgoing.worldEpoch;p.targetWorldEpoch=remote.worldEpoch;p.mapId=outgoing.mapId;
    p.sourceToken=s.token;p.targetToken=s.peerToken;p.sampleTimeUs=static_cast<std::uint64_t>(presentationClock()*1000.);p.rootSequence=outgoing.sequence;
    std::copy(candidate->origin,candidate->origin+3,p.position.begin());std::copy(candidate->axis,candidate->axis+9,p.axis.begin());
    p.variant=variantOf(candidate->ownerDefinition);
    if(p.variant==3&&waves==1){p.waveValid=1;std::copy(wave->origin,wave->origin+3,p.wave.begin());std::copy(wave->axis,wave->axis+9,p.waveAxis.begin());}
    s.lastPowerEvent=p.powerEvent;
    if(!validWindblastFxPacket(p,remote,outgoing)||!sockets||!connection||coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)!=k_EResultOK){++windblastFxRejected;return;}
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
        if(remoteIsCorvo()&&movementBankActive()&&presentationClock()-e.time<400&&startNativePowerFx(e.p.variant==2?30:8,e.p.position.data(),tick,true,e.p.axis.data())){++windblastFxApplied;
            if(e.p.waveValid){if(startNativePowerFx(29,e.p.wave.data(),tick,true,e.p.waveAxis.data()))++windblastFxWaves;else ++windblastFxRejected;}}
        else ++windblastFxRejected;
    }s.readyCount=0;
}
void duplicateNativeWindblastFx(){
    const auto& p=windblastFxStream.last;if(GetCurrentThreadId()==engineThread.load()&&windblastFxCurrent(GetTickCount64())&&p.sequence&&p.sourceToken==shadowStream.token&&p.targetToken==shadowStream.peerToken&&sockets&&connection)
        coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr);
}
void writeWindblastFxStream(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};if(std::snprintf(path,sizeof(path),"%s.windblast-fx.json",statusPath)>=sizeof(path)||std::snprintf(tmp,sizeof(tmp),"%s.tmp",path)>=sizeof(tmp))return;
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;const auto& s=windblastFxStream;
    fprintf(f,"{\"sent\":%llu,\"received\":%llu,\"rejected\":%llu,\"applied\":%llu,\"waves\":%llu,\"variant\":%u,\"readFailures\":%llu,\"pending\":%u,\"ready\":%u,\"baseline\":%s,\"lastNativeFx\":%u,\"sourceEvent\":%llu,\"receivedEvent\":%llu,\"sourcePosition\":[%.8f,%.8f,%.8f]}\n",windblastFxSent,windblastFxReceived,windblastFxRejected,windblastFxApplied,windblastFxWaves,s.last.variant,windblastFxReadFailures,s.pendingCount,s.readyCount,s.baseline?"true":"false",s.nativeHighWater,s.lastPowerEvent,s.order.event,s.last.position[0],s.last.position[1],s.last.position[2]);
    if(windblastFxSeenCount){fseek(f,-2,SEEK_CUR);fputs(",\"seen\":[",f);const auto n=(std::min)(windblastFxSeenCount,16ull);
        for(std::uint64_t i=0;i<n;++i){const auto& w=windblastFxSeen[(windblastFxSeenCount-n+i)%16];
            fprintf(f,"%s{\"id\":%u,\"parent\":%u,\"action\":%u,\"power\":%u,\"name\":\"%s\",\"owner\":\"%s\",\"origin\":[%.3f,%.3f,%.3f]}",i?",":"",w.id,w.parent,w.action,w.power,w.name,w.owner,w.origin[0],w.origin[1],w.origin[2]);}
        fputs("]}\n",f);}
    fclose(f);MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
