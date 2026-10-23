#pragma once
#include <cstdio>
#include <cstring>
#include "guard_protocol.h"
// 1214gb (#143): the lab fixture checkpoint (the guard map with the isolated checkpoint directory's fixture save) is
// only for the lab's one-PC loopback pair. The guard map is also the campaign's first map, A Long Day in Dunwall's
// tower. In product mode D2COOP_CHECKPOINT_DIR is the player's own SLOT82, which no player has, so on that map a
// product partner's checkpoint sync could never arm. In clean-PC take product-two-account-v1705a the host fell to
// GameOver alone (death_peer_unavailable, peer arm 0) while the partner played on. A product session, and a lab
// instance in an online (two-account) session, play the guard map as a mission: the digest is the mission autosave
// (map and chapter), and the autosave pair arms there too.
inline bool checkpointMissionScope(const char* map,bool product,bool online){return std::strcmp(map,kGuardMap)!=0||product||online;}
// 1214il (#171): the mission autosave lies under the game's current save root. A co-op session (1214hq, native_menu_ui.h)
// saves to the sibling folder savegame_coop, but the checkpoint directory names the player's own savegame\SLOT82, so the
// installed partner read no autosave of its map and its checkpoint sync never armed (clean-PC take
// product-two-account-v1755a: the host sent 157 hellos and received none, and its death was not shared). When the folder
// beside the checkpoint directory is the game's savegame, it follows the root name; a lab fixture directory keeps its
// own layout. A root name that is not savegame or a savegame sibling is refused.
inline bool missionSaveFolder(char* folder,std::size_t size,const char* rootName){
    char* cut=std::strrchr(folder,'\\');char* slash=std::strrchr(folder,'/');if(slash>cut)cut=slash;if(!cut)return false;
    if(std::strncmp(rootName,"savegame",8))return false;
    for(const char* c=rootName;*c;++c)if(!((*c>='a'&&*c<='z')||*c=='_'))return false;
    if(_stricmp(cut+1,"savegame")||!std::strcmp(rootName,"savegame"))return true;
    const std::size_t at=static_cast<std::size_t>(cut+1-folder);
    const int written=std::snprintf(folder+at,size-at,"%s",rootName);return written>0&&static_cast<std::size_t>(written)<size-at;
}
