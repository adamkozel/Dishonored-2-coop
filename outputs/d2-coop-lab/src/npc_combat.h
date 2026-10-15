#pragma once
#include "native_trigger_identity.h"
#include "native_player_ranged_item.h"
// Experimental generic NPC melee authority, including native lethal outcomes.
// Both roles explicitly opt in after NPC authority negotiation. Client hits
// become requests; only the host orders outcomes. Unsupported damage stops
// this experiment rather than silently writing a different local health.
// 1214o: host-world damage (creature or NPC sources) applies natively on the
// host and reaches the client as an ordered HostWorld commit of the host's
// observed native loss, removed by the native health subtraction.
struct NpcCombatChannel {
    std::uint64_t arm=0,peerArm=0,peerNonce=0,event=0,request=0,peerRequest=0,pending=0;
    bool ready=false,enabled=true,terminal=false,deathPending=false;
    ULONGLONG started=0,retiredAt=0;const char* retired="none";
    ULONGLONG deathAt=0;unsigned deathSource=2;std::uint64_t deathRequest=0,deathEvent=0,deathSourceArm=0;
    NpcCombatDamage deathDamage=NpcCombatDamage::None;EnemyMeleeGeometry deathGeometry{};
    NativeHealth deathBefore{},deathAfter{};float deathInflicted=0;NpcCombatScriptDamage deathScript{};NpcCombatRangedDamage deathRanged{};
    NpcCombatPendingRequests pendingRequests{};
    float health=0; // 1214o host: the health last shared on this channel
};
struct NpcCombatPacketTrace {
    ULONGLONG tick=0;const char* result="none";
    bool sent=false;unsigned kind=0;std::uint64_t sourceNonce=0,targetNonce=0,peerNonce=0;
    NpcAuthorityHeader binding{};
    std::uint64_t localEpoch=0,peerEpoch=0;bool worldReady=false;
};
struct NpcCombatRuntime {
    bool enabled=false;std::uint64_t nonce=0;
    ULONGLONG helloAt=0,started=0;
    DoorSessionScope scope{};
    std::vector<NpcCombatChannel> channels;
    std::vector<NativeDamageTrigger> triggers;
    std::vector<CoopNpcCombat> outgoing;
    CoopNpcCombat lastSent{};
    std::uint64_t deaths=0,preDeathReleased=0,localHits=0,requests=0,committed=0,applied=0,duplicates=0,rejected=0,suppressed=0;
    const char* result="disabled";
    std::array<NpcCombatPacketTrace,64> trace{};std::uint64_t traceCount=0;
    std::array<std::uint64_t,6> sent{},received{};
    std::uint64_t sendFailures=0,worldCommitted=0,worldApplied=0,replicaWorldSuppressed=0;unsigned pendingHighWater=0;
    std::uint64_t hostWorldCommitted=0,hostWorldApplied=0,hostWorldGains=0,helloFollowed=0,helloWaited=0;
};
const char* npcCombatNativeStage="idle";float npcCombatNativeInflicted=0,npcCombatNativeAfter=0;unsigned npcCombatNativeGeneration=0;
NpcCombatRuntime npcCombat{};std::uint64_t npcCombatNonce=0;int npcCombatRequest=-1;
// 1213v (#46): the combat session follows dynamic NPC relevance. It bound only
// the channels present when it began, stopped when none were ready or any bound
// NPC retired, and was never re-armed, so under the product controller it was
// off (take 1248b: host "no_ready_native_actors") and no takedown or hit outcome
// was shared. Bindings now join and retire per NPC, per-NPC divergence retires
// only that NPC, and the product controller re-arms a stopped session.
bool npcCombatWanted=false;ULONGLONG npcCombatRearmAt=0;std::uint64_t npcCombatChannelsAdded=0,npcCombatChannelsRetired=0,npcCombatRearms=0;
const char* npcCombatLastRetire="none";
std::atomic<std::uint64_t> npcCombatHostWorldPassed{0};std::uint64_t npcCombatFollowSubtractions=0;
void retireNpcCombatChannel(NpcCombatChannel& b,const char* reason);
bool npcCombatTriggerFixtureRequested=false;std::uint64_t npcCombatTriggerFixtureNonce=0;
const char* npcCombatTriggerFixtureResult="unused";
void traceNpcCombat(const CoopNpcCombat& p,bool sent,const char* result,std::uint64_t peerNonce=0){
    auto& t=npcCombat.trace[npcCombat.traceCount++%npcCombat.trace.size()];
    t={GetTickCount64(),result,sent,static_cast<unsigned>(p.kind),p.sourceNonce,p.targetNonce,peerNonce,p.binding,
        outgoing.worldEpoch,remote.worldEpoch,coopWorldPairReady(outgoing,remote)};
}
void rejectNpcCombat(const CoopNpcCombat& p,const char* reason,std::uint64_t peerNonce=0){
    ++npcCombat.rejected;traceNpcCombat(p,false,reason,peerNonce);
}
struct NpcCombatNativeApplication {NpcAuthorityChannel* channel=nullptr;NativeHealth before{};bool seen=false,terminal=false,failed=false,reconcile=false;};
thread_local NpcCombatNativeApplication* npcCombatNativeApplication=nullptr;
std::uint64_t npcCombatSubtractions=0,npcCombatLethalSubtractions=0;
std::array<unsigned char,144> npcCombatLastInfo{};unsigned npcCombatLastOwnerFlags=0;std::string npcCombatLastDecl;
struct NpcCombatCaptureBinding {NativeNpcAuthority native{};std::uint64_t nonce=0,arm=0;};
struct NpcCombatCapture {std::uint64_t nonce=0,arm=0;ULONGLONG tick=0;DWORD thread=0;std::uintptr_t component=0;float position[3]{};alignas(16) std::array<unsigned char,144> info{};NativeDamageSource inflictor{},attacker{};unsigned rangedWeapon=0;};
SRWLOCK npcCombatCaptureLock=SRWLOCK_INIT;
std::vector<NpcCombatCaptureBinding> npcCombatCaptureBindings;
std::vector<NpcCombatCapture> npcCombatCaptures;
std::array<NpcCombatCapture,8> npcCombatSourceHistory{};std::uint64_t npcCombatSourceCount=0;
std::atomic<unsigned> npcCombatCaptureFault{0};
std::atomic<std::uint64_t> npcCombatCaptured{0},npcCombatWorkerCaptured{0};
DWORD npcCombatLastCaptureThread=0;
std::atomic<ULONGLONG> npcCombatOrdinaryTestUntil{0};std::atomic<std::uint64_t> npcCombatTestAssassinationsRefused{0};
bool npcCombatTestRejectAssassination(std::uintptr_t context,std::uintptr_t candidate){
    if(GetTickCount64()>=npcCombatOrdinaryTestUntil.load())return false;
    bool reject=false;const auto base=reinterpret_cast<std::uintptr_t>(gameImage);AcquireSRWLockShared(&npcCombatCaptureLock);
    for(const auto& b:npcCombatCaptureBindings)if(candidate==b.native.npc.actor){
        const auto& n=b.native;std::uintptr_t states=0,owner=0,vt=0,game=0;unsigned generation=0;
        const auto player=worldIdentityEntity(memoryRead,n.health.game,1);
        reject=player&&memoryRead(base+0x39a5290,&game,8)&&game==n.health.game&&worldIdentityEntity(memoryRead,game,n.npc.handle)==candidate&&
            memoryRead(n.health.component+0x18,&generation,4)&&generation==n.healthGeneration&&memoryRead(context+0x20,&states,8)&&states&&
            memoryRead(states,&vt,8)&&vt==base+0x1c73d58&&memoryRead(states+0x10,&owner,8)&&owner==player;break;
    }
    ReleaseSRWLockShared(&npcCombatCaptureLock);if(reject)++npcCombatTestAssassinationsRefused;return reject;
}
void clearNpcCombatCapture(){
    npcCombatOrdinaryTestUntil.store(0);
    AcquireSRWLockExclusive(&npcCombatCaptureLock);npcCombatCaptureBindings.clear();npcCombatCaptures.clear();ReleaseSRWLockExclusive(&npcCombatCaptureLock);
}
// 1213w (#46): knockout damage passes through the capture. A native choke knocks
// its victim out with code_referenced/damage/generic_sleep_damage (probe 1251c
// reproduced the choke knockout exactly); captured, it was refused as unknown
// ("hit_damage_parameters"), suppressed - no knockout - and stopped the session
// (take 1248c host choke). Unconsciousness is shared by the NPC stream instead
// (Unconscious host->client, PairedKnockout client->host).
bool npcCombatPassThroughDecl(std::uintptr_t decl){
    std::uintptr_t name=0;std::string path;
    return decl&&memoryRead(decl+8,&name,8)&&worldIdentityString(memoryRead,name,path)&&
        (path=="code_referenced/damage/generic_sleep_damage.damage"||path=="components/base/damage/sleep_damage.damage"||path=="components/base/damage/sleep_no_reaction_damage.damage");
}
std::atomic<std::uint64_t> npcCombatPassedThrough{0};
bool routeNpcCombatDamage(void* component,void* info,void* result){
    // Explicit applications call realNativeDamage directly. Nested damage
    // callbacks still need ordinary capture/ordering; do not bypass all of
    // them merely because the original event is being applied.
    bool handled=false;AcquireSRWLockExclusive(&npcCombatCaptureLock);
    for(const auto& b:npcCombatCaptureBindings){const auto& n=b.native;const auto c=reinterpret_cast<std::uintptr_t>(component);if(c!=n.health.component)continue;
        std::uintptr_t owner=0,game=0;unsigned generation=0;
        if(!memoryRead(reinterpret_cast<std::uintptr_t>(gameImage)+0x39a5290,&game,8)||game!=n.health.game||worldIdentityEntity(memoryRead,n.health.game,n.npc.handle)!=n.npc.actor||!memoryRead(c+0x10,&owner,8)||owner!=n.npc.actor||
           !memoryRead(c+0x18,&generation,4)||generation!=n.healthGeneration)continue;
        std::uintptr_t decl=0;if(memoryRead(reinterpret_cast<std::uintptr_t>(info),&decl,8)&&npcCombatPassThroughDecl(decl)){++npcCombatPassedThrough;break;}
        if(!localRole){unsigned inflictor=0,attacker=0;const auto i=reinterpret_cast<std::uintptr_t>(info),base=reinterpret_cast<std::uintptr_t>(gameImage);
            if(memoryRead(i+0x50,&inflictor,4)&&memoryRead(i+0x54,&attacker,4)){
                const auto in=readNativeDamageSource(memoryRead,base,game,inflictor),at=readNativeDamageSource(memoryRead,base,game,attacker);
                if(npcCombatHostWorldSource(localRole,attacker,inflictor,at.valid&&at.localOwned,in.valid&&in.localOwned)){npcCombatHostWorldPassed.fetch_add(1);break;}}}
        handled=true;NpcCombatCapture e{};e.nonce=b.nonce;e.arm=b.arm;e.tick=GetTickCount64();e.thread=GetCurrentThreadId();e.component=c;
        if(npcCombatCaptures.size()>=64||!memoryRead(reinterpret_cast<std::uintptr_t>(info),e.info.data(),e.info.size())||!memoryRead(n.npc.actor+0x300,e.position,12))npcCombatCaptureFault.store(1);
        else{
            unsigned inflictor=0,attacker=0;std::memcpy(&inflictor,e.info.data()+0x50,4);std::memcpy(&attacker,e.info.data()+0x54,4);
            const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
            e.inflictor=readNativeDamageSource(memoryRead,base,game,inflictor);e.attacker=readNativeDamageSource(memoryRead,base,game,attacker);
            std::uint64_t flags=0;std::memcpy(&flags,e.info.data()+0x10,8);
            const auto weapon=npcCombatRangedWeapon(flags);
            if(weapon&&attacker==1&&e.attacker.valid&&e.attacker.localOwned&&e.inflictor.valid&&e.inflictor.localOwned){
                NativePlayerRangedItem gun{};
                if(readNativePlayerRangedItem(memoryRead,base,weapon,gun,true)&&gun.quantity==1&&gun.equipped&&gun.started&&
                   (flags==0x100000000ull?(inflictor==gun.handle&&e.inflictor.actor==gun.entity):(e.inflictor.vtable==base+0x1c5e758&&e.inflictor.parent==1)))e.rangedWeapon=weapon;
            }
            npcCombatSourceHistory[npcCombatSourceCount++%npcCombatSourceHistory.size()]=e;
            npcCombatCaptures.push_back(e);++npcCombatCaptured;if(e.thread!=engineThread.load())++npcCombatWorkerCaptured;}
        break;
    }
    ReleaseSRWLockExclusive(&npcCombatCaptureLock);
    // The established native empty-result constructor is also used by the
    // friendly-fire filter on worker callbacks. Game mutations happen later
    // on the engine thread, never while holding this capture lock.
    if(handled)reinterpret_cast<void(*)(void*)>(gameImage+0x780470)(result);
    return handled;
}
// Test-only finite source-AI hold. Never enabled by the combat protocol or
// normal gameplay. It isolates damage transport from moving/counterattacking
// source actors, and uses the same generation-checked native pause leases.
struct NpcCombatFixture {std::vector<NpcAiLease> leases;ULONGLONG until=0,started=0;bool ready=false,holdFrames=true;std::uint64_t acquired=0,released=0,lost=0,pairedFrameReleases=0;const char* result="disabled";};
NpcCombatFixture npcCombatFixture{};int npcCombatFixtureRequest=-1;
std::vector<NpcRenderIdentity> npcCombatFixtureFrames;
std::atomic<std::uint64_t> npcCombatFixtureFrameHolds{0};
bool npcCombatFixtureOwnsFrame(void* frame){
    bool held=false;AcquireSRWLockShared(&npcPresentationLock);
    for(const auto& id:npcCombatFixtureFrames)if(id.native.frame==reinterpret_cast<std::uintptr_t>(frame)&&npcRenderCurrent(id)){held=true;break;}
    ReleaseSRWLockShared(&npcPresentationLock);if(held)++npcCombatFixtureFrameHolds;return held;
}
void stopNpcCombatFixture(const char* reason){
    AcquireSRWLockExclusive(&npcPresentationLock);npcCombatFixtureFrames.clear();ReleaseSRWLockExclusive(&npcPresentationLock);
    auto& f=npcCombatFixture;for(auto& lease:f.leases)if(lease.references){releaseNpcAiLease(lease);f.released+=lease.released;f.lost+=lease.lost+lease.retired;}
    f.until=0;f.ready=false;f.result=reason;
}
NpcAuthorityChannel* npcCombatActor(std::uint64_t arm){
    auto it=std::find_if(npcChannels.begin(),npcChannels.end(),[&](const auto& c){return c.arm==arm;});return it==npcChannels.end()?nullptr:&*it;
}
bool npcCombatScope(ULONGLONG tick){return npcCombat.enabled&&npcAuthorityStreamActive&&npcStreamSessionState.enabled&&
    !npcStreamSessionState.reloadPending&&!npcStreamSessionState.menuSuspended&&!doorCheckpointBarrierActive()&&
    GetCurrentThreadId()==engineThread.load()&&coopNpcPairReady(tick)&&doorSessionWorldMatches(npcCombat.scope,outgoing,remote);}
