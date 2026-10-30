#pragma once
#include "native_coin.h"
#include "native_player_states.h"
#include "native_stack_item.h"
#include "native_food.h"
#include "coin_protocol.h"
using NativeCoinReward=void(*)(void*,void*,unsigned char);
NativeCoinReward realCoinReward=nullptr;
// Serialize only the native reward transaction, never an entire frame. Source
// callbacks may use worker threads; a recipient defers instead of waiting for
// another thread's reward. Same-thread native event reentry is recursive.
SRWLOCK coinWalletLock=SRWLOCK_INIT;
thread_local unsigned coinWalletDepth=0;
struct CoinWalletGuard {
    bool acquired=false;
    explicit CoinWalletGuard(bool tryOnly=false){
        if(coinWalletDepth){++coinWalletDepth;acquired=true;return;}
        if(tryOnly){if(!TryAcquireSRWLockExclusive(&coinWalletLock))return;}else AcquireSRWLockExclusive(&coinWalletLock);
        coinWalletDepth=1;acquired=true;
    }
    ~CoinWalletGuard(){if(acquired&&!--coinWalletDepth)ReleaseSRWLockExclusive(&coinWalletLock);}
    CoinWalletGuard(const CoinWalletGuard&)=delete;CoinWalletGuard& operator=(const CoinWalletGuard&)=delete;
};
unsigned char coinRewardPatch[16]{};
constexpr unsigned char coinRewardPrefix[]={0x44,0x88,0x44,0x24,0x18,0x55,0x56,0x57,0x41,0x54,0x41,0x55,0x41,0x56,0x41,0x57,0x48,0x81,0xec,0xb0,0,0,0,0x48,0xc7,0x44,0x24,0x68,0xfe,0xff,0xff,0xff};
// 1214jq (#189): code nativeCoinApiAvailable() requires verbatim (32 bytes from each). No other hook may patch it:
// 1214jl's entity event trace hooked 0x1408848a0, which refused every shared pickup binding (native_api) until 1214jq.
constexpr std::uintptr_t kCoinVerbatimRvas[]={0x841040,0x76e2b0,0x76ec20,0x8848a0};
bool nativeCoinApiAvailable(){
    const unsigned char claim[]={0x48,0x89,0x5c,0x24,8,0x48,0x89,0x6c,0x24,0x10,0x48,0x89,0x74,0x24,0x18,0x57,0x48,0x83,0xec,0x40,0x48,0x8b,0x79,0x10,0x48,0x8b,0x69,8,0x48,0x8b,0xd9,0x48};
    const unsigned char finish[]={0x40,0x53,0x48,0x83,0xec,0x20,0x80,0xa1,0xd0,0,0,0,0xfd,0x48,0x8b,0xd9,0x84,0xd2,0x74,0x25,0x48,0x8b,0x49,0x10,0x48,0x8b,1,0xff,0x90,0x80,0,0};
    const unsigned char eventPrefix[]={0x48,0x8b,0xc4,0x4c,0x89,0x48,0x20,0x48,0x89,0x50,0x10,0x57,0x48,0x83,0xec,0x40,0x48,0xc7,0x40,0xf0,0xfe,0xff,0xff,0xff,0x48,0x89,0x58,8,0x48,0x89,0x70,0x18};
    if(!realCoinReward||std::memcmp(gameImage+0x841030,coinRewardPatch,16)||std::memcmp(gameImage+0x841040,coinRewardPrefix+16,16)||
       std::memcmp(gameImage+0x76e2b0,claim,sizeof(claim))||std::memcmp(gameImage+0x76ec20,finish,sizeof(finish))||std::memcmp(gameImage+0x8848a0,eventPrefix,sizeof(eventPrefix)))return false;
    const auto event=reinterpret_cast<std::uintptr_t>(gameImage)+0x39acd40;std::uintptr_t p=0;std::string text;unsigned args=0;
    const auto check=[&](unsigned offset,const char* expected){return memoryRead(event+offset,&p,8)&&worldIdentityString(memoryRead,p,text)&&text==expected;};
    return check(0,"pickedUp")&&check(8,"bb")&&check(0x10,"bool;bool;")&&check(0x18,"_isSuccesful;_fromGrapple;")&&memoryRead(event+0x24,&args,4)&&args==2;
}
struct CoinPlayer {NativePlayerStates states;std::uintptr_t loot=0,inventory=0;unsigned generation=0;int money=0;};
bool readCoinPlayer(CoinPlayer& out,std::uintptr_t expectedGame=observedGame){
    CoinPlayer s{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t vt=0,owner=0;unsigned id=0,flags=0;
    if(!readNativePlayerStates(memoryRead,base,s.states)||!s.states.defaultMode||s.states.player.logic!=1||s.states.player.game!=expectedGame||
       !memoryRead(s.states.mode+0x10+47*8,&s.loot,8)||!s.loot||!memoryRead(s.loot,&vt,8)||vt!=base+0x1c7bcc8||
       !memoryRead(s.loot+8,&id,4)||id!=47||!memoryRead(s.loot+0x10,&owner,8)||owner!=s.states.component||
       !memoryRead(s.states.component+0x328,&s.inventory,8)||!s.inventory||!memoryRead(s.inventory,&vt,8)||vt!=base+0x1b9a878||
       !memoryRead(s.inventory+0x10,&owner,8)||owner!=s.states.player.player||!memoryRead(s.inventory+0x18,&s.generation,4)||s.generation==~0u||
       !memoryRead(s.inventory+0x1c,&flags,4)||(flags&12)!=8||!memoryRead(s.inventory+0x16c,&s.money,4)||s.money<0||s.money>1000000)return false;
    out=std::move(s);return true;
}
bool sameCoinWallet(const CoinPlayer& a,const CoinPlayer& b){return a.states.player.game==b.states.player.game&&a.states.player.player==b.states.player.player&&
    a.states.component==b.states.component&&a.loot==b.loot&&a.inventory==b.inventory&&a.generation==b.generation;}
struct CoinOutcomeValue {NativeStackItem stack{};NativeFood food{};unsigned family=0;int value=0,maximum=0;};
bool readCoinOutcome(const CoinPlayer& player,const NativeCoin& pickup,CoinOutcomeValue& out){
    CoinOutcomeValue v{};v.family=pickup.type;
    if(v.family==1){v.value=player.money;v.maximum=1000000;}
    else if(v.family==6){
        if(!readNativeFood(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),player.inventory,player.states.player.player,pickup.item,v.food)||v.food.stack.name!=pickup.itemName)return false;
        v.value=v.food.value.stored;v.maximum=v.food.value.storageMaximum;
    }else {
        if(!readNativeStackItem(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),player.inventory,player.states.player.player,pickup.item,pickup.type,v.stack)||v.stack.name!=pickup.itemName)return false;
        v.value=v.stack.quantity;v.maximum=v.stack.maximum;
    }
    out=std::move(v);return true;
}
bool sameCoinOutcomeObjects(const CoinOutcomeValue& a,const CoinOutcomeValue& b){return a.family==b.family&&a.maximum==b.maximum&&(a.family==1||(a.family==6?sameNativeFood(a.food,b.food):sameNativeStackItem(a.stack,b.stack)));}
bool validCoinOutcome(const CoinOutcomeValue& a,const CoinOutcomeValue& b,int qty){return a.family==6?validFoodOutcome(a.food.value,b.food.value,qty):pickupOutcomeValid(a.value,b.value,qty,a.family,a.maximum);}
enum class CoinApply {Deferred,Complete,Failed};
CoinApply applyNativeCoin(const NativeCoin& binding,CoinAwardReceipt& receipt,unsigned& calls,bool exerciseObserver=false){
    CoinWalletGuard wallet(true);if(!wallet.acquired)return CoinApply::Deferred;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);NativeCoin n{};CoinPlayer p{};
    if(receipt.awarded)return CoinApply::Complete;
    if(receipt.attempted||!nativeCoinApiAvailable()||!readNativeCoin(memoryRead,base,observedGame,binding.handle,n,true)||!sameNativeCoin(n,binding)||n.ownerFlags&1||n.flags&8)return CoinApply::Failed;
    // Native LootObject owns any claim already in flight. Wait for its exact
    // reward hook, never interpret disappearance or a wallet poll as success.
    if(n.flags&2)return CoinApply::Deferred;
    if(n.flags)return CoinApply::Failed;
    if(!readCoinPlayer(p)||!nativePlayerGroundIdleStates(p.states,base))return CoinApply::Deferred;
    CoinOutcomeValue beforeValue{};
    if(!readCoinOutcome(p,n,beforeValue)||!(n.type==6?receipt.reserveFood(beforeValue.food.value,n.quantity):receipt.reserve(beforeValue.value,n.quantity,n.type,beforeValue.maximum)))return CoinApply::Failed;
    ++calls;reinterpret_cast<void(*)(void*)>(gameImage+0x76e2b0)(reinterpret_cast<void*>(n.component));
    unsigned char flags=0;if(!memoryRead(n.component+0xd0,&flags,1)||!(flags&2)||flags&8)return CoinApply::Failed;
    // The isolated catalog exercise deliberately traverses the installed
    // source observer. Normal recipient application bypasses it to avoid echo.
    ++calls;const auto reward=exerciseObserver?reinterpret_cast<NativeCoinReward>(gameImage+0x841030):realCoinReward;
    reward(reinterpret_cast<void*>(p.loot),reinterpret_cast<void*>(n.component),0);
    CoinPlayer afterPlayer{};CoinOutcomeValue afterValue{};
    if(!readCoinPlayer(afterPlayer)||!sameCoinWallet(p,afterPlayer)||!readCoinOutcome(afterPlayer,n,afterValue)||!sameCoinOutcomeObjects(beforeValue,afterValue)||
       (n.type!=1&&afterPlayer.money!=p.money)||!validCoinOutcome(beforeValue,afterValue,n.quantity))return CoinApply::Failed;
    const int after=afterValue.value;
    struct EventValue {unsigned char bytes[24]{};};EventValue success{},grapple{},result{};success.bytes[0]=grapple.bytes[0]=0x69;const int yes=1;std::memcpy(success.bytes+8,&yes,4);
    ++calls;reinterpret_cast<void*(*)(void*,void*,void*,void*,void*)>(gameImage+0x8848a0)(reinterpret_cast<void*>(n.actor),&result,gameImage+0x39acd40,&success,&grapple);
    ++calls;reinterpret_cast<void(*)(void*)>(gameImage+0x882fc0)(&result);
    ++calls;reinterpret_cast<void(*)(void*,unsigned char)>(gameImage+0x76ec20)(reinterpret_cast<void*>(n.component),1);
    if(!memoryRead(n.component+0xd0,&flags,1)||!(flags&8)||flags&2)return CoinApply::Failed;
    return (n.type==6?receipt.finishFood(afterValue.food.value):receipt.finish(after))?CoinApply::Complete:CoinApply::Failed;
}
// Stage 8.4.2 (#238): story items reach both players. A story pickup is a key (item family 17), a note (12), the rewire
// tool (22) or a map (24, 27); every other family (money and valuables, ammunition, food, runes, elixirs, paintings,
// blueprints, books, whalebone) is each player's own (pickup census, edge of the world, items-per-player-v2033a time).
inline bool storyPickupFamily(unsigned family){return family==12||family==17||family==22||family==24||family==27;}
// The same exact pickup declaration as readNativeCoin, for a story family (any special index; quantity 1 to 1000).
template<class Read> bool readNativeStoryPickup(Read read,std::uintptr_t base,std::uintptr_t game,unsigned handle,NativeCoin& out){
    NativeCoin s{};s.game=game;s.handle=handle;std::uintptr_t vt=0,p=0,list=0;unsigned count=0;
    const auto eq=[&](std::uintptr_t at,std::uintptr_t value){return read(at,&vt,8)&&vt==value;};
    if(!game||!eq(game,base+0x1c940a8)||!(s.actor=worldIdentityEntity(read,game,handle))||!eq(s.actor,base+0x1c5e258)||
       !read(s.actor+0x20,&p,8)||!worldIdentityString(read,p,s.name)||
       !read(s.actor+0x268,&list,8)||!list||!read(s.actor+0x270,&count,4)||!(count&0xffffff)||(count&0xffffff)>256)return false;
    std::array<std::uintptr_t,256> members{};if(!read(list,members.data(),(count&0xffffff)*8))return false;
    for(unsigned i=0;i<(count&0xffffff);++i){if(!members[i]||!read(members[i],&vt,8))return false;if(vt==base+0x1c5a3b8){if(s.component)return false;s.component=members[i];}}
    unsigned char carried=0;
    if(!s.component||!eq(s.component+0x10,s.actor)||!read(s.component+0x18,&s.generation,4)||s.generation==~0u||
       !read(s.component+0x1c,&s.componentFlags,4)||(s.componentFlags&12)!=8||
       !read(s.component+8,&s.declaration,8)||!s.declaration||!eq(s.declaration,base+0x1c5a278)||
       !read(s.declaration+0xa0,&carried,1)||carried||!read(s.declaration+0xf8,&s.item,8)||!s.item||!eq(s.item,base+0x1b9ea48)||
       !read(s.item+8,&p,8)||!worldIdentityString(read,p,s.itemName)||!read(s.item+0x60,&s.type,4)||!storyPickupFamily(s.type)||
       !read(s.declaration+0x100,&s.quantity,4)||s.quantity<1||s.quantity>1000||
       !read(s.component+0xd0,&s.flags,1)||s.flags&0xf0||!read(s.actor+0xd0,&s.ownerFlags,1)||
       !read(s.actor+0x300,s.position.data(),12))return false;
    for(float f:s.position)if(!std::isfinite(f))return false;
    out=std::move(s);return true;
}
// The story pickup taken natively by this world's player, as the peer's player took it: claim, native reward, the
// pickedUp event and finish, the steps applyNativeCoin takes. A story item has no wallet outcome to check; the pickup's
// own flags must show the claim and then the finish.
CoinApply applyNativeStoryPickup(const NativeCoin& binding,unsigned& calls){
    CoinWalletGuard wallet(true);if(!wallet.acquired)return CoinApply::Deferred;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);NativeCoin n{};CoinPlayer p{};
    if(!nativeCoinApiAvailable()||!readNativeStoryPickup(memoryRead,base,observedGame,binding.handle,n)||!sameNativeCoin(n,binding)||n.ownerFlags&1||n.flags&8)return CoinApply::Failed;
    if(n.flags&2)return CoinApply::Deferred;
    if(n.flags)return CoinApply::Failed;
    if(!readCoinPlayer(p)||!nativePlayerGroundIdleStates(p.states,base))return CoinApply::Deferred;
    ++calls;reinterpret_cast<void(*)(void*)>(gameImage+0x76e2b0)(reinterpret_cast<void*>(n.component));
    unsigned char flags=0;if(!memoryRead(n.component+0xd0,&flags,1)||!(flags&2)||flags&8)return CoinApply::Failed;
    ++calls;realCoinReward(reinterpret_cast<void*>(p.loot),reinterpret_cast<void*>(n.component),0);
    struct EventValue {unsigned char bytes[24]{};};EventValue success{},grapple{},result{};success.bytes[0]=grapple.bytes[0]=0x69;const int yes=1;std::memcpy(success.bytes+8,&yes,4);
    ++calls;reinterpret_cast<void*(*)(void*,void*,void*,void*,void*)>(gameImage+0x8848a0)(reinterpret_cast<void*>(n.actor),&result,gameImage+0x39acd40,&success,&grapple);
    ++calls;reinterpret_cast<void(*)(void*)>(gameImage+0x882fc0)(&result);
    ++calls;reinterpret_cast<void(*)(void*,unsigned char)>(gameImage+0x76ec20)(reinterpret_cast<void*>(n.component),1);
    if(!memoryRead(n.component+0xd0,&flags,1)||!(flags&8)||flags&2)return CoinApply::Failed;
    return CoinApply::Complete;
}
