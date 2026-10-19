#include "movement_bank.h"
#include "pose_interpolation.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <limits>
void check(bool b,const char* why){if(!b){std::cerr<<"FAIL "<<why<<'\n';std::exit(1);}}
template<class T>void put(std::vector<unsigned char>& b,size_t off,T v){std::memcpy(b.data()+off,&v,sizeof(v));}
int main(int argc,char** argv){
    {MovementBank old;old.version=63;check(old.hasChoke(3)&&!old.hasChoke(4),"old choke bank cannot address absent breakout clip");}
    {MovementBank old;old.version=64;check(old.hasChoke(4)&&!old.hasChoke(5)&&!old.hasChoke(6)&&!old.hasChoke(58),"bank64 cannot address absent lethal clip");}
    {MovementBank b;b.version=65;check(b.hasChoke(58)&&b.hasChoke(92)&&b.hasChoke(kNativeTakedownSceneCount)&&!b.hasChoke(kNativeTakedownSceneCount+1),"bank65 presents every census takedown scene, nothing else");}
    {MovementBank old;old.version=65;check(!old.hasKick(),"old bank cannot address absent kick clip");}
    {MovementBank old;old.version=66;check(!old.hasCrouchHeavy(),"old bank cannot address crouched heavy clips");}
    {MovementBank b;b.version=73;check(!b.hasKnockdown()&&MovementBank::clipCount(73)==380,"bank73 cannot address the knockdown clip");
     b.version=74;check(b.hasKnockdown()&&!b.hasShop()&&MovementBank::clipCount(74)==381,"bank74 adds the knockdown clip 380");
     b.version=75;check(b.hasShop()&&MovementBank::clipCount(75)==382&&MovementBank::clipCount(76)==0,"bank75 adds the shop gesture clip 381");}
    check(argc==1||argc==3,"no args for protocol fixtures, or two actual native banks");
    for(unsigned format=(argc==1?1:59);format<=59;++format)for(unsigned role=0;role<2;++role){std::vector<unsigned char> bytes;
        const unsigned joints=role?219:215;const auto layout=role?0x84db84f5d1336b80ull:0x00db32d37719931full;MovementBank bank;
        if(argc==3){std::ifstream f(argv[role+1],std::ios::binary);bytes.assign(std::istreambuf_iterator<char>(f),{});}
        else{const unsigned clips=MovementBank::clipCount(format);const size_t entry=24+size_t(MovementBank::frames)*joints*28;bytes.resize(32+clips*entry);
            char magic[]="D2MOVE00";magic[6]=char('0'+format/10);magic[7]=char('0'+format%10);std::memcpy(bytes.data(),magic,8);put(bytes,8,format);put(bytes,12,joints);put(bytes,16,clips);put(bytes,20,60u);put(bytes,24,layout);
            for(unsigned c=0;c<clips;++c){const size_t at=32+c*entry;const unsigned s=c>=111?37:c>=107?29:c>=89?c-63:c;const bool direction=(s>=60&&s<=75)||(s>=77&&s<=84);const bool idle=!direction&&(s==0||s==17||s>=29);put(bytes,at,c);put(bytes,at+4,121u);put(bytes,at+16,idle?0.f:((s>=9&&s<=16)||s>=26?3.f:1.f));put(bytes,at+20,((s>=33&&s<=58)||s>=85)?1.f:direction?float((s-(s>=77?77:60))%8)*.7853981634f:idle?0.f:s>=26?(s==26?0.f:s==27?.7853981634f:-.7853981634f):float((s-(s>=18?18:1))%8)*.7853981634f);
                if(c==226||c==228||c==333)put(bytes,at+16,1.f);
                if(c>=141&&c<149){put(bytes,at+16,.6f+float((c-141)%4>=2));put(bytes,at+20,(c-141)%4>=2?.5f:1.f);}
                if(c>=149&&c<173){put(bytes,at+16,(c-149)%6<2?.7853981634f:(c-149)%6<4?1.570796327f:3.141592654f);put(bytes,at+20,1.f);}
                for(unsigned f=0;f<121;++f)for(unsigned j=0;j<joints;++j)put(bytes,at+24+(f*joints+j)*28+12,1.f);}}
        check(bank.decode(bytes,layout,joints),"actual bank decode");
        if(bank.hasChoke(4))for(unsigned j=0;j<joints;++j)for(float t:{0.f,.2f,.5f,1.f}){
            const auto pose=bank.sampleParameter(351,j,t);for(float x:pose)check(std::isfinite(x),"breakout full-body pose finite");
            if(j<2)for(unsigned k=0;k<7;++k)check(std::fabs(pose[k]-(k==3?1.f:0.f))<1e-6f,"breakout cannot move network root");
        }
        if(bank.hasChoke(5))for(unsigned j=0;j<joints;++j)for(float t:{0.f,.2f,.5f,1.f}){
            const auto pose=bank.sampleParameter(352,j,t);for(float x:pose)check(std::isfinite(x),"lethal full-body pose finite");
            if(j<2)for(unsigned k=0;k<7;++k)check(std::fabs(pose[k]-(k==3?1.f:0.f))<1e-6f,"lethal cannot move network root");
        }
        if(bank.hasCrouchHeavy())for(unsigned clip:{354u,355u,356u})for(unsigned j=0;j<joints;++j)for(float t:{0.f,.2f,.5f,1.f}){
            const auto pose=bank.sampleCombat(clip,j,t,0);for(float x:pose)check(std::isfinite(x),"crouched heavy pose finite");
            if(j<2)for(unsigned k=0;k<7;++k)check(std::fabs(pose[k]-(k==3?1.f:0.f))<1e-6f,"crouched heavy cannot move network root");
        }
        if(bank.hasKick())for(unsigned j=0;j<joints;++j)for(float t:{0.f,.2f,.5f,1.f}){
            const auto pose=bank.sampleCombat(353,j,t,0);for(float x:pose)check(std::isfinite(x),"kick full-body pose finite");
            if(j<2)for(unsigned k=0;k<7;++k)check(std::fabs(pose[k]-(k==3?1.f:0.f))<1e-6f,"kick cannot move network root");
        }
        check(bank.hasPossessionCrouch()==(bank.version>=53),"crouched Possession requires bank53");
        if(bank.hasPossessionCrouch())for(unsigned v:{26u,27u})for(float t:{0.f,.5f,1.f}){
            for(float low:{0.f,.5f,1.f})for(unsigned j:{2u,9u,16u,17u,20u,24u})
                for(float x:bank.samplePossession(v,j,t,low))check(std::isfinite(x),"Possession stance blend finite");
            if(argc==3){
                const auto a=bank.samplePossession(v,2,t,0),b=bank.samplePossession(v,2,t,1);
                check(b[6]<a[6]-.15f&&b[6]>.5f,"Possession crouch retains supported whole-body pelvis height");
                check(std::fabs(bank.clips[290+v].angle-(v==26?.333f:.2f))<1e-6f,"crouch uses observed native scene duration");
                for(unsigned j:{16u,17u}){const auto g=bank.sampleParameter(17,j,0),p=bank.samplePossession(v,j,t,1);
                    for(unsigned k=4;k<7;++k)check(std::fabs(g[k]-p[k])<1e-6f,"crouched sword hand/socket translations retained");}
            }
        }
        check(bank.hasShadowWalkSneak()==(bank.version>=59),"sneaking form requires complete bank59");
        if(bank.hasShadowWalkSneak()){
            for(unsigned j=0;j<joints;++j)for(unsigned k=0;k<7;++k){
                check(std::fabs(bank.sampleShadowWalk(35,j,0,0,0)[k]-bank.sampleParameter(334,j,1)[k])<1e-6f,"sneak entry starts at existing recovered crouch");
                check(std::fabs(bank.sampleShadowWalk(35,j,1,0,0)[k]-bank.sampleParameter(334,j,0)[k])<1e-6f,"sneak entry ends at existing low form");
            }
        }
        check(bank.hasShadowWalk()==(bank.version>=58),"Shadow Walk requires complete body bank58");
        if(bank.hasShadowWalk()){
            const float stride=bank.clips[333].stride;
            check(std::fabs(bank.shadowWalkDistancePhase(.2f,0,0,1,0)-.2f)<1e-6f,"stationary shadow crawl cannot advance");
            check(std::fabs(bank.shadowWalkDistancePhase(.2f,stride*.25f,0,1,0)-.45f)<1e-6f,"crawl cadence follows measured quarter-stride");
            check(std::fabs(bank.shadowWalkDistancePhase(.2f,-stride*.25f,0,1,0)-.95f)<1e-6f,"backward crawl reverses and wraps");
            for(unsigned v:{32u,33u,34u,36u})for(float t:{0.f,.5f,1.f})for(unsigned j=0;j<joints;++j){
                auto pose=bank.sampleShadowWalk(v,j,t,.4f,.7f);for(float x:pose)check(std::isfinite(x),"whole shadow body stays finite");
                if(j<2)for(unsigned k=0;k<7;++k)check(std::fabs(pose[k]-(k==3?1.f:0.f))<1e-6f,"shadow body cannot move network root");
            }
            auto bad=bytes;put(bad,32+333*(24+size_t(MovementBank::frames)*joints*28)+16,0.f);MovementBank refused;
            check(!refused.decode(bad,layout,joints),"zero shadow stride rejected before cadence division");
        }
        check(bank.hasDoppelganger()==(bank.version>=55),"Doppelganger requires bank55");
        check(bank.hasDoppelgangerHolstered()==(bank.version>=57),"hands-free Doppelganger requires bank57");
        if(bank.hasDoppelgangerHolstered())for(unsigned v=29;v<=31;++v)for(float t:{0.f,.5f,1.f}){
            for(float low:{0.f,.5f,1.f})for(float armed:{0.f,.5f,1.f})for(unsigned j:{2u,9u,16u,17u,20u,24u})
                for(float x:bank.sampleDoppelganger(v,j,t,low,armed))check(std::isfinite(x),"Doppelganger four-context blends finite");
            for(unsigned low=0;low<2;++low)for(unsigned j:{2u,9u,16u,17u,20u,24u}){
                const auto a=bank.sampleDoppelganger(v,j,t,float(low),0),b=bank.sampleParameter(325+(v-29)*2+low,j,t);
                for(unsigned k=0;k<7;++k)check(std::fabs(a[k]-b[k])<1e-6f,"empty-hand endpoint selects its authored complete-body clip");
            }
            if(argc==3){const auto a=bank.sampleDoppelganger(v,2,t,0,0),b=bank.sampleDoppelganger(v,2,t,1,0);
                check(b[6]<a[6]-.15f&&b[6]>.5f,"holstered crouch retains supported full-body pelvis");
                const float duration[]={.466f,1.2f,.666f};for(unsigned low=0;low<2;++low)
                    check(std::fabs(bank.clips[325+(v-29)*2+low].angle-duration[v-29])<1e-6f,"holstered timing matches native source");
            }
        }

        check(bank.hasDoppelgangerCrouch()==(bank.version>=56),"crouched Doppelganger requires the complete-body context bank56");
        if(bank.hasDoppelgangerCrouch())for(unsigned v=29;v<=31;++v)for(float t:{0.f,.5f,1.f}){
            for(unsigned j:{2u,9u,16u,17u,20u,24u})for(float low:{0.f,.5f,1.f})
                for(float x:bank.sampleDoppelganger(v,j,t,low))check(std::isfinite(x),"Doppelganger complete-body stance blend stays finite");
            if(argc==3){
                const auto a=bank.sampleDoppelganger(v,2,t,0),b=bank.sampleDoppelganger(v,2,t,1);
                check(b[6]<a[6]-.15f&&b[6]>.5f,"Doppelganger crouch keeps supported pelvis rather than standing leg pose");
                const float durations[]={.466f,1.2f,.666f};check(std::fabs(bank.clips[293+v].angle-durations[v-29])<1e-6f,"crouch has observed native phase duration");
                for(unsigned j:{16u,17u}){const auto g=bank.sampleParameter(17,j,0),p=bank.sampleDoppelganger(v,j,t,1);
                    for(unsigned k=4;k<7;++k)check(std::fabs(g[k]-p[k])<1e-6f,"crouch hand/socket relationship retains native offsets");}
            }
        }
        if(bank.hasDoppelganger())for(unsigned v=29;v<=31;++v)for(unsigned j:{2u,9u,16u,17u,20u,24u})for(float t:{0.f,.5f,1.f}){
            const auto a=bank.sampleDoppelganger(v,j,t),b=bank.sampleParameter(319+v-29,j,t);
            for(unsigned k=0;k<7;++k)check(std::isfinite(a[k])&&std::fabs(a[k]-b[k])<1e-6f,"Doppelganger uses its own complete body and attachment pose");
        }
        check(bank.hasPossessionCrouchRelease()==(bank.version>=54),"crouched release requires bank54");
        if(bank.hasPossessionCrouchRelease())for(float t:{0.f,.5f,1.f}){
            for(unsigned j:{2u,9u,16u,17u,20u,24u})for(float low:{0.f,.5f,1.f})
                for(float x:bank.samplePossession(28,j,t,low))check(std::isfinite(x),"crouched release stays finite");
            if(argc==3){
                const auto a=bank.samplePossession(28,2,t,0),b=bank.samplePossession(28,2,t,1);
                check(b[6]<a[6]-.15f&&b[6]>.5f,"release preserves complete crouched pelvis");
                check(std::fabs(bank.clips[318].angle-.4f)<1e-6f,"crouched release uses native400ms");
            }
        }
        check(bank.hasPossessionRelease()==(bank.version>=52),"successful Possession release requires bank52");
        if(bank.hasPossessionRelease()){
            for(unsigned j:{2u,9u,16u,17u,20u,24u})for(float phase:{0.f,.5f,1.f})for(float x:bank.sampleParameter(315,j,phase))check(std::isfinite(x),"Possession release finite native skeleton");
            if(argc==3)check(std::fabs(bank.clips[315].angle-.4f)<1e-6f,"Possession release matches400ms native scene");
        }
        check(bank.hasPossession()==(bank.version>=51),"Possession entry/aim requires bank51");
        if(bank.hasPossession()){
            for(unsigned clip:{313u,314u})for(unsigned joint:{2u,9u,16u,17u,20u,24u})for(float phase:{0.f,.5f,1.f})
                for(float x:bank.sampleParameter(clip,joint,phase))check(std::isfinite(x),"Possession body and sword samples are finite");
            if(argc==3)check(std::fabs(bank.clips[313].angle-.333f)<1e-6f&&std::fabs(bank.clips[314].angle-.2f)<1e-6f,"actual Possession bank uses observed native durations");
        }
        check(bank.hasSwarm()==(bank.version>=49),"Swarm clips require bank49");
        if(bank.hasSwarm())for(unsigned v=23;v<=25;++v)for(unsigned j:{2u,9u,16u,17u,20u,24u})for(float t:{0.f,.5f,1.f}){
            const auto a=bank.sampleSwarm(v,j,t),b=bank.sampleParameter(301+v-23,j,t);
            for(unsigned k=0;k<7;++k)check(std::isfinite(a[k])&&std::fabs(a[k]-b[k])<1e-6f,"Swarm phase stays in its own full-body clip and sword socket");
        }
        check(bank.hasSwarmContexts()==(bank.version>=50),"Swarm stance/equipment contexts require bank50");
        if(bank.hasSwarmContexts())for(unsigned v=23;v<=25;++v)for(float t:{0.f,.5f,1.f})for(float low:{0.f,.5f,1.f})for(float armed:{0.f,.5f,1.f}){
            for(unsigned j:{2u,9u,16u,17u,20u,24u})for(float x:bank.sampleSwarm(v,j,t,low,armed))check(std::isfinite(x),"Swarm context blends remain finite");
            if((low==0||low==1)&&(armed==0||armed==1)){
                const unsigned clip=low?(armed?305:306)+(v-23)*3:armed?301+v-23:304+(v-23)*3;
                for(unsigned j:{16u,17u}){const auto a=bank.sampleSwarm(v,j,t,low,armed),b=bank.sampleParameter(clip,j,t);float rotationDot=0;for(unsigned k=0;k<4;++k)rotationDot+=a[k]*b[k];check(std::fabs(std::fabs(rotationDot)-1.f)<1e-6f,"Swarm context keeps sword-hand/socket orientation, including equivalent quaternion signs");for(unsigned k=4;k<7;++k)check(std::fabs(a[k]-b[k])<1e-6f,"Swarm context keeps sword-hand/socket translation");}
            }
            if(argc==3&&low==1&&armed==1){const auto a=bank.sampleSwarm(v,2,t,0,1),b=bank.sampleSwarm(v,2,t,1,1);check(b[6]<a[6]-.15f&&b[6]>.5f,"Swarm crouch preserves supported pelvis height");}
        }
        check(bank.hasDomino()==(bank.version>=47),"Domino appended clips cannot read older banks");
        if(bank.hasDomino())for(unsigned v=19;v<=22;++v)for(unsigned j:{2u,9u,16u,20u,24u})for(float t:{0.f,.5f,1.f})for(float x:bank.sampleDomino(v,j,t))check(std::isfinite(x),"Domino whole-body samples finite");
        check(bank.hasDominoContexts()==(bank.version>=48),"Domino stance clips cannot read an older bank");
        if(bank.hasDominoContexts())for(unsigned v=19;v<=22;++v)for(float t:{0.f,.5f,1.f})for(float low:{0.f,.5f,1.f})for(float armed:{0.f,.5f,1.f}){
            for(unsigned j:{2u,9u,16u,20u,24u})for(float x:bank.sampleDomino(v,j,t,low,armed))check(std::isfinite(x),"Domino stance/equipment blend finite");
            if((low==0||low==1)&&(armed==0||armed==1)){
                const unsigned clip=low?(armed?290:291)+(v-19)*3:armed?285+v-19:289+(v-19)*3;
                const auto expected=bank.sampleParameter(clip,16,t),actual=bank.sampleDomino(v,16,t,low,armed);
                for(unsigned k=0;k<7;++k)check(std::fabs(expected[k]-actual[k])<1e-6f,"Domino context keeps matching sword hand and socket basis");
            }
            if(argc==3&&low==1&&armed==1){const auto standing=bank.sampleDomino(v,2,t,0,1),crouched=bank.sampleDomino(v,2,t,1,1);check(crouched[6]<standing[6]-.15f&&crouched[6]>.5f,"Domino crouch lowers supported whole-body pelvis");}
        }
        check(bank.hasMesmerize()==(bank.version>=45),"Mesmerize cannot sample beyond an older bank");
        check(bank.hasMesmerizeContexts()==(bank.version>=46),"Mesmerize stance contexts cannot read an older bank");
        if(bank.hasMesmerizeContexts())for(unsigned variant=16;variant<=18;++variant)for(unsigned j:{2u,9u,16u,20u,24u})for(float phase:{0.f,.5f,1.f}){
            for(float stance:{0.f,.5f,1.f})for(float armed:{0.f,.5f,1.f})for(float v:bank.sampleMesmerize(variant,j,phase,stance,armed))check(std::isfinite(v),"actual Mesmerize stance/equipment transition samples remain finite");
            if(argc==3){const auto standing=bank.sampleMesmerize(variant,2,phase,0,1),low=bank.sampleMesmerize(variant,2,phase,1,1);
                check(low[6]<standing[6]-.15f&&low[6]>.5f,"authored crouch lowers a supported whole-body pelvis");}
        }
        if(bank.hasMesmerize())for(unsigned c=273;c<276;++c)for(float a:{0.f,.5f,1.f})for(float v:bank.sampleParameter(c,9,a))check(std::isfinite(v),"Mesmerize phase wrist remains finite");
        check(bank.hasGrenade()==(bank.version>=42),"grenade clips unavailable in older banks");
        check(bank.hasStun()==(bank.version>=43),"stun clips require their appended bank");
        check(bank.hasUse()==(bank.version>=71),"interaction clips require bank71");
        if(bank.hasUse())for(unsigned c=369;c<374;++c)for(float a:{0.f,.5f,1.f})for(float v:bank.sampleParameter(c,2,a))check(std::isfinite(v),"interaction pelvis pose finite");
        check(bank.hasDeath()==(bank.version>=72),"death collapse clips require bank72");
        check(bank.hasRecoil()==(bank.version>=73),"recoil stagger clips require bank73");
        if(bank.hasRecoil())for(unsigned c=378;c<380;++c)for(float a:{0.f,.5f,1.f})for(float v:bank.sampleParameter(c,2,a))check(std::isfinite(v),"recoil pelvis pose finite");
        if(bank.hasDeath())for(unsigned c=374;c<378;++c){check(bank.sampleParameter(c,2,1.f)[6]<bank.sampleParameter(c,2,0.f)[6]-.5f,"death collapse lowers the pelvis to the ground");
            for(float a:{0.f,.5f,1.f})for(float v:bank.sampleParameter(c,2,a))check(std::isfinite(v),"death pelvis pose finite");}
        check(bank.hasCrouchedGrenade()==(bank.version>=44),"crouched grenade clips require their appended bank");
        if(bank.hasCrouchedGrenade())for(unsigned c=264;c<273;++c)for(float a:{0.f,.5f,1.f})for(float v:bank.sampleParameter(c,2,a))check(std::isfinite(v),"crouched grenade pelvis finite");
        if(bank.hasStun())for(unsigned c=260;c<264;++c)for(float a:{0.f,.5f,1.f})for(float v:bank.sampleParameter(c,2,a))check(std::isfinite(v),"stun pelvis pose finite");
        if(bank.hasGrenade())for(unsigned c=251;c<260;++c)for(float a:{0.f,.5f,1.f}){
            const auto j=bank.sampleParameter(c,9,a);for(float v:j)check(std::isfinite(v),"grenade phase keeps finite wrist pose");
        }
        check(bank.hasOutOfManaContexts()==(bank.version>=41),"failure contexts require appended bank version");
        if(bank.hasOutOfManaContexts())for(float low:{0.f,1.f})for(float armed:{0.f,1.f}){
            const unsigned clip=low?(armed?249:250):(armed?247:248);
            const auto a=bank.sampleOutOfMana(16,.5f,low,armed),b=bank.sampleParameter(clip,16,.5f);
            for(unsigned k=0;k<7;++k)check(std::fabs(a[k]-b[k])<1e-6f,"failure context selects matching stance and right-hand grip");
        }
        check(bank.hasOutOfMana()==(bank.version>=40),"failed cast requires its own bank version");
        if(bank.hasOutOfMana())for(float phase:{0.f,.5f,1.f}){
            const auto hand=bank.sampleParameter(247,9,phase);check(std::isfinite(hand[0]),"failure samples within appended clip");
        }
        check(bank.hasBendTimeContexts()==(bank.version>=39),"Bend Time contexts require the extended bank");
        if(bank.hasBendTimeContexts())for(unsigned variant:{12u,14u})for(float low:{0.f,1.f})for(float armed:{0.f,1.f}){
            const unsigned v=variant-12,clip=low?(armed?239:240)+v*3:armed?235+v:238+v*3;
            const auto actual=bank.sampleBendTime(variant,16,.5f,low,armed),expected=bank.sampleParameter(clip,16,.5f);
            for(unsigned k=0;k<7;++k)check(std::fabs(actual[k]-expected[k])<1e-6f,"Bend Time context selects matching hand and weapon pose");
        }
        check(bank.hasBendTime()==(bank.version>=38),"Bend Time phases require their own bank version");
        if(bank.hasBendTime())for(unsigned clip=235;clip<=237;++clip){
            const auto a=bank.sampleParameter(clip,9,0),b=bank.sampleParameter(clip,9,1);
            check(std::isfinite(a[0])&&std::isfinite(b[0]),"Bend Time samples full entry/hold/recovery without overrun");
        }
        check(bank.hasCarryFetalRelease()==(bank.version>=37),"compact release requires its own bank version");
        if(bank.hasCarryFetalRelease()){
            const auto a=bank.sampleParameter(234,9,0),b=bank.sampleParameter(234,9,1);
            check(std::isfinite(a[0])&&std::isfinite(b[0]),"compact release samples entry and recovery without overrunning bank");
        }
        check(bank.hasCarryThrow()==(bank.version>=36),"high-throw carrier cannot read beyond an older bank");
        if(bank.hasCarryThrow()){
            const auto a=bank.sampleParameter(233,9,0),b=bank.sampleParameter(233,9,1);
            check(std::isfinite(a[0])&&std::isfinite(b[0]),"complete throw samples start and recovery without looping");
        }
        check(bank.hasCrouchedCarryPickup()==(bank.version>=35),"crouched hoist requires the new clip");
        check(bank.hasCarryPickupSettle()==(bank.version>=34),"pickup settle is gated to new bank semantics");
        if(bank.hasCrouchedRanged()){
            for(unsigned clip=201;clip<=212;++clip)for(unsigned joint:{2u,3u,9u,16u,20u,24u}){
                for(float stance:{0.f,.5f,1.f}){
                    const auto expected=MovementBank::blend(bank.sampleParameter(clip,joint,.6f),bank.sampleParameter(clip+12,joint,.6f),stance);
                    const auto actual=bank.sampleRanged(clip,joint,.6f,stance);
                    for(unsigned k=0;k<7;++k)check(std::fabs(expected[k]-actual[k])<1e-6f,"ranged stance transitions retain a coherent full body at the native phase");
                }
            }
        }
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
            parents[14]=4;parents[15]=14;parents[16]=15;parents[17]=16;
            check(MovementBank::rightArmJoint(14,parents)&&MovementBank::rightArmJoint(17,parents)&&MovementBank::rightArmJoint(101,parents),"overlapping draw retains entire sword subtree including socket and descendants");
            check(!MovementBank::rightArmJoint(4,parents)&&!MovementBank::rightArmJoint(20,parents),"overlapping draw does not replace casting torso or legs");
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
            // Source-derived boundary geometry from v168, rendered through the
            // real interpolator at a coarse cadence that exposed both dropouts.
            PoseInterpolation trajectory;unsigned sequence=0;
            const double times[]={0,100,150,250,350,400,430,494.6413,600};
            const float heights[]={66.4370575f,66.4370575f,66.8370575f,67.4f,67.9f,66.9f,66.6283951f,66.4370575f,66.4370575f};
            const unsigned motions[]={0,0,2,2,3,3,0,0,0};
            for(unsigned i=0;i<9;++i){PresentationPose q{};q.time=times[i];q.session=q.epoch=q.map=1;q.sequence=++sequence;q.motionValid=true;q.motion=motions[i];q.position[2]=heights[i];q.orientation[0]=q.orientation[4]=q.orientation[8]=1;check(trajectory.push(q),"native air boundary fixture accepted");}
            MovementClock replay;PresentationPose rendered{};
            for(double t:{80.,180.,205.,230.,280.,330.,380.,430.,480.,510.,574.6413,680.}){
                check(trajectory.sample(t,rendered),"boundary trajectory samples");
                check(replay.advance(bank,t,rendered.position,rendered.orientation,true,0,false,false,0,false,rendered.motion,0,1,0,false,false,true,rendered.airTrajectory),"observed takeoff and landing interpolation cannot reset the body clock");
            }
            check(!replay.rejectCount&&replay.weight>.98f,"boundary replay keeps continuous body ownership");
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
            {
                MovementClock boundary;float bp[3]={0,0,1};boundary.advance(bank,0,bp,r,true);
                boundary.advance(bank,10,bp,r,true,0,false,false,0,false,3);
                const auto falling=boundary;bp[2]-=.15223694f;
                check(boundary.advance(bank,30.5489,bp,r,true)&&boundary.airState==3&&!boundary.rejectCount,"observed final falling segment reaches grounded recovery without a pose dropout");
                bp[2]-=.2f;check(!boundary.advance(bank,50.5489,bp,r,true)&&boundary.rejectCode==3,"later grounded frames retain the original vertical guard");
                auto upward=falling;bp[2]=1.2f;check(!upward.advance(bank,30,bp,r,true)&&upward.rejectCode==3,"falling-to-ground does not authorize an upward discontinuity");
                auto distant=falling;bp[2]=-.3f;check(!distant.advance(bank,30,bp,r,true)&&distant.rejectCode==3,"landing still refuses descent above the bounded airborne speed");
                auto lost=falling;bp[2]=.8f;check(!lost.advance(bank,30,bp,r,false)&&lost.rejectCode==3,"interrupted ownership cannot use the landing allowance");
                MovementClock ground;bp[2]=1;ground.advance(bank,0,bp,r,true);bp[2]=.8f;
                check(!ground.advance(bank,20,bp,r,true)&&ground.rejectCode==3,"ordinary ground movement never gains an airborne allowance");
                MovementClock ledge;bp[2]=1;ledge.advance(bank,0,bp,r,true);bp[2]=1-.18f;
                check(ledge.advance(bank,37.6,bp,r,true)&&!ledge.rejectCount,"a 4.8 m/s grounded step in a long frame keeps the body clock (1214cj)");
            }
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