bool npcCombatRangedFixtureReady(ULONGLONG tick,unsigned minimumMs){
    if(!npcCombatScope(tick)||npcCombat.channels.empty())return false;
    // Acquisition needs living ready targets; retaining already acquired
    // equipment must allow an ordered native death and its terminal tombstone.
    for(const auto& b:npcCombat.channels)if(!b.ready||!b.enabled||(minimumMs&&(b.terminal||b.deathPending)))return false;
    return localRole==1||(npcCombatFixture.ready&&npcCombatFixture.until>=tick+minimumMs);
}
CoopNpcCombat npcCombatPacket(NpcAuthorityChannel& c,NpcCombatChannel& b,NpcCombatMessage kind){
    CoopNpcCombat p{};p.binding=npcHeader(c,NpcAuthorityMessage::Stop);p.kind=kind;p.sourceNonce=npcCombat.nonce;
    if(kind!=NpcCombatMessage::Hello)p.targetNonce=b.peerNonce;
    return p;
}
bool sendNpcCombat(const CoopNpcCombat& p){
    const char* failure=nullptr;
    if(!sockets||!connection||connectionState!=k_ESteamNetworkingConnectionState_Connected)failure="send_disconnected";
    else if(!validNpcCombat(p,remote,outgoing))failure="send_invalid_packet";
    else if(coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)!=k_EResultOK)failure="send_transport_failed";
    if(failure){++npcCombat.sendFailures;traceNpcCombat(p,true,failure);return false;}
    ++npcCombat.sent[static_cast<unsigned>(p.kind)];traceNpcCombat(p,true,"sent");
    if(p.kind==NpcCombatMessage::Request||p.kind==NpcCombatMessage::Commit)npcCombat.lastSent=p;return true;
}
void stopNpcCombat(const char* reason,bool notify=true){
    npcCombatTriggerFixtureRequested=false;
    clearNpcCombatCapture();
    stopNpcCombatFixture(reason);
    if(npcCombat.enabled&&notify)for(auto& b:npcCombat.channels){auto* c=npcCombatActor(b.arm);if(c&&b.peerNonce)sendNpcCombat(npcCombatPacket(*c,b,NpcCombatMessage::Stop));}
    npcCombat.enabled=false;npcCombat.result=reason;npcCombat.outgoing.clear();for(auto& b:npcCombat.channels)b.ready=false;
}
bool npcCombatIdentity(NpcAuthorityChannel& c,NativeHealth& health){
    NativeNpcAuthority fresh{};return c.enabled&&c.ready&&c.peerArm&&c.lease.binding.npc.handle!=probeHandle&&
        readNativeNpcAuthority(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,c.lease.binding.npc.handle,fresh)&&
        sameNpcAuthorityIdentity(c.lease.binding,fresh,false)&&npcAuthorityLiving(fresh)&&(!localRole||npcAuthorityPaused(fresh))&&(health=fresh.health,true);
}
bool npcCombatNear(NpcAuthorityChannel& c,unsigned role,float range=3){
    const float* p=role==localRole?outgoing.position:remote.position;float root[3]{},d=0;
    if(!memoryRead(c.lease.binding.npc.actor+0x300,root,12))return false;
    for(unsigned i=0;i<3;++i){const float x=root[i]-p[i];if(!std::isfinite(x))return false;d+=x*x;}return d<=range*range;
}
constexpr char kNpcFatalityDamage[]="components/characters/player/base/damage/generic_fatality.damage";
constexpr char kNpcCorpseDamage[]="components/base/damage/corpse_damage.damage";
constexpr const char* kPlayerKickDamageDecl="components/characters/player/base/damage/player_kick.damage";
const char* npcCombatDeclName(NpcCombatDamage damage){
    return damage==NpcCombatDamage::Sword?kPlayerSwordDamageDecl:damage==NpcCombatDamage::Kick?kPlayerKickDamageDecl:damage==NpcCombatDamage::Fatality?kNpcFatalityDamage:damage==NpcCombatDamage::Assassination?kAssassinationDamage:damage==NpcCombatDamage::Corpse?kNpcCorpseDamage:nullptr;
}
bool npcCombatDeclaration(std::uintptr_t decl,NpcCombatDamage damage){
    if(damage==NpcCombatDamage::Sword)return nativePlayerSwordDeclaration(decl);
    if(damage==NpcCombatDamage::Kick){const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t vt=0,manager=0,name=0;std::uint64_t type=0;float amount=0;unsigned reactions[2]{};std::string path;
        return decl&&memoryRead(decl,&vt,8)&&vt==base+0x1c5d138&&memoryRead(decl+0x10,&manager,8)&&manager==base+0x3990b60&&
            memoryRead(decl+8,&name,8)&&worldIdentityString(memoryRead,name,path)&&path==kPlayerKickDamageDecl&&
            memoryRead(decl+0x5c,&amount,4)&&amount==10&&memoryRead(decl+0x60,&type,8)&&type==0x80&&
            memoryRead(decl+0x68,reactions,8)&&reactions[0]==1&&reactions[1]==1;}
    if(damage==NpcCombatDamage::Assassination)return nativeAssassinationDeclaration(decl);
    if(damage!=NpcCombatDamage::Fatality&&damage!=NpcCombatDamage::Corpse)return false;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t vt=0,manager=0,name=0;std::uint64_t flags=0;float amount=0;unsigned reactions[2]{};std::string path;
    return decl&&memoryRead(decl,&vt,8)&&vt==base+0x1c5d138&&memoryRead(decl+0x10,&manager,8)&&manager==base+0x3990b60&&
        memoryRead(decl+8,&name,8)&&worldIdentityString(memoryRead,name,path)&&path==npcCombatDeclName(damage)&&
        memoryRead(decl+0x5c,&amount,4)&&amount==(damage==NpcCombatDamage::Corpse?1.f:1000.f)&&memoryRead(decl+0x60,&flags,8)&&flags==(damage==NpcCombatDamage::Corpse?0x12ull:0x84ull)&&
        memoryRead(decl+0x68,reactions,8)&&reactions[0]==2&&reactions[1]==2;
}
NpcAuthorityChannel* npcCombatCorpseSource(std::uint64_t arm,bool peer,NativeHealth& h,bool& pending){
    for(const auto& b:npcCombat.channels)if(b.enabled&&(peer?b.peerArm:b.arm)==arm&&(b.terminal||b.deathPending)){
        auto* c=npcCombatActor(b.arm);if(!c)return nullptr;const auto& n=c->lease.binding;unsigned generation=0;
        if(readNativeHealth(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),n.npc.handle,h)&&h.game==observedGame&&h.game==n.health.game&&
           h.actor==n.npc.actor&&h.component==n.health.component&&h.declaration==n.health.declaration&&h.current==0&&
           memoryRead(h.component+0x18,&generation,4)&&generation==n.healthGeneration){pending=b.deathPending;return c;}
        return nullptr;
    }return nullptr;
}
bool npcCombatSourceNear(NpcAuthorityChannel& victim,unsigned role,std::uint64_t sourceArm){
    if(role<2)return !sourceArm&&npcCombatNear(victim,role);
    NativeHealth h{};bool pending=false;auto* source=npcCombatCorpseSource(sourceArm,false,h,pending);
    if(!source||(pending&&!localRole)||source->arm==victim.arm)return false;
    if(localRole)return true; // Host validates contact; replica corpse physics is not authoritative.
    float a[3]{},b[3]{},distance=0;if(!memoryRead(h.actor+0x300,a,12)||!memoryRead(victim.lease.binding.npc.actor+0x300,b,12))return false;
    for(unsigned i=0;i<3;++i){const float d=a[i]-b[i];if(!std::isfinite(d))return false;distance+=d*d;}return distance<=9;
}
bool npcCombatTrigger(std::uint64_t key,unsigned handle,NativeDamageTrigger& out){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    for(const auto& saved:npcCombat.triggers)if(saved.key&&(key?saved.key==key:saved.handle==handle)){
        NativeDamageTrigger fresh{};
        if(!readNativeDamageTrigger(memoryRead,base,observedGame,saved.handle,fresh)||!sameNativeDamageTrigger(saved,fresh))return false;
        out=std::move(fresh);return true;
    }return false;
}
bool npcCombatScriptDeclaration(std::uintptr_t decl,const NpcCombatScriptDamage& script,std::uint64_t& flags,unsigned* reactions){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t vt=0,manager=0,name=0;float maximum=0;std::string path;
    const char* expected=npcDamageDeclarationName(script.declaration);
    return validNpcScript(script)&&expected&&decl&&memoryRead(decl,&vt,8)&&vt==base+0x1c5d138&&
        memoryRead(decl+0x10,&manager,8)&&manager==base+0x3990b60&&memoryRead(decl+8,&name,8)&&worldIdentityString(memoryRead,name,path)&&path==expected&&
        memoryRead(decl+0x5c,&maximum,4)&&std::isfinite(maximum)&&maximum>=script.amount&&maximum<=1000000&&
        memoryRead(decl+0x60,&flags,8)&&memoryRead(decl+0x68,reactions,8);
}
bool npcCombatRangedDeclaration(std::uintptr_t decl,NpcCombatDamage damage,const NpcCombatRangedDamage& r){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t vt=0,manager=0,name=0;float minimum=0,maximum=0;std::uint64_t flags=0;unsigned reactions[2]{};std::string path;
    const auto* profile=npcCombatRangedProfile(damage,r);
    return profile&&validNpcRanged(damage,r)&&decl&&memoryRead(decl,&vt,8)&&vt==base+0x1c5d138&&
        memoryRead(decl+0x10,&manager,8)&&manager==base+0x3990b60&&memoryRead(decl+8,&name,8)&&worldIdentityString(memoryRead,name,path)&&
        path==npcDamageDeclarationName(r.declaration)&&memoryRead(decl+0x5c,&maximum,4)&&maximum==profile->maximum&&r.amount<=maximum&&
        memoryRead(decl+0x60,&flags,8)&&flags==profile->flags&&
        memoryRead(decl+0x68,reactions,8)&&reactions[0]==2&&reactions[1]==profile->reaction;
}
bool npcCombatExtract(NpcAuthorityChannel& c,void* info,NpcCombatDamage& damage,EnemyMeleeGeometry& g,const float* capturedPosition,std::uint64_t& sourceArm,bool& sourcePending,NpcCombatScriptDamage& script,NpcCombatRangedDamage& ranged,const NpcCombatCapture& capture){
    std::array<unsigned char,144> raw{};std::uintptr_t decl=0;unsigned inflictor=0,attacker=0;std::uint64_t flags=0;float amount=0,position[3]{};
    if(!memoryRead(reinterpret_cast<std::uintptr_t>(info),raw.data(),raw.size()))return false;
    std::memcpy(position,capturedPosition,12);
    std::memcpy(&decl,raw.data(),8);std::memcpy(&amount,raw.data()+8,4);std::memcpy(&flags,raw.data()+0x10,8);
    std::memcpy(&inflictor,raw.data()+0x50,4);std::memcpy(&attacker,raw.data()+0x54,4);
    NativeDamageTrigger trigger{};
    if(inflictor==0x3fff0010&&npcCombatTrigger(0,attacker,trigger)){
        std::uintptr_t name=0;std::string path;std::uint64_t declaredFlags=0;unsigned declaredReactions[2]{},actualReactions[2]{};
        if(!memoryRead(decl+8,&name,8)||!worldIdentityString(memoryRead,name,path))return false;
        script={trigger.key,npcDamageKey(path),amount,0};std::memcpy(actualReactions,raw.data()+0x18,8);
        if(!npcCombatScriptDeclaration(decl,script,declaredFlags,declaredReactions)||flags!=declaredFlags||
           actualReactions[0]!=declaredReactions[0]||actualReactions[1]!=declaredReactions[1])return false;
        unsigned regions=0,region=0;float values[9]{};std::memcpy(&regions,raw.data()+0x20,4);std::memcpy(&region,raw.data()+0x4c,4);std::memcpy(values,raw.data()+0x24,36);
        if(regions||region!=1)return false;
        damage=NpcCombatDamage::MapTrigger;g={};
        if(values[0]==1e30f&&values[1]==1e30f&&values[2]==1e30f){
            for(unsigned i=3;i<9;++i)if(values[i]!=0)return false;return true;
        }
        g.region=region;std::memcpy(g.offset,values,12);std::memcpy(g.normal,values+3,12);std::memcpy(g.direction,values+6,12);
        for(unsigned i=0;i<3;++i)g.offset[i]-=position[i];return npcCombatGeometry(damage,g);
    }
    if(capture.rangedWeapon&&capture.rangedWeapon==npcCombatRangedWeapon(flags)&&attacker==1){
        damage=flags==0x100000000ull?NpcCombatDamage::RangedSpread:NpcCombatDamage::RangedContact;
        std::uintptr_t name=0;std::string path;if(!memoryRead(decl+8,&name,8)||!worldIdentityString(memoryRead,name,path))return false;
        ranged.declaration=npcDamageKey(path);ranged.amount=amount;ranged.weapon=capture.rangedWeapon;std::memcpy(&ranged.distance,raw.data()+0x48,4);
        unsigned reactions[2]{};std::memcpy(reactions,raw.data()+0x18,8);
        const auto* profile=npcCombatRangedProfile(damage,ranged);
        if(!profile||!npcCombatRangedDeclaration(decl,damage,ranged)||reactions[0]!=2||reactions[1]!=profile->reaction||(raw[0x88]&1))return false;
        std::memcpy(&g.regions,raw.data()+0x20,4);std::memcpy(g.offset,raw.data()+0x24,12);std::memcpy(g.normal,raw.data()+0x30,12);
        std::memcpy(g.direction,raw.data()+0x3c,12);std::memcpy(&g.region,raw.data()+0x4c,4);for(unsigned i=0;i<3;++i)g.offset[i]-=position[i];
        return npcCombatGeometry(damage,g);
    }
    if(amount==1&&flags==0x12&&attacker==0x3fff0010){
        damage=NpcCombatDamage::Corpse;NativeHealth h{};
        for(const auto& b:npcCombat.channels){auto* source=npcCombatActor(b.arm);
            if(source&&source->lease.binding.npc.handle==inflictor&&npcCombatCorpseSource(b.arm,false,h,sourcePending)){sourceArm=b.arm;break;}}
        if(!sourceArm||sourceArm==c.arm)return false;
    }else if(attacker==1&&inflictor==1&&amount==10&&flags==0x80&&npcCombatDeclaration(decl,NpcCombatDamage::Kick))damage=NpcCombatDamage::Kick;
    else if(attacker!=1||!enemyPlayerSword(inflictor))return false;
    else if(amount==10&&flags==0x80)damage=NpcCombatDamage::Sword;
    else if(amount==1000&&flags==0x84)damage=NpcCombatDamage::Fatality;
    else if(amount==0&&flags==0x8c)damage=NpcCombatDamage::Assassination;
    else return false;
    if(!npcCombatDeclaration(decl,damage))return false;
    unsigned reactions[2]{};std::memcpy(reactions,raw.data()+0x18,8);
    const unsigned reaction=(damage==NpcCombatDamage::Sword||damage==NpcCombatDamage::Kick)?1:(damage==NpcCombatDamage::Fatality||damage==NpcCombatDamage::Corpse)?2:0;
    if(reactions[0]!=reaction||reactions[1]!=reaction)return false;
    if(damage==NpcCombatDamage::Assassination){
        // Native absent-hit sentinels: canonicalize to no wire geometry;
        // constructors restore native defaults, never these process bytes.
        unsigned regions=0,region=0;float absent[9]{};std::memcpy(&regions,raw.data()+0x20,4);std::memcpy(&region,raw.data()+0x4c,4);std::memcpy(absent,raw.data()+0x24,36);
        if(regions||region!=1||absent[0]!=1e30f||absent[1]!=1e30f||absent[2]!=1e30f)return false;
        for(unsigned i=3;i<9;++i)if(absent[i]!=0)return false;
        g={};return true;
    }
    std::memcpy(&g.regions,raw.data()+0x20,4);std::memcpy(g.offset,raw.data()+0x24,12);std::memcpy(g.normal,raw.data()+0x30,12);
    std::memcpy(g.direction,raw.data()+0x3c,12);std::memcpy(&g.region,raw.data()+0x4c,4);for(unsigned i=0;i<3;++i)g.offset[i]-=position[i];
    return npcCombatGeometry(damage,g);
}
bool npcCombatRetireBeforeDeath(NpcAuthorityChannel& c){
    // Wait out any in-flight render/capture reader before releasing native
    // references. No Stop is sent ahead of the authoritative death commit.
    AcquireSRWLockExclusive(&npcCombatCaptureLock);
    npcCombatCaptureBindings.erase(std::remove_if(npcCombatCaptureBindings.begin(),npcCombatCaptureBindings.end(),[&](const auto& b){return b.arm==c.arm;}),npcCombatCaptureBindings.end());
    ReleaseSRWLockExclusive(&npcCombatCaptureLock);
    clearNpcPresentation(c.arm);
    AcquireSRWLockExclusive(&npcPresentationLock);
    npcCombatFixtureFrames.erase(std::remove_if(npcCombatFixtureFrames.begin(),npcCombatFixtureFrames.end(),[&](const auto& f){return f.native.npc.actor==c.lease.binding.npc.actor;}),npcCombatFixtureFrames.end());
    ReleaseSRWLockExclusive(&npcPresentationLock);
    auto& f=npcCombatFixture;
    for(auto it=f.leases.begin();it!=f.leases.end();){
        if(it->binding.npc.actor!=c.lease.binding.npc.actor){++it;continue;}
        releaseNpcAiLease(*it);f.released+=it->released;f.lost+=it->lost+it->retired;npcCombat.preDeathReleased+=it->released;
        const bool clean=!it->references&&!it->lost&&!it->retired;it=f.leases.erase(it);if(!clean)return false;
    }
    // 1113: release the AI ownership for the native death, but keep the
    // channel as a corpse stream (host body authority for the settle window).
    releaseNpcAiLease(c.lease);{auto& s=npcStreamSessionState;s.released+=c.lease.released;s.retired+=c.lease.retired;s.lost+=c.lease.lost;}
    c.corpse=true;c.corpseAt=GetTickCount64();c.corpseUntil=c.corpseAt+8000;c.result=localRole?"client_corpse":"host_corpse";npcCombat.preDeathReleased+=c.lease.released;
    return !c.lease.references&&!c.lease.lost&&!c.lease.retired;
}
bool npcCombatReadResult(NpcAuthorityChannel& c,const NativeHealth& before,void* result,NativeHealth& after,float& inflicted,bool terminal,bool allowZero=false){
    unsigned protectedRegions=0,generation=0;float mana=0;const auto r=reinterpret_cast<std::uintptr_t>(result);const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    npcCombatNativeStage="result_or_health_identity";
    memoryRead(r,&npcCombatNativeInflicted,4);memoryRead(before.component+0x20,&npcCombatNativeAfter,4);memoryRead(before.component+0x18,&npcCombatNativeGeneration,4);
    if(!memoryRead(r,&inflicted,4)||!std::isfinite(inflicted)||(allowZero?inflicted<0:inflicted<=0)||!memoryRead(r+4,&mana,4)||mana!=0||
       !memoryRead(r+8,&protectedRegions,4)||protectedRegions||!readNativeHealth(memoryRead,base,before.handle,after)||
       after.game!=before.game||after.actor!=before.actor||after.component!=before.component||after.declaration!=before.declaration||
       !memoryRead(after.component+0x18,&generation,4)||generation!=c.lease.binding.healthGeneration||after.maximum!=before.maximum)return false;
    if(!terminal)return after.current>0&&npcCombatClose(before.current-after.current,inflicted)&&npcCombatIdentity(c,after);
    npcCombatNativeStage="await_native_death_completion";
    // This validates only the native damage result. Completion is separately
    // checked over following engine services, while this actor stays retired.
    return after.current==0&&inflicted<=before.current&&before.current-inflicted<0.5f;
}
bool npcCombatDeadProof(NpcAuthorityChannel& c,const NativeHealth& expected){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);NativeHealth h{};unsigned generation=0;
    return readNativeHealth(memoryRead,base,expected.handle,h)&&h.game==expected.game&&h.actor==expected.actor&&h.component==expected.component&&
        h.declaration==expected.declaration&&h.current==0&&h.maximum==expected.maximum&&
        memoryRead(h.component+0x18,&generation,4)&&generation==c.lease.binding.healthGeneration&&
        readNativeDeadAiRetirement(memoryRead,base,c.lease.binding.ai[0])&&readNativeDeadAiRetirement(memoryRead,base,c.lease.binding.ai[1]);
}
bool prepareNpcCombatSubtraction(void* component,float amount){
    auto* a=npcCombatNativeApplication;
    if(!a||reinterpret_cast<std::uintptr_t>(component)!=a->before.component)return true;
    auto* c=a->channel;NativeHealth current{};
    if(a->seen||!c||(!a->reconcile&&!npcCombatScope(GetTickCount64()))||!npcCombatIdentity(*c,current)||
       current.component!=a->before.component||current.actor!=a->before.actor||current.game!=a->before.game||
       current.current!=a->before.current||!std::isfinite(amount)||amount<0||amount>current.current){a->failed=true;return false;}
    a->seen=true;++npcCombatSubtractions;a->terminal=npcCombatNativeLethal(current.current,amount);
    if(a->reconcile&&!a->terminal){a->failed=true;return false;}
    if(a->terminal){
        npcCombatNativeStage="native_lethal_subtraction_ownership";
        if(!npcCombatRetireBeforeDeath(*c)){a->failed=true;return false;}
        ++npcCombatLethalSubtractions;
    }
    return true;
}
bool rangedPropDamageSource(unsigned kind,unsigned& handle);
bool npcCombatApply(NpcAuthorityChannel& c,const NativeHealth& before,unsigned sourceRole,NpcCombatDamage damage,const EnemyMeleeGeometry& g,NativeHealth& after,float& inflicted,std::uint64_t sourceArm=0,const NpcCombatScriptDamage& script={},const NpcCombatRangedDamage& ranged={}){
    npcCombatNativeStage="native_preconditions";
    const bool scripted=damage==NpcCombatDamage::MapTrigger,isRanged=npcCombatRanged(damage);NativeDamageTrigger trigger{};
    if(!swordDamageApiMatches()||!npcCombatGeometry(damage,g)||(before.ownerFlags&8)||
       (scripted?!npcCombatTrigger(script.trigger,0,trigger):isRanged?!npcCombatNear(c,sourceRole,32):!npcCombatSourceNear(c,sourceRole,sourceArm)))return false;
    const bool world=npcCombatWorldDamage(damage);if(world!=(sourceRole==2))return false;
    const auto avatar=shadowDamageIdentity();unsigned attacker=scripted?trigger.handle:world?0x3fff0010:sourceRole==localRole?1:avatar.handle;
    if(!world&&attacker!=1&&!validCoopDamageAvatar(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),avatar))return false;
    const auto decl=reinterpret_cast<std::uintptr_t(*)(void*,const char*,bool,bool)>(gameImage+0x1e030)(gameImage+0x3990b60,scripted?npcDamageDeclarationName(script.declaration):isRanged?npcDamageDeclarationName(ranged.declaration):npcCombatDeclName(damage),false,false);
    std::uint64_t declaredFlags=0;unsigned declaredReactions[2]{};
    if(scripted?!npcCombatScriptDeclaration(decl,script,declaredFlags,declaredReactions):isRanged?!npcCombatRangedDeclaration(decl,damage,ranged):!npcCombatDeclaration(decl,damage))return false;
    alignas(16) std::array<unsigned char,144> params{};alignas(16) std::array<unsigned char,32> result{};float hit[3]{};
    if(!memoryRead(before.actor+0x300,hit,12))return false;for(unsigned i=0;i<3;++i)hit[i]+=g.offset[i];
    reinterpret_cast<void(*)(void*,void*)>(gameImage+0x780260)(params.data(),reinterpret_cast<void*>(decl));
    reinterpret_cast<void(*)(void*)>(gameImage+0x780470)(result.data());
    if(damage!=NpcCombatDamage::Assassination&&(!scripted||!emptyEnemyMeleeGeometry(g))){std::memcpy(params.data()+0x20,&g.regions,4);std::memcpy(params.data()+0x24,hit,12);std::memcpy(params.data()+0x30,g.normal,12);
    std::memcpy(params.data()+0x3c,g.direction,12);std::memcpy(params.data()+0x4c,&g.region,4);}
    std::memcpy(params.data()+0x54,&attacker,4);
    if(isRanged){
        std::memcpy(params.data()+8,&ranged.amount,4);std::memcpy(params.data()+0x48,&ranged.distance,4);
        // The transient projectile is not replayed. The genuine gun source of
        // spread remains attributable; bullet damage retains its player source
        // and validated hit descriptor with the native absent-inflictor default.
        if(damage==NpcCombatDamage::RangedSpread){unsigned handle=0;
            if(sourceRole==localRole){NativePlayerRangedItem gun{};if(!readNativePlayerRangedItem(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),1,gun,true)||!gun.equipped||!gun.started)return false;handle=gun.handle;}
            else rangedPropDamageSource(1,handle);
            // A cosmetic attachment can lag the reliable contact or already
            // be hidden by a transition. It is optional impact context, not
            // ownership evidence: source ownership was proven at capture.
            // Missing presentation keeps the constructor's absent inflictor.
            if(handle)std::memcpy(params.data()+0x50,&handle,4);
        }
    }else if(scripted){std::memcpy(params.data()+8,&script.amount,4);}
    else if(world){NativeHealth source{};bool pending=false;
        if(!npcCombatCorpseSource(sourceArm,false,source,pending)||(pending&&!localRole))return false;
        std::memcpy(params.data()+0x50,&source.handle,4);
    }else if(damage==NpcCombatDamage::Kick){std::memcpy(params.data()+0x50,&attacker,4);
    }else if(sourceRole==localRole){NativePlayerSword sword{};
        if(!readNativePlayerSword(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),sword)||!sword.sword.equipped||!sword.sword.started)return false;
        std::memcpy(params.data()+0x50,&sword.sword.weaponHandle,4);
    }
    NativeHealth recheck{};
    if(!readNativeHealth(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),before.handle,recheck)||recheck.actor!=before.actor||
       recheck.component!=before.component||recheck.game!=before.game||recheck.current!=before.current)return false;
    // Evaluate exactly once inside the original native entry. Its F8 health
    // subtraction yields the final modified amount before health/death writes.
    if(npcCombatNativeApplication)return false;
    NpcCombatNativeApplication application{&c,before};npcCombatNativeApplication=&application;
    realNativeDamage(reinterpret_cast<void*>(before.component),params.data(),result.data());npcCombatNativeApplication=nullptr;
    return !application.failed&&npcCombatReadResult(c,before,result.data(),after,inflicted,application.terminal,world)&&
        (application.seen||(world&&inflicted==0&&after.current==before.current));
}
unsigned npcCombatDeathShare(std::uint64_t arm){unsigned share=0;
    for(const auto& b:npcCombat.channels)if(b.arm==arm){if(b.terminal)return 2;if(b.deathPending)share=1;}return share;
}
// 1214n (#14): Dead from the host. The replica dies by the native silent kill
// declaration inside the native entry; its AI ownership is released at the lethal
// subtraction exactly as for a shared death, and the stream keeps presenting the
// host's corpse. The attacker is the world (0x3fff0010), never the client player.
constexpr char kNpcReconcileKillDecl[]="components/base/damage/kill_no_reaction_damage.damage";
void clientNpcDead(NpcAuthorityChannel& c){
    const auto refuse=[&](const char* why){++npcClientDeadRefused;npcClientDeadLast=why;c.deathShare=why;};
    if(c.corpse){++npcClientDeadAlready;return;}
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);NativeHealth before{},after{};
    if(!readNativeHealth(memoryRead,base,c.lease.binding.npc.handle,before)||before.actor!=c.lease.binding.npc.actor){refuse("client_dead_identity");return;}
    if(!(before.current>0)){++npcClientDeadAlready;return;}
    if(npcCombatDeathShare(c.arm)==1)return;
    if(!realNativeDamage||!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load()||npcCombatNativeApplication||(before.ownerFlags&8)){refuse("client_dead_context");return;}
    const auto decl=reinterpret_cast<std::uintptr_t(*)(void*,const char*,bool,bool)>(gameImage+0x1e030)(gameImage+0x3990b60,kNpcReconcileKillDecl,false,false);
    std::uintptr_t vt=0;if(!decl||!memoryRead(decl,&vt,8)||vt!=base+0x1c5d138){refuse("client_dead_declaration");return;}
    memoryRead(decl+0x5c,&npcReconcileDeclAmount,4);memoryRead(decl+0x60,&npcReconcileDeclFlags,8);memoryRead(decl+0x68,npcReconcileDeclReactions,8);
    alignas(16) std::array<unsigned char,144> params{};alignas(16) std::array<unsigned char,32> result{};const unsigned attacker=0x3fff0010u;
    reinterpret_cast<void(*)(void*,void*)>(gameImage+0x780260)(params.data(),reinterpret_cast<void*>(decl));
    reinterpret_cast<void(*)(void*)>(gameImage+0x780470)(result.data());std::memcpy(params.data()+0x54,&attacker,4);
    NpcCombatNativeApplication application{&c,before};application.reconcile=true;npcCombatNativeApplication=&application;
    realNativeDamage(reinterpret_cast<void*>(before.component),params.data(),result.data());npcCombatNativeApplication=nullptr;
    const bool read=readNativeHealth(memoryRead,base,before.handle,after)&&after.actor==before.actor;
    if(read&&after.current<=0&&!c.corpse){npcCombatRetireBeforeDeath(c);refuse("client_dead_unhooked");return;}
    if(application.failed||!application.seen||!application.terminal||!read||after.current>0){refuse("client_dead_not_lethal");return;}
    ++npcClientDeadApplied;npcClientDeadLast=c.deathShare="client_dead_from_host";
}
// 1214o: the replica follows a host native loss it cannot replay (host-world damage,
// or a host hit outside a bound channel) by the native per-component health
// subtraction (health vtable +0xf8, where every native damage ends). Exact result,
// never lethal, never a gain.
bool npcCombatFollowHost(NpcAuthorityChannel& c,const NativeHealth& before,float target,NativeHealth& after){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(!localRole||!realNativeHealthSubtract||!std::isfinite(target)||!(target>0)||!(target<before.current)||
       GetCurrentThreadId()!=engineThread.load()||npcCombatNativeApplication||before.actor!=c.lease.binding.npc.actor)return false;
    for(unsigned pass=0;pass<2;++pass){NativeHealth now{};
        if(!readNativeHealth(memoryRead,base,before.handle,now)||now.actor!=before.actor||now.component!=before.component||now.game!=before.game||(!pass&&now.current!=before.current))return false;
        if(now.current==target){after=now;return true;}
        if(!(now.current>target)||(pass&&!npcCombatClose(now.current,target)))return false;
        realNativeHealthSubtract(reinterpret_cast<void*>(before.component),now.current-target);++npcCombatFollowSubtractions;
    }
    return readNativeHealth(memoryRead,base,before.handle,after)&&after.actor==before.actor&&after.current==target;
}
void npcCombatCommit(NpcAuthorityChannel& c,NpcCombatChannel& b,unsigned source,std::uint64_t request,NpcCombatDamage damage,const EnemyMeleeGeometry& geometry,const NativeHealth& before,const NativeHealth& after,float inflicted,std::uint64_t sourceArm=0,const NpcCombatScriptDamage& script={},const NpcCombatRangedDamage& ranged={}){
    auto p=npcCombatPacket(c,b,NpcCombatMessage::Commit);p.sourceRole=source;p.request=request;p.event=++b.event;p.damage=damage;p.geometry=geometry;
    p.before=before.current;p.after=after.current;p.maximum=before.maximum;p.inflicted=inflicted;
    p.sourceActorArm=sourceArm;p.script=script;p.ranged=ranged;
    if(npcCombat.outgoing.size()>=64||!validNpcCombat(p,remote,outgoing)){stopNpcCombat("commit_queue_or_validation");return;}
    npcCombat.outgoing.push_back(p);++npcCombat.committed;b.health=after.current;if(after.current==0){b.terminal=true;b.pending=0;b.pendingRequests.clear();++npcCombat.deaths;}
    if(source==2)++npcCombat.worldCommitted;
}
void npcCombatDeferDeath(NpcCombatChannel& b,unsigned source,std::uint64_t request,std::uint64_t event,NpcCombatDamage damage,
        const EnemyMeleeGeometry& geometry,const NativeHealth& before,const NativeHealth& after,float inflicted,std::uint64_t sourceArm=0,const NpcCombatScriptDamage& script={},const NpcCombatRangedDamage& ranged={}){
    b.deathPending=true;b.deathAt=GetTickCount64();b.deathSource=source;b.deathRequest=request;b.deathEvent=event;
    b.deathDamage=damage;b.deathGeometry=geometry;b.deathBefore=before;b.deathAfter=after;b.deathInflicted=inflicted;
    b.deathSourceArm=sourceArm;b.deathScript=script;b.deathRanged=ranged;
}
void serviceNpcCombatDeaths(ULONGLONG tick){
    for(auto& b:npcCombat.channels)if(b.enabled&&b.deathPending){
        auto* c=npcCombatActor(b.arm);if(!c){stopNpcCombat("death_binding_retired");return;}
        if(!npcCombatDeadProof(*c,b.deathAfter)){
            if(tick-b.deathAt>1500){stopNpcCombat("native_death_completion_timeout");return;}continue;
        }
        b.deathPending=false;npcCombatNativeStage="native_death_complete";
        if(!localRole){
            if(b.deathSource==1){b.peerRequest=b.deathRequest;++npcCombat.requests;}
            npcCombatCommit(*c,b,b.deathSource,b.deathRequest,b.deathDamage,b.deathGeometry,b.deathBefore,b.deathAfter,b.deathInflicted,b.deathSourceArm,b.deathScript,b.deathRanged);
            if(!npcCombat.enabled)return;
        }else{b.event=b.deathEvent;b.terminal=true;b.pending=0;b.pendingRequests.clear();++npcCombat.applied;++npcCombat.deaths;if(b.deathSource==2)++npcCombat.worldApplied;}
    }
}
// 1214o: host-world damage the capture passes natively (take 1270e: bloodflies on a
// bound checkpoint guard were refused, the channel retired, and the host's next
// sword hits and the guard's death went unshared). The host commits its observed
// native loss in event order; a death stays the native combat outcome's or Dead's.
void serviceNpcCombatHostWorld(){
    for(auto& b:npcCombat.channels){if(!b.enabled||!b.ready||b.terminal||b.deathPending)continue;auto* c=npcCombatActor(b.arm);NativeHealth h{};
        if(!c||!npcCombatIdentity(*c,h))continue;
        if(!(b.health>0)){b.health=h.current;continue;}
        if(h.current>b.health){++npcCombat.hostWorldGains;b.health=h.current;continue;}
        if(h.current==b.health)continue;
        NativeHealth before=h;before.current=b.health;
        npcCombatCommit(*c,b,2,0,NpcCombatDamage::HostWorld,{},before,h,b.health-h.current);if(!npcCombat.enabled)return;++npcCombat.hostWorldCommitted;
    }
}
bool processNpcCombatCapture(NpcCombatCapture& capture){
    if(!npcCombat.enabled||capture.nonce!=npcCombat.nonce||GetCurrentThreadId()!=engineThread.load())return false;
    std::uint64_t capturedFlags=0;std::memcpy(&capturedFlags,capture.info.data()+0x10,8);
    if(GetTickCount64()-capture.tick>(capturedFlags==0x12?1750ull:250ull)){stopNpcCombat("captured_hit_expired");return false;}
    void* component=reinterpret_cast<void*>(capture.component);void* info=capture.info.data();alignas(16) std::array<unsigned char,32> resultBytes{};void* result=resultBytes.data();
    reinterpret_cast<void(*)(void*)>(gameImage+0x780470)(result);npcCombatLastCaptureThread=capture.thread;
    for(auto& b:npcCombat.channels){if(!b.enabled||b.terminal||b.deathPending||b.arm!=capture.arm)continue;auto* c=npcCombatActor(b.arm);if(!c||c->lease.binding.health.component!=reinterpret_cast<std::uintptr_t>(component))continue;
        const auto empty=[&]{reinterpret_cast<void(*)(void*)>(gameImage+0x780470)(result);++npcCombat.suppressed;};
        NativeHealth before{};EnemyMeleeGeometry geometry{};NpcCombatDamage damage=NpcCombatDamage::None;
        std::uint64_t sourceArm=0;bool sourcePending=false;NpcCombatScriptDamage script{};NpcCombatRangedDamage ranged{};
        memoryRead(reinterpret_cast<std::uintptr_t>(info),npcCombatLastInfo.data(),npcCombatLastInfo.size());
        std::uintptr_t decl=0,name=0;std::memcpy(&decl,npcCombatLastInfo.data(),8);npcCombatLastDecl.clear();
        if(memoryRead(decl+8,&name,8))worldIdentityString(memoryRead,name,npcCombatLastDecl);
        unsigned inflictor=0,attacker=0;std::memcpy(&inflictor,capture.info.data()+0x50,4);std::memcpy(&attacker,capture.info.data()+0x54,4);
        const auto player=worldIdentityEntity(memoryRead,observedGame,1);
        const bool playerOwned=player&&((capture.attacker.valid&&capture.attacker.handle==attacker&&capture.attacker.localOwned)||
            (capture.inflictor.valid&&capture.inflictor.handle==inflictor&&capture.inflictor.localOwned));
        const char* refusal=nullptr;
        if(!npcCombatScope(GetTickCount64()))refusal="hit_scope";
        else if(!b.ready)refusal="hit_not_ready";
        else if(!npcCombatIdentity(*c,before))refusal="hit_native_identity";
        else if(before.ownerFlags&8)refusal="hit_native_deferred";
        else if(!player)refusal="hit_local_player_identity";
        else if(npcCombatReplicaWorldEvent(localRole,playerOwned)){
            // The host owns this victim's world simulation. Client-side
            // hazards, NPC/corpse contacts and scripts cannot become player
            // requests or independently alter its health. Indirect local
            // attacks still require explicit native owner attribution.
            empty();++npcCombat.replicaWorldSuppressed;return true;
        }
        else if(!npcCombatExtract(*c,info,damage,geometry,capture.position,sourceArm,sourcePending,script,ranged,capture))refusal="hit_damage_parameters";
        npcCombatLastOwnerFlags=before.ownerFlags;
        if(refusal){empty();++npcCombat.rejected;retireNpcCombatChannel(b,refusal);return true;}
        const bool world=npcCombatWorldDamage(damage);
        if(world&&localRole){empty();++npcCombat.replicaWorldSuppressed;return true;}
        if(world&&sourcePending){
            // Send the source death before a collision attributed to its corpse.
            AcquireSRWLockExclusive(&npcCombatCaptureLock);
            const bool room=npcCombatCaptures.size()<64;if(room)npcCombatCaptures.push_back(capture);
            ReleaseSRWLockExclusive(&npcCombatCaptureLock);
            if(!room)stopNpcCombat("world_source_wait_capacity");return true;
        }
        if(damage!=NpcCombatDamage::MapTrigger&&(npcCombatRanged(damage)?!npcCombatNear(*c,localRole,32):!npcCombatSourceNear(*c,world?2:localRole,sourceArm))){empty();++npcCombat.rejected;stopNpcCombat("hit_range");return true;}
        if(localRole){
            empty();
            auto p=npcCombatPacket(*c,b,NpcCombatMessage::Request);p.sourceRole=1;p.request=++b.request;p.damage=damage;p.geometry=geometry;p.ranged=ranged;
            if(!b.pendingRequests.push(p.request)){stopNpcCombat("pending_request_capacity");return true;}
            b.pending=b.pendingRequests.front();npcCombat.pendingHighWater=(std::max)(npcCombat.pendingHighWater,b.pendingRequests.count);
            if(npcCombat.outgoing.size()>=64){stopNpcCombat("request_queue_full");return true;}
            npcCombat.outgoing.push_back(p);++npcCombat.requests;return true;
        }
        NativeHealth after{};float inflicted=0;if(!world)++npcCombat.localHits;
        // Reconstruct locally: copied native input may contain temporary
        // pointer fields whose caller has returned on the worker thread.
        if(!npcCombatApply(*c,before,world?2:0,damage,geometry,after,inflicted,sourceArm,script,ranged)){stopNpcCombat("native_host_result_not_supported");return true;}
        if(after.current==0)npcCombatDeferDeath(b,world?2:0,0,0,damage,geometry,before,after,inflicted,sourceArm,script,ranged);
        else npcCombatCommit(*c,b,world?2:0,0,damage,geometry,before,after,inflicted,sourceArm,script,ranged);return true;
    }return false;
}
void beginNpcCombat(){
    if(npcCombat.enabled)return;
    if(!npcStreamContext(GetTickCount64())||!swordDamageApiMatches())return;
    npcCombat={};npcCombat.enabled=true;npcCombat.nonce=++npcCombatNonce;npcCombat.started=GetTickCount64();npcCombat.scope=doorSessionScope(outgoing,remote);npcCombat.result="negotiating";
    npcCombatNativeStage="idle";npcCombatNativeInflicted=npcCombatNativeAfter=0;npcCombatNativeGeneration=0;
    npcCombatSubtractions=npcCombatLethalSubtractions=0;
    npcCombatLastInfo={};npcCombatLastOwnerFlags=0;npcCombatLastDecl.clear();
    clearNpcCombatCapture();npcCombatCaptureFault.store(0);
    for(auto& c:npcChannels){NativeHealth h{};if(c.enabled&&c.ready&&c.peerArm&&!c.corpse&&!c.unconscious&&npcCombatIdentity(c,h)){NpcCombatChannel b{};b.arm=c.arm;b.peerArm=c.peerArm;b.started=npcCombat.started;npcCombat.channels.push_back(b);}}
    if(!readNativeDamageTriggers(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,npcCombat.triggers)){stopNpcCombat("trigger_catalog_unavailable");return;}
    if(npcCombat.channels.empty())npcCombat.result="waiting_for_actors";
}
void retireNpcCombatChannel(NpcCombatChannel& b,const char* reason){
    if(!b.enabled)return;b.enabled=b.ready=false;b.retired=reason;b.retiredAt=GetTickCount64();npcCombatLastRetire=reason;++npcCombatChannelsRetired;
}
inline bool npcCombatChannelBlocks(const NpcCombatChannel& b,ULONGLONG tick){
    return npcCombatChannelBlocksRebind(b.enabled,b.terminal,b.deathPending,b.retiredAt,tick);
}
void syncNpcCombatChannels(ULONGLONG tick){
    for(auto& b:npcCombat.channels){if(!b.enabled)continue;auto* c=npcCombatActor(b.arm);if(!c||!c->enabled||c->peerArm!=b.peerArm)retireNpcCombatChannel(b,"actor_unbound");}
    for(auto& c:npcChannels){if(!c.enabled||!c.ready||!c.peerArm||c.corpse||c.unconscious)continue;
        if(std::any_of(npcCombat.channels.begin(),npcCombat.channels.end(),[&](const auto& b){return b.arm==c.arm&&b.peerArm==c.peerArm&&npcCombatChannelBlocks(b,tick);}))continue;
        NativeHealth h{};if(!npcCombatIdentity(c,h))continue;
        NpcCombatChannel b{};b.arm=c.arm;b.peerArm=c.peerArm;b.started=tick;npcCombat.channels.push_back(b);++npcCombatChannelsAdded;}
    if(npcCombat.channels.size()>96)npcCombat.channels.erase(std::remove_if(npcCombat.channels.begin(),npcCombat.channels.end(),[](const auto& b){return !b.enabled&&!b.terminal&&!b.deathPending;}),npcCombat.channels.end());
}
void serviceNpcCombatFixture(ULONGLONG tick){
    auto& f=npcCombatFixture;
    if(npcCombatFixtureRequest>=0){const auto request=npcCombatFixtureRequest;npcCombatFixtureRequest=-1;
        if(!request){stopNpcCombatFixture("disabled");return;}
        if(f.until)return;
        if(localRole||!npcCombatScope(tick)||!nativeMutationAllowed){f.result="fixture_scope_refused";return;}
        f={};f.started=tick;f.until=tick+40000;f.result="pausing";f.holdFrames=request==1;
        for(const auto& c:npcChannels)if(c.enabled&&c.ready){NpcAiLease lease{};lease.binding=c.lease.binding;lease.epoch=outgoing.worldEpoch;f.leases.push_back(lease);}
        for(auto& lease:f.leases){const bool ok=acquireNpcAiLease(lease);f.acquired+=lease.acquired;if(!ok){stopNpcCombatFixture("fixture_acquire_failed");return;}}
    }
    if(!f.until)return;
    if(tick>=f.until||!npcCombatScope(tick)){stopNpcCombatFixture("fixture_expired_or_scope_changed");return;}
    if(f.holdFrames){NativePlayerStates states{};
        if(readNativePlayerStates(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),states)&&
           states.player.game==observedGame&&nativePlayerGroundAssassinationStates(states,reinterpret_cast<std::uintptr_t>(gameImage))){
            // Test-only positioning ownership ends before native paired root
            // motion, rather than fighting the finisher's own frame updates.
            f.holdFrames=false;++f.pairedFrameReleases;
            AcquireSRWLockExclusive(&npcPresentationLock);npcCombatFixtureFrames.clear();ReleaseSRWLockExclusive(&npcPresentationLock);
        }
    }
    bool ready=true;for(const auto& lease:f.leases){NativeNpcAuthority n{};
        if(!readNativeNpcAuthority(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,lease.binding.npc.handle,n)||!sameNpcAuthorityIdentity(lease.binding,n)||!npcAuthorityLiving(n)){stopNpcCombatFixture("fixture_actor_changed");return;}
        ready=ready&&npcAuthorityPaused(n);
    }
    if(!ready&&(f.ready||tick-f.started>1500)){stopNpcCombatFixture("fixture_ownership_changed");return;}
    f.ready=ready;if(ready){f.result="held_for_damage_test";
        std::vector<NpcRenderIdentity> frames;if(f.holdFrames)for(const auto& c:npcChannels)for(const auto& lease:f.leases)if(c.arm&&c.render.native.npc.handle==lease.binding.npc.handle)frames.push_back(c.render);
        AcquireSRWLockExclusive(&npcPresentationLock);npcCombatFixtureFrames=std::move(frames);ReleaseSRWLockExclusive(&npcPresentationLock);
    }
}
void receiveNpcCombat(const CoopNpcCombat& p,ULONGLONG tick){
    if(!npcCombatScope(tick)){rejectNpcCombat(p,"receive_scope");return;}
    if(!validNpcCombat(p,outgoing,remote)){rejectNpcCombat(p,"receive_invalid_packet");return;}
    ++npcCombat.received[static_cast<unsigned>(p.kind)];
    const auto findBinding=[&](){return std::find_if(npcCombat.channels.begin(),npcCombat.channels.end(),[&](const auto& b){return b.enabled&&b.arm==p.binding.targetArm&&b.peerArm==p.binding.sourceArm;});};
    auto it=findBinding();if(it==npcCombat.channels.end()){syncNpcCombatChannels(tick);it=findBinding();}
    if(it==npcCombat.channels.end()){rejectNpcCombat(p,"receive_binding");return;}auto& b=*it;auto* c=npcCombatActor(b.arm);
    if(!c||p.binding.sourceHandle!=c->peerHandle||p.binding.targetHandle!=c->lease.binding.npc.handle||p.binding.rig!=c->rig||p.binding.joints!=c->render.joints){rejectNpcCombat(p,"receive_identity",b.peerNonce);return;}
    if(p.kind==NpcCombatMessage::Hello){
        if(b.terminal||b.deathPending){rejectNpcCombat(p,"receive_terminal_hello",b.peerNonce);return;}
        traceNpcCombat(p,false,"hello",b.peerNonce);
        if(b.ready&&b.peerNonce==p.sourceNonce){sendNpcCombat(npcCombatPacket(*c,b,NpcCombatMessage::Ready));return;}
        NativeHealth h{};if((b.peerNonce&&b.peerNonce!=p.sourceNonce)||!npcCombatIdentity(*c,h)||h.maximum!=p.maximum){retireNpcCombatChannel(b,"initial_health_or_nonce_mismatch");return;}
        // 1214o: a host loss outside a bound channel no longer retires the NPC for good.
        switch(npcCombatHelloHealth(localRole,h.current,p.before)){
        case 0:break;
        case 1:{NativeHealth after{};if(!npcCombatFollowHost(*c,h,p.before,after)){retireNpcCombatChannel(b,"initial_health_follow_failed");return;}++npcCombat.helloFollowed;h=after;break;}
        case 2:++npcCombat.helloWaited;return;
        default:retireNpcCombatChannel(b,"initial_health_or_nonce_mismatch");return;
        }
        if(!localRole)b.health=h.current;
        b.peerNonce=p.sourceNonce;sendNpcCombat(npcCombatPacket(*c,b,NpcCombatMessage::Ready));return;
    }
    if(p.targetNonce!=npcCombat.nonce||!b.peerNonce||p.sourceNonce!=b.peerNonce){rejectNpcCombat(p,"receive_nonce",b.peerNonce);return;}
    traceNpcCombat(p,false,"accepted",b.peerNonce);
    if(p.kind==NpcCombatMessage::Ready){b.ready=true;return;}
    if(p.kind==NpcCombatMessage::Stop){b.enabled=b.ready=false;return;}
    if(!b.ready){rejectNpcCombat(p,"receive_not_ready",b.peerNonce);return;}
    if(b.deathPending){
        if((p.kind==NpcCombatMessage::Request&&p.request==b.deathRequest)||(p.kind==NpcCombatMessage::Commit&&p.event==b.deathEvent))++npcCombat.duplicates;
        else ++npcCombat.rejected;return;
    }
    const auto order=npcCombatOrder(p.kind==NpcCombatMessage::Request?b.peerRequest:b.event,p.kind==NpcCombatMessage::Request?p.request:p.event);
    if(order==NpcCombatOrder::Duplicate){++npcCombat.duplicates;return;}
    if(b.terminal||b.deathPending){++npcCombat.rejected;return;}
    if(order!=NpcCombatOrder::Next){retireNpcCombatChannel(b,"event_gap");return;}
    NativeHealth before{},after{};float inflicted=0;
    std::uint64_t sourceArm=0;
    if(p.damage==NpcCombatDamage::Corpse){NativeHealth source{};bool pending=false;
        auto* actor=npcCombatCorpseSource(p.sourceActorArm,true,source,pending);
        if(!actor){retireNpcCombatChannel(b,"world_source_identity");return;}sourceArm=actor->arm;
    }
    if(!npcCombatIdentity(*c,before)){retireNpcCombatChannel(b,"actor_lifecycle_changed");return;}
    if(p.kind==NpcCombatMessage::Commit&&(before.current!=p.before||before.maximum!=p.maximum||
       (p.sourceRole==1&&b.pending!=p.request))){retireNpcCombatChannel(b,"replica_or_request_diverged");return;}
    if(p.damage==NpcCombatDamage::HostWorld){
        if(p.kind!=NpcCombatMessage::Commit||!npcCombatFollowHost(*c,before,p.after,after)){retireNpcCombatChannel(b,"host_world_follow_failed");return;}
        inflicted=p.inflicted;++npcCombat.hostWorldApplied;
    }else if(!npcCombatApply(*c,before,p.sourceRole,p.damage,p.geometry,after,inflicted,sourceArm,p.script,p.ranged)){retireNpcCombatChannel(b,"native_replay_failed");return;}
    if(p.kind==NpcCombatMessage::Commit&&(after.current!=p.after||inflicted!=p.inflicted)){retireNpcCombatChannel(b,"native_replica_result_diverged");return;}
    if(after.current==0){npcCombatDeferDeath(b,p.sourceRole,p.request,p.event,p.damage,p.geometry,before,after,inflicted,sourceArm,p.script,p.ranged);return;}
    if(p.kind==NpcCombatMessage::Request){b.peerRequest=p.request;++npcCombat.requests;npcCombatCommit(*c,b,1,p.request,p.damage,p.geometry,before,after,inflicted,0,{},p.ranged);}
    else{b.event=p.event;++npcCombat.applied;if(p.sourceRole==2)++npcCombat.worldApplied;if(p.sourceRole==1){
            if(!b.pendingRequests.acknowledge(p.request)){retireNpcCombatChannel(b,"pending_request_order");return;}
            b.pending=b.pendingRequests.front();
        }
        if(after.current==0){b.terminal=true;b.pending=0;b.pendingRequests.clear();++npcCombat.deaths;}
        if(after.current!=p.after||inflicted!=p.inflicted)retireNpcCombatChannel(b,"native_replica_result_diverged");}
}
void serviceNpcCombatTriggerFixture(){
    if(!npcCombatTriggerFixtureRequested)return;npcCombatTriggerFixtureRequested=false;
    npcCombatTriggerFixtureResult="refused";
    // Local, explicit, once-per-combat-session reconstruction of the observed
    // native trigger callback. This tests the damage pipeline, not trigger
    // activation or natural campaign behavior. Requires the finite lab lease.
    if(localRole||!nativeMutationAllowed||!npcCombatScope(GetTickCount64())||!npcCombatFixture.ready||
       !npcCombatFixture.until||npcCombatTriggerFixtureNonce==npcCombat.nonce)return;
    NativeDamageTrigger trigger{};if(!npcCombatTrigger(npcDamageKey("esc_env_trigger_4"),0,trigger))return;
    NpcAuthorityChannel* target=nullptr;
    for(auto& c:npcChannels)if(c.enabled&&c.ready&&c.lease.binding.npc.spawnerName=="ld_spawner_guard_escape_1"){
        if(target)return;target=&c;
    }
    NativeHealth health{};if(!target||!npcCombatIdentity(*target,health))return;
    bool captured=false;AcquireSRWLockShared(&npcCombatCaptureLock);
    for(const auto& b:npcCombatCaptureBindings)if(b.nonce==npcCombat.nonce&&b.arm==target->arm&&b.native.health.component==health.component)captured=true;
    ReleaseSRWLockShared(&npcCombatCaptureLock);if(!captured)return;
    NpcCombatScriptDamage script{trigger.key,npcDamageKey("components/equipment/damage/bolt_incendiary.damage"),1,0};
    const auto decl=reinterpret_cast<std::uintptr_t(*)(void*,const char*,bool,bool)>(gameImage+0x1e030)(gameImage+0x3990b60,npcDamageDeclarationName(script.declaration),false,false);
    std::uint64_t flags=0;unsigned reactions[2]{};if(!npcCombatScriptDeclaration(decl,script,flags,reactions)||flags!=0x400020||reactions[0]||reactions[1])return;
    alignas(16) std::array<unsigned char,144> params{};alignas(16) std::array<unsigned char,32> result{};
    reinterpret_cast<void(*)(void*,void*)>(gameImage+0x780260)(params.data(),reinterpret_cast<void*>(decl));
    reinterpret_cast<void(*)(void*)>(gameImage+0x780470)(result.data());
    std::memcpy(params.data()+8,&script.amount,4);std::memcpy(params.data()+0x54,&trigger.handle,4);
    npcCombatTriggerFixtureNonce=npcCombat.nonce;const auto count=npcCombatCaptured.load();
    reinterpret_cast<NativeDamage>(gameImage+0x717950)(reinterpret_cast<void*>(health.component),params.data(),result.data());
    npcCombatTriggerFixtureResult=npcCombatCaptured.load()==count+1?"captured_observed_event_fixture":"capture_unconfirmed";
}
void serviceNpcCombat(ULONGLONG tick){
    if(npcCombatRequest>=0){const auto request=npcCombatRequest;npcCombatRequest=-1;npcCombatWanted=request!=0;if(request)beginNpcCombat();else stopNpcCombat("disabled");}
    serviceNpcCombatFixture(tick);
    if(!npcCombat.enabled&&npcCombatWanted&&(productMode||productAutoArm)&&npcAuthorityStreamActive&&tick-npcCombatRearmAt>=3000){npcCombatRearmAt=tick;beginNpcCombat();if(npcCombat.enabled)++npcCombatRearms;}
    if(!npcCombat.enabled)return;
    syncNpcCombatChannels(tick);
    if(!npcCombatScope(tick)){stopNpcCombat("session_or_world_changed");return;}
    serviceNpcCombatTriggerFixture();
    if(npcCombatCaptureFault.exchange(0)){stopNpcCombat("native_capture_queue_failed");return;}
    std::vector<NpcCombatCapture> captured;
    AcquireSRWLockExclusive(&npcCombatCaptureLock);captured.swap(npcCombatCaptures);ReleaseSRWLockExclusive(&npcCombatCaptureLock);
    for(auto& e:captured){processNpcCombatCapture(e);if(!npcCombat.enabled)return;}
    serviceNpcCombatDeaths(GetTickCount64());if(!npcCombat.enabled)return;
    if(!localRole){serviceNpcCombatHostWorld();if(!npcCombat.enabled)return;}
    // Drain commits before any new handshake health comparison.
    auto queue=std::move(npcCombat.outgoing);npcCombat.outgoing.clear();for(const auto& p:queue)if(!sendNpcCombat(p)){stopNpcCombat("transport_failed");return;}
    if(tick-npcCombat.helloAt>=500){npcCombat.helloAt=tick;
        for(auto& b:npcCombat.channels){if(!b.enabled||b.terminal||b.deathPending)continue;auto* c=npcCombatActor(b.arm);NativeHealth h{};
            if(!c||!npcCombatIdentity(*c,h)){retireNpcCombatChannel(b,"bound_actor_retired_without_combat_outcome");continue;}
            if(!b.ready){auto p=npcCombatPacket(*c,b,NpcCombatMessage::Hello);p.before=h.current;p.maximum=h.maximum;sendNpcCombat(p);}
        }
    }
    for(auto& b:npcCombat.channels)if(b.enabled&&!b.ready&&tick>=b.started&&tick-b.started>5000)retireNpcCombatChannel(b,"negotiation_timeout");
    const bool any=std::any_of(npcCombat.channels.begin(),npcCombat.channels.end(),[](const auto& b){return b.enabled;});
    const bool ready=std::all_of(npcCombat.channels.begin(),npcCombat.channels.end(),[](const auto& b){return !b.enabled||b.ready;});
    npcCombat.result=!any?"waiting_for_actors":ready?"active_native_melee":"negotiating";
    std::vector<NpcCombatCaptureBinding> bindings;
    if(npcCombat.enabled)for(const auto& b:npcCombat.channels)if(b.enabled&&b.ready&&!b.terminal&&!b.deathPending){auto* c=npcCombatActor(b.arm);if(c&&c->enabled)bindings.push_back({c->lease.binding,npcCombat.nonce,b.arm});}
    AcquireSRWLockExclusive(&npcCombatCaptureLock);npcCombatCaptureBindings=std::move(bindings);ReleaseSRWLockExclusive(&npcCombatCaptureLock);
}
void repeatNpcCombat(){if(npcCombat.enabled&&npcCombat.lastSent.sourceNonce==npcCombat.nonce&&npcCombat.lastSent.event+npcCombat.lastSent.request)sendNpcCombat(npcCombat.lastSent);}
void writeNpcCombat(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.npc-combat.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;
    const auto& s=npcCombat;fprintf(f,"{\"passedThrough\":%llu,\"lastDecl\":\"%s\",\"channelsAdded\":%llu,\"channelsRetired\":%llu,\"rearms\":%llu,\"lastRetire\":\"%s\",\"wanted\":%s,",static_cast<unsigned long long>(npcCombatPassedThrough.load()),npcCombatLastDecl.c_str(),static_cast<unsigned long long>(npcCombatChannelsAdded),static_cast<unsigned long long>(npcCombatChannelsRetired),static_cast<unsigned long long>(npcCombatRearms),npcCombatLastRetire,npcCombatWanted?"true":"false");fprintf(f,"\"enabled\":%s,\"result\":\"%s\",\"nonce\":%llu,\"localHits\":%llu,\"requests\":%llu,\"committed\":%llu,\"applied\":%llu,\"duplicates\":%llu,\"rejected\":%llu,\"suppressed\":%llu,\"eventsQueued\":%zu,\"lastOutcome\":{\"before\":%.9g,\"after\":%.9g,\"maximum\":%.9g,\"inflicted\":%.9g},\"entries\":[",s.enabled?"true":"false",s.result,s.nonce,s.localHits,s.requests,s.committed,s.applied,s.duplicates,s.rejected,s.suppressed,s.outgoing.size(),s.lastSent.before,s.lastSent.after,s.lastSent.maximum,s.lastSent.inflicted);
    for(unsigned i=0;i<s.channels.size();++i){const auto& b=s.channels[i];fprintf(f,"%s{\"arm\":%llu,\"peerArm\":%llu,\"peerNonce\":%llu,\"enabled\":%s,\"ready\":%s,\"event\":%llu,\"request\":%llu,\"peerRequest\":%llu,\"pending\":%llu,\"terminal\":%s,\"deathPending\":%s,\"retired\":\"%s\",\"health\":%.4f}",i?",":"",b.arm,b.peerArm,b.peerNonce,b.enabled?"true":"false",b.ready?"true":"false",b.event,b.request,b.peerRequest,b.pending,b.terminal?"true":"false",b.deathPending?"true":"false",b.retired,b.health);}
    fprintf(f,"],\"deaths\":%llu,\"preDeathReleased\":%llu",s.deaths,s.preDeathReleased);
    fprintf(f,",\"handshake\":{\"sendFailures\":%llu,\"traceCount\":%llu,\"sent\":[",s.sendFailures,s.traceCount);
    for(unsigned i=0;i<s.sent.size();++i)fprintf(f,"%s%llu",i?",":"",s.sent[i]);
    fputs("],\"received\":[",f);for(unsigned i=0;i<s.received.size();++i)fprintf(f,"%s%llu",i?",":"",s.received[i]);
    fputs("],\"trace\":[",f);const auto traceBegin=s.traceCount>s.trace.size()?s.traceCount-s.trace.size():0;
    for(auto i=traceBegin;i<s.traceCount;++i){const auto& t=s.trace[i%s.trace.size()];
        fprintf(f,"%s{\"tick\":%llu,\"sent\":%s,\"result\":\"%s\",\"kind\":%u,\"sourceNonce\":%llu,\"targetNonce\":%llu,\"knownPeerNonce\":%llu,\"sourceArm\":%llu,\"targetArm\":%llu,\"sourceEpoch\":%llu,\"targetEpoch\":%llu,\"localEpoch\":%llu,\"peerEpoch\":%llu,\"worldReady\":%s}",
            i==traceBegin?"":",",t.tick,t.sent?"true":"false",t.result,t.kind,t.sourceNonce,t.targetNonce,t.peerNonce,t.binding.sourceArm,t.binding.targetArm,t.binding.sourceEpoch,t.binding.targetEpoch,t.localEpoch,t.peerEpoch,t.worldReady?"true":"false");
    }fputs("]}",f);
    fprintf(f,",\"captured\":%llu,\"workerCaptured\":%llu,\"lastCaptureThread\":%lu,\"engineThread\":%lu,\"nativeAttempt\":{\"ownerFlags\":%u,\"declaration\":\"%s\",\"rawInfo\":\"",npcCombatCaptured.load(),npcCombatWorkerCaptured.load(),npcCombatLastCaptureThread,engineThread.load(),npcCombatLastOwnerFlags,npcCombatLastDecl.c_str());
    for(auto byte:npcCombatLastInfo)fprintf(f,"%02x",byte);fputs("\"}",f);
    fprintf(f,",\"nativeOutcome\":{\"stage\":\"%s\",\"inflicted\":%.9g,\"after\":%.9g,\"healthGeneration\":%u}",npcCombatNativeStage,npcCombatNativeInflicted,npcCombatNativeAfter,npcCombatNativeGeneration);
    const auto& fixture=npcCombatFixture;
    fprintf(f,",\"fixtureHoldsFrames\":%s,\"fixturePairedFrameReleases\":%llu",fixture.holdFrames?"true":"false",fixture.pairedFrameReleases);
    fprintf(f,",\"nativeSubtractions\":%llu,\"nativeLethalSubtractions\":%llu",npcCombatSubtractions,npcCombatLethalSubtractions);
    unsigned pendingRequests=0;for(const auto& b:s.channels)pendingRequests+=b.pendingRequests.count;
    fprintf(f,",\"pendingRequests\":%u,\"pendingHighWater\":%u",pendingRequests,s.pendingHighWater);
    std::array<NpcCombatCapture,8> sourceCopy{};std::uint64_t sourceCount=0;
    AcquireSRWLockShared(&npcCombatCaptureLock);sourceCopy=npcCombatSourceHistory;sourceCount=npcCombatSourceCount;ReleaseSRWLockShared(&npcCombatCaptureLock);
    fputs(",\"sourceHistory\":[",f);const auto begin=sourceCount>sourceCopy.size()?sourceCount-sourceCopy.size():0;
    for(auto i=begin;i<sourceCount;++i){const auto& e=sourceCopy[i%sourceCopy.size()];fprintf(f,"%s{\"nonce\":%llu,\"arm\":%llu,\"tick\":%llu,\"thread\":%lu,\"rawInfo\":\"",i==begin?"":",",e.nonce,e.arm,e.tick,e.thread);
        for(auto byte:e.info)fprintf(f,"%02x",byte);fputs("\",\"inflictor\":",f);writeNativeDamageSource(f,e.inflictor,reinterpret_cast<std::uintptr_t>(gameImage));
        fputs(",\"attacker\":",f);writeNativeDamageSource(f,e.attacker,reinterpret_cast<std::uintptr_t>(gameImage));fputs("}",f);}
    fputs("]",f);
    fprintf(f,",\"triggerFixture\":{\"nonce\":%llu,\"result\":\"%s\"}",npcCombatTriggerFixtureNonce,npcCombatTriggerFixtureResult);
    fprintf(f,",\"worldCommitted\":%llu,\"worldApplied\":%llu,\"replicaWorldSuppressed\":%llu",s.worldCommitted,s.worldApplied,s.replicaWorldSuppressed);
    fprintf(f,",\"hostWorldPassed\":%llu,\"hostWorldCommitted\":%llu,\"hostWorldApplied\":%llu,\"hostWorldGains\":%llu,\"helloFollowed\":%llu,\"helloWaited\":%llu,\"followSubtractions\":%llu",
        npcCombatHostWorldPassed.load(),s.hostWorldCommitted,s.hostWorldApplied,s.hostWorldGains,s.helloFollowed,s.helloWaited,npcCombatFollowSubtractions);
    fprintf(f,",\"ordinaryTestUntil\":%llu,\"testAssassinationsRefused\":%llu,\"fixtureFrameHolds\":%llu,\"fixture\":{\"active\":%s,\"ready\":%s,\"result\":\"%s\",\"acquired\":%llu,\"released\":%llu,\"lost\":%llu}}\n",npcCombatOrdinaryTestUntil.load(),npcCombatTestAssassinationsRefused.load(),npcCombatFixtureFrameHolds.load(),fixture.until?"true":"false",fixture.ready?"true":"false",fixture.result,fixture.acquired,fixture.released,fixture.lost);const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
