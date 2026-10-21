#pragma once
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
