#pragma once
#include "native_coin.h"
#include "coin_limits.h"
#include "coin_protocol.h"
#include <algorithm>
#include <vector>
struct NativeCoinCatalog {std::vector<NativeCoin> entries;unsigned ambiguous=0,ineligible=0;std::uint64_t digest=0;};
inline bool coinCanonicalName(const std::string& name){
    if(name.empty()||name.size()>=192)return false;
    char text[192]{};std::memcpy(text,name.data(),name.size());return coopWorldName(text);
}
inline bool coinCatalogEligible(const NativeCoin& c){
    return coinCanonicalName(c.name)&&coinCanonicalName(c.itemName)&&coinQuantityValid(c.quantity)&&pickupFamilyValid(c.type)&&!c.flags&&!(c.ownerFlags&1);
}
inline NativeCoinCatalog selectNativeCoinCatalog(std::vector<NativeCoin> coins){
    NativeCoinCatalog result{};std::sort(coins.begin(),coins.end(),[](const auto& a,const auto& b){return a.name<b.name;});
    for(std::size_t i=0;i<coins.size();){
        auto end=i+1;while(end<coins.size()&&coins[end].name==coins[i].name)++end;
        if(end-i!=1)result.ambiguous+=static_cast<unsigned>(end-i);
        else if(!coinCatalogEligible(coins[i]))++result.ineligible;
        else result.entries.push_back(std::move(coins[i]));
        i=end;
    }
    if(result.entries.empty()||result.entries.size()>kCoinCatalogMax)return result;
    std::uint64_t hash=14695981039346656037ull;
    const auto byte=[&](unsigned char b){hash^=b;hash*=1099511628211ull;};
    const auto string=[&](const std::string& s){for(unsigned char c:s)byte(c);byte(0);};
    for(const auto& coin:result.entries){string(coin.name);string(coin.itemName);for(unsigned i=0;i<4;++i)byte(static_cast<unsigned char>(static_cast<unsigned>(coin.quantity)>>(i*8)));for(unsigned i=0;i<4;++i)byte(static_cast<unsigned char>(coin.type>>(i*8)));}
    result.digest=hash;return result;
}

// 1059c: per-entry native identity (authored name, item declaration, quantity,
// family). Character worlds differ (script pickups), so the shared catalog is
// the intersection of both peers' identity lists; both peers compute it from
// the same two lists, so the shared digest agrees by construction.
inline std::uint64_t coinIdentity(const NativeCoin& coin){
    std::uint64_t hash=14695981039346656037ull;
    const auto byte=[&](unsigned char b){hash^=b;hash*=1099511628211ull;};
    for(unsigned char c:coin.name)byte(c);byte(0);for(unsigned char c:coin.itemName)byte(c);byte(0);
    for(unsigned i=0;i<4;++i)byte(static_cast<unsigned char>(static_cast<unsigned>(coin.quantity)>>(i*8)));
    for(unsigned i=0;i<4;++i)byte(static_cast<unsigned char>(coin.type>>(i*8)));
    return hash?hash:1;
}
inline std::uint64_t coinSharedDigest(std::vector<std::uint64_t> ids){
    std::sort(ids.begin(),ids.end());std::uint64_t hash=1469598103934665603ull^ids.size();
    for(auto id:ids)for(unsigned i=0;i<8;++i){hash^=static_cast<unsigned char>(id>>(i*8));hash*=1099511628211ull;}
    return hash?hash:1;
}
