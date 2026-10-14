#pragma once
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
    static constexpr size_t maximumBytes=32+126*(24+size_t(frames)*219*sizeof(Joint));
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
    static unsigned clipCount(unsigned v){constexpr unsigned counts[]={0,9,17,26,29,33,37,44,59,89,107,111,113,115,117,126};return v<=15?counts[v]:0;}
    static Joint blend(const Joint& a,const Joint& b,float t){
        Joint v{};float dot=0,n=0;for(unsigned k=0;k<4;++k)dot+=a[k]*b[k];
        for(unsigned k=0;k<4;++k){v[k]=a[k]+((dot<0?-b[k]:b[k])-a[k])*t;n+=v[k]*v[k];}
        const float scale=1/std::sqrt(n);for(unsigned k=0;k<4;++k)v[k]*=scale;
        for(unsigned k=4;k<7;++k)v[k]=a[k]+(b[k]-a[k])*t;return v;
    }
    bool decode(const std::vector<unsigned char>& bytes,std::uint64_t expectedLayout,unsigned expectedJoints){
        *this={};if(bytes.size()<32||bytes.size()>maximumBytes)return false;
        unsigned format=0;
        for(unsigned v=1;v<=15;++v){char magic[]="D2MOVE00";magic[6]=char('0'+v/10);magic[7]=char('0'+v%10);if(!std::memcmp(bytes.data(),magic,8)){format=v;break;}}
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
            if(m[0]!=c||m[1]!=frames||m[2]||m[3]||!std::isfinite(f[0])||!std::isfinite(f[1])||
                (idle?f[0]!=0:(f[0]<.2f||f[0]>((semantic>=9&&semantic<=16)||semantic>=26?6.f:3.f)))||
                ((semantic>=33&&semantic<=58)||semantic>=85?(f[1]<.05f||f[1]>10.f):std::fabs(std::remainder(f[1]-angle,6.283185307f))>.0001f))return false;
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
};
// Receives the SAME delayed root used to place the remote avatar. Phase is
// accumulated distance; idle time never advances the walking cycle.
struct MovementClock {
    unsigned slideState=0;float slideTime=0,slideWeight=0,leanUp=0;
    unsigned airState=0;float airTime=0,airWeight=0,airProgress=0,airRun=0,airCrouch=0,fallMix=0;
    bool positioned=false;double last=0;float position[3]{},phase=0,idle=0,crouchIdle=0,speed=0,weight=0,runMix=0,sprintMix=0,sprintDirection=0,leanAmount=0,leanWeight=0,stanceProgress=0,stanceMix=0,angleX=1,angleY=0;unsigned a=1,b=2,sprintSide=27;float mix=0;
    bool advance(const MovementBank& bank,double now,const float* p,const float* rotation,bool eligible,unsigned stance=0,bool sprinting=false,bool leaning=false,float lean=0,bool sliding=false,unsigned motion=0,float riseProgress=0,float armedAmount=1,float verticalPeek=0,bool traversing=false,bool powered=false){
        if(!bank.joints||!std::isfinite(now))return false;
        for(unsigned k=0;k<3;++k)if(!std::isfinite(p[k]))return false;
        for(unsigned k=0;k<9;++k)if(!std::isfinite(rotation[k]))return false;
        if(stance>1||(stance&&!bank.hasCrouch()))eligible=false;
        if(!positioned){positioned=true;last=now;std::memcpy(position,p,12);stanceProgress=stanceMix=bank.hasCrouch()&&stance==1?1.f:0.f;return true;}
        const double dt=(now-last)*.001;if(dt<=0||dt>.25){positioned=false;weight=0;return false;}last=now;
        float dx=p[0]-position[0],dy=p[1]-position[1],dz=p[2]-position[2];std::memcpy(position,p,12);
        powered=powered&&bank.hasPowers();
        const float distance=std::sqrt(dx*dx+dy*dy),physicalRate=distance/float(dt),rate=powered?0.f:physicalRate;
        const bool airborne=bank.hasAir()&&(motion==2||motion==3);
        if(physicalRate>(powered?500.f:10.f)||std::fabs(dz)>(powered?500.f*float(dt):(traversing&&bank.hasLowMantle())?20.f*float(dt):airborne?60.f*float(dt):.15f)){positioned=false;weight=0;return false;}
        const float alpha=1-std::exp(-float(dt)/.10f);speed+=(rate-speed)*alpha;
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
        if(distance>.00001f&&!powered){
            const float x=dx*rotation[0]+dy*rotation[1],y=dx*rotation[3]+dy*rotation[4];
            if(MovementBank::directions(x,y,a,b,mix)){angleX=x/distance;angleY=y/distance;}
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
            if(eligible&&rate<(bank.gaits>1?9.f:1.8f)){phase+=distance/stride;phase-=std::floor(phase);}
        }
        // Version 1 remains walking-only; version 2 contains separate running
        // poses. Both use travelled distance, including during gait blends.
        const float target=eligible&&speed<(bank.gaits>1?9.f:1.8f)?1.f:0.f;
        weight+=(target-weight)*(1-std::exp(-float(dt)/.12f));return true;
    }
};
