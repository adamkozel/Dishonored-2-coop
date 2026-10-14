#pragma once
#include <cmath>
#include <cstdint>
// Deduplicate rendered failure events within a validated stream incarnation.
// Late events are consumed without playback; an ambiguous native start is not retried.
struct PowerFailureFxOrder {
    std::uint64_t token=0,peerToken=0,event=0;
    bool consume(std::uint64_t own,std::uint64_t peer,std::uint64_t next,unsigned variant,float amount,bool current){
        if(!own||!peer||!current)return false;
        if(token!=own||peerToken!=peer){token=own;peerToken=peer;event=0;}
        if(variant!=15||!next||next<=event||!std::isfinite(amount)||amount<0||amount>1)return false;
        event=next;
        return amount<=.65f;
    }
};
