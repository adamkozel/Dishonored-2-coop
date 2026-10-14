#pragma once
#include "native_windblast_fx_records.h"
// Targeted read-only equivalent of native FX ID lookup 1401a04a0.
// Unlike a research inventory scan, this reads only the requested hash bucket
// and the one exact effect. No native API calls and no manager writes.
template<class Read> bool readNativeFxRecordById(Read read,std::uintptr_t base,std::uintptr_t game,unsigned id,NativeWindblastFxRecord& out){
    if(!id||id==~0u)return false;
    std::uintptr_t active=0,vt=0,pool=0,buckets=0,head=0;unsigned count=0,mask=0;
    const auto manager=game+0x1f8430,map=manager+0xeef60;
    if(!game||!read(base+0x39a5290,&active,8)||active!=game||!read(game,&vt,8)||vt!=base+0x1c940a8||
       !read(manager,&pool,8)||!pool||!read(map,&buckets,8)||!buckets||!read(map+0xc,&count,4)||!count||count>1024||
       !read(map+0x10,&mask,4)||mask>65535||((mask+1)&mask)||!read(buckets+(id&mask)*8,&head,8))return false;
    struct Node {unsigned id;std::uint16_t slot,pad;std::uintptr_t next;};static_assert(sizeof(Node)==16);
    std::vector<std::pair<std::uintptr_t,Node>> nodes;std::unordered_set<std::uintptr_t> seen;std::uintptr_t record=0;
    for(auto p=head;p;){Node n{};
        if(nodes.size()>=count||!seen.insert(p).second||!read(p,&n,sizeof(n))||n.slot>=1024||(n.id&mask)!=(id&mask))return false;
        nodes.push_back({p,n});if(n.id==id){if(record)return false;record=pool+std::uintptr_t(n.slot)*0x140;}p=n.next;
    }
    if(!record)return false;NativeWindblastFxRecord r{};r.record=record;std::array<std::uintptr_t,2> identity{},again{};std::uintptr_t name=0;
    if(!read(record,identity.data(),16))return false;r.owner=unsigned(identity[0]);r.id=unsigned(identity[0]>>32);r.declaration=identity[1];
    if(r.id!=id||!r.declaration||!read(r.declaration,&vt,8)||vt!=base+0x1ab3138||!read(r.declaration+8,&name,8)||!worldIdentityString(read,name,r.name)||
       !read(record+0x38,r.origin,12)||!read(record+0x44,r.axis,36))return false;
    for(float f:r.origin)if(!std::isfinite(f)||std::fabs(f)>100000)return false;
    for(float f:r.axis)if(!std::isfinite(f)||std::fabs(f)>1.01f)return false;
    r.ownerActor=worldIdentityEntity(read,game,r.owner);
    if(r.ownerActor){std::uintptr_t def=0,finalDef=0;
        if(!read(r.ownerActor+0xac,&r.parent,4)||!read(r.ownerActor+0x2e8,&def,8))return false;
        if(def&&(!read(def,&vt,8)||vt!=base+0x1ab58f8||!read(def+8,&name,8)||!worldIdentityString(read,name,r.ownerDefinition)))return false;
        if(worldIdentityEntity(read,game,r.owner)!=r.ownerActor||!read(r.ownerActor+0x2e8,&finalDef,8)||def!=finalDef)return false;
    }
    for(const auto& entry:nodes){Node n{};if(!read(entry.first,&n,sizeof(n))||std::memcmp(&n,&entry.second,sizeof(n)))return false;}
    unsigned countAgain=0,maskAgain=0;std::uintptr_t ptr=0;
    if(!read(record,again.data(),16)||identity!=again||!read(buckets+(id&mask)*8,&ptr,8)||ptr!=head||
       !read(map+0xc,&countAgain,4)||countAgain!=count||!read(map+0x10,&maskAgain,4)||maskAgain!=mask||
       !read(map,&ptr,8)||ptr!=buckets||!read(manager,&ptr,8)||ptr!=pool||!read(base+0x39a5290,&ptr,8)||ptr!=game)return false;
    out=std::move(r);return true;
}
