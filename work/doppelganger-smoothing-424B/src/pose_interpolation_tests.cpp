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
    std::cout<<"PASS: continuous XYZ resampling, shortest rigid rotation, packet loss, duplicate/clock rejection, teleport and world/outage resets\n";
}
