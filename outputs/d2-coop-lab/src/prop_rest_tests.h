#pragma once
#include "prop_rest_protocol.h"
#include <cstring>
void propRestTests(){
    CoopPosePacket host{},client{};
    host.role=0;client.role=1;host.session=11;client.session=22;host.worldEpoch=3;client.worldEpoch=5;host.mapId=client.mapId=7;
    host.flags=client.flags=1;host.gameLogicState=client.gameLogicState=1;host.cinematicMetadataValid=client.cinematicMetadataValid=1;
    require(coopWorldPairReady(host,client),"1214ip: prop rest test worlds are paired");
    CoopPropRestPacket p{};p.role=0;p.handle=0xa50066a;p.sequence=1;p.sourceSession=11;p.targetSession=22;p.sourceEpoch=3;p.targetEpoch=5;p.mapId=7;
    const float position[3]={-33.8f,-154.4f,67.4f},axis[9]={1,0,0,0,1,0,0,0,1};
    std::memcpy(p.position,position,12);std::memcpy(p.axis,axis,36);std::strcpy(p.name,"pickups_fruit_apple_01_5");
    require(validCoopPropRest(p,client,host),"1214ip (#178): the host sends where an apple came to rest");
    auto c=p;c.role=1;c.sourceSession=22;c.targetSession=11;c.sourceEpoch=5;c.targetEpoch=3;
    require(validCoopPropRest(c,host,client),"1214ip: the client sends a rest too");
    const float turned[9]={0,1,0,-1,0,0,0,0,1};auto t=p;std::memcpy(t.axis,turned,36);
    require(validCoopPropRest(t,client,host),"1214ip: a turned prop is a valid rest");
    auto bad=p;bad.handle=0;require(!validCoopPropRest(bad,client,host),"1214ip: a handle is required");
    bad=p;bad.name[0]=0;require(!validCoopPropRest(bad,client,host),"1214ip: a name is required");
    bad=p;bad.flags=1;require(!validCoopPropRest(bad,client,host),"1214ip: unknown flags are refused");
    bad=p;bad.targetEpoch=4;require(!validCoopPropRest(bad,client,host),"1214ip: another world epoch is refused");
    bad=p;bad.mapId=8;require(!validCoopPropRest(bad,client,host),"1214ip: another map is refused");
    bad=p;bad.role=1;require(!validCoopPropRest(bad,client,host),"1214ip: the role must be the sender's");
    bad=p;bad.position[1]=NAN;require(!validCoopPropRest(bad,client,host),"1214ip: the position is finite");
    bad=p;bad.position[0]=2e5f;require(!validCoopPropRest(bad,client,host),"1214ip: the position is bounded");
    bad=p;bad.axis[0]=2;require(!validCoopPropRest(bad,client,host),"1214ip: the axis is bounded");
    bad=p;bad.axis[8]=-1;require(!validCoopPropRest(bad,client,host),"1214ip: a mirrored axis is refused");
    bad=p;std::memset(bad.axis,0,36);require(!validCoopPropRest(bad,client,host),"1214ip: an empty axis is refused");
    require(propRestShouldSend(.5f,1000,false,false,false)&&propRestShouldSend(18.f,1500,false,false,false),"1214ip: a prop moved and still for 1 s is sent");
    require(!propRestShouldSend(.05f,1000,false,false,false)&&!propRestShouldSend(.5f,999,false,false,false),"1214ip: a small nudge or a prop still moving is not sent");
    require(!propRestShouldSend(.5f,1000,true,false,false),"1214ip: a held prop is carried by the held object stream");
    require(!propRestShouldSend(.5f,1000,false,true,false),"1214iq: a prop attached to another entity (a belt key) is not sent");
    require(!propRestShouldSend(.5f,1000,false,false,true),"1214ip: a prop in its quiet time after an applied rest is not sent back");
    require(propRestBound(0xbb0f83d0bc77792aull,0x3db5df06bc8d32ebull)&&!propRestBound(0,0),"1214ir: a prop with a local offset to its parent (the guard's key) is bound; fruit standing on a plate is not");
}
