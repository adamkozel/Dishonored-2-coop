param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='slide-bank-v1-test',[switch]$AuthoredAir)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$pids=@{};$failure=$null
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$x=0,$y=0,$button='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -LeftX $x -LeftY $y -Button $button -DurationMs $ms|Out-Null}
function B($r){Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json}
function M($r){Get-Content "$root/work/lab-instances/$r/coop-status.json.movement.json" -Raw|ConvertFrom-Json}
function N($r){for($i=0;$i-lt6;$i++){$raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$r] --player-states;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw 'Native read unavailable'}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong process'};$pids[$r]=$l.pid
  I $r;foreach($c in 'coop_review_clip off','coop_movement_bank off','coop_shadow_stream off','coop_mixamo_crouch off','coop_probe_body off','coop_probe_follow exact','coop_probe_locomotion on','coop_shadow_loco combat'){C $r $c}
  if((B $r).localPlayerStance-eq1){I $r 0 0 B 180;Start-Sleep -Milliseconds 1100}
 }
 foreach($src in 'host','client'){
  $dst=if($src-eq'host'){'client'}else{'host'};C $dst 'setviewpos -31 -146 68.1 270 8'
  foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 600;C $dst 'coop_movement_bank on';Start-Sleep -Milliseconds 900
  if(!(M $dst).active-or(M $dst).version-lt6-or($AuthoredAir-and(M $dst).version-lt7)){throw 'Slide bank unavailable'}
  foreach($trial in 'natural-crouch-recovery','jump-interruption'){
   if((B $src).localPlayerStance-eq1){I $src 0 0 B 180;Start-Sleep -Milliseconds 1200}
   C $src 'setviewpos -35 -153 68.1 0 0';Start-Sleep -Milliseconds 750
   I $src 0 32767 LeftThumb 1000;Start-Sleep -Milliseconds 350;I $src 0 32767 B 500
   $watch=[Diagnostics.Stopwatch]::StartNew();$seenNative=$false;$seenRemote=$false;$seenRecovery=$false;$jumped=$false;$seenAir=$false
   while($watch.ElapsedMilliseconds-lt3500){
    $n=N $src;$m=M $dst;$stream=Get-Content "$root/work/lab-instances/$dst/coop-status.json.shadow-stream.json" -Raw|ConvertFrom-Json
    $rows.Add([pscustomobject]@{source=$src;trial=$trial;ms=$watch.ElapsedMilliseconds;native=$n;movement=$m;stream=$stream;bridge=(B $dst)})
    if($n.active.id-contains7-and$n.stance-eq5){$seenNative=$true}
    if($stream.remoteState-eq6-and$m.slideState-eq1-and$m.slideWeight-gt.8-and$m.weight-gt.8){$seenRemote=$true}
    if($m.slideState-eq2){$seenRecovery=$true}
    if($trial-eq'jump-interruption'-and!$jumped-and$seenRemote){I $src 0 20000 A 180;$jumped=$true}
    if($jumped-and($n.active.id-contains2-or$n.active.id-contains3)-and$stream.remoteState-eq3-and(($AuthoredAir-and$m.airWeight-gt.6-and$m.airState-in1,2)-or(!$AuthoredAir-and$m.weight-lt.01))){$seenAir=$true}
    if(!$m.active-or!$stream.armed){throw 'Slide retired the live presentation'}
    Start-Sleep -Milliseconds 18
   }
   if(!$seenNative-or!$seenRemote){throw "Native slide missing from presentation: $src $trial"}
   $checks.Add("$src $trial exact native slide selects authored slide without changing root ownership")
   if($trial-eq'natural-crouch-recovery'-and!$seenRecovery){throw 'Native slide exit did not start full-body recovery'}
   if($trial-eq'jump-interruption'-and!$seenAir){throw 'Native jump did not interrupt slide presentation'}
   $m=M $dst;if($m.slideState-or$m.slideWeight-gt.01-or$m.weight-lt.9){throw 'Slide did not recover to native-driven gait'}
   $checks.Add("$src $trial recovery or interruption returns to native-driven movement")
  }
  if((B $src).localPlayerStance-eq1){I $src 0 0 B 180;Start-Sleep -Milliseconds 1200}
  C $dst 'coop_movement_bank off';foreach($r in 'host','client'){C $r 'coop_shadow_stream off'}
 }
 foreach($r in 'host','client'){$h=& "$lab/bin/d2_runtime_probe.exe" $pids[$r] --health 1|ConvertFrom-Json;if($LASTEXITCODE-or$h.current-ne56){throw 'Player health changed'}}
 $checks.Add('Native player health unchanged on both roles')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';if((B $r).localPlayerStance-eq1){I $r 0 0 B 180};C $r 'setviewpos -35 -153 68.1 0 0'}catch{}}
 $rows|ConvertTo-Json -Depth 12|Set-Content "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;checks=@($checks);dll=(Get-FileHash "$lab/bin/dinput8.dll").Hash;scope='Native sprint-to-slide, default crouch recovery and jump interruption on both characters. Low ceilings, slopes, combat, victim contact and visual acceptance remain separate.'}|ConvertTo-Json -Depth 6|Set-Content "$out/result.json"
}
[pscustomobject]@{Passed=(!$failure);Failure=$failure;Checks=$checks.Count};if($failure){throw $failure}
