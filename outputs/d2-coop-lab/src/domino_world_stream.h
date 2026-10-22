#pragma once
#include "domino_world_protocol.h"
#include "native_domino_links.h"
struct DominoWorldStream {
    struct Pending {CoopDominoWorldPacket packet{};double time=0;};
    std::array<Pending,8> pending{};unsigned count=0,sourceHandle=0;
    std::uint64_t token=0,peerToken=0,sourceEvent=0;ULONGLONG nextSample=0,lease=0;
    NativeDominoItem known{};bool discovered=false;DominoWorldOrder order{};CoopDominoWorldPacket presented{};
} dominoWorldStream;
std::uint64_t dominoWorldSequence=0,dominoWorldEvent=0,dominoWorldSent=0,dominoWorldReceived=0,dominoWorldRejected=0,dominoWorldUnknown=0,dominoWorldPresented=0,dominoWorldMarksOnly=0;
double dominoReadMaxMs=0,dominoScopedReadMaxMs=0;std::uint64_t dominoReadCount=0,dominoScopedReadCount=0;unsigned dominoReadStage=0;
bool dominoWorldCurrent(ULONGLONG tick){return shadowStreamCurrent(tick)&&shadowStream.peerReady&&dominoWorldStream.token==shadowStream.token&&dominoWorldStream.peerToken==shadowStream.peerToken;}
void sampleDominoWorld(ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;auto& s=dominoWorldStream;
    if(!shadowStreamCurrent(tick)||!shadowStream.peerReady){s={};return;}
    if(s.token!=shadowStream.token||s.peerToken!=shadowStream.peerToken){s={};s.token=shadowStream.token;s.peerToken=shadowStream.peerToken;}
    if(localCharacter()!=2||tick<s.nextSample)return;s.nextSample=tick+80; // 1214ia (#159)
    NativeDominoLinks links{};auto& world=links.network;const bool cached=s.discovered;const double start=presentationClock();
    const bool valid=readNativeDominoLinks(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),links,&dominoReadStage,cached?&s.known:nullptr);
    const double elapsed=presentationClock()-start;dominoReadMaxMs=(std::max)(dominoReadMaxMs,elapsed);++dominoReadCount;
    if(cached){dominoScopedReadMaxMs=(std::max)(dominoScopedReadMaxMs,elapsed);++dominoScopedReadCount;}
    // 1214cs (#11): after the casts in mission-coverage-v1328zl the connection part failed at stage 14 (a link's beam FX no
    // longer validated) while both marks stayed on the guards, and the whole snapshot was dropped, so the partner marks
    // vanished between casts. When only the connection part (stages 11-16) fails, publish the exactly read marks alone.
    bool marksOnly=false;
    if(!valid&&dominoReadStage>=11&&dominoReadStage<=16){links={};unsigned targetStage=0;
        if(readNativeDominoTargets(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),links.network,&targetStage,cached?&s.known:nullptr)){marksOnly=true;++dominoWorldMarksOnly;}}
    if(!valid&&!marksOnly){if(dominoReadStage==1)s.discovered=false;++dominoWorldUnknown;return;}
    if(world.equipment.instantiated){s.known=world.equipment;s.discovered=true;}
    const auto handle=world.present?world.handle:0u;
    if(!s.sourceEvent&&!handle)return;
    if(handle!=s.sourceHandle||!s.sourceEvent){s.sourceHandle=handle;s.sourceEvent=++dominoWorldEvent;}
    CoopDominoWorldPacket p{};p.role=localRole;p.sequence=++dominoWorldSequence;p.sourceSession=outgoing.session;p.targetSession=remote.session;
    p.sourceWorldEpoch=outgoing.worldEpoch;p.targetWorldEpoch=remote.worldEpoch;p.mapId=outgoing.mapId;p.sourceToken=s.token;p.targetToken=s.peerToken;
    p.event=s.sourceEvent;p.sampleTimeUs=static_cast<std::uint64_t>(presentationClock()*1000.);p.rootSequence=outgoing.sequence;p.nativeHandle=handle;p.present=world.present?1u:0u;
    if(!memoryRead(world.equipment.player.sword.actor+0x300,p.sourceOrigin.data(),12))return;
    for(const auto& target:world.targets){
        // Native partner targeting is a separate gameplay exclusion task.
        // Do not advertise a mark on our registered co-op presentation actor.
        if(target.actor==probeActor||target.key.handle==probeHandle||target.name.find("/d2coop/")!=std::string::npos)return;
        if(!target.hasMark)continue;if(p.count>=4)return;auto& mark=p.marks[p.count++];mark.id=target.key.mark;mark.target=target.key.handle;
        std::memcpy(mark.transform.data(),target.mark.origin,12);std::memcpy(mark.transform.data()+3,target.mark.axis,36);
    }
    for(const auto& link:links.connections)if(link.hasBeam){
        if(p.beamCount>=p.beams.size())return;
        bool from=false,to=false;for(unsigned i=0;i<p.count;++i){from|=p.marks[i].target==link.from;to|=p.marks[i].target==link.to;}
        if(!from||!to)return; // Do not publish links ahead of their owned marks.
        p.beams[p.beamCount++]={link.fxId,link.from,link.to,link.start,link.end};
    }
    if(!validDominoWorldPacket(p,remote,outgoing)||!sockets||!connection)return;
    if(coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_UnreliableNoDelay,nullptr)==k_EResultOK)++dominoWorldSent;
}
void receiveDominoWorld(const CoopDominoWorldPacket& p,ULONGLONG tick){
    ++dominoWorldReceived;auto& s=dominoWorldStream;
    if(!nativeMutationAllowed||!dominoWorldCurrent(tick)||!validDominoWorldPacket(p,outgoing,remote)||p.sourceToken!=shadowStream.peerToken||
       p.targetToken!=shadowStream.token||!movementBankActive()||s.count>=8){++dominoWorldRejected;return;}
    const double now=presentationClock(),mapped=double(p.sampleTimeUs)*.001+sourcePoseTimeline.offset;auto order=s.order;
    if(sourcePoseTimeline.session!=p.sourceSession||sourcePoseTimeline.epoch!=p.sourceWorldEpoch||sourcePoseTimeline.map!=p.mapId||
       mapped>now+80||now-mapped>250||!order.accept(p)){++dominoWorldRejected;return;}
    s.order=order;s.pending[s.count++]={p,mapped};
}
void presentDominoWorld(ULONGLONG tick){
    auto& s=dominoWorldStream;if(!dominoWorldCurrent(tick)||!movementBankActive()||!coopGameplayReady(outgoing)||!coopGameplayReady(remote)){s.lease=0;return;}
    const double now=presentationClock()-80.;while(s.count&&s.pending[0].time<=now){auto p=s.pending[0];for(unsigned i=1;i<s.count;++i)s.pending[i-1]=s.pending[i];--s.count;
        if(now-p.time>250){++dominoWorldRejected;continue;}s.presented=p.packet;s.lease=tick+300;++dominoWorldPresented;
    }
}
