#pragma once
#include "presentation_hold.h"
#include <array>
#include <algorithm>
#include <cmath>
#include <cstdint>
// Source-observed regular-Heart presentation only. No projectile or damage.
// 1214eu (#102): held 2 is the Timepiece (models/weapons/timeshift_device). Its variants are the native left-arm
// gadget_timeshift scenes seen in both roles (takes 1590b/c): 4 scan_in (500 ms), 5 unfold_idle_loop (the peek, a
// loop; sent with elapsed 0), 6 scan_out (500), 7 scan_loop (the shift's raise, a loop; sent as the time since its
// instance began, capped at 500) and 8 scan_teleport (500). The partner plays the Heart's authored held-item acting:
// idle 336 and stretches of use 339 (raise .47-.60, hold .60, lower .60-1).
struct HeartPresentation {
    struct Sample {double time=0;std::uint64_t event=0;unsigned held=0,variant=0,elapsed=0,duration=0;};
    std::array<Sample,32> samples{};unsigned count=0;
    static unsigned duration(unsigned v){constexpr unsigned d[]={0,533,333,1000};return v<=3?d[v]:0;}
    static unsigned duration(unsigned held,unsigned v){
        if(held!=2)return duration(v);
        constexpr unsigned d[]={0,0,0,0,500,3333,500,500,500};return v<=8?d[v]:0;
    }
    static bool variantFor(unsigned held,unsigned v){return held==2?(!v||(v>=4&&v<=8)):v<=3;}
    static unsigned clip(unsigned v){return 336+v;}
    static unsigned clip(unsigned held,unsigned v){return held==2?(v?339u:336u):clip(v);}
    static float clipAmount(unsigned held,unsigned v,float a){
        if(held!=2)return a;
        switch(v){case 4:case 7:return .47f+.13f*a;case 5:return .60f;case 6:return .60f+.40f*a;case 8:return .60f+.10f*a;default:return a;}
    }
    // The Heart prop's wrist-relative hand track: idle 0 or use 3 at the clip time above.
    static unsigned trackVariant(unsigned held,unsigned v){return held==2?(v?3u:0u):v;}
    static bool valid(const Sample& s){return std::isfinite(s.time)&&s.event&&s.held<=2&&variantFor(s.held,s.variant)&&s.duration==duration(s.held,s.variant)&&s.elapsed<=s.duration&&(!s.variant||s.held);}
    static bool visible(unsigned held,unsigned variant,float amount){
        if(!held||held>2||!variantFor(held,variant)||!std::isfinite(amount)||amount<0||amount>1)return false;
        if(held==2)return true;
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
