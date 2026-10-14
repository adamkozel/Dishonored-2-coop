param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-keyhole-corvo-v1',[ValidateSet('host','client')][string]$Source='host',[switch]$CrouchedStart,[switch]$Reverse,[switch]$HolsteredStart)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$rows=[Collections.Generic.List[object]]::new();$pids=@{};$failure=$null
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($b='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $Source -Button $b -DurationMs $ms|Out-Null}
function Q($r,$mode,$id=0){for($i=0;$i-lt10;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw "Probe $r $mode"}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong process'};$pids[$r]=$l.pid
  foreach($c in 'coop_movement_bank off','coop_shadow_stream off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $c}
  if((Q $r '--health' 1).current-ne56){throw 'Baseline health changed'}
 }
 I;if((Q $Source '--player-states').stance-eq1){I B 100;Start-Sleep -Milliseconds 650}
 if($HolsteredStart-and(Q $Source '--player-sword').equipped){C $Source 'setviewpos -33.5 -153 68.1 0 0';I X 1500;Start-Sleep -Milliseconds 2500}
 if($Reverse){C $Source 'setviewpos -37.9 -153.55 68.1 0 0'}else{C $Source 'setviewpos -35.9 -153.55 68.1 180 0'};Start-Sleep -Milliseconds 500
 if($CrouchedStart){I B 100;Start-Sleep -Milliseconds 700}
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Source -Pitch 0 -Evidence "$out/source-camera.json"
 I X 1000;$watch=[Diagnostics.Stopwatch]::StartNew();$exitSent=$false
 while($watch.ElapsedMilliseconds-lt8000){
  if(!$exitSent-and$watch.ElapsedMilliseconds-ge4000-and@((Q $Source '--player-states').active|Where-Object id -eq 24).Count){I B 100;$exitSent=$true}
  $rows.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;native=(Q $Source '--player-states');midnight=(Q $Source '--midnight');sword=(Q $Source '--player-sword')})
  Start-Sleep -Milliseconds 25
 }
 if(!@($rows|Where-Object{@($_.native.active|Where-Object id -eq 24).Count}).Count){throw 'Native peeking not observed'}
 foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56){throw 'Health changed'}}
}catch{$failure=$_.Exception.Message}finally{
 try{I;if(@((Q $Source '--player-states').active|Where-Object id -eq 24).Count){I B 100;Start-Sleep -Milliseconds 1800};C $Source 'setviewpos -33.5 -153 68.1 0 0';if(!(Q $Source '--player-sword').equipped){I X 650;Start-Sleep -Milliseconds 2300}}catch{}
 foreach($r in 'host','client'){try{C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 20|Set-Content -Encoding utf8 "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;source=$Source;crouchedStart=[bool]$CrouchedStart;reverse=[bool]$Reverse;scope='Native door keyhole held X entry, B exit; body/arm/door scenes observed without custom playback.'}|ConvertTo-Json|Set-Content "$out/result.json"
}
if($failure){throw $failure};'KEYHOLE_OBSERVED'
