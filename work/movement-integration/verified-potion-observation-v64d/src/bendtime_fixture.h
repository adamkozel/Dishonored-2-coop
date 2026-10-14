#pragma once
#include "native_player_bendtime_item.h"
// Bounded isolated Corvo inventory experiment. Normal controller input alone
// may toggle the native power. Never call time-scale, FX or gameplay functions here.
struct BendTimeFixture {
    NativeBendTimeItem before{},owned{};std::uint64_t epoch=0;
    ULONGLONG until=0,nextRead=0,restoreUntil=0;
    bool restoring=false,unequipSent=false,quantityRestored=false,observed=false;
} bendtimeFixture;
unsigned bendtimeFixtureRequest=0;
std::uint64_t bendtimeFixtureStarted=0,bendtimeFixtureObserved=0,bendtimeFixtureRestored=0,bendtimeFixtureRefused=0,bendtimeFixtureRetired=0,bendtimeFixtureLost=0;
const char* bendtimeFixtureResult="idle";
void serviceBendTimeFixture(ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    const auto request=bendtimeFixtureRequest;bendtimeFixtureRequest=0;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);auto& f=bendtimeFixture;
    if(request==1){NativeBendTimeItem p{};
        if(f.until||movementRangedFixtureBusy()||powerFixture.until||darkVisionFixture.until||windblastFixture.until||!powerFixtureApiMatches()||std::strcmp(mapName,kGuardMap)||!coopGameplayReady(outgoing)||
           !observerIsolation.ready||!observerIsolation.movementFixture||observerIsolation.until<tick+24000||movementBankActive()||shadowStream.until||reviewClipActive()||
           !readNativeBendTimeItem(memoryRead,base,p)||p.quantity||p.equipped||p.started||p.powerActive||p.player.sword.slots[1]){
            ++bendtimeFixtureRefused;bendtimeFixtureResult="unsupported_scope_or_power_owned";
        }else{
            f={};f.before=p;f.epoch=outgoing.worldEpoch;f.until=tick+20000;++bendtimeFixtureStarted;
            reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.declaration),1);
            bendtimeFixtureResult="native_bendtime_acquisition_requested";
        }
    }
    if(!f.until)return;
    if(f.epoch!=outgoing.worldEpoch||f.before.player.sword.game!=observedGame){f={};++bendtimeFixtureRetired;bendtimeFixtureResult="old_world_retired";return;}
    if((request==2||tick>=f.until||!observerIsolation.ready||!observerIsolation.movementFixture)&&!f.restoring){f.restoring=true;f.restoreUntil=tick+5000;}
    if(!coopGameplayReady(outgoing)){if(f.restoring)f.restoreUntil=tick+5000;return;}
    if(tick<f.nextRead)return;f.nextRead=tick+100;
    NativeBendTimeItem p{};
    const bool read=powerFixtureApiMatches()&&readNativeBendTimeItem(memoryRead,base,p);
    const auto& a=p.player.sword;const auto& b=f.before.player.sword;
    const bool identity=read&&p.player.character==1&&a.game==b.game&&a.actor==b.actor&&a.inventory==b.inventory&&a.items==b.items&&a.itemCount==b.itemCount&&p.entry==f.before.entry&&p.declaration==f.before.declaration;
    const bool same=!f.owned.component||(p.component==f.owned.component&&p.entity==f.owned.entity&&p.handle==f.owned.handle&&p.equipmentDeclaration==f.owned.equipmentDeclaration);
    if(!identity||!same)bendtimeFixtureResult="waiting_for_original_native_equipment";
    else{
        if(!f.owned.component&&p.instantiated)f.owned=p;
        if(!f.restoring){
            if(p.quantity==1&&p.instantiated&&!p.equipped){
                if(p.player.sword.slots[1]){f.restoring=true;f.restoreUntil=tick+5000;bendtimeFixtureResult="foreign_slot_owner";}
                else{reinterpret_cast<void(*)(void*,void*,bool)>(gameImage+0x614b20)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.entry),true);bendtimeFixtureResult="native_bendtime_equip_requested";}
            }else if(p.quantity==1&&p.equipped&&p.started){
                if(!f.observed){f.observed=true;++bendtimeFixtureObserved;}bendtimeFixtureResult="native_bendtime_ready";
            }
        }else if(p.powerActive){
            // Do not remove the equipment while its native time request lives.
            // Ordinary controller toggle / native timer must end the power.
            // The fixed cleanup deadline still applies; caller then reloads
            // its isolated checkpoint if native cleanup cannot be verified.
            bendtimeFixtureResult="waiting_for_native_bendtime_end";
        }else if(p.equipped&&!f.unequipSent){
            reinterpret_cast<void(*)(void*,void*)>(gameImage+0x619640)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.entry));
            f.unequipSent=true;bendtimeFixtureResult="native_bendtime_unequip_requested";
        }else if(!p.equipped&&!p.started){
            if(p.quantity&&!f.quantityRestored){
                reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.declaration),0);
                f.quantityRestored=true;bendtimeFixtureResult="native_bendtime_quantity_restore_requested";
            }else if(!p.quantity&&!p.player.sword.slots[1]){f={};++bendtimeFixtureRestored;bendtimeFixtureResult="quantity_and_selection_restored";return;}
        }
    }
    if(f.restoring&&tick>=f.restoreUntil){f={};++bendtimeFixtureLost;bendtimeFixtureResult="restore_unconfirmed_reload_isolated_checkpoint";}
}
void writeBendTimeFixture(ULONGLONG tick){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.bendtime-fixture.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* file=nullptr;if(fopen_s(&file,tmp,"wb")||!file)return;const auto& f=bendtimeFixture;
    fprintf(file,"{\"testFixtureOnly\":true,\"active\":%s,\"restoring\":%s,\"remainingMs\":%llu,\"started\":%llu,\"observed\":%llu,\"restored\":%llu,\"refused\":%llu,\"retired\":%llu,\"lost\":%llu,\"result\":\"%s\",\"handle\":%u}\n",f.until?"true":"false",f.restoring?"true":"false",f.until>tick?f.until-tick:0,bendtimeFixtureStarted,bendtimeFixtureObserved,bendtimeFixtureRestored,bendtimeFixtureRefused,bendtimeFixtureRetired,bendtimeFixtureLost,bendtimeFixtureResult,f.owned.handle);
    const bool ok=!ferror(file);const int closed=fclose(file);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
