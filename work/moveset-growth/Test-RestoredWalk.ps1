$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab'
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function B($r){Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json}
function M($r){Get-Content "$root/work/lab-instances/$r/coop-status.json.movement.json" -Raw|ConvertFrom-Json}
$results=[Collections.Generic.List[object]]::new();$failure=$null
try{
 foreach($src in 'host','client'){
  $dst=if($src-eq'host'){'client'}else{'host'}
  foreach($r in 'host','client'){
   $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
   if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong test process'}
   & "$lab/Send-D2Input.ps1" -Role $r -DurationMs 100|Out-Null
   C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_probe_follow exact';C $r 'coop_probe_locomotion on';C $r 'coop_shadow_loco combat'
  }
  C $src 'setviewpos -33 -154.8 68.1 0 0';C $dst 'setviewpos -28 -151.5 68.1 214 11';Start-Sleep -Milliseconds 600
  foreach($r in 'host','client'){C $r 'coop_shadow_stream 15000'};Start-Sleep -Milliseconds 500
  C $dst 'coop_movement_bank 60000';Start-Sleep -Milliseconds 600
  $before=B $src;$m0=M $dst;if(!$m0.active-or$m0.weight-lt.9){throw "Restored $dst bank did not start"}
  & "$lab/Send-D2Input.ps1" -Role $src -LeftY 11000 -DurationMs 1400|Out-Null
  Start-Sleep -Milliseconds 1000;$moving=B $src;$m1=M $dst;$target=B $dst
  if(!$m1.active-or$m1.weight-lt.9-or$m1.applied-le$m0.applied-or$m1.speed-lt.1-or!$target.probeFollowEnabled){throw "Restored $dst walking did not publish"}
  $distance=[Math]::Sqrt([Math]::Pow($moving.localPosition[0]-$before.localPosition[0],2)+[Math]::Pow($moving.localPosition[1]-$before.localPosition[1],2))
  if($distance-lt.2){throw 'No native source movement'}
  & "$lab/Send-D2Input.ps1" -Role $src -DurationMs 100|Out-Null
  C $dst 'coop_movement_bank off';Start-Sleep -Milliseconds 400;$off1=M $dst;Start-Sleep -Milliseconds 300;$off2=M $dst
  if($off2.active-or$off1.applied-ne$off2.applied){throw 'Cancellation failed'}
  $results.Add([pscustomobject]@{source=$src;epoch=$moving.localEpoch;targetHandle=$target.probeActorHandle;distance=$distance;before=$m0;during=$m1;after=$off2;passed=$true})
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{& "$lab/Send-D2Input.ps1" -Role $r -DurationMs 100|Out-Null;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_probe_follow exact';C $r 'coop_shadow_loco combat'}catch{}}
 try{C 'host' 'setviewpos -33 -153 68.1 0 0';C 'client' 'setviewpos -29 -151.5 68.1 204 11'}catch{}
 [ordered]@{passed=(!$failure);failure=$failure;cases=@($results);scope='Re-arming the existing walking bank on fresh post-reload clone handles; both role forward motion and cancellation. Not a coordinated reload test.'}|ConvertTo-Json -Depth 7|Set-Content "$PSScriptRoot/restored-walk-result.json"
}
if($failure){throw $failure};Get-Content "$PSScriptRoot/restored-walk-result.json"
