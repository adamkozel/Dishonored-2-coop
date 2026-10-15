#pragma once
#include "presentation_hold.h"
#include <array>
#include <algorithm>
#include <cmath>
#include <cstdint>
// Body and cosmetic equipment use the same delayed source-scene clock.
// This carries no projectile, damage, inventory mutation or resource path.
struct RangedPresentation {
    struct Sample {double time=0;std::uint64_t event=0;unsigned kind=0,variant=0,elapsed=0,duration=0,mechanism=0,shot=0;};
    std::array<Sample,32> samples{};unsigned count=0;
    static unsigned duration(unsigned v){constexpr unsigned d[]={0,500,500,666,1500,533,333,666,900,533,333};return v<=10?d[v]:0;}
    static unsigned kind(unsigned v){return v==1||(v>=3&&v<=6)?1:v==2||(v>=7&&v<=10)?2:0;}
    static unsigned slot(unsigned v){return v<=2?1:v==3||v==7?2:v==4||v==8?3:v==5||v==9?4:5;}
    static unsigned clip(unsigned k,unsigned v){return 201+(k-1)*6+(v?slot(v):0);}
    static bool valid(const Sample& s){return std::isfinite(s.time)&&s.event&&s.kind<=2&&s.variant<=10&&s.shot<=1&&(!s.shot||(s.kind==1&&s.variant==3))&&s.mechanism<=2&&(!s.mechanism||s.kind==2)&&
        s.duration==duration(s.variant)&&s.elapsed<=3600000&&
        (s.variant?s.kind==kind(s.variant):!s.elapsed);}
    void clear(){count=0;}
    bool push(const Sample& s){
        if(!valid(s))return false;
        if(count){const auto& p=samples[count-1];
            if(s.time<=p.time||s.event<p.event||(s.event==p.event&&(s.kind!=p.kind||s.variant!=p.variant||s.duration!=p.duration||s.elapsed<p.elapsed)))return false;
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
