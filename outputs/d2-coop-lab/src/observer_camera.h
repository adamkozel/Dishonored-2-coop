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
// 1214dl (#11 stagger_knockdown_recovery, hit_reactions_directions; lab only): "coop_lab_hit shove|sword
// front|back|left|right" applies a native damage declaration to the local player from that side, through the original
// native damage function (not the hooked entry, so no damage record or relay). shove is
// components/base/damage/shove_damage.damage (DAMAGETYPE_SHOVED, no amount, PLAYER_HITREACT_KNOCKDOWN; export
// knockdown-decls-v1367): the native knockdown (KnockedDown, state 16). sword is the guards' npc_sword.damage (20,
// PLAYER_HITREACT_BIG): the native directional stun (state 14). The lab fixtures have no natural knockdown source
// (1375), and the source's own grenade stuns only after one turn, in an unreliable direction (1363, 1390c-f).
// 1214dp: the sword hit applied in every direction without a stun (1397m-o); blast is the grenade's own
// explosion_grenade.damage (PLAYER_HITREACT_BIG), centred 1.2 m out on the chosen side.
unsigned labHitKind=0,labHitSide=0;bool labKnockdownRequest=false;std::uint64_t labKnockdowns=0,labKnockdownRefused=0;const char* labKnockdownResult="idle";
void serviceLabKnockdown(){
    if(!labKnockdownRequest||!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    labKnockdownRequest=false;const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    const auto refuse=[](const char* why){++labKnockdownRefused;labKnockdownResult=why;};
    NativeHealth h{};float position[3]{},direction[3]{};
    if(!swordDamageApiMatches()||!coopGameplayReady(outgoing)||!readNativeHealth(memoryRead,base,1,h)||h.logic!=1||h.current<1.f||
       !memoryRead(h.actor+0x300,position,12)||!memoryRead(h.actor+0x30c,direction,12)){refuse("refused_player_state");return;}
    float length=0;for(float v:direction){if(!std::isfinite(v)){refuse("direction");return;}length+=v*v;}
    for(float v:position)if(!std::isfinite(v)){refuse("position");return;}
    if(length<.99f||length>1.01f){refuse("direction");return;}
    const char* declName=labHitKind==3?"components/equipment/damage/explosion_grenade.damage":
        labHitKind==2?"components/equipment/damage/npc_sword.damage":"components/base/damage/shove_damage.damage";
    const auto decl=reinterpret_cast<std::uintptr_t(*)(void*,const char*,bool,bool)>(gameImage+0x1e030)(gameImage+0x3990b60,declName,false,false);
    std::uintptr_t vt=0,manager=0;if(!decl||!memoryRead(decl,&vt,8)||vt!=base+0x1c5d138||!memoryRead(decl+0x10,&manager,8)||manager!=base+0x3990b60){refuse("declaration");return;}
    // The parameter is the direction the hit travels: from the front it runs against the facing, from behind along
    // it, from the left toward the player's right and from the right toward the left (z up).
    const float fx=direction[0],fy=direction[1];
    if(labHitSide==0)for(float& v:direction)v=-v;
    else if(labHitSide==2){direction[0]=fy;direction[1]=-fx;direction[2]=0;}
    else if(labHitSide==3){direction[0]=-fy;direction[1]=fx;direction[2]=0;}
    position[2]+=1;
    alignas(16) std::array<unsigned char,144> params{};alignas(16) std::array<unsigned char,32> result{};
    reinterpret_cast<void(*)(void*,void*)>(gameImage+0x780260)(params.data(),reinterpret_cast<void*>(decl));
    reinterpret_cast<void(*)(void*)>(gameImage+0x780470)(result.data());
    // 1214dn: the hit starts 0.6 m out on the side it comes from; a sword hit names the nearest living NPC as attacker.
    for(unsigned k=0;k<2;++k)position[k]-=direction[k]*(labHitKind==3?1.2f:.6f);
    std::uint32_t attacker=0x3fff0010;
    if(labHitKind==2){std::vector<NativeWorldNpc> catalog;unsigned unbound=0;float best=1600.f;
        if(readWorldNpcCatalog(memoryRead,base,observedGame,catalog,unbound))for(const auto& n:catalog){if(!n.unique)continue;
            const float dx=n.position[0]-position[0],dy=n.position[1]-position[1],d=dx*dx+dy*dy;if(d<best){best=d;attacker=n.handle;}}}
    const unsigned regions=1,region=0;
    std::memcpy(params.data()+0x20,&regions,4);std::memcpy(params.data()+0x4c,&region,4);
    std::memcpy(params.data()+0x24,position,12);std::memcpy(params.data()+0x3c,direction,12);std::memcpy(params.data()+0x54,&attacker,4);
    realNativeDamage(reinterpret_cast<void*>(h.component),params.data(),result.data());
    ++labKnockdowns;labKnockdownResult=labHitKind==3?"blast_applied":labHitKind==2?"sword_applied":"shove_applied";
}
bool labHitAllowed(const char* t,unsigned* kind=nullptr,unsigned* side=nullptr){
    const char* kinds[]={"shove ","sword ","blast "};const char* sides[]={"front","back","left","right"};
    for(unsigned k=0;k<3;++k){const auto n=std::strlen(kinds[k]);if(std::strncmp(t,kinds[k],n))continue;
        for(unsigned d=0;d<4;++d)if(!std::strcmp(t+n,sides[d])){if(kind)*kind=k+1;if(side)*side=d;return true;}}
    return false;}
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
    // The native routine drinks only after a native power spent the pool (audit 1290); a lab pool write does not
    // help (1214aw, removed in 1214ax). The pool before the drink is recorded.
    labManaBefore=mana.current;
    reinterpret_cast<void(*)(void*)>(gameImage+0x745b20)(reinterpret_cast<void*>(mana.component));++labUses;labUseResult="mana_use_requested";
}
// 1214ck (#11 ranged and item coverage; lab only): "coop_grant_item pistol|crossbow|stunmine|springrazor|heart" grant
// the weapon with six rounds of its current ammunition, two of a trap, or the Heart, through the same native add, and
// select it once with the native equip when the left-hand slot is free; the takes restore the checkpoint after each case.
unsigned labGrantEquipKind=0;
struct LabGrantItem {std::uintptr_t inventory=0,entry=0,declaration=0,component=0,ammo=0;int quantity=0;bool equipped=false,slotBusy=false;};
bool readLabGrantItem(unsigned r,LabGrantItem& g){const auto base=reinterpret_cast<std::uintptr_t>(gameImage);g={};
    if(r==1){NativePlayerGrenade x{};if(!readNativePlayerGrenade(memoryRead,base,x))return false;
        g.inventory=x.player.sword.inventory;g.entry=x.entry;g.declaration=x.declaration;g.component=x.component;g.quantity=x.quantity;g.equipped=x.equipped;g.slotBusy=x.player.sword.slots[1]!=0;return true;}
    if(r==2||r==3){NativePlayerPotion x{};if(!readNativePlayerPotion(memoryRead,base,r==2?1u:2u,x))return false;g.inventory=x.player.sword.inventory;g.declaration=x.declaration;return true;}
    if(r==4||r==5){NativePlayerRangedItem x{};NativeRangedAmmo a{};if(!readNativePlayerRangedItem(memoryRead,base,r-3,x)||!readNativeRangedAmmo(memoryRead,base,x,a))return false;
        g.inventory=x.player.sword.inventory;g.entry=x.entry;g.declaration=x.declaration;g.component=x.component;g.ammo=a.declaration;g.quantity=x.quantity;g.equipped=x.equipped;g.slotBusy=x.player.sword.slots[1]!=0;return true;}
    if(r==6||r==7){NativePlayerPlaceable x{};if(!readNativePlayerPlaceable(memoryRead,base,r-5,x))return false;
        g.inventory=x.player.sword.inventory;g.entry=x.entry;g.declaration=x.declaration;g.component=x.component;g.quantity=x.quantity;g.equipped=x.equipped;g.slotBusy=x.player.sword.slots[1]!=0;return true;}
    if(r==8){NativePlayerHeart x{};if(!readNativePlayerHeart(memoryRead,base,x))return false;
        g.inventory=x.player.sword.inventory;g.entry=x.entry;g.declaration=x.declaration;g.component=x.component;g.quantity=x.quantity;g.equipped=x.equipped;g.slotBusy=x.player.sword.slots[1]!=0;return true;}
    return false;}
