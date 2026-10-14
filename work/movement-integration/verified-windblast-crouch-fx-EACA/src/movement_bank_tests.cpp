#include "movement_bank.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <limits>
void check(bool b,const char* why){if(!b){std::cerr<<"FAIL "<<why<<'\n';std::exit(1);}}
template<class T>void put(std::vector<unsigned char>& b,size_t off,T v){std::memcpy(b.data()+off,&v,sizeof(v));}
int main(int argc,char** argv){
    check(argc==1||argc==3,"no args for protocol fixtures, or two actual native banks");
    for(unsigned format=(argc==1?1:28);format<=28;++format)for(unsigned role=0;role<2;++role){std::vector<unsigned char> bytes;
        const unsigned joints=role?219:215;const auto layout=role?0x84db84f5d1336b80ull:0x00db32d37719931full;MovementBank bank;
        if(argc==3){std::ifstream f(argv[role+1],std::ios::binary);bytes.assign(std::istreambuf_iterator<char>(f),{});}
        else{const unsigned clips=MovementBank::clipCount(format);const size_t entry=24+size_t(MovementBank::frames)*joints*28;bytes.resize(32+clips*entry);
            char magic[]="D2MOVE00";magic[6]=char('0'+format/10);magic[7]=char('0'+format%10);std::memcpy(bytes.data(),magic,8);put(bytes,8,format);put(bytes,12,joints);put(bytes,16,clips);put(bytes,20,60u);put(bytes,24,layout);
            for(unsigned c=0;c<clips;++c){const size_t at=32+c*entry;const unsigned s=c>=111?37:c>=107?29:c>=89?c-63:c;const bool direction=(s>=60&&s<=75)||(s>=77&&s<=84);const bool idle=!direction&&(s==0||s==17||s>=29);put(bytes,at,c);put(bytes,at+4,121u);put(bytes,at+16,idle?0.f:((s>=9&&s<=16)||s>=26?3.f:1.f));put(bytes,at+20,((s>=33&&s<=58)||s>=85)?1.f:direction?float((s-(s>=77?77:60))%8)*.7853981634f:idle?0.f:s>=26?(s==26?0.f:s==27?.7853981634f:-.7853981634f):float((s-(s>=18?18:1))%8)*.7853981634f);
                if(c>=141&&c<149){put(bytes,at+16,.6f+float((c-141)%4>=2));put(bytes,at+20,(c-141)%4>=2?.5f:1.f);}
                if(c>=149&&c<173){put(bytes,at+16,(c-149)%6<2?.7853981634f:(c-149)%6<4?1.570796327f:3.141592654f);put(bytes,at+20,1.f);}
                for(unsigned f=0;f<121;++f)for(unsigned j=0;j<joints;++j)put(bytes,at+24+(f*joints+j)*28+12,1.f);}}
        check(bank.decode(bytes,layout,joints),"actual bank decode");
        if(bank.hasWindblastContexts()){
            for(unsigned stance=0;stance<2;++stance)for(unsigned armed=0;armed<2;++armed){
                const unsigned clip=stance?(armed?199:200):(armed?197:198);
                for(unsigned joint:{2u,3u,16u,20u,24u}){
                    const auto actual=bank.sampleWindblast(joint,.5f,float(stance),float(armed)),expected=bank.sampleParameter(clip,joint,.5f);
                    for(unsigned k=0;k<7;++k)check(std::fabs(actual[k]-expected[k])<.000001f,"Windblast selects coherent stance/equipment body including pelvis/spine/hand/legs");
                }
            }
        }
        if(bank.hasDarkVisionContexts()){
            for(unsigned stance=0;stance<2;++stance)for(unsigned armed=0;armed<2;++armed){
                const unsigned clip=stance?(armed?195:196):(armed?193:194);
                const auto actual=bank.sampleDarkVision(2,.5f,float(stance),float(armed)),expected=bank.sampleParameter(clip,2,.5f);
                for(unsigned k=0;k<7;++k)check(std::fabs(actual[k]-expected[k])<.000001f,"Dark Vision chooses full-body native stance and equipment variant");
            }
        }
        if(bank.hasAirCombat()){
            std::vector<std::int16_t> parents(joints,-1);parents[2]=1;parents[3]=2;parents[4]=3;parents[16]=4;parents[19]=2;parents[20]=19;parents[100]=20;parents[101]=16;
            check(MovementBank::combatLocomotionJoint(2,parents)&&MovementBank::combatLocomotionJoint(20,parents)&&MovementBank::combatLocomotionJoint(100,parents),"moving sword preserves gait pelvis, legs and corrective descendants");
            check(!MovementBank::combatLocomotionJoint(3,parents)&&!MovementBank::combatLocomotionJoint(101,parents),"moving sword retains authored torso, wrist and fingers");
            parents[20]=100;check(!MovementBank::combatLocomotionJoint(100,parents),"invalid cyclic anatomy is not a gait permission");
            auto airborne=bank;airborne.clips[44].values[119*joints+19][4]=.1f;airborne.clips[126].values[119*joints+19][4]=.3f;
            check(std::fabs(airborne.sampleCombat(44,19,1,1)[4]-.3f)<1e-6f,"air attack uses authored free legs");
            check(std::fabs(airborne.sampleCombat(44,19,1,0)[4]-.1f)<1e-6f,"landing restores grounded action at the same native scene phase");
            check(std::fabs(airborne.sampleCombat(44,19,1,.5f)[4]-.2f)<1e-6f,"air to ground action blends continuously");
        }
        for(unsigned c=0;c<bank.clips.size();++c)for(unsigned j=0;j<joints;++j){
            if((c>=29&&c<141)||c>=149){const auto endpoint=bank.sampleParameter(c,j,1.f);for(unsigned k=4;k<7;++k)
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
        if(bank.hasTurns()){
            const size_t first=32+149*(24+size_t(MovementBank::frames)*joints*28);
            edited=bytes;put(edited,first+16,0.f);check(!bad.decode(edited,layout,joints),"turn cannot omit measured angular arc");
            edited=bytes;put(edited,first+20,0.f);check(!bad.decode(edited,layout,joints),"turn cannot omit source duration");
            for(float sign:{-1.f,1.f})for(float rate:{.8f,2.f,4.f}){
                MovementClock turn;float tp[3]{},facing[9]={1,0,0,0,1,0,0,0,1};turn.advance(bank,0,tp,facing,true);
                double expected=0;unsigned serial=0;
                for(unsigned n=1;n<=400;++n){const float angle=sign*rate*n*.01f;facing[0]=std::cos(angle);facing[1]=std::sin(angle);facing[3]=-facing[1];facing[4]=facing[0];
                    check(turn.advance(bank,n*10.,tp,facing,true),"stationary turning clock");
                    if(turn.turnSerial){if(serial!=turn.turnSerial){serial=turn.turnSerial;expected=0;}
                        expected+=rate*.01/bank.clips[turn.turnClip].stride;expected-=std::floor(expected);
                        const double error=std::fabs(std::remainder(turn.turnPhase-expected,1.));check(error<.0001,"turn footwork follows angular distance across yaw wrap");}
                }
                check(turn.turnWeight>.999f&&turn.phase==0&&(turn.turnClip-149)%2==(sign<0?1u:0u),"turn direction without invented forward footsteps");
                check((turn.turnClip-149)/2==(rate<1.4f?0u:rate<3.3f?1u:2u),"small medium and fast turns choose bounded native arcs");
                const float frozen=turn.turnPhase;
                for(unsigned n=401;n<=550;++n)turn.advance(bank,n*10.,tp,facing,true);
                check(turn.turnPhase==frozen&&turn.turnWeight<.001f,"stopped facing freezes angular phase and recovers standing");
                turn.advance(bank,5510,tp,facing,true,0,false,false,0,false,0,0,1,0,false,false,false);
                check(turn.turnWeight==0,"native action immediately owns the body over turning");
            }
            MovementClock accelerated;float origin[3]{},facing[9]={1,0,0,0,1,0,0,0,1};float angle=0;accelerated.advance(bank,0,origin,facing,true);
            for(unsigned n=1;n<=500;++n){angle+=(std::min)(6.f,n*.025f)*.01f;facing[0]=std::cos(angle);facing[1]=std::sin(angle);facing[3]=-facing[1];facing[4]=facing[0];accelerated.advance(bank,n*10.,origin,facing,true);}
            check(accelerated.turnClip==153&&accelerated.turnVelocity/bank.clips[accelerated.turnClip].stride<2.f,"controller acceleration upgrades completed steps to native large-turn arcs instead of overcranking short steps");
        }
        if(bank.hasCrouchTurns()){
            auto distinct=bank;for(unsigned k=0;k<4;++k)distinct.clips[149+k*6].values[119*joints+19][4]=.1f*(k+1);
            check(std::fabs(distinct.sampleTurn(149,19,1,1,0)[4]-.1f)<1e-6f,"standing armed turn selects its complete pose");
            check(std::fabs(distinct.sampleTurn(149,19,1,0,1)[4]-.4f)<1e-6f,"crouched empty-hand turn selects its complete pose");
            check(std::fabs(distinct.sampleTurn(149,19,1,.5f,.5f)[4]-.25f)<1e-6f,"stance and equipment transitions blend whole turn poses");
            MovementClock low;float pos[3]{},facing[9]={1,0,0,0,1,0,0,0,1};low.advance(bank,0,pos,facing,true,1);
            for(unsigned n=1;n<=200;++n){const float angle=n*.01f;facing[0]=std::cos(angle);facing[1]=std::sin(angle);facing[3]=-facing[1];facing[4]=facing[0];low.advance(bank,n*10.,pos,facing,true,1);}
            check(low.turnWeight>.999f&&low.stanceMix==1&&low.phase==0,"crouched turns advance angular footwork without translating gait");
            for(unsigned n=201;n<=220;++n){const float angle=n*.01f;facing[0]=std::cos(angle);facing[1]=std::sin(angle);facing[3]=-facing[1];facing[4]=facing[0];low.advance(bank,n*10.,pos,facing,true,0);}
            check(low.turnWeight>.999f&&low.stanceMix>0&&low.stanceMix<1,"standing up retains angular turn phase through full-body stance blend");
        }
        if(bank.hasTerrain()){
            const size_t first=32+141*(24+size_t(MovementBank::frames)*joints*28);
            edited=bytes;put(edited,first+16,0.f);check(!bad.decode(edited,layout,joints),"stairs cannot omit measured stride");
            edited=bytes;put(edited,first+20,0.f);check(!bad.decode(edited,layout,joints),"stair cycles cannot omit native source duration");
            for(float armed:{0.f,1.f})for(float grade:{-.55f,.55f}){
                MovementClock stairs;float sp[3]{};double expected=0;stairs.advance(bank,0,sp,r,true,0,false,false,0,false,0,0,armed);
                for(unsigned n=1;n<=350;++n){const float before=sp[0];sp[0]+=.009f;sp[2]+=.009f*grade;
                    check(stairs.advance(bank,n*10.,sp,r,true,0,false,false,0,false,0,0,armed),"bounded native stair trajectory");
                    const float stand=bank.clips[1].stride+(bank.clips[9].stride-bank.clips[1].stride)*stairs.runMix;
                    const float empty=bank.clips[60].stride+(bank.clips[68].stride-bank.clips[60].stride)*stairs.runMix;
                    float stride=empty+(stand-empty)*armed;stride+=(bank.terrainStride(stairs.terrainDown,stairs.terrainRun,armed)-stride)*stairs.terrainWeight;
                    expected+=(sp[0]-before)/stride;expected-=std::floor(expected);
                    check(std::fabs(stairs.phase-expected)<.0002,"stair gait blend uses exactly the same distance and stride as the pose");
                }
                check(stairs.terrainWeight>.999f&&(grade<0?stairs.terrainDown>.999f:stairs.terrainDown<.001f),"up/down and armed/unarmed stair selection");
                check(std::fabs(bank.terrainStride(stairs.terrainDown,stairs.terrainRun,armed)/bank.terrainDuration(stairs.terrainDown,stairs.terrainRun,armed)-stairs.speed)<.002f,"native stride and duration choose natural stair cadence at intermediate speed");
                const float phase=stairs.phase;
                for(unsigned n=351;n<=500;++n)stairs.advance(bank,n*10.,sp,r,true,0,false,false,0,false,0,0,armed);
                check(stairs.phase==phase&&stairs.terrainWeight<.001f,"stopping on stairs cannot keep walking");
                for(unsigned n=501;n<=700;++n){sp[0]+=.009f;stairs.advance(bank,n*10.,sp,r,true,0,false,false,0,false,0,0,armed);}
                check(stairs.terrainWeight<.001f,"flat floor restores level gait after staircase");
                for(unsigned n=701;n<=850;++n){sp[0]+=.009f;sp[2]+=.005f;stairs.advance(bank,n*10.,sp,r,true);}
                check(stairs.terrainWeight>.99f,"second staircase detected");
                sp[2]+=.05f;stairs.advance(bank,8510,sp,r,true,0,false,false,0,false,2,.2f);
                check(stairs.terrainWeight==0&&stairs.terrainRiseRate==0,"airborne movement immediately clears terrain intent");
            }
            for(unsigned stance:{0u,1u}){MovementClock backwards;float bp[3]{};backwards.advance(bank,0,bp,r,true,stance);
                for(unsigned n=1;n<=200;++n){bp[0]-=.01f;bp[2]+=.005f;backwards.advance(bank,n*10.,bp,r,true,stance);}
                check(backwards.terrainWeight==0,"forward stair donor cannot take over backward or crouched gait");}
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
        if(bank.hasLowMantle()){
            MovementClock climb;float cp[3]{};climb.advance(bank,0,cp,r,true,0,false,false,0,false,2);
            cp[2]=.3f;check(climb.advance(bank,20,cp,r,true,0,false,false,0,false,0,0,1,0,true),"native mantle permits bounded vertical root trajectory");
            check(!climb.airState&&!climb.airWeight&&!climb.slideState,"mantle clears prior airborne/slide pose ownership");
            cp[2]+=.5f;check(!climb.advance(bank,30,cp,r,true,0,false,false,0,false,0,0,1,0,true),"mantle does not accept unbounded vertical jumps");
        }
        if(bank.hasPowers()){
            MovementClock power,ordinary;float pp[3]{};power.advance(bank,0,pp,r,true);ordinary.advance(bank,0,pp,r,true);
            pp[0]=4;pp[2]=1;
            check(power.advance(bank,20,pp,r,true,0,false,false,0,false,0,0,1,0,false,true),"native power explicitly permits rapid replicated travel");
            check(power.phase==0&&power.speed==0&&!power.airState,"Blink or Far Reach travel does not drive running feet or inferred jump");
            check(!ordinary.advance(bank,20,pp,r,true),"ordinary locomotion retains displacement guard");
            pp[0]+=20;check(!power.advance(bank,30,pp,r,true,0,false,false,0,false,0,0,1,0,false,true),"power presentation still rejects unbounded discontinuity");
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
