#include "movement_arm_reach.h"
#include <iostream>
#include <cstdlib>
void need(bool v,const char* message){if(!v){std::cerr<<message<<'\n';std::exit(1);}}
float distance(const float* a,const float* b){float d=0;for(unsigned k=0;k<3;++k)d+=(a[k]-b[k])*(a[k]-b[k]);return std::sqrt(d);}
BodyMatrix at(float x,float y,float z){BodyMatrix m=bodyIdentity();m.v[12]=x;m.v[13]=y;m.v[14]=z;return m;}
// Chain from the confirmed runtime remote-avatar rig (1232 probe): right arm 14 -> elbow 15 -> wrist 16,
// roughly (0.06,0.12,0.38) -> (0.04,0.18,0.13) -> (0.01,0.24,-0.11) in model space; parent = shoulder 13.
void solve(const BodyMatrix& parent,const BodyMatrix* chain,const float* target,float weight,BodyMatrix* out,float& error){
    BodyMatrix local[3]{};unsigned line=0;
    if(!movementArmReach(parent,chain,target,weight,local,error,&line))std::cerr<<"refused at line "<<line<<std::endl;
    need(line==0,"Valid reach refused");
    for(unsigned j=0;j<3;++j)out[j]=bodyMultiply(j?out[j-1]:parent,local[j]);
}
int main(){
    const BodyMatrix parent=at(0,.01f,.40f);
    const BodyMatrix chain[3]{at(.06f,.12f,.38f),at(.04f,.18f,.13f),at(.01f,.24f,-.11f)};
    const float a=distance(chain[0].v+12,chain[1].v+12),b=distance(chain[1].v+12,chain[2].v+12);
    BodyMatrix out[3]{};float error=0;
    // Reachable target in front of the chest: the wrist lands on it; the arm joint stays; lengths kept.
    const float front[3]{.45f,.2f,.2f};solve(parent,chain,front,1,out,error);
    need(error<.001f,"Wrist did not reach a reachable target");
    need(distance(out[0].v+12,chain[0].v+12)<.00001f,"Arm joint moved");
    need(std::fabs(distance(out[0].v+12,out[1].v+12)-a)<.0001f&&std::fabs(distance(out[1].v+12,out[2].v+12)-b)<.0001f,"Segment length changed");
    for(unsigned j=0;j<3;++j)need(bodyRigid(out[j]),"Non-rigid result");
    // Weight 0 keeps the authored chain exactly.
    solve(parent,chain,front,0,out,error);
    for(unsigned j=0;j<3;++j)need(distance(out[j].v+12,chain[j].v+12)<.00001f,"Weight 0 moved the chain");
    for(unsigned j=0;j<3;++j)for(unsigned k=0;k<12;++k)if(k%4!=3)need(std::fabs(out[j].v[k]-chain[j].v[k])<.00001f,"Weight 0 rotated the chain");
    // Half weight moves the wrist part of the way, still rigid and length-preserving.
    solve(parent,chain,front,.5f,out,error);
    const float half=distance(out[2].v+12,chain[2].v+12);need(half>.05f&&error>.05f,"Half weight did not blend");
    need(std::fabs(distance(out[1].v+12,out[2].v+12)-b)<.0001f,"Half weight changed the forearm length");
    // Unreachable target: full extension minus the margin along the target direction; no hyperextension.
    const float far[3]{1.5f,.2f,.3f};solve(parent,chain,far,1,out,error);
    need(std::fabs(distance(out[0].v+12,out[2].v+12)-(a+b-.01f))<.001f,"Unreachable target not clamped to extension");
    // The elbow keeps the authored bend side (the authored elbow bends away from +x in this rig).
    solve(parent,chain,front,1,out,error);
    float sw[3]{},se[3]{},seOld[3]{},swOld[3]{};
    for(unsigned k=0;k<3;++k){sw[k]=out[2].v[12+k]-out[0].v[12+k];se[k]=out[1].v[12+k]-out[0].v[12+k];swOld[k]=chain[2].v[12+k]-chain[0].v[12+k];seOld[k]=chain[1].v[12+k]-chain[0].v[12+k];}
    float n[3]{},nOld[3]{};arm_reach_detail::cross(sw,se,n);arm_reach_detail::cross(swOld,seOld,nOld);
    need(arm_reach_detail::dot(n,nOld)>0,"Elbow flipped to the other side");
    // The hand keeps its orientation relative to the forearm.
    const BodyMatrix relOld=bodyMultiply(bodyInverse(chain[1]),chain[2]),relNew=bodyMultiply(bodyInverse(out[1]),out[2]);
    for(unsigned k=0;k<12;++k)if(k%4!=3)need(std::fabs(relOld.v[k]-relNew.v[k])<.0005f,"Hand orientation relative to the forearm changed");
    // Degenerate inputs are refused.
    BodyMatrix local[3]{};const BodyMatrix zero[3]{chain[0],chain[0],chain[2]};
    need(!movementArmReach(parent,zero,front,1,local,error),"Zero-length upper arm accepted");
    const float nan[3]{std::nanf(""),0,0};need(!movementArmReach(parent,chain,nan,1,local,error),"Non-finite target accepted");
    std::cout<<"PASS_ARM_REACH: reach, fixed arm joint, lengths, weight 0/0.5, extension clamp, bend side, hand-forearm orientation, degenerate refusal\n";
}
