#pragma once
#include <array>
#include <algorithm>
#include <cmath>
#include <cstdint>
// Source movement semantics on the SAME clock as root/scene presentation.
// Receiving a future landing, lean reversal or sprint exit does not apply it
// to an older body pose. Inputs never infer animation state.
struct MovementSemantics {
    struct Sample {
        double time=0;std::uint64_t event=0;unsigned state=0;
        float leanSide=0,leanUp=0,startZ=0,targetZ=0;unsigned airStage=0,remaining=0;
    };
    std::array<Sample,32> samples{};unsigned count=0;
    void clear(){count=0;}
    bool push(const Sample& s){
        if(!std::isfinite(s.time)||!s.event||s.state>12||!std::isfinite(s.leanSide)||!std::isfinite(s.leanUp)||
           std::fabs(s.leanSide)>1||s.leanUp<0||s.leanUp>1||(s.state!=5&&(s.leanSide||s.leanUp))||
           !std::isfinite(s.startZ)||!std::isfinite(s.targetZ)||std::fabs(s.startZ)>100000||std::fabs(s.targetZ)>100000||s.remaining>3)return false;
        // Packet validation permits power+air for observed Dark Vision and
        // Windblast. Keep that air clock on the same delayed body timeline.
        if(s.state==3||(s.state==10&&s.airStage)){
            if(s.airStage==1){if(s.targetZ-s.startZ<.05f||s.targetZ-s.startZ>10.f)return false;}
            else if(s.airStage!=2||s.startZ||s.targetZ||s.remaining)return false;
        }else if(s.airStage||s.startZ||s.targetZ||s.remaining)return false;
        if(count){const auto& prev=samples[count-1];
            if(s.time<=prev.time||s.event<prev.event||(s.event==prev.event&&s.state!=prev.state))return false;
            if(s.time-prev.time>250)clear();
        }
        if(count==samples.size()){std::move(samples.begin()+1,samples.end(),samples.begin());--count;}
        samples[count++]=s;return true;
    }
    bool sample(double at,Sample& out)const{
        if(!count||!std::isfinite(at)||at<samples[0].time||at-samples[count-1].time>200)return false;
        unsigned index=0;for(unsigned i=1;i<count&&samples[i].time<=at;++i)index=i;
        out=samples[index];
        if(out.state==5&&index+1<count&&samples[index+1].event==out.event){
            const auto& next=samples[index+1];const float t=float((at-out.time)/(next.time-out.time));
            out.leanSide+=(next.leanSide-out.leanSide)*t;out.leanUp+=(next.leanUp-out.leanUp)*t;
        }
        return true;
    }
};
