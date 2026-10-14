#pragma once
#include "native_player_agility.h"
// Isolated native-upgrade observation. Native618DE0/6170B0/616210 apply and
// remove the exact enhancement's modifier. No jump-state/physics writes.
struct AgilityFixture {NativePlayerAgility before{};std::uint64_t epoch=0;ULONGLONG until=0;bool restoring=false;} agilityFixture;
unsigned agilityFixtureRequest=0;
std::uint64_t agilityFixtureStarted=0,agilityFixtureRestored=0,agilityFixtureRetired=0,agilityFixtureRefused=0;
const char* agilityFixtureResult="idle";
bool sameAgilityObjects(const NativePlayerAgility& a,const NativePlayerAgility& b){
    return a.player.character==b.player.character&&a.player.sword.game==b.player.sword.game&&a.player.sword.actor==b.player.sword.actor&&
        a.player.sword.inventory==b.player.sword.inventory&&a.player.sword.items==b.player.sword.items&&a.player.sword.itemCount==b.player.sword.itemCount&&
        a.entry==b.entry&&a.declaration==b.declaration;
}
void serviceAgilityFixture(ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);const auto request=agilityFixtureRequest;agilityFixtureRequest=0;
    if(request==1){NativePlayerAgility p{};
        if(agilityFixture.until||!powerFixtureApiMatches()||std::strcmp(mapName,kGuardMap)||!coopGameplayReady(outgoing)||!observerIsolation.ready||
           !observerIsolation.movementFixture||observerIsolation.until<tick+31000||movementBankActive()||shadowStream.until||reviewClipActive()||
           !readNativePlayerAgility(memoryRead,base,p)||p.quantity){++agilityFixtureRefused;agilityFixtureResult="unsupported_scope_or_upgrade_owned";}
        else{
            agilityFixture={p,outgoing.worldEpoch,tick+30000,false};++agilityFixtureStarted;
            reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.declaration),1);
            agilityFixtureResult="native_agility_requested";
        }
    }
    if(!agilityFixture.until)return;
    if(agilityFixture.epoch!=outgoing.worldEpoch||agilityFixture.before.player.sword.game!=observedGame){agilityFixture={};++agilityFixtureRetired;agilityFixtureResult="old_world_retired";return;}
    if(request==2||tick>=agilityFixture.until||!observerIsolation.ready||!observerIsolation.movementFixture)agilityFixture.restoring=true;
    if(!coopGameplayReady(outgoing))return; // Native menu owns callbacks until resume.
    NativePlayerAgility p{};
    if(!powerFixtureApiMatches()||!readNativePlayerAgility(memoryRead,base,p)||!sameAgilityObjects(p,agilityFixture.before)){agilityFixtureResult="waiting_for_original_inventory";return;}
    if(!agilityFixture.restoring){if(p.quantity==1)agilityFixtureResult="native_agility_ready";else agilityFixture.restoring=true;}
    if(agilityFixture.restoring){
        if(p.quantity){reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.declaration),0);agilityFixtureResult="native_agility_restore_requested";}
        else{agilityFixture={};++agilityFixtureRestored;agilityFixtureResult="native_quantity_restored";}
    }
}
void writeAgilityFixture(ULONGLONG tick){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.agility-fixture.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;
    fprintf(f,"{\"testFixtureOnly\":true,\"active\":%s,\"restoring\":%s,\"remainingMs\":%llu,\"started\":%llu,\"restored\":%llu,\"retired\":%llu,\"refused\":%llu,\"result\":\"%s\"}\n",agilityFixture.until?"true":"false",agilityFixture.restoring?"true":"false",agilityFixture.until>tick?agilityFixture.until-tick:0,agilityFixtureStarted,agilityFixtureRestored,agilityFixtureRetired,agilityFixtureRefused,agilityFixtureResult);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
