#include "movement_bank.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <limits>
void check(bool b,const char* why){if(!b){std::cerr<<"FAIL "<<why<<'\n';std::exit(1);}}
template<class T>void put(std::vector<unsigned char>& b,size_t off,T v){std::memcpy(b.data()+off,&v,sizeof(v));}
int main(int argc,char** argv){
    check(argc==1||argc==3,"no args for protocol fixtures, or two actual native banks");
    for(unsigned format=(argc==1?1:2);format<=2;++format)for(unsigned role=0;role<2;++role){std::vector<unsigned char> bytes;
        const unsigned joints=role?219:215;const auto layout=role?0x84db84f5d1336b80ull:0x00db32d37719931full;MovementBank bank;
        if(argc==3){std::ifstream f(argv[role+1],std::ios::binary);bytes.assign(std::istreambuf_iterator<char>(f),{});}
        else{const unsigned clips=format==1?9:17;const size_t entry=24+size_t(MovementBank::frames)*joints*28;bytes.resize(32+clips*entry);
            std::memcpy(bytes.data(),format==1?"D2MOVE01":"D2MOVE02",8);put(bytes,8,format);put(bytes,12,joints);put(bytes,16,clips);put(bytes,20,60u);put(bytes,24,layout);
            for(unsigned c=0;c<clips;++c){const size_t at=32+c*entry;put(bytes,at,c);put(bytes,at+4,121u);put(bytes,at+16,c?(c>8?3.f:1.f):0.f);put(bytes,at+20,c?float((c-1)%8)*.7853981634f:0.f);
                for(unsigned f=0;f<121;++f)for(unsigned j=0;j<joints;++j)put(bytes,at+24+(f*joints+j)*28+12,1.f);}}
        check(bank.decode(bytes,layout,joints),"actual bank decode");
        for(unsigned c=0;c<bank.clips.size();++c)for(unsigned j=0;j<joints;++j){const auto a=bank.sample(c,j,.999999f),b=bank.sample(c,j,0);
            for(unsigned k=4;k<7;++k)check(std::fabs(a[k]-b[k])<.00001f,"closed loop translation");}
        MovementBank bad;check(!bad.decode(bytes,layout^1,joints)&&!bad.joints,"wrong layout refused");
        auto edited=bytes;edited.pop_back();check(!bad.decode(edited,layout,joints),"truncation");
        edited=bytes;edited.push_back(0);check(!bad.decode(edited,layout,joints),"trailing payload");
        edited=bytes;put(edited,32+24+28*2,std::numeric_limits<float>::quiet_NaN());check(!bad.decode(edited,layout,joints),"nonfinite pose");
        edited=bytes;put(edited,32+24+16,.1f);check(!bad.decode(edited,layout,joints),"root placement refused");
        edited=bytes;put(edited,32+8,1u);check(!bad.decode(edited,layout,joints),"unsupported event flags");
        unsigned a=0,b=0;float mix=0;check(MovementBank::directions(1,0,a,b,mix)&&a==1&&b==2&&mix==0,"forward");
        check(MovementBank::directions(0,-1,a,b,mix)&&a==7&&mix<.00001f,"right");
        check(MovementBank::directions(1,-.000001f,a,b,mix)&&a==8&&b==1&&mix>.99f,"angle seam");
        check(!MovementBank::directions(0,0,a,b,mix),"idle has no invented direction");
        const float r[9]={1,0,0,0,1,0,0,0,1};float p[3]{};MovementClock clock;
        check(clock.advance(bank,0,p,r,true),"first root");
        for(unsigned n=1;n<=100;++n)check(clock.advance(bank,n*10.,p,r,true),"idle clock");
        check(clock.phase==0,"idle cannot advance walking feet");
        const float stride=bank.clips[1].stride;
        for(unsigned n=1;n<=100;++n){p[0]=stride*.5f*n/100;check(clock.advance(bank,1000+n*10.,p,r,true),"moving clock");}
        check(std::fabs(clock.phase-.5f)<.00001f,"half stride advances exactly half a cycle");
        const float phase=clock.phase;
        for(unsigned n=1;n<=50;++n)clock.advance(bank,2000+n*10.,p,r,true);
        check(clock.phase==phase,"stopping freezes gait phase");
        p[0]+=3;check(!clock.advance(bank,2510,p,r,true)&&clock.weight==0,"teleport discards interpolation");
        clock.advance(bank,3000,p,r,true);p[0]+=.005f;clock.advance(bank,3010,p,r,false);check(clock.phase==phase,"unsupported state never advances gait");
        check(!clock.advance(bank,5000,p,r,true)&&clock.weight==0,"stale presentation gap");
        if(bank.gaits==2){MovementClock run;float rp[3]{};run.advance(bank,0,rp,r,true);double expected=0;
            for(unsigned n=1;n<=600;++n){rp[0]+=.05f;const float before=run.position[0];check(run.advance(bank,n*10.,rp,r,true),"running root");
                const float blendedStride=bank.clips[1].stride+(bank.clips[9].stride-bank.clips[1].stride)*run.runMix;expected+=(rp[0]-before)/blendedStride;expected-=std::floor(expected);
                check(std::fabs(run.phase-expected)<.0002,"gait blend keeps distance phase");}
            check(run.runMix>.999f&&run.weight>.999f,"running is a distinct gait");const auto phase=run.phase;
            for(unsigned n=601;n<650;++n)run.advance(bank,n*10.,rp,r,true);check(run.phase==phase,"running feet stop with travel");
            rp[0]+=.03f;run.advance(bank,6500,rp,r,false);check(run.phase==phase,"actions retain priority while running");}
    }
    std::cout<<"PASS: both actual banks, closed cycles, malformed payloads, layout/root/event guards, angle seam and distance-based phase\n";
}
