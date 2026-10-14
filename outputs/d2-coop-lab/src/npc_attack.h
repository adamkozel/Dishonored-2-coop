#pragma once
#include "npc_attack_protocol.h"
// Opt-in enemy sword contacts through existing native NPC authority bindings.
// Host owns physical hits; the receiving actual player retains native damage
// modifiers, reactions and death. No health or damage amount is sent remotely.
struct NpcAttackBinding {std::uint64_t arm=0,peerNonce=0;bool ready=false;};
struct NpcAttackPending {std::uint64_t event=0,arm=0;ULONGLONG at=0;unsigned weapon=0;};
struct NpcAttackCaptureIdentity {NativeNpcAuthority source{};std::uint64_t arm=0,nonce=0;};
struct NpcAttackContact {std::uint64_t arm=0,nonce=0;ULONGLONG tick=0;unsigned attacker=0,inflictor=0,flags=0;float offset[3]{},direction[3]{};};
struct NpcAttackCaptureScope {
    NativeHealth victim{};unsigned generation=0;ULONGLONG until=0;bool host=false,terminal=false;
    std::vector<NpcAttackCaptureIdentity> sources;
};
struct NpcAttackRuntime {
    bool enabled=false;std::uint64_t nonce=0,event=0,consumed=0,acked=0;
    ULONGLONG started=0,helloAt=0;DoorSessionScope scope{};
    std::vector<NpcAttackBinding> bindings;std::vector<NpcAttackPending> pending;
    std::uint64_t sent=0,applied=0,duplicates=0,rejected=0;
    float before=0,after=0,inflicted=0;unsigned lastAttacker=0;
    // Receiver native defense outcomes and host-side attacker reactions.
    std::uint64_t blocked=0,parryLose=0,parryWin=0,reactions=0,reactionFailures=0;
    NpcAttackDefense lastDefense=NpcAttackDefense::Damage;
    const char* result="disabled";CoopNpcAttack lastHit{};
};
struct NpcAttackSession {bool enabled=false,waitingCheckpoint=false,terminal=false;DoorSessionScope scope{};ULONGLONG deadline=0;std::uint64_t renewals=0;const char* retired="none";};
NpcAttackSession npcAttackSession{};
NpcAttackRuntime npcAttack{};std::uint64_t npcAttackNonce=0;int npcAttackRequest=-1;
SRWLOCK npcAttackCaptureLock=SRWLOCK_INIT;NpcAttackCaptureScope npcAttackCaptureScope{};
std::vector<NpcAttackContact> npcAttackContacts;
std::atomic<unsigned> npcAttackFault{0};
std::atomic<std::uint64_t> npcAttackCaptured{0},npcAttackReplicaBlocked{0},npcAttackTerminalBlocked{0};
thread_local bool npcAttackApplying=false;
// Passive observer of native arkPlayerStateBlock melee resolution 140859980
// (block 0, parry-lose 1, parry-win 2). It never changes arguments or result;
// it records whether a call came from the co-op relay or from native play.
// Window/gate fields: work/natural-moveset/block-window-v1016c (window58 is
// open for about 265 ms after block entry).
struct MeleeDefenseObservation {ULONGLONG tick=0;DWORD thread=0;std::uint32_t attacker=0;int result=-1;unsigned char heavy=0,window=0,gate=0,relay=0;float linger=0;};
using NativeMeleeDefense=int(*)(std::uintptr_t,std::uintptr_t,bool);
NativeMeleeDefense realNativeMeleeDefense=nullptr;
const unsigned char kMeleeDefensePrologue[17]={0x48,0x8b,0xc4,0x55,0x56,0x57,0x41,0x56,0x41,0x57,0x48,0x81,0xec,0x90,0x00,0x00,0x00};
SRWLOCK meleeDefenseObserveLock=SRWLOCK_INIT;
std::array<MeleeDefenseObservation,64> meleeDefenseObservations{};std::uint64_t meleeDefenseObservationCount=0;
std::array<std::atomic<std::uint64_t>,3> meleeDefenseNative{},meleeDefenseRelayed{};
thread_local bool meleeDefenseRelay=false;
int observeNativeMeleeDefense(std::uintptr_t state,std::uintptr_t attacker,bool heavy){
    MeleeDefenseObservation e{};e.tick=GetTickCount64();e.thread=GetCurrentThreadId();e.heavy=heavy;e.relay=meleeDefenseRelay;
    if(attacker)memoryRead(attacker+0xa8,&e.attacker,4);
    if(state){memoryRead(state+0x58,&e.window,1);memoryRead(state+0x59,&e.gate,1);memoryRead(state+0x74,&e.linger,4);}
    e.result=realNativeMeleeDefense(state,attacker,heavy);
    if(e.result>=0&&e.result<=2)(e.relay?meleeDefenseRelayed:meleeDefenseNative)[e.result].fetch_add(1);
    AcquireSRWLockExclusive(&meleeDefenseObserveLock);meleeDefenseObservations[meleeDefenseObservationCount++%meleeDefenseObservations.size()]=e;ReleaseSRWLockExclusive(&meleeDefenseObserveLock);
    return e.result;
}
bool meleeDefenseHooked(){
    if(!realNativeMeleeDefense)return false;
    void* target=nullptr;const auto* p=gameImage+0x859980;
    if(p[0]!=0xff||p[1]!=0x25||std::memcmp(p+2,"\0\0\0\0",4))return false;
    std::memcpy(&target,p+6,8);return target==reinterpret_cast<void*>(&observeNativeMeleeDefense);
}
bool installMeleeDefenseObserver(){
    // Seventeen bytes of complete non-relative instructions: mov rax,rsp; five
    // pushes; sub rsp,90. The original keeps its RAX=entry RSP convention.
    auto* target=gameImage+0x859980;if(std::memcmp(target,kMeleeDefensePrologue,sizeof(kMeleeDefensePrologue)))return false;
    auto* trampoline=static_cast<unsigned char*>(VirtualAlloc(nullptr,64,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));if(!trampoline)return false;
    std::memcpy(trampoline,kMeleeDefensePrologue,sizeof(kMeleeDefensePrologue));
    unsigned char jump[14]{0xff,0x25,0,0,0,0};void* resume=target+sizeof(kMeleeDefensePrologue);std::memcpy(jump+6,&resume,8);
    std::memcpy(trampoline+sizeof(kMeleeDefensePrologue),jump,sizeof(jump));DWORD old=0,unused=0;
    if(!VirtualProtect(trampoline,64,PAGE_EXECUTE_READ,&old)){VirtualFree(trampoline,0,MEM_RELEASE);return false;}
    FlushInstructionCache(GetCurrentProcess(),trampoline,64);realNativeMeleeDefense=reinterpret_cast<NativeMeleeDefense>(trampoline);
    void* wrapper=reinterpret_cast<void*>(&observeNativeMeleeDefense);std::memcpy(jump+6,&wrapper,8);
    if(!VirtualProtect(target,sizeof(jump),PAGE_EXECUTE_READWRITE,&old))return false;
    std::memcpy(target,jump,sizeof(jump));FlushInstructionCache(GetCurrentProcess(),target,sizeof(jump));
    return VirtualProtect(target,sizeof(jump),old,&unused)!=0;
}
// Exact-image entry points used by arkCpntWeaponMeleeNPC victim resolution
// 1406357d0. Static/runtime evidence: work/movement-integration/
// npc-block-native-v1011.json. Parry-win 140636140 and its FX helper
// 140635bf0: work/movement-integration/npc-parry-ghidra-v1015.
bool meleeDefenseApiMatches(){
    struct Entry {std::uintptr_t rva;unsigned char bytes[10];};
    static const Entry entries[]={
        {0x877720,{0x48,0x89,0x5c,0x24,0x10,0x44,0x89,0x44,0x24,0x18}},
        {0x878230,{0x48,0x89,0x5c,0x24,0x08,0x57,0x48,0x83,0xec,0x30}},
        {0x60d750,{0x48,0x89,0x5c,0x24,0x08,0x48,0x89,0x74,0x24,0x10}},
        {0x8112c0,{0x48,0x89,0x5c,0x24,0x08,0x48,0x89,0x74,0x24,0x10}},
        {0x810240,{0x48,0x8b,0x05,0x49,0x50,0x19,0x03,0x48,0x05,0x68}},
        {0x636140,{0x48,0x8b,0xc4,0x48,0x89,0x58,0x10,0x48,0x89,0x78}},
        {0x635bf0,{0x48,0x89,0x74,0x24,0x18,0x57,0x48,0x83,0xec,0x30}}};
    if(!gameImage)return false;
    for(const auto& e:entries)if(std::memcmp(gameImage+e.rva,e.bytes,sizeof(e.bytes)))return false;
    // 859980 is either untouched or carries exactly this observer.
    return meleeDefenseHooked()||!std::memcmp(gameImage+0x859980,kMeleeDefensePrologue,sizeof(kMeleeDefensePrologue));
}
// Generic NPC melee weapon from its equipped inventory slot0 (inventory
// reflection 1599460: equipment entries +58, slots +68; entry +8 weapon
// handle, +10 component, +18 declaration). Used only for cosmetic FX.
bool npcMeleeWeapon(std::uintptr_t actor,unsigned handle,std::uintptr_t& component,std::uintptr_t& decl){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t list=0,vt=0,owner=0,inventory=0,entry=0,game=0;unsigned flags=0,weapon=0,parent=0;
    if(!memoryRead(actor+0x268,&list,8)||!list||!memoryRead(actor+0x270,&flags,4)||(flags&0xffffff)>256)return false;
    for(unsigned i=0;i<(flags&0xffffff);++i){std::uintptr_t c=0;if(!memoryRead(list+i*8,&c,8)||!c||!memoryRead(c,&vt,8))return false;if(vt==base+0x1b9a878){if(inventory)return false;inventory=c;}}
    if(!inventory||!memoryRead(inventory+0x10,&owner,8)||owner!=actor||!memoryRead(inventory+0x68,&entry,8)||!entry||
       !memoryRead(entry+8,&weapon,4)||!memoryRead(entry+0x10,&component,8)||!component||!memoryRead(entry+0x18,&decl,8)||!decl||
       !memoryRead(component,&vt,8)||vt!=base+0x1b9cf48||!memoryRead(component+8,&owner,8)||owner!=decl||
       !memoryRead(base+0x39a5290,&game,8))return false;
    const auto entity=worldIdentityEntity(memoryRead,game,weapon);
    return entity&&memoryRead(component+0x10,&owner,8)&&owner==entity&&memoryRead(entity+0xac,&parent,4)&&parent==handle;
}
std::uintptr_t nativeComponent(std::uintptr_t entity,std::uintptr_t descriptorRva){
    return reinterpret_cast<std::uintptr_t(*)(std::uintptr_t,void*,int)>(gameImage+0x877720)(entity,gameImage+descriptorRva,2);
}
// Host: the attack bits native 1406357d0 reads from the hitting weapon.
bool nativeMeleeAttackFlags(std::uintptr_t weapon,unsigned& flags){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t vt=0,owner=0,decl=0;unsigned char blockable=0,state=0;
    if(!weapon||!meleeDefenseApiMatches())return false;
    const auto component=nativeComponent(weapon,0x39ba940); // arkCpntWeaponMeleeNPC
    if(!component||!memoryRead(component,&vt,8)||vt!=base+0x1b9cf48||!memoryRead(component+0x10,&owner,8)||owner!=weapon||
       !memoryRead(component+8,&decl,8)||!decl||!memoryRead(decl+0x180,&blockable,1)||!memoryRead(component+0x3428,&state,1))return false;
    flags=(blockable?kNpcAttackBlockable:0)|((state>>2&1)?kNpcAttackHeavy:0);return true;
}
// Receiver: native block cone (camera-based, requires PlayerStateMachine
// state44) then the actual block state's own resolution, which plays the
// native block impact/parry presentation. Returns false only for a fault.
bool resolveNativeMeleeDefense(std::uintptr_t player,std::uintptr_t attacker,unsigned flags,NpcAttackDefense& outcome){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);outcome=NpcAttackDefense::Damage;
    if(GetCurrentThreadId()!=engineThread.load()||!player||!attacker||!meleeDefenseApiMatches())return false;
    const auto block=nativeComponent(player,0x39edc20); // arkCpntBlock
    if(!block)return true;
    std::uintptr_t vt=0;if(!memoryRead(block,&vt,8)||vt!=base+0x1b99248)return false;
    alignas(16) float center[4]{};reinterpret_cast<float*(*)(std::uintptr_t,float*)>(gameImage+0x878230)(attacker,center);
    for(unsigned i=0;i<3;++i)if(!std::isfinite(center[i]))return false;
    const bool inCone=reinterpret_cast<bool(*)(std::uintptr_t,float*)>(gameImage+0x60d750)(block,center);
    if(!inCone||!(flags&kNpcAttackBlockable))return true;
    const auto machine=nativeComponent(player,0x39aed60); // arkCpntPlayerStateMachine
    std::uintptr_t table=0,state=0;unsigned char active=0;
    if(!machine||!memoryRead(machine,&vt,8)||vt!=base+0x1c73d58||!memoryRead(machine+0x90,&table,8)||!table||
       !memoryRead(table+0x170,&state,8)||!state||!memoryRead(state+0x20,&active,1)||!active)return false;
    meleeDefenseRelay=true;
    const int result=reinterpret_cast<int(*)(std::uintptr_t,std::uintptr_t,bool)>(gameImage+0x859980)(state,attacker,(flags&kNpcAttackHeavy)!=0);
    meleeDefenseRelay=false;
    if(result<0||result>2)return false;
    outcome=static_cast<NpcAttackDefense>(result+1);return true;
}
// Parry-win (native result 2) is applied by the attacker's weapon in
// 1406357d0 through 140636140: the weapon's m_blowbackDamage
// (blowback_player.damage: DAMAGETYPE_BLOWBACK, NPC_HITREACT_BIG, no
// m_maxDamage) is sent to the weapon owner with the defender as source. For a
// player defender native code also plays m_blowBackNpcFX (decl+168) on the
// attacker. The host runs exactly that on its authoritative attacker, with
// the owned clone standing in for the remote defender.
struct NpcParryWinScope {std::uintptr_t component=0;std::uint32_t source=0;bool seen=false,failed=false;float amount=-1,inflicted=-1,before=0,after=0;std::uint64_t flags=0;};
thread_local NpcParryWinScope* npcParryWinScope=nullptr;
constexpr char kBlowbackDamageDecl[]="components/characters/player/base/damage/blowback_player.damage";
std::uint64_t npcParryWinApplied=0,npcParryWinFailures=0,npcParryWinFx=0,npcParryWinFxFailures=0;
float npcParryWinLastAmount=-1,npcParryWinLastInflicted=-1;const char* npcParryWinLastResult="none";
bool routeNpcParryWinDamage(void* component,void* info,void* result){
    auto* s=npcParryWinScope;if(!s||reinterpret_cast<std::uintptr_t>(component)!=s->component||!info||!result)return false;
    std::array<unsigned char,144> data{};std::uintptr_t decl=0,name=0;unsigned handles[2]{};std::string path;
    if(s->seen||!memoryRead(reinterpret_cast<std::uintptr_t>(info),data.data(),data.size())){s->failed=true;return false;}
    std::memcpy(&decl,data.data(),8);std::memcpy(&s->amount,data.data()+8,4);std::memcpy(&s->flags,data.data()+0x10,8);std::memcpy(handles,data.data()+0x50,8);
    // Exactly the native blowback from the owned clone; anything else keeps
    // the ordinary firewall and filters.
    if(handles[0]!=s->source||handles[1]!=s->source||!memoryRead(decl+8,&name,8)||!worldIdentityString(memoryRead,name,path)||path!=kBlowbackDamageDecl){s->failed=true;return false;}
    s->seen=true;memoryRead(s->component+0x20,&s->before,4);
    realNativeDamage(component,info,result);
    if(!memoryRead(reinterpret_cast<std::uintptr_t>(result),&s->inflicted,4)||!std::isfinite(s->inflicted))s->failed=true;
    memoryRead(s->component+0x20,&s->after,4);
    return true;
}
bool applyNativeParryWin(NpcAuthorityChannel& c,unsigned weaponHandle){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);npcParryWinLastResult="preconditions";
    NativeHealth attacker{},clone{};std::uintptr_t vt=0,owner=0,decl=0,blowback=0;unsigned parent=0;
    if(GetCurrentThreadId()!=engineThread.load()||!meleeDefenseApiMatches()||!npcCombatIdentity(c,attacker)||!probeHandle||
       !readNativeHealth(memoryRead,base,probeHandle,clone)||clone.actor!=probeActor||clone.game!=attacker.game)return false;
    npcParryWinLastResult="weapon";
    const auto weapon=worldIdentityEntity(memoryRead,attacker.game,weaponHandle);
    if(!weapon||!memoryRead(weapon+0xac,&parent,4)||parent!=c.lease.binding.npc.handle)return false;
    const auto component=nativeComponent(weapon,0x39ba940);
    if(!component||!memoryRead(component,&vt,8)||vt!=base+0x1b9cf48||!memoryRead(component+0x10,&owner,8)||owner!=weapon||
       !memoryRead(component+8,&decl,8)||!decl||!memoryRead(decl+0x128,&blowback,8)||!blowback)return false;
    npcParryWinLastResult="native_blowback";
    NpcParryWinScope scope{attacker.component,probeHandle};npcParryWinScope=&scope;
    reinterpret_cast<void(*)(std::uintptr_t,std::uintptr_t)>(gameImage+0x636140)(component,clone.actor);
    npcParryWinScope=nullptr;npcParryWinLastAmount=scope.amount;npcParryWinLastInflicted=scope.inflicted;
    // A health change here would need an ordered combat commit; the observed
    // native declaration has no damage, so any loss is treated as a fault.
    if(!scope.seen||scope.failed||scope.inflicted!=0||scope.after!=scope.before){npcParryWinLastResult=!scope.seen?"blowback_not_observed":"blowback_unexpected";return false;}
    reinterpret_cast<void(*)(std::uintptr_t,std::uintptr_t,std::uintptr_t)>(gameImage+0x635bf0)(component,attacker.actor,decl+0x168);
    npcParryWinLastResult="applied";return true;
}
// Receiver: the defender's view of the same parry-win. Only the native NPC
// blowback FX is played on the local replica; the stagger itself arrives
// from the authoritative host through the NPC body stream.
bool playParryWinReplicaFx(NpcAuthorityChannel& c){
    std::uintptr_t component=0,decl=0;NativeHealth h{};
    if(GetCurrentThreadId()!=engineThread.load()||!meleeDefenseApiMatches()||!npcCombatIdentity(c,h)||
       !npcMeleeWeapon(h.actor,c.lease.binding.npc.handle,component,decl))return false;
    reinterpret_cast<void(*)(std::uintptr_t,std::uintptr_t,std::uintptr_t)>(gameImage+0x635bf0)(component,h.actor,decl+0x168);return true;
}
// Host: native attacker reaction for a receiver block/parry-lose, as posted
// by 1406357d0: event 0xe6 {victim, result} to the attacking NPC. The owned
// clone stands in for the remote victim. Parry-win stagger is not mirrored.
bool postNativeAttackBlocked(unsigned attacker,unsigned victim,NpcAttackDefense outcome){
    if(GetCurrentThreadId()!=engineThread.load()||!meleeDefenseApiMatches()||(outcome!=NpcAttackDefense::Blocked&&outcome!=NpcAttackDefense::ParryLose))return false;
    const auto queue=reinterpret_cast<std::uintptr_t(*)()>(gameImage+0x810240)();if(!queue)return false;
    auto* payload=reinterpret_cast<std::uint32_t*>(reinterpret_cast<void*(*)(std::uintptr_t,unsigned,unsigned,unsigned)>(gameImage+0x8112c0)(queue+0xc8e8,0xe6,8,attacker));
    if(!payload)return false;
    payload[0]=victim;payload[1]=static_cast<unsigned>(outcome)-1;return true;
}
bool npcAttackScope(ULONGLONG tick){
    return npcAttack.enabled&&npcStreamContext(tick)&&doorSessionWorldMatches(npcAttack.scope,outgoing,remote);
}
void clearNpcAttackCapture(){
    AcquireSRWLockExclusive(&npcAttackCaptureLock);npcAttackCaptureScope={};npcAttackContacts.clear();ReleaseSRWLockExclusive(&npcAttackCaptureLock);
}
void notifyNpcAttackStop();
void stopNpcAttack(const char* reason){
    clearNpcAttackCapture();
    const bool terminal=!strcmp(reason,"native_player_death")||!strcmp(reason,"peer_player_death");
    const bool retain=npcAttackSession.enabled&&(terminal||!strcmp(reason,"shared_death")||!strcmp(reason,"checkpoint_barrier")||!strcmp(reason,"shared_checkpoint")||!strcmp(reason,"native_checkpoint_command"));
    if(!retain&&npcAttack.enabled&&strcmp(reason,"peer_stopped"))notifyNpcAttackStop();
    npcAttack.enabled=false;npcAttack.result=reason;
    for(auto& b:npcAttack.bindings)b.ready=false;
    npcAttack.pending.clear();
    if(retain){
        // Waiting at Game Over is not a recovery timeout. The coordinated
        // native retry owns the bounded deadline once an actual load begins.
        if(npcStreamSessionState.reloadPending&&!npcAttackSession.deadline)npcAttackSession.deadline=npcStreamSessionState.deadline;
        npcAttackSession.waitingCheckpoint=true;npcAttackSession.terminal|=terminal;
    }else {if(npcAttackSession.enabled)npcAttackSession.retired=reason;npcAttackSession.enabled=npcAttackSession.waitingCheckpoint=npcAttackSession.terminal=false;}
}

