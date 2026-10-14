#pragma once
// A local, two-second native equip/restore experiment. It sends no command
// over Steam and cannot select arbitrary entities or inventory items.
struct GuardEquipmentLease {
    NativeSwordEquipment before{};
    std::uint64_t epoch=0,binding=0;
    ULONGLONG until=0,restoreUntil=0;
    bool observed=false,releaseRequested=false,streamed=false;
};
GuardEquipmentLease guardEquipment{};
std::uint64_t guardEquipmentStarted=0,guardEquipmentObserved=0,guardEquipmentReleased=0,guardEquipmentRestored=0,guardEquipmentRetired=0,guardEquipmentRefused=0,guardEquipmentLost=0;
const char* guardEquipmentResult="idle";
bool guardEquipmentApiMatches(){
    const unsigned char equip[]={0x40,0x55,0x57,0x48,0x83,0xec,0x28,0x83,0x7a,0x20,0x00,0x48,0x8b,0xfa,0x48,0x8b,0xe9};
    const unsigned char unequip[]={0x48,0x89,0x74,0x24,0x20,0x57,0x48,0x83,0xec,0x20,0x48,0x8b,0xf1,0x8b,0x4a,0x08};
    return gameImage&&!std::memcmp(gameImage+0x614b20,equip,sizeof(equip))&&!std::memcmp(gameImage+0x619640,unequip,sizeof(unequip));
}
bool guardEquipmentSameObjects(const NativeSwordEquipment& s){
    const auto& b=guardEquipment.before;
    return s.game==b.game&&s.actor==b.actor&&s.inventory==b.inventory&&s.items==b.items&&s.entry==b.entry&&s.itemDeclaration==b.itemDeclaration&&
        s.entity==b.entity&&s.component==b.component&&s.declaration==b.declaration&&s.frame==b.frame&&s.weaponHandle==b.weaponHandle&&s.parent==b.parent;
}
bool guardEquipmentRead(NativeSwordEquipment& s){
    return GetCurrentThreadId()==engineThread.load()&&guardEquipmentApiMatches()&&outgoing.worldEpoch==guardEquipment.epoch&&
        observedGame==guardEquipment.before.game&&readNativeSwordEquipment(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),guardEquipment.before.handle,s)&&guardEquipmentSameObjects(s);
}
void stopGuardEquipment(const char* reason){
    if(!guardEquipment.until)return;
    guardEquipment.until=0;guardEquipmentResult=reason;
    // A pending native pause makes the strict reader temporarily unavailable.
    // Retain the identity for a bounded cleanup retry instead of forgetting
    // the still-equipped sword before the transition has completed.
    guardEquipment.restoreUntil=GetTickCount64()+1000;
    NativeSwordEquipment s{};
    const bool valid=guardEquipmentRead(s);
    if(valid&&s.equipped&&s.logic==1){
        reinterpret_cast<void(*)(void*,void*)>(gameImage+0x619640)(reinterpret_cast<void*>(s.inventory),reinterpret_cast<void*>(s.entry));
        ++guardEquipmentReleased;guardEquipment.releaseRequested=true;
    }else if(valid&&!s.equipped&&!s.started&&s.attachedTag==s.holsterTag){++guardEquipmentRestored;guardEquipment.restoreUntil=0;}
}
bool beginGuardEquipment(ULONGLONG tick,bool streamed=false,ULONGLONG expires=0){
    const auto refuse=[&](const char* reason){++guardEquipmentRefused;guardEquipmentResult=reason;return false;};
    if(guardEquipment.until||guardEquipment.restoreUntil)return refuse("duplicate_or_restoring");
    if(streamed?(!guardWeaponEnabled||expires<=tick||expires>tick+250):guardWeaponEnabled)return refuse("equipment_owner_conflict");
    if(localRole!=1||GetCurrentThreadId()!=engineThread.load()||!guardEquipmentApiMatches()||!guardPairReady(tick)||
       guardLink.phase!=GuardPhase::Active||guardLink.key.mode!=1||guardLink.deadline<=(streamed?tick:tick+2000)||!guardReadLocal()||!guardAiPaused()||!guardAttackInactive()){
        return refuse("unavailable");
    }
    NativeSwordEquipment before{};
    if(!readNativeSwordEquipment(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),guardLink.npc.handle,before)||
       before.logic!=1||before.actor!=guardLink.npc.actor||before.equipped||before.started||before.attachedTag!=before.holsterTag)return refuse("not_holstered");
    guardEquipment={before,outgoing.worldEpoch,guardLink.key.binding,streamed?expires:tick+2000,0,false,false,streamed};
    reinterpret_cast<void(*)(void*,void*,bool)>(gameImage+0x614b20)(reinterpret_cast<void*>(before.inventory),reinterpret_cast<void*>(before.entry),true);
    ++guardEquipmentStarted;guardEquipmentResult="equip_requested";return true;
}
void serviceGuardEquipment(ULONGLONG tick){
    if(guardEquipment.restoreUntil){
        NativeSwordEquipment s{};
        if(observedGame!=guardEquipment.before.game||outgoing.worldEpoch!=guardEquipment.epoch){
            // The old world cannot be read back after destruction. Retirement
            // is distinct from a confirmed native holster restoration.
            ++guardEquipmentRetired;guardEquipment.restoreUntil=0;
        }else{
            const bool valid=guardEquipmentRead(s);
            if(valid&&!s.equipped&&!s.started&&s.attachedTag==s.holsterTag){++guardEquipmentRestored;guardEquipment.restoreUntil=0;}
            else if(valid&&s.logic==1&&s.equipped&&!guardEquipment.releaseRequested){
                reinterpret_cast<void(*)(void*,void*)>(gameImage+0x619640)(reinterpret_cast<void*>(s.inventory),reinterpret_cast<void*>(s.entry));
                ++guardEquipmentReleased;guardEquipment.releaseRequested=true;guardEquipment.restoreUntil=tick+1000;
            }else if(outgoing.gameLogicState==2||outgoing.gameLogicState==13||outgoing.pendingGameLogicState==2||outgoing.pendingGameLogicState==13){
                // Native component start/stop callbacks do not advance while
                // gameplay is paused. No new mutation is issued in the menu.
                guardEquipment.restoreUntil=tick+1000;
            }else if(tick>=guardEquipment.restoreUntil){++guardEquipmentLost;guardEquipment.restoreUntil=0;guardEquipmentResult="restore_unconfirmed";}
        }
    }
    if(!guardEquipment.until)return;
    if(tick>=guardEquipment.until){stopGuardEquipment("expired");return;}
    if(!guardPairReady(tick)||guardLink.phase!=GuardPhase::Active||guardLink.key.mode!=1||guardLink.key.binding!=guardEquipment.binding||
       !guardReadLocal()||!guardAiPaused()||!guardAttackInactive()){
        stopGuardEquipment("scope_changed");return;
    }
    NativeSwordEquipment s{};
    if(guardEquipmentRead(s)&&s.equipped&&s.started&&s.attachedTag==s.drawTag){
        if(!guardEquipment.observed){guardEquipment.observed=true;++guardEquipmentObserved;guardEquipmentResult="native_drawn";}
    }
}
void writeGuardEquipment(ULONGLONG tick){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.guard-equipment.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\"localExperiment\":%s,\"active\":%s,\"restoring\":%s,\"remainingMs\":%llu,\"result\":\"%s\",\"started\":%llu,\"observed\":%llu,\"released\":%llu,\"restored\":%llu,\"retired\":%llu,\"refused\":%llu,\"lost\":%llu,\"handle\":%u,\"weaponHandle\":%u,\"binding\":%llu}\n",
        guardEquipment.streamed?"false":"true",guardEquipment.until?"true":"false",guardEquipment.restoreUntil?"true":"false",guardEquipment.until>tick?guardEquipment.until-tick:0,guardEquipmentResult,
        guardEquipmentStarted,guardEquipmentObserved,guardEquipmentReleased,guardEquipmentRestored,guardEquipmentRetired,guardEquipmentRefused,guardEquipmentLost,
        guardEquipment.before.handle,guardEquipment.before.weaponHandle,guardEquipment.binding);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
