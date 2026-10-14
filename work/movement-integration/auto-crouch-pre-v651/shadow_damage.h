#pragma once
#include "friendly_fire.h"
#include "carry_damage_identity.h"
// Presentation damage firewall. No packet can nominate an arbitrary target:
// only this locally spawned, reciprocal custom clone incarnation is excluded.
std::atomic<std::uintptr_t> shadowDamageActor{0},shadowDamageGame{0};
std::atomic<std::uint32_t> shadowDamageHandle{0};
std::atomic<std::uint64_t> shadowDamageBlocked{0};
std::atomic<std::uint64_t> friendlyFireLocalBlocked{0},friendlyFirePartnerBlocked{0};
SRWLOCK carryDamageLock=SRWLOCK_INIT;
CarryDamageLease carryDamageLease{};
std::atomic<std::uint64_t> carryDamageBlocked{0};
CoopDamageAvatar shadowDamageIdentity(){
    CoopDamageAvatar a{};a.actor=shadowDamageActor.load(std::memory_order_acquire);
    a.game=shadowDamageGame.load();a.handle=shadowDamageHandle.load();
    if(a.actor!=shadowDamageActor.load(std::memory_order_acquire))return {};
    return a;
}
void retireShadowDamageIdentity(){shadowDamageActor.store(0,std::memory_order_release);shadowDamageHandle.store(0);shadowDamageGame.store(0);}
void registerCarryDamageLease(std::uintptr_t actor,std::uint32_t handle,ULONGLONG until){
    const CarryDamageLease next{shadowDamageIdentity(),actor,handle,until};
    AcquireSRWLockExclusive(&carryDamageLock);carryDamageLease=next;ReleaseSRWLockExclusive(&carryDamageLock);
}
void finishCarryDamageLease(std::uintptr_t actor,std::uint32_t handle,ULONGLONG tick){
    AcquireSRWLockExclusive(&carryDamageLock);
    if(carryDamageLease.actor==actor&&carryDamageLease.handle==handle)carryDamageLease.until=tick+2000;
    ReleaseSRWLockExclusive(&carryDamageLock);
}
void retireCarryDamageLease(){
    AcquireSRWLockExclusive(&carryDamageLock);carryDamageLease={};ReleaseSRWLockExclusive(&carryDamageLock);
}
bool suppressCarryPresentationDamage(void* info,void* result){
    const unsigned char ctor[]={0x33,0xc0,0x48,0x89,0x01,0x89,0x41,0x08,0x48,0x89,0x41,0x10};
    std::uint32_t handles[2]{};CarryDamageLease snapshot{};
    if(!info||!result||std::memcmp(gameImage+0x780470,ctor,sizeof(ctor))||
       !memoryRead(reinterpret_cast<std::uintptr_t>(info)+0x50,handles,sizeof(handles)))return false;
    AcquireSRWLockShared(&carryDamageLock);snapshot=carryDamageLease;ReleaseSRWLockShared(&carryDamageLock);
    if(!isCarryPresentationDamage(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),snapshot,GetTickCount64(),handles[0],handles[1]))return false;
    // The body is replaying remote movement. Its incidental local impact must
    // not invent a second gameplay outcome. A two-second release tail covers
    // the verified ragdoll impact; unrelated bodies and expired leases pass.
    reinterpret_cast<void(*)(void*)>(gameImage+0x780470)(result);++carryDamageBlocked;return true;
}
bool suppressFriendlyFire(void* component,void* info,void* result){
    std::uint32_t handles[2]{};
    const unsigned char ctor[]={0x33,0xc0,0x48,0x89,0x01,0x89,0x41,0x08,0x48,0x89,0x41,0x10};
    if(!info||!result||std::memcmp(gameImage+0x780470,ctor,sizeof(ctor))||
       !memoryRead(reinterpret_cast<std::uintptr_t>(info)+0x50,handles,sizeof(handles)))return false;
    const auto decision=classifyCoopFriendlyHit(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),shadowDamageIdentity(),
        reinterpret_cast<std::uintptr_t>(component),handles[0],handles[1]);
    if(decision==CoopFriendlyHit::None)return false;
    // Native empty result, before health, reactions and queued damage. No
    // health restoration, faction changes, global god mode or wire target IDs.
    reinterpret_cast<void(*)(void*)>(gameImage+0x780470)(result);
    if(decision==CoopFriendlyHit::LocalToPartner)++friendlyFireLocalBlocked;else ++friendlyFirePartnerBlocked;
    return true;
}
bool shadowOwnedDamageSource(std::uint32_t handle){
    const auto actor=shadowDamageActor.load(std::memory_order_acquire);
    const auto game=shadowDamageGame.load();const auto ownedHandle=shadowDamageHandle.load();
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    std::uintptr_t activeGame=0,vt=0,definition=0,name=0;std::string path;
    if(!actor||!game||!ownedHandle||!memoryRead(base+0x39a5290,&activeGame,8)||activeGame!=game||
       worldIdentityEntity(memoryRead,game,ownedHandle)!=actor||!memoryRead(actor,&vt,8)||vt!=base+0x1c5e108||
       !memoryRead(actor+0x2e8,&definition,8)||!definition||!memoryRead(definition+8,&name,8)||
       !worldIdentityString(memoryRead,name,path)||(path!="models/characters/def/d2coop/doppelganger_emily.def"&&path!="models/characters/def/d2coop/doppelganger_corvo.def"))return false;
    if(handle==ownedHandle)return true;
    // Bound parent walk, resolving generation-bearing handles at each step.
    for(unsigned depth=0;depth<4&&handle&&handle!=1;++depth){
        const auto child=worldIdentityEntity(memoryRead,game,handle);std::uint32_t parent=0;
        if(!child||!memoryRead(child+0xac,&parent,4)||parent==handle)return false;
        if(parent==ownedHandle)return true;
        handle=parent;
    }
    return false;
}
bool suppressShadowDamage(void* info,void* result){
    std::uint32_t handles[2]{};
    const unsigned char ctor[]={0x33,0xc0,0x48,0x89,0x01,0x89,0x41,0x08,0x48,0x89,0x41,0x10};
    if(!info||!result||std::memcmp(gameImage+0x780470,ctor,sizeof(ctor))||
       !memoryRead(reinterpret_cast<std::uintptr_t>(info)+0x50,handles,sizeof(handles))||
       (!shadowOwnedDamageSource(handles[0])&&!shadowOwnedDamageSource(handles[1])))return false;
    // Use the native result constructor; skip health, reactions, death and
    // queued damage entirely. Authoritative actual-player damage remains native.
    reinterpret_cast<void(*)(void*)>(gameImage+0x780470)(result);
    ++shadowDamageBlocked;return true;
}
