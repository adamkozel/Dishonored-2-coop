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
    std::uint64_t checksum=0;
    bool decode(const std::vector<unsigned char>& bytes){
        values.clear();checksum=0;
        constexpr size_t expected=32+size_t(joints)*frames*sizeof(Joint);
        if(bytes.size()!=expected||std::memcmp(bytes.data(),"D2PROOF1",8))return false;
        std::uint32_t header[4]{};std::uint64_t layout=0;
        std::memcpy(header,bytes.data()+8,16);std::memcpy(&layout,bytes.data()+24,8);
        if(header[0]!=1||header[1]!=joints||header[2]!=frames||header[3]!=fps||layout!=0x00db32d37719931full)return false;
        std::vector<Joint> v(joints*frames);std::memcpy(v.data(),bytes.data()+32,v.size()*sizeof(Joint));
        for(unsigned f=0;f<frames;++f)for(unsigned j=0;j<joints;++j){const auto& a=v[f*joints+j];
            for(float x:a)if(!std::isfinite(x)||std::fabs(x)>3.f)return false;
            float n=0;for(unsigned k=0;k<4;++k)n+=a[k]*a[k];if(std::fabs(n-1)>.001f)return false;
            if(j<2){for(unsigned k=0;k<7;++k)if(std::fabs(a[k]-(k==3?1.f:0.f))>.00001f)return false;}
        }
        std::uint64_t hash=14695981039346656037ull;for(auto b:bytes){hash^=b;hash*=1099511628211ull;}
        checksum=hash;values=std::move(v);return true;
    }
    bool sample(float seconds,std::vector<Joint>& out)const{
        if(values.size()!=joints*frames||!std::isfinite(seconds)||seconds<0||seconds>6.f)return false;
        const float frame=seconds*fps;const auto a=(std::min)(unsigned(frame),frames-1),b=(std::min)(a+1,frames-1);const float t=frame-a;
        out.resize(joints);for(unsigned j=0;j<joints;++j){auto& v=out[j];const auto& x=values[a*joints+j];const auto& y=values[b*joints+j];
            float dot=0;for(unsigned k=0;k<4;++k)dot+=x[k]*y[k];float n=0;
            for(unsigned k=0;k<4;++k){v[k]=x[k]+((dot<0?-y[k]:y[k])-x[k])*t;n+=v[k]*v[k];}
            if(n<.5f)return false;n=std::sqrt(n);for(unsigned k=0;k<4;++k)v[k]/=n;
            for(unsigned k=4;k<7;++k)v[k]=x[k]+(y[k]-x[k])*t;
        }return true;
    }
};
