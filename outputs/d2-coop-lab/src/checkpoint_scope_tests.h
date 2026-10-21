#pragma once
#include "checkpoint_scope.h"
void checkpointScopeTests(){
    require(!checkpointMissionScope(kGuardMap,false,false),"1214gb: the lab loopback pair keeps the fixture checkpoint on the guard map");
    require(checkpointMissionScope(kGuardMap,true,false),"1214gb (#143): a product session plays the guard map (Dunwall's tower) as a mission");
    require(checkpointMissionScope(kGuardMap,false,true),"1214gb: a lab instance in an online two-account session plays it as a mission");
    require(checkpointMissionScope("campaign/void/void_01/void_01_p",false,false)&&checkpointMissionScope("campaign/dunwall/escape/streets/dunwall_escape_streets_p",false,false),
            "1214gb: every other map is a mission, as before");
}
