#include "movement_bank.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <limits>
void check(bool b,const char* why){if(!b){std::cerr<<"FAIL "<<why<<'\n';std::exit(1);}}
template<class T>void put(std::vector<unsigned char>& b,size_t off,T v){std::memcpy(b.data()+off,&v,sizeof(v));}
int main(int argc,char** argv){
    check(argc==1||argc==3,"no args for protocol fixtures, or two actual native banks");
    for(unsigned format=(argc==1?1:11);format<=11;++format)for(unsigned role=0;role<2;++role){std::vector<unsigned char> bytes;
        const unsigned joints=role?219:215;const auto layout=role?0x84db84f5d1336b80ull:0x00db32d37719931full;MovementBank bank;
        if(argc==3){std::ifstream f(argv[role+1],std::ios::binary);bytes.assign(std::istreambuf_iterator<char>(f),{});}
        else{const unsigned clips=format==1?9:format==2?17:format==3?26:format==4?29:format==5?33:format==6?37:format==7?44:format==8?59:format==9?89:format==10?107:111;const size_t entry=24+size_t(MovementBank::frames)*joints*28;bytes.resize(32+clips*entry);
            char magic[]="D2MOVE00";magic[6]=char('0'+format/10);magic[7]=char('0'+format%10);std::memcpy(bytes.data(),magic,8);put(bytes,8,format);put(bytes,12,joints);put(bytes,16,clips);put(bytes,20,60u);put(bytes,24,layout);
            for(unsigned c=0;c<clips;++c){const size_t at=32+c*entry;const unsigned s=c>=107?29:c>=89?c-63:c;const bool direction=(s>=60&&s<=75)||(s>=77&&s<=84);const bool idle=!direction&&(s==0||s==17||s>=29);put(bytes,at,c);put(bytes,at+4,121u);put(bytes,at+16,idle?0.f:((s>=9&&s<=16)||s>=26?3.f:1.f));put(bytes,at+20,((s>=33&&s<=58)||s>=85)?1.f:direction?float((s-(s>=77?77:60))%8)*.7853981634f:idle?0.f:s>=26?(s==26?0.f:s==27?.7853981634f:-.7853981634f):float((s-(s>=18?18:1))%8)*.7853981634f);
                for(unsigned f=0;f<121;++f)for(unsigned j=0;j<joints;++j)put(bytes,at+24+(f*joints+j)*28+12,1.f);}}
        check(bank.decode(bytes,layout,joints),"actual bank decode");
        for(unsigned c=0;c<bank.clips.size();++c)for(unsigned j=0;j<joints;++j){
            if(c>=29){const auto endpoint=bank.sampleParameter(c,j,1.f);for(unsigned k=4;k<7;++k)
                check(std::fabs(endpoint[k]-bank.clips[c].values[119*joints+j][k])<.000001f,"parameter endpoint is not a cycle");continue;}
            const auto a=bank.sample(c,j,.999999f),b=bank.sample(c,j,0);
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
        if(bank.hasEquipment()){
            MovementClock empty;float ep[3]{};empty.advance(bank,0,ep,r,true,0,false,false,0,false,0,0,0);
            const float step=bank.clips[60].stride*.5f/200;
            for(unsigned n=1;n<=200;++n){ep[0]+=step;check(empty.advance(bank,n*10.,ep,r,true,0,false,false,0,false,0,0,0),"unarmed distance clock");}
            check(std::fabs(empty.phase-.5f)<.0001f,"unarmed footsteps use native unarmed stride");
            const float stopped=empty.phase;for(unsigned n=201;n<=250;++n)empty.advance(bank,n*10.,ep,r,true,0,false,false,0,false,0,0,0);
            check(empty.phase==stopped,"unarmed idle cannot move feet");
        }
        if(bank.hasUnarmedSpecial()){
            auto asymmetric=bank;asymmetric.clips[89].stride=.5f*bank.clips[26].stride;
            MovementClock empty;float ep[3]{};empty.advance(asymmetric,0,ep,r,true,0,true,false,0,false,0,0,0);
            for(unsigned n=1;n<=200;++n)empty.advance(asymmetric,n*10.,ep,r,true,0,true,false,0,false,0,0,0);
            const float start=empty.phase;const float step=asymmetric.clips[89].stride*.5f/100;
            for(unsigned n=201;n<=300;++n){ep[0]+=step;empty.advance(asymmetric,n*10.,ep,r,true,0,true,false,0,false,0,0,0);}
            check(std::fabs(empty.phase-start-.5f)<.0001f,"holstered sprint uses its own stride, not armed run or sprint stride");
            asymmetric.clips[100].values[119*joints+2][4]=.2f;asymmetric.clips[37].values[119*joints+2][4]=-.1f;
            check(std::fabs(asymmetric.sampleWeaponParameter(37,2,1,0)[4]-.2f)<1e-6f,"weaponless jump selects complete-body donor endpoint");
            check(std::fabs(asymmetric.sampleWeaponParameter(37,2,1,1)[4]+.1f)<1e-6f,"armed jump endpoint preserved");
            check(std::fabs(asymmetric.sampleWeaponParameter(37,2,1,.5f)[4]-.05f)<1e-6f,"equipment changes blend both complete poses");
            edited=bytes;put(edited,32+89*(24+size_t(MovementBank::frames)*joints*28)+16,0.f);
            check(!bad.decode(edited,layout,joints),"weaponless sprint cannot omit measured stride");
        }
        for(unsigned n=1;n<=50;++n)clock.advance(bank,2000+n*10.,p,r,true);
        check(clock.phase==phase,"stopping freezes gait phase");
        p[0]+=3;check(!clock.advance(bank,2510,p,r,true)&&clock.weight==0,"teleport discards interpolation");
        clock.advance(bank,3000,p,r,true);p[0]+=.005f;clock.advance(bank,3010,p,r,false);check(clock.phase==phase,"unsupported state never advances gait");
        check(!clock.advance(bank,5000,p,r,true)&&clock.weight==0,"stale presentation gap");
        if(bank.hasCrouch()){
            MovementClock crouch;float cp[3]{};crouch.advance(bank,0,cp,r,true,1);
            check(crouch.stanceMix==1,"initial crouch uses its full-body bank");
            const float crouchStride=bank.clips[18].stride;
            for(unsigned n=1;n<=100;++n){cp[0]=crouchStride*.5f*n/100;crouch.advance(bank,n*10.,cp,r,true,1);}
            check(std::fabs(crouch.phase-.5f)<.00001f,"crouch uses its own stride, not standing stride");
            const float crouchPhase=crouch.phase;
            for(unsigned n=101;n<=125;++n)crouch.advance(bank,n*10.,cp,r,true,0);
            const float partial=crouch.stanceMix;check(partial>0&&partial<1,"standing recovery blends the complete body");
            crouch.advance(bank,1260,cp,r,true,1);check(crouch.stanceMix>partial&&crouch.stanceMix-partial<.03f,"mid-transition reversal remains continuous");
            for(unsigned n=127;n<=250;++n)crouch.advance(bank,n*10.,cp,r,true,0);
            check(crouch.stanceMix==0&&crouch.phase==crouchPhase,"standing recovery finishes without extra footsteps");
            cp[0]+=.005f;crouch.advance(bank,2510,cp,r,false,1);check(crouch.phase==crouchPhase,"crouched action cannot advance feet");
        }
        if(bank.hasSprint()){
            for(float lateral:{-.025f,0.f,.025f}){
                MovementClock sprint;float sp[3]{};sprint.advance(bank,0,sp,r,true,0,true);double expected=0;
                for(unsigned n=1;n<=300;++n){const float x=sp[0],y=sp[1];sp[0]+=.035f;sp[1]+=lateral;check(sprint.advance(bank,n*10.,sp,r,true,0,true),"sprint advance");
                    float stride=bank.clips[sprint.a].stride*(1-sprint.mix)+bank.clips[sprint.b].stride*sprint.mix;
                    const float run=bank.clips[sprint.a+8].stride*(1-sprint.mix)+bank.clips[sprint.b+8].stride*sprint.mix;stride+=(run-stride)*sprint.runMix;
                    const float full=bank.clips[26].stride+(bank.clips[sprint.sprintSide].stride-bank.clips[26].stride)*sprint.sprintDirection;stride+=(full-stride)*sprint.sprintMix;
                    expected+=std::hypot(sp[0]-x,sp[1]-y)/stride;expected-=std::floor(expected);check(std::fabs(sprint.phase-expected)<.0002,"sprint blend uses exact travelled distance");
                }
                check(sprint.sprintMix>.999f,"native sprint activates authored sprint");
                check(sprint.sprintSide==(lateral<0?28u:27u),"sprint diagonal mapping");
                const float frozen=sprint.phase;
                for(unsigned n=301;n<=500;++n)sprint.advance(bank,n*10.,sp,r,true,0,false);
                check(sprint.sprintMix<.001f&&sprint.phase==frozen,"sprint exit recovers without moving feet");
            }
        }
        if(bank.hasAir()){
            MovementClock air;float ap[3]{};air.advance(bank,0,ap,r,true);
            for(unsigned n=1;n<=100;++n)air.advance(bank,n*10.,ap,r,true);
            for(unsigned n=1;n<=30;++n){ap[2]=n*.05f;air.advance(bank,1000+n*10.,ap,r,true,0,false,false,0,false,2,n/30.f);}
            check(air.airState==1&&air.airWeight>.98f&&std::fabs(air.airProgress-1)<.0001f,"rise parameter follows supplied native height instead of elapsed clip time");
            for(unsigned n=1;n<=30;++n){ap[2]=1.5f-n*.05f;air.advance(bank,1300+n*10.,ap,r,true,0,false,false,0,false,3);}
            check(air.airState==2&&air.fallMix==1,"native falling selects airborne loop");
            air.advance(bank,1610,ap,r,true);check(air.airState==3&&air.airTime==0,"native ground starts landing recovery");
            for(unsigned n=162;n<=500;++n)air.advance(bank,n*10.,ap,r,true);
            check(air.airState==0&&air.airWeight<.001f,"landing recovers to distance-driven gait");
            air.advance(bank,5010,ap,r,true,0,false,false,0,false,2,.2f);air.advance(bank,5020,ap,r,false);
            check(air.airState==0&&air.airWeight==0,"native interruption retires airborne ownership");
        }
        if(bank.hasSlide()){
            MovementClock slide;float sp[3]{};slide.advance(bank,0,sp,r,true,1);
            for(unsigned n=1;n<=80;++n)slide.advance(bank,n*10.,sp,r,true,1,false,false,0,true);
            check(slide.slideState==1&&slide.slideWeight>.999f&&slide.slideTime>.75f,"native slide holds independently of a fixed clip timeout");
            slide.advance(bank,810,sp,r,true,1);check(slide.slideState==2&&slide.slideTime==0,"native exit starts authored recovery");
            for(unsigned n=82;n<=300;++n)slide.advance(bank,n*10.,sp,r,true,1);
            check(slide.slideState==0&&slide.slideWeight<.001f,"recovery returns to ordinary crouch");
            slide.advance(bank,3010,sp,r,true,1,false,false,0,true);slide.advance(bank,3020,sp,r,false);
            check(slide.slideState==0&&slide.slideWeight==0,"unknown action interrupts slide ownership immediately");
        }
        if(bank.hasLean()){
            // Endpoint sampling must not wrap back to neutral like a gait.
            auto synthetic=bank;synthetic.clips[29].values[119*joints+2][4]=.1f;
            check(std::fabs(synthetic.sampleParameter(29,2,1.f)[4]-.1f)<1e-6f,"full native lean samples endpoint without loop wrap");
            MovementClock lean;float lp[3]{};lean.advance(bank,0,lp,r,true,0,false,true,-1);
            for(unsigned n=1;n<=100;++n)lean.advance(bank,n*10.,lp,r,true,0,false,true,-1);
            check(lean.leanWeight>.999f&&lean.leanAmount<-.999f&&lean.phase==0,"native lean reaches authored magnitude without foot cadence");
            for(unsigned n=101;n<=200;++n)lean.advance(bank,n*10.,lp,r,true,0,false,true,1);
            check(lean.leanAmount>.999f,"lean reversal crosses neutral continuously");
            for(unsigned n=201;n<=300;++n)lean.advance(bank,n*10.,lp,r,true);
            check(lean.leanWeight<.001f&&std::fabs(lean.leanAmount)<.001f,"lean release recovers native gait");
        }
        if(bank.hasVerticalPeek()){
            MovementClock peek;float pp[3]{};peek.advance(bank,0,pp,r,true,1,false,true,0,false,0,0,0,1);
            for(unsigned n=1;n<=100;++n)peek.advance(bank,n*10.,pp,r,true,1,false,true,0,false,0,0,0,1);
            check(peek.leanUp>.999f&&peek.leanWeight>.999f&&peek.stanceMix==1&&peek.phase==0,"native vertical peek preserves crouch and stationary root phase");
            peek.advance(bank,1010,pp,r,true,1,false,true,-1,false,0,0,0,0);
            check(peek.leanUp>.8f&&peek.leanUp<1&&peek.leanAmount<0,"up-to-side transition starts continuously");
            for(unsigned n=102;n<=200;++n)peek.advance(bank,n*10.,pp,r,true,1,false,true,-1,false,0,0,0,0);
            check(peek.leanUp<.001f&&peek.leanAmount<-.999f,"side transition clears vertical pose");
            for(unsigned n=201;n<=300;++n)peek.advance(bank,n*10.,pp,r,true,1,false,true,0,false,0,0,0,std::numeric_limits<float>::quiet_NaN());
            check(peek.leanUp<.001f,"invalid vertical amplitude cannot enter a pose");
            edited=bytes;put(edited,32+107*(24+size_t(MovementBank::frames)*joints*28)+20,1.f);
            check(!bad.decode(edited,layout,joints),"peek parameter curve cannot claim an unverified duration");
        }
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
