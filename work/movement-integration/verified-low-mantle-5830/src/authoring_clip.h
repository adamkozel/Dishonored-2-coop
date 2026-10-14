#pragma once
#include <vector>
#include <array>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <algorithm>
// Deliberately narrow interchange for ONE offline-authored Corvo proof.
// No executable data, filenames, animation events or root motion in this file.
struct AuthoringClip {
    using Joint=std::array<float,7>;
    std::vector<Joint> values;
    static constexpr unsigned joints=215,frames=361,fps=60;
    static constexpr unsigned generatedFrames=481;
    static constexpr size_t maximumBytes=32+16+generatedFrames*sizeof(float)+size_t(joints)*generatedFrames*sizeof(Joint);
    unsigned frameCount=frames,moveStart=48,moveEnd=138,guardFrame=184;
    std::vector<float> distanceCurve;
    float duration()const{return float(frameCount-1)/fps;}
    bool generated()const{return !distanceCurve.empty();}
    std::uint64_t checksum=0;
    bool decode(const std::vector<unsigned char>& bytes){
        values.clear();distanceCurve.clear();checksum=0;frameCount=frames;moveStart=48;moveEnd=138;guardFrame=184;
        if(bytes.size()<32)return false;
        const bool v2=!std::memcmp(bytes.data(),"D2PROOF2",8);
        if(!v2&&std::memcmp(bytes.data(),"D2PROOF1",8))return false;
        const unsigned count=v2?generatedFrames:frames;
        const size_t offset=v2?32+16+count*sizeof(float):32;
        const size_t expected=offset+size_t(joints)*count*sizeof(Joint);
        if(bytes.size()!=expected)return false;
        std::uint32_t header[4]{};std::uint64_t layout=0;
        std::memcpy(header,bytes.data()+8,16);std::memcpy(&layout,bytes.data()+24,8);
        if(header[0]!=(v2?2u:1u)||header[1]!=joints||header[2]!=count||header[3]!=fps||layout!=0x00db32d37719931full)return false;
        std::vector<float> curve;
        if(v2){
            std::uint32_t phase[4]{};std::memcpy(phase,bytes.data()+32,16);
            if(phase[0]!=120||phase[1]!=360||phase[2]!=405||phase[3]!=0)return false;
            curve.resize(count);std::memcpy(curve.data(),bytes.data()+48,count*sizeof(float));
            for(unsigned i=0;i<count;++i){
                if(!std::isfinite(curve[i])||curve[i]<0||curve[i]>5||(i&&curve[i]<curve[i-1]))return false;
                if(i<=phase[0]&&curve[i]!=0)return false;
                if(i>=phase[1]&&curve[i]!=curve[phase[1]])return false;
            }
            if(curve[phase[1]]<.2f)return false;
        }
        std::vector<Joint> v(joints*count);std::memcpy(v.data(),bytes.data()+offset,v.size()*sizeof(Joint));
        for(unsigned f=0;f<count;++f)for(unsigned j=0;j<joints;++j){const auto& a=v[f*joints+j];
            for(float x:a)if(!std::isfinite(x)||std::fabs(x)>3.f)return false;
            float n=0;for(unsigned k=0;k<4;++k)n+=a[k]*a[k];if(std::fabs(n-1)>.001f)return false;
            if(j<2){for(unsigned k=0;k<7;++k)if(std::fabs(a[k]-(k==3?1.f:0.f))>.00001f)return false;}
        }
        std::uint64_t hash=14695981039346656037ull;for(auto b:bytes){hash^=b;hash*=1099511628211ull;}
        checksum=hash;values=std::move(v);frameCount=count;distanceCurve=std::move(curve);
        if(v2){moveStart=120;moveEnd=360;guardFrame=405;}return true;
    }
    bool secondsAtDistance(float distance,float& seconds)const{
        if(!generated()||!std::isfinite(distance)||distance<0||distance>distanceCurve[moveEnd])return false;
        if(distance==0){seconds=float(moveStart)/fps;return true;}
        if(distance==distanceCurve[moveEnd]){seconds=float(moveEnd)/fps;return true;}
        const auto begin=distanceCurve.begin();const auto upper=std::upper_bound(begin+moveStart,begin+moveEnd+1,distance);
        const auto b=unsigned(upper-begin),a=b-1;
        const float span=distanceCurve[b]-distanceCurve[a];if(span<=0)return false;
        seconds=(a+(distance-distanceCurve[a])/span)/fps;return true;
    }
    bool sample(float seconds,std::vector<Joint>& out)const{
        if(values.size()!=joints*frameCount||!std::isfinite(seconds)||seconds<0||seconds>duration())return false;
        const float frame=seconds*fps;const auto a=(std::min)(unsigned(frame),frameCount-1),b=(std::min)(a+1,frameCount-1);const float t=frame-a;
        out.resize(joints);for(unsigned j=0;j<joints;++j){auto& v=out[j];const auto& x=values[a*joints+j];const auto& y=values[b*joints+j];
            float dot=0;for(unsigned k=0;k<4;++k)dot+=x[k]*y[k];float n=0;
            for(unsigned k=0;k<4;++k){v[k]=x[k]+((dot<0?-y[k]:y[k])-x[k])*t;n+=v[k]*v[k];}
            if(n<.5f)return false;n=std::sqrt(n);for(unsigned k=0;k<4;++k)v[k]/=n;
            for(unsigned k=4;k<7;++k)v[k]=x[k]+(y[k]-x[k])*t;
        }return true;
    }
};
