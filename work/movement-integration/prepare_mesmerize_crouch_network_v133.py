from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-MesmerizeLinks-v106.ps1').read_text(encoding='utf-8-sig')
def rep(a,b):
 global s
 assert s.count(a)==1,(a,s.count(a));s=s.replace(a,b)
s=s.replace('mesmerize-banks-v77/emily-mesmerize-bank.bin','mesmerize-context-banks-v132/emily-mesmerize-context-bank.bin').replace('$m.version-eq45','$m.version-eq46').replace('$m.version-ne45','$m.version-ne46').replace('bank45/276','bank46/285')
at=' $hwnd=(Get-Process -Id $pids.client).MainWindowHandle.ToInt64();'
rep(at,r''' & "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180|Out-Null
 $w.Restart();do{$m=S host '.movement.json';$native=Q client '--player-states';if($native.stance-eq1-and$m.stanceMix-gt.98-and$m.active){break};Start-Sleep -Milliseconds 30}while($w.ElapsedMilliseconds-lt3000)
 if($native.stance-ne1-or$m.stanceMix-lt.98-or!$m.active){throw 'Native and remote crouch not ready before cast'}
 [ordered]@{native=$native;remote=$m}|ConvertTo-Json -Depth 8|Set-Content "$out/crouch-ready.json"
 $checks.Add('Native crouch drives the remote Posture-100 stance before the cast')
'''+at)
rep('$worldFramed=$false', '$worldFramed=$false;$stood=$false')
rep('if(!$worldFramed-and$w.ElapsedMilliseconds-ge5500)', 'if(!$worldFramed-and$w.ElapsedMilliseconds-ge8500)')
at='  if(!$duplicated-and$w.ElapsedMilliseconds-ge8000)'
rep(at,r'''  if(!$stood-and$w.ElapsedMilliseconds-ge6200){
   & "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180|Out-Null;$stood=$true
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native stand after crouched cast recovery'})
  }
'''+at)
at=" $checks.Add('All exact source scene incarnations, including native aim-loop renewal, match remote phase events through Steam')"
rep(at,at+r'''
 $castRows=@($rows|Where-Object {$_.movement.powerVariant-in16,17,18-and$_.movement.powerWeight-gt.5})
 if(!$castRows.Count-or@($castRows|Where-Object {$_.player.stance-ne1-or$_.movement.stanceMix-lt.98}).Count){throw 'Power phases did not remain in the native/remote crouched context'}
 $standing=@($rows|Where-Object {$_.ms-gt7600-and$_.ms-lt8500-and$_.player.stance-eq0-and$_.movement.stanceMix-lt.02-and$_.movement.powerWeight-lt.02})
 if(!$stood-or!$standing.Count){throw 'Crouched cast did not recover into the native standing movement bank'}
 [ordered]@{crouchedPhaseSamples=$castRows.Count;standingRecoverySamples=$standing.Count;minimumCrouchedWeight=($castRows.movement.stanceMix|Measure-Object -Minimum).Minimum}|ConvertTo-Json|Set-Content "$out/crouch-context-summary.json"
 $checks.Add('All three source-native crouched phases drive the complete-body crouch clips; native standing recovery retains the movement bank')''')
rep('Native source Mesmerize acquisition and controller casting drive standing armed Emily body,', 'Native source Mesmerize acquisition and crouched armed controller casting drive the complete-body Emily stance candidate, followed by native standing recovery,')
(P/'Test-MesmerizeCrouchNetwork-v133.ps1').write_text(s,encoding='utf-8')
