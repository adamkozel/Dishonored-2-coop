#pragma once
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
struct KisculeActionCount{std::string entity;std::uint32_t node=0,input=0,cls=0;std::uintptr_t instanceNode=0;std::uint32_t requests=0,executed=0;};
struct KisculeActionEvent{std::uint64_t tick=0;std::uint32_t cls=0,node=0,input=0,count=0;char entity[64]{};const char* what="";};
struct KisculePending{KisculeActionPacket packet{};std::uint64_t at=0;};
struct KisculeMirror {
    std::uint64_t sequence=0,peerSequence=0,local=0,personal=0,sent=0,received=0,applied=0,suppressed=0,suppressFallback=0,refused=0,notFound=0,nested=0,identity=0,expired=0,otherMap=0,dropped=0,threadMismatch=0;
    std::uint32_t dispatchThread=0;const char* result="idle";
    std::vector<KisculeActionCount> counts;std::vector<KisculeActionPacket> outgoingQueue;std::vector<KisculePending> pending;
    KisculeActionEvent events[24]{};std::uint64_t eventCount=0;
};
KisculeMirror kisculeMirror{};SRWLOCK kisculeLock=SRWLOCK_INIT;bool kisculeMirrorEnabled=false;bool applyingKiscule=false;
bool kisculeMirrorActive(){return kisculeMirrorEnabled||productMode;}
// Caller holds kisculeLock.
void kisculeEvent(const char* what,std::uint32_t cls,const std::string& entity,std::uint32_t node,std::uint32_t input,std::uint32_t count){
    auto& e=kisculeMirror.events[kisculeMirror.eventCount++%24];e={};e.tick=GetTickCount64();e.what=what;e.cls=cls;e.node=node;e.input=input;e.count=count;
    std::snprintf(e.entity,sizeof(e.entity),"%s",entity.c_str());
}
int kisculeClassOfVtable(std::uintptr_t vt){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    for(int i=0;i<kisculeMirrorClassCount;++i)if(vt==base+kisculeMirrorClasses[i].vtable)return i;
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
    if(kisculeMirror.counts.size()>=512)kisculeMirror.counts.erase(kisculeMirror.counts.begin());
    KisculeActionCount c{};c.entity=entity;c.node=node;c.input=input;c.cls=cls;c.instanceNode=instanceNode;kisculeMirror.counts.push_back(c);return kisculeMirror.counts.back();
}
char kisculeHandlerSkipped(void*){return 1;}
unsigned char* kisculeDispatchWrapper(void* self,unsigned char* out,unsigned char* signal){
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
    else if(!kisculeMirrorClasses[cls].mirrored){++kisculeMirror.personal;kisculeEvent("personal",cls,entity,nodeId,input,0);}
    else if(std::string item;cls==0&&kisculePlayerScopedItem(node,item)){++kisculeMirror.personal;kisculeEvent("personal",cls,entity,nodeId,input,0);}
    else{auto& c=kisculeCount(entity,nodeId,input,cls,node);++c.requests;
        if(c.requests<=c.executed){skip=true;++kisculeMirror.suppressed;kisculeEvent("suppressed",cls,entity,nodeId,input,c.executed);}
        else{++c.executed;++kisculeMirror.local;kisculeEvent("local",cls,entity,nodeId,input,c.executed);
            if(localRole==0){KisculeActionPacket p{};p.role=localRole;p.classIndex=static_cast<std::uint32_t>(cls);p.mapId=outgoing.mapId;p.node=nodeId;p.input=input;p.count=c.executed;
            std::snprintf(p.entity,sizeof(p.entity),"%s",entity.c_str());
            if(kisculeMirror.outgoingQueue.size()<128)kisculeMirror.outgoingQueue.push_back(p);else ++kisculeMirror.dropped;}}}
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
    if(!kisculeMirrorActive()||p.magic!=0x414B3244||p.version!=1||p.size!=sizeof(p)||p.role!=0||localRole!=1||p.session!=remote.session||
       p.sequence<=k.peerSequence||p.classIndex>=static_cast<std::uint32_t>(kisculeMirrorClassCount)||!kisculeMirrorClasses[p.classIndex].mirrored||!p.count||!p.mapId||!std::memchr(p.entity,0,sizeof(p.entity))||!p.entity[0]){++k.refused;k.result="refused";}
    else{k.peerSequence=p.sequence;if(k.pending.size()<128)k.pending.push_back({p,GetTickCount64()});else ++k.dropped;k.result="received";}
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
    std::vector<KisculeActionPacket> send;std::vector<KisculePending> work;
    AcquireSRWLockExclusive(&kisculeLock);send.swap(k.outgoingQueue);work.swap(k.pending);ReleaseSRWLockExclusive(&kisculeLock);
    const bool connected=sockets&&connection&&connectionState==k_ESteamNetworkingConnectionState_Connected;
    std::vector<KisculeActionPacket> later;
    for(auto& p:send){
        if(!p.mapId)p.mapId=outgoing.mapId;
        if(!p.mapId||!connected){if(later.size()<128)later.push_back(p);else ++k.dropped;continue;}
        p.session=outgoing.session;p.sequence=++k.sequence;
        if(coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)==k_EResultOK){++k.sent;k.result="sent";}else ++k.dropped;}
    if(!later.empty()){AcquireSRWLockExclusive(&kisculeLock);for(auto& p:later)if(k.outgoingQueue.size()<128)k.outgoingQueue.push_back(p);ReleaseSRWLockExclusive(&kisculeLock);}
    std::vector<KisculePending> keep;
    const bool loading=outgoing.gameLogicState==15||outgoing.pendingGameLogicState==15||outgoing.gameLogicState==0||!observedPlayer;
    for(auto& w:work){const auto& p=w.packet;
        if(tick-w.at>60000){++k.expired;k.result="expired";continue;}
        if(loading||p.mapId!=outgoing.mapId){if(!loading&&outgoing.gameLogicState==1&&tick-w.at>20000){++k.otherMap;k.result="other_map";continue;}keep.push_back(w);continue;}
        if(k.dispatchThread&&k.dispatchThread!=GetCurrentThreadId()){++k.threadMismatch;k.result="thread_mismatch";continue;}
        const auto instance=kisculeInstanceByName(p.entity);const auto node=instance?kisculeNodeIn(instance,p.node):0;
        const auto desc=node?kisculeInputDescriptor(node,p.input):0;std::uintptr_t h0=0,h1=0;
        if(!node||kisculeClassOf(node)!=static_cast<int>(p.classIndex)||!desc||!memoryRead(desc+0x30,&h0,8)||!memoryRead(desc+0x38,&h1,8)||
           !(h1?kisculeCodePointer(h1):kisculeCodePointer(h0))){
            if(tick-w.at<10000){keep.push_back(w);continue;}
            AcquireSRWLockExclusive(&kisculeLock);++k.notFound;k.result="not_found";kisculeEvent("not_found",p.classIndex,p.entity,p.node,p.input,p.count);ReleaseSRWLockExclusive(&kisculeLock);continue;}
        for(unsigned guard=0;guard<4;++guard){
            AcquireSRWLockExclusive(&kisculeLock);auto& c=kisculeCount(p.entity,p.node,p.input,p.classIndex,node);
            const bool run=c.executed<p.count;if(run){++c.executed;++k.applied;k.result="applied";kisculeEvent("applied",p.classIndex,p.entity,p.node,p.input,c.executed);}
            ReleaseSRWLockExclusive(&kisculeLock);if(!run)break;
            alignas(16) unsigned char signal[32]{};std::memcpy(signal+0xc,&p.node,4);std::memcpy(signal+0x10,&p.input,4);std::memcpy(signal+0x18,&node,8);
            applyingKiscule=true;
            if(h1)reinterpret_cast<NativeKisculeHandlerEx>(h1)(reinterpret_cast<void*>(node),reinterpret_cast<void*>(desc),signal);
            else reinterpret_cast<NativeKisculeHandler>(h0)(reinterpret_cast<void*>(node));
            applyingKiscule=false;}
    }
    if(!keep.empty()){AcquireSRWLockExclusive(&kisculeLock);for(auto& w:keep)if(k.pending.size()<128)k.pending.push_back(w);ReleaseSRWLockExclusive(&kisculeLock);}
}
void writeKisculeMirror(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.kiscule.json",statusPath)>=static_cast<int>(sizeof(path))||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=static_cast<int>(sizeof(temp)))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    AcquireSRWLockShared(&kisculeLock);const auto& k=kisculeMirror;
    fprintf(f,"{\"enabled\":%s,\"result\":\"%s\",\"role\":%u,\"dispatchThread\":%u,\"engineThread\":%lu,\"local\":%llu,\"personal\":%llu,\"sent\":%llu,\"received\":%llu,\"applied\":%llu,\"suppressed\":%llu,\"suppressFallback\":%llu,\"refused\":%llu,\"notFound\":%llu,\"nested\":%llu,\"unidentified\":%llu,\"expired\":%llu,\"otherMap\":%llu,\"dropped\":%llu,\"threadMismatch\":%llu,\"pending\":%zu,\"counts\":%zu,\"events\":[",
        kisculeMirrorActive()?"true":"false",k.result,localRole,k.dispatchThread,engineThread.load(),k.local,k.personal,k.sent,k.received,k.applied,k.suppressed,k.suppressFallback,k.refused,k.notFound,k.nested,k.identity,k.expired,k.otherMap,k.dropped,k.threadMismatch,k.pending.size(),k.counts.size());
    const auto first=k.eventCount>24?k.eventCount-24:0;
    for(auto i=first;i<k.eventCount;++i){const auto& e=k.events[i%24];
        fprintf(f,"%s{\"tick\":%llu,\"what\":\"%s\",\"class\":\"%s\",\"entity\":\"%s\",\"node\":%u,\"input\":%u,\"count\":%u}",i==first?"":",",e.tick,e.what,e.cls<static_cast<std::uint32_t>(kisculeMirrorClassCount)?kisculeMirrorClasses[e.cls].name:"?",e.entity,e.node,e.input,e.count);}
    fprintf(f,"]}\n");
    ReleaseSRWLockShared(&kisculeLock);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
