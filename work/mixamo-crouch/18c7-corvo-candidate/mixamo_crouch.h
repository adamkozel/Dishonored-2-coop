#pragma once
#include "body_crouch.h"
#include "mixamo_corvo_clip.h"
#include "mixamo_corvo_grip.h"
// Experimental offline-baked rotations. Not an FBX/edgeanim loader.
// Only mapped native joints are changed; first-person rigs never enter here.
inline bool composeMixamoCorvo(const BodyRetarget& rig,const std::array<BodyMatrix,20>& source,
    const std::vector<BodyMatrix>& standing,float phase,std::vector<BodyMatrix>& result){
    if(standing.size()!=215||rig.parents.size()!=215||rig.sourceBindGlobal.size()!=20||!std::isfinite(phase)||phase<0)return false;
    const float drop=rig.sourceBindGlobal[1].v[14]-source[1].v[14];
    if(!std::isfinite(drop)||drop<-.1f||drop>1.2f)return false;
    const float weight=(std::max)(0.f,(std::min)(1.f,drop/.55f));
    const float frame=std::fmod(phase,1.f)*62.f;const unsigned a=static_cast<unsigned>(frame),b=a+1;const float t=frame-a;
    result=standing;
    for(unsigned k=0;k<22;++k){const auto j=kMixamoCorvoJoints[k];BodyMatrix ma{},mb{};
        std::array<float,12> qa{},qb{};qa[8]=qa[9]=qa[10]=qb[8]=qb[9]=qb[10]=1;
        for(unsigned n=0;n<7;++n){qa[n]=kMixamoCorvoFrames[a][k][n];qb[n]=kMixamoCorvoFrames[b][k][n];}
        if(!j||j>=standing.size()||!bodyRigid(standing[j])||!bodyFromQts(qa,ma)||!bodyFromQts(qb,mb))return false;
        float ar[9]{},br[9]{},native[9]{},clip[9]{},blended[9]{};
        for(unsigned c=0;c<3;++c)for(unsigned r=0;r<3;++r){ar[c*3+r]=ma.v[c*4+r];br[c*3+r]=mb.v[c*4+r];native[c*3+r]=standing[j].v[c*4+r];}
        presentationRotation(ar,br,t,clip);presentationRotation(native,clip,weight,blended);
        for(unsigned c=0;c<3;++c)for(unsigned r=0;r<3;++r)result[j].v[c*4+r]=blended[c*3+r];
        // Keep native translations/bone lengths, except the authored pelvis
        // sway and height correction. Root 0 remains network-owned.
        if(j==2)for(unsigned n=0;n<3;++n){const float v=qa[4+n]+(qb[4+n]-qa[4+n])*t;result[j].v[12+n]=standing[j].v[12+n]+(v-standing[j].v[12+n])*weight;}
    }
    // The unarmed donor wrist is not a sword grip. Reuse the native authored
    // sword_idle wrist, attachment and complete finger chain as one unit.
    // Keep live native translations and blend out with the crouch itself.
    for(unsigned k=0;k<std::size(kMixamoGripJoints);++k){
        const auto j=kMixamoGripJoints[k];BodyMatrix grip{};
        std::array<float,12> q={kMixamoGripRotation[k][0],kMixamoGripRotation[k][1],kMixamoGripRotation[k][2],kMixamoGripRotation[k][3],0,0,0,0,1,1,1,1};
        if(j>=standing.size()||!bodyFromQts(q,grip)||!bodyRigid(standing[j]))return false;
        float native[9]{},authored[9]{},blended[9]{};
        for(unsigned c=0;c<3;++c)for(unsigned r=0;r<3;++r){native[c*3+r]=standing[j].v[c*4+r];authored[c*3+r]=grip.v[c*4+r];}
        presentationRotation(native,authored,weight,blended);
        for(unsigned c=0;c<3;++c)for(unsigned r=0;r<3;++r)result[j].v[c*4+r]=blended[c*3+r];
    }
    std::vector<BodyMatrix> global;return bodyForward(rig.parents,result,global);
}
