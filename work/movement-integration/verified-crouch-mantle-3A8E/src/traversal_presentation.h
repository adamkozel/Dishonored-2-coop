#pragma once
#include <array>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include "native_player_traversal_scene.h"
struct TraversalPresentation {
    struct Sample{double time=0;std::uint64_t event=0;unsigned variant=0,elapsed=0,duration=0,endStance=0;std::array<float,3> start{},goal{};};
    std::array<Sample,32> samples{};unsigned count=0;
    void clear(){count=0;}
    static bool valid(const Sample& s){
        if(!std::isfinite(s.time)||!s.event||s.variant>2||s.endStance!=(s.variant==2?1u:0u))return false;
        if(s.variant)return s.duration==500&&s.elapsed<=s.duration&&validTraversalGeometry(s.start,s.goal);
        return !s.duration&&!s.elapsed&&s.start==std::array<float,3>{}&&s.goal==std::array<float,3>{};
    }
    static bool same(const Sample& a,const Sample& b){return a.variant==b.variant&&a.duration==b.duration&&a.endStance==b.endStance&&a.start==b.start&&a.goal==b.goal;}
    bool push(const Sample& s){
        if(!valid(s))return false;
        if(count){const auto& p=samples[count-1];if(s.time<=p.time||s.event<p.event||(s.event==p.event&&(!same(s,p)||s.elapsed<p.elapsed)))return false;if(s.time-p.time>250)clear();}
        if(count==samples.size()){std::move(samples.begin()+1,samples.end(),samples.begin());--count;}samples[count++]=s;return true;
    }
    bool sample(double at,Sample& out,float& amount)const{
        if(!count||!std::isfinite(at)||at<samples[0].time||at-samples[count-1].time>200)return false;
        unsigned k=0;for(unsigned i=1;i<count&&samples[i].time<=at;++i)k=i;
        out=samples[k];if(!out.variant){amount=0;return true;}double elapsed=out.elapsed;
        if(k+1<count&&samples[k+1].event==out.event){const auto& n=samples[k+1];elapsed+=(n.elapsed-out.elapsed)*(at-out.time)/(n.time-out.time);}
        else elapsed+=(std::min)(100.,(std::max)(0.,at-out.time));amount=(std::min)(1.f,float(elapsed/out.duration));return true;
    }
};
