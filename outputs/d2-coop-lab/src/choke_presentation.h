#pragma once
#include "presentation_hold.h"
#include <array>
#include <algorithm>
#include <cmath>
#include <cstdint>
// Legacy choke field names carry paired actor presentation only.
// Variants1-4: rear choke;5: native fast-back lethal;6: native assassination_back
// lethal (1933 ms), presented with the fast-back donor clip normalized to its clock
// (no separate third-person donor exists). Never replay victim events.
struct ChokePresentation {
    struct Sample {double time=0;std::uint64_t event=0;unsigned variant=0,elapsed=0,duration=0;std::uint64_t spawner=0,definition=0;};
    std::array<Sample,32> samples{};unsigned count=0;
    static unsigned duration(unsigned v){constexpr unsigned d[]={0,600,1400,1800,1500,1333,1933};return v<=6?d[v]:0;}
    static unsigned clip(unsigned v){return 347+(v==6?5:v);}
    static bool valid(const Sample& s){return std::isfinite(s.time)&&s.event&&s.variant<=6&&s.duration==duration(s.variant)&&s.elapsed<=s.duration&&
        (s.variant?(s.spawner&&s.definition):(!s.elapsed&&!s.spawner&&!s.definition));}
    void clear(){count=0;}
    bool push(const Sample& s){
        if(!valid(s))return false;
        if(count){const auto& p=samples[count-1];
            if(s.time<=p.time||s.event<p.event||(s.event==p.event&&(s.variant!=p.variant||s.duration!=p.duration||s.spawner!=p.spawner||s.definition!=p.definition||s.elapsed<p.elapsed)))return false;
            if(s.time-p.time>presentationHold().gapMs)clear();}
        if(count==samples.size()){std::move(samples.begin()+1,samples.end(),samples.begin());--count;}
        samples[count++]=s;return true;
    }
    bool sample(double at,Sample& out,float& amount)const{
        if(!count||!std::isfinite(at)||at<samples[0].time||at-samples[count-1].time>presentationHold().sampleMs)return false;
        unsigned k=0;for(unsigned i=1;i<count&&samples[i].time<=at;++i)k=i;out=samples[k];
        if(!out.variant){amount=0;return true;}
        double elapsed=out.elapsed;
        if(k+1<count&&samples[k+1].event==out.event){const auto& n=samples[k+1];elapsed+=(n.elapsed-out.elapsed)*(at-out.time)/(n.time-out.time);}
        else elapsed+=(std::min)(presentationHold().extrapolateMs,(std::max)(0.,at-out.time));
        amount=(std::min)(1.f,float(elapsed/out.duration));return true;
    }
};
