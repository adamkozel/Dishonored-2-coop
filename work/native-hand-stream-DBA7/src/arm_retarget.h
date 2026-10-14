#pragma once
#include "body_retarget.h"

// The two observed first-person rigs have separate fingers/deformation joints.
// Start with their five shared arm/weapon joints only. Runtime name IDs are
// matched inside one process; these IDs are never a network bone identity.
constexpr unsigned kArmCoreJoints[]={1,2,3,4,59};
constexpr unsigned kArmStreamJoints[]={1,2,3,4,59,5,6,7,16,17,18,27,28,29,38,39,40,48,49,50,51};
constexpr unsigned kArmStreamJointCount=21;
struct ArmRetarget {
    std::uint32_t side=0,count=5;
    std::vector<std::int16_t> parents;
    std::vector<BodyMatrix> targetBindLocal,targetBindGlobal;
    std::array<BodyMatrix,kArmStreamJointCount> sourceBindGlobal{};
    std::array<unsigned,kArmStreamJointCount> targets{};
};
inline bool prepareArmRetarget(const NativeSkeleton& source,const NativeSkeleton& target,ArmRetarget& out,bool fingers=false){
    const auto side=source.playerArm.side;
    if((side!=1&&side!=2)||source.jointIds.size()!=69||source.parents.size()!=69||source.bindLocal.size()!=69||
       std::strcmp(source.skeletonName,side==1?"models/characters/player/emily/player_leftarm_emily.md6skl":"models/characters/player/emily/player_rightarm_emily.md6skl")||
       (std::strcmp(target.skeletonName,"models/characters/female/emily/emily.md6skl")&&
        std::strcmp(target.skeletonName,"models/characters/tall/str/corvo/t_str_corvo01.md6skl"))||
       target.jointIds.size()<18||target.jointIds.size()>256||target.parents.size()!=target.jointIds.size()||target.bindLocal.size()!=target.parents.size())return false;
    if(source.parents[0]!=-1||source.parents[1]!=0||source.parents[2]!=1||source.parents[3]!=2||source.parents[4]!=3||source.parents[59]!=4)return false;
    ArmRetarget r{};r.side=side;r.parents=target.parents;r.targetBindLocal.resize(target.parents.size());
    std::vector<BodyMatrix> local(69),global;
    for(unsigned j=0;j<69;++j)if(!bodyFromQts(source.bindLocal[j],local[j]))return false;
    for(unsigned j=0;j<r.parents.size();++j)if(!bodyFromQts(target.bindLocal[j],r.targetBindLocal[j]))return false;
    if(!bodyForward(source.parents,local,global)||!bodyForward(target.parents,r.targetBindLocal,r.targetBindGlobal))return false;
    for(unsigned k=0;k<5;++k){
        const auto src=kArmCoreJoints[k];unsigned matches=0;
        for(unsigned j=0;j<target.jointIds.size();++j)if(source.jointIds[src]==target.jointIds[j]){r.targets[k]=j;++matches;}
        if(matches!=1)return false;r.sourceBindGlobal[k]=global[src];
        // Both observed third-person rigs use the same direct arm chain.
        if(k&&target.parents[r.targets[k]]!=static_cast<int>(r.targets[k-1]))return false;
        for(unsigned previous=0;previous<k;++previous)if(r.targets[previous]==r.targets[k])return false;
    }
    if(!r.targets[0]||target.parents[r.targets[0]]<0)return false;
    if(fingers){
        // The first-person index/middle/ring rigs add one metacarpal. Their
        // next three joints correspond to the NPC's three phalanges. Global
        // bind-space rotation includes that extra parent's motion without
        // importing its length. Pinky has four bones in both native rigs.
        if(side!=2||r.targets[3]!=16)return false;
        const bool corvo=!std::strcmp(target.skeletonName,"models/characters/tall/str/corvo/t_str_corvo01.md6skl");
        if(target.parents.size()!=(corvo?215u:219u)||source.parents[15]!=4||source.parents[26]!=4||source.parents[37]!=4)return false;
        const unsigned distal=corvo?63:60;
        const unsigned destination[]={38,39,distal,40,41,distal+2,42,43,distal+4,44,45,distal+6,46,47,48,distal+8};
        const int sourceParent[]={4,5,6,15,16,17,26,27,28,37,38,39,4,48,49,50};
        const int targetParent[]={16,38,39,16,40,41,16,42,43,16,44,45,16,46,47,48};
        for(unsigned i=0;i<16;++i){
            const unsigned k=i+5,src=kArmStreamJoints[k],dst=destination[i];
            if(source.parents[src]!=sourceParent[i]||target.parents[dst]!=targetParent[i])return false;
            for(unsigned p=0;p<k;++p)if(r.targets[p]==dst)return false;
            r.targets[k]=dst;r.sourceBindGlobal[k]=global[src];
        }
        r.count=kArmStreamJointCount;
    }
    out=std::move(r);return true;
}
template<std::size_t N> bool retargetArm(const ArmRetarget& rig,const std::array<BodyMatrix,N>& sourcePose,const std::vector<BodyMatrix>& nativeLocal,std::vector<BodyMatrix>& result){
    const auto n=rig.parents.size();
    if(n<18||n>256||nativeLocal.size()!=n||rig.targetBindLocal.size()!=n||rig.targetBindGlobal.size()!=n||
       (rig.count!=5&&rig.count!=kArmStreamJointCount)||rig.count>N)return false;
    for(unsigned k=0;k<rig.count;++k){
        if(rig.targets[k]>=n||!bodyRigid(sourcePose[k],3)||!bodyRigid(rig.sourceBindGlobal[k],3))return false;
        for(unsigned j=0;j<k;++j)if(rig.targets[k]==rig.targets[j])return false;
    }
    result=nativeLocal;std::vector<BodyMatrix> global(n);std::vector<unsigned char> visited(n);
    std::function<bool(unsigned)> visit=[&](unsigned j){
        if(visited[j]==2)return true;if(visited[j]==1||!bodyRigid(result[j]))return false;visited[j]=1;
        const int parent=rig.parents[j];if(parent<-1||parent>=static_cast<int>(n)||(parent>=0&&!visit(parent)))return false;
        for(unsigned k=0;k<rig.count;++k)if(rig.targets[k]==j){
            const auto wanted=bodyMultiply(bodyMultiply(sourcePose[k],bodyInverse(rig.sourceBindGlobal[k])),rig.targetBindGlobal[j]);
            const auto relative=parent>=0?bodyMultiply(bodyInverse(global[parent]),wanted):wanted;
            for(unsigned c=0;c<3;++c)for(unsigned axis=0;axis<3;++axis)result[j].v[c*4+axis]=relative.v[c*4+axis];
            // Keep the native target's local lengths and all other joints.
            // First-person camera translation is not a third-person shoulder.
        }
        global[j]=parent>=0?bodyMultiply(global[parent],result[j]):result[j];visited[j]=2;
        return bodyRigid(result[j])&&bodyRigid(global[j]);
    };
    for(unsigned j=0;j<n;++j)if(!visit(j))return false;return true;
}
