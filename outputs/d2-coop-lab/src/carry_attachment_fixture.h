#pragma once
#include "native_retargeting.h"
#include "carry_victim_scene.h"
#include "carry_snapshot_blend.h"
// Explicit, non-renewable Dunwall mechanics experiment. Native victim-only
// Midnight playback is experimental; pose / physics handoff remain unaccepted.
struct CarryAttachmentFixture {
    NativeRetargeting before{},held{};NativeWorldNpc target{};
    CarryVictimScene scene{};
    CarrySnapshotBlend snapshot{};
    std::uintptr_t physics=0;bool physicsOwned=false,physicsRestoring=false;
    std::array<unsigned,4> settings{};
    std::uint64_t epoch=0,peerEpoch=0,peerSession=0;ULONGLONG until=0,startedAt=0;
    unsigned sourceIncarnation=0;bool restoring=false,carriedFlag=false,compactReleaseFlag=false;
    bool anchorCalibrated=false,attachmentPending=false;
    bool sourcePickup=false;float pickupBlend=0;
    ULONGLONG lastHeldAt=0,dropUntil=0,attachedAt=0;
    unsigned dropStage=0,dropVariant=0;bool crouchedPickup=false,releasePhysicsRequested=false;
    std::uint64_t throwEvent=0;std::array<float,3> throwVelocity{};bool throwRagdollRequested=false;
    float dropRoot[12]{};
    bool product=false;unsigned victimState=0;ULONGLONG lostAt=0;
} carryAttachmentFixture;
// 1214dm (#11 carry_body_lift_idle_move_drop_throw): product mode. When the partner picks up or carries a body in an
// armed product session, the observer attaches its own copy of that body (resolved by the streamed spawner/definition
// and the local handle) to the presented partner with this fixture's native machinery: any map, any human NPC, dead or
// unconscious, a rolling 3 s lease renewed while the partner's carry samples arrive, ending on the native drop, compact
// drop or throw, a sample gap or peer loss. The NPC stream leaves the body alone meanwhile (npcCarrySuppression).
bool carryAttachmentProductRequested=false;std::uint64_t carryProductStarts=0,carryProductRefused=0,carryProductLost=0;
unsigned carryProductAttempts=0,carryProductIncarnation=0;ULONGLONG carryProductLastTry=0;const char* carryProductResult="idle";
unsigned carryAttachmentRequest=0;
bool carryAttachmentSnapshotRequested=false;
bool carryAttachmentPickupRequested=false;
unsigned carryAttachmentDuration=8000;
unsigned carryAttachmentRefusalStage=0;
ULONGLONG carryAttachmentRequestScopeMs=0,carryAttachmentRequestCatalogAge=0;
std::uint64_t carryAttachmentStarted=0,carryAttachmentReleased=0,carryAttachmentRetired=0,carryAttachmentRefused=0;
std::uint64_t carryThrowQueued=0,carryThrowRefused=0;std::array<float,3> carryLastThrowVelocity{};
const char* carryAttachmentResult="idle";
// 1214dq: the first reason an attempt turned to restoring, and the fixture result at that moment. 1277u: the host
// product attempt reached waiting_for_native_victim_scene_stop in the tick its snapshot started (pickup at 14%), so the
// per-tick result log could not name the cause. Bits 0x10000 | peer mask (1 stop request, 2 lease, 4 stream not
// current, 8 peer not ready, 16 no state within 200 ms, 32 scope, 64 bank, 128 epoch, 256 session); 0x40000 the
// Carry45 to Drop46 gap; 0x200000 the drop handoff; 0x800000 any other cause, named by the saved result.
unsigned carryRestoreCause=0;const char* carryRestoreResult="";ULONGLONG carryRestoreTick=0;
bool carryAttachmentApi(){
    constexpr unsigned char setup[]={0x48,0x8b,0xc4,0x55,0x41,0x55,0x41,0x56,0x48,0x8d,0x68,0xb8,0x48,0x81,0xec,0x30,0x01,0,0};
    constexpr unsigned char start[]={0x4c,0x8b,0xdc,0x55,0x56,0x41,0x56,0x48,0x81,0xec,0xb0,0,0,0};
    constexpr unsigned char stop[]={0x40,0x53,0x57,0x41,0x56,0x48,0x83,0xec,0x30,0x45,0x33,0xf6};
    constexpr unsigned char animated[]={0x40,0x53,0x48,0x83,0xec,0x20,0x80,0xa1,0x40,2,0,0,0xf8,0x48,0x8b,0xd9};
    constexpr unsigned char ragdoll[]={0x40,0x57,0x48,0x83,0xec,0x20,0xf6,0x81,0x41,2,0,0,8,0x48,0x8b,0xf9};
    constexpr unsigned char poseRagdoll[]={0x48,0x89,0x5c,0x24,0x10,0x48,0x89,0x6c,0x24,0x18,0x48,0x89,0x74,0x24,0x20,0x57,0x41,0x54,0x41,0x55,0x41,0x56,0x41,0x57};
    return gameImage&&!std::memcmp(gameImage+0x755870,setup,sizeof(setup))&&
        !std::memcmp(gameImage+0x757830,start,sizeof(start))&&!std::memcmp(gameImage+0x757550,stop,sizeof(stop))&&
        !std::memcmp(gameImage+0x736ef0,animated,sizeof(animated))&&!std::memcmp(gameImage+0x734a10,ragdoll,sizeof(ragdoll))&&
        !std::memcmp(gameImage+0x734d90,poseRagdoll,sizeof(poseRagdoll));
}
bool carryAttachmentPhysicsOwned(const CarryAttachmentFixture& f){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t list=0,owner=0,vt=0;unsigned flags=0,matches=0;
    if(!f.physics||worldIdentityEntity(memoryRead,observedGame,f.target.handle)!=f.target.actor||
       !memoryRead(f.target.actor+0x268,&list,8)||!list||!memoryRead(f.target.actor+0x270,&flags,4)||(flags&0xffffff)>256||
       !memoryRead(f.physics,&vt,8)||vt!=base+0x1c54008||!memoryRead(f.physics+0x10,&owner,8)||owner!=f.target.actor)return false;
    for(unsigned i=0;i<(flags&0xffffff);++i){std::uintptr_t p=0;if(!memoryRead(list+i*8,&p,8))return false;if(p==f.physics)++matches;}
    return matches==1;
}
bool carryAttachmentThrowReady(){
    AcquireSRWLockShared(&movementLock);const bool ready=movementPlayback.bank.hasCarryThrow();ReleaseSRWLockShared(&movementLock);return ready;
}
bool carryAttachmentFetalReady(){
    AcquireSRWLockShared(&movementLock);const bool ready=movementPlayback.bank.hasCarryFetalRelease();ReleaseSRWLockShared(&movementLock);return ready;
}
bool queueCarryThrowImpulse(const CarryAttachmentFixture& f){
    // 832290 queues event149 (28 bytes): camera-forward * decl+a0,
    // zero angular vector and bytes24/25=1. Handler727100 accumulates it
    // and wakes the owned rigid bodies. Our victim asset has no event tracks.
    constexpr unsigned char managerBytes[]={0x48,0x8b,5,0x49,0x50,0x19,3,0x48,5,0x68,0x89,0x1e,0,0xc3};
    constexpr unsigned char queueBytes[]={0x48,0x89,0x5c,0x24,8,0x48,0x89,0x74,0x24,0x10,0x44,0x89,0x4c,0x24,0x20,0x57};
    unsigned mode=99,pending=0;float length=0;
    for(float v:f.throwVelocity){if(!std::isfinite(v))return false;length+=v*v;}
    if(f.dropVariant!=8||!f.throwEvent||length<15.84f||length>16.16f||GetCurrentThreadId()!=engineThread.load()||
       !carryAttachmentPhysicsOwned(f)||!memoryRead(f.physics+0x238,&mode,4)||mode!=0||
       !memoryRead(f.physics+0x23c,&pending,4)||pending!=0xffffffff||
       std::memcmp(gameImage+0x810240,managerBytes,sizeof(managerBytes))||std::memcmp(gameImage+0x8112c0,queueBytes,sizeof(queueBytes)))return false;
    auto* manager=reinterpret_cast<unsigned char*(*)()>(gameImage+0x810240)();
    if(manager!=reinterpret_cast<unsigned char*>(observedGame+0x1e8968))return false;
    auto* payload=reinterpret_cast<unsigned char*(*)(void*,unsigned short,int,unsigned)>(gameImage+0x8112c0)(manager+0xc8e8,0x149,28,f.target.handle);
    if(!payload)return false;
    std::memset(payload,0,28);std::memcpy(payload,f.throwVelocity.data(),12);payload[24]=payload[25]=1;
    carryLastThrowVelocity=f.throwVelocity;++carryThrowQueued;return true;
}
bool carryAttachmentJoint(const NativeSkeleton& s,const char* wanted,std::uint16_t& id){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t manager=0,vt=0,entries=0;unsigned count=0,matches=0;
    if(!memoryRead(base+0x22995c0,&manager,8)||!manager||!memoryRead(manager,&vt,8)||vt!=base+0x1b093c8||
       !memoryRead(manager+8,&entries,8)||!entries||!memoryRead(manager+0x10,&count,4)||(count&0xffffff)>65535)return false;
    for(auto candidate:s.jointIds){std::uintptr_t name=0;char text[96]{};
        if(candidate>=(count&0xffffff)||!memoryRead(entries+candidate*72+16,&name,8)||!nativeName(name,text,sizeof(text)))return false;
        if(!std::strcmp(text,wanted)){id=candidate;++matches;}
    }
    return matches==1;
}
bool carryAttachmentScope(bool product){return product?productSessionArmed():(observerIsolation.ready&&observerIsolation.movementFixture);}
bool carryAttachmentPeerHeld(ULONGLONG tick,bool product=false){
    return shadowStreamCurrent(tick)&&shadowStream.peerReady&&shadowStream.lastState&&tick-shadowStream.lastState<200&&
        shadowStream.remoteState==14&&(shadowStream.remoteCarry==2||shadowStream.remoteCarry==3)&&
        carryAttachmentScope(product)&&movementBankActive();
}
bool carryAttachmentPeerCurrent(ULONGLONG tick,bool product=false){
    return shadowStreamCurrent(tick)&&shadowStream.peerReady&&shadowStream.lastState&&tick-shadowStream.lastState<200&&
        carryAttachmentScope(product)&&movementBankActive();
}
bool carryAttachmentSource(const CarryAttachmentFixture& f,CarryPresentation::Sample& sample,float& amount){
    const double at=presentationClock()-(poseSmoothingEnabled?PoseInterpolation::delayMs:0.);
    return shadowStream.carry.sample(at,sample,amount)&&sample.incarnation==f.sourceIncarnation&&
        sample.spawner==carryIdentityKey(f.target.spawnerName)&&sample.definition==carryIdentityKey(f.target.definition);
}
bool carryAttachmentPeerPickup(ULONGLONG tick,unsigned* stance=nullptr,bool product=false){
    if(!carryAttachmentPeerCurrent(tick,product)||shadowStream.remoteState!=14)return false;
    AcquireSRWLockShared(&movementLock);
    const bool carrier=movementPlayback.end>tick&&movementPlayback.lease>tick&&movementPlayback.bank.hasCarryPickup();
    const bool crouched=movementPlayback.bank.hasCrouchedCarryPickup();
    ReleaseSRWLockShared(&movementLock);
    CarryPresentation::Sample sample{};float amount=0;
    const double at=presentationClock()-(poseSmoothingEnabled?PoseInterpolation::delayMs:0.);
    if(!carrier||!shadowStream.carry.sample(at,sample,amount))return false;
    if(stance)*stance=sample.stance;
    return (sample.stance==0||(crouched&&sample.stance==1))&&
        (sample.variant==7||(sample.variant==1&&amount<.15f))&&
        sample.incarnation==shadowStream.remoteCarryIncarnation&&sample.spawner==shadowStream.remoteCarrySpawner&&sample.definition==shadowStream.remoteCarryDefinition;
}
bool prepareCarrySnapshotPhysics(CarryAttachmentFixture& f){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    std::uintptr_t list=0;unsigned flags=0,mode=99,pending=0;
    if(!memoryRead(f.target.actor+0x268,&list,8)||!list||
       !memoryRead(f.target.actor+0x270,&flags,4)||(flags&0xffffff)>256)return false;
    for(unsigned i=0;i<(flags&0xffffff);++i){std::uintptr_t p=0,vt=0;
        if(!memoryRead(list+i*8,&p,8)||!p||!memoryRead(p,&vt,8))return false;
        if(vt==base+0x1c54008){if(f.physics)return false;f.physics=p;}
    }
    if(!carryAttachmentPhysicsOwned(f)||!memoryRead(f.physics+0x238,&mode,4)||(mode!=0&&!(f.product&&mode==3))||
       !memoryRead(f.physics+0x23c,&pending,4)||pending!=0xffffffff)return false;
    if(mode==0)reinterpret_cast<void(*)(void*)>(gameImage+0x736ef0)(reinterpret_cast<void*>(f.physics));
    f.physicsOwned=true;f.attachmentPending=true;return true;
}
bool finishCarrySnapshotAttachment(CarryAttachmentFixture& f,const NativeRetargeting& current){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    NativeSkeleton carrier{},victim{};std::uintptr_t frame=0,owner=0;std::uint64_t emptyName=0;
    std::uint16_t ownerJoint=0xffff,targetJoint=0xffff;unsigned mode=99,pending=0;
    if(current.count||f.snapshot.bound||!carryAttachmentPhysicsOwned(f)||
       !memoryRead(f.physics+0x238,&mode,4)||mode!=3||!memoryRead(f.physics+0x23c,&pending,4)||pending!=0xffffffff||
       !memoryRead(f.target.actor+0x358,&frame,8)||!frame||!memoryRead(frame+0x3f0,&owner,8)||owner||
       !readNativeSkeleton(memoryRead,base,f.before.handle,carrier)||!readNativeSkeleton(memoryRead,base,f.target.handle,victim)||
       !carrier.stable||!victim.stable||carrier.actor!=f.before.actor||victim.actor!=f.target.actor||
       !carryAttachmentJoint(carrier,"center_spine03_jnt",ownerJoint)||!carryAttachmentJoint(victim,"anchor_jnt",targetJoint)||
       !memoryRead(base+0x2293f48,&emptyName,8))return false;
    const auto oi=std::find(carrier.jointIds.begin(),carrier.jointIds.end(),ownerJoint)-carrier.jointIds.begin();
    const auto ti=std::find(victim.jointIds.begin(),victim.jointIds.end(),targetJoint)-victim.jointIds.begin();
    BodyMatrix shoulder{},anchor{};
    std::memcpy(shoulder.v,carrier.globalPose[oi].data(),64);std::memcpy(anchor.v,victim.globalPose[ti].data(),64);
    const auto delta=bodyMultiply(bodyInverse(carryRootMatrix(carrier.root)),carryRootMatrix(victim.root));
    const auto relative=bodyMultiply(bodyInverse(shoulder),bodyMultiply(delta,anchor));
    if(!bodyRigid(relative,4.f))return false;
    float rotation[9]{},offset[7]{};for(unsigned c=0;c<3;++c)for(unsigned r=0;r<3;++r)rotation[c*3+r]=relative.v[c*4+r];
    presentationQuaternion(rotation,offset+3);
    for(unsigned k=0;k<3;++k){offset[k]=relative.v[12+k];offset[k+3]=-offset[k+3];}
    // Delay both setup and activation: the native solver reads its clip anchor,
    // while globalPose still contains the recovery blend. Combining those two
    // before the blend finishes displaced the body below the floor in v18.
    alignas(16) std::array<unsigned char,48> params{};
    for(unsigned i=0;i<3;++i)std::memcpy(params.data()+i*8,&emptyName,8);
    const float sentinel=1e30f;std::memcpy(params.data()+0x18,&sentinel,4);std::memcpy(params.data()+0x1c,&sentinel,4);
    reinterpret_cast<void(*)(void*,void*,void*)>(gameImage+0x755870)(reinterpret_cast<void*>(current.component),reinterpret_cast<void*>(f.target.actor),params.data());
    std::uintptr_t record=0;unsigned count=0;
    if(!memoryRead(current.component+0x20,&record,8)||!record||!memoryRead(current.component+0x28,&count,4)||(count&0xffffff)!=1){
        f.restoring=true;carryAttachmentResult="native_setup_failed";return false;
    }
    const float identity[7]={0,0,0,0,0,0,1};
    std::memcpy(reinterpret_cast<void*>(record+0x50),identity,28);std::memcpy(reinterpret_cast<void*>(record+0x70),offset,28);
    std::memcpy(reinterpret_cast<void*>(record+0x6c),&targetJoint,2);std::memcpy(reinterpret_cast<void*>(record+0x8c),&ownerJoint,2);
    const unsigned settings[4]={0,0,7,(f.settings[3]&0xff000000)|0x10100};
    std::memcpy(reinterpret_cast<void*>(current.component+0x34),settings,16);
    reinterpret_cast<void(*)(void*)>(gameImage+0x757830)(reinterpret_cast<void*>(current.component));
    ++carryAttachmentStarted;f.attachmentPending=false;f.attachedAt=GetTickCount64();
    if(!readNativeRetargeting(memoryRead,base,f.before.handle,f.held)||f.held.target!=f.target.actor||f.held.targetPhysics!=f.physics){
        f.restoring=true;carryAttachmentResult="native_start_unconfirmed";return false;
    }
    carryAttachmentResult="native_snapshot_attachment_started";return true;
}
bool calibrateCarryAnchor(CarryAttachmentFixture& f,const NativeRetargeting& attachment,float contactWeight=1.f){
    NativeSkeleton carrier{},victim{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(!readNativeSkeleton(memoryRead,base,f.before.handle,carrier)||!readNativeSkeleton(memoryRead,base,f.target.handle,victim)||
       !carrier.stable||!victim.stable||carrier.actor!=f.before.actor||victim.actor!=f.target.actor)return false;
    const auto owner=std::find(carrier.jointIds.begin(),carrier.jointIds.end(),attachment.ownerJoint);
    const auto target=std::find(victim.jointIds.begin(),victim.jointIds.end(),attachment.targetJoint);
    if(owner==carrier.jointIds.end()||target==victim.jointIds.end())return false;
    BodyMatrix shoulder{},anchor{};
    std::memcpy(shoulder.v,carrier.globalPose[owner-carrier.jointIds.begin()].data(),64);
    std::memcpy(anchor.v,victim.globalPose[target-victim.jointIds.begin()].data(),64);
    // Wait for the native folded-body clip, not the preceding ragdoll or bind
    // pose. Both transforms are model-space, relative to coincident roots.
    if(!bodyRigid(shoulder)||!bodyRigid(anchor)||(!f.snapshot.captured&&(anchor.v[14]<1.f||anchor.v[14]>1.6f)))return false;
    const auto nativeAnchor=anchor;
    const auto delta=bodyMultiply(bodyInverse(carryRootMatrix(carrier.root)),carryRootMatrix(victim.root));
    if(f.sourcePickup){
        // Only position needs the lowered victim frame. Feeding the victim's
        // current world rotation back into the native attachment offset made
        // it accumulate yaw (v28: +63 degrees while carrier heading stayed
        // fixed). Retain the native clip's orientation, as the standing path
        // does, while transforming the contact positions into carrier space.
        const auto localAnchor=anchor;anchor=bodyMultiply(delta,anchor);
        for(unsigned c=0;c<3;++c)for(unsigned r=0;r<3;++r)anchor.v[c*4+r]=localAnchor.v[c*4+r];
    }
    BodyMatrix desired=anchor;
    if(!f.anchorCalibrated||f.sourcePickup){
        // Match the authored carry-body contact to the actual victim skeleton:
        // abdomen (spine02) over the loaded left shoulder, not coincident actor
        // origins. This reuses author_carry_body.py's clearance in model space.
        std::uint16_t armId=0xffff,abdomenId=0xffff;
        if(!carryAttachmentJoint(carrier,"left_arm_jnt",armId)||
           !carryAttachmentJoint(victim,"center_spine02_jnt",abdomenId))return false;
        const auto arm=std::find(carrier.jointIds.begin(),carrier.jointIds.end(),armId);
        const auto abdomen=std::find(victim.jointIds.begin(),victim.jointIds.end(),abdomenId);
        if(arm==carrier.jointIds.end()||abdomen==victim.jointIds.end())return false;
        const auto& a=carrier.globalPose[arm-carrier.jointIds.begin()];
        BodyMatrix abdomenPose{};std::memcpy(abdomenPose.v,victim.globalPose[abdomen-victim.jointIds.begin()].data(),64);
        const auto* b=abdomenPose.v;
        constexpr float clearance[3]={.025f,.19f,.115f};
        for(unsigned k=0;k<3;++k){
            if(f.sourcePickup){
                // Match the physical load to the current shoulder each frame.
                // A frozen shoulder-space point changes the abdomen's height
                // when the spine pitches during standing/crouching. Use the
                // native anchor-to-abdomen vector, not previous actor roots.
                const float goal=nativeAnchor.v[12+k]+a[12+k]+clearance[k]-b[12+k];
                desired.v[12+k]+=(goal-desired.v[12+k])*(f.anchorCalibrated?1.f:contactWeight);
            }else desired.v[12+k]+=(a[12+k]+clearance[k]-b[12+k])*contactWeight;
        }
    }
    auto relative=bodyMultiply(bodyInverse(shoulder),desired);
    // Keep the established contact point in shoulder space, but cancel its
    // changing pitch/roll in the orientation offset. Native NPC root physics
    // retains yaw; feeding it a tilted target rotation creates a false lift.
    if(f.anchorCalibrated&&!f.sourcePickup)for(unsigned k=0;k<3;++k)relative.v[12+k]=attachment.ownerOffset[k];
    if(!bodyRigid(relative,2.f))return false;
    float rotation[9]{},offset[7]{};
    for(unsigned c=0;c<3;++c)for(unsigned r=0;r<3;++r)rotation[c*3+r]=relative.v[c*4+r];
    presentationQuaternion(rotation,offset+3);
    // Native idQuat (110080/111410) is conjugate to skeleton QTS.
    for(unsigned k=0;k<3;++k){offset[k]=relative.v[12+k];offset[3+k]=-offset[3+k];}
    std::array<float,12> check={-offset[3],-offset[4],-offset[5],offset[6],offset[0],offset[1],offset[2],0,1,1,1,1};
    BodyMatrix reconstructed{};if(!bodyFromQts(check,reconstructed))return false;
    for(unsigned c=0;c<4;++c)for(unsigned r=0;r<3;++r)if(std::abs(reconstructed.v[c*4+r]-relative.v[c*4+r])>.001f)return false;
    std::memcpy(reinterpret_cast<void*>(attachment.records+0x70),offset,sizeof(offset));
    f.anchorCalibrated=contactWeight>=1.f;return true;
}
bool rebaseCarryThrowAnchor(const CarryAttachmentFixture& f,const NativeRetargeting& attachment){
    // Native Carry45->Drop46 (829e30->829180) retains retargeting and
    // changes its reference from spine03 to spine02. Preserve our current
    // model-space attachment frame when changing that reference. This is
    // an isolated analogue, not reuse of the player's first-person anchors.
    NativeSkeleton carrier{};std::uint16_t nextJoint=0xffff;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(attachment.count!=1||attachment.target!=f.target.actor||attachment.targetPhysics!=f.physics||
       !readNativeSkeleton(memoryRead,base,f.before.handle,carrier)||!carrier.stable||carrier.actor!=f.before.actor||
       !carryAttachmentJoint(carrier,"center_spine02_jnt",nextJoint))return false;
    const auto old=std::find(carrier.jointIds.begin(),carrier.jointIds.end(),attachment.ownerJoint);
    const auto next=std::find(carrier.jointIds.begin(),carrier.jointIds.end(),nextJoint);
    if(old==carrier.jointIds.end()||next==carrier.jointIds.end())return false;
    BodyMatrix oldPose{},newPose{},offset{};
    std::memcpy(oldPose.v,carrier.globalPose[old-carrier.jointIds.begin()].data(),64);
    std::memcpy(newPose.v,carrier.globalPose[next-carrier.jointIds.begin()].data(),64);
    const auto& o=attachment.ownerOffset;
    const std::array<float,12> q={-o[3],-o[4],-o[5],o[6],o[0],o[1],o[2],0,1,1,1,1};
    if(!bodyRigid(oldPose)||!bodyRigid(newPose)||!bodyFromQts(q,offset))return false;
    const auto desired=bodyMultiply(oldPose,offset);
    const auto rebased=bodyMultiply(bodyInverse(newPose),desired);
    if(!bodyRigid(rebased,2.f))return false;
    float rotation[9]{},replacement[7]{};
    for(unsigned c=0;c<3;++c)for(unsigned r=0;r<3;++r)rotation[c*3+r]=rebased.v[c*4+r];
    presentationQuaternion(rotation,replacement+3);
    for(unsigned k=0;k<3;++k){replacement[k]=rebased.v[12+k];replacement[k+3]=-replacement[k+3];}
    std::memcpy(reinterpret_cast<void*>(attachment.records+0x70),replacement,28);
    std::memcpy(reinterpret_cast<void*>(attachment.records+0x8c),&nextJoint,2);
    return true;
}
void serviceCarryAttachmentFixture(ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);const auto request=carryAttachmentRequest;carryAttachmentRequest=0;
    const bool product=request==1&&carryAttachmentProductRequested;if(request==1)carryAttachmentProductRequested=false;
    auto& f=carryAttachmentFixture;
    if(request==1){
        carryAttachmentRequestScopeMs=observerIsolation.until>tick?observerIsolation.until-tick:0;
        carryAttachmentRequestCatalogAge=tick>=worldCatalogTick?tick-worldCatalogTick:0;
        if(f.until){++carryAttachmentRefused;carryAttachmentResult="duplicate_refused";}
        else{
            NativeRetargeting before{};NativeWorldNpc target{};NativeHealth health{};NativeSkeleton carrier{},victim{};
            std::uintptr_t frame=0,owner=0;std::uint16_t ownerJoint=0xffff,targetJoint=0xffff;std::uint64_t emptyName=0;
            unsigned char carryFlags=0;unsigned pickupStance=0;
            carryAttachmentRefusalStage=1;
            bool valid=carryAttachmentApi()&&guardNativeApiMatches()&&(product||!std::strcmp(mapName,kGuardMap))&&probeDoppelganger&&
                (carryAttachmentPickupRequested?carryAttachmentPeerPickup(tick,&pickupStance,product):carryAttachmentPeerHeld(tick,product))&&
                (carryAttachmentRefusalStage=2,true)&&
                (product||((carryAttachmentDuration==8000||carryAttachmentDuration==16000)&&
                observerIsolation.until>tick+carryAttachmentDuration+2000))&&worldCatalogTick&&tick-worldCatalogTick<750&&
                (carryAttachmentRefusalStage=3,true)&&resolveCarryIdentity(memoryRead,base,observedGame,worldCatalog,shadowStream.remoteCarrySpawner,shadowStream.remoteCarryDefinition,target)&&
                (product?[&]{std::uintptr_t v=0;return memoryRead(target.actor,&v,8)&&v==base+0x1c5e108;}():
                 (target.spawnerName=="ld_spawner_alexi_escape"&&target.definition=="models/characters/def/story/alexi_mayhew.def"))&&
                target.handle==shadowStream.remoteCarryLocalHandle&&readNativeHealth(memoryRead,base,target.handle,health)&&
                (product?(health.logic==1&&((health.current<=0&&health.maximum>0)||((health.ownerFlags&0x20)&&health.current>0))):health.current==0)&&
                (carryAttachmentRefusalStage=4,true)&&readNativeRetargeting(memoryRead,base,probeHandle,before)&&!before.count&&before.actor==probeActor&&
                readNativeSkeleton(memoryRead,base,probeHandle,carrier)&&readNativeSkeleton(memoryRead,base,target.handle,victim)&&
                (carryAttachmentRefusalStage=51,true)&&carryAttachmentJoint(carrier,"center_spine03_jnt",ownerJoint)&&
                (carryAttachmentRefusalStage=52,true)&&carryAttachmentJoint(victim,"anchor_jnt",targetJoint)&&
                (carryAttachmentRefusalStage=53,true)&&memoryRead(target.actor+0x358,&frame,8)&&frame&&memoryRead(frame+0x3f0,&owner,8)&&!owner&&
                (carryAttachmentRefusalStage=54,true)&&memoryRead(target.actor+0x368,&carryFlags,1)&&!(carryFlags&6)&&
                (carryAttachmentRefusalStage=55,true)&&memoryRead(base+0x2293f48,&emptyName,8);
            if(valid)carryAttachmentRefusalStage=6;
            // A knockout destroys the AI (npc_authority_stream.h), so in product an unreadable AI component counts as inactive.
            for(auto kind:{NativeAiKind::Decision,NativeAiKind::Reflex,NativeAiKind::Attack})if(valid){NativeAiComponentState ai{};
                const bool readable=readNativeAiComponent(memoryRead,base,observedGame,target.handle,kind,ai);
                valid=readable?(!ai.schedulerActive&&!(ai.flags&0xc)):product;}
            std::array<unsigned,4> original{};
            if(valid){carryAttachmentRefusalStage=7;valid=memoryRead(before.component+0x34,original.data(),16);}
            if(!valid){++carryAttachmentRefused;carryAttachmentResult="unsupported_or_claimed_target";
                if(product){++carryProductRefused;carryProductResult="refused";npcCarryAttachRelease(tick,0);}}
            else{
                carryAttachmentRefusalStage=0;
                f.before=before;f.target=target;f.settings=original;f.epoch=outgoing.worldEpoch;f.peerEpoch=remote.worldEpoch;
                f.peerSession=remote.session;f.sourceIncarnation=shadowStream.remoteCarryIncarnation;f.startedAt=f.lastHeldAt=tick;carryRestoreCause=0;carryRestoreResult="";carryRestoreTick=0;
                f.product=product;f.until=tick+(product?3000:carryAttachmentDuration);f.victimState=health.current>0?2:1;if(product){++carryProductStarts;carryProductResult="started";}
                f.sourcePickup=carryAttachmentPickupRequested;f.crouchedPickup=f.sourcePickup&&pickupStance==1;
                // Exact-build native Carry45 crouched victim root is 0.674m
                // lower in both archived role references. Bounded Mayhew
                // hypothesis: use the same root for capture and placement,
                // preserving the original floor pose at blend weight zero.
                auto victimFrame=carrier;if(f.crouchedPickup)victimFrame.root[2]-=.674f;
                if(carryAttachmentSnapshotRequested&&!startCarrySnapshot(f.snapshot,victim,victimFrame)){
                    f.restoring=true;carryAttachmentResult="native_snapshot_refused";
                }else{
                registerCarryDamageLease(target.actor,target.handle,f.until+2000,product?target.definition.c_str():nullptr);
                // Native Carry45 sets NPC+368 bit1 at 828845 and clears it
                // on cleanup (829a10). Physics tick 736ccb otherwise returns
                // dead NPCs to ragdoll even while a Midnight clip is active.
                *reinterpret_cast<unsigned char*>(target.actor+0x368)|=2;f.carriedFlag=true;
                if(!startCarryVictimScene(f.scene,target)){f.restoring=true;carryAttachmentResult="native_victim_scene_refused";}
                else{
                // Native generic_carrycorpse_idle raises anchor_jnt about
                // 1.34m above its actor root. Start at the carrier root;
                // placing the root on the shoulder double-counts that lift.
                // Joint orientation / crouch alignment remain experimental.
                float anchor[3]={victimFrame.root[0],victimFrame.root[1],victimFrame.root[2]};
                reinterpret_cast<void(*)(void*,const float*,const float*,bool,int)>(gameImage+0x87d200)(reinterpret_cast<void*>(target.actor),anchor,carrier.root+3,true,0);
                if(f.snapshot.captured){
                    if(!prepareCarrySnapshotPhysics(f)){f.restoring=true;carryAttachmentResult="native_snapshot_physics_refused";}
                    else carryAttachmentResult="native_snapshot_blending_unattached";
                }else{
                alignas(16) std::array<unsigned char,48> params{};
                for(unsigned i=0;i<3;++i)std::memcpy(params.data()+i*8,&emptyName,8);
                const float sentinel=1e30f;std::memcpy(params.data()+0x18,&sentinel,4);std::memcpy(params.data()+0x1c,&sentinel,4);
                reinterpret_cast<void(*)(void*,void*,void*)>(gameImage+0x755870)(reinterpret_cast<void*>(before.component),reinterpret_cast<void*>(target.actor),params.data());
                std::uintptr_t record=0;unsigned count=0;
                if(!memoryRead(before.component+0x20,&record,8)||!record||!memoryRead(before.component+0x28,&count,4)||(count&0xffffff)!=1){f.restoring=true;carryAttachmentResult="native_setup_failed";}
                else{
                    // These are process-local interned joint IDs, not indices.
                    const float identity[7]={0,0,0,0,0,0,1};
                    std::memcpy(reinterpret_cast<void*>(record+0x50),identity,28);std::memcpy(reinterpret_cast<void*>(record+0x70),identity,28);
                    std::memcpy(reinterpret_cast<void*>(record+0x6c),&targetJoint,2);std::memcpy(reinterpret_cast<void*>(record+0x8c),&ownerJoint,2);
                    // 7570e0 otherwise selects the target-space branch when
                    // the NPC owner's PhysicsSystem is in mode0/2. The player
                    // carry owner lacks that component. +42 forces owner-space
                    // calculation; isolated experiment, not a release default.
                    const unsigned settings[4]={0,0,7,(original[3]&0xff000000)|0x10100};
                    std::memcpy(reinterpret_cast<void*>(before.component+0x34),settings,16);
                    reinterpret_cast<void(*)(void*)>(gameImage+0x757830)(reinterpret_cast<void*>(before.component));
                    ++carryAttachmentStarted;
                    if(!readNativeRetargeting(memoryRead,base,probeHandle,f.held)||f.held.target!=target.actor){f.restoring=true;carryAttachmentResult="native_start_unconfirmed";}
                    else{
                        f.physics=f.held.targetPhysics;unsigned mode=99,pending=0;
                        if(!carryAttachmentPhysicsOwned(f)||!memoryRead(f.physics+0x238,&mode,4)||mode!=0||
                           !memoryRead(f.physics+0x23c,&pending,4)||pending!=0xffffffff){f.restoring=true;carryAttachmentResult="native_physics_transition_refused";}
                        else{
                            // Exact PhysicsSystem callback registered for the
                            // native Carry45 pickup event 0xba (732ecc/8288c7).
                            reinterpret_cast<void(*)(void*)>(gameImage+0x736ef0)(reinterpret_cast<void*>(f.physics));
                            f.physicsOwned=true;carryAttachmentResult="native_animated_corpse_requested";
                        }
                    }
                }
                }
                }
                }
            }
        }
    }
    if(!f.until)return;
    if(f.epoch!=outgoing.worldEpoch||f.before.game!=observedGame){retireCarryDamageLease();if(f.product)npcCarryAttachRelease(tick,300);f={};++carryAttachmentRetired;carryAttachmentResult="old_world_retired";return;}
    {const unsigned peerMask=(request==2?1u:0u)|(tick>=f.until?2u:0u)|(!shadowStreamCurrent(tick)?4u:0u)|(!shadowStream.peerReady?8u:0u)|
        (!shadowStream.lastState||tick-shadowStream.lastState>=200?16u:0u)|(!carryAttachmentScope(f.product)?32u:0u)|(!movementBankActive()?64u:0u)|
        (f.peerEpoch!=remote.worldEpoch?128u:0u)|(f.peerSession!=remote.session?256u:0u);
     if(peerMask&&!f.restoring&&!carryRestoreCause){carryRestoreCause=0x10000u|peerMask;carryRestoreResult=carryAttachmentResult;carryRestoreTick=tick;}
     if(peerMask)f.restoring=true;}
    CarryPresentation::Sample source{};float amount=0;
    const bool sourceOwned=carryAttachmentSource(f,source,amount);
    // Product: a rolling lease while the partner's carry samples arrive (the drop and throw stages included).
    if(f.product&&sourceOwned&&!f.restoring){f.until=tick+3000;renewCarryDamageLease(f.target.actor,f.target.handle,f.until+2000);}
    float pickupAmount=0;const bool pickupOwned=f.sourcePickup&&sourceOwned&&carryVictimPickupAmount(source.variant,source.stance,amount,pickupAmount,f.crouchedPickup);
    if(pickupOwned)f.pickupBlend=(std::max)(f.pickupBlend,pickupAmount);
    if(!f.restoring&&!(f.sourcePickup?tickCarrySnapshotAmount(f.snapshot,f.pickupBlend):tickCarrySnapshot(f.snapshot,tick))){
        f.restoring=true;carryAttachmentResult="snapshot_ownership_lost";
    }
    if(!f.restoring&&!f.dropStage){
        if(pickupOwned||(sourceOwned&&(source.variant==2||source.variant==3)))f.lastHeldAt=tick;
        else if(sourceOwned&&(source.variant==4||source.variant==6||(source.variant==8&&carryAttachmentThrowReady())||
                (source.variant==5&&source.stance==0&&carryAttachmentFetalReady()))&&
                carryReleaseElapsedMs(source.variant,amount)<(source.variant==5?100u:source.variant==8?180u:450u)&&f.anchorCalibrated&&
                memoryRead(f.target.actor+0x300,f.dropRoot,sizeof(f.dropRoot))){
            bool finite=true;for(float value:f.dropRoot)finite=finite&&std::isfinite(value);
            if(finite){f.dropStage=1;f.dropVariant=source.variant;f.throwEvent=source.event;f.throwVelocity=source.throwVelocity;carryAttachmentResult="native_drop_pending";}
            else f.restoring=true;
        }else if(tick>f.lastHeldAt+180){ // Bounded native Carry45 -> Drop46 gap.
            if(!carryRestoreCause){carryRestoreCause=0x40000u|(sourceOwned?source.variant:99u);carryRestoreResult=carryAttachmentResult;carryRestoreTick=tick;}
            f.restoring=true;}
    }
    if(!coopGameplayReady(outgoing))return;
    NativeRetargeting current{};
    if(!readNativeRetargeting(memoryRead,base,f.before.handle,current)||current.actor!=f.before.actor||current.component!=f.before.component||
       (current.count&&(current.targetHandle!=f.target.handle||current.target!=f.target.actor))){
        // Product: the presented partner can be replaced (product repair) mid-carry; after 1 s release the body alone.
        if(f.product){if(!f.lostAt)f.lostAt=tick;
            if(tick-f.lostAt>=1000){stopCarrySnapshot(f.snapshot);stopCarryVictimScene(f.scene,f.target);
                if(f.carriedFlag||f.compactReleaseFlag)*reinterpret_cast<unsigned char*>(f.target.actor+0x368)&=static_cast<unsigned char>(~6u);
                if(f.physicsOwned&&carryAttachmentPhysicsOwned(f)&&carryAttachmentApi()){
                    reinterpret_cast<void(*)(void*)>(gameImage+0x734d90)(reinterpret_cast<void*>(f.physics));
                    reinterpret_cast<void(*)(void*)>(gameImage+0x734a10)(reinterpret_cast<void*>(f.physics));}
                retireCarryDamageLease();npcCarryAttachRelease(tick,300);f={};++carryAttachmentRetired;++carryProductLost;carryAttachmentResult="product_carrier_lost";return;}}
        f.restoring=true;carryAttachmentResult="waiting_for_owned_attachment";return;}
    f.lostAt=0;
    if(!f.restoring&&f.attachmentPending){
        if(current.count){f.restoring=true;carryAttachmentResult="unexpected_attachment_during_snapshot";}
        else if(f.sourcePickup?(f.snapshot.restoredAt&&tick>=f.snapshot.restoredAt+50):(tick>=f.snapshot.started+850)){
            if(finishCarrySnapshotAttachment(f,current))return; // Read the new reciprocal record on the next tick.
            if(tick>=(f.sourcePickup?f.snapshot.restoredAt+650:f.snapshot.started+1500)){f.restoring=true;carryAttachmentResult="native_snapshot_attachment_refused";}
        }
        if(!f.restoring)return;
    }
    if(!f.restoring&&!f.dropStage&&current.count&&f.physicsOwned&&
       (f.snapshot.captured?tick>=f.snapshot.started+850:tick>=f.startedAt+250)){
        const float contact=f.sourcePickup?std::clamp(float(tick-f.attachedAt)/350.f,0.f,1.f):f.snapshot.captured?std::clamp((float(tick-f.snapshot.started)-850.f)/350.f,0.f,1.f):1.f;
        if(calibrateCarryAnchor(f,current,contact))carryAttachmentResult=f.anchorCalibrated?"native_anchor_calibrated":"native_snapshot_blending";
    }
    if(!f.restoring&&f.dropStage==1){
        const unsigned handoff=carryReleaseHandoffMs(f.dropVariant);
        if(!sourceOwned||source.variant!=f.dropVariant||carryReleaseElapsedMs(source.variant,amount)>=handoff){
            if(!carryRestoreCause){carryRestoreCause=0x200000u;carryRestoreResult=carryAttachmentResult;carryRestoreTick=tick;}
            f.restoring=true;}
        else if(stopCarryVictimScene(f.scene,f.target)){
            // Scene retirement may put this exact body back in ragdoll mode.
            // Wait for the verified animation request to settle BEFORE binding
            // the next native clip. A post-start request can invalidate the
            // pose whose graph ownership was just established.
            unsigned mode=99,pending=0;
            if(!carryAttachmentPhysicsOwned(f)||!carryAttachmentApi()){
                f.restoring=true;carryAttachmentResult="release_physics_ownership_lost";return;
            }
            if(!f.releasePhysicsRequested){
                reinterpret_cast<void(*)(void*)>(gameImage+0x736ef0)(reinterpret_cast<void*>(f.physics));
                f.releasePhysicsRequested=true;carryAttachmentResult="release_animation_requested_before_scene";return;
            }
            if(!memoryRead(f.physics+0x238,&mode,4)||mode!=3||
               !memoryRead(f.physics+0x23c,&pending,4)||pending!=0xffffffff){
                carryAttachmentResult="waiting_for_release_animation_mode";return;
            }
            const bool retainedThrow=f.dropVariant==8||f.dropVariant==5;
            if(retainedThrow&&!rebaseCarryThrowAnchor(f,current)){
                f.restoring=true;carryAttachmentResult="native_throw_attachment_rebase_refused";return;
            }
            if(!retainedThrow&&current.count)reinterpret_cast<void(*)(void*)>(gameImage+0x757550)(reinterpret_cast<void*>(current.component));
            NativeRetargeting detached{};
            if(!readNativeRetargeting(memoryRead,base,f.before.handle,detached)||detached.count!=(retainedThrow?1u:0u)||detached.component!=current.component){
                carryAttachmentResult="waiting_for_drop_detach";return;
            }
            if(!retainedThrow)std::memcpy(reinterpret_cast<void*>(current.component+0x34),f.settings.data(),16);
            const unsigned elapsed=carryReleaseElapsedMs(source.variant,amount);
            if(!carryAttachmentPhysicsOwned(f)||!startCarryVictimScene(f.scene,f.target,f.dropVariant,elapsed)){
                f.restoring=true;carryAttachmentResult="native_drop_scene_refused";
            }else{
                if(!retainedThrow)reinterpret_cast<void(*)(void*,const float*,const float*,bool,int)>(gameImage+0x87d200)(reinterpret_cast<void*>(f.target.actor),f.dropRoot,f.dropRoot+3,true,0);
                f.dropStage=2;f.dropUntil=tick+handoff-elapsed;carryAttachmentResult="native_drop_playing";
            }
        }else carryAttachmentResult="waiting_for_drop_loop_stop";
    }
    if(!f.restoring&&f.dropStage==2){
        if(f.dropVariant==8&&sourceOwned&&source.variant==8&&source.event==f.throwEvent)f.throwVelocity=source.throwVelocity;
        if(tick>=f.dropUntil||!sourceOwned||source.variant!=f.dropVariant){
            if((f.dropVariant==8||f.dropVariant==5)&&current.count){
                reinterpret_cast<void(*)(void*)>(gameImage+0x757550)(reinterpret_cast<void*>(current.component));
                NativeRetargeting detached{};
                if(!readNativeRetargeting(memoryRead,base,f.before.handle,detached)||detached.count||detached.component!=current.component){
                    carryAttachmentResult="waiting_for_throw_detach";return;
                }
                std::memcpy(reinterpret_cast<void*>(current.component+0x34),f.settings.data(),16);current=detached;
            }
            if(carryAttachmentPhysicsOwned(f)){
                // The victim-only drop scene does not run the original
                // player's carried-body release callback. Seed rigid bodies
                // from the currently visible pose and clear their animated
                // velocities before handing control back to ragdoll physics.
                // Without this, continuous pickup/walk/drop launched Mayhew
                // several metres in the v26 Corvo recording.
                if(!carryAttachmentApi()){f.restoring=true;carryAttachmentResult="drop_physics_api_changed";return;}
                reinterpret_cast<void(*)(void*)>(gameImage+0x734d90)(reinterpret_cast<void*>(f.physics));
                if(f.dropVariant==5){
                    // Native Drop46 selector4 sets bit4 while bit2 remains.
                    // 736c30 then seeds physics from the visible compact pose,
                    // rather than using the ordinary carried-release branch.
                    *reinterpret_cast<unsigned char*>(f.target.actor+0x368)|=4;f.compactReleaseFlag=true;
                }else if(f.carriedFlag){*reinterpret_cast<unsigned char*>(f.target.actor+0x368)&=static_cast<unsigned char>(~2u);f.carriedFlag=false;}
                reinterpret_cast<void(*)(void*)>(gameImage+0x734a10)(reinterpret_cast<void*>(f.physics));f.physicsRestoring=true;
            }
            if(f.dropVariant==8&&f.physicsRestoring&&sourceOwned&&source.variant==8&&source.event==f.throwEvent){
                f.dropStage=3;carryAttachmentResult="native_throw_waiting_for_physics";
            }else if(f.dropVariant==5&&f.physicsRestoring){
                f.dropStage=4;carryAttachmentResult="native_compact_waiting_for_physics";
            }else f.restoring=true;
        }
        else carryAttachmentResult="native_drop_playing";
    }
    if(!f.restoring&&f.dropStage==3){
        unsigned mode=99,pending=0;
        if(!sourceOwned||source.variant!=8||source.event!=f.throwEvent||tick>f.dropUntil+200){
            ++carryThrowRefused;f.restoring=true;carryAttachmentResult="native_throw_impulse_expired";
        }else if(!stopCarryVictimScene(f.scene,f.target)){
            // The victim scene still owns animation until native stop retires
            // its exact handle. Do not wait for ragdoll while retaining that
            // ownership, or queue an impulse that scene cleanup could erase.
            carryAttachmentResult="native_throw_waiting_for_scene_stop";
        }else if(carryAttachmentPhysicsOwned(f)&&!f.throwRagdollRequested){
            // 734a10 ignores requests while +241 bit3 is set. Scene shutdown
            // must complete before making this request; the pre-stop request
            // used to seed the release cannot stand in for this transition.
            reinterpret_cast<void(*)(void*)>(gameImage+0x734a10)(reinterpret_cast<void*>(f.physics));
            f.throwRagdollRequested=true;carryAttachmentResult="native_throw_ragdoll_requested_after_stop";
        }else if(carryAttachmentPhysicsOwned(f)&&memoryRead(f.physics+0x238,&mode,4)&&mode==0&&
                 memoryRead(f.physics+0x23c,&pending,4)&&pending==0xffffffff){
            if(!queueCarryThrowImpulse(f)){++carryThrowRefused;carryAttachmentResult="native_throw_impulse_refused";}
            else carryAttachmentResult="native_throw_impulse_queued";
            f.restoring=true; // Consume one attempt; no later-tick replay.
        }
    }
    if(!f.restoring&&f.dropStage==4){
        unsigned mode=99,pending=0;
        if(carryAttachmentPhysicsOwned(f)&&memoryRead(f.physics+0x238,&mode,4)&&mode==0&&
           memoryRead(f.physics+0x23c,&pending,4)&&pending==0xffffffff){
            f.restoring=true;carryAttachmentResult="native_compact_physics_ready";
        }else if(tick>f.dropUntil+600){
            f.restoring=true;carryAttachmentResult="native_compact_physics_timeout";
        }
    }
    if(f.restoring){
        if(!carryRestoreCause){carryRestoreCause=0x800000u;carryRestoreResult=carryAttachmentResult;carryRestoreTick=tick;}
        if(f.attachmentPending&&f.snapshot.bound&&f.physicsOwned&&!current.count){
            // Preserve the visible blended pose until native physics has
            // consumed it. Clearing the blend first exposes the folded clip
            // during handoff and launched the body in the v20 cancel video.
            if(!carryAttachmentPhysicsOwned(f)){carryAttachmentResult="waiting_for_owned_snapshot_physics";return;}
            if(!f.physicsRestoring){
                // Native carried-body release (736c30 -> 734d90) copies the
                // current world joint pose into every rigid body and clears
                // linear/angular velocity. Merely changing mode left animated
                // body velocities behind in Emily's v21 cancellation recording.
                // Keep the snapshot bound while this exact native routine reads
                // the visible pose; no blanket world-physics writes or impulse.
                unsigned mode=99,pending=0;NativeSkeleton victim{};
                if(!carryAttachmentApi()||!carrySnapshotOwned(f.snapshot)||
                   !readNativeSkeleton(memoryRead,base,f.target.handle,victim)||!victim.stable||
                   victim.actor!=f.target.actor||victim.animation!=f.snapshot.animation||
                   !memoryRead(f.physics+0x238,&mode,4)||mode!=3||
                   !memoryRead(f.physics+0x23c,&pending,4)||pending!=0xffffffff){
                    carryAttachmentResult="waiting_for_stable_snapshot_pose";return;
                }
                reinterpret_cast<void(*)(void*)>(gameImage+0x734d90)(reinterpret_cast<void*>(f.physics));
                if(f.carriedFlag){*reinterpret_cast<unsigned char*>(f.target.actor+0x368)&=static_cast<unsigned char>(~2u);f.carriedFlag=false;}
                reinterpret_cast<void(*)(void*)>(gameImage+0x734a10)(reinterpret_cast<void*>(f.physics));f.physicsRestoring=true;
            }
            unsigned mode=99,pending=0;
            if(!memoryRead(f.physics+0x238,&mode,4)||mode!=0||!memoryRead(f.physics+0x23c,&pending,4)||pending!=0xffffffff){
                carryAttachmentResult="waiting_for_blended_pose_ragdoll";return;
            }
        }
        if(!stopCarrySnapshot(f.snapshot)){carryAttachmentResult="waiting_for_owned_snapshot";return;}
        if(!stopCarryVictimScene(f.scene,f.target)){carryAttachmentResult="waiting_for_native_victim_scene_stop";return;}
        if(current.count)reinterpret_cast<void(*)(void*)>(gameImage+0x757550)(reinterpret_cast<void*>(current.component));
        NativeRetargeting released{};std::uintptr_t owner=0;
        if(readNativeRetargeting(memoryRead,base,current.handle,released)&&!released.count&&released.component==current.component&&
           worldIdentityEntity(memoryRead,observedGame,f.target.handle)==f.target.actor&&
           memoryRead(f.target.actor+0x358,&owner,8)&&owner&&memoryRead(owner+0x3f0,&owner,8)&&!owner){
            std::memcpy(reinterpret_cast<void*>(current.component+0x34),f.settings.data(),16);
            if(f.carriedFlag){*reinterpret_cast<unsigned char*>(f.target.actor+0x368)&=static_cast<unsigned char>(~2u);f.carriedFlag=false;}
            if(f.compactReleaseFlag){*reinterpret_cast<unsigned char*>(f.target.actor+0x368)&=static_cast<unsigned char>(~4u);f.compactReleaseFlag=false;}
            if(f.physicsOwned){
                if(!carryAttachmentPhysicsOwned(f)){carryAttachmentResult="waiting_for_owned_physics";return;}
                if(!f.physicsRestoring){
                    // Native ragdoll request; let the physics scheduler apply
                    // it before declaring this bounded fixture released.
                    reinterpret_cast<void(*)(void*)>(gameImage+0x734a10)(reinterpret_cast<void*>(f.physics));f.physicsRestoring=true;
                }
                unsigned mode=99,pending=0;
                if(!memoryRead(f.physics+0x238,&mode,4)||mode!=0||!memoryRead(f.physics+0x23c,&pending,4)||pending!=0xffffffff){carryAttachmentResult="waiting_for_native_ragdoll";return;}
            }
            finishCarryDamageLease(f.target.actor,f.target.handle,tick);
            if(f.product){npcCarryAttachRelease(tick,f.dropVariant==8?1500:300);carryProductResult="released";}
            f={};++carryAttachmentReleased;carryAttachmentResult="native_attachment_released";
        }else carryAttachmentResult="native_release_unconfirmed";
    }
}
// 1214do: every change of the fixture's result, with its refusal stage and the source's carry sample, so a take shows
// which step ends a product attempt (1277s: three product pickups on the host released without a native attachment,
// while the client's held start in 1277t attached).
struct CarryResultEntry{ULONGLONG tick=0;const char* result="";unsigned stage=0,variant=0,stance=0;float amount=0,blend=0;bool pickup=false,snapshot=false,pending=false;};
std::array<CarryResultEntry,32> carryResultLog{};unsigned carryResultLogCount=0;const char* carryResultLast="";
void logCarryResult(ULONGLONG tick){
    if(carryAttachmentResult==carryResultLast)return;carryResultLast=carryAttachmentResult;const auto& f=carryAttachmentFixture;
    CarryPresentation::Sample s{};float amount=0;const double at=presentationClock()-(poseSmoothingEnabled?PoseInterpolation::delayMs:0.);
    const bool owned=shadowStream.carry.sample(at,s,amount);
    carryResultLog[carryResultLogCount%carryResultLog.size()]={tick,carryAttachmentResult,carryAttachmentRefusalStage,owned?s.variant:99u,owned?s.stance:99u,amount,f.pickupBlend,f.sourcePickup,f.snapshot.captured,f.attachmentPending};
    ++carryResultLogCount;
}
void serviceCarryAttachmentProduct(ULONGLONG tick){
    logCarryResult(tick);
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load()||!productSessionArmed()||carryAttachmentFixture.until||
       carryAttachmentRequest||observerIsolation.until||!probeDoppelganger)return;
    if(!carryAttachmentPeerCurrent(tick,true)||shadowStream.remoteState!=14||!shadowStream.remoteCarryLocalHandle)return;
    CarryPresentation::Sample sample{};float amount=0;
    const double at=presentationClock()-(poseSmoothingEnabled?PoseInterpolation::delayMs:0.);
    if(!shadowStream.carry.sample(at,sample,amount)||sample.incarnation!=shadowStream.remoteCarryIncarnation)return;
    const bool pickup=sample.variant==7||(sample.variant==1&&amount<.15f),held=sample.variant==2||sample.variant==3;
    if(!pickup&&!held)return;
    if(carryProductIncarnation!=sample.incarnation){carryProductIncarnation=sample.incarnation;carryProductAttempts=0;}
    if(carryProductAttempts>=3||tick-carryProductLastTry<250)return;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    // Never while the local player carries the same body in this world.
    NativePlayerStates st{};if(readNativePlayerStates(memoryRead,base,st))for(const auto& a:st.active){unsigned h=0;
        if(a.id==45&&a.vtable==base+0x1c77748&&memoryRead(a.pointer+0xf8,&h,4)&&h==shadowStream.remoteCarryLocalHandle){carryProductResult="local_carry_same_body";return;}
        if(a.id==46&&a.vtable==base+0x1c790c8&&memoryRead(a.pointer+0x38,&h,4)&&h==shadowStream.remoteCarryLocalHandle){carryProductResult="local_carry_same_body";return;}}
    const auto actor=worldIdentityEntity(memoryRead,observedGame,shadowStream.remoteCarryLocalHandle);if(!actor){carryProductResult="no_local_body";return;}
    ++carryProductAttempts;carryProductLastTry=tick;npcCarryAttachClaim(shadowStream.remoteCarryLocalHandle,actor);
    carryAttachmentPickupRequested=pickup;carryAttachmentSnapshotRequested=true;carryAttachmentProductRequested=true;carryAttachmentRequest=1;
    carryProductResult=pickup?"pickup_requested":"held_requested";
}
void writeCarryAttachmentFixture(ULONGLONG tick){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.carry-attachment.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* file=nullptr;if(fopen_s(&file,tmp,"wb")||!file)return;
    const auto& f=carryAttachmentFixture;
    unsigned throwMode=99,throwPending=0,throwFlags=0;
    if(f.physics&&carryAttachmentPhysicsOwned(f)){
        memoryRead(f.physics+0x238,&throwMode,4);memoryRead(f.physics+0x23c,&throwPending,4);memoryRead(f.physics+0x240,&throwFlags,4);
    }
    fprintf(file,"{\"throwQueued\":%llu,\"throwRefused\":%llu,\"lastThrowVelocity\":[%.7f,%.7f,%.7f],",carryThrowQueued,carryThrowRefused,carryLastThrowVelocity[0],carryLastThrowVelocity[1],carryLastThrowVelocity[2]);
    fprintf(file,"\"physicsMode\":%u,\"physicsPending\":%u,\"physicsFlags\":%u,\"throwRagdollRequested\":%s,",throwMode,throwPending,throwFlags,f.throwRagdollRequested?"true":"false");
    fprintf(file,"\"attachmentPending\":%s,",f.attachmentPending?"true":"false");
    fprintf(file,"\"restoreCause\":%u,\"restoreResult\":\"%s\",\"restoreTick\":%llu,",carryRestoreCause,carryRestoreResult,carryRestoreTick);
    fprintf(file,"\"resultLog\":[");
    {const unsigned total=(std::min)(carryResultLogCount,unsigned(carryResultLog.size()));
     for(unsigned i=0;i<total;++i){const auto& e=carryResultLog[(carryResultLogCount-total+i)%carryResultLog.size()];
        fprintf(file,"%s{\"tick\":%llu,\"result\":\"%s\",\"stage\":%u,\"variant\":%u,\"stance\":%u,\"amount\":%.3f,\"blend\":%.3f,\"pickup\":%s,\"snapshot\":%s,\"pending\":%s}",
            i?",":"",e.tick,e.result,e.stage,e.variant,e.stance,e.amount,e.blend,e.pickup?"true":"false",e.snapshot?"true":"false",e.pending?"true":"false");}}
    fprintf(file,"],");
    fprintf(file,"\"product\":%s,\"productResult\":\"%s\",\"productStarts\":%llu,\"productRefused\":%llu,\"productLost\":%llu,\"productAttempts\":%u,\"victimState\":%u,",
        f.product?"true":"false",carryProductResult,carryProductStarts,carryProductRefused,carryProductLost,carryProductAttempts,f.victimState);
    {const auto& s=npcCarrySuppression;fprintf(file,"\"streamSuppressed\":{\"handle\":%u,\"claims\":%llu,\"releases\":%llu,\"presentSkips\":%llu,\"holdSkips\":%llu,\"physicsSkips\":%llu,\"evictSkips\":%llu},",
        s.handle,s.claims,s.releases,s.presentSkips,s.holdSkips,s.physicsSkips,s.evictSkips);}
    fprintf(file,"\"releasePhysicsRequested\":%s,",f.releasePhysicsRequested?"true":"false");
    fprintf(file,"\"sourcePickup\":%s,\"pickupBlend\":%.7f,",f.sourcePickup?"true":"false",f.pickupBlend);
    fprintf(file,"\"snapshotRestoredAfterMs\":%llu,\"attachmentStartedAfterMs\":%llu,",f.snapshot.restoredAt&&f.snapshot.restoredAt>=f.snapshot.started?f.snapshot.restoredAt-f.snapshot.started:0,f.attachedAt&&f.attachedAt>=f.snapshot.started?f.attachedAt-f.snapshot.started:0);
    fprintf(file,"\"snapshotCaptured\":%s,\"snapshotBound\":%s,\"snapshotElapsedMs\":%llu,",f.snapshot.captured?"true":"false",f.snapshot.bound?"true":"false",f.snapshot.started&&tick>=f.snapshot.started?tick-f.snapshot.started:0);
    fprintf(file,"\"testFixtureOnly\":true,\"active\":%s,\"restoring\":%s,\"remainingMs\":%llu,\"started\":%llu,\"released\":%llu,\"retired\":%llu,\"refused\":%llu,\"targetHandle\":%u,\"result\":\"%s\",\"nativeVictimSceneActive\":%s,\"anchorCalibrated\":%s,\"victimSceneHandle\":%u,\"dropStage\":%u,\"dropVariant\":%u,\"refusalStage\":%u,\"requestScopeMs\":%llu,\"requestCatalogAge\":%llu,\"requestedDuration\":%u,\"complete\":false}\n",f.until?"true":"false",f.restoring?"true":"false",f.until>tick?f.until-tick:0,carryAttachmentStarted,carryAttachmentReleased,carryAttachmentRetired,carryAttachmentRefused,f.target.handle,carryAttachmentResult,f.scene.verified?"true":"false",f.anchorCalibrated?"true":"false",f.scene.handle,f.dropStage,f.dropVariant,carryAttachmentRefusalStage,carryAttachmentRequestScopeMs,carryAttachmentRequestCatalogAge,carryAttachmentDuration);
    const bool ok=!ferror(file);const int closed=fclose(file);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
