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
    // 1213x: distance gates only the start of a hold; the held NPC then follows the
    // client's victim pose (PairedPose), which the native choke drags with the attacker.
    if(!c.hostPairedLease&&!npcPairedPartnerNear(c,3.f)){++npcPairedRefused;c.pairedResult="host_paired_refused_distance";return;}
    c.pairedVariant=p.h.reserved;c.pairedUntil=tick+1500;
    if(p.h.kind==NpcAuthorityMessage::PairedHold){
        if(!c.hostPairedLease){
            if(acquireNpcAiLease(c.lease)){c.hostPairedLease=true;++npcPairedHolds;c.pairedResult="host_paired_hold";}
            else{releaseNpcAiLease(c.lease);++npcPairedRefused;c.pairedResult="host_paired_hold_refused";}
        }
        return;
    }
    if(!npcPairedKnockoutVariant(c.pairedVariant))return; // only a knockout scene knocks out
    if(c.hostPairedLease){releaseNpcAiLease(c.lease);c.hostPairedLease=false;}
    NativeHealth before{},after{};
    if(!readNativeHealth(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),c.lease.binding.npc.handle,before)||before.actor!=c.lease.binding.npc.actor){
        ++npcPairedRefused;c.pairedResult="host_knockout_health_refused";return;}
    const auto avatar=shadowDamageIdentity();
    const unsigned attacker=validCoopDamageAvatar(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),avatar)?avatar.handle:0x3fff0010u;
    if(!applyNativeKnockout(before,attacker,after,kNpcPairedKnockoutDecl)||!nativeNpcUnconscious(after)){++npcPairedRefused;c.pairedResult="host_knockout_refused";return;}
    c.unconscious=true;c.unconsciousAt=tick;++npcPairedKnockouts;c.pairedResult="host_knockout_applied";c.result="host_unconscious";
}
// 1214a: after the knockout the held NPC is pinned for 3 s at the client victim's
// last root while its native collapse plays. Take 1248i: when the victim pose
// stream ended at the knockout, the host NPC's pending walk carried it ~3.8 m.
std::uint64_t npcPairedPins=0;
void serviceHostNpcPaired(NpcAuthorityChannel& c,ULONGLONG tick){
    if(c.hostPairedLease&&tick>=c.pairedUntil){releaseNpcAiLease(c.lease);c.hostPairedLease=false;c.pairedResult="host_paired_released";}
    if(c.unconscious&&c.reversePoses&&c.unconsciousAt&&tick-c.unconsciousAt<3000&&(!c.reversePoseAt||tick-c.reversePoseAt>=350)&&nativeMutationAllowed){
        float transform[12]{};std::memcpy(transform,c.reverseRoot,12);
        if(!memoryRead(c.lease.binding.npc.actor+0x30c,transform+3,36))return;
        reinterpret_cast<void(*)(void*,const float*,const float*,bool,int)>(gameImage+0x87d200)(reinterpret_cast<void*>(c.lease.binding.npc.actor),transform,transform+3,true,0);++npcPairedPins;
    }
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
