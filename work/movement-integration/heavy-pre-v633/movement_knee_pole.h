#pragma once
#include "body_retarget.h"
// Preserve the authored bend plane after native terrain IK. Rotate the solved
// two-bone chain around its hip/ankle axis: both endpoints, segment lengths and
// the native ankle orientation remain unchanged. No actor or physics writes.
inline bool movementKneePole(const BodyMatrix& parent,const BodyMatrix* reference,
                             const BodyMatrix* solved,BodyMatrix* local,float& angle){
    angle=0;
    if(!bodyRigid(parent))return false;
    for(unsigned i=0;i<3;++i)if(!bodyRigid(reference[i])||!bodyRigid(solved[i]))return false;
    float axis[3]{},pole[3]{},wanted[3]{};float length=0;
    for(unsigned k=0;k<3;++k){axis[k]=solved[2].v[12+k]-solved[0].v[12+k];length+=axis[k]*axis[k];
        pole[k]=solved[1].v[12+k]-solved[0].v[12+k];wanted[k]=reference[1].v[12+k]-reference[0].v[12+k];}
    if(length<.000001f)return false;
    length=std::sqrt(length);float a=0,b=0;
    for(unsigned k=0;k<3;++k){axis[k]/=length;a+=axis[k]*pole[k];b+=axis[k]*wanted[k];}
    float pn=0,wn=0;
    for(unsigned k=0;k<3;++k){pole[k]-=a*axis[k];wanted[k]-=b*axis[k];pn+=pole[k]*pole[k];wn+=wanted[k]*wanted[k];}
    // A straight knee has no stable pole; retain the native solution.
    BodyMatrix corrected[3]{solved[0],solved[1],solved[2]};
    if(pn>.000025f&&wn>.000025f){
        float cross[3]{pole[1]*wanted[2]-pole[2]*wanted[1],pole[2]*wanted[0]-pole[0]*wanted[2],pole[0]*wanted[1]-pole[1]*wanted[0]};
        float sn=0,cs=0;for(unsigned k=0;k<3;++k){sn+=axis[k]*cross[k];cs+=pole[k]*wanted[k];}
        angle=std::atan2(sn,cs);const float c=std::cos(angle),s=std::sin(angle);
        auto rotate=[&](const float* in,float* out){const float d=axis[0]*in[0]+axis[1]*in[1]+axis[2]*in[2];
            const float x[3]{axis[1]*in[2]-axis[2]*in[1],axis[2]*in[0]-axis[0]*in[2],axis[0]*in[1]-axis[1]*in[0]};
            for(unsigned k=0;k<3;++k)out[k]=in[k]*c+x[k]*s+axis[k]*d*(1-c);};
        for(unsigned j=0;j<2;++j){
            for(unsigned col=0;col<3;++col)rotate(solved[j].v+col*4,corrected[j].v+col*4);
            float offset[3]{},next[3]{};for(unsigned k=0;k<3;++k)offset[k]=solved[j].v[12+k]-solved[0].v[12+k];
            rotate(offset,next);for(unsigned k=0;k<3;++k)corrected[j].v[12+k]=solved[0].v[12+k]+next[k];
        }
    }
    for(unsigned j=0;j<3;++j){local[j]=bodyMultiply(bodyInverse(j?corrected[j-1]:parent),corrected[j]);if(!bodyRigid(local[j]))return false;}
    return true;
}
