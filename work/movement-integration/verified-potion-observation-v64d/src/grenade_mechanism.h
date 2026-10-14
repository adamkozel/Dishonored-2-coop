#pragma once
#include "grenade_mechanism_data.h"
// Pure native model transforms on the exact cosmetic-only child. No native
// animation events, equipment component, projectile or source-player writes.
SRWLOCK grenadeMechanismLock=SRWLOCK_INIT;
struct GrenadeMechanismState {
    std::uintptr_t game=0,actor=0,animation=0,skeleton=0;unsigned handle=0,character=0,variant=0;
    std::uint64_t epoch=0;ULONGLONG until=0;std::array<BodyMatrix,6> pose{};
} grenadeMechanism;
std::atomic<std::uint64_t> grenadeMechanismApplied{0},grenadeMechanismRefused{0};
void stopGrenadeMechanism(){AcquireSRWLockExclusive(&grenadeMechanismLock);grenadeMechanism.until=0;ReleaseSRWLockExclusive(&grenadeMechanismLock);}
bool publishGrenadeMechanism(unsigned handle,unsigned variant,float amount,ULONGLONG until){
    NativeSkeleton s{};NativeCosmeticRanged prop{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage),tick=GetTickCount64();
    if(variant>8||!std::isfinite(amount)||amount<0||amount>1||until<=tick||until-tick>250||
       !bodyNativeFunctions()||!readNativeCosmeticRanged(memoryRead,base,observedGame,handle,prop)||prop.kind!=3||
       prop.owner!=probeActor||prop.ownerHandle!=probeHandle||
       !readNativeSkeleton(memoryRead,base,handle,s,NativeSkeletonScope::CosmeticRanged)||!s.stable||s.logic!=1||
       s.game!=observedGame||s.actor!=prop.actor||s.animation!=prop.animation||s.parents.size()!=6)return false;
    for(unsigned j=0;j<6;++j){
        if(s.parents[j]!=grenadeMechanismParents[j])return false;
        float same=0,opposite=0;
        for(unsigned k=0;k<4;++k){same+=std::fabs(s.bindLocal[j][k]-grenadeMechanismBind[j][k]);opposite+=std::fabs(s.bindLocal[j][k]+grenadeMechanismBind[j][k]);}
        if((std::min)(same,opposite)>.00001f)return false;
        for(unsigned k=4;k<7;++k)if(std::fabs(s.bindLocal[j][k]-grenadeMechanismBind[j][k])>.00001f)return false;
    }
    const auto clip=grenadeMechanismClip(variant);
    // Cooking entry/loop follow their exact replicated native phase. During
    // release the armed mechanism stays at the loop's terminal pose until Hide.
    const float phase=variant>=5?1.f:amount,f=phase*(clip.frames-1);
    const unsigned a=unsigned(f),b=(std::min)(clip.frames-1,a+1);
    GrenadeMechanismState next{};next.game=s.game;next.actor=s.actor;next.animation=s.animation;next.skeleton=s.skeleton;
    next.handle=handle;next.character=prop.character;next.variant=variant;next.epoch=outgoing.worldEpoch;next.until=until;
    for(unsigned j=0;j<6;++j){
        MovementBank::Joint x{},y{};std::copy(clip.qts[a*6+j],clip.qts[a*6+j]+7,x.begin());std::copy(clip.qts[b*6+j],clip.qts[b*6+j]+7,y.begin());
        const auto q=MovementBank::blend(x,y,f-a);std::array<float,12> qts{};std::copy(q.begin(),q.end(),qts.begin());qts[8]=qts[9]=qts[10]=1;
        if(!bodyFromQts(qts,next.pose[j]))return false;
    }
    AcquireSRWLockExclusive(&grenadeMechanismLock);grenadeMechanism=next;ReleaseSRWLockExclusive(&grenadeMechanismLock);return true;
}
void applyGrenadeMechanism(void* component,void* actor){
    AcquireSRWLockShared(&grenadeMechanismLock);const auto& p=grenadeMechanism;const auto tick=GetTickCount64();
    if(!p.until||tick>=p.until||p.actor!=reinterpret_cast<std::uintptr_t>(actor)||p.animation!=reinterpret_cast<std::uintptr_t>(component)){
        ReleaseSRWLockShared(&grenadeMechanismLock);return;
    }
    NativeCosmeticRanged prop{};std::uintptr_t skeleton=0;
    if(p.game==observedGame&&p.epoch==outgoing.worldEpoch&&coopGameplayReady(outgoing)&&coopGameplayReady(remote)&&
       readNativeCosmeticRanged(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),p.game,p.handle,prop)&&
       prop.actor==p.actor&&prop.animation==p.animation&&prop.kind==3&&prop.character==p.character&&prop.owner==probeActor&&prop.ownerHandle==probeHandle&&
       memoryRead(p.animation+0x80,&skeleton,8)&&skeleton==p.skeleton){
        for(unsigned j=0;j<6;++j)reinterpret_cast<void(*)(void*,std::uint16_t,const BodyMatrix*)>(gameImage+0x9cfa0)(reinterpret_cast<void*>(p.animation+0x40),static_cast<std::uint16_t>(j),&p.pose[j]);
        reinterpret_cast<void(*)(void*)>(gameImage+0x9dc40)(reinterpret_cast<void*>(p.animation+0x40));++grenadeMechanismApplied;
    }else ++grenadeMechanismRefused;
    ReleaseSRWLockShared(&grenadeMechanismLock);
}
