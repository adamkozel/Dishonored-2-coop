#pragma once
#include "doppelganger_world_protocol.h"
#include "native_doppelganger_world.h"
#include "native_doppelganger_fx_scene.h"
// Source-owned clone body snapshots; no native animation events travel with poses.
struct DoppelgangerWorldStream {
    struct Pending{CoopDoppelgangerWorldPacket packet{};double time=0;};
    std::array<Pending,8> pending{};unsigned count=0;
    std::uint64_t token=0,peerToken=0;ULONGLONG nextSample=0,lease=0;
    NativeDoppelgangerItem known{};bool discovered=false;unsigned alternate=0;
    DoppelgangerWorldOrder order{};CoopDoppelgangerWorldPacket sent{},presented{};double presentedTime=0;
};
// 1214eh (#11 power_upgrade_variants): one stream per clone slot (Twin Shade keeps two shades). Slot 0
// also holds the source's discovery and sampling state; both slots carry the same session tokens.
std::array<DoppelgangerWorldStream,kDoppelgangerWireSlots> doppelgangerWorldStreams{};
DoppelgangerWorldStream& doppelgangerWorldStream=doppelgangerWorldStreams[0];
std::uint64_t doppelgangerWorldSequence=0,doppelgangerWorldEvent=0,doppelgangerWorldSent=0,doppelgangerWorldReceived=0,doppelgangerWorldRejected=0,doppelgangerWorldUnknown=0,doppelgangerWorldPresented=0;
double doppelgangerReadMaxMs=0,doppelgangerPoseReadMaxMs=0;std::uint64_t doppelgangerPoseReadFailed=0;unsigned doppelgangerReadStage=0;
unsigned doppelgangerPoseReadLine=0,doppelgangerPoseLogic=0,doppelgangerPoseJoints=0;
std::uint64_t doppelgangerPoseRigObserved=0;bool doppelgangerPoseStable=false,doppelgangerPoseOwner=false;
double doppelgangerSceneReadMaxMs=0;unsigned doppelgangerSceneReadLine=0;std::uint64_t doppelgangerSceneUnknown=0;
std::uint64_t doppelgangerPacketRefused=0,doppelgangerSendFailed=0;unsigned doppelgangerPacketRefusedLine=0,doppelgangerPacketRefusedJoint=~0u;int doppelgangerLastSendResult=0;
SteamNetConnectionRealTimeStatus_t doppelgangerFailedSendStatus{};ULONGLONG doppelgangerSendStatusNext=0;
std::array<float,12> doppelgangerRefusedJointMatrix{};std::uint64_t doppelgangerRefusedSequence=0;
struct DoppelgangerWorldRecord{std::uint64_t sequence=0,event=0,digest=0;unsigned handle=0,generation=0,present=0,fxKnown=0,fxScene=0,fxVariant=0,fxTimeMs=0;};
std::array<DoppelgangerWorldRecord,128> doppelgangerWorldHistory{};std::uint64_t doppelgangerWorldHistoryCount=0;
void recordDoppelgangerWorld(const CoopDoppelgangerWorldPacket& p){doppelgangerWorldHistory[doppelgangerWorldHistoryCount++%128]={p.sequence,p.event,doppelgangerWorldDigest(p),p.nativeHandle,p.generation,p.present,p.fxKnown,p.fxScene,p.fxVariant,p.fxTimeMs};}
bool doppelgangerWorldCurrent(ULONGLONG tick){return shadowStreamCurrent(tick)&&shadowStream.peerReady&&doppelgangerWorldStream.token==shadowStream.token&&doppelgangerWorldStream.peerToken==shadowStream.peerToken;}
void sendDoppelgangerWorldSlot(ULONGLONG tick,DoppelgangerWorldStream& s,const NativeDoppelgangerWorld& w,unsigned slot){
    CoopDoppelgangerWorldPacket p{};p.sequence=++doppelgangerWorldSequence;p.sourceSession=outgoing.session;p.targetSession=remote.session;
    p.sourceWorldEpoch=outgoing.worldEpoch;p.targetWorldEpoch=remote.worldEpoch;p.mapId=outgoing.mapId;p.sourceToken=s.token;p.targetToken=s.peerToken;
    p.sampleTimeUs=static_cast<std::uint64_t>(presentationClock()*1000.);p.rootSequence=outgoing.sequence;p.present=w.present?1u:0u;p.slot=slot;
    if(!memoryRead(w.equipment.player.sword.actor+0x300,p.sourceOrigin.data(),12))return;
    if(w.present){
        const double sceneStart=presentationClock();NativeDoppelgangerFxScene scene{};
        if(readNativeDoppelgangerFxScene(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),w,scene,&doppelgangerSceneReadLine)){
            p.fxKnown=1;p.fxScene=scene.id;p.fxVariant=scene.variant;p.fxTimeMs=scene.timeMs;
        }else ++doppelgangerSceneUnknown; // Unknown effects never interrupt the body/root stream.
        doppelgangerSceneReadMaxMs=(std::max)(doppelgangerSceneReadMaxMs,presentationClock()-sceneStart);
        const double poseStart=presentationClock();NativeSkeleton pose{};
        const bool readPose=readNativeSkeleton(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),w.handle,pose);
        doppelgangerPoseReadLine=pose.failureLine;doppelgangerPoseLogic=pose.logic;doppelgangerPoseJoints=static_cast<unsigned>(pose.localPose.size());
        doppelgangerPoseRigObserved=doppelgangerRig(pose);doppelgangerPoseStable=pose.stable;
        doppelgangerPoseOwner=pose.game==w.equipment.player.sword.game&&pose.actor==w.actor;
        const bool poseValid=readPose&&doppelgangerPoseStable&&pose.logic==1&&doppelgangerPoseOwner&&pose.localPose.size()==kDoppelgangerJoints&&doppelgangerPoseRigObserved==kDoppelgangerRig;
        doppelgangerPoseReadMaxMs=(std::max)(doppelgangerPoseReadMaxMs,presentationClock()-poseStart);
        if(!poseValid){++doppelgangerPoseReadFailed;return;}
        p.nativeHandle=w.handle;p.equipmentHandle=w.equipment.handle;p.generation=w.generation;p.rig=kDoppelgangerRig;
        std::memcpy(p.transform.data(),pose.root,48);
        for(unsigned j=0;j<kDoppelgangerJoints;++j)for(unsigned c=0;c<4;++c)for(unsigned r=0;r<3;++r)p.local[j][c*3+r]=pose.localPose[j][c*4+r];
    }
    const bool changed=!s.sent.event||!sameDoppelgangerIncarnation(p,s.sent);p.event=changed?++doppelgangerWorldEvent:s.sent.event;
    unsigned packetFailure=0;if(!validDoppelgangerWorldPacket(p,remote,outgoing,&packetFailure)){
        ++doppelgangerPacketRefused;doppelgangerPacketRefusedLine=packetFailure;doppelgangerPacketRefusedJoint=~0u;
        for(unsigned i=0;i<kDoppelgangerJoints;++i)if(!doppelgangerJointValid(i,doppelgangerLocalMatrix(p.local[i]))){doppelgangerPacketRefusedJoint=i;doppelgangerRefusedJointMatrix=p.local[i];doppelgangerRefusedSequence=p.sequence;break;}return;
    }
    if(!sockets||!connection)return;
    const bool sceneChanged=p.fxKnown&&(p.fxScene!=s.sent.fxScene||p.fxVariant!=s.sent.fxVariant);
    const int flags=changed||sceneChanged?k_nSteamNetworkingSend_ReliableNoNagle:k_nSteamNetworkingSend_UnreliableNoDelay;
    const auto result=coopSendMessage(connection,&p,sizeof(p),flags,nullptr);doppelgangerLastSendResult=static_cast<int>(result);
    if(result==k_EResultOK){s.sent=p;recordDoppelgangerWorld(p);++doppelgangerWorldSent;}
    else{++doppelgangerSendFailed;if(tick>=doppelgangerSendStatusNext){doppelgangerSendStatusNext=tick+500;sockets->GetConnectionRealTimeStatus(connection,&doppelgangerFailedSendStatus,0,nullptr);}}
}
void sampleDoppelgangerWorld(ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;auto& s=doppelgangerWorldStream;
    if(!shadowStreamCurrent(tick)||!shadowStream.peerReady){for(auto& x:doppelgangerWorldStreams)x={};return;}
    if(s.token!=shadowStream.token||s.peerToken!=shadowStream.peerToken){for(auto& x:doppelgangerWorldStreams){x={};x.token=shadowStream.token;x.peerToken=shadowStream.peerToken;}}
    if(localRole!=1||tick<s.nextSample)return;s.nextSample=tick+(s.discovered?33:500);
    // Discover from the selected exact power only. Never scan the full mixed
    // inventory on the engine thread. Late join with unequipped clone is pending.
    if(!s.discovered){
        NativeDoppelgangerItem item{};if(!readNativeDoppelgangerItem(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),item,true)||!item.instantiated)return;
        s.known=item;s.discovered=true;
    }
    std::array<NativeDoppelgangerWorld,kDoppelgangerWireSlots> worlds{};const double start=presentationClock();
    for(unsigned a=0;a<worlds.size();++a)if(!readNativeDoppelgangerWorld(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),worlds[a],&doppelgangerReadStage,&s.known,a)){
        doppelgangerReadMaxMs=(std::max)(doppelgangerReadMaxMs,presentationClock()-start);if(doppelgangerReadStage==1)s.discovered=false;++doppelgangerWorldUnknown;return;}
    doppelgangerReadMaxMs=(std::max)(doppelgangerReadMaxMs,presentationClock()-start);
    // Successful rediscovery must immediately return to the active cadence.
    // The discovery backoff was chosen before discovery above; retaining its
    // 500ms deadline after a valid snapshot exceeds the peer's 300ms lease.
    s.nextSample=tick+33;s.known=worlds[0].equipment;
    // 1214eh (#11 power_upgrade_variants): a clone keeps its wire slot while it stays committed, so the
    // partner's two visuals never swap shades; a new clone takes a free slot. With two clones the slots
    // take turns (about 15 Hz each, the single clone's 30 Hz budget), and a slot whose clone is gone sends
    // that change once (reliable) instead of an absence packet every sample.
    std::array<int,kDoppelgangerWireSlots> wire{};wire.fill(-1);
    for(unsigned a=0;a<wire.size();++a)if(worlds[a].present)for(unsigned k=0;k<wire.size();++k){const auto& x=doppelgangerWorldStreams[k].sent;
        if(x.present&&x.nativeHandle==worlds[a].handle&&x.generation==worlds[a].generation&&std::find(wire.begin(),wire.end(),static_cast<int>(k))==wire.end()){wire[a]=static_cast<int>(k);break;}}
    for(unsigned a=0;a<wire.size();++a)if(worlds[a].present&&wire[a]<0)for(unsigned k=0;k<wire.size();++k)
        if(std::find(wire.begin(),wire.end(),static_cast<int>(k))==wire.end()){wire[a]=static_cast<int>(k);break;}
    unsigned present=0;for(const auto& w:worlds)present+=w.present?1u:0u;
    const unsigned turn=present>1?(s.alternate++&1u):kDoppelgangerWireSlots;
    for(unsigned k=0;k<kDoppelgangerWireSlots;++k){auto& x=doppelgangerWorldStreams[k];
        NativeDoppelgangerWorld absent{};absent.equipment=worlds[0].equipment;const NativeDoppelgangerWorld* w=&absent;
        for(unsigned a=0;a<wire.size();++a)if(wire[a]==static_cast<int>(k))w=&worlds[a];
        const bool fresh=x.sent.present!=(w->present?1u:0u)||(w->present&&(x.sent.nativeHandle!=w->handle||x.sent.generation!=w->generation));
        if(!w->present&&(!x.sent.event||!fresh))continue;
        if(w->present&&turn<kDoppelgangerWireSlots&&turn!=k&&x.sent.event&&!fresh)continue;
        sendDoppelgangerWorldSlot(tick,x,*w,k);
    }
}
void receiveDoppelgangerWorld(const CoopDoppelgangerWorldPacket& p,ULONGLONG tick){
    ++doppelgangerWorldReceived;auto& s=doppelgangerWorldStreams[p.slot<kDoppelgangerWireSlots?p.slot:0];
    if(!nativeMutationAllowed||!doppelgangerWorldCurrent(tick)||!validDoppelgangerWorldPacket(p,outgoing,remote)||p.sourceToken!=shadowStream.peerToken||
       p.targetToken!=shadowStream.token||!movementBankActive()||s.count>=8){++doppelgangerWorldRejected;return;}
    const double now=presentationClock(),mapped=double(p.sampleTimeUs)*.001+sourcePoseTimeline.offset;auto order=s.order;
    if(sourcePoseTimeline.session!=p.sourceSession||sourcePoseTimeline.epoch!=p.sourceWorldEpoch||sourcePoseTimeline.map!=p.mapId||
       mapped>now+80||now-mapped>250||!order.accept(p)){++doppelgangerWorldRejected;return;}
    s.order=order;s.pending[s.count++]={p,mapped};recordDoppelgangerWorld(p);
}
void presentDoppelgangerWorld(ULONGLONG tick){
    for(auto& s:doppelgangerWorldStreams){if(!doppelgangerWorldCurrent(tick)||!movementBankActive()||!coopGameplayReady(outgoing)||!coopGameplayReady(remote)){s.lease=0;continue;}
    const double now=presentationClock()-120.;while(s.count&&s.pending[0].time<=now){const auto p=s.pending[0];for(unsigned i=1;i<s.count;++i)s.pending[i-1]=s.pending[i];--s.count;
        if(now-p.time>250){++doppelgangerWorldRejected;continue;}s.presented=p.packet;s.presentedTime=p.time;s.lease=p.packet.present?tick+300:0;++doppelgangerWorldPresented;
    }}
}
void writeDoppelgangerWorld(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.doppelganger-world.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;const auto& s=doppelgangerWorldStream;const auto& p=localRole==1?s.sent:s.presented;
    fprintf(f,"{\"transportOnly\":false,\"sent\":%llu,\"received\":%llu,\"rejected\":%llu,\"unknown\":%llu,\"presented\":%llu,\"sequence\":%llu,\"event\":%llu,\"nativeHandle\":%u,\"generation\":%u,\"present\":%u,\"leased\":%s,\"readMaxMs\":%.4f,\"readStage\":%u,\"transform\":[",doppelgangerWorldSent,doppelgangerWorldReceived,doppelgangerWorldRejected,doppelgangerWorldUnknown,doppelgangerWorldPresented,p.sequence,p.event,p.nativeHandle,p.generation,p.present,s.lease>GetTickCount64()?"true":"false",doppelgangerReadMaxMs,doppelgangerReadStage);
    for(unsigned i=0;i<12;++i)fprintf(f,"%s%.6f",i?",":"",p.transform[i]);fputs("],\"history\":[",f);const auto n=(std::min)(doppelgangerWorldHistoryCount,128ull);
    for(std::uint64_t i=0;i<n;++i){const auto& r=doppelgangerWorldHistory[(doppelgangerWorldHistoryCount-n+i)%128];fprintf(f,"%s{\"sequence\":%llu,\"event\":%llu,\"digest\":\"%016llx\",\"handle\":%u,\"generation\":%u,\"present\":%u,\"fxKnown\":%u,\"fxScene\":%u,\"fxVariant\":%u,\"fxTimeMs\":%u}",i?",":"",r.sequence,r.event,r.digest,r.handle,r.generation,r.present,r.fxKnown,r.fxScene,r.fxVariant,r.fxTimeMs);}
    fprintf(f,"],\"sceneReadMaxMs\":%.4f,\"sceneReadLine\":%u,\"sceneUnknown\":%llu,\"fxKnown\":%u,\"fxScene\":%u,\"fxVariant\":%u,\"fxTimeMs\":%u",doppelgangerSceneReadMaxMs,doppelgangerSceneReadLine,doppelgangerSceneUnknown,p.fxKnown,p.fxScene,p.fxVariant,p.fxTimeMs);
    fprintf(f,",\"refusedSequence\":%llu,\"refusedJointMatrix\":[",doppelgangerRefusedSequence);for(unsigned i=0;i<12;++i){if(i)fputs(",",f);if(std::isfinite(doppelgangerRefusedJointMatrix[i]))fprintf(f,"%.9g",doppelgangerRefusedJointMatrix[i]);else fputs("null",f);}fputs("]",f);
    fprintf(f,",\"prepared\":%llu,\"packetRefused\":%llu,\"packetRefusedLine\":%u,\"packetRefusedJoint\":%u,\"sendFailed\":%llu,\"lastSendResult\":%d,\"failureSendRate\":%d,\"failurePendingBytes\":%d,\"failureQueueUs\":%lld",doppelgangerWorldSequence,doppelgangerPacketRefused,doppelgangerPacketRefusedLine,doppelgangerPacketRefusedJoint,doppelgangerSendFailed,doppelgangerLastSendResult,doppelgangerFailedSendStatus.m_nSendRateBytesPerSecond,doppelgangerFailedSendStatus.m_cbPendingUnreliable+doppelgangerFailedSendStatus.m_cbPendingReliable,doppelgangerFailedSendStatus.m_usecQueueTime);
    fputs(",\"slots\":[",f);for(unsigned k=0;k<kDoppelgangerWireSlots;++k){const auto& x=doppelgangerWorldStreams[k];const auto& q=localRole==1?x.sent:x.presented;
        fprintf(f,"%s{\"present\":%u,\"handle\":%u,\"generation\":%u,\"event\":%llu,\"sequence\":%llu,\"leased\":%s,\"fxScene\":%u,\"fxVariant\":%u}",k?",":"",q.present,q.nativeHandle,q.generation,q.event,q.sequence,x.lease>GetTickCount64()?"true":"false",q.fxScene,q.fxVariant);}
    fputs("]",f);
    fprintf(f,",\"poseReadMaxMs\":%.4f,\"poseReadFailed\":%llu,\"poseReadLine\":%u,\"poseLogic\":%u,\"poseJoints\":%u,\"poseRig\":\"%016llx\",\"poseStable\":%s,\"poseOwner\":%s}\n",doppelgangerPoseReadMaxMs,doppelgangerPoseReadFailed,doppelgangerPoseReadLine,doppelgangerPoseLogic,doppelgangerPoseJoints,doppelgangerPoseRigObserved,doppelgangerPoseStable?"true":"false",doppelgangerPoseOwner?"true":"false");const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
