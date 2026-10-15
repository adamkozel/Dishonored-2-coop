#pragma once
#include "world_protocol.h"
#include "body_retarget.h"
#include "pose_interpolation.h"
#include "door_session_scope.h"
#include <cstddef>

constexpr unsigned kNpcAuthorityMaxJoints=256;
// 1213u (#46): PairedHold/PairedKnockout come from the client while ITS native
// paired takedown (rear choke variants 1-3, fast-back lethal 5, carried in
// reserved) targets a host-owned NPC: the host holds that NPC's AI for the scene,
// and knocks it out natively when the client's choke_win begins.
enum class NpcAuthorityMessage : unsigned {Hello=1,Ready,Pose,Stop,PairedHold,PairedKnockout};
inline bool npcPairedVariant(std::uint32_t v){return (v>=1&&v<=3)||v==5;}
#pragma pack(push,1)
struct NpcAuthorityHeader {
    std::uint32_t magic=0x414e3244,build=0x67a50cde;
    std::uint16_t version=2,size=0;
    std::uint32_t role=0;
    NpcAuthorityMessage kind=NpcAuthorityMessage::Hello;
    std::uint32_t reserved=0;
    std::uint64_t sequence=0,sourceSession=0,targetSession=0,sourceEpoch=0,targetEpoch=0,map=0,sourceArm=0,targetArm=0;
    std::uint32_t sourceHandle=0,targetHandle=0;
    std::uint64_t rig=0;
    std::uint32_t joints=0,reserved2=0;
};
struct CoopNpcControl {
    NpcAuthorityHeader h{};
    char spawner[192]{},definition[192]{};
};
struct CoopNpcPose {
    NpcAuthorityHeader h{};
    std::uint64_t sampleUs=0;
    float root[12]{},health=0,maximum=0;
    float local[kNpcAuthorityMaxJoints][12]{};
};
struct NpcCompactJoint {std::int16_t quaternion[4]{},translation[3]{};};
#pragma pack(pop)
static_assert(sizeof(NpcAuthorityHeader)==112);
static_assert(sizeof(CoopNpcControl)==496);
static_assert(offsetof(CoopNpcPose,local)==176);
static_assert(sizeof(NpcCompactJoint)==14);
inline unsigned npcPoseBytes(unsigned joints){return joints&&joints<=kNpcAuthorityMaxJoints?176+joints*sizeof(NpcCompactJoint):0;}
// Presentation can expire while reliable actor heartbeats remain current.
// Dropping a skeletal sample must not transfer gameplay authority back to AI.
inline bool npcStreamPeerExpired(std::uint64_t now,std::uint64_t started,std::uint64_t peerAt,bool bound){
    return (!bound&&now>=started&&now-started>5000)||(peerAt&&now>=peerAt&&now-peerAt>1500);
}
inline bool npcStreamPoseStale(std::uint64_t now,std::uint64_t started,std::uint64_t poseAt){
    const auto base=poseAt?poseAt:started;return now>=base&&now-base>(poseAt?500u:3000u);
}
inline bool validNpcHeader(const NpcAuthorityHeader& h,const CoopPosePacket& local,const CoopPosePacket& peer){
    const bool paired=h.kind==NpcAuthorityMessage::PairedHold||h.kind==NpcAuthorityMessage::PairedKnockout;
    if(h.magic!=0x414e3244||h.build!=0x67a50cde||h.version!=2||h.role!=peer.role||h.role>1||(paired?!npcPairedVariant(h.reserved):h.reserved!=0)||h.reserved2||
       !h.sequence||!h.sourceArm||!h.sourceHandle||!h.rig||!h.joints||h.joints>kNpcAuthorityMaxJoints||!coopWorldPairReady(local,peer)||
       h.sourceSession!=peer.session||h.targetSession!=local.session||h.sourceEpoch!=peer.worldEpoch||h.targetEpoch!=local.worldEpoch||h.map!=local.mapId)return false;
    switch(h.kind){
    case NpcAuthorityMessage::Hello:return h.size==sizeof(CoopNpcControl)&&!h.targetArm&&!h.targetHandle;
    case NpcAuthorityMessage::Ready:return h.size==sizeof(CoopNpcControl)&&h.role==1&&h.targetArm&&h.targetHandle;
    case NpcAuthorityMessage::Stop:return h.size==sizeof(CoopNpcControl)&&h.targetArm&&h.targetHandle;
    case NpcAuthorityMessage::Pose:return h.size==npcPoseBytes(h.joints)&&h.role==0&&h.targetArm&&h.targetHandle;
    case NpcAuthorityMessage::PairedHold:return h.size==sizeof(CoopNpcControl)&&h.role==1&&h.targetArm&&h.targetHandle;
    case NpcAuthorityMessage::PairedKnockout:return h.size==sizeof(CoopNpcControl)&&h.role==1&&h.targetArm&&h.targetHandle&&h.reserved<=3;
    }return false;
}
inline bool validNpcControl(const CoopNpcControl& p,const CoopPosePacket& local,const CoopPosePacket& peer){
    return p.h.kind!=NpcAuthorityMessage::Pose&&validNpcHeader(p.h,local,peer)&&coopWorldName(p.spawner)&&coopWorldName(p.definition);
}
inline BodyMatrix npcWireMatrix(const float* p){BodyMatrix m{};for(unsigned c=0;c<4;++c)for(unsigned r=0;r<3;++r)m.v[c*4+r]=p[c*3+r];return m;}
using NpcPoseWire=std::array<unsigned char,176+kNpcAuthorityMaxJoints*sizeof(NpcCompactJoint)>;
inline bool encodeNpcPose(const CoopNpcPose& p,NpcPoseWire& bytes){
    if(p.h.version!=2||p.h.kind!=NpcAuthorityMessage::Pose||!npcPoseBytes(p.h.joints)||p.h.size!=npcPoseBytes(p.h.joints))return false;
    std::memcpy(bytes.data(),&p,176);
    for(unsigned j=0;j<p.h.joints;++j){
        if(!bodyRigid(npcWireMatrix(p.local[j]),3.f))return false;
        float q[4]{};presentationQuaternion(p.local[j],q);float norm=0;for(float v:q)norm+=v*v;if(!std::isfinite(norm)||norm<.98f||norm>1.02f)return false;
        norm=1.f/std::sqrt(norm);NpcCompactJoint joint{};
        for(unsigned i=0;i<4;++i)joint.quaternion[i]=static_cast<std::int16_t>(std::lround(q[i]*norm*32767.f));
        for(unsigned i=0;i<3;++i)joint.translation[i]=static_cast<std::int16_t>(std::lround(p.local[j][9+i]*10000.f));
        std::memcpy(bytes.data()+176+j*sizeof(joint),&joint,sizeof(joint));
    }return true;
}
inline bool decodeNpcPose(const void* bytes,std::size_t size,CoopNpcPose& out){
    if(!bytes||size<176||size>sizeof(NpcPoseWire))return false;
    CoopNpcPose p{};std::memcpy(&p,bytes,176);
    if(p.h.magic!=0x414e3244||p.h.version!=2||p.h.kind!=NpcAuthorityMessage::Pose||size!=npcPoseBytes(p.h.joints)||p.h.size!=size)return false;
    for(unsigned j=0;j<p.h.joints;++j){NpcCompactJoint joint{};std::memcpy(&joint,static_cast<const unsigned char*>(bytes)+176+j*sizeof(joint),sizeof(joint));
        float q[4]{},norm=0;for(unsigned i=0;i<4;++i){if(joint.quaternion[i]==-32768)return false;q[i]=joint.quaternion[i]/32767.f;norm+=q[i]*q[i];}
        if(norm<.999f||norm>1.001f)return false;norm=1.f/std::sqrt(norm);for(float& v:q)v*=norm;
        const auto x=q[0],y=q[1],z=q[2],w=q[3];const float r[]={1-2*(y*y+z*z),2*(x*y+z*w),2*(x*z-y*w),2*(x*y-z*w),1-2*(x*x+z*z),2*(y*z+x*w),2*(x*z+y*w),2*(y*z-x*w),1-2*(x*x+y*y)};
        std::memcpy(p.local[j],r,sizeof(r));for(unsigned i=0;i<3;++i){if(std::abs(static_cast<int>(joint.translation[i]))>30000)return false;p.local[j][9+i]=joint.translation[i]/10000.f;}
    }out=p;return true;
}
inline bool validNpcPose(const CoopNpcPose& p,const CoopPosePacket& local,const CoopPosePacket& peer){
    if(p.h.kind!=NpcAuthorityMessage::Pose||!validNpcHeader(p.h,local,peer)||!p.sampleUs||p.sampleUs>9007199254740991ull||
       !std::isfinite(p.health)||!std::isfinite(p.maximum)||p.health<0||p.maximum<=0||p.health>p.maximum||p.maximum>1000000)return false; // 1113: corpse poses carry health0
    CoopPosePacket root=peer;std::memcpy(root.position,p.root,12);std::memcpy(root.orientation,p.root+3,36);
    if(!validCoopPose(root,local.role))return false;
    for(unsigned j=0;j<p.h.joints;++j)if(!bodyRigid(npcWireMatrix(p.local[j]),3.f))return false;
    return true;
}
inline std::uint64_t npcAuthorityRig(const NativeSkeleton& s){
    const auto n=s.parents.size();if(!n||n>kNpcAuthorityMaxJoints||s.bindLocal.size()!=n||s.localPose.size()!=n||!s.modelName[0]||!s.skeletonName[0])return 0;
    auto hash=coopMapId(s.skeletonName);const auto byte=[&](unsigned char b){hash^=b;hash*=1099511628211ULL;};
    for(const char* p=s.modelName;*p;++p)byte(static_cast<unsigned char>(*p));
    for(unsigned j=0;j<n;++j){const int parent=s.parents[j];if(parent<-1||parent>=static_cast<int>(j))return 0;
        byte(static_cast<unsigned char>(parent));byte(static_cast<unsigned char>(parent>>8));BodyMatrix bind{};if(!bodyFromQts(s.bindLocal[j],bind))return 0;
        for(unsigned i=0;i<11;++i)if(i!=7){std::uint32_t bits=0;std::memcpy(&bits,&s.bindLocal[j][i],4);for(unsigned k=0;k<4;++k)byte(static_cast<unsigned char>(bits>>(k*8)));}
    }return hash;
}
struct NpcPoseSample {double at=0;CoopNpcPose pose{};};
struct NpcPoseBuffer {
    std::array<NpcPoseSample,8> samples{};unsigned count=0;SourcePoseTimeline clock{};std::uint64_t replayFloor=0;
    void resume(){count=0;clock={};} // Preserve replay floor while dropping pre-menu interpolation.
    bool push(const CoopNpcPose& p,double arrival){
        if(p.h.sequence<=replayFloor)return false;
        auto trial=clock;double at=0;if(!trial.mapTime(p.sampleUs,arrival,p.h.sourceSession,p.h.sourceEpoch,p.h.map,at))return false;
        if(count){const auto& last=samples[count-1];float distance=0;for(unsigned i=0;i<3;++i){const auto d=last.pose.root[i]-p.root[i];distance+=d*d;}
            if(at-last.at>250.||distance>9.f)count=0;}
        if(count==samples.size()){for(unsigned i=1;i<count;++i)samples[i-1]=samples[i];--count;}
        samples[count++]={at,p};clock=trial;replayFloor=p.h.sequence;return true;
    }
    bool sample(double now,CoopNpcPose& out)const{
        if(!count||now-samples[count-1].at>350.)return false;
        const double target=now-80.;unsigned upper=0;while(upper<count&&samples[upper].at<target)++upper;
        if(!upper){out=samples[0].pose;return true;}if(upper==count){out=samples[count-1].pose;return true;}
        const auto& a=samples[upper-1];const auto& b=samples[upper];out=b.pose;
        if(a.pose.h.rig!=b.pose.h.rig||a.pose.h.joints!=b.pose.h.joints||b.at<=a.at)return false;
        const float t=static_cast<float>((target-a.at)/(b.at-a.at));
        for(unsigned i=0;i<3;++i)out.root[i]=a.pose.root[i]+t*(b.pose.root[i]-a.pose.root[i]);
        presentationRotation(a.pose.root+3,b.pose.root+3,t,out.root+3);
        for(unsigned j=0;j<out.h.joints;++j){presentationRotation(a.pose.local[j],b.pose.local[j],t,out.local[j]);
            for(unsigned i=9;i<12;++i)out.local[j][i]=a.pose.local[j][i]+t*(b.pose.local[j][i]-a.pose.local[j][i]);}
        return true;
    }
};
