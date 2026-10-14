#pragma once
#include "body_crouch.h"
#include "mixamo_corvo_clip.h"
#include "mixamo_corvo_grip.h"
#include "mixamo_emily_clip.h"
#include "mixamo_emily_grip.h"
inline bool mixamoGripJoint(unsigned j,unsigned count){return j==16||j==17||(j>=38&&j<=48)||(count==215?(j>=63&&j<=72):(j>=60&&j<=69));}
inline bool composeMixamoSwordGrip(const std::vector<BodyMatrix>& native,std::vector<BodyMatrix>& result){
    if(native.size()!=215&&native.size()!=219)return false;
    const bool corvo=native.size()==215;
    const auto& joints=corvo?kMixamoGripJoints:kMixamoEmilyGripJoints;
    const auto& rotations=corvo?kMixamoGripRotation:kMixamoEmilyGripRotation;
    const auto& wrist=corvo?kMixamoCorvoStandingWrist:kMixamoEmilyStandingWrist;
    const auto& socket=corvo?kMixamoCorvoStandingSocket:kMixamoEmilyStandingSocket;
    const auto& position=corvo?kMixamoCorvoStandingSocketPosition:kMixamoEmilyStandingSocketPosition;
    result=native;
    for(unsigned k=0;k<=std::size(joints);++k){
        const unsigned j=k==std::size(joints)?16:joints[k];
        const float* r=j==16?wrist:j==17?socket:rotations[k];
        std::array<float,12> q={r[0],r[1],r[2],r[3],0,0,0,0,1,1,1,1};BodyMatrix m{};
        if(!bodyRigid(native[j])||!bodyFromQts(q,m))return false;
        for(unsigned c=0;c<3;++c)for(unsigned a=0;a<3;++a)result[j].v[c*4+a]=m.v[c*4+a];
        if(j==17)for(unsigned a=0;a<3;++a)result[j].v[12+a]=position[a];
    }
    return true;
}
// Experimental offline-baked rotations. Not an FBX/edgeanim loader.
// Only mapped native joints are changed; first-person rigs never enter here.
inline bool composeMixamoCrouch(const BodyRetarget& rig,const std::array<BodyMatrix,20>& source,
    const std::vector<BodyMatrix>& standing,float phase,std::vector<BodyMatrix>& result){
    if((standing.size()!=215&&standing.size()!=219)||rig.parents.size()!=standing.size()||rig.sourceBindGlobal.size()!=20||!std::isfinite(phase)||phase<0)return false;
    const bool corvo=standing.size()==215;
    const auto& frames=corvo?kMixamoCorvoFrames:kMixamoEmilyFrames;
    const auto& mapped=corvo?kMixamoCorvoJoints:kMixamoEmilyJoints;
    const auto& gripJoints=corvo?kMixamoGripJoints:kMixamoEmilyGripJoints;
    const auto& gripRotation=corvo?kMixamoGripRotation:kMixamoEmilyGripRotation;
    const auto& attachmentPosition=corvo?kMixamoCorvoAttachmentPosition:kMixamoEmilyAttachmentPosition;
    const float drop=rig.sourceBindGlobal[1].v[14]-source[1].v[14];
    if(!std::isfinite(drop)||drop<-.1f||drop>1.2f)return false;
    const float weight=(std::max)(0.f,(std::min)(1.f,drop/.55f));
    const float frame=std::fmod(phase,1.f)*62.f;const unsigned a=static_cast<unsigned>(frame),b=a+1;const float t=frame-a;
    if(!composeMixamoSwordGrip(standing,result))return false;
    const auto held=result;
    for(unsigned k=0;k<22;++k){const auto j=mapped[k];BodyMatrix ma{},mb{};
        std::array<float,12> qa{},qb{};qa[8]=qa[9]=qa[10]=qb[8]=qb[9]=qb[10]=1;
        for(unsigned n=0;n<7;++n){qa[n]=frames[a][k][n];qb[n]=frames[b][k][n];}
        if(!j||j>=standing.size()||!bodyRigid(standing[j])||!bodyFromQts(qa,ma)||!bodyFromQts(qb,mb))return false;
        float ar[9]{},br[9]{},native[9]{},clip[9]{},blended[9]{};
        for(unsigned c=0;c<3;++c)for(unsigned r=0;r<3;++r){ar[c*3+r]=ma.v[c*4+r];br[c*3+r]=mb.v[c*4+r];native[c*3+r]=held[j].v[c*4+r];}
        presentationRotation(ar,br,t,clip);presentationRotation(native,clip,weight,blended);
        for(unsigned c=0;c<3;++c)for(unsigned r=0;r<3;++r)result[j].v[c*4+r]=blended[c*3+r];
        // Keep native translations/bone lengths, except the authored pelvis
        // sway and height correction. Root 0 remains network-owned.
        if(j==2)for(unsigned n=0;n<3;++n){const float v=qa[4+n]+(qb[4+n]-qa[4+n])*t;result[j].v[12+n]=standing[j].v[12+n]+(v-standing[j].v[12+n])*weight;}
    }
    // Keep fingers closed throughout stance transitions. Reverse the socket
    // around the native grip centre while the baked wrist follows FP blade
    // orientation. Standing body motion remains entirely native.
    for(unsigned k=0;k<std::size(gripJoints);++k){
        const auto j=gripJoints[k];BodyMatrix grip{};
        std::array<float,12> q={gripRotation[k][0],gripRotation[k][1],gripRotation[k][2],gripRotation[k][3],0,0,0,0,1,1,1,1};
        if(j>=standing.size()||!bodyFromQts(q,grip)||!bodyRigid(standing[j]))return false;
        float native[9]{},authored[9]{},blended[9]{};
        for(unsigned c=0;c<3;++c)for(unsigned r=0;r<3;++r){native[c*3+r]=held[j].v[c*4+r];authored[c*3+r]=grip.v[c*4+r];}
        presentationRotation(native,authored,weight,blended);
        for(unsigned c=0;c<3;++c)for(unsigned r=0;r<3;++r)result[j].v[c*4+r]=blended[c*3+r];
        // Only the attachment offset changes; no anatomical bone scales.
        if(j==17)for(unsigned n=0;n<3;++n)result[j].v[12+n]=held[j].v[12+n]+(attachmentPosition[n]-held[j].v[12+n])*weight;
    }
    std::vector<BodyMatrix> global;return bodyForward(rig.parents,result,global);
}
