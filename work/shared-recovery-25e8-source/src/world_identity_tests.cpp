#include "world_identity.h"
#include "world_protocol.h"
#include "native_ai_state.h"
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
int main() {
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
