#pragma once
#include "native_ai_state.h"
#include "native_health.h"
#include <array>

// General authored-NPC binding. No names, health maxima, or local handles are
// hardcoded. This reader grants no mutation authority: a session and an owned
// pause lease must still be established by the caller.
struct NativeNpcAuthority {
    NativeWorldNpc npc{};
    NativeHealth health{};
    std::array<NativeAiComponentState,3> ai{}; // decision, reflex, attack
    std::array<unsigned,3> aiGeneration{};
    std::uintptr_t frame=0;
    unsigned frameGeneration=0,healthGeneration=0,spawnerGeneration=0;
};
// tree=false (1104): an ongoing channel keeps the same AI incarnation when the
// native behaviour tree swaps (patrol -> combat, confirmed 1103 lifecycleDiff
// 0x200); acquisition and double reads keep the strict comparison.
inline bool sameNpcAiIdentity(const NativeAiComponentState& a,const NativeAiComponentState& b,bool tree=true){
    return a.game==b.game&&a.actor==b.actor&&a.handle==b.handle&&a.component==b.component&&a.kind==b.kind&&
        a.manager==b.manager&&a.registry==b.registry&&a.poolIndex==b.poolIndex&&(!tree||a.tree==b.tree);
}
// 1102 diagnostic: which identity terms changed (bit per field group).
inline unsigned npcAuthorityIdentityDiff(const NativeNpcAuthority& a,const NativeNpcAuthority& b){
    const auto& x=a.npc;const auto& y=b.npc;unsigned d=0;
    if(x.actor!=y.actor||x.handle!=y.handle)d|=1;if(x.spawner!=y.spawner||x.spawnerHandle!=y.spawnerHandle||a.spawnerGeneration!=b.spawnerGeneration)d|=2;
    if(x.component!=y.component)d|=4;if(x.childCount!=y.childCount)d|=8;if(x.spawnerName!=y.spawnerName||x.definition!=y.definition)d|=16;
    if(!x.unique||!y.unique||!x.reciprocal||!y.reciprocal)d|=32;if(a.frame!=b.frame||a.frameGeneration!=b.frameGeneration)d|=64;
    if(a.healthGeneration!=b.healthGeneration||a.health.component!=b.health.component||a.health.declaration!=b.health.declaration)d|=128;
    if(a.aiGeneration!=b.aiGeneration)d|=256;
    for(unsigned i=0;i<3;++i){const auto& p=a.ai[i];const auto& q=b.ai[i];
        if(p.tree!=q.tree)d|=512u<<i;if(p.component!=q.component||p.manager!=q.manager||p.registry!=q.registry||p.poolIndex!=q.poolIndex||p.kind!=q.kind)d|=4096u<<i;}
    return d;
}
inline bool sameNpcAuthorityIdentity(const NativeNpcAuthority& a,const NativeNpcAuthority& b,bool tree=true){
    const auto& x=a.npc;const auto& y=b.npc;
    if(x.actor!=y.actor||x.handle!=y.handle||x.spawner!=y.spawner||x.spawnerHandle!=y.spawnerHandle||
       x.component!=y.component||x.childCount!=y.childCount||x.spawnerName!=y.spawnerName||x.definition!=y.definition||
       !x.unique||!y.unique||!x.reciprocal||!y.reciprocal||a.frame!=b.frame||a.frameGeneration!=b.frameGeneration||
       a.spawnerGeneration!=b.spawnerGeneration||a.healthGeneration!=b.healthGeneration||
       a.health.game!=b.health.game||a.health.actor!=b.health.actor||a.health.handle!=b.health.handle||
       a.health.component!=b.health.component||a.health.declaration!=b.health.declaration||a.aiGeneration!=b.aiGeneration)return false;
    for(unsigned i=0;i<3;++i)if(!sameNpcAiIdentity(a.ai[i],b.ai[i],tree))return false;
    return true;
}
inline bool npcAuthorityLiving(const NativeNpcAuthority& s){
    return s.health.logic==1&&std::isfinite(s.health.current)&&std::isfinite(s.health.maximum)&&
        s.health.maximum>0&&s.health.current>0&&s.health.current<=s.health.maximum;
}
inline bool npcAuthorityAttackInactive(const NativeNpcAuthority& s){return !(s.ai[2].flags&12)&&!s.ai[2].schedulerActive;}
inline bool npcAuthorityAcquireEligible(const NativeNpcAuthority& s){
    if(!s.npc.unique||!s.npc.reciprocal||!npcAuthorityLiving(s)||!npcAuthorityAttackInactive(s)||!s.ai[0].tree)return false;
    for(unsigned i=0;i<2;++i)if((s.ai[i].flags&12)!=8||(s.ai[i].flags&0x7fffff0)||!s.ai[i].schedulerActive)return false;
    return true;
}
// 1214el (#118): the host streams its own NPCs without pausing them, so an NPC whose decision and reflex AI the game
// itself has paused is streamable on the host (1514a: the dormant office gravehound had one pause reference and an
// inactive scheduler, so the host never bound it); the client still needs a running AI to take its lease.
inline bool npcAuthorityHostStreamEligible(const NativeNpcAuthority& s){
    if(!s.npc.unique||!s.npc.reciprocal||!npcAuthorityLiving(s)||!npcAuthorityAttackInactive(s)||!s.ai[0].tree)return false;
    for(unsigned i=0;i<2;++i)if((s.ai[i].flags&12)!=8)return false;
    return true;
}
inline bool npcAuthorityPaused(const NativeNpcAuthority& s){
    if(!npcAuthorityLiving(s)||!npcAuthorityAttackInactive(s))return false;
    for(unsigned i=0;i<2;++i)if((s.ai[i].flags&12)!=8||((s.ai[i].flags>>6)&0x1fffff)<1||
        ((s.ai[i].flags>>4)&3)!=2||s.ai[i].schedulerActive)return false;
    return true;
}
template<class Read> bool readNativeNpcAuthorityFrame(Read read,std::uintptr_t base,std::uintptr_t game,unsigned handle,NativeNpcAuthority& out){
    NativeNpcAuthority s{};
    if(!readWorldNpcIdentity(read,base,game,handle,s.npc)||!s.npc.unique||!s.npc.reciprocal||
       !readNativeHealth(read,base,handle,s.health)||s.health.game!=game||s.health.actor!=s.npc.actor||s.health.logic!=1)return false;
    constexpr NativeAiKind kinds[]={NativeAiKind::Decision,NativeAiKind::Reflex,NativeAiKind::Attack};
    for(unsigned i=0;i<3;++i){
        if(!readNativeAiComponent(read,base,game,handle,kinds[i],s.ai[i])||s.ai[i].actor!=s.npc.actor||
           !read(s.ai[i].component+0x18,&s.aiGeneration[i],4)||s.aiGeneration[i]==~0u)return false;
    }
    std::uintptr_t vt=0,owner=0,list=0;unsigned count=0;
    if(!read(s.npc.actor+0x358,&s.frame,8)||!s.frame||!read(s.frame,&vt,8)||vt!=base+0x1c573e8||
       !read(s.frame+0x10,&owner,8)||owner!=s.npc.actor||!read(s.frame+0x18,&s.frameGeneration,4)||s.frameGeneration==~0u||
       !read(s.health.component+0x18,&s.healthGeneration,4)||s.healthGeneration==~0u||
       !read(s.npc.component+0x18,&s.spawnerGeneration,4)||s.spawnerGeneration==~0u||
       !read(s.npc.actor+0x268,&list,8)||!list||!read(s.npc.actor+0x270,&count,4)||(count&0xffffff)>256)return false;
    unsigned matches=0;
    for(unsigned i=0;i<(count&0xffffff);++i){std::uintptr_t p=0;if(!read(list+i*8,&p,8))return false;if(p==s.frame)++matches;}
    if(matches!=1)return false;
    out=std::move(s);return true;
}
template<class Read> bool readNativeNpcAuthority(Read read,std::uintptr_t base,std::uintptr_t game,unsigned handle,NativeNpcAuthority& out){
    NativeNpcAuthority a{},b{};
    if(!readNativeNpcAuthorityFrame(read,base,game,handle,a)||!readNativeNpcAuthorityFrame(read,base,game,handle,b)||
       !sameNpcAuthorityIdentity(a,b)||a.health.current!=b.health.current||a.health.maximum!=b.health.maximum)return false;
    for(unsigned i=0;i<3;++i)if(a.ai[i].flags!=b.ai[i].flags||a.ai[i].schedulerActive!=b.ai[i].schedulerActive)return false;
    out=std::move(b);return true;
}

