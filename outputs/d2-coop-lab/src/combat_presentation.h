#pragma once
#include "presentation_hold.h"
#include <array>
#include <algorithm>
#include <cmath>
#include <cstdint>
// Source animation semantics sampled on the delayed root's exact timeline.
// No game calls, root writes, native animation events or damage outcomes.
struct CombatPresentation {
    static bool recovery(unsigned v){return v==15||v==16;}
    static unsigned clip(unsigned v,unsigned phase){
        return v==21?358:v==19||v==20?357:v==18?356:v==17?353:v>=15?48+v-15:v<=4?44+v-1:v<=6?48+v-5:v==7?(phase==2?51:50):v<=9?52:v==10||v==13?53:v==11?54:55;
    }
    static float continuationStart(unsigned v,unsigned previous,float amount,float weight){
        // A missed entry starts from the authored follow-through rather than
        // replaying a whole strike. Exact source impact timing is retained.
        return recovery(v)&&previous==v-10&&weight>.1f&&std::isfinite(amount)?
            (std::max)(0.f,(std::min)(1.f,amount)):.5f;
    }
    static float authoredAmount(unsigned v,float elapsed,float start){
        const float u=(std::max)(0.f,(std::min)(1.f,elapsed));
        return recovery(v)?start+(1-start)*u:u;
    }
    struct Sample {double time=0;std::uint64_t event=0;unsigned variant=0,phase=0,elapsed=0,duration=0;};
    std::array<Sample,32> samples{};unsigned count=0;
    void clear(){count=0;}
    bool push(const Sample& s){
        if(!std::isfinite(s.time)||!s.event||s.variant>21||
           (s.variant>=19&&(s.duration!=(s.variant==21?500u:533u)||s.phase!=1||s.elapsed>s.duration))||
           (s.variant==18&&((s.duration!=1000&&s.duration!=833)||s.phase!=1||s.elapsed>s.duration))||
           (s.variant==17&&(s.duration!=800||s.phase!=1||s.elapsed>s.duration))||
           (recovery(s.variant)&&(s.duration!=(s.variant==15?866u:666u)||s.phase!=1||s.elapsed>s.duration))||
           (s.variant?(!s.phase||s.phase>2||!s.duration||s.duration>10000||s.elapsed>3600000||(s.phase==2&&s.variant!=7&&s.variant!=11)):(s.phase||s.duration||s.elapsed)))return false;
        if(count){const auto& p=samples[count-1];
            if(s.time<=p.time||s.event<p.event||(s.event==p.event&&(s.variant!=p.variant||s.phase!=p.phase||s.duration!=p.duration)))return false;
            if(s.time-p.time>presentationHold().gapMs)clear();
        }
        if(count==samples.size()){std::move(samples.begin()+1,samples.end(),samples.begin());--count;}
        samples[count++]=s;return true;
    }
    bool sample(double at,Sample& out,float& amount)const{
        if(!count||!std::isfinite(at)||at<samples[0].time||at-samples[count-1].time>presentationHold().sampleMs)return false;
        unsigned index=0;for(unsigned i=1;i<count&&samples[i].time<=at;++i)index=i;
        out=samples[index];if(!out.variant){amount=0;return true;}
        double elapsed=out.elapsed;
        // Interpolate only within this scene event; never blend timestamps
        // across a combo replacement, cancellation or a guard transition.
        if(index+1<count&&samples[index+1].event==out.event&&samples[index+1].elapsed>=out.elapsed){
            const auto& next=samples[index+1];elapsed+=(next.elapsed-out.elapsed)*(at-out.time)/(next.time-out.time);
        }else elapsed+=(std::min)(presentationHold().extrapolateMs,(std::max)(0.,at-out.time));
        amount=float(elapsed/out.duration);amount=out.phase==2?amount-std::floor(amount):(std::min)(1.f,amount);return true;
    }
};
