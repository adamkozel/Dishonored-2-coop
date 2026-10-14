param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-jump-timing-v1')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$rows=[Collections.Generic.List[object]]::new();$failure=$null;$pids=@{}
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$x=0,$y=0,$button='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -LeftX $x -LeftY $y -Button $button -DurationMs $ms|Out-Null}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong process'};$pids[$r]=$l.pid
  I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off'
 }
 foreach($r in 'host','client'){
  foreach($trial in 'stand-short','stand-held','crouch','running'){
   $s=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json;$stance=if($trial-eq'crouch'){1}else{0}
   if($s.localPlayerStance-ne$stance){I $r 0 0 B 180;Start-Sleep -Milliseconds 1100}
   C $r 'setviewpos -35 -153 68.1 0 0';Start-Sleep -Milliseconds 750
   $y=if($trial-eq'running'){20000}else{0};$ms=if($trial-eq'stand-held'){800}else{180}
   I $r 0 $y A $ms;$w=[Diagnostics.Stopwatch]::StartNew()
   while($w.ElapsedMilliseconds-lt2000){
    $raw=& "$root/work/build/Release/d2_runtime_probe.exe" $pids[$r] --player-states
    if(!$LASTEXITCODE){$rows.Add([pscustomobject]@{role=$r;trial=$trial;ms=$w.ElapsedMilliseconds;native=($raw|ConvertFrom-Json)})};Start-Sleep -Milliseconds 10
   }
  }
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;$s=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json;if($s.localPlayerStance-eq1){I $r 0 0 B 180};C $r 'setviewpos -35 -153 68.1 0 0'}catch{}}
 $rows|ConvertTo-Json -Depth 9|Set-Content "$out/states.json"
 [ordered]@{failure=$failure;scope='Read-only exact Jumping state fields under actual native short/held/crouched/running jump input on both roles. No native FP pose or physics writes.'}|ConvertTo-Json|Set-Content "$out/result.json"
}
if($failure){throw $failure};'Native jump timing observations saved'
