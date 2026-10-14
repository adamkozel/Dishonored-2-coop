// Two-second, explicitly armed local research experiment. The source pose is
// captured once from this instance's actual player. This does NOT replicate
// remote body animation yet. Native evaluation regenerates the pose on expiry.
SRWLOCK bodyProbeLock=SRWLOCK_INIT;
struct BodyProbeState {
    ULONGLONG until=0;std::uintptr_t game=0,actor=0,animation=0,skeleton=0;std::uint32_t handle=0;
    std::uint64_t epoch=0,peerSession=0,peerEpoch=0;bool remoteDriven=false;
    BodyRetarget rig;std::array<BodyMatrix,20> source{};
};
BodyProbeState bodyProbe{};
std::atomic<std::uint64_t> bodyProbeApplied{0},bodyProbeRefused{0};
std::atomic<DWORD> bodyProbeThread{0};
bool bodyStreamEnabled=false;
std::atomic<std::uint64_t> bodyStreamApplied{0};
bool bodyKnownSkeletons(const NativeSkeleton& source,const NativeSkeleton& target){
    return !std::strcmp(source.skeletonName,"models/characters/player/emily/player_body_emily.md6skl")&&
        (!std::strcmp(target.skeletonName,"models/characters/female/emily/emily.md6skl")||!std::strcmp(target.skeletonName,"models/characters/tall/str/corvo/t_str_corvo01.md6skl"));
}
void sampleBodyPose(){
    NativeSkeleton source{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(!readNativeSkeleton(memoryRead,base,1,source)||!source.stable||source.logic!=1||source.actor!=observedPlayer||source.game!=observedGame||source.jointIds.size()!=20||
       std::strcmp(source.skeletonName,"models/characters/player/emily/player_body_emily.md6skl"))return;
    constexpr std::int16_t parents[]={-1,0,1,2,3,4,5,6,6,1,9,10,11,12,1,14,15,16,17,0};
    if(source.parents.size()!=20||!std::equal(source.parents.begin(),source.parents.end(),parents))return;
    for(unsigned j=0;j<20;++j){BodyMatrix m{};std::memcpy(m.v,source.globalPose[j].data(),64);if(!bodyRigid(m,3.f))return;}
    const auto identity=bodyIdentity();for(unsigned c=0;c<4;++c)for(unsigned r=0;r<3;++r)if(std::fabs(source.globalPose[0][c*4+r]-identity.v[c*4+r])>.001f)return;
    for(unsigned j=0;j<20;++j)for(unsigned c=0;c<4;++c)for(unsigned r=0;r<3;++r)outgoing.bodyPose[j][c*3+r]=source.globalPose[j][c*4+r];
    outgoing.bodyPoseValid=1;
    std::memcpy(outgoing.position,source.root,12);std::memcpy(outgoing.orientation,source.root+3,36);
}
bool bodyNativeFunctions(){
    const unsigned char setter[]={0x41,0x0f,0x28,0x00,0x66,0x89,0x54,0x24,0x10,0x4c,0x8b,0xc9};
    const unsigned char rebuild[]={0x48,0x89,0x5c,0x24,0x08,0x57,0x48,0x83,0xec,0x20};
    unsigned char actual[sizeof(setter)]{};
    return memoryRead(reinterpret_cast<std::uintptr_t>(gameImage)+0x9cfa0,actual,sizeof(setter))&&!std::memcmp(actual,setter,sizeof(setter))&&
        memoryRead(reinterpret_cast<std::uintptr_t>(gameImage)+0x9dc40,actual,sizeof(rebuild))&&!std::memcmp(actual,rebuild,sizeof(rebuild));
}
void stopBodyProbe(){AcquireSRWLockExclusive(&bodyProbeLock);bodyProbe.until=0;ReleaseSRWLockExclusive(&bodyProbeLock);}
void beginBodyProbe(){
    const auto tick=GetTickCount64();
    AcquireSRWLockShared(&bodyProbeLock);const bool active=bodyProbe.until>tick;ReleaseSRWLockShared(&bodyProbeLock);
    if(active||bodyStreamEnabled||GetCurrentThreadId()!=engineThread.load()||!probeFollow||!networkFrameOwned.load()||!coopGameplayReady(outgoing)||!coopGameplayReady(remote)||
       !lastReceive||tick-lastReceive>=250||!bodyNativeFunctions()){++bodyProbeRefused;return;}
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);NativeSkeleton source{},target{};
    if(!readNativeSkeleton(memoryRead,base,1,source)||!source.stable||source.logic!=1||source.game!=observedGame||
       !readNativeSkeleton(memoryRead,base,probeHandle,target)||!target.stable||target.logic!=1||target.game!=source.game||target.actor!=probeActor||
       !bodyKnownSkeletons(source,target)) {++bodyProbeRefused;return;}
    BodyProbeState next{};if(!prepareBodyRetarget(source,target,next.rig)){++bodyProbeRefused;return;}
    for(unsigned j=0;j<20;++j){std::memcpy(next.source[j].v,source.globalPose[j].data(),64);if(!bodyRigid(next.source[j])){++bodyProbeRefused;return;}}
    next.game=target.game;next.actor=target.actor;next.animation=target.animation;next.skeleton=target.skeleton;next.handle=probeHandle;next.until=GetTickCount64()+2000;
    AcquireSRWLockExclusive(&bodyProbeLock);bodyProbe=std::move(next);ReleaseSRWLockExclusive(&bodyProbeLock);
}
void publishBodyStream(ULONGLONG tick){
    if(!bodyStreamEnabled)return;
    if(!probeFollow||!networkFrameOwned.load()||!coopGameplayReady(outgoing)||!coopGameplayReady(remote)||!remote.bodyPoseValid||
       !lastReceive||tick-lastReceive>=250||(!remote.playerStance&&remote.playerMotion==0)){stopBodyProbe();return;}
    BodyProbeState next{};
    AcquireSRWLockShared(&bodyProbeLock);
    const bool reuse=bodyProbe.remoteDriven&&bodyProbe.game==observedGame&&bodyProbe.actor==probeActor&&bodyProbe.handle==probeHandle&&
        bodyProbe.epoch==outgoing.worldEpoch&&bodyProbe.peerSession==remote.session&&bodyProbe.peerEpoch==remote.worldEpoch;
    if(reuse)next=bodyProbe;
    ReleaseSRWLockShared(&bodyProbeLock);
    if(!reuse){NativeSkeleton source{},target{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
        if(!bodyNativeFunctions()||!readNativeSkeleton(memoryRead,base,1,source)||source.logic!=1||source.game!=observedGame||
           !readNativeSkeleton(memoryRead,base,probeHandle,target)||target.logic!=1||target.game!=observedGame||target.actor!=probeActor||
           !bodyKnownSkeletons(source,target)||!prepareBodyRetarget(source,target,next.rig)){++bodyProbeRefused;stopBodyProbe();return;}
        next.game=target.game;next.actor=target.actor;next.animation=target.animation;next.skeleton=target.skeleton;next.handle=probeHandle;
        next.epoch=outgoing.worldEpoch;next.peerEpoch=remote.worldEpoch;next.peerSession=remote.session;next.remoteDriven=true;
    }
    for(unsigned j=0;j<20;++j)for(unsigned c=0;c<4;++c)for(unsigned r=0;r<3;++r)next.source[j].v[c*4+r]=remote.bodyPose[j][c*3+r];
    next.until=lastReceive+250;
    AcquireSRWLockExclusive(&bodyProbeLock);bodyProbe=std::move(next);ReleaseSRWLockExclusive(&bodyProbeLock);
}
void applyBodyProbe(void* component,void* actor){
    AcquireSRWLockShared(&bodyProbeLock);
    if(bodyProbe.animation!=reinterpret_cast<std::uintptr_t>(component)||bodyProbe.actor!=reinterpret_cast<std::uintptr_t>(actor)||
       !bodyProbe.until||GetTickCount64()>=bodyProbe.until){ReleaseSRWLockShared(&bodyProbeLock);return;}
    // The original animation routine has just returned for this same owner.
    // Keep the lease lock through native calls; cancellation cannot race a call.
    const auto& state=bodyProbe;NativeSkeleton current{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    bool valid=networkFrameOwned.load(std::memory_order_acquire)&&readNativeSkeleton(memoryRead,base,state.handle,current)&&current.stable&&
        current.logic==1&&current.game==state.game&&current.actor==state.actor&&current.animation==state.animation&&current.skeleton==state.skeleton&&
        current.parents==state.rig.parents&&GetTickCount64()<state.until;
    std::vector<BodyMatrix> local,result;
    if(valid){local.resize(current.localPose.size());for(unsigned j=0;j<local.size();++j)std::memcpy(local[j].v,current.localPose[j].data(),64);
        valid=retargetBody(state.rig,state.source,local,result);}
    if(valid){
        for(unsigned j=0;j<result.size();++j)if(state.rig.sourceForTarget[j]>=0)
            reinterpret_cast<void(*)(void*,std::uint16_t,const BodyMatrix*)>(gameImage+0x9cfa0)(reinterpret_cast<void*>(state.animation+0x40),static_cast<std::uint16_t>(j),&result[j]);
        reinterpret_cast<void(*)(void*)>(gameImage+0x9dc40)(reinterpret_cast<void*>(state.animation+0x40));
        ++bodyProbeApplied;if(state.remoteDriven)++bodyStreamApplied;bodyProbeThread.store(GetCurrentThreadId());
    }else ++bodyProbeRefused;
    ReleaseSRWLockShared(&bodyProbeLock);
}
