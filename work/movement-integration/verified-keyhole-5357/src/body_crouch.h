#pragma once
#include "body_retarget.h"
#include "pose_interpolation.h"
// Whole-body research blend. Start with the complete authored native pose.
// Transfer only motion relative to a settled first-person crouch reference,
// never its absolute resting leg angles or pelvis drop.
// The reference is captured from the owned clone, never from the local player.
inline bool composeNativeCrouch(const BodyRetarget& rig,const std::array<BodyMatrix,20>& source,const std::array<BodyMatrix,20>& sourceReference,
    const std::vector<BodyMatrix>& reference,const std::vector<BodyMatrix>& standing,std::vector<BodyMatrix>& result,bool uprightTorso=false){
    const auto n=rig.parents.size();
    if(n<27||reference.size()!=n||standing.size()!=n||rig.parents[3]!=2||rig.sourceForTarget[3]!=2)return false;
    std::vector<BodyMatrix> refGlobal,retarget,global;
    if(!bodyForward(rig.parents,reference,refGlobal))return false;
    BodyRetarget relative=rig;
    relative.targetBindLocal=reference;relative.targetBindGlobal=refGlobal;
    relative.sourceBindGlobal.assign(sourceReference.begin(),sourceReference.end());
    for(const auto& m:sourceReference)if(!bodyRigid(m))return false;
    const float referenceDrop=rig.sourceBindGlobal[1].v[14]-sourceReference[1].v[14];
    if(referenceDrop<.55f||referenceDrop>.85f)return false;
    // During entry/exit, the source is changing stance, not stepping. Do not
    // apply that large standing-to-crouch leg delta on top of the authored pose.
    const float drop=rig.sourceBindGlobal[1].v[14]-source[1].v[14];
    const float stanceWeight=(std::max)(0.f,(std::min)(1.f,drop/.55f));
    const bool settled=drop>=referenceDrop-.08f;
    if(!retargetBody(relative,settled?source:sourceReference,reference,retarget))return false;
    // Root stays network-owned, including during stance transitions.
    retarget[0]=standing[0];
    if(uprightTorso){
        // User-requested A/B: retain calibrated crouched legs, but restore the
        // native standing spine/head/arms subtree. Counter-rotate at the first
        // spine joint so a tilted crouched pelvis cannot tilt the whole torso.
        // Keep its authored local translation: no gap or stretched connector.
        std::vector<BodyMatrix> nativeGlobal,crouchGlobal;
        if(!bodyForward(rig.parents,standing,nativeGlobal)||!bodyForward(rig.parents,retarget,crouchGlobal))return false;
        for(unsigned j=3;j<n;++j){
            int ancestor=static_cast<int>(j);
            for(unsigned depth=0;depth<n&&ancestor>=0&&ancestor!=3;++depth)ancestor=rig.parents[ancestor];
            if(ancestor==3)retarget[j]=standing[j];
        }
        const auto torso=bodyMultiply(bodyInverse(crouchGlobal[2]),nativeGlobal[3]);
        for(unsigned c=0;c<3;++c)for(unsigned r=0;r<3;++r)retarget[3].v[c*4+r]=torso.v[c*4+r];
    }
    const float weight=stanceWeight;
    result=standing;
    for(unsigned j=1;j<n;++j){
        if(!bodyRigid(standing[j])||!bodyRigid(retarget[j]))return false;
        float a[9]{},b[9]{},q[9]{};
        for(unsigned c=0;c<3;++c)for(unsigned r=0;r<3;++r){a[c*3+r]=standing[j].v[c*4+r];b[c*3+r]=retarget[j].v[c*4+r];}
        presentationRotation(a,b,weight,q);
        for(unsigned c=0;c<3;++c)for(unsigned r=0;r<3;++r)result[j].v[c*4+r]=q[c*3+r];
        for(unsigned r=12;r<15;++r)result[j].v[r]=standing[j].v[r]+(retarget[j].v[r]-standing[j].v[r])*weight;
    }
    return bodyForward(rig.parents,result,global);
}
