#pragma once
#include <cstdlib>
// Developer-only, finite inventory setup for capacity edge cases. The relay
// continues to use LootObject; this setter is never a network reward handler.
std::string coinCapacityName;
int coinCapacityQuantity=-1;
bool coinCapacityOff=false;
struct PickupCapacityFixture {
    CoinPlayer player{};NativeCoin pickup{};CoinOutcomeValue before{};
    std::uint64_t epoch=0;ULONGLONG until=0,restoreUntil=0;
    bool restoring=false,restoreSent=false;
} pickupCapacityFixture;
std::uint64_t pickupCapacityCalls=0,pickupCapacityRestored=0,pickupCapacityRefused=0,pickupCapacityRetired=0;
const char* pickupCapacityResult="idle";
bool parseCoinCapacityCommand(const char* text,std::string& name,int& quantity){
    constexpr const char* prefix="coop_coin_sync capacity ";
    if(std::strncmp(text,prefix,24))return false;
    const std::string body(text+24);const auto split=body.rfind(' ');
    if(split==std::string::npos||!coinCanonicalName(body.substr(0,split)))return false;
    const auto digits=body.substr(split+1);
    if(digits.empty()||digits.size()>4||digits.find_first_not_of("0123456789")!=std::string::npos)return false;
    const int value=std::atoi(digits.c_str());if(value>1000)return false;
    name=body.substr(0,split);quantity=value;return true;
}
bool pickupCapacityApiMatches(){
    const unsigned char prefix[]={0x4c,0x8b,0xdc,0x57,0x48,0x83,0xec,0x60,0x49,0xc7,0x43,0xb8,0xfe,0xff,0xff,0xff,0x49,0x89,0x5b,0x10,0x49,0x89,0x6b,0x18,0x49,0x89,0x73,0x20,0x41,0x8b,0xf8,0x48};
    return !std::memcmp(gameImage+0x618de0,prefix,sizeof(prefix));
}
bool setPickupCapacity(const CoinPlayer& player,const NativeCoin& pickup,const CoinOutcomeValue& value,int quantity){
    CoinPlayer again{};CoinOutcomeValue before{},after{};
    if(!pickupCapacityApiMatches()||quantity<0||quantity>value.maximum||!readCoinPlayer(again)||!sameCoinWallet(player,again)||
       !readCoinOutcome(again,pickup,before)||!sameCoinOutcomeObjects(value,before))return false;
    if(before.value!=quantity){
        ++pickupCapacityCalls;
        reinterpret_cast<int(*)(void*,void*,int)>(gameImage+0x618de0)(reinterpret_cast<void*>(again.inventory),reinterpret_cast<void*>(pickup.item),quantity);
    }
    CoinPlayer check{};
    return readCoinPlayer(check)&&sameCoinWallet(again,check)&&check.money==again.money&&
        readCoinOutcome(check,pickup,after)&&sameCoinOutcomeObjects(before,after)&&after.value==quantity;
}
void servicePickupCapacityFixture(ULONGLONG tick){
    auto& f=pickupCapacityFixture;CoinWalletGuard wallet(true);if(!wallet.acquired)return;
    std::string request;request.swap(coinCapacityName);const int quantity=coinCapacityQuantity;coinCapacityQuantity=-1;
    const bool off=coinCapacityOff;coinCapacityOff=false;
    if(!request.empty()){
        auto found=std::find_if(coinSync.entries.begin(),coinSync.entries.end(),[&](const auto& c){return c.binding.name==request;});
        CoinPlayer p{};CoinOutcomeValue v{};
        if(f.until||!coinSync.ready||!coinReadyContext(tick)||!observerIsolation.ready||!observerIsolation.movementFixture||observerIsolation.until<tick+15000||
           movementBankActive()||shadowStream.until||reviewClipActive()||movementPotionFixtureBusy()||movementRangedFixtureBusy()||
           powerFixture.until||darkVisionFixture.until||windblastFixture.until||bendtimeFixture.until||agilityFixture.until||
           found==coinSync.entries.end()||(found->binding.type!=4&&found->binding.type!=14)||found->receipt.attempted||
           !readCoinPlayer(p)||!nativePlayerGroundIdleStates(p.states,reinterpret_cast<std::uintptr_t>(gameImage))||
           !readCoinOutcome(p,found->binding,v)||quantity<0||quantity>v.maximum){
            ++pickupCapacityRefused;pickupCapacityResult="scope_refused";
        }else{
            f={};f.player=p;f.pickup=found->binding;f.before=v;f.epoch=outgoing.worldEpoch;f.until=tick+30000;
            pickupCapacityResult=setPickupCapacity(p,f.pickup,v,quantity)?"native_capacity_ready":"native_setup_unconfirmed";
            if(std::strcmp(pickupCapacityResult,"native_capacity_ready")){f.restoring=true;f.restoreUntil=tick+5000;}
        }
    }
    if(!f.until)return;
    if(f.epoch!=outgoing.worldEpoch||f.player.states.player.game!=observedGame){f={};++pickupCapacityRetired;pickupCapacityResult="old_world_retired";return;}
    if(off||tick>=f.until||!coinSync.enabled||!observerIsolation.ready||!observerIsolation.movementFixture){
        if(!f.restoring){f.restoring=true;f.restoreUntil=tick+5000;}
    }
    if(!f.restoring)return;
    if(!coopGameplayReady(outgoing)){f.restoreUntil=tick+5000;return;}
    if(!f.restoreSent){
        f.restoreSent=true;
        if(setPickupCapacity(f.player,f.pickup,f.before,f.before.value)){f={};++pickupCapacityRestored;pickupCapacityResult="native_quantity_restored";return;}
        pickupCapacityResult="restore_unconfirmed_reload_isolated_checkpoint";
    }
    if(tick>=f.restoreUntil){f={};pickupCapacityResult="restore_unconfirmed_reload_isolated_checkpoint";}
}
void writePickupCapacityFixture(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.pickup-capacity.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* file=nullptr;if(fopen_s(&file,tmp,"wb")||!file)return;
    const auto& f=pickupCapacityFixture;const auto tick=GetTickCount64();
    std::fprintf(file,"{\"testFixtureOnly\":true,\"active\":%s,\"restoring\":%s,\"family\":%u,\"original\":%d,\"maximum\":%d,\"remainingMs\":%llu,\"calls\":%llu,\"restored\":%llu,\"refused\":%llu,\"retired\":%llu,\"result\":\"%s\"}\n",f.until?"true":"false",f.restoring?"true":"false",f.before.family,f.before.value,f.before.maximum,f.until>tick?f.until-tick:0,pickupCapacityCalls,pickupCapacityRestored,pickupCapacityRefused,pickupCapacityRetired,pickupCapacityResult);
    std::fclose(file);MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
