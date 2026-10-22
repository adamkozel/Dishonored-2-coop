#pragma once
// Client-side ownership proof for the general NPC binding. This deliberately
// does not replicate roots, animation, damage, or outcomes. Reusable per-actor
// leases below will underpin the separate authority transport.
struct NpcAiLease {
    NativeNpcAuthority binding{};
    std::uint64_t epoch=0;
    unsigned references=0;
    std::uint64_t acquired=0,released=0,retired=0,lost=0;
    bool ready=false;
};
struct NpcAuthorityHold {
    std::vector<NpcAiLease> actors;
    std::uintptr_t game=0;
    std::uint64_t epoch=0,peerSession=0,peerEpoch=0;
    ULONGLONG until=0,started=0,nextCheck=0;
    unsigned ineligible=0,unavailable=0,unbound=0,scanMs=0;
    std::uint64_t acquired=0,released=0,retired=0,lost=0,checks=0,refused=0;
    bool ready=false;
    const char* result="idle";
};
NpcAuthorityHold npcAuthorityHold{};
int npcAuthorityHoldRequest=-1;
bool npcLeaseGenerationCurrent(const NpcAiLease& lease,unsigned index){
    if(index>=2||outgoing.worldEpoch!=lease.epoch||observedGame!=lease.binding.health.game)return false;
    const auto& s=lease.binding.ai[index];unsigned generation=~0u;
    return worldIdentityEntity(memoryRead,s.game,s.handle)==s.actor&&
        memoryRead(s.component+0x18,&generation,4)&&generation==lease.binding.aiGeneration[index];
}
void releaseNpcAiLease(NpcAiLease& lease){
    for(unsigned i=lease.references;i>0;--i){
        if(!npcLeaseGenerationCurrent(lease,i-1)){++lease.lost;continue;}
        const auto& held=lease.binding.ai[i-1];
        if(releaseNativeAiReference(held,lease.epoch))++lease.released;
        else if(readNativeDeadAiRetirement(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),held))++lease.retired;
        else ++lease.lost;
    }
    lease.references=0;lease.ready=false;
}
// 1214ij (#160): forced=true skips the calm-AI eligibility, for her Possession only: the game suspends a possessed
// victim's AI whatever its state, and an alert guard refused the lease (partner-corvo-power-v1798a, v1800b).
bool acquireNpcAiLease(NpcAiLease& lease,bool forced=false){
    if(lease.references||outgoing.worldEpoch!=lease.epoch||!nativeAiRequestsMatch())return false;
    NativeNpcAuthority fresh{};
    if(!readNativeNpcAuthority(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,lease.binding.npc.handle,fresh)||
       !sameNpcAuthorityIdentity(lease.binding,fresh)||(!forced&&!npcAuthorityLeaseEligible(fresh)))return false;
    lease.binding=std::move(fresh);
    for(unsigned i=0;i<2;++i){
        const auto& s=lease.binding.ai[i];
        if(!npcLeaseGenerationCurrent(lease,i))return false;
        // 1214ex (#113): the game's own pause references (a scripted post) stay; the lease adds exactly one.
        const unsigned before=(s.flags>>6)&0x1fffff;
        reinterpret_cast<void(*)(void*,void*)>(gameImage+0x7e7800)(reinterpret_cast<void*>(s.component),reinterpret_cast<void*>(s.manager));
        ++lease.references;++lease.acquired;
        unsigned flags=0;if(!memoryRead(s.component+0x1c,&flags,4)||((flags>>6)&0x1fffff)!=before+1)return false;
    }
    return true;
}
void stopNpcAuthorityHold(const char* reason){
    if(!npcAuthorityHold.until)return;
    for(auto it=npcAuthorityHold.actors.rbegin();it!=npcAuthorityHold.actors.rend();++it){releaseNpcAiLease(*it);npcAuthorityHold.released+=it->released;npcAuthorityHold.retired+=it->retired;npcAuthorityHold.lost+=it->lost;}
    npcAuthorityHold.until=0;npcAuthorityHold.ready=false;npcAuthorityHold.result=reason;npcAuthorityHoldActive=false;
}
void beginNpcAuthorityHold(ULONGLONG tick){
    auto& h=npcAuthorityHold;
    if(h.until){++h.refused;return;}
    h={};
    if(localRole!=1||!coopNpcPairReady(tick)||!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load()||
       npcAuthorityStreamActive||observerIsolation.until||guardLink.phase!=GuardPhase::Idle||aiDecisionHold.expires||guardAiOwned||!nativeAiRequestsMatch()){
        ++h.refused;h.result="scope_or_existing_owner_refused";return;
    }
    const auto begin=GetTickCount64();std::vector<NativeNpcAuthority> catalog;
    if(!readNativeNpcAuthorityCatalog(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,catalog,h.ineligible,h.unavailable,h.unbound)||catalog.empty()){
        ++h.refused;h.result="no_eligible_catalog";return;
    }
    h.scanMs=static_cast<unsigned>(GetTickCount64()-begin);h.game=observedGame;h.epoch=outgoing.worldEpoch;
    h.peerSession=remote.session;h.peerEpoch=remote.worldEpoch;h.started=GetTickCount64();h.until=h.started+12000;
    // Freeze the catalog for this bounded test; newly activated script actors
    // require fresh negotiation, never silently acquire a different target.
    for(auto& n:catalog){NpcAiLease lease{};lease.binding=std::move(n);lease.epoch=h.epoch;h.actors.push_back(std::move(lease));}
    npcAuthorityHoldActive=true;h.result="pausing";
    for(auto& lease:h.actors){const bool ok=acquireNpcAiLease(lease);h.acquired+=lease.acquired;if(!ok){++h.refused;stopNpcAuthorityHold("acquire_failed");return;}}
}
void serviceNpcAuthorityHold(ULONGLONG tick){
    if(npcAuthorityHoldRequest>=0){const int requested=npcAuthorityHoldRequest;npcAuthorityHoldRequest=-1;
        if(requested){beginNpcAuthorityHold(tick);return;}else stopNpcAuthorityHold("cancelled");}
    auto& h=npcAuthorityHold;if(!h.until)return;
    if(tick>=h.until){stopNpcAuthorityHold("expired");return;}
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load()||observedGame!=h.game||outgoing.worldEpoch!=h.epoch||
       remote.session!=h.peerSession||remote.worldEpoch!=h.peerEpoch||!coopNpcPairReady(tick)){
        stopNpcAuthorityHold("scope_changed");return;
    }
    if(tick<h.nextCheck)return;h.nextCheck=tick+100;bool ready=true;
    for(auto& lease:h.actors){NativeNpcAuthority fresh{};
        if(!readNativeNpcAuthority(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),h.game,lease.binding.npc.handle,fresh)||
           !sameNpcAuthorityIdentity(lease.binding,fresh)||!npcAuthorityLiving(fresh)||!npcAuthorityAttackInactive(fresh)){
            stopNpcAuthorityHold("identity_or_lifecycle_changed");return;
        }
        lease.ready=npcAuthorityPaused(fresh);ready=ready&&lease.ready;
    }
    ++h.checks;
    if(!ready&&(h.ready||tick-h.started>=1000)){stopNpcAuthorityHold("pause_not_owned");return;}
    h.ready=ready;if(ready)h.result="held";
}
void writeNpcAuthorityHold(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.npc-authority.json",statusPath);std::snprintf(temp,sizeof(temp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;const auto& h=npcAuthorityHold;
    fprintf(f,"{\"role\":%u,\"active\":%s,\"ready\":%s,\"result\":\"%s\",\"actors\":%zu,\"ineligible\":%u,\"unavailable\":%u,\"unbound\":%u,\"scanMs\":%u,\"acquired\":%llu,\"released\":%llu,\"retired\":%llu,\"lost\":%llu,\"checks\":%llu,\"refused\":%llu,\"epoch\":%llu,\"handles\":[",
        localRole,h.until?"true":"false",h.ready?"true":"false",h.result,h.actors.size(),h.ineligible,h.unavailable,h.unbound,h.scanMs,h.acquired,h.released,h.retired,h.lost,h.checks,h.refused,h.epoch);
    for(unsigned i=0;i<h.actors.size();++i)fprintf(f,"%s%u",i?",":"",h.actors[i].binding.npc.handle);
    fprintf(f,"],\"entries\":[");
    for(unsigned i=0;i<h.actors.size();++i){const auto& l=h.actors[i];fprintf(f,"%s{\"handle\":%u,\"references\":%u,\"ready\":%s,\"health\":%.9g,\"maximum\":%.9g,\"generations\":[%u,%u,%u]}",i?",":"",l.binding.npc.handle,l.references,l.ready?"true":"false",l.binding.health.current,l.binding.health.maximum,l.binding.aiGeneration[0],l.binding.aiGeneration[1],l.binding.aiGeneration[2]);}
    fprintf(f,"]}\n");const bool ok=!ferror(f);const int close=fclose(f);if(ok&&!close)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
