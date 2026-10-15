#pragma once
#include "body_retarget.h"
// #6: two-bone hand reach for the authored use clips. The authored reach puts the hand near a
// generic contact point; this pulls the wrist toward the real mechanism. Inputs are model-space
// globals: the shoulder (parent of the chain) and the arm, elbow and wrist joints of the posed
// partner body, plus a model-space target and a weight (0 keeps the pose exactly).
// - the arm joint never moves; both segment lengths are preserved exactly;
// - the elbow keeps the authored bend plane (its offset from the shoulder-wrist line);
// - the forearm and the hand follow the corrected segments by minimal rotations, so twist and the
//   authored hand orientation relative to the forearm are kept;
// - an unreachable target stops at full extension minus a margin (no hyperextension).
// Outputs local matrices for the three joints (parent-relative), like movementKneePole.
namespace arm_reach_detail {
inline float dot(const float* a,const float* b){return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];}
inline float norm(const float* a){return std::sqrt(dot(a,a));}
inline void cross(const float* a,const float* b,float* o){o[0]=a[1]*b[2]-a[2]*b[1];o[1]=a[2]*b[0]-a[0]*b[2];o[2]=a[0]*b[1]-a[1]*b[0];}
// Minimal rotation taking unit direction a onto unit direction b, applied to v.
inline void rotateMinimal(const float* a,const float* b,const float* v,float* out){
    float axis[3]{};cross(a,b,axis);const float s=norm(axis),c=dot(a,b);
    if(s<1e-7f){for(unsigned k=0;k<3;++k)out[k]=v[k];return;} // parallel: no rotation (antiparallel never occurs for clamped reaches)
    for(unsigned k=0;k<3;++k)axis[k]/=s;
    float x[3]{};cross(axis,v,x);const float d=dot(axis,v);
    for(unsigned k=0;k<3;++k)out[k]=v[k]*c+x[k]*s+axis[k]*d*(1-c);
}
inline void rotateMatrix(const float* a,const float* b,BodyMatrix& m){
    for(unsigned col=0;col<3;++col){float v[3]{m.v[col*4],m.v[col*4+1],m.v[col*4+2]},o[3]{};rotateMinimal(a,b,v,o);for(unsigned k=0;k<3;++k)m.v[col*4+k]=o[k];}
}
}
inline bool movementArmReach(const BodyMatrix& parent,const BodyMatrix* chain,const float* target,float weight,BodyMatrix* local,float& reachError,unsigned* failure=nullptr){
    using namespace arm_reach_detail;
    reachError=0;
    const auto fail=[&](unsigned line){if(failure)*failure=line;return false;};
    if(!bodyRigid(parent)||!std::isfinite(weight))return fail(__LINE__);
    for(unsigned i=0;i<3;++i)if(!bodyRigid(chain[i]))return fail(__LINE__);
    for(unsigned k=0;k<3;++k)if(!std::isfinite(target[k]))return fail(__LINE__);
    weight=(std::max)(0.f,(std::min)(1.f,weight));
    if(weight<=0){for(unsigned j=0;j<3;++j){local[j]=bodyMultiply(bodyInverse(j?chain[j-1]:parent),chain[j]);if(!bodyRigid(local[j]))return fail(__LINE__);}return true;}
    const float* S=chain[0].v+12;const float* E=chain[1].v+12;const float* W=chain[2].v+12;
    float se[3]{},ew[3]{},sw[3]{},st[3]{};
    for(unsigned k=0;k<3;++k){se[k]=E[k]-S[k];ew[k]=W[k]-E[k];sw[k]=W[k]-S[k];st[k]=target[k]-S[k];}
    const float a=norm(se),b=norm(ew),dw=norm(sw),dt=norm(st);
    if(a<1e-4f||b<1e-4f||dw<1e-4f||dt<1e-4f)return fail(__LINE__);
    // Desired wrist: blend from the authored wrist toward the clamped target.
    const float lo=std::fabs(a-b)+.01f,hi=a+b-.01f;
    const float reach=(std::max)(lo,(std::min)(hi,dt));
    float want[3]{};for(unsigned k=0;k<3;++k)want[k]=S[k]+st[k]/dt*reach;
    for(unsigned k=0;k<3;++k)want[k]=W[k]+(want[k]-W[k])*weight;
    // The blend of two reachable points can fall just outside the chain's range (an authored straight
    // arm sits at full extension): clamp onto the reachable shell instead of refusing.
    float sw2[3]{};for(unsigned k=0;k<3;++k)sw2[k]=want[k]-S[k];const float raw=norm(sw2);
    if(raw<1e-5f)return fail(__LINE__);
    const float d=(std::max)(std::fabs(a-b)+1e-4f,(std::min)(a+b-1e-5f,raw));
    float u[3]{};for(unsigned k=0;k<3;++k){u[k]=sw2[k]/raw;want[k]=S[k]+u[k]*d;}
    // Bend plane: the authored elbow offset from the authored shoulder-wrist line, carried to the new line.
    float uOld[3]{};for(unsigned k=0;k<3;++k)uOld[k]=sw[k]/dw;
    float pole[3]{};const float along=dot(se,uOld);for(unsigned k=0;k<3;++k)pole[k]=se[k]-along*uOld[k];
    if(norm(pole)<1e-4f){float fallback[3]{0,0,-1};const float f=dot(fallback,uOld);for(unsigned k=0;k<3;++k)pole[k]=fallback[k]-f*uOld[k];}
    {float carried[3]{};rotateMinimal(uOld,u,pole,carried);const float c=dot(carried,u);for(unsigned k=0;k<3;++k)pole[k]=carried[k]-c*u[k];}
    const float pn=norm(pole);if(pn<1e-5f)return fail(__LINE__);for(unsigned k=0;k<3;++k)pole[k]/=pn;
    const float cosA=(std::max)(-1.f,(std::min)(1.f,(a*a+d*d-b*b)/(2*a*d))),sinA=std::sqrt((std::max)(0.f,1-cosA*cosA));
    float newE[3]{};for(unsigned k=0;k<3;++k)newE[k]=S[k]+a*(cosA*u[k]+sinA*pole[k]);
    // Segment rotations.
    float seU[3]{},seN[3]{};for(unsigned k=0;k<3;++k){seU[k]=se[k]/a;seN[k]=(newE[k]-S[k])/a;}
    BodyMatrix out[3]{chain[0],chain[1],chain[2]};
    rotateMatrix(seU,seN,out[0]);rotateMatrix(seU,seN,out[1]);rotateMatrix(seU,seN,out[2]);
    float ewR[3]{};rotateMinimal(seU,seN,ew,ewR);float ewU[3]{},ewN[3]{};
    for(unsigned k=0;k<3;++k){ewU[k]=ewR[k]/b;ewN[k]=(want[k]-newE[k])/b;}
    const float ewNn=norm(ewN);if(ewNn<.9f||ewNn>1.1f)return fail(__LINE__);for(unsigned k=0;k<3;++k)ewN[k]/=ewNn;
    rotateMatrix(ewU,ewN,out[1]);rotateMatrix(ewU,ewN,out[2]);
    for(unsigned k=0;k<3;++k){out[0].v[12+k]=S[k];out[1].v[12+k]=newE[k];out[2].v[12+k]=newE[k]+ewN[k]*b;}
    float err=0;for(unsigned k=0;k<3;++k){const float e=out[2].v[12+k]-want[k];err+=e*e;}
    for(unsigned k=0;k<3;++k){const float e=out[2].v[12+k]-target[k];reachError+=e*e;}reachError=std::sqrt(reachError);
    if(std::sqrt(err)>.001f)return fail(__LINE__);
    for(unsigned j=0;j<3;++j){local[j]=bodyMultiply(bodyInverse(j?out[j-1]:parent),out[j]);if(!bodyRigid(local[j]))return fail(__LINE__);}
    return true;
}
