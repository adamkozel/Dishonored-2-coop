#pragma once
#include "world_protocol.h"
#include "coin_limits.h"
#include "food_outcome.h"
enum class CoinMessage:std::uint32_t {Hello=0,Request=1,Commit=2,Ack=3};
// Version4 adds native food outcomes to the ordinary pickup catalog.
// Version7 (1059c): the shared catalog is the intersection of both peers'
// identity lists exchanged once in CoopCoinCatalogPacket (character-specific
// script pickups stay local). Hello becomes a heartbeat whose catalogId and
// catalogCount carry the sender's shared digest and size.
// Catalog digest excludes local pointers/handles and includes sorted authored
// names, item declarations, quantities and native families. It is identity,
// not authentication.
#pragma pack(push,1)
struct CoopCoinPacket {
    std::uint32_t magic=0x43433244;
    std::uint16_t version=7,size=sizeof(CoopCoinPacket);
    std::uint32_t build=0x67a50cde,role=0;
    CoinMessage kind=CoinMessage::Hello;std::uint32_t quantity=0;
    std::uint32_t catalogCount=0,family=1;
    std::uint64_t catalogId=0;
    std::uint64_t sequence=0,sourceSession=0,targetSession=0,sourceEpoch=0,targetEpoch=0,mapId=0,sourceArm=0,targetArm=0,revision=0;
    char name[192]{},item[192]{};
};
struct CoopCoinCatalogEntry {std::uint64_t identity=0,arm=0;};
struct CoopCoinCatalogPacket {
    std::uint32_t magic=0x4C433244;
    std::uint16_t version=1,size=sizeof(CoopCoinCatalogPacket);
    std::uint32_t build=0x67a50cde,role=0,count=0,reserved=0;
    std::uint64_t sequence=0,sourceSession=0,targetSession=0,sourceEpoch=0,targetEpoch=0,mapId=0;
    CoopCoinCatalogEntry entries[kCoinCatalogMax]{};
};
#pragma pack(pop)
static_assert(sizeof(CoopCoinPacket)==496);
static_assert(sizeof(CoopCoinCatalogPacket)==72+16*kCoinCatalogMax);
inline bool coinQuantityValid(int qty){return qty>=1&&qty<=1000;}
inline bool coinMoneyDelta(int before,int after,int qty){return coinQuantityValid(qty)&&before>=0&&before<=1000000-qty&&after==before+qty;}
inline bool pickupFamilyValid(unsigned family){return family==1||family==4||family==14||family==6;}
inline bool pickupOutcomeValid(int before,int after,int qty,unsigned family,int maximum){
    if(family==1)return maximum==1000000&&coinMoneyDelta(before,after,qty);
    // Native stack modifiers may alter the requested amount. Verify the
    // observed positive bounded grant, or a zero grant at full capacity.
    return (family==4||family==14)&&coinQuantityValid(qty)&&maximum>=1&&maximum<=1000&&before>=0&&before<=maximum&&
        after>=before&&after<=maximum&&(after>before||before==maximum);
}
inline bool validCoopCoin(const CoopCoinPacket& p,const CoopPosePacket& local,const CoopPosePacket& peer){
    if(p.magic!=0x43433244||p.version!=7||p.size!=sizeof(p)||p.build!=0x67a50cde||p.role>1||p.role!=peer.role||
       !p.sequence||!p.sourceArm||!coinQuantityValid(static_cast<int>(p.quantity))||!p.catalogId||!p.catalogCount||p.catalogCount>kCoinCatalogMax||!pickupFamilyValid(p.family)||
       !coopWorldPairReady(local,peer)||p.sourceSession!=peer.session||
       p.targetSession!=local.session||p.sourceEpoch!=peer.worldEpoch||p.targetEpoch!=local.worldEpoch||p.mapId!=local.mapId||
       !coopWorldName(p.name)||!coopWorldName(p.item))return false;
    if(p.kind==CoinMessage::Hello)return !p.targetArm&&!p.revision;
    if(!p.targetArm)return false;
    return (p.kind==CoinMessage::Request&&p.role==1&&!p.revision)||
        (p.kind==CoinMessage::Commit&&p.role==0&&p.revision==1)||
        (p.kind==CoinMessage::Ack&&p.role==1&&p.revision==1);
}
// Reserve before any native mutation. A partial application is terminal for
// this incarnation. Simultaneous ordinary pickups already credited locally
// acknowledge the same host commit without another reward.
struct CoinAwardReceipt {
    bool attempted=false,awarded=false,nativeSource=false;
    int before=0,after=0,quantity=0,maximum=1000000;unsigned family=1;
    PickupFoodValue foodBefore{},foodAfter{};
    bool reserveFood(const PickupFoodValue& b,int qty){
        if(attempted||awarded||!coinQuantityValid(qty)||!validFoodValue(b))return false;
        attempted=true;family=6;before=after=b.stored;maximum=b.storageMaximum;quantity=qty;foodBefore=foodAfter=b;return true;
    }
    bool finishFood(const PickupFoodValue& a){
        if(!attempted||awarded||nativeSource||family!=6||!validFoodOutcome(foodBefore,a,quantity))return false;
        foodAfter=a;after=a.stored;awarded=true;return true;
    }
    bool observeFood(const PickupFoodValue& b,const PickupFoodValue& a,int qty){
        if(attempted||awarded||!validFoodOutcome(b,a,qty)||!reserveFood(b,qty)||!finishFood(a))return false;
        nativeSource=true;return true;
    }
    bool observe(int b,int a,int qty,unsigned type=1,int cap=1000000){
        if(attempted||awarded||!pickupOutcomeValid(b,a,qty,type,cap))return false;
        attempted=awarded=nativeSource=true;before=b;after=a;quantity=qty;family=type;maximum=cap;return true;
    }
    bool reserve(int b,int qty=1,unsigned type=1,int cap=1000000){
        if(attempted||awarded||!coinQuantityValid(qty)||!pickupFamilyValid(type)||type==6||b<0||
           (type==1?(cap!=1000000||b>cap-qty):(cap<1||cap>1000||b>cap)))return false;
        attempted=true;before=after=b;quantity=qty;family=type;maximum=cap;return true;
    }
    bool finish(int a){if(!attempted||awarded||nativeSource||!pickupOutcomeValid(before,a,quantity,family,maximum))return false;after=a;awarded=true;return true;}
};

// Identity lists are strictly ascending (no duplicates), nonzero, armed and
// scoped to the exact session/world pair; unused slots stay zero.
inline bool validCoopCoinCatalog(const CoopCoinCatalogPacket& p,const CoopPosePacket& local,const CoopPosePacket& peer){
    if(p.magic!=0x4C433244||p.version!=1||p.size!=sizeof(p)||p.build!=0x67a50cde||p.role>1||p.role!=peer.role||p.reserved||
       !p.sequence||!p.count||p.count>kCoinCatalogMax||!coopWorldPairReady(local,peer)||p.sourceSession!=peer.session||
       p.targetSession!=local.session||p.sourceEpoch!=peer.worldEpoch||p.targetEpoch!=local.worldEpoch||p.mapId!=local.mapId)return false;
    for(unsigned i=0;i<kCoinCatalogMax;++i){const auto& e=p.entries[i];
        if(i<p.count){if(!e.identity||!e.arm||(i&&e.identity<=p.entries[i-1].identity))return false;}
        else if(e.identity||e.arm)return false;}
    return true;
}
