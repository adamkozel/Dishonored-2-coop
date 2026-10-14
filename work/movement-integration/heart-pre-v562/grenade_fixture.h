#pragma once
#include "native_player_grenade.h"
// Isolated equip-only observation lease. Native inventory callbacks perform
// acquisition, equip and rollback. This service never cooks, fires or spawns.
struct GrenadeFixture {
    NativePlayerGrenade before{};std::uint64_t epoch=0;
    ULONGLONG until=0,nextRead=0,restoreUntil=0;
    bool restoring=false,equipSent=false,unequipSent=false,quantitySent=false,observed=false;
} grenadeFixture;
unsigned grenadeFixtureRequest=0;
std::uint64_t grenadeFixtureStarted=0,grenadeFixtureObserved=0,grenadeFixtureRestored=0,grenadeFixtureRefused=0,grenadeFixtureRetired=0,grenadeFixtureLost=0;
const char* grenadeFixtureResult="idle";
bool movementGrenadeFixtureBusy(){return grenadeFixture.until!=0||movementPotionFixtureBusy();}
void serviceGrenadeFixture(ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    const auto request=grenadeFixtureRequest;grenadeFixtureRequest=0;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);auto& f=grenadeFixture;
    if(request==1){NativePlayerGrenade p{};
        if(f.until||movementPotionFixtureBusy()||powerFixture.until||darkVisionFixture.until||windblastFixture.until||bendtimeFixture.until||rangedFixture.until||
           !powerFixtureApiMatches()||std::strcmp(mapName,kGuardMap)||!coopGameplayReady(outgoing)||
           !observerIsolation.ready||!observerIsolation.movementFixture||observerIsolation.until<tick+18000||
           movementBankActive()||shadowStream.until||reviewClipActive()||!readNativePlayerGrenade(memoryRead,base,p)||
           p.quantity||p.equipped||p.started||p.player.sword.slots[1]){
            ++grenadeFixtureRefused;grenadeFixtureResult="unsupported_scope_or_item_owned";
        }else{
            f={};f.before=p;f.epoch=outgoing.worldEpoch;f.until=tick+12000;++grenadeFixtureStarted;
            reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.declaration),1);
            grenadeFixtureResult="native_grenade_quantity_requested";
        }
    }
    if(!f.until)return;
    if(f.epoch!=outgoing.worldEpoch||f.before.player.sword.game!=observedGame){f={};++grenadeFixtureRetired;grenadeFixtureResult="old_world_retired";return;}
    if((request==2||tick>=f.until||!observerIsolation.ready||!observerIsolation.movementFixture)&&!f.restoring){f.restoring=true;f.restoreUntil=tick+5000;}
    if(!coopGameplayReady(outgoing)){if(f.restoring)f.restoreUntil=tick+5000;return;}
    if(tick<f.nextRead)return;f.nextRead=tick+50;
    NativePlayerGrenade p{};const bool read=powerFixtureApiMatches()&&readNativePlayerGrenade(memoryRead,base,p);
    const auto& a=p.player.sword;const auto& b=f.before.player.sword;
    const bool same=read&&p.player.character==f.before.player.character&&a.game==b.game&&a.actor==b.actor&&
        a.inventory==b.inventory&&a.items==b.items&&a.itemCount==b.itemCount&&p.entry==f.before.entry&&p.declaration==f.before.declaration&&
        p.component==f.before.component&&p.entity==f.before.entity&&p.handle==f.before.handle&&p.equipmentDeclaration==f.before.equipmentDeclaration;
    if(!same)grenadeFixtureResult="waiting_for_original_native_grenade";
    else if(!f.restoring){
        if(p.quantity==1&&p.equipped&&p.started){if(!f.observed){f.observed=true;++grenadeFixtureObserved;}grenadeFixtureResult="native_grenade_ready";}
        else if(p.quantity==1&&!p.equipped&&!f.equipSent){
            if(a.slots[1]){f.restoring=true;f.restoreUntil=tick+5000;grenadeFixtureResult="foreign_slot_owner";}
            else{f.equipSent=true;reinterpret_cast<void(*)(void*,void*,bool)>(gameImage+0x614b20)(reinterpret_cast<void*>(a.inventory),reinterpret_cast<void*>(p.entry),true);grenadeFixtureResult="native_grenade_equip_requested";}
        }
    }else if(p.equipped&&!f.unequipSent){
        f.unequipSent=true;reinterpret_cast<void(*)(void*,void*)>(gameImage+0x619640)(reinterpret_cast<void*>(a.inventory),reinterpret_cast<void*>(p.entry));grenadeFixtureResult="native_grenade_unequip_requested";
    }else if(!p.equipped&&!p.started){
        if(p.quantity&&!f.quantitySent){f.quantitySent=true;reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(a.inventory),reinterpret_cast<void*>(p.declaration),0);grenadeFixtureResult="native_grenade_quantity_restore_requested";}
        else if(!p.quantity&&!a.slots[1]){f={};++grenadeFixtureRestored;grenadeFixtureResult="quantity_and_selection_restored";return;}
    }
    if(f.restoring&&tick>=f.restoreUntil){f={};++grenadeFixtureLost;grenadeFixtureResult="restore_unconfirmed_reload_isolated_checkpoint";}
}
void writeGrenadeFixture(ULONGLONG tick){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.grenade-fixture.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* file=nullptr;if(fopen_s(&file,tmp,"wb")||!file)return;const auto& f=grenadeFixture;
    fprintf(file,"{\"testFixtureOnly\":true,\"active\":%s,\"restoring\":%s,\"remainingMs\":%llu,\"started\":%llu,\"observed\":%llu,\"restored\":%llu,\"refused\":%llu,\"retired\":%llu,\"lost\":%llu,\"result\":\"%s\",\"handle\":%u}\n",f.until?"true":"false",f.restoring?"true":"false",f.until>tick?f.until-tick:0,grenadeFixtureStarted,grenadeFixtureObserved,grenadeFixtureRestored,grenadeFixtureRefused,grenadeFixtureRetired,grenadeFixtureLost,grenadeFixtureResult,f.before.handle);
    const bool ok=!ferror(file);const int closed=fclose(file);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
