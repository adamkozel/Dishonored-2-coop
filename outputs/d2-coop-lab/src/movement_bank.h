#pragma once
#include "native_takedown_scenes.h"
#include <array>
#include <vector>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <algorithm>
// Versioned, bounded offline data. No filenames, commands, root animation,
// events, arbitrary layouts or executable content are accepted.
struct MovementBank {
    using Joint=std::array<float,7>;
    struct Clip {float stride=0,angle=0;std::vector<Joint> values;};
    unsigned joints=0,gaits=0,version=0;std::uint64_t checksum=0,layout=0;std::vector<Clip> clips;
    static constexpr unsigned frames=121;
    static constexpr size_t maximumBytes=32+382*(24+size_t(frames)*219*sizeof(Joint));
    bool hasClimb()const{return version>=70;}
    bool hasSwim()const{return version>=69;}
    bool hasParryImpact()const{return version>=68;}
    bool hasCrouchHeavy()const{return version>=67;}
    bool hasKick()const{return version>=66;}
    bool hasChoke(unsigned variant=0)const{return version>=63&&(variant<=3||(variant==4&&version>=64)||(variant>4&&nativeTakedownScene(variant)&&version>=65));}
    bool hasTrap()const{return version>=62;}
    bool hasHeart()const{return version>=60;}
    bool hasShadowWalkSneak()const{return version>=59;}
    bool hasShadowWalk()const{return version>=58;}
    Joint sampleShadowWalk(unsigned variant,unsigned joint,float amount,float travelPhase,float moving)const{
        if(variant==35)return sampleParameter(335,joint,amount);
        if(variant==32)return sampleParameter(331,joint,amount);
        if(variant==34||variant==36)return sampleParameter(334,joint,amount); // 36: attack_out exit (1214da)
        return blend(sample(332,joint,amount),sample(333,joint,travelPhase),moving);
    }
    float shadowWalkDistancePhase(float phase,float dx,float dy,float forwardX,float forwardY)const{
        const float distance=std::sqrt(dx*dx+dy*dy);
        // One complete authored cycle covers the recorded contact distance.
        // Backward travel reverses this first forward-only study. Dedicated
        // sideways/backward clips and their quality remain separate work.
        phase+=(dx*forwardX+dy*forwardY<0?-distance:distance)/clips[333].stride;
        return phase-std::floor(phase);
    }
    bool hasDoppelganger()const{return version>=55;}
    bool hasDoppelgangerCrouch()const{return version>=56;}
    bool hasDoppelgangerHolstered()const{return version>=57;}
    Joint sampleDoppelganger(unsigned variant,unsigned joint,float phase,float crouched=0,float armed=1)const{
        const unsigned v=variant-29;
        const auto standing=sampleParameter(319+v,joint,phase);
        const auto low=hasDoppelgangerCrouch()?sampleParameter(322+v,joint,phase):standing;
        if(!hasDoppelgangerHolstered())return blend(standing,low,crouched);
        return blend(blend(sampleParameter(325+v*2,joint,phase),standing,armed),
                     blend(sampleParameter(326+v*2,joint,phase),low,armed),crouched);
    }
    bool hasPossessionCrouchRelease()const{return version>=54;}
    bool hasPossessionCrouch()const{return version>=53;}
    Joint samplePossession(unsigned variant,unsigned joint,float phase,float crouched=0)const{
        const auto standing=sampleParameter(287+variant,joint,phase);
        return hasPossessionCrouch()&&variant>=26&&(variant<=27||(variant==28&&hasPossessionCrouchRelease()))?
            blend(standing,sampleParameter(290+variant,joint,phase),crouched):standing;
    }
    bool hasPossessionRelease()const{return version>=52;}
    bool hasPossession()const{return version>=51;}
    bool hasSwarm()const{return version>=49;}
    bool hasSwarmContexts()const{return version>=50;}
    Joint sampleSwarm(unsigned variant,unsigned joint,float phase,float crouched=0,float armed=1)const{
        const unsigned v=variant-23;
        if(!hasSwarmContexts())return sampleParameter(301+v,joint,phase);
        const auto standing=blend(sampleParameter(304+v*3,joint,phase),sampleParameter(301+v,joint,phase),armed);
        const auto low=blend(sampleParameter(306+v*3,joint,phase),sampleParameter(305+v*3,joint,phase),armed);
        return blend(standing,low,crouched);
    }
    bool hasDomino()const{return version>=47;}
    bool hasDominoContexts()const{return version>=48;}
    Joint sampleDomino(unsigned variant,unsigned joint,float phase,float crouched=0,float armed=1)const{
        const unsigned v=variant-19;
        if(!hasDominoContexts())return sampleParameter(285+v,joint,phase);
        const auto standing=blend(sampleParameter(289+v*3,joint,phase),sampleParameter(285+v,joint,phase),armed);
        const auto low=blend(sampleParameter(291+v*3,joint,phase),sampleParameter(290+v*3,joint,phase),armed);
        return blend(standing,low,crouched);
    }
    bool hasMesmerize()const{return version>=45;}
    bool hasMesmerizeContexts()const{return version>=46;}
    Joint sampleMesmerize(unsigned variant,unsigned joint,float phase,float crouched,float armed)const{
        const unsigned v=variant-16;
        if(!hasMesmerizeContexts())return sampleParameter(273+v,joint,phase);
        const auto standing=blend(sampleParameter(276+v*3,joint,phase),sampleParameter(273+v,joint,phase),armed);
        const auto low=blend(sampleParameter(278+v*3,joint,phase),sampleParameter(277+v*3,joint,phase),armed);
        return blend(standing,low,crouched);
    }
    bool hasCrouchedGrenade()const{return version>=44;}
    bool hasStun()const{return version>=43;}
    bool hasKnockdown()const{return version>=74;}
    // 1214dr: clip 381, the partner's black market gestures (think, then an open hand; loops), bank75.
    bool hasShop()const{return version>=75;}
    bool hasUse()const{return version>=71;} // 1213: clips 369-373 authored interactions
    bool hasDeath()const{return version>=72;} // 1214cv: clips 374-377 partner death collapse (front, back, left, right)
    bool hasRecoil()const{return version>=73;} // 1214cz: clips 378-379 recoil stagger (left, right)
    bool hasGrenade()const{return version>=42;}
    bool hasCrouch()const{return version>=3;}
    bool hasSprint()const{return version>=4;}
    bool hasLean()const{return version>=5;}
    bool hasSlide()const{return version>=6;}
    bool hasAir()const{return version>=7;}
    bool hasCombat()const{return version>=8;}
    bool hasEquipment()const{return version>=9;}
    bool hasUnarmedSpecial()const{return version>=10;}
    bool hasVerticalPeek()const{return version>=11;}
    bool hasLowMantle()const{return version>=12;}
    bool hasCrouchMantle()const{return version>=13;}
    bool hasVault()const{return version>=14;} bool hasPowers()const{return version>=15;}
    bool hasAirCombat()const{return version>=16;}
    bool hasTerrain()const{return version>=17;}
    bool hasTurns()const{return version>=18;}
    bool hasCrouchTurns()const{return version>=19;}
    bool hasHighMantle()const{return version>=20;}
    bool hasDoubleJump()const{return version>=21;}
    bool hasHardLanding()const{return version>=22;}
    bool hasCrouchLanding()const{return version>=23;}
    bool hasKeyhole()const{return version>=24;}
    bool hasDarkVision()const{return version>=25;}
    bool hasDarkVisionContexts()const{return version>=26;}
    bool hasWindblast()const{return version>=27;}
    bool hasWindblastContexts()const{return version>=28;}
    bool hasRanged()const{return version>=29;}
    bool hasCarry()const{return version>=31;}
    bool hasCarryRelease()const{return version>=32;}
    bool hasCarryPickup()const{return version>=33;}
    bool hasCarryPickupSettle()const{return version>=34;}
    bool hasCrouchedCarryPickup()const{return version>=35;}
    bool hasCarryThrow()const{return version>=36;}
    bool hasOutOfManaContexts()const{return version>=41;}
    Joint sampleOutOfMana(unsigned joint,float phase,float crouched,float armed)const{
        if(!hasOutOfManaContexts())return sampleParameter(247,joint,phase);
        const auto stand=blend(sampleParameter(248,joint,phase),sampleParameter(247,joint,phase),armed);
        const auto low=blend(sampleParameter(250,joint,phase),sampleParameter(249,joint,phase),armed);
        return blend(stand,low,crouched);
    }
    bool hasOutOfMana()const{return version>=40;}
    bool hasBendTimeContexts()const{return version>=39;}
    Joint sampleBendTime(unsigned variant,unsigned joint,float phase,float crouched,float armed)const{
        const unsigned v=variant-12;
        if(!hasBendTimeContexts())return sampleParameter(235+v,joint,phase);
        const auto stand=blend(sampleParameter(238+v*3,joint,phase),sampleParameter(235+v,joint,phase),armed);
        const auto low=blend(sampleParameter(240+v*3,joint,phase),sampleParameter(239+v*3,joint,phase),armed);
        return blend(stand,low,crouched);
    }
    bool hasBendTime()const{return version>=38;}
    bool hasCarryFetalRelease()const{return version>=37;}
    bool hasCrouchedRanged()const{return version>=30;}
    Joint sampleRanged(unsigned clip,unsigned joint,float phase,float crouched)const{
        const auto standing=sampleParameter(clip,joint,phase);
        return hasCrouchedRanged()&&clip>=201&&clip<=212?blend(standing,sampleParameter(clip+12,joint,phase),crouched):standing;
    }
    Joint sampleWindblast(unsigned joint,float phase,float crouched,float armed)const{
        if(!hasWindblastContexts())return sampleParameter(197,joint,phase);
        const auto stand=blend(sampleParameter(198,joint,phase),sampleParameter(197,joint,phase),armed);
        const auto low=blend(sampleParameter(200,joint,phase),sampleParameter(199,joint,phase),armed);
        return blend(stand,low,crouched);
    }
    Joint sampleDarkVision(unsigned joint,float phase,float crouched,float armed)const{
        if(!hasDarkVisionContexts())return sampleParameter(193,joint,phase);
        const auto stand=blend(sampleParameter(194,joint,phase),sampleParameter(193,joint,phase),armed);
        const auto low=blend(sampleParameter(196,joint,phase),sampleParameter(195,joint,phase),armed);
        return blend(stand,low,crouched);
    }
    static bool rightArmJoint(unsigned joint,const std::vector<std::int16_t>& parents){
        for(unsigned depth=0;depth<parents.size()&&joint<parents.size();++depth){
            if(joint==14)return true;if(parents[joint]<0)return false;joint=unsigned(parents[joint]);
        }return false;
    }
    static bool combatLocomotionJoint(unsigned joint,const std::vector<std::int16_t>& parents){
        // The exact native pelvis and its leg/corrective subtrees carry the
        // travelling gait. Spine/head/arms keep the complete authored action.
        // Following ancestry also preserves extra native boot/calf helpers.
        for(unsigned depth=0;depth<parents.size()&&joint<parents.size();++depth){
            if(joint==3)return false;if(joint==2)return true;
            if(parents[joint]<0)return false;joint=unsigned(parents[joint]);
        }return false;
    }
    // 2136 (#271): a child's rotation kept in the world orientation its authored parent gave it, under another parent
    // rotation (xyzw; translation unchanged). The pistol idle's spine counter-rotates its own center_root (-48 deg);
    // moving, the gait's center_root (+28 to +51 deg) replaced it and turned the head and gun arm 65 to 90 deg left of
    // the walk (measure_gun_walk_mix_v2135d.py).
    static Joint keepUnder(const Joint& authoredParent,const Joint& parent,const Joint& child){
        const auto mul=[](const float* a,const float* b){return std::array<float,4>{a[3]*b[0]+a[0]*b[3]+a[1]*b[2]-a[2]*b[1],
            a[3]*b[1]-a[0]*b[2]+a[1]*b[3]+a[2]*b[0],a[3]*b[2]+a[0]*b[1]-a[1]*b[0]+a[2]*b[3],a[3]*b[3]-a[0]*b[0]-a[1]*b[1]-a[2]*b[2]};};
        const float inverse[4]{-parent[0],-parent[1],-parent[2],parent[3]};
        const auto world=mul(authoredParent.data(),child.data());const auto local=mul(inverse,world.data());
        Joint v=child;for(unsigned k=0;k<4;++k)v[k]=local[k];return v;
    }
    static unsigned clipCount(unsigned v){constexpr unsigned counts[]={0,9,17,26,29,33,37,44,59,89,107,111,113,115,117,126,141,149,161,173,177,179,181,183,193,194,197,198,201,213,225,229,231,232,232,233,234,235,238,247,248,251,260,264,273,276,285,289,301,304,313,315,316,318,319,322,325,331,335,336,340,347,348,351,352,353,354,357,359,365,369,374,378,380,381,382};return v<=75?counts[v]:0;}
    static Joint blend(const Joint& a,const Joint& b,float t){
        Joint v{};float dot=0,n=0;for(unsigned k=0;k<4;++k)dot+=a[k]*b[k];
        for(unsigned k=0;k<4;++k){v[k]=a[k]+((dot<0?-b[k]:b[k])-a[k])*t;n+=v[k]*v[k];}
        const float scale=1/std::sqrt(n);for(unsigned k=0;k<4;++k)v[k]*=scale;
        for(unsigned k=4;k<7;++k)v[k]=a[k]+(b[k]-a[k])*t;return v;
    }
    bool decode(const std::vector<unsigned char>& bytes,std::uint64_t expectedLayout,unsigned expectedJoints){
        *this={};if(bytes.size()<32||bytes.size()>maximumBytes)return false;
        unsigned format=0;
        for(unsigned v=1;v<=75;++v){char magic[]="D2MOVE00";magic[6]=char('0'+v/10);magic[7]=char('0'+v%10);if(!std::memcmp(bytes.data(),magic,8)){format=v;break;}}
        if(!format)return false;
        std::uint32_t h[4]{};std::uint64_t hash=0;std::memcpy(h,bytes.data()+8,16);std::memcpy(&hash,bytes.data()+24,8);
        if(h[0]!=format||(h[1]!=215&&h[1]!=219)||h[1]!=expectedJoints||h[2]!=clipCount(format)||h[3]!=60||!expectedLayout||hash!=expectedLayout)return false;
        const size_t payload=size_t(frames)*h[1]*sizeof(Joint),entry=24+payload;
        if(bytes.size()!=32+h[2]*entry)return false;
        std::vector<Clip> next(h[2]);
        for(unsigned c=0;c<h[2];++c){const auto* p=bytes.data()+32+c*entry;std::uint32_t m[4]{};float f[2]{};
            std::memcpy(m,p,16);std::memcpy(f,p+16,8);
            const unsigned semantic=c>=111?37:c>=107?29:c>=89?c-63:c;
            const bool unarmedDirection=(semantic>=60&&semantic<=75)||(semantic>=77&&semantic<=84);
            const bool idle=!unarmedDirection&&(semantic==0||semantic==17||semantic>=29);
            const float angle=unarmedDirection?float((semantic-(semantic>=77?77:60))%8)*.7853981634f:idle?0.f:semantic>=26?(semantic==26?0.f:semantic==27?.7853981634f:-.7853981634f):float((semantic-(semantic>=18?18:1))%8)*.7853981634f;
            const bool terrain=c>=141&&c<149,turn=c>=149&&c<173;
            const float turnArc=turn?(c-149)%6<2?.7853981634f:(c-149)%6<4?1.570796327f:3.141592654f:0.f;
            if(m[0]!=c||m[1]!=frames||m[2]||m[3]||!std::isfinite(f[0])||!std::isfinite(f[1])||
                (turn?std::fabs(f[0]-turnArc)>.0001f:(c==226||c==228)?(f[0]<.2f||f[0]>3.f):(terrain||c==333)?(f[0]<.2f||f[0]>6.f):idle?f[0]!=0:(f[0]<.2f||f[0]>((semantic>=9&&semantic<=16)||semantic>=26?6.f:3.f)))||
                (terrain||turn?(f[1]<.05f||f[1]>3.f):((semantic>=33&&semantic<=58)||semantic>=85?(f[1]<.05f||f[1]>10.f):std::fabs(std::remainder(f[1]-angle,6.283185307f))>.0001f)))return false;
            auto& clip=next[c];clip.stride=f[0];clip.angle=f[1];clip.values.resize(size_t(frames)*h[1]);std::memcpy(clip.values.data(),p+24,payload);
            for(unsigned frame=0;frame<frames;++frame)for(unsigned j=0;j<h[1];++j){const auto& v=clip.values[frame*h[1]+j];
                for(float x:v)if(!std::isfinite(x)||std::fabs(x)>3.f)return false;
                float n=0;for(unsigned k=0;k<4;++k)n+=v[k]*v[k];if(std::fabs(n-1)>.001f)return false;
                if(j<2)for(unsigned k=0;k<7;++k)if(std::fabs(v[k]-(k==3?1.f:0.f))>.00001f)return false;
                if(frame==frames-1){const auto& first=clip.values[j];float dot=0;for(unsigned k=0;k<4;++k)dot+=first[k]*v[k];if(std::fabs(dot)<.999999f)return false;
                    for(unsigned k=4;k<7;++k)if(std::fabs(first[k]-v[k])>.000001f)return false;}
            }
        }
        joints=h[1];gaits=format==1?1:2;version=format;layout=hash;clips=std::move(next);checksum=14695981039346656037ull;for(auto b:bytes){checksum^=b;checksum*=1099511628211ull;}return true;
    }
    static bool directions(float x,float y,unsigned& a,unsigned& b,float& t){
        if(!std::isfinite(x)||!std::isfinite(y)||x*x+y*y<1e-12f)return false;
        float u=std::atan2(y,x)/.7853981634f;if(u<0)u+=8;u=(std::min)(u,7.9999995f);
        const auto sector=unsigned(u);a=sector+1;b=(sector+1)%8+1;t=u-sector;return true;
    }
    // 2114 (#266): the phase of a clip whose pose is nearest the idle pose (clip 0 at phase 0), by the summed joint
    // distance (quaternion 1-|dot| plus squared translation) over every joint below the root, in 60 steps; cached.
    mutable std::vector<float> startPhases;
    float startPhase(unsigned clip)const{
        if(clip>=clips.size()||clips[0].values.size()<joints||clips[clip].values.size()<size_t(frames)*joints)return 0;
        if(startPhases.size()!=clips.size())startPhases.assign(clips.size(),-1.f);
        if(startPhases[clip]>=0)return startPhases[clip];
        float best=0,bestCost=1e30f;
        for(unsigned s=0;s<60;++s){const float ph=s/60.f;float cost=0;
            for(unsigned j=2;j<joints;++j){const auto w=sample(clip,j,ph),i=sample(0,j,0.f);float dot=0;for(unsigned k=0;k<4;++k)dot+=w[k]*i[k];
                cost+=1-std::fabs(dot);for(unsigned k=4;k<7;++k)cost+=(w[k]-i[k])*(w[k]-i[k]);}
            if(cost<bestCost){bestCost=cost;best=ph;}}
        return startPhases[clip]=best;}
    // 2221 (#269): the phase of the gait pair (na, nb, nm) whose pose is nearest the pose of (oa, ob, om) at phase op, by
    // startPhase's joint distance, in 60 steps. A reversal continues from it, so the crossfade blends two similar poses
    // (animquality-eotw-v2218a: the crossfade from the forward stride slid the planted toe 19 cm in 0.1 s).
    // 2223: with two gaits the pose is the walk/run blend the rig plays (runMix), as a full-speed reversal is mostly run.
    float matchPhase(unsigned oa,unsigned ob,float om,float op,unsigned na,unsigned nb,float nm,float run=0)const{
        const size_t need=size_t(frames)*joints;
        if(oa>=clips.size()||ob>=clips.size()||na>=clips.size()||nb>=clips.size()||clips[oa].values.size()<need||clips[ob].values.size()<need||
           clips[na].values.size()<need||clips[nb].values.size()<need)return op;
        const bool runs=gaits>1&&run>.001f&&oa+8<clips.size()&&ob+8<clips.size()&&na+8<clips.size()&&nb+8<clips.size()&&
            clips[oa+8].values.size()>=need&&clips[ob+8].values.size()>=need&&clips[na+8].values.size()>=need&&clips[nb+8].values.size()>=need;
        const auto pose=[&](unsigned x,unsigned y,float m,float ph,unsigned j){auto v=blend(sample(x,j,ph),sample(y,j,ph),m);
            if(runs)v=blend(v,blend(sample(x+8,j,ph),sample(y+8,j,ph),m),run);return v;};
        float best=op,bestCost=1e30f;
        for(unsigned s=0;s<60;++s){const float ph=s/60.f;float cost=0;
            for(unsigned j=2;j<joints;++j){const auto w=pose(na,nb,nm,ph,j),o=pose(oa,ob,om,op,j);float dot=0;
                for(unsigned k=0;k<4;++k)dot+=w[k]*o[k];cost+=1-std::fabs(dot);for(unsigned k=4;k<7;++k)cost+=(w[k]-o[k])*(w[k]-o[k]);}
            if(cost<bestCost){bestCost=cost;best=ph;}}
        return best;}
    Joint sample(unsigned clip,unsigned joint,float phase)const{
        phase-=std::floor(phase);const float f=phase*(frames-1);const auto a=unsigned(f),b=a+1;
        return blend(clips[clip].values[a*joints+joint],clips[clip].values[b*joints+joint],f-a);
    }
    Joint sampleParameter(unsigned clip,unsigned joint,float amount)const{
        const float f=(std::max)(0.f,(std::min)(1.f,amount))*(frames-2);const auto a=unsigned(f);
        return blend(clips[clip].values[a*joints+joint],clips[clip].values[(a+1)*joints+joint],f-a);
    }
    Joint sampleWeaponParameter(unsigned clip,unsigned joint,float amount,float armed)const{
        const auto held=sampleParameter(clip,joint,amount);
        return hasUnarmedSpecial()&&clip>=29&&clip<=43?blend(sampleParameter(clip+63,joint,amount),held,armed):held;
    }
    Joint sampleCombat(unsigned clip,unsigned joint,float amount,float air)const{
        const auto ground=sampleParameter(clip,joint,amount);
        return hasAirCombat()&&clip>=44&&clip<=58?blend(ground,sampleParameter(126+clip-44,joint,amount),air):ground;
    }
    Joint sampleTerrain(unsigned joint,float phase,float down,float run,float armed)const{
        const auto gait=[&](unsigned c){return blend(blend(sample(c,joint,phase),sample(c+1,joint,phase),down),blend(sample(c+2,joint,phase),sample(c+3,joint,phase),down),run);};
        return blend(gait(145),gait(141),armed);
    }
    float terrainStride(float down,float run,float armed)const{
        const auto gait=[&](unsigned c){const float walk=clips[c].stride+(clips[c+1].stride-clips[c].stride)*down,running=clips[c+2].stride+(clips[c+3].stride-clips[c+2].stride)*down;return walk+(running-walk)*run;};
        const float empty=gait(145);return empty+(gait(141)-empty)*armed;
    }
    float terrainDuration(float down,float run,float armed)const{
        const auto gait=[&](unsigned c){const float walk=clips[c].angle+(clips[c+1].angle-clips[c].angle)*down,running=clips[c+2].angle+(clips[c+3].angle-clips[c+2].angle)*down;return walk+(running-walk)*run;};
        const float empty=gait(145);return empty+(gait(141)-empty)*armed;
    }
    float terrainRunForSpeed(float down,float speed,float armed)const{
        const float walkS=terrainStride(down,0,armed),runS=terrainStride(down,1,armed);
        const float walkT=terrainDuration(down,0,armed),runT=terrainDuration(down,1,armed);
        // Solve blended stride / blended duration = actual speed. Native
        // stairs have shorter steps; level-ground gait thresholds overcrank
        // the walking clip even while its distance phase is mathematically exact.
        const float denominator=runS-walkS-speed*(runT-walkT);
        return denominator>.0001f?(std::max)(0.f,(std::min)(1.f,(speed*walkT-walkS)/denominator)):0.f;
    }
    Joint sampleTurn(unsigned clip,unsigned joint,float phase,float armed,float crouch)const{
        const auto standing=blend(sampleParameter(clip+6,joint,phase),sampleParameter(clip,joint,phase),armed);
        return hasCrouchTurns()?blend(standing,blend(sampleParameter(clip+18,joint,phase),sampleParameter(clip+12,joint,phase),armed),crouch):standing;
    }
};
// Receives the SAME delayed root used to place the remote avatar. Phase is
// accumulated distance; idle time never advances the walking cycle.
// 2114 (#266): lab switch coop_movement_start_phase on|off (on by default).
inline bool movementStartPhaseEnabled=true;
// 2216 (#269): crossfade a direction reversal (coop_movement_direction_blend on|off).
inline bool movementDirectionBlendEnabled=true;
struct MovementClock {
    unsigned startPhaseResets=0;
    // 2216 (#269): the gait's direction pair (a, b, mix) follows each frame's displacement, so a reversal swaps the
    // forward clips for the back clips in one frame and the legs jump (animquality-eotw-v2212c, the host seen from the
    // VM: both toes 0.2-0.4 m in one 31 ms sample with the root still; a and c failed the skate limits on that contact).
    // A turn of the travel direction over about 95 degrees while moving bumps directionSerial; the playback blends from
    // the last locomotion pose over 0.18 s (directionTransition).
    unsigned directionSerial=0;float directionTransition=1;unsigned directionPhaseMatches=0; // 2221: phase matches
    // Last rejected physical step, retained for independent runtime diagnosis.
    unsigned rejectCode=0,rejectCount=0,rejectMotion=0,rejectAirState=0;
    float rejectDt=0,rejectDz=0,rejectRate=0;
    void rejectStep(unsigned code,double dt,float dz,float rate,unsigned motion){
        rejectCode=code;++rejectCount;rejectDt=float(dt);rejectDz=dz;rejectRate=rate;rejectMotion=motion;rejectAirState=airState;
    }
    float facingX=1,facingY=0,turnWeight=0,turnPhase=0,turnVelocity=0,turnTransition=1;
    unsigned turnClip=149,turnSerial=0;
    void clearTurn(){turnWeight=turnPhase=turnVelocity=0;turnTransition=1;}
    float terrainWeight=0,terrainDown=0,terrainGrade=0,terrainRiseRate=0,terrainRun=0;
    void clearTerrain(){terrainWeight=terrainDown=terrainGrade=terrainRiseRate=terrainRun=0;}
    unsigned slideState=0;float slideTime=0,slideWeight=0,leanUp=0;
    unsigned airState=0;float airTime=0,airWeight=0,airProgress=0,airRun=0,airCrouch=0,fallMix=0;
    bool positioned=false;double last=0;float position[3]{},phase=0,idle=0,crouchIdle=0,speed=0,weight=0,runMix=0,sprintMix=0,sprintDirection=0,leanAmount=0,leanWeight=0,stanceProgress=0,stanceMix=0,angleX=1,angleY=0;unsigned a=1,b=2,sprintSide=27;float mix=0;
    bool advance(const MovementBank& bank,double now,const float* p,const float* rotation,bool eligible,unsigned stance=0,bool sprinting=false,bool leaning=false,float lean=0,bool sliding=false,unsigned motion=0,float riseProgress=0,float armedAmount=1,float verticalPeek=0,bool traversing=false,bool powered=false,bool turningAllowed=true,bool airTrajectory=false,bool settling=false){
        if(!bank.joints||!std::isfinite(now))return false;
        for(unsigned k=0;k<3;++k)if(!std::isfinite(p[k]))return false;
        for(unsigned k=0;k<9;++k)if(!std::isfinite(rotation[k]))return false;
        if(stance>1||(stance&&!bank.hasCrouch()))eligible=false;
        if(!positioned){clearTerrain();clearTurn();facingX=rotation[0];facingY=rotation[1];positioned=true;last=now;std::memcpy(position,p,12);stanceProgress=stanceMix=bank.hasCrouch()&&stance==1?1.f:0.f;return true;}
        const double dt=(now-last)*.001;if(dt<=0||dt>.25){rejectStep(1,dt,0,0,motion);clearTerrain();clearTurn();positioned=false;weight=0;return false;}last=now;
        const float yaw=std::atan2(facingX*rotation[1]-facingY*rotation[0],facingX*rotation[0]+facingY*rotation[1]);facingX=rotation[0];facingY=rotation[1];
        float dx=p[0]-position[0],dy=p[1]-position[1],dz=p[2]-position[2];std::memcpy(position,p,12);
        powered=powered&&bank.hasPowers();
        const float distance=std::sqrt(dx*dx+dy*dy),physicalRate=distance/float(dt),rate=powered?0.f:physicalRate;
        const bool airborne=bank.hasAir()&&(motion==2||motion==3);
        // Native Fall -> Ground may end a descending interpolation segment
        // over 15cm in one frame (D0DE replay: -0.15223694m / 20.5489ms).
        // That ONE downward boundary keeps the existing airborne rate bound;
        // upward steps, interrupted ownership and subsequent ground frames do not.
        const bool landingStep=bank.hasAir()&&eligible&&motion==0&&airState==2&&dz<0;
        // The root may straddle native takeoff/contact while discrete motion
        // still names the older sample. Only the validated interpolation span
        // can extend this vertical allowance; it never selects an action pose.
        const bool interpolatedAir=bank.hasAir()&&eligible&&!sliding&&!traversing&&!powered&&airTrajectory;
        // 1214ci (#11): while a traversal presentation is still blending out, its exit can snap the root down a step in
        // one frame (vault exit: 0.16-0.20 m, take 1326a); keep the traversal's vertical allowance until it has ended.
        // 1214cj (#11): a grounded vertical step is bounded by max(0.15 m, 6 m/s x dt), not a flat 0.15 m per frame: a
        // 0.18 m step in a 38 ms frame (about 4.9 m/s, a jump landing on a ledge edge, take 1326b) reset the clock and
        // re-ramped the body weight. A 10 m/s grounded discontinuity is still refused.
        if(physicalRate>(powered?500.f:10.f)||std::fabs(dz)>(powered?500.f*float(dt):((traversing||settling)&&bank.hasLowMantle())?20.f*float(dt):(airborne||landingStep||interpolatedAir)?60.f*float(dt):(std::max)(.15f,6.f*float(dt)))){
            rejectStep(physicalRate>(powered?500.f:10.f)?2u:3u,dt,dz,physicalRate,motion);clearTerrain();clearTurn();positioned=false;weight=0;return false;}
        const float speedBefore=speed;const float alpha=1-std::exp(-float(dt)/.10f);speed+=(rate-speed)*alpha;
        // Full-body reversible stance blend; never adjust just the pelvis/legs.
        // Cadence below uses this same blend, so visual steps and travelled
        // distance cannot run on separate clocks during a stance transition.
        const float stanceTarget=bank.hasCrouch()&&stance==1?1.f:0.f;
        const float stanceStep=float(dt)/.8f;
        stanceProgress+=(std::max)(-stanceStep,(std::min)(stanceStep,stanceTarget-stanceProgress));
        stanceMix=stanceProgress*stanceProgress*(3-2*stanceProgress);
        const float gaitTarget=bank.gaits>1?(std::max)(0.f,(std::min)(1.f,(speed-1.7f)/1.3f)):0.f;
        runMix+=(gaitTarget-runMix)*(1-std::exp(-float(dt)/.12f));
        const float sprintTarget=bank.hasSprint()&&sprinting&&stance==0?1.f:0.f;
        sprintMix+=(sprintTarget-sprintMix)*(1-std::exp(-float(dt)/.12f));
        const float leanTarget=bank.hasLean()&&leaning&&std::isfinite(lean)&&std::fabs(lean)<=1.f?1.f:0.f;
        leanAmount+=((leanTarget?lean:0.f)-leanAmount)*(1-std::exp(-float(dt)/.06f));
        const float upTarget=bank.hasVerticalPeek()&&leanTarget&&std::isfinite(verticalPeek)&&verticalPeek>=0&&verticalPeek<=1?verticalPeek:0.f;
        leanUp+=(upTarget-leanUp)*(1-std::exp(-float(dt)/.06f));
        leanWeight+=(leanTarget-leanWeight)*(1-std::exp(-float(dt)/.10f));
        if(!bank.hasSlide()||!eligible||traversing||powered){slideState=0;slideTime=slideWeight=0;}
        else{
            if(sliding&&slideState!=1){slideState=1;slideTime=0;}
            else if(!sliding&&slideState==1){slideState=2;slideTime=0;}
            else if(slideState)slideTime+=float(dt);
            const float recoveryDuration=bank.clips[35].angle*(1-stanceMix)+bank.clips[36].angle*stanceMix;
            if(slideState==2&&slideTime>=recoveryDuration)slideState=0;
            slideWeight+=((slideState?1.f:0.f)-slideWeight)*(1-std::exp(-float(dt)/.08f));
        }
        if(!bank.hasAir()||!eligible||sliding||traversing||powered){airState=0;airTime=airWeight=fallMix=0;}
        else{
            if(airborne){
                const unsigned desired=motion==2?1u:2u;
                if(airState==0||airState==3){airRun=runMix;airCrouch=stanceMix;airTime=0;fallMix=0;}
                if(airState!=desired)airTime=0;else airTime+=float(dt);
                airState=desired;
                if(desired==1){airProgress=(std::max)(0.f,(std::min)(1.f,riseProgress));fallMix=0;}
                else fallMix=(std::min)(1.f,airTime/.15f);
            }else if(airState==1||airState==2){airState=3;airTime=0;}
            else if(airState==3){
                airTime+=float(dt);
                const float standing=bank.clips[39].angle*(1-airRun)+bank.clips[41].angle*airRun;
                const float duration=standing*(1-stanceMix)+bank.clips[42].angle*stanceMix;
                if(airTime>=duration)airState=0;
            }
            airWeight+=((airState?1.f:0.f)-airWeight)*(1-std::exp(-float(dt)/.07f));
        }
        idle+=float(dt)*.5f;idle-=std::floor(idle);
        crouchIdle+=float(dt)*.4f;crouchIdle-=std::floor(crouchIdle);
        // Terrain intent uses the same delayed physical root as the gait.
        // Filtering vertical velocity keeps stair risers from toggling the
        // cycle; jumps, slides, powers and traversal cannot masquerade as a slope.
        if(!bank.hasTerrain()||!eligible||airborne||airWeight>.001f||sliding||slideWeight>.001f||traversing||powered){clearTerrain();}
        else{
            const float filter=1-std::exp(-float(dt)/.18f);
            terrainRiseRate+=(dz/float(dt)-terrainRiseRate)*filter;
            terrainGrade=(std::max)(-1.2f,(std::min)(1.2f,terrainRiseRate/(std::max)(.3f,speed)));
            const float forward=distance>.00001f?(dx*rotation[0]+dy*rotation[1])/distance:angleX;
            const float steep=(std::max)(0.f,(std::min)(1.f,(std::fabs(terrainGrade)-.12f)/.24f));
            const float direction=(std::max)(0.f,(std::min)(1.f,(forward-.6f)/.3f));
            const float target=stance==0&&stanceMix<.001f&&rate>.2f&&leanWeight<.001f?steep*direction:0.f;
            terrainWeight+=(target-terrainWeight)*(1-std::exp(-float(dt)/.12f));
            if(target>.01f)terrainDown+=((terrainGrade<0?1.f:0.f)-terrainDown)*filter;
            terrainRun+=(bank.terrainRunForSpeed(terrainDown,speed,armedAmount)-terrainRun)*(1-std::exp(-float(dt)/.10f));
        }
        if(!bank.hasTurns()||!eligible||!turningAllowed||airborne||airWeight>.001f||sliding||slideWeight>.001f||traversing||powered||std::fabs(yaw)>1.f){clearTurn();}
        else{
            const float velocity=yaw/float(dt);turnVelocity+=(velocity-turnVelocity)*(1-std::exp(-float(dt)/.065f));
            const bool turnStance=bank.hasCrouchTurns()?stance<=1:stance==0&&stanceMix<.001f;
            const bool turning=turnStance&&speed<.3f&&rate<.22f&&leanWeight<.001f&&terrainWeight<.001f&&std::fabs(turnVelocity)>.3f;
            if(turning&&std::fabs(yaw)>.00001f){
                const unsigned right=yaw<0?1u:0u;
                if(turnWeight<.01f||(turnClip-149)%2!=right){
                    const float magnitude=std::fabs(velocity);const unsigned tier=magnitude<1.4f?0u:magnitude<3.3f?1u:2u;
                    turnClip=149+2*tier+right;turnPhase=0;turnTransition=0;++turnSerial;
                }
                const float arc=bank.clips[turnClip].stride;float angle=turnPhase*arc+std::fabs(yaw);
                if(angle>=arc){
                    angle=std::fmod(angle,arc);
                    // Native controller acceleration can begin below the
                    // large-turn threshold. Re-select only at a completed
                    // step, so fast spins cannot stay on a tiny 45-degree arc.
                    const float magnitude=std::fabs(turnVelocity);const unsigned tier=magnitude<1.4f?0u:magnitude<3.3f?1u:2u;
                    const unsigned next=149+2*tier+right;
                    if(next!=turnClip){turnClip=next;turnTransition=0;++turnSerial;}
                }
                turnPhase=angle/bank.clips[turnClip].stride;
            }
            turnWeight+=((turning?1.f:0.f)-turnWeight)*(1-std::exp(-float(dt)/.10f));
            turnTransition=(std::min)(1.f,turnTransition+float(dt)/.10f);
        }
        directionTransition=(std::min)(1.f,directionTransition+float(dt)/.18f);
        if(distance>.00001f&&!powered){
            const float x=dx*rotation[0]+dy*rotation[1],y=dx*rotation[3]+dy*rotation[4];
            // 2218: the flip happens in the slow frames of the reversal (animquality-eotw-v2216a: serial 0 with the 2216 rate gates),
            // so any frame that moves over 2 mm against the last frame's direction starts the crossfade; a still rig never does.
            // 2225: any jump of the travel direction over 60 degrees in one frame (animquality-eotw-v2223c: back to sideways, 90
            // degrees, popped a toe 0.5 m; the 2216 trigger needed over 95). Steering turns a few degrees per frame and never fires it.
            const bool reversal=movementDirectionBlendEnabled&&eligible&&distance>.002f&&(x*angleX+y*angleY)/distance<.5f;
            const unsigned oldA=a,oldB=b;const float oldMix=mix;
            if(MovementBank::directions(x,y,a,b,mix)){angleX=x/distance;angleY=y/distance;}
            // 2221: the reversal continues from the new clips' phase nearest the old pose.
            if(reversal){++directionSerial;directionTransition=0;phase=bank.matchPhase(oldA,oldB,oldMix,phase,a,b,mix,runMix);++directionPhaseMatches;}
            sprintSide=y<0?28:27;sprintDirection=(std::min)(1.f,std::fabs(std::atan2(y,x))/.7853981634f);
            float stride=bank.clips[a].stride*(1-mix)+bank.clips[b].stride*mix;
            if(bank.gaits>1){const float runStride=bank.clips[a+8].stride*(1-mix)+bank.clips[b+8].stride*mix;stride+=(runStride-stride)*runMix;}
            if(bank.hasSprint()){
                const float sprintStride=bank.clips[26].stride+(bank.clips[sprintSide].stride-bank.clips[26].stride)*sprintDirection;
                stride+=(sprintStride-stride)*sprintMix;
            }
            if(bank.hasCrouch()){
                const float crouchStride=bank.clips[a+17].stride*(1-mix)+bank.clips[b+17].stride*mix;
                stride+=(crouchStride-stride)*stanceMix;
            }
            if(bank.hasEquipment()){
                float empty=bank.clips[a+59].stride*(1-mix)+bank.clips[b+59].stride*mix;
                const float running=bank.clips[a+67].stride*(1-mix)+bank.clips[b+67].stride*mix;empty+=(running-empty)*runMix;
                if(bank.hasUnarmedSpecial()){
                    const float sprint=bank.clips[89].stride+(bank.clips[sprintSide+63].stride-bank.clips[89].stride)*sprintDirection;
                    empty+=(sprint-empty)*sprintMix;
                }
                const float low=bank.clips[a+76].stride*(1-mix)+bank.clips[b+76].stride*mix;empty+=(low-empty)*stanceMix;
                stride=empty+(stride-empty)*armedAmount;
            }
            if(bank.hasTerrain())stride+=(bank.terrainStride(terrainDown,terrainRun,armedAmount)-stride)*terrainWeight;
            // 2114 (#266): a start from rest (the gait still fully out: speed under the playback's 0.025 m/s blend floor)
            // begins at the clip's phase nearest the idle pose. Seen from the host, a walk start popped a planted toe 14.5 cm
            // while the root moved 7 cm (animquality-eotw-v2091a): the cycle resumed wherever the last walk had ended.
            if(movementStartPhaseEnabled&&eligible&&speedBefore<.025f){phase=bank.startPhase(a);++startPhaseResets;}
            if(eligible&&rate<(bank.gaits>1?9.f:1.8f)){phase+=distance/stride;phase-=std::floor(phase);}
        }
        // Version 1 remains walking-only; version 2 contains separate running
        // poses. Both use travelled distance, including during gait blends.
        const float target=eligible&&speed<(bank.gaits>1?9.f:1.8f)?1.f:0.f;
        weight+=(target-weight)*(1-std::exp(-float(dt)/.12f));return true;
    }
};
