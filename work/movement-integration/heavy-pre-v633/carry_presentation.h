#pragma once
#include <array>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <string>
// Native source semantics only. A target key identifies an authored spawner
// and definition, never a pointer or a remote entity-pool index.
inline std::uint64_t carryIdentityKey(const std::string& value){
    if(value.empty()||value.size()>=192)return 0;
    std::uint64_t h=14695981039346656037ull;
    for(unsigned char c:value){if(c<32||c>126)return 0;h^=c;h*=1099511628211ull;}
    return h;
}
inline float carryTravelWeight(float speed,bool active){
    // Native FP carry walk begins above 1 m/s, but the full body must step
    // during slower travel too (0.8 m/s crouch observed in carrier Corvo v2).
    if(!active||!std::isfinite(speed))return 0;
    return (std::max)(0.f,(std::min)(1.f,(speed-.04f)/.3f));
}
// Preparation has no native Midnight scene clock. This measures how long
// the exact Carry45+Equip53 preparation was continuously observed locally.
// 300ms is the authored gather window, not a claimed native scene duration.
struct CarryPreparationClock {
    std::uint64_t start=0,last=0;std::uintptr_t state=0;unsigned corpse=0;
    void clear(){*this={};}
    unsigned observe(std::uint64_t tick,std::uintptr_t pointer,unsigned target){
        if(!tick||!pointer||!target){clear();return 0;}
        if(!start||pointer!=state||target!=corpse||tick<last||tick-last>200){start=tick;state=pointer;corpse=target;}
        last=tick;return unsigned((std::min)(tick-start,std::uint64_t(300)));
    }
};
inline float carryPickupAmount(unsigned variant,float phase){
    phase=(std::max)(0.f,(std::min)(1.f,phase));
    return variant==7?.30f*phase/1.4f:(std::min)(1.f,(.30f+1.10f*phase)/1.4f);
}
inline bool carryVictimPickupAmount(unsigned variant,unsigned stance,float phase,float& amount,bool allowCrouched=false){
    if(!std::isfinite(phase)||phase<0||phase>1)return false;
    if(variant==2||variant==3){amount=1;return true;}
    if(stance>1||(stance==1&&!allowCrouched))return false;
    if(variant==7){amount=0;return true;}
    if(variant!=1)return false;
    // Separate contact experiment: floor snapshot remains intact during
    // gather, then blends during the source hoist. No local timer advances it.
    amount=std::clamp((phase-.08f)/.72f,0.f,1.f);return true;
}
struct CarryPresentation {
    struct Sample {
        double time=0;std::uint64_t event=0;
        unsigned variant=0,elapsed=0,duration=0,stance=0,incarnation=0;
        std::uint64_t spawner=0,definition=0;
        std::array<float,3> throwVelocity{};
    };
    std::array<Sample,32> samples{};unsigned count=0;
    static unsigned duration(unsigned v){constexpr unsigned d[]={0,1000,3000,1600,1000,666,1000,300,666};return v<=8?d[v]:0;}
    static bool loop(unsigned v){return v==2||v==3;}
    static bool valid(const Sample& s){
        float length=0;for(float f:s.throwVelocity){if(!std::isfinite(f))return false;length+=f*f;}
        if(s.variant==8?(s.stance!=0||s.elapsed>666||length<15.84f||length>16.16f):(length!=0))return false;
        return std::isfinite(s.time)&&s.event&&s.variant<=8&&s.duration==duration(s.variant)&&s.elapsed<=3600000&&s.stance<=1&&
            (s.variant!=7||s.elapsed<=300)&&
            (s.variant?(s.incarnation&&s.spawner&&s.definition):(!s.elapsed&&!s.stance&&!s.incarnation&&!s.spawner&&!s.definition));
    }
    void clear(){count=0;}
    bool push(const Sample& s){
        if(!valid(s))return false;
        Sample next=s;
        if(count){const auto& p=samples[count-1];
            // Runtime native carry clocks wrap within the same scene handle
            // (2991 -> 76 ms observed). Unwrap only a plausible forward
            // boundary; an ordinary backwards clock must remain rejected.
            if(s.event==p.event&&loop(s.variant)&&s.duration==p.duration&&s.elapsed<p.elapsed){
                if(s.elapsed>=s.duration)return false;
                next.elapsed=p.elapsed-p.elapsed%p.duration+s.elapsed;
                if(next.elapsed<p.elapsed)next.elapsed+=p.duration;
                if(double(next.elapsed-p.elapsed)>(std::min)(250.,(std::max)(80.,(s.time-p.time)*2.+50.)))return false;
            }
            if(s.time<=p.time||s.event<p.event||(s.event==p.event&&(s.variant!=p.variant||s.duration!=p.duration||
               s.incarnation!=p.incarnation||s.spawner!=p.spawner||s.definition!=p.definition||next.elapsed<p.elapsed)))return false;
            if(s.time-p.time>250)clear();
        }
        if(count==samples.size()){std::move(samples.begin()+1,samples.end(),samples.begin());--count;}
        samples[count++]=next;return true;
    }
    bool sample(double at,Sample& out,float& amount)const{
        if(!count||!std::isfinite(at)||at<samples[0].time||at-samples[count-1].time>200)return false;
        unsigned k=0;for(unsigned i=1;i<count&&samples[i].time<=at;++i)k=i;out=samples[k];
        if(!out.variant){amount=0;return true;}
        double elapsed=out.elapsed;
        if(k+1<count&&samples[k+1].event==out.event){const auto& n=samples[k+1];elapsed+=(n.elapsed-out.elapsed)*(at-out.time)/(n.time-out.time);}
        else elapsed+=(std::min)(100.,(std::max)(0.,at-out.time));
        amount=loop(out.variant)?float(std::fmod(elapsed,double(out.duration))/out.duration):(std::min)(1.f,float(elapsed/out.duration));
        return true;
    }
};
// Variant5 has no native ragdoll event:166ms is its victim animation endpoint,
// used only by the isolated compact-release experiment, not a claimed callback.
inline unsigned carryReleaseHandoffMs(unsigned variant){return variant==4?656u:variant==5?166u:variant==6?500u:variant==8?280u:0u;}
inline unsigned carryReleaseElapsedMs(unsigned variant,float amount){
    return !carryReleaseHandoffMs(variant)||!std::isfinite(amount)||amount<0||amount>1?0xffffffffu:
        static_cast<unsigned>(amount*CarryPresentation::duration(variant));
}
