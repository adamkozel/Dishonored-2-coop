#pragma once
#include <cstdio>
// 1214gd (#140): the partner's copy of a corpse that vanishes. With the key-target finisher recognized (1214gc), her kill
// of Ramsey was shared, but in all three takes route-dunwall-lead-v1708a-c his corpse then left her world: the search
// found no mortimer_ramsey entity, her world held an ARK_CUE_NPC_DISAPPEARED cue at his spot and an anonymous entity
// 70 m below the floor. This trace samples her corpse from its death (every 100 ms for 4 s, then every 500 ms): the root,
// whether the corpse hold (npcCorpseHoldAnimated) holds it, whether her paired scene still runs and the ragdoll mode,
// and when its native read fails. coop_corpse_hold own_paired_skip leaves the corpse of her own paired kill to its
// native physics instead of holding it on the host's stream (own_paired_keep restores the hold), so takes can compare.
struct NpcCorpseSample {unsigned ms=0;float root[3]{};unsigned char held=0,paired=0,mode=0xff;};
struct NpcCorpseTrace {unsigned arm=0,handle=0,count=0,vanishedMs=0;bool vanished=false,ownPaired=false,holdSkipped=false;unsigned long long startedAt=0;NpcCorpseSample samples[80]{};};
NpcCorpseTrace npcCorpseTraces[4]{};unsigned npcCorpseTraceCount=0;
bool npcSkipOwnPairedCorpseHold=false;unsigned long long npcOwnPairedHoldSkips=0;
inline NpcCorpseTrace* npcCorpseTraceFor(unsigned arm,unsigned handle){for(auto& t:npcCorpseTraces)if(t.startedAt&&t.arm==arm&&t.handle==handle)return &t;return nullptr;}
inline void npcCorpseTraceBegin(unsigned arm,unsigned handle,unsigned long long tick,bool ownPaired){
    auto& t=npcCorpseTraces[npcCorpseTraceCount++%4];t=NpcCorpseTrace{};t.arm=arm;t.handle=handle;t.startedAt=tick?tick:1;t.ownPaired=ownPaired;}
inline void npcCorpseTraceSample(NpcCorpseTrace& t,unsigned long long tick,const float* root,bool held,bool paired,unsigned mode){
    if(t.vanished||t.count>=80)return;const unsigned ms=static_cast<unsigned>(tick-t.startedAt);
    if(t.count&&ms>4000&&ms-t.samples[t.count-1].ms<500)return;
    auto& s=t.samples[t.count++];s.ms=ms;if(root){s.root[0]=root[0];s.root[1]=root[1];s.root[2]=root[2];}
    s.held=held?1:0;s.paired=paired?1:0;s.mode=static_cast<unsigned char>(mode>255?255:mode);}
inline void writeNpcCorpseTraces(FILE* f){
    fprintf(f,",\"ownPairedHoldSkip\":%s,\"ownPairedHoldSkips\":%llu,\"corpseTraces\":[",npcSkipOwnPairedCorpseHold?"true":"false",npcOwnPairedHoldSkips);
    bool first=true;
    for(const auto& t:npcCorpseTraces){if(!t.startedAt)continue;
        fprintf(f,"%s{\"arm\":%u,\"handle\":%u,\"ownPaired\":%s,\"holdSkipped\":%s,\"vanished\":%s,\"vanishedMs\":%u,\"samples\":[",first?"":",",t.arm,t.handle,
            t.ownPaired?"true":"false",t.holdSkipped?"true":"false",t.vanished?"true":"false",t.vanishedMs);first=false;
        for(unsigned i=0;i<t.count;++i){const auto& s=t.samples[i];fprintf(f,"%s[%u,%.2f,%.2f,%.2f,%u,%u,%u]",i?",":"",s.ms,s.root[0],s.root[1],s.root[2],s.held,s.paired,s.mode);}
        fputs("]}",f);}
    fputs("]",f);
}
