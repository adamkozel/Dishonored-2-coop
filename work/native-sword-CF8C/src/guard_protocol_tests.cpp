#include "guard_protocol.h"
#include <iostream>
#include <limits>
#include <cstdlib>
void require(bool ok,const char* message){if(!ok){std::cerr<<"FAIL: "<<message<<'\n';std::exit(1);}}
int main() {
    CoopPosePacket host{},client{};host.role=0;client.role=1;host.sequence=client.sequence=1;host.flags=client.flags=1;
    host.session=11;client.session=22;host.worldEpoch=3;client.worldEpoch=7;host.mapId=client.mapId=coopMapId(kGuardMap);
    host.gameLogicState=client.gameLogicState=1;host.cinematicMetadataValid=client.cinematicMetadataValid=1;host.cinematicEpoch=client.cinematicEpoch=1;
    host.orientation[0]=host.orientation[4]=host.orientation[8]=client.orientation[0]=client.orientation[4]=client.orientation[8]=1;
    CoopGuardPacket request{};request.role=1;request.sequence=1;request.request=123;request.hostSession=11;request.clientSession=22;
    request.hostEpoch=3;request.clientEpoch=7;request.mapId=host.mapId;request.entityKey=guardEntityKey();request.clientHandle=0x70051;
    require(validCoopGuard(request,host,client),"client may request only its actual incarnation");
    auto offer=request;offer.role=0;offer.kind=GuardMessage::Offer;offer.binding=55;offer.hostHandle=0x10003;
    require(validCoopGuard(offer,client,host),"host offers distinct native incarnation");
    auto accept=offer;accept.role=1;accept.kind=GuardMessage::Accept;
    require(validCoopGuard(accept,host,client)&&guardSameBinding(accept,offer),"client acknowledges exact binding");
    auto pose=offer;pose.kind=GuardMessage::Pose;pose.position[0]=4;pose.orientation[0]=pose.orientation[4]=pose.orientation[8]=1;
    pose.healthValid=1;pose.health=pose.maximumHealth=40;
    require(validCoopGuard(pose,client,host)&&guardSameBinding(pose,accept),"pose remains bound to both different local handles");
    auto changed=pose;changed.clientHandle++;require(!guardSameBinding(changed,accept),"reused target slot generation cannot reuse binding");
    changed=pose;changed.hostHandle++;require(!guardSameBinding(changed,accept),"host respawn cannot reuse binding");
    changed=pose;changed.request++;require(!guardSameBinding(changed,accept),"delayed offer from old arming request cannot rebind");
    changed=pose;changed.binding++;require(!guardSameBinding(changed,accept),"old lease poses cannot affect a new binding");
    changed=pose;changed.hostEpoch++;require(!validCoopGuard(changed,client,host),"host reload invalidates packets");
    changed=pose;changed.clientEpoch++;require(!validCoopGuard(changed,client,host),"client reload invalidates packets");
    changed=pose;changed.hostSession++;require(!validCoopGuard(changed,client,host),"restarted host invalidates packets");
    changed=pose;changed.role=1;require(!validCoopGuard(changed,host,client),"client cannot author guard poses");
    changed=pose;changed.entityKey++;require(!validCoopGuard(changed,client,host),"different authored identity rejected");
    changed=pose;changed.reserved=1;require(!validCoopGuard(changed,client,host),"reserved bits rejected");
    changed=pose;changed.position[1]=std::numeric_limits<float>::quiet_NaN();require(!validCoopGuard(changed,client,host),"NaN pose rejected");
    changed=pose;changed.orientation[0]=-1;require(!validCoopGuard(changed,client,host),"reflected pose rejected");
    changed=offer;changed.orientation[0]=1;require(!validCoopGuard(changed,client,host),"control packets carry no pose");
    changed=pose;changed.health=35;require(validCoopGuard(changed,client,host),"nonfatal native health accepted");
    changed=pose;changed.version=1;require(!validCoopGuard(changed,client,host),"old guard protocol refused");
    changed=pose;changed.healthValid=0;require(!validCoopGuard(changed,client,host),"missing health cannot authorize mutation");
    changed=pose;changed.health=0;require(!validCoopGuard(changed,client,host),"fatal health is outside this experiment");
    changed=pose;changed.health=41;require(!validCoopGuard(changed,client,host),"health above verified maximum refused");
    changed=pose;changed.maximumHealth=39;require(!validCoopGuard(changed,client,host),"unexpected health modifier refused");
    changed=pose;changed.health=std::numeric_limits<float>::quiet_NaN();require(!validCoopGuard(changed,client,host),"nonfinite health refused");
    changed=offer;changed.health=1;require(!validCoopGuard(changed,client,host),"control packets carry no health");
    changed=request;changed.binding=1;require(!validCoopGuard(changed,host,client),"client cannot assign host binding");
    auto paused=client;paused.gameLogicState=2;require(!validCoopGuard(pose,paused,host),"pause disables pose acceptance");
    require(!guardPhaseAccepts(GuardPhase::Requested,GuardMessage::Pose,1),"pose before negotiation rejected");
    require(!guardPhaseAccepts(GuardPhase::Accepted,GuardMessage::Pose,1),"pose before native pause rejected");
    require(!guardPhaseAccepts(GuardPhase::Pausing,GuardMessage::Pose,1),"pose before pause completes rejected");
    require(guardPhaseAccepts(GuardPhase::Active,GuardMessage::Pose,1),"only active client accepts pose");
    require(!guardPhaseAccepts(GuardPhase::Active,GuardMessage::Offer,1),"active binding cannot be silently replaced");
    require(!guardPhaseAccepts(GuardPhase::Idle,GuardMessage::Commit,1),"unarmed client rejects commit");
    require(!guardPhaseAccepts(GuardPhase::Committed,GuardMessage::Accept,0),"duplicate accept does not recommit");
    require(guardPhaseAccepts(GuardPhase::Committed,GuardMessage::Ready,0),"host waits for native ownership acknowledgement");
    std::cout<<"PASS: explicit guard handshake, both incarnations, arming nonce, role/epoch/session guards, and pose validation\n";
}
