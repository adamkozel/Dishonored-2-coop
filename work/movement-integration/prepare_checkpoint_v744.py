from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-DoorReload-v743.ps1').read_text()
a="""   foreach($r in 'host','client'){
    C $r 'coop_checkpoint_sync retry_probe'
    if(!(RS $r).retryProbeUntil){throw 'Native retry probe arm refused'}
   }
"""
assert s.count(a)==1
s=s.replace(a,'')
a='   $key="$source-$($case.name)";'
assert a in s
s=s.replace(a,"""   foreach($r in 'host','client'){
    C $r 'coop_checkpoint_sync retry_probe'
    $probeWatch=[Diagnostics.Stopwatch]::StartNew()
    do{$armed=RS $r;if($armed.retryProbeUntil){break};Start-Sleep -Milliseconds 35}while($probeWatch.ElapsedMilliseconds-lt2500)
    $armed|ConvertTo-Json -Depth 8|Set-Content "$out/retry-probe-arm-$r.json"
    if(!$armed.retryProbeUntil){throw 'Native retry probe did not arm'}
   }
"""+a)
s=s.replace('NativeDoorRetryProbe743','NativeDoorRetryProbe744').replace('native-door-retry-probe-client-v743','native-door-retry-probe-client-v744').replace('$peerCapture.WaitForExit(35000)','$peerCapture.WaitForExit(45000)')
old="  foreach($r in 'host','client'){try{I $r;C $r 'coop_checkpoint_sync off';C $r 'coop_checkpoint_delay 0';C $r 'coop_door_sync off';C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_movement_fixture off'}catch{}}"
assert old in s
s=s.replace(old,"  foreach($r in 'host','client'){try{I $r}catch{}}")
s=s.replace('  if($dirty){try{',old.replace('try{I $r;','try{')+'\n  if($dirty){try{')
(N/'Test-DoorReload-v744.ps1').write_text(s)
print('Prepared readiness-based retry probe before native door input')
