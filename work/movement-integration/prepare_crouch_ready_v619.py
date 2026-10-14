from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-CrouchCombat-v617.ps1').read_text().replace('CrouchCombat617','CrouchCombat619')
mark='     Start-Sleep -Milliseconds 90;& "$lab/Send-D2Input.ps1"'
assert mark in s
s=s.replace(mark,'''     # Capture startup and camera collision trials can temporarily stall root
     # updates. Start the measured presentation lifetime only after setup.
     foreach($r in 'host','client'){C $r 'coop_shadow_stream off'}
     C $peer 'coop_movement_bank off';C $peer 'coop_movement_bank on'
     $bankWait=[Diagnostics.Stopwatch]::StartNew()
     do{$m=Get-Content "$root/work/lab-instances/$peer/coop-status.json.movement.json" -Raw|ConvertFrom-Json;if($m.active-and$m.version-eq65){break};Start-Sleep -Milliseconds 40}while($bankWait.ElapsedMilliseconds-lt5000)
     if(!$m.active-or$m.version-ne65){throw 'Measured bank not ready after camera setup'}
     foreach($r in 'host','client'){C $r 'coop_shadow_stream on'}
     $readyWatch=[Diagnostics.Stopwatch]::StartNew();$readyPair=@{}
     do{
      foreach($r in 'host','client'){$readyPair[$r]=Get-Content "$root/work/lab-instances/$r/coop-status.json.shadow-stream.json" -Raw|ConvertFrom-Json}
      $bothReady=$readyPair.host.ready-and$readyPair.host.armed-and$readyPair.client.ready-and$readyPair.client.armed
      if(!$bothReady){Start-Sleep -Milliseconds 40}
     }while(!$bothReady-and$readyWatch.ElapsedMilliseconds-lt6000)
     $readyPair|ConvertTo-Json -Depth 8|Set-Content "$out/stream-before-input.json"
     if(!$bothReady){throw 'Measured stream not ready after camera setup'}
'''+mark)
(N/'Test-CrouchCombat-v619.ps1').write_text(s)
print('Prepared619: fresh verified presentation lifetime after capture/camera setup; existing assertions unchanged.')
