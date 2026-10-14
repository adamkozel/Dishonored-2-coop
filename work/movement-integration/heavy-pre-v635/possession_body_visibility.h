#pragma once
// Finite cosmetic lease on our exact Corvo clone. Native local player,
// possessed NPC, collisions, AI and gameplay are never mutated here.
struct PossessionBodyVisibility {
    NativeHealth before{};
    std::uint64_t epoch=0,peerEpoch=0,session=0,token=0,peerToken=0;
    ULONGLONG until=0,restoreUntil=0;
    bool hidden=false;
} possessionBody;
std::uint64_t possessionBodyHides=0,possessionBodyShows=0,possessionBodyRefused=0,possessionBodyRetired=0,possessionBodyLost=0;
const char* possessionBodyResult="idle";
bool possessionBodyIdentity(NativeHealth& h){
    return GetCurrentThreadId()==engineThread.load()&&observedGame==possessionBody.before.game&&outgoing.worldEpoch==possessionBody.epoch&&
        readNativeHealth(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),possessionBody.before.handle,h)&&
        h.actor==possessionBody.before.actor&&h.component==possessionBody.before.component&&h.declaration==possessionBody.before.declaration;
}
bool possessionBodySetHidden(const NativeHealth& h,bool hidden){
    unsigned char flags=0;
    if(!shadowSwordVisibilityApi()||h.handle==1||worldIdentityEntity(memoryRead,h.game,h.handle)!=h.actor||!memoryRead(h.actor+0xd0,&flags,1))return false;
    if(bool(flags&1)!=hidden){
        reinterpret_cast<void(*)(void*)>(gameImage+(hidden?0x8784b0:0x87c590))(reinterpret_cast<void*>(h.actor));
        if(!memoryRead(h.actor+0xd0,&flags,1)||bool(flags&1)!=hidden)return false;
        if(hidden)++possessionBodyHides;else ++possessionBodyShows;
    }
    possessionBody.hidden=hidden;return true;
}
void retirePossessionBodyVisibility(const char* reason){
    if(!possessionBody.until)return;
    possessionBody.until=0;possessionBody.restoreUntil=GetTickCount64()+5000;possessionBodyResult=reason;
}
void requestPossessionBodyVisibility(bool hide){
    const auto tick=GetTickCount64();
    if(!hide){retirePossessionBodyVisibility("native_context_ended");return;}
    if(localRole!=1||!shadowStreamCurrent(tick)||!shadowStream.peerReady||tick-shadowStream.lastState>=250||
       !movementBankActive()||!shadowEquipmentAiReady()||possessionBody.restoreUntil)return;
    NativeHealth h{};
    if(possessionBody.until){
        if(possessionBody.token!=shadowStream.token||possessionBody.peerToken!=shadowStream.peerToken||
           possessionBody.peerEpoch!=remote.worldEpoch||possessionBody.session!=remote.session||!possessionBodyIdentity(h)){
            retirePossessionBodyVisibility("identity_or_peer_changed");return;
        }
    }else{
        NativeSwordEquipment sword{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);unsigned char flags=0;
        if(!readNativeHealth(memoryRead,base,probeHandle,h)||h.actor!=probeActor||h.logic!=1||h.current!=40||
           !readNativeSwordEquipment(memoryRead,base,probeHandle,sword,NativeSwordScope::DoppelgangerCorvo)||sword.actor!=h.actor||
           !memoryRead(h.actor+0xd0,&flags,1)||(flags&1)||!shadowSwordVisibilityApi()){
            ++possessionBodyRefused;possessionBodyResult="clone_unavailable_or_already_hidden";return;
        }
        possessionBody={h,outgoing.worldEpoch,remote.worldEpoch,remote.session,shadowStream.token,shadowStream.peerToken,tick+250};
    }
    if(!possessionBodySetHidden(h,true)){++possessionBodyRefused;retirePossessionBodyVisibility("hide_unconfirmed");return;}
    possessionBody.until=tick+250;possessionBodyResult="native_corvo_clone_hidden";
}
void servicePossessionBodyVisibility(ULONGLONG tick){
    if(possessionBody.until&&(tick>=possessionBody.until||!shadowStreamCurrent(tick)||!movementBankActive()||
       possessionBody.token!=shadowStream.token||possessionBody.peerToken!=shadowStream.peerToken||
       possessionBody.epoch!=outgoing.worldEpoch||possessionBody.peerEpoch!=remote.worldEpoch||possessionBody.session!=remote.session))
        retirePossessionBodyVisibility("scope_or_freshness_ended");
    if(!possessionBody.restoreUntil)return;
    if(observedGame!=possessionBody.before.game||outgoing.worldEpoch!=possessionBody.epoch){
        ++possessionBodyRetired;possessionBody={};possessionBodyResult="world_retired";return;
    }
    NativeHealth h{};
    if(possessionBodyIdentity(h)&&possessionBodySetHidden(h,false)){
        possessionBody={};possessionBodyResult="original_visibility_restored";
    }else if(tick>=possessionBody.restoreUntil){
        ++possessionBodyLost;possessionBody={};possessionBodyResult="restore_unconfirmed_checkpoint_required";
    }
}
void writePossessionBodyVisibility(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.possession-body.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\"cosmeticOnly\":true,\"active\":%s,\"restoring\":%s,\"hidden\":%s,\"handle\":%u,\"hideCalls\":%llu,\"showCalls\":%llu,\"refused\":%llu,\"retired\":%llu,\"lost\":%llu,\"result\":\"%s\"}\n",
        possessionBody.until?"true":"false",possessionBody.restoreUntil?"true":"false",possessionBody.hidden?"true":"false",possessionBody.before.handle,
        possessionBodyHides,possessionBodyShows,possessionBodyRefused,possessionBodyRetired,possessionBodyLost,possessionBodyResult);
    const bool ok=!ferror(f);const auto closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
