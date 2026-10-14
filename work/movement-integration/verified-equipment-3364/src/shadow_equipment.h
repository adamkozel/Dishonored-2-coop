#pragma once
// Local, finite native clone equipment experiment. The same existing sword
// stays in its inventory. No new equipment, damage, attack or pose calls.
struct ShadowEquipmentLease {
    NativeSwordEquipment before{};std::uint64_t epoch=0,peerEpoch=0,session=0;
    ULONGLONG until=0,restoreUntil=0;bool observed=false,restoreRequested=false;
    const char* stopReason="none";
};
ShadowEquipmentLease shadowEquipment;
std::uint64_t shadowEquipmentStarted=0,shadowEquipmentObserved=0,shadowEquipmentRestored=0,shadowEquipmentRefused=0,shadowEquipmentLost=0,shadowEquipmentRetired=0;
const char* shadowEquipmentResult="idle";
bool shadowEquipmentObjects(const NativeSwordEquipment& a,const NativeSwordEquipment& b){
    return a.game==b.game&&a.actor==b.actor&&a.handle==b.handle&&a.inventory==b.inventory&&a.items==b.items&&a.equipments==b.equipments&&
        a.entry==b.entry&&a.itemDeclaration==b.itemDeclaration&&a.entity==b.entity&&a.weaponHandle==b.weaponHandle&&a.component==b.component&&
        a.declaration==b.declaration&&a.frame==b.frame&&a.parent==b.parent;
}
bool shadowEquipmentRead(NativeSwordEquipment& s){
    const auto& b=shadowEquipment.before;
    return GetCurrentThreadId()==engineThread.load()&&guardEquipmentApiMatches()&&shadowEquipment.epoch==outgoing.worldEpoch&&observedGame==b.game&&
        readNativeSwordEquipment(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),b.handle,s,localRole?NativeSwordScope::DoppelgangerCorvo:NativeSwordScope::DoppelgangerEmily)&&
        shadowEquipmentObjects(s,b);
}
bool shadowEquipmentAiReady(){
    NativeAiComponentState decision{},attack{};NativeHealth health{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    const bool controlled=avatarAiHeld&&avatarAiTarget.actor==probeActor&&
        readNativeAiComponent(memoryRead,base,observedGame,probeHandle,NativeAiKind::Decision,decision)&&decision.component==avatarAiTarget.component&&
        (decision.flags&0xc)==8&&((decision.flags>>6)&0x1fffff)&&((decision.flags>>4)&3)==2&&!decision.schedulerActive&&
        readNativeHealth(memoryRead,base,probeHandle,health)&&health.logic==1&&health.current==40;
    if(!controlled)return false;
    // The isolated definitions currently omit AIAttack. Prove absence from
    // the registered list instead of treating any failed reader as absence.
    unsigned found=0;
    for(unsigned i=0;i<(health.componentCount&0xffffff);++i){std::uintptr_t c=0,vt=0;
        if(!memoryRead(health.list+i*8,&c,8)||!c||!memoryRead(c,&vt,8))return false;
        if(vt==base+nativeAiVtable(NativeAiKind::Attack))++found;
    }
    if(found&&(found!=1||!readNativeAiComponent(memoryRead,base,observedGame,probeHandle,NativeAiKind::Attack,attack)||(attack.flags&0xc)||attack.schedulerActive))return false;
    std::uintptr_t list=0;unsigned count=0;
    return memoryRead(health.actor+0x268,&list,8)&&list==health.list&&memoryRead(health.actor+0x270,&count,4)&&count==health.componentCount;
}
void stopShadowEquipment(const char* reason){
    if(!shadowEquipment.until)return;
    shadowEquipment.until=0;shadowEquipment.restoreUntil=GetTickCount64()+1000;shadowEquipmentResult=reason;shadowEquipment.stopReason=reason;
}
void beginShadowEquipment(){
    const auto tick=GetTickCount64();const auto refuse=[&](const char* reason){++shadowEquipmentRefused;shadowEquipmentResult=reason;};
    if(shadowEquipment.until||shadowEquipment.restoreUntil){refuse("duplicate_or_restoring");return;}
    if(shadowStream.until||shadowAction.actor||bodyStreamEnabled||movementBankActive()||avatarSword.until||avatarWeapon.until||
        !shadowStreamPairReady(tick)||!guardEquipmentApiMatches()||!shadowEquipmentAiReady()){refuse("unsupported_scope");return;}
    NativeSwordEquipment s{};
    if(!readNativeSwordEquipment(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),probeHandle,s,localRole?NativeSwordScope::DoppelgangerCorvo:NativeSwordScope::DoppelgangerEmily)||
        s.actor!=probeActor||s.logic!=1||!s.equipped||!s.started||s.attachedTag!=s.drawTag){refuse("not_native_drawn");return;}
    shadowEquipment={s,outgoing.worldEpoch,remote.worldEpoch,remote.session,tick+2500};
    reinterpret_cast<void(*)(void*,void*)>(gameImage+0x619640)(reinterpret_cast<void*>(s.inventory),reinterpret_cast<void*>(s.entry));
    ++shadowEquipmentStarted;shadowEquipmentResult="holster_requested";
}
void serviceShadowEquipment(ULONGLONG tick){
    if(shadowEquipment.until){
        if(tick>=shadowEquipment.until||!shadowStreamPairReady(tick)||!shadowEquipmentAiReady()||probeActor!=shadowEquipment.before.actor||
            remote.worldEpoch!=shadowEquipment.peerEpoch||remote.session!=shadowEquipment.session||shadowEquipment.epoch!=outgoing.worldEpoch||
            shadowStream.until||shadowAction.actor||bodyStreamEnabled||movementBankActive())stopShadowEquipment(tick>=shadowEquipment.until?"expired":"scope_changed");
        else{NativeSwordEquipment s{};
            // Start/Stop is queued by the inventory routine. The component
            // legitimately crosses an intermediate lifecycle value for a
            // frame; do not interpret that failed strict read as a new actor.
            if(!shadowEquipmentRead(s)){if(tick+2250<shadowEquipment.until)return;stopShadowEquipment("equipment_identity_changed");}
            else if(!s.equipped&&!s.started&&s.attachedTag==s.holsterTag&&!shadowEquipment.observed){
                shadowEquipment.observed=true;++shadowEquipmentObserved;shadowEquipmentResult="native_holstered";
            }
        }
    }
    if(!shadowEquipment.restoreUntil)return;
    if(shadowEquipment.epoch!=outgoing.worldEpoch||observedGame!=shadowEquipment.before.game){
        ++shadowEquipmentRetired;shadowEquipment.restoreUntil=0;shadowEquipmentResult="world_retired";return;
    }
    NativeSwordEquipment s{};const bool valid=shadowEquipmentRead(s);
    if(valid&&s.equipped&&s.started&&s.attachedTag==s.drawTag){++shadowEquipmentRestored;shadowEquipment.restoreUntil=0;shadowEquipmentResult="native_draw_restored";}
    else if(valid&&s.logic==1&&!s.equipped&&!s.started&&!shadowEquipment.restoreRequested){
        reinterpret_cast<void(*)(void*,void*,bool)>(gameImage+0x614b20)(reinterpret_cast<void*>(s.inventory),reinterpret_cast<void*>(s.entry),true);
        shadowEquipment.restoreRequested=true;shadowEquipment.restoreUntil=tick+1000;
    }else if(outgoing.gameLogicState==2||outgoing.gameLogicState==13||outgoing.pendingGameLogicState==2||outgoing.pendingGameLogicState==13){shadowEquipment.restoreUntil=tick+1000;}
    else if(tick>=shadowEquipment.restoreUntil){++shadowEquipmentLost;shadowEquipment.restoreUntil=0;shadowEquipmentResult="restore_unconfirmed";}
}
void writeShadowEquipment(ULONGLONG tick){
    char path[MAX_PATH]{},temp[MAX_PATH]{};if(std::snprintf(path,sizeof(path),"%s.shadow-equipment.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\"localExperiment\":true,\"stopReason\":\"%s\",\"active\":%s,\"restoring\":%s,\"remainingMs\":%llu,\"result\":\"%s\",\"started\":%llu,\"observed\":%llu,\"restored\":%llu,\"lost\":%llu,\"retired\":%llu,\"refused\":%llu,\"handle\":%u,\"weaponHandle\":%u}\n",
        shadowEquipment.stopReason,shadowEquipment.until?"true":"false",shadowEquipment.restoreUntil?"true":"false",shadowEquipment.until>tick?shadowEquipment.until-tick:0,shadowEquipmentResult,
        shadowEquipmentStarted,shadowEquipmentObserved,shadowEquipmentRestored,shadowEquipmentLost,shadowEquipmentRetired,shadowEquipmentRefused,shadowEquipment.before.handle,shadowEquipment.before.weaponHandle);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
