#pragma once
#include "gameflow_advance.h"
void gameflowAdvanceTests(){
    require(gameflowPartnerChanged(2,2,3,2)&&gameflowPartnerChanged(2,2,5,2),"1214fx: the partner moved away from the host's unchanged state");
    require(!gameflowPartnerChanged(2,2,2,3)&&!gameflowPartnerChanged(2,2,5,3)&&!gameflowPartnerChanged(2,3,3,2),
            "1214fx: the host moved first (Ramsey's kill, takes 1678a-1687a): no partner advance, the host wins");
    require(!gameflowPartnerChanged(3,2,3,2)&&!gameflowPartnerChanged(2,2,2,2)&&!gameflowPartnerChanged(2,2,0,2),"1214fx: no change, an old difference, a deactivation");
    require(gameflowAdvanceAllowed(2,3)&&gameflowAdvanceAllowed(3,5),"1214fr: a partner completes or cancels a node the host has active");
    require(!gameflowAdvanceAllowed(0,2)&&!gameflowAdvanceAllowed(2,0)&&!gameflowAdvanceAllowed(3,3)&&!gameflowAdvanceAllowed(2,16)&&!gameflowAdvanceAllowed(-1,2),
            "1214fr: no activations from 0, no deactivations, no unchanged or out-of-range states");
    GameflowAdvancePacket p{};p.session=22;p.mapId=7;p.sequence=5;p.count=1;p.entries[0]={0x1234,2,3};
    require(validGameflowAdvance(p,22,4),"1214fr: the partner's advance packet");
    auto bad=p;bad.role=0;require(!validGameflowAdvance(bad,22,4),"1214fr: only the partner sends advances");
    bad=p;bad.session=23;require(!validGameflowAdvance(bad,22,4),"1214fr: another session is refused");
    require(!validGameflowAdvance(p,22,5),"1214fr: an old or repeated sequence is refused");
    bad=p;bad.count=0;require(!validGameflowAdvance(bad,22,4),"1214fr: an empty packet is refused");
    bad=p;bad.count=33;require(!validGameflowAdvance(bad,22,4),"1214fr: at most 32 entries");
    bad=p;bad.entries[0].from=0;require(!validGameflowAdvance(bad,22,4),"1214fr: an activation entry is refused");
    bad=p;bad.entries[0].id=0;require(!validGameflowAdvance(bad,22,4),"1214fr: a node id is required");
    bad=p;bad.mapId=0;require(!validGameflowAdvance(bad,22,4),"1214fr: a map is required");
    const GameflowAdvanceEntry e{0x1234,2,3};
    require(gameflowAdvanceDecision(2,e)==GameflowAdvanceDecision::Apply,"1214fr: the host applies an advance from its own state");
    require(gameflowAdvanceDecision(3,e)==GameflowAdvanceDecision::Already,"1214fr: the host already has the advanced state");
    require(gameflowAdvanceDecision(5,e)==GameflowAdvanceDecision::Refuse&&gameflowAdvanceDecision(0,e)==GameflowAdvanceDecision::Refuse,
            "1214fr: a host node that moved elsewhere refuses the advance");
}
