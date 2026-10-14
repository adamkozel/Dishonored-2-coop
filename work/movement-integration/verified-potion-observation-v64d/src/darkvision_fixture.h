#pragma once
#include "native_player_darkvision_item.h"
// Isolated Dunwall research. Native inventory acquisition/selection only;
// never invoke the power's fire, damage, VFX or camera functions directly.
struct DarkVisionFixture {
    NativeDarkVisionItem before{},owned{};std::uint64_t epoch=0;
    ULONGLONG until=0,nextRead=0,restoreUntil=0;
    bool restoring=false,unequipSent=false,quantityRestored=false,observed=false;
} darkVisionFixture;
unsigned darkVisionFixtureRequest=0;
std::uint64_t darkVisionFixtureStarted=0,darkVisionFixtureObserved=0,darkVisionFixtureRestored=0,darkVisionFixtureRefused=0,darkVisionFixtureRetired=0,darkVisionFixtureLost=0;
const char* darkVisionFixtureResult="idle";
bool sameDarkVisionInventory(const NativeDarkVisionItem& a,const NativeDarkVisionItem& b){
    return a.player.character==b.player.character&&a.player.sword.game==b.player.sword.game&&a.player.sword.actor==b.player.sword.actor&&
        a.player.sword.inventory==b.player.sword.inventory&&a.player.sword.items==b.player.sword.items&&a.player.sword.itemCount==b.player.sword.itemCount&&a.entry==b.entry&&a.declaration==b.declaration;
}
void serviceDarkVisionFixture(ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    const auto request=darkVisionFixtureRequest;darkVisionFixtureRequest=0;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);auto& f=darkVisionFixture;
    if(request==1){NativeDarkVisionItem p{};
        if(f.until||movementRangedFixtureBusy()||powerFixture.until||!powerFixtureApiMatches()||std::strcmp(mapName,kGuardMap)||!coopGameplayReady(outgoing)||
           !observerIsolation.ready||!observerIsolation.movementFixture||observerIsolation.until<tick+24000||movementBankActive()||shadowStream.until||reviewClipActive()||
           !readNativeDarkVisionItem(memoryRead,base,p)||p.quantity||p.equipped||p.started||p.managerStarted||p.player.sword.slots[1]){
            ++darkVisionFixtureRefused;darkVisionFixtureResult="unsupported_scope_or_power_owned";
        }else{
            f={};f.before=p;f.epoch=outgoing.worldEpoch;f.until=tick+20000;++darkVisionFixtureStarted;
            reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.declaration),1);
            darkVisionFixtureResult="native_darkvision_acquisition_requested";
        }
    }
    if(!f.until)return;
    if(f.epoch!=outgoing.worldEpoch||f.before.player.sword.game!=observedGame){f={};++darkVisionFixtureRetired;darkVisionFixtureResult="old_world_retired";return;}
    // Native selection stop does not cancel Dark Vision's 24-second reveal.
    // Wait for that native lifetime, without directly invoking effect/fire APIs.
    if((request==2||tick>=f.until||!observerIsolation.ready||!observerIsolation.movementFixture)&&!f.restoring){f.restoring=true;f.restoreUntil=tick+30000;}
    if(!coopGameplayReady(outgoing)){if(f.restoring)f.restoreUntil=tick+30000;return;}
    if(tick<f.nextRead)return;f.nextRead=tick+100;
    NativeDarkVisionItem p{};
    const bool read=powerFixtureApiMatches()&&readNativeDarkVisionItem(memoryRead,base,p)&&sameDarkVisionInventory(p,f.before);
    const bool same=!f.owned.component||(p.component==f.owned.component&&p.entity==f.owned.entity&&p.handle==f.owned.handle&&p.equipmentDeclaration==f.owned.equipmentDeclaration&&p.manager==f.owned.manager);
    if(!read||!same){darkVisionFixtureResult="waiting_for_original_native_equipment";}
    else{
        if(!f.owned.component&&p.instantiated)f.owned=p;
        if(!f.restoring){
            if(p.quantity==1&&p.instantiated&&!p.equipped){
                if(p.player.sword.slots[1]){f.restoring=true;f.restoreUntil=tick+30000;darkVisionFixtureResult="foreign_slot_owner";}
                else{reinterpret_cast<void(*)(void*,void*,bool)>(gameImage+0x614b20)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.entry),true);darkVisionFixtureResult="native_darkvision_equip_requested";}
            }else if(p.quantity==1&&p.equipped&&p.started){
                if(!f.observed){f.observed=true;++darkVisionFixtureObserved;}darkVisionFixtureResult="native_darkvision_ready";
            }
        }else if(p.equipped&&!f.unequipSent){
            reinterpret_cast<void(*)(void*,void*)>(gameImage+0x619640)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.entry));
            f.unequipSent=true;darkVisionFixtureResult="native_darkvision_unequip_requested";
        }else if(!p.equipped&&!p.started&&!p.managerStarted){
            if(p.quantity&&!f.quantityRestored){
                reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.declaration),0);
                f.quantityRestored=true;darkVisionFixtureResult="native_darkvision_quantity_restore_requested";
            }else if(!p.quantity&&!p.player.sword.slots[1]){f={};++darkVisionFixtureRestored;darkVisionFixtureResult="quantity_selection_and_effect_inactive";return;}
        }
    }
    if(f.restoring&&tick>=f.restoreUntil){f={};++darkVisionFixtureLost;darkVisionFixtureResult="restore_unconfirmed_reload_isolated_checkpoint";}
}
void writeDarkVisionFixture(ULONGLONG tick){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.darkvision-fixture.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* file=nullptr;if(fopen_s(&file,tmp,"wb")||!file)return;const auto& f=darkVisionFixture;
    fprintf(file,"{\"testFixtureOnly\":true,\"active\":%s,\"restoring\":%s,\"remainingMs\":%llu,\"started\":%llu,\"observed\":%llu,\"restored\":%llu,\"refused\":%llu,\"retired\":%llu,\"lost\":%llu,\"result\":\"%s\",\"handle\":%u}\n",f.until?"true":"false",f.restoring?"true":"false",f.until>tick?f.until-tick:0,darkVisionFixtureStarted,darkVisionFixtureObserved,darkVisionFixtureRestored,darkVisionFixtureRefused,darkVisionFixtureRetired,darkVisionFixtureLost,darkVisionFixtureResult,f.owned.handle);
    const bool ok=!ferror(file);const int closed=fclose(file);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
