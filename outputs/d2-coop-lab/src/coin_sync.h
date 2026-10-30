#pragma once
#include "coin_native_api.h"
#include "coin_catalog.h"
#include "door_session_scope.h"
#include "pickup_catchup.h"
struct CoinChannel {
    NativeCoin binding{};CoinAwardReceipt receipt{},exerciseReceipt{};
    std::uint64_t arm=0,peerArm=0,peerSequence=0,helloAt=0,helloSent=0,actionSent=0,deadline=0;
    unsigned revision=0,ack=0,calls=0,observations=0,exerciseCalls=0;
    std::uint64_t identity=0; // 1059c: coinIdentity(binding)
    const char* result="awaiting_peer";
};
struct CoinSync {
    bool enabled=false,ready=false,paused=false,reloadPending=false,reloadReady=false;
    DoorSessionScope scope{};std::uintptr_t game=0;std::vector<CoinChannel> entries;
    std::uint64_t catalogId=0,pauseAt=0,deadline=0,overflowBaseline=0;
    std::size_t cursor=0;
    // 1059c: mission worlds differ per character (script pickups). Each peer
    // sends its identity list once (reliably, repeated until confirmed); both
    // derive the same intersection. Hello is then a 10/s heartbeat carrying the
    // sender's shared digest; ready requires equal digests and a fresh peer.
    bool settled=false,peerListValid=false;std::vector<CoopCoinCatalogEntry> peerList;
    std::uint64_t listSentAt=0,helloSentAt=0,sharedDigest=0,peerDigest=0,lastPeerHello=0,peerOnly=0,listsSent=0,listsReceived=0;
    unsigned peerCount=0,localOnly=0;std::size_t helloCursor=0;
    std::uint64_t sent=0,received=0,rejected=0,duplicates=0,observations=0,commits=0,reloads=0,rearms=0;
    const char* result="disabled";
} coinSync;
// Stage 8.4.2 (#238): items per player. Each player takes each pickup once, in their own world: the shared catalog
// (request, commit and apply) and the pickup catch-up stay off, so a pickup leaves only the taker's world. In the
// product it is on unless the host's d2coop.cfg sets items_per_player=0 (D2COOP_ITEMS_PER_PLAYER=0); the partner
// follows the host's setting from its hello (control flag 8). The lab sets it with coop_items_per_player on|off.
bool itemsPerPlayer=false,productItemsPerPlayer=true;std::uint64_t itemsPerPlayerRefusals=0;
bool coopItemsPerPlayerFlag(){return localRole==0&&itemsPerPlayer;}
// Lab only (coop_items_per_player take <name>): this world's player takes the named story pickup natively, through the
// same native steps as the story catch-up, so a test can check that the peer's world follows (Test-ItemsPerPlayer).
std::string storyTakeRequest;const char* storyTakeResult="idle";std::uint64_t storyTakes=0;
std::uint64_t coinArmCounter=0,coinSequence=0;
int coinEnableRequest=-1,coinReplayRequest=0;
std::string coinExerciseRequest;
std::uint64_t coinExerciseRefused=0;
CoopCoinPacket coinLastAction{},coinRetiredAction{};
// Only immutable identities and bounded observations cross the worker/engine
// boundary. Never hold this lock during a native call or network/file I/O.
SRWLOCK coinHookLock=SRWLOCK_INIT;
struct CoinHookLease {NativeCoin binding{};std::uint64_t arm=0,epoch=0;};
struct CoinHookEvent {std::uint64_t arm=0,epoch=0;int before=0,after=0,maximum=0;unsigned thread=0;bool valid=false;PickupFoodValue foodBefore{},foodAfter{};bool grapple=false;};
std::vector<CoinHookLease> coinHookLeases;
std::array<CoinHookEvent,128> coinHookEvents{};unsigned coinHookCount=0;
std::atomic<std::uint64_t> coinHookCalls{0},coinHookCandidates{0},coinHookOtherThread{0},coinHookOverflow{0};
std::atomic<unsigned> coinHookLastThread{0};
// 1214jg (#186): Far Reach rewards (fromGrapple) and refused rewards. For the last Far Reach or refused reward: the
// checks it passed (bits: 1 player, 2 own state, 4 item, 8 claim flags, 16 outcome before, 32 wallet after, 64 item
// after, 128 outcome delta), its state id and its state's vtable RVA.
std::atomic<std::uint64_t> coinHookGrapple{0},coinHookGrappleValid{0},coinHookRefused{0},coinHookOtherState{0};
std::atomic<unsigned> coinHookLastChecks{0},coinHookLastStateId{~0u},coinHookLastStateVt{0},coinHookLastGrapple{0};
void publishCoinHookLease(){
    std::vector<CoinHookLease> next;for(const auto& c:coinSync.entries)next.push_back({c.binding,c.arm,coinSync.scope.epoch});
    AcquireSRWLockExclusive(&coinHookLock);coinHookLeases.swap(next);ReleaseSRWLockExclusive(&coinHookLock);
}
void clearCoinHookLease(){AcquireSRWLockExclusive(&coinHookLock);coinHookLeases.clear();coinHookCount=0;ReleaseSRWLockExclusive(&coinHookLock);}
bool doorCheckpointBarrierActive();
void stopCoinSync(const char* why){coinSync.enabled=coinSync.ready=false;coinSync.result=why;clearCoinHookLease();}
bool coinConnection(){return sockets&&connection&&connectionState==k_ESteamNetworkingConnectionState_Connected;}
bool coinWorldContext(){const auto& s=coinSync;return s.enabled&&!s.reloadPending&&observedGame==s.game&&
    doorSessionWorldMatches(s.scope,outgoing,remote)&&!doorCheckpointBarrierActive();}
