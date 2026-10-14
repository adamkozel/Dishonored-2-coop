#pragma once
#include "door_protocol.h"
#include "door_session_scope.h"
inline void doorProtocolTests(){
    CoopPosePacket host{},client{};host.role=0;client.role=1;host.session=11;client.session=22;host.worldEpoch=3;client.worldEpoch=5;
    host.mapId=client.mapId=9;host.flags=client.flags=1;host.gameLogicState=client.gameLogicState=1;host.cinematicMetadataValid=client.cinematicMetadataValid=1;
    const auto scope=doorSessionScope(host,client);
    require(doorSessionWorldMatches(scope,host,client)&&!doorSessionRenewalReady(scope,host,client),"active door scope cannot renew into its own world");
    auto nextHost=host,nextClient=client;++nextHost.worldEpoch;
    require(!doorSessionRenewalReady(scope,nextHost,nextClient),"one loading peer cannot renew the pair");
    ++nextClient.worldEpoch;require(doorSessionRenewalReady(scope,nextHost,nextClient),"both new native epochs permit coordinated renewal");
    nextClient.gameLogicState=15;require(!doorSessionRenewalReady(scope,nextHost,nextClient),"loading peer blocks door renewal");nextClient=client;++nextClient.worldEpoch;
    nextClient.pendingGameLogicState=1;require(!doorSessionRenewalReady(scope,nextHost,nextClient),"pending transition blocks door renewal");nextClient.pendingGameLogicState=0;
    ++nextClient.session;require(!doorSessionRenewalReady(scope,nextHost,nextClient),"new Steam session cannot inherit door intent");--nextClient.session;
    ++nextHost.mapId;++nextClient.mapId;require(!doorSessionRenewalReady(scope,nextHost,nextClient),"another map requires a new explicitly armed session");
    require(!doorSessionRenewalReady(DoorSessionScope{},host,client),"empty door intent cannot renew");
    for(unsigned menu:{2u,4u,8u,12u}){
        auto h=host,c=client;h.gameLogicState=menu;h.cinematicMetadataValid=0;
        require(doorSessionCanRetainMenu(scope,h,c)&&!coopWorldPairReady(h,c),"known host menu retains IDs but forbids native mutation");
        h=host;c.gameLogicState=menu;c.cinematicMetadataValid=0;
        require(doorSessionCanRetainMenu(scope,h,c)&&!coopWorldPairReady(h,c),"known client menu retains IDs but forbids native mutation");
        h.pendingGameLogicState=menu;require(doorSessionCanRetainMenu(scope,h,c),"same-world pending menu can suspend");
        ++c.worldEpoch;require(!doorSessionCanRetainMenu(scope,h,c),"menu cannot retain a changed native incarnation");
    }
    {auto c=client;c.pendingGameLogicState=8;c.cinematicMetadataValid=0;require(doorSessionCanRetainMenu(scope,host,c),"opening item wheel suspends without cinematic metadata");
     c.cinematicNodeCount=1;require(!doorSessionCanRetainMenu(scope,host,c),"opening overlay cannot hide a cinematic");}
    for(unsigned state:{0u,3u,5u,13u,15u,99u}){auto c=client;c.gameLogicState=state;require(!doorSessionCanRetainMenu(scope,host,c),"unknown, death, cinematic and loading phases do not inherit menu retention");}
    auto invalidMenu=client;invalidMenu.gameLogicState=2;invalidMenu.pendingGameLogicState=15;
    require(!doorSessionCanRetainMenu(scope,host,invalidMenu),"pending load overrides menu retention");
    invalidMenu=client;invalidMenu.gameLogicState=2;invalidMenu.cinematicNodeCount=1;
    require(!doorSessionCanRetainMenu(scope,host,invalidMenu),"cinematic cannot masquerade as a menu");
    require(doorResumeDeadline(2000,6200)==8200&&doorResumeDeadline(0,6200)==0&&doorResumeDeadline(UINT64_MAX-5,10)==UINT64_MAX,"menu time extends only armed deadlines without wraparound");
    CoopDoorPacket p{};p.sequence=1;p.role=0;p.sourceSession=11;p.targetSession=22;p.sourceEpoch=3;p.targetEpoch=5;p.mapId=9;p.sourceArm=1;
    std::strcpy(p.name,"door_3");std::strcpy(p.definition,"door_definition");
    require(validCoopDoor(p,client,host),"scoped native door hello");
    auto bad=p;bad.targetArm=1;require(!validCoopDoor(bad,client,host),"hello cannot carry an apply scope");
    p.kind=DoorMessage::State;p.targetArm=2;p.revision=1;p.target=2;p.side=1;
    require(validCoopDoor(p,client,host),"host desired open state accepted");
    for(unsigned field=0;field<12;++field){bad=p;switch(field){case 0:bad.sourceSession++;break;case 1:bad.targetSession++;break;case 2:bad.sourceEpoch++;break;case 3:bad.targetEpoch++;break;case 4:bad.mapId++;break;case 5:bad.target=1;break;case 6:bad.side=2;break;case 7:bad.revision=0;break;case 8:bad.sourceArm=0;break;case 9:bad.targetArm=0;break;case 10:bad.reserved=1;break;case 11:bad.role=1;break;}require(!validCoopDoor(bad,client,host),"foreign or malformed door scope rejected");}
    bad=p;bad.name[191]='a';require(!validCoopDoor(bad,client,host),"noncanonical padded authored identity rejected");
    bad=p;bad.kind=DoorMessage::Request;require(!validCoopDoor(bad,client,host),"host cannot impersonate client request");
    p.role=1;p.sourceSession=22;p.targetSession=11;p.sourceEpoch=5;p.targetEpoch=3;p.kind=DoorMessage::Request;
    require(validCoopDoor(p,host,client),"client desired state request accepted");
    bad=p;bad.kind=DoorMessage::State;require(!validCoopDoor(bad,host,client),"client cannot publish host authority");
    auto paused=host;paused.gameLogicState=2;require(!validCoopDoor(p,paused,client),"no door mutation during native pause");
    DoorOrder order;require(order.set(0,0)&&order.revision==1,"initial host baseline revision");
    require(!order.set(0,1)&&order.revision==1,"closed target duplicate does not create toggle");
    require(order.set(2,0)&&order.revision==2&&order.set(0,0)&&order.revision==3,"host serializes conflicting desired states");
    require(!order.accept(2,2,0)&&order.target==0,"stale commit cannot reopen door");
    require(order.accept(4,2,1)&&!order.accept(4,2,1)&&order.side==1,"new commit accepted once");
    require(!order.accept(5,1,0)&&!order.accept(5,2,2),"invalid order cannot replace latest");
    std::cout<<"PASS: door packet roles, world epochs, authored identities and monotonic desired-state revisions\n";
}
