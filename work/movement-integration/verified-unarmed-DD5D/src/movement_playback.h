#pragma once
#include "movement_bank.h"
#include "movement_equipment_contact.h"
void requestMovementEquipment(bool drawn);
SRWLOCK movementLock=SRWLOCK_INIT;
struct MovementPlayback {
    MovementBank bank;MovementClock clock;
    std::uintptr_t game=0,actor=0,animation=0,skeleton=0;unsigned handle=0;
    std::uint64_t epoch=0,peerEpoch=0,session=0;ULONGLONG end=0,lease=0;
    std::vector<std::int16_t> parents;std::uint64_t applied=0,refused=0;bool persistent=false;
    std::uint64_t combatEvent=0;unsigned combatVariant=0,combatPhase=0,combatClip=0;
    double combatLastTime=0;float combatAmount=0,combatWeight=0,combatTransition=0;
    std::vector<MovementBank::Joint> combatFrom,combatLast;
    unsigned equipmentVariant=0,equipmentClip=0;float equipmentAmount=0,equipmentWeight=0,armedMix=1;bool presentedDrawn=true;
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
bool movementOwnsCombat(){AcquireSRWLockShared(&movementLock);const auto tick=GetTickCount64();const bool active=movementPlayback.bank.hasCombat()&&movementPlayback.end>tick&&movementPlayback.lease>tick;ReleaseSRWLockShared(&movementLock);return active;}
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
    const auto tick=GetTickCount64();bool equipmentRequest=false,equipmentDrawn=true;AcquireSRWLockExclusive(&movementLock);auto& p=movementPlayback;
    if(p.end&&tick<p.lease){
        // Sword semantics travel through the existing native Steam action
        // stream. Unknown/stale actions never count as an idle permission.
        const bool ground=pose.stance==0&&pose.motion==0;
        const bool crouched=p.bank.hasCrouch()&&pose.stance==1&&pose.motion==1;
        CombatPresentation::Sample combat{};float amount=0;
        const bool combatValid=p.bank.hasCombat()&&shadowStream.combat.sample(pose.time,combat,amount);
        const bool combatActive=combatValid&&combat.variant&&pose.motionValid&&(ground||crouched);
        const float combatDt=p.combatLastTime?float((now-p.combatLastTime)*.001):0;p.combatLastTime=now;
        if(combatDt>=0&&combatDt<.25f){
            if(combatActive){
                if(combat.event!=p.combatEvent){p.combatFrom=p.combatLast;p.combatTransition=0;p.combatEvent=combat.event;}
                p.combatVariant=combat.variant;p.combatPhase=combat.phase;p.combatAmount=amount;
                const unsigned v=combat.variant;
                p.combatClip=v<=4?44+v-1:v<=6?48+v-5:v==7?(combat.phase==2?51:50):v<=9?52:v==10||v==13?53:v==11?54:55;
                p.combatTransition=(std::min)(1.f,p.combatTransition+combatDt/.09f);
            }
            p.combatWeight+=((combatActive?1.f:0.f)-p.combatWeight)*(1-std::exp(-combatDt/(combatActive?.06f:.16f)));
        }else p.combatWeight=0;
        EquipmentPresentation::Sample equipment{};float equipmentAmount=0;
        const bool equipmentValid=p.bank.hasEquipment()&&shadowStream.equipment.sample(pose.time,equipment,equipmentAmount);
        const bool equipmentActive=equipmentValid&&equipment.variant&&pose.motionValid&&(ground||crouched);
        if(combatDt>=0&&combatDt<.25f){
            if(equipmentActive){p.equipmentVariant=equipment.variant;p.equipmentClip=84+equipment.variant;p.equipmentAmount=equipmentAmount;}
            p.equipmentWeight+=((equipmentActive?1.f:0.f)-p.equipmentWeight)*(1-std::exp(-combatDt/(equipmentActive?.035f:.12f)));
            if(equipmentValid){
                equipmentDrawn=equipment.sword==2;
                if(equipmentActive){const float contact=movementEquipmentContact[localRole?0:1][equipment.variant-1];
                    equipmentDrawn=equipment.variant==1||equipment.variant==3?equipmentAmount>=contact:equipmentAmount<contact;}
                p.presentedDrawn=equipmentDrawn;
                p.armedMix+=((equipmentDrawn?1.f:0.f)-p.armedMix)*(1-std::exp(-combatDt/.15f));
            }
        }else p.equipmentWeight=0;
        const bool horizontalLean=p.bank.hasLean()&&shadowStream.remoteState==5&&shadowStream.remoteLeanUp<=.0001f;
        const bool sliding=p.bank.hasSlide()&&shadowStream.remoteState==6;
        const bool airState=p.bank.hasAir()&&pose.motionValid&&(pose.motion==2||pose.motion==3)&&
            (shadowStream.remoteState==3||shadowStream.remoteState==0||shadowStream.remoteState==4);
        // Native Sliding uses physics stance 5, outside the legacy body-pose
        // channel. Its exact-class action packet validates presentation while
        // the independent root channel continues to own world placement.
        const bool slideRecovery=p.bank.hasSlide()&&p.clock.slideState&&(shadowStream.remoteState==0||shadowStream.remoteState==4);
        const bool eligible=((pose.motionValid&&(ground||crouched))||sliding||slideRecovery||airState)&&shadowStream.until>tick&&shadowStream.peerReady&&
            shadowStream.lastState&&tick-shadowStream.lastState<250&&(shadowStream.remoteState==0||(p.bank.hasSprint()&&shadowStream.remoteState==4)||horizontalLean||sliding||airState||(p.bank.hasAir()&&shadowStream.remoteState==3)||
                (p.bank.hasCombat()&&(shadowStream.remoteState==1||shadowStream.remoteState==2))||combatActive||p.combatWeight>.001f||(p.bank.hasEquipment()&&shadowStream.remoteState==7))&&!shadowAction.actor;
        const unsigned stance=sliding?1u:slideRecovery&&!pose.motionValid?(p.clock.stanceMix>.5f?1u:0u):pose.stance;
        const float height=shadowStream.jumpTargetZ-shadowStream.jumpStartZ;
        const float rise=shadowStream.jumpTargetValid&&height>=.05f?(pose.position[2]-shadowStream.jumpStartZ)/height:0;
        if(!p.clock.advance(p.bank,now,position,pose.orientation,eligible,stance,shadowStream.remoteState==4,horizontalLean,shadowStream.remoteLeanSide,sliding,airState?pose.motion:0,rise,p.armedMix))++p.refused;
        equipmentRequest=eligible&&equipmentValid;
        if(!eligible)p.clock.weight=0; // native action has immediate priority
    }ReleaseSRWLockExclusive(&movementLock);
    if(equipmentRequest)requestMovementEquipment(equipmentDrawn);
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
        const auto special=[&](unsigned clip,float amount){return p.bank.sampleWeaponParameter(clip,j,amount,p.armedMix);};
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
        if(p.bank.hasEquipment()){
            auto emptyGait=MovementBank::blend(p.bank.sample(p.clock.a+59,j,p.clock.phase),p.bank.sample(p.clock.b+59,j,p.clock.phase),p.clock.mix);
            const auto emptyRun=MovementBank::blend(p.bank.sample(p.clock.a+67,j,p.clock.phase),p.bank.sample(p.clock.b+67,j,p.clock.phase),p.clock.mix);
            emptyGait=MovementBank::blend(emptyGait,emptyRun,p.clock.runMix);
            if(p.bank.hasUnarmedSpecial())emptyGait=MovementBank::blend(emptyGait,MovementBank::blend(p.bank.sample(89,j,p.clock.phase),p.bank.sample(p.clock.sprintSide+63,j,p.clock.phase),p.clock.sprintDirection),p.clock.sprintMix);
            auto empty=MovementBank::blend(p.bank.sample(59,j,p.clock.idle),emptyGait,moving);
            const auto lowGait=MovementBank::blend(p.bank.sample(p.clock.a+76,j,p.clock.phase),p.bank.sample(p.clock.b+76,j,p.clock.phase),p.clock.mix);
            const auto low=MovementBank::blend(p.bank.sample(76,j,p.clock.crouchIdle),lowGait,moving);
            empty=MovementBank::blend(empty,low,p.clock.stanceMix);v=MovementBank::blend(empty,v,p.armedMix);
        }
        if(p.bank.hasLean()){
            const unsigned clip=p.clock.leanAmount>0?30:29;const float amount=std::fabs(p.clock.leanAmount);
            const auto lean=MovementBank::blend(special(clip,amount),special(clip+2,amount),p.clock.stanceMix);
            v=MovementBank::blend(v,lean,p.clock.leanWeight);
        }
        if(p.bank.hasSlide()&&p.clock.slideWeight>.001f){
            MovementBank::Joint slide{};
            if(p.clock.slideState==1){
                const float entry=p.bank.clips[33].angle;
                slide=p.clock.slideTime<entry?special(33,p.clock.slideTime/entry):
                    special(34,std::fmod((p.clock.slideTime-entry)/p.bank.clips[34].angle,1.f));
            }else{
                slide=MovementBank::blend(special(35,p.clock.slideTime/p.bank.clips[35].angle),
                    special(36,p.clock.slideTime/p.bank.clips[36].angle),p.clock.stanceMix);
            }
            v=MovementBank::blend(v,slide,p.clock.slideWeight);
        }
        if(p.bank.hasAir()&&p.clock.airWeight>.001f){
            MovementBank::Joint air{};
            if(p.clock.airState==1||p.clock.airState==2){
                const float rise=p.clock.airState==1?p.clock.airProgress:1.f;
                auto standing=MovementBank::blend(special(37,rise),special(40,rise),p.clock.airRun);
                air=MovementBank::blend(standing,special(43,rise),p.clock.airCrouch);
                if(p.clock.airState==2)air=MovementBank::blend(air,special(38,std::fmod(p.clock.airTime/p.bank.clips[38].angle,1.f)),p.clock.fallMix);
            }else{
                auto standing=MovementBank::blend(special(39,p.clock.airTime/p.bank.clips[39].angle),special(41,p.clock.airTime/p.bank.clips[41].angle),p.clock.airRun);
                air=MovementBank::blend(standing,special(42,p.clock.airTime/p.bank.clips[42].angle),p.clock.stanceMix);
            }
            v=MovementBank::blend(v,air,p.clock.airWeight);
        }
        if(p.bank.hasCombat()&&p.combatWeight>.001f&&p.combatClip>=44){
            auto action=p.bank.sampleParameter(p.combatClip,j,p.combatAmount);
            if(p.combatClip>=53&&p.combatClip<=55)action=MovementBank::blend(action,p.bank.sampleParameter(p.combatClip+3,j,p.combatAmount),p.clock.stanceMix);
            if(p.combatFrom.size()==count)action=MovementBank::blend(p.combatFrom[j],action,p.combatTransition*p.combatTransition*(3-2*p.combatTransition));
            if(p.combatLast.size()!=count)p.combatLast.resize(count);p.combatLast[j]=action;
            v=MovementBank::blend(v,action,p.combatWeight);
        }
        if(p.bank.hasEquipment()&&p.equipmentWeight>.001f&&p.equipmentClip>=85){
            const auto equipment=p.bank.sampleParameter(p.equipmentClip,j,p.equipmentAmount);v=MovementBank::blend(v,equipment,p.equipmentWeight);
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
    fprintf(f,"{\"experimental\":true,\"active\":%s,\"end\":%llu,\"joints\":%u,\"phase\":%.7f,\"speed\":%.6f,\"weight\":%.6f,\"a\":%u,\"b\":%u,\"mix\":%.6f,\"checksum\":%llu,\"applied\":%llu,\"refused\":%llu,\"gaits\":%u,\"runMix\":%.6f,\"version\":%u,\"stanceMix\":%.6f,\"sprintMix\":%.6f,\"sprintSide\":%u,\"sprintDirection\":%.6f,\"leanAmount\":%.6f,\"leanWeight\":%.6f,\"slideState\":%u,\"slideTime\":%.6f,\"slideWeight\":%.6f,\"airState\":%u,\"airTime\":%.6f,\"airWeight\":%.6f,\"airProgress\":%.6f,\"airRun\":%.6f,\"airCrouch\":%.6f,\"combatVariant\":%u,\"combatPhase\":%u,\"combatClip\":%u,\"combatAmount\":%.6f,\"combatWeight\":%.6f,\"combatEvent\":%llu,\"equipmentVariant\":%u,\"equipmentAmount\":%.6f,\"equipmentWeight\":%.6f,\"armedMix\":%.6f,\"presentedDrawn\":%s}\n",p.end?"true":"false",p.end,p.bank.joints,p.clock.phase,p.clock.speed,p.clock.weight,p.clock.a,p.clock.b,p.clock.mix,p.bank.checksum,p.applied,p.refused,p.bank.gaits,p.clock.runMix,p.bank.version,p.clock.stanceMix,p.clock.sprintMix,p.clock.sprintSide,p.clock.sprintDirection,p.clock.leanAmount,p.clock.leanWeight,p.clock.slideState,p.clock.slideTime,p.clock.slideWeight,p.clock.airState,p.clock.airTime,p.clock.airWeight,p.clock.airProgress,p.clock.airRun,p.clock.airCrouch,p.combatVariant,p.combatPhase,p.combatClip,p.combatAmount,p.combatWeight,p.combatEvent,p.equipmentVariant,p.equipmentAmount,p.equipmentWeight,p.armedMix,p.presentedDrawn?"true":"false");
    ReleaseSRWLockShared(&movementLock);const bool ok=!ferror(f);const int close=fclose(f);if(ok&&!close)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
