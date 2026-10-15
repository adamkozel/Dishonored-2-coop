#pragma once
#include "movement_bank.h"
#include "movement_second_jump.h"
#include "movement_knee_pole.h"
#include "movement_equipment_contact.h"
#include "movement_vault_lift.h"
#include "presentation_refusal.h"
#include "movement_arm_reach.h"
void requestMovementEquipment(bool drawn);
void requestPossessionBodyVisibility(bool hide);
SRWLOCK movementLock=SRWLOCK_INIT;
bool movementNativeFeetRequested=false; // isolated A/B experiment; frozen per bank incarnation
bool movementCurrentFeetRequested=false;
// 1203b: in-place reactivation of a dormant, already validated bank and the
// engine-thread commit cost (diagnostics for the 1204 transport stalls).
std::uint64_t movementReactivations=0;double movementCommitMs=0,movementCommitMaxMs=0;
extern std::atomic<std::uint64_t> movementFeetCasts,movementFeetSolves,movementFeetResetsAvoided,movementFeetReferences;
struct MovementPlayback {
    // Freeze bounded local root evidence on the first clock rejection. No
    // native reads, hooks, packet changes or relaxation of presentation guards.
    struct ClockTrace {double now=0,poseTime=0;float z=0;unsigned motion=0,airState=0;bool trajectory=false,eligible=false;};
    struct RootTrace {double time=0;float z=0;unsigned sequence=0,motion=0;bool valid=false;};
    std::array<ClockTrace,32> clockTrace{};std::array<RootTrace,16> clockRoots{};
    unsigned clockTraceCount=0,clockRootCount=0;bool clockTraceFrozen=false;
    // Fixed-size evidence for presentation eligibility failures. No relaxed leases.
    struct EligibilityRecord {
        double now=0,poseTime=0,firstSemantic=0,lastSemantic=0;
        unsigned mask=0,state=0,motion=0,stance=0,semanticCount=0;
        ULONGLONG stateAge=0;bool motionValid=false;
        unsigned rootCount=0;std::uint64_t rootResets=0;
        double firstRoot=0,lastRoot=0,renderTime=0;
    };
    std::array<EligibilityRecord,16> eligibilityRecords{};
    std::uint64_t eligibilityFailures=0,eligibilityRecordCount=0;
    unsigned eligibilityMask=0;
    MovementBank bank;MovementClock clock;MovementSecondJump secondJump;
    std::vector<MovementBank::Joint> airLast,secondFrom;
    std::uintptr_t game=0,actor=0,animation=0,skeleton=0;unsigned handle=0;
    std::uint64_t epoch=0,peerEpoch=0,session=0;ULONGLONG end=0,lease=0;
    std::vector<std::int16_t> parents;std::uint64_t applied=0,refused=0;PresentationRefusalGate refusal;bool persistent=false,nativeFeet=false,currentFeet=false;
    std::uint64_t combatEvent=0;unsigned combatVariant=0,combatPhase=0,combatClip=0;float combatRecoveryStart=.5f;
    double combatLastTime=0;float combatAmount=0,combatWeight=0,combatTransition=0,combatAirMix=0,combatMoveMix=0;
    std::vector<MovementBank::Joint> combatFrom,combatLast;
    unsigned equipmentVariant=0,equipmentClip=0;float equipmentAmount=0,equipmentWeight=0,armedMix=1;bool presentedDrawn=true;
    unsigned traversalVariant=0;float traversalAmount=0,traversalWeight=0,traversalHeight=0,landingCrouchMix=0;std::uint64_t traversalEvent=0;
    float shadowWalkPhase=0;
    unsigned powerVariant=0;float powerAmount=0,powerWeight=0,powerTransition=0,powerMoveMix=0;std::uint64_t powerEvent=0;
    std::vector<MovementBank::Joint> powerFrom,powerLast;
    unsigned rangedKind=0,rangedVariant=0,rangedMechanism=0,rangedShot=0;float rangedAmount=0,rangedWeight=0,rangedTransition=0,rangedMoveMix=0;
    std::uint64_t rangedEvent=0;std::vector<MovementBank::Joint> rangedFrom,rangedLast;
    unsigned stunVariant=0;float stunAmount=0,stunWeight=0,stunTransition=0;std::uint64_t stunEvent=0;
    unsigned useVariant=0;float useAmount=0,useWeight=0,useTransition=0;std::uint64_t useEvent=0;std::vector<MovementBank::Joint> useFrom,useLast;
    // 1213k (#6): the used mechanism (world) and the right-hand reach toward it.
    float useTarget[3]{};bool useTargetValid=false;std::uint64_t useReachApplied=0,useReachRefused=0;float useReachError=0,useReachWeight=0;
    std::vector<MovementBank::Joint> stunFrom,stunLast;
    unsigned grenadeHeld=0,grenadeVariant=0;float grenadeAmount=0,grenadeWeight=0,grenadeTransition=0,grenadeMoveMix=0;
    std::uint64_t grenadeEvent=0;std::vector<MovementBank::Joint> grenadeFrom,grenadeLast;
    unsigned heartHeld=0,heartVariant=0;float heartAmount=0,heartWeight=0,heartTransition=0,heartMoveMix=0;
    std::uint64_t heartEvent=0;std::vector<MovementBank::Joint> heartFrom,heartLast;
    unsigned chokeVariant=0;float chokeAmount=0,chokeWeight=0,chokeTransition=0;
    // Bank69 swimming: 359 tread loop, 360 stroke loop, 361 surface/up, 362 dive/down.
    // Bank70 climbing: 365 ascend, 366 descend cycles advanced by replicated vertical distance.
    float climbWeight=0,climbPhase=0,climbDescend=0,climbVz=0,climbZ=0;double climbTime=0;std::uint64_t climbSamples=0;
    float swimWeight=0,swimMove=0,swimRise=0,swimSink=0,swimTread=0,swimStroke=0,swimSpeed=0,swimVz=0,swimPos[3]{};double swimTime=0;std::uint64_t swimSamples=0;
    std::uint64_t chokeEvent=0;std::vector<MovementBank::Joint> chokeFrom,chokeLast;
    unsigned trapStock=0,trapHeld=0,trapVariant=0;float trapAmount=0,trapWeight=0,trapTransition=0,trapMoveMix=0;
    std::uint64_t trapEvent=0;std::vector<MovementBank::Joint> trapFrom,trapLast;
    unsigned carryVariant=0,carryStance=0;std::uint64_t carryEvent=0;
    float carryAmount=0,carryWeight=0,carryMove=0,carryPhase[2]{};
    float carrySettle=1;std::vector<MovementBank::Joint> carryFrom,carryLast;
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
    // 1203b: bind pose captured on the engine thread at request time; the
    // worker checks every clip against it, so the engine-thread commit only
    // compares this snapshot with the live skeleton (no full-bank sweep).
    std::vector<std::array<float,12>> bind;bool bindMatches=false;
} movementLoad;
DWORD WINAPI movementLoadWorker(void*){
    const auto start=GetTickCount64();bool ok=false;FILE* f=nullptr;
    try {
        if(!fopen_s(&f,movementLoad.path,"rb")&&f){
            std::vector<unsigned char> bytes(MovementBank::maximumBytes+1);
            const auto n=fread(bytes.data(),1,bytes.size(),f);const bool io=!ferror(f);fclose(f);f=nullptr;bytes.resize(n);
            ok=io&&movementLoad.next.bank.decode(bytes,movementLoad.layout,movementLoad.joints);
            bool matches=ok&&movementLoad.bind.size()==movementLoad.next.bank.joints;
            const auto& b=movementLoad.next.bank;
            if(matches)for(const auto& clip:b.clips){for(unsigned frame=0;frame<MovementBank::frames&&matches;++frame)for(unsigned j=3;j<=26&&matches;++j)if(j!=17)
                for(unsigned k=0;k<3;++k)if(std::fabs(clip.values[frame*b.joints+j][4+k]-movementLoad.bind[j][4+k])>.0001f){matches=false;break;}
                if(!matches)break;}
            movementLoad.bindMatches=matches;
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
// 1213i (#34/#35): why the last bank request was refused (status beginRefusal); 0 = accepted.
// 1 load busy, 2 already active, 3 review/authoring, 4 partner avatar not held/followed, 5 body stream,
// 6 shadow action active, 7 frame not owned, 8 gameplay not ready, 9 stale peer, 10 native functions,
// 11 skeleton read/unstable/not gameplay/identity, 12 shadow target, 13 helpers, 14 sword read,
// 15 sword not equipped/started, 16 bank path, 17 worker thread.
unsigned movementBeginRefusal=0;std::array<std::uint64_t,18> movementBeginRefusals{};
bool movementBeginRefused(unsigned reason){movementBeginRefusal=reason;if(reason<movementBeginRefusals.size())++movementBeginRefusals[reason];return false;}
bool beginMovementBank(bool persistent=false,std::uint64_t expectedChecksum=0){
    const auto tick=GetTickCount64();
    if(movementLoad.state.load(std::memory_order_acquire))return movementBeginRefused(1);
    if(movementBankActive())return movementBeginRefused(2);
    if(reviewClipActive()||authoringProof.end)return movementBeginRefused(3);
    if(!probeDoppelganger||!avatarAiHeld||!probeFollow||!locomotionEnabled)return movementBeginRefused(4);
    if(bodyStreamEnabled)return movementBeginRefused(5);
    if(shadowAction.actor)return movementBeginRefused(6);
    if(!networkFrameOwned.load())return movementBeginRefused(7);
    if(!coopGameplayReady(outgoing)||!coopGameplayReady(remote))return movementBeginRefused(8);
    if(!lastReceive||tick-lastReceive>=presentationFreshMs())return movementBeginRefused(9);
    if(!bodyNativeFunctions())return movementBeginRefused(10);
    NativeSkeleton s{};NativeSwordEquipment sword{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(!readNativeSkeleton(memoryRead,base,probeHandle,s)||!s.stable||s.logic!=1||s.actor!=probeActor||s.game!=observedGame)return movementBeginRefused(11);
    if(!bodyShadowTarget(s))return movementBeginRefused(12);
    if(!movementHelperMatches(s))return movementBeginRefused(13);
    if(!readNativeSwordEquipment(memoryRead,base,probeHandle,sword,localRole?NativeSwordScope::DoppelgangerCorvo:NativeSwordScope::DoppelgangerEmily))return movementBeginRefused(14);
    // 1213j (#35): a settled native sword - drawn OR holstered - is valid. The partner's holster state
    // (requestMovementEquipment) holsters the avatar sword natively; requiring it drawn meant a bank that
    // dropped while the partner was holstered (1230/1230b: after the tutorial modal) could never re-arm.
    if(sword.equipped!=sword.started||sword.attachedTag!=(sword.equipped?sword.drawTag:sword.holsterTag))return movementBeginRefused(15);
    movementBeginRefusal=0;
    // 1203b: a bank already committed for this exact partner actor/skeleton in
    // this world is dormant after a follow interruption (stale peer, menu,
    // modal). Reactivate it in place with fresh runtime state instead of
    // re-decoding ~270 MB from disk, which stalled the engine thread and made
    // the other peer's follow go stale (1204 ping-pong).
    {
        bool reactivated=false;
        AcquireSRWLockExclusive(&movementLock);auto& d=movementPlayback;
        if(!d.end&&!d.bank.clips.empty()&&d.game==s.game&&d.actor==s.actor&&d.animation==s.animation&&d.skeleton==s.skeleton&&d.handle==probeHandle&&
           d.parents==s.parents&&d.epoch==outgoing.worldEpoch&&(!expectedChecksum||d.bank.checksum==expectedChecksum)){
            MovementPlayback fresh{};fresh.bank=std::move(d.bank);fresh.nativeFeet=movementNativeFeetRequested;fresh.currentFeet=movementCurrentFeetRequested;
            fresh.game=s.game;fresh.actor=s.actor;fresh.animation=s.animation;fresh.skeleton=s.skeleton;fresh.handle=probeHandle;fresh.parents=s.parents;
            fresh.epoch=outgoing.worldEpoch;fresh.peerEpoch=remote.worldEpoch;fresh.session=remote.session;fresh.persistent=persistent;
            fresh.end=tick+60000;fresh.lease=lastReceive+presentationFreshMs();fresh.presentedDrawn=sword.equipped;fresh.armedMix=sword.equipped?1.f:0.f;d=std::move(fresh);reactivated=true;
        }
        ReleaseSRWLockExclusive(&movementLock);
        if(reactivated){bodyPublication.animation.store(s.animation,std::memory_order_release);++movementReactivations;consoleOutput("COOP: movement bank reactivated in place\n");return true;}
    }
    // 1181: an installed release keeps both partner banks in <game>\d2coop and
    // loads the one for the partner it renders (host renders Emily).
    if(productMode){if(std::snprintf(movementLoad.path,sizeof(movementLoad.path),"%s\\%s-choke-bank.bin",productDataDir,localRole?"corvo":"emily")>=static_cast<int>(sizeof(movementLoad.path)))return movementBeginRefused(16);}
    else if(std::snprintf(movementLoad.path,sizeof(movementLoad.path),"%s.movement.bin",statusPath)>=sizeof(movementLoad.path))return movementBeginRefused(16);
    MovementPlayback next{};next.nativeFeet=movementNativeFeetRequested;next.currentFeet=movementCurrentFeetRequested;
    next.game=s.game;next.actor=s.actor;next.animation=s.animation;next.skeleton=s.skeleton;next.handle=probeHandle;next.parents=s.parents;
    next.epoch=outgoing.worldEpoch;next.peerEpoch=remote.worldEpoch;next.session=remote.session;next.persistent=persistent;
    next.presentedDrawn=sword.equipped;next.armedMix=sword.equipped?1.f:0.f; // 1213j (#35): start in the native sword state
    movementLoad.next=std::move(next);movementLoad.layout=localRole?0x00db32d37719931full:0x84db84f5d1336b80ull;movementLoad.joints=localRole?215:219;
    movementLoad.bind=s.bindLocal;movementLoad.bindMatches=false;
    movementLoad.expectedChecksum=expectedChecksum;movementLoad.requested=true;movementLoad.deadline=tick+5000;movementLoad.state.store(1,std::memory_order_release);
    HANDLE worker=CreateThread(nullptr,0,movementLoadWorker,nullptr,0,nullptr);
    if(!worker){movementLoad.requested=false;movementLoad.state.store(0);return movementBeginRefused(17);}CloseHandle(worker);return true;
}
void serviceMovementBank(ULONGLONG tick){
    const auto state=movementLoad.state.load(std::memory_order_acquire);
    if(state>=2){
        const double commitStart=presentationClock();
        auto& next=movementLoad.next;NativeSkeleton s{};NativeSwordEquipment sword{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
        bool valid=state==2&&(!movementLoad.expectedChecksum||next.bank.checksum==movementLoad.expectedChecksum)&&movementLoad.requested&&tick<movementLoad.deadline&&next.actor==probeActor&&next.game==observedGame&&next.handle==probeHandle&&
            next.epoch==outgoing.worldEpoch&&next.peerEpoch==remote.worldEpoch&&next.session==remote.session&&probeFollow&&avatarAiHeld&&locomotionEnabled&&!bodyStreamEnabled&&
            !reviewClipActive()&&!authoringProof.end&&!shadowAction.actor&&networkFrameOwned.load()&&coopGameplayReady(outgoing)&&coopGameplayReady(remote)&&lastReceive&&tick-lastReceive<presentationFreshMs()&&
            readNativeSkeleton(memoryRead,base,probeHandle,s)&&s.stable&&s.logic==1&&s.actor==next.actor&&s.game==next.game&&s.animation==next.animation&&s.skeleton==next.skeleton&&s.parents==next.parents&&movementHelperMatches(s)&&
            readNativeSwordEquipment(memoryRead,base,probeHandle,sword,localRole?NativeSwordScope::DoppelgangerCorvo:NativeSwordScope::DoppelgangerEmily)&&
            sword.equipped==sword.started&&sword.attachedTag==(sword.equipped?sword.drawTag:sword.holsterTag); // 1213j (#35): drawn or holstered
        // 1203b: the worker already checked every clip against the bind pose
        // captured at request time; only that snapshot is compared here.
        valid=valid&&movementLoad.bindMatches&&movementLoad.bind.size()==s.bindLocal.size();
        if(valid)for(unsigned j=3;j<=26&&valid;++j)if(j!=17)for(unsigned k=0;k<3;++k)if(std::fabs(movementLoad.bind[j][4+k]-s.bindLocal[j][4+k])>.0001f)valid=false;
        if(valid){next.end=tick+60000;next.lease=lastReceive+presentationFreshMs();AcquireSRWLockExclusive(&movementLock);movementPlayback=std::move(next);ReleaseSRWLockExclusive(&movementLock);
            bodyPublication.animation.store(s.animation,std::memory_order_release);}
        movementLoad.next={};movementLoad.requested=false;movementLoad.state.store(0,std::memory_order_release);
        movementCommitMs=presentationClock()-commitStart;movementCommitMaxMs=(std::max)(movementCommitMaxMs,movementCommitMs);
        consoleOutput("COOP: movement bank async %s (decode %llu ms, commit %.1f ms)\n",valid?"committed":"discarded",movementLoad.decodeMs.load(),movementCommitMs);
    }
    AcquireSRWLockExclusive(&movementLock);auto& p=movementPlayback;
    if(p.end){const bool ok=tick<p.end&&p.actor==probeActor&&p.game==observedGame&&p.handle==probeHandle&&p.epoch==outgoing.worldEpoch&&
        p.peerEpoch==remote.worldEpoch&&p.session==remote.session&&probeFollow&&avatarAiHeld&&locomotionEnabled&&!bodyStreamEnabled&&
        networkFrameOwned.load()&&coopPresentationReady(outgoing)&&coopPresentationReady(remote)&&lastReceive&&tick-lastReceive<presentationFreshMs();
        if(ok){p.lease=lastReceive+presentationFreshMs();if(p.persistent)p.end=tick+60000;}else p.end=p.lease=0;}
    ReleaseSRWLockExclusive(&movementLock);
}
void publishMovementBank(const PresentationPose& pose,const float* position,double now){
    const auto tick=GetTickCount64();bool equipmentRequest=false,equipmentDrawn=true,possessionHidden=false;AcquireSRWLockExclusive(&movementLock);auto& p=movementPlayback;
    if(p.end&&tick<p.lease){
        MovementSemantics::Sample semantic{};const bool semanticValid=shadowStream.movement.sample(pose.time,semantic);
        possessionHidden=semanticValid&&semantic.state==17&&p.bank.hasPossessionRelease();
        // Sword semantics travel through the existing native Steam action
        // stream. Unknown/stale actions never count as an idle permission.
        const bool ground=pose.stance==0&&pose.motion==0;
        const bool crouched=p.bank.hasCrouch()&&pose.stance==1&&pose.motion==1;
        CombatPresentation::Sample combat{};float amount=0;
        const bool combatValid=p.bank.hasCombat()&&shadowStream.combat.sample(pose.time,combat,amount);
        const bool combatAir=p.bank.hasAirCombat()&&pose.motionValid&&(pose.motion==2||pose.motion==3);
        const bool combatActive=combatValid&&combat.variant&&(combat.variant!=17||p.bank.hasKick())&&(combat.variant!=18||p.bank.hasCrouchHeavy())&&(combat.variant<19||p.bank.hasParryImpact())&&pose.motionValid&&(ground||crouched||combatAir);
        const float combatDt=p.combatLastTime?float((now-p.combatLastTime)*.001):0;p.combatLastTime=now;
        CarryPresentation::Sample carry{};float carryAmount=0;
        const bool carryValid=p.bank.hasCarry()&&shadowStream.carry.sample(pose.time,carry,carryAmount);
        // v35 adds a separate whole-body crouched hoist. Older banks retain
        // their standing-only pickup behavior.
        const bool carryLoop=carry.variant==2||carry.variant==3;
        const bool carryDrop=(p.bank.hasCarryRelease()&&(carry.variant==4||carry.variant==6))||(p.bank.hasCarryThrow()&&carry.variant==8)||
            (p.bank.hasCarryFetalRelease()&&carry.variant==5&&carry.stance==0);
        const bool carryPickupStance=(ground&&carry.stance==0)||(p.bank.hasCrouchedCarryPickup()&&crouched&&carry.stance==1);
        const bool carryPickup=p.bank.hasCarryPickup()&&(carry.variant==7||carry.variant==1)&&carryPickupStance;
        const bool carryUnbound=carryValid&&(carry.variant==7||carry.variant==1)&&pose.motionValid&&(ground||crouched);
        const bool carryActive=carryValid&&(carryLoop||carryDrop||carryPickup)&&pose.motionValid&&(ground||crouched);
        if(combatDt>=0&&combatDt<.25f){
            if(!carryActive||!carryPickupStance||p.carryStance!=carry.stance){p.carryFrom.clear();p.carrySettle=1;}
            if(carryActive){
                // v34 ends the hoist with contact still supported. Crossfade
                // once into the loaded pose, not at each native loop wrap.
                if(p.bank.hasCarryPickupSettle()&&carry.event!=p.carryEvent){
                    if(p.carryVariant==1&&carry.variant==2&&carryPickupStance&&p.carryStance==carry.stance&&p.carryWeight>.99f&&p.carryLast.size()==p.bank.joints){
                        p.carryFrom=p.carryLast;p.carrySettle=0;
                    }else if(!carryLoop){p.carryFrom.clear();p.carrySettle=1;}
                }
                p.carrySettle=(std::min)(1.f,p.carrySettle+combatDt/.35f);
                p.carryVariant=carry.variant;p.carryStance=carry.stance;p.carryAmount=carryAmount;p.carryEvent=carry.event;
                const float dx=position[0]-p.clock.position[0],dy=position[1]-p.clock.position[1],distance=std::sqrt(dx*dx+dy*dy);
                if(carryLoop&&p.carryWeight>.001f&&!pose.teleport&&distance<=6.f*combatDt+.01f)
                    for(unsigned i=0;i<2;++i)p.carryPhase[i]=std::fmod(p.carryPhase[i]+distance/p.bank.clips[226+i*2].stride,1.f);
            }
            p.carryWeight+=((carryActive?1.f:0.f)-p.carryWeight)*(1-std::exp(-combatDt/.10f));
            const float moving=carryTravelWeight(p.clock.speed,carryActive);
            p.carryMove+=(moving-p.carryMove)*(1-std::exp(-combatDt/.09f));
        }else{p.carryWeight=p.carryMove=0;p.carryFrom.clear();p.carrySettle=1;}
        RangedPresentation::Sample ranged{};float rangedAmount=0;
        const bool rangedValid=p.bank.hasRanged()&&shadowStream.ranged.sample(pose.time,ranged,rangedAmount);
        const bool rangedActive=rangedValid&&ranged.kind&&(ground||(p.bank.hasCrouchedRanged()&&crouched)||combatAir)&&pose.motionValid;
        if(combatDt>=0&&combatDt<.25f){
            if(rangedActive){
                if(ranged.event!=p.rangedEvent){p.rangedFrom=p.rangedLast;p.rangedTransition=0;p.rangedEvent=ranged.event;}
                p.rangedKind=ranged.kind;p.rangedVariant=ranged.variant;p.rangedMechanism=ranged.mechanism;p.rangedShot=ranged.shot;p.rangedAmount=ranged.variant?rangedAmount:std::fmod(float(pose.time*.001)/p.bank.clips[RangedPresentation::clip(ranged.kind,0)].angle,1.f);
                p.rangedTransition=(std::min)(1.f,p.rangedTransition+combatDt/.06f);
            }
            p.rangedWeight+=((rangedActive?1.f:0.f)-p.rangedWeight)*(1-std::exp(-combatDt/(rangedActive?.025f:.08f)));
            const float moving=(std::max)(0.f,(std::min)(1.f,(p.clock.speed-.1f)/.5f));
            p.rangedMoveMix+=(moving-p.rangedMoveMix)*(1-std::exp(-combatDt/.07f));
        }else p.rangedWeight=0;
        StunPresentation::Sample stun{};float stunAmount=0;
        const bool stunActive=p.bank.hasStun()&&shadowStream.stun.sample(pose.time,stun,stunAmount)&&stun.variant&&ground&&pose.motionValid;
        if(combatDt>=0&&combatDt<.25f){
            if(stunActive){if(stun.event!=p.stunEvent){p.stunFrom=p.stunLast;p.stunTransition=0;p.stunEvent=stun.event;}
                p.stunVariant=stun.variant;p.stunAmount=stunAmount;p.stunTransition=(std::min)(1.f,p.stunTransition+combatDt/.035f);}
            p.stunWeight+=((stunActive?1.f:0.f)-p.stunWeight)*(1-std::exp(-combatDt/(stunActive?.025f:.10f)));
        }else p.stunWeight=0;
        // 1213: authored interaction for a native use; fades out once the
        // remote player moves (root stays network owned).
        UsePresentation::Sample use{};float useAmount=0;
        const bool useActive=p.bank.hasUse()&&shadowStream.use.sample(pose.time,use,useAmount)&&use.variant&&ground&&pose.motionValid&&p.clock.speed<.8f;
        if(combatDt>=0&&combatDt<.25f){
            if(useActive){if(use.event!=p.useEvent){p.useFrom=p.useLast;p.useTransition=0;p.useEvent=use.event;}
                p.useVariant=use.variant;p.useAmount=useAmount;p.useTransition=(std::min)(1.f,p.useTransition+combatDt/.20f);
                std::memcpy(p.useTarget,use.target,sizeof(p.useTarget));p.useTargetValid=use.targetValid;}
            p.useWeight+=((useActive?1.f:0.f)-p.useWeight)*(1-std::exp(-combatDt/(useActive?.12f:.18f)));
        }else p.useWeight=0;
        GrenadePresentation::Sample grenade{};float grenadeAmount=0;
        const bool grenadeValid=p.bank.hasGrenade()&&shadowStream.grenade.sample(pose.time,grenade,grenadeAmount);
        const bool grenadeActive=grenadeValid&&grenade.held&&(ground||(p.bank.hasCrouchedGrenade()&&crouched)||combatAir)&&pose.motionValid;
        if(combatDt>=0&&combatDt<.25f){
            if(grenadeActive){
                if(grenade.event!=p.grenadeEvent){p.grenadeFrom=p.grenadeLast;p.grenadeTransition=0;p.grenadeEvent=grenade.event;}
                p.grenadeHeld=grenade.held;p.grenadeVariant=grenade.variant;
                p.grenadeAmount=grenade.variant?grenadeAmount:std::fmod(float(pose.time*.001)/p.bank.clips[251].angle,1.f);
                p.grenadeTransition=(std::min)(1.f,p.grenadeTransition+combatDt/.06f);
            }else p.grenadeHeld=0;
            p.grenadeWeight+=((grenadeActive?1.f:0.f)-p.grenadeWeight)*(1-std::exp(-combatDt/(grenadeActive?.025f:.10f)));
            const float moving=(std::max)(0.f,(std::min)(1.f,(p.clock.speed-.1f)/.5f));
            p.grenadeMoveMix+=(moving-p.grenadeMoveMix)*(1-std::exp(-combatDt/.07f));
        }else p.grenadeWeight=p.grenadeHeld=0;
        HeartPresentation::Sample heart{};float heartAmount=0;
        const bool heartValid=p.bank.hasHeart()&&shadowStream.heart.sample(pose.time,heart,heartAmount);
        const bool heartActive=heartValid&&heart.held&&(ground||(crouched))&&pose.motionValid;
        if(combatDt>=0&&combatDt<.25f){
            if(heartActive){
                if(heart.event!=p.heartEvent){p.heartFrom=p.heartLast;p.heartTransition=0;p.heartEvent=heart.event;}
                p.heartHeld=heart.held;p.heartVariant=heart.variant;
                p.heartAmount=heart.variant?heartAmount:std::fmod(float(pose.time*.001)/p.bank.clips[336].angle,1.f);
                p.heartTransition=(std::min)(1.f,p.heartTransition+combatDt/.06f);
            }else p.heartHeld=0;
            p.heartWeight+=((heartActive?1.f:0.f)-p.heartWeight)*(1-std::exp(-combatDt/(heartActive?.025f:.10f)));
            const float moving=(std::max)(0.f,(std::min)(1.f,(p.clock.speed-.1f)/.5f));
            p.heartMoveMix+=(moving-p.heartMoveMix)*(1-std::exp(-combatDt/.07f));
        }else p.heartWeight=p.heartHeld=0;
        ChokePresentation::Sample choke{};float chokeAmount=0;
        const bool chokeValid=p.bank.hasChoke()&&shadowStream.choke.sample(pose.time,choke,chokeAmount);
        // Native stance12 intentionally stays outside the legacy stance wire.
        // As with traversal, exact owned semantic scenes authorize the body;
        // the independent root channel continues to determine its position.
        const bool chokeActive=chokeValid&&choke.variant&&p.bank.hasChoke(choke.variant)&&semantic.state==21;
        // Native swimming (state22) has no stance/motion flag on the legacy
        // pose wire; the exact source class authorizes the body while the
        // independent root channel keeps world position and depth.
        const bool swimActive=p.bank.hasSwim()&&semanticValid&&semantic.state==22;
        const bool climbActive=p.bank.hasClimb()&&semanticValid&&semantic.state==23;
        {
            const double dt=p.climbTime?(now-p.climbTime)*.001:0;p.climbTime=now;
            if(climbActive&&dt>0&&dt<.25){
                const float dz=position[2]-p.climbZ;
                if(std::isfinite(dz)&&std::fabs(dz)<1.f){
                    p.climbVz+=(dz/float(dt)-p.climbVz)*(1-std::exp(-float(dt)/.12f));
                    // Assumed 1.5 m of travel per authored rung cycle (stand-in).
                    p.climbPhase=std::fmod(p.climbPhase+std::fabs(dz)/1.5f+1.f,1.f);
                    const float down=p.climbVz<-.05f?1.f:p.climbVz>.05f?0.f:p.climbDescend;
                    p.climbDescend+=(down-p.climbDescend)*(1-std::exp(-float(dt)/.10f));
                }
                ++p.climbSamples;
            }
            if(dt>0&&dt<.25)p.climbWeight+=((climbActive?1.f:0.f)-p.climbWeight)*(1-std::exp(-float(dt)/(climbActive?.08f:.15f)));
            else if(!climbActive)p.climbWeight=0;
            p.climbZ=position[2];
        }
        {
            const double dt=p.swimTime?(now-p.swimTime)*.001:0;p.swimTime=now;
            if(swimActive&&dt>0&&dt<.25){
                const float dx=position[0]-p.swimPos[0],dy=position[1]-p.swimPos[1],dz=position[2]-p.swimPos[2];
                const float horizontal=std::sqrt(dx*dx+dy*dy)/float(dt),vertical=dz/float(dt);
                if(std::isfinite(horizontal)&&horizontal<12.f&&std::isfinite(vertical)&&std::fabs(vertical)<12.f){
                    const float k=1-std::exp(-float(dt)/.15f);p.swimSpeed+=(horizontal-p.swimSpeed)*k;p.swimVz+=(vertical-p.swimVz)*k;
                }
                float move=(std::max)(0.f,(std::min)(1.f,(p.swimSpeed-.15f)/.8f));move=move*move*(3-2*move);
                p.swimMove+=(move-p.swimMove)*(1-std::exp(-float(dt)/.12f));
                const float rise=(std::max)(0.f,(std::min)(1.f,(p.swimVz-.25f)/.75f)),sink=(std::max)(0.f,(std::min)(1.f,(-p.swimVz-.25f)/.75f));
                p.swimRise+=(rise-p.swimRise)*(1-std::exp(-float(dt)/.12f));p.swimSink+=(sink-p.swimSink)*(1-std::exp(-float(dt)/.12f));
                p.swimTread=std::fmod(p.swimTread+float(dt)/p.bank.clips[359].angle,1.f);
                p.swimStroke=std::fmod(p.swimStroke+float(dt)/p.bank.clips[360].angle*(0.6f+0.4f*p.swimMove),1.f);
                ++p.swimSamples;
            }
            if(dt>0&&dt<.25)p.swimWeight+=((swimActive?1.f:0.f)-p.swimWeight)*(1-std::exp(-float(dt)/(swimActive?.08f:.15f)));
            else if(!swimActive)p.swimWeight=0;
            std::memcpy(p.swimPos,position,12);
        }
        if(p.bank.hasChoke()){
            if(chokeActive){
                if(choke.event!=p.chokeEvent){p.chokeFrom=p.chokeLast;p.chokeTransition=0;p.chokeEvent=choke.event;}
                p.chokeVariant=choke.variant;p.chokeAmount=chokeAmount;
                p.chokeTransition=(std::min)(1.f,p.chokeTransition+combatDt/.06f);
            }
            p.chokeWeight+=((chokeActive?1.f:0.f)-p.chokeWeight)*(1-std::exp(-combatDt/(chokeActive?.025f:.10f)));
        }else p.chokeWeight=0;
        TrapPresentation::Sample trap{};float trapAmount=0;
        const bool trapValid=p.bank.hasTrap()&&shadowStream.trap.sample(pose.time,trap,trapAmount);
        const bool trapActive=trapValid&&trap.held&&(ground||(crouched))&&pose.motionValid;
        if(combatDt>=0&&combatDt<.25f){
            if(trapActive){
                if(trap.event!=p.trapEvent){p.trapFrom=p.trapLast;p.trapTransition=0;p.trapEvent=trap.event;}
                p.trapHeld=trap.held;p.trapStock=trap.stock;p.trapVariant=trap.variant;
                p.trapAmount=trap.variant?trapAmount:std::fmod(float(pose.time*.001)/p.bank.clips[340].angle,1.f);
                p.trapTransition=(std::min)(1.f,p.trapTransition+combatDt/.06f);
            }else p.trapHeld=0;
            p.trapWeight+=((trapActive?1.f:0.f)-p.trapWeight)*(1-std::exp(-combatDt/(trapActive?.025f:.10f)));
            const float moving=(std::max)(0.f,(std::min)(1.f,(p.clock.speed-.1f)/.5f));
            p.trapMoveMix+=(moving-p.trapMoveMix)*(1-std::exp(-combatDt/.07f));
        }else p.trapWeight=p.trapHeld=0;
        EquipmentPresentation::Sample equipment{};float equipmentAmount=0;
        const bool equipmentValid=p.bank.hasEquipment()&&shadowStream.equipment.sample(pose.time,equipment,equipmentAmount);
        const bool possessionDraw=equipmentValid&&(equipment.variant==1||equipment.variant==3)&&equipment.sword==2;
        PowerPresentation::Sample power{};float powerAmount=0;
        const bool powerValid=p.bank.hasPowers()&&shadowStream.power.sample(pose.time,power,powerAmount);
        // Native v142/v144 observations cast all three Mesmerize phases with
        // the sword still holstered in standing and crouch. Context banks
        // supply complete empty-hand poses; older banks stay standing/armed.
        // Native armed/holstered air phases use air locomotion for pelvis/legs;
        // the complete authored cast supplies the remaining changing pose.
        const bool dominoActive=power.variant>=19&&power.variant<=22&&p.bank.hasDomino()&&(p.bank.hasDominoContexts()?(ground||crouched):(ground&&p.presentedDrawn));
        const bool swarmActive=power.variant>=23&&power.variant<=25&&p.bank.hasSwarm()&&(p.bank.hasSwarmContexts()?(ground||crouched):(ground&&p.presentedDrawn));
        // Release has its own exact native context; it does not advertise
        // generic ground locomotion while entering a possessed host.
        const bool possessionActive=(power.variant>=26&&power.variant<=27&&p.bank.hasPossession()&&(p.presentedDrawn||possessionDraw)&&
            (ground||(crouched&&p.bank.hasPossessionCrouch())))||
            ((pose.stance==0||(pose.stance==1&&p.bank.hasPossessionCrouchRelease()))&&
             power.variant==28&&p.bank.hasPossessionRelease()&&!p.presentedDrawn);
        const bool doppelgangerActive=power.variant>=29&&power.variant<=31&&p.bank.hasDoppelganger()&&(ground||combatAir||(crouched&&p.bank.hasDoppelgangerCrouch()))&&(p.presentedDrawn||p.bank.hasDoppelgangerHolstered());
        // Alternate native mode has no default Running/Crouch motion flag.
        // Its exact owned power reader supplies presentation eligibility;
        // replicated root placement and gameplay remain independent.
        const bool shadowWalkActive=((power.variant>=32&&power.variant<=34&&p.bank.hasShadowWalk())||(power.variant==35&&p.bank.hasShadowWalkSneak()))&&equipmentValid&&equipment.sword==1;
        const bool powerActive=powerValid&&power.variant&&(shadowWalkActive||doppelgangerActive||possessionActive||swarmActive||dominoActive||(power.variant>=16&&power.variant<=18&&p.bank.hasMesmerize()&&((p.bank.hasMesmerizeContexts()&&(ground||crouched||combatAir))||(ground&&p.presentedDrawn)))||power.variant<=9||
            (power.variant==10&&p.bank.hasDarkVision()&&(p.bank.hasDarkVisionContexts()||(ground&&p.presentedDrawn)))||
            (power.variant==11&&p.bank.hasWindblast()&&((p.bank.hasWindblastContexts()&&(ground||crouched||combatAir))||(ground&&p.presentedDrawn)))||
            (power.variant==15&&p.bank.hasOutOfMana()&&((p.bank.hasOutOfManaContexts()&&(ground||crouched))||(ground&&p.presentedDrawn)))||
            (power.variant>=12&&power.variant<=14&&p.bank.hasBendTime()&&((p.bank.hasBendTimeContexts()&&(ground||crouched||combatAir))||(ground&&p.presentedDrawn))));
        if(combatDt>=0&&combatDt<.25f){
            if(powerActive){
                if(power.event!=p.powerEvent){p.powerFrom=p.powerLast;p.powerTransition=0;p.powerEvent=power.event;}
                if((power.variant==32||power.variant==35)&&p.powerVariant!=power.variant)p.shadowWalkPhase=0;
                p.powerVariant=power.variant;p.powerAmount=power.variant==33?std::fmod(float(pose.time*.001)/p.bank.clips[332].angle,1.f):powerAmount;p.powerTransition=(std::min)(1.f,p.powerTransition+combatDt/.06f);
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
                    if(CombatPresentation::recovery(combat.variant))
                        p.combatRecoveryStart=CombatPresentation::continuationStart(combat.variant,p.combatVariant,p.combatAmount,p.combatWeight);
                    // A landing can advance the semantic event while the same
                    // native sword scene continues. Do not restart its blend.
                    if(combat.variant!=p.combatVariant||combat.phase!=p.combatPhase||(combat.phase!=2&&amount+.005f<p.combatAmount)){
                        p.combatFrom=p.combatLast;p.combatTransition=0;
                    }
                    p.combatEvent=combat.event;
                }
                p.combatVariant=combat.variant;p.combatPhase=combat.phase;p.combatAmount=amount;
                const unsigned v=combat.variant;
                p.combatClip=CombatPresentation::clip(v,combat.phase);
                p.combatTransition=(std::min)(1.f,p.combatTransition+combatDt/.09f);
            }
            p.combatAirMix+=((combatAir?1.f:0.f)-p.combatAirMix)*(1-std::exp(-combatDt/(combatAir?.045f:.10f)));
            float travelling=p.bank.hasAirCombat()?(std::max)(0.f,(std::min)(1.f,(p.clock.speed-.1f)/.5f)):0;
            travelling=travelling*travelling*(3-2*travelling);
            p.combatMoveMix+=(travelling-p.combatMoveMix)*(1-std::exp(-combatDt/.07f));
            p.combatWeight+=((combatActive?1.f:0.f)-p.combatWeight)*(1-std::exp(-combatDt/(combatActive?.06f:.16f)));
        }else p.combatWeight=p.combatAirMix=p.combatMoveMix=0;
        // Fast arm recovery after a climb does not reach toward the hip. Keep
        // the full-body traversal recovery and track actual native equipment.
        const bool equipmentActive=equipmentValid&&(equipment.variant>=1&&equipment.variant<=4||equipment.variant==7||equipment.variant==8)&&pose.motionValid&&(ground||crouched);
        if(combatDt>=0&&combatDt<.25f){
            if(equipmentActive){p.equipmentVariant=equipment.variant;p.equipmentClip=equipment.variant==7?86:equipment.variant==8?88:84+equipment.variant;p.equipmentAmount=equipmentAmount;}
            p.equipmentWeight+=((equipmentActive?1.f:0.f)-p.equipmentWeight)*(1-std::exp(-combatDt/(equipmentActive?.035f:.12f)));
            if(equipmentValid){
                equipmentDrawn=equipment.sword==2;
                if(equipmentActive&&equipment.variant<=4){const float contact=movementEquipmentContact[localRole?0:1][equipment.variant-1];
                    equipmentDrawn=equipment.variant==1||equipment.variant==3?equipmentAmount>=contact:equipmentAmount<contact;}
                p.presentedDrawn=equipmentDrawn;
                p.armedMix+=((equipmentDrawn?1.f:0.f)-p.armedMix)*(1-std::exp(-combatDt/.15f));
            }
        }else p.equipmentWeight=0;
        const bool horizontalLean=p.bank.hasLean()&&semantic.state==5&&(p.bank.hasVerticalPeek()||semantic.leanUp<=.0001f);
        const bool sliding=p.bank.hasSlide()&&semantic.state==6;
        // The confirmed root is deliberately delayed. A newer grounded
        // combat packet must not cancel its last airborne presentation frames.
        const bool airState=p.bank.hasAir()&&!traversalActive&&(!powerActive||power.variant==10||power.variant==12||power.variant==14||(power.variant>=16&&power.variant<=18)||(power.variant>=29&&power.variant<=31))&&pose.motionValid&&(pose.motion==2||pose.motion==3);
        // Native Sliding uses physics stance 5, outside the legacy body-pose
        // channel. Its exact-class action packet validates presentation while
        // the independent root channel continues to own world placement.
        const bool slideRecovery=p.bank.hasSlide()&&p.clock.slideState&&(semantic.state==0||semantic.state==4);
        const bool physicalEligible=((pose.motionValid&&(ground||crouched))||sliding||slideRecovery||airState||traversalActive||powerActive||carryActive||chokeActive||swimActive||climbActive);
        const bool actionEligible=(semantic.state==0||semantic.state==18||(p.bank.hasSprint()&&semantic.state==4)||horizontalLean||sliding||airState||(p.bank.hasAir()&&semantic.state==3)||
                (p.bank.hasCombat()&&(semantic.state==1||semantic.state==2))||combatActive||p.combatWeight>.001f||(p.bank.hasEquipment()&&semantic.state==7)||traversalActive||(p.bank.hasLowMantle()&&semantic.state==8)||(p.bank.hasVault()&&semantic.state==9)||(p.bank.hasHardLanding()&&semantic.state==11)||powerActive||rangedActive||grenadeActive||heartActive||trapActive||chokeActive||swimActive||climbActive||carryActive||carryUnbound||(p.bank.hasRanged()&&semantic.state==13)||(p.bank.hasPowers()&&semantic.state==10));
        const unsigned eligibilityMask=(!semanticValid?1u:0u)|(!physicalEligible?2u:0u)|
            (shadowStream.until<=tick?4u:0u)|(!shadowStream.peerReady?8u:0u)|
            (!shadowStream.lastState||tick-shadowStream.lastState>=250?16u:0u)|(!actionEligible?32u:0u)|(shadowAction.actor?64u:0u);
        const bool eligible=eligibilityMask==0;
        if(!eligible){
            ++p.eligibilityFailures;
            const auto count=shadowStream.movement.count;
            const auto lastIndex=p.eligibilityRecordCount?(p.eligibilityRecordCount-1)%p.eligibilityRecords.size():0;
            if(p.eligibilityMask!=eligibilityMask||!p.eligibilityRecordCount||now-p.eligibilityRecords[lastIndex].now>=250.){
                p.eligibilityRecords[p.eligibilityRecordCount++%p.eligibilityRecords.size()]={now,pose.time,
                    count?shadowStream.movement.samples[0].time:0,count?shadowStream.movement.samples[count-1].time:0,
                    eligibilityMask,semantic.state,pose.motion,pose.stance,count,
                    shadowStream.lastState?tick-shadowStream.lastState:0,pose.motionValid,
                    poseInterpolation.count,poseInterpolation.resets,
                    poseInterpolation.count?poseInterpolation.samples[0].time:0,
                    poseInterpolation.count?poseInterpolation.samples[poseInterpolation.count-1].time:0,poseInterpolation.renderTime};
            }
        }
        p.eligibilityMask=eligibilityMask;
        if(p.secondJump.advance(pose.time,semantic,pose.position[2],pose.motion,p.bank.hasDoubleJump()&&eligible&&airState))p.secondFrom=p.airLast;
        const bool transportPower=powerActive&&power.variant<=9;
        const unsigned stance=(transportPower||shadowWalkActive||swimActive||climbActive)?0u:traversalActive?traversal.endStance:sliding?1u:slideRecovery&&!pose.motionValid?(p.clock.stanceMix>.5f?1u:0u):pose.stance;
        const float height=semantic.targetZ-semantic.startZ;
        const float rise=semantic.airStage==1&&height>=.05f?(pose.position[2]-semantic.startZ)/height:p.clock.airProgress;
        // Confirmed native Blink arrival is a network presentation warp, not
        // a footstep or an implausibly fast walking segment.
        if(pose.teleport&&powerActive)std::memcpy(p.clock.position,position,12);
        if(!p.clockTraceFrozen)p.clockTrace[p.clockTraceCount++%p.clockTrace.size()]={now,pose.time,position[2],airState?pose.motion:0,p.clock.airState,pose.airTrajectory,eligible};
        const float shadowDx=position[0]-p.clock.position[0],shadowDy=position[1]-p.clock.position[1];
        const bool shadowStep=powerActive&&power.variant==33&&eligible&&p.clock.positioned&&!pose.teleport;
        if(!p.clock.advance(p.bank,now,position,pose.orientation,eligible,stance,semantic.state==4,horizontalLean,semantic.leanSide,sliding,airState?pose.motion:0,rise,p.armedMix,semantic.leanUp,traversalActive,transportPower,p.combatWeight<.001f&&p.equipmentWeight<.001f&&!powerActive&&!rangedActive&&!carryActive,pose.airTrajectory)){
            ++p.refused;
            if(!p.clockTraceFrozen){
                p.clockTraceFrozen=true;p.clockRootCount=(std::min)(poseInterpolation.count,unsigned(p.clockRoots.size()));
                for(unsigned i=0;i<p.clockRootCount;++i){const auto& q=poseInterpolation.samples[i];p.clockRoots[i]={q.time,q.position[2],q.sequence,q.motion,q.motionValid};}
            }
        }
        else if(shadowStep)p.shadowWalkPhase=p.bank.shadowWalkDistancePhase(p.shadowWalkPhase,shadowDx,shadowDy,pose.orientation[0],pose.orientation[1]);
        equipmentRequest=eligible&&equipmentValid;
        if(!eligible)p.clock.weight=0; // native action has immediate priority
    }ReleaseSRWLockExclusive(&movementLock);
    requestPossessionBodyVisibility(possessionHidden);
    if(possessionHidden)requestMovementEquipment(false);
    else if(equipmentRequest)requestMovementEquipment(equipmentDrawn);
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
    unsigned refuseReason=PresentationRefusalGate::None;
    bool ok=networkFrameOwned.load();if(!ok)refuseReason=PresentationRefusalGate::NotOwned;
    if(ok&&!(ok=readNativeSkeleton(memoryRead,base,p.handle,s)))refuseReason=PresentationRefusalGate::SkeletonRead;
    if(ok&&!(ok=s.stable))refuseReason=PresentationRefusalGate::Unstable;
    if(ok&&!(ok=s.logic==1))refuseReason=PresentationRefusalGate::NotGameplay;
    if(ok&&!(ok=s.game==p.game&&s.actor==p.actor&&s.animation==p.animation&&s.skeleton==p.skeleton&&s.parents==p.parents))refuseReason=PresentationRefusalGate::Identity;
    if(ok&&!(ok=movementHelperMatches(s)))refuseReason=PresentationRefusalGate::Helpers;
    const unsigned count=p.bank.joints;std::vector<BodyMatrix> pose(count);
    if(p.bank.hasCarryPickupSettle())p.carryLast.resize(count);
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
            const float authored=CombatPresentation::authoredAmount(p.combatVariant,p.combatAmount,p.combatRecoveryStart);
            auto action=p.bank.sampleCombat(p.combatClip,j,authored,p.combatAirMix);
            if(p.combatVariant==5||p.combatVariant==6||CombatPresentation::recovery(p.combatVariant))
                action=MovementBank::blend(p.bank.sampleCombat(p.combatClip-4,j,authored,0),action,p.clock.stanceMix);
            // Reuse the complete authored heavy action with the existing
            // whole-body crouch adaptation. Source stance/root stay native.
            if(p.bank.hasCrouchHeavy()&&p.combatClip>=50&&p.combatClip<=52)
                action=MovementBank::blend(action,p.bank.sampleCombat(354+p.combatClip-50,j,authored,0),p.clock.stanceMix*(1-p.combatAirMix));
            if(p.combatVariant==18)
                action=MovementBank::blend(p.bank.sampleCombat(52,j,authored,0),action,p.clock.stanceMix);
            if(p.combatClip>=53&&p.combatClip<=55)action=MovementBank::blend(action,p.bank.sampleCombat(p.combatClip+3,j,p.combatAmount,p.combatAirMix),p.clock.stanceMix);
            if(p.combatFrom.size()==count)action=MovementBank::blend(p.combatFrom[j],action,p.combatTransition*p.combatTransition*(3-2*p.combatTransition));
            if(p.combatLast.size()!=count)p.combatLast.resize(count);p.combatLast[j]=action;
            if(p.combatVariant!=17&&p.bank.hasAirCombat()&&MovementBank::combatLocomotionJoint(j,p.parents))
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
        if(p.bank.hasPowers()&&p.powerWeight>.001f&&p.powerVariant>=1&&(p.powerVariant<=9||(p.powerVariant==10&&p.bank.hasDarkVision())||(p.powerVariant==11&&p.bank.hasWindblast())||(p.powerVariant>=12&&p.powerVariant<=14&&p.bank.hasBendTime())||(p.powerVariant==15&&p.bank.hasOutOfMana())||(p.powerVariant>=16&&p.powerVariant<=18&&p.bank.hasMesmerize())||(p.powerVariant>=19&&p.powerVariant<=22&&p.bank.hasDomino())||(p.powerVariant>=23&&p.powerVariant<=25&&p.bank.hasSwarm())||(p.powerVariant>=26&&p.powerVariant<=27&&p.bank.hasPossession())||(p.powerVariant==28&&p.bank.hasPossessionRelease())||(p.powerVariant>=29&&p.powerVariant<=31&&p.bank.hasDoppelganger())||(p.powerVariant>=32&&p.powerVariant<=34&&p.bank.hasShadowWalk())||(p.powerVariant==35&&p.bank.hasShadowWalkSneak()))){
            auto power=p.powerVariant>=32?p.bank.sampleShadowWalk(p.powerVariant,j,p.powerAmount,p.shadowWalkPhase,p.powerMoveMix):p.powerVariant>=29?p.bank.sampleDoppelganger(p.powerVariant,j,p.powerAmount,p.clock.stanceMix,p.armedMix):p.powerVariant>=26?p.bank.samplePossession(p.powerVariant,j,p.powerAmount,p.clock.stanceMix):p.powerVariant>=23?p.bank.sampleSwarm(p.powerVariant,j,p.powerAmount,p.clock.stanceMix,p.armedMix):p.powerVariant>=19?p.bank.sampleDomino(p.powerVariant,j,p.powerAmount,p.clock.stanceMix,p.armedMix):p.powerVariant>=16?p.bank.sampleMesmerize(p.powerVariant,j,p.powerAmount,p.clock.stanceMix,p.armedMix):p.powerVariant==15?p.bank.sampleOutOfMana(j,p.powerAmount,p.clock.stanceMix,p.armedMix):p.powerVariant>=12?p.bank.sampleBendTime(p.powerVariant,j,p.powerAmount,p.clock.stanceMix,p.armedMix):p.powerVariant==10?p.bank.sampleDarkVision(j,p.powerAmount,p.clock.stanceMix,p.armedMix):p.powerVariant==11?p.bank.sampleWindblast(j,p.powerAmount,p.clock.stanceMix,p.armedMix):p.bank.sampleParameter(116+p.powerVariant,j,p.powerAmount);
            if(p.powerFrom.size()==count)power=MovementBank::blend(p.powerFrom[j],power,p.powerTransition*p.powerTransition*(3-2*p.powerTransition));
            if(p.powerLast.size()!=count)p.powerLast.resize(count);p.powerLast[j]=power;
            if(((p.powerVariant==10&&p.bank.hasDarkVisionContexts())||(p.powerVariant==11&&p.bank.hasWindblastContexts())||(p.powerVariant>=12&&p.powerVariant<=15&&p.bank.hasBendTime())||(p.powerVariant>=16&&p.powerVariant<=31&&p.bank.hasMesmerize()))&&MovementBank::combatLocomotionJoint(j,p.parents))power=MovementBank::blend(power,v,(std::max)(p.powerMoveMix,p.clock.airWeight));
            // Native holstered Possession casts with the left hand while the
            // sword hand is still drawing. Keep its independently timed full
            // draw subtree (including socket/fingers/helpers) through contact.
            if(p.powerVariant>=26&&p.powerVariant<=27&&p.equipmentWeight>.001f&&
               (p.equipmentVariant==1||p.equipmentVariant==3)&&MovementBank::rightArmJoint(j,p.parents))
                power=MovementBank::blend(power,v,p.equipmentWeight);
            v=MovementBank::blend(v,power,p.powerWeight);
        }
        if(p.bank.hasRanged()&&p.rangedWeight>.001f&&p.rangedKind){
            auto ranged=p.bank.sampleRanged(RangedPresentation::clip(p.rangedKind,p.rangedVariant),j,p.rangedAmount,p.clock.stanceMix);
            if(p.rangedFrom.size()==count)ranged=MovementBank::blend(p.rangedFrom[j],ranged,p.rangedTransition*p.rangedTransition*(3-2*p.rangedTransition));
            if(p.rangedLast.size()!=count)p.rangedLast.resize(count);p.rangedLast[j]=ranged;
            if(MovementBank::combatLocomotionJoint(j,p.parents))ranged=MovementBank::blend(ranged,v,(std::max)(p.rangedMoveMix,p.clock.airWeight));
            v=MovementBank::blend(v,ranged,p.rangedWeight);
        }
        if(p.bank.hasGrenade()&&p.grenadeWeight>.001f){
            auto grenade=p.bank.sampleParameter(GrenadePresentation::clip(p.grenadeVariant),j,p.grenadeAmount);
            if(p.bank.hasCrouchedGrenade())grenade=MovementBank::blend(grenade,p.bank.sampleParameter(264+p.grenadeVariant,j,p.grenadeAmount),p.clock.stanceMix);
            if(p.grenadeFrom.size()==count)grenade=MovementBank::blend(p.grenadeFrom[j],grenade,p.grenadeTransition*p.grenadeTransition*(3-2*p.grenadeTransition));
            if(p.grenadeLast.size()!=count)p.grenadeLast.resize(count);p.grenadeLast[j]=grenade;
            if(MovementBank::combatLocomotionJoint(j,p.parents))grenade=MovementBank::blend(grenade,v,(std::max)(p.grenadeMoveMix,p.clock.airWeight));
            v=MovementBank::blend(v,grenade,p.grenadeWeight);
        }
        if(p.bank.hasHeart()&&p.heartWeight>.001f){
            auto heart=p.bank.sampleParameter(HeartPresentation::clip(p.heartVariant),j,p.heartAmount);
            if(p.heartFrom.size()==count)heart=MovementBank::blend(p.heartFrom[j],heart,p.heartTransition*p.heartTransition*(3-2*p.heartTransition));
            if(p.heartLast.size()!=count)p.heartLast.resize(count);p.heartLast[j]=heart;
            if(MovementBank::combatLocomotionJoint(j,p.parents))heart=MovementBank::blend(heart,v,(std::max)(p.heartMoveMix,p.clock.stanceMix));
            v=MovementBank::blend(v,heart,p.heartWeight);
        }
        if(p.bank.hasChoke(p.chokeVariant)&&p.chokeWeight>.001f&&p.chokeVariant){
            auto choke=p.bank.sampleParameter(ChokePresentation::clip(p.chokeVariant),j,p.chokeAmount);
            if(p.chokeFrom.size()==count)choke=MovementBank::blend(p.chokeFrom[j],choke,p.chokeTransition*p.chokeTransition*(3-2*p.chokeTransition));
            if(p.chokeLast.size()!=count)p.chokeLast.resize(count);p.chokeLast[j]=choke;
            v=MovementBank::blend(v,choke,p.chokeWeight);
        }
        if(p.bank.hasClimb()&&p.climbWeight>.001f){
            const auto climb=MovementBank::blend(p.bank.sample(365,j,p.climbPhase),p.bank.sample(366,j,1.f-p.climbPhase),p.climbDescend);
            v=MovementBank::blend(v,climb,p.climbWeight);
        }
        if(p.bank.hasSwim()&&p.swimWeight>.001f){
            auto swim=MovementBank::blend(p.bank.sample(359,j,p.swimTread),p.bank.sample(360,j,p.swimStroke),p.swimMove);
            if(p.swimRise>.001f)swim=MovementBank::blend(swim,p.bank.sample(361,j,p.swimTread),p.swimRise*.7f);
            if(p.swimSink>.001f)swim=MovementBank::blend(swim,p.bank.sample(362,j,p.swimTread),p.swimSink*.7f);
            v=MovementBank::blend(v,swim,p.swimWeight);
        }
        if(p.bank.hasTrap()&&p.trapWeight>.001f){
            auto trap=p.bank.sampleParameter(TrapPresentation::clip(p.trapVariant),j,p.trapAmount);
            if(p.trapFrom.size()==count)trap=MovementBank::blend(p.trapFrom[j],trap,p.trapTransition*p.trapTransition*(3-2*p.trapTransition));
            if(p.trapLast.size()!=count)p.trapLast.resize(count);p.trapLast[j]=trap;
            if(MovementBank::combatLocomotionJoint(j,p.parents))trap=MovementBank::blend(trap,v,(std::max)(p.trapMoveMix,p.clock.stanceMix));
            v=MovementBank::blend(v,trap,p.trapWeight);
        }
        if(p.bank.hasStun()&&p.stunWeight>.001f&&p.stunVariant){
            auto stun=p.bank.sampleParameter(StunPresentation::clip(p.stunVariant),j,p.stunAmount);
            if(p.stunFrom.size()==count)stun=MovementBank::blend(p.stunFrom[j],stun,p.stunTransition*p.stunTransition*(3-2*p.stunTransition));
            if(p.stunLast.size()!=count)p.stunLast.resize(count);p.stunLast[j]=stun;
            v=MovementBank::blend(v,stun,p.stunWeight);
        }
        if(p.bank.hasUse()&&p.useWeight>.001f&&p.useVariant){
            auto use=p.bank.sampleParameter(UsePresentation::clip(p.useVariant),j,p.useAmount);
            if(p.useFrom.size()==count)use=MovementBank::blend(p.useFrom[j],use,p.useTransition*p.useTransition*(3-2*p.useTransition));
            if(p.useLast.size()!=count)p.useLast.resize(count);p.useLast[j]=use;
            v=MovementBank::blend(v,use,p.useWeight);
        }
        if(p.bank.hasCarry()&&p.carryWeight>.001f){
            MovementBank::Joint carryPose{};
            if(p.bank.hasCarryPickup()&&(p.carryVariant==7||p.carryVariant==1))
                carryPose=p.bank.sampleParameter(p.bank.hasCrouchedCarryPickup()&&p.carryStance==1?232:231,j,carryPickupAmount(p.carryVariant,p.carryAmount));
            else if(p.bank.hasCarryThrow()&&p.carryVariant==8)
                carryPose=p.bank.sampleParameter(233,j,p.carryAmount);
            else if(p.bank.hasCarryFetalRelease()&&p.carryVariant==5)
                carryPose=p.bank.sampleParameter(234,j,p.carryAmount);
            else if(p.bank.hasCarryRelease()&&(p.carryVariant==4||p.carryVariant==6))
                carryPose=p.bank.sampleParameter(229+(p.carryVariant==6),j,p.carryAmount);
            else{
                const auto stand=MovementBank::blend(p.bank.sample(225,j,p.carryAmount),p.bank.sample(226,j,p.carryPhase[0]),p.carryMove);
                const auto low=MovementBank::blend(p.bank.sample(227,j,p.carryAmount),p.bank.sample(228,j,p.carryPhase[1]),p.carryMove);
                carryPose=MovementBank::blend(stand,low,p.clock.stanceMix);
                if(p.bank.hasCarryPickupSettle()&&p.carryFrom.size()==count&&p.carrySettle<1){
                    const float t=p.carrySettle;carryPose=MovementBank::blend(p.carryFrom[j],carryPose,t*t*(3-2*t));
                }
            }
            if(p.bank.hasCarryPickupSettle())p.carryLast[j]=carryPose;
            v=MovementBank::blend(v,carryPose,p.carryWeight);
        }
        std::array<float,12> q{};std::copy(v.begin(),v.end(),q.begin());q[8]=q[9]=q[10]=1;ok=bodyFromQts(q,pose[j]);
        if(ok&&p.clock.weight<.999f){BodyMatrix native{};std::memcpy(native.v,s.localPose[j].data(),64);if(!bodyRigid(native)){ok=false;break;}
            float x[9]{},y[9]{},r[9]{};for(unsigned c=0;c<3;++c)for(unsigned k=0;k<3;++k){x[c*3+k]=native.v[c*4+k];y[c*3+k]=pose[j].v[c*4+k];}
            presentationRotation(x,y,p.clock.weight,r);for(unsigned c=0;c<3;++c)for(unsigned k=0;k<3;++k)pose[j].v[c*4+k]=r[c*3+k];
            for(unsigned k=12;k<15;++k)pose[j].v[k]=native.v[k]+(pose[j].v[k]-native.v[k])*p.clock.weight;}
    }
    if(!ok&&!refuseReason)refuseReason=PresentationRefusalGate::PoseBuild;
    // 1213k (#6): during the use clip's contact window, pull the right wrist (arm 14 -> elbow 15 -> wrist 16,
    // parent shoulder 13) toward the used mechanism. Forward kinematics of this frame's authored pose
    // gives the model-space chain; the target comes from the source's focused entity origin.
    p.useReachWeight=0;
    if(ok&&p.useWeight>.001f&&p.useVariant&&p.useTargetValid&&count>16&&p.parents.size()>16&&p.parents[14]==13&&p.parents[15]==14&&p.parents[16]==15){
        const float w=p.useWeight*UsePresentation::contact(p.useAmount);p.useReachWeight=w;
        if(w>.001f){
            std::array<BodyMatrix,17> chainLocal{},chainGlobal{};bool fk=true;
            for(unsigned j=0;j<17&&fk;++j){chainLocal[j]=pose[j];if(j<2)std::memcpy(chainLocal[j].v,s.localPose[j].data(),64);
                const int parent=p.parents[j];if(parent>=static_cast<int>(j)||parent<-1||!bodyRigid(chainLocal[j])){fk=false;break;}
                chainGlobal[j]=parent>=0?bodyMultiply(chainGlobal[parent],chainLocal[j]):chainLocal[j];}
            float target[3]{};useModelTarget(s.root,p.useTarget,target);
            BodyMatrix reach[3]{};float error=0;
            if(fk&&movementArmReach(chainGlobal[13],chainGlobal.data()+14,target,w,reach,error)){
                // Rotations change; the authored local bone offsets stay exact.
                for(unsigned j=0;j<3;++j){for(unsigned k=12;k<15;++k)reach[j].v[k]=pose[14+j].v[k];pose[14+j]=reach[j];}
                ++p.useReachApplied;p.useReachError=error;
            }else ++p.useReachRefused;
        }
    }
    if(ok&&phase==0&&p.currentFeet&&p.clock.weight>.99f){
        // Build only the native primary chain; helper bones are recomputed by
        // the engine and are not treated as authored knee references.
        for(unsigned j=0;j<27;++j){BodyMatrix local=pose[j];if(j<2)std::memcpy(local.v,s.localPose[j].data(),64);
            const int parent=p.parents[j];if(parent>=static_cast<int>(j)||parent<-1||!bodyRigid(local)){ok=false;break;}
            p.kneeReference[j]=parent>=0?bodyMultiply(p.kneeReference[parent],local):local;
        }
        p.kneeReady=ok;p.kneeSequence=bodyPublication.sequence.load();
        if(!ok)refuseReason=PresentationRefusalGate::KneeChain;
    }
    if(ok){for(unsigned j=2;j<count;++j)reinterpret_cast<void(*)(void*,std::uint16_t,const BodyMatrix*)>(gameImage+0x9cfa0)(reinterpret_cast<void*>(p.animation+0x40),static_cast<std::uint16_t>(j),&pose[j]);
        reinterpret_cast<void(*)(void*)>(gameImage+0x9dc40)(reinterpret_cast<void*>(p.animation+0x40));
        reinterpret_cast<void(*)(void*,void*,void*)>(gameImage+0x23fca0)(reinterpret_cast<void*>(s.model),reinterpret_cast<void*>(p.animation+0x40),reinterpret_cast<void*>(p.animation+0x2b0));++p.applied;p.refusal.accept();
    }else{++p.refused;if(p.refusal.refuse(refuseReason))p.end=p.lease=0;} // 1213i (#34): a transient refusal skips this frame only
    ReleaseSRWLockExclusive(&movementLock);
}
void writeMovementStatus(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.movement.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;
    AcquireSRWLockShared(&movementLock);const auto& p=movementPlayback;
    fputs("{\"clockTrace\":{\"frames\":[",f);
    if(p.clockTraceFrozen){const auto count=(std::min)(p.clockTraceCount,unsigned(p.clockTrace.size()));
        for(unsigned i=0;i<count;++i){const auto& e=p.clockTrace[(p.clockTraceCount-count+i)%p.clockTrace.size()];
            fprintf(f,"%s{\"now\":%.6f,\"poseTime\":%.6f,\"z\":%.8f,\"motion\":%u,\"airState\":%u,\"trajectory\":%s,\"eligible\":%s}",i?",":"",e.now,e.poseTime,e.z,e.motion,e.airState,e.trajectory?"true":"false",e.eligible?"true":"false");}}
    fputs("],\"roots\":[",f);
    for(unsigned i=0;i<p.clockRootCount;++i){const auto& e=p.clockRoots[i];fprintf(f,"%s{\"time\":%.6f,\"z\":%.8f,\"sequence\":%u,\"motion\":%u,\"valid\":%s}",i?",":"",e.time,e.z,e.sequence,e.motion,e.valid?"true":"false");}
    fputs("]},",f);
    fprintf(f,"\"clockRejection\":{\"code\":%u,\"count\":%u,\"dt\":%.8f,\"dz\":%.8f,\"rate\":%.8f,\"motion\":%u,\"previousAirState\":%u},",p.clock.rejectCode,p.clock.rejectCount,p.clock.rejectDt,p.clock.rejectDz,p.clock.rejectRate,p.clock.rejectMotion,p.clock.rejectAirState);
    fprintf(f,"\"eligibility\":{\"mask\":%u,\"failures\":%llu,\"recordCount\":%llu,\"records\":[",p.eligibilityMask,p.eligibilityFailures,p.eligibilityRecordCount);
    const auto eligibilityCount=(std::min)(p.eligibilityRecordCount,std::uint64_t(p.eligibilityRecords.size()));
    for(std::uint64_t i=0;i<eligibilityCount;++i){const auto& e=p.eligibilityRecords[(p.eligibilityRecordCount-eligibilityCount+i)%p.eligibilityRecords.size()];
        fprintf(f,"%s{\"now\":%.6f,\"poseTime\":%.6f,\"firstSemantic\":%.6f,\"lastSemantic\":%.6f,\"mask\":%u,\"state\":%u,\"motion\":%u,\"stance\":%u,\"semanticCount\":%u,\"stateAge\":%llu,\"motionValid\":%s,\"rootCount\":%u,\"rootResets\":%llu,\"firstRoot\":%.6f,\"lastRoot\":%.6f,\"renderTime\":%.6f}",i?",":"",e.now,e.poseTime,e.firstSemantic,e.lastSemantic,e.mask,e.state,e.motion,e.stance,e.semanticCount,e.stateAge,e.motionValid?"true":"false",e.rootCount,e.rootResets,e.firstRoot,e.lastRoot,e.renderTime);
    }
    fputs("]},",f);
    fprintf(f,"\"carryPlaybackStance\":%u,",p.carryStance);
    fprintf(f,"\"combatRecoveryStart\":%.7f,\"combatAuthoredAmount\":%.7f,",p.combatRecoveryStart,CombatPresentation::authoredAmount(p.combatVariant,p.combatAmount,p.combatRecoveryStart));
    fprintf(f,"\"stunVariant\":%u,\"stunAmount\":%.7f,\"stunWeight\":%.7f,\"stunEvent\":%llu,",p.stunVariant,p.stunAmount,p.stunWeight,p.stunEvent);
    fprintf(f,"\"useVariant\":%u,\"useAmount\":%.7f,\"useWeight\":%.7f,\"useEvent\":%llu,\"sourceUses\":%llu,\"sourceUseUnfocused\":%llu,\"sourceUseName\":\"%s\",\"sourceUseFrames\":%llu,\"sourceUseEdges\":%llu,\"sourceUseLastVariant\":%u,\"sourceUseEndReason\":%u,\"sourceUseEndAction\":%u,",p.useVariant,p.useAmount,p.useWeight,p.useEvent,shadowUse.uses,shadowUse.unfocused,shadowUse.name,shadowUse.frames,shadowUse.edges,shadowUse.lastVariant,shadowUse.endReason,shadowUse.endAction);
    fprintf(f,"\"grenadeHeld\":%u,\"grenadeVariant\":%u,\"grenadeAmount\":%.7f,\"grenadeWeight\":%.7f,\"grenadeEvent\":%llu,",p.grenadeHeld,p.grenadeVariant,p.grenadeAmount,p.grenadeWeight,p.grenadeEvent);
    fprintf(f,"\"heartHeld\":%u,\"heartVariant\":%u,\"heartAmount\":%.7f,\"heartWeight\":%.7f,\"heartEvent\":%llu,",p.heartHeld,p.heartVariant,p.heartAmount,p.heartWeight,p.heartEvent);
    fprintf(f,"\"trapHeld\":%u,\"trapVariant\":%u,\"trapAmount\":%.7f,\"trapWeight\":%.7f,\"trapEvent\":%llu,",p.trapHeld,p.trapVariant,p.trapAmount,p.trapWeight,p.trapEvent);
    fprintf(f,"\"chokeVariant\":%u,\"chokeAmount\":%.7f,\"chokeWeight\":%.7f,\"chokeEvent\":%llu,",p.chokeVariant,p.chokeAmount,p.chokeWeight,p.chokeEvent);
    fprintf(f,"\"climbWeight\":%.7f,\"climbPhase\":%.7f,\"climbDescend\":%.7f,\"climbVz\":%.7f,\"climbSamples\":%llu,",p.climbWeight,p.climbPhase,p.climbDescend,p.climbVz,p.climbSamples);
    fprintf(f,"\"swimWeight\":%.7f,\"swimMove\":%.7f,\"swimRise\":%.7f,\"swimSink\":%.7f,\"swimSpeed\":%.7f,\"swimVz\":%.7f,\"swimSamples\":%llu,",p.swimWeight,p.swimMove,p.swimRise,p.swimSink,p.swimSpeed,p.swimVz,p.swimSamples);
    fprintf(f,"\"trapStock\":%u,",p.trapStock);
    fprintf(f,"\"carrySampleAmount\":%.7f,\"carryAuthoredAmount\":%.7f,\"carrySettle\":%.7f,",p.carryAmount,(p.carryVariant==7||p.carryVariant==1)?carryPickupAmount(p.carryVariant,p.carryAmount):0.f,p.carrySettle);
    fprintf(f,"\"useTargetValid\":%s,\"useTarget\":[%.4f,%.4f,%.4f],\"useReachApplied\":%llu,\"useReachRefused\":%llu,\"useReachError\":%.4f,\"useReachWeight\":%.4f,",
        p.useTargetValid?"true":"false",p.useTarget[0],p.useTarget[1],p.useTarget[2],static_cast<unsigned long long>(p.useReachApplied),static_cast<unsigned long long>(p.useReachRefused),p.useReachError,p.useReachWeight);
    fprintf(f,"\"beginRefusal\":%u,\"beginRefusals\":[",movementBeginRefusal);
    for(unsigned i=0;i<movementBeginRefusals.size();++i)fprintf(f,"%s%llu",i?",":"",static_cast<unsigned long long>(movementBeginRefusals[i]));
    fputs("],",f);
    fprintf(f,"\"refuseStreak\":%u,\"refuseLongest\":%u,\"refuseLast\":%u,\"refuseReasons\":[",p.refusal.streak,p.refusal.longest,p.refusal.last);
    for(unsigned i=0;i<p.refusal.reasons.size();++i)fprintf(f,"%s%llu",i?",":"",static_cast<unsigned long long>(p.refusal.reasons[i]));
    fputs("],",f);
    fprintf(f,"\"experimental\":true,\"active\":%s,\"end\":%llu,\"joints\":%u,\"phase\":%.7f,\"speed\":%.6f,\"weight\":%.6f,\"a\":%u,\"b\":%u,\"mix\":%.6f,\"checksum\":%llu,\"applied\":%llu,\"refused\":%llu,\"gaits\":%u,\"runMix\":%.6f,\"version\":%u,\"stanceMix\":%.6f,\"sprintMix\":%.6f,\"sprintSide\":%u,\"sprintDirection\":%.6f,\"leanAmount\":%.6f,\"leanWeight\":%.6f,\"leanUp\":%.6f,\"slideState\":%u,\"slideTime\":%.6f,\"slideWeight\":%.6f,\"airState\":%u,\"airTime\":%.6f,\"airWeight\":%.6f,\"airProgress\":%.6f,\"airRun\":%.6f,\"airCrouch\":%.6f,\"combatVariant\":%u,\"combatPhase\":%u,\"combatClip\":%u,\"combatAmount\":%.6f,\"combatWeight\":%.6f,\"combatEvent\":%llu,\"equipmentVariant\":%u,\"equipmentAmount\":%.6f,\"equipmentWeight\":%.6f,\"armedMix\":%.6f,\"presentedDrawn\":%s,\"traversalVariant\":%u,\"traversalAmount\":%.6f,\"traversalWeight\":%.6f,\"traversalEvent\":%llu,\"powerVariant\":%u,\"powerAmount\":%.6f,\"powerWeight\":%.6f,\"powerEvent\":%llu,\"shadowWalkPhase\":%.6f,\"combatAirMix\":%.6f,\"combatMoveMix\":%.6f,\"nativeFeet\":%s,\"currentFeet\":%s,\"feetCasts\":%llu,\"feetSolves\":%llu,\"feetResetsAvoided\":%llu,\"feetReferences\":%llu,\"terrainWeight\":%.6f,\"terrainGrade\":%.6f,\"terrainDown\":%.6f,\"terrainRun\":%.6f,\"turnWeight\":%.6f,\"turnPhase\":%.6f,\"turnVelocity\":%.6f,\"turnClip\":%u,\"turnSerial\":%u,\"kneeCorrections\":%llu,\"kneeReject\":%u,\"kneeOffsetError\":%.8f,\"secondJump\":%s,\"secondJumpSerial\":%u,\"secondJumpProgress\":%.6f,\"secondJumpTransition\":%.6f,\"landingCrouchMix\":%.6f,\"rangedShot\":%u,\"rangedMechanism\":%u,\"rangedKind\":%u,\"rangedVariant\":%u,\"rangedAmount\":%.6f,\"rangedWeight\":%.6f,\"rangedEvent\":%llu,\"carryVariant\":%u,\"carryWeight\":%.6f,\"carryMove\":%.6f,\"carryPhase\":%.6f,\"carryCrouchPhase\":%.6f,\"carryEvent\":%llu}\n",p.end?"true":"false",p.end,p.bank.joints,p.clock.phase,p.clock.speed,p.clock.weight,p.clock.a,p.clock.b,p.clock.mix,p.bank.checksum,p.applied,p.refused,p.bank.gaits,p.clock.runMix,p.bank.version,p.clock.stanceMix,p.clock.sprintMix,p.clock.sprintSide,p.clock.sprintDirection,p.clock.leanAmount,p.clock.leanWeight,p.clock.leanUp,p.clock.slideState,p.clock.slideTime,p.clock.slideWeight,p.clock.airState,p.clock.airTime,p.clock.airWeight,p.clock.airProgress,p.clock.airRun,p.clock.airCrouch,p.combatVariant,p.combatPhase,p.combatClip,p.combatAmount,p.combatWeight,p.combatEvent,p.equipmentVariant,p.equipmentAmount,p.equipmentWeight,p.armedMix,p.presentedDrawn?"true":"false",p.traversalVariant,p.traversalAmount,p.traversalWeight,p.traversalEvent,p.powerVariant,p.powerAmount,p.powerWeight,p.powerEvent,p.shadowWalkPhase,p.combatAirMix,p.combatMoveMix,p.nativeFeet?"true":"false",p.currentFeet?"true":"false",movementFeetCasts.load(),movementFeetSolves.load(),movementFeetResetsAvoided.load(),movementFeetReferences.load(),p.clock.terrainWeight,p.clock.terrainGrade,p.clock.terrainDown,p.clock.terrainRun,p.clock.turnWeight,p.clock.turnPhase,p.clock.turnVelocity,p.clock.turnClip,p.clock.turnSerial,p.kneeCorrections,p.kneeReject,p.kneeOffsetError,p.secondJump.active?"true":"false",p.secondJump.serial,p.secondJump.progress,p.secondJump.transition,p.landingCrouchMix,p.rangedShot,p.rangedMechanism,p.rangedKind,p.rangedVariant,p.rangedAmount,p.rangedWeight,p.rangedEvent,p.carryVariant,p.carryWeight,p.carryMove,p.carryPhase[0],p.carryPhase[1],p.carryEvent);
    ReleaseSRWLockShared(&movementLock);const bool ok=!ferror(f);const int close=fclose(f);if(ok&&!close)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
