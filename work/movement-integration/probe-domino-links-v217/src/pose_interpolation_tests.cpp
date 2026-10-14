#include "pose_interpolation.h"
#include <iostream>
#include <cstdlib>
void check(bool c,const char* s){if(!c){std::cerr<<"FAIL "<<s<<'\n';std::exit(1);}}
PresentationPose pose(double t,unsigned seq,float x,float y=0,float z=0,float yaw=0){
    PresentationPose p{};p.time=t;p.sequence=seq;p.session=1;p.epoch=2;p.map=3;p.position[0]=x;p.position[1]=y;p.position[2]=z;
    p.orientation[0]=p.orientation[4]=std::cos(yaw);p.orientation[1]=std::sin(yaw);p.orientation[3]=-std::sin(yaw);p.orientation[8]=1;return p;
}
int main(){
    SourcePoseTimeline timeline;double mapped=0;
    check(timeline.mapTime(1000000,10000,1,2,3,mapped)&&mapped==10000,"map unrelated source/receiver clocks");
    check(timeline.mapTime(1033000,10070,1,2,3,mapped),"delayed source packet");const double previous=mapped;
    check(timeline.mapTime(1066000,10072,1,2,3,mapped)&&mapped-previous>32.9&&mapped-previous<33.1,"batched arrivals preserve source cadence instead of 2ms sprint step");
    check(!timeline.mapTime(1066000,10073,1,2,3,mapped)&&!timeline.mapTime(1065000,10074,1,2,3,mapped),"duplicate/backward source clock refused");
    check(!timeline.mapTime(9000000,10075,1,2,3,mapped),"source clock jump refused");
    check(timeline.mapTime(2000000,11000,1,2,3,mapped)&&mapped==11000,"long outage starts fresh timeline");
    check(timeline.mapTime(500000,12000,7,8,3,mapped)&&mapped==12000,"new incarnation can reset source clock");
    check(!timeline.mapTime(0,12010,7,8,3,mapped)&&!timeline.mapTime(1,NAN,7,8,3,mapped),"invalid source time refused");
    // Re-arming follow clears rendered samples, not the established peer clock.
    // A cached root is 95 ms old here; treating it as a new arrival would move
    // the clock forward and reject current action samples as >80 ms future.
    SourcePoseTimeline rearmClock;PoseInterpolation rearmBuffer;
    check(rearmClock.mapTime(1000000,10000,1,2,3,mapped),"rearm initial peer clock");
    check(rearmBuffer.push(pose(mapped,1,0)),"rearm initial root");
    rearmBuffer.clear();
    check(!rearmClock.mapTime(1000000,10095,1,2,3,mapped),"cached root cannot rebase established clock");
    check(rearmClock.mapTime(1095000,10095,1,2,3,mapped)&&std::fabs(mapped-10095)<.001,"fresh sample after rearm keeps real arrival time");
    const double actionTime=1095000*.001+rearmClock.offset;
    check(actionTime<=10095+80&&10095-actionTime<=250,"parallel action stream remains inside unchanged freshness gate");
    check(rearmBuffer.push(pose(mapped,2,.57f))&&rearmBuffer.count==1,"fresh root rebuilds empty render buffer");
    SourcePoseTimeline cachedRebase;double wrong=0;
    check(cachedRebase.mapTime(1000000,10095,1,2,3,wrong)&&1095000*.001+cachedRebase.offset>10095+80,"regression fixture reproduces cached-rebase future rejection");
    PoseInterpolation h;PresentationPose p;check(!h.sample(100,p),"empty");
    // 20Hz snapshots rendered at 200Hz, with all three spatial axes moving.
    for(unsigned i=0;i<8;++i)check(h.push(pose(i*50.,i+1,i*.2f,i*.1f,i*.3f)),"valid sample");
    for(int t=100;t<=350;t+=5){check(h.sample(t+80,p),"sample");check(std::fabs(p.position[0]-t*.004f)<.00001f&&std::fabs(p.position[1]-t*.002f)<.00001f&&std::fabs(p.position[2]-t*.006f)<.00001f,"continuous XYZ interpolation");}
    check(h.sample(2000,p)&&std::fabs(p.position[0]-1.4f)<.000001f,"loss holds latest without extrapolation");
    check(!h.push(pose(360,8,1.5f)),"duplicate");check(!h.push(pose(340,9,1.5f)),"clock reorder");
    check(h.push(pose(400,9,20))&&h.count==1&&h.sample(405,p)&&p.position[0]==20,"teleport snaps");
    auto changed=pose(450,1,25);changed.epoch=3;check(h.push(changed)&&h.count==1&&h.sample(451,p)&&p.position[0]==25,"world replacement cannot interpolate");
    changed=pose(800,2,25);changed.epoch=3;check(h.push(changed)&&h.count==1,"outage resets history");
    h.clear();check(h.push(pose(0,1,0,0,0,3.12413936f))&&h.push(pose(50,2,0,0,0,-3.12413936f)),"yaw wrap samples");
    check(h.sample(105,p)&&p.orientation[0]<-.999f&&std::fabs(p.orientation[1])<.001f,"shortest yaw arc");
    h.clear();h.push(pose(0,1,0));h.push(pose(50,2,0,0,0,3.14159265f));h.sample(105,p);
    float dot=0;for(int i=0;i<3;++i)dot+=p.orientation[i]*p.orientation[i+3];check(std::fabs(dot)<.0001f,"half turn stays orthogonal");
    auto bad=pose(100,3,0);bad.orientation[0]=0;check(!h.push(bad),"non-rigid rejected");bad=pose(100,3,0);bad.orientation[8]=-1;check(!h.push(bad),"reflection rejected");
    h.clear();auto a=pose(0,1,0),b=pose(50,2,1);a.bodyValid=b.bodyValid=true;
    for(unsigned j=0;j<20;++j){a.body[j][0]=a.body[j][4]=a.body[j][8]=1;b.body[j][1]=1;b.body[j][3]=-1;b.body[j][8]=1;b.body[j][11]=1;}
    h.push(a);h.push(b);check(h.sample(105,p)&&h.sampleBody(105,p)&&p.bodyValid&&std::fabs(p.position[0]-.5f)<.00001f&&std::fabs(p.body[1][11]-.5f)<.00001f,"body and root share delayed time");
    check(std::fabs(p.body[1][0]-.70710678f)<.0001f&&std::fabs(p.body[1][1]-.70710678f)<.0001f,"body orientation stays rigid between packets");
    b.sequence=3;b.time=100;b.bodyValid=false;h.push(b);check(h.sampleBody(155,p)&&p.bodyValid&&p.body[1][11]==1.f,"short optional-channel gap holds last coherent pose");
    check(!h.sampleBody(250,p)&&!p.bodyValid,"body channel expires independently of fresh root traffic");
    b.sequence=4;b.time=150;b.bodyValid=true;for(auto& joint:b.body)joint[11]=3;h.push(b);
    check(h.sampleBody(180,p)&&std::fabs(p.body[1][11]-2.f)<.00001f,"interpolate across missing optional body sample");
    b.epoch=9;b.sequence=1;b.time=200;b.bodyValid=false;h.push(b);check(!h.sampleBody(205,p),"world replacement cannot borrow previous crouch pose");
    h.clear();h.push(pose(0,1,0));h.push(pose(33,2,.198f));
    for(int t=0;t<=180;++t)h.sample(t,p);
    const float held=p.position[0];check(std::fabs(held-.198f)<.00001f,"underrun holds newest authoritative position");
    h.push(pose(66,3,.396f));h.sample(181,p);
    check(p.position[0]>=held&&p.position[0]-held<.0064f,"late packet cannot snap accumulated travel into one render frame");
    float previousPosition=p.position[0];
    h.push(pose(99,4,.594f));h.push(pose(132,5,.792f));h.push(pose(165,6,.99f));
    for(int t=182;t<220;++t){h.sample(t,p);check(p.position[0]>=previousPosition&&p.position[0]-previousPosition<.0064f,"buffer recovery maintains bounded 5 percent catch-up");previousPosition=p.position[0];}
    check(p.position[0]<.99f,"recovery does not extrapolate or snap to latest root");
    h.clear();a=pose(0,1,0,0,1);b=pose(50,2,0,0,0);a.motionValid=b.motionValid=true;a.motion=3;b.motion=0;
    check(h.push(a)&&h.push(b)&&h.sample(105,p)&&p.motionValid&&!p.bodyValid&&p.motion==3&&std::fabs(p.position[2]-.5f)<.00001f,"fall semantics survive missing optional FP matrices and do not land before source contact time");
    check(h.sample(130,p)&&p.motionValid&&p.motion==0&&p.position[2]==0,"ground begins at replicated contact sample");
    b.sequence=3;b.time=100;b.motion=9;check(!h.push(b),"malformed optional motion refused");
    h.clear();a=pose(0,1,0,0,66.4370575f);b=pose(50,2,0,0,66.8370575f);
    a.motionValid=b.motionValid=true;b.motion=2;h.push(a);h.push(b);
    check(h.sample(105,p)&&p.motion==0&&p.airTrajectory&&p.position[2]>66.6f,"takeoff interpolation carries air-trajectory evidence without moving native state forward");
    auto settle=[](float drop,double elapsed,bool coherent=true,bool fall=true){
        PoseInterpolation q;auto x=pose(0,1,0,0,66.9f),y=pose(30,2,0,0,66.6283951f),z=pose(30+elapsed,3,0,0,66.6283951f-drop);
        x.motionValid=y.motionValid=z.motionValid=true;x.motion=fall?3:0;y.motionValid=coherent;
        q.push(x);q.push(y);q.push(z);PresentationPose v;q.sample(110+elapsed,v);return v.airTrajectory;
    };
    check(settle(.19133759f,64.6413),"v168 descending native contact settlement remains an airborne trajectory segment");
    check(!settle(.26f,64.6413)&&!settle(.19f,101)&&!settle(-.19f,64)&&!settle(.19f,64,false)&&!settle(.19f,64,true,false),"settlement refuses excessive distance, elapsed time, ascent, missing native state and ordinary ground");
    h.clear();a=pose(0,1,0,0,1);b=pose(30,2,0,0,.8f);a.motionValid=b.motionValid=true;a.motion=3;h.push(a);h.push(b);
    auto c=pose(60,3,0,0,.9f);c.motionValid=true;h.push(c);c=pose(90,4,0,0,.7f);c.motionValid=true;h.push(c);
    check(h.sample(170,p)&&!p.airTrajectory,"a later descent cannot reuse landing permission after an intervening ascent");
    c=pose(100,1,0,0,.6f);c.motionValid=true;c.epoch=8;h.push(c);
    check(h.sample(180,p)&&!p.airTrajectory,"world replacement cannot reuse landing evidence");
    std::cout<<"PASS: continuous XYZ resampling, shortest rigid rotation, packet loss, duplicate/clock rejection, teleport and world/outage resets\n";
}
