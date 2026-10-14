param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='sprint-bank-v1-test')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$pids=@{}
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$x=0,$y=0,$button='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -LeftX $x -LeftY $y -Button $button -DurationMs $ms|Out-Null}
function B($r){Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json}
function M($r){Get-Content "$root/work/lab-instances/$r/coop-status.json.movement.json" -Raw|ConvertFrom-Json}
function Snap($src,$dst,$trial){
 $raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$src] --player-states
 if($LASTEXITCODE){return $null}
 $v=[pscustomobject]@{source=$src;trial=$trial;native=($raw|ConvertFrom-Json);sourceBridge=(B $src);targetBridge=(B $dst);movement=(M $dst);stream=(Get-Content "$root/work/lab-instances/$dst/coop-status.json.shadow-stream.json" -Raw|ConvertFrom-Json)};$rows.Add($v);return $v
}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong process'};$pids[$r]=$l.pid
  I $r;foreach($c in 'coop_review_clip off','coop_movement_bank off','coop_shadow_stream off','coop_mixamo_crouch off','coop_probe_body off','coop_probe_follow exact','coop_probe_locomotion on','coop_shadow_loco combat'){C $r $c}
  if((B $r).localPlayerStance-eq1){I $r 0 0 B 180;Start-Sleep -Milliseconds 1000}
 }
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 600
 foreach($r in 'host','client'){C $r 'coop_movement_bank on'};Start-Sleep -Milliseconds 600
 foreach($src in 'host','client'){
  $dst=if($src-eq'host'){'client'}else{'host'}
  foreach($trial in @(@('forward',0,32767,27),@('left',-16000,28000,27),@('right',16000,28000,28))){
   C $src 'setviewpos -35 -153 68.1 0 0';C $dst 'setviewpos -30.5 -150.2 68.1 214 13';Start-Sleep -Milliseconds 750
   if(!(M $dst).active-or(M $dst).version-ne4){throw 'Version 4 movement bank unavailable'}
   I $src $trial[1] $trial[2] LeftThumb 750
   $watch=[Diagnostics.Stopwatch]::StartNew();$samples=@()
   while($watch.ElapsedMilliseconds-lt1500){$v=Snap $src $dst $trial[0];if($v){$samples+=$v};Start-Sleep -Milliseconds 20}
   I $src
   $active=@($samples|Where-Object {$_.native.active.id-contains1-and$_.sourceBridge.localPlayerMotionValid-and$_.sourceBridge.localPlayerMotion-eq0-and$_.stream.remoteState-eq4-and$_.movement.sprintMix-gt.85-and$_.movement.weight-gt.9})
   if(!$active.Count){throw "Sprint was not natively bound: $src $($trial[0])"}
   if($trial[0]-ne'forward'-and!@($active|Where-Object {$_.movement.sprintSide-eq$trial[3]-and$_.movement.sprintDirection-gt.25}).Count){throw 'Wrong sprint diagonal blend'}
   $checks.Add("$src $($trial[0]): exact native sprint, Steam semantic 4, authored distance-clocked presentation")
   Start-Sleep -Milliseconds 900;$v=Snap $src $dst 'standing-recovery'
   if(!$v.stream.armed-or!$v.movement.active-or$v.movement.sprintMix-gt.01-or$v.movement.weight-lt.9){throw 'Sprint recovery failed'}
   $phase=$v.movement.phase;Start-Sleep -Milliseconds 400;$v=Snap $src $dst 'stationary'
   if([Math]::Abs($phase-$v.movement.phase)-gt.00005){throw 'Stopped sprint kept advancing foot phase'}
   $checks.Add("$src $($trial[0]): normal standing recovery and no cadence without travel")
  }
 }
 foreach($r in 'host','client'){
  $health=& "$lab/bin/d2_runtime_probe.exe" $pids[$r] --health 1|ConvertFrom-Json
  if($LASTEXITCODE-or$health.current-ne56){throw 'Player health changed'}
 }
 $checks.Add('Native player health unchanged on both roles')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off'}catch{}}
 $rows|ConvertTo-Json -Depth 12|Set-Content "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;checks=@($checks);dll=(Get-FileHash "$lab/bin/dinput8.dll").Hash;scope='Native armed sprint forward/diagonals and stationary recovery. No claim for full starts/stops, contact polish, upgrades or broader movement completion.'}|ConvertTo-Json -Depth 5|Set-Content "$out/result.json"
}
[pscustomobject]@{Passed=(!$failure);Failure=$failure;Checks=$checks.Count}
if($failure){throw $failure}
