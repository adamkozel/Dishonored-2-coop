#pragma once
#include "native_player_domino_item.h"
// Bounded isolated Emily inventory experiment. Normal controller input alone
// may fire the native power. Never call native spawn, AI or damage functions here. World effects are observed
// independently and reset by the mandatory isolated checkpoint reload.
struct DominoFixture {
    NativeDominoItem before{},owned{};std::uint64_t epoch=0;
    ULONGLONG until=0,nextRead=0,restoreUntil=0;
    bool restoring=false,unequipSent=false,quantityRestored=false,observed=false;
} dominoFixture;
unsigned dominoFixtureRequest=0;
std::uint64_t dominoFixtureStarted=0,dominoFixtureObserved=0,dominoFixtureRestored=0,dominoFixtureRefused=0,dominoFixtureRetired=0,dominoFixtureLost=0;
const char* dominoFixtureResult="idle";
bool movementDominoFixtureBusy(){return dominoFixture.until!=0;}
void serviceDominoFixture(ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    const auto request=dominoFixtureRequest;dominoFixtureRequest=0;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);auto& f=dominoFixture;
    if(request==1||request==3){NativeDominoItem p{};
        const ULONGLONG duration=request==3?40000:20000;
        if(f.until||movementRangedFixtureBusy()||powerFixture.until||darkVisionFixture.until||bendtimeFixture.until||agilityFixture.until||!powerFixtureApiMatches()||std::strcmp(mapName,kGuardMap)||!coopGameplayReady(outgoing)||
           !observerIsolation.ready||!observerIsolation.movementFixture||observerIsolation.until<tick+duration+4000||movementBankActive()||shadowStream.until||reviewClipActive()||
           !readNativeDominoItem(memoryRead,base,p)||p.quantity||p.equipped||p.started||p.player.sword.slots[1]){
            ++dominoFixtureRefused;dominoFixtureResult="unsupported_scope_or_power_owned";
        }else{
            f={};f.before=p;f.epoch=outgoing.worldEpoch;f.until=tick+duration;++dominoFixtureStarted;
            reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.declaration),1);
            dominoFixtureResult="native_domino_acquisition_requested";
        }
    }
    if(!f.until)return;
    if(f.epoch!=outgoing.worldEpoch||f.before.player.sword.game!=observedGame){f={};++dominoFixtureRetired;dominoFixtureResult="old_world_retired";return;}
    if((request==2||tick>=f.until||!observerIsolation.ready||!observerIsolation.movementFixture)&&!f.restoring){f.restoring=true;f.restoreUntil=tick+5000;}
    if(!coopGameplayReady(outgoing)){if(f.restoring)f.restoreUntil=tick+5000;return;}
    if(tick<f.nextRead)return;f.nextRead=tick+100;
    NativeDominoItem p{};
    const bool read=powerFixtureApiMatches()&&readNativeDominoItem(memoryRead,base,p);
    const auto& a=p.player.sword;const auto& b=f.before.player.sword;
    const bool identity=read&&p.player.character==2&&a.game==b.game&&a.actor==b.actor&&a.inventory==b.inventory&&a.items==b.items&&a.itemCount==b.itemCount&&p.entry==f.before.entry&&p.declaration==f.before.declaration;
    const bool same=!f.owned.component||(p.component==f.owned.component&&p.entity==f.owned.entity&&p.handle==f.owned.handle&&p.equipmentDeclaration==f.owned.equipmentDeclaration);
    if(!identity||!same)dominoFixtureResult="waiting_for_original_native_equipment";
    else{
        if(!f.owned.component&&p.instantiated)f.owned=p;
        if(!f.restoring){
            if(p.quantity==1&&p.instantiated&&!p.equipped){
                if(p.player.sword.slots[1]){f.restoring=true;f.restoreUntil=tick+5000;dominoFixtureResult="foreign_slot_owner";}
                else{reinterpret_cast<void(*)(void*,void*,bool)>(gameImage+0x614b20)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.entry),true);dominoFixtureResult="native_domino_equip_requested";}
            }else if(p.quantity==1&&p.equipped&&p.started){
                if(!f.observed){f.observed=true;++dominoFixtureObserved;}dominoFixtureResult="native_domino_ready";
            }
        }else if(p.equipped&&!f.unequipSent){
            reinterpret_cast<void(*)(void*,void*)>(gameImage+0x619640)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.entry));
            f.unequipSent=true;dominoFixtureResult="native_domino_unequip_requested";
        }else if(!p.equipped&&!p.started){
            if(p.quantity&&!f.quantityRestored){
                reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.declaration),0);
                f.quantityRestored=true;dominoFixtureResult="native_domino_quantity_restore_requested";
            }else if(!p.quantity&&!p.player.sword.slots[1]){f={};++dominoFixtureRestored;dominoFixtureResult="quantity_and_selection_restored";return;}
        }
    }
    if(f.restoring&&tick>=f.restoreUntil){f={};++dominoFixtureLost;dominoFixtureResult="restore_unconfirmed_reload_isolated_checkpoint";}
}
void writeDominoFixture(ULONGLONG tick){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.domino-fixture.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* file=nullptr;if(fopen_s(&file,tmp,"wb")||!file)return;const auto& f=dominoFixture;
    fprintf(file,"{\"testFixtureOnly\":true,\"active\":%s,\"restoring\":%s,\"remainingMs\":%llu,\"started\":%llu,\"observed\":%llu,\"restored\":%llu,\"refused\":%llu,\"retired\":%llu,\"lost\":%llu,\"result\":\"%s\",\"handle\":%u}\n",f.until?"true":"false",f.restoring?"true":"false",f.until>tick?f.until-tick:0,dominoFixtureStarted,dominoFixtureObserved,dominoFixtureRestored,dominoFixtureRefused,dominoFixtureRetired,dominoFixtureLost,dominoFixtureResult,f.owned.handle);
    const bool ok=!ferror(file);const int closed=fclose(file);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
