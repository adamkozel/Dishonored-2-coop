#pragma once
#include "presentation_hold.h"
#include <array>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include "native_player_traversal_scene.h"
#include "native_player_vault.h"
struct TraversalPresentation {
    // Variant 3 uses normalized distance [0,1000], never a wall-clock duration.
    struct Sample{double time=0;std::uint64_t event=0;unsigned variant=0,elapsed=0,duration=0,endStance=0;std::array<float,3> start{},goal{},point{};float height=0;};
    std::array<Sample,32> samples{};unsigned count=0;
    void clear(){count=0;}
    static bool valid(const Sample& s){
        if(!std::isfinite(s.time)||!s.event||s.variant>16)return false;
        if(s.variant>=7){
            const unsigned phase=(s.variant-7)%5;constexpr unsigned durations[]={1500,500,1000,1500,1000};
            return s.duration==durations[phase]&&s.elapsed<=(phase==1?3600000u:s.duration)&&
                (phase==1?s.endStance<=1:s.endStance==unsigned(phase>=3))&&s.start==std::array<float,3>{}&&s.goal==std::array<float,3>{}&&s.point==std::array<float,3>{}&&s.height==0;
        }
        if(s.variant==6?s.endStance>1:s.endStance!=((s.variant==2||s.variant==5)?1u:0u))return false;
        if(s.variant==3)return s.duration==1000&&s.elapsed<=1000&&validVaultGeometry(s.start,s.point,s.goal,s.height);
        if(s.point!=std::array<float,3>{}||s.height!=0)return false;
        if(s.variant==6)return s.duration==1000&&s.elapsed<=1000&&s.start==std::array<float,3>{}&&s.goal==std::array<float,3>{};
        if(s.variant)return s.duration==(s.variant>=4?833u:500u)&&s.elapsed<=s.duration&&validTraversalGeometry(s.start,s.goal);
        return !s.duration&&!s.elapsed&&s.start==std::array<float,3>{}&&s.goal==std::array<float,3>{};
    }
    static bool same(const Sample& a,const Sample& b){return a.variant==b.variant&&a.duration==b.duration&&(a.variant==6||a.endStance==b.endStance)&&a.start==b.start&&a.goal==b.goal&&a.point==b.point&&a.height==b.height;}
    bool push(const Sample& s){
        if(!valid(s))return false;
        if(count){const auto& p=samples[count-1];if(s.time<=p.time||s.event<p.event||(s.event==p.event&&(!same(s,p)||s.elapsed<p.elapsed)))return false;if(s.time-p.time>presentationHold().gapMs)clear();}
        if(count==samples.size()){std::move(samples.begin()+1,samples.end(),samples.begin());--count;}samples[count++]=s;return true;
    }
    bool sample(double at,Sample& out,float& amount)const{
        if(!count||!std::isfinite(at)||at<samples[0].time||at-samples[count-1].time>presentationHold().sampleMs)return false;
        unsigned k=0;for(unsigned i=1;i<count&&samples[i].time<=at;++i)k=i;
        out=samples[k];if(!out.variant){amount=0;return true;}double elapsed=out.elapsed;
        if(k+1<count&&samples[k+1].event==out.event){const auto& n=samples[k+1];elapsed+=(n.elapsed-out.elapsed)*(at-out.time)/(n.time-out.time);}
        else if(out.variant!=3)elapsed+=(std::min)(presentationHold().extrapolateMs,(std::max)(0.,at-out.time));
        // The native 500ms keyhole hold is a logical loop. Preserve the
        // complete-body donor's 3.1667s look/breathing cadence, using native
        // accumulated hold time. Network arrival timing never speeds it up.
        if(out.variant==8||out.variant==13){amount=float(elapsed/(9500./3.));amount-=std::floor(amount);}
        else amount=(std::min)(1.f,float(elapsed/out.duration));return true;
    }
};
