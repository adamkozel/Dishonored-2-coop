#include "body_crouch.h"
#include "mixamo_crouch.h"
// Explicitly armed pose research. Remote poses use the existing native Steam
// channel. Native evaluation regenerates the pose after cancellation.
SRWLOCK bodyProbeLock=SRWLOCK_INIT;
struct BodyProbeState {
    ULONGLONG until=0;std::uintptr_t game=0,actor=0,animation=0,skeleton=0;std::uint32_t handle=0;
    std::uint64_t epoch=0,peerSession=0,peerEpoch=0;bool remoteDriven=false,shadowCrouch=false,uprightTorso=false,mixamoCorvo=false;
    float mixamoPhase=0;
    BodyRetarget rig;std::array<BodyMatrix,20> source{},sourceCrouchReference{};
    std::vector<BodyMatrix> nativeCrouch;
};
BodyProbeState bodyProbe{};
struct NativeCrouchReference {
    std::uintptr_t game=0,actor=0,skeleton=0;std::uint32_t handle=0;
    std::uint64_t epoch=0,peerSession=0,peerEpoch=0;
    std::vector<BodyMatrix> local;
    std::array<BodyMatrix,20> sourceReference{};
};
NativeCrouchReference nativeCrouchReference{}; // engine-thread capture only
// External read-only probes bracket snapshots with this stage sequence. Odd
// means native evaluation is between phase0 and final phase3 publication.
struct BodyPublication {std::atomic<std::uint64_t> sequence{0};std::atomic<std::uintptr_t> animation{0};};
BodyPublication bodyPublication{};
std::atomic<std::uint64_t> bodyProbeApplied{0},bodyProbeRefused{0};
std::atomic<DWORD> bodyProbeThread{0};
bool bodyStreamEnabled=false;
bool bodyCrouchUpright=false; // engine-thread selection, copied under pose lock
std::atomic<std::uint64_t> bodyStreamApplied{0};
std::atomic<std::uint64_t> bodyCrouchHelpersApplied{0};
struct MixamoResearch {ULONGLONG until=0;std::uintptr_t actor=0;std::uint64_t epoch=0,peerEpoch=0,peerSession=0;float position[3]{},phase=0;bool positioned=false;} mixamoResearch;
std::atomic<std::uint64_t> mixamoApplied{0};
bool bodyShadowTarget(const NativeSkeleton& target){
    return (!std::strcmp(target.modelName,"models/characters/d2coop/shadow_emily.md6")&&
        !std::strcmp(target.skeletonName,"models/characters/female/emily/head01/emily_head01.md6skl")&&target.parents.size()==219)||
        (!std::strcmp(target.modelName,"models/characters/d2coop/shadow_corvo.md6")&&
        !std::strcmp(target.skeletonName,"models/characters/tall/str/corvo/head01/t_str_corvo01_head01.md6skl")&&target.parents.size()==215);
}
bool bodyKnownSkeletons(const NativeSkeleton& source,const NativeSkeleton& target){
    return !std::strcmp(source.skeletonName,"models/characters/player/emily/player_body_emily.md6skl")&&
        (bodyShadowTarget(target)||!std::strcmp(target.skeletonName,"models/characters/female/emily/emily.md6skl")||!std::strcmp(target.skeletonName,"models/characters/tall/str/corvo/t_str_corvo01.md6skl"));
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
void clearBodyProbe(){AcquireSRWLockExclusive(&bodyProbeLock);bodyProbe={};ReleaseSRWLockExclusive(&bodyProbeLock);}
bool nativeCrouchReferenceReady(){const auto& c=nativeCrouchReference;return c.actor&&c.actor==probeActor&&c.game==observedGame&&c.handle==probeHandle&&
    c.epoch==outgoing.worldEpoch&&c.peerSession==remote.session&&c.peerEpoch==remote.worldEpoch&&!c.local.empty();}
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
    next.shadowCrouch=bodyShadowTarget(target);
    for(unsigned j=0;j<20;++j){std::memcpy(next.source[j].v,source.globalPose[j].data(),64);if(!bodyRigid(next.source[j])){++bodyProbeRefused;return;}}
    next.game=target.game;next.actor=target.actor;next.animation=target.animation;next.skeleton=target.skeleton;next.handle=probeHandle;next.until=GetTickCount64()+2000;
    AcquireSRWLockExclusive(&bodyProbeLock);bodyProbe=std::move(next);ReleaseSRWLockExclusive(&bodyProbeLock);
}
void publishBodyStream(ULONGLONG tick){
    if(!bodyStreamEnabled)return;
    if(!probeFollow||!networkFrameOwned.load()||!coopGameplayReady(outgoing)||!coopGameplayReady(remote)||(!remote.bodyPoseValid&&!probeDoppelganger)||
       !lastReceive||tick-lastReceive>=250||(!probeDoppelganger&&!remote.playerStance&&remote.playerMotion==0)){stopBodyProbe();return;}
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
        next.shadowCrouch=bodyShadowTarget(target);
        if(next.shadowCrouch){const auto& c=nativeCrouchReference;
            if(c.game!=target.game||c.actor!=target.actor||c.skeleton!=target.skeleton||c.handle!=probeHandle||
               c.epoch!=outgoing.worldEpoch||c.peerSession!=remote.session||c.peerEpoch!=remote.worldEpoch||c.local.size()!=target.parents.size()){
                ++bodyProbeRefused;stopBodyProbe();return;
            }
            next.nativeCrouch=c.local;
            next.sourceCrouchReference=c.sourceReference;
        }
        next.game=target.game;next.actor=target.actor;next.animation=target.animation;next.skeleton=target.skeleton;next.handle=probeHandle;
        next.epoch=outgoing.worldEpoch;next.peerEpoch=remote.worldEpoch;next.peerSession=remote.session;next.remoteDriven=true;
    }
    for(unsigned j=0;j<20;++j)for(unsigned c=0;c<4;++c)for(unsigned r=0;r<3;++r)next.source[j].v[c*4+r]=remote.bodyPose[j][c*3+r];
    if(next.shadowCrouch){
        PresentationPose sampled{};sampled.bodyValid=remote.bodyPoseValid!=0;sampled.stance=remote.playerStance;sampled.motion=remote.playerMotion;
        std::memcpy(sampled.body,remote.bodyPose,sizeof(sampled.body));
        if(poseSmoothingEnabled&&!poseInterpolation.sampleBody(presentationClock(),sampled)){stopBodyProbe();return;}
        // Airborne keeps the verified native clone loop. Follow the native
        // crouch-out pelvis until it rises, instead of cutting off at a flag.
        const float drop=sampled.body[1][11]-next.rig.sourceBindGlobal[1].v[14];
        if(!sampled.bodyValid||sampled.motion>1||remote.playerMotion>1||(!sampled.stance&&drop>-.04f)){stopBodyProbe();return;}
        for(unsigned j=0;j<20;++j)for(unsigned c=0;c<4;++c)for(unsigned r=0;r<3;++r)next.source[j].v[c*4+r]=sampled.body[j][c*3+r];
    }
    next.until=lastReceive+250;
    next.uprightTorso=bodyCrouchUpright;
    auto& m=mixamoResearch;
    next.mixamoCorvo=next.shadowCrouch&&localRole==1&&tick<m.until&&m.actor==probeActor&&m.epoch==outgoing.worldEpoch&&m.peerEpoch==remote.worldEpoch&&m.peerSession==remote.session&&next.rig.parents.size()==215;
    if(next.mixamoCorvo){
        if(m.positioned){float d2=0;for(unsigned i=0;i<2;++i){const float d=probeTargetPosition[i]-m.position[i];d2+=d*d;}
            if(d2>.04f){m.until=0;next.mixamoCorvo=false;} // refuse teleport-sized step
            else if(d2>.00000001f)m.phase=std::fmod(m.phase+std::sqrt(d2)/kMixamoCorvoStride,1.f); // measured retargeted contact stride; forward-only experiment
        }
        std::memcpy(m.position,probeTargetPosition,12);m.positioned=true;next.mixamoPhase=m.phase;
    }
    if(next.shadowCrouch)bodyPublication.animation.store(next.animation,std::memory_order_release);
    AcquireSRWLockExclusive(&bodyProbeLock);bodyProbe=std::move(next);ReleaseSRWLockExclusive(&bodyProbeLock);
}
void applyBodyProbe(void* component,void* actor,bool finalPose=false){
    AcquireSRWLockShared(&bodyProbeLock);
    if(bodyProbe.animation!=reinterpret_cast<std::uintptr_t>(component)||bodyProbe.actor!=reinterpret_cast<std::uintptr_t>(actor)||
       !bodyProbe.until||GetTickCount64()>=bodyProbe.until||bodyProbe.shadowCrouch!=finalPose){ReleaseSRWLockShared(&bodyProbeLock);return;}
    // The original animation routine has just returned for this same owner.
    // Keep the lease lock through native calls; cancellation cannot race a call.
    const auto& state=bodyProbe;NativeSkeleton current{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    bool valid=networkFrameOwned.load(std::memory_order_acquire)&&readNativeSkeleton(memoryRead,base,state.handle,current)&&current.stable&&
        current.logic==1&&current.game==state.game&&current.actor==state.actor&&current.animation==state.animation&&current.skeleton==state.skeleton&&
        current.parents==state.rig.parents&&GetTickCount64()<state.until;
    std::vector<BodyMatrix> local,result;
    if(valid){local.resize(current.localPose.size());for(unsigned j=0;j<local.size();++j)std::memcpy(local[j].v,current.localPose[j].data(),64);
        valid=state.mixamoCorvo?composeMixamoCorvo(state.rig,state.source,local,state.mixamoPhase,result):(state.shadowCrouch?composeNativeCrouch(state.rig,state.source,state.sourceCrouchReference,state.nativeCrouch,local,result,state.uprightTorso):retargetBody(state.rig,state.source,local,result));}
    // 541110 calls model helper evaluation immediately BEFORE publishing phase3.
    // Our late pose write invalidates those corrective joints again. A forward
    // hierarchy rebuild alone cannot recompute their non-parent dependencies.
    if(valid&&state.shadowCrouch){
        const unsigned char helperEntry[]={0x40,0x55,0x53,0x41,0x56,0x41,0x57,0x48,0x8d,0x6c,0x24,0xb8,0x48,0x81,0xec,0x48};
        std::uintptr_t helpers=0;unsigned count=0,maskWords=0,maskJoints=0;
        valid=!std::memcmp(gameImage+0x23fca0,helperEntry,sizeof(helperEntry))&&
            memoryRead(current.model+0xa0,&helpers,8)&&helpers&&
            memoryRead(current.model+0xa8,&count,4)&&(count&0xffffff)>0&&(count&0xffffff)<=256&&
            memoryRead(current.animation+0x2d0,&maskWords,4)&&maskWords>0&&maskWords<=8&&
            memoryRead(current.animation+0x2d4,&maskJoints,4)&&maskJoints==result.size();
    }
    if(valid){
        for(unsigned j=0;j<result.size();++j)if(state.shadowCrouch?j>0:state.rig.sourceForTarget[j]>=0)
            reinterpret_cast<void(*)(void*,std::uint16_t,const BodyMatrix*)>(gameImage+0x9cfa0)(reinterpret_cast<void*>(state.animation+0x40),static_cast<std::uint16_t>(j),&result[j]);
        reinterpret_cast<void(*)(void*)>(gameImage+0x9dc40)(reinterpret_cast<void*>(state.animation+0x40));
        if(state.shadowCrouch){
            reinterpret_cast<void(*)(void*,void*,void*)>(gameImage+0x23fca0)(
                reinterpret_cast<void*>(current.model),reinterpret_cast<void*>(state.animation+0x40),
                reinterpret_cast<void*>(state.animation+0x2b0));
            ++bodyCrouchHelpersApplied;
        }
        ++bodyProbeApplied;if(state.remoteDriven)++bodyStreamApplied;if(state.mixamoCorvo)++mixamoApplied;bodyProbeThread.store(GetCurrentThreadId());
    }else ++bodyProbeRefused;
    ReleaseSRWLockShared(&bodyProbeLock);
}

// Native0x540a90 publishes animation-phase caches. Clone crouch must be
// applied before phase3 (final rendering), after native IK has finished.
// This composes the complete native torso/arms with player stepping, then runs
// native deformation helpers again before the final render cache is published.
using CapturePosePhase=void(*)(void*,int);
CapturePosePhase realCapturePosePhase=nullptr;
void capturePosePhase(void* component,int phase){
    const bool tracked=bodyPublication.animation.load(std::memory_order_acquire)==reinterpret_cast<std::uintptr_t>(component);
    if(tracked&&(phase==0||phase==3)&&!(bodyPublication.sequence.load()&1))bodyPublication.sequence.fetch_add(1);
    if(phase==3){std::uintptr_t actor=0;
        if(memoryRead(reinterpret_cast<std::uintptr_t>(component)+0x10,&actor,8)&&actor)
            applyBodyProbe(component,reinterpret_cast<void*>(actor),true);
    }
    realCapturePosePhase(component,phase);
    if(tracked&&phase==3){const auto seq=bodyPublication.sequence.load();bodyPublication.sequence.fetch_add((seq&1)?1:2);}
}
bool installBodyPoseCapture(){
    const unsigned char bytes[]={0x48,0x89,0x5c,0x24,0x20,0x55,0x56,0x57,0x41,0x54,0x41,0x55,0x48,0x83,0xec,0x20};
    auto* entry=gameImage+0x540a90;
    if(std::memcmp(entry,bytes,sizeof(bytes)))return false;
    auto* trampoline=static_cast<unsigned char*>(VirtualAlloc(nullptr,64,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));
    if(!trampoline)return false;
    std::memcpy(trampoline,bytes,sizeof(bytes));
    unsigned char jump[16]{0xff,0x25,0,0,0,0};void* resume=entry+sizeof(bytes);
    std::memcpy(jump+6,&resume,8);jump[14]=jump[15]=0x90;std::memcpy(trampoline+sizeof(bytes),jump,14);
    DWORD old=0,unused=0;
    if(!VirtualProtect(trampoline,64,PAGE_EXECUTE_READ,&old))return false;
    FlushInstructionCache(GetCurrentProcess(),trampoline,64);
    realCapturePosePhase=reinterpret_cast<CapturePosePhase>(trampoline);
    void* wrapper=reinterpret_cast<void*>(&capturePosePhase);std::memcpy(jump+6,&wrapper,8);
    if(!VirtualProtect(entry,sizeof(jump),PAGE_EXECUTE_READWRITE,&old))return false;
    std::memcpy(entry,jump,sizeof(jump));FlushInstructionCache(GetCurrentProcess(),entry,sizeof(jump));
    return VirtualProtect(entry,sizeof(jump),old,&unused)!=0;
}
