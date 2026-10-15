#pragma once
// 1213u (#46): host side of a CLIENT's paired takedown on a host-owned NPC.
// Take 1248: the host's guard never learned of the client's choke and walked on
// (~6 m), and the client's replica, knocked out natively, retired its channel.
// PairedHold (every 200 ms during the client's scene) pauses the NPC's AI with
// the existing native AI lease while the client's partner is within 3 m; the
// hold lapses 1.5 s after the last request (cancel or breakout). PairedKnockout
// at the client's choke_win releases the hold and applies the game's own
// sleep_damage (drowsy collapse, probe 1251b), so the host's body ends
// unconscious next to where the client's lies. A lethal (5) only holds: its
// death rides the existing assassination damage relay and the corpse stream.
constexpr char kNpcPairedKnockoutDecl[]="components/base/damage/sleep_damage.damage";
bool npcPairedPartnerNear(const NpcAuthorityChannel& c,float range){
    float npc[3]{};if(!memoryRead(c.lease.binding.npc.actor+0x300,npc,12))return false;
    float d=0;for(unsigned i=0;i<3;++i){const float x=npc[i]-remote.position[i];d+=x*x;}return d<=range*range;
}
void hostNpcPaired(NpcAuthorityChannel& c,const CoopNpcControl& p,ULONGLONG tick){
    if(!c.ready||c.corpse||c.unconscious||!npcPairedVariant(p.h.reserved)){++npcPairedRefused;c.pairedResult="host_paired_refused_state";return;}
    if(!npcPairedPartnerNear(c,3.f)){++npcPairedRefused;c.pairedResult="host_paired_refused_distance";return;}
    c.pairedVariant=p.h.reserved;c.pairedUntil=tick+1500;
    if(p.h.kind==NpcAuthorityMessage::PairedHold){
        if(!c.hostPairedLease){
            if(acquireNpcAiLease(c.lease)){c.hostPairedLease=true;++npcPairedHolds;c.pairedResult="host_paired_hold";}
            else{releaseNpcAiLease(c.lease);++npcPairedRefused;c.pairedResult="host_paired_hold_refused";}
        }
        return;
    }
    if(c.pairedVariant>3)return; // a lethal is never a knockout
    if(c.hostPairedLease){releaseNpcAiLease(c.lease);c.hostPairedLease=false;}
    NativeHealth before{},after{};
    if(!readNativeHealth(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),c.lease.binding.npc.handle,before)||before.actor!=c.lease.binding.npc.actor){
        ++npcPairedRefused;c.pairedResult="host_knockout_health_refused";return;}
    const auto avatar=shadowDamageIdentity();
    const unsigned attacker=validCoopDamageAvatar(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),avatar)?avatar.handle:0x3fff0010u;
    if(!applyNativeKnockout(before,attacker,after,kNpcPairedKnockoutDecl)||!nativeNpcUnconscious(after)){++npcPairedRefused;c.pairedResult="host_knockout_refused";return;}
    c.unconscious=true;++npcPairedKnockouts;c.pairedResult="host_knockout_applied";c.result="host_unconscious";
}
void serviceHostNpcPaired(NpcAuthorityChannel& c,ULONGLONG tick){
    if(c.hostPairedLease&&tick>=c.pairedUntil){releaseNpcAiLease(c.lease);c.hostPairedLease=false;c.pairedResult="host_paired_released";}
}
// 1213v: the host's NPC is natively unconscious (a host choke, or the host's
// knockout for a client choke). The client's replica follows the host's body by
// the pose stream, but stayed natively conscious (take 1248b host choke): knock it
// out silently (no reaction - the stream drives the visible body).
std::uint64_t npcClientUnconsciousApplied=0,npcClientUnconsciousRefused=0;
void clientNpcUnconscious(NpcAuthorityChannel& c){
    if(c.unconscious||c.corpse)return;
    NativeHealth before{},after{};
    if(!readNativeHealth(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),c.lease.binding.npc.handle,before)||before.actor!=c.lease.binding.npc.actor){++npcClientUnconsciousRefused;return;}
    if(nativeNpcUnconscious(before)){c.unconscious=true;c.result="client_unconscious";return;}
    if(!applyNativeKnockout(before,0x3fff0010u,after,kNpcKnockoutDecl)||!nativeNpcUnconscious(after)){++npcClientUnconsciousRefused;c.pairedResult="client_unconscious_refused";return;}
    c.unconscious=true;++npcClientUnconsciousApplied;c.result="client_unconscious";c.pairedResult="client_unconscious_from_host";
}
