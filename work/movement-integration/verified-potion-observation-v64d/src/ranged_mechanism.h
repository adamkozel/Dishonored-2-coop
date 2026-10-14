#pragma once
#include "crossbow_mechanism_data.h"
// Only our exact, purely cosmetic crossbow child is eligible. Source weapon
// components, native firing messages, inventory and projectiles are untouched.
SRWLOCK rangedMechanismLock=SRWLOCK_INIT;
struct RangedMechanismState {
    std::uintptr_t game=0,actor=0,animation=0,skeleton=0;unsigned handle=0,character=0,joints=0,variant=0;
    std::uint64_t epoch=0;ULONGLONG until=0;std::array<BodyMatrix,33> pose{};
} rangedMechanism;
std::atomic<std::uint64_t> rangedMechanismApplied{0},rangedMechanismRefused{0};
void stopRangedMechanism(){AcquireSRWLockExclusive(&rangedMechanismLock);rangedMechanism.until=0;ReleaseSRWLockExclusive(&rangedMechanismLock);}
bool publishRangedMechanism(unsigned handle,unsigned variant,float phase,unsigned mechanism,ULONGLONG until){
    NativeSkeleton s{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);NativeCosmeticRanged prop{};
    if(!bodyNativeFunctions()||!readNativeCosmeticRanged(memoryRead,base,observedGame,handle,prop)||prop.kind!=2||
       !readNativeSkeleton(memoryRead,base,handle,s,NativeSkeletonScope::CosmeticRanged)||!s.stable||s.logic!=1||s.game!=observedGame||
       prop.owner!=probeActor||prop.ownerHandle!=probeHandle)return false;
    if(variant!=7&&!mechanism){stopRangedMechanism();return true;} // Unknown native state supplies no guessed mechanical pose.
    const auto clip=crossbowMechanism(prop.character,variant==7?7:mechanism==2?2:0);if(s.parents.size()!=clip.joints)return false;
    const auto* parents=prop.character==1?crossbow_corvo_parents:crossbow_emily_parents;
    const auto* bind=prop.character==1?crossbow_corvo_bind:crossbow_emily_bind;
    for(unsigned j=0;j<clip.joints;++j){
        if(s.parents[j]!=parents[j])return false;
        float same=0,opposite=0;for(unsigned k=0;k<4;++k){same+=std::fabs(s.bindLocal[j][k]-bind[j][k]);opposite+=std::fabs(s.bindLocal[j][k]+bind[j][k]);}
        if((std::min)(same,opposite)>.00001f)return false; // q and -q encode the same rotation.
        for(unsigned k=4;k<7;++k)if(std::fabs(s.bindLocal[j][k]-bind[j][k])>.00001f)return false;
    }
    RangedMechanismState next{};next.game=s.game;next.actor=s.actor;next.animation=s.animation;next.skeleton=s.skeleton;next.handle=handle;
    next.character=prop.character;next.joints=clip.joints;next.variant=variant;next.epoch=outgoing.worldEpoch;next.until=until;
    // Native Fire schedules the mechanical clip for 333ms within its 666ms
    // arm scene. Idle/fired selection follows the delayed native defaultAnim identity,
    // including reload contact and an empty weapon held after the action ends.
    const float f=(std::max)(0.f,(std::min)(1.f,variant==7?phase*2.f:phase))*(clip.frames-1);
    const unsigned a=unsigned(f),b=(std::min)(clip.frames-1,a+1);
    for(unsigned j=0;j<clip.joints;++j){
        MovementBank::Joint x{},y{};std::copy(clip.qts[a*clip.joints+j],clip.qts[a*clip.joints+j]+7,x.begin());std::copy(clip.qts[b*clip.joints+j],clip.qts[b*clip.joints+j]+7,y.begin());
        const auto q=MovementBank::blend(x,y,f-a);std::array<float,12> qts{};std::copy(q.begin(),q.end(),qts.begin());qts[8]=qts[9]=qts[10]=1;
        if(!bodyFromQts(qts,next.pose[j]))return false;
    }
    AcquireSRWLockExclusive(&rangedMechanismLock);rangedMechanism=next;ReleaseSRWLockExclusive(&rangedMechanismLock);return true;
}
void applyRangedMechanism(void* component,void* actor){
    AcquireSRWLockShared(&rangedMechanismLock);const auto& p=rangedMechanism;const auto tick=GetTickCount64();
    if(!p.until||tick>=p.until||p.actor!=reinterpret_cast<std::uintptr_t>(actor)||p.animation!=reinterpret_cast<std::uintptr_t>(component)){
        ReleaseSRWLockShared(&rangedMechanismLock);return;}
    NativeCosmeticRanged prop{};std::uintptr_t skeleton=0;
    if(p.game==observedGame&&p.epoch==outgoing.worldEpoch&&coopGameplayReady(outgoing)&&coopGameplayReady(remote)&&
       readNativeCosmeticRanged(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),p.game,p.handle,prop)&&
       prop.actor==p.actor&&prop.animation==p.animation&&prop.kind==2&&prop.character==p.character&&prop.owner==probeActor&&
       memoryRead(p.animation+0x80,&skeleton,8)&&skeleton==p.skeleton){
        for(unsigned j=0;j<p.joints;++j)reinterpret_cast<void(*)(void*,std::uint16_t,const BodyMatrix*)>(gameImage+0x9cfa0)(reinterpret_cast<void*>(p.animation+0x40),static_cast<std::uint16_t>(j),&p.pose[j]);
        reinterpret_cast<void(*)(void*)>(gameImage+0x9dc40)(reinterpret_cast<void*>(p.animation+0x40));++rangedMechanismApplied;
    }else ++rangedMechanismRefused;
    ReleaseSRWLockShared(&rangedMechanismLock);
}
