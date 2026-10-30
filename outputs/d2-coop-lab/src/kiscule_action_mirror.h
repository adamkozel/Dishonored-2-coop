#pragma once
#include "kiscule_policy.h" // stage 8.4.1 (#237)
// 1214bi: player-scoped script actions run in both worlds (chapter 97).
// Story grants are Kiscule action nodes that act on the player of the world
// that runs them: the Outsider's Mark, Blink, the Heart and the mana pool in
// the void (take route-eotw-v1302c: Corvo had the Mark, Emily did not), and
// the Dunwall safe-room loadout. Every queued Kiscule signal is dispatched
// by 0x1407c0e30 (confirmed static): signal+0xc node id, +0x10 input id,
// +0x18 node instance (its vtable is the node class). node+0x20 is the
// node's arkKisculeInstance, whose +0x60 is the owning entity's handle; the
// Kiscule manager (game+0x1f6e48) hashes entity handle -> instance (buckets
// +0x18, mask +0x28, sorted chains {key, value +8, next +0x10}). node+0x28 is
// the node decl (+8 id, inputs at +0x40, stride 0x48, input+8 id, input+0x20
// descriptor with the handler at +0x30 (node) or +0x38 (node, desc, signal)).
// For five classes (inventory, loadout, mana, the Outsider's Mark, the tutorial end) each world
// counts its handler runs per (entity, node, input) and sends the count. The
// peer runs the same node's native handler in its own world until its count
// matches, and a later local request the mirror already covered skips only
// the handler, so the node's outputs still fire. Mirrored runs never echo.
// 1214ca (#73): the host's world is the story authority, as for gameflow.
// Only the host sends its counts; a partner's own runs stay local and are
// only counted. In held take route-tut-v1310g the partner's world looped its
// stealth restart (its own Corvo kept seeing the idle partner at the start)
// and each restart's empty loadout (node 1868853291, 51 runs) was mirrored
// into the host's world, emptying the host's hands in the crossbow lesson.
// 1214jc (#183): a partner's first run of a mirrored node reaches the host once (count 1, reserved 1), when that first
// run was her own (the host had not run it), off the Memories of Training map. In the partner-led Dunwall route
// (route-dunwall-lead-v1756b) her safe-room locker (ld_ring_locker_01_3) ran three grants in her world only (host
// local 1, hers 4), and the shared locker then stayed open and empty for the host. The host runs the node once; its own
// later request is then covered, and her restart loops cannot replay (one run per node, never on the tutorial map).
constexpr std::uint32_t kKisculePartnerFirst=1;
bool kisculePartnerScope(){return std::strncmp(mapName,"campaign/tutorial/",18)!=0;}
using NativeKisculeDispatch=unsigned char*(*)(void*,unsigned char*,unsigned char*);
using NativeKisculeHandler=char(*)(void*);
using NativeKisculeHandlerEx=char(*)(void*,void*,void*);
NativeKisculeDispatch realKisculeDispatch=nullptr;
const unsigned char kisculeDispatchPrefix[15]={0x48,0x89,0x5c,0x24,0x10,0x48,0x89,0x74,0x24,0x18,0x57,0x48,0x83,0xec,0x20};
struct KisculeMirrorClass{std::uintptr_t vtable;const char* name;bool mirrored;};
// 1214bk: the Outsider's offer is each player's own choice. Its scene runs in
// both worlds (the trigger mirror carries ld_trigger_19), so the Mark and the
// character's powers (Corvo Blink, Emily Far Reach) come from each world's
// own scripts (take route-eotw-v1302e); 1214bi copied Corvo's Blink grant
// into Emily's world (take route-eotw-v1302d). The Mark control and power or
// no-power inventory items stay local and are only recorded.
// 1214bz (#73): Memories of Training ends with arkKisculeNodeActionTutorialEnd
// (its vtable at 0x141b7b628 follows the parameters vtable; RTTI type
// descriptor .?AVarkKisculeNodeActionTutorialEnd@@, confirmed static), which
// returns the world that runs it to the main menu. Only the world that played
// Corvo's fatality ran it (held take route-tut-v1310b: the host went to the
// menu, the partner stayed in the combat room), so it is mirrored like a
// story grant: the peer runs the same node in its own world.
constexpr int kisculeMirrorClassCount=5;
const KisculeMirrorClass kisculeMirrorClasses[kisculeMirrorClassCount]={{0x1b7ddf8,"inventory",true},{0x1b7df48,"loadout",true},{0x1c65ef8,"mana",true},{0x1cd4238,"mark",false},{0x1b7b628,"tutorial_end",true}};
const unsigned char kisculeVariableAccessorPrefix[19]={0x48,0x89,0x5c,0x24,0x10,0x57,0x48,0x83,0xec,0x30,0x48,0x8b,0x01,0x48,0x8b,0xfa,0x48,0x8b,0xd9};
using NativeKisculeVariable=std::uintptr_t*(*)(void*,const void*);
// The item an inventory node acts on, through the node's own variable
// accessor (0x140534540 with the m_inventoryItem name, as its Add handler
// does). Powers are declarations of type 9; no_power is the refusal item.
bool kisculePlayerScopedItem(std::uintptr_t node,std::string& item){
    if(std::memcmp(gameImage+0x534540,kisculeVariableAccessorPrefix,sizeof(kisculeVariableAccessorPrefix)))return false;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    const auto* slot=reinterpret_cast<NativeKisculeVariable>(gameImage+0x534540)(reinterpret_cast<void*>(node),gameImage+0x3984308);
    std::uintptr_t decl=0,vt=0,name=0;std::uint32_t type=0;
    if(!slot||!memoryRead(reinterpret_cast<std::uintptr_t>(slot),&decl,8)||!decl||!memoryRead(decl,&vt,8)||vt!=base+0x1b9ea48||
       !memoryRead(decl+0x60,&type,4)||!memoryRead(decl+8,&name,8)||!worldIdentityString(memoryRead,name,item))return false;
    return type==9||item.find("/powers/")!=std::string::npos||item.find("no_power")!=std::string::npos;
}
// 1214cb (#85): a few grants differ by character. CompareEntityDef(player,
// corvo/body.def) feeds an ActionTestBool whose True and False outputs give
// Corvo's or Emily's item (confirmed static; tools/diag/d2_character_grants.py
// over every campaign map finds these three). The host's run carries its own
// branch, so a partner of the other character runs the other branch's node
// (take loadout-dunwall-v1311c: Emily received gun_player, which her item
// wheel does not show, instead of gun_emily).
struct KisculeCharacterGrant{const char* map;const char* entity;std::uint32_t corvoNode,corvoInput,emilyNode,emilyInput;};
const KisculeCharacterGrant kisculeCharacterGrants[]={
    {"campaign/dunwall/escape/tower/dunwall_escape_tower_p","ld_kiscule",2305,2,3778,2},      // gun_player / gun_emily
    {"campaign/dunwall/escape/tower/dunwall_escape_tower_p","ld_kiscule_coup",4209,2,4215,2}, // gun_player / gun_emily
    {"campaign/boat/edge_of_the_world/boat_eotw_p","script_kiscule",7033,2,7038,2},          // crossbow_corvo / crossbow_emily
};
// The local player's character: 1 Corvo, 2 Emily, 0 unknown (body definition at actor+0x2e8).
int kisculeLocalCharacter(){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t definition=0,vt=0,name=0;std::string body;
    if(!observedPlayer||!memoryRead(observedPlayer+0x2e8,&definition,8)||!definition||!memoryRead(definition,&vt,8)||vt!=base+0x1ab58f8||
       !memoryRead(definition+8,&name,8)||!worldIdentityString(memoryRead,name,body))return 0;
    return body=="models/characters/player/corvo/body.def"?1:body=="models/characters/player/emily/body.def"?2:0;
}
// The node and input this world runs for a peer's (node, input): the same one, or the other character's branch.
void kisculeCharacterCounterpart(const char* entity,std::uint32_t& node,std::uint32_t& input){
    const int character=kisculeLocalCharacter();if(!character)return;
    for(const auto& g:kisculeCharacterGrants){
        if(std::strcmp(g.entity,entity)||std::strcmp(g.map,mapName))continue;
        if(character==2&&node==g.corvoNode&&input==g.corvoInput){node=g.emilyNode;input=g.emilyInput;return;}
        if(character==1&&node==g.emilyNode&&input==g.emilyInput){node=g.corvoNode;input=g.corvoInput;return;}
    }
}
#pragma pack(push,1)
struct KisculeActionPacket {
    std::uint32_t magic=0x414B3244; // "D2KA"
    std::uint16_t version=1,size=sizeof(KisculeActionPacket);
    std::uint32_t role=0,classIndex=0;
    std::uint64_t session=0,mapId=0,sequence=0;
    std::uint32_t node=0,input=0,count=0,reserved=0;
    char entity[96]{};
};
#pragma pack(pop)
static_assert(sizeof(KisculeActionPacket)==152);
struct KisculeActionCount{std::string entity;std::uint32_t node=0,input=0,cls=0;std::uintptr_t instanceNode=0;std::uint32_t requests=0,executed=0,asked=0,hostCount=0;};
struct KisculeActionEvent{std::uint64_t tick=0;std::uint32_t cls=0,node=0,input=0,count=0;char entity[64]{};const char* what="";};
struct KisculePending{KisculeActionPacket packet{};std::uint64_t at=0;};
struct KisculeMirror {
    std::uint64_t hostStoryBlocked=0,hostStoryAsked=0,hostStoryLeaked=0,hostStoryInFlow=0,resultScreenHeld=0,sequence=0,peerSequence=0,local=0,personal=0,sent=0,received=0,applied=0,suppressed=0,suppressFallback=0,refused=0,notFound=0,nested=0,identity=0,expired=0,otherMap=0,dropped=0,threadMismatch=0;
    std::uint32_t dispatchThread=0;const char* result="idle";
    std::vector<KisculeActionCount> counts;std::vector<KisculeActionPacket> outgoingQueue;std::vector<KisculePending> pending;
    KisculeActionEvent events[256]{};std::uint64_t eventCount=0;
};
KisculeMirror kisculeMirror{};SRWLOCK kisculeLock=SRWLOCK_INIT;bool kisculeMirrorEnabled=false;bool applyingKiscule=false;
// Stage 8.4.1 (#237): host story. With coop_host_story on, every World and LocalPlayer class of kiscule_policy.h is
// mirrored host-authoritatively: the host runs it and sends its count; the partner never runs it on its own (the
// handler is stubbed, so the partner's own flow and presentation continue) and runs it only as the host's replay.
// A node the host has not run yet is requested once (partner-first), so a step the partner reaches first still
// happens, in the host's world. Class indices from kKisculeHostStoryBase are policy-table indices.
bool kisculeHostStory=false;constexpr int kKisculeHostStoryBase=100;
// In the product, host story is on unless the host's d2coop.cfg sets host_story=0 (D2COOP_HOST_STORY=0); the partner
// follows the host's setting from its hello (control flag 4). The lab sets it with coop_host_story instead.
bool productHostStory=true;
bool coopHostStoryFlag(){return localRole==0&&kisculeHostStory;}
// Lab bisection (coop_host_story exempt <Class>): a World class taken out of host story runs in both worlds again.
// Character-specific story items (Ramsey's ring is ld_imperial_ring_pickup_corvo or _emily) are suspected to suffer
// when the host's branch is replayed into the partner's world (hoststory-lead-dunwall-v2017d to v2021a).
std::array<bool,kKisculePolicyCount> kisculeHostStoryExempt{};
// Mutation twin (lab only, coop_host_story twin_no_block): the partner runs its own story nodes again while the
// host's replays still arrive. Takes must catch it (the partner's own story-node runs must be zero).
bool kisculeHostStoryTwinNoBlock=false;
// Partner-first requests for World nodes: a story step only the partner reaches (searching Ramsey's body in the
// partner-led route, hoststory-lead-dunwall-v2017d) must still happen in the host's world. They are asked and run only
// while both games are in plain gameplay: with host story on through a mission intro, the host's game vanished in the
// opening cinematic twice while running such requests (hoststory-lead-dunwall-v2014c, v2016c).
bool kisculeHostStoryAsk=true;
bool kisculePlainGameplay(){return outgoing.gameLogicState==1&&!outgoing.pendingGameLogicState;}
// Any World class may be requested, by count: a node is requested whenever the partner reached it more often than the
// host has run it (the host had already run the loot's task nodes 2899 and 2973 earlier on the route, so a one-time
// request never went out, loot-trace-on-v2018e). A story-only list was too narrow: Ramsey's ring task follows an
// ActionUsable in the partner's flow (hoststory-lead-dunwall-v2019a). Ambient loops run in both worlds at the same
// pace, so the partner is rarely ahead on them and their requests stay rare.
bool kisculeHostStoryAskable(int cls){return cls>=kKisculeHostStoryBase&&cls<kKisculeHostStoryBase+static_cast<int>(kKisculePolicyCount);}
bool kisculeClassIndexMirrored(std::uint32_t cls){
    if(cls<static_cast<std::uint32_t>(kisculeMirrorClassCount))return kisculeMirrorClasses[cls].mirrored;
    return kisculeHostStory&&cls>=static_cast<std::uint32_t>(kKisculeHostStoryBase)&&cls<kKisculeHostStoryBase+kKisculePolicyCount;}
