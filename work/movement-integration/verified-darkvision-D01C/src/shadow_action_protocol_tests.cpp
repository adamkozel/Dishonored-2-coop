#include "shadow_action_protocol.h"
#include "native_player_action.h"
#include "combat_presentation.h"
#include "equipment_presentation.h"
#include "power_fx_protocol.h"
#include "native_player_blink.h"
#include "grapple_fx_protocol.h"
#include "movement_semantics.h"
#include <iostream>
#include <cstdlib>
void check(bool ok,const char* m){if(!ok){std::cerr<<"FAIL: "<<m<<'\n';std::exit(1);}}
int main(){
    {MovementSemantics t;MovementSemantics::Sample o{};
      check(t.push({100,1,3,0,0,1,2,1,0})&&t.push({140,1,3,0,0,0,0,2,0})&&t.push({180,2,1}),"airborne and landed combat timeline");
      check(t.sample(179,o)&&o.state==3&&o.airStage==2,"future landed attack cannot retire delayed falling body");
      check(t.sample(120,o)&&o.airStage==1&&o.startZ==1&&o.targetZ==2,"jump height comes from the same presentation instant");
      check(t.sample(180,o)&&o.state==1&&!o.airStage,"landing changes at the source boundary");
      check(!t.push({220,2,0}),"state change requires a new semantic event");
      check(!t.sample(381,o),"stale movement state expires");
      t.clear();check(!t.sample(180,o),"pause removes queued movement states");
      check(t.push({100,1,5,-1,0})&&t.push({140,1,5,1,1})&&t.sample(120,o)&&std::fabs(o.leanSide)<1e-6f&&std::fabs(o.leanUp-.5f)<1e-6f,"lean axes interpolate on the body clock");
      check(!t.push({160,1,5,NAN,0})&&!t.push({130,1,5,0,0}),"invalid axes and reordered states refused");
    }
    CoopPosePacket h{},c{};h.role=0;c.role=1;h.sequence=c.sequence=100;h.flags=c.flags=1;h.session=11;c.session=22;h.sampleTimeUs=c.sampleTimeUs=1000000;
    h.worldEpoch=3;c.worldEpoch=7;h.mapId=c.mapId=99;h.gameLogicState=c.gameLogicState=1;h.cinematicMetadataValid=c.cinematicMetadataValid=1;
    {
      CoopGrappleFxPacket g{};g.sequence=1;g.sourceSession=22;g.targetSession=11;g.sourceWorldEpoch=7;g.targetWorldEpoch=3;g.mapId=99;g.sourceToken=9;g.targetToken=4;g.event=1;g.sampleTimeUs=1000000;g.rootSequence=99;g.phase=1;g.normal[2]=1;
      check(validGrappleFxPacket(g,h,c),"Far Reach exact Emily-to-Corvo scope");auto bad=g;bad.role=0;check(!validGrappleFxPacket(bad,h,c),"Corvo cannot emit Far Reach");
      bad=g;bad.targetWorldEpoch++;check(!validGrappleFxPacket(bad,h,c),"Far Reach wrong world");bad=g;bad.goal[0]=NAN;check(!validGrappleFxPacket(bad,h,c),"Far Reach nonfinite endpoint");
      bad=g;bad.progress=1.1f;check(!validGrappleFxPacket(bad,h,c),"Far Reach extension bounds");bad=g;bad.normal={0,0,0};check(!validGrappleFxPacket(bad,h,c),"Far Reach requires real surface normal");
      bad=g;bad.goal[0]=31;check(!validGrappleFxPacket(bad,h,c),"Far Reach target distance bounded");auto paused=h;paused.gameLogicState=2;check(!validGrappleFxPacket(g,paused,c),"Far Reach pause refuses data");
      GrappleFxTimeline line;CoopGrappleFxPacket out{};check(line.push(g,100)&&!line.push(g,101),"Far Reach duplicate is inert");
      auto b=g;b.sequence++;b.sampleTimeUs+=40000;b.progress=1;check(line.push(b,140)&&line.sample(120,out)&&std::fabs(out.progress-.5f)<.00001f,"beam extension shares delayed root clock");
      bad=b;bad.sequence++;bad.sampleTimeUs+=40000;bad.goal[0]=.1f;check(!line.push(bad,180),"static-world cast cannot change target mid-event");
      b.sequence++;b.sampleTimeUs+=40000;b.phase=2;check(line.push(b,180)&&line.sample(180,out)&&out.phase==2,"native pull produces impact phase");
      b.sequence++;b.sampleTimeUs+=40000;b.phase=3;check(line.push(b,220)&&line.sample(220,out)&&out.phase==3,"native end retires tether");
      b.sequence++;b.sampleTimeUs+=40000;b.phase=1;check(!line.push(b,260),"ended cast cannot replay");b.event++;check(line.push(b,260),"new native event accepted after prior end");
      check(!line.sample(441,out),"stale beam stream expires");GrappleFxTimeline empty;b.phase=2;check(!empty.push(b,300),"orphan pull cannot spawn beam or impact");
    }
    {
      CoopPowerFxPacket fx{};fx.sequence=1;fx.sourceSession=11;fx.targetSession=22;fx.sourceWorldEpoch=3;fx.targetWorldEpoch=7;fx.mapId=99;fx.sourceToken=4;fx.targetToken=9;fx.event=1;fx.sampleTimeUs=1000000;fx.rootSequence=99;fx.kind=1;
      check(validPowerFxPacket(fx,c,h),"scoped native Blink departure");auto badFx=fx;badFx.role=1;check(!validPowerFxPacket(badFx,c,h),"Emily cannot emit Corvo Blink effects");
      badFx=fx;badFx.targetWorldEpoch++;check(!validPowerFxPacket(badFx,c,h),"old world effects refused");badFx=fx;badFx.sourceToken=0;check(!validPowerFxPacket(badFx,c,h),"effects require opt-in incarnation tokens");
      badFx=fx;badFx.position[2]=NAN;check(!validPowerFxPacket(badFx,c,h),"nonfinite world FX refused");badFx=fx;badFx.kind=3;check(!validPowerFxPacket(badFx,c,h),"unimplemented effects refused");
      auto paused=c;paused.gameLogicState=2;check(!validPowerFxPacket(fx,paused,h),"paused peer cannot accept effects");
      PowerFxOrder order;check(order.accept(fx),"departure establishes event");check(!order.accept(fx),"duplicate departure refused");
      auto arrive=fx;arrive.sequence=2;arrive.kind=2;arrive.sampleTimeUs+=100000;arrive.position[1]=4;
      PowerFxOrder orphan;check(!orphan.accept(arrive),"arrival without departure refused");check(order.accept(arrive),"arrival closes same event");check(!order.accept(arrive),"duplicate arrival refused");
      auto replay=arrive;replay.sequence++;replay.sampleTimeUs++;check(!order.accept(replay),"new packet sequence cannot repeat completed arrival");
      auto next=fx;next.event=2;next.sequence=3;next.sampleTimeUs=2000000;check(order.accept(next),"new native Blink event");
      auto excessive=next;excessive.sequence++;excessive.sampleTimeUs+=100000;excessive.kind=2;excessive.position[0]=26;check(!order.accept(excessive),"out-of-range arrival refused");
      excessive.position[0]=0;excessive.sampleTimeUs=5000001;check(!order.accept(excessive),"expired arrival refused");
      const float geometry[]={-33.5f,-149.189f,66.424f,-33.5f,-149.189f,68.174f,-33.5f,-152.845f,68.154f};const float origin[]={-33.5f,-151.845f,68.159f};NativePlayerBlink blink{};
      check(nativeBlinkGeometry(geometry,origin,false,blink)&&std::fabs(blink.foot[2]-66.409f)<.0001f&&std::fabs(blink.start[2]-66.404f)<.0001f,"observed Blink eye origin converts to native foot position");
      check(nativeBlinkGeometry(geometry,geometry,true,blink)&&std::fabs(blink.foot[2]-66.424f)<.0001f,"completed Blink position is already foot based");
      float badGeometry[9];std::memcpy(badGeometry,geometry,sizeof(geometry));badGeometry[5]=badGeometry[2];check(!nativeBlinkGeometry(badGeometry,origin,false,blink),"unknown Blink height relation refused");
      std::memcpy(badGeometry,geometry,sizeof(geometry));badGeometry[3]+=1;check(!nativeBlinkGeometry(badGeometry,origin,false,blink),"mismatched foot and eye targets refused");
    }
    CoopShadowActionPacket p{};p.sampleTimeUs=1000000;p.sequence=1;p.sourceSession=11;p.targetSession=22;p.sourceWorldEpoch=3;p.targetWorldEpoch=7;p.mapId=99;p.sourceToken=4;p.rootSequence=99;p.character=1;
    check(validShadowAction(p,c,h),"initial hello");p.targetToken=9;check(validShadowAction(p,c,h)&&shadowActionTokensMatch(p,9,4),"two tokens");
    p.kind=1;p.event=1;p.swordState=2;for(unsigned state=0;state<=6;++state){p.state=state;p.airStage=state==3?2:0;p.combatVariant=state==1?1:state==2?10:0;p.combatPhase=p.combatVariant?1:0;p.combatDurationMs=p.combatVariant?1000:0;check(validShadowAction(p,c,h),"known native state");}p.state=5;
    p.leanSide=-1;p.leanUp=.5f;check(validShadowAction(p,c,h),"bounded native lean axes");p.leanSide=1.01f;check(!validShadowAction(p,c,h),"excess lean refused");
    p.leanSide=NAN;check(!validShadowAction(p,c,h),"nonfinite lean refused");p.leanSide=p.leanUp=0;
    check(!shadowActionTokensMatch(p,8,4)&&!shadowActionTokensMatch(p,9,5),"old tokens");
    auto bad=p;bad.event=0;check(!validShadowAction(bad,c,h),"state without event");
    {auto landing=p;landing.state=11;landing.traversalVariant=6;landing.traversalDurationMs=1000;landing.traversalTimeMs=200;
      check(validShadowAction(landing,c,h),"owned hard landing semantic packet");landing.swordState=1;check(validShadowAction(landing,c,h),"holstered landing packet");
      auto bad=landing;bad.traversalDurationMs=999;check(!validShadowAction(bad,c,h),"unobserved landing duration refused");
      bad=landing;bad.traversalTimeMs=1001;check(!validShadowAction(bad,c,h),"expired landing refused");
      bad=landing;bad.traversalGoal[2]=1;check(!validShadowAction(bad,c,h),"landing cannot carry root trajectory");
      bad=landing;bad.traversalEndStance=1;check(validShadowAction(bad,c,h),"observed native crouch latch can select landing recovery");
      bad.traversalEndStance=2;check(!validShadowAction(bad,c,h),"unknown landing exit refused");
      bad=landing;bad.state=8;check(!validShadowAction(bad,c,h),"mantle cannot impersonate landing");
    }
    {auto keyhole=p;keyhole.state=12;keyhole.swordState=1;keyhole.traversalVariant=7;keyhole.traversalDurationMs=1500;keyhole.traversalTimeMs=200;
      check(validShadowAction(keyhole,c,h),"owned native keyhole entry packet");
      for(unsigned v=7;v<=16;++v){auto phase=keyhole;const auto k=(v-7)%5;phase.traversalVariant=v;phase.traversalDurationMs=k==0||k==3?1500:k==1?500:1000;phase.traversalEndStance=k>=3;check(validShadowAction(phase,c,h),"mirrored native keyhole phase packet");}
      auto bad=keyhole;bad.swordState=2;check(!validShadowAction(bad,c,h),"keyhole cannot retain drawn sword");
      bad=keyhole;bad.traversalPoint[0]=1;check(!validShadowAction(bad,c,h),"keyhole cannot inject actor root geometry");
      bad=keyhole;bad.state=8;check(!validShadowAction(bad,c,h),"mantle cannot impersonate keyhole");
      bad=keyhole;bad.version=17;check(!validShadowAction(bad,c,h),"pre-keyhole protocol refused");
      bad=keyhole;bad.traversalVariant=8;bad.traversalDurationMs=500;bad.traversalTimeMs=3400;check(validShadowAction(bad,c,h),"held keyhole uses accumulated source clock");
    }
    {auto power=p;power.state=10;power.powerVariant=1;power.powerDurationMs=333;power.powerTimeMs=80;
      check(validShadowAction(power,c,h),"native Corvo Blink phase packet");
      auto emily=power;emily.role=1;emily.character=2;emily.powerVariant=7;emily.powerDurationMs=266;auto eh=h,ec=c;eh.role=1;ec.role=0;
      check(validShadowAction(emily,ec,eh),"native Emily Far Reach pull phase packet");
      auto dark=power;dark.powerVariant=10;dark.powerDurationMs=933;check(validShadowAction(dark,c,h),"native Corvo Dark Vision phase packet");
      auto femaleDark=emily;femaleDark.powerVariant=10;femaleDark.powerDurationMs=933;check(validShadowAction(femaleDark,ec,eh),"native Emily Dark Vision phase packet");
      auto oldDark=dark;oldDark.version=18;check(!validShadowAction(oldDark,c,h),"pre-Dark Vision packet version refused");
      oldDark=dark;oldDark.powerVariant=11;check(!validShadowAction(oldDark,c,h),"unobserved power variant refused");
      auto invalid=power;invalid.state=0;check(!validShadowAction(invalid,c,h),"idle cannot carry power phase");
      invalid=power;invalid.powerVariant=4;invalid.powerDurationMs=200;check(!validShadowAction(invalid,c,h),"Corvo cannot claim Emily starting power");
      invalid=power;invalid.swordState=1;check(!validShadowAction(invalid,c,h),"initial armed power study rejects missing sword");
      invalid=power;invalid.powerDurationMs=334;check(!validShadowAction(invalid,c,h),"unknown native power duration refused");
      invalid=power;invalid.powerTimeMs=3600001;check(!validShadowAction(invalid,c,h),"unbounded power clock refused");
      invalid=power;invalid.version=13;check(!validShadowAction(invalid,c,h),"pre-power packet version refused");
      PowerPresentation t;PowerPresentation::Sample s{};float amount=0;
      check(t.push({100,1,1,0,333})&&t.push({140,1,1,40,333})&&t.sample(120,s,amount)&&std::fabs(amount-20.f/333)<.00001f,"power uses delayed root timeline");
      check(!t.push({160,1,2,60,333})&&!t.push({139,1,1,50,333}),"same-event phase mutation and reorder refused");
      check(t.push({180,2,2,320,333})&&t.push({220,2,2,360,333})&&t.sample(200,s,amount)&&std::fabs(amount-7.f/333)<.00001f,"native power loop wraps without changing event");
      check(t.push({260,3,0,0,0})&&t.sample(260,s,amount)&&s.variant==0,"power release stops only on observed native phase end");
      check(!t.sample(461,s,amount),"stale powers retire");t.clear();check(!t.sample(260,s,amount),"world reset clears power timeline");
    }
    {auto climb=p;climb.state=8;climb.traversalVariant=1;climb.traversalTimeMs=250;climb.traversalDurationMs=500;climb.traversalStart={-32,-147,66.8f};climb.traversalGoal={-32,-146,67.8f};
      check(validShadowAction(climb,c,h),"observed native low mantle packet");
      auto invalid=climb;invalid.state=0;check(!validShadowAction(invalid,c,h),"ground cannot smuggle traversal");
      invalid=climb;invalid.traversalGoal[2]=NAN;check(!validShadowAction(invalid,c,h),"nonfinite ledge refused");
      invalid=climb;invalid.traversalGoal[0]+=10;check(!validShadowAction(invalid,c,h),"unbounded traversal target refused");
      invalid=climb;invalid.traversalDurationMs=700;check(!validShadowAction(invalid,c,h),"unobserved duration refused");
      invalid=climb;invalid.traversalEndStance=1;check(!validShadowAction(invalid,c,h),"unobserved low-mantle crouched recovery refused");
      invalid=climb;invalid.traversalVariant=2;invalid.traversalEndStance=1;check(validShadowAction(invalid,c,h),"observed crouched mantle has an explicit low exit");
      invalid.traversalEndStance=0;check(!validShadowAction(invalid,c,h),"crouched mantle cannot claim standing exit");
      auto high=climb;high.traversalVariant=4;high.traversalDurationMs=833;high.traversalGoal[2]=68.7f;
      check(validShadowAction(high,c,h),"native high mantle uses its observed 833ms scene");
      invalid=high;invalid.version=14;check(!validShadowAction(invalid,c,h),"old semantic protocol cannot masquerade as high-mantle capable");
      invalid=high;invalid.traversalDurationMs=500;check(!validShadowAction(invalid,c,h),"high mantle cannot use the low scene clock");
      invalid=high;invalid.traversalTimeMs=834;check(!validShadowAction(invalid,c,h),"expired high mantle refused");
      high.traversalVariant=5;high.traversalEndStance=1;check(validShadowAction(high,c,h),"high crouched mantle carries the native low exit");
      high.traversalEndStance=0;check(!validShadowAction(high,c,h),"high crouched mantle cannot claim a standing exit");
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
    {TraversalPresentation t;TraversalPresentation::Sample a{100,1,6,300,1000,0,{},{},{},0},b=a,o{};b.time=140;b.elapsed=340;b.endStance=1;float phase=0;
      check(t.push(a)&&t.push(b),"native crouch request changes within same landing scene without restarting its clock");
      check(t.sample(139,o,phase)&&o.endStance==0&&t.sample(140,o,phase)&&o.endStance==1,"landing exit stance changes only at source presentation time");
      b.time=180;b.elapsed=380;b.endStance=0;check(t.push(b),"native crouch cancellation during landing");
      b.time=220;b.elapsed=379;check(!t.push(b),"changing exit cannot rewind landing clock");
    }
    {TraversalPresentation t;TraversalPresentation::Sample a{100,1,8,450,500,1},b=a,o{};b.time=180;b.elapsed=530;float amount=0;
      check(t.push(a)&&t.push(b)&&t.sample(150,o,amount)&&std::fabs(amount-float(500./(9500./3.)))<1e-5f,"keyhole hold uses continuous native time and natural donor cadence across native 500ms loops");
      auto bad=b;bad.time=200;bad.elapsed=30;check(!t.push(bad),"native loop restart cannot rewind accumulated hold clock");
      bad=b;bad.time=200;bad.variant=16;bad.elapsed=0;bad.duration=1000;check(!t.push(bad),"keyhole phase change requires fresh event even when native scene handle is unchanged");
      bad.event++;check(t.push(bad)&&t.sample(199,o,amount)&&o.variant==8&&t.sample(200,o,amount)&&o.variant==16,"keyhole exit waits for delayed phase boundary");
      bad.time=240;bad.event++;bad.variant=9;bad.endStance=1;check(!t.push(bad),"standing exit cannot claim crouched recovery");
      t.clear();check(!t.sample(200,o,amount),"pause retires keyhole hold");
    }
    {s.active={{0,1,base+0x1c85628},{75,2,base+0x1c86ab8}};unsigned motion=99;
    check(nativePlayerShadowAction(s,base,action)&&action==0&&nativePlayerMotion(s,base,motion)&&motion==0,"native held interaction preserves ground before keyhole entry");
    s.active[0]={6,1,base+0x1c84258};check(nativePlayerShadowAction(s,base,action)&&action==0&&nativePlayerMotion(s,base,motion)&&motion==1,"crouched native held interaction preserves ground stance");
    s.active[0]={2,1,base+0x1c85028};check(!nativePlayerShadowAction(s,base,action)&&!nativePlayerMotion(s,base,motion),"unobserved airborne interaction refused");}
    s.active={{24,1,base+0x1c7c4a8}};check(nativePlayerShadowAction(s,base,action)&&action==12,"exact Peeking reaches owned scene validation");
    s.active[0].vtable++;check(!nativePlayerShadowAction(s,base,action),"foreign Peeking class refused");
    s.active={{4,1,base+0x1c82158}};check(nativePlayerShadowAction(s,base,action)&&action==11,"exact CatFall reaches body scene validation");
    s.active[0].vtable++;check(!nativePlayerShadowAction(s,base,action),"foreign CatFall class refused");
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
