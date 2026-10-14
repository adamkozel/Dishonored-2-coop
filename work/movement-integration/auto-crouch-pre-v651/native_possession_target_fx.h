#pragma once
#include "native_player_possession_item.h"
#include "native_fx_record_by_id.h"
// Exact-image 448200 selects NPC weak reference at power+b8/c0 (handle+b0).
// 4c4350 registers message135; 4c4e10 creates the target FX actor at NPC
// component+28 and stores its ID at+30, using declaration+a8. 4c4f20 retires
// that ID. Read only: never send135 or invoke the NPC's gameplay handlers.
constexpr const char* kPossessionHumanTargetFx="models/effects/gameplay/powers/possession/hosts_target/possession_target_human_01.fx";
struct NativePossessionTargetFx {
    NativePossessionItem equipment{};
    std::uintptr_t component=0,actor=0,declaration=0,fxActor=0,fxDeclaration=0,swarm=0;
    unsigned handle=0,generation=0,flags=0,id=~0u;
    bool selected=false,present=false;
    NativeWindblastFxRecord effect{};
};
template<class Read> bool readNativePossessionTargetFxFrame(Read read,std::uintptr_t base,NativePossessionTargetFx& s,unsigned* stage=nullptr){
    const auto at=[&](unsigned x){if(stage)*stage=x;};at(1);
    if(!readNativePossessionItem(read,base,s.equipment,true)||!s.equipment.started)return false;
    const auto pc=s.equipment.component,game=s.equipment.player.sword.game;
    at(2);
    if(!read(pc+0xb0,&s.handle,4)||!read(pc+0xb8,&s.generation,4)||!read(pc+0xc0,&s.component,8)||!read(pc+0xc8,&s.swarm,8)||s.swarm)return false;
    if(s.generation==~0u)return !s.component&&s.handle==0x3fff0010;
    const auto eq=[&](std::uintptr_t p,std::uintptr_t value){std::uintptr_t x=0;return read(p,&x,8)&&x==value;};
    const auto named=[&](std::uintptr_t d,std::uintptr_t vt,const char* expected){std::uintptr_t p=0;std::string name;return d&&eq(d,base+vt)&&read(d+8,&p,8)&&worldIdentityString(read,p,name)&&name==expected;};
    unsigned generation=0;at(3);
    if(!s.handle||s.handle==1||s.handle==0x3fff0010||s.handle==~0u||!s.component||!eq(s.component,base+0x1b641a8)||
       !read(s.component+0x18,&generation,4)||generation!=s.generation||!read(s.component+0x1c,&s.flags,4)||(s.flags&0xc)!=8||
       !read(s.component+0x10,&s.actor,8)||!s.actor||worldIdentityEntity(read,game,s.handle)!=s.actor||!eq(s.actor,base+0x1c5e108))return false;
    std::uintptr_t list=0;unsigned count=0,members=0;at(4);
    if(!read(s.actor+0x268,&list,8)||!list||!read(s.actor+0x270,&count,4)||!(count&0xffffff)||(count&0xffffff)>256)return false;
    for(unsigned i=0;i<(count&0xffffff);++i){std::uintptr_t c=0;if(!read(list+i*8,&c,8))return false;if(c==s.component)++members;}
    if(members!=1||!read(s.component+8,&s.declaration,8)||
       !named(s.declaration,0x1b63d68,"components/characters/npc/base/possession_human.cpntnpcpossession"))return false;
    at(5);
    if(!read(s.declaration+0xa8,&s.fxDeclaration,8)||!named(s.fxDeclaration,0x1ab3138,kPossessionHumanTargetFx)||
       !read(s.component+0x28,&s.fxActor,8)||!read(s.component+0x30,&s.id,4))return false;
    s.selected=true;
    // Native stop resets only the ID. A stale actor pointer is never followed.
    if(s.id==~0u)return true;
    at(6);
    if(!s.id||!s.fxActor||!eq(s.fxActor,base+0x1c887f8)||!eq(s.fxActor+0x2e8,0)||
       !readNativeFxRecordById(read,base,game,s.id,s.effect)||s.effect.declaration!=s.fxDeclaration||s.effect.name!=kPossessionHumanTargetFx||
       s.effect.ownerActor!=s.fxActor||s.effect.parent!=0x3fff0010||s.effect.owner==1||s.effect.owner==s.handle)return false;
    float root[3]{};at(7);
    if(!read(s.actor+0x300,root,12))return false;
    float distance=0;for(unsigned i=0;i<3;++i){if(!std::isfinite(root[i]))return false;const auto d=s.effect.origin[i]-root[i];distance+=d*d;}
    if(!std::isfinite(distance)||distance>16.f)return false;
    const auto a=s.effect.axis;
    for(unsigned i=0;i<3;++i)for(unsigned j=i;j<3;++j){float dot=0;for(unsigned k=0;k<3;++k)dot+=a[i*3+k]*a[j*3+k];if(std::fabs(dot-(i==j?1.f:0.f))>.04f)return false;}
    const auto det=a[0]*(a[4]*a[8]-a[5]*a[7])-a[1]*(a[3]*a[8]-a[5]*a[6])+a[2]*(a[3]*a[7]-a[4]*a[6]);
    if(std::fabs(det-1.f)>.04f)return false;
    s.present=true;return true;
}
template<class Read> bool readNativePossessionTargetFx(Read read,std::uintptr_t base,NativePossessionTargetFx& out,unsigned* stage=nullptr){
    NativePossessionTargetFx a{},b{};
    if(!readNativePossessionTargetFxFrame(read,base,a,stage)||!readNativePossessionTargetFxFrame(read,base,b,stage))return false;
    if(stage)*stage=8;
    if(!sameSwordEquipmentIdentity(a.equipment.player.sword,b.equipment.player.sword)||a.equipment.entry!=b.equipment.entry||
       a.equipment.component!=b.equipment.component||a.equipment.equipmentDeclaration!=b.equipment.equipmentDeclaration||a.equipment.flags!=b.equipment.flags||
       a.equipment.quantity!=b.equipment.quantity||a.equipment.parentDeclaration!=b.equipment.parentDeclaration||
       a.component!=b.component||a.actor!=b.actor||a.declaration!=b.declaration||a.fxActor!=b.fxActor||a.fxDeclaration!=b.fxDeclaration||
       a.handle!=b.handle||a.generation!=b.generation||a.flags!=b.flags||a.id!=b.id||a.selected!=b.selected||a.present!=b.present||
       a.effect.record!=b.effect.record||a.effect.owner!=b.effect.owner||a.effect.ownerActor!=b.effect.ownerActor)return false;
    out=std::move(b);if(stage)*stage=0;return true;
}
