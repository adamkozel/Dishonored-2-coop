#pragma once
#include <array>
#include <algorithm>
#include <cmath>
#include <cstdint>
struct PowerPresentation {
    struct Sample{double time=0;std::uint64_t event=0;unsigned variant=0,elapsed=0,duration=0;};
    std::array<Sample,32> samples{};unsigned count=0;
    static unsigned duration(unsigned v){constexpr unsigned d[]={0,333,333,333,200,200,133,266,100,366,933,1000,533,300,433,333,466,1200,666,266,1333,400,266,400,333,1600,333,200,400,466,1200,666,1000,1000,1133,1000};return v<=35?d[v]:0;}
    static bool loop(unsigned v){return v==2||v==5||v==8||v==13||v==17||v==20||v==24||v==27||v==30||v==33;}
    static bool valid(const Sample& s){return std::isfinite(s.time)&&s.event&&s.variant<=35&&s.variant!=13&&s.duration==duration(s.variant)&&s.elapsed<=3600000&&(s.variant||!s.elapsed);}
    void clear(){count=0;}
    bool push(const Sample& s){
        if(!valid(s))return false;
        if(count){const auto& p=samples[count-1];if(s.time<=p.time||s.event<p.event||(s.event==p.event&&(s.variant!=p.variant||s.duration!=p.duration)))return false;
            if(s.time-p.time>250)clear();}
        if(count==samples.size()){std::move(samples.begin()+1,samples.end(),samples.begin());--count;}
        samples[count++]=s;return true;
    }
    bool sample(double at,Sample& out,float& amount)const{
        if(!count||!std::isfinite(at)||at<samples[0].time||at-samples[count-1].time>200)return false;
        unsigned k=0;for(unsigned i=1;i<count&&samples[i].time<=at;++i)k=i;out=samples[k];if(!out.variant){amount=0;return true;}
        double elapsed=out.elapsed;
        if(k+1<count&&samples[k+1].event==out.event&&samples[k+1].elapsed>=out.elapsed){const auto& n=samples[k+1];elapsed+=(n.elapsed-out.elapsed)*(at-out.time)/(n.time-out.time);}
        else elapsed+=(std::min)(100.,(std::max)(0.,at-out.time));
        amount=float(elapsed/out.duration);amount=loop(out.variant)?amount-std::floor(amount):(std::min)(1.f,amount);return true;
    }
};
