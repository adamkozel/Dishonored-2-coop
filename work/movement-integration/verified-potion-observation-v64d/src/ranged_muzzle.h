#pragma once
// Render-only muzzle effect, once per delayed source-confirmed shot. Neither
// the native weapon fire path nor projectile/damage code is called here.
struct RangedMuzzleState {std::uint64_t token=0,peerToken=0,event=0;};
RangedMuzzleState rangedMuzzle;
void serviceRangedMuzzle(ULONGLONG tick){
    if(!nativeMutationAllowed||!shadowStreamCurrent(tick)||!shadowStream.peerReady){rangedMuzzle={};return;}
    if(rangedMuzzle.token!=shadowStream.token||rangedMuzzle.peerToken!=shadowStream.peerToken)
        rangedMuzzle={shadowStream.token,shadowStream.peerToken,0};
    unsigned shot=0,kind=0,variant=0;std::uint64_t event=0;bool current=false;
    AcquireSRWLockShared(&movementLock);const auto& p=movementPlayback;
    current=p.end>tick&&p.lease>tick&&p.game==observedGame&&p.epoch==outgoing.worldEpoch&&p.actor==probeActor&&p.rangedWeight>.9f;
    shot=p.rangedShot;kind=p.rangedKind;variant=p.rangedVariant;event=p.rangedEvent;ReleaseSRWLockShared(&movementLock);
    if(!current||!shot||kind!=1||variant!=3||!event||event<=rangedMuzzle.event||!rangedProp.network||
       rangedProp.kind!=1||rangedProp.until<=tick||!rangedPropIdentity(true))return;
    NativeSkeleton s{};
    if(!readNativeSkeleton(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),rangedProp.handle,s,NativeSkeletonScope::CosmeticRanged)||
       !s.stable||s.actor!=rangedProp.actor||s.globalPose.empty())return;
    // Both exact native pistol declarations define muzzle=(.38,0,.055),
    // identity rotation, parent=origin. Compose the native origin joint and
    // entity world frame so grip/attachment changes also move the flash.
    if(s.parents.empty()||s.parents[0]!=-1)return;
    const auto& j=s.globalPose[0];float point[3]{},axis[9]{},local[3]{};
    for(unsigned r=0;r<3;++r){local[r]=j[12+r]+j[r]*.38f+j[8+r]*.055f;
        point[r]=s.root[r];for(unsigned c=0;c<3;++c)point[r]+=s.root[3+c*3+r]*local[c];
        for(unsigned c=0;c<3;++c)for(unsigned k=0;k<3;++k)axis[c*3+r]+=s.root[3+k*3+r]*j[c*4+k];}
    // Consume only after ownership and transforms validate. A native start
    // attempt is never retried, even if its returned ownership is ambiguous.
    rangedMuzzle.event=event;
    startNativePowerFx(9,point,tick,true,axis);
}
