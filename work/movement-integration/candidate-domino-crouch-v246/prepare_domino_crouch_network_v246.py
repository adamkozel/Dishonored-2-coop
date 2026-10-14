from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Test-DominoNetwork-v196.ps1').read_text(encoding='utf-8-sig')
a=s.index("    $catalog=Q client '--world-npcs'");b=s.index(' Start-Sleep -Milliseconds 750',a);s=s[:a]+s[b:]
s=s.replace('-Role host -Pitch -3','-Role host -Pitch 15').replace('-Role client -Pitch 3','-Role client -Pitch 12')
s=s.replace('domino-banks-v192/emily-domino-bank.bin','domino-context-banks-v243/emily-domino-context-bank.bin').replace('-eq47','-eq48').replace('-ne47','-ne48').replace('bank47/289','bank48/301')
a=s.index(' $hwnd=(Get-Process')
s=s[:a]+''' & "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180|Out-Null
 $w.Restart();do{$m=S host '.movement.json';$native=Q client '--player-states';if($native.stance-eq1-and$m.stanceMix-gt.98-and$m.active){break};Start-Sleep -Milliseconds 30}while($w.ElapsedMilliseconds-lt3000)
 if($native.stance-ne1-or$m.stanceMix-lt.98-or!$m.active){throw 'Native and peer crouch not ready before cast'}
 [ordered]@{native=$native;remote=$m}|ConvertTo-Json -Depth 8|Set-Content "$out/crouch-ready.json"
 $checks.Add('Native B establishes crouch and remote complete-body Posture100 context before casting')
''' +s[a:]
s=s.replace('$w.Restart();$cast=$false','$w.Restart();$cast=$false;$stood=$false')
s=s.replace("  $m=S host '.movement.json';$stream=S host '.shadow-stream.json'", """  if(!$stood-and$w.ElapsedMilliseconds-ge6500){& "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180|Out-Null;$stood=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native standing recovery after crouched cast'})}
  $m=S host '.movement.json';$stream=S host '.shadow-stream.json'""")
s=s.replace('19,20,21,22','19,20,22').replace('all four phases','three observed crouched phases').replace('Four native subscene transitions','Three native crouched subscene transitions')
a=s.index(' $nativeMarks=@(');b=s.index(" C client 'coop_domino_fixture off'",a)
s=s[:a]+''' $nativeMarks=@($rows|ForEach-Object {$_.effects.effects}|Where-Object name -like '*/phantasmal_link/target/*')
 if($nativeMarks.Count){throw 'Empty crouched cast unexpectedly selected a target'}
 $castRows=@($rows|Where-Object {$_.movement.powerVariant-in19,20,22-and$_.movement.powerWeight-gt.5})
 if(!$castRows.Count-or@($castRows|Where-Object {$_.player.stance-ne1-or$_.movement.stanceMix-lt.98}).Count){throw 'Cast left native or complete-body remote crouched context'}
 $standing=@($rows|Where-Object {$_.ms-gt8500-and$_.player.stance-eq0-and$_.movement.stanceMix-lt.02-and$_.movement.weight-gt.95})
 if($standing.Count-lt5){throw 'Native standing recovery not observed'}
 [ordered]@{crouchedPhaseSamples=$castRows.Count;standingRecoverySamples=$standing.Count;minimumCrouchedWeight=($castRows.movement.stanceMix|Measure-Object -Minimum).Minimum}|ConvertTo-Json|Set-Content "$out/crouch-context-summary.json"
 $checks.Add('All three native phases retain whole-body crouched context, then recover to standing without root-clock rejection or native target events')
''' +s[b:]
s=s.replace(";C $r 'coop_movement_fixture off'}catch{}", "}catch{}")
s=s.replace(' $rows|ConvertTo-Json -Depth 20', " foreach($r in 'host','client'){try{C $r 'coop_movement_fixture off'}catch{}}\n $rows|ConvertTo-Json -Depth 20")
s=s.replace('Native source Domino acquisition and targeted controller selection drive four standing armed Emily full-body phases', 'Native source Domino acquisition and crouched empty-target controller casting drive three observed armed Emily complete-body crouched phases then standing recovery')
s=s.replace('Remote target marks/links, AI and shared outcomes remain pending; receiver plays only body poses.', 'This take exercises no target marks or links. Their earlier scoped presentation proofs remain separate; NPC AI and shared outcomes are unfinished.')
(P/'Test-DominoCrouchNetwork-v246.ps1').write_text(s,encoding='utf-8')
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig').replace("'DominoObserve','DominoCrouchObserve'","'DominoObserve','DominoCrouchNetwork','DominoCrouchObserve'")
s=s.replace("  'DominoCrouchObserve'{", "  'DominoCrouchNetwork'{& \"$root/work/natural-moveset/Test-DominoCrouchNetwork-v246.ps1\" -Take $Take -Pitch $Pitch}\n  'DominoCrouchObserve'{");p.write_text(s,encoding='utf-8')