void serviceLabItemGrant(){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    // A granted left-hand item is then selected once with the native equip (+0x614b20, as the isolated fixtures do) when
    // the left-hand slot is free, so the trigger uses it.
    if(labGrantEquipUntil&&!labGrantRequest){const auto tick=GetTickCount64();
        if(tick>=labGrantEquipUntil){labGrantEquipUntil=0;if(!labGrantEquipSent)labGrantResult="grant_equip_timeout";return;}
        LabGrantItem g{};if(!readLabGrantItem(labGrantEquipKind,g))return;
        if(g.equipped){labGrantEquipUntil=0;labGrantResult="granted_item_equipped";return;}
        if(!labGrantEquipSent&&g.quantity>=1&&g.component){
            if(g.slotBusy){labGrantEquipUntil=0;labGrantResult="grant_slot_occupied";return;}
            labGrantEquipSent=true;reinterpret_cast<void(*)(void*,void*,bool)>(gameImage+0x614b20)(reinterpret_cast<void*>(g.inventory),reinterpret_cast<void*>(g.entry),true);
            labGrantResult="grant_equip_requested";}
        return;}
    if(!labGrantRequest)return;
    const unsigned r=labGrantRequest;labGrantRequest=0;
    if(!coopGameplayReady(outgoing)){++labGrantRefused;labGrantResult="refused_not_gameplay";return;}
    LabGrantItem g{};if(!readLabGrantItem(r,g)){++labGrantRefused;labGrantResult="grant_read_failed";return;}
    if(!g.inventory||!g.declaration||((r==4||r==5)&&!g.ammo)){++labGrantRefused;labGrantResult="no_inventory_or_declaration";return;}
    reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(g.inventory),reinterpret_cast<void*>(g.declaration),(r==6||r==7)?2:1);
    if(r==4||r==5)reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(g.inventory),reinterpret_cast<void*>(g.ammo),6);
    ++labGrants;static const char* names[]={"","granted_grenade","granted_health_potion","granted_mana_potion","granted_pistol","granted_crossbow","granted_stun_mine","granted_spring_razor","granted_heart"};labGrantResult=names[r];
    if(r!=2&&r!=3){labGrantEquipKind=r;labGrantEquipUntil=GetTickCount64()+3000;labGrantEquipSent=false;}
}
// 1214cm (#11 powers and upgrades coverage; lab only): "coop_grant_named <name>" adds one native inventory entry by its exact
// declaration name, restricted to powers (models/powers/...) and player attributes (upgrades, Agility's super_jump), through the
// same native add (+0x618de0); the entry list is the one the probe's --player-inventory reads. Takes reload the checkpoint after.
char labGrantName[192]{};bool labGrantNamedRequest=false;std::uint64_t labNamedGrants=0,labNamedRefused=0;const char* labNamedResult="idle";
// A granted power is then selected once in the left hand with the native equip (+0x614b20), as the item grants are.
std::uintptr_t labNamedEquipDecl=0;ULONGLONG labNamedEquipUntil=0;bool labNamedEquipSent=false;
bool labGrantNameAllowed(const char* n){const auto len=std::strlen(n);if(len<20||len>=sizeof(labGrantName))return false;
    for(std::size_t i=0;i<len;++i){const char c=n[i];if(!((c>='a'&&c<='z')||(c>='0'&&c<='9')||c=='_'||c=='/'||c=='.'))return false;}
    if(std::strstr(n,".."))return false;
    // 1214df (#11 knockdown, lab only): a player equipment upgrade (attributes/*.equipmentupgrade) too; explosive rounds
    // fire only with gun_explosive_bullets unlocked (1375a/b: the pistol kept its regular rounds, no burst).
    const bool item=len>=14&&!std::strcmp(n+len-14,".inventoryitem");
    const bool upgrade=len>=18&&!std::strcmp(n+len-18,".equipmentupgrade")&&!std::strncmp(n,"components/characters/player/base/attributes/",45);
    if(!item&&!upgrade)return false;
    // 1214dd (#11 knockdown, lab only): pistol ammunition too (components/equipment/ammo/bullet_explosive), whose
    // explosion carries PLAYER_HITREACT_KNOCKDOWN (bullet_explosive_damage, confirmed static); never equipped.
    return !std::strncmp(n,"models/powers/",14)||!std::strncmp(n,"components/characters/player/base/attributes/",45)||
           !std::strncmp(n,"components/equipment/ammo/",26);}
