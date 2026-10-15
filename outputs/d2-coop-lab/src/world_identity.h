#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <cmath>
#include <utility>

// Retail 1.77.9.0. Native arkCpntSpawner::Create (75b250) stores its
// owner's handle at child+ac. The component retains its live children at
// +30/+38. Require both directions; generated NPC names and local handles
// are deliberately excluded from cross-instance identity.
struct NativeWorldNpc {
    std::uintptr_t actor=0,spawner=0,component=0;
    std::uint32_t handle=0,spawnerHandle=0,childCount=0;
    std::string spawnerName,definition;
    float position[3]{},orientation[9]{};
    bool reciprocal=false,unique=false;
};
template<class Read> bool worldIdentityString(Read read,std::uintptr_t pointer,std::string& text) {
    text.clear();
    if(!pointer)return false;
    if(pointer>UINTPTR_MAX-191)return false;
    for(unsigned i=0;i<192;) {
        // ReadProcessMemory on our own process still enters the kernel. The
        // nested item/scene readers used one call per character, thousands of
        // calls per sample. Batch bytes without caching mutable native data.
        unsigned char bytes[32]{};unsigned count=(192-i)<32?(192-i):32;
        if(!read(pointer+i,bytes,count)){
            // A valid terminator can precede an unreadable allocation/page.
            // Keep the original byte-wise semantics at that boundary.
            count=1;if(!read(pointer+i,bytes,1))return false;
        }
        for(unsigned j=0;j<count;++j){const auto c=bytes[j];
            if(!c)return !text.empty();
            if(c<32||c>126)return false;
            text+=static_cast<char>(c);
        }
        i+=count;
    }
    return false;
}
template<class Read> std::uintptr_t worldIdentityEntity(Read read,std::uintptr_t game,std::uint32_t handle) {
    const auto slot=static_cast<std::int32_t>(handle)>>16;
    std::uintptr_t entity=0;std::uint32_t actual=0;
    if(slot<0||slot>=16384||!read(game+0x3130b0+slot*8,&entity,8)||!entity||
       !read(entity+0xa8,&actual,4)||actual!=handle)return 0;
    return entity;
}
template<class Read> bool readWorldNpcIdentity(Read read,std::uintptr_t base,std::uintptr_t game,
                                             std::uint32_t handle,NativeWorldNpc& out) {
    NativeWorldNpc n{};n.handle=handle;n.actor=worldIdentityEntity(read,game,handle);
    std::uintptr_t vt=0,name=0,definition=0,list=0;std::uint32_t flags=0;
    if(!n.actor||!read(n.actor,&vt,8)||vt!=base+0x1c5e108||
       !read(n.actor+0xac,&n.spawnerHandle,4))return false;
    n.spawner=worldIdentityEntity(read,game,n.spawnerHandle);
    if(!n.spawner||!read(n.spawner,&vt,8)||vt!=base+0x1c5e948||
       !read(n.spawner+0x20,&name,8)||!worldIdentityString(read,name,n.spawnerName)||
       !read(n.actor+0x2e8,&definition,8)||!definition||!read(definition,&vt,8)||vt!=base+0x1ab58f8||
       !read(definition+8,&name,8)||!worldIdentityString(read,name,n.definition)||
       !read(n.spawner+0x268,&list,8)||!list||!read(n.spawner+0x270,&flags,4)||(flags&0xffffff)>256)return false;
    for(unsigned i=0;i<(flags&0xffffff);++i) {
        std::uintptr_t component=0,owner=0;
        if(!read(list+i*8,&component,8)||!component||!read(component,&vt,8)||vt!=base+0x1c57d78)continue;
        if(n.component||!read(component+0x10,&owner,8)||owner!=n.spawner)return false;
        n.component=component;
    }
    if(!n.component)return false;
    std::uintptr_t children=0;std::uint32_t childFlags=0;
    if(!read(n.component+0x30,&children,8)||!read(n.component+0x38,&childFlags,4)||(childFlags&0xffffff)>256)return false;
    n.childCount=childFlags&0xffffff;
    unsigned matches=0;
    for(unsigned i=0;i<n.childCount;++i) {
        std::uint32_t child=0;if(!children||!read(children+i*4,&child,4))return false;
        if(child==handle)++matches;
    }
    n.reciprocal=matches==1;
    // Until spawn generations/ordinals are replicated, accept only a sole
    // child. Ambiguous multi-child spawners cannot select a mutation target.
    n.unique=n.reciprocal&&n.childCount==1;
    if(!read(n.actor+0x300,n.position,sizeof(n.position))||!read(n.actor+0x30c,n.orientation,sizeof(n.orientation)))return false;
    for(float f:n.position)if(!std::isfinite(f))return false;
    for(float f:n.orientation)if(!std::isfinite(f))return false;
    std::uintptr_t childrenAgain=0;std::uint32_t flagsAgain=0,parentAgain=0;
    if(worldIdentityEntity(read,game,handle)!=n.actor||worldIdentityEntity(read,game,n.spawnerHandle)!=n.spawner||
       !read(n.actor+0xac,&parentAgain,4)||parentAgain!=n.spawnerHandle||
       !read(n.component+0x30,&childrenAgain,8)||childrenAgain!=children||
       !read(n.component+0x38,&flagsAgain,4)||flagsAgain!=childFlags)return false;
    if(n.unique){std::uint32_t child=0;if(!read(children,&child,4)||child!=handle)return false;}
    out=std::move(n);return true;
}
template<class Read> bool readWorldNpcCatalog(Read read,std::uintptr_t base,std::uintptr_t game,
                                            std::vector<NativeWorldNpc>& catalog,unsigned& unbound) {
    catalog.clear();unbound=0;
    std::uintptr_t vt=0,handles=0;std::uint32_t flags=0;
    if(!game||!read(game,&vt,8)||vt!=base+0x1c940a8||!read(game+0x601c0,&handles,8)||!handles||
       !read(game+0x601c8,&flags,4)||(flags&0xffffff)>16384)return false;
    const unsigned count=flags&0xffffff;
    std::vector<std::uint32_t> live(count);
    if(count&&!read(handles,live.data(),count*4))return false;
    // Include unused spawners in the duplicate-name check. Collect once so
    // validation stays linear in engine-memory reads, not NPCs x entities.
    // 1213n: one pass - each live entity's vtable is read once for both NPC
    // actors and spawner names (two passes read every entity twice; a mission
    // catalog took ~100 ms of engine-thread reads, take 1235e).
    std::vector<std::string> names;
    for(auto handle:live) {
        const auto entity=worldIdentityEntity(read,game,handle);
        if(!entity||!read(entity,&vt,8))continue;
        if(vt==base+0x1c5e108){
            NativeWorldNpc n{};
            if(readWorldNpcIdentity(read,base,game,handle,n))catalog.push_back(std::move(n));else ++unbound;
        }else if(vt==base+0x1c5e948){
            std::uintptr_t name=0;if(!read(entity+0x20,&name,8))continue;
            std::string text;if(!worldIdentityString(read,name,text))return false;
            names.push_back(std::move(text));
        }
    }
    for(auto& n:catalog) {
        unsigned matches=0;for(const auto& name:names)if(name==n.spawnerName)++matches;
        n.unique=n.unique&&matches==1;
    }
    std::uintptr_t handlesAgain=0;std::uint32_t flagsAgain=0;
    if(!read(game+0x601c0,&handlesAgain,8)||handlesAgain!=handles||!read(game+0x601c8,&flagsAgain,4)||flagsAgain!=flags)return false;
    return true;
}
