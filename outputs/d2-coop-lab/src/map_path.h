#pragma once
// 1214by (#82 campaign): a script LevelTransition may name its map with
// backslashes. Death to the Empress's boat sends the player to
// "campaign\dunwall\return\streets\dunwall_return_streets_p.map" (confirmed static,
// boat_dunwall_return_p.entities). The engine loads it either way, so the level
// transition mirror normalizes the name before its "campaign/" check.
#include <cstring>

// Turns every backslash into a slash; true when the result names a campaign map.
inline bool normalizeCampaignMapPath(char* map){
    for(char* c=map;*c;++c)if(*c=='\\')*c='/';
    return !std::strncmp(map,"campaign/",9);
}
