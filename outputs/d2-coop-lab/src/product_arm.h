#pragma once
#include <cstdint>
// 1214ga (#145): the product controller's arming state (1181), split out of session_product.h so the new-world rule
// has a unit test. Times are GetTickCount64 values (ULONGLONG).
struct ProductArm {
    std::uint64_t epoch=0,peerEpoch=0,map=0;unsigned step=0,worlds=0;unsigned long long nextAt=0;
    std::uint64_t dispatched=0,hostKeys=0,leaveKeys=0,maintenance=0,repairs=0,quickRepairs=0;unsigned long long lostAt=0,liveSince=0,repairBackoff=0;unsigned missing=0,repairMask=0,repairStep=0;
    const char* result="idle";const char* last="";
};
// A new shared world: a map load, a checkpoint or death restart. Candidates 1181-1203 reset the whole state here and ran
// the 22 arming steps again in every world. From 1203b to 1214fz a comment merged into that line kept the reset from
// running (#145; confirmed runtime: soak mission-soak-v1698a reported worlds 0 with step 22 after shared restarts), so
// the steps ran once per game process and every system armed once kept itself armed across worlds (the systems' own
// re-arms, 1214ah). All of the stage 4-7 validation ran that way, the campaign on two accounts included, so that is
// kept: the step and the counters stay, and a new world now counts and restarts the repair backoff, as the 1203b
// comment meant. F8 and coop_product_auto still reset everything, so a new session arms from step 0.
inline void productNewWorld(ProductArm& a,std::uint64_t epoch,std::uint64_t peerEpoch,std::uint64_t map,unsigned long long tick){
    ++a.worlds;a.repairBackoff=0;
    a.epoch=epoch;a.peerEpoch=peerEpoch;a.map=map;a.nextAt=tick+3000;a.result="world_settling";
}
