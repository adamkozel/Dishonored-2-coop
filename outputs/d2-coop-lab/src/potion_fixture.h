#pragma once
#include "native_player_potion.h"
#include "native_player_mana.h"
// Finite isolated grant. The optional health-use command calls the exact
// native routine used by the verified wheel callback; no health/mana setter,
// invented scene or receiver outcome is used. Normal gameplay only.
struct PotionFixture {NativePlayerPotion before{};std::uint64_t epoch=0;ULONGLONG until=0,nextRead=0,restoreUntil=0;bool restoring=false,observed=false,restoreSent=false,useSent=false;} potionFixture;
unsigned potionFixtureRequest=0;
std::uint64_t potionFixtureStarted=0,potionFixtureRestored=0,potionFixtureRetired=0,potionFixtureRefused=0,potionFixtureLost=0;
const char* potionFixtureResult="idle";
bool movementPotionFixtureBusy(){return potionFixture.until!=0||movementMesmerizeFixtureBusy()||movementDominoFixtureBusy()||movementSwarmFixtureBusy()||movementPossessionFixtureBusy()||movementDoppelgangerFixtureBusy()||movementShadowWalkFixtureBusy();}
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
    if(tick<f.nextRead&&request!=4&&request!=5)return;f.nextRead=tick+50;
    NativePlayerPotion p{};
    if(!powerFixtureApiMatches()||!readNativePlayerPotion(memoryRead,base,f.before.kind,p)||!samePotionObjects(p,f.before))potionFixtureResult="waiting_for_original_inventory";
    else if(!f.restoring){
        if(request==4){
            // Native 1404f5fb0 invokes 1407194a0 on the resolved health component.
            // The routine itself checks eligibility, spends the item, heals and
            // queues its native drinking sound outside power-wheel logic8.
            const unsigned char bytes[]={0x48,0x89,0x5c,0x24,0x10,0x57,0x48,0x83,0xec,0x40,0x48,0x8b,0xf9,0xba,2,0,0,0,0x48,0x81,0xc1,0xe0,0,0,0,0xe8,0x52,0x14,0xcc,0xff};
            NativeHealth h{};
            if(f.useSent||!f.observed||p.kind!=1||p.quantity!=1||std::memcmp(gameImage+0x7194a0,bytes,sizeof(bytes))||
               !readNativeHealth(memoryRead,base,1,h)||h.logic!=1||h.actor!=p.player.sword.actor||h.game!=p.player.sword.game||h.current<0.5f||h.current>=h.maximum){
                ++potionFixtureRefused;potionFixtureResult="native_health_use_refused";return;
            }
            f.useSent=true;
            reinterpret_cast<void(*)(void*)>(gameImage+0x7194a0)(reinterpret_cast<void*>(h.component));
            potionFixtureResult="native_health_consume_requested";return;
        }
        if(request==5){
            // Native wheel callback 1404f5ff0 invokes this same routine.
            // Eligibility, quantity, attributes and regeneration stay native.
            const unsigned char bytes[]={0x48,0x89,0x5c,0x24,0x18,0x57,0x48,0x83,0xec,0x50,0x48,0x8b,0xd9,0xba,2,0,0,0,0x48,0x83,0xc1,0x50,0xe8,0xd5,0x4d,0xc9,0xff};
            NativePlayerMana mana{};NativeHealth h{};
            if(f.useSent||!f.observed||p.kind!=2||p.quantity!=1||std::memcmp(gameImage+0x745b20,bytes,sizeof(bytes))||
               !readNativePlayerMana(memoryRead,base,mana)||mana.player!=p.player.sword.actor||mana.game!=p.player.sword.game||
               !readNativeHealth(memoryRead,base,1,h)||h.logic!=1||h.actor!=mana.player||h.game!=mana.game||h.current<0.5f){
                ++potionFixtureRefused;potionFixtureResult="native_mana_use_refused";return;
            }
            f.useSent=true;
            reinterpret_cast<void(*)(void*)>(gameImage+0x745b20)(reinterpret_cast<void*>(mana.component));
            potionFixtureResult="native_mana_consume_requested";return;
        }
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
