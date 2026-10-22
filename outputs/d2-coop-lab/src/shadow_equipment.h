#pragma once
// Local, finite native clone equipment experiment. The same existing sword
// stays in its inventory. No new equipment, damage, attack or pose calls.
struct ShadowEquipmentLease {
    NativeSwordEquipment before{};std::uint64_t epoch=0,peerEpoch=0,session=0;
    ULONGLONG until=0,restoreUntil=0;bool observed=false,restoreRequested=false;
    const char* stopReason="none";
    bool remoteDriven=false;ULONGLONG lastRequest=0;
    bool visibilitySaved=false,hiddenBefore=false,hiddenNow=false;
};
ShadowEquipmentLease shadowEquipment;
std::uint64_t shadowEquipmentStarted=0,shadowEquipmentObserved=0,shadowEquipmentRestored=0,shadowEquipmentRefused=0,shadowEquipmentLost=0,shadowEquipmentRetired=0;
const char* shadowEquipmentResult="idle";
std::uint64_t shadowSwordHidden=0,shadowSwordShown=0,shadowSwordVisibilityRefused=0;
bool shadowSwordVisibilityApi(){
    const unsigned char hide[]={0x0f,0xb6,0x81,0xd0,0,0,0,0xa8,1,0x75,0x0d,0x0c,1};
    const unsigned char show[]={0x0f,0xb6,0x81,0xd0,0,0,0,0xa8,1,0x74,0x0d,0x24,0xfe};
    return gameImage&&!std::memcmp(gameImage+0x8784b0,hide,sizeof(hide))&&!std::memcmp(gameImage+0x87c590,show,sizeof(show));
}
// Called only with the exact validated inventory child. Native visibility
// updates its render object/children; never target the avatar or local hands.
bool shadowSwordSetHidden(const NativeSwordEquipment& s,bool hidden){
    unsigned char flags=0;
    if(GetCurrentThreadId()!=engineThread.load()||!shadowSwordVisibilityApi()||
       s.entity!=shadowEquipment.before.entity||s.weaponHandle!=shadowEquipment.before.weaponHandle||
       worldIdentityEntity(memoryRead,s.game,s.weaponHandle)!=s.entity||!memoryRead(s.entity+0xd0,&flags,1))return false;
    if(bool(flags&1)!=hidden){
        reinterpret_cast<void(*)(void*)>(gameImage+(hidden?0x8784b0:0x87c590))(reinterpret_cast<void*>(s.entity));
        if(!memoryRead(s.entity+0xd0,&flags,1)||bool(flags&1)!=hidden){++shadowSwordVisibilityRefused;return false;}
        if(hidden)++shadowSwordHidden;else ++shadowSwordShown;
    }
    shadowEquipment.hiddenNow=hidden;return true;
}
bool shadowEquipmentObjects(const NativeSwordEquipment& a,const NativeSwordEquipment& b){
    return a.game==b.game&&a.actor==b.actor&&a.handle==b.handle&&a.inventory==b.inventory&&a.items==b.items&&a.equipments==b.equipments&&
        a.entry==b.entry&&a.itemDeclaration==b.itemDeclaration&&a.entity==b.entity&&a.weaponHandle==b.weaponHandle&&a.component==b.component&&
        a.declaration==b.declaration&&a.frame==b.frame&&a.parent==b.parent;
}
bool shadowEquipmentRead(NativeSwordEquipment& s){
    const auto& b=shadowEquipment.before;
    return GetCurrentThreadId()==engineThread.load()&&guardEquipmentApiMatches()&&shadowEquipment.epoch==outgoing.worldEpoch&&observedGame==b.game&&
        readNativeSwordEquipment(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),b.handle,s,remoteIsCorvo()?NativeSwordScope::DoppelgangerCorvo:NativeSwordScope::DoppelgangerEmily)&&
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
    if(!readNativeSwordEquipment(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),probeHandle,s,remoteIsCorvo()?NativeSwordScope::DoppelgangerCorvo:NativeSwordScope::DoppelgangerEmily)||
        s.actor!=probeActor||s.logic!=1||!s.equipped||!s.started||s.attachedTag!=s.drawTag){refuse("not_native_drawn");return;}
    shadowEquipment={s,outgoing.worldEpoch,remote.worldEpoch,remote.session,tick+2500};
    reinterpret_cast<void(*)(void*,void*)>(gameImage+0x619640)(reinterpret_cast<void*>(s.inventory),reinterpret_cast<void*>(s.entry));
    ++shadowEquipmentStarted;shadowEquipmentResult="holster_requested";
}
void serviceShadowEquipment(ULONGLONG tick){
    if(shadowEquipment.until){
        if(tick>=shadowEquipment.until||!shadowStreamPairReady(tick)||!shadowEquipmentAiReady()||probeActor!=shadowEquipment.before.actor||
            remote.worldEpoch!=shadowEquipment.peerEpoch||remote.session!=shadowEquipment.session||shadowEquipment.epoch!=outgoing.worldEpoch||
            shadowAction.actor||bodyStreamEnabled||(shadowEquipment.remoteDriven?(!shadowStreamCurrent(tick)||!movementBankActive()):(shadowStream.until||movementBankActive())))stopShadowEquipment(tick>=shadowEquipment.until?"expired":"scope_changed");
        else{NativeSwordEquipment s{};
            // Start/Stop is queued by the inventory routine. The component
            // legitimately crosses an intermediate lifecycle value for a
            // frame; do not interpret that failed strict read as a new actor.
            if(!shadowEquipmentRead(s)){if(shadowEquipment.remoteDriven?(tick-shadowEquipment.lastRequest<250):(tick+2250<shadowEquipment.until))return;stopShadowEquipment("equipment_identity_changed");}
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
    const bool originallyDrawn=shadowEquipment.before.equipped;
    if(valid&&s.equipped==originallyDrawn&&s.started==originallyDrawn&&s.attachedTag==(originallyDrawn?s.drawTag:s.holsterTag)&&
       (!shadowEquipment.visibilitySaved||shadowSwordSetHidden(s,shadowEquipment.hiddenBefore))){++shadowEquipmentRestored;shadowEquipment.restoreUntil=0;shadowEquipmentResult="native_original_restored";}
    else if(valid&&s.logic==1&&s.equipped==s.started&&!shadowEquipment.restoreRequested){
        if(originallyDrawn)reinterpret_cast<void(*)(void*,void*,bool)>(gameImage+0x614b20)(reinterpret_cast<void*>(s.inventory),reinterpret_cast<void*>(s.entry),true);
        else reinterpret_cast<void(*)(void*,void*)>(gameImage+0x619640)(reinterpret_cast<void*>(s.inventory),reinterpret_cast<void*>(s.entry));
        shadowEquipment.restoreRequested=true;shadowEquipment.restoreUntil=tick+1000;
    }else if(outgoing.gameLogicState==2||outgoing.gameLogicState==13||outgoing.pendingGameLogicState==2||outgoing.pendingGameLogicState==13){shadowEquipment.restoreUntil=tick+1000;}
    else if(tick>=shadowEquipment.restoreUntil){++shadowEquipmentLost;shadowEquipment.restoreUntil=0;shadowEquipmentResult="restore_unconfirmed";}
}
// Called on the engine thread after presentation-lock release, at the same
// delayed sample used for the whole-body equipment pose. Cleanup retains the
// original equipment state and exact actor generation across pause only.
void requestMovementEquipment(bool drawn){
    const auto tick=GetTickCount64();
    if(shadowEquipment.restoreUntil||(shadowEquipment.until&&!shadowEquipment.remoteDriven)||!shadowStreamCurrent(tick)||
        !shadowStream.peerReady||tick-shadowStream.lastState>250||!guardEquipmentApiMatches()||!shadowEquipmentAiReady())return;
    NativeSwordEquipment s{};
    if(shadowEquipment.until){
        if(!shadowEquipmentRead(s))return;
        shadowEquipment.until=tick+250;
    }else{
        if(!readNativeSwordEquipment(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),probeHandle,s,remoteIsCorvo()?NativeSwordScope::DoppelgangerCorvo:NativeSwordScope::DoppelgangerEmily)||
            s.actor!=probeActor||s.logic!=1||s.equipped!=s.started)return;
        unsigned char visibility=0;
        if(!shadowSwordVisibilityApi()||!memoryRead(s.entity+0xd0,&visibility,1))return;
        if(s.equipped==drawn&&bool(visibility&1)==!drawn)return;
        shadowEquipment={s,outgoing.worldEpoch,remote.worldEpoch,remote.session,tick+250};shadowEquipment.remoteDriven=true;++shadowEquipmentStarted;
        shadowEquipment.visibilitySaved=true;shadowEquipment.hiddenBefore=shadowEquipment.hiddenNow=bool(visibility&1);
    }
    // Wait for native inventory lifecycle and the matching attachment tag.
    if(s.equipped==drawn&&s.started==drawn&&s.attachedTag==(drawn?s.drawTag:s.holsterTag)){
        if(!shadowSwordSetHidden(s,!drawn))stopShadowEquipment("visibility_identity_changed");
        return;
    }
    if(s.logic!=1||s.equipped!=s.started||s.equipped==drawn||tick-shadowEquipment.lastRequest<250)return;
    if(drawn)reinterpret_cast<void(*)(void*,void*,bool)>(gameImage+0x614b20)(reinterpret_cast<void*>(s.inventory),reinterpret_cast<void*>(s.entry),true);
    else reinterpret_cast<void(*)(void*,void*)>(gameImage+0x619640)(reinterpret_cast<void*>(s.inventory),reinterpret_cast<void*>(s.entry));
    shadowEquipment.lastRequest=tick;shadowEquipmentResult=drawn?"remote_draw_requested":"remote_holster_requested";
}
void writeShadowEquipment(ULONGLONG tick){
    char path[MAX_PATH]{},temp[MAX_PATH]{};if(std::snprintf(path,sizeof(path),"%s.shadow-equipment.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\"visibilitySaved\":%s,\"hiddenBefore\":%s,\"hiddenNow\":%s,\"nativeHideCalls\":%llu,\"nativeShowCalls\":%llu,\"visibilityRefused\":%llu,",
        shadowEquipment.visibilitySaved?"true":"false",shadowEquipment.hiddenBefore?"true":"false",shadowEquipment.hiddenNow?"true":"false",shadowSwordHidden,shadowSwordShown,shadowSwordVisibilityRefused);
    fprintf(f,"\"localExperiment\":%s,\"remoteDriven\":%s,\"stopReason\":\"%s\",\"active\":%s,\"restoring\":%s,\"remainingMs\":%llu,\"result\":\"%s\",\"started\":%llu,\"observed\":%llu,\"restored\":%llu,\"lost\":%llu,\"retired\":%llu,\"refused\":%llu,\"handle\":%u,\"weaponHandle\":%u}\n",
        shadowEquipment.remoteDriven?"false":"true",shadowEquipment.remoteDriven?"true":"false",shadowEquipment.stopReason,shadowEquipment.until?"true":"false",shadowEquipment.restoreUntil?"true":"false",shadowEquipment.until>tick?shadowEquipment.until-tick:0,shadowEquipmentResult,
        shadowEquipmentStarted,shadowEquipmentObserved,shadowEquipmentRestored,shadowEquipmentLost,shadowEquipmentRetired,shadowEquipmentRefused,shadowEquipment.before.handle,shadowEquipment.before.weaponHandle);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
