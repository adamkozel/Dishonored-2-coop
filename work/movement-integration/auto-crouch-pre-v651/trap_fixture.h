#pragma once
#include "native_player_placeable.h"
// Isolated equip-only observation lease. Native inventory callbacks perform
// acquisition, equip and rollback. This service never cooks, fires or spawns.
struct TrapFixture {
    NativePlayerPlaceable before{},acquired{};std::uint64_t epoch=0;
    ULONGLONG until=0,nextRead=0,restoreUntil=0;
    bool restoring=false,equipSent=false,unequipSent=false,quantitySent=false,observed=false;
} trapFixture;
unsigned trapFixtureRequest=0;
std::uint64_t trapFixtureStarted=0,trapFixtureObserved=0,trapFixtureRestored=0,trapFixtureRefused=0,trapFixtureRetired=0,trapFixtureLost=0;
const char* trapFixtureResult="idle";
bool movementTrapFixtureBusy(){return trapFixture.until!=0;}
void serviceTrapFixture(ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    const auto request=trapFixtureRequest;trapFixtureRequest=0;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);auto& f=trapFixture;
    if(request==1||request==2){NativePlayerPlaceable p{};
        if(f.until||movementGrenadeFixtureBusy()||movementPotionFixtureBusy()||powerFixture.until||darkVisionFixture.until||windblastFixture.until||bendtimeFixture.until||rangedFixture.until||
           !powerFixtureApiMatches()||std::strcmp(mapName,kGuardMap)||!coopGameplayReady(outgoing)||
           !observerIsolation.ready||!observerIsolation.movementFixture||observerIsolation.until<tick+26000||
           movementBankActive()||shadowStream.until||reviewClipActive()||!readNativePlayerPlaceable(memoryRead,base,request,p)||
           p.quantity||p.equipped||p.started||p.player.sword.slots[1]){
            ++trapFixtureRefused;trapFixtureResult="unsupported_scope_or_item_owned";
        }else{
            f={};f.before=p;f.epoch=outgoing.worldEpoch;f.until=tick+20000;++trapFixtureStarted;
            reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.declaration),1);
            trapFixtureResult="native_trap_quantity_requested";
        }
    }
    if(!f.until)return;
    if(f.epoch!=outgoing.worldEpoch||f.before.player.sword.game!=observedGame){f={};++trapFixtureRetired;trapFixtureResult="old_world_retired";return;}
    if((request==3||tick>=f.until||!observerIsolation.ready||!observerIsolation.movementFixture)&&!f.restoring){f.restoring=true;f.restoreUntil=tick+5000;}
    if(!coopGameplayReady(outgoing)){if(f.restoring)f.restoreUntil=tick+5000;return;}
    if(tick<f.nextRead)return;f.nextRead=tick+50;
    NativePlayerPlaceable p{};const bool read=powerFixtureApiMatches()&&readNativePlayerPlaceable(memoryRead,base,f.before.kind,p);
    const auto& a=p.player.sword;const auto& b=f.before.player.sword;
    const bool same=read&&p.player.character==f.before.player.character&&a.game==b.game&&a.actor==b.actor&&
        a.inventory==b.inventory&&a.items==b.items&&a.itemCount==b.itemCount&&p.entry==f.before.entry&&p.declaration==f.before.declaration&&
        (!f.acquired.component||(p.component==f.acquired.component&&p.entity==f.acquired.entity&&p.handle==f.acquired.handle&&p.equipmentDeclaration==f.acquired.equipmentDeclaration));
    if(same&&p.component&&!f.acquired.component)f.acquired=p;
    if(!same)trapFixtureResult="waiting_for_original_native_trap";
    else if(!f.restoring){
        if(p.quantity==1&&p.component&&p.equipped&&p.started){if(!f.observed){f.observed=true;++trapFixtureObserved;}trapFixtureResult="native_trap_ready";}
        else if(p.quantity==1&&p.component&&!p.equipped&&!f.equipSent){
            if(a.slots[1]){f.restoring=true;f.restoreUntil=tick+5000;trapFixtureResult="foreign_slot_owner";}
            else{f.equipSent=true;reinterpret_cast<void(*)(void*,void*,bool)>(gameImage+0x614b20)(reinterpret_cast<void*>(a.inventory),reinterpret_cast<void*>(p.entry),true);trapFixtureResult="native_trap_equip_requested";}
        }
    }else if(p.equipped&&!f.unequipSent){
        f.unequipSent=true;reinterpret_cast<void(*)(void*,void*)>(gameImage+0x619640)(reinterpret_cast<void*>(a.inventory),reinterpret_cast<void*>(p.entry));trapFixtureResult="native_trap_unequip_requested";
    }else if(!p.equipped&&!p.started){
        if(p.quantity&&!f.quantitySent){f.quantitySent=true;reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(a.inventory),reinterpret_cast<void*>(p.declaration),0);trapFixtureResult="native_trap_quantity_restore_requested";}
        else if(!p.quantity&&!a.slots[1]){f={};++trapFixtureRestored;trapFixtureResult="quantity_and_selection_restored";return;}
    }
    if(f.restoring&&tick>=f.restoreUntil){f={};++trapFixtureLost;trapFixtureResult="restore_unconfirmed_reload_isolated_checkpoint";}
}
void writeTrapFixture(ULONGLONG tick){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.trap-fixture.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* file=nullptr;if(fopen_s(&file,tmp,"wb")||!file)return;const auto& f=trapFixture;
    fprintf(file,"{\"testFixtureOnly\":true,\"active\":%s,\"restoring\":%s,\"remainingMs\":%llu,\"started\":%llu,\"observed\":%llu,\"restored\":%llu,\"refused\":%llu,\"retired\":%llu,\"lost\":%llu,\"result\":\"%s\",\"handle\":%u}\n",f.until?"true":"false",f.restoring?"true":"false",f.until>tick?f.until-tick:0,trapFixtureStarted,trapFixtureObserved,trapFixtureRestored,trapFixtureRefused,trapFixtureRetired,trapFixtureLost,trapFixtureResult,f.before.handle);
    const bool ok=!ferror(file);const int closed=fclose(file);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
