from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset';S=R/'outputs/d2-coop-lab/src'
p=S/'shadow_action_protocol_tests.cpp';s=p.read_text();mark='    {auto heart=p;heart.state=19;';assert mark in s
s=s.replace(mark,'''    {auto trap=p;trap.state=20;trap.trapHeld=2;trap.trapStock=1;trap.trapVariant=5;trap.trapDurationMs=300;
      check(validShadowAction(trap,c,h),"native spring razor placement presentation");
      for(unsigned v=1;v<=6;++v){auto phase=trap;phase.trapVariant=v;phase.trapDurationMs=TrapPresentation::duration(v);check(validShadowAction(phase,c,h),"native trap phase accepted");}
      auto bad=trap;bad.version=60;check(!validShadowAction(bad,c,h),"pre-trap wire refused");
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
'''+mark);p.write_text(s)
b=(W/'Build-Traps-v567.ps1').read_text().replace('v567','v569').replace('Build567','Build569').replace('heart-banks-v564/corvo-heart-bank.bin','trap-banks-v569/corvo-trap-bank.bin').replace('heart-banks-v564/emily-heart-bank.bin','trap-banks-v569/emily-trap-bank.bin').replace('340 clip','347 clip');(W/'Build-Traps-v569.ps1').write_text(b)

s=(P/'Test-HeartNetwork-v565.ps1').read_text().replace('heart','trap').replace('Heart','Trap').replace('565','570').replace('v564','v569')
s=s.replace("Take='trap-network-v570')","Take='trap-network-v570',[ValidateSet('stun','razor')][string]$Trap='stun')")
s=s.replace('$pids=@{};',"$mode=if($Trap-eq'stun'){'--player-stunmine'}else{'--player-springrazor'};$kind=if($Trap-eq'stun'){1}else{2};$pids=@{};")
s=s.replace("'--player-trap'",'$mode').replace("C $Source 'coop_trap_fixture 20000'",'C $Source "coop_trap_fixture $Trap"')
s=s.replace('-eq60','-eq61').replace('-ne60','-ne61').replace('-match\'trap\'','-match\'gadget_stunmine_\'')
s=s.replace('I $Source Neutral 250 255','I $Source Neutral 650 255')
s=s.replace('foreach($variant in 0,1,2,3)', 'foreach($variant in 0,1,2,3,4,5,6)').replace('$_.movement.trapHeld-eq1','$_.movement.trapHeld-eq$kind').replace('$_.movement.trapWeight-gt.8-and$_.movement.weight-gt.9','$_.movement.trapWeight-gt.3-and$_.movement.weight-gt.85')
needle=" C $peer 'coop_movement_bank off';foreach($r in 'host','client'){C $r 'coop_shadow_stream off'};Start-Sleep -Milliseconds 350"
s=s.replace(needle,""" if(!@($rows|Where-Object {$_.item.quantity-eq0-and$_.movement.trapStock-eq0-and$_.movement.trapHeld-eq$kind-and!$_.prop.active}).Count){throw 'Empty trap prop still visible'};$checks.Add('Consumed inventory removes held remote gadget')
"""+needle)
s=s.replace("scope='Native Trap equip/use/holster and selected idle through Steam with existing authored full-body bank60 and native render-only held prop. Crouch uses existing grounded lower-body blend. Trap mechanism heartbeat, targeting dialogue effects and visual polish remain unaccepted.'", "trap=$Trap;scope='Native trap equip/ready-loop/place/empty/holster and selected idle through Steam. Bank61/347 and static native held props, no animation gameplay events. Local native placement consumes stock; world trap replication is separate and not claimed. Fine grip, surface contact, reload/ready-out/slide remain unverified.'")
(P/'Test-TrapNetwork-v570.ps1').write_text(s)
s=(W/'stage_traps_v567.py').read_text().replace('candidate-trap-observation-v567','candidate-trap-network-v569').replace('candidate-heart-network-v564','candidate-trap-observation-v567').replace("f'shadowwalk-body-{tag}-v567.log'","f'shadowwalk-body-{tag}-v569.log'")
s=s.replace('heart-banks-v564','trap-banks-v569').replace("shutil.copy2(L/'assets/heart_prop.def',A/'heart_prop.def')", "shutil.copy2(L/'assets/stunmine_prop.def',A/'stunmine_prop.def');shutil.copy2(L/'assets/springrazor_prop.def',A/'springrazor_prop.def')")
s=s.replace('prepare_traps_v567.py','implement_traps_v569.py').replace('Build-Traps-v567.ps1','Build-Traps-v569.ps1').replace('prepare_trap_runtime_v568.py','prepare_trap_runtime_v570.py').replace('protocol=60,bankVersion=60,clips=340','protocol=61,bankVersion=61,clips=347')
s=s.replace('Preserves Heart/Steam/bank60 behavior; adds isolated20s native trap acquisition and rollback plus read-only placeable ownership probes. Trap runtime observation pending.','Preserves prior340 clips. Adds native trap body/held-prop presentation through Steam, state20, protocol61. No remote native placement/damage calls. Runtime570 verification pending.')
(W/'stage_traps_v569.py').write_text(s)
s=(W/'Deploy-Heart-v564.ps1').read_text().replace('Heart','Traps').replace('heart','traps').replace('564','569')
s=s.replace('Build-D2TrapsPropAssets.ps1','Build-D2TrapPropAssets.ps1')
(W/'Deploy-Traps-v569.ps1').write_text(s)
print('Trap wire/motion checks, build, archive, asset deploy and both-role recording harness prepared.')
