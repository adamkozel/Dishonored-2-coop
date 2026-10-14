#pragma once
#include <array>
#include <algorithm>
#include <cmath>
#include <cstdint>
// Source animation semantics sampled on the delayed root's exact timeline.
// No game calls, root writes, native animation events or damage outcomes.
struct CombatPresentation {
    struct Sample {double time=0;std::uint64_t event=0;unsigned variant=0,phase=0,elapsed=0,duration=0;};
    std::array<Sample,32> samples{};unsigned count=0;
    void clear(){count=0;}
    bool push(const Sample& s){
        if(!std::isfinite(s.time)||!s.event||s.variant>14||
           (s.variant?(!s.phase||s.phase>2||!s.duration||s.duration>10000||s.elapsed>3600000||(s.phase==2&&s.variant!=7&&s.variant!=11)):(s.phase||s.duration||s.elapsed)))return false;
        if(count){const auto& p=samples[count-1];
            if(s.time<=p.time||s.event<p.event||(s.event==p.event&&(s.variant!=p.variant||s.phase!=p.phase||s.duration!=p.duration)))return false;
            if(s.time-p.time>250)clear();
        }
        if(count==samples.size()){std::move(samples.begin()+1,samples.end(),samples.begin());--count;}
        samples[count++]=s;return true;
    }
    bool sample(double at,Sample& out,float& amount)const{
        if(!count||!std::isfinite(at)||at<samples[0].time||at-samples[count-1].time>200)return false;
        unsigned index=0;for(unsigned i=1;i<count&&samples[i].time<=at;++i)index=i;
        out=samples[index];if(!out.variant){amount=0;return true;}
        double elapsed=out.elapsed;
        // Interpolate only within this scene event; never blend timestamps
        // across a combo replacement, cancellation or a guard transition.
        if(index+1<count&&samples[index+1].event==out.event&&samples[index+1].elapsed>=out.elapsed){
            const auto& next=samples[index+1];elapsed+=(next.elapsed-out.elapsed)*(at-out.time)/(next.time-out.time);
        }else elapsed+=(std::min)(100.,(std::max)(0.,at-out.time));
        amount=float(elapsed/out.duration);amount=out.phase==2?amount-std::floor(amount):(std::min)(1.f,amount);return true;
    }
};
