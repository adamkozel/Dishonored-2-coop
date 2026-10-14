from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';W=R/'work/movement-integration'
p=S/'heart_prop.h';s=p.read_text().replace('fprintf(f,"\\"testFixtureOnly','fprintf(f,"{\\"testFixtureOnly');p.write_text(s)
p=S/'shadow_action_protocol_tests.cpp';s=p.read_text();mark='    {auto unbound=p;unbound.state=18;';assert mark in s
s=s.replace(mark,'''    {auto heart=p;heart.state=19;heart.heartHeld=1;heart.heartVariant=3;heart.heartDurationMs=1000;
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
'''+mark);p.write_text(s)
b=(W/'Build-Heart-v562.ps1').read_text().replace('v562','v564').replace('Build562','Build564').replace('shadowwalk-sneak-banks-v548/corvo-shadowwalk-sneak-bank.bin','heart-banks-v564/corvo-heart-bank.bin').replace('shadowwalk-sneak-banks-v548/emily-shadowwalk-sneak-bank.bin','heart-banks-v564/emily-heart-bank.bin').replace('336 clip','340 clip');(W/'Build-Heart-v564.ps1').write_text(b)
print('Heart protocol and motion regression tests added.')
