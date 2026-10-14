#pragma once
// Isolated Dunwall research only. Enable an already instantiated native
// starting power using the same quantity path as arkKisculeNodeActionPlayerLoadout.
// No remote effects, arbitrary grants, save operations or power-state writes.
struct PowerFixture {
    NativePlayerPowerItem before{};
    std::uint64_t epoch=0;ULONGLONG until=0,restoreUntil=0;
    bool restoring=false,unequipSent=false,quantityRestored=false,observed=false;
};
PowerFixture powerFixture{};
int powerFixtureRequest=0;
std::uint64_t powerFixtureStarted=0,powerFixtureObserved=0,powerFixtureRestored=0,powerFixtureRefused=0,powerFixtureRetired=0,powerFixtureLost=0;
const char* powerFixtureResult="idle";
bool powerFixtureApiMatches(){
    const unsigned char setQuantity[]={0x4c,0x8b,0xdc,0x57,0x48,0x83,0xec,0x60,0x49,0xc7,0x43,0xb8,0xfe,0xff,0xff,0xff,0x49,0x89,0x5b,0x10,0x49,0x89,0x6b,0x18};
    char path[MAX_PATH]{};
    return gameImage&&guardEquipmentApiMatches()&&!std::memcmp(gameImage+0x618de0,setQuantity,sizeof(setQuantity))&&
        GetModuleFileNameA(nullptr,path,sizeof(path))&&
        !_stricmp(path,"C:\\Users\\adako\\Documents\\Codex\\2026-10-08\\i-x20\\work\\asset-game\\Dishonored2.exe");
}
bool powerFixtureSame(const NativePlayerPowerItem& p){
    const auto& b=powerFixture.before;const auto& a=p.player.sword;const auto& s=b.player.sword;
    // Sword draw/holster and native power callbacks may change presentation.
    // Identity comparisons deliberately exclude those mutable fields.
    return a.game==s.game&&a.actor==s.actor&&a.inventory==s.inventory&&a.items==s.items&&a.itemCount==s.itemCount&&
        p.player.character==b.player.character&&p.entry==b.entry&&p.declaration==b.declaration&&p.component==b.component&&
        p.equipmentDeclaration==b.equipmentDeclaration&&p.entity==b.entity&&p.handle==b.handle;
}
bool powerFixtureRead(NativePlayerPowerItem& p){
    return nativeMutationAllowed&&GetCurrentThreadId()==engineThread.load()&&powerFixtureApiMatches()&&
        powerFixture.epoch==outgoing.worldEpoch&&powerFixture.before.player.sword.game==observedGame&&
        readNativePlayerPowerItem(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),p)&&powerFixtureSame(p);
}
void stopPowerFixture(ULONGLONG tick,const char* reason){
    if(!powerFixture.until||powerFixture.restoring)return;
    powerFixture.restoring=true;powerFixture.restoreUntil=tick+3000;powerFixtureResult=reason;
}
void beginPowerFixture(ULONGLONG tick){
    const auto refuse=[&](const char* reason){++powerFixtureRefused;powerFixtureResult=reason;};
    if(powerFixture.until){refuse("already_active_or_restoring");return;}
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load()||!powerFixtureApiMatches()||std::strcmp(mapName,kGuardMap)||
       !coopGameplayReady(outgoing)||!observerIsolation.ready||!observerIsolation.movementFixture||observerIsolation.until<tick+22000||
       movementBankActive()||reviewClipActive()||shadowStream.until){refuse("unsupported_scope");return;}
    NativePlayerPowerItem before{};
    if(!readNativePlayerPowerItem(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),before)||before.quantity||before.equipped||before.started||
       before.player.sword.slots[1]){refuse("native_power_not_unowned_and_unselected");return;}
    powerFixture={};powerFixture.before=before;powerFixture.epoch=outgoing.worldEpoch;powerFixture.until=tick+20000;
    ++powerFixtureStarted;
    reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(before.player.sword.inventory),reinterpret_cast<void*>(before.declaration),1);
    NativePlayerPowerItem p{};
    if(!powerFixtureRead(p)||p.quantity!=1){stopPowerFixture(tick,"quantity_change_unconfirmed");return;}
    // The native quantity callback may already select the new equipment.
    if(!p.equipped)reinterpret_cast<void(*)(void*,void*,bool)>(gameImage+0x614b20)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.entry),true);
    powerFixtureResult="native_equip_requested";
}
void servicePowerFixture(ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    const int request=powerFixtureRequest;powerFixtureRequest=0;
    if(request==1)beginPowerFixture(tick);else if(request==2)stopPowerFixture(tick,"cancelled");
    if(!powerFixture.until)return;
    if(observedGame!=powerFixture.before.player.sword.game||outgoing.worldEpoch!=powerFixture.epoch){
        ++powerFixtureRetired;powerFixture={};powerFixtureResult="old_world_retired";return;
    }
    if(tick>=powerFixture.until||!observerIsolation.ready||!observerIsolation.movementFixture||!coopGameplayReady(outgoing))stopPowerFixture(tick,"expired_or_scope_changed");
    NativePlayerPowerItem p{};const bool valid=coopGameplayReady(outgoing)&&powerFixtureRead(p);
    if(!powerFixture.restoring){
        if(valid&&p.quantity==1&&p.equipped&&p.started&&!powerFixture.observed){powerFixture.observed=true;++powerFixtureObserved;powerFixtureResult="native_power_ready";}
        return;
    }
    if(valid){
        if(p.equipped&&!powerFixture.unequipSent){
            reinterpret_cast<void(*)(void*,void*)>(gameImage+0x619640)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.entry));
            powerFixture.unequipSent=true;powerFixture.restoreUntil=tick+3000;powerFixtureResult="native_unequip_requested";
        }else if(!p.equipped&&!p.started){
            if(p.quantity&&!powerFixture.quantityRestored){
                reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(p.player.sword.inventory),reinterpret_cast<void*>(p.declaration),0);
                powerFixture.quantityRestored=true;powerFixture.restoreUntil=tick+3000;powerFixtureResult="native_quantity_restore_requested";
            }else if(!p.quantity&&!p.player.sword.slots[1]){++powerFixtureRestored;powerFixture={};powerFixtureResult="quantity_and_selection_restored";return;}
        }
    }
    // Native equipment stop callbacks are frozen in menus. Retain the lease
    // for cleanup on resume, without making native calls during the pause.
    if(outgoing.gameLogicState==2||outgoing.gameLogicState==13||outgoing.pendingGameLogicState==2||outgoing.pendingGameLogicState==13)powerFixture.restoreUntil=tick+3000;
    else if(tick>=powerFixture.restoreUntil){++powerFixtureLost;powerFixture={};powerFixtureResult="restore_unconfirmed";}
}
void writePowerFixture(ULONGLONG tick){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.power-fixture.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\"testFixtureOnly\":true,\"active\":%s,\"restoring\":%s,\"remainingMs\":%llu,\"result\":\"%s\",\"started\":%llu,\"observed\":%llu,\"restored\":%llu,\"refused\":%llu,\"retired\":%llu,\"lost\":%llu,\"character\":%u,\"handle\":%u}\n",
        powerFixture.until?"true":"false",powerFixture.restoring?"true":"false",powerFixture.until>tick?powerFixture.until-tick:0,powerFixtureResult,
        powerFixtureStarted,powerFixtureObserved,powerFixtureRestored,powerFixtureRefused,powerFixtureRetired,powerFixtureLost,powerFixture.before.player.character,powerFixture.before.handle);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
