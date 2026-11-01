#pragma once
// Stage 8 (#253): the host decides what both worlds do at a shared mission end. On the result screen (logic 11) the
// native Continue runs RestartFromMemoryCheckpoint (the next mission's start save, cached at Results) and Replay Mission
// runs LoadGame <slot> (this mission's start save, slot 20 + chapter; confirmed runtime, take replay-dunwall-vm-v2044a:
// the host's Replay was LoadGame 21 and the partner's Continue loaded slot 22). The host's choice is sent as D2MD; the
// partner's own Continue and Replay wait for it while connected, and the partner then makes the same choice with its own
// saves.
#include <cstdint>
#include <cstring>
constexpr std::uint32_t kMissionDecisionMagic=0x444d3244; // "D2MD"
constexpr std::uint32_t kMissionContinue=1,kMissionReplay=2;
#pragma pack(push,1)
struct MissionDecisionPacket {
    std::uint32_t magic=kMissionDecisionMagic;std::uint16_t version=1,size=sizeof(MissionDecisionPacket);
    std::uint32_t role=0,decision=0;std::int32_t slot=-1;std::uint32_t reserved=0;
    std::uint64_t session=0,sequence=0,endEpoch=0;
    char startMap[160]{}; // Replay: the map name in the host's mission-start save (empty when unreadable)
};
#pragma pack(pop)
static_assert(sizeof(MissionDecisionPacket)==208);
inline bool validMissionDecision(const MissionDecisionPacket& p){
    if(p.magic!=kMissionDecisionMagic||p.version!=1||p.size!=sizeof(p)||p.role!=0||!p.sequence||!p.endEpoch||!std::memchr(p.startMap,0,sizeof(p.startMap)))return false;
    if(p.decision==kMissionContinue)return p.slot==-1&&!*p.startMap;
    if(p.decision==kMissionReplay)return p.slot>=0&&p.slot<=99&&(!*p.startMap||(!std::strncmp(p.startMap,"campaign/",9)&&!std::strpbrk(p.startMap," ;\"\r\n\t")));
    return false;
}
// The result screen is logic 11, and a message box over it (logic 3: Replay Mission's confirmation) once this world's
// mission end is shared. In replay-decision-vm-v2047a the host's LoadGame 21 ran with the box still open (logic 3, where
// v2044a had seen 11), so the checkpoint rules consumed it. A box during gameplay (a pause-menu load) is not the screen.
inline bool missionResultsScreen(unsigned logic,bool shared){return logic==11||(logic==3&&shared);}
// The native commands that carry out the host's choice in the partner's world: the one it dispatched, and after a Replay
// the RestartFromMemoryCheckpoint that LoadGame queues for the slot it cached (confirmed runtime, replay-decision-vm-v2047d:
// the host's LoadGame 21 was followed by kind 1 with cached slot 21, accepted; the partner's was consumed as her own press).
inline bool missionDecisionPermits(unsigned applied,unsigned kind){return kind==applied||(applied==kMissionReplay&&kind==kMissionContinue);}
// What happens to a native Continue (kind 1) or LoadGame (kind 2) on the result screen: -1 no opinion (the checkpoint
// rules decide), 0 pass, 1 consume (the partner waits for the host). The host's choice always runs; so does a partner's
// when the mission end was not shared, when it is not connected, or when the choice is the host's, dispatched by the mod.
inline int missionDecisionRule(unsigned role,unsigned kind,bool atResults,bool shared,bool connected,bool permitted){
    if(!atResults||(kind!=1&&kind!=2))return -1;
    if(role==0||permitted||!shared||!connected)return 0;
    return 1;
}
