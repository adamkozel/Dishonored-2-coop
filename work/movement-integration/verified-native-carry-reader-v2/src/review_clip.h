#pragma once
#include "movement_bank.h"
// Offline authored poses for bounded visual review only. No root track, events,
// commands, external paths or gameplay outcomes are present in this format.
struct ReviewClip {
    using Joint=MovementBank::Joint;
    unsigned joints=0,frames=0;float fps=0;std::uint64_t layout=0,checksum=0;
    std::vector<Joint> values;
    static constexpr size_t maximumBytes=28+size_t(1801)*219*sizeof(Joint);
    float duration()const{return frames>1?float(frames-1)/fps:0;}
    bool decode(const std::vector<unsigned char>& bytes,unsigned count,std::uint64_t expected) {
        *this={};if(bytes.size()<28||bytes.size()>maximumBytes||std::memcmp(bytes.data(),"D2NATV01",8))return false;
        unsigned n=0,f=0;float rate=0;std::uint64_t hash=0;
        std::memcpy(&n,bytes.data()+8,4);std::memcpy(&f,bytes.data()+12,4);std::memcpy(&rate,bytes.data()+16,4);std::memcpy(&hash,bytes.data()+20,8);
        if((n!=215&&n!=219)||n!=count||hash!=expected||!expected||f<2||f>1801||(rate!=30&&rate!=60)||float(f-1)/rate>30||bytes.size()!=28+size_t(f)*n*sizeof(Joint))return false;
        std::vector<Joint> next(size_t(n)*f);std::memcpy(next.data(),bytes.data()+28,next.size()*sizeof(Joint));
        for(unsigned frame=0;frame<f;++frame)for(unsigned j=0;j<n;++j){const auto& v=next[frame*n+j];float norm=0;
            for(unsigned k=0;k<7;++k){if(!std::isfinite(v[k])||std::fabs(v[k])>3.f)return false;if(k<4)norm+=v[k]*v[k];}
            if(std::fabs(norm-1)>.001f)return false;
            if(j<2)for(unsigned k=0;k<7;++k)if(std::fabs(v[k]-(k==3?1.f:0.f))>.00001f)return false;
        }
        values=std::move(next);joints=n;frames=f;fps=rate;layout=hash;checksum=14695981039346656037ull;
        for(auto v:bytes){checksum^=v;checksum*=1099511628211ull;}return true;
    }
    Joint sample(unsigned j,float seconds)const {
        const float t=(std::max)(0.f,(std::min)(float(frames-1),seconds*fps));const auto a=unsigned(t),b=(std::min)(a+1,frames-1);
        return MovementBank::blend(values[a*joints+j],values[b*joints+j],t-a);
    }
};
