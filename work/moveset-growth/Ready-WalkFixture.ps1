"" # Prepare swords after Restore-D2Lab -Doppelganger; native scene timing.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab'
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function B($r){Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json}
foreach($r in 'host','client'){
 $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
 if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'}
 if((B $r).localGameLogicState-ne1){throw 'Fixture not in gameplay'}
 C $r 'coop_movement_bank off';C $r 'coop_authoring_proof off';C $r 'coop_shadow_stream off'
 C $r 'setviewpos -39.4 -161.4 68.1 270 20'
}
Start-Sleep -Milliseconds 600
foreach($r in 'host','client'){& "$lab/Send-D2Input.ps1" -Role $r -Button X -DurationMs 400|Out-Null}
$watch=[Diagnostics.Stopwatch]::StartNew();$seen=@{host=$false;client=$false};$records=[Collections.Generic.List[object]]::new()
do{
 $ready=$true
 foreach($r in 'host','client'){
  $s=B $r;if($s.localGameLogicState-eq5){$seen[$r]=$true}
  $records.Add([pscustomobject]@{role=$r;ms=$watch.ElapsedMilliseconds;logic=$s.localGameLogicState;epoch=$s.localEpoch})
  if(!$seen[$r]-or$s.localGameLogicState-ne1-or!$s.remoteFresh){$ready=$false}
 }
 if(!$ready){Start-Sleep -Milliseconds 300}
}while(!$ready-and$watch.ElapsedMilliseconds-lt55000)
$records|ConvertTo-Json|Set-Content "$PSScriptRoot/ready-fixture-scene.json"
if(!$ready){throw 'Native initialization scene did not complete on both roles'}
foreach($r in 'host','client'){
 & "$lab/Send-D2Input.ps1" -Role $r -DurationMs 100|Out-Null
 C $r 'coop_probe_follow exact';C $r 'coop_probe_locomotion on';C $r 'coop_shadow_loco combat'
}
C 'host' 'setviewpos -33 -153 68.1 0 0';C 'client' 'setviewpos -29 -151.5 68.1 204 11'
Start-Sleep -Milliseconds 700
foreach($r in 'host','client'){$s=B $r;if(!$s.probeFollowEnabled-or!$s.remoteFresh){throw 'Follow did not return'};[pscustomobject]@{Role=$r;Logic=$s.localGameLogicState;Follow=$s.probeFollowEnabled;RemoteFresh=$s.remoteFresh}}
