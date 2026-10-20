#pragma once
#include <cstdint>
// 1214fr (#134): partner advances of gameflow nodes. The gameflow mirror (1214bg) sets every differing partner node
// back to the host's state, so an objective the partner completed in its own world was reverted within half a second
// unless the host's world completed it too (confirmed static, serviceGameflowMirror; take route-dunwall-lead-v1665b:
// Emily's search of Ramsey's body never stuck). A partner node that differs from a node the host has non-zero, and that
// the mirror did not set itself, is the partner's own advance: it is held against the mirror for a while and reported
// with the host state it advanced from (D2GV). The host sets it through the same native routine when its node still
// has that state, so the host's scripts react. Activations from 0 and deactivations to 0 are never reported, so each
// world's character branches (the asCorvo and asEmily nodes) stay in their own world.
// 1214fs: the magic is D2GV. 1214fr used D2GA, which the guard body packet (guard_body_protocol.h) already had: the
// host's guard body branch took every advance as a rejected guard body, so no advance reached the host (take
// route-dunwall-autosave-v1669b: the partner sent 20, the host received 0; confirmed static and runtime).
#pragma pack(push,1)
struct GameflowAdvanceEntry {std::uint32_t id=0;std::int32_t from=0,to=0;};
struct GameflowAdvancePacket {
    std::uint32_t magic=0x56473244; // "D2GV"
    std::uint16_t version=1,size=sizeof(GameflowAdvancePacket);
    std::uint32_t role=1,count=0;
    std::uint64_t session=0,mapId=0,sequence=0;
    GameflowAdvanceEntry entries[32]{};
};
#pragma pack(pop)
static_assert(sizeof(GameflowAdvancePacket)==424);
constexpr std::uint64_t kGameflowAdvanceHoldMs=10000;
// 1214fy: the age of a timestamp against a frame tick. A packet received during the frame's transport pump is stamped
// with GetTickCount64() after the frame sampled its tick, so tick-stamp wrapped to a huge age when the clock had moved
// on: the partner's mission end "expired" at once and its world stayed in Edge of the World while the host loaded The
// Good Doctor (campaign-eotw-vm-v1689a). A stamp later than the tick has age 0.
inline std::uint64_t tickAge(std::uint64_t now,std::uint64_t since){return now>since?now-since:0;}
inline bool gameflowAdvanceAllowed(std::int32_t from,std::int32_t to){
    return from>0&&from<16&&to>0&&to<16&&to!=from;
}
// The host's check of a received packet (it comes from the partner, role 1, in the host's own session and map).
inline bool validGameflowAdvance(const GameflowAdvancePacket& p,std::uint64_t peerSession,std::uint64_t peerSequence){
    if(p.magic!=0x56473244||p.version!=1||p.size!=sizeof(p)||p.role!=1||!p.session||p.session!=peerSession||
       p.sequence<=peerSequence||!p.count||p.count>32||!p.mapId)return false;
    for(std::uint32_t i=0;i<p.count;++i)if(!p.entries[i].id||!gameflowAdvanceAllowed(p.entries[i].from,p.entries[i].to))return false;
    return true;
}
// 1214fx: a partner advance is a change of this world's node away from a host state that stayed the same since the
// previous scan. 1214fr took any difference the mirror had not set for one, so when the host moved first (a node
// completed) and this world still showed the old state it had reached by itself, a "partner advance" back to that
// old state went to the host and the host applied it: in every host-led Dunwall take with the partner present on
// 1214fs or later, Ramsey's kill was undone (Obj_DW_EliminateRamsey back to active, gameflow [237,2,7,2]; takes
// 1678a, 1684a-1687a). When the host moved, the host wins: no report, and the mirror applies its state.
inline bool gameflowPartnerChanged(std::int32_t prevHave,std::int32_t prevHost,std::int32_t have,std::int32_t host){
    return prevHave==host&&prevHost==host&&have!=host&&gameflowAdvanceAllowed(host,have);
}
// What the host does with one reported advance, given its node's current state.
enum class GameflowAdvanceDecision {Apply,Already,Refuse};
inline GameflowAdvanceDecision gameflowAdvanceDecision(std::int32_t hostState,const GameflowAdvanceEntry& e){
    if(hostState==e.to)return GameflowAdvanceDecision::Already;
    return hostState==e.from&&gameflowAdvanceAllowed(e.from,e.to)?GameflowAdvanceDecision::Apply:GameflowAdvanceDecision::Refuse;
}