bool routeNpcAttackDamage(void* component,void* info,void* result){
    if(npcAttackApplying||!component||!info||!result)return false;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);bool handled=false;
    AcquireSRWLockExclusive(&npcAttackCaptureLock);
    const auto& s=npcAttackCaptureScope;std::uintptr_t game=0,owner=0;unsigned generation=0;
    if(!s.sources.empty()&&GetTickCount64()<s.until&&reinterpret_cast<std::uintptr_t>(component)==s.victim.component&&
       memoryRead(base+0x39a5290,&game,8)&&game==s.victim.game&&worldIdentityEntity(memoryRead,game,s.victim.handle)==s.victim.actor&&
       memoryRead(s.victim.component+0x10,&owner,8)&&owner==s.victim.actor&&memoryRead(s.victim.component+0x18,&generation,4)&&generation==s.generation){
        std::array<unsigned char,144> data{};
        if(memoryRead(reinterpret_cast<std::uintptr_t>(info),data.data(),data.size())){
            std::uintptr_t decl=0;unsigned attacker=0,inflictor=0;float amount=0;std::uint64_t flags=0;
            std::memcpy(&decl,data.data(),8);std::memcpy(&amount,data.data()+8,4);std::memcpy(&flags,data.data()+0x10,8);
            std::memcpy(&inflictor,data.data()+0x50,4);std::memcpy(&attacker,data.data()+0x54,4);
            for(const auto& b:s.sources)if(attacker==b.source.npc.handle){
                const auto& n=b.source;unsigned parent=0;
                const auto weapon=worldIdentityEntity(memoryRead,game,inflictor);
                if(worldIdentityEntity(memoryRead,game,attacker)!=n.npc.actor||!weapon||!memoryRead(weapon+0xac,&parent,4)||parent!=attacker||
                   amount!=20||flags!=0x80||!nativeSwordDeclaration(decl))break;
                handled=true;
                if(s.host&&!s.terminal){
                    NpcAttackContact e{};e.arm=b.arm;e.nonce=b.nonce;e.tick=GetTickCount64();e.attacker=attacker;e.inflictor=inflictor;
                    if(!nativeMeleeAttackFlags(weapon,e.flags)){npcAttackFault.store(1);break;}
                    float root[3]{};bool good=memoryRead(s.victim.actor+0x300,root,12);
                    std::memcpy(e.offset,data.data()+0x24,12);std::memcpy(e.direction,data.data()+0x3c,12);
                    float norm=0;for(unsigned i=0;i<3;++i){e.offset[i]-=root[i];good=good&&std::isfinite(e.offset[i])&&std::fabs(e.offset[i])<=3&&std::isfinite(e.direction[i]);norm+=e.direction[i]*e.direction[i];}
                    good=good&&norm>=.9f&&norm<=1.1f;
                    if(good&&npcAttackContacts.size()<32){npcAttackContacts.push_back(e);++npcAttackCaptured;}
                    else npcAttackFault.store(1);
                }else if(s.terminal)++npcAttackTerminalBlocked;else ++npcAttackReplicaBlocked;
                break;
            }
        }
    }
    ReleaseSRWLockExclusive(&npcAttackCaptureLock);
    if(handled)reinterpret_cast<void(*)(void*)>(gameImage+0x780470)(result);
    return handled;
}
NpcAttackBinding* npcAttackBinding(std::uint64_t arm){for(auto& b:npcAttack.bindings)if(b.arm==arm)return &b;return nullptr;}
CoopNpcAttack npcAttackPacket(NpcAuthorityChannel& c,NpcAttackBinding& b,NpcAttackMessage kind){
    CoopNpcAttack p{};p.binding=npcHeader(c,NpcAuthorityMessage::Stop);p.kind=kind;p.sourceNonce=npcAttack.nonce;
    if(kind!=NpcAttackMessage::Hello)p.targetNonce=b.peerNonce;return p;
}
bool sendNpcAttack(CoopNpcAttack p,NpcAuthorityChannel& c){
    p.binding.sequence=++c.sequence;
    if(!sockets||!connection||connectionState!=k_ESteamNetworkingConnectionState_Connected||!validNpcAttack(p,remote,outgoing)||
       coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)!=k_EResultOK)return false;
    if(p.kind==NpcAttackMessage::Hit){++npcAttack.sent;npcAttack.lastHit=p;}return true;
}
void notifyNpcAttackStop(){
    if(GetCurrentThreadId()!=engineThread.load())return;
    for(auto& b:npcAttack.bindings)if(b.ready&&b.peerNonce){auto* c=npcCombatActor(b.arm);if(c&&c->enabled){sendNpcAttack(npcAttackPacket(*c,b,NpcAttackMessage::Stop),*c);break;}}
}
void beginNpcAttack(ULONGLONG tick){
    if(npcAttack.enabled)return;
    if(!npcStreamContext(tick)||!swordDamageApiMatches()||!probeFollow||!avatarAiHeld||!probeDoppelganger){npcAttack.result="unavailable";return;}
    clearNpcAttackCapture();npcAttackFault.store(0);npcAttackCaptured.store(0);npcAttackReplicaBlocked.store(0);npcAttackTerminalBlocked.store(0);
    npcAttack={};npcAttack.enabled=true;npcAttack.nonce=++npcAttackNonce;npcAttack.started=tick;npcAttack.scope=doorSessionScope(outgoing,remote);npcAttack.result="negotiating";
    for(auto& c:npcChannels)if(c.enabled&&c.ready&&c.peerArm)npcAttack.bindings.push_back({c.arm});
    if(npcAttack.bindings.empty())stopNpcAttack("no_ready_actors");
}
bool applyNpcAttack(NpcAuthorityChannel& c,const CoopNpcAttack& p){
    NativeHealth h{},source{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(!npcCombatIdentity(c,source)||!readNativeHealth(memoryRead,base,1,h)||h.game!=observedGame||h.logic!=1||h.current<=0||h.current>h.maximum||!swordDamageApiMatches())return false;
    const auto decl=reinterpret_cast<std::uintptr_t(*)(void*,const char*,bool,bool)>(gameImage+0x1e030)(gameImage+0x3990b60,kSwordDamageDecl,false,false);
    if(!nativeSwordDeclaration(decl))return false;
    float position[3]{};if(!memoryRead(h.actor+0x300,position,12))return false;
    for(unsigned i=0;i<3;++i){position[i]+=p.offset[i];if(!std::isfinite(position[i]))return false;}
    alignas(16) std::array<unsigned char,144> params{};alignas(16) std::array<unsigned char,32> result{};
    reinterpret_cast<void(*)(void*,void*)>(gameImage+0x780260)(params.data(),reinterpret_cast<void*>(decl));
    reinterpret_cast<void(*)(void*)>(gameImage+0x780470)(result.data());
    const unsigned regionMask=1,region=0;std::memcpy(params.data()+0x20,&regionMask,4);std::memcpy(params.data()+0x4c,&region,4);
    std::memcpy(params.data()+0x24,position,12);std::memcpy(params.data()+0x3c,p.direction,12);std::memcpy(params.data()+0x54,&c.lease.binding.npc.handle,4);
    npcAttack.before=h.current;npcAttack.lastAttacker=c.lease.binding.npc.handle;
    // A native block is resolved by the attacker's weapon before any damage
    // event (1406357d0). Run that resolution against the local replica of the
    // authoritative attacker; a defended contact never reaches 717950.
    if(source.actor!=c.lease.binding.npc.actor||!resolveNativeMeleeDefense(h.actor,source.actor,p.flags,npcAttack.lastDefense))return false;
    if(npcAttack.lastDefense!=NpcAttackDefense::Damage){
        npcAttack.inflicted=0;npcAttack.after=h.current;
        (npcAttack.lastDefense==NpcAttackDefense::Blocked?npcAttack.blocked:npcAttack.lastDefense==NpcAttackDefense::ParryLose?npcAttack.parryLose:npcAttack.parryWin)++;
        if(npcAttack.lastDefense==NpcAttackDefense::ParryWin)(playParryWinReplicaFx(c)?npcParryWinFx:npcParryWinFxFailures)++;
        return true;
    }
    npcAttackApplying=true;reinterpret_cast<NativeDamage>(gameImage+0x717950)(reinterpret_cast<void*>(h.component),params.data(),result.data());npcAttackApplying=false;
    std::memcpy(&npcAttack.inflicted,result.data(),4);NativeHealth after{};
    if(!readNativeHealth(memoryRead,base,1,after)||after.actor!=h.actor||after.component!=h.component||!std::isfinite(npcAttack.inflicted))return false;
    npcAttack.after=after.current;++npcAttack.applied;return true;
}
void receiveNpcAttack(const CoopNpcAttack& p,ULONGLONG tick){
    if(!npcAttackScope(tick)||!validNpcAttack(p,outgoing,remote)){++npcAttack.rejected;return;}
    auto* b=npcAttackBinding(p.binding.targetArm);auto* c=npcCombatActor(p.binding.targetArm);
    if(!b||!c||!c->enabled||!c->ready||p.binding.sourceArm!=c->peerArm||p.binding.sourceHandle!=c->peerHandle||
       p.binding.targetHandle!=c->lease.binding.npc.handle||p.binding.rig!=c->rig||p.binding.joints!=c->render.joints){++npcAttack.rejected;return;}
    if(p.kind==NpcAttackMessage::Hello){
        if(b->peerNonce&&b->peerNonce!=p.sourceNonce){stopNpcAttack("peer_replaced");return;}
        b->peerNonce=p.sourceNonce;if(!sendNpcAttack(npcAttackPacket(*c,*b,NpcAttackMessage::Ready),*c))stopNpcAttack("send_failed");return;
    }
    if(p.targetNonce!=npcAttack.nonce||!b->peerNonce||p.sourceNonce!=b->peerNonce){++npcAttack.rejected;return;}
    if(p.kind==NpcAttackMessage::Ready){b->ready=true;return;}
    if(p.kind==NpcAttackMessage::Stop){stopNpcAttack("peer_stopped");return;}
    if(!b->ready){++npcAttack.rejected;return;}
    if(p.kind==NpcAttackMessage::Terminal){
        if(localRole||!p.event||p.event!=npcAttack.acked){++npcAttack.rejected;return;}
        stopNpcAttack("peer_player_death");return;
    }
    if(p.kind==NpcAttackMessage::Ack){
        if(p.event<=npcAttack.acked){++npcAttack.duplicates;return;}
        if(npcAttack.pending.empty()||npcAttack.pending.front().event!=p.event||npcAttack.pending.front().arm!=c->arm){stopNpcAttack("ack_order");return;}
        const auto weapon=npcAttack.pending.front().weapon;
        npcAttack.acked=p.event;npcAttack.pending.erase(npcAttack.pending.begin());
        const auto outcome=static_cast<NpcAttackDefense>(p.flags);npcAttack.lastDefense=outcome;
        if(outcome==NpcAttackDefense::Blocked)++npcAttack.blocked;else if(outcome==NpcAttackDefense::ParryLose)++npcAttack.parryLose;else if(outcome==NpcAttackDefense::ParryWin)++npcAttack.parryWin;
        if(outcome==NpcAttackDefense::Blocked||outcome==NpcAttackDefense::ParryLose){
            NativeHealth attacker{};
            if(npcCombatIdentity(*c,attacker)&&probeHandle&&postNativeAttackBlocked(c->lease.binding.npc.handle,probeHandle,outcome))++npcAttack.reactions;
            else ++npcAttack.reactionFailures;
        }else if(outcome==NpcAttackDefense::ParryWin){
            if(applyNativeParryWin(*c,weapon)){++npcAttack.reactions;++npcParryWinApplied;}
            else{++npcAttack.reactionFailures;++npcParryWinFailures;}
        }
        return;
    }
    if(p.event<=npcAttack.consumed)++npcAttack.duplicates;
    else{
        if(p.event!=npcAttack.consumed+1){stopNpcAttack("event_gap");return;}
        // Consume before the native call, including failed readback.
        npcAttack.consumed=p.event;if(!applyNpcAttack(*c,p)){stopNpcAttack("native_application_failed");return;}
    }
    auto ack=npcAttackPacket(*c,*b,NpcAttackMessage::Ack);ack.event=p.event;
    if(p.event==npcAttack.consumed)ack.flags=static_cast<unsigned>(npcAttack.lastDefense);
    if(!sendNpcAttack(ack,*c)){stopNpcAttack("ack_failed");return;}
    if(npcAttack.after==0&&npcAttack.applied){
        auto terminal=npcAttackPacket(*c,*b,NpcAttackMessage::Terminal);terminal.event=p.event;
        if(!sendNpcAttack(terminal,*c)){stopNpcAttack("terminal_send_failed");return;}
        stopNpcAttack("native_player_death");
    }
}
void publishNpcAttackCapture(ULONGLONG tick,bool terminal){
    NpcAttackCaptureScope next{};
    {
        const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
        if(!probeFollow||!avatarAiHeld||!readNativeHealth(memoryRead,base,localRole?1:probeHandle,next.victim)||
           next.victim.game!=observedGame||next.victim.logic!=1||next.victim.current<=0||
           (!localRole&&next.victim.actor!=probeActor)||!memoryRead(next.victim.component+0x18,&next.generation,4)){
            // Death UI may retire the presentation actor before the paired
            // checkpoint barrier is published. Only discard this temporary
            // protection registry; retained session IDs must survive that gap.
            if(terminal)clearNpcAttackCapture();else stopNpcAttack("victim_replaced");return;
        }
        next.host=!localRole;next.terminal=terminal;next.until=tick+250;
        for(auto& b:npcAttack.bindings){auto* c=npcCombatActor(b.arm);if(c&&c->enabled&&c->ready)next.sources.push_back({c->lease.binding,b.arm,npcAttack.nonce});}
    }
    AcquireSRWLockExclusive(&npcAttackCaptureLock);npcAttackCaptureScope=std::move(next);ReleaseSRWLockExclusive(&npcAttackCaptureLock);
}
void serviceNpcAttack(ULONGLONG tick){
    if(npcAttackRequest>=0){
        const auto q=npcAttackRequest;npcAttackRequest=-1;
        if(!q){stopNpcAttack("disabled");return;}
        if(!npcAttackSession.enabled){
            beginNpcAttack(tick);
            if(npcAttack.enabled){npcAttackSession={};npcAttackSession.enabled=true;npcAttackSession.scope=doorSessionScope(outgoing,remote);}
        }
    }
    if(!npcAttack.enabled){
        auto& intent=npcAttackSession;
        if(!intent.enabled||!intent.waitingCheckpoint)return;
        if(!sockets||!connection||connectionState!=k_ESteamNetworkingConnectionState_Connected||
           outgoing.session!=intent.scope.session||remote.session!=intent.scope.peerSession||
           !npcStreamSessionState.enabled||(intent.deadline&&tick>intent.deadline)){
            stopNpcAttack("recovery_scope_expired");return;
        }
        // Native loading legitimately publishes an unavailable map ID. Only
        // compare maps once both normal gameplay identities are present.
        if(coopWorldPairReady(outgoing,remote)&&(outgoing.mapId!=intent.scope.map||remote.mapId!=intent.scope.map)){
            stopNpcAttack("recovery_map_changed");return;
        }
        if(intent.terminal&&!localRole&&npcStreamContext(tick)&&doorSessionWorldMatches(intent.scope,outgoing,remote))publishNpcAttackCapture(tick,true);
        if(!doorSessionRenewalReady(intent.scope,outgoing,remote)||npcStreamSessionState.reloadPending||!npcStreamContext(tick)||
           !probeFollow||!avatarAiHeld||npcChannels.empty()||
           !std::all_of(npcChannels.begin(),npcChannels.end(),[](const auto& c){return c.enabled&&c.ready&&c.peerArm;}))return;
        beginNpcAttack(tick);
        if(npcAttack.enabled){intent.scope=doorSessionScope(outgoing,remote);intent.waitingCheckpoint=intent.terminal=false;intent.deadline=0;++intent.renewals;}
        else stopNpcAttack("renewal_refused");
        return;
    }
    if(!npcAttackScope(tick)){stopNpcAttack("session_or_world_changed");return;}
    if(npcAttackFault.exchange(0)){stopNpcAttack("capture_failed");return;}
    std::vector<NpcAttackContact> captured;AcquireSRWLockExclusive(&npcAttackCaptureLock);captured.swap(npcAttackContacts);ReleaseSRWLockExclusive(&npcAttackCaptureLock);
    for(const auto& e:captured){
        auto* b=npcAttackBinding(e.arm);auto* c=npcCombatActor(e.arm);NativeHealth h{};
        if(localRole||e.nonce!=npcAttack.nonce||tick<e.tick||tick-e.tick>=250||!b||!b->ready||!c||!npcCombatIdentity(*c,h)||npcAttack.pending.size()>=32){stopNpcAttack("contact_expired_or_replaced");return;}
        auto p=npcAttackPacket(*c,*b,NpcAttackMessage::Hit);p.event=++npcAttack.event;
        std::memcpy(p.offset,e.offset,12);std::memcpy(p.direction,e.direction,12);p.flags=e.flags;
        if(!sendNpcAttack(p,*c)){stopNpcAttack("hit_send_failed");return;}
        npcAttack.pending.push_back({p.event,c->arm,tick,e.inflictor});
    }
    if(!npcAttack.pending.empty()&&tick-npcAttack.pending.front().at>5000){stopNpcAttack("ack_timeout");return;}
    if(tick-npcAttack.helloAt>=500){npcAttack.helloAt=tick;
        for(auto& b:npcAttack.bindings){auto* c=npcCombatActor(b.arm);NativeHealth h{};
            if(!c||!npcCombatIdentity(*c,h)){stopNpcAttack("actor_retired");return;}
            if(!b.ready&&!sendNpcAttack(npcAttackPacket(*c,b,NpcAttackMessage::Hello),*c)){stopNpcAttack("hello_failed");return;}
        }
    }
    const bool ready=std::all_of(npcAttack.bindings.begin(),npcAttack.bindings.end(),[](const auto& b){return b.ready;});
    if(!ready&&tick-npcAttack.started>5000){stopNpcAttack("negotiation_timeout");return;}
    npcAttack.result=ready?"active_native_sword":"negotiating";
    if(ready)publishNpcAttackCapture(tick,false);
}
void repeatNpcAttack(){
    if(!localRole&&npcAttack.enabled&&npcAttack.lastHit.event){auto* c=npcCombatActor(npcAttack.lastHit.binding.sourceArm);if(c)sendNpcAttack(npcAttack.lastHit,*c);}
}
void writeNpcAttack(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.npc-attack.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;
    fprintf(f,"{\"experimental\":true,\"enabled\":%s,\"result\":\"%s\",\"nonce\":%llu,\"event\":%llu,\"consumed\":%llu,\"acked\":%llu,\"pending\":%zu,\"captured\":%llu,\"replicaBlocked\":%llu,\"sent\":%llu,\"applied\":%llu,\"duplicates\":%llu,\"rejected\":%llu,\"before\":%.9g,\"after\":%.9g,\"inflicted\":%.9g,\"lastAttacker\":%u,\"blocked\":%llu,\"parryLose\":%llu,\"parryWin\":%llu,\"reactions\":%llu,\"reactionFailures\":%llu,\"lastDefense\":%u,\"entries\":[",
        npcAttack.enabled?"true":"false",npcAttack.result,npcAttack.nonce,npcAttack.event,npcAttack.consumed,npcAttack.acked,npcAttack.pending.size(),npcAttackCaptured.load(),npcAttackReplicaBlocked.load(),npcAttack.sent,npcAttack.applied,npcAttack.duplicates,npcAttack.rejected,npcAttack.before,npcAttack.after,npcAttack.inflicted,npcAttack.lastAttacker,npcAttack.blocked,npcAttack.parryLose,npcAttack.parryWin,npcAttack.reactions,npcAttack.reactionFailures,static_cast<unsigned>(npcAttack.lastDefense));
    for(unsigned i=0;i<npcAttack.bindings.size();++i){const auto& b=npcAttack.bindings[i];fprintf(f,"%s{\"arm\":%llu,\"peerNonce\":%llu,\"ready\":%s}",i?",":"",b.arm,b.peerNonce,b.ready?"true":"false");}
    std::array<MeleeDefenseObservation,64> defense{};std::uint64_t defenseCount=0;
    AcquireSRWLockShared(&meleeDefenseObserveLock);defense=meleeDefenseObservations;defenseCount=meleeDefenseObservationCount;ReleaseSRWLockShared(&meleeDefenseObserveLock);
    fprintf(f,"],\"blowback\":{\"applied\":%llu,\"failures\":%llu,\"replicaFx\":%llu,\"replicaFxFailures\":%llu,\"lastAmount\":%.9g,\"lastInflicted\":%.9g,\"lastResult\":\"%s\"}",
        npcParryWinApplied,npcParryWinFailures,npcParryWinFx,npcParryWinFxFailures,npcParryWinLastAmount,npcParryWinLastInflicted,npcParryWinLastResult);
    fprintf(f,",\"defenseObserver\":{\"hooked\":%s,\"native\":[%llu,%llu,%llu],\"relayed\":[%llu,%llu,%llu],\"count\":%llu,\"recent\":[",meleeDefenseHooked()?"true":"false",
        meleeDefenseNative[0].load(),meleeDefenseNative[1].load(),meleeDefenseNative[2].load(),meleeDefenseRelayed[0].load(),meleeDefenseRelayed[1].load(),meleeDefenseRelayed[2].load(),defenseCount);
    const auto first=defenseCount>defense.size()?defenseCount-defense.size():0;
    for(auto n=first;n<defenseCount;++n){const auto& e=defense[n%defense.size()];
        fprintf(f,"%s{\"tick\":%llu,\"thread\":%lu,\"attacker\":%u,\"result\":%d,\"heavy\":%u,\"window\":%u,\"gate\":%u,\"linger\":%.6g,\"relay\":%s}",n>first?",":"",e.tick,e.thread,e.attacker,e.result,e.heavy,e.window,e.gate,e.linger,e.relay?"true":"false");}
    fprintf(f,"]},\"session\":{\"enabled\":%s,\"waitingCheckpoint\":%s,\"terminal\":%s,\"renewals\":%llu,\"terminalBlocked\":%llu,\"retired\":\"%s\"}}\n",npcAttackSession.enabled?"true":"false",npcAttackSession.waitingCheckpoint?"true":"false",npcAttackSession.terminal?"true":"false",npcAttackSession.renewals,npcAttackTerminalBlocked.load(),npcAttackSession.retired);const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
