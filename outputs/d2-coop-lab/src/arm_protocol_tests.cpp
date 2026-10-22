#include "arm_protocol.h"
#include <iostream>
#include <limits>
#include <cstdlib>
void check(bool ok,const char* message){if(!ok){std::cerr<<"FAIL: "<<message<<'\n';std::exit(1);}}
int main(){
    CoopPosePacket h{},c{};h.role=0;c.role=1;h.sequence=c.sequence=100;h.flags=c.flags=1;h.session=11;c.session=22;h.sampleTimeUs=c.sampleTimeUs=1000000;
    h.worldEpoch=3;c.worldEpoch=7;h.mapId=c.mapId=99;h.gameLogicState=c.gameLogicState=1;h.cinematicMetadataValid=c.cinematicMetadataValid=1;
    NativeSkeleton rig{};strcpy_s(rig.skeletonName,"models/characters/player/emily/player_rightarm_emily.md6skl");rig.playerArm.side=2;
    rig.parents.resize(69);rig.bindLocal.resize(69);
    for(unsigned j=0;j<69;++j){rig.parents[j]=static_cast<std::int16_t>(j)-1;rig.bindLocal[j][3]=1;for(unsigned n=8;n<12;++n)rig.bindLocal[j][n]=1;}
    rig.parents[59]=4;const auto id=nativeArmRig(rig);check(id!=0,"known right arm rig");
    auto other=rig;other.jointIds.resize(69,99);other.playerArm.character=2;check(nativeArmRig(other)==id,"local names and role do not become wire rig identity");
    other=rig;other.bindLocal[2][4]=.1f;check(nativeArmRig(other)!=id,"different bind changes rig");
    other=rig;other.parents[4]=1;check(!nativeArmRig(other),"wrong core hierarchy");
    other=rig;other.parents[20]=20;check(!nativeArmRig(other),"cycle rejected");
    other=rig;other.playerArm.side=1;check(!nativeArmRig(other),"left arm refused by right arm protocol");
    other=rig;other.bindLocal[2][8]=2;check(!nativeArmRig(other),"scaled rig refused");
    CoopArmPacket p{};p.sequence=2;p.sourceSession=11;p.targetSession=22;p.sourceWorldEpoch=3;p.targetWorldEpoch=7;p.mapId=99;p.rig=id;p.rootSequence=99;p.character=1;
    for(auto& m:p.global)m[0]=m[4]=m[8]=1;
    check(validCoopArm(p,c,h),"Corvo to Emily observer");
    auto reverse=p;reverse.role=1;reverse.character=2;reverse.sourceSession=22;reverse.targetSession=11;reverse.sourceWorldEpoch=7;reverse.targetWorldEpoch=3;
    check(validCoopArm(reverse,h,c),"Emily to Corvo observer");
    auto bad=p;bad.character=3;check(!validCoopArm(bad,c,h),"incorrect character");
    bad=p;bad.side=1;check(!validCoopArm(bad,c,h),"incorrect arm side");
    bad=p;bad.rig=0;check(!validCoopArm(bad,c,h),"missing rig");
    bad=p;bad.sourceWorldEpoch++;check(!validCoopArm(bad,c,h),"changed source world");
    bad=p;bad.targetWorldEpoch++;check(!validCoopArm(bad,c,h),"changed receiver world");
    bad=p;bad.sourceSession++;check(!validCoopArm(bad,c,h),"restarted source");
    bad=p;bad.targetSession++;check(!validCoopArm(bad,c,h),"restarted receiver");
    bad=p;bad.mapId++;check(!validCoopArm(bad,c,h),"different map");
    bad=p;bad.rootSequence=91;check(!validCoopArm(bad,c,h),"stale root");
    bad=p;bad.rootSequence=101;check(!validCoopArm(bad,c,h),"future root");
    bad=p;bad.global[4][11]=4;check(!validCoopArm(bad,c,h),"bounded final joint");
    bad=p;bad.global[3][7]=std::numeric_limits<float>::quiet_NaN();check(!validCoopArm(bad,c,h),"nonfinite pose");
    bad=p;bad.global[1][0]=-1;check(!validCoopArm(bad,c,h),"reflected pose");
    bad=p;bad.global[1][0]=2;check(!validCoopArm(bad,c,h),"scaled pose");
    bad=p;bad.version++;check(!validCoopArm(bad,c,h),"unknown protocol");
    bad=p;bad.size--;check(!validCoopArm(bad,c,h),"truncated packet");
    bad=p;bad.spaceMode=2;check(!validCoopArm(bad,c,h),"unknown coordinate mode");
    bad=p;bad.spaceMode=1;check(validCoopArm(bad,c,h),"explicit player-space experiment");
    bad=p;bad.version=2;check(!validCoopArm(bad,c,h),"prior raw-space protocol rejected");
    bad=p;bad.version=1;check(!validCoopArm(bad,c,h),"old five-joint packet cannot drive fingers");
    bad=p;bad.global[20][0]=2;check(!validCoopArm(bad,c,h),"last finger matrix must remain rigid");
    auto paused=c;paused.gameLogicState=2;check(!validCoopArm(p,paused,h),"receiver pause");
    paused=h;paused.gameLogicState=5;check(!validCoopArm(p,c,paused),"source cinematic");
    paused=h;paused.pendingGameLogicState=15;check(!validCoopArm(p,c,paused),"pending load");
    check(!armSequenceFresh(2,11,11,2)&&!armSequenceFresh(1,11,11,2)&&armSequenceFresh(3,11,11,2)&&armSequenceFresh(1,12,11,2),"replay, reordering and fresh process");
    check(armRootFresh(0xffffffffu,1)&&!armRootFresh(0,1)&&!armRootFresh(1,0xffffffffu),"root wrap and future validation");
    std::cout<<"PASS: right-arm protocol roles, rig identity, fresh root, session/world scope, pause/loading, replay and rigid poses\n";
}
