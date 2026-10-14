#pragma once
#include "native_skeleton.h"
#include <functional>
#include <algorithm>
// Native SIMD affine storage: only XYZ of each column is meaningful. The
// fourth lanes may contain scratch values; do not use 4x4 matrix products.
struct alignas(16) BodyMatrix {float v[16]{};};
inline BodyMatrix bodyIdentity(){BodyMatrix m{};m.v[0]=m.v[5]=m.v[10]=1;return m;}
inline BodyMatrix bodyMultiply(const BodyMatrix& a,const BodyMatrix& b){
    BodyMatrix out{};for(unsigned c=0;c<4;++c)for(unsigned r=0;r<3;++r){out.v[c*4+r]=c==3?a.v[12+r]:0;
        for(unsigned k=0;k<3;++k)out.v[c*4+r]+=a.v[k*4+r]*b.v[c*4+k];}return out;
}
inline BodyMatrix bodyInverse(const BodyMatrix& a){
    BodyMatrix out{};for(unsigned c=0;c<3;++c)for(unsigned r=0;r<3;++r)out.v[c*4+r]=a.v[r*4+c];
    for(unsigned r=0;r<3;++r)for(unsigned k=0;k<3;++k)out.v[12+r]-=out.v[k*4+r]*a.v[12+k];return out;
}
inline bool bodyRigid(const BodyMatrix& m,float translationLimit=10.f){
    for(unsigned c=0;c<4;++c)for(unsigned r=0;r<3;++r)if(!std::isfinite(m.v[c*4+r])||std::fabs(m.v[c*4+r])>(c==3?translationLimit:1.01f))return false;
    for(unsigned a=0;a<3;++a)for(unsigned b=a;b<3;++b){float dot=0;for(unsigned r=0;r<3;++r)dot+=m.v[a*4+r]*m.v[b*4+r];if(std::fabs(dot-(a==b?1.f:0.f))>.01f)return false;}
    const auto* a=m.v;const float det=a[0]*(a[5]*a[10]-a[6]*a[9])-a[4]*(a[1]*a[10]-a[2]*a[9])+a[8]*(a[1]*a[6]-a[2]*a[5]);
    return det>.99f&&det<1.01f;
}
inline bool bodyFromQts(const std::array<float,12>& q,BodyMatrix& out){
    for(float f:q)if(!std::isfinite(f))return false;
    for(unsigned i=8;i<11;++i)if(std::fabs(q[i]-1.f)>.001f)return false;
    float x=q[0],y=q[1],z=q[2],w=q[3];const float n=std::sqrt(x*x+y*y+z*z+w*w);
    if(n<.99f||n>1.01f)return false;x/=n;y/=n;z/=n;w/=n;
    out={{1-2*(y*y+z*z),2*(x*y+z*w),2*(x*z-y*w),0,2*(x*y-z*w),1-2*(x*x+z*z),2*(y*z+x*w),0,
        2*(x*z+y*w),2*(y*z-x*w),1-2*(x*x+y*y),0,q[4],q[5],q[6],0}};return bodyRigid(out);
}
inline bool bodyForward(const std::vector<std::int16_t>& parents,const std::vector<BodyMatrix>& local,std::vector<BodyMatrix>& global){
    if(parents.empty()||parents.size()!=local.size()||parents.size()>256)return false;
    global.resize(local.size());std::vector<unsigned char> visited(local.size());
    std::function<bool(unsigned)> joint=[&](unsigned j){if(visited[j]==2)return true;if(visited[j]==1||!bodyRigid(local[j]))return false;visited[j]=1;
        const int parent=parents[j];if(parent>=static_cast<int>(local.size())||parent<-1)return false;
        if(parent>=0){if(!joint(parent))return false;global[j]=bodyMultiply(global[parent],local[j]);}else global[j]=local[j];visited[j]=2;return bodyRigid(global[j]);};
    for(unsigned j=0;j<local.size();++j)if(!joint(j))return false;return true;
}
struct BodyRetarget {
    std::vector<std::int16_t> parents;
    std::vector<BodyMatrix> targetBindLocal,targetBindGlobal,sourceBindGlobal;
    std::vector<int> sourceForTarget;
};
inline bool prepareBodyRetarget(const NativeSkeleton& source,const NativeSkeleton& target,BodyRetarget& out){
    // Exact player-body layout recovered from the local game asset. Native
    // name IDs are compared only within this process, never sent as wire IDs.
    constexpr std::int16_t sourceParents[]={-1,0,1,2,3,4,5,6,6,1,9,10,11,12,1,14,15,16,17,0};
    if(source.jointIds.size()!=20||source.parents.size()!=20||source.bindLocal.size()!=20||target.jointIds.size()>256||
       target.jointIds.size()!=target.parents.size()||target.bindLocal.size()!=target.parents.size()||
       !std::equal(source.parents.begin(),source.parents.end(),sourceParents))return false;
    BodyRetarget r{};r.parents=target.parents;r.targetBindLocal.resize(target.parents.size());std::vector<BodyMatrix> src(20);
    for(unsigned j=0;j<20;++j)if(!bodyFromQts(source.bindLocal[j],src[j]))return false;
    for(unsigned j=0;j<target.parents.size();++j)if(!bodyFromQts(target.bindLocal[j],r.targetBindLocal[j]))return false;
    if(!bodyForward(source.parents,src,r.sourceBindGlobal)||!bodyForward(target.parents,r.targetBindLocal,r.targetBindGlobal))return false;
    std::unordered_set<unsigned> matched;r.sourceForTarget.resize(target.parents.size(),-1);
    for(unsigned j=0;j<target.parents.size();++j)for(unsigned k=1;k<19;++k)if(k!=8&&source.jointIds[k]==target.jointIds[j]){
        if(!matched.insert(k).second)return false;r.sourceForTarget[j]=static_cast<int>(k);
    }
    if(matched.size()!=17)return false;out=std::move(r);return true;
}
// The clone keeps its native spine, head, arms, fingers and sword poses.
// Only center_root and both five-joint leg chains supplement native playback.
inline bool restrictBodyToLower(BodyRetarget& r){
    unsigned matched=0;
    for(int& src:r.sourceForTarget){
        if(src==1||(src>=9&&src<=18))++matched;else src=-1;
    }
    return matched==11;
}
inline bool retargetBody(const BodyRetarget& r,const std::array<BodyMatrix,20>& sourcePose,const std::vector<BodyMatrix>& nativeLocal,std::vector<BodyMatrix>& local){
    const auto n=r.parents.size();if(n>256||nativeLocal.size()!=n||r.sourceForTarget.size()!=n||r.sourceBindGlobal.size()!=20||r.targetBindGlobal.size()!=n||r.targetBindLocal.size()!=n)return false;
    for(const auto& m:sourcePose)if(!bodyRigid(m))return false;
    local=nativeLocal;std::vector<BodyMatrix> global(n);std::vector<unsigned char> visited(n);
    std::function<bool(unsigned)> joint=[&](unsigned j){if(visited[j]==2)return true;if(visited[j]==1||!bodyRigid(local[j]))return false;visited[j]=1;
        const int parent=r.parents[j];if(parent<-1||parent>=static_cast<int>(n)||(parent>=0&&!joint(parent)))return false;
        const int src=r.sourceForTarget[j];if(src<-1||src>=20)return false;
        if(src>=0){const auto wanted=bodyMultiply(bodyMultiply(sourcePose[src],bodyInverse(r.sourceBindGlobal[src])),r.targetBindGlobal[j]);
            const auto relative=parent>=0?bodyMultiply(bodyInverse(global[parent]),wanted):wanted;
            for(unsigned c=0;c<3;++c)for(unsigned v=0;v<3;++v)local[j].v[c*4+v]=relative.v[c*4+v];
            for(unsigned v=0;v<3;++v)local[j].v[12+v]=r.targetBindLocal[j].v[12+v];
            if(src==1)for(unsigned v=0;v<3;++v)local[j].v[12+v]+=sourcePose[src].v[12+v]-r.sourceBindGlobal[src].v[12+v];
        }
        global[j]=parent>=0?bodyMultiply(global[parent],local[j]):local[j];visited[j]=2;return bodyRigid(local[j])&&bodyRigid(global[j]);
    };
    for(unsigned j=0;j<n;++j)if(!joint(j))return false;return true;
}
