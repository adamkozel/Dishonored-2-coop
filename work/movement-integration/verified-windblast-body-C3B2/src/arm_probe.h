// Explicit two-second local pose experiment. No arm pose is sent over Steam.
// Apply five right-arm joints to the existing owned counterpart, using the
// same native post-evaluation joint APIs already verified for body poses.
struct ArmProbeState {
    ULONGLONG until=0;std::uint64_t epoch=0,peerSession=0,peerEpoch=0;
    std::uint64_t sequence=0,rigId=0;bool remoteDriven=false;
    std::uintptr_t game=0,actor=0,animation=0,skeleton=0;
    std::uint32_t handle=0,sourceHandle=0;
    ArmRetarget rig{};std::array<BodyMatrix,kArmStreamJointCount> source{};
};
SRWLOCK armProbeLock=SRWLOCK_INIT;
ArmProbeState armProbe{},armProbeLast{};
std::atomic<std::uint64_t> armProbeApplied{0},armProbeRefused{0};
std::atomic<DWORD> armProbeThread{0};
std::uint64_t armProbeStarted=0;
const char* armProbeResult="idle";
bool armStreamEnabled=false,armModelSpaceEnabled=false;
std::atomic<std::uint64_t> armStreamApplied{0},armStreamAppliedSequence{0},armStreamRefused{0};
void stopArmProbe(const char* reason){
    AcquireSRWLockExclusive(&armProbeLock);
    if(armProbe.until){armProbe.until=0;armProbeResult=reason;}
    ReleaseSRWLockExclusive(&armProbeLock);
}
bool armProbePairReady(ULONGLONG tick){
    return probeFollow&&networkFrameOwned.load()&&lastReceive&&tick-lastReceive<250&&
        coopWorldPairReady(outgoing,remote)&&!std::strcmp(mapName,kGuardMap);
}
bool resolveRightArm(NativePlayerArm& out){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    std::uintptr_t player=worldIdentityEntity(memoryRead,observedGame,1),children=0;unsigned count=0,matches=0;
    if(!player||player!=observedPlayer||!memoryRead(player+0xb0,&children,8)||!children||!memoryRead(player+0xb8,&count,4)||(count&0xffffff)>32)return false;
    for(unsigned i=0;i<(count&0xffffff);++i){
        std::uintptr_t actor=0;std::uint32_t handle=0;
        if(!memoryRead(children+i*8,&actor,8)||!actor||!memoryRead(actor+0xa8,&handle,4))return false;
        NativePlayerArm arm{};
        if(readNativePlayerArm(memoryRead,base,observedGame,handle,arm)&&arm.actor==actor&&arm.side==2){out=arm;++matches;}
    }
    return matches==1;
}
void beginArmProbe(){
    const auto tick=GetTickCount64();
    AcquireSRWLockShared(&armProbeLock);const bool active=armProbe.until!=0;ReleaseSRWLockShared(&armProbeLock);
    if(active||armStreamEnabled){++armProbeRefused;armProbeResult="duplicate_arm";return;}
    if(GetCurrentThreadId()!=engineThread.load()||!armProbePairReady(tick)||!bodyNativeFunctions()){
        ++armProbeRefused;armProbeResult="unsupported_scope";return;
    }
    NativePlayerArm arm{};NativeSkeleton source{},target{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    ArmProbeState next{};
    if(!resolveRightArm(arm)||!readNativeSkeleton(memoryRead,base,arm.handle,source,NativeSkeletonScope::PlayerArm)||!source.stable||source.logic!=1||
       !readNativeSkeleton(memoryRead,base,probeHandle,target)||!target.stable||target.logic!=1||target.game!=observedGame||target.actor!=probeActor||
       !prepareArmRetarget(source,target,next.rig)){++armProbeRefused;armProbeResult="rig_unavailable";return;}
    for(unsigned k=0;k<5;++k){std::memcpy(next.source[k].v,source.globalPose[kArmCoreJoints[k]].data(),64);if(!bodyRigid(next.source[k],3)){++armProbeRefused;armProbeResult="invalid_pose";return;}}
    next.until=GetTickCount64()+2000;next.epoch=outgoing.worldEpoch;next.peerEpoch=remote.worldEpoch;next.peerSession=remote.session;
    next.game=target.game;next.actor=target.actor;next.animation=target.animation;next.skeleton=target.skeleton;next.handle=target.handle;next.sourceHandle=source.handle;
    AcquireSRWLockExclusive(&armProbeLock);armProbe=next;armProbeLast=std::move(next);++armProbeStarted;armProbeResult="armed";ReleaseSRWLockExclusive(&armProbeLock);
}
void serviceArmProbe(ULONGLONG tick){
    AcquireSRWLockShared(&armProbeLock);
    const auto until=armProbe.until;
    const bool current=until&&armProbePairReady(tick)&&outgoing.worldEpoch==armProbe.epoch&&remote.worldEpoch==armProbe.peerEpoch&&
        remote.session==armProbe.peerSession&&probeActor==armProbe.actor&&probeHandle==armProbe.handle&&observedGame==armProbe.game;
    ReleaseSRWLockShared(&armProbeLock);
    if(until&&(tick>=until||!current))stopArmProbe(tick>=until?"expired":"scope_changed");
}
void applyArmProbe(void* component,void* actor){
    AcquireSRWLockShared(&armProbeLock);const auto& s=armProbe;
    if(!s.until||GetTickCount64()>=s.until||s.animation!=reinterpret_cast<std::uintptr_t>(component)||s.actor!=reinterpret_cast<std::uintptr_t>(actor)){
        ReleaseSRWLockShared(&armProbeLock);return;
    }
    NativeSkeleton current{};
    bool valid=networkFrameOwned.load(std::memory_order_acquire)&&readNativeSkeleton(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),s.handle,current)&&
        current.stable&&current.logic==1&&current.game==s.game&&current.actor==s.actor&&current.animation==s.animation&&current.skeleton==s.skeleton&&current.parents==s.rig.parents;
    BodyMatrix correction=bodyIdentity();
    if(valid&&s.rig.directModelSpace){
        NativeSwordEquipment weapon{};float q[4]{},again[4]{};
        valid=readNativeSwordEquipment(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),s.handle,weapon,localRole?NativeSwordScope::CorvoTutorial:NativeSwordScope::EmilyPartner)&&weapon.actor==s.actor&&weapon.game==s.game;
        if(valid&&weapon.equipped){
            valid=weapon.started&&memoryRead(weapon.frame+0x3e0,q,sizeof(q))&&memoryRead(weapon.frame+0x3e0,again,sizeof(again))&&!std::memcmp(q,again,sizeof(q));
            BodyMatrix targetBinding{},sourceBinding=bodyIdentity();
            // Frame binding quaternions use the conjugate of skeleton QTS.
            std::array<float,12> qts={-q[0],-q[1],-q[2],q[3],0,0,0,0,1,1,1,1};
            valid=valid&&bodyFromQts(qts,targetBinding);
            sourceBinding.v[0]=sourceBinding.v[10]=-1.f; // Verified native player binding: 180 degrees about Y.
            if(valid)correction=bodyMultiply(sourceBinding,bodyInverse(targetBinding));
        }
    }
    std::vector<BodyMatrix> local,result;
    if(valid){local.resize(current.localPose.size());for(unsigned j=0;j<local.size();++j)std::memcpy(local[j].v,current.localPose[j].data(),64);valid=retargetArm(s.rig,s.source,local,result,s.rig.directModelSpace?&correction:nullptr);}
    if(valid&&GetTickCount64()<s.until){
        for(unsigned k=0;k<s.rig.count;++k){const auto j=s.rig.targets[k];reinterpret_cast<void(*)(void*,std::uint16_t,const BodyMatrix*)>(gameImage+0x9cfa0)(reinterpret_cast<void*>(s.animation+0x40),static_cast<std::uint16_t>(j),&result[j]);}
        reinterpret_cast<void(*)(void*)>(gameImage+0x9dc40)(reinterpret_cast<void*>(s.animation+0x40));
        if(s.remoteDriven){++armStreamApplied;armStreamAppliedSequence.store(s.sequence);}else ++armProbeApplied;
        armProbeThread.store(GetCurrentThreadId());
    }else {if(s.remoteDriven)++armStreamRefused;else ++armProbeRefused;}
    ReleaseSRWLockShared(&armProbeLock);
}
void writeArmProbe(ULONGLONG tick){
    AcquireSRWLockShared(&armProbeLock);const auto s=armProbeLast;const auto until=armProbe.remoteDriven?0:armProbe.until;const auto reason=armProbeResult;ReleaseSRWLockShared(&armProbeLock);
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.arm-probe.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\"localExperiment\":true,\"active\":%s,\"remainingMs\":%llu,\"result\":\"%s\",\"started\":%llu,\"applied\":%llu,\"refused\":%llu,\"thread\":%lu,\"epoch\":%llu,\"sourceHandle\":%u,\"targetHandle\":%u,\"targets\":[",
        until?"true":"false",until>tick?until-tick:0,reason,armProbeStarted,armProbeApplied.load(),armProbeRefused.load(),armProbeThread.load(),s.epoch,s.sourceHandle,s.handle);
    for(unsigned k=0;k<5;++k)fprintf(f,"%s%u",k?",":"",s.rig.targets[k]);fputs("],\"source\":[",f);
    for(unsigned k=0;k<5;++k){fputs(k?",[":"[",f);for(unsigned c=0;c<4;++c)for(unsigned a=0;a<3;++a)fprintf(f,"%s%.9g",c||a?",":"",s.source[k].v[c*4+a]);fputs("]",f);}
    fputs("]}\n",f);const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
