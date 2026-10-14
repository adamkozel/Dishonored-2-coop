#include "pose_interpolation.h"
#include <iostream>
#include <cstdlib>
void check(bool c,const char* s){if(!c){std::cerr<<"FAIL "<<s<<'\n';std::exit(1);}}
PresentationPose pose(double t,unsigned seq,float x,float y=0,float z=0,float yaw=0){
    PresentationPose p{};p.time=t;p.sequence=seq;p.session=1;p.epoch=2;p.map=3;p.position[0]=x;p.position[1]=y;p.position[2]=z;
    p.orientation[0]=p.orientation[4]=std::cos(yaw);p.orientation[1]=std::sin(yaw);p.orientation[3]=-std::sin(yaw);p.orientation[8]=1;return p;
}
int main(){
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
    std::cout<<"PASS: continuous XYZ resampling, shortest rigid rotation, packet loss, duplicate/clock rejection, teleport and world/outage resets\n";
}
