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
    static constexpr size_t maximumBytes=32+44*(24+size_t(frames)*219*sizeof(Joint));
    bool hasCrouch()const{return version>=3;}
    bool hasSprint()const{return version>=4;}
    bool hasLean()const{return version>=5;}
    bool hasSlide()const{return version>=6;}
    bool hasAir()const{return version>=7;}
    static Joint blend(const Joint& a,const Joint& b,float t){
        Joint v{};float dot=0,n=0;for(unsigned k=0;k<4;++k)dot+=a[k]*b[k];
        for(unsigned k=0;k<4;++k){v[k]=a[k]+((dot<0?-b[k]:b[k])-a[k])*t;n+=v[k]*v[k];}
        const float scale=1/std::sqrt(n);for(unsigned k=0;k<4;++k)v[k]*=scale;
        for(unsigned k=4;k<7;++k)v[k]=a[k]+(b[k]-a[k])*t;return v;
    }
    bool decode(const std::vector<unsigned char>& bytes,std::uint64_t expectedLayout,unsigned expectedJoints){
        *this={};if(bytes.size()<32||bytes.size()>maximumBytes)return false;
        const unsigned format=!std::memcmp(bytes.data(),"D2MOVE01",8)?1:!std::memcmp(bytes.data(),"D2MOVE02",8)?2:!std::memcmp(bytes.data(),"D2MOVE03",8)?3:!std::memcmp(bytes.data(),"D2MOVE04",8)?4:!std::memcmp(bytes.data(),"D2MOVE05",8)?5:!std::memcmp(bytes.data(),"D2MOVE06",8)?6:!std::memcmp(bytes.data(),"D2MOVE07",8)?7:0;
        if(!format)return false;
        std::uint32_t h[4]{};std::uint64_t hash=0;std::memcpy(h,bytes.data()+8,16);std::memcpy(&hash,bytes.data()+24,8);
        if(h[0]!=format||(h[1]!=215&&h[1]!=219)||h[1]!=expectedJoints||h[2]!=(format==1?9u:format==2?17u:format==3?26u:format==4?29u:format==5?33u:format==6?37u:44u)||h[3]!=60||!expectedLayout||hash!=expectedLayout)return false;
        const size_t payload=size_t(frames)*h[1]*sizeof(Joint),entry=24+payload;
        if(bytes.size()!=32+h[2]*entry)return false;
        std::vector<Clip> next(h[2]);
        for(unsigned c=0;c<h[2];++c){const auto* p=bytes.data()+32+c*entry;std::uint32_t m[4]{};float f[2]{};
            std::memcpy(m,p,16);std::memcpy(f,p+16,8);
            const bool idle=c==0||c==17||c>=29;
            const float angle=idle?0.f:c>=26?(c==26?0.f:c==27?.7853981634f:-.7853981634f):float((c-(c>=18?18:1))%8)*.7853981634f;
            if(m[0]!=c||m[1]!=frames||m[2]||m[3]||!std::isfinite(f[0])||!std::isfinite(f[1])||
                (idle?f[0]!=0:(f[0]<.2f||f[0]>((c>=9&&c<=16)||c>=26?6.f:3.f)))||
                (c>=33?(f[1]<.05f||f[1]>10.f):std::fabs(std::remainder(f[1]-angle,6.283185307f))>.0001f))return false;
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
};
// Receives the SAME delayed root used to place the remote avatar. Phase is
// accumulated distance; idle time never advances the walking cycle.
struct MovementClock {
    unsigned slideState=0;float slideTime=0,slideWeight=0;
    unsigned airState=0;float airTime=0,airWeight=0,airProgress=0,airRun=0,airCrouch=0,fallMix=0;
    bool positioned=false;double last=0;float position[3]{},phase=0,idle=0,crouchIdle=0,speed=0,weight=0,runMix=0,sprintMix=0,sprintDirection=0,leanAmount=0,leanWeight=0,stanceProgress=0,stanceMix=0,angleX=1,angleY=0;unsigned a=1,b=2,sprintSide=27;float mix=0;
    bool advance(const MovementBank& bank,double now,const float* p,const float* rotation,bool eligible,unsigned stance=0,bool sprinting=false,bool leaning=false,float lean=0,bool sliding=false,unsigned motion=0,float riseProgress=0){
        if(!bank.joints||!std::isfinite(now))return false;
        for(unsigned k=0;k<3;++k)if(!std::isfinite(p[k]))return false;
        for(unsigned k=0;k<9;++k)if(!std::isfinite(rotation[k]))return false;
        if(stance>1||(stance&&!bank.hasCrouch()))eligible=false;
        if(!positioned){positioned=true;last=now;std::memcpy(position,p,12);stanceProgress=stanceMix=bank.hasCrouch()&&stance==1?1.f:0.f;return true;}
        const double dt=(now-last)*.001;if(dt<=0||dt>.25){positioned=false;weight=0;return false;}last=now;
        float dx=p[0]-position[0],dy=p[1]-position[1],dz=p[2]-position[2];std::memcpy(position,p,12);
        const float distance=std::sqrt(dx*dx+dy*dy),rate=distance/float(dt);
        const bool airborne=bank.hasAir()&&(motion==2||motion==3);
        if(rate>10||std::fabs(dz)>(airborne?60.f*float(dt):.15f)){positioned=false;weight=0;return false;}
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
        leanWeight+=(leanTarget-leanWeight)*(1-std::exp(-float(dt)/.10f));
        if(!bank.hasSlide()||!eligible){slideState=0;slideTime=slideWeight=0;}
        else{
            if(sliding&&slideState!=1){slideState=1;slideTime=0;}
            else if(!sliding&&slideState==1){slideState=2;slideTime=0;}
            else if(slideState)slideTime+=float(dt);
            const float recoveryDuration=bank.clips[35].angle*(1-stanceMix)+bank.clips[36].angle*stanceMix;
            if(slideState==2&&slideTime>=recoveryDuration)slideState=0;
            slideWeight+=((slideState?1.f:0.f)-slideWeight)*(1-std::exp(-float(dt)/.08f));
        }
        if(!bank.hasAir()||!eligible||sliding){airState=0;airTime=airWeight=fallMix=0;}
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
        if(distance>.00001f){
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
            if(eligible&&rate<(bank.gaits>1?9.f:1.8f)){phase+=distance/stride;phase-=std::floor(phase);}
        }
        // Version 1 remains walking-only; version 2 contains separate running
        // poses. Both use travelled distance, including during gait blends.
        const float target=eligible&&speed<(bank.gaits>1?9.f:1.8f)?1.f:0.f;
        weight+=(target-weight)*(1-std::exp(-float(dt)/.12f));return true;
    }
};
