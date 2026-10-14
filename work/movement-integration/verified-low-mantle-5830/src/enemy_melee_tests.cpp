#include "enemy_melee_protocol.h"
#include <iostream>
#include <cstdlib>
#include <limits>
void check(bool b,const char* message){if(!b){std::cerr<<"FAIL: "<<message<<'\n';std::exit(1);}}
int main(){
    CoopPosePacket h{},c{};h.role=0;c.role=1;h.sequence=c.sequence=1;h.flags=c.flags=1;h.session=11;c.session=22;h.sampleTimeUs=c.sampleTimeUs=1000000;
    h.worldEpoch=3;c.worldEpoch=7;h.mapId=c.mapId=coopMapId(kGuardMap);h.gameLogicState=c.gameLogicState=1;
    h.cinematicMetadataValid=c.cinematicMetadataValid=1;h.cinematicEpoch=c.cinematicEpoch=1;
    h.orientation[0]=h.orientation[4]=h.orientation[8]=c.orientation[0]=c.orientation[4]=c.orientation[8]=1;
    CoopEnemyMeleePacket p{};auto& b=p.binding;b.kind=GuardMessage::Stop;b.mode=1;b.sequence=1;b.binding=4;b.request=5;
    b.hostSession=11;b.clientSession=22;b.hostEpoch=3;b.clientEpoch=7;b.mapId=h.mapId;b.entityKey=guardEntityKey(1);b.hostHandle=0x10003;b.clientHandle=0x70051;
    p.hostArm=100;p.clientArm=200;p.kind=EnemyMeleeMessage::Hit;p.event=1;p.declaration=coopMapId(kPlayerSwordDamageDecl);p.sourceRole=0;
    p.geometry.offset[2]=1.5f;p.geometry.normal[1]=-1;p.geometry.direction[1]=1;p.geometry.region=2;p.geometry.regions=4;
    check(validEnemyMelee(p,c,h),"host native nonfatal sword hit accepted");
    auto reverse=p;reverse.binding.role=1;reverse.sourceRole=1;check(validEnemyMelee(reverse,h,c),"client native nonfatal sword hit accepted");
    auto bad=p;bad.geometry.region=5;bad.geometry.regions=32;check(validEnemyMelee(bad,c,h),"other independently observed guard region accepted");
    bad=p;bad.geometry.region=3;bad.geometry.regions=8;check(!validEnemyMelee(bad,c,h),"unobserved guard hit region refused");
    bad=p;bad.geometry.regions=32;check(!validEnemyMelee(bad,c,h),"region mask mismatch refused");
    bad=p;bad.geometry.offset[0]=3;check(!validEnemyMelee(bad,c,h),"hit outside bounded victim radius refused");
    bad=p;bad.geometry.normal[1]=0;check(!validEnemyMelee(bad,c,h),"missing hit normal refused");
    bad=p;bad.geometry.direction[1]=std::numeric_limits<float>::quiet_NaN();check(!validEnemyMelee(bad,c,h),"nonfinite geometry refused");
    bad=p;bad.declaration++;check(!validEnemyMelee(bad,c,h),"arbitrary damage declaration refused");
    bad=p;bad.binding.clientEpoch++;check(!validEnemyMelee(bad,c,h),"old native world refused");
    bad=p;bad.binding.healthValid=1;check(!validEnemyMelee(bad,c,h),"hidden health payload refused");
    bad=p;bad.event=2;check(!validEnemyMelee(bad,c,h),"second event refused");
    bad=p;bad.kind=EnemyMeleeMessage::Ack;bad.sourceRole=1;check(!validEnemyMelee(bad,c,h),"ack cannot carry hidden hit geometry");
    bad.geometry={};check(validEnemyMelee(bad,c,h),"ack identifies original receiving source");
    bad=p;bad.kind=EnemyMeleeMessage::Arm;bad.clientArm=0;bad.event=bad.declaration=0;bad.sourceRole=2;bad.geometry={};
    check(validEnemyMelee(bad,c,h),"local-only arm nonce accepted");bad.clientArm=200;check(!validEnemyMelee(bad,c,h),"arm cannot choose peer nonce");
    auto paused=c;paused.gameLogicState=2;check(!validEnemyMelee(p,paused,h),"native pause rejects damage");
    bool consumed=false;check(!enemyMeleeConsume(false,consumed,p,p),"unready lease cannot consume damage");
    check(enemyMeleeConsume(true,consumed,p,p),"first exact hit consumes authority");check(!enemyMeleeConsume(true,consumed,p,p),"duplicate cannot consume twice");
    consumed=false;bad=p;bad.hostArm++;check(!enemyMeleeConsume(true,consumed,p,bad),"foreign local token refused");
    std::cout<<"PASS: ordinary enemy sword event binds either source, observed geometry, one consumption, native phase, world and local arm tokens\n";
}
