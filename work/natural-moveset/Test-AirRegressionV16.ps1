param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='air-bank-v1-test')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$pids=@{};$failure=$null
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$x=0,$y=0,$button='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -LeftX $x -LeftY $y -Button $button -DurationMs $ms|Out-Null}
function B($r){Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json}
function M($r){Get-Content "$root/work/lab-instances/$r/coop-status.json.movement.json" -Raw|ConvertFrom-Json}
function N($r){for($i=0;$i-lt6;$i++){$raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$r] --player-states;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 12};throw 'Native read unavailable'}
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
  if(!(M $dst).active-or(M $dst).version-ne16){throw 'Air bank unavailable'}
  foreach($trial in 'standing','held','crouched','moving'){
   $stance=if($trial-eq'crouched'){1}else{0}
   if((B $src).localPlayerStance-ne$stance){I $src 0 0 B 180;Start-Sleep -Milliseconds 1200}
   C $src 'setviewpos -35 -153 68.1 0 0';Start-Sleep -Milliseconds 850
   if($trial-eq'moving'){I $src 0 20000 Neutral 500;Start-Sleep -Milliseconds 300}
   $y=if($trial-eq'moving'){20000}else{0};$duration=if($trial-eq'held'){800}else{180}
   I $src 0 $y A $duration;$watch=[Diagnostics.Stopwatch]::StartNew();$rise=$false;$fall=$false;$land=$false;[double]$progress=0
   while($watch.ElapsedMilliseconds-lt2800){
    $n=N $src;$m=M $dst;$stream=Get-Content "$root/work/lab-instances/$dst/coop-status.json.shadow-stream.json" -Raw|ConvertFrom-Json
    $rows.Add([pscustomobject]@{source=$src;trial=$trial;ms=$watch.ElapsedMilliseconds;native=$n;movement=$m;stream=$stream;bridge=(B $dst)})
    if($m.airState-eq1-and$m.airWeight-gt.6-and$m.weight-gt.6){$rise=$true}
    # Status is sampled at 250 ms. Falling retains the final rising parameter,
    # so a short native rise cannot disappear between status publications.
    if($m.airState-in1,2-and$m.airWeight-gt.6){$progress=[Math]::Max([double]$progress,[double]$m.airProgress)}
    if($m.airState-eq2-and$m.airWeight-gt.8){$fall=$true}
    if($m.airState-eq3-and$m.airWeight-gt.8){$land=$true}
    if(!$m.active-or!$stream.armed){throw "Air presentation retired: $src $trial $($stream.result)"}
    Start-Sleep -Milliseconds 12
   }
   if(!$rise-or!$fall-or!$land-or$progress-lt.65){throw "Air phase coverage missing: $src $trial rise=$rise fall=$fall land=$land progress=$progress"}
   $checks.Add("$src $trial height-driven rising pose, native falling and grounded landing all observed")
   $m=M $dst;if($m.airState-or$m.airWeight-gt.01-or$m.weight-lt.9){throw 'Air recovery did not return to gait'}
   if((N $dst).active.id-contains2-or(N $dst).active.id-contains3){throw 'Remote animation changed observer player movement state'}
   $checks.Add("$src $trial recovers to native-driven gait without changing observer first-person movement state")
  }
  if((B $src).localPlayerStance-eq1){I $src 0 0 B 180;Start-Sleep -Milliseconds 1200}
  C $dst 'coop_movement_bank off';foreach($r in 'host','client'){C $r 'coop_shadow_stream off'}
 }
 foreach($r in 'host','client'){$h=& "$lab/bin/d2_runtime_probe.exe" $pids[$r] --health 1|ConvertFrom-Json;if($LASTEXITCODE-or$h.current-ne56){throw 'Player health changed'}}
 $checks.Add('Native player health unchanged on both roles')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';if((B $r).localPlayerStance-eq1){I $r 0 0 B 180};C $r 'setviewpos -35 -153 68.1 0 0'}catch{}}
 $rows|ConvertTo-Json -Depth 12|Set-Content "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;checks=@($checks);dll=(Get-FileHash "$lab/bin/dinput8.dll").Hash;scope='Standing, held, crouched and moving ordinary jumps on both characters. Double jump, hard impacts, arbitrary airborne attacks, tall falls and visual acceptance are not covered.'}|ConvertTo-Json -Depth 6|Set-Content "$out/result.json"
}
[pscustomobject]@{Passed=(!$failure);Failure=$failure;Checks=$checks.Count};if($failure){throw $failure}