bool kisculeMirrorActive(){return kisculeMirrorEnabled||productMode;}
// Caller holds kisculeLock.
void kisculeEvent(const char* what,std::uint32_t cls,const std::string& entity,std::uint32_t node,std::uint32_t input,std::uint32_t count){
    auto& e=kisculeMirror.events[kisculeMirror.eventCount++%256];e={};e.tick=GetTickCount64();e.what=what;e.cls=cls;e.node=node;e.input=input;e.count=count;
    std::snprintf(e.entity,sizeof(e.entity),"%s",entity.c_str());
}
int kisculeClassOfVtable(std::uintptr_t vt){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    for(int i=0;i<kisculeMirrorClassCount;++i)if(vt==base+kisculeMirrorClasses[i].vtable)return i;
    if(kisculeHostStory&&vt>base){const int i=kisculePolicyIndex(static_cast<std::uint32_t>(vt-base));if(i>=0&&!kisculeHostStoryExempt[i])return kKisculeHostStoryBase+i;}
    return -1;
}
int kisculeClassOf(std::uintptr_t node){std::uintptr_t vt=0;return node&&memoryRead(node,&vt,8)?kisculeClassOfVtable(vt):-1;}
// The top-level Kiscule instance of an entity, from the manager's handle hash.
std::uintptr_t kisculeInstanceFor(std::uint32_t handle){
    if(!observedGame)return 0;const auto m=observedGame+0x1f6e48;std::uintptr_t buckets=0,entry=0;std::uint32_t mask=0;
    if(!memoryRead(m+0x18,&buckets,8)||!buckets||!memoryRead(m+0x28,&mask,4)||mask>0xfffff||(mask&(mask+1)))return 0;
    if(!memoryRead(buckets+static_cast<std::uint64_t>(mask&handle)*8,&entry,8))return 0;
    for(unsigned hops=0;entry&&hops<1024;++hops){std::int32_t key=0;std::uintptr_t value=0,next=0;
        if(!memoryRead(entry,&key,4)||!memoryRead(entry+8,&value,8)||!memoryRead(entry+0x10,&next,8))return 0;
        if(static_cast<std::int32_t>(handle)<=key)return key==static_cast<std::int32_t>(handle)?value:0;
        entry=next;}
    return 0;
}
// Owning entity name of a node in a top-level instance (nested graphs are refused).
bool kisculeNodeEntity(std::uintptr_t node,std::string& entity){
    std::uintptr_t instance=0,actor=0,name=0;std::uint32_t handle=0;
    if(!memoryRead(node+0x20,&instance,8)||!instance||!memoryRead(instance+0x60,&handle,4))return false;
    if(kisculeInstanceFor(handle)!=instance){++kisculeMirror.nested;return false;}
    actor=worldIdentityEntity(memoryRead,observedGame,handle);
    return actor&&memoryRead(actor+0x20,&name,8)&&worldIdentityString(memoryRead,name,entity)&&!entity.empty()&&entity.size()<96;
}
std::uintptr_t kisculeNodeIn(std::uintptr_t instance,std::uint32_t id){
    std::uintptr_t nodes=0;std::uint32_t count=0;
    if(!memoryRead(instance+0x20,&nodes,8)||!nodes||!memoryRead(instance+0x28,&count,4)||(count&0xffffff)>65536)return 0;
    for(std::uint32_t i=0;i<(count&0xffffff);++i){std::uintptr_t n=0,decl=0;std::uint32_t nid=0;
        if(memoryRead(nodes+i*8ull,&n,8)&&n&&memoryRead(n+0x28,&decl,8)&&decl&&memoryRead(decl+8,&nid,4)&&nid==id)return n;}
    return 0;
}
std::uintptr_t kisculeInputDescriptor(std::uintptr_t node,std::uint32_t input){
    std::uintptr_t decl=0,inputs=0;std::uint32_t count=0;
    if(!memoryRead(node+0x28,&decl,8)||!decl||!memoryRead(decl+0x40,&inputs,8)||!inputs||!memoryRead(decl+0x48,&count,4)||(count&0xffffff)>256)return 0;
    for(std::uint32_t i=0;i<(count&0xffffff);++i){std::uint32_t id=0;std::uintptr_t desc=0;
        if(memoryRead(inputs+i*0x48ull+8,&id,4)&&id==input&&memoryRead(inputs+i*0x48ull+0x20,&desc,8))return desc;}
    return 0;
}
bool kisculeCodePointer(std::uintptr_t p){const auto base=reinterpret_cast<std::uintptr_t>(gameImage);return p>base+0x1000&&p<base+0x1a00000;}
// Caller holds kisculeLock. A new node instance means a new world: counts restart.
KisculeActionCount& kisculeCount(const std::string& entity,std::uint32_t node,std::uint32_t input,std::uint32_t cls,std::uintptr_t instanceNode){
    for(auto& c:kisculeMirror.counts)if(c.node==node&&c.input==input&&c.cls==cls&&c.entity==entity){
        if(c.instanceNode!=instanceNode){c.instanceNode=instanceNode;c.requests=c.executed=0;}
        return c;}
    if(kisculeMirror.counts.size()>=8192)kisculeMirror.counts.erase(kisculeMirror.counts.begin());
    KisculeActionCount c{};c.entity=entity;c.node=node;c.input=input;c.cls=cls;c.instanceNode=instanceNode;kisculeMirror.counts.push_back(c);return kisculeMirror.counts.back();
}
char kisculeHandlerSkipped(void*){return 1;}
// 1214jj (#187, lab diagnostic): with coop_kiscule_trace on, every dispatched signal (entity, node, input, node vtable
// RVA) is kept, the first 8192 after the trace starts (1214jk; 1024 filled before the lever press in mission-lever-v1859b), in coop-status.json.kiscule-trace.json.
struct KisculeTraceEntry{std::uint64_t tick=0;std::uint32_t node=0,input=0,vt=0;char entity[96]{};};
bool kisculeTraceOn=false;std::array<KisculeTraceEntry,8192> kisculeTrace{};unsigned kisculeTraceCount=0;std::uint64_t kisculeTraceTotal=0,kisculeTraceStarted=0;
void traceKisculeSignal(const unsigned char* signal){
    const auto node=*reinterpret_cast<const std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(signal)+0x18);if(!node)return;
    KisculeTraceEntry e{};e.tick=GetTickCount64();std::memcpy(&e.node,signal+0xc,4);std::memcpy(&e.input,signal+0x10,4);
    std::uintptr_t vt=0;const auto base=reinterpret_cast<std::uintptr_t>(gameImage);if(memoryRead(node,&vt,8)&&vt>base)e.vt=static_cast<std::uint32_t>(vt-base);
    std::string entity;if(kisculeNodeEntity(node,entity))std::snprintf(e.entity,sizeof(e.entity),"%s",entity.c_str());
    AcquireSRWLockExclusive(&kisculeLock);if(kisculeTraceCount<kisculeTrace.size())kisculeTrace[kisculeTraceCount++]=e;++kisculeTraceTotal;ReleaseSRWLockExclusive(&kisculeLock);
}
// Stage 8.4.1 (#237, lab diagnostic): with coop_kiscule_census on, every dispatched signal is counted by node class
// (node vtable RVA) for the whole session, written to <status>.kiscule-census.json with the trace files. It shows which
// script node classes run in each game; the partner must run none that write the shared world.
bool kisculeCensusOn=false;std::unordered_map<std::uint32_t,std::uint64_t> kisculeCensus;std::uint64_t kisculeCensusTotal=0,kisculeCensusStarted=0;
// Dry run of the partner policy (kiscule_policy.h): signals of World and LocalPlayer classes this game ran on its own
// (not as a replay of the peer's run). On the partner both must reach zero once area 4.1 is enforced.
std::uint64_t kisculeOwnWorld=0,kisculeOwnLocalPlayer=0,kisculeReplayed=0;std::unordered_map<std::uint32_t,std::uint64_t> kisculeOwnByClass;
void countKisculeSignal(const unsigned char* signal){
    const auto node=*reinterpret_cast<const std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(signal)+0x18);if(!node)return;
    std::uintptr_t vt=0;const auto base=reinterpret_cast<std::uintptr_t>(gameImage);if(!memoryRead(node,&vt,8)||vt<=base)return;
    const auto rva=static_cast<std::uint32_t>(vt-base);const int policy=kisculePolicyIndex(rva);
    AcquireSRWLockExclusive(&kisculeLock);++kisculeCensus[rva];++kisculeCensusTotal;
    if(policy>=0){if(applyingKiscule)++kisculeReplayed;else{++kisculeOwnByClass[rva];if(kKisculePolicy[policy].policy==KisculePolicy::World)++kisculeOwnWorld;else ++kisculeOwnLocalPlayer;}}
    ReleaseSRWLockExclusive(&kisculeLock);
}
unsigned char* kisculeDispatchWrapper(void* self,unsigned char* out,unsigned char* signal){
    if(kisculeTraceOn)traceKisculeSignal(signal);if(kisculeCensusOn)countKisculeSignal(signal);
    if(!kisculeMirrorActive()||applyingKiscule)return realKisculeDispatch(self,out,signal);
    // Hot path: the native dispatcher dereferences the same pointers next.
    const auto s=reinterpret_cast<std::uintptr_t>(signal);const auto node=*reinterpret_cast<const std::uintptr_t*>(s+0x18);
    if(!node)return realKisculeDispatch(self,out,signal);
    const int cls=kisculeClassOfVtable(*reinterpret_cast<const std::uintptr_t*>(node));if(cls<0)return realKisculeDispatch(self,out,signal);
    std::uint32_t nodeId=0,input=0;std::memcpy(&nodeId,signal+0xc,4);std::memcpy(&input,signal+0x10,4);
    std::string entity;bool skip=false;
    AcquireSRWLockExclusive(&kisculeLock);
    kisculeMirror.dispatchThread=GetCurrentThreadId();
    // 1214bj: an action during a load or an intro may run before the map id is
    // known; it is counted now and sent once the map id is (service).
    if(!kisculeNodeEntity(node,entity)){++kisculeMirror.identity;kisculeEvent("unidentified",cls,entity,nodeId,input,0);}
    else if(cls<kKisculeHostStoryBase&&!kisculeMirrorClasses[cls].mirrored){++kisculeMirror.personal;kisculeEvent("personal",cls,entity,nodeId,input,0);}
    else if(std::string item;cls==0&&kisculePlayerScopedItem(node,item)){++kisculeMirror.personal;kisculeEvent("personal",cls,entity,nodeId,input,0);}
    else{auto& c=kisculeCount(entity,nodeId,input,cls,node);++c.requests;
        if(cls>=kKisculeHostStoryBase&&localRole==1&&kisculeHostStoryTwinNoBlock)++kisculeMirror.hostStoryLeaked; // the twin: a World node runs on the partner's own
        // A run the host already made (its count arrived first) runs here, in the partner's own flow: applied out of
        // flow before the flow reached it, Meagan's talk setup left the partner's boat scene stalled
        // (saferoom-trace-v2028a; hoststory-lead-dunwall-v2027a reached the node first and passed).
        if(cls>=kKisculeHostStoryBase&&localRole==1&&!kisculeHostStoryTwinNoBlock&&c.executed<c.requests&&c.requests<=c.hostCount){
            ++c.executed;++kisculeMirror.hostStoryInFlow;kisculeEvent("in_flow",cls,entity,nodeId,input,c.executed);}
        else if(cls>=kKisculeHostStoryBase&&localRole==1&&!kisculeHostStoryTwinNoBlock){skip=true;++kisculeMirror.hostStoryBlocked;kisculeEvent("host_story_blocked",cls,entity,nodeId,input,c.executed);
            // A request goes out at once, whatever state the partner's game is in: the host runs it only in its own plain
            // gameplay. Ramsey's ring task nodes run in the instant the loot scene ends, while the partner is still in its
            // cinematic state (onlytask-v2022e), and the task that ends Dunwall runs inside the partner's ending
            // cinematic: held until plain gameplay, it never went out (hoststory-lead-dunwall-v2025a).
            if(kisculeHostStoryAsk&&kisculeHostStoryAskable(cls)&&c.requests>c.executed&&c.requests>c.asked&&kisculePartnerScope()){c.asked=c.requests;++kisculeMirror.hostStoryAsked;
                KisculeActionPacket p{};p.role=1;p.classIndex=static_cast<std::uint32_t>(cls);p.mapId=outgoing.mapId;p.node=nodeId;p.input=input;p.count=c.requests;p.reserved=kKisculePartnerFirst;
                std::snprintf(p.entity,sizeof(p.entity),"%s",entity.c_str());
                if(kisculeMirror.outgoingQueue.size()<1024)kisculeMirror.outgoingQueue.push_back(p);else ++kisculeMirror.dropped;}}
        else if(c.requests<=c.executed){skip=true;++kisculeMirror.suppressed;kisculeEvent("suppressed",cls,entity,nodeId,input,c.executed);}
        else{++c.executed;++kisculeMirror.local;kisculeEvent("local",cls,entity,nodeId,input,c.executed);
            const bool partnerFirst=localRole==1&&c.executed==1&&kisculePartnerScope(); // 1214jc (#183)
            if(localRole==0||partnerFirst){KisculeActionPacket p{};p.role=localRole;p.classIndex=static_cast<std::uint32_t>(cls);p.mapId=outgoing.mapId;p.node=nodeId;p.input=input;p.count=c.executed;
            if(partnerFirst){p.reserved=kKisculePartnerFirst;kisculeEvent("partner_first",cls,entity,nodeId,input,1);}
            std::snprintf(p.entity,sizeof(p.entity),"%s",entity.c_str());
            if(kisculeMirror.outgoingQueue.size()<1024)kisculeMirror.outgoingQueue.push_back(p);else ++kisculeMirror.dropped;}}}
    ReleaseSRWLockExclusive(&kisculeLock);
    if(!skip)return realKisculeDispatch(self,out,signal);
    // The mirror already ran this handler here: dispatch with the handler
    // stubbed so the node's outputs still fire in this world.
    const auto desc=kisculeInputDescriptor(node,input);MEMORY_BASIC_INFORMATION mbi{};
    if(!desc||!VirtualQuery(reinterpret_cast<void*>(desc),&mbi,sizeof(mbi))||!(mbi.Protect&(PAGE_READWRITE|PAGE_EXECUTE_READWRITE))){
        AcquireSRWLockExclusive(&kisculeLock);++kisculeMirror.suppressFallback;ReleaseSRWLockExclusive(&kisculeLock);return realKisculeDispatch(self,out,signal);}
    std::uintptr_t h0=0,h1=0;memoryRead(desc+0x30,&h0,8);memoryRead(desc+0x38,&h1,8);
    const std::uintptr_t stub=reinterpret_cast<std::uintptr_t>(&kisculeHandlerSkipped),none=0;
    std::memcpy(reinterpret_cast<void*>(desc+0x30),&stub,8);std::memcpy(reinterpret_cast<void*>(desc+0x38),&none,8);
    auto* result=realKisculeDispatch(self,out,signal);
    std::memcpy(reinterpret_cast<void*>(desc+0x30),&h0,8);std::memcpy(reinterpret_cast<void*>(desc+0x38),&h1,8);
    return result;
}
bool installKisculeDispatchObserver(){
    auto* target=gameImage+0x7c0e30;if(std::memcmp(target,kisculeDispatchPrefix,sizeof(kisculeDispatchPrefix)))return false;
    // The first 15 bytes are four complete instructions with no branches or
    // RIP-relative operands; the original continues at entry+15.
    auto* trampoline=static_cast<unsigned char*>(VirtualAlloc(nullptr,64,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));if(!trampoline)return false;
    std::memcpy(trampoline,kisculeDispatchPrefix,15);unsigned char jump[15]{0xff,0x25,0,0,0,0};jump[14]=0x90;
    void* resume=target+15;std::memcpy(jump+6,&resume,8);std::memcpy(trampoline+15,jump,14);
    DWORD old=0,unused=0;if(!VirtualProtect(trampoline,64,PAGE_EXECUTE_READ,&old)){VirtualFree(trampoline,0,MEM_RELEASE);return false;}
    FlushInstructionCache(GetCurrentProcess(),trampoline,64);void* wrapper=reinterpret_cast<void*>(&kisculeDispatchWrapper);std::memcpy(jump+6,&wrapper,8);
    if(!VirtualProtect(target,15,PAGE_EXECUTE_READWRITE,&old)){VirtualFree(trampoline,0,MEM_RELEASE);return false;}
    realKisculeDispatch=reinterpret_cast<NativeKisculeDispatch>(trampoline);std::memcpy(target,jump,15);FlushInstructionCache(GetCurrentProcess(),target,15);
    return VirtualProtect(target,15,old,&unused)!=0;
}
void receiveKisculeAction(const KisculeActionPacket& p){
    AcquireSRWLockExclusive(&kisculeLock);auto& k=kisculeMirror;++k.received;
    const bool hostCount=p.role==0&&localRole==1,partnerFirst=p.role==1&&localRole==0&&(p.count==1||p.classIndex>=static_cast<std::uint32_t>(kKisculeHostStoryBase))&&p.reserved==kKisculePartnerFirst&&kisculePartnerScope(); // 1214jc
    if(!kisculeMirrorActive()||p.magic!=0x414B3244||p.version!=1||p.size!=sizeof(p)||!(hostCount||partnerFirst)||p.session!=remote.session||
       p.sequence<=k.peerSequence||!kisculeClassIndexMirrored(p.classIndex)||!p.count||!p.mapId||!std::memchr(p.entity,0,sizeof(p.entity))||!p.entity[0]){++k.refused;k.result="refused";}
    else{k.peerSequence=p.sequence;if(k.pending.size()<1024)k.pending.push_back({p,GetTickCount64()});else ++k.dropped;k.result="received";}
    ReleaseSRWLockExclusive(&kisculeLock);
}
// The named entity's top-level instance in this world.
std::uintptr_t kisculeInstanceByName(const char* name){
    if(!observedGame)return 0;
    for(unsigned slot=0;slot<16384;++slot){std::uintptr_t actor=0,p=0;std::uint32_t handle=0;
        if(!memoryRead(observedGame+0x3130b0+slot*8,&actor,8)||!actor||!memoryRead(actor+0x20,&p,8))continue;
        std::string n;if(!worldIdentityString(memoryRead,p,n)||n!=name||!memoryRead(actor+0xa8,&handle,4))continue;
        if(const auto instance=kisculeInstanceFor(handle))return instance;}
    return 0;
}
void serviceKisculeMirror(std::uint64_t tick){
    if(GetCurrentThreadId()!=engineThread.load())return;
    auto& k=kisculeMirror;
    if(!kisculeMirrorActive()){AcquireSRWLockExclusive(&kisculeLock);k.outgoingQueue.clear();k.pending.clear();ReleaseSRWLockExclusive(&kisculeLock);return;}
    // The partner follows the host's setting in the product and under the lab's product controller (coop_product_auto),
    // which VM takes run (campaign-dunwall-vm-hoststory-v2030a: the VM partner stayed off); the host's own setting comes
    // from d2coop.cfg only in the product, so lab commands still decide it.
    if((productMode||productAutoArm)&&localRole==1||productMode&&localRole==0){const bool want=localRole==0?productHostStory:coopControl.helloOk&&coopControl.peerHostStory;
        if(want!=kisculeHostStory){AcquireSRWLockExclusive(&kisculeLock);kisculeHostStory=want;ReleaseSRWLockExclusive(&kisculeLock);}}
    std::vector<KisculeActionPacket> send;std::vector<KisculePending> work;
    AcquireSRWLockExclusive(&kisculeLock);send.swap(k.outgoingQueue);work.swap(k.pending);
    ReleaseSRWLockExclusive(&kisculeLock);
    const bool connected=sockets&&connection&&connectionState==k_ESteamNetworkingConnectionState_Connected;
    std::vector<KisculeActionPacket> later;
    for(auto& p:send){
        if(!p.mapId)p.mapId=outgoing.mapId;
        if(!p.mapId||!connected){if(later.size()<1024)later.push_back(p);else ++k.dropped;continue;}
        p.session=outgoing.session;p.sequence=++k.sequence;
        if(coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)==k_EResultOK){++k.sent;k.result="sent";}else ++k.dropped;}
    if(!later.empty()){AcquireSRWLockExclusive(&kisculeLock);for(auto& p:later)if(k.outgoingQueue.size()<1024)k.outgoingQueue.push_back(p);ReleaseSRWLockExclusive(&kisculeLock);}
    std::vector<KisculePending> keep;
    const bool loading=outgoing.gameLogicState==15||outgoing.pendingGameLogicState==15||outgoing.gameLogicState==0||!observedPlayer;
    for(auto& w:work){const auto& p=w.packet;
        if(tick-w.at>60000){++k.expired;k.result="expired";continue;}
        if(p.role==1&&p.classIndex>=static_cast<std::uint32_t>(kKisculeHostStoryBase)&&!kisculePlainGameplay()){keep.push_back(w);continue;} // stage 8.4.1: a partner's request waits for plain gameplay
        // A replay never runs on the mission result screen (logic 11), where the world is being torn down: the VM partner's game
        // vanished right after eight host inventory replays there (campaign-curator-vm-hoststory-v2031a and v2031b). It waits;
        // on the next map it lapses as other_map.
        if(outgoing.gameLogicState==11||outgoing.pendingGameLogicState==11){++k.resultScreenHeld;keep.push_back(w);continue;}
        if(loading||p.mapId!=outgoing.mapId){if(!loading&&outgoing.gameLogicState==1&&tick-w.at>20000){++k.otherMap;k.result="other_map";continue;}keep.push_back(w);continue;}
        if(k.dispatchThread&&k.dispatchThread!=GetCurrentThreadId()){++k.threadMismatch;k.result="thread_mismatch";continue;}
        std::uint32_t nodeId=p.node,inputId=p.input;kisculeCharacterCounterpart(p.entity,nodeId,inputId);
        const auto instance=kisculeInstanceByName(p.entity);const auto node=instance?kisculeNodeIn(instance,nodeId):0;
        const auto desc=node?kisculeInputDescriptor(node,inputId):0;std::uintptr_t h0=0,h1=0;
        if(!node||kisculeClassOf(node)!=static_cast<int>(p.classIndex)||!desc||!memoryRead(desc+0x30,&h0,8)||!memoryRead(desc+0x38,&h1,8)||
           !(h1?kisculeCodePointer(h1):kisculeCodePointer(h0))){
            if(tick-w.at<10000){keep.push_back(w);continue;}
            AcquireSRWLockExclusive(&kisculeLock);++k.notFound;k.result="not_found";kisculeEvent("not_found",p.classIndex,p.entity,p.node,p.input,p.count);ReleaseSRWLockExclusive(&kisculeLock);continue;}
        std::uint32_t ranTo=0;
        for(unsigned guard=0;guard<4;++guard){
            AcquireSRWLockExclusive(&kisculeLock);auto& c=kisculeCount(p.entity,nodeId,inputId,p.classIndex,node);
            const bool hostStoryRun=p.role==0&&localRole==1&&p.classIndex>=static_cast<std::uint32_t>(kKisculeHostStoryBase);
            if(hostStoryRun&&p.count>c.hostCount)c.hostCount=p.count;
            // Up to 750 ms for the partner's own flow to reach the node and run it in flow; then it is applied here.
            if(hostStoryRun&&c.requests<p.count&&c.executed<p.count&&tick-w.at<750){ReleaseSRWLockExclusive(&kisculeLock);keep.push_back(w);break;}
            const bool run=c.executed<p.count;if(run){++c.executed;ranTo=c.executed;++k.applied;k.result="applied";kisculeEvent(p.role==1?(nodeId==p.node?"applied_partner":"applied_partner_counterpart"):(nodeId==p.node?"applied":"applied_counterpart"),p.classIndex,p.entity,nodeId,inputId,c.executed);}
            ReleaseSRWLockExclusive(&kisculeLock);if(!run)break;
            alignas(16) unsigned char signal[32]{};std::memcpy(signal+0xc,&nodeId,4);std::memcpy(signal+0x10,&inputId,4);std::memcpy(signal+0x18,&node,8);
            applyingKiscule=true;
            if(h1)reinterpret_cast<NativeKisculeHandlerEx>(h1)(reinterpret_cast<void*>(node),reinterpret_cast<void*>(desc),signal);
            else reinterpret_cast<NativeKisculeHandler>(h0)(reinterpret_cast<void*>(node));
            applyingKiscule=false;}
        // A host-story request the host ran goes back to the partner as a host run, so the partner's world gets it
        // too: the partner stubbed its own run, and the host calls the handler directly, which queues nothing
        // (hoststory-lead-dunwall-v2026b: the safe-room doors opened only in the host's world).
        if(ranTo&&p.role==1&&localRole==0&&p.classIndex>=static_cast<std::uint32_t>(kKisculeHostStoryBase)){
            KisculeActionPacket back{};back.role=0;back.classIndex=p.classIndex;back.mapId=outgoing.mapId;back.node=nodeId;back.input=inputId;back.count=ranTo;
            std::memcpy(back.entity,p.entity,sizeof(back.entity));
            AcquireSRWLockExclusive(&kisculeLock);if(k.outgoingQueue.size()<1024)k.outgoingQueue.push_back(back);else ++k.dropped;kisculeEvent("sent_back",p.classIndex,p.entity,nodeId,inputId,ranTo);ReleaseSRWLockExclusive(&kisculeLock);}
    }
    if(!keep.empty()){AcquireSRWLockExclusive(&kisculeLock);for(auto& w:keep)if(k.pending.size()<1024)k.pending.push_back(w);ReleaseSRWLockExclusive(&kisculeLock);}
}
// 1214jl (#187, lab diagnostic): entity events posted while the trace is on. Each post variant is hooked with a jump to
// a pass-through wrapper (the original always runs); the prologue copies are complete instructions without branches or
// RIP-relative operands: 0x140883f80 (entity, event, ...) 20 bytes, 0x140884920 (entity, out, event, arg) 15 bytes,
// 0x1408848a0 (entity, out, event, arg, arg) 16 bytes.
struct EventTraceEntry{std::uint64_t tick=0;std::uint32_t variant=0;char entity[96]{};char event[48]{};};
std::array<EventTraceEntry,4096> eventTrace{};unsigned eventTraceCount=0;std::uint64_t eventTraceTotal=0;
void traceEntityEvent(std::uint32_t variant,void* entity,void* event){
    EventTraceEntry e{};e.tick=GetTickCount64();e.variant=variant;std::uintptr_t name=0;std::string text;
    if(entity&&memoryRead(reinterpret_cast<std::uintptr_t>(entity)+0x20,&name,8)&&worldIdentityString(memoryRead,name,text))std::snprintf(e.entity,sizeof(e.entity),"%s",text.c_str());
    text.clear();if(event&&memoryRead(reinterpret_cast<std::uintptr_t>(event),&name,8)&&worldIdentityString(memoryRead,name,text))std::snprintf(e.event,sizeof(e.event),"%s",text.c_str());
    AcquireSRWLockExclusive(&kisculeLock);if(eventTraceCount<eventTrace.size())eventTrace[eventTraceCount++]=e;++eventTraceTotal;ReleaseSRWLockExclusive(&kisculeLock);
}
using NativeEventPost=void*(*)(void*,void*,void*,void*,void*);
NativeEventPost realEventPost883=nullptr,realEventPost884=nullptr;
void* eventPost883(void* a,void* b,void* c,void* d,void* e){if(kisculeTraceOn)traceEntityEvent(0,a,b);return realEventPost883(a,b,c,d,e);}
// 1214jn (#187): every "use" event (descriptor 0x1439aa130) also goes to the use mirror's observer.
void* eventPost884(void* a,void* b,void* c,void* d,void* e){if(kisculeTraceOn)traceEntityEvent(1,a,c);if(c==reinterpret_cast<void*>(gameImage+0x39aa130))observeUseEvent(a,d);return realEventPost884(a,b,c,d,e);}
// 1214jq (#189): the third post 0x1408848a0 (pickedUp) is not hooked; the shared pickup API requires its bytes verbatim.
struct EntityEventHookSite {std::uintptr_t rva;unsigned length;};
constexpr EntityEventHookSite kEntityEventHookSites[]={{0x883f80,20},{0x884920,15}};
constexpr bool entityEventHooksSpareCoinApi(){
    for(const auto& h:kEntityEventHookSites)for(auto c:kCoinVerbatimRvas)if(h.rva<c+32&&c<h.rva+h.length)return false;
    return true;
}
static_assert(entityEventHooksSpareCoinApi(),"an entity event hook would patch code the shared pickup API requires verbatim");
bool hookEventPost(std::uintptr_t rva,const unsigned char* prefix,unsigned length,void* wrapper,NativeEventPost& real){
    auto* target=gameImage+rva;if(std::memcmp(target,prefix,length))return false;
    auto* trampoline=static_cast<unsigned char*>(VirtualAlloc(nullptr,64,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));if(!trampoline)return false;
    std::memcpy(trampoline,prefix,length);unsigned char jump[14]{0xff,0x25,0,0,0,0};void* resume=target+length;std::memcpy(jump+6,&resume,8);std::memcpy(trampoline+length,jump,14);
    DWORD old=0,unused=0;if(!VirtualProtect(trampoline,64,PAGE_EXECUTE_READ,&old)){VirtualFree(trampoline,0,MEM_RELEASE);return false;}
    FlushInstructionCache(GetCurrentProcess(),trampoline,64);
    unsigned char patch[20];std::memset(patch,0x90,sizeof(patch));std::memcpy(patch,jump,6);std::memcpy(patch+6,&wrapper,8);
    if(!VirtualProtect(target,length,PAGE_EXECUTE_READWRITE,&old)){VirtualFree(trampoline,0,MEM_RELEASE);return false;}
    real=reinterpret_cast<NativeEventPost>(trampoline);std::memcpy(target,patch,length);FlushInstructionCache(GetCurrentProcess(),target,length);
    return VirtualProtect(target,length,old,&unused)!=0;
}
bool installEntityEventTrace(){
    const unsigned char p883[20]={0x48,0x8b,0xc4,0x4c,0x89,0x48,0x20,0x57,0x48,0x83,0xec,0x40,0x48,0xc7,0x40,0xe8,0xfe,0xff,0xff,0xff};
    const unsigned char p884[15]={0x4c,0x89,0x4c,0x24,0x20,0x48,0x89,0x54,0x24,0x10,0x57,0x48,0x83,0xec,0x40};
    return hookEventPost(0x883f80,p883,20,reinterpret_cast<void*>(&eventPost883),realEventPost883)&&
        hookEventPost(0x884920,p884,15,reinterpret_cast<void*>(&eventPost884),realEventPost884);
}
void writeKisculeCensus(){
    if(!kisculeCensusStarted)return;
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.kiscule-census.json",statusPath)>=static_cast<int>(sizeof(path))||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=static_cast<int>(sizeof(temp)))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    AcquireSRWLockShared(&kisculeLock);
    fprintf(f,"{\"on\":%s,\"role\":%u,\"started\":%llu,\"total\":%llu,\"classes\":{",kisculeCensusOn?"true":"false",localRole,kisculeCensusStarted,kisculeCensusTotal);
    bool first=true;for(const auto& c:kisculeCensus){fprintf(f,"%s\"%u\":%llu",first?"":",",c.first,c.second);first=false;}
    fprintf(f,"},\"ownWorld\":%llu,\"ownLocalPlayer\":%llu,\"replayed\":%llu,\"own\":{",kisculeOwnWorld,kisculeOwnLocalPlayer,kisculeReplayed);
    first=true;for(const auto& c:kisculeOwnByClass){const int i=kisculePolicyIndex(c.first);fprintf(f,"%s\"%s\":%llu",first?"":",",i>=0?kKisculePolicy[i].name:"?",c.second);first=false;}
    fprintf(f,"}}\n");
    ReleaseSRWLockShared(&kisculeLock);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
void writeKisculeTrace(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.kiscule-trace.json",statusPath)>=static_cast<int>(sizeof(path))||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=static_cast<int>(sizeof(temp)))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    AcquireSRWLockShared(&kisculeLock);
    fprintf(f,"{\"on\":%s,\"started\":%llu,\"total\":%llu,\"kept\":%u,\"entries\":[",kisculeTraceOn?"true":"false",kisculeTraceStarted,kisculeTraceTotal,kisculeTraceCount);
    for(unsigned i=0;i<kisculeTraceCount;++i){const auto& e=kisculeTrace[i];
        fprintf(f,"%s{\"tick\":%llu,\"entity\":\"%s\",\"node\":%u,\"input\":%u,\"vtRva\":%u}",i?",":"",e.tick,e.entity,e.node,e.input,e.vt);}
    fprintf(f,"],\"eventsTotal\":%llu,\"eventsKept\":%u,\"events\":[",eventTraceTotal,eventTraceCount);
    for(unsigned i=0;i<eventTraceCount;++i){const auto& e=eventTrace[i];
        fprintf(f,"%s{\"tick\":%llu,\"variant\":%u,\"entity\":\"%s\",\"event\":\"%s\"}",i?",":"",e.tick,e.variant,e.entity,e.event);}
    fprintf(f,"]}\n");
    ReleaseSRWLockShared(&kisculeLock);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
void writeKisculeMirror(){
    writeKisculeTrace();writeKisculeCensus();
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.kiscule.json",statusPath)>=static_cast<int>(sizeof(path))||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=static_cast<int>(sizeof(temp)))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    AcquireSRWLockShared(&kisculeLock);const auto& k=kisculeMirror;
    fprintf(f,"{\"hostStory\":%s,\"hostStoryBlocked\":%llu,\"hostStoryAsked\":%llu,\"hostStoryLeaked\":%llu,\"hostStoryInFlow\":%llu,\"resultScreenHeld\":%llu,",kisculeHostStory?"true":"false",k.hostStoryBlocked,k.hostStoryAsked,k.hostStoryLeaked,k.hostStoryInFlow,k.resultScreenHeld);
    fprintf(f,"\"enabled\":%s,\"result\":\"%s\",\"role\":%u,\"dispatchThread\":%u,\"engineThread\":%lu,\"local\":%llu,\"personal\":%llu,\"sent\":%llu,\"received\":%llu,\"applied\":%llu,\"suppressed\":%llu,\"suppressFallback\":%llu,\"refused\":%llu,\"notFound\":%llu,\"nested\":%llu,\"unidentified\":%llu,\"expired\":%llu,\"otherMap\":%llu,\"dropped\":%llu,\"threadMismatch\":%llu,\"pending\":%zu,\"counts\":%zu,\"events\":[",
        kisculeMirrorActive()?"true":"false",k.result,localRole,k.dispatchThread,engineThread.load(),k.local,k.personal,k.sent,k.received,k.applied,k.suppressed,k.suppressFallback,k.refused,k.notFound,k.nested,k.identity,k.expired,k.otherMap,k.dropped,k.threadMismatch,k.pending.size(),k.counts.size());
    const auto first=k.eventCount>256?k.eventCount-256:0;
    for(auto i=first;i<k.eventCount;++i){const auto& e=k.events[i%256];
        fprintf(f,"%s{\"tick\":%llu,\"what\":\"%s\",\"class\":\"%s\",\"entity\":\"%s\",\"node\":%u,\"input\":%u,\"count\":%u}",i==first?"":",",e.tick,e.what,e.cls<static_cast<std::uint32_t>(kisculeMirrorClassCount)?kisculeMirrorClasses[e.cls].name:e.cls>=static_cast<std::uint32_t>(kKisculeHostStoryBase)&&e.cls<kKisculeHostStoryBase+kKisculePolicyCount?kKisculePolicy[e.cls-kKisculeHostStoryBase].name:"?",e.entity,e.node,e.input,e.count);}
    fprintf(f,"]}\n");
    ReleaseSRWLockShared(&kisculeLock);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
