#pragma once
#include "world_identity.h"
#include <algorithm>
#include <unordered_set>

// Retail 1.77.9.0, read-only reconstruction of native 604c60. Relationships
// are directional. The manager's entity override takes precedence unless it
// is UNDEFINED (3); otherwise the current faction relationship table wins.
struct NativeFactionRelation {
    std::uint32_t handle=0,index=0,base=3,effective=3,overrideValue=3;
    bool overridden=false;
    bool operator==(const NativeFactionRelation& b)const{return handle==b.handle&&index==b.index&&base==b.base&&effective==b.effective&&overrideValue==b.overrideValue&&overridden==b.overridden;}
};
struct NativeFaction {
    std::uintptr_t game=0,actor=0,component=0,declaration=0,list=0,stack=0,table=0,names=0,buckets=0,node=0,overrides=0,members=0;
    std::uint32_t handle=0,logic=0,logicCount=0,componentCount=0,flags=0,index=0,factionCount=0,bucketCount=0,nodeCount=0,mask=0,overrideCount=0,memberCount=0;
    std::string name,definition;
    std::vector<NativeFactionRelation> relations;
    bool operator==(const NativeFaction& b)const {
        return game==b.game&&actor==b.actor&&component==b.component&&declaration==b.declaration&&list==b.list&&stack==b.stack&&table==b.table&&names==b.names&&
            buckets==b.buckets&&node==b.node&&overrides==b.overrides&&members==b.members&&handle==b.handle&&logic==b.logic&&logicCount==b.logicCount&&componentCount==b.componentCount&&
            flags==b.flags&&index==b.index&&factionCount==b.factionCount&&bucketCount==b.bucketCount&&nodeCount==b.nodeCount&&mask==b.mask&&overrideCount==b.overrideCount&&memberCount==b.memberCount&&
            name==b.name&&definition==b.definition&&relations==b.relations;
    }
};
template<class Read> bool readNativeFactionFrame(Read read,std::uintptr_t base,std::uint32_t handle,NativeFaction& out){
    NativeFaction s{};s.handle=handle;std::uintptr_t vt=0,owner=0,text=0;unsigned phase=0,pending=0;
    if(!read(base+0x39a5290,&s.game,8)||!s.game||!read(s.game,&vt,8)||vt!=base+0x1c940a8||
       !read(s.game+0x341058,&phase,4)||phase!=3||!read(s.game+0x1f6ed0,&owner,8)||owner!=s.game||
       !read(s.game+0x1f73f0,&s.stack,8)||!s.stack||!read(s.game+0x1f73f8,&s.logicCount,4)||
       !(s.logicCount&0xffffff)||(s.logicCount&0xffffff)>32||!read(s.stack+((s.logicCount&0xffffff)-1)*4,&s.logic,4)||
       (s.logic!=1&&s.logic!=2&&s.logic!=13)||!read(s.game+0x1f7400,&pending,4)||pending)return false;
    s.actor=worldIdentityEntity(read,s.game,handle);
    if(!s.actor||!read(s.actor,&vt,8)||vt!=base+(handle==1?0x1c5e428:0x1c5e108)||
       !read(s.actor+0x268,&s.list,8)||!s.list||!read(s.actor+0x270,&s.componentCount,4)||
       !(s.componentCount&0xffffff)||(s.componentCount&0xffffff)>256)return false;
    for(unsigned i=0;i<(s.componentCount&0xffffff);++i){
        std::uintptr_t c=0;if(!read(s.list+i*8,&c,8)||!c||!read(c,&vt,8))return false;
        if(vt==base+0x1ce6268){if(s.component)return false;s.component=c;}
    }
    unsigned char index=255;
    if(!s.component||!read(s.component+0x10,&owner,8)||owner!=s.actor||!read(s.component+8,&s.declaration,8)||!s.declaration||
       !read(s.declaration,&vt,8)||vt!=base+0x1d1f7e8||!read(s.component+0x1c,&s.flags,4)||(s.flags&0xc)!=8||
       !read(s.component+0x20,&index,1)||index>=128||!read(s.component+0x28,&text,8)||!worldIdentityString(read,text,s.name)||
       !read(s.declaration+8,&text,8)||!worldIdentityString(read,text,s.definition))return false;
    s.index=index;const auto manager=s.game+0x1f7bb8;
    unsigned count=0;
    if(!read(manager,&s.table,8)||!s.table||!read(manager+8,&s.factionCount,4)||!(s.factionCount&0xffffff)||(s.factionCount&0xffffff)>128||
       !read(manager+0x10,&s.names,8)||!s.names||!read(manager+0x18,&count,4)||(count&0xffffff)!=(s.factionCount&0xffffff)||s.index>=(s.factionCount&0xffffff)||
       !read(manager+0x20,&s.buckets,8)||!s.buckets||!read(manager+0x28,&s.bucketCount,4)||!s.bucketCount||s.bucketCount>16384||
       !read(manager+0x2c,&s.nodeCount,4)||s.nodeCount>16384||!read(manager+0x30,&s.mask,4)||s.mask!=s.bucketCount-1||((s.mask+1)&s.mask))return false;
    const auto faction=s.table+s.index*0x38;
    std::uintptr_t declaration=0,name=0,nativeName=0,relationTable=0;
    if(!read(faction+0x30,&declaration,8)||declaration!=s.declaration||!read(s.names+s.index*8,&name,8)||
       !read(s.declaration+8,&nativeName,8)||name!=nativeName||!read(faction,&s.members,8)||!s.members||
       !read(faction+8,&s.memberCount,4)||(s.memberCount&0xffffff)>16384||
       !read(faction+0x10,&relationTable,8)||!relationTable||!read(faction+0x18,&count,4)||(count&0xffffff)!=(s.factionCount&0xffffff))return false;
    unsigned memberMatches=0;
    for(unsigned i=0;i<(s.memberCount&0xffffff);++i){std::uint32_t h=0;if(!read(s.members+i*4,&h,4))return false;if(h==handle)++memberMatches;}
    if(memberMatches!=1)return false;
    std::unordered_set<std::uintptr_t> nodes;std::unordered_set<std::uint32_t> handles;
    for(unsigned bucket=0;bucket<s.bucketCount;++bucket){
        std::uintptr_t node=0;if(!read(s.buckets+bucket*8,&node,8))return false;
        while(node){
            NativeFactionRelation r{};unsigned char idx=255;std::uintptr_t next=0;
            if(nodes.size()>=16384||!nodes.insert(node).second||!read(node,&r.handle,4)||(r.handle&s.mask)!=bucket||!handles.insert(r.handle).second||
               !worldIdentityEntity(read,s.game,r.handle)||!read(node+8,&idx,1)||idx>=(s.factionCount&0xffffff)||!read(node+0x20,&next,8))return false;
            r.index=idx;
            if(!read(relationTable+r.index*4,&r.base,4)||r.base>3)return false;r.effective=r.base;
            if(r.handle==handle){
                if(s.node||r.index!=s.index||!read(node+0x10,&s.overrides,8)||!read(node+0x18,&s.overrideCount,4)||(s.overrideCount&0xffffff)>16384)return false;
                s.node=node;
            }
            s.relations.push_back(r);node=next;
        }
    }
    if(!s.node||nodes.size()!=s.nodeCount)return false;
    std::unordered_set<std::uint32_t> overridden;
    for(unsigned i=0;i<(s.overrideCount&0xffffff);++i){
        std::uint32_t h=0,value=0;
        if(!s.overrides||!read(s.overrides+i*8,&h,4)||!read(s.overrides+i*8+4,&value,4)||value>3||!overridden.insert(h).second)return false;
        auto it=std::find_if(s.relations.begin(),s.relations.end(),[&](const auto& r){return r.handle==h;});
        if(it==s.relations.end())return false;
        it->overridden=true;it->overrideValue=value;if(value!=3)it->effective=value;
    }
    std::sort(s.relations.begin(),s.relations.end(),[](const auto& a,const auto& b){return a.handle<b.handle;});
    out=std::move(s);return true;
}
template<class Read> bool readNativeFaction(Read read,std::uintptr_t base,std::uint32_t handle,NativeFaction& out){
    NativeFaction a{},b{};
    if(!readNativeFactionFrame(read,base,handle,a)||!readNativeFactionFrame(read,base,handle,b)||!(a==b))return false;
    out=std::move(a);return true;
}
