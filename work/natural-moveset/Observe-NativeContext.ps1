param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-context-observation-v1')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$rows=[Collections.Generic.List[object]]::new();$failure=$null;$pids=@{}
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$x=0,$y=0,$button='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -LeftX $x -LeftY $y -Button $button -DurationMs $ms|Out-Null}
function Capture($r,$trial,$ms){
 $w=[Diagnostics.Stopwatch]::StartNew();while($w.ElapsedMilliseconds-lt$ms){
  $raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$r] --player-states
  if(!$LASTEXITCODE){$rows.Add([pscustomobject]@{role=$r;trial=$trial;ms=$w.ElapsedMilliseconds;native=($raw|ConvertFrom-Json)})}
  Start-Sleep -Milliseconds 25
 }
}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong process'};$pids[$r]=$l.pid
  I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off'
 }
 foreach($r in 'host','client'){
  C $r 'setviewpos -35 -153 68.1 0 0';Start-Sleep -Milliseconds 800
  foreach($stance in 0,1){
   $s=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json
   if($s.localPlayerStance-ne$stance){I $r 0 0 B 180;Start-Sleep -Milliseconds 1000}
   foreach($trial in @(@('left',-32767,0),@('right',32767,0),@('up',0,32767))){
    I $r $trial[1] $trial[2] Y 900;Capture $r "lean-$stance-$($trial[0])" 1500;I $r;Start-Sleep -Milliseconds 600
   }
  }
  I $r 0 0 B 180;Start-Sleep -Milliseconds 1000;C $r 'setviewpos -35 -153 68.1 0 0';Start-Sleep -Milliseconds 650
  I $r 0 32767 LeftThumb 1000;Start-Sleep -Milliseconds 350
  I $r 0 32767 B 500;Capture $r 'sprint-to-slide' 2000;I $r
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;$s=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json;if($s.localPlayerStance-eq1){I $r 0 0 B 180};C $r 'setviewpos -35 -153 68.1 0 0'}catch{}}
 $rows|ConvertTo-Json -Depth 9|Set-Content "$out/states.json"
 [ordered]@{failure=$failure;scope='Native activation and read-only lean fields from exact-class Ghidra update, observed under real native controls; no new playback binding.'}|ConvertTo-Json|Set-Content "$out/result.json"
}
$rows|Group-Object role,trial|ForEach-Object {[pscustomobject]@{Trial=$_.Name;States=(@($_.Group.native.active|ForEach-Object {$_.id.ToString()+':'+$_.type}|Sort-Object -Unique)-join', ')}}|Format-Table -AutoSize
if($failure){throw $failure}