bool coinReadyContext(ULONGLONG tick){return coinWorldContext()&&coinConnection()&&coopWorldPairReady(outgoing,remote)&&lastReceive&&tick-lastReceive<250;}
// 1214jp (#189): why the last binding was refused, and the pickup it was refused on.
const char* coinBindRefusal="none";std::string coinBindRefusalItem;std::uint64_t coinBindRefusals=0;
bool refuseCoinBinding(const char* why,const std::string& item={}){coinBindRefusal=why;coinBindRefusalItem=item;++coinBindRefusals;return false;}
bool bindSharedCoin(){
    auto& s=coinSync;const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t list=0;unsigned count=0;
    // 1059: map-agnostic; both peers on the same native campaign map and world.
    if(!nativeCoinApiAvailable())return refuseCoinBinding("native_api");
    if(!coopWorldPairReady(outgoing,remote))return refuseCoinBinding("world_pair");
    if(!memoryRead(observedGame+0x601c0,&list,8)||!list||!memoryRead(observedGame+0x601c8,&count,4)||!(count&0xffffff)||(count&0xffffff)>16384)return refuseCoinBinding("entity_list");
    std::vector<unsigned> handles(count&0xffffff);if(!memoryRead(list,handles.data(),handles.size()*4))return refuseCoinBinding("entity_list");
    std::vector<NativeCoin> coins;
    for(auto h:handles){NativeCoin n{};if(readNativeCoin(memoryRead,base,observedGame,h,n,true))coins.push_back(std::move(n));}
    auto catalog=selectNativeCoinCatalog(coins);CoinPlayer player{};
    if(catalog.ambiguous){std::vector<std::string> names;for(const auto& n:coins)names.push_back(n.name);std::sort(names.begin(),names.end());
        const auto twin=std::adjacent_find(names.begin(),names.end());return refuseCoinBinding("ambiguous",twin!=names.end()?*twin:std::string{});}
    if(!catalog.digest||catalog.entries.empty())return refuseCoinBinding("empty_catalog");
    if(catalog.entries.size()>kCoinCatalogMax)return refuseCoinBinding("catalog_overflow");
    if(!readCoinPlayer(player))return refuseCoinBinding("player");
    for(const auto& n:catalog.entries){CoinOutcomeValue value{};if(!readCoinOutcome(player,n,value))return refuseCoinBinding("outcome",n.name+" "+n.itemName);}
    coinBindRefusal="none";coinBindRefusalItem.clear();
    s.game=observedGame;s.scope=doorSessionScope(outgoing,remote);s.catalogId=catalog.digest;s.entries.clear();
    for(auto& n:catalog.entries){CoinChannel c{};c.identity=coinIdentity(n);c.binding=std::move(n);c.arm=++coinArmCounter;s.entries.push_back(std::move(c));}
    s.cursor=0;s.deadline=s.pauseAt=0;s.overflowBaseline=coinHookOverflow.load();
    s.ready=s.paused=s.reloadPending=s.reloadReady=false;s.settled=s.peerListValid=false;s.peerList.clear();
    s.listSentAt=s.helloSentAt=s.sharedDigest=s.peerDigest=s.lastPeerHello=0;s.peerCount=s.localOnly=0;s.helloCursor=0;
    s.result="awaiting_catalog_peer";publishCoinHookLease();return true;
}
void beginCoinCheckpointRecovery(){
    auto& s=coinSync;if(!s.enabled)return;
    if(outgoing.session!=s.scope.session||remote.session!=s.scope.peerSession||outgoing.worldEpoch!=s.scope.epoch||outgoing.mapId!=s.scope.map){stopCoinSync("checkpoint_scope_changed");return;}
    s.reloadPending=true;s.reloadReady=s.ready=s.paused=false;s.deadline=GetTickCount64()+60000;++s.reloads;
    coinRetiredAction=coinLastAction;s.game=0;s.entries.clear();s.catalogId=0;clearCoinHookLease();s.result="waiting_shared_checkpoint";
}
void finishCoinCheckpointRecovery(){auto& s=coinSync;if(s.enabled&&s.reloadPending&&doorSessionRenewalReady(s.scope,outgoing,remote))s.reloadReady=true;}
bool sendCoinPacket(CoinChannel& c,CoinMessage kind,ULONGLONG tick){
    auto& s=coinSync;if(!coinReadyContext(tick))return false;
    CoopCoinPacket p{};p.role=localRole;p.kind=kind;p.quantity=c.binding.quantity;p.family=c.binding.type;p.catalogCount=static_cast<unsigned>(s.entries.size());p.catalogId=s.settled?s.sharedDigest:s.catalogId;
    p.sequence=++coinSequence;p.sourceSession=s.scope.session;p.targetSession=s.scope.peerSession;p.sourceEpoch=s.scope.epoch;p.targetEpoch=s.scope.peerEpoch;p.mapId=s.scope.map;p.sourceArm=c.arm;
    std::memcpy(p.name,c.binding.name.data(),c.binding.name.size());std::memcpy(p.item,c.binding.itemName.data(),c.binding.itemName.size());
    if(kind!=CoinMessage::Hello)p.targetArm=c.peerArm;
    if(kind==CoinMessage::Commit||kind==CoinMessage::Ack)p.revision=1;
    if(!validCoopCoin(p,remote,outgoing)||coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)!=k_EResultOK)return false;
    ++s.sent;if(kind==CoinMessage::Request||kind==CoinMessage::Commit)coinLastAction=p;return true;
}
// 1214jr (#189): the pickup census, every pickup actor in this world, active or dormant (route-resumed-join-v1875a:
// about a fifth of the pickups are dormant in one world and active in the other). Each cycle (5 s) reads the entity slot
// table (game+0x3130b0, worldIdentityEntity) in one call and each occupied slot's handle; only a slot whose entity or
// handle changed is classified again, and a tick spends at most 256 reads.
constexpr unsigned kPickupCensusSlots=16384,kPickupCensusReads=256;
struct PickupCensusActor {std::uintptr_t entity=0;std::uint32_t handle=0;std::string name;ULONGLONG seenAt=0;};
struct PickupCensus {
    std::uintptr_t game=0;std::uint64_t epoch=0;std::vector<std::uintptr_t> table,entities;std::vector<std::uint32_t> handles;
    std::unordered_map<unsigned,PickupCensusActor> actors;unsigned cursor=0;bool scanning=false;
    ULONGLONG cycleAt=0,completedAt=0,lastCycleMs=0;std::uint64_t cycles=0,classifications=0,maxReads=0;
} pickupCensus;
bool classifyPickupActor(std::uintptr_t entity,std::uint32_t handle,PickupCensusActor& out,unsigned& reads){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t vt=0,p=0,list=0;unsigned count=0;
    reads+=3;if(!memoryRead(entity,&vt,8)||vt!=base+0x1c5e258||!memoryRead(entity+0x268,&list,8)||!list||!memoryRead(entity+0x270,&count,4)||!(count&0xffffff)||(count&0xffffff)>256)return false;
    std::array<std::uintptr_t,256> members{};++reads;if(!memoryRead(list,members.data(),(count&0xffffff)*8))return false;
    bool pickup=false;for(unsigned i=0;i<(count&0xffffff)&&!pickup;++i){++reads;pickup=members[i]&&memoryRead(members[i],&vt,8)&&vt==base+0x1c5a3b8;}
    std::string name;reads+=2;if(!pickup||!memoryRead(entity+0x20,&p,8)||!worldIdentityString(memoryRead,p,name))return false;
    out.entity=entity;out.handle=handle;out.name=std::move(name);return true;
}
void servicePickupCensus(ULONGLONG tick){
    auto& c=pickupCensus;if(!observedGame||!coopGameplayReady(outgoing))return;
    if(c.game!=observedGame||c.epoch!=outgoing.worldEpoch){c=PickupCensus{};c.game=observedGame;c.epoch=outgoing.worldEpoch;}
    if(!c.scanning){
        if(c.cycleAt&&tick-c.cycleAt<5000)return;
        c.table.resize(kPickupCensusSlots);if(!memoryRead(c.game+0x3130b0,c.table.data(),kPickupCensusSlots*8))return;
        if(c.entities.size()!=kPickupCensusSlots){c.entities.assign(kPickupCensusSlots,0);c.handles.assign(kPickupCensusSlots,0);}
        c.cursor=0;c.scanning=true;c.cycleAt=tick;
    }
    unsigned reads=0;
    for(;c.cursor<kPickupCensusSlots&&reads<kPickupCensusReads;++c.cursor){
        const auto slot=c.cursor;const auto entity=c.table[slot];std::uint32_t handle=0;
        if(entity){++reads;if(!memoryRead(entity+0xa8,&handle,4)||(handle>>16)!=slot)handle=0;}
        if(entity==c.entities[slot]&&handle==c.handles[slot])continue;
        c.entities[slot]=entity;c.handles[slot]=handle;++c.classifications;
        const auto known=c.actors.find(slot);PickupCensusActor a{};
        if(entity&&handle&&classifyPickupActor(entity,handle,a,reads)){
            a.seenAt=known!=c.actors.end()&&known->second.handle==handle&&known->second.name==a.name?known->second.seenAt:tick;c.actors[slot]=std::move(a);}
        else if(known!=c.actors.end())c.actors.erase(known);
    }
    c.maxReads=(std::max)(c.maxReads,static_cast<std::uint64_t>(reads));
    if(c.cursor>=kPickupCensusSlots){c.scanning=false;c.completedAt=tick;c.lastCycleMs=tick-c.cycleAt;++c.cycles;}
}
std::vector<PickupPresenceItem> pickupCensusItems(){
    std::vector<PickupPresenceItem> items;
    for(const auto& [slot,a]:pickupCensus.actors){PickupPresenceItem i{a.name,a.handle,{}};
        if(pickupRuntimeNamed(a.name,a.handle)&&!memoryRead(a.entity+0x300,i.position,12))continue;items.push_back(std::move(i));}
    return items;
}
struct PickupCatchUpTarget {std::string name;std::uint32_t handle=0;};
struct PickupCatchUp {
    std::uint64_t epoch=0,peerEpoch=0,map=0,presenceSent=0,presenceReceived=0,rejected=0,refusedSends=0,awarded=0,failed=0,deferrals=0,planned=0;
    std::uint64_t sentCycle=0,plannedCycle=0,sentDigest=0,sentEpoch=0,sentPeerEpoch=0,sentMap=0;ULONGLONG sentAt=0,peerAt=0,previousAt=0;std::uint32_t sentEntries=0;
    std::vector<PickupPresenceEntry> peer,previous;std::vector<PickupCatchUpTarget> targets;std::size_t cursor=0;std::unordered_map<std::string,bool> attempted;
    CoinAwardReceipt receipt{};unsigned calls=0;const char* result="idle";std::vector<std::string> awardedNames,failedNames;
} pickupCatchUp;
std::uint64_t pickupPresenceSequence=0;
// This world's presence, after each completed census cycle when it changed (positions of runtime pickups included), or
// at least every 15 s, or at once for a new pair of worlds. Since 1214jv (#203) both roles send it.
void sendPickupPresence(ULONGLONG tick){
    auto& c=pickupCatchUp;const auto& census=pickupCensus;
    if(localRole>1||!census.cycles||c.sentCycle==census.cycles||census.game!=observedGame||!coinConnection()||!coopWorldPairReady(outgoing,remote)||outgoing.mapId!=remote.mapId)return;
    c.sentCycle=census.cycles;const auto entries=pickupPresenceEntries(pickupCensusItems());
    if(entries.size()>kPickupPresenceMax){c.result="presence_overflow";return;}
    std::uint64_t digest=1469598103934665603ull;
    for(const auto& e:entries){const auto* b=reinterpret_cast<const unsigned char*>(&e);for(std::size_t i=0;i<sizeof(e);++i){digest^=b[i];digest*=1099511628211ull;}}
    if(c.sentEpoch==outgoing.worldEpoch&&c.sentPeerEpoch==remote.worldEpoch&&c.sentMap==outgoing.mapId&&c.sentDigest==digest&&tick-c.sentAt<15000)return;
    static PickupPresencePacket p;p=PickupPresencePacket{};auto& h=p.header;h.role=localRole;h.sequence=++pickupPresenceSequence;
    h.sourceSession=outgoing.session;h.targetSession=remote.session;h.sourceEpoch=outgoing.worldEpoch;h.targetEpoch=remote.worldEpoch;h.mapId=outgoing.mapId;
    h.count=static_cast<std::uint32_t>(entries.size());std::copy(entries.begin(),entries.end(),p.entries);const auto bytes=pickupPresenceBytes(h.count);
    if(!validPickupPresence(p,bytes,remote,outgoing)||coopSendMessage(connection,&p,static_cast<std::uint32_t>(bytes),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)!=k_EResultOK){++c.refusedSends;return;}
    ++c.presenceSent;c.sentAt=tick;c.sentDigest=digest;c.sentEpoch=outgoing.worldEpoch;c.sentPeerEpoch=remote.worldEpoch;c.sentMap=outgoing.mapId;c.sentEntries=h.count;c.result="presence_sent";
}
bool sendCoinCatalog(ULONGLONG tick){
    auto& s=coinSync;if(!coinReadyContext(tick)||s.entries.empty()||s.entries.size()>kCoinCatalogMax)return false;
    // Engine thread only; the 8 KB wire image is reused instead of stack-allocated.
    static CoopCoinCatalogPacket buffer;buffer=CoopCoinCatalogPacket{};auto* list=&buffer;list->role=localRole;list->sequence=++coinSequence;
    list->sourceSession=s.scope.session;list->targetSession=s.scope.peerSession;list->sourceEpoch=s.scope.epoch;list->targetEpoch=s.scope.peerEpoch;list->mapId=s.scope.map;
    std::vector<CoopCoinCatalogEntry> mine;for(const auto& c:s.entries)mine.push_back({c.identity,c.arm});
    std::sort(mine.begin(),mine.end(),[](const auto& a,const auto& b){return a.identity<b.identity;});
    list->count=static_cast<std::uint32_t>(mine.size());std::copy(mine.begin(),mine.end(),list->entries);
    if(!validCoopCoinCatalog(*list,remote,outgoing)||coopSendMessage(connection,list,sizeof(*list),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)!=k_EResultOK)return false;
    ++s.sent;++s.listsSent;return true;
}
void receiveCoinCatalog(const CoopCoinCatalogPacket& p,ULONGLONG tick){
    auto& s=coinSync;++s.received;
    if(!coinReadyContext(tick)||s.paused||!validCoopCoinCatalog(p,outgoing,remote)){++s.rejected;return;}
    std::vector<CoopCoinCatalogEntry> list(p.entries,p.entries+p.count);
    if(s.peerListValid){
        // The peer repeats one list until confirmed; a different list without a
        // shared checkpoint renewal means the peer rearmed out of scope.
        if(list.size()!=s.peerList.size()||!std::equal(list.begin(),list.end(),s.peerList.begin(),[](const auto& a,const auto& b){return a.identity==b.identity&&a.arm==b.arm;})){
            stopCoinSync("peer_catalog_changed_without_reload");return;}
        ++s.duplicates;return;
    }
    s.peerList=std::move(list);s.peerListValid=true;++s.listsReceived;
}
void receiveCoinPacket(const CoopCoinPacket& p,ULONGLONG tick){
    auto& s=coinSync;++s.received;
    if(!coinReadyContext(tick)||s.paused||!validCoopCoin(p,outgoing,remote)){++s.rejected;return;}
    // Only the settled intersection is addressable; a peer heartbeat that
    // arrives before the local intersection is simply repeated later.
    if(!s.settled){++s.rejected;return;}
    auto found=std::find_if(s.entries.begin(),s.entries.end(),[&](const auto& c){return c.binding.name==p.name&&c.binding.itemName==p.item&&
        static_cast<unsigned>(c.binding.quantity)==p.quantity&&c.binding.type==p.family;});
    if(found==s.entries.end()){++s.peerOnly;return;}auto& c=*found;
    if(p.kind==CoinMessage::Hello){
        if(c.peerArm!=p.sourceArm){stopCoinSync("peer_rearmed_without_reload");return;}
        if(p.sequence<=c.peerSequence){++s.duplicates;return;}
        c.peerSequence=p.sequence;c.helloAt=tick;s.peerDigest=p.catalogId;s.peerCount=p.catalogCount;s.lastPeerHello=tick;return;
    }
    if(!s.ready||p.targetArm!=c.arm||p.sourceArm!=c.peerArm){++s.rejected;return;}
    if(p.sequence<=c.peerSequence){++s.duplicates;return;}c.peerSequence=p.sequence;
    if(p.kind==CoinMessage::Ack){if(c.revision&&c.receipt.awarded)c.ack=1;else ++s.rejected;return;}
    if(!c.revision){c.revision=1;++s.commits;c.deadline=tick+10000;c.result="host_commit_received";}else ++s.duplicates;
    c.actionSent=0;if(localRole)c.ack=0;
}
// The reward's state object must be the local player's own: LootObject (47) for an ordinary pickup, otherwise (a Far
// Reach pull, a theft) any of the player's mode state slots whose id resolves back to it and whose owner is the player
// component.
bool coinRewardStateOwned(const CoinPlayer& p,std::uintptr_t state,unsigned& id){
    if(p.loot==state){id=47;return true;}
    std::uintptr_t slot=0,owner=0;
    return state&&memoryRead(state+8,&id,4)&&id<90&&memoryRead(p.states.mode+0x10+id*8ull,&slot,8)&&slot==state&&
        memoryRead(state+0x10,&owner,8)&&owner==p.states.component;
}
void observeCoinReward(void* state,void* pickable,unsigned char fromGrapple){
    CoinWalletGuard wallet;
    ++coinHookCalls;const auto thread=GetCurrentThreadId();coinHookLastThread=thread;if(thread!=engineThread.load())++coinHookOtherThread;
    CoinHookLease lease{};AcquireSRWLockShared(&coinHookLock);
    for(const auto& candidate:coinHookLeases)if(reinterpret_cast<std::uintptr_t>(pickable)==candidate.binding.component){lease=candidate;break;}
    ReleaseSRWLockShared(&coinHookLock);
    const bool candidate=lease.arm&&lease.epoch&&lease.binding.component;
    NativeCoin beforeCoin{};CoinPlayer before{};CoinOutcomeValue beforeValue{};bool valid=false;
    unsigned checks=0,stateId=~0u;const auto step=[&](bool ok,unsigned bit){if(ok)checks|=bit;return ok;};
    if(candidate){++coinHookCandidates;
        valid=step(readCoinPlayer(before,lease.binding.game),1)&&step(coinRewardStateOwned(before,reinterpret_cast<std::uintptr_t>(state),stateId),2)&&
            step(readNativeCoin(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),lease.binding.game,lease.binding.handle,beforeCoin,true)&&sameNativeCoin(beforeCoin,lease.binding),4)&&
            step((beforeCoin.flags&2)&&!(beforeCoin.flags&8)&&!(beforeCoin.ownerFlags&1),8)&&step(readCoinOutcome(before,beforeCoin,beforeValue),16);
    }
    realCoinReward(state,pickable,fromGrapple);
    if(!candidate)return;
    CoinPlayer after{};NativeCoin afterCoin{};CoinOutcomeValue afterValue{};
    valid=valid&&step(readCoinPlayer(after,lease.binding.game)&&sameCoinWallet(before,after),32)&&
       step(readNativeCoin(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),lease.binding.game,lease.binding.handle,afterCoin,true)&&sameNativeCoin(beforeCoin,afterCoin),64)&&
       step(readCoinOutcome(after,afterCoin,afterValue)&&sameCoinOutcomeObjects(beforeValue,afterValue)&&(lease.binding.type==1||before.money==after.money)&&
       validCoinOutcome(beforeValue,afterValue,lease.binding.quantity),128);
    if(fromGrapple){++coinHookGrapple;if(valid)++coinHookGrappleValid;}
    if(valid&&stateId!=47)++coinHookOtherState;
    if(fromGrapple||!valid){std::uintptr_t vt=0;coinHookLastChecks=checks;coinHookLastStateId=stateId;coinHookLastGrapple=fromGrapple!=0;
        if(state&&memoryRead(reinterpret_cast<std::uintptr_t>(state),&vt,8)&&vt>reinterpret_cast<std::uintptr_t>(gameImage))coinHookLastStateVt=static_cast<unsigned>(vt-reinterpret_cast<std::uintptr_t>(gameImage));}
    AcquireSRWLockExclusive(&coinHookLock);
    if(coinHookCount<coinHookEvents.size())coinHookEvents[coinHookCount++]={lease.arm,lease.epoch,beforeValue.value,afterValue.value,beforeValue.maximum,thread,valid,beforeValue.food.value,afterValue.food.value,fromGrapple!=0};else ++coinHookOverflow;
    ReleaseSRWLockExclusive(&coinHookLock);
}
void consumeCoinHookEvents(){
    std::array<CoinHookEvent,128> events{};unsigned count=0;
    AcquireSRWLockExclusive(&coinHookLock);count=coinHookCount;std::copy_n(coinHookEvents.begin(),count,events.begin());coinHookCount=0;ReleaseSRWLockExclusive(&coinHookLock);
    auto& s=coinSync;if(coinHookOverflow.load()!=s.overflowBaseline){stopCoinSync("ordinary_reward_queue_overflow");return;}
    for(unsigned i=0;i<count;++i){const auto& e=events[i];
        auto found=std::find_if(s.entries.begin(),s.entries.end(),[&](const auto& c){return c.arm==e.arm;});
        if(!coinWorldContext()||found==s.entries.end()||e.epoch!=s.scope.epoch){++s.rejected;continue;}auto& c=*found;
        // 1214jg: an unconfirmed reward stays local to that item instead of stopping every shared pickup.
        if(!e.valid){++coinHookRefused;++s.rejected;c.result=e.grapple?"far_reach_reward_unconfirmed":"native_reward_unconfirmed";continue;}
        if(!e.valid||!(c.binding.type==6?c.receipt.observeFood(e.foodBefore,e.foodAfter,c.binding.quantity):c.receipt.observe(e.before,e.after,c.binding.quantity,c.binding.type,e.maximum))){stopCoinSync("ordinary_reward_identity_or_delta_unconfirmed");return;}
        ++c.observations;++s.observations;c.result=s.result="ordinary_native_reward_observed";
    }
}
bool installCoinRewardObserver(){
    auto* target=gameImage+0x841030;if(std::memcmp(target,coinRewardPrefix,sizeof(coinRewardPrefix)))return false;
    // First16 bytes are complete stack-save instructions, with no branches
    // or RIP-relative operands. The remaining prologue runs at entry+16.
    auto* trampoline=static_cast<unsigned char*>(VirtualAlloc(nullptr,64,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));if(!trampoline)return false;
    std::memcpy(trampoline,coinRewardPrefix,16);unsigned char jump[16]{0xff,0x25,0,0,0,0};jump[14]=jump[15]=0x90;
    void* resume=target+16;std::memcpy(jump+6,&resume,8);std::memcpy(trampoline+16,jump,14);
    DWORD old=0,unused=0;if(!VirtualProtect(trampoline,64,PAGE_EXECUTE_READ,&old)){VirtualFree(trampoline,0,MEM_RELEASE);return false;}
    FlushInstructionCache(GetCurrentProcess(),trampoline,64);void* wrapper=reinterpret_cast<void*>(&observeCoinReward);std::memcpy(jump+6,&wrapper,8);
    if(!VirtualProtect(target,16,PAGE_EXECUTE_READWRITE,&old)){VirtualFree(trampoline,0,MEM_RELEASE);return false;}
    realCoinReward=reinterpret_cast<NativeCoinReward>(trampoline);std::memcpy(coinRewardPatch,jump,16);std::memcpy(target,jump,16);FlushInstructionCache(GetCurrentProcess(),target,16);
    return VirtualProtect(target,16,old,&unused)!=0;
}
#include "pickup_capacity_fixture.h"
void serviceCoinSync(ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    servicePickupCapacityFixture(tick);
    auto& s=coinSync;
    if((productMode||productAutoArm)&&localRole==1||productMode&&localRole==0)itemsPerPlayer=localRole==0?productItemsPerPlayer:coopControl.helloOk&&coopControl.peerItemsPerPlayer;
    if(itemsPerPlayer){if(coinEnableRequest>0){coinEnableRequest=-1;++itemsPerPlayerRefusals;}if(s.enabled)stopCoinSync("items_per_player");}
    // A developer command is consumed once even when the session cannot run.
    std::string exercise;exercise.swap(coinExerciseRequest);
    if(coinEnableRequest>=0){const bool on=coinEnableRequest!=0;coinEnableRequest=-1;
        if(!on)stopCoinSync("disabled");else if(!s.enabled){s={};coinLastAction=coinRetiredAction={};coinReplayRequest=0;
            if(!coinConnection())refuseCoinBinding("connection");else if(doorCheckpointBarrierActive())refuseCoinBinding("checkpoint_barrier");
            if(coinConnection()&&!doorCheckpointBarrierActive()&&bindSharedCoin())s.enabled=true;else s.result="initial_binding_refused";}}
    if(!s.enabled){if(!exercise.empty())++coinExerciseRefused;return;}
    consumeCoinHookEvents();if(!s.enabled)return;
    if(!coinConnection()||outgoing.session!=s.scope.session||remote.session!=s.scope.peerSession){stopCoinSync("session_disconnected");return;}
    if(doorCheckpointBarrierActive()){s.ready=false;clearCoinHookLease();return;}
    if(s.reloadPending){
        if(tick>s.deadline){stopCoinSync("checkpoint_renewal_timeout");return;}
        if(!s.reloadReady||!doorSessionRenewalReady(s.scope,outgoing,remote)||!lastReceive||tick-lastReceive>=250)return;
        if(!bindSharedCoin()){stopCoinSync("checkpoint_coin_binding_refused");return;}++s.rearms;
    }
    if(!doorSessionCanRetainMenu(s.scope,outgoing,remote)){stopCoinSync("world_or_native_phase_changed");return;}
    if(!lastReceive||tick-lastReceive>=1000){stopCoinSync("peer_pose_stale");return;}
    if(!coopWorldPairReady(outgoing,remote)){
        if(!s.paused){s.paused=true;s.pauseAt=tick;s.ready=false;}s.result="native_menu_suspended";return;
    }
    if(s.paused){if(tick-lastReceive>=250)return;for(auto& c:s.entries){c.deadline=doorResumeDeadline(c.deadline,tick-s.pauseAt);c.helloAt=c.helloSent=c.actionSent=0;if(localRole)c.ack=0;}
        s.paused=false;s.ready=false;s.lastPeerHello=0;s.result="resumed_after_menu";}
    if(!coinReadyContext(tick))return;
    // Send the local identity list until the peer heartbeat confirms the same
    // shared digest (the list is reliable; repeats cover a peer that bound late).
    if((!s.settled||s.peerDigest!=s.sharedDigest)&&(!s.listSentAt||tick-s.listSentAt>=1000)&&sendCoinCatalog(tick))s.listSentAt=tick;
    if(!s.settled){
        if(!s.peerListValid)return;
        const auto& peer=s.peerList;const auto before=s.entries.size();
        for(auto& c:s.entries){const auto at=std::lower_bound(peer.begin(),peer.end(),c.identity,[](const auto& e,std::uint64_t id){return e.identity<id;});
            c.peerArm=at!=peer.end()&&at->identity==c.identity?at->arm:0;}
        // Unpaired entries become ordinary local native pickups (lease released).
        s.entries.erase(std::remove_if(s.entries.begin(),s.entries.end(),[](const auto& c){return !c.peerArm;}),s.entries.end());
        std::vector<std::uint64_t> ids;for(const auto& c:s.entries)ids.push_back(c.identity);
        s.localOnly=static_cast<unsigned>(before-s.entries.size());s.sharedDigest=coinSharedDigest(ids);s.settled=true;s.cursor=s.helloCursor=0;publishCoinHookLease();
        if(s.entries.empty()){stopCoinSync("no_shared_pickups");return;}
    }
    if(tick-s.helloSentAt>=100){auto& c=s.entries[s.helloCursor++%s.entries.size()];if(sendCoinPacket(c,CoinMessage::Hello,tick))s.helloSentAt=tick;}
    s.ready=s.peerDigest==s.sharedDigest&&s.peerCount==s.entries.size()&&s.lastPeerHello&&tick-s.lastPeerHello<1000;if(!s.ready)return;
    if(!exercise.empty()){
        const auto found=std::find_if(s.entries.begin(),s.entries.end(),[&](const auto& c){return c.binding.name==exercise;});
        if(!observerIsolation.ready||!observerIsolation.movementFixture||observerIsolation.until<tick+5000||
           movementBankActive()||shadowStream.until||reviewClipActive()||found==s.entries.end())++coinExerciseRefused;
        else if(!found->receipt.attempted&&!found->exerciseReceipt.attempted){
            const auto applied=applyNativeCoin(found->binding,found->exerciseReceipt,found->exerciseCalls,true);
            if(applied==CoinApply::Failed){stopCoinSync("catalog_exercise_application_unconfirmed");return;}
            if(applied==CoinApply::Deferred)++coinExerciseRefused;
            consumeCoinHookEvents();if(!s.enabled)return;
        }
    }
    if(coinReplayRequest){const auto& p=coinReplayRequest==2?coinRetiredAction:coinLastAction;coinReplayRequest=0;
        if(p.sequence&&coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)==k_EResultOK)++s.sent;else ++s.rejected;}
    const auto started=GetTickCount64();
    for(std::size_t visited=0;visited<s.entries.size()&&s.enabled;++visited){auto& c=s.entries[s.cursor++%s.entries.size()];
        if(!localRole&&c.receipt.nativeSource&&!c.revision){c.revision=1;++s.commits;c.deadline=tick+10000;}
        if(localRole&&c.receipt.nativeSource&&!c.revision&&(!c.actionSent||tick-c.actionSent>=250))if(sendCoinPacket(c,CoinMessage::Request,tick))c.actionSent=tick;
        if(!c.revision)continue;
        if(!c.receipt.awarded){
            if(c.deadline&&tick>c.deadline){stopCoinSync("native_award_wait_timeout");return;}
            // Drain under the reward guard so a just-completed native pickup
            // cannot be mistaken for a disappeared recipient before its event.
            CoinWalletGuard wallet(true);if(!wallet.acquired)continue;consumeCoinHookEvents();if(!s.enabled)return;
            const auto applied=applyNativeCoin(c.binding,c.receipt,c.calls);
            if(applied==CoinApply::Failed){stopCoinSync("native_application_unconfirmed_no_retry");return;}
            if(applied==CoinApply::Deferred)continue;
        }
        c.result=s.result="coin_committed_once";
        if(!c.ack&&(!c.actionSent||tick-c.actionSent>=250)){
            if(sendCoinPacket(c,localRole?CoinMessage::Ack:CoinMessage::Commit,tick)){c.actionSent=tick;if(localRole)c.ack=1;}
        }
        if(GetTickCount64()-started>=3)break;
    }
}
void receivePickupPresence(const PickupPresencePacket& p,std::size_t bytes,ULONGLONG tick){
    auto& c=pickupCatchUp;
    if(!validPickupPresence(p,bytes,outgoing,remote)){++c.rejected;return;}
    if(c.epoch!=outgoing.worldEpoch||c.peerEpoch!=remote.worldEpoch||c.map!=outgoing.mapId){c.attempted.clear();c.targets.clear();c.cursor=0;c.receipt={};c.peer.clear();c.peerAt=0;}
    c.previous.swap(c.peer);c.previousAt=c.peerAt; // 1214jv (#203): a target must be absent from both
    ++c.presenceReceived;c.peer.assign(p.entries,p.entries+p.header.count);c.peerAt=tick;
    c.epoch=outgoing.worldEpoch;c.peerEpoch=remote.worldEpoch;c.map=outgoing.mapId;c.result="presence_received";
}
// Engine thread, in the native mutation window, with the shared catalog ready: after each local census cycle, this
// world awards itself, one at a time through the shared pickup's native application, each eligible pickup that is not
// a shared catalog entry and is absent from the peer's last two presences, the older one received at least 10 s after
// this world first had the pickup. Since 1214jv (#203) the host does too, for authored pickups only.
void servicePickupCatchUp(ULONGLONG tick){
    servicePickupCensus(tick);sendPickupPresence(tick);
    if(!storyTakeRequest.empty()&&nativeMutationAllowed&&GetCurrentThreadId()==engineThread.load()){std::string name;name.swap(storyTakeRequest);storyTakeResult="not_found";
        for(const auto& [slot,a]:pickupCensus.actors)if(a.name==name){NativeCoin n{};
            if(!readNativeStoryPickup(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,a.handle,n)){storyTakeResult="not_story";break;}
            unsigned calls=0;const auto r=applyNativeStoryPickup(n,calls);storyTakeResult=r==CoinApply::Complete?"taken":r==CoinApply::Deferred?"deferred":"failed";
            if(r==CoinApply::Deferred)storyTakeRequest=name;else ++storyTakes;break;}}
    auto& c=pickupCatchUp;if(!c.peerAt||!c.previousAt)return;
    if(c.epoch!=outgoing.worldEpoch||c.peerEpoch!=remote.worldEpoch||c.map!=outgoing.mapId){c.peer.clear();c.previous.clear();c.peerAt=c.previousAt=0;c.targets.clear();c.cursor=0;c.receipt={};c.result="presence_out_of_scope";return;}
    // Stage 8.4.2 (#238): with items per player, only story pickups are caught up, without the shared catalog.
    const bool story=itemsPerPlayer;
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    if(story?!(coinConnection()&&coopWorldPairReady(outgoing,remote)&&coopGameplayReady(outgoing)&&!doorCheckpointBarrierActive()&&lastReceive&&tick-lastReceive<250):(!coinSync.ready||!coinReadyContext(tick)))return;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(c.cursor>=c.targets.size()&&pickupCensus.cycles&&c.plannedCycle!=pickupCensus.cycles){
        c.plannedCycle=pickupCensus.cycles;c.targets.clear();c.cursor=0;
        std::unordered_map<std::string,bool> shared;for(const auto& e:coinSync.entries)shared[e.binding.name]=true;
        for(const auto& item:pickupCensusItems()){
            const auto at=pickupCensus.actors.find(item.handle>>16);
            if(at==pickupCensus.actors.end()||c.previousAt<at->second.seenAt+10000||shared.count(item.name)||c.attempted.count(item.name)||!pickupCatchUpTarget(item,c.peer,c.previous,!localRole))continue;
            NativeCoin n{};if(story?readNativeStoryPickup(memoryRead,base,observedGame,item.handle,n):readNativeCoin(memoryRead,base,observedGame,item.handle,n,true)&&coinCatalogEligible(n))c.targets.push_back({item.name,item.handle}); // dormant: a later cycle
        }
        c.planned+=c.targets.size();c.result=c.targets.empty()?"caught_up":"catching_up";
    }
    if(c.cursor>=c.targets.size())return;
    const auto& t=c.targets[c.cursor];NativeCoin n{};
    if(!(story?readNativeStoryPickup(memoryRead,base,observedGame,t.handle,n):readNativeCoin(memoryRead,base,observedGame,t.handle,n,true))||n.name!=t.name){++c.cursor;c.receipt={};return;} // taken here meanwhile
    const auto r=story?applyNativeStoryPickup(n,c.calls):applyNativeCoin(n,c.receipt,c.calls);
    if(r==CoinApply::Deferred){++c.deferrals;return;}
    c.attempted[t.name]=true;
    auto& names=r==CoinApply::Complete?c.awardedNames:c.failedNames;++(r==CoinApply::Complete?c.awarded:c.failed);if(names.size()<64)names.push_back(t.name);
    ++c.cursor;c.receipt={};
}
void writePickupCatchUp(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.pickup-catchup.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;const auto& c=pickupCatchUp;const auto& s=pickupCensus;
    std::fprintf(f,"{\"storyTake\":\"%s\",\"storyTakes\":%llu,\"role\":%u,\"result\":\"%s\",\"census\":{\"cycles\":%llu,\"actors\":%zu,\"classifications\":%llu,\"maxReads\":%llu,\"lastCycleMs\":%llu,\"scanning\":%s},"
        "\"map\":%llu,\"epoch\":%llu,\"peerEpoch\":%llu,\"presenceSent\":%llu,\"sentEntries\":%u,\"refusedSends\":%llu,\"presenceReceived\":%llu,\"rejected\":%llu,\"peerEntries\":%zu,"
        "\"planned\":%llu,\"targets\":%zu,\"cursor\":%zu,\"awarded\":%llu,\"failed\":%llu,\"deferrals\":%llu,\"awardedNames\":[",
        storyTakeResult,storyTakes,localRole,c.result,s.cycles,s.actors.size(),s.classifications,s.maxReads,s.lastCycleMs,s.scanning?"true":"false",
        c.map,c.epoch,c.peerEpoch,c.presenceSent,c.sentEntries,c.refusedSends,c.presenceReceived,c.rejected,c.peer.size(),c.planned,c.targets.size(),c.cursor,c.awarded,c.failed,c.deferrals);
    const auto list=[&](const std::vector<std::string>& names){for(std::size_t i=0;i<names.size();++i){if(i)std::fputc(',',f);std::fputc('"',f);std::fputs(names[i].c_str(),f);std::fputc('"',f);}};
    list(c.awardedNames);std::fputs("],\"failedNames\":[",f);list(c.failedNames);std::fputs("],\"targetNames\":[",f);
    std::vector<std::string> targets;for(const auto& t:c.targets)targets.push_back(t.name);list(targets);
    std::fputs("]}\n",f);const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
void coinJsonString(FILE* f,const std::string& value){std::fputc('"',f);for(unsigned char c:value){if(c=='"'||c=='\\')std::fputc('\\',f);std::fputc(c,f);}std::fputc('"',f);}
void writeCoinSync(){
    writePickupCapacityFixture();writePickupCatchUp();
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.coin-sync.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;const auto& s=coinSync;
    // Retain the original test coin's flat diagnostics for existing regressions;
    // eligibility and gameplay above never special-case its name or value.
    CoinChannel empty{};const CoinChannel* primary=&empty;unsigned calls=0,awarded=0;
    for(const auto& c:s.entries){calls+=c.calls;awarded+=c.receipt.awarded;if(c.binding.name=="pickups_coin_emily_lowvalue_4")primary=&c;}
    const auto& c=*primary;
    std::fprintf(f,"{\"itemsPerPlayer\":%s,\"itemsPerPlayerRefusals\":%llu,\"bindRefusal\":\"%s\",\"bindRefusalItem\":\"%s\",\"bindRefusals\":%llu,\"enabled\":%s,\"ready\":%s,\"paused\":%s,\"reloadPending\":%s,\"reloadReady\":%s,\"observerInstalled\":%s,\"epoch\":%llu,\"peerEpoch\":%llu,\"arm\":%llu,\"peerArm\":%llu,\"revision\":%u,\"ack\":%u,\"attempted\":%s,\"awarded\":%s,\"nativeSource\":%s,\"before\":%d,\"after\":%d,\"nativeCalls\":%u,\"observations\":%llu,\"commits\":%llu,\"sent\":%llu,\"received\":%llu,\"rejected\":%llu,\"duplicates\":%llu,\"reloads\":%llu,\"rearms\":%llu,\"result\":\"%s\",\"settled\":%s,\"localOnly\":%u,\"peerOnly\":%llu,\"listsSent\":%llu,\"listsReceived\":%llu,\"sharedDigest\":\"%016llx\",\"peerDigest\":\"%016llx\",\"catalogCount\":%zu,\"catalogId\":\"%016llx\",\"awardedCount\":%u,\"entries\":[",
        itemsPerPlayer?"true":"false",itemsPerPlayerRefusals,coinBindRefusal,coinBindRefusalItem.c_str(),coinBindRefusals,s.enabled?"true":"false",s.ready?"true":"false",s.paused?"true":"false",s.reloadPending?"true":"false",s.reloadReady?"true":"false",realCoinReward?"true":"false",
        s.scope.epoch,s.scope.peerEpoch,c.arm,c.peerArm,c.revision,c.ack,c.receipt.attempted?"true":"false",c.receipt.awarded?"true":"false",c.receipt.nativeSource?"true":"false",
        c.receipt.before,c.receipt.after,calls,s.observations,s.commits,s.sent,s.received,s.rejected,s.duplicates,s.reloads,s.rearms,s.result,s.settled?"true":"false",s.localOnly,s.peerOnly,s.listsSent,s.listsReceived,s.sharedDigest,s.peerDigest,s.entries.size(),s.catalogId,awarded);
    bool first=true;for(const auto& entry:s.entries){if(!first)std::fputc(',',f);first=false;std::fputs("{\"name\":",f);coinJsonString(f,entry.binding.name);std::fputs(",\"item\":",f);coinJsonString(f,entry.binding.itemName);
        std::fprintf(f,",\"family\":%u,\"maximum\":%d,\"accepted\":%d,\"quantity\":%d,\"arm\":%llu,\"peerArm\":%llu,\"revision\":%u,\"ack\":%u,\"attempted\":%s,\"awarded\":%s,\"nativeSource\":%s,\"before\":%d,\"after\":%d,\"nativeCalls\":%u,\"observations\":%u,\"exerciseCalls\":%u,\"result\":\"%s\"",entry.binding.type,entry.receipt.maximum,entry.receipt.after-entry.receipt.before,entry.binding.quantity,entry.arm,entry.peerArm,entry.revision,entry.ack,entry.receipt.attempted?"true":"false",entry.receipt.awarded?"true":"false",entry.receipt.nativeSource?"true":"false",entry.receipt.before,entry.receipt.after,entry.calls,entry.observations,entry.exerciseCalls,entry.result);
        if(entry.binding.type==6){std::fputs(",\"foodBefore\":",f);writePickupFoodValue(f,entry.receipt.foodBefore);std::fputs(",\"foodAfter\":",f);writePickupFoodValue(f,entry.receipt.foodAfter);}
        std::fputc('}',f);
    }
    std::fputs("]}\n",f);const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
    std::snprintf(path,sizeof(path),"%s.coin-hook.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    if(fopen_s(&f,tmp,"wb")||!f)return;
    std::fprintf(f,"{\"calls\":%llu,\"candidates\":%llu,\"otherThreadCalls\":%llu,\"overflow\":%llu,\"lastThread\":%u,\"engineThread\":%u,\"grapple\":%llu,\"grappleValid\":%llu,\"refused\":%llu,\"otherState\":%llu,\"lastChecks\":%u,\"lastStateId\":%u,\"lastStateVtRva\":%u,\"lastGrapple\":%u}\n",coinHookCalls.load(),coinHookCandidates.load(),coinHookOtherThread.load(),coinHookOverflow.load(),coinHookLastThread.load(),engineThread.load(),coinHookGrapple.load(),coinHookGrappleValid.load(),coinHookRefused.load(),coinHookOtherState.load(),coinHookLastChecks.load(),coinHookLastStateId.load(),coinHookLastStateVt.load(),coinHookLastGrapple.load());
    const bool hookOk=!ferror(f);const int hookClosed=fclose(f);if(hookOk&&!hookClosed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}

