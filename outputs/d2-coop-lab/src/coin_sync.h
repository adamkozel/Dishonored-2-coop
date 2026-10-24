#pragma once
#include "coin_native_api.h"
#include "coin_catalog.h"
#include "door_session_scope.h"
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
bool bindSharedCoin(){
    auto& s=coinSync;const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t list=0;unsigned count=0;
    // 1059: map-agnostic; both peers on the same native campaign map and world.
    if(!nativeCoinApiAvailable()||!coopWorldPairReady(outgoing,remote)||
       !memoryRead(observedGame+0x601c0,&list,8)||!list||!memoryRead(observedGame+0x601c8,&count,4)||!(count&0xffffff)||(count&0xffffff)>16384)return false;
    std::vector<unsigned> handles(count&0xffffff);if(!memoryRead(list,handles.data(),handles.size()*4))return false;
    std::vector<NativeCoin> coins;
    for(auto h:handles){NativeCoin n{};if(readNativeCoin(memoryRead,base,observedGame,h,n,true))coins.push_back(std::move(n));}
    auto catalog=selectNativeCoinCatalog(std::move(coins));CoinPlayer player{};
    if(catalog.ambiguous||!catalog.digest||catalog.entries.empty()||catalog.entries.size()>kCoinCatalogMax||!readCoinPlayer(player))return false;
    for(const auto& n:catalog.entries){CoinOutcomeValue value{};if(!readCoinOutcome(player,n,value))return false;}
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
    // A developer command is consumed once even when the session cannot run.
    std::string exercise;exercise.swap(coinExerciseRequest);
    if(coinEnableRequest>=0){const bool on=coinEnableRequest!=0;coinEnableRequest=-1;
        if(!on)stopCoinSync("disabled");else if(!s.enabled){s={};coinLastAction=coinRetiredAction={};coinReplayRequest=0;
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
void coinJsonString(FILE* f,const std::string& value){std::fputc('"',f);for(unsigned char c:value){if(c=='"'||c=='\\')std::fputc('\\',f);std::fputc(c,f);}std::fputc('"',f);}
void writeCoinSync(){
    writePickupCapacityFixture();
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.coin-sync.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;const auto& s=coinSync;
    // Retain the original test coin's flat diagnostics for existing regressions;
    // eligibility and gameplay above never special-case its name or value.
    CoinChannel empty{};const CoinChannel* primary=&empty;unsigned calls=0,awarded=0;
    for(const auto& c:s.entries){calls+=c.calls;awarded+=c.receipt.awarded;if(c.binding.name=="pickups_coin_emily_lowvalue_4")primary=&c;}
    const auto& c=*primary;
    std::fprintf(f,"{\"enabled\":%s,\"ready\":%s,\"paused\":%s,\"reloadPending\":%s,\"reloadReady\":%s,\"observerInstalled\":%s,\"epoch\":%llu,\"peerEpoch\":%llu,\"arm\":%llu,\"peerArm\":%llu,\"revision\":%u,\"ack\":%u,\"attempted\":%s,\"awarded\":%s,\"nativeSource\":%s,\"before\":%d,\"after\":%d,\"nativeCalls\":%u,\"observations\":%llu,\"commits\":%llu,\"sent\":%llu,\"received\":%llu,\"rejected\":%llu,\"duplicates\":%llu,\"reloads\":%llu,\"rearms\":%llu,\"result\":\"%s\",\"settled\":%s,\"localOnly\":%u,\"peerOnly\":%llu,\"listsSent\":%llu,\"listsReceived\":%llu,\"sharedDigest\":\"%016llx\",\"peerDigest\":\"%016llx\",\"catalogCount\":%zu,\"catalogId\":\"%016llx\",\"awardedCount\":%u,\"entries\":[",
        s.enabled?"true":"false",s.ready?"true":"false",s.paused?"true":"false",s.reloadPending?"true":"false",s.reloadReady?"true":"false",realCoinReward?"true":"false",
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

