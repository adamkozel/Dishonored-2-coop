#pragma once
#include "possession_fx_protocol.h"
#include "native_possession_target_fx.h"
struct PossessionFxStream {
    struct Pending {CoopPossessionFxPacket packet{};double time=0;};
    std::array<Pending,8> pending{};unsigned count=0,sourceTarget=0,sourceId=0;
    std::uint64_t token=0,peerToken=0,sourceEvent=0;ULONGLONG nextSample=0,lease=0;
    PossessionFxOrder order{};CoopPossessionFxPacket presented{},sent{};
} possessionFxStream;
std::uint64_t possessionFxSequence=0,possessionFxEvent=0,possessionFxSent=0,possessionFxReceived=0,possessionFxRejected=0,possessionFxUnknown=0,possessionFxPresented=0,possessionFxReads=0;
double possessionFxReadMaxMs=0;unsigned possessionFxReadStage=0;
bool possessionFxCurrent(ULONGLONG tick){return shadowStreamCurrent(tick)&&shadowStream.peerReady&&possessionFxStream.token==shadowStream.token&&possessionFxStream.peerToken==shadowStream.peerToken;}
void samplePossessionFx(ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;auto& s=possessionFxStream;
    if(!shadowStreamCurrent(tick)||!shadowStream.peerReady){s={};return;}
    if(s.token!=shadowStream.token||s.peerToken!=shadowStream.peerToken){s={};s.token=shadowStream.token;s.peerToken=shadowStream.peerToken;}
    if(localCharacter()!=1||tick<s.nextSample)return;s.nextSample=tick+80; // 1214ia (#159)
    NativePossessionTargetFx native{};const double start=presentationClock();
    const bool valid=readNativePossessionTargetFx(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),native,&possessionFxReadStage);
    possessionFxReadMaxMs=(std::max)(possessionFxReadMaxMs,presentationClock()-start);++possessionFxReads;
    if(!valid){++possessionFxUnknown;return;} // Unknown never becomes an invented target; lease expires.
    const auto target=native.present?native.handle:0u,id=native.present?native.id:0u;
    if(!s.sourceEvent&&!id)return;
    if(target!=s.sourceTarget||id!=s.sourceId||!s.sourceEvent){s.sourceTarget=target;s.sourceId=id;s.sourceEvent=++possessionFxEvent;}
    if(native.actor==probeActor||native.handle==probeHandle)return;
    CoopPossessionFxPacket p{};p.role=localRole;p.sequence=++possessionFxSequence;p.sourceSession=outgoing.session;p.targetSession=remote.session;
    p.sourceWorldEpoch=outgoing.worldEpoch;p.targetWorldEpoch=remote.worldEpoch;p.mapId=outgoing.mapId;p.sourceToken=s.token;p.targetToken=s.peerToken;
    p.event=s.sourceEvent;p.sampleTimeUs=static_cast<std::uint64_t>(presentationClock()*1000.);p.rootSequence=outgoing.sequence;p.target=target;p.id=id;p.present=native.present?1u:0u;
    if(!memoryRead(native.equipment.player.sword.actor+0x300,p.sourceOrigin.data(),12))return;
    if(native.present){std::memcpy(p.transform.data(),native.effect.origin,12);std::memcpy(p.transform.data()+3,native.effect.axis,36);}
    if(!validPossessionFxPacket(p,remote,outgoing)||!sockets||!connection)return;
    if(coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_UnreliableNoDelay,nullptr)==k_EResultOK){s.sent=p;++possessionFxSent;}
}
void receivePossessionFx(const CoopPossessionFxPacket& p,ULONGLONG tick){
    ++possessionFxReceived;auto& s=possessionFxStream;
    if(!nativeMutationAllowed||!possessionFxCurrent(tick)||!validPossessionFxPacket(p,outgoing,remote)||p.sourceToken!=shadowStream.peerToken||p.targetToken!=shadowStream.token||!movementBankActive()||s.count>=8){++possessionFxRejected;return;}
    const double now=presentationClock(),mapped=double(p.sampleTimeUs)*.001+sourcePoseTimeline.offset;auto order=s.order;
    if(sourcePoseTimeline.session!=p.sourceSession||sourcePoseTimeline.epoch!=p.sourceWorldEpoch||sourcePoseTimeline.map!=p.mapId||mapped>now+80||now-mapped>250||!order.accept(p)){++possessionFxRejected;return;}
    s.order=order;s.pending[s.count++]={p,mapped};
}
void presentPossessionFx(ULONGLONG tick){
    auto& s=possessionFxStream;if(!possessionFxCurrent(tick)||!movementBankActive()||!coopGameplayReady(outgoing)||!coopGameplayReady(remote)){s.lease=0;return;}
    const double now=presentationClock()-80.;while(s.count&&s.pending[0].time<=now){auto p=s.pending[0];for(unsigned i=1;i<s.count;++i)s.pending[i-1]=s.pending[i];--s.count;
        if(now-p.time>250){++possessionFxRejected;continue;}s.presented=p.packet;s.lease=tick+300;++possessionFxPresented;
    }
}
struct PossessionPresentedFx {NativePowerFx fx{};unsigned sourceId=0,target=0;std::uint64_t token=0,peerToken=0,event=0;} possessionPresentedFx;
std::uint64_t possessionFxStarted=0,possessionFxStopped=0,possessionFxUpdated=0,possessionFxRefused=0,possessionFxRetired=0,possessionFxAppliedSequence=0;
void stopPossessionTargetFx(){
    auto& m=possessionPresentedFx;if(!m.sourceId)return;
    if(m.fx.game!=observedGame||m.fx.epoch!=outgoing.worldEpoch){m={};++possessionFxRetired;return;}
    if(nativePowerFxRecord(m.fx))reinterpret_cast<void(*)(std::uintptr_t,unsigned)>(gameImage+0x1a31f0)(nativePowerFxManager(),m.fx.id);
    m={};++possessionFxStopped;
}
void servicePossessionTargetFx(ULONGLONG tick){
    if(!nativePowerFxApi())return;auto& m=possessionPresentedFx;const auto& s=possessionFxStream;const auto& p=s.presented;
    if(m.sourceId&&(m.fx.game!=observedGame||m.fx.epoch!=outgoing.worldEpoch)){m={};++possessionFxRetired;}
    if(!coopGameplayReady(outgoing))return; // Native pause freezes world; cleanup before resume playback.
    const bool ready=remoteIsCorvo()&&possessionFxCurrent(tick)&&s.lease>tick&&movementBankActive()&&coopGameplayReady(remote)&&
        // 1214ej: product sessions too, on any paired map.
        ((!std::strcmp(mapName,kGuardMap)&&observerIsolation.ready&&observerIsolation.movementFixture)||productSessionArmed())&&probeDoppelganger&&avatarAiHeld&&probeFollowExact&&
        avatarAiTarget.actor==probeActor&&worldIdentityEntity(memoryRead,observedGame,probeHandle)==probeActor;
    const bool snapshot=ready&&p.sequence!=possessionFxAppliedSequence;
    if(m.sourceId&&(!ready||!p.present||m.fx.actor!=probeActor||m.fx.owner!=probeHandle||m.token!=s.token||m.peerToken!=s.peerToken||m.event!=p.event||m.sourceId!=p.id||m.target!=p.target))stopPossessionTargetFx();
    if(!snapshot)return;possessionFxAppliedSequence=p.sequence;if(!p.present)return;
    NativeSwordEquipment sword{};
    if(!readNativeSwordEquipment(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),probeHandle,sword,NativeSwordScope::DoppelgangerCorvo)||sword.actor!=probeActor||sword.game!=observedGame){++possessionFxRefused;return;}
    if(!m.sourceId){
        m.sourceId=p.id;m.target=p.target;m.token=s.token;m.peerToken=s.peerToken;m.event=p.event;
        auto& fx=m.fx;fx.game=observedGame;fx.epoch=outgoing.worldEpoch;fx.actor=probeActor;fx.owner=probeHandle;fx.kind=22;
        fx.declaration=nativePowerFxDefinition(22);if(!fx.declaration){m={};++possessionFxRefused;return;}
        using Start=unsigned*(*)(std::uintptr_t,unsigned*,std::uintptr_t,std::uintptr_t,std::uintptr_t,std::uintptr_t,const float*,const float*,std::uintptr_t,unsigned,std::uintptr_t,std::uintptr_t);
        reinterpret_cast<Start>(gameImage+0x87cb90)(fx.actor,&fx.id,0,0,0,fx.declaration,p.transform.data(),p.transform.data()+3,0,0,0,0);
        if(fx.id==~0u){m={};++possessionFxRefused;return;}fx.record=nativePowerFxRecord(fx);++possessionFxStarted;
    }
    const auto record=nativePowerFxRecord(m.fx);std::uintptr_t callback=1;
    if(!record||!memoryRead(record+0x128,&callback,8)||callback){stopPossessionTargetFx();++possessionFxRefused;return;}
    std::memcpy(reinterpret_cast<void*>(record+0x38),p.transform.data(),48);m.fx.expires=tick+300;++possessionFxUpdated;
}
void writePossessionFx(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.possession-fx.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;const auto& s=possessionFxStream;const auto& m=possessionPresentedFx;
    fprintf(f,"{\"sent\":%llu,\"received\":%llu,\"rejected\":%llu,\"unknown\":%llu,\"presented\":%llu,\"reads\":%llu,\"readMaxMs\":%.4f,\"readStage\":%u,\"started\":%llu,\"stopped\":%llu,\"updated\":%llu,\"refused\":%llu,\"retired\":%llu,\"sourceId\":%u,\"target\":%u,\"nativeId\":%u,\"nativeOwner\":%u,\"leased\":%s,\"lastSentId\":%u,\"lastPresentedId\":%u}\n",possessionFxSent,possessionFxReceived,possessionFxRejected,possessionFxUnknown,possessionFxPresented,possessionFxReads,possessionFxReadMaxMs,possessionFxReadStage,possessionFxStarted,possessionFxStopped,possessionFxUpdated,possessionFxRefused,possessionFxRetired,m.sourceId,m.target,m.fx.id,m.fx.owner,s.lease>GetTickCount64()?"true":"false",s.sent.id,s.presented.id);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
