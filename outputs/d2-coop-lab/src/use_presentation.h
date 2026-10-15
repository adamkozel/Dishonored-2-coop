#pragma once
#include "presentation_hold.h"
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
    // 1213k (#6, protocol 75): world position of the used mechanism (its entity origin), when known.
    struct Sample {double time=0;std::uint32_t event=0;unsigned variant=0,elapsed=0,duration=0;float target[3]{};bool targetValid=false;};
    std::array<Sample,32> samples{};unsigned count=0;
    // 1214m (#11 carry_object_place_throw, take 1269b): 6 holds a native movable object - the
    // generic reach approaches its contact pose and holds it while HoldObject49 lasts (up to 60 s);
    // 7 releases it (thrown or put down) by finishing the same reach in 1.3 s.
    static unsigned duration(unsigned variant){static const unsigned d[]={0,2300,3200,4800,5200,3200,60000,1300};return variant<=7?d[variant]:0;}
    static unsigned clip(unsigned variant){return variant>=6?369:368+variant;}
    static constexpr float kHoldAmount=.45f;
    static float clipAmount(unsigned variant,double elapsed,unsigned duration){
        if(variant==6)return (std::min)(kHoldAmount,float(elapsed/2300.));
        if(variant==7)return kHoldAmount+(1-kHoldAmount)*(std::min)(1.f,float(elapsed/duration));
        return (std::min)(1.f,float(elapsed/duration));}
    static bool valid(const Sample& s){
        if(s.targetValid&&!s.variant)return false;
        if(s.targetValid)for(float f:s.target)if(!std::isfinite(f)||std::fabs(f)>100000.f)return false;
        return std::isfinite(s.time)&&s.variant<=7&&s.duration==duration(s.variant)&&s.elapsed<=s.duration&&(!s.variant||s.event);}
    // #6: hand-reach weight over a clip: none while reaching out (0-12%), eased in to full contact
    // (30%), held while operating, eased out during recovery (72-92%).
    static float contact(float amount){
        const auto ease=[](float t){t=(std::max)(0.f,(std::min)(1.f,t));return t*t*(3-2*t);};
        if(!std::isfinite(amount))return 0;
        return ease((amount-.12f)/.18f)*(1-ease((amount-.72f)/.20f));}
    void clear(){count=0;}
    bool push(const Sample& s){
        if(!valid(s))return false;
        if(count){const auto& p=samples[count-1];
            if(s.time<=p.time)return false;
            // A use incarnation keeps its variant and never runs backwards; a new
            // use needs a newer event. Variant 0 (no use) is always admissible.
            if(s.variant&&s.event<p.event)return false;
            if(s.variant&&p.variant&&s.event==p.event&&(s.variant!=p.variant||s.elapsed<p.elapsed))return false;
            if(s.time-p.time>presentationHold().gapMs)clear();}
        if(count==samples.size()){std::move(samples.begin()+1,samples.end(),samples.begin());--count;}
        samples[count++]=s;return true;
    }
    bool sample(double at,Sample& out,float& amount)const{
        if(!count||!std::isfinite(at)||at<samples[0].time||at-samples[count-1].time>presentationHold().sampleMs)return false;
        unsigned k=0;for(unsigned i=1;i<count&&samples[i].time<=at;++i)k=i;out=samples[k];
        if(!out.variant){amount=0;return true;}
        double elapsed=out.elapsed;
        if(k+1<count&&samples[k+1].event==out.event&&samples[k+1].variant==out.variant){const auto& n=samples[k+1];elapsed+=(n.elapsed-out.elapsed)*(at-out.time)/(n.time-out.time);}
        else elapsed+=(std::min)(presentationHold().extrapolateMs,(std::max)(0.,at-out.time));
        amount=clipAmount(out.variant,elapsed,out.duration);return true;
    }
};
// 1213h (#5): a use ends when the source walks away - more than 0.6 m of HORIZONTAL travel from where
// it started. Vertical change never ends it: in 1228/1228b the Dunwall window's opening sash dropped the
// source 0.46 m off a ledge with only 0.36 m of horizontal travel, which the old 3D rule (0.668 m) read
// as walking away and ended the clip at 20-40% (confirmed runtime).
inline bool useMovedAway(const float* origin,const float* position){
    const float dx=position[0]-origin[0],dy=position[1]-origin[1];return dx*dx+dy*dy>.36f;}
// #6: a world point into the model space of an actor whose transform is NativeSkeleton::root
// (actor+0x300: position, then the rotation columns) - the inverse of the joint-to-world mapping
// used by grenade_fx/grapple_fx.
inline void useModelTarget(const float* root,const float* world,float* model){
    for(unsigned c=0;c<3;++c){model[c]=0;for(unsigned r=0;r<3;++r)model[c]+=root[3+c*3+r]*(world[r]-root[r]);}}
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
