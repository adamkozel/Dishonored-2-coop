$ErrorActionPreference='Stop'
$lab=Join-Path (Split-Path $PSScriptRoot -Parent) 'outputs/d2-coop-lab'
$samples=[Collections.Generic.List[object]]::new()
foreach($role in 'host','client'){
 $launch=Get-Content "$PSScriptRoot/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
 $s=Get-Content "$PSScriptRoot/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json
 for($i=0;$i -lt 20;$i++){
  $raw=& "$lab/bin/d2_runtime_probe.exe" $launch.pid --bones $s.probeActorHandle;$code=$LASTEXITCODE
  $samples.Add([pscustomobject]@{Role=$role;Exit=$code;Native=$(if(!$code){$raw|ConvertFrom-Json}else{$null});State=(Get-Content "$PSScriptRoot/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json)})
  Start-Sleep -Milliseconds 70
 }
}
[ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$lab/bin/dinput8.dll").Hash;Scope='Idle native animation baseline with standing peers, after clean Dunwall restore. No body retargeting active.';Samples=$samples.ToArray()}|ConvertTo-Json -Depth 16|Set-Content "$PSScriptRoot/body-physics-idle-baseline.json"
foreach($role in 'host','client'){
 $rows=@($samples|Where-Object {$_.Role -eq $role -and $_.Native.stable});$max=0.0
 foreach($r in $rows){foreach($b in $r.Native.bodies){$sum=0.0;for($i=0;$i -lt 3;$i++){$sum+=[Math]::Pow($b.position[$i]-$b.expectedPosition[$i],2)};$max=[Math]::Max($max,[Math]::Sqrt($sum))}}
 [pscustomobject]@{Role=$role;CoherentSamples=$rows.Count;MaxError=$max;Applications=$rows[-1].State.bodyStreamApplied-$rows[0].State.bodyStreamApplied}
}
