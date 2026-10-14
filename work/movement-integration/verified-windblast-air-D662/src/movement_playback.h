#pragma once
#include "movement_bank.h"
#include "movement_second_jump.h"
#include "movement_knee_pole.h"
#include "movement_equipment_contact.h"
#include "movement_vault_lift.h"
void requestMovementEquipment(bool drawn);
SRWLOCK movementLock=SRWLOCK_INIT;
bool movementNativeFeetRequested=false; // isolated A/B experiment; frozen per bank incarnation
bool movementCurrentFeetRequested=false;
extern std::atomic<std::uint64_t> movementFeetCasts,movementFeetSolves,movementFeetResetsAvoided,movementFeetReferences;
struct MovementPlayback {
    MovementBank bank;MovementClock clock;MovementSecondJump secondJump;
    std::vector<MovementBank::Joint> airLast,secondFrom;
    std::uintptr_t game=0,actor=0,animation=0,skeleton=0;unsigned handle=0;
    std::uint64_t epoch=0,peerEpoch=0,session=0;ULONGLONG end=0,lease=0;
    std::vector<std::int16_t> parents;std::uint64_t applied=0,refused=0;bool persistent=false,nativeFeet=false,currentFeet=false;
    std::uint64_t combatEvent=0;unsigned combatVariant=0,combatPhase=0,combatClip=0;
    double combatLastTime=0;float combatAmount=0,combatWeight=0,combatTransition=0,combatAirMix=0,combatMoveMix=0;
    std::vector<MovementBank::Joint> combatFrom,combatLast;
    unsigned equipmentVariant=0,equipmentClip=0;float equipmentAmount=0,equipmentWeight=0,armedMix=1;bool presentedDrawn=true;
    unsigned traversalVariant=0;float traversalAmount=0,traversalWeight=0,traversalHeight=0,landingCrouchMix=0;std::uint64_t traversalEvent=0;
    unsigned powerVariant=0;float powerAmount=0,powerWeight=0,powerTransition=0,powerMoveMix=0;std::uint64_t powerEvent=0;
    std::vector<MovementBank::Joint> powerFrom,powerLast;
    unsigned turnSerial=0;std::vector<MovementBank::Joint> turnFrom,turnLast;
    std::array<BodyMatrix,27> kneeReference{};std::uint64_t kneeSequence=0,kneeCorrections=0;bool kneeReady=false;unsigned kneeReject=0;float kneeOffsetError=0;
} movementPlayback;
// File validation grows with the authored library. Keep it entirely off the
// game thread; only the final incarnation/skeleton checks run on that thread.
struct MovementLoad {
    std::atomic<unsigned> state{0}; // 0 idle, 1 worker, 2 decoded, 3 failed
    std::atomic<ULONGLONG> decodeMs{0};
    MovementPlayback next;char path[MAX_PATH]{};unsigned joints=0;
    std::uint64_t layout=0,expectedChecksum=0;ULONGLONG deadline=0;bool requested=false;
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
bool beginMovementBank(bool persistent=false,std::uint64_t expectedChecksum=0){
    const auto tick=GetTickCount64();
    if(movementLoad.state.load(std::memory_order_acquire)||movementBankActive()||reviewClipActive()||authoringProof.end||!probeDoppelganger||!avatarAiHeld||!probeFollow||!locomotionEnabled||bodyStreamEnabled||
        shadowAction.actor||!networkFrameOwned.load()||!coopGameplayReady(outgoing)||!coopGameplayReady(remote)||
        !lastReceive||tick-lastReceive>=250||!bodyNativeFunctions())return false;
    NativeSkeleton s{};NativeSwordEquipment sword{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(!readNativeSkeleton(memoryRead,base,probeHandle,s)||!s.stable||s.logic!=1||s.actor!=probeActor||s.game!=observedGame||
        !bodyShadowTarget(s)||!movementHelperMatches(s)||
        !readNativeSwordEquipment(memoryRead,base,probeHandle,sword,localRole?NativeSwordScope::DoppelgangerCorvo:NativeSwordScope::DoppelgangerEmily)||!sword.equipped||!sword.started)return false;
    if(std::snprintf(movementLoad.path,sizeof(movementLoad.path),"%s.movement.bin",statusPath)>=sizeof(movementLoad.path))return false;
    MovementPlayback next{};next.nativeFeet=movementNativeFeetRequested;next.currentFeet=movementCurrentFeetRequested;
    next.game=s.game;next.actor=s.actor;next.animation=s.animation;next.skeleton=s.skeleton;next.handle=probeHandle;next.parents=s.parents;
    next.epoch=outgoing.worldEpoch;next.peerEpoch=remote.worldEpoch;next.session=remote.session;next.persistent=persistent;
    movementLoad.next=std::move(next);movementLoad.layout=localRole?0x00db32d37719931full:0x84db84f5d1336b80ull;movementLoad.joints=localRole?215:219;
    movementLoad.expectedChecksum=expectedChecksum;movementLoad.requested=true;movementLoad.deadline=tick+5000;movementLoad.state.store(1,std::memory_order_release);
    HANDLE worker=CreateThread(nullptr,0,movementLoadWorker,nullptr,0,nullptr);
    if(!worker){movementLoad.requested=false;movementLoad.state.store(0);return false;}CloseHandle(worker);return true;
}
void serviceMovementBank(ULONGLONG tick){
    const auto state=movementLoad.state.load(std::memory_order_acquire);
    if(state>=2){
        auto& next=movementLoad.next;NativeSkeleton s{};NativeSwordEquipment sword{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
        bool valid=state==2&&(!movementLoad.expectedChecksum||next.bank.checksum==movementLoad.expectedChecksum)&&movementLoad.requested&&tick<movementLoad.deadline&&next.actor==probeActor&&next.game==observedGame&&next.handle==probeHandle&&
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
        MovementSemantics::Sample semantic{};const bool semanticValid=shadowStream.movement.sample(pose.time,semantic);
        // Sword semantics travel through the existing native Steam action
        // stream. Unknown/stale actions never count as an idle permission.
        const bool ground=pose.stance==0&&pose.motion==0;
        const bool crouched=p.bank.hasCrouch()&&pose.stance==1&&pose.motion==1;
        CombatPresentation::Sample combat{};float amount=0;
        const bool combatValid=p.bank.hasCombat()&&shadowStream.combat.sample(pose.time,combat,amount);
        const bool combatAir=p.bank.hasAirCombat()&&pose.motionValid&&(pose.motion==2||pose.motion==3);
        const bool combatActive=combatValid&&combat.variant&&pose.motionValid&&(ground||crouched||combatAir);
        const float combatDt=p.combatLastTime?float((now-p.combatLastTime)*.001):0;p.combatLastTime=now;
        PowerPresentation::Sample power{};float powerAmount=0;
        const bool powerValid=p.bank.hasPowers()&&shadowStream.power.sample(pose.time,power,powerAmount);
        const bool powerActive=powerValid&&power.variant&&(power.variant<=9||
            (power.variant==10&&p.bank.hasDarkVision()&&(p.bank.hasDarkVisionContexts()||(ground&&p.presentedDrawn)))||
            (power.variant==11&&p.bank.hasWindblast()&&((p.bank.hasWindblastContexts()&&(ground||crouched||combatAir))||(ground&&p.presentedDrawn))));
        if(combatDt>=0&&combatDt<.25f){
            if(powerActive){
                if(power.event!=p.powerEvent){p.powerFrom=p.powerLast;p.powerTransition=0;p.powerEvent=power.event;}
                p.powerVariant=power.variant;p.powerAmount=powerAmount;p.powerTransition=(std::min)(1.f,p.powerTransition+combatDt/.06f);
            }
            p.powerWeight+=((powerActive?1.f:0.f)-p.powerWeight)*(1-std::exp(-combatDt/(powerActive?.035f:.12f)));
            float moving=(std::max)(0.f,(std::min)(1.f,(p.clock.speed-.1f)/.5f));moving=moving*moving*(3-2*moving);
            p.powerMoveMix+=(moving-p.powerMoveMix)*(1-std::exp(-combatDt/.07f));
        }else p.powerWeight=p.powerMoveMix=0;
        TraversalPresentation::Sample traversal{};float traversalAmount=0;
        const bool traversalValid=p.bank.hasLowMantle()&&shadowStream.traversal.sample(pose.time,traversal,traversalAmount);
        const bool traversalActive=traversalValid&&(traversal.variant==1||(traversal.variant==2&&p.bank.hasCrouchMantle())||(traversal.variant==3&&p.bank.hasVault())||((traversal.variant==4||traversal.variant==5)&&p.bank.hasHighMantle())||(traversal.variant==6&&p.bank.hasHardLanding())||(traversal.variant>=7&&p.bank.hasKeyhole()));
        if(combatDt>=0&&combatDt<.25f){
            if(traversalActive){
                if(traversal.event!=p.traversalEvent)p.landingCrouchMix=0;
                if(traversal.variant==6&&p.bank.hasCrouchLanding())p.landingCrouchMix+=(float(traversal.endStance)-p.landingCrouchMix)*(1-std::exp(-combatDt/.06f));
                p.traversalVariant=traversal.variant;p.traversalAmount=traversalAmount;p.traversalEvent=traversal.event;p.traversalHeight=traversal.height;
            }
            p.traversalWeight+=((traversalActive?1.f:0.f)-p.traversalWeight)*(1-std::exp(-combatDt/(traversalActive?.025f:.12f)));
        }else p.traversalWeight=0;
        if(combatDt>=0&&combatDt<.25f){
            if(combatActive){
                if(combat.event!=p.combatEvent){
                    // A landing can advance the semantic event while the same
                    // native sword scene continues. Do not restart its blend.
                    if(combat.variant!=p.combatVariant||combat.phase!=p.combatPhase||(combat.phase!=2&&amount+.005f<p.combatAmount)){
                        p.combatFrom=p.combatLast;p.combatTransition=0;
                    }
                    p.combatEvent=combat.event;
                }
                p.combatVariant=combat.variant;p.combatPhase=combat.phase;p.combatAmount=amount;
                const unsigned v=combat.variant;
                p.combatClip=v<=4?44+v-1:v<=6?48+v-5:v==7?(combat.phase==2?51:50):v<=9?52:v==10||v==13?53:v==11?54:55;
                p.combatTransition=(std::min)(1.f,p.combatTransition+combatDt/.09f);
            }
            p.combatAirMix+=((combatAir?1.f:0.f)-p.combatAirMix)*(1-std::exp(-combatDt/(combatAir?.045f:.10f)));
            float travelling=p.bank.hasAirCombat()?(std::max)(0.f,(std::min)(1.f,(p.clock.speed-.1f)/.5f)):0;
            travelling=travelling*travelling*(3-2*travelling);
            p.combatMoveMix+=(travelling-p.combatMoveMix)*(1-std::exp(-combatDt/.07f));
            p.combatWeight+=((combatActive?1.f:0.f)-p.combatWeight)*(1-std::exp(-combatDt/(combatActive?.06f:.16f)));
        }else p.combatWeight=p.combatAirMix=p.combatMoveMix=0;
        EquipmentPresentation::Sample equipment{};float equipmentAmount=0;
        const bool equipmentValid=p.bank.hasEquipment()&&shadowStream.equipment.sample(pose.time,equipment,equipmentAmount);
        // Fast arm recovery after a climb does not reach toward the hip. Keep
        // the full-body traversal recovery and track actual native equipment.
        const bool equipmentActive=equipmentValid&&equipment.variant>=1&&equipment.variant<=4&&pose.motionValid&&(ground||crouched);
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
        const bool horizontalLean=p.bank.hasLean()&&semantic.state==5&&(p.bank.hasVerticalPeek()||semantic.leanUp<=.0001f);
        const bool sliding=p.bank.hasSlide()&&semantic.state==6;
        // The confirmed root is deliberately delayed. A newer grounded
        // combat packet must not cancel its last airborne presentation frames.
        const bool airState=p.bank.hasAir()&&!traversalActive&&(!powerActive||power.variant==10)&&pose.motionValid&&(pose.motion==2||pose.motion==3);
        // Native Sliding uses physics stance 5, outside the legacy body-pose
        // channel. Its exact-class action packet validates presentation while
        // the independent root channel continues to own world placement.
        const bool slideRecovery=p.bank.hasSlide()&&p.clock.slideState&&(semantic.state==0||semantic.state==4);
        const bool eligible=semanticValid&&((pose.motionValid&&(ground||crouched))||sliding||slideRecovery||airState||traversalActive||powerActive)&&shadowStream.until>tick&&shadowStream.peerReady&&
            shadowStream.lastState&&tick-shadowStream.lastState<250&&(semantic.state==0||(p.bank.hasSprint()&&semantic.state==4)||horizontalLean||sliding||airState||(p.bank.hasAir()&&semantic.state==3)||
                (p.bank.hasCombat()&&(semantic.state==1||semantic.state==2))||combatActive||p.combatWeight>.001f||(p.bank.hasEquipment()&&semantic.state==7)||traversalActive||(p.bank.hasLowMantle()&&semantic.state==8)||(p.bank.hasVault()&&semantic.state==9)||(p.bank.hasHardLanding()&&semantic.state==11)||powerActive||(p.bank.hasPowers()&&semantic.state==10))&&!shadowAction.actor;
        if(p.secondJump.advance(pose.time,semantic,pose.position[2],pose.motion,p.bank.hasDoubleJump()&&eligible&&airState))p.secondFrom=p.airLast;
        const bool transportPower=powerActive&&power.variant<=9;
        const unsigned stance=transportPower?0u:traversalActive?traversal.endStance:sliding?1u:slideRecovery&&!pose.motionValid?(p.clock.stanceMix>.5f?1u:0u):pose.stance;
        const float height=semantic.targetZ-semantic.startZ;
        const float rise=semantic.airStage==1&&height>=.05f?(pose.position[2]-semantic.startZ)/height:p.clock.airProgress;
        // Confirmed native Blink arrival is a network presentation warp, not
        // a footstep or an implausibly fast walking segment.
        if(pose.teleport&&powerActive)std::memcpy(p.clock.position,position,12);
        if(!p.clock.advance(p.bank,now,position,pose.orientation,eligible,stance,semantic.state==4,horizontalLean,semantic.leanSide,sliding,airState?pose.motion:0,rise,p.armedMix,semantic.leanUp,traversalActive,transportPower,p.combatWeight<.001f&&p.equipmentWeight<.001f&&!powerActive))++p.refused;
        equipmentRequest=eligible&&equipmentValid;
        if(!eligible)p.clock.weight=0; // native action has immediate priority
    }ReleaseSRWLockExclusive(&movementLock);
    if(equipmentRequest)requestMovementEquipment(equipmentDrawn);
}
bool movementFootGrounded(const MovementPlayback& p){return p.clock.airWeight<.001f&&p.clock.slideWeight<.001f&&p.traversalWeight<.001f&&p.powerWeight<.001f;}
void applyMovementBank(void* component,void* actor,int phase){
    const auto tick=GetTickCount64();AcquireSRWLockExclusive(&movementLock);auto& p=movementPlayback;
    // The native callback visits every animated actor. Unrelated actors must
    // not consume this avatar's phase0 reference before its own phase3.
    if(p.animation!=reinterpret_cast<std::uintptr_t>(component)||p.actor!=reinterpret_cast<std::uintptr_t>(actor)){
        ReleaseSRWLockExclusive(&movementLock);return;
    }
    // One phase0 reference belongs to exactly one native publication. A later
    // frame or cancelled lease cannot consume a stale knee plane.
    const bool restoreKnees=phase==3&&p.kneeReady&&p.kneeSequence==bodyPublication.sequence.load()&&
        p.nativeFeet&&p.currentFeet&&movementFootGrounded(p);
    if(phase==0||phase==3)p.kneeReady=false;
    if(restoreKnees){
        p.kneeReject=0;p.kneeOffsetError=0;NativeSkeleton s{};
        bool ok=p.end>tick&&p.lease>tick&&p.clock.weight>.99f&&networkFrameOwned.load()&&
            p.animation==reinterpret_cast<std::uintptr_t>(component)&&p.actor==reinterpret_cast<std::uintptr_t>(actor);
        if(!ok)p.kneeReject=1;
        if(ok){ok=readNativeSkeleton(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),p.handle,s);if(!ok)p.kneeReject=2;}
        if(ok){ok=s.stable&&s.logic==1&&s.game==p.game&&s.actor==p.actor&&s.animation==p.animation&&s.skeleton==p.skeleton&&s.parents==p.parents&&movementHelperMatches(s);if(!ok)p.kneeReject=3;}
        BodyMatrix corrected[2][3]{};float angles[2]{};
        for(unsigned side=0;ok&&side<2;++side){const unsigned hip=19+side*4;BodyMatrix parent{},solved[3]{};
            ok=s.parents[hip]==18&&s.parents[hip+1]==hip&&s.parents[hip+2]==hip+1;if(!ok)p.kneeReject=4+side*10;
            std::memcpy(parent.v,s.globalPose[18].data(),64);
            for(unsigned j=0;j<3;++j)std::memcpy(solved[j].v,s.globalPose[hip+j].data(),64);
            if(ok){ok=movementKneePole(parent,p.kneeReference.data()+hip,solved,corrected[side],angles[side]);if(!ok)p.kneeReject=5+side*10;}
            // Rotations change; exact native local bone offsets must not.
            for(unsigned j=0;ok&&j<3;++j)for(unsigned k=12;k<15;++k){
                p.kneeOffsetError=(std::max)(p.kneeOffsetError,std::fabs(corrected[side][j].v[k]-s.localPose[hip+j][k]));
                if(p.kneeOffsetError>.0001f){ok=false;p.kneeReject=6+side*10;}
                corrected[side][j].v[k]=s.localPose[hip+j][k];
            }
        }
        if(ok){
            for(unsigned side=0;side<2;++side)for(unsigned j=0;j<3;++j)
                reinterpret_cast<void(*)(void*,std::uint16_t,const BodyMatrix*)>(gameImage+0x9cfa0)(reinterpret_cast<void*>(p.animation+0x40),static_cast<std::uint16_t>(19+side*4+j),&corrected[side][j]);
            reinterpret_cast<void(*)(void*)>(gameImage+0x9dc40)(reinterpret_cast<void*>(p.animation+0x40));
            reinterpret_cast<void(*)(void*,void*,void*)>(gameImage+0x23fca0)(reinterpret_cast<void*>(s.model),reinterpret_cast<void*>(p.animation+0x40),reinterpret_cast<void*>(p.animation+0x2b0));++p.kneeCorrections;
        }else ++p.refused;
        ReleaseSRWLockExclusive(&movementLock);return;
    }
    // Native54f500 publishes phase0 after graph evaluation. The foot/IK
    // stages follow, then phase3 publishes corrected render/attachment data.
    // Default remains the archived phase3 path until the A/B test passes.
    if(phase!=((p.nativeFeet&&movementFootGrounded(p))?0:3)){ReleaseSRWLockExclusive(&movementLock);return;}
    if(!p.end||tick>=p.end||tick>=p.lease||p.clock.weight<.001f||p.animation!=reinterpret_cast<std::uintptr_t>(component)||p.actor!=reinterpret_cast<std::uintptr_t>(actor)){
        ReleaseSRWLockExclusive(&movementLock);return;}
    NativeSkeleton s{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    bool ok=networkFrameOwned.load()&&readNativeSkeleton(memoryRead,base,p.handle,s)&&s.stable&&s.logic==1&&s.game==p.game&&s.actor==p.actor&&s.animation==p.animation&&s.skeleton==p.skeleton&&s.parents==p.parents&&movementHelperMatches(s);
    const unsigned count=p.bank.joints;std::vector<BodyMatrix> pose(count);
    if(p.bank.hasTurns()&&p.turnSerial!=p.clock.turnSerial){p.turnSerial=p.clock.turnSerial;p.turnFrom=p.turnLast;}
    if(p.bank.hasTurns()&&p.turnLast.size()!=count)p.turnLast.resize(count);
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
        if(p.bank.hasTerrain())v=MovementBank::blend(v,p.bank.sampleTerrain(j,p.clock.phase,p.clock.terrainDown,p.clock.terrainRun,p.armedMix),p.clock.terrainWeight*moving);
        if(p.bank.hasTurns()){
            auto turn=p.bank.sampleTurn(p.clock.turnClip,j,p.clock.turnPhase,p.armedMix,p.clock.stanceMix);
            if(p.turnFrom.size()==count&&p.clock.turnTransition<1)turn=MovementBank::blend(p.turnFrom[j],turn,p.clock.turnTransition);
            p.turnLast[j]=turn;v=MovementBank::blend(v,turn,p.clock.turnWeight);
        }
        if(p.bank.hasLean()){
            const unsigned clip=p.clock.leanAmount>0?30:29;const float amount=std::fabs(p.clock.leanAmount);
            auto lean=MovementBank::blend(special(clip,amount),special(clip+2,amount),p.clock.stanceMix);
            if(p.bank.hasVerticalPeek()&&p.clock.leanUp>.00001f){
                auto up=MovementBank::blend(p.bank.sampleParameter(107,j,p.clock.leanUp),p.bank.sampleParameter(108,j,p.clock.leanUp),p.clock.stanceMix);
                const auto empty=MovementBank::blend(p.bank.sampleParameter(109,j,p.clock.leanUp),p.bank.sampleParameter(110,j,p.clock.leanUp),p.clock.stanceMix);
                up=MovementBank::blend(empty,up,p.armedMix);
                // Both native axes can coexist during direction changes.
                // Blend complete balance poses instead of adding two maximum
                // offsets and overextending the planted legs.
                lean=MovementBank::blend(lean,up,p.clock.leanUp/(p.clock.leanUp+amount));
            }
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
                if(p.bank.hasDoubleJump()&&p.secondJump.active){
                    auto second=MovementBank::blend(p.bank.sampleParameter(178,j,p.secondJump.progress),p.bank.sampleParameter(177,j,p.secondJump.progress),p.armedMix);
                    if(p.secondFrom.size()==count){const float t=p.secondJump.transition;second=MovementBank::blend(p.secondFrom[j],second,t*t*(3-2*t));}
                    air=second;
                }
                if(p.clock.airState==2)air=MovementBank::blend(air,special(38,std::fmod(p.clock.airTime/p.bank.clips[38].angle,1.f)),p.clock.fallMix);
            }else{
                auto standing=MovementBank::blend(special(39,p.clock.airTime/p.bank.clips[39].angle),special(41,p.clock.airTime/p.bank.clips[41].angle),p.clock.airRun);
                air=MovementBank::blend(standing,special(42,p.clock.airTime/p.bank.clips[42].angle),p.clock.stanceMix);
            }
            if(p.airLast.size()!=count)p.airLast.resize(count);p.airLast[j]=air;
            v=MovementBank::blend(v,air,p.clock.airWeight);
        }
        if(p.bank.hasCombat()&&p.combatWeight>.001f&&p.combatClip>=44){
            auto action=p.bank.sampleCombat(p.combatClip,j,p.combatAmount,p.combatAirMix);
            if(p.combatClip>=53&&p.combatClip<=55)action=MovementBank::blend(action,p.bank.sampleCombat(p.combatClip+3,j,p.combatAmount,p.combatAirMix),p.clock.stanceMix);
            if(p.combatFrom.size()==count)action=MovementBank::blend(p.combatFrom[j],action,p.combatTransition*p.combatTransition*(3-2*p.combatTransition));
            if(p.combatLast.size()!=count)p.combatLast.resize(count);p.combatLast[j]=action;
            if(p.bank.hasAirCombat()&&MovementBank::combatLocomotionJoint(j,p.parents))
                action=MovementBank::blend(action,v,p.combatMoveMix*(1-p.combatAirMix));
            v=MovementBank::blend(v,action,p.combatWeight);
        }
        if(p.bank.hasEquipment()&&p.equipmentWeight>.001f&&p.equipmentClip>=85){
            const auto equipment=p.bank.sampleParameter(p.equipmentClip,j,p.equipmentAmount);v=MovementBank::blend(v,equipment,p.equipmentWeight);
        }
        if(p.bank.hasLowMantle()&&p.traversalWeight>.001f){
            const unsigned armedClip=p.traversalVariant==6&&p.bank.hasHardLanding()?179u:(p.traversalVariant==4||p.traversalVariant==5)&&p.bank.hasHighMantle()?173u+(p.traversalVariant-4)*2u:p.traversalVariant==3&&p.bank.hasVault()?115u:p.traversalVariant==2&&p.bank.hasCrouchMantle()?113u:111u;
            auto climb=MovementBank::blend(p.bank.sampleParameter(armedClip+1,j,p.traversalAmount),p.bank.sampleParameter(armedClip,j,p.traversalAmount),p.armedMix);
            if(p.traversalVariant>=7&&p.bank.hasKeyhole())climb=p.bank.sampleParameter(183+p.traversalVariant-7,j,p.traversalAmount);
            if(p.traversalVariant==6&&p.bank.hasCrouchLanding()){
                const auto crouch=MovementBank::blend(p.bank.sampleParameter(182,j,p.traversalAmount),p.bank.sampleParameter(181,j,p.traversalAmount),p.armedMix);
                climb=MovementBank::blend(climb,crouch,p.landingCrouchMix);
            }
            if(j==2&&p.traversalVariant==3&&p.bank.hasVault()){
                const float frame=(std::max)(0.f,(std::min)(1.f,p.traversalAmount))*119;const auto a=unsigned(frame),b=(std::min)(119u,a+1);
                const auto& curve=movementVaultLift[localRole?0:1];const float lift=curve[a]+(curve[b]-curve[a])*(frame-a);
                climb[6]+=(p.traversalHeight-1.f)*lift; // Visual pelvis only; no actor/root or animation events.
            }
            v=MovementBank::blend(v,climb,p.traversalWeight);
        }
        if(p.bank.hasPowers()&&p.powerWeight>.001f&&p.powerVariant>=1&&(p.powerVariant<=9||(p.powerVariant==10&&p.bank.hasDarkVision())||(p.powerVariant==11&&p.bank.hasWindblast()))){
            auto power=p.powerVariant==10?p.bank.sampleDarkVision(j,p.powerAmount,p.clock.stanceMix,p.armedMix):p.powerVariant==11?p.bank.sampleWindblast(j,p.powerAmount,p.clock.stanceMix,p.armedMix):p.bank.sampleParameter(116+p.powerVariant,j,p.powerAmount);
            if(p.powerFrom.size()==count)power=MovementBank::blend(p.powerFrom[j],power,p.powerTransition*p.powerTransition*(3-2*p.powerTransition));
            if(p.powerLast.size()!=count)p.powerLast.resize(count);p.powerLast[j]=power;
            if(((p.powerVariant==10&&p.bank.hasDarkVisionContexts())||(p.powerVariant==11&&p.bank.hasWindblastContexts()))&&MovementBank::combatLocomotionJoint(j,p.parents))power=MovementBank::blend(power,v,(std::max)(p.powerMoveMix,p.clock.airWeight));
            v=MovementBank::blend(v,power,p.powerWeight);
        }
        std::array<float,12> q{};std::copy(v.begin(),v.end(),q.begin());q[8]=q[9]=q[10]=1;ok=bodyFromQts(q,pose[j]);
        if(ok&&p.clock.weight<.999f){BodyMatrix native{};std::memcpy(native.v,s.localPose[j].data(),64);if(!bodyRigid(native)){ok=false;break;}
            float x[9]{},y[9]{},r[9]{};for(unsigned c=0;c<3;++c)for(unsigned k=0;k<3;++k){x[c*3+k]=native.v[c*4+k];y[c*3+k]=pose[j].v[c*4+k];}
            presentationRotation(x,y,p.clock.weight,r);for(unsigned c=0;c<3;++c)for(unsigned k=0;k<3;++k)pose[j].v[c*4+k]=r[c*3+k];
            for(unsigned k=12;k<15;++k)pose[j].v[k]=native.v[k]+(pose[j].v[k]-native.v[k])*p.clock.weight;}
    }
    if(ok&&phase==0&&p.currentFeet&&p.clock.weight>.99f){
        // Build only the native primary chain; helper bones are recomputed by
        // the engine and are not treated as authored knee references.
        for(unsigned j=0;j<27;++j){BodyMatrix local=pose[j];if(j<2)std::memcpy(local.v,s.localPose[j].data(),64);
            const int parent=p.parents[j];if(parent>=static_cast<int>(j)||parent<-1||!bodyRigid(local)){ok=false;break;}
            p.kneeReference[j]=parent>=0?bodyMultiply(p.kneeReference[parent],local):local;
        }
        p.kneeReady=ok;p.kneeSequence=bodyPublication.sequence.load();
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
    fprintf(f,"{\"experimental\":true,\"active\":%s,\"end\":%llu,\"joints\":%u,\"phase\":%.7f,\"speed\":%.6f,\"weight\":%.6f,\"a\":%u,\"b\":%u,\"mix\":%.6f,\"checksum\":%llu,\"applied\":%llu,\"refused\":%llu,\"gaits\":%u,\"runMix\":%.6f,\"version\":%u,\"stanceMix\":%.6f,\"sprintMix\":%.6f,\"sprintSide\":%u,\"sprintDirection\":%.6f,\"leanAmount\":%.6f,\"leanWeight\":%.6f,\"leanUp\":%.6f,\"slideState\":%u,\"slideTime\":%.6f,\"slideWeight\":%.6f,\"airState\":%u,\"airTime\":%.6f,\"airWeight\":%.6f,\"airProgress\":%.6f,\"airRun\":%.6f,\"airCrouch\":%.6f,\"combatVariant\":%u,\"combatPhase\":%u,\"combatClip\":%u,\"combatAmount\":%.6f,\"combatWeight\":%.6f,\"combatEvent\":%llu,\"equipmentVariant\":%u,\"equipmentAmount\":%.6f,\"equipmentWeight\":%.6f,\"armedMix\":%.6f,\"presentedDrawn\":%s,\"traversalVariant\":%u,\"traversalAmount\":%.6f,\"traversalWeight\":%.6f,\"traversalEvent\":%llu,\"powerVariant\":%u,\"powerAmount\":%.6f,\"powerWeight\":%.6f,\"powerEvent\":%llu,\"combatAirMix\":%.6f,\"combatMoveMix\":%.6f,\"nativeFeet\":%s,\"currentFeet\":%s,\"feetCasts\":%llu,\"feetSolves\":%llu,\"feetResetsAvoided\":%llu,\"feetReferences\":%llu,\"terrainWeight\":%.6f,\"terrainGrade\":%.6f,\"terrainDown\":%.6f,\"terrainRun\":%.6f,\"turnWeight\":%.6f,\"turnPhase\":%.6f,\"turnVelocity\":%.6f,\"turnClip\":%u,\"turnSerial\":%u,\"kneeCorrections\":%llu,\"kneeReject\":%u,\"kneeOffsetError\":%.8f,\"secondJump\":%s,\"secondJumpSerial\":%u,\"secondJumpProgress\":%.6f,\"secondJumpTransition\":%.6f,\"landingCrouchMix\":%.6f}\n",p.end?"true":"false",p.end,p.bank.joints,p.clock.phase,p.clock.speed,p.clock.weight,p.clock.a,p.clock.b,p.clock.mix,p.bank.checksum,p.applied,p.refused,p.bank.gaits,p.clock.runMix,p.bank.version,p.clock.stanceMix,p.clock.sprintMix,p.clock.sprintSide,p.clock.sprintDirection,p.clock.leanAmount,p.clock.leanWeight,p.clock.leanUp,p.clock.slideState,p.clock.slideTime,p.clock.slideWeight,p.clock.airState,p.clock.airTime,p.clock.airWeight,p.clock.airProgress,p.clock.airRun,p.clock.airCrouch,p.combatVariant,p.combatPhase,p.combatClip,p.combatAmount,p.combatWeight,p.combatEvent,p.equipmentVariant,p.equipmentAmount,p.equipmentWeight,p.armedMix,p.presentedDrawn?"true":"false",p.traversalVariant,p.traversalAmount,p.traversalWeight,p.traversalEvent,p.powerVariant,p.powerAmount,p.powerWeight,p.powerEvent,p.combatAirMix,p.combatMoveMix,p.nativeFeet?"true":"false",p.currentFeet?"true":"false",movementFeetCasts.load(),movementFeetSolves.load(),movementFeetResetsAvoided.load(),movementFeetReferences.load(),p.clock.terrainWeight,p.clock.terrainGrade,p.clock.terrainDown,p.clock.terrainRun,p.clock.turnWeight,p.clock.turnPhase,p.clock.turnVelocity,p.clock.turnClip,p.clock.turnSerial,p.kneeCorrections,p.kneeReject,p.kneeOffsetError,p.secondJump.active?"true":"false",p.secondJump.serial,p.secondJump.progress,p.secondJump.transition,p.landingCrouchMix);
    ReleaseSRWLockShared(&movementLock);const bool ok=!ferror(f);const int close=fclose(f);if(ok&&!close)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
