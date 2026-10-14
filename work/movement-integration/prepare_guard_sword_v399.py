from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';out=P/'Test-GuardSwordControl-v399.ps1';assert not out.exists()
s=(P/'Test-GuardChokeControl-v390.ps1').read_text(encoding='utf-8-sig').replace('ordinary-guard-choke-control-v390','ordinary-guard-sword-control-v399').replace("if($dll-ne'440B0F9FD8BE7D24D22B62DE0BD7FB43C2E9C85FE6AB3551DA9A988AA273C6CD'){throw 'Expected archived387 candidate'}","$expected=Get-Content \"$root/work/movement-integration/candidate-partner-interaction-v396/identity.json\" -Raw|ConvertFrom-Json;if($dll-ne$expected.dll){throw 'Expected archived396 candidate'}")
s=s.replace("function I($r,$b='Neutral',$ms=100)","function I($r,$b='Neutral',$ms=100,$rt=0)").replace('-Button $b -DurationMs $ms|','-Button $b -DurationMs $ms -RightTrigger $rt|')
s=s.replace("name='rear-choke'","name='front-sword'").replace('-1.15*$forwardX','+1.15*$forwardX').replace('-1.15*$forwardY','+1.15*$forwardY').replace('$forwardX)*180/[Math]::PI','$forwardX)*180/[Math]::PI+180')
s=s.replace('I $source RightShoulder 2000','I $source Neutral 300 255')
s=s.replace('.assassination-partner-filter.json','.interaction-partner-filter.json').replace('$filterAfter.otherAccepted-le$filterBefore.otherAccepted','$filterAfter.otherReturned-le$filterBefore.otherReturned').replace("throw 'Ordinary native target eligibility not accepted'","throw 'Ordinary native interaction target not returned'")
start=s.index('   if(@($samples.player.active|');end=s.index('   $rows.Add(',start)
s=s[:start]+'''   if(!@($samples.player.active|Where-Object {$_.id-eq39}).Count){throw 'Native sword state39 not demonstrated'}
   $targetHandle=[Convert]::ToUInt32($target.handle.Substring(2),16)
   $paired=@($samples.midnight.instances|Where-Object {$_.state-eq3-and@($_.bindingsResearch|Where-Object {$_.handle-eq1-and$_.reciprocal}).Count-and@($_.bindingsResearch|Where-Object {$_.handle-eq$targetHandle-and$_.reciprocal}).Count})
   if(!$paired.Count){throw 'Ordinary guard paired sword scene not demonstrated'}
   Q $source '--health' $targetHandle|ConvertTo-Json -Depth 8|Set-Content "$out/guard-health-after.json"
   $checks.Add('Ordinary guard native interaction target returned; real RT input produces state39 and reciprocal player/guard paired scene')

'''+s[end:]
s=s.replace('Native RightShoulder held2000ms behind exact authored guard2; paired scene owner verification and visual review remain required.','Native RT300ms in front of exact authored guard2; source placed by read-only orientation, no direct target mutation; visual review remains required.')
start=s.index("scope='Ordinary native guard choke");end=s.index("'}|ConvertTo-Json",start)
s=s[:start]+"scope='Ordinary guard sword positive control for396. Native source placed in front of exact authored guard2, RT300ms, reciprocal player/guard paired scene and state39 required. Native target getter must return ordinary NPC. Checkpoint inventory/health and14 protected hashes restored; collision retained. Visual review separate."+s[end:]
out.write_text(s,encoding='utf-8');print('Prepared ordinary guard sword control399')
