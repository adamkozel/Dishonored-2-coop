#pragma once
#include "body_retarget.h"
#include "pose_interpolation.h"
// Whole-body research blend. The native reference supplies the entire torso,
// arms, hands and helpers; player retargeting supplies crouched stepping.
// The reference is captured from the owned clone, never from the local player.
inline bool composeNativeCrouch(const BodyRetarget& rig,const std::array<BodyMatrix,20>& source,
    const std::vector<BodyMatrix>& reference,const std::vector<BodyMatrix>& standing,std::vector<BodyMatrix>& result){
    const auto n=rig.parents.size();
    if(n<27||reference.size()!=n||standing.size()!=n||rig.parents[3]!=2||rig.sourceForTarget[3]!=2)return false;
    std::vector<BodyMatrix> refGlobal,retarget,global;
    if(!bodyForward(rig.parents,reference,refGlobal)||!retargetBody(rig,source,reference,retarget))return false;
    // Root stays network-owned. Place the authored crouched torso on the
    // retargeted pelvis without inheriting the first-person body's upright back.
    retarget[0]=standing[0];
    if(!bodyForward(rig.parents,retarget,global))return false;
    const auto spine=bodyMultiply(bodyInverse(global[2]),refGlobal[3]);
    for(unsigned j=0;j<n;++j){
        unsigned ancestor=j;bool torso=false;
        for(unsigned depth=0;depth<n;++depth){if(ancestor==3){torso=true;break;}const int p=rig.parents[ancestor];if(p<0)break;ancestor=static_cast<unsigned>(p);}
        if(torso){retarget[j]=reference[j];if(j==3)for(unsigned c=0;c<3;++c)for(unsigned r=0;r<3;++r)retarget[j].v[c*4+r]=spine.v[c*4+r];}
    }
    const float drop=rig.sourceBindGlobal[1].v[14]-source[1].v[14];
    const float weight=(std::max)(0.f,(std::min)(1.f,drop/.55f));
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
