#pragma once
#include "world_identity.h"
#include <array>
#include <algorithm>
// Exact retail ArkDoor/Openable identity. Local handles and pointers never
// identify the corresponding door in another process; authored names do.
struct NativeOpenable {
    std::uintptr_t game=0,actor=0,component=0,definition=0,animation=0,child=0;
    unsigned handle=0,generation=0,childHandle=0,animationGeneration=0,current=0,pending=0;
    std::string name,definitionName;
    unsigned char locked=0,blocked=0,broken=0,openSide=0;
    std::array<unsigned char,8> entries{};
    bool linked=false;
};
template<class Read> bool readNativeOpenable(Read read,std::uintptr_t base,std::uintptr_t game,unsigned handle,NativeOpenable& out){
    NativeOpenable s{};s.game=game;s.handle=handle;
    std::uintptr_t v=0,name=0,list=0,link=0;unsigned flags=0,componentFlags=0,childGeneration=0;
    const auto eq=[&](std::uintptr_t at,std::uintptr_t expected){return read(at,&v,8)&&v==expected;};
    if(!game||!eq(game,base+0x1c940a8)||!(s.actor=worldIdentityEntity(read,game,handle))||
       !eq(s.actor,base+0x1cb7c68)||!read(s.actor+0x20,&name,8)||!worldIdentityString(read,name,s.name)||
       !read(s.actor+0x2e8,&s.definition,8)||!s.definition||!eq(s.definition,base+0x1ab58f8)||
       !read(s.definition+8,&name,8)||!worldIdentityString(read,name,s.definitionName)||
       !read(s.actor+0x370,&s.component,8)||!s.component||!eq(s.component,base+0x1c569c8)||!eq(s.component+0x10,s.actor)||
       !read(s.component+0x18,&s.generation,4)||s.generation==~0u||!read(s.component+0x1c,&componentFlags,4)||(componentFlags&12)!=8||
       !read(s.actor+0x268,&list,8)||!list||!read(s.actor+0x270,&flags,4)||!(flags&0xffffff)||(flags&0xffffff)>256)return false;
    std::array<std::uintptr_t,256> members{};
    if(!read(list,members.data(),(flags&0xffffff)*8))return false;unsigned matches=0;
    for(unsigned i=0;i<(flags&0xffffff);++i)if(members[i]==s.component)++matches;
    if(matches!=1||!eq(s.component+0x40,base+0x1b5e208)||!eq(s.component+0x60,base+0x1c56908)||
       !eq(s.component+0x80,base+0x1c56948)||!eq(s.component+0xb8,base+0x1c56988)||
       !eq(s.component+0x68,s.component+0x40)||!eq(s.component+0x88,s.component+0x40)||!eq(s.component+0xc0,s.component+0x40)||
       !eq(s.component+0x78,s.component)||!eq(s.component+0x98,s.component)||!eq(s.component+0xd0,s.component)||
       !read(s.component+0x48,&s.current,4)||s.current>2||!read(s.component+0x4c,&s.pending,4)||s.pending>2||
       !read(s.component+0xdc,&s.childHandle,4)||!(s.child=worldIdentityEntity(read,game,s.childHandle))||
       !read(s.component+0xf8,&s.animation,8)||!s.animation||!eq(s.animation,base+0x1b7f408)||!eq(s.animation+0x10,s.child)||
       !read(s.animation+0x18,&childGeneration,4)||childGeneration==~0u||
       !read(s.component+0xf0,&link,8)||!read(s.component+0x118,&s.blocked,1)||
       !read(s.component+0x1e8,&s.locked,1)||!read(s.component+0x1ed,&s.broken,1)||
       !read(s.component+0xc8,&s.openSide,1)||(s.current==2&&s.openSide>1)||
       !read(s.component+0x1e0,s.entries.data(),s.entries.size()))return false;
    // This byte belongs to the Open state and can be uninitialized while
    // Closed (observed 0xf0 in a fresh native door). Native closing reads it
    // only from active Open; Moving has its own direction at component+0x92.
    // Canonicalize the snapshot only. Never write the inactive state's data.
    if(s.current!=2)s.openSide=0;
    s.linked=link!=0;unsigned again=0;
    if(!eq(s.actor+0x370,s.component)||!eq(s.component+0x10,s.actor)||!read(s.component+0x18,&again,4)||again!=s.generation||
       !read(s.component+0x1c,&again,4)||again!=componentFlags||!eq(s.component+0xf8,s.animation)||
       !read(s.animation+0x18,&again,4)||again!=childGeneration||!eq(s.animation+0x10,s.child)||
       worldIdentityEntity(read,game,handle)!=s.actor||worldIdentityEntity(read,game,s.childHandle)!=s.child)return false;
    s.animationGeneration=childGeneration;out=std::move(s);return true;
}
inline bool sameNativeOpenableIdentity(const NativeOpenable& a,const NativeOpenable& b){
    return a.game==b.game&&a.handle==b.handle&&a.actor==b.actor&&a.component==b.component&&a.generation==b.generation&&
        a.definition==b.definition&&a.childHandle==b.childHandle&&a.child==b.child&&a.animation==b.animation&&
        a.animationGeneration==b.animationGeneration&&a.name==b.name&&a.definitionName==b.definitionName;
}
inline unsigned nativeOpenableEntry(unsigned target,unsigned side){return target==2?side:3-side;}
inline bool nativeOpenableMatchesTarget(const NativeOpenable& s,unsigned target,unsigned side){
    return (target==0||target==2)&&side<=1&&s.current==s.pending&&s.current==target&&
        (target==0||s.openSide==side);
}
inline bool nativeOpenableNeedsSideCorrection(const NativeOpenable& s,unsigned target,unsigned side){
    return target==2&&side<=1&&s.current==2&&s.pending==2&&s.openSide!=side;
}
inline bool nativeOpenableTransitionAllowed(const NativeOpenable& s,unsigned target,unsigned side){
    return (target==0||target==2)&&side<=1&&(s.current==0||s.current==2)&&s.pending==s.current&&
        !s.locked&&!s.blocked&&!s.broken&&!s.linked&&s.entries[nativeOpenableEntry(target,side)]!=255;
}
inline std::array<unsigned char,4> nativeOpenableParameters(const NativeOpenable& s,unsigned target,unsigned side){
    // Match ordinary interaction74b6e0 and native auto-close74ac50. The
    // recorded opening side stays constant; closing uses its opposite clip.
    return {static_cast<unsigned char>(target==2),0,static_cast<unsigned char>(side),s.entries[nativeOpenableEntry(target,side)]};
}
struct NativeOpenableCatalog {std::vector<NativeOpenable> entries;unsigned ambiguous=0,ineligible=0;};
inline NativeOpenableCatalog selectNativeOpenableCatalog(std::vector<NativeOpenable> all){
    NativeOpenableCatalog result;std::sort(all.begin(),all.end(),[](const auto& a,const auto& b){return a.name<b.name;});
    for(std::size_t first=0;first<all.size();){
        std::size_t end=first+1;while(end<all.size()&&all[end].name==all[first].name)++end;
        if(end-first!=1)result.ambiguous+=static_cast<unsigned>(end-first);
        else{
            const auto& n=all[first];bool eligible=!n.name.empty()&&!n.definitionName.empty();
            for(unsigned target:{0u,2u})for(unsigned side:{0u,1u})eligible=eligible&&nativeOpenableTransitionAllowed(n,target,side);
            if(eligible)result.entries.push_back(n);else ++result.ineligible;
        }
        first=end;
    }
    return result;
}
