#pragma once
#include "breakable_protocol.h"
#include "native_breakable.h"
#include <cstring>
#include <unordered_map>
void breakableProtocolTests(){
    CoopPosePacket host{},client{};
    host.role=0;client.role=1;host.session=11;client.session=22;host.worldEpoch=3;client.worldEpoch=5;host.mapId=client.mapId=7;
    host.flags=client.flags=1;host.gameLogicState=client.gameLogicState=1;host.cinematicMetadataValid=client.cinematicMetadataValid=1;
    require(coopWorldPairReady(host,client)&&coopWorldPairReady(client,host),"1214fq: breakable test worlds are paired");
    CoopBreakablePacket p{};p.role=1;p.handle=0x6d302ed;p.step=1;p.steps=1;p.sequence=1;p.sourceSession=22;p.targetSession=11;
    p.sourceEpoch=5;p.targetEpoch=3;p.mapId=7;std::strcpy(p.name,"esc_env_display_case_glass_01a_4");
    require(validCoopBreakable(p,host,client),"1214fq: the client reports a broken step to the host");
    auto q=p;q.flags=1;q.hit[0]=-29.5f;q.hit[1]=-155.4f;q.hit[2]=67.f;q.direction[0]=1;
    require(validCoopBreakable(q,host,client),"1214fq: a step carries its native hit point and direction");
    auto h=p;h.role=0;h.sourceSession=11;h.targetSession=22;h.sourceEpoch=3;h.targetEpoch=5;
    require(validCoopBreakable(h,client,host),"1214fq: the host reports a broken step to the client");
    auto bad=p;bad.step=0;require(!validCoopBreakable(bad,host,client),"1214fq: step 0 is never sent");
    bad=p;bad.step=2;require(!validCoopBreakable(bad,host,client),"1214fq: a step above the breakable's count is refused");
    bad=p;bad.handle=0;require(!validCoopBreakable(bad,host,client),"1214fq: a handle is required");
    bad=p;bad.name[0]=0;require(!validCoopBreakable(bad,host,client),"1214fq: a name is required");
    bad=p;bad.targetEpoch=4;require(!validCoopBreakable(bad,host,client),"1214fq: another world epoch is refused");
    bad=p;bad.mapId=8;require(!validCoopBreakable(bad,host,client),"1214fq: another map is refused");
    bad=p;bad.role=0;require(!validCoopBreakable(bad,host,client),"1214fq: the role must be the sender's");
    bad=p;bad.hit[0]=1;require(!validCoopBreakable(bad,host,client),"1214fq: no hit values without the hit flag");
    bad=q;bad.direction[1]=NAN;require(!validCoopBreakable(bad,host,client),"1214fq: hit values are finite");
    bad=q;bad.direction[2]=5;require(!validCoopBreakable(bad,host,client),"1214fq: the direction is bounded");
    bad=p;bad.flags=2;require(!validCoopBreakable(bad,host,client),"1214fq: unknown flags are refused");
    require(breakableStepsToApply(0,1,1)==1&&breakableStepsToApply(0,3,3)==3&&breakableStepsToApply(1,3,3)==2,"1214fq: the receiver advances to the sender's step");
    require(!breakableStepsToApply(1,1,1)&&!breakableStepsToApply(2,1,3)&&!breakableStepsToApply(0,4,3)&&!breakableStepsToApply(-1,1,1),"1214fq: never backward, past the count or from an unread step");
    // Reader over a synthetic entity: game slot table, arkBreakable entity, arkCpntBreakable component.
    const std::uintptr_t base=0x140000000ull,game=0x1000000,actor=0x2000000,list=0x2100000,comp=0x2200000,decl=0x2300000,steps=0x2400000,health=0x2500000,name=0x2600000;
    const std::uint32_t handle=0x6d302ed;std::unordered_map<std::uintptr_t,unsigned char> mem;
    auto put=[&](std::uintptr_t a,const void* v,std::size_t n){for(std::size_t i=0;i<n;++i)mem[a+i]=static_cast<const unsigned char*>(v)[i];};
    auto put64=[&](std::uintptr_t a,std::uint64_t v){put(a,&v,8);};auto put32=[&](std::uintptr_t a,std::uint32_t v){put(a,&v,4);};
    put64(game+0x3130b0+static_cast<std::uintptr_t>(handle>>16)*8,actor);put32(actor+0xa8,handle);put64(actor,base+0x1b67bc8);
    put64(actor+0x268,list);put32(actor+0x270,2);put64(list,0x2700000);put64(0x2700000,base+0x1c573e8);put64(list+8,comp);
    put64(comp,base+0x1c71a88);put64(comp+0x10,actor);put64(comp+8,decl);put64(decl+0x68,steps);put32(decl+0x70,1);put32(comp+0x38,0);
    put64(comp+0x20,health);float hp=10,th=0;put(health+0x20,&hp,4);put(steps,&th,4);
    put64(actor+0x20,name);const char text[]="esc_env_display_case_glass_01a_4";put(name,text,sizeof(text));
    auto read=[&](std::uintptr_t a,void* out,std::size_t n){for(std::size_t i=0;i<n;++i){auto it=mem.find(a+i);if(it==mem.end())return false;static_cast<unsigned char*>(out)[i]=it->second;}return true;};
    NativeBreakable b{};
    require(readNativeBreakable(read,base,game,handle,b,true)&&b.component==comp&&b.step==0&&b.stepCount==1&&b.current==10&&b.name==text,"1214fq: the reader finds the breakable component, its step, health and name");
    float threshold=-1;require(readNativeBreakableThreshold(read,b,0,threshold)&&threshold==0&&!readNativeBreakableThreshold(read,b,1,threshold),"1214fq: step thresholds are bounded by the count");
    put32(comp+0x38,2);require(!readNativeBreakable(read,base,game,handle,b),"1214fq: a step past the count is refused");put32(comp+0x38,1);
    put64(actor,base+0x1c5e108);require(!readNativeBreakable(read,base,game,handle,b),"1214fq: only arkBreakable entities are breakables");put64(actor,base+0x1b67bc8);
    put64(comp+0x10,0x9999);require(!readNativeBreakable(read,base,game,handle,b),"1214fq: the component must belong to the entity");put64(comp+0x10,actor);
    put32(actor+0xa8,handle+1);require(!readNativeBreakable(read,base,game,handle,b),"1214fq: a recycled handle is refused");
}
