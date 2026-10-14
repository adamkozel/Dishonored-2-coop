#include "shadow_action_protocol.h"
#include "native_player_action.h"
#include "combat_presentation.h"
#include "equipment_presentation.h"
#include <iostream>
#include <cstdlib>
void check(bool ok,const char* m){if(!ok){std::cerr<<"FAIL: "<<m<<'\n';std::exit(1);}}
int main(){
    CoopPosePacket h{},c{};h.role=0;c.role=1;h.sequence=c.sequence=100;h.flags=c.flags=1;h.session=11;c.session=22;h.sampleTimeUs=c.sampleTimeUs=1000000;
    h.worldEpoch=3;c.worldEpoch=7;h.mapId=c.mapId=99;h.gameLogicState=c.gameLogicState=1;h.cinematicMetadataValid=c.cinematicMetadataValid=1;
    CoopShadowActionPacket p{};p.sampleTimeUs=1000000;p.sequence=1;p.sourceSession=11;p.targetSession=22;p.sourceWorldEpoch=3;p.targetWorldEpoch=7;p.mapId=99;p.sourceToken=4;p.rootSequence=99;p.character=1;
    check(validShadowAction(p,c,h),"initial hello");p.targetToken=9;check(validShadowAction(p,c,h)&&shadowActionTokensMatch(p,9,4),"two tokens");
    p.kind=1;p.event=1;p.swordState=2;for(unsigned state=0;state<=6;++state){p.state=state;p.airStage=state==3?2:0;p.combatVariant=state==1?1:state==2?10:0;p.combatPhase=p.combatVariant?1:0;p.combatDurationMs=p.combatVariant?1000:0;check(validShadowAction(p,c,h),"known native state");}p.state=5;
    p.leanSide=-1;p.leanUp=.5f;check(validShadowAction(p,c,h),"bounded native lean axes");p.leanSide=1.01f;check(!validShadowAction(p,c,h),"excess lean refused");
    p.leanSide=NAN;check(!validShadowAction(p,c,h),"nonfinite lean refused");p.leanSide=p.leanUp=0;
    check(!shadowActionTokensMatch(p,8,4)&&!shadowActionTokensMatch(p,9,5),"old tokens");
    auto bad=p;bad.event=0;check(!validShadowAction(bad,c,h),"state without event");
    {auto climb=p;climb.state=8;climb.traversalVariant=1;climb.traversalTimeMs=250;climb.traversalDurationMs=500;climb.traversalStart={-32,-147,66.8f};climb.traversalGoal={-32,-146,67.8f};
      check(validShadowAction(climb,c,h),"observed native low mantle packet");
      auto invalid=climb;invalid.state=0;check(!validShadowAction(invalid,c,h),"ground cannot smuggle traversal");
      invalid=climb;invalid.traversalGoal[2]=NAN;check(!validShadowAction(invalid,c,h),"nonfinite ledge refused");
      invalid=climb;invalid.traversalGoal[0]+=10;check(!validShadowAction(invalid,c,h),"unbounded traversal target refused");
      invalid=climb;invalid.traversalDurationMs=700;check(!validShadowAction(invalid,c,h),"unobserved duration refused");
      invalid=climb;invalid.traversalEndStance=1;check(!validShadowAction(invalid,c,h),"unobserved low-mantle crouched recovery refused");
      invalid=climb;invalid.traversalVariant=2;invalid.traversalEndStance=1;check(validShadowAction(invalid,c,h),"observed crouched mantle has an explicit low exit");
      invalid.traversalEndStance=0;check(!validShadowAction(invalid,c,h),"crouched mantle cannot claim standing exit");
    }
    {auto vault=p;vault.state=9;vault.traversalVariant=3;vault.traversalTimeMs=500;vault.traversalDurationMs=1000;
      vault.traversalStart={0,0,1};vault.traversalPoint={0,1,1};vault.traversalGoal={0,2.6f,1};vault.traversalHeight=.97f;
      check(validShadowAction(vault,c,h),"native vault sends bounded distance phase and obstacle geometry");
      auto invalid=vault;invalid.state=8;check(!validShadowAction(invalid,c,h),"mantle cannot carry vault geometry");
      invalid=vault;invalid.traversalHeight=NAN;check(!validShadowAction(invalid,c,h),"nonfinite vault height refused");
      invalid=vault;invalid.traversalPoint[1]=5;check(!validShadowAction(invalid,c,h),"vault point must lie inside traversal");
      invalid=vault;invalid.traversalTimeMs=1001;check(!validShadowAction(invalid,c,h),"excess geometric progress refused");
      invalid=p;invalid.traversalHeight=1;check(!validShadowAction(invalid,c,h),"idle cannot smuggle obstacle geometry");
    }
    {auto equip=p;equip.state=7;equip.equipmentVariant=1;equip.equipmentDurationMs=1066;equip.equipmentTimeMs=400;
      check(validShadowAction(equip,c,h),"bounded native equipment scene");
      equip.swordState=0;check(!validShadowAction(equip,c,h),"unknown source equipment cannot drive attachment");equip.swordState=2;
      equip.equipmentVariant=5;check(validShadowAction(equip,c,h),"native fast arm recovery has distinct equipment semantic");
      equip.equipmentVariant=6;check(validShadowAction(equip,c,h),"native crouched fast arm recovery has distinct semantic");
      equip.equipmentVariant=7;check(!validShadowAction(equip,c,h),"unknown equipment action refused");equip.equipmentVariant=1;
      equip.equipmentDurationMs=10001;check(!validShadowAction(equip,c,h),"unbounded equipment duration refused");equip.equipmentDurationMs=1066;
      equip.state=0;check(!validShadowAction(equip,c,h),"idle cannot smuggle an equipment scene");
      equip=p;equip.swordState=1;check(validShadowAction(equip,c,h),"known holstered source state");
      equip.state=1;equip.combatVariant=1;equip.combatPhase=1;equip.combatDurationMs=1000;check(!validShadowAction(equip,c,h),"holstered source cannot send sword combat");
    }
    bad=p;bad.targetToken=0;check(!validShadowAction(bad,c,h),"state before receiver nonce");
    bad=p;bad.sourceToken=0;check(!validShadowAction(bad,c,h),"missing source nonce");
    bad=p;bad.kind=2;check(!validShadowAction(bad,c,h),"unknown kind");bad=p;bad.state=8;check(!validShadowAction(bad,c,h),"unknown action");
    bad=p;bad.state=0;bad.leanSide=.5f;check(!validShadowAction(bad,c,h),"idle cannot smuggle lean axes");
    bad=p;bad.state=3;bad.airStage=1;bad.airStartZ=66;bad.airTargetZ=67.5f;check(validShadowAction(bad,c,h),"bounded native jump height pair");
    bad.airTargetZ=66;check(!validShadowAction(bad,c,h),"zero jump height refused");bad.airTargetZ=NAN;check(!validShadowAction(bad,c,h),"nonfinite jump target refused");
    bad=p;bad.airStage=2;check(!validShadowAction(bad,c,h),"ground cannot carry airborne phase");
    bad=p;bad.version=2;check(!validShadowAction(bad,c,h),"pre-sprint protocol refused");
    bad=p;bad.kind=0;check(!validShadowAction(bad,c,h),"hello cannot carry action/event");
    auto reverse=p;reverse.role=1;reverse.character=2;reverse.sourceSession=22;reverse.targetSession=11;reverse.sourceWorldEpoch=7;reverse.targetWorldEpoch=3;
    check(validShadowAction(reverse,h,c),"Emily action");
    bad=p;bad.character=2;check(!validShadowAction(bad,c,h),"wrong character");
    bad=p;bad.sourceSession++;check(!validShadowAction(bad,c,h),"source session");bad=p;bad.targetSession++;check(!validShadowAction(bad,c,h),"target session");
    bad=p;bad.sourceWorldEpoch++;check(!validShadowAction(bad,c,h),"source epoch");bad=p;bad.targetWorldEpoch++;check(!validShadowAction(bad,c,h),"target epoch");
    bad=p;bad.mapId++;check(!validShadowAction(bad,c,h),"map");bad=p;bad.rootSequence=91;check(!validShadowAction(bad,c,h),"stale root");
    bad=p;bad.rootSequence=101;check(!validShadowAction(bad,c,h),"future root");bad=p;bad.size--;check(!validShadowAction(bad,c,h),"size");
    bad=p;bad.version++;check(!validShadowAction(bad,c,h),"version");bad=p;bad.build++;check(!validShadowAction(bad,c,h),"executable");
    auto paused=c;paused.gameLogicState=2;check(!validShadowAction(p,paused,h),"receiver pause");paused=h;paused.gameLogicState=13;check(!validShadowAction(p,c,paused),"source death");
    paused=h;paused.pendingGameLogicState=15;check(!validShadowAction(p,c,paused),"pending reload");
    p.sequence=10;p.event=3;p.state=2;
    check(!shadowActionEventFresh(p,10,3,2)&&!shadowActionEventFresh(p,11,3,2),"replay and reordering");
    check(!shadowActionEventFresh(p,9,4,2)&&!shadowActionEventFresh(p,9,3,1),"old event and altered event contents");
    check(shadowActionEventFresh(p,9,3,2)&&shadowActionEventFresh(p,9,2,0),"fresh continued hold and new event");
    { CombatPresentation t;CombatPresentation::Sample o{};float a=0;
      check(t.push({100,1,1,1,0,1000})&&t.push({140,1,1,1,40,1000}),"source cadence samples");
      check(t.sample(120,o,a)&&std::fabs(a-.02f)<.00001f,"phase matches delayed root time");
      check(!t.sample(99,o,a),"do not announce future action");
      check(!t.push({139,1,1,1,50,1000})&&!t.push({180,1,2,1,80,1000}),"reordered and altered same event refused");
      check(t.push({180,2,2,1,0,1000}),"combo changes while Slash state remains active");
      check(t.sample(179,o,a)&&o.event==1&&t.sample(180,o,a)&&o.event==2&&a==0,"no interpolation across combo replacement");
      check(t.push({220,3,11,2,2130,2133})&&t.push({260,3,11,2,2170,2133}),"guard loop samples");
      check(t.sample(260,o,a)&&a>0&&a<.03f,"loop wraps native duration");
      check(t.push({300,4,0,0,0,0})&&t.sample(300,o,a)&&!o.variant,"native release");
      check(!t.sample(501,o,a),"stale action refuses presentation");
      t.clear();check(!t.sample(300,o,a),"pause clears pending history");
    }
    {EquipmentPresentation t;EquipmentPresentation::Sample o{};float a=0;
      check(t.push({100,1,1,0,0,0})&&t.push({140,2,2,1,0,1000})&&t.push({180,2,2,1,40,1000}),"equipment source samples");
      check(t.sample(139,o,a)&&o.sword==1&&!o.variant,"future draw does not attach sword early");
      check(t.sample(160,o,a)&&o.variant==1&&std::fabs(a-.02f)<.00001f,"equipment shares delayed root clock");
      check(!t.push({220,2,1,1,80,1000})&&!t.push({179,2,2,1,50,1000}),"same-event weapon change and reordering refused");
      check(t.push({220,3,1,0,0,0})&&t.sample(220,o,a)&&!o.variant&&o.sword==1,"native cancellation retires draw scene");
      check(!t.sample(421,o,a),"stale equipment cannot drive attachment");t.clear();check(!t.sample(220,o,a),"pause clears equipment history");
      check(t.push({100,1,2,5,100,500})&&t.sample(100,o,a)&&o.variant==5&&o.sword==2,"fast recovery preserves observed sword state");
      t.clear();check(t.push({100,1,2,6,100,333})&&t.sample(100,o,a)&&o.variant==6&&o.sword==2,"crouched fast recovery preserves observed sword state");
    }
    const std::uintptr_t base=0x140000000ull;NativePlayerStates s{};unsigned action=99;s.defaultMode=true;
    {TraversalPresentation t;TraversalPresentation::Sample a{100,1,1,50,500,0,{-32,-147,66.8f},{-32,-146,67.8f}},b=a,o{};b.time=140;b.elapsed=90;float phase=0;
      check(t.push(a)&&t.push(b)&&t.sample(120,o,phase)&&std::fabs(phase-.14f)<.00001f,"traversal scene phase shares delayed root time");
      auto bad=b;bad.time=180;bad.goal[0]+=.1f;check(!t.push(bad),"same mantle event cannot change ledge");
      bad=b;bad.time=180;bad.elapsed=80;check(!t.push(bad),"mantle clock cannot rewind");
      check(!t.sample(99,o,phase)&&!t.sample(341,o,phase),"future and stale mantle samples refused");
      check(t.push({180,2,0,0,0,0,{},{}})&&t.sample(180,o,phase)&&!o.variant,"native exit retires mantle");t.clear();check(!t.sample(180,o,phase),"pause clears traversal");
    }
    {TraversalPresentation t;TraversalPresentation::Sample a{100,1,3,100,1000,0,{0,0,1},{0,2.6f,1},{0,1,1},.97f},b=a,o{};b.time=140;b.elapsed=300;float phase=0;
      check(t.push(a)&&t.push(b)&&t.sample(120,o,phase)&&std::fabs(phase-.2f)<.00001f,"vault interpolates measured geometric progress");
      check(t.sample(190,o,phase)&&std::fabs(phase-.3f)<.00001f,"time alone never advances vault over obstacle");
      auto invalid=b;invalid.time=180;invalid.height=1;check(!t.push(invalid),"vault event cannot change obstacle height");
      invalid=b;invalid.time=180;invalid.elapsed=299;check(!t.push(invalid),"vault progress cannot rewind");
      check(!t.sample(341,o,phase),"stale vault progress expires");
    }
    s.active={{9,1,base+0x1c86e18}};check(nativePlayerShadowAction(s,base,action)&&action==9,"exact native vault class reaches geometry and scene validation");
    s.active[0].vtable++;check(!nativePlayerShadowAction(s,base,action),"foreign vault class refused");
    s.active={{8,1,base+0x1c82f68}};check(nativePlayerShadowAction(s,base,action)&&action==8,"exact native mantle class reaches owned-scene validation");
    s.active[0].vtable++;check(!nativePlayerShadowAction(s,base,action),"wrong mantle class refused");
    s.active={{0,1,base+0x1c85628}};check(nativePlayerSwordAction(s,base,action)&&action==0,"native idle");
    s.active.push_back({39,2,base+0x1c85d28});check(nativePlayerSwordAction(s,base,action)&&action==1,"native slash");
    s.active[1]={44,2,base+0x1c81a68};check(nativePlayerSwordAction(s,base,action)&&action==2,"native block");
    s.active[1].vtable++;check(!nativePlayerSwordAction(s,base,action),"wrong native class");
    s.active={{0,1,base+0x1c85628},{39,2,base+0x1c85d28},{44,3,base+0x1c81a68}};check(!nativePlayerSwordAction(s,base,action),"ambiguous actions");
    s.active={{55,1,base+0x1c85628}};check(!nativePlayerSwordAction(s,base,action),"unknown state");
    s.active={{2,1,base+0x1c85028}};check(nativePlayerShadowAction(s,base,action)&&action==3,"native jump presentation");
    s.active={{3,1,base+0x1c845c8}};check(nativePlayerShadowAction(s,base,action)&&action==3,"native fall presentation");
    s.active.push_back({39,2,base+0x1c85d28});check(nativePlayerShadowAction(s,base,action)&&action==3,"airborne takes presentation priority over slash");
    s.active[0].vtable++;check(!nativePlayerShadowAction(s,base,action),"wrong airborne native class");
    s.active={{6,1,base+0x1c84258}};check(nativePlayerShadowAction(s,base,action)&&action==0,"sneak is grounded, not fake crouch playback");
    s.active={{1,1,base+0x1c85948}};check(nativePlayerShadowAction(s,base,action)&&action==4,"observed native sprint has its own semantic state");
    std::uint32_t motion=9;check(nativePlayerMotion(s,base,motion)&&motion==0,"sprint remains grounded root motion");
    for(auto overlay:{NativePlayerActiveState{53,2,base+0x1c79cf8},NativePlayerActiveState{54,2,base+0x1c78a68},NativePlayerActiveState{81,2,base+0x1c7df88}}){
      s.active={overlay,{6,1,base+0x1c84258}};check(nativePlayerMotion(s,base,motion)&&motion==1,"equipment preserves crouched root semantics independent of list order");
      check(nativePlayerShadowAction(s,base,action)&&action==(overlay.id==53?7u:0u),"equipment scene differs from request state");
      s.active[1]={2,1,base+0x1c85028};check(!nativePlayerMotion(s,base,motion)&&!nativePlayerShadowAction(s,base,action),"unobserved airborne equipment refuses semantics");
    }
    s.active={{1,1,base+0x1c85948}};
    s.active[0].vtable++;check(!nativePlayerShadowAction(s,base,action)&&!nativePlayerMotion(s,base,motion),"wrong sprint class refused");
    s.active={{1,1,base+0x1c85948},{39,2,base+0x1c85d28}};check(!nativePlayerShadowAction(s,base,action),"unobserved sprint attack combination refused");
    for(auto ground:{NativePlayerActiveState{0,1,base+0x1c85628},NativePlayerActiveState{6,1,base+0x1c84258}}){
        s.active={ground,{18,2,base+0x1c82b28}};check(nativePlayerShadowAction(s,base,action)&&action==5,"exact observed standing/crouched lean overlay");
        check(nativePlayerMotion(s,base,motion)&&motion==(ground.id==6?1u:0u),"lean retains underlying native stance");
        s.active[1].vtable++;check(!nativePlayerShadowAction(s,base,action),"wrong lean class refused");
    }
    s.active={{2,1,base+0x1c85028},{18,2,base+0x1c82b28}};check(!nativePlayerShadowAction(s,base,action),"unobserved airborne lean refused");
    s.active={{7,1,base+0x1c86108}};s.player.current=5;check(nativePlayerShadowAction(s,base,action)&&action==6,"exact sliding state and observed physics stance");
    s.player.current=0;check(nativePlayerShadowAction(s,base,action)&&action==6,"native slide exit can change capsule stance before retiring ID7");
    s.player.current=2;check(!nativePlayerShadowAction(s,base,action),"unobserved slide stance refused");s.player.current=5;
    s.active[0].vtable++;check(!nativePlayerShadowAction(s,base,action),"wrong slide class refused");s.player.current=0;
    s.active={{0,1,base+0x1c85628}};s.defaultMode=false;check(!nativePlayerSwordAction(s,base,action),"non-default player mode");
    std::cout<<"PASS: clone action roles/tokens/worlds, replay/event integrity, phase/freshness, and exact native slash/block classes\n";
}
