#pragma once
#include "native_player_potion.h"
// Finite isolated grant only. Native input must consume the item; this
// service never heals, changes mana, invokes a drink or plays a scene.
struct PotionFixture {NativePlayerPotion before{};std::uint64_t epoch=0;ULONGLONG until=0,nextRead=0,restoreUntil=0;bool restoring=false,observed=false,restoreSent=false;} potionFixture;
unsigned potionFixtureRequest=0;
std::uint64_t potionFixtureStarted=0,potionFixtureRestored=0,potionFixtureRetired=0,potionFixtureRefused=0,potionFixtureLost=0;
const char* potionFixtureResult="idle";
bool movementPotionFixtureBusy(){return potionFixture.until!=0;}
void servicePotionFixture(ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);const auto request=potionFixtureRequest;potionFixtureRequest=0;auto& f=potionFixture;
    if(request==1||request==2){NativePlayerPotion p{};
        if(f.until||movementRangedFixtureBusy()||powerFixture.until||darkVisionFixture.until||windblastFixture.until||bendtimeFixture.until||agilityFixture.until||
           !powerFixtureApiMatches()||std::strcmp(mapName,kGuardMap)||!coopGameplayReady(outgoing)||!observerIsolation.ready||
           !observerIsolation.movementFixture||observerIsolation.until<tick+18000||movementBankActive()||shadowStream.until||reviewClipActive()||
           !readNativePlayerPotion(memoryRead,base,request,p)||p.quantity){++potionFixtureRefused;potionFixtureResult="unsupported_scope_or_item_owned";}
        else{f={};f.before=p;f.epoch=outgoing.worldEpoch;f.until=tick+12000;++potionFixtureStarted;
            reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.declaration),1);
            potionFixtureResult="native_potion_quantity_requested";}
    }
    if(!f.until)return;
    if(f.epoch!=outgoing.worldEpoch||f.before.player.sword.game!=observedGame){f={};++potionFixtureRetired;potionFixtureResult="old_world_retired";return;}
    if((request==3||tick>=f.until||!observerIsolation.ready||!observerIsolation.movementFixture)&&!f.restoring){f.restoring=true;f.restoreUntil=tick+5000;}
    if(!coopGameplayReady(outgoing)){if(f.restoring)f.restoreUntil=tick+5000;return;}
    if(tick<f.nextRead)return;f.nextRead=tick+50;
    NativePlayerPotion p{};
    if(!powerFixtureApiMatches()||!readNativePlayerPotion(memoryRead,base,f.before.kind,p)||!samePotionObjects(p,f.before))potionFixtureResult="waiting_for_original_inventory";
    else if(!f.restoring){
        if(p.quantity==1){f.observed=true;potionFixtureResult="native_potion_ready";}
        else if(f.observed&&p.quantity==0)potionFixtureResult="native_potion_consumed";
        else if(p.quantity>1){f.restoring=true;f.restoreUntil=tick+5000;potionFixtureResult="unexpected_quantity_restore_required";}
    }else if(!p.quantity){f={};++potionFixtureRestored;potionFixtureResult="native_quantity_restored";return;}
    else if(!f.restoreSent){f.restoreSent=true;reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.declaration),0);potionFixtureResult="native_potion_restore_requested";}
    if(f.restoring&&tick>=f.restoreUntil){f={};++potionFixtureLost;potionFixtureResult="restore_unconfirmed_reload_isolated_checkpoint";}
}
void writePotionFixture(ULONGLONG tick){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.potion-fixture.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* file=nullptr;if(fopen_s(&file,tmp,"wb")||!file)return;const auto& f=potionFixture;
    fprintf(file,"{\"testFixtureOnly\":true,\"active\":%s,\"restoring\":%s,\"kind\":%u,\"remainingMs\":%llu,\"started\":%llu,\"restored\":%llu,\"retired\":%llu,\"refused\":%llu,\"lost\":%llu,\"result\":\"%s\"}\n",f.until?"true":"false",f.restoring?"true":"false",f.before.kind,f.until>tick?f.until-tick:0,potionFixtureStarted,potionFixtureRestored,potionFixtureRetired,potionFixtureRefused,potionFixtureLost,potionFixtureResult);
    const bool ok=!ferror(file);const int closed=fclose(file);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
