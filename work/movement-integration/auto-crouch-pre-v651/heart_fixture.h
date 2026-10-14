#pragma once
#include "native_player_heart.h"
// Isolated equip-only observation lease. Native inventory callbacks perform
// acquisition, equip and rollback. This service never cooks, fires or spawns.
struct HeartFixture {
    NativePlayerHeart before{};std::uint64_t epoch=0;
    ULONGLONG until=0,nextRead=0,restoreUntil=0;
    bool restoring=false,equipSent=false,unequipSent=false,quantitySent=false,observed=false;
} heartFixture;
unsigned heartFixtureRequest=0;
std::uint64_t heartFixtureStarted=0,heartFixtureObserved=0,heartFixtureRestored=0,heartFixtureRefused=0,heartFixtureRetired=0,heartFixtureLost=0;
const char* heartFixtureResult="idle";
bool movementHeartFixtureBusy(){return heartFixture.until!=0;}
void serviceHeartFixture(ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    const auto request=heartFixtureRequest;heartFixtureRequest=0;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);auto& f=heartFixture;
    if(request==1){NativePlayerHeart p{};
        if(f.until||movementGrenadeFixtureBusy()||movementPotionFixtureBusy()||powerFixture.until||darkVisionFixture.until||windblastFixture.until||bendtimeFixture.until||rangedFixture.until||
           !powerFixtureApiMatches()||std::strcmp(mapName,kGuardMap)||!coopGameplayReady(outgoing)||
           !observerIsolation.ready||!observerIsolation.movementFixture||observerIsolation.until<tick+26000||
           movementBankActive()||shadowStream.until||reviewClipActive()||!readNativePlayerHeart(memoryRead,base,p)||
           p.quantity||p.equipped||p.started||p.player.sword.slots[1]){
            ++heartFixtureRefused;heartFixtureResult="unsupported_scope_or_item_owned";
        }else{
            f={};f.before=p;f.epoch=outgoing.worldEpoch;f.until=tick+20000;++heartFixtureStarted;
            reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.declaration),1);
            heartFixtureResult="native_heart_quantity_requested";
        }
    }
    if(!f.until)return;
    if(f.epoch!=outgoing.worldEpoch||f.before.player.sword.game!=observedGame){f={};++heartFixtureRetired;heartFixtureResult="old_world_retired";return;}
    if((request==2||tick>=f.until||!observerIsolation.ready||!observerIsolation.movementFixture)&&!f.restoring){f.restoring=true;f.restoreUntil=tick+5000;}
    if(!coopGameplayReady(outgoing)){if(f.restoring)f.restoreUntil=tick+5000;return;}
    if(tick<f.nextRead)return;f.nextRead=tick+50;
    NativePlayerHeart p{};const bool read=powerFixtureApiMatches()&&readNativePlayerHeart(memoryRead,base,p);
    const auto& a=p.player.sword;const auto& b=f.before.player.sword;
    const bool same=read&&p.player.character==f.before.player.character&&a.game==b.game&&a.actor==b.actor&&
        a.inventory==b.inventory&&a.items==b.items&&a.itemCount==b.itemCount&&p.entry==f.before.entry&&p.declaration==f.before.declaration&&
        p.component==f.before.component&&p.entity==f.before.entity&&p.handle==f.before.handle&&p.equipmentDeclaration==f.before.equipmentDeclaration;
    if(!same)heartFixtureResult="waiting_for_original_native_heart";
    else if(!f.restoring){
        if(p.quantity==1&&p.equipped&&p.started){if(!f.observed){f.observed=true;++heartFixtureObserved;}heartFixtureResult="native_heart_ready";}
        else if(p.quantity==1&&!p.equipped&&!f.equipSent){
            if(a.slots[1]){f.restoring=true;f.restoreUntil=tick+5000;heartFixtureResult="foreign_slot_owner";}
            else{f.equipSent=true;reinterpret_cast<void(*)(void*,void*,bool)>(gameImage+0x614b20)(reinterpret_cast<void*>(a.inventory),reinterpret_cast<void*>(p.entry),true);heartFixtureResult="native_heart_equip_requested";}
        }
    }else if(p.equipped&&!f.unequipSent){
        f.unequipSent=true;reinterpret_cast<void(*)(void*,void*)>(gameImage+0x619640)(reinterpret_cast<void*>(a.inventory),reinterpret_cast<void*>(p.entry));heartFixtureResult="native_heart_unequip_requested";
    }else if(!p.equipped&&!p.started){
        if(p.quantity&&!f.quantitySent){f.quantitySent=true;reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(a.inventory),reinterpret_cast<void*>(p.declaration),0);heartFixtureResult="native_heart_quantity_restore_requested";}
        else if(!p.quantity&&!a.slots[1]){f={};++heartFixtureRestored;heartFixtureResult="quantity_and_selection_restored";return;}
    }
    if(f.restoring&&tick>=f.restoreUntil){f={};++heartFixtureLost;heartFixtureResult="restore_unconfirmed_reload_isolated_checkpoint";}
}
void writeHeartFixture(ULONGLONG tick){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.heart-fixture.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* file=nullptr;if(fopen_s(&file,tmp,"wb")||!file)return;const auto& f=heartFixture;
    fprintf(file,"{\"testFixtureOnly\":true,\"active\":%s,\"restoring\":%s,\"remainingMs\":%llu,\"started\":%llu,\"observed\":%llu,\"restored\":%llu,\"refused\":%llu,\"retired\":%llu,\"lost\":%llu,\"result\":\"%s\",\"handle\":%u}\n",f.until?"true":"false",f.restoring?"true":"false",f.until>tick?f.until-tick:0,heartFixtureStarted,heartFixtureObserved,heartFixtureRestored,heartFixtureRefused,heartFixtureRetired,heartFixtureLost,heartFixtureResult,f.before.handle);
    const bool ok=!ferror(file);const int closed=fclose(file);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