void serviceLabNamedEquip(){
    if(!labNamedEquipUntil||labGrantNamedRequest||!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    if(GetTickCount64()>=labNamedEquipUntil){labNamedEquipUntil=0;if(!labNamedEquipSent)labNamedResult="named_equip_timeout";return;}
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);NativePlayerSword s{};if(!readNativePlayerSword(memoryRead,base,s)||!s.sword.items||s.sword.itemCount>4096)return;
    for(unsigned i=0;i<s.sword.itemCount;++i){const auto entry=s.sword.items+i*56;std::uintptr_t decl=0;int quantity=0;
        if(!memoryRead(entry,&decl,8)||decl!=labNamedEquipDecl||!memoryRead(entry+32,&quantity,4))continue;
        if(quantity<1)return;
        if(!labNamedEquipSent){if(s.sword.slots[1]){labNamedEquipUntil=0;labNamedResult="named_slot_occupied";return;}
            labNamedEquipSent=true;reinterpret_cast<void(*)(void*,void*,bool)>(gameImage+0x614b20)(reinterpret_cast<void*>(s.sword.inventory),reinterpret_cast<void*>(entry),true);
            labNamedEquipUntil=0;labNamedResult="named_power_equip_requested";}
        return;}
}
void serviceLabNamedGrant(){
    serviceLabNamedEquip();
    if(!labGrantNamedRequest||!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    labGrantNamedRequest=false;const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(!coopGameplayReady(outgoing)){++labNamedRefused;labNamedResult="refused_not_gameplay";return;}
    NativePlayerSword s{};if(!readNativePlayerSword(memoryRead,base,s)||!s.sword.inventory||!s.sword.items||!s.sword.itemCount||s.sword.itemCount>4096){++labNamedRefused;labNamedResult="inventory_read_failed";return;}
    std::vector<std::array<unsigned char,56>> raw(s.sword.itemCount);if(!memoryRead(s.sword.items,raw.data(),raw.size()*56)){++labNamedRefused;labNamedResult="inventory_read_failed";return;}
    for(const auto& r:raw){std::uintptr_t decl=0,vt=0,name=0;std::string text;std::memcpy(&decl,r.data(),8);
        if(!decl||!memoryRead(decl,&vt,8)||(vt!=base+0x1b9ea48&&vt!=base+0x1b555a8)||!memoryRead(decl+8,&name,8)||!worldIdentityString(memoryRead,name,text))continue;
        if(text!=labGrantName)continue;
        reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(s.sword.inventory),reinterpret_cast<void*>(decl),1);
        ++labNamedGrants;labNamedResult="granted_named";
        if(!std::strncmp(labGrantName,"models/powers/",14)){labNamedEquipDecl=decl;labNamedEquipUntil=GetTickCount64()+3000;labNamedEquipSent=false;}
        return;}
    ++labNamedRefused;labNamedResult="name_not_in_inventory";
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
    servicePlayerHealthHold();serviceLabItemGrant();serviceLabNamedGrant();serviceLabItemUse();serviceLabPartnerEffect(tick);serviceLabKnockdown();
    {bool active=false;AcquireSRWLockShared(&observerCameraLock);active=observerCamera.active;ReleaseSRWLockShared(&observerCameraLock);
     if(active)observerCameraArms(true,tick);else if(observerHiddenArms[0].actor||observerHiddenArms[1].actor)observerCameraArms(false,tick);}
    const int request=observerCameraRequest;observerCameraRequest=-1;
    AcquireSRWLockExclusive(&observerCameraLock);
    if(request==0){observerCamera.active=false;observerCamera.result="off";}
    // 1214dh (#11, lab only): hold_at during an active hold moves the eye and keeps the held orientation, so a take can
    // follow a walking partner. A held view stops refreshing its native record, so the old path found no active view
    // and dropped the hold (the 1328zf flashes, carry takes 1277k-m losing the carrier).
    else if(request==2&&observerCamera.active){std::memcpy(observerCamera.position,observerCameraAt,12);observerCamera.until=tick+120000;observerCamera.result="holding_at";}
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
    fprintf(f,"\"itemGrant\":{\"grants\":%llu,\"refused\":%llu,\"result\":\"%s\",\"named\":{\"grants\":%llu,\"refused\":%llu,\"result\":\"%s\"}},",static_cast<unsigned long long>(labGrants),static_cast<unsigned long long>(labGrantRefused),labGrantResult,static_cast<unsigned long long>(labNamedGrants),static_cast<unsigned long long>(labNamedRefused),labNamedResult);
    {NativePlayerMana m{};const bool mr=readNativePlayerMana(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),m);
     fprintf(f,"\"labKnockdown\":{\"applied\":%llu,\"refused\":%llu,\"result\":\"%s\"},",static_cast<unsigned long long>(labKnockdowns),static_cast<unsigned long long>(labKnockdownRefused),labKnockdownResult);
     fprintf(f,"\"itemUse\":{\"uses\":%llu,\"refused\":%llu,\"result\":\"%s\",\"manaBefore\":%.3f,\"manaNow\":%.3f},",static_cast<unsigned long long>(labUses),static_cast<unsigned long long>(labUseRefused),labUseResult,labManaBefore,mr?m.current:-1.f);}
    fprintf(f,"\"healthHold\":{\"active\":%s,\"refills\":%llu,\"misses\":%llu,\"last\":%.3f}}\n",playerHealthHold?"true":"false",
        static_cast<unsigned long long>(playerHealthRefills),static_cast<unsigned long long>(playerHealthHoldMisses),playerHealthLast);const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
