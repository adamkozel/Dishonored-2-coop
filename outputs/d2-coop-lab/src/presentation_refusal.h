#pragma once
#include <array>
#include <cstdint>
// 1213i (#34): one refused presentation frame (a transient native skeleton read, an unstable or
// non-gameplay frame, a pose build failure) used to stop the whole partner presentation
// (end=lease=0) until the product watchdog re-armed it. In 1228e a lever use froze at 27% and the
// partner stayed unpresented for the rest of the 10 s window after a single refusal. A refusal now
// skips only that frame; the presentation stops after a sustained streak. Lifecycle loss (actor,
// world, session, freshness) is still stopped at once by serviceMovementBank.
struct PresentationRefusalGate {
    enum Reason:unsigned{None=0,NotOwned=1,SkeletonRead=2,Unstable=3,NotGameplay=4,Identity=5,Helpers=6,PoseBuild=7,KneeChain=8,Count=9};
    static constexpr unsigned stopStreak=20; // about 0.5 s of consecutive refused frames at 40 fps
    unsigned streak=0,last=0,longest=0;std::array<std::uint64_t,Count> reasons{};
    void accept(){streak=0;}
    // Records one refused frame; true when the presentation must stop.
    bool refuse(unsigned reason){
        last=reason<Count?reason:0;++reasons[last];
        if(++streak>longest)longest=streak;
        if(streak<stopStreak)return false;
        streak=0;return true;
    }
};
