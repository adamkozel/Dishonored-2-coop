#pragma once
#include "native_usable.h"
#include "usable_catalog.h"
#include <cstring>
// 1214jn (#187): a player's use of a usable that no other sync covers reaches the other world by name. Covered: one-state
// usables (lift and elevator buttons, escape buttons, valves) and two-state usables outside the shared toggle family
// (levers with transition times or reversed prompts). Not covered: shared toggles (usable sync), safes and combination
// dials (per world, chapter 106), doors and pickups (their own sync).
#pragma pack(push,1)
struct UseMirrorPacket {
    std::uint32_t magic=0x4D553244; // "D2UM"
    std::uint16_t version=1,size=sizeof(UseMirrorPacket);
    std::uint32_t role=0;
    std::uint64_t session=0,epoch=0,mapId=0,sequence=0;
    char name[128]{};
};
#pragma pack(pop)
static_assert(sizeof(UseMirrorPacket)==172);
inline bool useMirrorEligibleName(const char* name){
    if(!name||!name[0])return false;
    static const char* const excluded[]={"safe","digit","combinaison"};
    for(const char* x:excluded)if(std::strstr(name,x))return false;
    for(const char* c=name;*c;++c)if(!((*c>='a'&&*c<='z')||(*c>='0'&&*c<='9')||*c=='_'))return false;
    return true;
}
inline bool useMirrorEligible(const NativeUsable& n){
    return n.enabled&&!n.name.empty()&&n.name.size()<127&&n.definitionName==n.name&&useMirrorEligibleName(n.name.c_str())&&
        (n.stateCount==1||(n.stateCount==2&&!nativeUsableToggleFamily(n)));
}
inline bool validUseMirror(const UseMirrorPacket& p,unsigned receiver){
    return p.magic==0x4D553244&&p.version==1&&p.size==sizeof(p)&&p.role<=1&&p.role!=receiver&&p.session&&p.epoch&&p.mapId&&p.sequence&&
        std::memchr(p.name,0,sizeof(p.name))&&useMirrorEligibleName(p.name);
}
