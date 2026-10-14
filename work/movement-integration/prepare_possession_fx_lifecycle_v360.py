from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Test-PossessionFxLifecycle-v359.ps1').read_text()
a=s.index(" $final=Q host '--player-states';$final|ConvertTo-Json")
b=s.index(' $manaAfter=',a)
s=s[:a]+'''
 $cleanupStates=[Collections.Generic.List[object]]::new();$cleanupWait=[Diagnostics.Stopwatch]::StartNew()
 do{
  $final=Q host '--player-states';$cleanupStates.Add([pscustomobject]@{ms=$cleanupWait.ElapsedMilliseconds;stage='settle';player=$final})
  if($final.defaultMode-or$final.stance-eq18){break}
  Start-Sleep -Milliseconds 70
 }while($cleanupWait.ElapsedMilliseconds-lt4500)
 if($final.stance-eq18){
  if($Case-eq'cancel'){throw 'Cancellation unexpectedly entered native possession'}
  I host 255 120;$cleanupWait.Restart()
  do{
   $final=Q host '--player-states';$cleanupStates.Add([pscustomobject]@{ms=$cleanupWait.ElapsedMilliseconds;stage='leave';player=$final})
   if($final.defaultMode-and$final.stance-eq0){break}
   Start-Sleep -Milliseconds 70
  }while($cleanupWait.ElapsedMilliseconds-lt4500)
 }
 $cleanupStates|ConvertTo-Json -Depth 10|Set-Content "$out/cleanup-transitions.json"
 $final|ConvertTo-Json -Depth 8|Set-Content "$out/final-player.json"
 if(!$final.defaultMode-or$final.stance-ne0){throw 'Native input did not restore normal source mode'}
'''+s[b:]
(P/'Test-PossessionFxLifecycle-v360.ps1').write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text();assert s.count('Test-PossessionFxLifecycle-v359.ps1')==2;s=s.replace('Test-PossessionFxLifecycle-v359.ps1','Test-PossessionFxLifecycle-v360.ps1');p.write_text(s)
print('360 waits for actual native occupation before leave-host input and observes exit completion')
