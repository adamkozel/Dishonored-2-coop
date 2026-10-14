#pragma once
#include "world_identity.h"
#include <array>
struct CoopDamageAvatar {std::uintptr_t game=0,actor=0;std::uint32_t handle=0;};
enum class CoopFriendlyHit {None,LocalToPartner,PartnerToLocal};
template<class Read> bool validCoopDamageAvatar(Read read,std::uintptr_t base,const CoopDamageAvatar& a){
    std::uintptr_t game=0,vt=0,decl=0,name=0;std::string path;
    return a.game&&a.actor&&a.handle&&a.handle!=1&&read(base+0x39a5290,&game,8)&&game==a.game&&
        read(game,&vt,8)&&vt==base+0x1c940a8&&worldIdentityEntity(read,game,a.handle)==a.actor&&
        read(a.actor,&vt,8)&&vt==base+0x1c5e108&&read(a.actor+0x2e8,&decl,8)&&decl&&
        read(decl,&vt,8)&&vt==base+0x1ab58f8&&read(decl+8,&name,8)&&worldIdentityString(read,name,path)&&
        (path=="models/characters/def/d2coop/doppelganger_emily.def"||path=="models/characters/def/d2coop/doppelganger_corvo.def");
}
// Resolve generation-bearing handles at each edge. Native damage's attacker
// is preferred evidence; the inflictor/parent chain also covers attached items.
// Unattributed indirect hazards are not assumed to belong to either player.
template<class Read> bool coopDamageSourceOwned(Read read,std::uintptr_t game,std::uint32_t handle,
    std::uint32_t ownerHandle,std::uintptr_t owner){
    std::array<std::uint32_t,8> visited{};
    for(unsigned depth=0;depth<visited.size()&&handle;++depth){
        for(unsigned n=0;n<depth;++n)if(visited[n]==handle)return false;
        visited[depth]=handle;const auto entity=worldIdentityEntity(read,game,handle);
        if(!entity)return false;
        if(handle==ownerHandle)return entity==owner;
        std::uint32_t parent=0;
        if(!read(entity+0xac,&parent,4)||worldIdentityEntity(read,game,handle)!=entity)return false;
        handle=parent;
    }
    return false;
}
template<class Read> CoopFriendlyHit classifyCoopFriendlyHit(Read read,std::uintptr_t base,const CoopDamageAvatar& a,
    std::uintptr_t component,std::uint32_t inflictor,std::uint32_t attacker){
    if(!component||!validCoopDamageAvatar(read,base,a))return CoopFriendlyHit::None;
    const auto player=worldIdentityEntity(read,a.game,1);std::uintptr_t vt=0,victim=0;
    if(!player||!read(player,&vt,8)||vt!=base+0x1c5e428||!read(component,&vt,8)||vt!=base+0x1c51588||
       !read(component+0x10,&victim,8)||(victim!=player&&victim!=a.actor))return CoopFriendlyHit::None;
    const auto sourceHandle=victim==a.actor?1:a.handle;
    const auto sourceActor=victim==a.actor?player:a.actor;
    if(!coopDamageSourceOwned(read,a.game,attacker,sourceHandle,sourceActor)&&
       !coopDamageSourceOwned(read,a.game,inflictor,sourceHandle,sourceActor))return CoopFriendlyHit::None;
    std::uintptr_t ownerAgain=0;
    if(!validCoopDamageAvatar(read,base,a)||worldIdentityEntity(read,a.game,1)!=player||
       !read(component+0x10,&ownerAgain,8)||ownerAgain!=victim)return CoopFriendlyHit::None;
    return victim==a.actor?CoopFriendlyHit::LocalToPartner:CoopFriendlyHit::PartnerToLocal;
}
