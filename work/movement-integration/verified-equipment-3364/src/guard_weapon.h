#pragma once
// Only an existing, locally validated sword is selected. Native inventory
// callbacks own attachment changes; no item pointers or weapon handles travel
// over Steam. Both peers explicitly enable this fixed-guard experiment.
struct GuardWeaponLease {
    ULONGLONG until=0;
    std::uint64_t binding=0,sequence=0,rootSequence=0;
    bool drawn=false;
};
GuardWeaponLease guardWeapon{};
ULONGLONG guardWeaponLastSend=0;
std::uint64_t guardWeaponSequence=0,guardWeaponSent=0,guardWeaponReceived=0,guardWeaponRejected=0,guardWeaponAccepted=0;
std::uint64_t guardWeaponLastSession=0,guardWeaponLastSequence=0,guardWeaponDrawSamples=0,guardWeaponHolsterSamples=0;
const char* guardWeaponResult="idle";
void stopGuardWeapon(const char* reason){
    guardWeapon.until=0;
    if(guardEquipment.streamed)stopGuardEquipment(reason);
    guardWeaponResult=reason;
}
void sendGuardWeapon(ULONGLONG tick){
    if(!guardWeaponEnabled||localRole||tick-guardWeaponLastSend<90||guardLink.phase!=GuardPhase::Active||guardLink.key.mode!=1||!guardReadLocal())return;
    guardWeaponLastSend=tick;
    NativeSwordEquipment s{};
    if(!readNativeSwordEquipment(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),guardLink.npc.handle,s)||
       s.logic!=1||s.game!=guardLink.game||s.actor!=guardLink.npc.actor){++guardWeaponRejected;return;}
    const bool drawn=s.equipped&&s.started&&s.attachedTag==s.drawTag;
    const bool holstered=!s.equipped&&!s.started&&s.attachedTag==s.holsterTag;
    if(!drawn&&!holstered){++guardWeaponRejected;return;}
    CoopGuardWeaponPacket p{};p.binding=guardLastPose;p.rootSequence=p.binding.sequence;p.binding.sequence=++guardWeaponSequence;
    p.item=coopMapId(kGuardSwordItem);p.drawn=drawn?1:0;
    if(!validGuardWeapon(p,remote,outgoing)||!guardSameBinding(p.binding,guardLink.key)){++guardWeaponRejected;return;}
    if(sockets->SendMessageToConnection(connection,&p,sizeof(p),k_nSteamNetworkingSend_UnreliableNoNagle,nullptr)==k_EResultOK){
        ++guardWeaponSent;if(drawn)++guardWeaponDrawSamples;else ++guardWeaponHolsterSamples;
    }else ++guardWeaponRejected;
}
void receiveGuardWeapon(const CoopGuardWeaponPacket& p,ULONGLONG tick){
    ++guardWeaponReceived;
    if(!guardWeaponEnabled||!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load()||!guardPairReady(tick)||
       guardLink.phase!=GuardPhase::Active||guardLink.key.mode!=1||tick>=guardLink.deadline||tick-guardLink.poseTick>=250||
       !validGuardWeapon(p,outgoing,remote)||!guardSameBinding(p.binding,guardLink.key)||!guardWeaponRootFresh(p.rootSequence,guardLink.pose.sequence)||
       !guardWeaponSequenceFresh(p,remote.session,guardWeaponLastSession,guardWeaponLastSequence)||
       !guardReadLocal()||!guardAiPaused()||!guardAttackInactive()||!guardEquipmentApiMatches()||
       (guardEquipment.until&&!guardEquipment.streamed)){++guardWeaponRejected;return;}
    guardWeaponLastSession=remote.session;guardWeaponLastSequence=p.binding.sequence;
    guardWeapon={(std::min)(tick+250,guardLink.deadline),p.binding.binding,p.binding.sequence,p.rootSequence,p.drawn!=0};
    ++guardWeaponAccepted;guardWeaponResult=p.drawn?"draw_received":"holster_received";
}
void serviceGuardWeapon(ULONGLONG tick){
    if(!guardWeapon.until)return;
    if(tick>=guardWeapon.until){stopGuardWeapon("weapon_stale");return;}
    if(!guardWeaponEnabled||!guardPairReady(tick)||guardLink.phase!=GuardPhase::Active||guardLink.key.mode!=1||
       guardLink.key.binding!=guardWeapon.binding||tick-guardLink.poseTick>=250||!guardReadLocal()||!guardAiPaused()||!guardAttackInactive()){
        stopGuardWeapon("scope_changed");return;
    }
    if(!guardWeapon.drawn){if(guardEquipment.streamed)stopGuardEquipment("host_holstered");return;}
    if(guardEquipment.restoreUntil)return;
    if(guardEquipment.until){
        if(!guardEquipment.streamed||guardEquipment.binding!=guardWeapon.binding){stopGuardWeapon("equipment_owner_conflict");return;}
        guardEquipment.until=guardWeapon.until;
    }else if(!beginGuardEquipment(tick,true,guardWeapon.until)){stopGuardWeapon("native_equip_refused");}
}
void writeGuardWeapon(ULONGLONG tick){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.guard-weapon.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\"enabled\":%s,\"active\":%s,\"drawn\":%s,\"remainingMs\":%llu,\"binding\":%llu,\"sequence\":%llu,\"rootSequence\":%llu,\"result\":\"%s\",\"sent\":%llu,\"received\":%llu,\"accepted\":%llu,\"rejected\":%llu,\"sentDrawn\":%llu,\"sentHolstered\":%llu,\"packetBytes\":196,\"protocolVersion\":1}\n",
        guardWeaponEnabled?"true":"false",guardWeapon.until>tick?"true":"false",guardWeapon.drawn?"true":"false",guardWeapon.until>tick?guardWeapon.until-tick:0,
        guardWeapon.binding,guardWeapon.sequence,guardWeapon.rootSequence,guardWeaponResult,guardWeaponSent,guardWeaponReceived,guardWeaponAccepted,guardWeaponRejected,guardWeaponDrawSamples,guardWeaponHolsterSamples);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
