#pragma once
#include "movement_bank.h"
SRWLOCK movementLock=SRWLOCK_INIT;
struct MovementPlayback {
    MovementBank bank;MovementClock clock;
    std::uintptr_t game=0,actor=0,animation=0,skeleton=0;unsigned handle=0;
    std::uint64_t epoch=0,peerEpoch=0,session=0;ULONGLONG end=0,lease=0;
    std::vector<std::int16_t> parents;std::uint64_t applied=0,refused=0;bool persistent=false;
} movementPlayback;
// File validation grows with the authored library. Keep it entirely off the
// game thread; only the final incarnation/skeleton checks run on that thread.
struct MovementLoad {
    std::atomic<unsigned> state{0}; // 0 idle, 1 worker, 2 decoded, 3 failed
    std::atomic<ULONGLONG> decodeMs{0};
    MovementPlayback next;char path[MAX_PATH]{};unsigned joints=0;
    std::uint64_t layout=0;ULONGLONG deadline=0;bool requested=false;
} movementLoad;
DWORD WINAPI movementLoadWorker(void*){
    const auto start=GetTickCount64();bool ok=false;FILE* f=nullptr;
    try {
        if(!fopen_s(&f,movementLoad.path,"rb")&&f){
            std::vector<unsigned char> bytes(MovementBank::maximumBytes+1);
            const auto n=fread(bytes.data(),1,bytes.size(),f);const bool io=!ferror(f);fclose(f);f=nullptr;bytes.resize(n);
            ok=io&&movementLoad.next.bank.decode(bytes,movementLoad.layout,movementLoad.joints);
        }
    }catch(...){if(f)fclose(f);}
    movementLoad.decodeMs.store(GetTickCount64()-start,std::memory_order_relaxed);
    movementLoad.state.store(ok?2u:3u,std::memory_order_release);return 0;
}
bool movementBankActive(){AcquireSRWLockShared(&movementLock);const bool active=movementPlayback.end!=0;ReleaseSRWLockShared(&movementLock);return active;}
bool movementOwnsAirborne(){AcquireSRWLockShared(&movementLock);const auto tick=GetTickCount64();const bool active=movementPlayback.bank.hasAir()&&movementPlayback.end>tick&&movementPlayback.lease>tick;ReleaseSRWLockShared(&movementLock);return active;}
void stopMovementBank(){movementLoad.requested=false;AcquireSRWLockExclusive(&movementLock);movementPlayback.end=movementPlayback.lease=0;ReleaseSRWLockExclusive(&movementLock);}
bool movementHelperMatches(const NativeSkeleton& s){
    const unsigned char bytes[]={0x40,0x55,0x53,0x41,0x56,0x41,0x57,0x48,0x8d,0x6c,0x24,0xb8,0x48,0x81,0xec,0x48};
    std::uintptr_t h=0;unsigned count=0,words=0,joints=0;
    return !std::memcmp(gameImage+0x23fca0,bytes,sizeof(bytes))&&memoryRead(s.model+0xa0,&h,8)&&h&&
        memoryRead(s.model+0xa8,&count,4)&&(count&0xffffff)>0&&(count&0xffffff)<=256&&
        memoryRead(s.animation+0x2d0,&words,4)&&words>0&&words<=8&&memoryRead(s.animation+0x2d4,&joints,4)&&joints==s.parents.size()&&(joints==215||joints==219);
}
bool beginMovementBank(bool persistent=false){
    const auto tick=GetTickCount64();
    if(movementLoad.state.load(std::memory_order_acquire)||movementBankActive()||reviewClipActive()||authoringProof.end||!probeDoppelganger||!avatarAiHeld||!probeFollow||!locomotionEnabled||bodyStreamEnabled||
        shadowAction.actor||!networkFrameOwned.load()||!coopGameplayReady(outgoing)||!coopGameplayReady(remote)||
        !lastReceive||tick-lastReceive>=250||!bodyNativeFunctions())return false;
    NativeSkeleton s{};NativeSwordEquipment sword{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(!readNativeSkeleton(memoryRead,base,probeHandle,s)||!s.stable||s.logic!=1||s.actor!=probeActor||s.game!=observedGame||
        !bodyShadowTarget(s)||!movementHelperMatches(s)||
        !readNativeSwordEquipment(memoryRead,base,probeHandle,sword,localRole?NativeSwordScope::DoppelgangerCorvo:NativeSwordScope::DoppelgangerEmily)||!sword.equipped||!sword.started)return false;
    if(std::snprintf(movementLoad.path,sizeof(movementLoad.path),"%s.movement.bin",statusPath)>=sizeof(movementLoad.path))return false;
    MovementPlayback next{};
    next.game=s.game;next.actor=s.actor;next.animation=s.animation;next.skeleton=s.skeleton;next.handle=probeHandle;next.parents=s.parents;
    next.epoch=outgoing.worldEpoch;next.peerEpoch=remote.worldEpoch;next.session=remote.session;next.persistent=persistent;
    movementLoad.next=std::move(next);movementLoad.layout=localRole?0x00db32d37719931full:0x84db84f5d1336b80ull;movementLoad.joints=localRole?215:219;
    movementLoad.requested=true;movementLoad.deadline=tick+5000;movementLoad.state.store(1,std::memory_order_release);
    HANDLE worker=CreateThread(nullptr,0,movementLoadWorker,nullptr,0,nullptr);
    if(!worker){movementLoad.requested=false;movementLoad.state.store(0);return false;}CloseHandle(worker);return true;
}
void serviceMovementBank(ULONGLONG tick){
    const auto state=movementLoad.state.load(std::memory_order_acquire);
    if(state>=2){
        auto& next=movementLoad.next;NativeSkeleton s{};NativeSwordEquipment sword{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
        bool valid=state==2&&movementLoad.requested&&tick<movementLoad.deadline&&next.actor==probeActor&&next.game==observedGame&&next.handle==probeHandle&&
            next.epoch==outgoing.worldEpoch&&next.peerEpoch==remote.worldEpoch&&next.session==remote.session&&probeFollow&&avatarAiHeld&&locomotionEnabled&&!bodyStreamEnabled&&
            !reviewClipActive()&&!authoringProof.end&&!shadowAction.actor&&networkFrameOwned.load()&&coopGameplayReady(outgoing)&&coopGameplayReady(remote)&&lastReceive&&tick-lastReceive<250&&
            readNativeSkeleton(memoryRead,base,probeHandle,s)&&s.stable&&s.logic==1&&s.actor==next.actor&&s.game==next.game&&s.animation==next.animation&&s.skeleton==next.skeleton&&s.parents==next.parents&&movementHelperMatches(s)&&
            readNativeSwordEquipment(memoryRead,base,probeHandle,sword,localRole?NativeSwordScope::DoppelgangerCorvo:NativeSwordScope::DoppelgangerEmily)&&sword.equipped&&sword.started;
        if(valid)for(const auto& clip:next.bank.clips)for(unsigned frame=0;frame<MovementBank::frames;++frame)for(unsigned j=3;j<=26;++j)if(j!=17)
            for(unsigned k=0;k<3;++k)if(std::fabs(clip.values[frame*next.bank.joints+j][4+k]-s.bindLocal[j][4+k])>.0001f)valid=false;
        if(valid){next.end=tick+60000;next.lease=lastReceive+250;AcquireSRWLockExclusive(&movementLock);movementPlayback=std::move(next);ReleaseSRWLockExclusive(&movementLock);
            bodyPublication.animation.store(s.animation,std::memory_order_release);}
        consoleOutput("COOP: movement bank async %s (decode %llu ms)\n",valid?"committed":"discarded",movementLoad.decodeMs.load());
        movementLoad.next={};movementLoad.requested=false;movementLoad.state.store(0,std::memory_order_release);
    }
    AcquireSRWLockExclusive(&movementLock);auto& p=movementPlayback;
    if(p.end){const bool ok=tick<p.end&&p.actor==probeActor&&p.game==observedGame&&p.handle==probeHandle&&p.epoch==outgoing.worldEpoch&&
        p.peerEpoch==remote.worldEpoch&&p.session==remote.session&&probeFollow&&avatarAiHeld&&locomotionEnabled&&!bodyStreamEnabled&&
        networkFrameOwned.load()&&coopGameplayReady(outgoing)&&coopGameplayReady(remote)&&lastReceive&&tick-lastReceive<250;
        if(ok){p.lease=lastReceive+250;if(p.persistent)p.end=tick+60000;}else p.end=p.lease=0;}
    ReleaseSRWLockExclusive(&movementLock);
}
void publishMovementBank(const PresentationPose& pose,const float* position,double now){
    const auto tick=GetTickCount64();AcquireSRWLockExclusive(&movementLock);auto& p=movementPlayback;
    if(p.end&&tick<p.lease){
        // Sword semantics travel through the existing native Steam action
        // stream. Unknown/stale actions never count as an idle permission.
        const bool ground=pose.stance==0&&pose.motion==0;
        const bool crouched=p.bank.hasCrouch()&&pose.stance==1&&pose.motion==1;
        const bool horizontalLean=p.bank.hasLean()&&shadowStream.remoteState==5&&shadowStream.remoteLeanUp<=.0001f;
        const bool sliding=p.bank.hasSlide()&&shadowStream.remoteState==6;
        const bool airState=p.bank.hasAir()&&pose.motionValid&&(pose.motion==2||pose.motion==3)&&
            (shadowStream.remoteState==3||shadowStream.remoteState==0||shadowStream.remoteState==4);
        // Native Sliding uses physics stance 5, outside the legacy body-pose
        // channel. Its exact-class action packet validates presentation while
        // the independent root channel continues to own world placement.
        const bool slideRecovery=p.bank.hasSlide()&&p.clock.slideState&&(shadowStream.remoteState==0||shadowStream.remoteState==4);
        const bool eligible=((pose.motionValid&&(ground||crouched))||sliding||slideRecovery||airState)&&shadowStream.until>tick&&shadowStream.peerReady&&
            shadowStream.lastState&&tick-shadowStream.lastState<250&&(shadowStream.remoteState==0||(p.bank.hasSprint()&&shadowStream.remoteState==4)||horizontalLean||sliding||airState||(p.bank.hasAir()&&shadowStream.remoteState==3))&&!shadowAction.actor;
        const unsigned stance=sliding?1u:slideRecovery&&!pose.motionValid?(p.clock.stanceMix>.5f?1u:0u):pose.stance;
        const float height=shadowStream.jumpTargetZ-shadowStream.jumpStartZ;
        const float rise=shadowStream.jumpTargetValid&&height>=.05f?(pose.position[2]-shadowStream.jumpStartZ)/height:0;
        if(!p.clock.advance(p.bank,now,position,pose.orientation,eligible,stance,shadowStream.remoteState==4,horizontalLean,shadowStream.remoteLeanSide,sliding,airState?pose.motion:0,rise))++p.refused;
        if(!eligible)p.clock.weight=0; // native action has immediate priority
    }ReleaseSRWLockExclusive(&movementLock);
}
void applyMovementBank(void* component,void* actor){
    const auto tick=GetTickCount64();AcquireSRWLockExclusive(&movementLock);auto& p=movementPlayback;
    if(!p.end||tick>=p.end||tick>=p.lease||p.clock.weight<.001f||p.animation!=reinterpret_cast<std::uintptr_t>(component)||p.actor!=reinterpret_cast<std::uintptr_t>(actor)){
        ReleaseSRWLockExclusive(&movementLock);return;}
    NativeSkeleton s{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    bool ok=networkFrameOwned.load()&&readNativeSkeleton(memoryRead,base,p.handle,s)&&s.stable&&s.logic==1&&s.game==p.game&&s.actor==p.actor&&s.animation==p.animation&&s.skeleton==p.skeleton&&s.parents==p.parents&&movementHelperMatches(s);
    const unsigned count=p.bank.joints;std::vector<BodyMatrix> pose(count);
    float moving=(std::max)(0.f,(std::min)(1.f,(p.clock.speed-.025f)/.30f));moving=moving*moving*(3-2*moving);
    for(unsigned j=2;ok&&j<count;++j){
        auto a=p.bank.sample(p.clock.a,j,p.clock.phase),b=p.bank.sample(p.clock.b,j,p.clock.phase);
        auto gait=MovementBank::blend(a,b,p.clock.mix);
        if(p.bank.gaits>1)gait=MovementBank::blend(gait,MovementBank::blend(p.bank.sample(p.clock.a+8,j,p.clock.phase),p.bank.sample(p.clock.b+8,j,p.clock.phase),p.clock.mix),p.clock.runMix);
        if(p.bank.hasSprint())gait=MovementBank::blend(gait,MovementBank::blend(p.bank.sample(26,j,p.clock.phase),p.bank.sample(p.clock.sprintSide,j,p.clock.phase),p.clock.sprintDirection),p.clock.sprintMix);
        auto v=MovementBank::blend(p.bank.sample(0,j,p.clock.idle),gait,moving);
        if(p.bank.hasCrouch()){
            const auto crouchGait=MovementBank::blend(p.bank.sample(p.clock.a+17,j,p.clock.phase),p.bank.sample(p.clock.b+17,j,p.clock.phase),p.clock.mix);
            const auto crouch=MovementBank::blend(p.bank.sample(17,j,p.clock.crouchIdle),crouchGait,moving);
            v=MovementBank::blend(v,crouch,p.clock.stanceMix);
        }
        if(p.bank.hasLean()){
            const unsigned clip=p.clock.leanAmount>0?30:29;const float amount=std::fabs(p.clock.leanAmount);
            const auto lean=MovementBank::blend(p.bank.sampleParameter(clip,j,amount),p.bank.sampleParameter(clip+2,j,amount),p.clock.stanceMix);
            v=MovementBank::blend(v,lean,p.clock.leanWeight);
        }
        if(p.bank.hasSlide()&&p.clock.slideWeight>.001f){
            MovementBank::Joint slide{};
            if(p.clock.slideState==1){
                const float entry=p.bank.clips[33].angle;
                slide=p.clock.slideTime<entry?p.bank.sampleParameter(33,j,p.clock.slideTime/entry):
                    p.bank.sampleParameter(34,j,std::fmod((p.clock.slideTime-entry)/p.bank.clips[34].angle,1.f));
            }else{
                slide=MovementBank::blend(p.bank.sampleParameter(35,j,p.clock.slideTime/p.bank.clips[35].angle),
                    p.bank.sampleParameter(36,j,p.clock.slideTime/p.bank.clips[36].angle),p.clock.stanceMix);
            }
            v=MovementBank::blend(v,slide,p.clock.slideWeight);
        }
        if(p.bank.hasAir()&&p.clock.airWeight>.001f){
            MovementBank::Joint air{};
            if(p.clock.airState==1||p.clock.airState==2){
                const float rise=p.clock.airState==1?p.clock.airProgress:1.f;
                auto standing=MovementBank::blend(p.bank.sampleParameter(37,j,rise),p.bank.sampleParameter(40,j,rise),p.clock.airRun);
                air=MovementBank::blend(standing,p.bank.sampleParameter(43,j,rise),p.clock.airCrouch);
                if(p.clock.airState==2)air=MovementBank::blend(air,p.bank.sampleParameter(38,j,std::fmod(p.clock.airTime/p.bank.clips[38].angle,1.f)),p.clock.fallMix);
            }else{
                auto standing=MovementBank::blend(p.bank.sampleParameter(39,j,p.clock.airTime/p.bank.clips[39].angle),p.bank.sampleParameter(41,j,p.clock.airTime/p.bank.clips[41].angle),p.clock.airRun);
                air=MovementBank::blend(standing,p.bank.sampleParameter(42,j,p.clock.airTime/p.bank.clips[42].angle),p.clock.stanceMix);
            }
            v=MovementBank::blend(v,air,p.clock.airWeight);
        }
        std::array<float,12> q{};std::copy(v.begin(),v.end(),q.begin());q[8]=q[9]=q[10]=1;ok=bodyFromQts(q,pose[j]);
        if(ok&&p.clock.weight<.999f){BodyMatrix native{};std::memcpy(native.v,s.localPose[j].data(),64);if(!bodyRigid(native)){ok=false;break;}
            float x[9]{},y[9]{},r[9]{};for(unsigned c=0;c<3;++c)for(unsigned k=0;k<3;++k){x[c*3+k]=native.v[c*4+k];y[c*3+k]=pose[j].v[c*4+k];}
            presentationRotation(x,y,p.clock.weight,r);for(unsigned c=0;c<3;++c)for(unsigned k=0;k<3;++k)pose[j].v[c*4+k]=r[c*3+k];
            for(unsigned k=12;k<15;++k)pose[j].v[k]=native.v[k]+(pose[j].v[k]-native.v[k])*p.clock.weight;}
    }
    if(ok){for(unsigned j=2;j<count;++j)reinterpret_cast<void(*)(void*,std::uint16_t,const BodyMatrix*)>(gameImage+0x9cfa0)(reinterpret_cast<void*>(p.animation+0x40),static_cast<std::uint16_t>(j),&pose[j]);
        reinterpret_cast<void(*)(void*)>(gameImage+0x9dc40)(reinterpret_cast<void*>(p.animation+0x40));
        reinterpret_cast<void(*)(void*,void*,void*)>(gameImage+0x23fca0)(reinterpret_cast<void*>(s.model),reinterpret_cast<void*>(p.animation+0x40),reinterpret_cast<void*>(p.animation+0x2b0));++p.applied;
    }else{++p.refused;p.end=p.lease=0;}
    ReleaseSRWLockExclusive(&movementLock);
}
void writeMovementStatus(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.movement.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;
    AcquireSRWLockShared(&movementLock);const auto& p=movementPlayback;
    fprintf(f,"{\"experimental\":true,\"active\":%s,\"end\":%llu,\"joints\":%u,\"phase\":%.7f,\"speed\":%.6f,\"weight\":%.6f,\"a\":%u,\"b\":%u,\"mix\":%.6f,\"checksum\":%llu,\"applied\":%llu,\"refused\":%llu,\"gaits\":%u,\"runMix\":%.6f,\"version\":%u,\"stanceMix\":%.6f,\"sprintMix\":%.6f,\"sprintSide\":%u,\"sprintDirection\":%.6f,\"leanAmount\":%.6f,\"leanWeight\":%.6f,\"slideState\":%u,\"slideTime\":%.6f,\"slideWeight\":%.6f,\"airState\":%u,\"airTime\":%.6f,\"airWeight\":%.6f,\"airProgress\":%.6f,\"airRun\":%.6f,\"airCrouch\":%.6f}\n",p.end?"true":"false",p.end,p.bank.joints,p.clock.phase,p.clock.speed,p.clock.weight,p.clock.a,p.clock.b,p.clock.mix,p.bank.checksum,p.applied,p.refused,p.bank.gaits,p.clock.runMix,p.bank.version,p.clock.stanceMix,p.clock.sprintMix,p.clock.sprintSide,p.clock.sprintDirection,p.clock.leanAmount,p.clock.leanWeight,p.clock.slideState,p.clock.slideTime,p.clock.slideWeight,p.clock.airState,p.clock.airTime,p.clock.airWeight,p.clock.airProgress,p.clock.airRun,p.clock.airCrouch);
    ReleaseSRWLockShared(&movementLock);const bool ok=!ferror(f);const int close=fclose(f);if(ok&&!close)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
