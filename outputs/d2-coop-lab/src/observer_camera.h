#pragma once
// Lab-only observer camera for reviewing remote presentation from a host
// window while the host player stays parked. Native render-view setter
// 140890c50(view, position[3], basis[9]) writes view+A8/+C0 position, +CC
// basis and +3EB0 view matrices (cf. IGCS Dishonored 2 notes, same build
// family). "hold" freezes the most active view at its current native pose;
// nothing is synthesized, no gameplay entity or player transform is changed.
// Holds expire after 120 s and on "off". Not network-visible.
// 1214an (#61): "hold_at x y z" holds the same native view orientation with the eye
// placed at a world point, so the observer's body never has to stand there (lab takes
// lost views to platform edges, walls, candelabras and auto-crouch).
struct ObserverViewRecord {std::uintptr_t view=0;float position[3]{},basis[9]{};ULONGLONG tick=0;std::uint64_t count=0;};
using NativeRenderViewSet=void(*)(std::uintptr_t,const float*,const float*);
NativeRenderViewSet realNativeRenderViewSet=nullptr;
const unsigned char kRenderViewSetPrologue[18]={0x48,0x89,0x5c,0x24,0x08,0x48,0x89,0x74,0x24,0x10,0x57,0x48,0x81,0xec,0xf0,0x00,0x00,0x00};
SRWLOCK observerCameraLock=SRWLOCK_INIT;
std::array<ObserverViewRecord,8> observerViews{};
struct ObserverCameraHold {bool active=false;std::uintptr_t view=0;float position[3]{},basis[9]{};ULONGLONG until=0,started=0;std::uint64_t overrides=0;const char* result="off";};
ObserverCameraHold observerCamera{};
int observerCameraRequest=-1;float observerCameraAt[3]{};
// 1214ap (#9, lab only): "coop_player_health_hold on|off" refills the local player's native health (component
// +0x20) to its maximum every service tick while on, so long sword-fight takes do not end in the source's death
// (1258m/1258n/1258y/1258ai). The native god mode cannot do it: ark_god_player is read only by the god console
// toggle (+0x718f93), never by damage code (confirmed static, build 1.77.9.0). A single hit larger than the current
// health still kills; hits, reactions and parries stay native.
bool playerHealthHold=false;std::uint64_t playerHealthRefills=0,playerHealthHoldMisses=0;float playerHealthLast=0;
// 1214aq (#11, lab only): while a hold is active the local player's own first-person arm entities (left_arm_1 /
// right_arm_2, children of the player) are hidden with the native hide (+0x8784b0: sets actor+0xD0 bit 0, then the
// render update) and shown again on release (+0x87c590 show, +0x87c5d0 update). Takes 1199e-1199g: with the camera
// held away from the observer's body, those arms floated beside the filmed partner. Release re-validates each arm by
// handle and actor before touching it, so a reload never leaves a stale pointer in use.
struct ObserverHiddenArm {std::uint32_t handle=0;std::uintptr_t actor=0;};
std::array<ObserverHiddenArm,2> observerHiddenArms{};std::uint64_t observerArmHides=0,observerArmShows=0;ULONGLONG observerArmScanAt=0;
void observerCameraArms(bool hide,ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load()||!observedGame)return;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(!hide){
        for(auto& a:observerHiddenArms){if(!a.actor)continue;NativePlayerArm arm{};
            if(readNativePlayerArm(memoryRead,base,observedGame,a.handle,arm)&&arm.actor==a.actor){
                reinterpret_cast<void(*)(void*)>(gameImage+0x87c590)(reinterpret_cast<void*>(a.actor));
                reinterpret_cast<void(*)(void*)>(gameImage+0x87c5d0)(reinterpret_cast<void*>(a.actor));++observerArmShows;}
            a={};}
        return;
    }
    if(tick-observerArmScanAt<500)return;observerArmScanAt=tick;
    std::uintptr_t handles=0;std::uint32_t flags=0;
    if(!memoryRead(observedGame+0x601c0,&handles,8)||!handles||!memoryRead(observedGame+0x601c8,&flags,4)||(flags&0xffffff)>16384)return;
    for(std::uint32_t i=0;i<(flags&0xffffff);++i){std::uint32_t h=0;if(!memoryRead(handles+i*4,&h,4)||!h)continue;
        NativePlayerArm arm{};if(!readNativePlayerArm(memoryRead,base,observedGame,h,arm)||arm.side<1||arm.side>2)continue;
        unsigned char f=0;if(!memoryRead(arm.actor+0xd0,&f,1))continue;
        if(!(f&1)){reinterpret_cast<void(*)(void*)>(gameImage+0x8784b0)(reinterpret_cast<void*>(arm.actor));++observerArmHides;}
        observerHiddenArms[arm.side-1]={h,arm.actor};
    }
}
// 1214au (#14 real grenade, #11 mana elixir; lab only): "coop_grant_item grenade|health|mana" adds one native item to
// the local inventory through the native inventory add (+0x618de0, the routine the isolated fixtures use) in any
// gameplay scope, product mode included. The isolated fixtures refuse product mode and roll the item back after 12 s
// (take grenade-bound-guard-v1273a: unsupported_scope_or_item_owned); takes restore the inventory baseline at the end.
unsigned labGrantRequest=0;std::uint64_t labGrants=0,labGrantRefused=0;const char* labGrantResult="idle";ULONGLONG labGrantEquipUntil=0;bool labGrantEquipSent=false;
// 1214av (#11 mana elixir; lab only): "coop_use_item health|mana" drinks one potion through the native routine the
// item wheel calls (health +0x7194a0 on the health component, mana +0x745b20 on the mana component), with the same
// byte guards as the isolated potion fixture; eligibility, quantity and effects stay native. Product mode included.
unsigned labUseRequest=0;std::uint64_t labUses=0,labUseRefused=0;const char* labUseResult="idle";float labManaBefore=-1;
void serviceLabItemUse(){
    if(!labUseRequest||!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    const unsigned u=labUseRequest;labUseRequest=0;const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    NativeHealth h{};if(!coopGameplayReady(outgoing)||!readNativeHealth(memoryRead,base,1,h)||h.logic!=1||h.current<0.5f){++labUseRefused;labUseResult="refused_player_state";return;}
    if(u==1){
        const unsigned char bytes[]={0x48,0x89,0x5c,0x24,0x10,0x57,0x48,0x83,0xec,0x40,0x48,0x8b,0xf9,0xba,2,0,0,0,0x48,0x81,0xc1,0xe0,0,0,0,0xe8,0x52,0x14,0xcc,0xff};
        if(std::memcmp(gameImage+0x7194a0,bytes,sizeof(bytes))){++labUseRefused;labUseResult="health_routine_mismatch";return;}
        reinterpret_cast<void(*)(void*)>(gameImage+0x7194a0)(reinterpret_cast<void*>(h.component));++labUses;labUseResult="health_use_requested";return;
    }
    const unsigned char bytes[]={0x48,0x89,0x5c,0x24,0x18,0x57,0x48,0x83,0xec,0x50,0x48,0x8b,0xd9,0xba,2,0,0,0,0x48,0x83,0xc1,0x50,0xe8,0xd5,0x4d,0xc9,0xff};
    NativePlayerMana mana{};
    if(std::memcmp(gameImage+0x745b20,bytes,sizeof(bytes))){++labUseRefused;labUseResult="mana_routine_mismatch";return;}
    if(!readNativePlayerMana(memoryRead,base,mana)||mana.player!=h.actor||mana.game!=h.game){++labUseRefused;labUseResult="mana_read_failed";return;}
    // 1214aw: the native routine refuses a full mana pool (1287a: the potion stayed 1 -> 1). The lab first empties the
    // pool (component +0x20, as the reader reads it) and records the previous value; the drink then refills natively.
    labManaBefore=mana.current;*reinterpret_cast<float*>(mana.component+0x20)=0.f;
    reinterpret_cast<void(*)(void*)>(gameImage+0x745b20)(reinterpret_cast<void*>(mana.component));++labUses;labUseResult="mana_use_requested";
}
void serviceLabItemGrant(){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    // A granted grenade is then selected once with the native equip (+0x614b20, as the isolated fixture does) when the
    // left-hand slot is free, so the trigger throws it.
    if(labGrantEquipUntil&&!labGrantRequest){const auto tick=GetTickCount64();
        if(tick>=labGrantEquipUntil){labGrantEquipUntil=0;if(!labGrantEquipSent)labGrantResult="grenade_equip_timeout";return;}
        NativePlayerGrenade g{};if(!readNativePlayerGrenade(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),g))return;
        if(g.equipped){labGrantEquipUntil=0;labGrantResult="granted_grenade_equipped";return;}
        if(!labGrantEquipSent&&g.quantity>=1){
            if(g.player.sword.slots[1]){labGrantEquipUntil=0;labGrantResult="grenade_slot_occupied";return;}
            labGrantEquipSent=true;reinterpret_cast<void(*)(void*,void*,bool)>(gameImage+0x614b20)(reinterpret_cast<void*>(g.player.sword.inventory),reinterpret_cast<void*>(g.entry),true);
            labGrantResult="grenade_equip_requested";}
        return;}
    if(!labGrantRequest)return;
    const unsigned r=labGrantRequest;labGrantRequest=0;const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(!coopGameplayReady(outgoing)){++labGrantRefused;labGrantResult="refused_not_gameplay";return;}
    std::uintptr_t inventory=0,declaration=0;
    if(r==1){NativePlayerGrenade g{};if(!readNativePlayerGrenade(memoryRead,base,g)){++labGrantRefused;labGrantResult="grenade_read_failed";return;}
        inventory=g.player.sword.inventory;declaration=g.declaration;}
    else{NativePlayerPotion p{};if(!readNativePlayerPotion(memoryRead,base,r==2?1u:2u,p)){++labGrantRefused;labGrantResult="potion_read_failed";return;}
        inventory=p.player.sword.inventory;declaration=p.declaration;}
    if(!inventory||!declaration){++labGrantRefused;labGrantResult="no_inventory_or_declaration";return;}
    reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(inventory),reinterpret_cast<void*>(declaration),1);
    ++labGrants;labGrantResult=r==1?"granted_grenade":r==2?"granted_health_potion":"granted_mana_potion";
    if(r==1){labGrantEquipUntil=GetTickCount64()+3000;labGrantEquipSent=false;}
}
void servicePlayerHealthHold(){
    if(!playerHealthHold||!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    NativeHealth h{};if(!readNativeHealth(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),1,h)||!h.component){++playerHealthHoldMisses;return;}
    playerHealthLast=h.current;
    if(h.current>0&&h.current<h.maximum&&std::isfinite(h.maximum)){*reinterpret_cast<float*>(h.component+0x20)=h.maximum;++playerHealthRefills;}
}
void observeNativeRenderViewSet(std::uintptr_t view,const float* position,const float* basis){
    const auto tick=GetTickCount64();float p[3]{},b[9]{};bool override=false;
    AcquireSRWLockExclusive(&observerCameraLock);
    if(observerCamera.active&&view==observerCamera.view&&tick<observerCamera.until){
        std::memcpy(p,observerCamera.position,sizeof(p));std::memcpy(b,observerCamera.basis,sizeof(b));override=true;++observerCamera.overrides;
    }else if(position&&basis){
        unsigned slot=0;bool found=false;
        for(unsigned i=0;i<observerViews.size();++i)if(observerViews[i].view==view){slot=i;found=true;break;}
        if(!found){ULONGLONG oldest=~0ull;for(unsigned i=0;i<observerViews.size();++i)if(observerViews[i].tick<oldest){oldest=observerViews[i].tick;slot=i;}observerViews[slot]={};observerViews[slot].view=view;}
        auto& r=observerViews[slot];std::memcpy(r.position,position,12);std::memcpy(r.basis,basis,36);r.tick=tick;++r.count;
    }
    ReleaseSRWLockExclusive(&observerCameraLock);
    realNativeRenderViewSet(view,override?p:position,override?b:basis);
}
bool installObserverCamera(){
    auto* target=gameImage+0x890c50;if(std::memcmp(target,kRenderViewSetPrologue,sizeof(kRenderViewSetPrologue)))return false;
    auto* trampoline=static_cast<unsigned char*>(VirtualAlloc(nullptr,64,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));if(!trampoline)return false;
    std::memcpy(trampoline,kRenderViewSetPrologue,sizeof(kRenderViewSetPrologue));
    unsigned char jump[14]{0xff,0x25,0,0,0,0};void* resume=target+sizeof(kRenderViewSetPrologue);std::memcpy(jump+6,&resume,8);
    std::memcpy(trampoline+sizeof(kRenderViewSetPrologue),jump,sizeof(jump));DWORD old=0,unused=0;
    if(!VirtualProtect(trampoline,64,PAGE_EXECUTE_READ,&old)){VirtualFree(trampoline,0,MEM_RELEASE);return false;}
    FlushInstructionCache(GetCurrentProcess(),trampoline,64);realNativeRenderViewSet=reinterpret_cast<NativeRenderViewSet>(trampoline);
    void* wrapper=reinterpret_cast<void*>(&observeNativeRenderViewSet);std::memcpy(jump+6,&wrapper,8);
    if(!VirtualProtect(target,sizeof(jump),PAGE_EXECUTE_READWRITE,&old))return false;
    std::memcpy(target,jump,sizeof(jump));FlushInstructionCache(GetCurrentProcess(),target,sizeof(jump));
    return VirtualProtect(target,sizeof(jump),old,&unused)!=0;
}
void serviceObserverCamera(ULONGLONG tick){
    servicePlayerHealthHold();serviceLabItemGrant();serviceLabItemUse();
    {bool active=false;AcquireSRWLockShared(&observerCameraLock);active=observerCamera.active;ReleaseSRWLockShared(&observerCameraLock);
     if(active)observerCameraArms(true,tick);else if(observerHiddenArms[0].actor||observerHiddenArms[1].actor)observerCameraArms(false,tick);}
    const int request=observerCameraRequest;observerCameraRequest=-1;
    AcquireSRWLockExclusive(&observerCameraLock);
    if(request==0){observerCamera.active=false;observerCamera.result="off";}
    else if(request==1||request==2){
        // The main rendered view is the one updated most often recently.
        const ObserverViewRecord* best=nullptr;
        for(const auto& r:observerViews)if(r.view&&tick-r.tick<250&&(!best||r.count>best->count))best=&r;
        bool finite=best!=nullptr;
        if(best){for(float f:best->position)finite=finite&&std::isfinite(f);for(float f:best->basis)finite=finite&&std::isfinite(f);}
        if(!finite){observerCamera.active=false;observerCamera.result="no_active_view";}
        else{observerCamera={};observerCamera.active=true;observerCamera.view=best->view;std::memcpy(observerCamera.position,request==2?observerCameraAt:best->position,12);
            std::memcpy(observerCamera.basis,best->basis,36);observerCamera.started=tick;observerCamera.until=tick+120000;observerCamera.result=request==2?"holding_at":"holding";}
    }
    if(observerCamera.active&&tick>=observerCamera.until){observerCamera.active=false;observerCamera.result="expired";}
    ReleaseSRWLockExclusive(&observerCameraLock);
}
void writeObserverCamera(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.observer-camera.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    ObserverCameraHold h{};std::array<ObserverViewRecord,8> views{};
    AcquireSRWLockShared(&observerCameraLock);h=observerCamera;views=observerViews;ReleaseSRWLockShared(&observerCameraLock);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;
    fprintf(f,"{\"labOnly\":true,\"installed\":%s,\"active\":%s,\"result\":\"%s\",\"view\":\"%llx\",\"overrides\":%llu,\"position\":[%.6f,%.6f,%.6f],\"views\":[",
        realNativeRenderViewSet?"true":"false",h.active?"true":"false",h.result,static_cast<unsigned long long>(h.view),h.overrides,h.position[0],h.position[1],h.position[2]);
    bool first=true;for(const auto& r:views)if(r.view){fprintf(f,"%s{\"view\":\"%llx\",\"count\":%llu,\"tick\":%llu,\"position\":[%.6f,%.6f,%.6f]}",first?"":",",static_cast<unsigned long long>(r.view),r.count,r.tick,r.position[0],r.position[1],r.position[2]);first=false;}
    fprintf(f,"],\"armsHidden\":{\"hides\":%llu,\"shows\":%llu,\"left\":%s,\"right\":%s},",static_cast<unsigned long long>(observerArmHides),
        static_cast<unsigned long long>(observerArmShows),observerHiddenArms[0].actor?"true":"false",observerHiddenArms[1].actor?"true":"false");
    fprintf(f,"\"itemGrant\":{\"grants\":%llu,\"refused\":%llu,\"result\":\"%s\"},",static_cast<unsigned long long>(labGrants),static_cast<unsigned long long>(labGrantRefused),labGrantResult);
    {NativePlayerMana m{};const bool mr=readNativePlayerMana(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),m);
     fprintf(f,"\"itemUse\":{\"uses\":%llu,\"refused\":%llu,\"result\":\"%s\",\"manaBefore\":%.3f,\"manaNow\":%.3f},",static_cast<unsigned long long>(labUses),static_cast<unsigned long long>(labUseRefused),labUseResult,labManaBefore,mr?m.current:-1.f);}
    fprintf(f,"\"healthHold\":{\"active\":%s,\"refills\":%llu,\"misses\":%llu,\"last\":%.3f}}\n",playerHealthHold?"true":"false",
        static_cast<unsigned long long>(playerHealthRefills),static_cast<unsigned long long>(playerHealthHoldMisses),playerHealthLast);const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
