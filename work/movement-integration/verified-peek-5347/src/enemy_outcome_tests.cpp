#include "enemy_outcome_protocol.h"
#include <iostream>
#include <cstdlib>
void check(bool b,const char* message){if(!b){std::cerr<<"FAIL: "<<message<<'\n';std::exit(1);}}
int main(){
    CoopPosePacket h{},c{};h.role=0;c.role=1;h.sequence=c.sequence=1;h.flags=c.flags=1;h.session=11;c.session=22;h.sampleTimeUs=c.sampleTimeUs=1000000;
    h.worldEpoch=3;c.worldEpoch=7;h.mapId=c.mapId=coopMapId(kGuardMap);h.gameLogicState=c.gameLogicState=1;
    h.cinematicMetadataValid=c.cinematicMetadataValid=1;h.cinematicEpoch=c.cinematicEpoch=1;
    h.orientation[0]=h.orientation[4]=h.orientation[8]=c.orientation[0]=c.orientation[4]=c.orientation[8]=1;
    CoopEnemyOutcomePacket p{};auto& b=p.binding;b.kind=GuardMessage::Stop;b.mode=1;b.sequence=1;b.binding=4;b.request=5;
    b.hostSession=11;b.clientSession=22;b.hostEpoch=3;b.clientEpoch=7;b.mapId=h.mapId;b.entityKey=guardEntityKey(1);b.hostHandle=0x10003;b.clientHandle=0x70051;
    p.hostArm=100;p.clientArm=200;p.kind=EnemyOutcomeMessage::Kill;p.event=1;p.declaration=coopMapId(kEnemyAssassinationDecl);p.sourceRole=0;
    check(validEnemyOutcome(p,c,h),"host canonical kill accepted");
    auto reverse=p;reverse.binding.role=1;reverse.sourceRole=1;check(validEnemyOutcome(reverse,h,c),"client canonical kill accepted");
    auto bad=p;bad.declaration++;check(!validEnemyOutcome(bad,c,h),"arbitrary damage declaration refused");
    bad=p;bad.sourceRole=1;check(!validEnemyOutcome(bad,c,h),"spoofed attacker role refused");
    bad=p;bad.hostArm=0;check(!validEnemyOutcome(bad,c,h),"unarmed kill refused");
    bad=p;bad.binding.clientEpoch++;check(!validEnemyOutcome(bad,c,h),"replaced world refused");
    bad=p;bad.binding.hostSession++;check(!validEnemyOutcome(bad,c,h),"old connection refused");
    bad=p;bad.binding.healthValid=1;check(!validEnemyOutcome(bad,c,h),"unused native health payload refused");
    bad=p;bad.binding.orientation[0]=1;check(!validEnemyOutcome(bad,c,h),"hidden pose payload refused");
    bad=p;bad.event=2;check(!validEnemyOutcome(bad,c,h),"second outcome refused");
    bad=p;bad.size--;check(!validEnemyOutcome(bad,c,h),"wrong packet size refused");
    bad=p;bad.version++;check(!validEnemyOutcome(bad,c,h),"unknown version refused");
    bad=p;bad.kind=EnemyOutcomeMessage::Ack;check(!validEnemyOutcome(bad,c,h),"ack cannot claim source peer");bad.sourceRole=1;check(validEnemyOutcome(bad,c,h),"ack identifies receiving source");
    auto paused=c;paused.gameLogicState=2;check(!validEnemyOutcome(p,paused,h),"native pause rejects outcome");
    auto loading=h;loading.pendingGameLogicState=15;check(!validEnemyOutcome(p,c,loading),"pending load rejects outcome");
    bool consumed=false;check(!enemyConsume(false,consumed,p,p),"unready local outcome refused");check(enemyConsume(true,consumed,p,p),"first bound outcome consumed");check(!enemyConsume(true,consumed,p,p),"duplicate cannot apply twice");
    consumed=false;bad=p;bad.clientArm++;check(!enemyConsume(true,consumed,p,bad),"old arm token refused");
    bad=p;bad.binding.clientHandle++;check(!enemyConsume(true,consumed,p,bad),"different native incarnation refused");
    bad=p;bad.kind=EnemyOutcomeMessage::Arm;bad.clientArm=0;bad.event=bad.declaration=0;bad.sourceRole=2;check(validEnemyOutcome(bad,c,h),"local-only arming nonce accepted");
    bad.clientArm=200;check(!validEnemyOutcome(bad,c,h),"arm message cannot choose peer nonce");
    std::cout<<"PASS: enemy outcome identity, either source role, local arms, canonical declaration, duplicate consumption, acknowledgments, world replacement and native phase guards\n";
}
