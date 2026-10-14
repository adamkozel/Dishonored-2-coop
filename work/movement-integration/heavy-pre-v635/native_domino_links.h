#pragma once
#include "native_domino_targets.h"
// Native v214 observation + 1404dd500 / 1404e2de0 decompilation. Read-only.
// Connection objects are gameplay objects: never call their update/destructors.
constexpr const char* kDominoLinkFx="models/effects/gameplay/powers/phantasmal_link/links/phlink_links_01.fx";
struct NativeDominoConnection {
    std::uintptr_t pointer=0,fromNode=0,toNode=0;
    unsigned state=0,from=0,to=0,fxId=~0u,owner=0x3fff0010;
    float progress=0;bool hasBeam=false;
    NativeWindblastFxRecord beam{};
    std::array<float,12> start{},end{};
};
struct NativeDominoLinks {
    NativeDominoTargets network{};
    std::uintptr_t list=0,pool=0,declaration=0;
    unsigned count=0,capacity=0;
    std::vector<NativeDominoConnection> connections;
};
template<class Read> bool readNativeDominoLinksFrame(Read read,std::uintptr_t base,NativeDominoLinks& out,unsigned* stage,const NativeDominoItem* known=nullptr){
    auto& n=out.network;
    if(!readNativeDominoTargetsFrame(read,base,n,stage,known))return false;
    if(!n.present)return true;
    const auto at=[&](unsigned s){if(stage)*stage=s;};at(11);
    const auto c=n.component;std::uintptr_t vt=0,name=0;std::string text;
    if(!read(c+0x15b0,&out.list,8)||!read(c+0x15b8,&out.count,4)||(out.count&0xffffff)>12||((out.count&0xffffff)&&!out.list)||
       !read(c+0x15c0,&out.pool,8)||!out.pool||!read(c+0x15c8,&out.capacity,4)||(out.capacity&0xffffff)!=32||
       !read(n.declaration+0xb0,&out.declaration,8)||!out.declaration||!read(out.declaration,&vt,8)||vt!=base+0x1ab3138||
       !read(out.declaration+8,&name,8)||!worldIdentityString(read,name,text)||text!=kDominoLinkFx)return false;
    std::unordered_set<std::uintptr_t> objects;std::unordered_set<std::uint64_t> pairs;std::unordered_set<unsigned> effects;
    for(unsigned i=0;i<(out.count&0xffffff);++i){NativeDominoConnection x{};at(12);
        if(!read(out.list+i*8,&x.pointer,8)||x.pointer<out.pool||x.pointer-out.pool>=32*0x80||(x.pointer-out.pool)%0x80||!objects.insert(x.pointer).second||
           !read(x.pointer,&x.state,4)||x.state>1||!read(x.pointer+8,&x.fromNode,8)||!read(x.pointer+0x10,&x.toNode,8)||x.fromNode==x.toNode||
           !read(x.pointer+0x28,&x.progress,4)||!std::isfinite(x.progress)||x.progress<0||x.progress>1||
           !read(x.pointer+0x2c,&x.fxId,4)||!read(x.pointer+0x30,&x.owner,4))return false;
        const NativeDominoTarget* from=nullptr;const NativeDominoTarget* to=nullptr;at(13);
        for(const auto& t:n.targets){if(t.key.node==x.fromNode)from=&t;if(t.key.node==x.toNode)to=&t;}
        if(!from||!to)return false;x.from=from->key.handle;x.to=to->key.handle;
        if(!pairs.insert((std::uint64_t(x.from)<<32)|x.to).second)return false;
        if(x.fxId!=~0u){at(14);
            if(!x.fxId||x.owner!=1||!effects.insert(x.fxId).second||
               !readNativeFxRecordById(read,base,n.equipment.player.sword.game,x.fxId,x.beam)||x.beam.declaration!=out.declaration||x.beam.name!=kDominoLinkFx||
               x.beam.owner!=x.owner||x.beam.ownerActor!=n.equipment.player.sword.actor)return false;
            std::uintptr_t callback=1;at(15);
            std::memcpy(x.start.data(),x.beam.origin,12);std::memcpy(x.start.data()+3,x.beam.axis,36);
            if(!read(x.beam.record+0x80,x.end.data(),48)||!read(x.beam.record+0x128,&callback,8)||callback)return false;
            std::array<float,3> end{};for(unsigned axis=0;axis<3;++axis)end[axis]=from->transform[axis]+x.progress*(to->transform[axis]-from->transform[axis]);
            if(!dominoTargetTransform(x.start.data(),from->transform.data(),1.f)||!dominoTargetTransform(x.end.data(),end.data(),1.f))return false;
            x.hasBeam=true;
        }else if(x.owner!=0x3fff0010)return false;
        out.connections.push_back(std::move(x));
    }
    return true;
}
template<class Read> bool readNativeDominoLinks(Read read,std::uintptr_t base,NativeDominoLinks& out,unsigned* stage=nullptr,const NativeDominoItem* known=nullptr){
    NativeDominoLinks a{},b{};
    if(!readNativeDominoLinksFrame(read,base,a,stage,known)||!readNativeDominoLinksFrame(read,base,b,stage,known))return false;
    if(stage)*stage=16;
    if(!sameNativeDominoTargets(a.network,b.network)||a.list!=b.list||a.pool!=b.pool||a.count!=b.count||a.capacity!=b.capacity||a.declaration!=b.declaration||a.connections.size()!=b.connections.size())return false;
    for(unsigned i=0;i<a.connections.size();++i){const auto& x=a.connections[i];const auto& y=b.connections[i];
        if(x.pointer!=y.pointer||x.fromNode!=y.fromNode||x.toNode!=y.toNode||x.state!=y.state||x.from!=y.from||x.to!=y.to||x.fxId!=y.fxId||x.owner!=y.owner||x.hasBeam!=y.hasBeam||
           x.beam.record!=y.beam.record||x.beam.declaration!=y.beam.declaration||x.beam.ownerActor!=y.beam.ownerActor)return false;
    }
    out=std::move(b);if(stage)*stage=0;return true;
}
