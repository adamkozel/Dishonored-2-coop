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
    unsigned joints=0;std::uint64_t checksum=0,layout=0;std::array<Clip,9> clips{};
    static constexpr unsigned frames=121;
    static constexpr size_t maximumBytes=32+9*(24+size_t(frames)*219*sizeof(Joint));
    static Joint blend(const Joint& a,const Joint& b,float t){
        Joint v{};float dot=0,n=0;for(unsigned k=0;k<4;++k)dot+=a[k]*b[k];
        for(unsigned k=0;k<4;++k){v[k]=a[k]+((dot<0?-b[k]:b[k])-a[k])*t;n+=v[k]*v[k];}
        const float scale=1/std::sqrt(n);for(unsigned k=0;k<4;++k)v[k]*=scale;
        for(unsigned k=4;k<7;++k)v[k]=a[k]+(b[k]-a[k])*t;return v;
    }
    bool decode(const std::vector<unsigned char>& bytes,std::uint64_t expectedLayout,unsigned expectedJoints){
        *this={};if(bytes.size()<32||bytes.size()>maximumBytes||std::memcmp(bytes.data(),"D2MOVE01",8))return false;
        std::uint32_t h[4]{};std::uint64_t hash=0;std::memcpy(h,bytes.data()+8,16);std::memcpy(&hash,bytes.data()+24,8);
        if(h[0]!=1||(h[1]!=215&&h[1]!=219)||h[1]!=expectedJoints||h[2]!=9||h[3]!=60||!expectedLayout||hash!=expectedLayout)return false;
        const size_t payload=size_t(frames)*h[1]*sizeof(Joint),entry=24+payload;
        if(bytes.size()!=32+9*entry)return false;
        std::array<Clip,9> next{};
        for(unsigned c=0;c<9;++c){const auto* p=bytes.data()+32+c*entry;std::uint32_t m[4]{};float f[2]{};
            std::memcpy(m,p,16);std::memcpy(f,p+16,8);
            const float angle=c?float(c-1)*.7853981634f:0;
            if(m[0]!=c||m[1]!=frames||m[2]||m[3]||!std::isfinite(f[0])||!std::isfinite(f[1])||
                (c?(f[0]<.2f||f[0]>3.f):f[0]!=0)||std::fabs(std::remainder(f[1]-angle,6.283185307f))>.0001f)return false;
            auto& clip=next[c];clip.stride=f[0];clip.angle=f[1];clip.values.resize(size_t(frames)*h[1]);std::memcpy(clip.values.data(),p+24,payload);
            for(unsigned frame=0;frame<frames;++frame)for(unsigned j=0;j<h[1];++j){const auto& v=clip.values[frame*h[1]+j];
                for(float x:v)if(!std::isfinite(x)||std::fabs(x)>3.f)return false;
                float n=0;for(unsigned k=0;k<4;++k)n+=v[k]*v[k];if(std::fabs(n-1)>.001f)return false;
                if(j<2)for(unsigned k=0;k<7;++k)if(std::fabs(v[k]-(k==3?1.f:0.f))>.00001f)return false;
                if(frame==frames-1){const auto& first=clip.values[j];float dot=0;for(unsigned k=0;k<4;++k)dot+=first[k]*v[k];if(std::fabs(dot)<.999999f)return false;
                    for(unsigned k=4;k<7;++k)if(std::fabs(first[k]-v[k])>.000001f)return false;}
            }
        }
        joints=h[1];layout=hash;clips=std::move(next);checksum=14695981039346656037ull;for(auto b:bytes){checksum^=b;checksum*=1099511628211ull;}return true;
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
};
// Receives the SAME delayed root used to place the remote avatar. Phase is
// accumulated distance; idle time never advances the walking cycle.
struct MovementClock {
    bool positioned=false;double last=0;float position[3]{},phase=0,idle=0,speed=0,weight=0,angleX=1,angleY=0;unsigned a=1,b=2;float mix=0;
    bool advance(const MovementBank& bank,double now,const float* p,const float* rotation,bool eligible){
        if(!bank.joints||!std::isfinite(now))return false;
        for(unsigned k=0;k<3;++k)if(!std::isfinite(p[k]))return false;
        for(unsigned k=0;k<9;++k)if(!std::isfinite(rotation[k]))return false;
        if(!positioned){positioned=true;last=now;std::memcpy(position,p,12);return true;}
        const double dt=(now-last)*.001;if(dt<=0||dt>.25){positioned=false;weight=0;return false;}last=now;
        float dx=p[0]-position[0],dy=p[1]-position[1],dz=p[2]-position[2];std::memcpy(position,p,12);
        const float distance=std::sqrt(dx*dx+dy*dy),rate=distance/float(dt);
        if(rate>10||std::fabs(dz)>.15f){positioned=false;weight=0;return false;}
        const float alpha=1-std::exp(-float(dt)/.10f);speed+=(rate-speed)*alpha;
        idle+=float(dt)*.5f;idle-=std::floor(idle);
        if(distance>.00001f){
            const float x=dx*rotation[0]+dy*rotation[1],y=dx*rotation[3]+dy*rotation[4];
            if(MovementBank::directions(x,y,a,b,mix)){angleX=x/distance;angleY=y/distance;}
            const float stride=bank.clips[a].stride*(1-mix)+bank.clips[b].stride*mix;
            if(eligible&&rate<1.8f){phase+=distance/stride;phase-=std::floor(phase);}
        }
        // This first bank is walking only. High-speed and unsupported actions
        // return to native evaluation; they are never labelled sprint coverage.
        const float target=eligible&&speed<1.8f?1.f:0.f;
        weight+=(target-weight)*(1-std::exp(-float(dt)/.12f));return true;
    }
};
