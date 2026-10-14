#pragma once
// Fixed-rig, opt-in presentation stream. It never starts native AI or damage.
SRWLOCK guardBodyLock=SRWLOCK_INIT;
struct GuardBodyLease {
    ULONGLONG until=0;
    std::uintptr_t game=0,actor=0,animation=0,skeleton=0,frame=0;
    std::uint32_t handle=0;
    std::uint64_t rig=0,sequence=0;
    std::array<BodyMatrix,kGuardBodyJoints> local{};
};
GuardBodyLease guardBodyLease{};
bool guardBodyEnabled=false;
ULONGLONG guardBodyLastSend=0;
std::uint64_t guardBodySequence=0,guardBodySent=0,guardBodyReceived=0,guardBodyRejected=0,guardBodyLastSequence=0,guardBodyLastSession=0;
std::atomic<std::uint64_t> guardBodyApplied{0},guardBodyRefused{0},guardBodyAppliedSequence{0};
std::atomic<DWORD> guardBodyThread{0};
struct GuardBodyTrace {
    std::uint64_t sequence=0;
    float local[27][12]{};
};
// Bounded received-pose history for external native pose verification. Only
// the 27 core body joints are logged; native application covers the full rig.
std::array<GuardBodyTrace,8> guardBodyTrace{};
unsigned guardBodyTraceCount=0;
void stopGuardBody(){AcquireSRWLockExclusive(&guardBodyLock);guardBodyLease.until=0;guardBodyTraceCount=0;ReleaseSRWLockExclusive(&guardBodyLock);}
void sendGuardBody(ULONGLONG tick){
    if(!guardBodyEnabled||localRole||tick-guardBodyLastSend<50||guardLink.phase!=GuardPhase::Active||guardLink.key.mode!=1||!guardReadLocal())return;
    guardBodyLastSend=tick;
    NativeSkeleton s{};
    if(!readNativeSkeleton(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),guardLink.npc.handle,s)||!s.stable||s.logic!=1||
       s.game!=guardLink.game||s.actor!=guardLink.npc.actor||s.localPose.size()!=kGuardBodyJoints){++guardBodyRejected;return;}
    CoopGuardBodyPacket p{};p.rig=guardBodyRig(s);p.binding=guardLastPose;p.binding.sequence=++guardBodySequence;
    if(!p.rig||!guardSameBinding(p.binding,guardLink.key)){++guardBodyRejected;return;}
    std::memcpy(p.binding.position,s.root,12);std::memcpy(p.binding.orientation,s.root+3,36);
    for(unsigned j=0;j<kGuardBodyJoints;++j)for(unsigned c=0;c<4;++c)for(unsigned r=0;r<3;++r)p.local[j][c*3+r]=s.localPose[j][c*4+r];
    if(!validGuardBody(p,remote,outgoing)){++guardBodyRejected;return;}
    if(coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_UnreliableNoNagle,nullptr)==k_EResultOK)++guardBodySent;
    else ++guardBodyRejected;
}
void receiveGuardBody(const CoopGuardBodyPacket& p,ULONGLONG tick){
    ++guardBodyReceived;
    if(!guardBodyEnabled||!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load()||!guardPairReady(tick)||
       guardLink.phase!=GuardPhase::Active||guardLink.key.mode!=1||tick>=guardLink.deadline||
       !validGuardBody(p,outgoing,remote)||!guardSameBinding(p.binding,guardLink.key)||
       (guardBodyLastSession==remote.session&&p.binding.sequence<=guardBodyLastSequence)||
       !guardReadLocal()||!guardAiPaused()||!guardAttackInactive()||!bodyNativeFunctions()){++guardBodyRejected;return;}
    NativeSkeleton s{};
    if(!readNativeSkeleton(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),guardLink.npc.handle,s)||s.logic!=1||
       s.game!=guardLink.game||s.actor!=guardLink.npc.actor||guardBodyRig(s)!=p.rig){++guardBodyRejected;return;}
    GuardBodyLease next{};next.until=(std::min)(tick+250,guardLink.deadline);next.game=s.game;next.actor=s.actor;next.animation=s.animation;
    next.skeleton=s.skeleton;next.frame=guardLink.frame;next.handle=s.handle;next.rig=p.rig;next.sequence=p.binding.sequence;
    for(unsigned j=0;j<kGuardBodyJoints;++j)next.local[j]=guardBodyMatrix(p.local[j]);
    guardBodyLastSession=remote.session;guardBodyLastSequence=p.binding.sequence;
    auto& trace=guardBodyTrace[guardBodyTraceCount++%guardBodyTrace.size()];trace.sequence=p.binding.sequence;
    std::memcpy(trace.local,p.local,sizeof(trace.local));
    AcquireSRWLockExclusive(&guardBodyLock);guardBodyLease=next;ReleaseSRWLockExclusive(&guardBodyLock);
}
void applyGuardBody(void* component,void* actor){
    AcquireSRWLockShared(&guardBodyLock);
    const auto& lease=guardBodyLease;const auto tick=GetTickCount64();
    if(lease.animation!=reinterpret_cast<std::uintptr_t>(component)||lease.actor!=reinterpret_cast<std::uintptr_t>(actor)||!lease.until||tick>=lease.until){ReleaseSRWLockShared(&guardBodyLock);return;}
    NativeSkeleton s{};
    const bool valid=guardFrameUntil.load(std::memory_order_acquire)>tick&&guardControlledFrame.load(std::memory_order_acquire)==reinterpret_cast<void*>(lease.frame)&&
        readNativeSkeleton(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),lease.handle,s)&&s.stable&&s.logic==1&&
        s.game==lease.game&&s.actor==lease.actor&&s.animation==lease.animation&&s.skeleton==lease.skeleton&&guardBodyRig(s)==lease.rig&&GetTickCount64()<lease.until;
    if(valid){
        for(unsigned j=0;j<kGuardBodyJoints;++j)reinterpret_cast<void(*)(void*,std::uint16_t,const BodyMatrix*)>(gameImage+0x9cfa0)(reinterpret_cast<void*>(lease.animation+0x40),static_cast<std::uint16_t>(j),&lease.local[j]);
        reinterpret_cast<void(*)(void*)>(gameImage+0x9dc40)(reinterpret_cast<void*>(lease.animation+0x40));
        ++guardBodyApplied;guardBodyAppliedSequence.store(lease.sequence);guardBodyThread.store(GetCurrentThreadId());
    }else ++guardBodyRefused;
    ReleaseSRWLockShared(&guardBodyLock);
}
void writeGuardBody(ULONGLONG tick){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.guard-body.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    AcquireSRWLockShared(&guardBodyLock);const auto& s=guardBodyLease;
    fprintf(f,"{\"enabled\":%s,\"active\":%s,\"sent\":%llu,\"received\":%llu,\"rejected\":%llu,\"applied\":%llu,\"refused\":%llu,\"thread\":%lu,\"rig\":\"%llx\",\"sequence\":%llu,\"appliedSequence\":%llu,\"remainingMs\":%llu,\"jointCount\":%u,\"local\":[",
        guardBodyEnabled?"true":"false",s.until>tick?"true":"false",guardBodySent,guardBodyReceived,guardBodyRejected,guardBodyApplied.load(),guardBodyRefused.load(),guardBodyThread.load(),s.rig,s.sequence,guardBodyAppliedSequence.load(),s.until>tick?s.until-tick:0,kGuardBodyJoints);
    if(s.until>tick)for(unsigned j=0;j<kGuardBodyJoints;++j){fprintf(f,"%s[",j?",":"");for(unsigned c=0;c<4;++c)for(unsigned r=0;r<3;++r)fprintf(f,"%s%.9g",c||r?",":"",s.local[j].v[c*4+r]);fprintf(f,"]");}
    fprintf(f,"],\"traceJointCount\":27,\"history\":[");
    if(s.until>tick){const auto first=guardBodyTraceCount>guardBodyTrace.size()?guardBodyTraceCount-guardBodyTrace.size():0;
        for(auto i=first;i<guardBodyTraceCount;++i){const auto& t=guardBodyTrace[i%guardBodyTrace.size()];fprintf(f,"%s{\"sequence\":%llu,\"local\":[",i==first?"":",",t.sequence);
            for(unsigned j=0;j<27;++j){fprintf(f,"%s[",j?",":"");for(unsigned n=0;n<12;++n)fprintf(f,"%s%.9g",n?",":"",t.local[j][n]);fprintf(f,"]");}fprintf(f,"]}");}}
    fprintf(f,"]}\n");ReleaseSRWLockShared(&guardBodyLock);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
