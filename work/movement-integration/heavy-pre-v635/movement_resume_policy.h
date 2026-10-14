#pragma once
#include <array>
#include <cstdint>
// Resume only an explicitly armed movement presentation after a native menu
// pause in the same world/actor/session. A load, replacement, stale stream or
// gameplay failure is not permission to reuse an old incarnation.
struct MovementResumePolicy {
    using Identity=std::array<std::uint64_t,8>;
    enum Phase {Off,Running,Suspended,Settling,Rearming,Fault};
    enum Decision {Wait,Continue,Restore,Reject};
    Identity identity{};Phase phase=Off;std::uint64_t since=0,deadline=0,missingSince=0;
    bool enabled()const{return phase!=Off&&phase!=Fault;}
    bool arm(const Identity& value,bool live){
        if(!live)return false;for(auto v:value)if(!v)return false;
        identity=value;phase=Running;since=deadline=missingSince=0;return true;
    }
    void off(){phase=Off;since=deadline=missingSince=0;}
    bool running(bool live,std::uint64_t now){
        if(phase!=Running)return false;
        if(live){missingSince=0;return true;}
        if(!missingSince)missingSince=now;
        // Native pose ownership can retire before the next sampled logic
        // packet exposes the menu transition. This grace never rearms anything.
        if(now>=missingSince&&now-missingSince>=350){phase=Fault;return false;}
        return true;
    }
    Decision observe(const Identity& value,bool paused,bool ready,bool fresh,std::uint64_t now){
        if(!enabled())return Wait;
        if(value!=identity){phase=Fault;return Reject;}
        if(paused){phase=Suspended;since=deadline=missingSince=0;return Wait;}
        if(phase==Running)return Continue;
        if(!fresh||!ready){phase=Suspended;since=deadline=0;return Wait;}
        if(phase==Suspended){phase=Settling;since=now;return Wait;}
        if(phase==Settling){if(now<since||now-since<300)return Wait;phase=Rearming;deadline=now+10000;}
        if(phase==Rearming){if(now>=deadline){phase=Fault;return Reject;}return Restore;}
        return Wait;
    }
    bool restored(){if(phase!=Rearming)return false;phase=Running;since=deadline=missingSince=0;return true;}
};
