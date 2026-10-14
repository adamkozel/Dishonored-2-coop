#pragma once
#include "native_player_possession_item.h"
#include "native_player_possession_upgrade.h"
#include "native_player_possession_target.h"
// Bounded isolated Corvo inventory experiment. Normal controller input alone
// may exercise the native power. No direct transfer, AI, damage or target
// control invocation. A separate human40000 request acquires the exact native human upgrade;
// mandatory isolated checkpoint reload restores all native state.
struct PossessionFixture {
    NativePossessionItem before{},owned{};NativePossessionHumanUpgrade humanBefore{};bool human=false,humanRestoreSent=false;std::uint64_t epoch=0;
    ULONGLONG until=0,nextRead=0,restoreUntil=0;
    bool restoring=false,unequipSent=false,quantityRestored=false,observed=false;
} possessionFixture;
unsigned possessionFixtureRequest=0;
std::uint64_t possessionFixtureStarted=0,possessionFixtureObserved=0,possessionFixtureRestored=0,possessionFixtureRefused=0,possessionFixtureRetired=0,possessionFixtureLost=0;
const char* possessionFixtureResult="idle";
bool movementPossessionFixtureBusy(){return possessionFixture.until!=0;}
void servicePossessionFixture(ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    const auto request=possessionFixtureRequest;possessionFixtureRequest=0;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);auto& f=possessionFixture;
    if(request==1||request==3||request==4){NativePossessionItem p{};NativePossessionHumanUpgrade human{};
        const ULONGLONG duration=(request==3||request==4)?40000:20000;
        if(f.until||movementRangedFixtureBusy()||powerFixture.until||darkVisionFixture.until||windblastFixture.until||bendtimeFixture.until||agilityFixture.until||!powerFixtureApiMatches()||std::strcmp(mapName,kGuardMap)||!coopGameplayReady(outgoing)||
           !observerIsolation.ready||!observerIsolation.movementFixture||observerIsolation.until<tick+duration+4000||movementBankActive()||shadowStream.until||reviewClipActive()||
           !readNativePossessionItem(memoryRead,base,p)||(request==4&&(!readNativePossessionHumanUpgrade(memoryRead,base,human)||human.quantity||!sameSwordEquipmentIdentity(p.player.sword,human.player.sword)))||p.quantity||p.equipped||p.started||p.player.sword.slots[1]){
            ++possessionFixtureRefused;possessionFixtureResult="unsupported_scope_or_power_owned";
        }else{
            f={};f.before=p;f.human=request==4;f.humanBefore=human;f.epoch=outgoing.worldEpoch;f.until=tick+duration;++possessionFixtureStarted;
            if(f.human)reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(human.player.sword.inventory),reinterpret_cast<void*>(human.declaration),1);
            reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.declaration),1);
            possessionFixtureResult="native_possession_acquisition_requested";
        }
    }
    if(!f.until)return;
    if(f.epoch!=outgoing.worldEpoch||f.before.player.sword.game!=observedGame){f={};++possessionFixtureRetired;possessionFixtureResult="old_world_retired";return;}
    if((request==2||tick>=f.until||!observerIsolation.ready||!observerIsolation.movementFixture)&&!f.restoring){f.restoring=true;f.restoreUntil=tick+5000;}
    if(!coopGameplayReady(outgoing)){if(f.restoring)f.restoreUntil=tick+5000;return;}
    if(tick<f.nextRead)return;f.nextRead=tick+100;
    // Native possession can retire the NPC AI isolation lease when ownership
    // changes. Request cleanup then, but never revoke an acquired power or
    // upgrade from under an occupied host. Only a verified inactive controller
    // with restored default player mode permits inventory rollback.
    if(f.human&&f.observed&&f.restoring){NativePossessionTarget target{};
        if(!readNativePossessionTarget(memoryRead,base,target)||target.player.player.game!=f.before.player.sword.game||
           target.player.player.player!=f.before.player.sword.actor||target.state!=14||target.hasTarget||
           !target.player.defaultMode||target.player.player.current>1){
            possessionFixtureResult="waiting_for_native_host_exit";
            if(tick>=f.until+5000){f={};++possessionFixtureLost;possessionFixtureResult="host_exit_unconfirmed_reload_isolated_checkpoint";}
            else f.restoreUntil=tick+5000;
            return;
        }
    }
    NativePossessionItem p{};
    const bool read=powerFixtureApiMatches()&&readNativePossessionItem(memoryRead,base,p);
    const auto& a=p.player.sword;const auto& b=f.before.player.sword;
    const bool identity=read&&p.player.character==1&&a.game==b.game&&a.actor==b.actor&&a.inventory==b.inventory&&a.items==b.items&&a.itemCount==b.itemCount&&p.entry==f.before.entry&&p.declaration==f.before.declaration;
    const bool same=!f.owned.component||(p.component==f.owned.component&&p.entity==f.owned.entity&&p.handle==f.owned.handle&&p.equipmentDeclaration==f.owned.equipmentDeclaration);
    if(!identity||!same)possessionFixtureResult="waiting_for_original_native_equipment";
    else{
        if(!f.owned.component&&p.instantiated)f.owned=p;
        if(!f.restoring){
            // Equip only during acquisition. Possession itself temporarily
            // changes slot ownership; do not fight the native host lifecycle
            // or interpret its temporary slot as an external fixture conflict.
            if(!f.observed&&p.quantity==1&&p.instantiated&&!p.equipped){
                if(p.player.sword.slots[1]){f.restoring=true;f.restoreUntil=tick+5000;possessionFixtureResult="foreign_slot_owner";}
                else{reinterpret_cast<void(*)(void*,void*,bool)>(gameImage+0x614b20)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.entry),true);possessionFixtureResult="native_possession_equip_requested";}
            }else if(p.quantity==1&&p.equipped&&p.started){
                if(!f.observed){f.observed=true;++possessionFixtureObserved;}possessionFixtureResult="native_possession_ready";
            }
        }else if(p.equipped&&!f.unequipSent){
            reinterpret_cast<void(*)(void*,void*)>(gameImage+0x619640)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.entry));
            f.unequipSent=true;possessionFixtureResult="native_possession_unequip_requested";
        }else if(!p.equipped&&!p.started){
            if(p.quantity&&!f.quantityRestored){
                reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.declaration),0);
                f.quantityRestored=true;possessionFixtureResult="native_possession_quantity_restore_requested";
            }else if(!p.quantity&&!p.player.sword.slots[1]){
                bool done=true;
                if(f.human){NativePossessionHumanUpgrade h{};
                    if(!readNativePossessionHumanUpgrade(memoryRead,base,h)||!sameSwordEquipmentIdentity(h.player.sword,p.player.sword)||
                       h.entry!=f.humanBefore.entry||h.declaration!=f.humanBefore.declaration){possessionFixtureResult="waiting_for_original_human_upgrade";done=false;}
                    else if(h.quantity){
                        if(!f.humanRestoreSent){reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(h.player.sword.inventory),reinterpret_cast<void*>(h.declaration),0);f.humanRestoreSent=true;}
                        possessionFixtureResult="native_human_upgrade_restore_requested";done=false;
                    }
                }
                if(done){f={};++possessionFixtureRestored;possessionFixtureResult="quantity_and_selection_restored";return;}
            }
        }
    }
    if(f.restoring&&tick>=f.restoreUntil){f={};++possessionFixtureLost;possessionFixtureResult="restore_unconfirmed_reload_isolated_checkpoint";}
}
void writePossessionFixture(ULONGLONG tick){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.possession-fixture.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* file=nullptr;if(fopen_s(&file,tmp,"wb")||!file)return;const auto& f=possessionFixture;
    fprintf(file,"{\"testFixtureOnly\":true,\"active\":%s,\"restoring\":%s,\"remainingMs\":%llu,\"started\":%llu,\"observed\":%llu,\"restored\":%llu,\"refused\":%llu,\"retired\":%llu,\"lost\":%llu,\"result\":\"%s\",\"handle\":%u}\n",f.until?"true":"false",f.restoring?"true":"false",f.until>tick?f.until-tick:0,possessionFixtureStarted,possessionFixtureObserved,possessionFixtureRestored,possessionFixtureRefused,possessionFixtureRetired,possessionFixtureLost,possessionFixtureResult,f.owned.handle);
    const bool ok=!ferror(file);const int closed=fclose(file);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
