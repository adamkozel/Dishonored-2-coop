#include "avatar_weapon_protocol.h"
#include <iostream>
#include <cstdlib>
void check(bool ok,const char* m){if(!ok){std::cerr<<"FAIL: "<<m<<'\n';std::exit(1);}}
int main(){
    CoopPosePacket h{},c{};h.role=0;c.role=1;h.sequence=c.sequence=100;h.flags=c.flags=1;h.session=11;c.session=22;
    h.worldEpoch=3;c.worldEpoch=7;h.mapId=c.mapId=99;h.gameLogicState=c.gameLogicState=1;h.cinematicMetadataValid=c.cinematicMetadataValid=1;
    CoopAvatarWeaponPacket p{};p.sequence=1;p.sourceSession=11;p.targetSession=22;p.sourceWorldEpoch=3;p.targetWorldEpoch=7;p.mapId=99;p.sourceToken=4;p.rootSequence=99;p.character=1;
    check(validAvatarWeapon(p,c,h),"host initial hello");p.targetToken=9;check(validAvatarWeapon(p,c,h)&&avatarWeaponTokensMatch(p,9,4),"acknowledged hello");
    p.kind=1;p.state=1;check(validAvatarWeapon(p,c,h),"Corvo drawn state");p.state=0;check(validAvatarWeapon(p,c,h),"Corvo holstered state");
    check(!avatarWeaponTokensMatch(p,8,4)&&!avatarWeaponTokensMatch(p,9,5)&&!avatarWeaponTokensMatch(p,0,4),"old or absent opt-in tokens");
    auto bad=p;bad.targetToken=0;check(!validAvatarWeapon(bad,c,h),"state before receiver nonce");
    bad=p;bad.sourceToken=0;check(!validAvatarWeapon(bad,c,h),"missing source nonce");
    bad=p;bad.kind=2;check(!validAvatarWeapon(bad,c,h),"unknown kind");bad=p;bad.state=2;check(!validAvatarWeapon(bad,c,h),"unknown equipment state");
    bad=p;bad.kind=0;bad.state=1;check(!validAvatarWeapon(bad,c,h),"hello cannot carry equip request");
    auto reverse=p;reverse.role=1;reverse.character=2;reverse.sourceSession=22;reverse.targetSession=11;reverse.sourceWorldEpoch=7;reverse.targetWorldEpoch=3;
    check(!validAvatarWeapon(reverse,h,c),"Emily item not supported");reverse.kind=0;check(validAvatarWeapon(reverse,h,c),"receiver opt-in hello");
    bad=p;bad.character=2;check(!validAvatarWeapon(bad,c,h),"wrong role character");
    bad=p;bad.sourceSession++;check(!validAvatarWeapon(bad,c,h),"old source process");bad=p;bad.targetSession++;check(!validAvatarWeapon(bad,c,h),"old target process");
    bad=p;bad.sourceWorldEpoch++;check(!validAvatarWeapon(bad,c,h),"old source world");bad=p;bad.targetWorldEpoch++;check(!validAvatarWeapon(bad,c,h),"old receiver world");
    bad=p;bad.mapId++;check(!validAvatarWeapon(bad,c,h),"different map");bad=p;bad.rootSequence=91;check(!validAvatarWeapon(bad,c,h),"stale root");bad=p;bad.rootSequence=101;check(!validAvatarWeapon(bad,c,h),"future root");
    bad=p;bad.version++;check(!validAvatarWeapon(bad,c,h),"unknown version");bad=p;bad.size--;check(!validAvatarWeapon(bad,c,h),"truncation");bad=p;bad.build++;check(!validAvatarWeapon(bad,c,h),"different executable");
    auto paused=c;paused.gameLogicState=2;check(!validAvatarWeapon(p,paused,h),"receiver pause");paused=h;paused.gameLogicState=13;check(!validAvatarWeapon(p,c,paused),"source death");paused=h;paused.pendingGameLogicState=15;check(!validAvatarWeapon(p,c,paused),"pending reload");
    check(!armSequenceFresh(3,11,11,3)&&!armSequenceFresh(2,11,11,3)&&armSequenceFresh(4,11,11,3),"duplicate and reordered sequence");
    std::cout<<"PASS: Corvo weapon opt-in tokens, exact direction, known states, native phase, sessions/worlds, root age and replay refusal\n";
}
