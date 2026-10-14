#pragma once
// Two-second local Corvo equipment experiment. Uses his existing authored
// tutorial sword; does not create inventory or send a network equip request.
struct AvatarSwordLease {NativeSwordEquipment before{};std::uint64_t epoch=0,peerEpoch=0,peerSession=0;ULONGLONG until=0,restoreUntil=0;bool observed=false,releaseRequested=false;};
AvatarSwordLease avatarSword{};
std::uint64_t avatarSwordStarted=0,avatarSwordObserved=0,avatarSwordReleased=0,avatarSwordRestored=0,avatarSwordRetired=0,avatarSwordLost=0,avatarSwordRefused=0;
const char* avatarSwordResult="idle";
bool avatarSwordSame(const NativeSwordEquipment& s){
    const auto& b=avatarSword.before;
    return s.game==b.game&&s.actor==b.actor&&s.inventory==b.inventory&&s.items==b.items&&s.entry==b.entry&&s.itemDeclaration==b.itemDeclaration&&
        s.entity==b.entity&&s.component==b.component&&s.declaration==b.declaration&&s.frame==b.frame&&s.weaponHandle==b.weaponHandle&&s.parent==b.parent;
}
bool avatarSwordRead(NativeSwordEquipment& s){
    return GetCurrentThreadId()==engineThread.load()&&guardEquipmentApiMatches()&&outgoing.worldEpoch==avatarSword.epoch&&observedGame==avatarSword.before.game&&
        readNativeSwordEquipment(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),avatarSword.before.handle,s,NativeSwordScope::CorvoTutorial)&&avatarSwordSame(s);
}
bool avatarSwordReady(ULONGLONG tick){
    if(localRole!=1||!probeFollowExact||!armProbePairReady(tick)||!avatarAiHeld||avatarAiTarget.actor!=probeActor)return false;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);NativeAiDecisionState ai{};NativeHealth health{};
    if(!readNativeAiDecision(memoryRead,base,observedGame,probeHandle,ai)||ai.actor!=probeActor||ai.component!=avatarAiTarget.component||
       (ai.flags&0xc)!=8||ai.schedulerActive||!((ai.flags>>6)&0x1fffff)||((ai.flags>>4)&3)!=2||
       !readNativeHealth(memoryRead,base,probeHandle,health)||health.current!=1)return false;
    // This exact story Corvo has no native AIAttack component. Do not equate
    // a failed attack-state read with absence: inspect the registered list.
    for(unsigned i=0;i<(health.componentCount&0xffffff);++i){std::uintptr_t c=0,vt=0;
        if(!memoryRead(health.list+i*8,&c,8)||!c||!memoryRead(c,&vt,8)||vt==base+nativeAiVtable(NativeAiKind::Attack))return false;
    }
    return true;
}
void stopAvatarSword(const char* reason){
    if(!avatarSword.until)return;avatarSword.until=0;avatarSwordResult=reason;avatarSword.restoreUntil=GetTickCount64()+1000;
    NativeSwordEquipment s{};
    if(avatarSwordRead(s)){
        if(s.equipped&&s.logic==1){reinterpret_cast<void(*)(void*,void*)>(gameImage+0x619640)(reinterpret_cast<void*>(s.inventory),reinterpret_cast<void*>(s.entry));++avatarSwordReleased;avatarSword.releaseRequested=true;}
        else if(!s.equipped&&!s.started&&s.attachedTag==s.holsterTag){++avatarSwordRestored;avatarSword.restoreUntil=0;}
    }
}
void beginAvatarSword(){
    const auto tick=GetTickCount64();const auto refuse=[&](const char* reason){++avatarSwordRefused;avatarSwordResult=reason;};
    if(avatarSword.until||avatarSword.restoreUntil){refuse("duplicate_or_restoring");return;}
    if(GetCurrentThreadId()!=engineThread.load()||!guardEquipmentApiMatches()||!avatarSwordReady(tick)){refuse("unsupported_scope");return;}
    NativeSwordEquipment s{};
    if(!readNativeSwordEquipment(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),probeHandle,s,NativeSwordScope::CorvoTutorial)||
       s.logic!=1||s.actor!=probeActor||s.equipped||s.started||s.attachedTag!=s.holsterTag){refuse("not_native_holster");return;}
    avatarSword={s,outgoing.worldEpoch,remote.worldEpoch,remote.session,tick+2000,0,false,false};
    reinterpret_cast<void(*)(void*,void*,bool)>(gameImage+0x614b20)(reinterpret_cast<void*>(s.inventory),reinterpret_cast<void*>(s.entry),true);
    ++avatarSwordStarted;avatarSwordResult="equip_requested";
}
void serviceAvatarSword(ULONGLONG tick){
    if(avatarSword.restoreUntil){
        if(observedGame!=avatarSword.before.game||outgoing.worldEpoch!=avatarSword.epoch){++avatarSwordRetired;avatarSword.restoreUntil=0;avatarSwordResult="world_retired";}
        else{
            NativeSwordEquipment s{};const bool valid=avatarSwordRead(s);
            if(valid&&!s.equipped&&!s.started&&s.attachedTag==s.holsterTag){++avatarSwordRestored;avatarSword.restoreUntil=0;}
            else if(valid&&s.logic==1&&s.equipped&&!avatarSword.releaseRequested){
                reinterpret_cast<void(*)(void*,void*)>(gameImage+0x619640)(reinterpret_cast<void*>(s.inventory),reinterpret_cast<void*>(s.entry));
                ++avatarSwordReleased;avatarSword.releaseRequested=true;avatarSword.restoreUntil=tick+1000;
            }else if(outgoing.gameLogicState==2||outgoing.gameLogicState==13||outgoing.pendingGameLogicState==2||outgoing.pendingGameLogicState==13){avatarSword.restoreUntil=tick+1000;}
            else if(tick>=avatarSword.restoreUntil){++avatarSwordLost;avatarSword.restoreUntil=0;avatarSwordResult="restore_unconfirmed";}
        }
    }
    if(!avatarSword.until)return;
    if(tick>=avatarSword.until||!avatarSwordReady(tick)||outgoing.worldEpoch!=avatarSword.epoch||remote.worldEpoch!=avatarSword.peerEpoch||remote.session!=avatarSword.peerSession||probeActor!=avatarSword.before.actor){
        stopAvatarSword(tick>=avatarSword.until?"expired":"scope_changed");return;
    }
    NativeSwordEquipment s{};
    if(avatarSwordRead(s)&&s.equipped&&s.started&&s.attachedTag==s.drawTag&&!avatarSword.observed){avatarSword.observed=true;++avatarSwordObserved;avatarSwordResult="native_drawn";}
}
void writeAvatarSword(ULONGLONG tick){
    char path[MAX_PATH]{},temp[MAX_PATH]{};if(std::snprintf(path,sizeof(path),"%s.avatar-sword.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\"localExperiment\":true,\"childrenUpdated\":%llu,\"childrenRefused\":%llu,\"active\":%s,\"restoring\":%s,\"remainingMs\":%llu,\"result\":\"%s\",\"started\":%llu,\"observed\":%llu,\"released\":%llu,\"restored\":%llu,\"retired\":%llu,\"lost\":%llu,\"refused\":%llu,\"epoch\":%llu,\"handle\":%u,\"weaponHandle\":%u}\n",
        avatarChildrenUpdated.load(),avatarChildrenRefused.load(),avatarSword.until?"true":"false",avatarSword.restoreUntil?"true":"false",avatarSword.until>tick?avatarSword.until-tick:0,avatarSwordResult,
        avatarSwordStarted,avatarSwordObserved,avatarSwordReleased,avatarSwordRestored,avatarSwordRetired,avatarSwordLost,avatarSwordRefused,avatarSword.epoch,avatarSword.before.handle,avatarSword.before.weaponHandle);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}

