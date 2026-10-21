#pragma once
#include "native_doppelganger_equipment_owner.h"
#include <array>
// Read-only candidate416. The equipment's committed array selects the clone;
// generated names, preview handles and the co-op avatar never select it.
// Static43c950 and raw406b/409/411 identify +b8/+c0/+d4. Runtime reciprocal
// validation is required before this observer can source presentation.
// 1214eh (#11 power_upgrade_variants): Twin Shade keeps two committed clones at once.
constexpr unsigned kDoppelgangerArraySlots=2;
struct NativeDoppelgangerWorld {
    NativeDoppelgangerItem equipment{};
    std::uintptr_t handles=0,actor=0,definition=0,components=0,component=0,declaration=0,parentDeclaration=0;
    unsigned arrayFlags=0,preview=0x3fff0010,last=0x3fff0010,handle=0x3fff0010;
    unsigned componentFlags=0,generation=0,componentCount=0,rawA0=0,registryHandle=0;
    std::uintptr_t registryActor=0;
    std::array<float,12> transform{};
    bool present=false,retired=false;
    unsigned kind=0; // 1214gu (#131): 1 the decoy shade, 2 the Deadly Shade's deadly shadow
};
inline bool validDoppelgangerTransform(const std::array<float,12>& t){
    for(unsigned i=0;i<12;++i)if(!std::isfinite(t[i])||std::fabs(t[i])>(i<3?100000.f:1.01f))return false;
    for(unsigned i=0;i<3;++i)for(unsigned j=i;j<3;++j){float dot=0;for(unsigned k=0;k<3;++k)dot+=t[3+i*3+k]*t[3+j*3+k];if(std::fabs(dot-(i==j?1.f:0.f))>.02f)return false;}
    const float det=t[3]*(t[7]*t[11]-t[10]*t[8])-t[6]*(t[4]*t[11]-t[10]*t[5])+t[9]*(t[4]*t[8]-t[7]*t[5]);return det>.98f&&det<1.02f;
}
template<class Read> bool readNativeDoppelgangerWorldFrame(Read read,std::uintptr_t base,NativeDoppelgangerWorld& s,unsigned* stage=nullptr,const NativeDoppelgangerItem* known=nullptr,unsigned cloneSlot=0){
    const auto mark=[&](unsigned n){if(stage)*stage=n;};mark(1);
    if(!(known?readCurrentNativeDoppelgangerEquipment(read,base,*known,s.equipment):readNativeDoppelgangerItem(read,base,s.equipment)))return false;
    if(!s.equipment.instantiated){mark(0);return true;}
    const auto pc=s.equipment.component,game=s.equipment.player.sword.game;
    mark(2);
    // 1214eh: each slot reads its own committed entry (Twin Shade: two); the last spawned (+d4)
    // must be the one entry, or one of the two.
    if(!read(pc+0xb8,&s.handles,8)||!read(pc+0xc0,&s.arrayFlags,4)||(s.arrayFlags&0xffffff)>kDoppelgangerArraySlots||
       !read(pc+0xb0,&s.preview,4)||!read(pc+0xd4,&s.last,4))return false;
    const unsigned committed=s.arrayFlags&0xffffff;
    if(cloneSlot>=committed){mark(0);return true;}
    std::array<unsigned,kDoppelgangerArraySlots> entries{};
    if(!s.handles||!read(s.handles,entries.data(),committed*4))return false;
    for(unsigned i=0;i<committed;++i)if(entries[i]==0x3fff0010||entries[i]==1)return false;
    if(committed==2&&entries[0]==entries[1])return false;
    s.handle=entries[cloneSlot];if(committed==1?s.handle!=s.last:(entries[0]!=s.last&&entries[1]!=s.last))return false;
    mark(3);
    // A stale array entry is native behavior:43c950 prunes it on a later cast.
    // Successful registry reads can prove retirement. A read failure cannot.
    const auto slot=static_cast<std::int32_t>(s.handle)>>16;
    if(slot<0||slot>=16384||!read(game+0x3130b0+slot*8,&s.registryActor,8))return false;
    if(s.registryActor&&!read(s.registryActor+0xa8,&s.registryHandle,4))return false;
    if(!s.registryActor||s.registryHandle!=s.handle){s.retired=true;mark(0);return true;}
    s.actor=s.registryActor;
    const auto eq=[&](std::uintptr_t p,std::uintptr_t v){std::uintptr_t x=0;return read(p,&x,8)&&x==v;};
    const auto named=[&](std::uintptr_t d,std::uintptr_t vt,const char* name){std::uintptr_t p=0;std::string text;return d&&eq(d,base+vt)&&read(d+8,&p,8)&&worldIdentityString(read,p,text)&&text==name;};
    if(!s.actor||s.actor==s.equipment.entity||!eq(s.actor,base+0x1c5e108)||
       !read(s.actor+0x2e8,&s.definition,8)||
       !read(s.actor+0x268,&s.components,8)||!s.components||!read(s.actor+0x270,&s.componentCount,4)||
       !(s.componentCount&0xffffff)||(s.componentCount&0xffffff)>256)return false;
    // 1214gu (#131): with Deadly Shade the committed clone is models/powers/doppelganger/deadlyshadow_entity.def, an arkNPC
    // with arkCpntDeadlyShadow and no Decoy component (partner-shade-v1729a: 595 unknown reads here). It is presented by
    // its actor alone: no Decoy generation or flags.
    if(named(s.definition,0x1ab58f8,"models/powers/doppelganger/deadlyshadow_entity.def")){
        mark(5);if(!read(s.actor+0x300,s.transform.data(),sizeof(s.transform))||!validDoppelgangerTransform(s.transform))return false;
        s.kind=2;s.present=true;mark(0);return true;}
    if(!named(s.definition,0x1ab58f8,"models/powers/doppelganger/decoy_entity.def"))return false;
    s.kind=1;
    std::array<std::uintptr_t,256> componentEntries{};
    if(!read(s.components,componentEntries.data(),(s.componentCount&0xffffff)*8))return false;
    for(unsigned i=0;i<(s.componentCount&0xffffff);++i){std::uintptr_t c=componentEntries[i],vt=0;if(!c||!read(c,&vt,8))return false;
        if(vt==base+0x1c4fe28){if(s.component)return false;s.component=c;}}
    mark(4);std::uintptr_t name=0;std::string text;
    if(!s.component||!eq(s.component+0x10,s.actor)||!read(s.component+0x18,&s.generation,4)||
       !read(s.component+0x1c,&s.componentFlags,4)||(s.componentFlags&0xc)!=8||
       !read(s.component+8,&s.declaration,8)||!s.declaration||!eq(s.declaration,base+0x1d20068)||
       !read(s.declaration+8,&name,8)||!worldIdentityString(read,name,text))return false;
    constexpr auto exact="components/characters/player/base/powers/decoy/decoy.cpntdecoy";
    if(text=="unnamed"){
        if(!read(s.declaration+0x48,&s.parentDeclaration,8)||s.parentDeclaration==s.declaration||!named(s.parentDeclaration,0x1d20068,exact))return false;
    }else if(text!=exact)return false;
    // Opaque data only:39e140 acts on a separate helper, not this Decoy component.
    // Actual component+a0 is128 in415; do not assign a lifecycle meaning.
    mark(5);if(!read(s.component+0xa0,&s.rawA0,4)||!read(s.actor+0x300,s.transform.data(),sizeof(s.transform))||!validDoppelgangerTransform(s.transform))return false;
    s.present=true;mark(0);return true;
}
template<class Read> bool readNativeDoppelgangerWorld(Read read,std::uintptr_t base,NativeDoppelgangerWorld& out,unsigned* stage=nullptr,const NativeDoppelgangerItem* known=nullptr,unsigned slot=0){
    NativeDoppelgangerWorld a{},b{};
    if(!readNativeDoppelgangerWorldFrame(read,base,a,stage,known,slot)||!readNativeDoppelgangerWorldFrame(read,base,b,stage,known,slot))return false;
    if(stage)*stage=6;
    const auto& x=a.equipment;const auto& y=b.equipment;
    if(!sameSwordEquipmentIdentity(x.player.sword,y.player.sword)||x.entry!=y.entry||x.component!=y.component||x.entity!=y.entity||x.handle!=y.handle||
       x.equipmentDeclaration!=y.equipmentDeclaration||x.parentDeclaration!=y.parentDeclaration||x.quantity!=y.quantity||x.flags!=y.flags||
       a.present!=b.present||a.retired!=b.retired||a.registryActor!=b.registryActor||a.registryHandle!=b.registryHandle||a.handles!=b.handles||a.arrayFlags!=b.arrayFlags||a.preview!=b.preview||a.last!=b.last||a.handle!=b.handle||
       a.actor!=b.actor||a.definition!=b.definition||a.components!=b.components||a.componentCount!=b.componentCount||
       a.component!=b.component||a.generation!=b.generation||a.componentFlags!=b.componentFlags||a.declaration!=b.declaration||a.parentDeclaration!=b.parentDeclaration)return false;
    out=std::move(b);if(stage)*stage=0;return true;
}
