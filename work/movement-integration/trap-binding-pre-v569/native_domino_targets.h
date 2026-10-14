#pragma once
#include "native_player_domino_item.h"
#include "native_domino_equipment_owner.h"
#include "native_fx_record_by_id.h"
// Read-only selected Domino network, observed in v198. Never calls native
// 4bc950/4db360/4e2940: those routines create gameplay links or owned effects.
constexpr const char* kDominoValidatedFx="models/effects/gameplay/powers/phantasmal_link/target/phlink_validated_01.fx";
struct NativeDominoNodeKey {
    std::uintptr_t node=0,animation=0;unsigned handle=0,generation=0,mark=~0u,markOwner=0x3fff0010;
    bool operator==(const NativeDominoNodeKey& b)const{return node==b.node&&animation==b.animation&&handle==b.handle&&generation==b.generation&&mark==b.mark&&markOwner==b.markOwner;}
};
struct NativeDominoTarget {
    NativeDominoNodeKey key{};std::uintptr_t actor=0,definition=0;std::string name;
    std::array<float,12> transform{};NativeWindblastFxRecord mark{};bool hasMark=false;
};
struct NativeDominoTargets {
    NativeDominoItem equipment{};bool present=false;
    unsigned handle=0x3fff0010,flags=0,count=0,poolCount=0;
    std::uintptr_t actor=0,definition=0,component=0,declaration=0,list=0,pool=0,markDeclaration=0;
    std::vector<NativeDominoTarget> targets;
};
inline bool dominoTargetTransform(const float* p,const float* origin,float radius){
    float distance=0;for(unsigned i=0;i<12;++i)if(!std::isfinite(p[i])||std::fabs(p[i])>(i<3?100000.f:1.01f))return false;
    for(unsigned i=0;i<3;++i){const float d=p[i]-origin[i];distance+=d*d;}if(!std::isfinite(distance)||distance>radius*radius)return false;
    for(unsigned i=0;i<3;++i)for(unsigned j=i;j<3;++j){float d=0;for(unsigned k=0;k<3;++k)d+=p[3+i*3+k]*p[3+j*3+k];if(std::fabs(d-(i==j?1.f:0.f))>.02f)return false;}
    const auto m=p+3;const float det=m[0]*(m[4]*m[8]-m[7]*m[5])-m[3]*(m[1]*m[8]-m[7]*m[2])+m[6]*(m[1]*m[5]-m[4]*m[2]);return det>.98f&&det<1.02f;
}
template<class Read> bool readDominoNodeKey(Read read,std::uintptr_t node,NativeDominoNodeKey& k){
    k.node=node;return read(node,&k.handle,4)&&read(node+8,&k.generation,4)&&read(node+0x10,&k.animation,8)&&read(node+0x90,&k.mark,4)&&read(node+0x94,&k.markOwner,4);
}
template<class Read> bool readNativeDominoTargetsFrame(Read read,std::uintptr_t base,NativeDominoTargets& s,unsigned* stage,const NativeDominoItem* known=nullptr){
    const auto at=[&](unsigned n){if(stage)*stage=n;};at(1);
    if(!(known?readCurrentNativeDominoEquipment(read,base,*known,s.equipment):readNativeDominoItem(read,base,s.equipment)))return false;
    if(!s.equipment.instantiated)return true;
    at(2);if(!read(s.equipment.component+0xb0,&s.handle,4)||!read(s.equipment.component+0xb8,&s.component,8))return false;
    if(s.handle==0x3fff0010)return !s.component;
    const auto game=s.equipment.player.sword.game;s.actor=worldIdentityEntity(read,game,s.handle);
    const auto eq=[&](std::uintptr_t at,std::uintptr_t wanted){std::uintptr_t p=0;return read(at,&p,8)&&p==wanted;};
    const auto decl=[&](std::uintptr_t d,std::uintptr_t vt,const char* expected){std::uintptr_t p=0;std::string text;return d&&eq(d,vt)&&read(d+8,&p,8)&&worldIdentityString(read,p,text)&&text==expected;};
    const auto member=[&](std::uintptr_t a,std::uintptr_t c,unsigned limit){std::uintptr_t list=0;unsigned flags=0,found=0;
        if(!read(a+0x268,&list,8)||!list||!read(a+0x270,&flags,4)||!(flags&0xffffff)||(flags&0xffffff)>limit)return false;
        for(unsigned i=0;i<(flags&0xffffff);++i){std::uintptr_t p=0;if(!read(list+i*8,&p,8))return false;if(p==c)++found;}return found==1;
    };
    unsigned parent=0;at(3);
    if(!s.actor||s.actor==s.equipment.entity||!eq(s.actor,base+0x1c87d58)||!read(s.actor+0xac,&parent,4)||parent!=0x3fff0010||
       !read(s.actor+0x2e8,&s.definition,8)||!decl(s.definition,base+0x1ab58f8,"models/powers/phantasmallink/phantasmal_network.def")||
       !s.component||!eq(s.component,base+0x1b62b48)||!eq(s.component+0x10,s.actor)||!member(s.actor,s.component,32)||
       !read(s.component+8,&s.declaration,8)||!decl(s.declaration,base+0x1b62a08,"components/characters/player/base/powers/phantasmallink/phantasmal_network.cpntphantasmalnetwork")||
       !read(s.component+0x1c,&s.flags,4)||!(s.flags&2))return false;
    at(4);if(!read(s.component+0x28,&s.list,8)||!read(s.component+0x30,&s.count,4)||(s.count&0xffffff)>4||((s.count&0xffffff)&&!s.list)||
       !read(s.component+0x68,&s.pool,8)||!s.pool||!read(s.component+0x70,&s.poolCount,4)||(s.poolCount&0xffffff)!=16||
       !read(s.declaration+0x88,&s.markDeclaration,8)||!decl(s.markDeclaration,base+0x1ab3138,kDominoValidatedFx))return false;
    std::unordered_set<unsigned> actors,marks,owners;std::unordered_set<std::uintptr_t> nodes;
    for(unsigned i=0;i<(s.count&0xffffff);++i){NativeDominoTarget t{};std::uintptr_t node=0;at(5);
        if(!read(s.list+i*8,&node,8)||node<s.pool||node-s.pool>=16*0x150||(node-s.pool)%0x150||!nodes.insert(node).second||!readDominoNodeKey(read,node,t.key)||
           !t.key.handle||t.key.handle==1||t.key.handle==0x3fff0010||t.key.handle==~0u||!actors.insert(t.key.handle).second||t.key.generation==~0u||!t.key.animation)return false;
        t.actor=worldIdentityEntity(read,game,t.key.handle);unsigned generation=0,flags=0;std::uintptr_t name=0;at(6);
        // Initial supported target scope is native NPCs. No player/prop inference.
        if(!t.actor||!eq(t.actor,base+0x1c5e108)||!read(t.actor+0x2e8,&t.definition,8)||!t.definition||!eq(t.definition,base+0x1ab58f8)||
           !read(t.definition+8,&name,8)||!worldIdentityString(read,name,t.name)||
           !eq(t.key.animation,base+0x1b80408)||!eq(t.key.animation+0x10,t.actor)||!member(t.actor,t.key.animation,256)||
           !read(t.key.animation+0x18,&generation,4)||generation!=t.key.generation||!read(t.key.animation+0x1c,&flags,4)||!(flags&2))return false;
        std::array<float,3> root{};at(7);
        if(!read(t.actor+0x300,root.data(),12)||!read(node+0x38,t.transform.data(),48)||!dominoTargetTransform(t.transform.data(),root.data(),4.f))return false;
        if(t.key.mark!=~0u){at(8);
            if(!t.key.mark||!marks.insert(t.key.mark).second||!owners.insert(t.key.markOwner).second||
               !readNativeFxRecordById(read,base,game,t.key.mark,t.mark)||t.mark.declaration!=s.markDeclaration||t.mark.name!=kDominoValidatedFx||
               t.mark.owner!=t.key.markOwner||!t.mark.ownerActor||!eq(t.mark.ownerActor,base+0x1c887f8)||!eq(t.mark.ownerActor+0x2e8,0)||t.mark.parent!=0x3fff0010)return false;
            std::array<float,12> pose{};std::memcpy(pose.data(),t.mark.origin,12);std::memcpy(pose.data()+3,t.mark.axis,36);at(9);
            if(!dominoTargetTransform(pose.data(),t.transform.data(),1.f))return false;
            t.hasMark=true;
        }else if(t.key.markOwner!=0x3fff0010)return false;
        s.targets.push_back(std::move(t));
    }
    s.present=true;return true;
}
inline bool sameNativeDominoTargets(const NativeDominoTargets& a,const NativeDominoTargets& b){
    if(!sameSwordEquipmentIdentity(a.equipment.player.sword,b.equipment.player.sword)||a.equipment.component!=b.equipment.component||a.equipment.entity!=b.equipment.entity||
       a.equipment.entry!=b.equipment.entry||a.equipment.equipmentDeclaration!=b.equipment.equipmentDeclaration||a.equipment.parentDeclaration!=b.equipment.parentDeclaration||
       a.equipment.flags!=b.equipment.flags||a.equipment.quantity!=b.equipment.quantity||a.equipment.equipped!=b.equipment.equipped||a.present!=b.present||a.handle!=b.handle||a.actor!=b.actor||a.definition!=b.definition||
       a.component!=b.component||a.declaration!=b.declaration||a.flags!=b.flags||a.list!=b.list||a.count!=b.count||a.pool!=b.pool||a.poolCount!=b.poolCount||
       a.markDeclaration!=b.markDeclaration||a.targets.size()!=b.targets.size())return false;
    for(unsigned i=0;i<a.targets.size();++i){const auto& x=a.targets[i];const auto& y=b.targets[i];
        // World transforms may advance naturally; identity may not. Each frame
        // separately bounds the mark against its own native target transform.
        if(!(x.key==y.key)||x.actor!=y.actor||x.definition!=y.definition||x.name!=y.name||x.hasMark!=y.hasMark||
           x.mark.record!=y.mark.record||x.mark.declaration!=y.mark.declaration||x.mark.ownerActor!=y.mark.ownerActor)return false;
    }
    return true;
}
template<class Read> bool readNativeDominoTargets(Read read,std::uintptr_t base,NativeDominoTargets& out,unsigned* stage=nullptr,const NativeDominoItem* known=nullptr){
    NativeDominoTargets a{},b{};
    if(!readNativeDominoTargetsFrame(read,base,a,stage,known)||!readNativeDominoTargetsFrame(read,base,b,stage,known))return false;
    if(stage)*stage=10;
    if(!sameNativeDominoTargets(a,b))return false;
    out=std::move(b);if(stage)*stage=0;return true;
}
