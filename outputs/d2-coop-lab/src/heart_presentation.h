#pragma once
#include "presentation_hold.h"
#include <array>
#include <algorithm>
#include <cmath>
#include <cstdint>
// Source-observed regular-Heart presentation only. No projectile or damage.
struct HeartPresentation {
    struct Sample {double time=0;std::uint64_t event=0;unsigned held=0,variant=0,elapsed=0,duration=0;};
    std::array<Sample,32> samples{};unsigned count=0;
    static unsigned duration(unsigned v){constexpr unsigned d[]={0,533,333,1000};return v<=3?d[v]:0;}
    static unsigned clip(unsigned v){return 336+v;}
    static bool valid(const Sample& s){return std::isfinite(s.time)&&s.event&&s.held<=1&&s.variant<=3&&s.duration==duration(s.variant)&&s.elapsed<=s.duration&&(!s.variant||s.held);}
    static bool visible(unsigned held,unsigned variant,float amount){
        if(!held||variant>3||!std::isfinite(amount)||amount<0||amount>1)return false;
        if(variant==1)return amount>=.22f;
        if(variant==2)return amount<=.75f;
        return true;
    }
    void clear(){count=0;}
    bool push(const Sample& s){
        if(!valid(s))return false;
        if(count){const auto& p=samples[count-1];
            if(s.time<=p.time||s.event<p.event||(s.event==p.event&&(s.held!=p.held||s.variant!=p.variant||s.duration!=p.duration||s.elapsed<p.elapsed)))return false;
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
