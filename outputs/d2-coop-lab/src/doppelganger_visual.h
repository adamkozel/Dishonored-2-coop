#pragma once
#include "native_doppelganger_visual.h"
// One cosmetic idEntity per world. No AI, physics, health, Decoy component,
// inventory, targeting or action events exist on this model.
struct DoppelgangerCloneLease {
    NativeDoppelgangerVisual target{};std::uintptr_t game=0,skeleton=0;ULONGLONG until=0;
    std::uint64_t sequence=0;std::array<BodyMatrix,kDoppelgangerJoints> local{};
};
// Probe brackets native phase3 publication and this exact expected pose.
struct DoppelgangerClonePublication {
    std::atomic<std::uint64_t> sequence{0};std::atomic<std::uintptr_t> animation{0};
    std::uint64_t sourceSequence=0;std::array<std::array<float,12>,kDoppelgangerJoints> expected{};
} doppelgangerClonePublication;
static_assert(offsetof(DoppelgangerClonePublication,expected)==24);
void doppelgangerClonePosePhase(void* component,int phase,bool finished){
    auto& p=doppelgangerClonePublication;if(phase!=3||p.animation.load()!=reinterpret_cast<std::uintptr_t>(component))return;
    if(finished){const auto seq=p.sequence.load();p.sequence.fetch_add((seq&1)?1:2);}else if(!(p.sequence.load()&1))p.sequence.fetch_add(1);
}
// 1214eh (#11 power_upgrade_variants): one cosmetic clone, lease and effect presentation per clone slot.
SRWLOCK doppelgangerCloneLock=SRWLOCK_INIT;std::array<DoppelgangerCloneLease,kDoppelgangerWireSlots> doppelgangerCloneLeases{};
struct DoppelgangerVisual {
    NativeDoppelgangerVisual target{};std::uintptr_t game=0,skeleton=0;std::uint64_t epoch=0;
    ULONGLONG readyAfter=0;bool visible=false;float alpha=0,error=0,maxError=0;
    std::array<float,12> transform{};std::uint64_t sequence=0,event=0;
};
std::array<DoppelgangerVisual,kDoppelgangerWireSlots> doppelgangerVisuals{};DoppelgangerVisual& doppelgangerVisual=doppelgangerVisuals[0];
std::uint64_t doppelgangerVisualCreated=0,doppelgangerVisualShown=0,doppelgangerVisualHidden=0,doppelgangerVisualRefused=0,doppelgangerVisualUpdated=0,doppelgangerVisualInterpolated=0;
std::atomic<std::uint64_t> doppelgangerCloneApplied{0},doppelgangerCloneRefused{0},doppelgangerCloneAppliedSequence{0};
const char* doppelgangerVisualResult="idle";double doppelgangerVisualMaxMs=0;
struct DoppelgangerHideRecord{std::uint64_t tick=0,sequence=0,event=0;unsigned mask=0,pairMask=0;long long receiveAge=0,leaseRemaining=0;};
std::array<DoppelgangerHideRecord,16> doppelgangerHideHistory{};unsigned doppelgangerHideCount=0;
void clearDoppelgangerCloneLease(unsigned slot){AcquireSRWLockExclusive(&doppelgangerCloneLock);doppelgangerCloneLeases[slot].until=0;ReleaseSRWLockExclusive(&doppelgangerCloneLock);}
bool currentDoppelgangerVisual(const NativeDoppelgangerVisual& expected,std::uintptr_t game){
    NativeDoppelgangerVisual actual{};return readNativeDoppelgangerVisual(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),game,expected.handle,actual)&&
        actual.actor==expected.actor&&actual.definition==expected.definition&&actual.animation==expected.animation&&actual.frame==expected.frame&&
        actual.render==expected.render&&actual.model==expected.model&&actual.animationGeneration==expected.animationGeneration&&actual.frameGeneration==expected.frameGeneration;
}
#include "doppelganger_fx_scene.h"
#include "doppelganger_fx_presentation.h"
void applyDoppelgangerCloneBody(void* component,void* actor){
    AcquireSRWLockShared(&doppelgangerCloneLock);const DoppelgangerCloneLease* found=nullptr;const auto now=GetTickCount64();
    for(const auto& x:doppelgangerCloneLeases)if(x.until&&now<x.until&&x.target.animation==reinterpret_cast<std::uintptr_t>(component)&&x.target.actor==reinterpret_cast<std::uintptr_t>(actor))found=&x;
    if(!found){ReleaseSRWLockShared(&doppelgangerCloneLock);return;}const auto& s=*found;
    std::uintptr_t game=0,skeleton=0;unsigned count=0;
    if(memoryRead(reinterpret_cast<std::uintptr_t>(gameImage)+0x39a5290,&game,8)&&game==s.game&&currentDoppelgangerVisual(s.target,game)&&
       memoryRead(s.target.animation+0x80,&skeleton,8)&&skeleton==s.skeleton&&memoryRead(s.target.animation+0x190,&count,4)&&(count&0xffffff)==kDoppelgangerJoints&&bodyNativeFunctions()){
        for(unsigned j=0;j<kDoppelgangerJoints;++j)reinterpret_cast<void(*)(void*,std::uint16_t,const BodyMatrix*)>(gameImage+0x9cfa0)(reinterpret_cast<void*>(s.target.animation+0x40),static_cast<std::uint16_t>(j),&s.local[j]);
        reinterpret_cast<void(*)(void*)>(gameImage+0x9dc40)(reinterpret_cast<void*>(s.target.animation+0x40));
        if((doppelgangerClonePublication.sequence.load()&1)&&doppelgangerClonePublication.animation.load()==s.target.animation){
            doppelgangerClonePublication.sourceSequence=s.sequence;
            for(unsigned j=0;j<kDoppelgangerJoints;++j)for(unsigned c=0;c<4;++c)for(unsigned r=0;r<3;++r)doppelgangerClonePublication.expected[j][c*3+r]=s.local[j].v[c*4+r];
        }
        ++doppelgangerCloneApplied;doppelgangerCloneAppliedSequence.store(s.sequence);
    }else ++doppelgangerCloneRefused;
    ReleaseSRWLockShared(&doppelgangerCloneLock);
}
void serviceDoppelgangerVisualSlot(ULONGLONG tick,unsigned slot){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;const double started=presentationClock();
    auto& p=doppelgangerVisuals[slot];const auto& s=doppelgangerWorldStreams[slot];
    const auto refuse=[&](const char* why){++doppelgangerVisualRefused;doppelgangerVisualResult=why;clearDoppelgangerCloneLease(slot);};
    if(p.target.actor&&(p.game!=observedGame||p.epoch!=outgoing.worldEpoch)){clearDoppelgangerCloneLease(slot);p={};}
    const bool active=localRole==0&&doppelgangerWorldCurrent(tick)&&s.lease>tick&&s.presented.present&&movementBankActive()&&
        coopGameplayReady(outgoing)&&coopGameplayReady(remote)&&((observerIsolation.ready&&observerIsolation.movementFixture)||productSessionArmed()); // 1214cp: product mode too (#104)
    if(!active){
        if(p.visible){
            const unsigned mask=(!doppelgangerWorldCurrent(tick)?1u:0u)|(s.lease<=tick?2u:0u)|(!s.presented.present?4u:0u)|(!movementBankActive()?8u:0u)|
                (!coopGameplayReady(outgoing)?16u:0u)|(!coopGameplayReady(remote)?32u:0u)|(!observerIsolation.ready?64u:0u)|(!observerIsolation.movementFixture?128u:0u);
            const unsigned pairMask=(!probeFollow?1u:0u)|(!networkFrameOwned.load()?2u:0u)|(!coopWorldPairReady(outgoing,remote)?4u:0u)|(!probeFollowExact?8u:0u)|
                (!avatarAiHeld?16u:0u)|(avatarAiTarget.actor!=probeActor?32u:0u)|(!locomotionEnabled?64u:0u)|(shadowStream.until<=tick?128u:0u);
            doppelgangerHideHistory[doppelgangerHideCount++%16]={tick,s.presented.sequence,s.presented.event,mask,pairMask,static_cast<long long>(tick)-static_cast<long long>(lastReceive),static_cast<long long>(s.lease)-static_cast<long long>(tick)};
        }
        serviceDoppelgangerFx(tick,slot,p.target,p.game,p.epoch,false);clearDoppelgangerCloneLease(slot);if(p.visible&&currentDoppelgangerVisual(p.target,p.game)&&powerFixtureApiMatches()){
            reinterpret_cast<void(*)(void*)>(gameImage+0x8784b0)(reinterpret_cast<void*>(p.target.actor));p.visible=false;++doppelgangerVisualHidden;doppelgangerVisualResult="hidden";}return;}
    if(!partnerAssetsEnabled||!coopWorldPairReady(outgoing,remote)||!powerFixtureApiMatches()||!rangedPropApiMatches()||!bodyNativeFunctions()){refuse("unsupported_scope");return;}
    const auto* next=s.count?&s.pending[0].packet:nullptr;const double nextTime=s.count?s.pending[0].time:0;
    if(!doppelgangerWorldPresentation(s.presented,s.presentedTime,next,nextTime,presentationClock()-120.,p.transform,p.alpha)){refuse("no_presented_transform");return;}
    if(!p.target.actor){
        const auto def=lookupEntityDefinition(kDoppelgangerVisualDefinition);if(!def){refuse("visual_definition_missing");return;}
        unsigned char parameters[12]{};reinterpret_cast<void*(*)(void*)>(gameImage+0x146520)(parameters);
        void* actor=reinterpret_cast<void*(*)(void*,void*,void*)>(gameImage+0x8a78f0)(reinterpret_cast<void*>(observedGame),reinterpret_cast<void*>(def),parameters);
        if(!actor){refuse("creation_refused");return;}
        p.target.actor=reinterpret_cast<std::uintptr_t>(actor);p.target.definition=def;p.game=observedGame;p.epoch=outgoing.worldEpoch;++doppelgangerVisualCreated;
        reinterpret_cast<void(*)(void*,const float*)>(gameImage+0x87c320)(actor,p.transform.data());reinterpret_cast<void(*)(void*,const float*)>(gameImage+0x87c2c0)(actor,p.transform.data()+3);
        reinterpret_cast<void(*)(void*,void*,int)>(gameImage+0x8aecc0)(reinterpret_cast<void*>(observedGame),actor,-1);memoryRead(p.target.actor+0xa8,&p.target.handle,4);
        // Let the native animation initialize, then use the actual model/rig
        // readiness checks below. The old fixed200ms wait visibly delayed FX.
        reinterpret_cast<void(*)(void*)>(gameImage+0x8784b0)(actor);p.readyAfter=tick+16;
    }
    if(tick<p.readyAfter)return;
    NativeDoppelgangerVisual target{};
    if(!readNativeDoppelgangerVisual(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),p.game,p.target.handle,target)||target.actor!=p.target.actor||target.definition!=p.target.definition){refuse("visual_identity_refused");return;}
    if(!p.skeleton){NativeSkeleton rig{};
        if(!readNativeSkeleton(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),target.handle,rig,NativeSkeletonScope::CosmeticDoppelganger)||doppelgangerRig(rig)!=kDoppelgangerRig){refuse("visual_skeleton_not_ready");return;}
        p.skeleton=rig.skeleton;p.target=target;if(!slot)doppelgangerClonePublication.animation.store(target.animation);
    }else if(!currentDoppelgangerVisual(p.target,p.game)){refuse("visual_incarnation_changed");return;}
    DoppelgangerCloneLease lease{};lease.game=p.game;lease.skeleton=p.skeleton;lease.target=p.target;lease.until=tick+300;lease.sequence=s.presented.sequence;
    for(unsigned j=0;j<kDoppelgangerJoints;++j){auto local=s.presented.local[j];
        if(p.alpha>0&&next){presentationRotation(s.presented.local[j].data(),next->local[j].data(),p.alpha,local.data());for(unsigned k=9;k<12;++k)local[k]+=p.alpha*(next->local[j][k]-local[k]);}
        lease.local[j]=doppelgangerLocalMatrix(local);if(!doppelgangerJointValid(j,lease.local[j])){refuse("pose_refused");return;}
    }
    const unsigned char code[]={0x48,0x8b,0xc4,0x44,0x88,0x48,0x20,0x48,0x89,0x50,0x10,0x55,0x56,0x57,0x48,0x8d};
    std::uintptr_t vt=0,set=0;const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(std::memcmp(gameImage+0x87d200,code,sizeof(code))||!memoryRead(p.target.frame,&vt,8)||vt!=base+0x1c573e8||!memoryRead(vt+22*8,&set,8)||set!=base+0x751d00){refuse("frame_dispatch_refused");return;}
    AcquireSRWLockExclusive(&doppelgangerCloneLock);doppelgangerCloneLeases[slot]=lease;ReleaseSRWLockExclusive(&doppelgangerCloneLock);
    reinterpret_cast<void(*)(void*,const float*,const float*,bool,int)>(gameImage+0x87d200)(reinterpret_cast<void*>(p.target.actor),p.transform.data(),p.transform.data()+3,true,0);
    std::array<float,12> actual{};if(!memoryRead(p.target.actor+0x300,actual.data(),48)){refuse("frame_read_refused");return;}
    float error=0;for(unsigned i=0;i<12;++i){if(!std::isfinite(actual[i])){refuse("frame_nonfinite");return;}error=(std::max)(error,std::fabs(actual[i]-p.transform[i]));}
    p.error=error;p.maxError=(std::max)(p.maxError,error);if(error>.001f){refuse("frame_mismatch");return;}
    if(!p.visible){applyDoppelgangerCloneBody(reinterpret_cast<void*>(p.target.animation),reinterpret_cast<void*>(p.target.actor));
        reinterpret_cast<void(*)(void*)>(gameImage+0x87c590)(reinterpret_cast<void*>(p.target.actor));reinterpret_cast<void(*)(void*)>(gameImage+0x87c5d0)(reinterpret_cast<void*>(p.target.actor));p.visible=true;++doppelgangerVisualShown;}
    p.sequence=lease.sequence;p.event=s.presented.event;++doppelgangerVisualUpdated;if(p.alpha>0)++doppelgangerVisualInterpolated;doppelgangerVisualResult="native_clone_pose";
    serviceDoppelgangerFx(tick,slot,p.target,p.game,p.epoch,true);
    doppelgangerVisualMaxMs=(std::max)(doppelgangerVisualMaxMs,presentationClock()-started);
}
void serviceDoppelgangerVisual(ULONGLONG tick){for(unsigned k=0;k<kDoppelgangerWireSlots;++k)serviceDoppelgangerVisualSlot(tick,k);}
void writeDoppelgangerVisual(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.doppelganger-visual.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;
    const auto& p=doppelgangerVisual;
    fprintf(f,"{\"visible\":%s,\"handle\":%u,\"sequence\":%llu,\"event\":%llu,\"created\":%llu,\"shown\":%llu,\"hidden\":%llu,\"refused\":%llu,\"updated\":%llu,\"interpolated\":%llu,\"applied\":%llu,\"poseRefused\":%llu,\"appliedSequence\":%llu,\"jointCount\":219,\"alpha\":%.6f,\"maxTransformError\":%.8f,\"serviceMaxMs\":%.4f,\"result\":\"%s\",\"transform\":[",p.visible?"true":"false",p.target.handle,p.sequence,p.event,doppelgangerVisualCreated,doppelgangerVisualShown,doppelgangerVisualHidden,doppelgangerVisualRefused,doppelgangerVisualUpdated,doppelgangerVisualInterpolated,doppelgangerCloneApplied.load(),doppelgangerCloneRefused.load(),doppelgangerCloneAppliedSequence.load(),p.alpha,p.maxError,doppelgangerVisualMaxMs,doppelgangerVisualResult);
    for(unsigned i=0;i<12;++i)fprintf(f,"%s%.7g",i?",":"",p.transform[i]);fprintf(f,"],\"publication\":\"0x%llx\",\"publicationSequence\":%llu",reinterpret_cast<unsigned long long>(&doppelgangerClonePublication),doppelgangerClonePublication.sequence.load());writeDoppelgangerFx(f);{const auto& q=doppelgangerVisuals[1];fprintf(f,",\"slot1\":{\"visible\":%s,\"handle\":%u,\"sequence\":%llu,\"event\":%llu,\"result\":\"%s\"}",q.visible?"true":"false",q.target.handle,q.sequence,q.event,doppelgangerFxPresentations[1].scene.handle?"fx_active":"fx_idle");}
    fputs(",\"hideHistory\":[",f);const auto count=(std::min)(doppelgangerHideCount,16u);for(unsigned i=0;i<count;++i){const auto& r=doppelgangerHideHistory[(doppelgangerHideCount-count+i)%16];
        fprintf(f,"%s{\"tick\":%llu,\"sequence\":%llu,\"event\":%llu,\"mask\":%u,\"pairMask\":%u,\"receiveAge\":%lld,\"leaseRemaining\":%lld}",i?",":"",r.tick,r.sequence,r.event,r.mask,r.pairMask,r.receiveAge,r.leaseRemaining);}
    fputs("]}\n",f);const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
