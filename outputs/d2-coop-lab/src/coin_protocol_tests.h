#pragma once
#include <memory>
#include "coin_protocol.h"
#include "coin_catalog.h"
#include "native_stack_item.h"
inline void coinProtocolTests(){
    CoopPosePacket h{},c{};h.role=0;c.role=1;h.session=11;c.session=22;h.worldEpoch=3;c.worldEpoch=5;
    h.mapId=c.mapId=9;h.flags=c.flags=1;h.gameLogicState=c.gameLogicState=1;h.cinematicMetadataValid=c.cinematicMetadataValid=1;
    CoopCoinPacket p{};p.sourceSession=11;p.targetSession=22;p.sourceEpoch=3;p.targetEpoch=5;p.mapId=9;p.sequence=1;p.sourceArm=7;p.quantity=1;p.catalogCount=11;p.catalogId=765;
    std::strcpy(p.name,"pickups_coin_emily_lowvalue_4");std::strcpy(p.item,"models/interactive/valuables/coins/coin1_lowvalue.inventoryitem");
    require(validCoopCoin(p,c,h),"coin hello exact world scope");
    p.kind=CoinMessage::Commit;p.targetArm=8;p.revision=1;require(validCoopCoin(p,c,h),"coin host commit accepted");
    auto medium=p;medium.quantity=2;require(validCoopCoin(medium,c,h),"declared medium coin value accepted");
    for(unsigned i=0;i<23;++i){auto b=p;switch(i){
        case 0:++b.sourceSession;break;case 1:++b.targetSession;break;case 2:++b.sourceEpoch;break;case 3:++b.targetEpoch;break;case 4:++b.mapId;break;
        case 5:b.quantity=0;break;case 6:b.quantity=1001;break;case 7:b.revision=0;break;case 8:b.revision=2;break;case 9:b.sequence=0;break;
        case 10:b.sourceArm=0;break;case 11:b.targetArm=0;break;case 12:b.role=1;break;case 13:b.name[191]='x';break;case 14:b.item[191]='x';break;
        case 15:b.kind=CoinMessage::Request;break;case 16:b.kind=CoinMessage::Ack;break;case 17:b.catalogCount=0;break;
        case 18:b.catalogCount=static_cast<std::uint32_t>(kCoinCatalogMax+1);break;case 19:b.catalogId=0;break;case 20:b.family=25;break;case 21:b.version=6;break;case 22:b.quantity=0xffffffff;break;}
        require(!validCoopCoin(b,c,h),"coin foreign world, malformed identity or client impersonation rejected");
    }
    auto renewed=c;++renewed.worldEpoch;require(!validCoopCoin(p,renewed,h),"old pickup packet cannot credit restored checkpoint");
    auto paused=c;paused.gameLogicState=2;require(!validCoopCoin(p,paused,h),"coin delivery cannot mutate paused native player");
    p.role=1;p.sourceSession=22;p.targetSession=11;p.sourceEpoch=5;p.targetEpoch=3;p.kind=CoinMessage::Request;p.revision=0;
    require(validCoopCoin(p,h,c),"client ordinary reward request accepted");
    p.kind=CoinMessage::Commit;p.revision=1;require(!validCoopCoin(p,h,c),"client cannot author coin commit");p.kind=CoinMessage::Ack;require(validCoopCoin(p,h,c),"client receipt acknowledgement accepted");
    CoinAwardReceipt source{},recipient{};
    require(source.observe(0,1,1)&&source.awarded&&source.nativeSource,"native exact award observed");
    require(!source.reserve(1)&&!source.observe(1,2,1),"local pickup cannot be credited again by commit or second observation");
    require(recipient.reserve(17)&&!recipient.reserve(17)&&!recipient.observe(17,18,1),"reserve before native calls blocks retry and reentrant duplicate");
    require(recipient.finish(18)&&!recipient.finish(19)&&!recipient.reserve(18),"recipient commit is one delta, never absolute wallet replacement");
    CoinAwardReceipt partial{};require(partial.reserve(2)&&!partial.finish(4)&&!partial.reserve(4),"unexpected native award remains terminal without blind retry");
    CoinAwardReceipt simultaneousHost{},simultaneousClient{};
    require(simultaneousHost.observe(0,1,1)&&simultaneousClient.observe(8,9,1)&&!simultaneousHost.reserve(1)&&!simultaneousClient.reserve(9),"simultaneous native awards converge without another reward on either role");
    CoinAwardReceipt invalid{};require(!invalid.observe(0,2,1)&&!invalid.observe(-1,0,1)&&!invalid.observe(0,1,2)&&!invalid.attempted,"unrelated wallet changes cannot authorize a pickup receipt");
    CoinAwardReceipt hostLow{},hostMid{},clientLow{},clientMid{};
    require(hostLow.observe(0,1,1)&&clientMid.observe(0,2,2)&&hostMid.reserve(1,2)&&hostMid.finish(3)&&clientLow.reserve(2,1)&&clientLow.finish(3),"different simultaneous coins add both deltas independently on each wallet");
    require(!hostLow.reserve(3,1)&&!hostMid.reserve(3,2)&&!clientLow.reserve(3,1)&&!clientMid.reserve(3,2),"mixed value duplicate commits never add another delta");
    require(!invalid.reserve(999999,2)&&!invalid.observe(999999,1000001,2)&&!invalid.reserve(0,1001),"bounded wallet arithmetic refuses overflow and unsupported values");
    NativeCoin low{},mid{};low.name="coin_low";low.itemName="items/low.inventoryitem";low.quantity=1;mid.name="coin_mid";mid.itemName="items/mid.inventoryitem";mid.quantity=2;
    auto catalog=selectNativeCoinCatalog({low,mid});auto otherLow=low,otherMid=mid;otherLow.actor=123;otherLow.component=456;otherLow.handle=9;otherMid.game=987;otherMid.generation=777;
    auto peer=selectNativeCoinCatalog({otherMid,otherLow});require(catalog.digest&&peer.digest==catalog.digest&&catalog.entries.size()==2,"catalog order and local pointers do not affect authored identity");
    otherMid.quantity=3;require(selectNativeCoinCatalog({low,otherMid}).digest!=catalog.digest,"different authored values change catalog identity");
    otherMid=mid;otherMid.itemName="items/other.inventoryitem";require(selectNativeCoinCatalog({low,otherMid}).digest!=catalog.digest,"different inventory declarations change catalog identity");
    auto duplicate=selectNativeCoinCatalog({low,mid,low});require(duplicate.ambiguous==2&&duplicate.entries.size()==1&&duplicate.entries[0].name==mid.name,"all ambiguous names excluded rather than arbitrary first binding");
    auto collected=low;collected.flags=8;auto retiring=low;retiring.name="retiring";retiring.ownerFlags=1;
    auto unsafe=selectNativeCoinCatalog({collected,retiring,mid});require(unsafe.ineligible==2&&unsafe.entries.size()==1,"collected and retiring coins cannot enter new catalog");
    std::vector<NativeCoin> many;for(unsigned i=0;i<kCoinCatalogMax;++i){auto n=low;n.name="coin_"+std::to_string(i);many.push_back(n);}
    require(selectNativeCoinCatalog(many).digest!=0,"bounded mission-size catalog supported");auto extra=mid;extra.name="overflow";many.push_back(extra);require(!selectNativeCoinCatalog(many).digest,"oversized catalog refuses silent partial selection");
    {auto list=std::make_unique<CoopCoinCatalogPacket>();list->sequence=1;list->sourceSession=11;list->targetSession=22;list->sourceEpoch=3;list->targetEpoch=5;list->mapId=9;list->count=3;
     for(unsigned i=0;i<3;++i){list->entries[i].identity=100+i;list->entries[i].arm=7+i;}
     require(validCoopCoinCatalog(*list,c,h),"ascending scoped identity list accepted");
     for(unsigned i=0;i<9;++i){auto bad=std::make_unique<CoopCoinCatalogPacket>(*list);switch(i){
        case 0:bad->entries[1].identity=100;break;case 1:bad->entries[2].identity=50;break;case 2:bad->entries[0].arm=0;break;
        case 3:bad->count=0;break;case 4:bad->count=static_cast<std::uint32_t>(kCoinCatalogMax+1);break;case 5:bad->entries[5].identity=9;break;
        case 6:bad->version=2;break;case 7:++bad->targetSession;break;case 8:++bad->sourceEpoch;break;}
      require(!validCoopCoinCatalog(*bad,c,h),"malformed or out-of-scope identity list refused");}
     NativeCoin a=low,b=mid,corvoOnly=low,emilyOnly=mid;a.name="shared_a";b.name="shared_b";corvoOnly.name="script_ammo_x";emilyOnly.name="pickups_fruit_y";
     auto conflict=a;conflict.quantity=a.quantity+1;
     require(coinIdentity(a)!=coinIdentity(conflict),"same name with different native quantity is a different identity");
     std::vector<std::uint64_t> host{coinIdentity(a),coinIdentity(b),coinIdentity(corvoOnly)},client{coinIdentity(emilyOnly),coinIdentity(b),coinIdentity(a)};
     const auto intersect=[](std::vector<std::uint64_t> mine,std::vector<std::uint64_t> theirs){std::sort(theirs.begin(),theirs.end());std::vector<std::uint64_t> out;
        for(auto id:mine)if(std::binary_search(theirs.begin(),theirs.end(),id))out.push_back(id);return out;};
     const auto hs=intersect(host,client),cs=intersect(client,host);
     require(hs.size()==2&&cs.size()==2&&coinSharedDigest(hs)==coinSharedDigest(cs),"both peers derive the same shared intersection digest");
     require(coinSharedDigest(hs)!=coinSharedDigest(host),"character-only pickups change the shared digest");}
    require(!coinCanonicalName(std::string("coin\0hidden",11))&&!coinCanonicalName(std::string(192,'x')),"noncanonical names cannot alias a wire identity");
    CoinAwardReceipt full{},clamped{},modified{},storedPotion{},notGranted{};
    require(full.reserve(10,2,4,10)&&full.finish(10)&&!full.reserve(10,2,4,10),"full recipient acknowledges zero native stack grant once");
    require(clamped.reserve(9,2,4,10)&&clamped.finish(10),"recipient accepts actual capacity-limited grant");
    require(modified.observe(0,3,2,4,10),"native stack modifier may differ from declared requested amount");
    require(storedPotion.observe(0,1,1,14,10)&&!notGranted.observe(0,0,1,14,10),"stored potion needs a grant unless already full");
    require(!notGranted.observe(4,3,2,4,10)&&!notGranted.observe(9,11,2,4,10)&&!notGranted.reserve(11,1,4,10)&&!notGranted.reserve(0,1,6,1),"depletion, excess capacity and immediate-food path rejected");
    auto ammoPacket=p;ammoPacket.family=4;require(validCoopCoin(ammoPacket,h,c),"stack family wire envelope accepted");
    auto foodPacket=p;foodPacket.family=6;require(validCoopCoin(foodPacket,h,c),"food family wire envelope accepted");
    PickupFoodValue foodBefore{56,0,100,100,100,100,5,0,0,1,false,true},foodAfter=foodBefore;foodAfter.healthTarget=61;
    CoinAwardReceipt food{},foodSource{};require(food.reserveFood(foodBefore,1)&&food.finishFood(foodAfter)&&!food.reserveFood(foodBefore,1),"food recipient reserves native queue outcome once");
    require(foodSource.observeFood(foodBefore,foodAfter,1)&&foodSource.nativeSource&&!foodSource.observeFood(foodBefore,foodAfter,1),"observed food cannot receive a duplicate heal");
    auto changedFamily=mid;changedFamily.type=4;require(selectNativeCoinCatalog({low,changedFamily}).digest!=catalog.digest,"native family participates in authored catalog digest");
    {
        constexpr std::uintptr_t base=0x140000000,inv=0x600000000,owner=inv+0x1000,list=inv+0x2000,item=inv+0x3000,name=inv+0x4000;
        Memory m;std::array<unsigned char,112> bytes{};m.put(list,bytes.data(),bytes.size());m.q(inv,base+0x1b9a878);m.q(inv+0x10,owner);m.d(inv+0x18,17);m.d(inv+0x1c,8);m.q(inv+0x38,list);m.d(inv+0x40,2);
        m.q(item,base+0x1b9ea48);m.d(item+0x60,4);m.q(item+8,name);m.str(name,"components/equipment/ammo/bullet.inventoryitem");m.q(list,item);m.d(list+0x20,9);m.d(list+0x24,10);
        NativeStackItem v{};const auto inspect=[&](Memory& data){return readNativeStackItem([&](auto p,auto b,auto n){return data.read(p,b,n);},base,inv,owner,item,4,v);};
        require(inspect(m)&&v.quantity==9&&v.maximum==10,"owned native ammo entry and capacity read");
        auto bad=m;bad.q(inv+0x10,owner+8);require(!inspect(bad),"foreign inventory owner refused");
        bad=m;bad.q(list+56,item);require(!inspect(bad),"duplicate native item entry refused");
        bad=m;bad.d(list+0x24,0);require(!inspect(bad),"zero stack maximum refused");
        bad=m;bad.d(list+0x20,11);require(!inspect(bad),"overfull stack refused");
        bad=m;bad.d(item+0x60,6);require(!inspect(bad),"food cannot impersonate ammo");
        bad=m;bad.d(inv+0x40,2049);require(!inspect(bad),"unbounded item table refused");
        bad=m;unsigned calls=0;require(!readNativeStackItem([&](auto p,auto b,auto n){const bool ok=bad.read(p,b,n);if(p==list&&++calls==1)bad.d(list+0x20,8);return ok;},base,inv,owner,item,4,v),"quantity change during stack read refused");
    }
    std::cout<<"PASS: coin world/role envelopes, exact native deltas, simultaneous receipts and partial-application no-retry\n";
}
