#include "damage_protocol.h"
#include <iostream>
#include <limits>
#include <cstdlib>
void check(bool ok,const char* message){if(!ok){std::cerr<<message<<'\n';std::exit(1);}}
int main(){
    CoopPosePacket h{},c{};h.role=0;c.role=1;h.session=11;c.session=22;h.worldEpoch=3;c.worldEpoch=7;
    h.mapId=c.mapId=coopMapId(kGuardMap);h.flags=c.flags=1;h.gameLogicState=c.gameLogicState=1;
    h.cinematicMetadataValid=c.cinematicMetadataValid=1;
    CoopSwordPacket p{};p.sequence=1;p.hostSession=11;p.clientSession=22;p.hostEpoch=3;p.clientEpoch=7;p.mapId=h.mapId;p.guardKey=swordGuardKey();p.hostArm=5;
    check(validCoopSword(p,c,h),"host arming scope");p.role=1;p.hostArm=0;p.clientArm=6;check(validCoopSword(p,h,c),"client arming scope");
    p.role=0;p.hostArm=5;p.kind=SwordMessage::Ready;check(validCoopSword(p,c,h),"both fresh arm tokens");
    auto hit=p;hit.kind=SwordMessage::Hit;hit.event=1;hit.hitOffset[2]=1.5f;hit.direction[1]=-1;
    check(validCoopSword(hit,c,h),"bounded pointer-free sword hit");
    bool consumed=false;check(!swordConsumeHit(false,consumed,p,hit)&&!consumed,"unarmed receiver refuses hit");
    auto wrong=hit;wrong.clientArm++;check(!swordConsumeHit(true,consumed,p,wrong)&&!consumed,"old client arm refused");
    check(swordConsumeHit(true,consumed,p,hit)&&consumed,"one hit consumed before native dispatch");
    check(!swordConsumeHit(true,consumed,p,hit),"duplicate does not dispatch");
    wrong=hit;wrong.hostEpoch++;check(!validCoopSword(wrong,c,h),"host reload");wrong=hit;wrong.clientEpoch++;check(!validCoopSword(wrong,c,h),"client reload");
    wrong=hit;wrong.hostSession++;check(!validCoopSword(wrong,c,h),"host reconnect");wrong=hit;wrong.clientSession++;check(!validCoopSword(wrong,c,h),"client reconnect");
    wrong=hit;wrong.role=1;check(!validCoopSword(wrong,h,c),"client cannot author damage");wrong=hit;wrong.event=2;check(!validCoopSword(wrong,c,h),"only one bounded hit");
    wrong=hit;wrong.guardKey++;check(!validCoopSword(wrong,c,h),"authored guard identity");wrong=hit;wrong.version++;check(!validCoopSword(wrong,c,h),"protocol version");
    wrong=hit;wrong.hitOffset[0]=4;check(!validCoopSword(wrong,c,h),"unbounded relative location");wrong=hit;wrong.direction[0]=std::numeric_limits<float>::quiet_NaN();check(!validCoopSword(wrong,c,h),"nonfinite direction");
    wrong=hit;wrong.direction[1]=0;check(!validCoopSword(wrong,c,h),"zero direction");wrong=hit;wrong.reservedTail[2]=1;check(!validCoopSword(wrong,c,h),"reserved data");
    wrong=p;wrong.hitOffset[2]=1;check(!validCoopSword(wrong,c,h),"control packet payload");auto paused=c;paused.gameLogicState=2;check(!validCoopSword(hit,paused,h),"native pause");
    std::cout<<"PASS: sword hit scoped to role, world, sessions, two arm tokens, one consumption and bounded direction/location\n";
}
