#include "shadow_action_protocol.h"
#include "native_possession_context.h"
#include "native_player_action.h"
#include "combat_presentation.h"
#include "equipment_presentation.h"
#include "power_fx_protocol.h"
#include "mesmerize_world_protocol.h"
#include "domino_world_protocol.h"
#include "swarm_world_protocol.h"
#include "doppelganger_world_protocol.h"
#include "presentation_refusal.h"
#include "swarm_spawn_fx_protocol.h"
#include "possession_fx_protocol.h"
#include "swarm_world_presentation.h"
#include "power_failure_fx_policy.h"
#include "native_player_blink.h"
#include "grapple_fx_protocol.h"
#include "windblast_fx_protocol.h"
#include "ranged_shot_observation.h"
#include "movement_semantics.h"
#include "native_presentation_fallback.h"
#include "native_player_trap_scene.h"
#include "native_player_choke_scene.h"
#include <iostream>
#include <cstdlib>
void check(bool ok,const char* m){if(!ok){std::cerr<<"FAIL: "<<m<<'\n';std::exit(1);}}
#include "domino_world_protocol_tests.h"
#include "swarm_world_protocol_tests.h"
#include "doppelganger_world_protocol_tests.h"
#include "swarm_spawn_fx_protocol_tests.h"
#include "possession_fx_protocol_tests.h"
void rangedShotTests(){
    RangedShotObservation o;float root[3]{};NativeWindblastFxRecord a{};
    a.id=10;a.ownerActor=123;a.name="models/effects/weapons/gun/muzzle_1p_corvo.fx";
    check(!o.observe(1,2,50,0,true,root,{a}),"stream re-arm baselines existing muzzle effects");
    check(!o.observe(1,2,50,0,true,root,{a}),"old native flash cannot become a shot");
    a.id=11;check(o.observe(1,2,50,0,true,root,{a})==1,"fresh matching native first-person flash confirms gun scene");
    check(o.observe(1,2,50,0,true,root,{})==1,"confirmed shot survives short native effect lifetime");
    check(!o.observe(1,2,51,0,false,root,{a}),"empty or reload scene has no muzzle flash");
    check(!o.observe(1,2,52,0,true,root,{a}),"one native flash cannot confirm another scene");
    a.id=12;a.name="models/effects/weapons/gun/muzzle_1p_emily.fx";
    check(!o.observe(1,2,52,0,true,root,{a}),"other role flash refused");
    a.id=13;a.name="models/effects/weapons/gun/muzzle_1p_corvo.fx";a.origin[0]=5;
    check(!o.observe(1,2,52,0,true,root,{a}),"distant flash refused");
    a.id=14;a.origin[0]=0;auto b=a;b.id=15;
    check(!o.observe(1,2,52,0,true,root,{a,b}),"ambiguous native flashes refused");
    a.id=16;a.ownerActor=0;check(!o.observe(1,2,52,0,true,root,{a}),"retired native owner refused");
    a.id=17;a.ownerActor=123;check(o.observe(1,2,52,0,true,root,{a}),"next unique live flash accepted");
    check(!o.observe(3,4,52,0,true,root,{a}),"new token pair cannot replay current shot");
    a.id=18;a.name="models/effects/weapons/gun/muzzle_1p_emily.fx";
    check(o.observe(3,4,53,1,true,root,{a}),"Emily uses her own native flash identity");
    a.id=19;a.name="models/effects/d2coop/muzzle_remote_emily.fx";
    check(!o.observe(3,4,54,1,true,root,{a}),"remote cosmetic Emily effect cannot feed back into native shot confirmation");
    a.id=20;a.name="models/effects/d2coop/muzzle_remote_corvo.fx";
    check(!o.observe(3,4,55,0,true,root,{a}),"remote cosmetic Corvo effect cannot feed back into native shot confirmation");
    RangedPresentation t;RangedPresentation::Sample s{};float amount=0;
    check(t.push({100,1,1,3,100,666,0,0})&&t.push({133,1,1,3,133,666,0,1}),"native shot confirmation can arrive inside one fire scene");
    check(t.sample(132,s,amount)&&!s.shot&&t.sample(133,s,amount)&&s.shot,"flash confirmation follows delayed source boundary");
    check(!RangedPresentation::valid({0,1,2,7,0,666,0,1})&&!RangedPresentation::valid({0,1,1,6,0,333,0,1}),"crossbow and empty actions cannot flash");
    check(!RangedPresentation::valid({0,1,1,3,0,666,0,2}),"unknown shot flag rejected");
}
#include "carry_presentation_tests.h"
#include "grenade_presentation_tests.h"
void unboundGroundTests(){
    const std::uintptr_t base=0x140000000;NativePlayerStates s{};s.defaultMode=true;s.player.logic=1;
    s.active={{0,1,base+0x1c85628},{53,2,base+0x1c79cf8}};
    check(nativePresentationFallbackGround(s,base),"owned standing physical mode allows unbound equipment presentation");
    s.player.current=1;s.active[0]={6,1,base+0x1c84258};
    check(nativePresentationFallbackGround(s,base),"owned crouched physical mode allows unbound equipment presentation");
    s.defaultMode=false;check(!nativePresentationFallbackGround(s,base),"alternate power/cutscene mode cannot become ground fallback");s.defaultMode=true;
    s.player.logic=2;check(!nativePresentationFallbackGround(s,base),"paused native state cannot become fallback");s.player.logic=1;
    s.active[1].vtable++;check(!nativePresentationFallbackGround(s,base),"unknown native class still rejected");s.active[1].vtable--;
    s.player.current=0;s.active={{14,1,base+0x1c86768}};check(!nativePresentationFallbackGround(s,base),"stun has no confirmed ground locomotion fallback");
    s.active={{2,1,base+0x1c85028}};check(!nativePresentationFallbackGround(s,base),"airborne state cannot become ground fallback");
    unsigned sword=99;check(!readNativePresentationFallback([](auto,auto,auto){return false;},base,1,2,1,sword)&&sword==99,"failed owner read cannot publish fallback equipment");
    sword=99;check(!readNativePresentationHold([](auto,auto,auto){return false;},base,1,2,1,sword)&&sword==99,"failed owner read cannot publish hold equipment");
    MovementSemantics timeline;MovementSemantics::Sample sample{};sample.time=100;sample.event=1;sample.state=18;
    check(timeline.push(sample),"explicit unbound state accepted on normal delayed movement timeline");
    sample.time=133;sample.airStage=2;check(!timeline.push(sample),"unbound state cannot smuggle airborne semantics");
}
void chokePresentationTests(){
    const std::uintptr_t base=0x140000000;NativePlayerStates native{};native.player.logic=1;native.defaultMode=true;native.active={{41,1,base+0x1c77c18}};
    unsigned motion=99;check(nativePlayerMotion(native,base,motion)&&motion==0,"exclusive native choke carries standing root");
    check(!nativePresentationFallbackGround(native,base),"unknown choke phase cannot infer ordinary ground fallback");
    native.active[0].vtable++;check(!nativePlayerMotion(native,base,motion),"wrong choke class cannot publish root");native.active[0].vtable--;
    native.player.current=12;check(nativePlayerGroundChokeStates(native,base),"recorded native choke stance12 remains owned by exact state41");
    {unsigned stance=99;check(readNativePlayerPresentationStance([](auto,auto,auto){return false;},base,native,stance)&&stance==0,"choke hold stance12 presents standing on the wire (1213r)");}
    native.player.current=13;check(!nativePlayerGroundChokeStates(native,base),"neighboring unobserved physical state refused");
    native.player.current=1;check(!nativePlayerMotion(native,base,motion),"unobserved crouched choke not inferred");native.player.current=0;
    native.active.push_back({0,2,base+0x1c85628});check(!nativePlayerGroundChokeStates(native,base),"unsupported concurrent choke state refused");
    check(nativeChokeVariant("midnight/gameplay/player/base/choke/choke_combat_lose.midnightscene","Npc",1500)==4&&
          !nativeChokeVariant("midnight/gameplay/player/base/choke/choke_cancel.midnightscene","Npc",400)&&
          !nativeChokeVariant("midnight/gameplay/player/base/choke/choke_combat_lose.midnightscene","Npc",400),"native combat lose distinguished from cancel");
    check(ChokePresentation::valid({0,1,4,750,1500,31,41})&&!ChokePresentation::valid({0,1,4,1501,1500,31,41}),"breakout owns exactly1500ms");
    native.active.resize(1);native.active[0].id=13;native.active[0].vtable=base+0x1c83468;native.player.current=0;
    check(nativePlayerGroundAssassinationStates(native,base)&&nativePlayerMotion(native,base,motion)&&motion==0,"native assassination carries independent standing root");
    check(!nativePlayerGroundChokeStates(native,base)&&!nativePresentationFallbackGround(native,base),"lethal identity does not admit choke or generic fallback");
    native.player.current=1;check(nativePlayerGroundAssassinationStates(native,base)&&nativePlayerMotion(native,base,motion)&&motion==0,"observed crouched fast-back lethal (1244b) keeps its crouched stance");
    native.player.current=12;check(!nativePlayerGroundAssassinationStates(native,base),"unobserved assassination stance refused");
    check(nativeChokeVariant("midnight/gameplay/player/base/assassination/assassination_fast_back.midnightscene","Npc",1333)==5&&
          !nativeChokeVariant("midnight/gameplay/player/base/assassination/assassination_fast_front.midnightscene","Npc",1333),"only observed native fast-back lethal admitted");
    check(nativeChokeVariant("midnight/gameplay/player/base/assassination/assassination_back.midnightscene","npc",1933)==6&&!nativeChokeVariant("midnight/gameplay/player/base/assassination/assassination_back.midnightscene","Npc",1933)&&!nativeChokeVariant("midnight/gameplay/player/base/assassination/assassination_back.midnightscene","npc",1333)&&
          ChokePresentation::clip(6)==ChokePresentation::clip(5)&&ChokePresentation::duration(6)==1933,"observed assassination_back (1248j) presented with the fast-back donor on its own clock");
    ChokePresentation t;ChokePresentation::Sample out{};float amount=0;
    check(t.push({100,1,1,200,600,31,41})&&t.push({140,1,1,240,600,31,41}),"confirmed choke clock accepted");
    check(t.sample(120,out,amount)&&std::fabs(amount-220.f/600)<1e-6f,"choke uses delayed native clock");
    check(!t.push({150,1,1,250,600,32,41})&&!t.push({150,1,1,239,600,31,41}),"target change and rewind cannot reuse event");
    check(t.push({180,2,2,0,1400,31,41})&&t.sample(179,out,amount)&&out.variant==1,"future choke loop cannot replace older root frame");
    check(!t.sample(381,out,amount),"stale choke expires");
    check(!ChokePresentation::valid({0,1,3,0,1800,0,41})&&!ChokePresentation::valid({0,1,4,0,400,31,41}),"targetless and unobserved cancel refused");
    check(nativeChokeVariant("midnight/gameplay/player/base/choke/choke_win.midnightscene","Npc",1800)==3&&
          !nativeChokeVariant("midnight/gameplay/player/base/choke/choke_win.midnightscene","Player",1800),"native phase and target subscene exact");
}
void presentationHoldTests(){
    // 1213m: the product controller holds every timeline across a gap up to the 1 s lease; the lab rules are unchanged.
    setPresentationHold(true);
    {MovementSemantics t;MovementSemantics::Sample o{};
      check(t.push({100,1,0})&&t.push({140,1,0}),"product movement timeline accepted");
      check(t.sample(1100,o)&&o.state==0,"product hold keeps the last movement state through a 960 ms gap");
      check(!t.sample(1141,o),"product hold still expires after the 1 s lease");
      check(t.push({540,1,0})&&t.sample(300,o),"a 400 ms source gap no longer clears the delayed body timeline");}
    {RangedPresentation t;RangedPresentation::Sample o{};float phase=0;
      check(t.push({100,1,1,3,0,666})&&t.sample(433,o,phase)&&o.variant==3&&std::fabs(phase-.5f)<1e-6f,"a held action keeps running on real time through a gap");
      check(t.sample(1100,o,phase)&&phase==1.f,"held action progress stops at its end");}
    setPresentationHold(false);
    {MovementSemantics t;MovementSemantics::Sample o{};
      check(t.push({100,1,0})&&!t.sample(301,o),"lab hold still expires after 200 ms");
      check(t.push({400,1,0})&&!t.sample(300,o),"lab still clears the timeline on a 250 ms gap");}
    {RangedPresentation t;RangedPresentation::Sample o{};float phase=0;
      check(t.push({100,1,1,3,0,666})&&t.sample(300,o,phase)&&std::fabs(phase-100.f/666)<1e-6f,"lab action extrapolation still stops at 100 ms");}
}
int main(){presentationHoldTests();unboundGroundTests();chokePresentationTests();
    grenadePresentationTests();
    carryPresentationTests();
    rangedShotTests();
    {RangedPresentation t;RangedPresentation::Sample o{};float phase=0;
      check(t.push({100,1,1,3,100,666})&&t.push({140,1,1,3,140,666}),"owned gun clock accepted");
      check(t.sample(120,o,phase)&&std::fabs(phase-120.f/666)<1e-6f,"ranged clock interpolates with delayed body");
      check(!t.push({150,1,2,7,150,666})&&!t.push({150,1,1,3,139,666}),"same event cannot change weapon or reverse clock");
      check(t.push({180,2,1,4,0,1500})&&t.sample(179,o,phase)&&o.variant==3,"future reload does not interrupt delayed fire");
      check(t.sample(180,o,phase)&&o.variant==4&&phase==0,"reload begins at its scene boundary");
      check(!t.sample(381,o,phase),"stale ranged state expires");
      check(!RangedPresentation::valid({0,1,1,7,0,666})&&!RangedPresentation::valid({0,1,2,8,0,899}),"weapon-kind and duration mismatches refused");
      check(!RangedPresentation::valid({0,1,0,3,0,666})&&!RangedPresentation::valid({0,1,1,0,1,0}),"orphan firing and idle clock refused");
      t.clear();check(!t.sample(180,o,phase),"pause clears ranged presentation");
      check(t.push({100,1,2,8,400,900,2})&&t.push({180,1,2,8,480,900,1}),"native reload contact can change mechanism inside one scene");
      check(t.sample(179,o,phase)&&o.mechanism==2&&t.sample(180,o,phase)&&o.mechanism==1,"mechanical reload contact follows delayed source state without early interpolation");
      check(t.push({220,2,2,0,0,0,2})&&t.sample(220,o,phase)&&o.mechanism==2,"empty held weapon survives the end of its firing scene");
      check(!RangedPresentation::valid({0,1,1,0,0,0,1})&&!RangedPresentation::valid({0,1,2,0,0,0,3}),"mechanism enum requires a crossbow and a known value");
    }
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
      t.clear();
      check(t.push({100,1,10,0,0,66,67.1f,1,0})&&t.push({140,1,10,0,0,0,0,2,0})&&t.push({180,1,10}),"one native power event spans jump, fall and landing");
      check(t.sample(139,o)&&o.state==10&&o.airStage==1&&o.event==1,"rising cast samples native height on delayed clock");
      check(t.sample(179,o)&&o.airStage==2&&o.event==1,"landing cannot prematurely retire falling cast");
      check(t.sample(180,o)&&o.state==10&&!o.airStage&&o.event==1,"landing does not restart ongoing cast");
      check(!t.push({200,1,10,0,0,1,30,1,0})&&!t.push({200,1,10,0,0,1,2,0,0}),"power air validation rejects implausible height and orphan air data");
      t.clear();
      check(t.push({100,1,13,0,0,66,67.1f,1,0})&&t.push({140,1,13,0,0,0,0,2,0})&&t.push({180,1,13}),"one owned ranged event spans rising, falling and landed body states");
      check(t.sample(139,o)&&o.airStage==1&&o.event==1&&t.sample(179,o)&&o.airStage==2&&o.event==1,"ranged action retains delayed jump/fall boundary");
      check(t.sample(180,o)&&!o.airStage&&o.event==1,"landing does not restart a continuing ranged action");
      check(!t.push({200,1,13,0,0,1,2,2,0})&&!t.push({200,1,13,0,0,1,2,0,0}),"falling and grounded ranged action reject stale apex data");
    }
    CoopPosePacket h{},c{};h.role=0;c.role=1;h.sequence=c.sequence=100;h.flags=c.flags=1;h.session=11;c.session=22;h.sampleTimeUs=c.sampleTimeUs=1000000;
    h.worldEpoch=3;c.worldEpoch=7;h.mapId=c.mapId=99;h.gameLogicState=c.gameLogicState=1;h.cinematicMetadataValid=c.cinematicMetadataValid=1;
    {
      CoopWindblastFxPacket w{};w.sequence=1;w.sourceSession=11;w.targetSession=22;w.sourceWorldEpoch=3;w.targetWorldEpoch=7;w.mapId=99;
      w.sourceToken=4;w.targetToken=9;w.event=1;w.sampleTimeUs=1000000;w.rootSequence=99;w.nativeFxId=300;w.powerEvent=2;w.castTimeMs=200;w.position[2]=1.7f;w.axis={1,0,0,0,1,0,0,0,1};
      check(validWindblastFxPacket(w,c,h),"basic Windblast native origin and axis in scoped Corvo cast");
      auto bad=w;bad.role=1;check(!validWindblastFxPacket(bad,c,h),"Windblast wrong role refused");bad=w;bad.targetWorldEpoch++;check(!validWindblastFxPacket(bad,c,h),"Windblast old world refused");
      bad=w;bad.sourceToken=0;check(!validWindblastFxPacket(bad,c,h),"Windblast missing incarnation token refused");bad=w;bad.powerEvent=0;check(!validWindblastFxPacket(bad,c,h),"Windblast requires owning cast event");
      bad=w;bad.position[0]=5;check(!validWindblastFxPacket(bad,c,h),"Windblast cannot place effects beyond native body range");bad=w;bad.axis[0]=-1;check(!validWindblastFxPacket(bad,c,h),"mirrored effect transform refused");
      bad=w;bad.axis[3]=.4f;check(!validWindblastFxPacket(bad,c,h),"sheared effect transform refused");bad=w;bad.axis[2]=NAN;check(!validWindblastFxPacket(bad,c,h),"nonfinite effect transform refused");
      bad=w;bad.castTimeMs=1001;check(!validWindblastFxPacket(bad,c,h),"expired native cast refused");auto paused=c;paused.gameLogicState=2;check(!validWindblastFxPacket(w,paused,h),"paused receiver refuses Windblast");
      WindblastFxOrder order;check(order.accept(w)&&!order.accept(w),"duplicate reliable Windblast packet is inert");
      bad=w;bad.sequence++;bad.sampleTimeUs++;check(!order.accept(bad),"fresh sequence cannot replay one native FX");
      bad.event++;bad.powerEvent++;check(!order.accept(bad),"reused native FX ID cannot pose as new cast");
      bad.nativeFxId++;check(order.accept(bad),"new native FX and cast accepted");
      auto replay=bad;replay.sequence++;replay.sampleTimeUs++;replay.event++;replay.nativeFxId++;check(!order.accept(replay),"second effect cannot duplicate same cast");
    }
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
      // Regression: the reliable effect packet may reach presentation after
      // a kinematic root sample. Every intermediate sample must still expose
      // the same departure, including short teleports below the 3m reset.
      for(float y:{-152.7f,-151.955307f,-150.1f}){
        const float travelling[]={-33.5f,y,68.159f};blink.completed=false;
        check(nativeBlinkGeometry(geometry,travelling,false,blink),"coherent native Blink travel sample");
        const auto network=nativeBlinkNetworkPoint(blink);
        check(std::fabs(network[1]+152.845f)<.0001f&&std::fabs(network[2]-66.404f)<.0001f,
              "network root holds departure independently of FX arrival");
        check(std::fabs(blink.foot[1]-y)<.0001f,"native kinematic observation remains unchanged");
      }
      blink.completed=true;check(nativeBlinkGeometry(geometry,geometry,true,blink),"coherent completed native Blink");
      check(std::fabs(nativeBlinkNetworkPoint(blink)[1]+149.189f)<.0001f,
            "only native completion publishes destination");
      float badGeometry[9];std::memcpy(badGeometry,geometry,sizeof(geometry));badGeometry[5]=badGeometry[2];check(!nativeBlinkGeometry(badGeometry,origin,false,blink),"unknown Blink height relation refused");
      std::memcpy(badGeometry,geometry,sizeof(geometry));badGeometry[3]+=1;check(!nativeBlinkGeometry(badGeometry,origin,false,blink),"mismatched foot and eye targets refused");
    }
    {
      dominoPacketTests(h,c);
      swarmPacketTests(h,c);doppelgangerPacketTests(h,c);swarmSpawnFxTests(h,c);possessionFxTests(h,c);
      CoopMesmerizeWorldPacket m{};m.sequence=1;m.sourceSession=22;m.targetSession=11;m.sourceWorldEpoch=7;m.targetWorldEpoch=3;m.mapId=99;m.sourceToken=9;m.targetToken=4;m.event=1;m.sampleTimeUs=1000000;m.rootSequence=99;m.nativeHandle=0xc881200;m.state=1;m.transform={-33,-155,66,1,0,0,0,1,0,0,0,1};
      check(validMesmerizeWorldPacket(m,h,c),"Mesmerize exact Emily world cosmetic scope");
      auto bad=m;bad.role=0;check(!validMesmerizeWorldPacket(bad,h,c),"Corvo cannot author Mesmerize world");
      bad=m;bad.targetWorldEpoch++;check(!validMesmerizeWorldPacket(bad,h,c),"Mesmerize old world refused");
      bad=m;bad.transform[0]=NAN;check(!validMesmerizeWorldPacket(bad,h,c),"Mesmerize nonfinite origin refused");
      bad=m;bad.transform[3]=-1;check(!validMesmerizeWorldPacket(bad,h,c),"Mesmerize reflected basis refused");
      bad=m;bad.transform[7]=0;check(!validMesmerizeWorldPacket(bad,h,c),"Mesmerize degenerate basis refused");
      bad=m;bad.state=0;check(!validMesmerizeWorldPacket(bad,h,c),"Preview or unknown Mesmerize state refused");
      bad=m;bad.nativeHandle=0x3fff0010;check(!validMesmerizeWorldPacket(bad,h,c),"Absent handle cannot create Mesmerize");
      bad=m;bad.snapshot=2;check(!validMesmerizeWorldPacket(bad,h,c),"Unknown Mesmerize snapshot semantics refused");
      bad=m;bad.snapshot=1;check(!validMesmerizeWorldPacket(bad,h,c),"Spawn cannot claim an active snapshot");
      bad=m;bad.version=3;check(!validMesmerizeWorldPacket(bad,h,c),"Previous world wire cannot authorize active snapshots");
      auto eye=m;eye.eyesValid=1;eye.eyeCount=1;eye.eyes[0].id=77;eye.eyes[0].transform=m.transform;eye.eyes[0].transform[2]+=2;
      check(validMesmerizeWorldPacket(eye,h,c),"source-owned eye transform within committed object accepted");
      bad=eye;bad.version=1;check(!validMesmerizeWorldPacket(bad,h,c),"old world packet cannot convey eye state");
      bad=eye;bad.eyesValid=0;check(!validMesmerizeWorldPacket(bad,h,c),"unreadable source cannot invent eye snapshot");
      bad=eye;bad.eyeCount=6;check(!validMesmerizeWorldPacket(bad,h,c),"unbounded eye count refused");
      bad=eye;bad.eyes[0].id=0;check(!validMesmerizeWorldPacket(bad,h,c),"missing eye incarnation refused");
      bad=eye;bad.eyes[0].transform[2]+=20;check(!validMesmerizeWorldPacket(bad,h,c),"eye cannot leave its committed object");
      bad=eye;bad.eyes[0].transform[3]=-1;check(!validMesmerizeWorldPacket(bad,h,c),"reflected eye basis refused");
      bad=eye;bad.eyes[0].transform[2]=NAN;check(!validMesmerizeWorldPacket(bad,h,c),"nonfinite eye origin refused");
      bad=eye;bad.eyeCount=2;bad.eyes[1]=bad.eyes[0];check(!validMesmerizeWorldPacket(bad,h,c),"duplicate eye incarnation refused");
      bad=eye;bad.eyes[4].transform[0]=1;check(!validMesmerizeWorldPacket(bad,h,c),"unused eye payload must be canonical");
      eye.eyeCount=0;eye.eyes={};check(validMesmerizeWorldPacket(eye,h,c),"confirmed absence distinct from unreadable eye snapshot");
      auto links=m;links.linksValid=1;links.linkCount=3;
      for(unsigned i=0;i<3;++i){auto& fx=links.links[i];fx.id=400+i;fx.target=0x1234001;fx.kind=i+1;fx.transform=m.transform;fx.transform[0]+=3;if(i<2)fx.other=m.transform;}
      check(validMesmerizeWorldPacket(links,h,c),"bounded source beam trail and head transforms accepted");
      bad=links;bad.version=2;check(!validMesmerizeWorldPacket(bad,h,c),"old eye-only wire cannot convey link payload");
      bad=links;bad.linksValid=0;check(!validMesmerizeWorldPacket(bad,h,c),"unknown link snapshot cannot carry effects");
      bad=links;bad.linkCount=16;check(!validMesmerizeWorldPacket(bad,h,c),"too many link effects refused");
      bad=links;bad.links[0].kind=4;check(!validMesmerizeWorldPacket(bad,h,c),"unknown effect kind refused");
      bad=links;bad.links[0].id=~0u;check(!validMesmerizeWorldPacket(bad,h,c),"invalid link incarnation refused");
      bad=links;bad.links[1].id=bad.links[0].id;check(!validMesmerizeWorldPacket(bad,h,c),"duplicate link incarnation refused");
      bad=links;bad.links[1].kind=1;check(!validMesmerizeWorldPacket(bad,h,c),"duplicate kind for same target refused");
      bad=links;bad.links[0].target=1;check(!validMesmerizeWorldPacket(bad,h,c),"local player is not a native Mesmerize victim");
      bad=links;bad.links[0].target=m.nativeHandle;check(!validMesmerizeWorldPacket(bad,h,c),"world object cannot be its own victim");
      bad=links;bad.links[0].transform[0]+=101;check(!validMesmerizeWorldPacket(bad,h,c),"unbounded target endpoint refused");
      bad=links;bad.links[0].other[0]+=11;check(!validMesmerizeWorldPacket(bad,h,c),"beam power endpoint must stay near committed object");
      bad=links;bad.links[0].other[3]=-1;check(!validMesmerizeWorldPacket(bad,h,c),"reflected beam endpoint refused");
      bad=links;bad.links[0].transform[2]=NAN;check(!validMesmerizeWorldPacket(bad,h,c),"nonfinite link endpoint refused");
      bad=links;bad.links[2].other[0]=1;check(!validMesmerizeWorldPacket(bad,h,c),"head effect has no invented second endpoint");
      bad=links;bad.links[14].target=7;check(!validMesmerizeWorldPacket(bad,h,c),"unused link payload must be canonical");
      bad=links;bad.eyesValid=1;bad.eyeCount=1;bad.eyes[0].id=400;bad.eyes[0].transform=m.transform;check(!validMesmerizeWorldPacket(bad,h,c),"same native effect cannot be an eye and a link");
      auto maximumLinks=links;maximumLinks.linkCount=15;
      for(unsigned i=0;i<15;++i){auto& fx=maximumLinks.links[i];fx.id=700+i;fx.target=0x1234001+i/3;fx.kind=i%3+1;fx.transform=m.transform;fx.other=i%3<2?m.transform:std::array<float,12>{};}
      check(validMesmerizeWorldPacket(maximumLinks,h,c),"five targets with three native effect types each fit bounded wire");
      bad=maximumLinks;bad.links[14].target=0x1234009;check(!validMesmerizeWorldPacket(bad,h,c),"sixth target refused even inside effect capacity");
      links.linkCount=0;links.links={};check(validMesmerizeWorldPacket(links,h,c),"confirmed empty target effects retire without guessed targets");
      MesmerizeWorldOrder order;check(order.accept(m)&&!order.accept(m),"Mesmerize incarnation starts once");
      auto loop=m;loop.sequence++;loop.sampleTimeUs+=100000;loop.state=3;
      MesmerizeWorldOrder orphan;check(!orphan.accept(loop),"Orphan active object cannot spawn");check(order.accept(loop),"Native active state follows spawn");
      bad=loop;bad.sequence++;bad.sampleTimeUs+=100000;bad.nativeHandle++;check(!order.accept(bad),"Handle replacement inside same event refused");
      bad=loop;bad.sequence++;bad.sampleTimeUs+=100000;bad.state=1;check(!order.accept(bad),"Active world cannot replay spawn");
      bad=loop;bad.sequence++;bad.sampleTimeUs+=100000;bad.transform[0]+=.03f;check(!order.accept(bad),"Static world anchor cannot drift");
      auto end=loop;end.sequence++;end.sampleTimeUs+=100000;end.state=2;check(order.accept(end),"Source native removal retires same event");
      bad=end;bad.sequence++;bad.sampleTimeUs+=100000;bad.state=3;check(!order.accept(bad),"Retired object cannot revive");
      auto next=m;next.sequence=4;next.sampleTimeUs=1500000;next.event++;next.nativeHandle++;check(order.accept(next),"New committed incarnation after retirement accepted");
      auto cancel=next;cancel.sequence++;cancel.sampleTimeUs+=100000;cancel.state=2;check(order.accept(cancel),"Spawn can retire without an active phase");
      auto snapshot=loop;snapshot.snapshot=1;
      check(validMesmerizeWorldPacket(snapshot,h,c),"Fresh scoped native active snapshot is valid");
      MesmerizeWorldOrder resumed;check(resumed.accept(snapshot),"Fresh stream may initialize an already active power");
      check(!resumed.accept(snapshot),"Snapshot duplicate cannot restart model or effects");
      auto update=snapshot;update.sequence++;update.sampleTimeUs+=100000;
      check(resumed.accept(update),"Active snapshot stream continues without a fabricated spawn");
      bad=update;bad.sequence++;bad.sampleTimeUs+=100000;bad.snapshot=0;check(!resumed.accept(bad),"Snapshot identity cannot change during active event");
      bad=update;bad.sequence++;bad.sampleTimeUs+=100000;bad.event++;bad.nativeHandle++;check(!resumed.accept(bad),"Another active snapshot cannot replace a live event");
      bad=update;bad.sequence++;bad.sampleTimeUs+=100000;bad.transform[0]+=.03f;check(!resumed.accept(bad),"Resumed power keeps its static native anchor");
      auto retired=update;retired.sequence++;retired.sampleTimeUs+=100000;retired.state=2;retired.snapshot=0;
      check(validMesmerizeWorldPacket(retired,h,c)&&resumed.accept(retired),"Source retirement ends resumed power");
      bad=retired;bad.sequence++;bad.sampleTimeUs+=100000;bad.state=3;bad.snapshot=1;check(!resumed.accept(bad),"Retired snapshot event cannot revive");
      bad=retired;bad.sequence++;bad.sampleTimeUs+=100000;bad.state=3;bad.snapshot=1;bad.event++;bad.nativeHandle++;check(!resumed.accept(bad),"Snapshot cannot replace retired event without a fresh stream");
      MesmerizeWorldOrder noRetired;check(!noRetired.accept(retired),"New stream cannot resurrect a retiring power");
      bad=retired;bad.snapshot=1;check(!validMesmerizeWorldPacket(bad,h,c),"Retirement never claims active snapshot semantics");
    }
    {auto wheelPeer=h;wheelPeer.pendingGameLogicState=8;CoopShadowActionPacket hello{};hello.sampleTimeUs=1000000;hello.sequence=1;hello.sourceSession=11;hello.targetSession=22;hello.sourceWorldEpoch=3;hello.targetWorldEpoch=7;hello.mapId=99;hello.sourceToken=4;hello.rootSequence=99;hello.character=1;
     check(validShadowAction(hello,c,h)&&validShadowAction(hello,c,wheelPeer),"action stream stays valid while the peer has the item wheel open (1083)");}
    {CoopPosePacket wheel=h;wheel.gameLogicState=8;check(!coopGameplayReady(wheel)&&coopPresentationReady(wheel),"native item wheel keeps presentation but not gameplay readiness (1074)");
     {auto unread=wheel;unread.cinematicMetadataValid=0;check(coopPresentationReady(unread),"wheel without native cinematic metadata stays presentation-ready (1075)");
      unread.cinematicMetadataValid=1;unread.cinematicNodeCount=2;check(!coopPresentationReady(unread),"wheel during a cinematic is not presentation-ready");}
     auto menu=h;menu.gameLogicState=2;check(!coopPresentationReady(menu),"pause menu is not presentation-ready");
     {auto opening=h;opening.pendingGameLogicState=8;check(coopPresentationReady(opening)&&!coopGameplayReady(opening),"wheel sampled as logic1 pending8 keeps presentation (1079)");}
     auto loading=wheel;loading.pendingGameLogicState=15;check(!coopPresentationReady(loading),"pending load is not presentation-ready");
     auto peer=c;peer.gameLogicState=8;check(coopPresentationPairReady(h,peer)&&!coopWorldPairReady(h,peer),"partner wheel keeps the presentation pair");}
    CoopShadowActionPacket p{};p.sampleTimeUs=1000000;p.sequence=1;p.sourceSession=11;p.targetSession=22;p.sourceWorldEpoch=3;p.targetWorldEpoch=7;p.mapId=99;p.sourceToken=4;p.rootSequence=99;p.character=1;
    check(validShadowAction(p,c,h),"initial hello");p.targetToken=9;check(validShadowAction(p,c,h)&&shadowActionTokensMatch(p,9,4),"two tokens");
    p.kind=1;p.event=1;p.swordState=2;for(unsigned state=0;state<=6;++state){p.state=state;p.airStage=state==3?2:0;p.combatVariant=state==1?1:state==2?10:0;p.combatPhase=p.combatVariant?1:0;p.combatDurationMs=p.combatVariant?1000:0;check(validShadowAction(p,c,h),"known native state");}p.state=5;
    {auto hit=p;hit.state=1;hit.combatPhase=1;
      for(unsigned v:{15u,16u,17u}){hit.combatVariant=v;hit.combatDurationMs=v==17?800:v==15?866:666;check(validShadowAction(hit,c,h),"native hit recovery wire");
        auto bad=hit;bad.combatDurationMs++;check(!validShadowAction(bad,c,h),"unobserved recovery wire timing refused");
        bad=hit;bad.state=3;bad.airStage=2;check(!validShadowAction(bad,c,h),"unobserved airborne hit recovery refused");
        bad=hit;bad.combatTimeMs=hit.combatDurationMs+1;check(!validShadowAction(bad,c,h),"expired recovery wire refused");
      }
    }
    {auto swim=p;swim.state=22;swim.combatVariant=swim.combatPhase=swim.combatTimeMs=swim.combatDurationMs=0;
      for(unsigned ch:{1u,2u}){swim.character=ch;swim.role=ch-1;
       auto context=c;context.role=1-swim.role;auto peer=h;peer.role=swim.role;
       check(validShadowAction(swim,context,peer),"native swimming state22 wire without action scenes");
       {auto climb=swim;climb.state=23;check(validShadowAction(climb,context,peer),"native climbing state23 wire without action scenes");climb.powerVariant=1;check(!validShadowAction(climb,context,peer),"climbing cannot carry a power scene");}
       auto bad=swim;bad.combatVariant=10;bad.combatPhase=1;bad.combatDurationMs=533;check(!validShadowAction(bad,context,peer),"swimming cannot carry a combat scene");
       bad=swim;bad.powerVariant=1;check(!validShadowAction(bad,context,peer),"swimming cannot carry a power scene");
       bad=swim;bad.airStage=2;check(!validShadowAction(bad,context,peer),"swimming is not airborne");
       bad=swim;bad.state=25;check(!validShadowAction(bad,context,peer),"unknown action state refused");
       {auto hold=swim;hold.state=24;hold.swordState=1;check(validShadowAction(hold,context,peer),"unclassified native state holds the stream without an action");
        hold.swordState=2;check(validShadowAction(hold,context,peer),"hold carries drawn equipment state only");
        auto badHold=hold;badHold.swordState=0;check(!validShadowAction(badHold,context,peer),"hold requires validated equipment state");
        badHold=hold;badHold.traversalVariant=4;badHold.traversalDurationMs=833;check(!validShadowAction(badHold,context,peer),"hold cannot carry a traversal scene");
        badHold=hold;badHold.combatVariant=10;badHold.combatPhase=1;badHold.combatDurationMs=533;check(!validShadowAction(badHold,context,peer),"hold cannot carry a combat scene");
        badHold=hold;badHold.powerVariant=1;check(!validShadowAction(badHold,context,peer),"hold cannot carry a power scene");
        badHold=hold;badHold.airStage=2;check(!validShadowAction(badHold,context,peer),"hold is not airborne");
        badHold=hold;badHold.leanSide=.5f;check(!validShadowAction(badHold,context,peer),"hold cannot carry lean");
        badHold=hold;badHold.version=72;check(!validShadowAction(badHold,context,peer),"protocol72 peer cannot receive a hold");}
      }
    }
    {auto impact=p;impact.state=2;impact.combatPhase=1;
      for(unsigned ch:{1u,2u})for(unsigned v:{19u,20u,21u}){impact.character=ch;impact.role=ch-1;impact.combatVariant=v;impact.combatDurationMs=v==21?500:533;impact.combatTimeMs=100;
       auto context=c;context.role=1-impact.role;auto peer=h;peer.role=impact.role;
       check(validShadowAction(impact,context,peer),"native block impact / timed parry wire inside block state");
       auto bad=impact;bad.combatDurationMs=v==21?533:500;check(!validShadowAction(bad,context,peer),"defense impact duration must match its native scene");
       bad=impact;bad.state=1;check(!validShadowAction(bad,context,peer),"defense impact cannot be a slash");
       bad=impact;bad.combatPhase=2;check(!validShadowAction(bad,context,peer),"defense impact is single-phase");
       bad=impact;bad.combatTimeMs=impact.combatDurationMs+1;check(!validShadowAction(bad,context,peer),"expired defense impact refused");
       bad=impact;bad.combatVariant=22;check(!validShadowAction(bad,context,peer),"unknown combat variant refused");
      }
    }
    {auto heavy=p;heavy.state=1;heavy.combatPhase=1;heavy.combatVariant=18;
      for(unsigned ch:{1u,2u}){heavy.character=ch;heavy.role=ch-1;heavy.combatDurationMs=833;
       auto context=c;context.role=1-heavy.role;auto peer=h;peer.role=heavy.role;
       check(validShadowAction(heavy,context,peer),"native crouched heavy release wire");
       auto other=heavy;other.combatDurationMs=1000;check(validShadowAction(other,context,peer),"either native heavy variant duration accepted by wire");
       auto bad=heavy;bad.combatDurationMs=834;check(!validShadowAction(bad,context,peer),"unknown heavy duration refused");
       bad=heavy;bad.combatTimeMs=heavy.combatDurationMs+1;check(!validShadowAction(bad,context,peer),"expired heavy wire refused");
       bad=heavy;bad.state=3;bad.airStage=2;check(!validShadowAction(bad,context,peer),"unobserved airborne sneak heavy refused");
      }
      CombatPresentation heavyClock;CombatPresentation::Sample sample{};float amount=0;
      check(heavyClock.push({100,1,18,1,0,833})&&heavyClock.push({200,1,18,1,100,833})&&heavyClock.sample(180,sample,amount)&&std::fabs(amount-80.f/833)<1e-6f,"heavy source clock interpolated");
      check(CombatPresentation::clip(18,1)==356&&!CombatPresentation::recovery(18),"heavy addresses new full-body release");
      check(!heavyClock.push({250,2,18,2,0,833})&&!heavyClock.push({250,2,18,1,0,834}),"heavy loop and duration guarded");
    }
    {auto choke=p;choke.state=21;choke.swordState=1;choke.chokeVariant=1;choke.chokeDurationMs=600;choke.chokeSpawner=31;choke.chokeDefinition=41;
      for(unsigned v=1;v<=6;++v){choke.swordState=v>=5?2u:1u;choke.chokeVariant=v;choke.chokeDurationMs=ChokePresentation::duration(v);check(validShadowAction(choke,c,h),"owned native choke wire phases");}
      auto bad=choke;bad.version=68;check(!validShadowAction(bad,c,h),"previous combat wire refused");
      bad=choke;bad.chokeSpawner=0;check(!validShadowAction(bad,c,h),"targetless choke refused");
      bad=choke;bad.swordState=1;check(!validShadowAction(bad,c,h),"lethal paired scene requires presented sword");
      bad=choke;bad.trapHeld=1;check(!validShadowAction(bad,c,h),"choke cannot conceal a held gadget");
      bad=choke;bad.state=18;check(!validShadowAction(bad,c,h),"unbound fallback cannot carry choke event");
      bad=choke;bad.chokeTimeMs=bad.chokeDurationMs+1;check(!validShadowAction(bad,c,h),"choke native duration bound");
    }
    {unsigned local=0;const unsigned start[]={217,17,200},wrap[]={910,44,866},equip[]={25,25,0},shifted[]={25,0,0};
      check(nativeTrapLocalClock(4,666,start,local)&&local==17,"recorded native ready-loop local clock");
      check(nativeTrapLocalClock(4,666,wrap,local)&&local==44,"recorded native ready-loop wrap");
      check(nativeTrapLocalClock(1,500,equip,local)&&local==25,"recorded native equip clock");
      check(!nativeTrapLocalClock(1,500,shifted,local),"shifted clock tuple refused");
    }
    {auto trap=p;trap.state=20;trap.trapHeld=2;trap.trapStock=1;trap.trapVariant=5;trap.trapDurationMs=300;
      check(validShadowAction(trap,c,h),"native spring razor placement presentation");
      for(unsigned v=1;v<=7;++v){auto phase=trap;phase.trapVariant=v;phase.trapDurationMs=TrapPresentation::duration(v);check(validShadowAction(phase,c,h),"native trap phase accepted");}
      auto bad=trap;bad.version=61;check(!validShadowAction(bad,c,h),"pre-recovery trap wire refused");
      bad=trap;bad.trapDurationMs=301;check(!validShadowAction(bad,c,h),"trap exact duration");
      bad=trap;bad.heartHeld=1;check(!validShadowAction(bad,c,h),"trap excludes other held items");
      bad=trap;bad.trapHeld=3;check(!validShadowAction(bad,c,h),"unknown trap kind refused");
      bad=trap;bad.trapStock=4;check(!validShadowAction(bad,c,h),"impossible trap stock refused");
      bad=trap;bad.airStage=2;check(!validShadowAction(bad,c,h),"unobserved airborne trap refused");
      bad=trap;bad.state=18;check(!validShadowAction(bad,c,h),"ground fallback cannot retain trap");
      TrapPresentation timeline;check(timeline.push({100,1,2,4,640,666,1}),"trap ready-loop sample");
      check(!timeline.push({130,1,2,4,4,666,1}),"trap loop rewind needs new event");
      check(timeline.push({130,2,2,4,4,666,1}),"trap source loop boundary accepted");
      check(!timeline.push({140,2,2,4,14,666,0}),"inventory change needs new event");
      check(!TrapPresentation::visible(1,0,0,0),"empty selected trap cannot render held object");
      check(TrapPresentation::visible(2,5,.70f,1)&&!TrapPresentation::visible(2,5,.80f,0),"trap visual releases at native236ms marker");
      NativePlayerStates st{};const std::uintptr_t base=0x140000000;st.player.logic=1;st.defaultMode=true;st.player.current=0;
      st.active={{0,1,base+0x1c85628},{61,2,base+0x1c3db88}};unsigned motion=99;
      check(nativePlayerMotion(st,base,motion)&&motion==0,"native ready retains grounded movement");
      st.active[1]={60,2,base+0x1c7c098};check(nativePlayerGroundTrapStates(st,base),"native placement retains ground");
      st.active[1].vtable++;check(!nativePlayerGroundTrapStates(st,base),"trap rejects wrong native class");
    }
    {auto heart=p;heart.state=19;heart.heartHeld=1;heart.heartVariant=3;heart.heartDurationMs=1000;
      check(validShadowAction(heart,c,h),"native Heart gesture on existing Steam ownership scope");
      auto bad=heart;bad.version=59;check(!validShadowAction(bad,c,h),"pre-Heart wire refused");
      bad=heart;bad.heartDurationMs=999;check(!validShadowAction(bad,c,h),"Heart duration exact");
      bad=heart;bad.heartHeld=0;check(!validShadowAction(bad,c,h),"unowned Heart gesture refused");
      bad=heart;bad.grenadeHeld=1;check(!validShadowAction(bad,c,h),"Heart cannot coexist with grenade");
      bad=heart;bad.airStage=2;check(!validShadowAction(bad,c,h),"unobserved airborne Heart refused");
      bad=heart;bad.state=18;check(!validShadowAction(bad,c,h),"unbound fallback cannot retain Heart gesture");
      bad=heart;bad.heartVariant=0;bad.heartDurationMs=0;bad.state=0;check(validShadowAction(bad,c,h),"selected Heart idle has no invented scene clock");
      HeartPresentation timeline;check(timeline.push({100,1,1,3,10,1000}),"Heart timeline source observation");
      check(!timeline.push({110,1,1,3,9,1000}),"Heart cannot rewind same native event");
      check(!timeline.push({120,1,1,1,0,533}),"Heart cannot change phase without new event");
      HeartPresentation::Sample sample{};float amount=0;check(!timeline.sample(301,sample,amount),"Heart stale samples expire");
      NativePlayerStates st{};const std::uintptr_t base=0x140000000;st.player.logic=1;st.defaultMode=true;st.player.current=0;
      st.active={{0,1,base+0x1c85628},{67,2,base+0x1c80b78}};unsigned motion=99;
      check(nativePlayerMotion(st,base,motion)&&motion==0,"Heart overlay retains exact Running root");
      st.active[1].vtable++;check(!nativePlayerGroundHeartStates(st,base),"Heart overlay rejects unknown vtable");
    }
    {auto unbound=p;unbound.state=18;
      check(validShadowAction(unbound,c,h),"explicit unbound standing/crouching presentation crosses scoped Steam wire");
      auto bad=unbound;bad.version=58;check(!validShadowAction(bad,c,h),"old peer cannot silently reinterpret unbound presentation");
      bad=unbound;bad.kind=0;bad.event=0;check(!validShadowAction(bad,c,h),"unbound state cannot masquerade as a handshake");
      bad=unbound;bad.equipmentVariant=1;bad.equipmentDurationMs=1066;check(!validShadowAction(bad,c,h),"unbound presentation cannot request equipment gesture");
      bad=unbound;bad.rangedKind=1;check(!validShadowAction(bad,c,h),"unbound presentation cannot create a weapon prop");
      bad=unbound;bad.powerVariant=1;bad.powerDurationMs=1000;check(!validShadowAction(bad,c,h),"unbound presentation cannot authorize a power effect");
      bad=unbound;bad.airStage=2;check(!validShadowAction(bad,c,h),"unbound ground cannot erase airborne requirements");
      bad=unbound;bad.targetWorldEpoch++;check(!validShadowAction(bad,c,h),"unbound presentation still rejects old worlds");
    }
    {auto r=p;r.state=14;r.swordState=1;r.carryVariant=2;r.carryDurationMs=3000;r.carryIncarnation=23;
      r.carrySpawner=carryIdentityKey("ld_spawner_alexi_escape");r.carryDefinition=carryIdentityKey("models/characters/def/story/alexi_mayhew.def");
      check(validShadowAction(r,c,h),"owned carry semantic with authored identity");auto bad=r;bad.carrySpawner=0;check(!validShadowAction(bad,c,h),"carry refuses orphan target");
      auto high=r;high.carryVariant=8;high.carryDurationMs=666;high.carryThrowVelocity={4,0,0};
      check(validShadowAction(high,c,h),"native high throw carries bounded source velocity and exact corpse identity");
      bad=high;bad.version=30;check(!validShadowAction(bad,c,h),"pre-throw wire layout cannot carry new velocity fields");
      bad=high;bad.carryThrowVelocity={0,0,0};check(!validShadowAction(bad,c,h),"high throw needs source velocity");
      bad=r;bad.carryThrowVelocity={4,0,0};check(!validShadowAction(bad,c,h),"ordinary carry refuses stale throw vector");
      bad=r;bad.state=0;check(!validShadowAction(bad,c,h),"neutral action cannot hide carry target");
      bad=r;bad.swordState=2;check(!validShadowAction(bad,c,h),"carry requires holstered sword");
      auto preparation=r;preparation.carryVariant=7;preparation.carryDurationMs=300;preparation.swordState=2;
      check(validShadowAction(preparation,c,h),"observed preparation permits sword while native holster is pending");
      preparation.swordState=1;check(validShadowAction(preparation,c,h),"same preparation permits completed native holster");
      preparation.carryTimeMs=301;check(!validShadowAction(preparation,c,h),"preparation clock cannot grow beyond authored gather endpoint");
      bad=r;bad.version=29;check(!validShadowAction(bad,c,h),"prior semantic version cannot be interpreted as preparation-aware stream");
      bad=r;bad.version=28;check(!validShadowAction(bad,c,h),"prior wire layout cannot be interpreted as carry");
      bad=r;bad.targetWorldEpoch++;check(!validShadowAction(bad,c,h),"carry refuses old world target");
      r.role=1;r.character=2;r.sourceSession=22;r.targetSession=11;r.sourceWorldEpoch=7;r.targetWorldEpoch=3;
      check(validShadowAction(r,h,c),"Emily carry has identical scoped semantics without handle translation");
    }
    {   // 1213: native use (state 75) -> authored interaction on the ground action only.
      auto use=p;use.state=0;use.combatVariant=0;use.combatPhase=0;use.combatDurationMs=0;use.airStage=0;use.useEvent=3;
      for(unsigned v=1;v<=5;++v){use.useVariant=v;use.useDurationMs=UsePresentation::duration(v);use.useTimeMs=use.useDurationMs/2;check(validShadowAction(use,c,h),"native use interaction accepted on the ground");}
      use.useVariant=2;use.useDurationMs=3200;use.useTimeMs=100;
      auto bad=use;bad.version=73;check(!validShadowAction(bad,c,h),"pre-use wire refused");
      bad=use;bad.useDurationMs=2300;check(!validShadowAction(bad,c,h),"use duration bound to its variant");
      bad=use;bad.useTimeMs=3201;check(!validShadowAction(bad,c,h),"use clock bounded");
      bad=use;bad.useEvent=0;check(!validShadowAction(bad,c,h),"use needs an incarnation");
      bad=use;bad.useVariant=6;bad.useDurationMs=0;check(!validShadowAction(bad,c,h),"unknown use variant refused");
      bad=use;bad.state=1;bad.combatVariant=1;bad.combatPhase=1;bad.combatDurationMs=1000;check(!validShadowAction(bad,c,h),"use never overlaps an attack");
      UsePresentation line;UsePresentation::Sample out{};float a=0;
      check(line.push({100,1,2,0,3200})&&line.push({140,1,2,40,3200})&&line.sample(120,out,a)&&out.variant==2&&std::fabs(a-20.f/3200.f)<.00001f,"use interpolation");
      check(!line.push({150,1,3,50,4800}),"same incarnation cannot change variant");
      check(line.push({160,1,0,0,0})&&line.push({200,2,1,0,2300}),"use ends and a newer use starts");
      check(!line.push({240,1,2,0,3200}),"older use incarnation refused");
      check(useVariantForName("script_lever_windturbine_1_1_1")==2&&useVariantForName("env_valve_wheel_03")==4&&useVariantForName("crank_gate_2")==3&&useVariantForName("door_lock_01")==5&&useVariantForName("env_vise_01_1_3")==3&&useVariantForName("env_keyhole_lock")==5&&useVariantForName("ld_rich_small_01_sndctr_4")==1,"use variant from the focused entity name");
  check(useVariantForName("esc_env_rich_sink_tap_01_1")==4&&useVariantForName("env_interiors_tap_right")==4&&useVariantForName("tap")==4&&useVariantForName("env_tapestry_01")==1&&useVariantForName("script_desktape_2")==1&&
        useVariantForName("script_door_latch_blackmarket_apartment")==5&&useVariantForName("ld_ring_locker_01_1_lock")==5&&useVariantForName("ld_wheel_01_1_1")==4&&useVariantForName("pa_ld_valve_set_01_2")==4,"1213g taps are valves, latches and ring lockers are locks");
  {const float o[3]{-27.53f,-149.89f,66.97f},pushed[3]{-27.89f,-149.89f,66.40f},walked[3]{-27.53f,-150.55f,66.97f},fell[3]{-27.53f,-149.89f,64.0f};
   check(!useMovedAway(o,pushed)&&!useMovedAway(o,fell)&&useMovedAway(o,walked)&&!useMovedAway(o,o),"1213h a use ends only on horizontal travel beyond 0.6 m (1228b window drop does not end it)");}
  {PresentationRefusalGate g{};bool stopped=false;for(unsigned i=0;i<PresentationRefusalGate::stopStreak-1;++i)stopped=stopped||g.refuse(PresentationRefusalGate::SkeletonRead);
   g.accept();const bool afterAccept=g.streak==0;bool late=false;for(unsigned i=0;i<PresentationRefusalGate::stopStreak;++i)late=g.refuse(PresentationRefusalGate::NotGameplay);
   check(!stopped&&afterAccept&&late&&g.streak==0&&g.last==PresentationRefusalGate::NotGameplay&&g.reasons[PresentationRefusalGate::SkeletonRead]==PresentationRefusalGate::stopStreak-1&&g.longest==PresentationRefusalGate::stopStreak&&!g.refuse(99)&&g.last==0,
     "1213i a transient refused frame is skipped; only a sustained streak stops the presentation (1228e lever froze at 27%)");}
  {using U=UsePresentation;
   check(U::contact(.05f)==0&&U::contact(.5f)>.999f&&U::contact(.95f)==0&&U::contact(.21f)>0&&U::contact(.21f)<1&&U::contact(.82f)>0&&U::contact(.82f)<1&&U::contact(std::nanf(""))==0,
     "1213k use contact window: none while reaching out, full while operating, none after recovery");
   U::Sample t{1.,1,4,100,U::duration(4),{1.f,2.f,3.f},true},noUse{1.,0,0,0,0,{1.f,2.f,3.f},true},bad{1.,1,4,100,U::duration(4),{std::nanf(""),0,0},true},far{1.,1,4,100,U::duration(4),{2e5f,0,0},true};
   check(U::valid(t)&&!U::valid(noUse)&&!U::valid(bad)&&!U::valid(far),"1213k a use target is finite, bounded and only travels with a use");
   // Joint-to-world (grenade_fx): world = root.pos + R * model with R's columns at root[3..11]. useModelTarget inverts it.
   const float c=std::cos(1.1f),sn=std::sin(1.1f);const float root[12]{10.f,-4.f,2.f, c,sn,0, -sn,c,0, 0,0,1};const float model[3]{.4f,.15f,.2f};float world[3]{},back[3]{};
   for(unsigned r=0;r<3;++r){world[r]=root[r];for(unsigned k=0;k<3;++k)world[r]+=root[3+k*3+r]*model[k];}
   useModelTarget(root,world,back);
   check(std::fabs(back[0]-model[0])<1e-5f&&std::fabs(back[1]-model[1])<1e-5f&&std::fabs(back[2]-model[2])<1e-5f,"1213k useModelTarget inverts the native joint-to-world mapping");}
    }
    {auto stun=p;stun.state=16;stun.stunVariant=1;stun.stunTimeMs=300;stun.stunDurationMs=800;
      for(unsigned v=1;v<=4;++v){stun.stunVariant=v;check(validShadowAction(stun,c,h),"owned native stun direction and clock accepted");}
      auto bad=stun;bad.version=37;check(!validShadowAction(bad,c,h),"pre-stun semantic layout refused");
      bad=stun;bad.state=0;check(!validShadowAction(bad,c,h),"stun cannot masquerade as idle");
      bad=stun;bad.stunVariant=0;check(!validShadowAction(bad,c,h),"orphan stun action refused");
      bad=stun;bad.stunTimeMs=801;check(!validShadowAction(bad,c,h),"stun clock bounded to native scene");
      bad=stun;bad.grenadeHeld=1;check(!validShadowAction(bad,c,h),"stun retires grenade ownership");
      bad=stun;bad.targetWorldEpoch++;check(!validShadowAction(bad,c,h),"stun refuses old world");
      StunPresentation line;StunPresentation::Sample out{};float a=0;
      check(line.push({100,1,1,100,800})&&line.push({140,1,1,140,800}),"stun samples share native source clock");
      check(line.sample(120,out,a)&&std::fabs(a-.15f)<.00001f,"stun interpolation");
      check(!line.push({140,1,1,140,800})&&!line.push({180,1,2,180,800})&&!line.push({180,1,1,139,800}),"duplicate direction swap and clock rewind refused");
      check(line.push({180,2,0,0,0})&&line.sample(180,out,a)&&!out.variant,"observed exit ends stun");
      check(!line.sample(381,out,a),"stale stun cannot drive body");line.clear();check(!line.sample(180,out,a),"world retirement clears stun");
    }
    {auto g=p;g.state=15;g.grenadeHeld=1;
      for(unsigned v=1;v<=8;++v){g.grenadeVariant=v;g.grenadeDurationMs=GrenadePresentation::duration(v);g.grenadeTimeMs=g.grenadeDurationMs/2;check(validShadowAction(g,c,h),"known native grenade phases accepted");}
      auto bad=g;bad.version=36;check(!validShadowAction(bad,c,h),"previous grenade-free wire version refused");
      bad=g;bad.rangedKind=1;check(!validShadowAction(bad,c,h),"simultaneous gun and grenade cannot own hand");
      bad=g;bad.grenadeHeld=0;check(!validShadowAction(bad,c,h),"orphan grenade phase refused");
      bad=g;bad.grenadeDurationMs--;check(!validShadowAction(bad,c,h),"wrong grenade duration refused");
      bad=g;bad.targetWorldEpoch++;check(!validShadowAction(bad,c,h),"grenade cannot cross world incarnation");
      auto air=g;air.airStage=1;air.airStartZ=66;air.airTargetZ=67.5f;
      check(validShadowAction(air,c,h),"owned grenade action carries bounded native jump apex");
      MovementSemantics grenadeMotion;
      check(grenadeMotion.push({100,1,air.state,air.leanSide,air.leanUp,air.airStartZ,air.airTargetZ,air.airStage,air.airRemaining}),"validated grenade packet also enters delayed movement clock");
      bad=air;bad.airTargetZ=100;check(!validShadowAction(bad,c,h),"unbounded grenade jump apex refused");
      bad=air;bad.version=38;check(!validShadowAction(bad,c,h),"old grounded-only grenade semantics refused");
      air.airStage=2;air.airStartZ=air.airTargetZ=0;check(validShadowAction(air,c,h),"falling grenade phase carries no stale apex");
      check(grenadeMotion.push({130,1,air.state,air.leanSide,air.leanUp,air.airStartZ,air.airTargetZ,air.airStage,air.airRemaining}),"same grenade event carries native jump-to-fall transition");
      MovementSemantics::Sample bodyAir{};check(grenadeMotion.sample(115,bodyAir)&&bodyAir.airStage==1,"future grenade fall cannot move an older jump pose");
      check(grenadeMotion.sample(130,bodyAir)&&bodyAir.airStage==2,"grenade fall reaches body on the source timeline");
      bad=air;bad.airStartZ=66;check(!validShadowAction(bad,c,h),"falling grenade cannot retain old jump apex");
      for(unsigned v=1;v<=2;++v){bad=air;bad.grenadeVariant=v;bad.grenadeDurationMs=GrenadePresentation::duration(v);bad.grenadeTimeMs=0;check(!validShadowAction(bad,c,h),"unobserved airborne equipment scene refused");}
      air.state=3;air.grenadeVariant=air.grenadeTimeMs=air.grenadeDurationMs=0;
      check(validShadowAction(air,c,h),"native airborne idle retains independent held grenade");
      bad=air;bad.state=0;check(!validShadowAction(bad,c,h),"ground action cannot masquerade as airborne held grenade");
      g.state=0;g.grenadeVariant=g.grenadeTimeMs=g.grenadeDurationMs=0;check(validShadowAction(g,c,h),"native standing held grenade is independent of action scene");
      g.role=1;g.character=2;g.sourceSession=22;g.targetSession=11;g.sourceWorldEpoch=7;g.targetWorldEpoch=3;check(validShadowAction(g,h,c),"Emily uses identical native grenade semantics");
    }
    {auto r=p;r.state=13;r.rangedKind=1;r.rangedVariant=3;r.rangedTimeMs=20;r.rangedDurationMs=666;
      check(validShadowAction(r,c,h),"owned native firearm scene packet");auto bad=r;bad.rangedKind=2;check(!validShadowAction(bad,c,h),"wrong weapon kind refused");
      bad=r;bad.state=0;check(!validShadowAction(bad,c,h),"firing cannot masquerade as idle");
      bad=r;bad.targetWorldEpoch++;check(!validShadowAction(bad,c,h),"ranged packet cannot cross world generations");
      auto air=r;air.airStage=1;air.airStartZ=66;air.airTargetZ=67.5f;check(validShadowAction(air,c,h),"owned airborne gun fire carries native apex");
      air.airStage=2;air.airStartZ=air.airTargetZ=0;check(validShadowAction(air,c,h),"owned falling gun fire carries no stale apex");
      bad=air;bad.airStartZ=1;check(!validShadowAction(bad,c,h),"falling gun fire refuses stale jump data");
      bad=air;bad.version=27;check(!validShadowAction(bad,c,h),"pre-airborne ranged protocol refused");
      bad=air;bad.rangedVariant=5;bad.rangedDurationMs=533;check(!validShadowAction(bad,c,h),"unobserved airborne equipment draw refused");
      air.rangedKind=2;air.rangedVariant=7;air.rangedMechanism=2;check(validShadowAction(air,c,h),"owned falling crossbow shot retains its mechanical state");
      auto reload=air;reload.rangedVariant=8;reload.rangedDurationMs=900;check(validShadowAction(reload,c,h),"owned falling crossbow reload preserves the native clock");
      reload.rangedKind=1;reload.rangedVariant=4;reload.rangedDurationMs=1500;reload.rangedMechanism=0;check(validShadowAction(reload,c,h),"owned falling gun reload preserves the native clock");
      air.state=3;air.rangedVariant=air.rangedTimeMs=air.rangedDurationMs=0;check(validShadowAction(air,c,h),"drawn ranged idle remains equipped during native air state");
      bad=air;bad.combatVariant=1;bad.combatPhase=1;bad.combatDurationMs=1000;check(!validShadowAction(bad,c,h),"unobserved simultaneous held ranged weapon and air sword attack refused");
      bad=air;bad.swordState=1;check(!validShadowAction(bad,c,h),"holstered airborne state cannot retain drawn ranged idle");
      r.state=0;r.rangedVariant=r.rangedTimeMs=r.rangedDurationMs=0;check(validShadowAction(r,c,h),"drawn ranged idle");
      bad=r;bad.rangedMechanism=2;check(!validShadowAction(bad,c,h),"gun cannot claim crossbow mechanical state");
      r.rangedKind=2;r.rangedMechanism=2;check(validShadowAction(r,c,h),"held fired crossbow state remains legal without an action scene");
      bad=r;bad.rangedMechanism=3;check(!validShadowAction(bad,c,h),"unknown mechanical state refused");
      bad=r;bad.version=26;check(!validShadowAction(bad,c,h),"older packet cannot omit mechanism semantics");
      r.swordState=1;check(!validShadowAction(r,c,h),"holstered state cannot retain drawn ranged idle");
    }
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
      for(unsigned v=32;v<=35;++v){
        auto shadow=emily;shadow.powerVariant=v;shadow.powerDurationMs=PowerPresentation::duration(v);shadow.swordState=1;
        check(validShadowAction(shadow,ec,eh)&&shadowPowerSwordContext(v,1),"owned Emily shadow form phases accepted hands-free");
        auto bad=shadow;bad.swordState=2;check(!validShadowAction(bad,ec,eh),"shadow form cannot retain drawn sword");
        bad=shadow;bad.character=1;bad.role=0;check(!validShadowAction(bad,c,h),"Corvo cannot claim Emily shadow form");
        bad=shadow;bad.airStage=2;check(!validShadowAction(bad,ec,eh),"unverified shadow air metadata refused");
        bad=shadow;bad.powerDurationMs++;check(!validShadowAction(bad,ec,eh),"exact native shadow transition duration required");
        bad=shadow;bad.version=56;check(!validShadowAction(bad,ec,eh),"old peer cannot claim unsupported shadow sneak contract");
      }
      {PowerPresentation t;PowerPresentation::Sample s{};float a=0;
       check(t.push({100,1,32,200,1000})&&t.push({150,1,32,250,1000})&&t.sample(125,s,a)&&std::fabs(a-.225f)<1e-6f,"entry follows native clock");
       check(!t.push({180,1,33,0,1000})&&t.push({180,2,33,0,1000}),"sustained form requires new semantic event");
       check(t.push({240,3,34,600,1133})&&t.sample(240,s,a)&&std::fabs(a-600.f/1133)<1e-6f,"exit follows native clock");
       check(!t.sample(441,s,a),"stale shadow form presentation retires");}
      for(unsigned v=29;v<=31;++v){
        auto cast=emily;cast.powerVariant=v;cast.powerDurationMs=PowerPresentation::duration(v);
        check(validShadowAction(cast,ec,eh),"observed Emily Doppelganger exact phase accepted");
        auto bad=cast;bad.role=0;bad.character=1;check(!validShadowAction(bad,c,h),"Corvo cannot claim campaign Emily Doppelganger");
        bad=cast;bad.swordState=1;check(validShadowAction(bad,ec,eh),"493/494 observed holstered Doppelganger accepted");
        bad.version=52;check(!validShadowAction(bad,ec,eh),"pre-holstered contract refused");
        bad=cast;bad.swordState=0;check(!validShadowAction(bad,ec,eh),"unknown Doppelganger equipment refused");
        for(unsigned sword:{1u,2u}){
          auto air=cast;air.swordState=sword;air.airStage=1;air.airStartZ=66.437f;air.airTargetZ=67.84f;
          check(validShadowAction(air,ec,eh),"Doppelganger cast preserves bounded source jump apex");
          bad=air;bad.airTargetZ=80;check(!validShadowAction(bad,ec,eh),"Doppelganger cannot bypass jump height bound");
          bad=air;bad.airStage=2;check(!validShadowAction(bad,ec,eh),"Doppelganger fall cannot retain old apex");
          air.airStage=2;air.airStartZ=air.airTargetZ=0;check(validShadowAction(air,ec,eh),"Doppelganger fall and known sword context accepted");
          bad=air;bad.version=53;check(!validShadowAction(bad,ec,eh),"previous ground-only Doppelganger contract refused");
          bad=air;bad.swordState=0;check(!validShadowAction(bad,ec,eh),"airborne Doppelganger still requires known equipment");
        }
        bad=cast;bad.powerDurationMs++;check(!validShadowAction(bad,ec,eh),"Doppelganger exact native timing required");
        bad=cast;bad.version=51;check(!validShadowAction(bad,ec,eh),"pre-Doppelganger protocol refused");
      }
      {PowerPresentation clock;PowerPresentation::Sample sample{};float amount=0;
        check(clock.push({100,1,30,1190,1200})&&clock.push({140,1,30,20,1200})&&clock.sample(140,sample,amount)&&std::fabs(amount-20.f/1200)<.00001f,"native Doppelganger hold reset stays within same event");
        check(!clock.push({150,1,31,0,666})&&clock.push({150,2,31,0,666}),"Doppelganger release requires distinct native event");
        check(!clock.sample(351,sample,amount),"stale Doppelganger presentation retires");}

      auto dark=power;dark.powerVariant=10;dark.powerDurationMs=933;check(validShadowAction(dark,c,h),"native Corvo Dark Vision phase packet");
      auto wind=power;wind.powerVariant=11;wind.powerDurationMs=1000;check(validShadowAction(wind,c,h),"observed Corvo Windblast cast packet");
      auto badWind=wind;badWind.powerDurationMs=933;check(!validShadowAction(badWind,c,h),"Windblast must use native 1000ms phase");
      auto emptyWind=wind;emptyWind.swordState=1;check(validShadowAction(emptyWind,c,h),"observed native holstered Windblast supported");
      badWind=emptyWind;badWind.version=22;check(!validShadowAction(badWind,c,h),"standing-only Windblast protocol refused");
      auto airWind=wind;airWind.airStage=2;check(validShadowAction(airWind,c,h),"observed falling Windblast preserves native cast clock");
      airWind.airStage=1;airWind.airStartZ=67;airWind.airTargetZ=68.5f;check(validShadowAction(airWind,c,h),"observed jumping Windblast carries native apex");
      airWind.swordState=1;check(validShadowAction(airWind,c,h),"holstered airborne Windblast retains equipment semantics");
      badWind=airWind;badWind.version=23;check(!validShadowAction(badWind,c,h),"pre-airborne Windblast protocol refused");
      badWind=airWind;badWind.airStage=2;check(!validShadowAction(badWind,c,h),"falling Windblast cannot retain stale jump apex");
      badWind=wind;badWind.role=1;badWind.character=2;check(!validShadowAction(badWind,ec,eh),"Windblast is Corvo-only in native campaign");
      badWind=wind;badWind.version=21;check(!validShadowAction(badWind,c,h),"pre-Windblast protocol refused");
      auto femaleDark=emily;femaleDark.powerVariant=10;femaleDark.powerDurationMs=933;check(validShadowAction(femaleDark,ec,eh),"native Emily Dark Vision phase packet");
      auto emptyDark=dark;emptyDark.swordState=1;check(validShadowAction(emptyDark,c,h),"native holstered Dark Vision cast packet");
      auto airDark=dark;airDark.airStage=1;airDark.airStartZ=66;airDark.airTargetZ=67.1f;
      check(validShadowAction(airDark,c,h),"native Dark Vision and jump apex share one scoped packet");
      auto airFemale=femaleDark;airFemale.airStage=2;check(validShadowAction(airFemale,ec,eh),"native Emily falling Dark Vision packet");
      auto badAir=airDark;badAir.powerVariant=1;badAir.powerDurationMs=333;check(!validShadowAction(badAir,c,h),"Blink cannot impersonate airborne Dark Vision");
      badAir=airFemale;badAir.powerVariant=7;badAir.powerDurationMs=266;check(!validShadowAction(badAir,ec,eh),"Far Reach cannot supply Dark Vision air metadata");
      badAir=airDark;badAir.version=20;check(!validShadowAction(badAir,c,h),"ground-only power protocol refused");
      badAir=airDark;badAir.airTargetZ=80;check(!validShadowAction(badAir,c,h),"airborne cast apex bounded");
      badAir=airDark;badAir.airStage=0;check(!validShadowAction(badAir,c,h),"ground cast cannot retain stale jump apex");
      emptyDark.version=19;check(!validShadowAction(emptyDark,c,h),"old armed-only Dark Vision protocol refused");
      auto oldDark=dark;oldDark.version=18;check(!validShadowAction(oldDark,c,h),"pre-Dark Vision packet version refused");
      oldDark=dark;oldDark.powerVariant=29;check(!validShadowAction(oldDark,c,h),"unobserved power variant refused");
      {
        auto release=power;release.powerVariant=28;release.powerDurationMs=400;release.swordState=1;
        check(validShadowAction(release,c,h)&&shadowPowerSwordContext(28,1),"native acquired Corvo release requires holstered sword");
        auto bad=release;bad.swordState=2;check(!validShadowAction(bad,c,h)&&!shadowPowerSwordContext(28,2),"drawn-sword release refused");
        bad=release;bad.role=1;bad.character=2;check(!validShadowAction(bad,ec,eh),"Emily cannot send successful Possession release");
        bad=release;bad.airStage=2;check(!validShadowAction(bad,c,h),"airborne Possession release refused");
        bad=release;bad.powerDurationMs=399;check(!validShadowAction(bad,c,h),"release duration must match observed native400ms");
        bad=release;bad.version=48;check(!validShadowAction(bad,c,h),"older protocol cannot authorize release");
        PowerPresentation timeline;PowerPresentation::Sample sample{};float amount=0;
        check(timeline.push({100,1,28,100,400})&&timeline.push({150,1,28,150,400})&&timeline.sample(125,sample,amount)&&std::fabs(amount-.3125f)<.00001f,"release presentation follows native source clock");
        check(!timeline.sample(351,sample,amount),"stale release expires without inventing occupied host state");
      }
      {
        auto occupied=power;occupied.powerVariant=occupied.powerDurationMs=occupied.powerTimeMs=0;occupied.state=17;occupied.swordState=1;
        check(validShadowAction(occupied,c,h),"Corvo occupied-host presentation is separate from power animation");
        auto bad=occupied;bad.swordState=2;check(!validShadowAction(bad,c,h),"occupied host cannot retain drawn sword");
        bad=occupied;bad.role=1;bad.character=2;check(!validShadowAction(bad,ec,eh),"Emily cannot claim native human Possession");
        bad=occupied;bad.version=49;check(!validShadowAction(bad,c,h),"old release-only protocol cannot hide body");
        bad=occupied;bad.powerVariant=28;bad.powerDurationMs=400;check(!validShadowAction(bad,c,h),"occupation cannot carry a release animation");
        bad=occupied;bad.targetWorldEpoch++;check(!validShadowAction(bad,c,h),"old occupied world cannot hide new clone");
        MovementSemantics timeline;MovementSemantics::Sample sample{};
        check(timeline.push({100,1,17})&&timeline.sample(200,sample)&&sample.state==17,"occupied visibility uses delayed body timeline");
        check(!timeline.sample(301,sample),"stale occupied sample cannot perpetuate hiding");
        check(timeline.push({310,2,0})&&timeline.sample(310,sample)&&sample.state==0,"native grounded recovery replaces occupancy");
        check(nativeHumanPossessionEquipment(1,false,false)&&nativeHumanPossessionEquipment(8,false,true),"native stopped transfer and restarted unselected occupied component distinguished");
        check(!nativeHumanPossessionEquipment(8,true,true)&&!nativeHumanPossessionEquipment(8,false,false)&&!nativeHumanPossessionEquipment(1,false,true),"unobserved possession equipment lifecycle refused");
        constexpr std::uintptr_t base=0x140000000;
        NativePlayerStates state{};state.player.logic=1;state.player.current=18;state.active.push_back({0,1,base+0x1c85628});
        check(nativeHumanPossessionContext(state,base,8),"exact occupied host Running0 physics18 admitted");
        check(!nativeHumanPossessionContext(state,base,14),"inactive sentinel is never occupation");
        state.defaultMode=true;check(!nativeHumanPossessionContext(state,base,8),"default movement cannot authorize hide");state.defaultMode=false;
        state.player.current=0;state.active[0]={26,1,base+0x1c79878};check(nativeHumanPossessionContext(state,base,1),"exact post-release entry context admitted");
        check(!nativeHumanPossessionContext(state,base,0),"release context stays separately scene-owned");
        for(unsigned stance:{0u,1u}){
            state.player.current=stance;
            check(nativeHumanPossessionEntryContext(state,base)&&nativeHumanPossessionContext(state,base,1),"observed standing/crouched host entry accepted");
            auto bad=state;bad.player.current=2;check(!nativeHumanPossessionEntryContext(bad,base),"airborne host entry remains unverified");
            bad=state;bad.defaultMode=true;check(!nativeHumanPossessionEntryContext(bad,base),"default locomotion cannot authorize release");
            bad=state;bad.active.push_back({6,2,base+0x1c84258});check(!nativeHumanPossessionEntryContext(bad,base),"extra state cannot authorize host entry");
            bad=state;bad.active[0].vtable++;check(!nativeHumanPossessionEntryContext(bad,base),"wrong entry class refused");
        }
        state.active[0].vtable++;check(!nativeHumanPossessionContext(state,base,1),"wrong native class refused");
      }
      for(unsigned variant=26;variant<=27;++variant){
        auto possession=power;possession.powerVariant=variant;possession.powerDurationMs=PowerPresentation::duration(variant);
        check(validShadowAction(possession,c,h)&&shadowPowerSwordContext(variant,2),"observed standing armed Corvo Possession entry/aim accepted");
        for(unsigned draw:{1u,3u}){
            auto dual=possession;dual.equipmentVariant=draw;dual.equipmentDurationMs=draw==1?1066:1133;dual.equipmentTimeMs=80;
            check(validShadowAction(dual,c,h),"native Possession overlap carries exact independent sword draw clock");
            auto bad=dual;bad.equipmentDurationMs++;check(!validShadowAction(bad,c,h),"Possession draw rejects unknown duration");
            bad=dual;bad.equipmentVariant=2;check(!validShadowAction(bad,c,h),"Possession cannot combine sword holster with aim");
            bad=dual;bad.equipmentTimeMs=2000;check(!validShadowAction(bad,c,h),"Possession refuses expired draw");
            bad=dual;bad.version=50;check(!validShadowAction(bad,c,h),"old protocol cannot authorize overlapping draw semantics");
            bad=dual;bad.powerVariant=23;bad.powerDurationMs=400;check(!validShadowAction(bad,c,h),"other powers cannot borrow Possession draw permission");
        }
        auto bad=possession;bad.role=1;bad.character=2;check(!validShadowAction(bad,ec,eh),"Emily cannot claim Possession");
        bad=possession;bad.swordState=1;check(!validShadowAction(bad,c,h)&&!shadowPowerSwordContext(variant,1),"unobserved holstered Possession refused");
        bad=possession;bad.airStage=2;check(!validShadowAction(bad,c,h),"unobserved airborne Possession refused");
        bad=possession;bad.powerDurationMs++;check(!validShadowAction(bad,c,h),"Possession requires exact observed native duration");
        bad=possession;bad.version=47;check(!validShadowAction(bad,c,h),"older peer cannot authorize Possession");
        bad=possession;bad.powerVariant=28;bad.powerDurationMs=0;check(!validShadowAction(bad,c,h),"release without native duration remains refused");
      }
      {PowerPresentation t;PowerPresentation::Sample sample{};float amount=0;
       check(t.push({100,1,27,80,200})&&t.push({140,1,27,120,200})&&t.sample(120,sample,amount)&&std::fabs(amount-.5f)<.00001f,"Possession aim samples native elapsed time");
       check(t.push({160,2,0,0,0})&&t.sample(160,sample,amount)&&sample.variant==0,"empty-target cancellation follows native phase retirement");
       check(!t.sample(361,sample,amount),"stale Possession state cannot hold remote pose");}
      for(unsigned variant=23;variant<=25;++variant){
        auto swarm=power;swarm.powerVariant=variant;swarm.powerDurationMs=PowerPresentation::duration(variant);
        check(validShadowAction(swarm,c,h)&&shadowPowerSwordContext(variant,2),"native standing armed Corvo Swarm phase accepted by sender and receiver");
        auto bad=swarm;bad.role=1;bad.character=2;check(!validShadowAction(bad,ec,eh),"Emily cannot claim Swarm");
        bad=swarm;bad.swordState=1;check(validShadowAction(bad,c,h)&&shadowPowerSwordContext(variant,1),"v273/v275 native holstered Swarm accepted by sender and receiver");
        bad=swarm;bad.airStage=2;check(!validShadowAction(bad,c,h),"unobserved airborne Swarm remains refused");
        bad=swarm;bad.powerDurationMs++;check(!validShadowAction(bad,c,h),"Swarm exact native duration required");
        bad=swarm;bad.version=46;check(!validShadowAction(bad,c,h),"armed-only Swarm peer cannot authorize expanded contexts");
      }
      {PowerPresentation t;PowerPresentation::Sample sample{};float amount=0;
       check(t.push({100,1,24,320,333})&&t.push({140,1,24,27,333})&&t.sample(140,sample,amount)&&std::fabs(amount-27.f/333)<.00001f,"Swarm native looping clock reset remains same semantic phase");
       check(!t.push({150,1,25,0,1600})&&t.push({150,2,25,0,1600}),"Swarm release requires a distinct event");
       check(!t.sample(351,sample,amount),"stale Swarm pose expires");}
      for(unsigned variant=19;variant<=22;++variant){
        auto domino=emily;domino.powerVariant=variant;domino.powerDurationMs=PowerPresentation::duration(variant);
        check(validShadowAction(domino,ec,eh),"observed standing armed native Emily Domino phase accepted");
        auto bad=domino;bad.role=0;bad.character=1;check(!validShadowAction(bad,c,h),"Corvo cannot claim Domino");
        auto holstered=domino;holstered.swordState=1;check(validShadowAction(holstered,ec,eh)&&shadowPowerSwordContext(variant,1),"observed native holstered Domino accepted by shared sender/wire context gate");
        bad=holstered;bad.swordState=0;check(!validShadowAction(bad,ec,eh)&&!shadowPowerSwordContext(variant,0),"missing holstered equipment identity refused by sender and receiver");
        bad=holstered;bad.version=44;check(!validShadowAction(bad,ec,eh),"previous armed-only Domino protocol cannot authorize holstered context");
        bad=domino;bad.airStage=2;check(!validShadowAction(bad,ec,eh),"unverified airborne Domino refused");
        bad=domino;bad.powerDurationMs++;check(!validShadowAction(bad,ec,eh),"Domino duration mismatch refused");
        bad=domino;bad.version=43;check(!validShadowAction(bad,ec,eh),"older peer cannot authorize Domino phases");
      }
      {PowerPresentation d;PowerPresentation::Sample sampled{};float phase=0;
       check(d.push({100,1,20,1320,1333})&&d.push({140,1,20,1360,1333})&&d.sample(130,sampled,phase)&&std::fabs(phase-17.f/1333)<.00001f,"Domino aim wraps native accumulated time");
       check(!d.push({160,1,21,0,400}),"one Midnight instance still needs a fresh semantic event on subscene change");
       check(d.push({160,2,21,0,400})&&d.push({190,2,21,30,400})&&d.sample(190,sampled,phase),"Domino selection uses its own clock within the native cast");
       check(!d.sample(391,sampled,phase),"stale Domino gesture retires");}
      for(unsigned variant=16;variant<=18;++variant){
        auto mesmer=emily;mesmer.powerVariant=variant;mesmer.powerDurationMs=PowerPresentation::duration(variant);
        check(validShadowAction(mesmer,ec,eh),"observed native Emily Mesmerize phase packet");
        auto bad=mesmer;bad.version=39;check(!validShadowAction(bad,ec,eh),"pre-Mesmerize peer refused");
        bad=mesmer;bad.role=0;bad.character=1;check(!validShadowAction(bad,c,h),"Corvo cannot claim Emily Mesmerize");
        bad=mesmer;bad.swordState=1;check(validShadowAction(bad,ec,eh),"native v142/v144 holstered Emily Mesmerize phase accepted");
        bad.version=40;check(!validShadowAction(bad,ec,eh),"armed-only Mesmerize peer cannot authorize holstered semantics");
        bad=mesmer;bad.swordState=0;check(!validShadowAction(bad,ec,eh),"unknown Mesmerize sword state refused");
        bad=mesmer;bad.airStage=2;check(validShadowAction(bad,ec,eh),"observed native armed airborne Mesmerize phase accepted");
        bad.version=41;check(!validShadowAction(bad,ec,eh),"old ground-only Mesmerize peer refused");
        bad=mesmer;bad.airStage=2;bad.swordState=1;check(validShadowAction(bad,ec,eh),"v175/v177 native empty-hand airborne Mesmerize accepted");
        bad.version=42;check(!validShadowAction(bad,ec,eh),"armed-air-only peer cannot authorize holstered airborne semantics");
        bad=mesmer;bad.airStage=2;bad.swordState=0;check(!validShadowAction(bad,ec,eh),"airborne Mesmerize still requires a known sword state");
        bad=mesmer;bad.airStage=1;bad.airStartZ=66.4f;bad.airTargetZ=67.8f;check(validShadowAction(bad,ec,eh),"Mesmerize retains bounded native jump apex");
        bad.airTargetZ=80;check(!validShadowAction(bad,ec,eh),"Mesmerize cannot bypass native jump bounds");
        bad=mesmer;bad.powerDurationMs++;check(!validShadowAction(bad,ec,eh),"incorrect Mesmerize phase duration refused");
      }
      {PowerPresentation m;PowerPresentation::Sample sampled{};float phase=0;
       check(m.push({100,1,17,1180,1200})&&m.push({160,1,17,1240,1200})&&m.sample(140,sampled,phase)&&std::fabs(phase-20.f/1200)<.00001f,"Mesmerize hold uses the accumulated native loop clock");
       check(!m.push({180,1,18,10,666}),"Mesmerize release requires a new event incarnation");
       check(m.push({180,2,18,0,666})&&m.push({210,2,18,30,666})&&m.sample(210,sampled,phase),"Mesmerize release starts its own timeline");
       check(!m.sample(411,sampled,phase),"stale Mesmerize release retires");}
      for(unsigned phase:{12u,14u}){
        auto bend=power;bend.powerVariant=phase;bend.powerDurationMs=phase==12?533:433;
        check(validShadowAction(bend,c,h),"observed Corvo Bend Time native start/release packet");
        auto bad=bend;bad.role=1;bad.character=2;check(!validShadowAction(bad,ec,eh),"Bend Time cannot impersonate Emily power");
        bad=bend;bad.version=32;check(!validShadowAction(bad,c,h),"pre-Bend Time wire semantics refused");
        bad=bend;bad.swordState=1;check(validShadowAction(bad,c,h),"native holstered Bend Time preserves equipment state");
        auto air=bend;air.airStage=2;check(validShadowAction(air,c,h),"observed falling Bend Time retains native cast clock");
        air.airStage=1;air.airStartZ=67;air.airTargetZ=68.5f;check(validShadowAction(air,c,h),"observed airborne Bend Time preserves source jump apex");
        air.swordState=1;check(validShadowAction(air,c,h),"airborne Bend Time does not require drawing an empty sword slot");
        bad=air;bad.version=33;check(!validShadowAction(bad,c,h),"ground-only Bend Time peer cannot join the expanded air protocol");
        bad=air;bad.airStage=2;check(!validShadowAction(bad,c,h),"falling Bend Time cannot reuse stale apex metadata");
        bad=air;bad.airTargetZ=99;check(!validShadowAction(bad,c,h),"Bend Time rejects implausible native jump height");
        bad=air;bad.powerVariant=13;bad.powerDurationMs=300;check(!validShadowAction(bad,c,h),"unobserved hold cannot impersonate an airborne cast");
        bad=bend;bad.powerDurationMs++;check(!validShadowAction(bad,c,h),"incorrect Bend Time phase duration refused");
      }
      auto empty=power;empty.powerVariant=15;empty.powerDurationMs=333;
      check(validShadowAction(empty,c,h),"owned Corvo insufficient-mana response");
      auto invalidEmpty=empty;invalidEmpty.airStage=2;check(!validShadowAction(invalidEmpty,c,h),"unobserved airborne failure refused");
      invalidEmpty=empty;invalidEmpty.swordState=1;check(validShadowAction(invalidEmpty,c,h),"observed empty-hand failure retains holstered equipment");
      invalidEmpty=empty;invalidEmpty.role=1;invalidEmpty.character=2;check(!validShadowAction(invalidEmpty,ec,eh),"Emily failure requires separate observed binding");
      invalidEmpty=empty;invalidEmpty.version=35;check(!validShadowAction(invalidEmpty,c,h),"older peer cannot receive expanded failure contexts");
      auto hold=power;hold.powerVariant=13;hold.powerDurationMs=300;check(!validShadowAction(hold,c,h),"asset-only Bend Time hold stays unbound until observed");
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
      {auto water=high;water.traversalStart[2]=65.0f;water.traversalGoal[2]=68.07f;check(validShadowAction(water,c,h),"native high mantle out of water rises 3.07m from swim depth (1054d)");
       water.traversalStart[2]=64.0f;water.traversalGoal[2]=68.7f;check(!validShadowAction(water,c,h),"mantle rise above 4m refused");}
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
      equip.equipmentVariant=9;check(!validShadowAction(equip,c,h),"unknown equipment action refused");equip.equipmentVariant=1;
      for(unsigned variant:{7u,8u}){auto quick=equip;auto ec=c,eh=h;ec.role=0;eh.role=1;quick.role=1;quick.character=2;quick.equipmentVariant=variant;quick.equipmentDurationMs=333;quick.equipmentTimeMs=150;
       check(validShadowAction(quick,ec,eh),"observed Emily quick holster packet accepted");
       auto bad=quick;bad.equipmentDurationMs=334;check(!validShadowAction(bad,ec,eh),"quick holster exact duration required");
       bad=quick;bad.role=0;bad.character=1;check(!validShadowAction(bad,c,h),"unobserved Corvo quick holster refused");
       bad=quick;bad.version=57;check(!validShadowAction(bad,ec,eh),"old peer cannot accept new equipment semantic");}
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
      for(unsigned v:{15u,16u}){
        CombatPresentation recovery;
        const auto duration=v==15?866u:666u;
        check(recovery.push({100,1,v,1,0,duration})&&recovery.push({150,1,v,1,50,duration}),"owned native hit recovery timeline");
        check(!recovery.push({200,2,v,1,0,duration+1})&&!recovery.push({200,2,v,2,0,duration}),"unknown recovery duration and loop refused");
        const float start=CombatPresentation::continuationStart(v,v-10,.37f,1);
        check(std::fabs(start-.37f)<1e-6f&&CombatPresentation::clip(v,1)==CombatPresentation::clip(v-10,1),"hit recovery continues the same authored attack");
        float previous=start;for(unsigned i=0;i<=20;++i){const float value=CombatPresentation::authoredAmount(v,i/20.f,start);check(value>=previous&&value<=1,"recovery cannot replay strike backwards");previous=value;}
        check(previous==1&&CombatPresentation::continuationStart(v,0,0,0)==.5f,"recovery reaches authored finish with bounded late-entry fallback");
      }
      {CombatPresentation kick;CombatPresentation::Sample k{};float u=0;
       check(kick.push({100,1,17,1,0,800})&&kick.push({200,1,17,1,100,800})&&kick.sample(180,k,u)&&k.variant==17&&std::fabs(u-.1f)<1e-6f,"kick uses the delayed source clock");
       check(CombatPresentation::clip(17,1)==353&&!CombatPresentation::recovery(17),"kick addresses its complete body clip, not hit recovery");
       check(!kick.push({250,2,17,1,0,801})&&!kick.push({250,2,17,2,0,800})&&!kick.push({250,2,17,1,801,800}),"kick duration, loop and expiry guarded");}
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
      t.clear();check(t.push({100,1,2,7,100,333})&&t.push({150,2,1,7,150,333})&&t.sample(150,o,a)&&o.sword==1&&o.variant==7,"quick holster respects native sword contact event");
      t.clear();check(t.push({100,1,2,8,100,333})&&t.push({150,2,1,8,150,333})&&t.sample(150,o,a)&&o.sword==1&&o.variant==8,"crouched quick holster respects native sword contact");
      t.clear();check(!t.push({100,1,1,8,334,333}),"expired crouched quick holster refused");
      t.clear();check(!t.push({100,1,1,7,334,333}),"expired quick holster cannot enter timeline");
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
    s.player.current=0;s.active={{0,1,base+0x1c85628},{64,2,base+0x1c79cf8},{53,3,base+0x1c79cf8}};
    check(nativePlayerShadowAction(s,base,action)&&action==7,"dual holster reaches strict two-scene ownership validation");
    s.active[1].vtable++;check(!nativePlayerShadowAction(s,base,action),"foreign dual holster class refused");
    s.active[1].vtable--;s.active[1].id=63;check(!nativePlayerShadowAction(s,base,action),"cast state cannot impersonate power holster");
    s.active[1].id=64;s.active[0]={2,1,base+0x1c85028};check(!nativePlayerShadowAction(s,base,action),"airborne dual holster remains unsupported");
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
    {
        PowerFailureFxOrder order;
        check(!order.consume(0,2,1,15,.1f,true)&&!order.consume(1,2,1,15,.1f,false),"failure FX requires current ownership");
        check(!order.consume(1,2,1,12,.1f,true),"successful cast is not failure FX");
        check(order.consume(1,2,2,15,.1f,true),"confirmed early failure presents once");
        check(!order.consume(1,2,2,15,.2f,true)&&!order.consume(1,2,1,15,.1f,true),"same and older failure events cannot replay");
        check(!order.consume(1,2,3,15,.8f,true)&&!order.consume(1,2,3,15,.1f,true),"late failure is consumed without delayed replay");
        check(!order.consume(1,2,4,15,NAN,true)&&!order.consume(1,2,4,15,-.1f,true),"invalid failure timing refused");
        check(order.consume(1,3,1,15,.1f,true),"new validated peer incarnation admits its own event");
        check(!order.consume(1,3,1,15,.1f,true),"native start attempt remains consumed even if start fails");
    }
    std::cout<<"PASS: clone action roles/tokens/worlds, replay/event integrity, phase/freshness, and exact native slash/block classes\n";
}
