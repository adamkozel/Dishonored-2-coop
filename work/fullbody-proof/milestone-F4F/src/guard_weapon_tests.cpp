#include "guard_weapon_protocol.h"
#include <iostream>
#include <cstdlib>
void check(bool ok,const char* message){if(!ok){std::cerr<<"FAIL: "<<message<<'\n';std::exit(1);}}
int main(){
    CoopPosePacket h{},c{};h.role=0;c.role=1;h.sequence=c.sequence=1;h.flags=c.flags=1;h.session=11;c.session=22;
    h.worldEpoch=3;c.worldEpoch=7;h.mapId=c.mapId=coopMapId(kGuardMap);h.gameLogicState=c.gameLogicState=1;
    h.cinematicMetadataValid=c.cinematicMetadataValid=1;h.cinematicEpoch=c.cinematicEpoch=1;
    h.orientation[0]=h.orientation[4]=h.orientation[8]=c.orientation[0]=c.orientation[4]=c.orientation[8]=1;
    CoopGuardWeaponPacket p{};p.item=coopMapId(kGuardSwordItem);p.rootSequence=8;
    auto& b=p.binding;b.mode=1;b.kind=GuardMessage::Pose;b.sequence=4;b.binding=4;b.request=5;
    b.hostSession=11;b.clientSession=22;b.hostEpoch=3;b.clientEpoch=7;b.mapId=h.mapId;b.entityKey=guardEntityKey(1);b.hostHandle=0x10003;b.clientHandle=0x70051;
    b.healthValid=1;b.health=b.maximumHealth=40;b.orientation[0]=b.orientation[4]=b.orientation[8]=1;
    check(validGuardWeapon(p,c,h),"known holstered sword accepted");p.drawn=1;check(validGuardWeapon(p,c,h),"known drawn sword accepted");
    auto bad=p;bad.drawn=2;check(!validGuardWeapon(bad,c,h),"unknown equipment state rejected");
    bad=p;bad.item++;check(!validGuardWeapon(bad,c,h),"arbitrary item cannot be requested");
    bad=p;bad.rootSequence=0;check(!validGuardWeapon(bad,c,h),"root correlation required");
    bad=p;bad.reserved=1;check(!validGuardWeapon(bad,c,h),"unknown flags rejected");
    bad=p;bad.binding.mode=0;bad.binding.entityKey=guardEntityKey();check(!validGuardWeapon(bad,c,h),"wrong native guard mode rejected");
    bad=p;bad.binding.hostEpoch++;check(!validGuardWeapon(bad,c,h),"old world rejected");
    bad=p;bad.binding.clientSession++;check(!validGuardWeapon(bad,c,h),"different receiving session rejected");
    bad=p;bad.binding.clientHandle++;check(!guardSameBinding(bad.binding,p.binding),"different incarnation cannot share binding");
    bad=p;bad.binding.request++;check(!guardSameBinding(bad.binding,p.binding),"old request cannot share binding");
    bad=p;bad.binding.binding++;check(!guardSameBinding(bad.binding,p.binding),"old handshake cannot share binding");
    bad=p;bad.version++;check(!validGuardWeapon(bad,c,h),"unknown protocol rejected");
    bad=p;bad.size--;check(!validGuardWeapon(bad,c,h),"truncated payload rejected");
    check(!validGuardWeapon(p,h,c),"host cannot receive client equipment authority");
    auto changed=c;changed.gameLogicState=2;check(!validGuardWeapon(p,changed,h),"pause rejects equipment authority");
    changed=h;changed.pendingGameLogicState=15;check(!validGuardWeapon(p,c,changed),"pending load rejects equipment authority");
    check(guardWeaponSequenceFresh(p,11,11,3),"fresh sequence accepted");
    check(!guardWeaponSequenceFresh(p,11,11,4),"duplicate cannot renew lease");
    check(!guardWeaponSequenceFresh(p,11,11,5),"out-of-order cannot renew lease");
    check(guardWeaponRootFresh(8,8)&&guardWeaponRootFresh(8,16),"current root window accepted");
    check(!guardWeaponRootFresh(8,17)&&!guardWeaponRootFresh(9,8)&&!guardWeaponRootFresh(0,8),"old, future and absent roots rejected");
    std::cout<<"PASS: fixed sword state, guard incarnation, world/session, root correlation and duplicate/out-of-order lifetime checks\n";
}
