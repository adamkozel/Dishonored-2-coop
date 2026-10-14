#pragma once
#include "carry_presentation.h"
#include "world_identity.h"
// Cached catalog supplies duplicate-name screening; every selected actor is
// re-read with reciprocal spawner ownership and its exact current generation.
// This lookup is read-only and does not grant corpse mutation authority.
template<class Read> bool resolveCarryIdentity(Read read,std::uintptr_t base,std::uintptr_t game,
    const std::vector<NativeWorldNpc>& catalog,std::uint64_t spawner,std::uint64_t definition,NativeWorldNpc& out){
    if(!spawner||!definition)return false;
    unsigned matches=0;NativeWorldNpc result{};
    for(const auto& cached:catalog){
        if(!cached.unique||carryIdentityKey(cached.spawnerName)!=spawner||carryIdentityKey(cached.definition)!=definition)continue;
        NativeWorldNpc current{};
        if(!readWorldNpcIdentity(read,base,game,cached.handle,current)||!current.unique||
            current.actor!=cached.actor||current.spawner!=cached.spawner||current.component!=cached.component||
            current.spawnerName!=cached.spawnerName||current.definition!=cached.definition)return false;
        if(++matches!=1)return false;result=std::move(current);
    }
    if(matches!=1)return false;out=std::move(result);return true;
}
