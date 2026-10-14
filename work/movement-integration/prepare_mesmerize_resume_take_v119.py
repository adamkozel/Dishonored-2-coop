from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-MesmerizeLinksInterrupt-v115.ps1').read_text(encoding='utf-8-sig')
def rep(a,b):
 global s
 assert s.count(a)==1,(a,s.count(a));s=s.replace(a,b)
rep("'receiver-stop','source-pause','receiver-pause'", "'source-pause','receiver-pause'")
rep('$aiBefore=@{}', '$aiBefore=@{};$resumeBefore=@{};$recovered=$false')
rep(' foreach($r in \'host\',\'client\'){C $r \'coop_shadow_stream on\'};Start-Sleep -Milliseconds 600;C host \'coop_movement_bank on\'', ''' Copy-Item "$PSScriptRoot/mesmerize-banks-v77/corvo-mesmerize-bank.bin" "$root/work/lab-instances/client/coop-status.json.movement.bin"
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 600;foreach($r in 'host','client'){C $r 'coop_movement_bank on'}''')
at=" $hwnd=(Get-Process -Id $pids.client).MainWindowHandle.ToInt64();"
rep(at,''' foreach($r in 'host','client'){
  $w.Restart();do{$bank=S $r '.movement.json';$ss=S $r '.shadow-stream.json';if($bank.active-and$bank.version-eq45-and$ss.ready){break};Start-Sleep -Milliseconds 30}while($w.ElapsedMilliseconds-lt4000)
  if(!$bank.active-or$bank.version-ne45-or!$ss.ready){throw 'Both banks must be active before arming menu resume'}
  C $r 'coop_movement_resume on';$w.Restart();do{$resume=S $r '.movement-resume.json';if($resume.enabled-and$resume.phase-eq1){break};Start-Sleep -Milliseconds 25}while($w.ElapsedMilliseconds-lt1200)
  if(!$resume.enabled-or$resume.phase-ne1){throw 'Menu resume did not arm'}
  $resumeBefore[$r]=[ordered]@{resume=$resume;stream=$ss;bridge=(S $r);bank=$bank}
 }
 $resumeBefore|ConvertTo-Json -Depth 9|Set-Content "$out/resume-before.json"
 $checks.Add('Both exact opposite-role banks and same-world movement recovery armed before the native cast')
'''+at)
at="  $m=S host '.movement.json';$stream=S host '.shadow-stream.json'"
rep(at,'''  if($interrupted-and!$recovered-and$w.ElapsedMilliseconds-ge13000){
   $ready=$true;$recovery=@{}
   foreach($r in 'host','client'){
    $resume=S $r '.movement-resume.json';$ss=S $r '.shadow-stream.json';$bank=S $r '.movement.json';$bridge=S $r
    $recovery[$r]=[ordered]@{resume=$resume;stream=$ss;bank=$bank;bridge=$bridge}
    if($resume.phase-ne1-or$resume.resumed-le$resumeBefore[$r].resume.resumed-or!$ss.ready-or!$bank.active){$ready=$false}
   }
   $v=S host '.mesmerize-visual.json';$ws=S host '.mesmerize-world-stream.json';$ls=S host '.mesmerize-links.json'
   if(!$v.active-or!$ws.leased-or$ws.snapshot-ne1-or$ws.state-ne3-or!@($ls.links|Where-Object kind -eq 1).Count){$ready=$false}
   if($ready){
    foreach($r in 'host','client'){
     $now=$recovery[$r];$beforeResume=$resumeBefore[$r]
     if($now.resume.checksum-ne$beforeResume.resume.checksum-or$now.stream.token-eq$beforeResume.stream.token-or$now.stream.peerToken-eq$beforeResume.stream.peerToken-or$now.bridge.probeActorHandle-ne$beforeResume.bridge.probeActorHandle){throw 'Recovery changed pinned bank/actor or reused old tokens'}
    }
    $native=Q client '--mesmerize-world'
    if(!$native.present-or$native.state-ne3-or$native.handle-ne$committed.handle-or$ws.nativeHandle-ne$committed.handle-or$v.handle-ne$active.handle-or$v.event-eq$active.event){throw 'Recovery did not preserve the exact native world and cached model with a new stream event'}
    $recovery.native=$native;$recovery.visual=$v;$recovery.world=$ws;$recovery.links=$ls
    $recovery|ConvertTo-Json -Depth 12|Set-Content "$out/recovery.json";$recovered=$true
    $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Automatic fresh-token active-power snapshot recovered; no second cast input'})
   }elseif($w.ElapsedMilliseconds-gt19000){$recovery|ConvertTo-Json -Depth 10|Set-Content "$out/recovery-failure.json";throw 'Same-world active-power reacquisition deadline exceeded'}
  }
'''+at)
rep(" if(!$interrupted){throw 'Interruption stimulus not applied'}", " if(!$interrupted-or!$recovered){throw 'Native menu and active-power reacquisition not both confirmed'}\n $checks.Add('Fresh tokens and pinned banks automatically recover both movement presentations and the exact still-active native power without recasting or duplicating the world model')")
rep("if($m.powerWeight-gt.02){throw 'Interrupted remote cast retained weight'}", "if($m.powerWeight-gt.02-or!$m.active-or$m.weight-lt.9-or!$stream.ready){throw 'Resumed movement did not remain active after power retirement'}")
rep("($visual.started[1]-$visualBefore.started[1])-ne1", "($visual.started[1]-$visualBefore.started[1])-ne2")
rep("($visual.fxStarted-$visualBefore.fxStarted)-ne1-or($visual.fxStopped-$visualBefore.fxStopped)-ne1", "($visual.fxStarted-$visualBefore.fxStarted)-ne2-or($visual.fxStopped-$visualBefore.fxStopped)-ne2")
rep("$_.ms-gt15000-and($_.visual.active-or$_.visual.pendingHide-or$_.world.leased)", "$_.ms-gt30000-and($_.visual.active-or$_.visual.pendingHide-or$_.world.leased)")
rep("$_.ms-gt15000-and(@($_.eyes.eyes).Count-or@($_.links.links).Count)", "$_.ms-gt30000-and(@($_.eyes.eyes).Count-or@($_.links.links).Count)")
rep(" $checks.Add('Native menu or stream interruption retires the cosmetic model and body effect; native tails finish without stale revival')", " $checks.Add('Menu retires the first cosmetic lease; fresh source snapshot reuses the one model in its native loop; source retirement ends the second lease')")
rep(" $checks.Add('Duplicate lifecycle packet rejected without a second model or effect; no late world revival')", " $checks.Add('Duplicate lifecycle packet rejected; snapshot resumes only under new stream tokens and reuses the cached model')")
rep(" $checks.Add('All source-referenced eye and target effects retire after interruption without stale revival')", " $checks.Add('Source-referenced effects reacquire through fresh snapshot identity and retire again with the native world')")
rep(" foreach($r in 'host','client'){try{I $r;C $r 'coop_mesmerize_visual off'", " foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_resume off';C $r 'coop_mesmerize_visual off'")
rep('Committed-world interruption tests finite cleanup with automatic stream resumption disabled. Same-world reacquisition, aiming cancellation and natural lifetime remain unverified.', 'Same-world native menu recovery uses new Steam stream tokens, the same pinned banks/avatars/native committed world and one reused cosmetic model. A current state3 snapshot enters the loop without replaying the cast or spawn animation. No second cast input is issued. Aiming cancellation, natural lifetime and shared NPC outcomes remain unverified.')
(P/'Test-MesmerizeResume-v119.ps1').write_text(s,encoding='utf-8')
p=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig')
s=s.replace("'MesmerizeLinks','MesmerizeVisual'", "'MesmerizeLinks','MesmerizeResume','MesmerizeVisual'")
at="  'MesmerizeLinks'{";assert s.count(at)==1
s=s.replace(at,"  'MesmerizeResume'{& \"$root/work/natural-moveset/Test-MesmerizeResume-v119.ps1\" -Take $Take -Pitch $Pitch -Interrupt $Interrupt}\n"+at)
p.write_text(s,encoding='utf-8')
p=R/'outputs/d2-coop-lab/src/mesmerize_world_stream.h';s=p.read_text(encoding='utf-8-sig')
at='    fprintf(f,"{\\"eyeReadMaxMs\\":';assert s.count(at)==1
s=s.replace(at,'    fprintf(f,"{\\"snapshot\\":%u,\\"sourceSnapshot\\":%s,",s.presented.snapshot,s.sourceSnapshot?"true":"false");\n    fprintf(f,"\\"eyeReadMaxMs\\":')
p.write_text(s,encoding='utf-8')
print('Prepared active power recovery experiment and diagnostics')
