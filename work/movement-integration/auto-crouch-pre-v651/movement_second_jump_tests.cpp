#include "movement_second_jump.h"
#include <cstdlib>
#include <iostream>
#include <limits>
void check(bool ok,const char* why){if(!ok){std::cerr<<"FAIL "<<why<<'\n';std::exit(1);}}
MovementSemantics::Sample sample(double t,unsigned remaining,float target=11.5f){
    MovementSemantics::Sample s{};s.time=t;s.event=1;s.state=3;s.airStage=1;s.remaining=remaining;s.startZ=10;s.targetZ=target;return s;
}
int main(){
    MovementSecondJump j;auto first=sample(100,1),second=sample(160,0,14);
    check(!j.advance(100,first,10.3f,2,true)&&j.primed,"first native impulse primes only");
    check(j.advance(160,second,11,2,true)&&j.active&&j.serial==1&&j.progress==0&&j.transition==0,"second native impulse begins continuously");
    for(unsigned i=1;i<=10;++i)check(!j.advance(160+i*10,second,11+.2f*i,2,true),"repeated packet cannot duplicate impulse");
    check(j.serial==1&&j.transition==1&&std::fabs(j.progress-2.f/3)<.0001f,"second ascent follows replicated height");
    auto fall=second;fall.time=270;fall.airStage=2;fall.startZ=fall.targetZ=0;
    j.advance(270,fall,12,3,true);check(j.active&&j.progress==1,"descent holds second extension before fall blend");
    j.advance(280,fall,10,0,true);check(!j.active&&!j.primed&&j.serial==1,"landing retires second pose");
    check(!j.advance(300,sample(300,0),10.4f,2,true)&&!j.active,"ordinary jump does not trigger");
    j.clear();check(!j.advance(160,second,11,2,true)&&!j.active,"joining midair never invents missing first impulse");
    for(unsigned mode=0;mode<7;++mode){
        j.clear();j.advance(100,first,10.4f,2,true);auto changed=second;double at=160;
        if(mode==0)changed.targetZ=13;if(mode==1)changed.startZ=9;if(mode==2)changed.time=at=500;
        if(mode==3)changed.time=90;if(mode==4)changed.event=0;if(mode==5)changed.time=161;
        if(mode==6)changed.targetZ=std::numeric_limits<float>::quiet_NaN();
        check(!j.advance(at,changed,11,2,true)&&!j.active,"wrong apex, identity or stale/reordered/nonfinite data cannot trigger");
    }
    j.clear();j.advance(100,first,10.4f,2,true);j.advance(160,second,11,2,false);
    check(!j.active&&!j.primed,"traversal, power, unknown action or expired ownership retires tracker");
    j.advance(300,sample(300,1),10.5f,2,true);check(j.advance(350,sample(350,0,14),11,2,true)&&j.serial==2,"fresh jump can trigger after retirement");
    std::cout<<"PASS native second-impulse presentation, duplicate/stale/identity guards, height phase and recovery\n";
}
