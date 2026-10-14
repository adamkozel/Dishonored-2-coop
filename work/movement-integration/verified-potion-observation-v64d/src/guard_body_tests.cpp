#include "guard_body_protocol.h"
#include <iostream>
#include <limits>
#include <cstdlib>
void check(bool ok,const char* message){if(!ok){std::cerr<<"FAIL: "<<message<<'\n';std::exit(1);}}
int main(){
    CoopPosePacket h{},c{};h.role=0;c.role=1;h.sequence=c.sequence=1;h.flags=c.flags=1;h.session=11;c.session=22;h.sampleTimeUs=c.sampleTimeUs=1000000;
    h.worldEpoch=3;c.worldEpoch=7;h.mapId=c.mapId=coopMapId(kGuardMap);h.gameLogicState=c.gameLogicState=1;
    h.cinematicMetadataValid=c.cinematicMetadataValid=1;h.cinematicEpoch=c.cinematicEpoch=1;
    h.orientation[0]=h.orientation[4]=h.orientation[8]=c.orientation[0]=c.orientation[4]=c.orientation[8]=1;
    NativeSkeleton rig{};std::memcpy(rig.skeletonName,kGuardBodySkeleton,std::strlen(kGuardBodySkeleton)+1);rig.parents.resize(kGuardBodyJoints);rig.bindLocal.resize(kGuardBodyJoints);
    for(unsigned j=0;j<kGuardBodyJoints;++j){rig.parents[j]=static_cast<std::int16_t>(j)-1;rig.bindLocal[j][3]=1;for(unsigned i=8;i<12;++i)rig.bindLocal[j][i]=1;}
    const auto id=guardBodyRig(rig);check(id!=0,"known rig has bounded asset identity");
    auto other=rig;other.jointIds.resize(kGuardBodyJoints,99);check(guardBodyRig(other)==id,"process-local interned names do not alter wire rig");
    other=rig;other.bindLocal[10][4]=.1f;check(guardBodyRig(other)!=id,"different bind translation changes rig identity");
    other=rig;other.parents[10]=10;check(!guardBodyRig(other),"cyclic or forward hierarchy refused");
    other=rig;other.parents.pop_back();check(!guardBodyRig(other),"different joint count refused");
    other=rig;other.skeletonName[0]='x';check(!guardBodyRig(other),"unknown rig asset refused");
    CoopGuardBodyPacket p{};p.rig=id;auto& b=p.binding;b.mode=1;b.kind=GuardMessage::Pose;b.sequence=1;b.binding=4;b.request=5;
    b.hostSession=11;b.clientSession=22;b.hostEpoch=3;b.clientEpoch=7;b.mapId=h.mapId;b.entityKey=guardEntityKey(1);b.hostHandle=0x10003;b.clientHandle=0x70051;
    b.healthValid=1;b.health=b.maximumHealth=40;b.orientation[0]=b.orientation[4]=b.orientation[8]=1;
    for(auto& joint:p.local)joint[0]=joint[4]=joint[8]=1;
    check(validGuardBody(p,c,h),"fixed-rig native local transforms accepted for bound client");
    auto bad=p;bad.local[222][11]=4;check(!validGuardBody(bad,c,h),"last joint cannot exceed translation bound");
    bad=p;bad.local[120][3]=std::numeric_limits<float>::quiet_NaN();check(!validGuardBody(bad,c,h),"nonfinite joint refused");
    bad=p;bad.local[80][0]=-1;check(!validGuardBody(bad,c,h),"reflected bone refused");
    bad=p;bad.local[80][0]=2;check(!validGuardBody(bad,c,h),"scaled bone refused");
    bad=p;bad.local[2][4]=0;check(!validGuardBody(bad,c,h),"singular bone refused");
    bad=p;bad.binding.mode=0;bad.binding.entityKey=guardEntityKey();check(!validGuardBody(bad,c,h),"inactive guard mode cannot receive combat animation");
    bad=p;bad.binding.hostEpoch++;check(!validGuardBody(bad,c,h),"world replacement refuses old animation");
    bad=p;bad.binding.hostSession++;check(!validGuardBody(bad,c,h),"restarted host refuses old animation");
    bad=p;bad.binding.binding++;check(!guardSameBinding(bad.binding,p.binding),"new binding cannot reuse prior animation");
    bad=p;bad.version++;check(!validGuardBody(bad,c,h),"unknown protocol refused");
    bad=p;bad.size--;check(!validGuardBody(bad,c,h),"truncated payload refused");
    bad=p;bad.rig=0;check(!validGuardBody(bad,c,h),"missing rig refused");
    check(!validGuardBody(p,h,c),"host does not accept client guard animation");
    auto paused=c;paused.gameLogicState=2;check(!validGuardBody(p,paused,h),"pause refuses guard animation");
    std::cout<<"PASS: guard animation fixed rig, local-name independence, native binding, phase, finite rigid joints and malformed payloads\n";
}
