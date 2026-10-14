#include "world_identity.h"
#include "world_protocol.h"
#include "native_ai_state.h"
#include "native_player_stance.h"
#include "native_player_states.h"
#include "native_health.h"
#include "native_faction.h"
#include "native_ai_targets.h"
#include <unordered_map>
#include <iostream>
#include <limits>
#include <cstdlib>
// Synthetic native layouts deliberately use unrelated local handles in the
// two processes. Test refusal paths before any identity can drive mutations.
struct Memory {
    std::unordered_map<std::uintptr_t,unsigned char> bytes;
    void put(std::uintptr_t p,const void* data,std::size_t n){auto b=static_cast<const unsigned char*>(data);for(std::size_t i=0;i<n;++i)bytes[p+i]=b[i];}
    void q(std::uintptr_t p,std::uint64_t n){put(p,&n,8);}void d(std::uintptr_t p,std::uint32_t n){put(p,&n,4);}
    void str(std::uintptr_t p,const char* s){put(p,s,std::strlen(s)+1);}
    bool read(std::uintptr_t p,void* data,std::size_t n){auto b=static_cast<unsigned char*>(data);for(std::size_t i=0;i<n;++i){auto it=bytes.find(p+i);if(it==bytes.end())return false;b[i]=it->second;}return true;}
};
void require(bool condition,const char* message){if(!condition){std::cerr<<"FAIL: "<<message<<'\n';std::exit(1);}}
void factionTests(){
    constexpr std::uintptr_t base=0x140000000,game=0x200000000,actor=0x300000000,stack=0x400000000,list=stack+0x1000;
    constexpr std::uintptr_t c=0x500000000,decl=c+0x1000,name=c+0x2000,path=c+0x3000,table=c+0x4000,names=c+0x5000,buckets=c+0x6000;
    constexpr std::uintptr_t node=c+0x7000,targetNode=node+0x100,members=node+0x1000,relations=node+0x2000,overrides=node+0x3000,targetActor=actor+0x1000;
    constexpr std::uint32_t target=0x10002;const auto manager=game+0x1f7bb8;
    Memory m;m.q(base+0x39a5290,game);m.q(game,base+0x1c940a8);m.d(game+0x341058,3);m.q(game+0x1f6ed0,game);m.q(game+0x1f73f0,stack);m.d(game+0x1f73f8,1);m.d(stack,1);m.d(game+0x1f7400,0);
    m.q(game+0x3130b0,actor);m.q(actor,base+0x1c5e428);m.d(actor+0xa8,1);m.q(actor+0x268,list);m.d(actor+0x270,1);m.q(list,c);
    m.q(c,base+0x1ce6268);m.q(c+8,decl);m.q(c+0x10,actor);m.d(c+0x1c,0xa);m.d(c+0x20,0);m.q(c+0x28,name);m.str(name,"Player");m.q(decl,base+0x1d1f7e8);m.q(decl+8,path);m.str(path,"factions/faction_player.cpntfaction");
    m.q(manager,table);m.d(manager+8,2);m.q(manager+0x10,names);m.d(manager+0x18,2);m.q(names,path);
    m.q(manager+0x20,buckets);m.d(manager+0x28,2);m.d(manager+0x2c,2);m.d(manager+0x30,1);m.q(buckets,targetNode);m.q(buckets+8,node);
    m.q(table+0x30,decl);m.q(table,members);m.d(table+8,1);m.d(members,1);m.q(table+0x10,relations);m.d(table+0x18,2);m.d(relations,0);m.d(relations+4,2);
    m.d(node,1);m.d(node+8,0);m.q(node+0x10,overrides);m.d(node+0x18,0);m.q(node+0x20,0);
    m.d(targetNode,target);m.d(targetNode+8,1);m.q(targetNode+0x20,0);m.q(game+0x3130b0+8,targetActor);m.d(targetActor+0xa8,target);
    NativeFaction s;const auto inspect=[&](Memory& mem){return readNativeFaction([&](auto p,auto d,auto n){return mem.read(p,d,n);},base,1,s);};
    require(inspect(m)&&s.relations.size()==2&&s.relations[1].effective==2&&!s.relations[1].overridden,"native faction registration and directional base relation");
    auto changed=m;changed.d(node+0x18,1);changed.d(overrides,target);changed.d(overrides+4,0);require(inspect(changed)&&s.relations[1].effective==0&&s.relations[1].overridden,"entity override precedes faction table");
    changed.d(overrides+4,3);require(inspect(changed)&&s.relations[1].effective==2&&s.relations[1].overridden,"undefined override uses faction relation");
    changed=m;changed.q(node+0x20,node);require(!inspect(changed),"cyclic native hash chain refused");
    changed=m;changed.d(manager+0x2c,3);require(!inspect(changed),"registration count mismatch refused");
    changed=m;changed.d(manager+0x28,65536);require(!inspect(changed),"unbounded hash allocation refused");
    changed=m;changed.d(node+8,1);require(!inspect(changed),"component and registered faction disagreement refused");
    changed=m;changed.q(c+0x10,targetActor);require(!inspect(changed),"faction component owner mismatch refused");
    changed=m;changed.d(members,target);require(!inspect(changed),"missing faction membership refused");
    changed=m;changed.d(targetActor+0xa8,target+1);require(!inspect(changed),"stale registered target refused");
    changed=m;changed.d(relations+4,4);require(!inspect(changed),"invalid relationship value refused");
    changed=m;changed.d(stack,15);require(!inspect(changed),"loading faction observation refused");
    changed=m;unsigned reads=0;require(!readNativeFaction([&](auto p,auto d,auto n){const bool ok=changed.read(p,d,n);if(p==relations+4&&++reads==1)changed.d(p,0);return ok;},base,1,s),"concurrent relationship change refused");
    std::cout<<"PASS: native faction registration, directional overrides, bounds, ownership and concurrent change\n";
}
void healthTests(){
    constexpr std::uintptr_t base=0x140000000,game=0x200000000,actor=0x300000000,stack=0x400000000,list=stack+0x1000;
    constexpr std::uintptr_t health=0x500000000,decl=0x600000000,attributes=0x700000000,table=0x800000000;
    Memory m;m.q(base+0x39a5290,game);m.q(game,base+0x1c940a8);m.d(game+0x341058,3);m.q(game+0x1f6ed0,game);
    m.q(game+0x1f73f0,stack);m.d(game+0x1f73f8,1);m.d(stack,1);m.d(game+0x1f7400,0);
    m.q(game+0x3130b0,actor);m.q(actor,base+0x1c5e428);m.d(actor+0xa8,1);m.q(actor+0x268,list);m.d(actor+0x270,2);m.d(actor+0xd1,0);
    m.q(list,health);m.q(list+8,attributes);m.q(health,base+0x1c51588);m.q(health+8,decl);m.q(health+0x10,actor);m.q(decl,base+0x1c51698);
    const auto f=[&](Memory& mem,std::uintptr_t p,float n){mem.put(p,&n,4);};f(m,health+0x20,56);f(m,decl+0x68,70);f(m,decl+0xb4,1);
    m.q(health+0x68,attributes);m.d(health+0xb0,0);m.d(health+0xb4,1);
    m.q(attributes,base+0x1ca7938);m.q(attributes+0x10,actor);m.q(attributes+0x48,table);m.d(attributes+0x50,2);
    f(m,table+8,0);f(m,table+0x24,10);f(m,table+40+8,0);f(m,table+40+0x24,0);
    NativeHealth s;const auto inspect=[&](Memory& mem){return readNativeHealth([&](auto p,auto d,auto n){return mem.read(p,d,n);},base,1,s);};
    require(inspect(m)&&s.current==56&&s.maximum==80,"native base maximum plus current health modifier");
    auto changed=m;f(changed,table+40+0x24,1);require(inspect(changed)&&s.maximum==70,"suppression attribute disables maximum modifier");
    changed=m;changed.d(actor+0xd1,0x20);require(inspect(changed)&&s.maximum==1,"owner suppression selects declaration maximum");
    changed=m;changed.d(health+0xb0,0xffffffff);require(inspect(changed)&&s.maximum==70,"missing maximum attribute contributes zero");
    changed=m;changed.q(health+0x68,0);require(inspect(changed)&&s.maximum==70,"absent attribute component uses base maximum");
    changed=m;changed.q(attributes+0x10,actor+8);require(!inspect(changed),"health attribute owner mismatch");
    changed=m;changed.d(health+0xb4,2);require(!inspect(changed),"out-of-range suppression index");
    changed=m;changed.d(health+0xb0,0xfffffffe);require(!inspect(changed),"unsupported negative attribute index");
    changed=m;changed.q(decl,base+0x1b05218);require(!inspect(changed),"wrong health declaration class");
    changed=m;changed.d(actor+0x270,3);changed.q(list+16,health);require(!inspect(changed),"duplicate health component");
    changed=m;f(changed,health+0x20,std::numeric_limits<float>::quiet_NaN());require(!inspect(changed),"nonfinite health");
    changed=m;f(changed,health+0x20,-1);require(!inspect(changed),"negative health");
    changed=m;changed.d(stack,15);require(!inspect(changed),"health observation refuses loading");
    const auto loadingHealth=[&](Memory& mem,unsigned handle=1){return readNativeHealth([&](auto p,auto d,auto n){return mem.read(p,d,n);},base,handle,s,true);};
    require(loadingHealth(changed)&&s.logic==15&&s.current==56,"explicit loading player health preserves checkpoint value");
    require(!loadingHealth(m),"loading health reader refuses gameplay");
    require(!loadingHealth(changed,0x10003),"loading health reader refuses NPC handles");
    changed.d(game+0x341058,2);require(!loadingHealth(changed),"loading health refuses incomplete world");
    changed=m;changed.d(stack,15);changed.d(game+0x1f7400,1);require(!loadingHealth(changed),"loading health refuses pending transition");
    changed=m;changed.d(stack,13);f(changed,health+0x20,0);require(inspect(changed)&&s.logic==13,"native game-over health remains observable");
    changed=m;unsigned reads=0;
    require(!readNativeHealth([&](auto p,auto d,auto n){bool ok=changed.read(p,d,n);if(p==health+0x20&&++reads==1)f(changed,p,55);return ok;},base,1,s),"health changing between native snapshots refused");
    std::cout<<"PASS: native health ownership, bounded attributes, suppressed maximum, transition and concurrent-change guards\n";
    constexpr auto knowledge=health+0x2000,targets=knowledge+0x2000,targetActor=actor+0x2000;
    constexpr std::uint32_t npcHandle=0x10003,targetHandle=0x20004;
    auto t=m;t.q(game+0x3130b0+8,actor);t.d(actor+0xa8,npcHandle);t.q(actor,base+0x1c5e108);t.q(attributes,base+0x1b99028);
    t.d(actor+0x270,3);t.q(list+16,knowledge);t.q(knowledge,base+0x1b95308);t.q(knowledge+0x10,actor);t.d(knowledge+0x1c,0xa);
    t.q(knowledge+0x168,targets);t.d(knowledge+0x170,1);t.q(knowledge+0xcd0,base+0x1b44828);t.d(knowledge+0xcd8,1);t.d(knowledge+0xcdc,targetHandle);
    t.q(game+0x3130b0+16,targetActor);t.d(targetActor+0xa8,targetHandle);
    t.q(targets,base+0x1b447c8);t.d(targets+8,1);t.d(targets+12,targetHandle);t.d(targets+0x168,2);t.d(targets+0x16c,2);
    NativeAiTargets ai;const auto inspectAi=[&](Memory& mem){return readNativeAiTargets([&](auto p,auto d,auto n){return mem.read(p,d,n);},base,npcHandle,ai);};
    require(inspectAi(t)&&ai.combat.handle==targetHandle&&ai.combat.resolved&&ai.attention.size()==1&&ai.attention[0].perceived==2,"AI combat and attention refer to registered native target");
    changed=t;changed.d(targetActor+0xa8,targetHandle+1);require(inspectAi(changed)&&!ai.combat.resolved&&!ai.attention[0].resolved,"retained stale AI target is reported without rebinding");
    changed=t;changed.d(knowledge+0x170,129);require(!inspectAi(changed),"oversized native attention list refused");
    changed=t;changed.q(knowledge+0x10,targetActor);require(!inspectAi(changed),"AI knowledge wrong owner refused");
    changed=t;changed.q(targets,base+0x1b44828);require(!inspectAi(changed),"combat record cannot stand in for attention record");
    changed=t;changed.d(targets+0x168,4);require(!inspectAi(changed),"unknown perceived relationship refused");
    changed=t;reads=0;require(!readNativeAiTargets([&](auto p,auto d,auto n){const bool ok=changed.read(p,d,n);if(p==knowledge+0xcdc&&++reads==1)changed.d(p,0x3fff0010);return ok;},base,npcHandle,ai),"concurrent AI target change refused");
    std::cout<<"PASS: AI knowledge ownership, bounded targets, stale target resolution and concurrent change\n";
}
void aiSchedulerTests() {
    constexpr std::uintptr_t base=0x140000000,game=0x200000000,actor=0x300000000,manager=0x400000000;
    constexpr std::uintptr_t registry=manager+0x10000,block=manager+0x20000,component=block+7*0x100;
    constexpr std::uintptr_t blocks=manager+0x30000,type=manager+0x40000,indices=manager+0x50000,list=manager+0x60000;
    constexpr std::uint32_t handle=0x10003;
    Memory m;m.q(game,base+0x1c940a8);m.q(game+0x3130b0+8,actor);m.q(game+0x1e8960,manager);
    m.q(actor,base+0x1c5e108);m.d(actor+0xa8,handle);m.q(actor+0x268,list);m.d(actor+0x270,1);m.q(list,component);
    m.q(component,base+0x1cc4108);m.q(component+0x10,actor);m.d(component+0x1c,0xa000000a);m.q(component+0x30,manager+0x70000);
    m.q(manager+0x2e0,registry);m.d(registry+8,16);m.q(registry+0x40,blocks);m.d(registry+0x48,1);m.q(blocks,block);
    m.q(registry+0x58,type);m.d(type+0x5c,0x100);m.q(registry+0x10,indices);m.d(registry+0x18,3);
    m.q(registry+0x20,indices+6);m.q(registry+0x28,indices+6);m.d(registry+0x30,3);m.d(registry+0x38,3);
    const std::uint16_t active[]={1,7,9};m.put(indices,active,sizeof(active));
    NativeAiDecisionState s{};
    const auto inspect=[&](Memory& source){return readNativeAiDecision([&](auto p,auto d,auto n){return source.read(p,d,n);},base,game,handle,s);};
    require(inspect(m)&&s.schedulerActive&&s.poolIndex==7&&s.activeCount==3,"native AI pool index and active membership");
    auto paused=m;const std::uint16_t partition[]={1,9,7};paused.put(indices,partition,sizeof(partition));
    paused.d(component+0x1c,0xa000006a);paused.d(registry+0x38,2);paused.q(registry+0x28,indices+4);
    require(inspect(paused)&&!s.schedulerActive&&s.liveCount==3&&s.activeCount==2,"paused component remains in inactive partition");
    auto reflex=m;reflex.q(component,base+nativeAiVtable(NativeAiKind::Reflex));reflex.q(manager+0x580,registry);
    require(readNativeAiComponent([&](auto p,auto d,auto n){return reflex.read(p,d,n);},base,game,handle,NativeAiKind::Reflex,s)&&
        s.schedulerActive&&s.kind==NativeAiKind::Reflex&&s.tree==0,"reflex uses its verified independent registry");
    auto attack=paused;attack.q(component,base+nativeAiVtable(NativeAiKind::Attack));attack.q(manager+0x600,registry);attack.d(component+0x1c,2);
    require(readNativeAiComponent([&](auto p,auto d,auto n){return attack.read(p,d,n);},base,game,handle,NativeAiKind::Attack,s)&&
        !s.schedulerActive&&(s.flags&0xc)==0,"inactive attack component can be checked without starting it");
    auto idle=paused;idle.q(component,base+nativeAiVtable(NativeAiKind::IdleBreak));idle.q(manager+0x530,registry);
    idle.d(component+0x20,1);idle.d(component+0x118,3);
    const auto inspectIdle=[&](Memory& source){return readNativeAiComponent([&](auto p,auto d,auto n){return source.read(p,d,n);},base,game,handle,NativeAiKind::IdleBreak,s);};
    require(inspectIdle(idle)&&!s.schedulerActive&&s.idleState==1&&s.idleAutoPlay==3,"idle break owns a distinct scheduler and native autoplay state");
    auto badIdle=idle;badIdle.d(component+0x20,4);require(!inspectIdle(badIdle),"unknown idle-break state refused");
    badIdle=idle;badIdle.q(manager+0x530,0);require(!inspectIdle(badIdle),"idle break does not borrow decision registry");
    badIdle=idle;unsigned idleReads=0;
    require(!readNativeAiComponent([&](auto p,auto d,auto n){const bool ok=badIdle.read(p,d,n);if(p==component+0x118&&++idleReads==1)badIdle.d(p,2);return ok;},base,game,handle,NativeAiKind::IdleBreak,s),"changing native autoplay state refused");
    auto changed=m;changed.q(component+0x10,actor+8);require(!inspect(changed),"AI wrong component owner refused");
    changed=m;changed.d(actor+0xa8,handle+1);require(!inspect(changed),"AI stale native handle refused");
    changed=m;changed.q(component,base+0x1b8a588);require(!inspect(changed),"AI reflex is not decision component");
    changed=m;changed.d(type+0x5c,0x101);require(!inspect(changed),"AI misaligned pool member refused");
    changed=m;changed.d(registry+0x38,4);require(!inspect(changed),"AI active count outside live bounds refused");
    changed=m;changed.q(registry+0x28,indices+4);require(!inspect(changed),"AI inconsistent end pointer refused");
    changed=m;changed.d(registry+0x48,257);require(!inspect(changed),"AI unbounded pool blocks refused");
    changed=m;const std::uint16_t duplicate[]={1,7,7};changed.put(indices,duplicate,sizeof(duplicate));require(!inspect(changed),"AI duplicated target index refused");
    changed=m;const std::uint16_t missing[]={1,8,9};changed.put(indices,missing,sizeof(missing));require(!inspect(changed),"AI missing scheduler member refused");
    changed=m;unsigned reads=0;
    require(!readNativeAiDecision([&](auto p,auto d,auto n){const bool ok=changed.read(p,d,n);if(p==component+0x1c&&++reads==1)changed.d(p,0xa000006a);return ok;},base,game,handle,s),"AI changing pause state refused");
    changed=m;reads=0;
    require(!readNativeAiDecision([&](auto p,auto d,auto n){const bool ok=changed.read(p,d,n);if(p==indices&&++reads==1)changed.put(p,missing,sizeof(missing));return ok;},base,game,handle,s),"AI changing scheduler membership refused");
    std::cout<<"PASS: AI scheduler pool membership, inactive partition, lifetime checks, bounds and concurrent-change refusal\n";
}
void playerStanceTests() {
    constexpr std::uintptr_t base=0x140000000,game=0x200000000,player=0x300000000;
    constexpr std::uintptr_t stack=0x400000000,list=0x500000000,component=0x600000000;
    Memory m;m.q(base+0x39a5290,game);m.q(game,base+0x1c940a8);m.d(game+0x341058,3);
    m.q(game+0x1f6ed0,game);m.q(game+0x1f73f0,stack);m.d(game+0x1f73f8,1);m.d(stack,1);m.d(game+0x1f7400,0);
    m.q(game+0x3130b0,player);m.q(player,base+0x1c5e428);m.d(player+0xa8,1);
    m.q(player+0x268,list);m.d(player+0x270,1);m.q(list,component);
    m.q(component,base+0x1c73268);m.q(component+0x10,player);m.d(component+0x50,2);m.d(component+0x54,1);
    const float position[]={1,2,3};m.put(player+0x300,position,sizeof(position));
    NativePlayerStance s{};
    const auto inspect=[&](Memory& source){return readNativePlayerStance([&](auto p,auto d,auto n){return source.read(p,d,n);},base,s);};
    require(inspect(m)&&s.current==2&&s.previous==1&&s.physics==component,"read stance from the actual registered player");
    auto changed=m;changed.q(component+0x10,player+8);require(!inspect(changed),"wrong stance component owner refused");
    changed=m;changed.q(component,base+0x1b80408);require(!inspect(changed),"animation component is not player physics");
    changed=m;changed.d(player+0xa8,0x10001);require(!inspect(changed),"non-player handle refused");
    changed=m;changed.d(player+0x270,257);require(!inspect(changed),"unbounded component list refused");
    changed=m;changed.d(player+0x270,2);changed.q(list+8,component);require(!inspect(changed),"duplicate physics component refused");
    changed=m;changed.d(stack,15);require(!inspect(changed),"loading world refused");
    changed=m;changed.d(game+0x1f7400,2);require(!inspect(changed),"pending native transition refused");
    changed=m;unsigned reads=0;
    require(!readNativePlayerStance([&](auto p,auto d,auto n){const bool ok=changed.read(p,d,n);if(p==component+0x50&&++reads==1)changed.d(p,3);return ok;},base,s),"mid-read stance change refused");
    changed=m;reads=0;
    require(!readNativePlayerStance([&](auto p,auto d,auto n){const bool ok=changed.read(p,d,n);if(p==player+0x300&&++reads==1)changed.q(game+0x3130b0,player+8);return ok;},base,s),"mid-read player replacement refused");
    std::cout<<"PASS: player stance ownership, bounded lookup, native transition and concurrent replacement refusal\n";
    constexpr std::uintptr_t machine=component+0x1000,mode=0x700000000,active=0x800000000,state=0x900000000;
    auto actions=m;actions.d(player+0x270,2);actions.q(list+8,machine);actions.q(machine,base+0x1c73d58);
    actions.q(machine+0x10,player);actions.q(machine+0x90,mode);actions.q(mode,machine);
    actions.q(machine+0x28,mode);
    actions.q(machine+0xa8,active);actions.d(machine+0xb0,1);actions.d(active,2);actions.q(mode+0x20,state);
    actions.q(state,base+0x1c85028);actions.d(state+8,2);actions.q(state+0x10,machine);
    NativePlayerStates actionState{};
    const auto inspectActions=[&](Memory& source){return readNativePlayerStates([&](auto p,auto d,auto n){return source.read(p,d,n);},base,actionState);};
    require(inspectActions(actions)&&actionState.active.size()==1&&actionState.active[0].id==2,"active state ID resolves through current mode");
    std::uint32_t motion=0;require(nativePlayerMotion(actionState,base,motion)&&motion==2,"verified native jump class maps to semantic jump");
    auto unknown=actionState;unknown.defaultMode=false;require(!nativePlayerMotion(unknown,base,motion),"non-default mode is not assumed to share movement semantics");
    unknown=actionState;unknown.active[0].vtable++;require(!nativePlayerMotion(unknown,base,motion),"known ID with wrong native class refused");
    unknown=actionState;unknown.active.push_back(unknown.active[0]);require(!nativePlayerMotion(unknown,base,motion),"ambiguous simultaneous active states refused");
    changed=actions;changed.q(mode,component);require(!inspectActions(changed),"mode owned by another component refused");
    changed=actions;changed.q(state+0x10,component);require(!inspectActions(changed),"active state owned by another component refused");
    changed=actions;changed.d(state+8,3);require(!inspectActions(changed),"state identity mismatch refused");
    changed=actions;changed.d(active,90);require(!inspectActions(changed),"out-of-bounds authored state ID refused");
    changed=actions;changed.d(machine+0xb0,91);require(!inspectActions(changed),"unbounded active list refused");
    changed=actions;changed.d(machine+0xb0,2);changed.d(active+4,2);require(!inspectActions(changed),"duplicate active state refused");
    changed=actions;reads=0;
    require(!readNativePlayerStates([&](auto p,auto d,auto n){const bool ok=changed.read(p,d,n);if(p==active&&++reads==1)changed.d(active,3);return ok;},base,actionState),"mid-read active-state change refused");
    changed=actions;reads=0;
    require(!readNativePlayerStates([&](auto p,auto d,auto n){const bool ok=changed.read(p,d,n);if(p==active&&++reads==1)changed.q(machine+0x90,mode+8);return ok;},base,actionState),"mid-read player-mode replacement refused");
    std::cout<<"PASS: active player states enforce reciprocal ownership, unique bounded IDs and stable mode/list lifetimes\n";
}
int main() {
    factionTests();
    healthTests();
    playerStanceTests();
    aiSchedulerTests();
    constexpr std::uintptr_t base=0x140000000,game=0x200000000,actor=0x300000000,spawner=0x300001000,component=0x300002000,definition=0x300003000;
    const auto fixture=[&](std::uint32_t handle,std::uint32_t parent) {
        Memory m;m.q(game,base+0x1c940a8);m.q(game+0x601c0,0x400000000);m.d(game+0x601c8,2);
        m.d(0x400000000,handle);m.d(0x400000004,parent);
        m.q(game+0x3130b0+(handle>>16)*8,actor);m.q(game+0x3130b0+(parent>>16)*8,spawner);
        m.q(actor,base+0x1c5e108);m.d(actor+0xa8,handle);m.d(actor+0xac,parent);m.q(actor+0x2e8,definition);
        m.q(definition,base+0x1ab58f8);m.q(definition+8,0x400001000);m.str(0x400001000,"models/characters/test.def");
        m.q(spawner,base+0x1c5e948);m.d(spawner+0xa8,parent);m.q(spawner+0x20,0x400002000);m.str(0x400002000,"ld_test_guard");
        m.q(spawner+0x268,0x400003000);m.d(spawner+0x270,1);m.q(0x400003000,component);
        m.q(component,base+0x1c57d78);m.q(component+0x10,spawner);m.q(component+0x30,0x400004000);m.d(component+0x38,1);m.d(0x400004000,handle);
        const float transform[]={1,2,3,1,0,0,0,1,0,0,0,1};m.put(actor+0x300,transform,sizeof(transform));return m;
    };
    auto a=fixture(0x10001,0x20002),b=fixture(0x70051,0x90062);
    const auto catalog=[&](Memory& m){std::vector<NativeWorldNpc> c;unsigned unbound=0;require(readWorldNpcCatalog([&](auto p,auto d,auto n){return m.read(p,d,n);},base,game,c,unbound),"catalog read");return c;};
    auto ac=catalog(a),bc=catalog(b);
    require(ac.size()==1&&bc.size()==1&&ac[0].unique&&bc[0].unique&&ac[0].handle!=bc[0].handle&&ac[0].spawnerName==bc[0].spawnerName&&ac[0].definition==bc[0].definition,"identity independent of local handle");
    auto changed=a;changed.d(0x400004000,0x40001);require(!catalog(changed)[0].unique,"broken reciprocal ownership refused");
    changed=a;changed.d(component+0x38,2);changed.d(0x400004004,0x40001);require(!catalog(changed)[0].unique,"multiple children refused");
    changed=a;changed.d(actor+0xa8,0x10002);require(catalog(changed).empty(),"stale actor generation refused");
    changed=a;changed.q(component+0x10,actor);require(catalog(changed).empty(),"wrong component owner refused");
    changed=a;changed.q(definition,base+1);require(catalog(changed).empty(),"wrong definition type refused");
    changed=a;changed.q(game+0x3130b0+3*8,spawner+0x10000);changed.d(game+0x601c8,3);changed.d(0x400000008,0x30003);
    changed.q(spawner+0x10000,base+0x1c5e948);changed.d(spawner+0x100a8,0x30003);changed.q(spawner+0x10020,0x400002000);
    require(!catalog(changed)[0].unique,"duplicate authored spawner name refused");
    CoopPosePacket host{},client{};host.role=0;client.role=1;host.sequence=client.sequence=1;host.flags=client.flags=1;
    host.session=11;client.session=22;host.worldEpoch=3;client.worldEpoch=7;host.mapId=client.mapId=123;
    host.gameLogicState=client.gameLogicState=1;host.cinematicMetadataValid=client.cinematicMetadataValid=1;host.cinematicEpoch=client.cinematicEpoch=1;
    host.orientation[0]=host.orientation[4]=host.orientation[8]=client.orientation[0]=client.orientation[4]=client.orientation[8]=1;
    CoopWorldNpcPacket p{};p.sequence=1;p.sourceSession=11;p.targetSession=22;p.sourceWorldEpoch=3;p.targetWorldEpoch=7;p.mapId=123;p.sourceHandle=0x10001;
    constexpr char spawnerName[]="ld_test_guard",definitionName[]="models/characters/test.def";
    std::memcpy(p.spawner,spawnerName,sizeof(spawnerName));std::memcpy(p.definition,definitionName,sizeof(definitionName));p.orientation[0]=p.orientation[4]=p.orientation[8]=1;
    require(validCoopWorldNpc(p,client,host),"valid host packet");
    auto hp=p;hp.healthValid=1;hp.health=40;hp.maximumHealth=40;
    require(validCoopWorldNpc(hp,client,host),"verified health observation accepted");
    hp.health=0;require(validCoopWorldNpc(hp,client,host),"zero health observation accepted without death inference");
    hp.health=50;require(validCoopWorldNpc(hp,client,host),"current above a changed maximum remains observable");
    hp.healthValid=0;require(!validCoopWorldNpc(hp,client,host),"unavailable health with stale payload refused");
    hp=p;hp.version=1;require(!validCoopWorldNpc(hp,client,host),"old world protocol refused");
    hp=p;hp.healthValid=2;require(!validCoopWorldNpc(hp,client,host),"unknown health validity refused");
    hp=p;hp.healthValid=1;hp.health=-1;require(!validCoopWorldNpc(hp,client,host),"negative health refused");
    hp.health=std::numeric_limits<float>::quiet_NaN();require(!validCoopWorldNpc(hp,client,host),"NaN health refused");
    hp.health=0;hp.maximumHealth=1000001;require(!validCoopWorldNpc(hp,client,host),"unbounded health maximum refused");
    auto bad=p;bad.role=1;require(!validCoopWorldNpc(bad,client,host),"client authority refused");
    bad=p;++bad.sourceWorldEpoch;require(!validCoopWorldNpc(bad,client,host),"old source world refused");
    bad=p;++bad.targetWorldEpoch;require(!validCoopWorldNpc(bad,client,host),"old target world refused");
    bad=p;++bad.sourceSession;require(!validCoopWorldNpc(bad,client,host),"wrong source session refused");
    bad=p;++bad.targetSession;require(!validCoopWorldNpc(bad,client,host),"wrong target session refused");
    bad=p;++bad.mapId;require(!validCoopWorldNpc(bad,client,host),"wrong map refused");
    bad=p;bad.position[0]=std::numeric_limits<float>::quiet_NaN();require(!validCoopWorldNpc(bad,client,host),"NaN refused");
    bad=p;bad.orientation[0]=-1;require(!validCoopWorldNpc(bad,client,host),"reflected basis refused");
    bad=p;std::memset(bad.spawner,'a',sizeof(bad.spawner));require(!validCoopWorldNpc(bad,client,host),"unterminated name refused");
    bad=p;bad.spawner[100]='x';require(!validCoopWorldNpc(bad,client,host),"noncanonical name padding refused");
    auto paused=client;paused.gameLogicState=2;require(!validCoopWorldNpc(p,paused,host),"pause refused");
    std::cout<<"PASS: reciprocal NPC identity, unrelated local handles, stale generations, ambiguous spawners, world packet scope and transform validation\n";
}
