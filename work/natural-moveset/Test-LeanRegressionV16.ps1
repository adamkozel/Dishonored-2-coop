param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='lean-bank-v1-test')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$pids=@{};$failure=$null
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$x=0,$y=0,$button='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -LeftX $x -LeftY $y -Button $button -DurationMs $ms|Out-Null}
function B($r){Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json}
function M($r){Get-Content "$root/work/lab-instances/$r/coop-status.json.movement.json" -Raw|ConvertFrom-Json}
function N($r){for($i=0;$i-lt6;$i++){$raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$r] --player-states;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw 'Native read unavailable'}
function ExitLean($r){
 I $r 0 -22000 Neutral 350;Start-Sleep -Milliseconds 800
 if((N $r).active.id-contains18){C $r 'setviewpos -35 -153 68.1 90 0';Start-Sleep -Milliseconds 650}
 if((N $r).active.id-contains18){throw 'Native lean did not exit under movement/turn controls'}
 I $r;C $r 'setviewpos -35 -153 68.1 0 0';Start-Sleep -Milliseconds 500
}
function Snap($src,$dst,$trial){$s=[pscustomobject]@{source=$src;trial=$trial;native=(N $src);sourceBridge=(B $src);movement=(M $dst);stream=(Get-Content "$root/work/lab-instances/$dst/coop-status.json.shadow-stream.json" -Raw|ConvertFrom-Json)};$rows.Add($s);return $s}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong process'};$pids[$r]=$l.pid
  I $r;foreach($c in 'coop_review_clip off','coop_movement_bank off','coop_shadow_stream off','coop_mixamo_crouch off','coop_probe_body off','coop_probe_follow exact','coop_probe_locomotion on','coop_shadow_loco combat'){C $r $c};ExitLean $r
  if((B $r).localPlayerStance-eq1){I $r 0 0 B 180;Start-Sleep -Milliseconds 1000}
 }
 foreach($src in 'host','client'){
  $dst=if($src-eq'host'){'client'}else{'host'};C $dst 'setviewpos -30.5 -150.2 68.1 214 13'
  foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 600;C $dst 'coop_movement_bank on';Start-Sleep -Milliseconds 900
  if(!(M $dst).active-or(M $dst).version-ne16){throw 'Lean bank unavailable'}
  foreach($stance in 0,1){
   if((B $src).localPlayerStance-ne$stance){I $src 0 0 B 180;Start-Sleep -Milliseconds 1400}
   foreach($side in -1,1){
    ExitLean $src;I $src ($side*32767) 0 Y 650
    $watch=[Diagnostics.Stopwatch]::StartNew();$ok=$false
    while($watch.ElapsedMilliseconds-lt1700){
     $s=Snap $src $dst "stance-$stance-side-$side";$nativeLean=@($s.native.active|Where-Object id -eq 18)
     if($nativeLean.Count-and$s.stream.remoteState-eq5-and$s.stream.leanUp-le.0001-and$s.stream.leanSide*$side-gt.9-and$s.movement.leanAmount*$side-gt.85-and$s.movement.leanWeight-gt.9-and$s.movement.weight-gt.9-and[Math]::Abs($s.movement.stanceMix-$stance)-lt.01){$ok=$true}
     Start-Sleep -Milliseconds 25
    }
    if(!$ok){throw "Native lean was not presented $src stance=$stance side=$side"}
    $checks.Add("$src stance=$stance side=${side}: exact native lean amount selects full-body parameter curve")
    ExitLean $src;Start-Sleep -Milliseconds 700;$s=Snap $src $dst 'lean-recovery'
    if(!$s.movement.active-or$s.movement.leanWeight-gt.01-or$s.movement.weight-lt.9){throw 'Lean recovery failed'}
    $checks.Add("$src stance=$stance side=${side}: movement exits lean and restores standing/crouched gait")
   }
  }
  if((B $src).localPlayerStance-eq1){I $src 0 0 B 180;Start-Sleep -Milliseconds 1400}
  C $dst 'coop_movement_bank off';foreach($r in 'host','client'){C $r 'coop_shadow_stream off'}
 }
 foreach($r in 'host','client'){$h=& "$lab/bin/d2_runtime_probe.exe" $pids[$r] --health 1|ConvertFrom-Json;if($LASTEXITCODE-or$h.current-ne56){throw 'Player health changed'}}
 $checks.Add('Native player health unchanged on both roles')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';ExitLean $r;if((B $r).localPlayerStance-eq1){I $r 0 0 B 180}}catch{}}
 $rows|ConvertTo-Json -Depth 12|Set-Content "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;checks=@($checks);dll=(Get-FileHash "$lab/bin/dinput8.dll").Hash;scope='Horizontal standing/crouched native lean and locomotion recovery on both characters. Vertical peeking, collision-limited lean and arbitrary combat interruptions remain unverified; visual acceptance is separate.'}|ConvertTo-Json -Depth 6|Set-Content "$out/result.json"
}
[pscustomobject]@{Passed=(!$failure);Failure=$failure;Checks=$checks.Count};if($failure){throw $failure}
