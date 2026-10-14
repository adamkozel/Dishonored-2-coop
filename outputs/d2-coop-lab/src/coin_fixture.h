#pragma once
#include "native_coin.h"
#include "native_player_sword.h"
#include "native_player_states.h"

// Bounded native reward/removal experiment only. No network packet or shared
// currency policy. The isolated checkpoint owns cleanup. Never retry a
// partially applied reward, and never write currency or picked flags directly.
bool coinFixtureRequest=false;
struct CoinFixture {
    bool completed=false;
    unsigned attempts=0,calls=0,refused=0,noops=0,handle=0,generation=0;
    std::uint64_t epoch=0;
    int before=0,after=0;
    unsigned flags=0;
    const char* result="idle";
} coinFixture;
void writeCoinFixture(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.coin-fixture.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;const auto& s=coinFixture;
    std::fprintf(f,"{\"localExperiment\":true,\"attempts\":%u,\"nativeCalls\":%u,\"refused\":%u,\"noops\":%u,\"handle\":%u,\"generation\":%u,\"epoch\":%llu,\"before\":%d,\"after\":%d,\"flags\":%u,\"result\":\"%s\"}\n",s.attempts,s.calls,s.refused,s.noops,s.handle,s.generation,s.epoch,s.before,s.after,s.flags,s.result);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
void serviceCoinFixture(ULONGLONG tick){
    if(!coinFixtureRequest||!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    coinFixtureRequest=false;auto& s=coinFixture;const auto refuse=[&](const char* why){++s.refused;s.result=why;writeCoinFixture();};
    const unsigned char grant[]={0x44,0x88,0x44,0x24,0x18,0x55,0x56,0x57,0x41,0x54,0x41,0x55,0x41,0x56,0x41,0x57,0x48,0x81,0xec,0xb0,0,0,0,0x48,0xc7,0x44,0x24,0x68,0xfe,0xff,0xff,0xff};
    const unsigned char claim[]={0x48,0x89,0x5c,0x24,8,0x48,0x89,0x6c,0x24,0x10,0x48,0x89,0x74,0x24,0x18,0x57,0x48,0x83,0xec,0x40,0x48,0x8b,0x79,0x10,0x48,0x8b,0x69,8,0x48,0x8b,0xd9,0x48};
    const unsigned char finish[]={0x40,0x53,0x48,0x83,0xec,0x20,0x80,0xa1,0xd0,0,0,0,0xfd,0x48,0x8b,0xd9,0x84,0xd2,0x74,0x25,0x48,0x8b,0x49,0x10,0x48,0x8b,1,0xff,0x90,0x80,0,0};
    if(std::strcmp(mapName,kGuardMap)||!coopGameplayReady(outgoing)||!coopGameplayReady(remote)||!lastReceive||tick-lastReceive>=250||
       !observerIsolation.ready||!observerIsolation.movementFixture||observerIsolation.until<tick+5000||
       movementBankActive()||shadowStream.until||reviewClipActive()||
       !nativeCoinApiAvailable()||std::memcmp(gameImage+0x76e2b0,claim,sizeof(claim))||std::memcmp(gameImage+0x76ec20,finish,sizeof(finish))){refuse("native_scope_required");return;}
    // Native finish removes the actor from the registry. Keep a world-scoped
    // receipt before looking it up again; absent memory is never an invitation
    // to replay the award. This command names only one fixed authored coin.
    if(s.attempts&&s.epoch==outgoing.worldEpoch){
        if(s.completed){++s.noops;s.result="already_collected";writeCoinFixture();}
        else refuse("prior_attempt_no_retry");
        return;
    }
    const unsigned char eventPrefix[]={0x48,0x8b,0xc4,0x4c,0x89,0x48,0x20,0x48,0x89,0x50,0x10,0x57,0x48,0x83,0xec,0x40,0x48,0xc7,0x40,0xf0,0xfe,0xff,0xff,0xff,0x48,0x89,0x58,8,0x48,0x89,0x70,0x18};
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);const auto event=base+0x39acd40;
    std::uintptr_t name=0;std::string text;unsigned args=0;
    const auto eventText=[&](unsigned offset,const char* expected){return memoryRead(event+offset,&name,8)&&worldIdentityString(memoryRead,name,text)&&text==expected;};
    if(std::memcmp(gameImage+0x8848a0,eventPrefix,sizeof(eventPrefix))||!eventText(0,"pickedUp")||!eventText(8,"bb")||!eventText(0x10,"bool;bool;")||!eventText(0x18,"_isSuccesful;_fromGrapple;")||!memoryRead(event+0x24,&args,4)||args!=2){refuse("native_pickup_event_identity_required");return;}
    NativePlayerSword player{};NativePlayerStates states{};
    if(!readNativePlayerSword(memoryRead,base,player)||!readNativePlayerStates(memoryRead,base,states)||states.player.player!=player.sword.actor||states.player.game!=player.sword.game||!states.defaultMode||states.player.logic!=1||states.player.current>1||states.active.size()!=1||
       states.active[0].id!=(states.player.current==0?0u:6u)){refuse("idle_local_player_required");return;}
    std::uintptr_t lootState=0,vt=0,owner=0,inventory=0;unsigned id=0;
    if(!memoryRead(states.mode+0x10+47*8,&lootState,8)||!lootState||!memoryRead(lootState,&vt,8)||vt!=base+0x1c7bcc8||
       !memoryRead(lootState+8,&id,4)||id!=47||!memoryRead(lootState+0x10,&owner,8)||owner!=states.component||
       !memoryRead(states.component+0x328,&inventory,8)||inventory!=player.sword.inventory){refuse("loot_state_owner_changed");return;}
    std::uintptr_t list=0;unsigned count=0;
    if(!memoryRead(observedGame+0x601c0,&list,8)||!list||!memoryRead(observedGame+0x601c8,&count,4)||!(count&0xffffff)||(count&0xffffff)>16384){refuse("registry_unavailable");return;}
    std::vector<unsigned> handles(count&0xffffff);if(!memoryRead(list,handles.data(),handles.size()*4)){refuse("registry_unreadable");return;}
    NativeCoin selected{};unsigned matches=0;
    for(auto handle:handles){const auto actor=worldIdentityEntity(memoryRead,observedGame,handle);std::uintptr_t name=0;std::string text;if(!actor||!memoryRead(actor,&vt,8)||vt!=base+0x1c5e258||!memoryRead(actor+0x20,&name,8)||!worldIdentityString(memoryRead,name,text)||text!="pickups_coin_emily_lowvalue_4")continue;++matches;if(!readNativeCoin(memoryRead,base,observedGame,handle,selected)){refuse("coin_identity_unavailable");return;}}
    if(matches!=1||selected.itemName!="models/interactive/valuables/coins/coin1_lowvalue.inventoryitem"||selected.quantity!=1){refuse("exact_authored_coin_required");return;}
    if(selected.flags&8){++s.noops;s.result="already_collected";writeCoinFixture();return;}
    if(s.attempts&&s.epoch==outgoing.worldEpoch&&s.handle==selected.handle&&s.generation==selected.generation){refuse("prior_attempt_no_retry");return;}
    NativeCoin fresh{};int money=0;
    if(selected.flags||selected.ownerFlags&1||!readNativeCoin(memoryRead,base,observedGame,selected.handle,fresh)||!sameNativeCoin(fresh,selected)||fresh.flags||fresh.ownerFlags&1||
       !memoryRead(inventory+0x16c,&money,4)||money<0||money>1000000){refuse("fresh_unclaimed_coin_required");return;}
    s.handle=selected.handle;s.generation=selected.generation;s.epoch=outgoing.worldEpoch;s.before=s.after=money;s.flags=selected.flags;++s.attempts;s.completed=false;s.result="native_claim_started";writeCoinFixture();
    ++s.calls;reinterpret_cast<void(*)(void*)>(gameImage+0x76e2b0)(reinterpret_cast<void*>(fresh.component));
    unsigned char flags=0;if(!memoryRead(fresh.component+0xd0,&flags,1)||!(flags&2)||flags&8){refuse("native_claim_unconfirmed");return;}
    ++s.calls;realCoinReward(reinterpret_cast<void*>(lootState),reinterpret_cast<void*>(fresh.component),0);
    if(!memoryRead(inventory+0x16c,&s.after,4)||s.after!=money+1){refuse("native_reward_unconfirmed_no_retry");return;}
    // Match LootObject841500/83fe70: two 24-byte native variants (type'i',
    // int payload at+8), pickedUp(true,false), then release the return variant.
    // The native wrapper owns destruction of its two input variants.
    struct EventValue {unsigned char bytes[24]{};};
    EventValue success{},fromGrapple{},result{};success.bytes[0]=fromGrapple.bytes[0]=0x69;const int yes=1;std::memcpy(success.bytes+8,&yes,4);
    ++s.calls;reinterpret_cast<void*(*)(void*,void*,void*,void*,void*)>(gameImage+0x8848a0)(reinterpret_cast<void*>(fresh.actor),&result,reinterpret_cast<void*>(event),&success,&fromGrapple);
    ++s.calls;reinterpret_cast<void(*)(void*)>(gameImage+0x882fc0)(&result);
    ++s.calls;reinterpret_cast<void(*)(void*,unsigned char)>(gameImage+0x76ec20)(reinterpret_cast<void*>(fresh.component),1);
    const bool done=memoryRead(fresh.component+0xd0,&flags,1)&&(flags&8)&&!(flags&2);s.flags=flags;s.completed=done;s.result=done?"native_coin_reward_event_and_removal":"native_removal_unconfirmed_no_retry";writeCoinFixture();
}
