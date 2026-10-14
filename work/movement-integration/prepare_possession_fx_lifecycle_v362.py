from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Test-PossessionFxLifecycle-v360.ps1').read_text()
a=' if($Case-ne\'cancel\'){& "$lab/Send-D2Input.ps1" -Role host -Button X -LeftTrigger 255 -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 250;I host}'
assert a in s
s=s.replace(a,''' $beforeCleanup=Q host '--player-states';$beforeCleanup|ConvertTo-Json -Depth 10|Set-Content "$out/before-cleanup-player.json"
 if($Case-ne'cancel'){
  if($beforeCleanup.defaultMode-and@($beforeCleanup.active|Where-Object id -eq 63).Count-eq1){
   & "$lab/Send-D2Input.ps1" -Role host -Button X -LeftTrigger 255 -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 250
  }
  I host
 }
''')
s=s.replace(' # Native cancellation while LT remains held, then neutral input. No synthetic gameplay outcome.',' # Cancel only a confirmed active native UsePower63 aim. Menus may already cancel it; pressing LT again would start a new cast.')
(P/'Test-PossessionFxLifecycle-v362.ps1').write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text();assert s.count('Test-PossessionFxLifecycle-v360.ps1')==2;s=s.replace('Test-PossessionFxLifecycle-v360.ps1','Test-PossessionFxLifecycle-v362.ps1');p.write_text(s)
print('362 cleanup cancels only observed UsePower63; avoids re-triggering after native menu already cancels aim')
