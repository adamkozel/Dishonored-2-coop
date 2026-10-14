#pragma once
// Opt-in right-arm presentation, using the already-owned counterpart only.
// Source silence expires in 250 ms even while ordinary root traffic continues.
ULONGLONG armStreamLastSend=0;
std::uint64_t armStreamSequence=0,armStreamSent=0,armStreamReceived=0,armStreamRejected=0,armStreamSampleRefused=0;
std::uint64_t armStreamLastSession=0,armStreamLastSequence=0;
CoopArmPacket armStreamLastPacket{};
std::array<CoopArmPacket,16> armStreamHistory{};
unsigned armStreamHistoryCount=0;
bool armStreamReady(ULONGLONG tick){return armStreamEnabled&&probeFollowExact&&armProbePairReady(tick);}
void sendArmStream(ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load()||!armStreamReady(tick)||tick-armStreamLastSend<33)return;
    armStreamLastSend=tick;
    NativePlayerArm arm{};NativeSkeleton s{};
    if(!resolveRightArm(arm)||arm.character!=localRole+1||!readNativeSkeleton(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),arm.handle,s,NativeSkeletonScope::PlayerArm)||
       !s.stable||s.logic!=1||s.game!=observedGame){++armStreamSampleRefused;return;}
    CoopArmPacket p{};p.sequence=++armStreamSequence;p.role=localRole;p.character=arm.character;p.rig=nativeArmRig(s);
    p.sourceSession=outgoing.session;p.targetSession=remote.session;p.sourceWorldEpoch=outgoing.worldEpoch;p.targetWorldEpoch=remote.worldEpoch;
    p.mapId=outgoing.mapId;p.rootSequence=outgoing.sequence;
    for(unsigned k=0;k<5;++k)for(unsigned c=0;c<4;++c)for(unsigned r=0;r<3;++r)p.global[k][c*3+r]=s.globalPose[kArmCoreJoints[k]][c*4+r];
    if(!validCoopArm(p,remote,outgoing)){++armStreamSampleRefused;return;}
    if(sockets->SendMessageToConnection(connection,&p,sizeof(p),k_nSteamNetworkingSend_UnreliableNoNagle,nullptr)==k_EResultOK){++armStreamSent;armStreamLastPacket=p;}
}
void duplicateArmStream(){
    const auto tick=GetTickCount64();
    if(armStreamReady(tick)&&armStreamLastPacket.sequence&&validCoopArm(armStreamLastPacket,remote,outgoing))
        sockets->SendMessageToConnection(connection,&armStreamLastPacket,sizeof(armStreamLastPacket),k_nSteamNetworkingSend_ReliableNoNagle,nullptr);
}
void receiveArmStream(const CoopArmPacket& p,ULONGLONG tick){
    ++armStreamReceived;
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load()||!armStreamReady(tick)||!validCoopArm(p,outgoing,remote)||
       !armSequenceFresh(p.sequence,p.sourceSession,armStreamLastSession,armStreamLastSequence)||!bodyNativeFunctions()){++armStreamRejected;return;}
    ArmProbeState next{};
    AcquireSRWLockShared(&armProbeLock);
    const bool reuse=armProbe.remoteDriven&&armProbe.game==observedGame&&armProbe.actor==probeActor&&armProbe.handle==probeHandle&&
        armProbe.epoch==outgoing.worldEpoch&&armProbe.peerEpoch==remote.worldEpoch&&armProbe.peerSession==remote.session&&armProbe.rigId==p.rig;
    if(reuse)next=armProbe;
    const bool localExperiment=armProbe.until&&!armProbe.remoteDriven;
    ReleaseSRWLockShared(&armProbeLock);
    if(localExperiment){++armStreamRejected;return;}
    if(!reuse){
        NativePlayerArm arm{};NativeSkeleton source{},target{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
        if(!resolveRightArm(arm)||!readNativeSkeleton(memoryRead,base,arm.handle,source,NativeSkeletonScope::PlayerArm)||source.logic!=1||
           nativeArmRig(source)!=p.rig||!readNativeSkeleton(memoryRead,base,probeHandle,target)||target.logic!=1||target.game!=observedGame||target.actor!=probeActor||
           std::strcmp(target.skeletonName,p.character==1?"models/characters/tall/str/corvo/t_str_corvo01.md6skl":"models/characters/female/emily/emily.md6skl")||
           !prepareArmRetarget(source,target,next.rig)){++armStreamRejected;return;}
        next.game=target.game;next.actor=target.actor;next.animation=target.animation;next.skeleton=target.skeleton;next.handle=target.handle;
        next.epoch=outgoing.worldEpoch;next.peerEpoch=remote.worldEpoch;next.peerSession=remote.session;next.remoteDriven=true;next.rigId=p.rig;
    }
    for(unsigned k=0;k<5;++k)for(unsigned c=0;c<4;++c)for(unsigned r=0;r<3;++r)next.source[k].v[c*4+r]=p.global[k][c*3+r];
    next.until=(std::min)(tick+250,lastReceive+250);next.sequence=p.sequence;
    armStreamLastSession=p.sourceSession;armStreamLastSequence=p.sequence;
    armStreamHistory[armStreamHistoryCount++%armStreamHistory.size()]=p;
    AcquireSRWLockExclusive(&armProbeLock);armProbe=std::move(next);ReleaseSRWLockExclusive(&armProbeLock);
}
void writeArmStream(ULONGLONG tick){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.arm-stream.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    AcquireSRWLockShared(&armProbeLock);const auto& s=armProbe;
    fprintf(f,"{\"enabled\":%s,\"active\":%s,\"sent\":%llu,\"received\":%llu,\"rejected\":%llu,\"sampleRefused\":%llu,\"applied\":%llu,\"appliedSequence\":%llu,\"sequence\":%llu,\"rig\":\"%llx\",\"epoch\":%llu,\"peerEpoch\":%llu,\"peerSession\":%llu,\"remainingMs\":%llu,\"targetHandle\":%u,\"targets\":[",
        armStreamEnabled?"true":"false",s.remoteDriven&&s.until>tick?"true":"false",armStreamSent,armStreamReceived,armStreamRejected,armStreamSampleRefused,
        armStreamApplied.load(),armStreamAppliedSequence.load(),s.sequence,s.rigId,s.epoch,s.peerEpoch,s.peerSession,s.until>tick?s.until-tick:0,s.handle);
    for(unsigned k=0;k<5;++k)fprintf(f,"%s%u",k?",":"",s.rig.targets[k]);fputs("],\"history\":[",f);
    const auto first=armStreamHistoryCount>armStreamHistory.size()?armStreamHistoryCount-armStreamHistory.size():0;
    for(auto i=first;i<armStreamHistoryCount;++i){const auto& p=armStreamHistory[i%armStreamHistory.size()];
        fprintf(f,"%s{\"sequence\":%llu,\"rootSequence\":%u,\"sourceSession\":%llu,\"sourceWorldEpoch\":%llu,\"targetWorldEpoch\":%llu,\"source\":[",i==first?"":",",p.sequence,p.rootSequence,p.sourceSession,p.sourceWorldEpoch,p.targetWorldEpoch);
        for(unsigned k=0;k<5;++k){fputs(k?",[":"[",f);for(unsigned n=0;n<12;++n)fprintf(f,"%s%.9g",n?",":"",p.global[k][n]);fputs("]",f);}fputs("]}",f);
    }
    fputs("]}\n",f);ReleaseSRWLockShared(&armProbeLock);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
