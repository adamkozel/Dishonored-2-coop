#pragma once
#include "native_player_ranged_item.h"
// Bounded isolated gun/crossbow equipment observation. Normal native controller
// input owns equip/use scenes. Loaded mode grants two regular rounds only.
// No direct firing, projectile, damage or save calls. Loaded experiments must
// restore the isolated checkpoint after observing normal native shots.
// Restore the role-specific default upgrade acquired by the native pistol.
struct RangedFixture {
    NativePlayerRangedItem before{};NativeRangedDefaultUpgrade upgradeBefore{};NativeRangedAmmo ammoBefore{};std::uint64_t epoch=0;
    ULONGLONG until=0,nextRead=0,restoreUntil=0;
    bool restoring=false,unequipSent=false,quantityRestored=false,observed=false,loaded=false;
} rangedFixture;
unsigned rangedFixtureRequest=0;
std::uint64_t rangedFixtureStarted=0,rangedFixtureObserved=0,rangedFixtureRestored=0,rangedFixtureRefused=0,rangedFixtureRetired=0,rangedFixtureLost=0;
const char* rangedFixtureResult="idle";
void serviceRangedFixture(ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    const auto request=rangedFixtureRequest;rangedFixtureRequest=0;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);auto& f=rangedFixture;
    if(request==1||request==2||request==4||request==5){NativePlayerRangedItem p{};NativeRangedDefaultUpgrade upgrade{};NativeRangedAmmo ammo{};
        const auto kind=request>=4?request-3:request;
        if(f.until||movementGrenadeFixtureBusy()||powerFixture.until||darkVisionFixture.until||windblastFixture.until||!powerFixtureApiMatches()||std::strcmp(mapName,kGuardMap)||!coopGameplayReady(outgoing)||
           !observerIsolation.ready||!observerIsolation.movementFixture||observerIsolation.until<tick+34000||movementBankActive()||shadowStream.until||reviewClipActive()||
           !readNativePlayerRangedItem(memoryRead,base,kind,p)||p.quantity||p.equipped||p.started||p.player.sword.slots[1]||
           !readNativeRangedDefaultUpgrade(memoryRead,base,p,upgrade)||!readNativeRangedAmmo(memoryRead,base,p,ammo)||ammo.quantity){
            ++rangedFixtureRefused;rangedFixtureResult="unsupported_scope_or_power_owned";
        }else{
            f={};f.before=p;f.upgradeBefore=upgrade;f.ammoBefore=ammo;f.loaded=request>=4;f.epoch=outgoing.worldEpoch;f.until=tick+30000;++rangedFixtureStarted;
            reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.declaration),1);
            if(f.loaded)reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(ammo.declaration),2);
            rangedFixtureResult="native_ranged_acquisition_requested";
        }
    }
    if(!f.until)return;
    if(f.epoch!=outgoing.worldEpoch||f.before.player.sword.game!=observedGame){f={};++rangedFixtureRetired;rangedFixtureResult="old_world_retired";return;}
    if((request==3||tick>=f.until||!observerIsolation.ready||!observerIsolation.movementFixture)&&!f.restoring){f.restoring=true;f.restoreUntil=tick+5000;}
    if(!coopGameplayReady(outgoing)){if(f.restoring)f.restoreUntil=tick+5000;return;}
    if(tick<f.nextRead)return;f.nextRead=tick+100;
    NativePlayerRangedItem p{};
    const bool read=powerFixtureApiMatches()&&readNativePlayerRangedItem(memoryRead,base,f.before.kind,p);
    const auto& a=p.player.sword;const auto& b=f.before.player.sword;
    const bool identity=read&&p.player.character==f.before.player.character&&a.game==b.game&&a.actor==b.actor&&a.inventory==b.inventory&&a.items==b.items&&a.itemCount==b.itemCount&&p.entry==f.before.entry&&p.declaration==f.before.declaration;
    const bool same=p.component==f.before.component&&p.entity==f.before.entity&&p.handle==f.before.handle&&p.equipmentDeclaration==f.before.equipmentDeclaration;
    if(!identity||!same)rangedFixtureResult="waiting_for_original_native_equipment";
    else{
        if(!f.restoring){
            if(p.quantity==1&&!p.equipped&&!f.observed){
                if(p.player.sword.slots[1]){f.restoring=true;f.restoreUntil=tick+5000;rangedFixtureResult="foreign_slot_owner";}
                else{reinterpret_cast<void(*)(void*,void*,bool)>(gameImage+0x614b20)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.entry),true);rangedFixtureResult="native_ranged_equip_requested";}
            }else if(p.quantity==1&&p.equipped&&p.started){
                if(!f.observed){f.observed=true;++rangedFixtureObserved;}rangedFixtureResult="native_ranged_ready";
            }
        }else if(p.equipped&&!f.unequipSent){
            reinterpret_cast<void(*)(void*,void*)>(gameImage+0x619640)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.entry));
            f.unequipSent=true;rangedFixtureResult="native_ranged_unequip_requested";
        }else if(!p.equipped&&!p.started){
            if(p.quantity&&!f.quantityRestored){
                reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.declaration),0);
                f.quantityRestored=true;rangedFixtureResult="native_ranged_quantity_restore_requested";
            }else if(!p.quantity&&!p.player.sword.slots[1]){
                NativeRangedDefaultUpgrade upgrade{};
                if(!readNativeRangedDefaultUpgrade(memoryRead,base,p,upgrade)||upgrade.entry!=f.upgradeBefore.entry||upgrade.declaration!=f.upgradeBefore.declaration){
                    rangedFixtureResult="waiting_for_original_default_upgrade";
                }else if(upgrade.quantity!=f.upgradeBefore.quantity){
                    reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(upgrade.declaration),f.upgradeBefore.quantity);
                    rangedFixtureResult="native_default_upgrade_restore_requested";
                }else{
                    NativeRangedAmmo ammo{};
                    if(!readNativeRangedAmmo(memoryRead,base,p,ammo)||ammo.entry!=f.ammoBefore.entry||ammo.declaration!=f.ammoBefore.declaration){rangedFixtureResult="waiting_for_original_ammo";}
                    else if(ammo.quantity!=f.ammoBefore.quantity){
                        reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(ammo.declaration),f.ammoBefore.quantity);
                        rangedFixtureResult="native_ammo_restore_requested";
                    }else{f={};++rangedFixtureRestored;rangedFixtureResult="quantity_and_selection_restored";return;}
                }
            }
        }
    }
    if(f.restoring&&tick>=f.restoreUntil){f={};++rangedFixtureLost;rangedFixtureResult="restore_unconfirmed_reload_isolated_checkpoint";}
}
void writeRangedFixture(ULONGLONG tick){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.ranged-fixture.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* file=nullptr;if(fopen_s(&file,tmp,"wb")||!file)return;const auto& f=rangedFixture;
    fprintf(file,"{\"testFixtureOnly\":true,\"active\":%s,\"restoring\":%s,\"remainingMs\":%llu,\"started\":%llu,\"observed\":%llu,\"restored\":%llu,\"refused\":%llu,\"retired\":%llu,\"lost\":%llu,\"result\":\"%s\",\"handle\":%u}\n",f.until?"true":"false",f.restoring?"true":"false",f.until>tick?f.until-tick:0,rangedFixtureStarted,rangedFixtureObserved,rangedFixtureRestored,rangedFixtureRefused,rangedFixtureRetired,rangedFixtureLost,rangedFixtureResult,f.before.handle);
    const bool ok=!ferror(file);const int closed=fclose(file);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}

bool movementRangedFixtureBusy(){return rangedFixture.until!=0||movementGrenadeFixtureBusy();}
