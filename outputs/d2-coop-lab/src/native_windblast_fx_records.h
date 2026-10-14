#pragma once
#include "world_identity.h"
#include <array>
#include <cstring>
#include <unordered_set>
// Read-only retail FX ID map, reconstructed from 19d100/1a04a0/1a2970.
// Records are not gameplay events. Windblast definitions and contact effects
// are returned. An externally changing snapshot is rejected, never repaired.
struct NativeWindblastFxRecord {
    std::uintptr_t record=0,declaration=0,ownerActor=0;
    unsigned id=0,owner=0,parent=0;
    float origin[3]{},axis[9]{};
    std::string name,ownerDefinition;
};
inline bool nativeWindblastFxName(const std::string& n){
    return n=="models/effects/d2coop/windblast_remote.fx"||
        (n.rfind("contactsystem/windblast_",0)==0&&n.size()>3&&n.compare(n.size()-3,3,".fx")==0)||
        n=="models/effects/gameplay/powers/windblast/blast/wblast_blast_01.fx"||
        n=="models/effects/gameplay/powers/windblast/blast/wblast_stronger_blast_01.fx"||
        n=="models/effects/gameplay/powers/windblast/blast/wblast_wave_blast_01.fx"||
        n=="models/effects/gameplay/powers/windblast/blast_impact/ply/wblast_ply_dust_grnd_wave_01.fx"||
        n=="models/effects/gameplay/powers/windblast/npc_impact/wblast_npc_impact_01.fx";
}
template<class Read,class Filter> bool readNativeFilteredFxRecords(Read read,std::uintptr_t base,std::uintptr_t game,std::vector<NativeWindblastFxRecord>& out,Filter wanted,unsigned* failureLine=nullptr,NativeWindblastFxRecord* diagnostic=nullptr){
    if(failureLine)*failureLine=0;const auto fail=[&](unsigned line){if(failureLine)*failureLine=line;return false;};
    out.clear();std::uintptr_t vt=0,active=0,pool=0,buckets=0;unsigned mask=0,count=0;
    const auto manager=game+0x1f8430,map=manager+0xeef60;
    if(!game||!read(base+0x39a5290,&active,8)||active!=game||!read(game,&vt,8)||vt!=base+0x1c940a8||
       !read(manager,&pool,8)||!pool||!read(map,&buckets,8)||!buckets||!read(map+0xc,&count,4)||count>1024||
       !read(map+0x10,&mask,4)||mask>65535||((mask+1)&mask))return fail(__LINE__);
    std::vector<std::uintptr_t> heads(mask+1),again(mask+1);
    if(!read(buckets,heads.data(),heads.size()*8))return fail(__LINE__);
    struct Node {unsigned id;std::uint16_t slot,pad;std::uintptr_t next;};
    static_assert(sizeof(Node)==16);
    std::vector<std::pair<std::uintptr_t,Node>> nodes;std::unordered_set<std::uintptr_t> visited;
    std::unordered_set<unsigned> ids,slots;std::vector<NativeWindblastFxRecord> records;
    for(unsigned bucket=0;bucket<=mask;++bucket){
        for(auto p=heads[bucket];p;){
            Node n{};if(nodes.size()>=1024||!visited.insert(p).second||!read(p,&n,sizeof(n))||n.slot>=1024||
                (n.id&mask)!=bucket||!ids.insert(n.id).second||!slots.insert(n.slot).second)return fail(__LINE__);
            nodes.push_back({p,n});NativeWindblastFxRecord r{};r.record=pool+std::uintptr_t(n.slot)*0x140;
            std::array<std::uintptr_t,2> identity{};std::uintptr_t text=0;
            if(!read(r.record,identity.data(),16))return fail(__LINE__);
            r.owner=unsigned(identity[0]);r.id=unsigned(identity[0]>>32);r.declaration=identity[1];
            if(r.id!=n.id||!r.declaration||!read(r.declaration,&vt,8)||vt!=base+0x1ab3138||
               !read(r.declaration+8,&text,8)||!worldIdentityString(read,text,r.name))return fail(__LINE__);
            if(wanted(r.name)){
                if(!read(r.record+0x38,r.origin,12)||!read(r.record+0x44,r.axis,36))return fail(__LINE__);
                for(float f:r.origin)if(!std::isfinite(f)||std::fabs(f)>100000)return fail(__LINE__);
                for(float f:r.axis)if(!std::isfinite(f)||std::fabs(f)>1.01f)return fail(__LINE__);
                r.ownerActor=worldIdentityEntity(read,game,r.owner);
                if(diagnostic)*diagnostic=r;
                if(r.ownerActor){
                    std::uintptr_t def=0,againDef=0;
                    if(!read(r.ownerActor+0xac,&r.parent,4)||!read(r.ownerActor+0x2e8,&def,8))return fail(__LINE__);
                    // Native world muzzle flashes use temporary idEntityFx
                    // owners with no entity definition. Preserve that absence;
                    // consumers must establish their own semantic attribution.
                    if(def&&(!read(def,&vt,8)||vt!=base+0x1ab58f8||!read(def+8,&text,8)||!worldIdentityString(read,text,r.ownerDefinition)))return fail(__LINE__);
                    if(worldIdentityEntity(read,game,r.owner)!=r.ownerActor||!read(r.ownerActor+0x2e8,&againDef,8)||againDef!=def)return fail(__LINE__);
                }
                std::array<std::uintptr_t,2> finalIdentity{};
                if(!read(r.record,finalIdentity.data(),16)||finalIdentity!=identity)return fail(__LINE__);
                records.push_back(std::move(r));
            }
            p=n.next;
        }
    }
    if(nodes.size()!=count||!read(buckets,again.data(),again.size()*8)||heads!=again)return fail(__LINE__);
    for(const auto& e:nodes){Node n{};if(!read(e.first,&n,sizeof(n))||std::memcmp(&n,&e.second,sizeof(n)))return fail(__LINE__);}
    unsigned countAgain=0,maskAgain=0;std::uintptr_t ptr=0;
    if(!read(map+0xc,&countAgain,4)||countAgain!=count||!read(map+0x10,&maskAgain,4)||maskAgain!=mask||
       !read(map,&ptr,8)||ptr!=buckets||!read(manager,&ptr,8)||ptr!=pool||!read(base+0x39a5290,&ptr,8)||ptr!=game)return fail(__LINE__);
    out=std::move(records);return true;
}
template<class Read> bool readNativeWindblastFxRecords(Read read,std::uintptr_t base,std::uintptr_t game,std::vector<NativeWindblastFxRecord>& out,unsigned* failureLine=nullptr){
    return readNativeFilteredFxRecords(read,base,game,out,nativeWindblastFxName,failureLine);
}
inline bool nativeRangedFxName(const std::string& n){return n=="models/effects/weapons/gun/muzzle_1p_corvo.fx"||n=="models/effects/weapons/gun/muzzle_1p_emily.fx"||
    n=="models/effects/weapons/gun/muzzle_1p_barrel_smoke.fx"||n=="models/effects/weapons/gun/muzzle_3p.fx"||
    n=="models/effects/d2coop/muzzle_remote_corvo.fx"||n=="models/effects/d2coop/muzzle_remote_emily.fx";}
template<class Read> bool readNativeRangedFxRecords(Read read,std::uintptr_t base,std::uintptr_t game,std::vector<NativeWindblastFxRecord>& out,unsigned* failureLine=nullptr,NativeWindblastFxRecord* diagnostic=nullptr){
    return readNativeFilteredFxRecords(read,base,game,out,nativeRangedFxName,failureLine,diagnostic);
}
