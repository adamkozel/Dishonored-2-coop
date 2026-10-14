#pragma once
#include "native_usable.h"

// 1123: structural two-state toggle family (map-agnostic). Census (Dunwall,
// Edge of the World): windows, cupboards, cabinet/room doors, sinks, bathtubs
// share instant two-state entries {enter 255, exit i}. Custom prompts must be
// uniform (Open/Close). Vaults (combination safes) and flagged quest devices
// (flags bit 0, e.g. audiographs) keep their own semantics.
inline bool nativeUsableToggleFamily(const NativeUsable& n){
    if(n.name.empty()||n.definitionName!=n.name||n.name.size()>=192||
       n.declarationName.rfind("map_cpntusablewithstatelist_decl_",0)!=0||
       n.stateCount!=2||n.initial!=0||!n.enabled||(n.flags&1)||n.name.find("safe")!=std::string::npos)return false;
    for(unsigned char c:n.name)if(!((c>='a'&&c<='z')||(c>='0'&&c<='9')||c=='_'))return false;
    // 1123b: reversed toggles (entry i exits to 1-i, e.g. sinks/taps) share the
    // same indexed semantics; the mapping must be uniform and is in the rules hash.
    const unsigned flip=n.entries[0].exit==1?1u:0u;
    for(unsigned i=0;i<2;++i){const auto& e=n.entries[i];
        if(e.enter!=255||e.exit!=(i^flip)||e.applyOnFinish||e.customPrompt!=n.entries[0].customPrompt||e.customPrompt>1||e.transitionSeconds!=0)return false;
    }
    return true;
}
// Canonical semantic fields only: no native pointer, padding, or mutable state.
inline std::uint64_t nativeUsableRulesHash(const NativeUsable& n){
    std::uint64_t h=14695981039346656037ull;
    const auto byte=[&](unsigned char b){h=(h^b)*1099511628211ull;};
    const auto word=[&](unsigned v){for(unsigned i=0;i<4;++i)byte(static_cast<unsigned char>(v>>(8*i)));};
    for(unsigned char c:n.declarationName)byte(c);byte(0);word(n.stateCount);word(static_cast<unsigned>(n.initial));
    for(unsigned i=0;i<n.stateCount&&i<n.entries.size();++i){const auto& e=n.entries[i];
        byte(e.enter);byte(e.exit);byte(e.applyOnFinish);byte(e.customPrompt);
        unsigned bits=0;std::memcpy(&bits,&e.transitionSeconds,4);word(bits);
    }
    return h;
}
struct NativeUsableCatalog {std::vector<NativeUsable> entries;unsigned ambiguous=0,ineligible=0;};
inline NativeUsableCatalog selectNativeUsableCatalog(std::vector<NativeUsable> all){
    NativeUsableCatalog result;std::sort(all.begin(),all.end(),[](const auto& a,const auto& b){return a.name<b.name;});
    for(std::size_t first=0;first<all.size();){
        std::size_t end=first+1;while(end<all.size()&&all[end].name==all[first].name)++end;
        if(end-first!=1)result.ambiguous+=static_cast<unsigned>(end-first);
        else if(nativeUsableToggleFamily(all[first])&&nativeUsableAtTarget(all[first],all[first].current))result.entries.push_back(all[first]);
        else ++result.ineligible;
        first=end;
    }
    return result;
}
