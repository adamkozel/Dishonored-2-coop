#pragma once
#include "checkpoint_scope.h"
void checkpointScopeTests(){
    require(!checkpointMissionScope(kGuardMap,false,false),"1214gb: the lab loopback pair keeps the fixture checkpoint on the guard map");
    require(checkpointMissionScope(kGuardMap,true,false),"1214gb (#143): a product session plays the guard map (Dunwall's tower) as a mission");
    require(checkpointMissionScope(kGuardMap,false,true),"1214gb: a lab instance in an online two-account session plays it as a mission");
    require(checkpointMissionScope("campaign/void/void_01/void_01_p",false,false)&&checkpointMissionScope("campaign/dunwall/escape/streets/dunwall_escape_streets_p",false,false),
            "1214gb: every other map is a mission, as before");
    char product[260]="C:\\Users\\p\\Saved Games\\Arkane Studios\\Dishonored2\\base\\savegame";
    require(missionSaveFolder(product,sizeof(product),"savegame_coop")&&
            !std::strcmp(product,"C:\\Users\\p\\Saved Games\\Arkane Studios\\Dishonored2\\base\\savegame_coop"),
            "1214il (#171): a co-op session reads its autosaves from savegame_coop");
    char own[260]="C:\\g\\base\\savegame";
    require(missionSaveFolder(own,sizeof(own),"savegame")&&!std::strcmp(own,"C:\\g\\base\\savegame"),"1214il: the player's own saves stay in savegame");
    char lab[260]="C:/w/lab-instances/host/base/savegame";
    require(missionSaveFolder(lab,sizeof(lab),"savegame_coop")&&!std::strcmp(lab,"C:/w/lab-instances/host/base/savegame_coop"),
            "1214il: a lab instance in co-op mode reads savegame_coop too");
    char fixture[260]="C:\\w\\checkpoint-fixture";
    require(missionSaveFolder(fixture,sizeof(fixture),"savegame_coop")&&!std::strcmp(fixture,"C:\\w\\checkpoint-fixture"),
            "1214il: a fixture directory keeps its own layout");
    char bad[260]="C:\\g\\base\\savegame";
    require(!missionSaveFolder(bad,sizeof(bad),"savegame\\..\\x")&&!missionSaveFolder(bad,sizeof(bad),"profile")&&
            !missionSaveFolder(bad,sizeof(bad),"Savegame_coop"),"1214il: other root names are refused");
    char tight[24]="C:\\b\\savegame";
    require(!missionSaveFolder(tight,sizeof(tight),"savegame_coop_toolong"),"1214il: a folder that does not fit is refused");
}