// 1214ad: life state of every unique, reciprocal NPC (0 alive, 1 unconscious, 2 dead).
struct NativeNpcLife{std::string spawner,definition;std::uint32_t handle=0;std::uintptr_t actor=0;unsigned state=0;};
// Catalog uniqueness includes unused authored spawners. Single-handle reads
// establish local lifetime only; they cannot discover cross-process targets.
template<class Read> bool readNativeNpcAuthorityCatalog(Read read,std::uintptr_t base,std::uintptr_t game,
        std::vector<NativeNpcAuthority>& entries,unsigned& ineligible,unsigned& unavailable,unsigned& unbound,std::vector<NativeNpcLife>* life=nullptr,bool hostStream=false){
    std::vector<NativeWorldNpc> catalog;entries.clear();ineligible=unavailable=0;
    if(!readWorldNpcCatalog(read,base,game,catalog,unbound))return false;
    for(const auto& n:catalog){
        if(!n.unique||!n.reciprocal){++ineligible;continue;}
        NativeHealth h{};
        if(!readNativeHealth(read,base,n.handle,h)){++unavailable;continue;}
        if(life&&h.game==game&&h.actor==n.actor&&std::isfinite(h.current)&&life->size()<1024)
            life->push_back({n.spawnerName,n.definition,n.handle,n.actor,h.current<=0?2u:((h.ownerFlags&0x20)?1u:0u)});
        if(h.game!=game||h.actor!=n.actor||h.logic!=1||h.current<=0){++ineligible;continue;}
        NativeNpcAuthority s{};
        if(!readNativeNpcAuthority(read,base,game,n.handle,s)){++unavailable;continue;}
        if(!(hostStream?npcAuthorityHostStreamEligible(s):npcAuthorityAcquireEligible(s))){++ineligible;continue;}
        if(s.npc.spawner!=n.spawner||s.npc.component!=n.component||s.npc.spawnerName!=n.spawnerName||s.npc.definition!=n.definition){++unavailable;continue;}
        if(entries.size()>=1024)return false;
        entries.push_back(std::move(s));
    }
    return true;
}
