#pragma once
#include <array>
#include <algorithm>
#include <cmath>
#include <cstdint>
struct EquipmentPresentation {
    struct Sample{double time=0;std::uint64_t event=0;unsigned sword=0,variant=0,elapsed=0,duration=0;};
    std::array<Sample,32> samples{};unsigned count=0;
    void clear(){count=0;}
    bool push(const Sample& s){
        if(!std::isfinite(s.time)||!s.event||s.sword<1||s.sword>2||s.variant>5||
            (s.variant?(!s.duration||s.duration>10000||s.elapsed>3600000):(s.duration||s.elapsed)))return false;
        if(count){const auto& p=samples[count-1];if(s.time<=p.time||s.event<p.event||
            (s.event==p.event&&(s.sword!=p.sword||s.variant!=p.variant||s.duration!=p.duration)))return false;
            if(s.time-p.time>250)clear();}
        if(count==samples.size()){std::move(samples.begin()+1,samples.end(),samples.begin());--count;}
        samples[count++]=s;return true;
    }
    bool sample(double at,Sample& out,float& amount)const{
        if(!count||!std::isfinite(at)||at<samples[0].time||at-samples[count-1].time>200)return false;
        unsigned k=0;for(unsigned i=1;i<count&&samples[i].time<=at;++i)k=i;
        out=samples[k];if(!out.variant){amount=0;return true;}
        double elapsed=out.elapsed;
        if(k+1<count&&samples[k+1].event==out.event&&samples[k+1].elapsed>=out.elapsed){const auto& n=samples[k+1];elapsed+=(n.elapsed-out.elapsed)*(at-out.time)/(n.time-out.time);}
        else elapsed+=(std::min)(100.,(std::max)(0.,at-out.time));
        amount=(std::min)(1.f,float(elapsed/out.duration));return true;
    }
};
