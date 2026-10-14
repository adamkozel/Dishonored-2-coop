#pragma once
void carryPresentationTests(){
    check(carryTravelWeight(.8f,true)==1&&carryTravelWeight(0,true)==0&&carryTravelWeight(.8f,false)==0,
          "observed slow crouch carry steps without native FP walk scene; stopped or unknown carry stays still");
    CarryPresentation t;CarryPresentation::Sample s{};float a=0;
    const auto key=carryIdentityKey("ld_spawner_alexi_escape"),def=carryIdentityKey("models/characters/def/story/alexi_mayhew.def");
    check(key&&def&&!carryIdentityKey("")&&!carryIdentityKey("bad\nname"),"authored keys are bounded canonical strings");
    check(t.push({100,1,2,2980,3000,0,10,key,def})&&t.push({140,1,2,3020,3000,0,10,key,def}),"unwrapped native carry loop clocks accepted");
    check(t.sample(120,s,a)&&a==0,"carry loop wraps only after interpolation");
    check(!t.push({141,1,2,3010,3000,0,10,key,def}),"backward source carry time rejected");
    check(!t.push({141,1,2,3021,3000,0,11,key,def}),"same carry event cannot swap corpse generation");
    check(!t.push({141,1,2,3021,3000,0,10,key+1,def}),"same carry event cannot swap authored target");
    check(t.push({180,1,2,3060,3000,1,10,key,def})&&t.sample(179,s,a)&&s.stance==0&&t.sample(180,s,a)&&s.stance==1,"crouch transition follows delayed source boundary");
    check(t.push({220,2,6,0,1000,1,10,key,def})&&t.sample(219,s,a)&&s.variant==2,"future release does not erase delayed carry");
    check(t.sample(220,s,a)&&s.variant==6&&a==0,"native low sneak release has distinct timing");
    check(!t.sample(421,s,a),"missing peer carry samples expire");
    check(!CarryPresentation::valid({0,1,8,0,1000,0,10,key,def}),"unobserved carry variants refused");
    CarryPresentation::Sample high{100,1,8,0,666,0,10,key,def,{4,0,0}};
    check(CarryPresentation::valid(high),"observed standing high throw uses exact source duration and bounded velocity");
    auto invalidThrow=high;invalidThrow.stance=1;check(!CarryPresentation::valid(invalidThrow),"unobserved crouched throw refused");
    invalidThrow=high;invalidThrow.throwVelocity={0,0,0};check(!CarryPresentation::valid(invalidThrow),"throw without native source vector refused");
    invalidThrow=high;invalidThrow.throwVelocity={400,0,0};check(!CarryPresentation::valid(invalidThrow),"unbounded throw vector refused");
    invalidThrow=high;invalidThrow.throwVelocity[0]=std::numeric_limits<float>::quiet_NaN();check(!CarryPresentation::valid(invalidThrow),"nonfinite throw refused");
    invalidThrow=high;invalidThrow.variant=4;invalidThrow.duration=1000;check(!CarryPresentation::valid(invalidThrow),"low drop cannot retain a stale throw impulse");
    check(carryReleaseHandoffMs(8)==280&&carryReleaseElapsedMs(8,.5f)==333&&carryReleaseElapsedMs(4,.5f)==500,
          "high throw release clock uses666ms, independently of1000ms low drop");
    check(carryReleaseElapsedMs(2,.5f)==0xffffffffu&&carryReleaseElapsedMs(8,1.1f)==0xffffffffu,"loop and invalid clock cannot trigger release");
    CarryPresentation throwTimeline;check(throwTimeline.push(high),"throw timeline accepts native event");
    high.time=200;high.elapsed=100;check(throwTimeline.push(high)&&throwTimeline.sample(150,s,a)&&s.throwVelocity==high.throwVelocity&&std::fabs(a-50.f/666)<1e-6f,"throw interpolation preserves source vector and scene clock");
    check(!throwTimeline.sample(401,s,a),"stale source cannot drive a delayed throw");
    CarryPresentation::Sample compact{100,1,5,50,666,0,10,key,def,{}};
    check(CarryPresentation::valid(compact)&&carryReleaseHandoffMs(5)==166&&carryReleaseElapsedMs(5,.5f)==333,
          "compact fallback preserves native666ms carrier clock and distinct victim clip endpoint");
    compact.throwVelocity={4,0,0};check(!CarryPresentation::valid(compact),"compact drop cannot inherit high-throw impulse");
    check(CarryPresentation::valid({0,1,7,299,300,0,10,key,def})&&!CarryPresentation::valid({0,1,7,301,300,0,10,key,def}),"preparation observation is bounded to authored gather window");
    CarryPreparationClock prep;
    check(prep.observe(1000,0x100,10)==0&&prep.observe(1080,0x100,10)==80&&prep.observe(1270,0x100,10)==270&&prep.observe(1350,0x100,10)==300,"continuous native preparation measures elapsed time and holds endpoint");
    check(prep.observe(1400,0x100,11)==0&&prep.observe(1450,0x101,11)==0&&prep.observe(1700,0x101,11)==0,"recycled corpse, different native state and observation gap restart gather");
    prep.clear();check(prep.observe(1750,0x101,11)==0,"stream restart cannot inherit preparation clock");
    check(std::fabs(carryPickupAmount(7,1)-carryPickupAmount(1,0))<1e-6f&&carryPickupAmount(7,0)==0&&carryPickupAmount(1,1)==1,"gather and native lift meet and end at loaded pose without looping");
    CarryPresentation prepTimeline;
    check(prepTimeline.push({100,1,7,0,300,0,10,key,def})&&prepTimeline.push({140,1,7,40,300,0,10,key,def})&&prepTimeline.push({400,2,1,0,1000,0,10,key,def}),"observed preparation switches to native lift scene");
    float victim=99;
    check(carryVictimPickupAmount(7,0,1,victim)&&victim==0,"native preparation never raises the floor snapshot");
    check(carryVictimPickupAmount(1,0,.08f,victim)&&victim==0&&carryVictimPickupAmount(1,0,.44f,victim)&&std::fabs(victim-.5f)<1e-6f&&carryVictimPickupAmount(1,0,.81f,victim)&&victim==1,"victim transfer follows confirmed native hoist clock");
    check(carryVictimPickupAmount(2,1,0,victim)&&victim==1&&!carryVictimPickupAmount(7,1,0,victim)&&!carryVictimPickupAmount(4,0,.2f,victim),"legacy banks retain stance gating; release cannot masquerade as hoist");
    check(carryVictimPickupAmount(7,1,.4f,victim,true)&&victim==0&&carryVictimPickupAmount(1,1,.44f,victim,true)&&std::fabs(victim-.5f)<1e-6f,"new crouched hoist uses the confirmed source timing");
    check(!carryVictimPickupAmount(1,2,.4f,victim,true)&&!carryVictimPickupAmount(6,1,.4f,victim,true),"crouch permission does not allow invalid stance or drop as hoist");
    check(!CarryPresentation::valid({0,1,2,0,1000,0,10,key,def}),"carry duration mismatch refused");
    check(!CarryPresentation::valid({0,1,0,0,0,0,10,key,def}),"neutral carry cannot retain stale target");
    check(t.push({500,3,0})&&t.count==1&&t.sample(500,s,a)&&!s.variant,"long interruption clears old target samples");
    t.clear();
    check(t.push({100,1,2,2991,3000,0,10,key,def})&&t.push({185,1,2,76,3000,0,10,key,def}),"actual native carry clock wrap retains scene ownership");
    check(t.sample(109,s,a)&&a<.00001f,"wrapped loop interpolates through zero without running backwards");
    check(t.push({219,1,2,110,3000,0,10,key,def})&&t.samples[t.count-1].elapsed==3110,"next raw clock remains in the unwrapped cycle");
    check(!t.push({252,1,2,109,3000,0,10,key,def}),"small backwards phase is not mistaken for another full cycle");
}
