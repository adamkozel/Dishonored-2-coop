#pragma once
#include "world_protocol.h"
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>
// 1214jo/1214jr (#189): the partner's world converges to the pickups the host's world no longer has. A partner joining a
// host who resumed from a mid-mission save loads the mission from its start, so the pickups the host took before
// saving are still there (route-resumed-join-v1871c: the banana). The host sends the names of every pickup actor in
// its world, active or dormant (1214jr; route-resumed-join-v1875a: about a fifth of the pickups are dormant in one
// world and active in the other, so the active ones alone were not a presence), and the partner awards itself natively
// each eligible pickup absent from it, as a shared pickup would have been.
// Pickups spawned at load are named "<declaration>_<entity slot>" and their slots can differ between worlds
// (ammo_bolt_pack_regular_12140 and _12358): those match by base name within 3 m. Character variants
// (pickups_news_kg_ontherun_corvo/_emily) are never caught up. Since 1214jv (#203) the partner sends its presence too,
// so a pickup outside the shared catalog that the partner took leaves the host's world as well (authored names only).
constexpr unsigned kPickupPresenceMax=2048;
#pragma pack(push,1)
struct PickupPresenceEntry {std::uint64_t name=0,base=0;float position[3]{};std::uint32_t reserved=0;};
struct PickupPresenceHeader {
    std::uint32_t magic=0x50503244; // "D2PP"
    std::uint16_t version=2,size=sizeof(PickupPresenceHeader);
    std::uint32_t role=0,count=0;
    std::uint64_t sequence=0,sourceSession=0,targetSession=0,sourceEpoch=0,targetEpoch=0,mapId=0;
};
struct PickupPresencePacket {PickupPresenceHeader header{};PickupPresenceEntry entries[kPickupPresenceMax]{};};
#pragma pack(pop)
static_assert(sizeof(PickupPresenceEntry)==32&&sizeof(PickupPresenceHeader)==64);
// Only the entries in use travel.
inline std::size_t pickupPresenceBytes(std::uint32_t count){return sizeof(PickupPresenceHeader)+static_cast<std::size_t>(count)*sizeof(PickupPresenceEntry);}
inline std::uint64_t pickupNameHash(const std::string& name){
    std::uint64_t h=1469598103934665603ull;for(unsigned char c:name){h^=c;h*=1099511628211ull;}
    return h?h:1;
}
// "<declaration>_<entity slot>", the slot being the handle's high 16 bits (worldIdentityEntity).
inline bool pickupRuntimeNamed(const std::string& name,std::uint32_t handle){
    const auto at=name.find_last_of('_');if(at==std::string::npos||at+1>=name.size()||name.size()-at-1>5)return false;
    unsigned slot=0;for(auto i=at+1;i<name.size();++i){if(name[i]<'0'||name[i]>'9')return false;slot=slot*10+static_cast<unsigned>(name[i]-'0');}
    return slot==(handle>>16);
}
inline std::string pickupBaseName(const std::string& name){const auto at=name.find_last_of('_');return at==std::string::npos?name:name.substr(0,at);}
inline bool pickupCharacterNamed(const std::string& name){return name.find("corvo")!=std::string::npos||name.find("emily")!=std::string::npos;}
struct PickupPresenceItem {std::string name;std::uint32_t handle=0;float position[3]{};};
inline PickupPresenceEntry pickupPresenceEntry(const PickupPresenceItem& item){
    PickupPresenceEntry e{};e.name=pickupNameHash(item.name);
    if(pickupRuntimeNamed(item.name,item.handle)){e.base=pickupNameHash(pickupBaseName(item.name));std::copy(item.position,item.position+3,e.position);}
    return e;
}
// Sorted by name, one entry per name (duplicate names keep the first).
inline std::vector<PickupPresenceEntry> pickupPresenceEntries(const std::vector<PickupPresenceItem>& items){
    std::vector<PickupPresenceEntry> out;for(const auto& i:items)out.push_back(pickupPresenceEntry(i));
    std::stable_sort(out.begin(),out.end(),[](const auto& a,const auto& b){return a.name<b.name;});
    out.erase(std::unique(out.begin(),out.end(),[](const auto& a,const auto& b){return a.name==b.name;}),out.end());
    return out;
}
// The peer's presence, from either role since 1214jv (#203), scoped to both sessions, both world epochs and the shared
// map, with exactly its entries' bytes; names are sorted and unique, and only runtime-named entries carry a base and a
// position.
inline bool validPickupPresence(const PickupPresencePacket& p,std::size_t bytes,const CoopPosePacket& local,const CoopPosePacket& peer){
    const auto& h=p.header;
    if(h.magic!=0x50503244||h.version!=2||h.size!=sizeof(PickupPresenceHeader)||h.role>1||h.role!=peer.role||h.role==local.role||h.count>kPickupPresenceMax||
       bytes!=pickupPresenceBytes(h.count)||!h.sequence||!coopWorldPairReady(local,peer)||h.sourceSession!=peer.session||h.targetSession!=local.session||
       h.sourceEpoch!=peer.worldEpoch||h.targetEpoch!=local.worldEpoch||h.mapId!=local.mapId)return false;
    for(unsigned i=0;i<h.count;++i){const auto& e=p.entries[i];
        if(!e.name||(i&&e.name<=p.entries[i-1].name)||e.reserved)return false;
        for(float f:e.position)if(!std::isfinite(f)||(!e.base&&f!=0.f))return false;}
    return true;
}
inline bool pickupInPresence(const PickupPresenceItem& item,const std::vector<PickupPresenceEntry>& host){
    const auto name=pickupNameHash(item.name);
    if(std::binary_search(host.begin(),host.end(),PickupPresenceEntry{name},[](const auto& a,const auto& b){return a.name<b.name;}))return true;
    if(!pickupRuntimeNamed(item.name,item.handle))return false;
    const auto base=pickupNameHash(pickupBaseName(item.name));
    for(const auto& e:host)if(e.base==base){float d=0;for(unsigned k=0;k<3;++k)d+=(e.position[k]-item.position[k])*(e.position[k]-item.position[k]);if(d<=9.f)return true;}
    return false;
}
// A local pickup the peer's world no longer has, in its last two presences. The host catches up only authored names
// (1214jv, #203): a pickup spawned at run time in the host's world (an NPC's drop) can be missing from the partner's
// world without the partner having taken it.
inline bool pickupCatchUpTarget(const PickupPresenceItem& item,const std::vector<PickupPresenceEntry>& peer,const std::vector<PickupPresenceEntry>& previous,bool authoredOnly=false){
    if(pickupCharacterNamed(item.name)||(authoredOnly&&pickupRuntimeNamed(item.name,item.handle)))return false;
    return !pickupInPresence(item,peer)&&!pickupInPresence(item,previous);
}
