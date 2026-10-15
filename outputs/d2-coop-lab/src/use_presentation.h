#pragma once
#include <array>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
// 1213: native use (player state 75, vtable 0x1c86ab8, ~15 ms per successful
// use - confirmed runtime 1210c) starts one authored full-body interaction on
// the remote body: 1 button/press or generic reach, 2 lever, 3 crank, 4 wheel/
// valve, 5 key/lock. The native first person has no lever/button animation, so
// the clip runs once on its own authored duration. No gameplay, usable state
// or camera effect: usable/door world state stays with usable/door sync.
struct UsePresentation {
    struct Sample {double time=0;std::uint32_t event=0;unsigned variant=0,elapsed=0,duration=0;};
    std::array<Sample,32> samples{};unsigned count=0;
    static unsigned duration(unsigned variant){static const unsigned d[]={0,2300,3200,4800,5200,3200};return variant<=5?d[variant]:0;}
    static unsigned clip(unsigned variant){return 368+variant;}
    static bool valid(const Sample& s){return std::isfinite(s.time)&&s.variant<=5&&s.duration==duration(s.variant)&&s.elapsed<=s.duration&&(!s.variant||s.event);}
    void clear(){count=0;}
    bool push(const Sample& s){
        if(!valid(s))return false;
        if(count){const auto& p=samples[count-1];
            if(s.time<=p.time)return false;
            // A use incarnation keeps its variant and never runs backwards; a new
            // use needs a newer event. Variant 0 (no use) is always admissible.
            if(s.variant&&s.event<p.event)return false;
            if(s.variant&&p.variant&&s.event==p.event&&(s.variant!=p.variant||s.elapsed<p.elapsed))return false;
            if(s.time-p.time>250)clear();}
        if(count==samples.size()){std::move(samples.begin()+1,samples.end(),samples.begin());--count;}
        samples[count++]=s;return true;
    }
    bool sample(double at,Sample& out,float& amount)const{
        if(!count||!std::isfinite(at)||at<samples[0].time||at-samples[count-1].time>200)return false;
        unsigned k=0;for(unsigned i=1;i<count&&samples[i].time<=at;++i)k=i;out=samples[k];
        if(!out.variant){amount=0;return true;}
        double elapsed=out.elapsed;
        if(k+1<count&&samples[k+1].event==out.event&&samples[k+1].variant==out.variant){const auto& n=samples[k+1];elapsed+=(n.elapsed-out.elapsed)*(at-out.time)/(n.time-out.time);}
        else elapsed+=(std::min)(100.,(std::max)(0.,at-out.time));
        amount=(std::min)(1.f,float(elapsed/out.duration));return true;
    }
};
// Source classification of the focused usable's entity name (lower case).
inline unsigned useVariantForName(const char* name){
    if(!name||!*name)return 1;
    char lower[192]{};for(unsigned i=0;i<sizeof(lower)-1&&name[i];++i){const char c=name[i];lower[i]=(c>='A'&&c<='Z')?char(c-'A'+'a'):c;}
    const auto has=[&](const char* s){return std::strstr(lower,s)!=nullptr;};
    // Whole '_'-separated token: "rich_sink_tap_01" and "tap_right" are taps, "tapestry" is not.
    const auto token=[&](const char* s){const auto n=std::strlen(s);
        for(const char* at=std::strstr(lower,s);at;at=std::strstr(at+1,s))
            if((at==lower||at[-1]=='_')&&(!at[n]||at[n]=='_'))return true;
        return false;};
    if(has("lever"))return 2;
    if(has("crank")||has("vise"))return 3; // 1213c: a bench vise is turned by its screw handle
    // 1213g: sink and dissection-table taps are native Usable_tap valves (confirmed static, mission entities).
    if(has("wheel")||has("valve")||has("faucet")||token("tap"))return 4;
    // 1213g: door latches are native Usable_latch; ring lockers are Usable_lock (confirmed static).
    if((has("key")&&!has("keyhole"))||has("lock")||has("latch"))return 5;
    return 1;
}
