param([ValidateSet('host','client')][string]$Source='host',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='gait-corvo-video-1',[switch]$AwaitRecording)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$target=if($Source-eq'host'){'client'}else{'host'}
function B($r){Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json}
function C($r,$s){& "$lab/Send-D2Command.ps1" -Role $r -Command $s -WaitConsumed|Out-Null}
function I($x=0,$y=0,$ms=100){& "$lab/Send-D2Input.ps1" -Role $Source -LeftX $x -LeftY $y -DurationMs $ms|Out-Null}
$events=@(@(0,0,0,100,'idle'),@(1000,0,14000,1100,'walk forward'),@(2100,0,32767,600,'accelerate to run'),@(2800,0,0,100,'stop'),@(3400,-22000,0,800,'left'),@(4400,0,-32767,850,'backward run'),@(5350,0,0,100,'stop'),@(6000,22000,0,800,'right'),@(7000,-14000,14000,800,'diagonal'),@(8100,0,0,100,'stop'),@(8600,0,-14000,1000,'backward walk'),@(9800,0,0,100,'standing recovery'))
$rows=[Collections.Generic.List[object]]::new();$failure=$null;$capture=$null;$start=0
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne("$root/work/asset-game/Dishonored2.exe".Replace('/','\'))){throw 'Wrong isolated process'}
  C $r 'coop_review_clip off';C $r 'coop_movement_bank off';C $r 'coop_authoring_proof off';C $r 'coop_shadow_stream off';C $r 'coop_probe_follow exact';C $r 'coop_probe_locomotion on';C $r 'coop_shadow_loco combat'
 }
 I;C $Source 'setviewpos -34.3 -154.8 68.1 0 0';C $target 'setviewpos -29 -151.5 68.1 214 11';Start-Sleep -Milliseconds 600
 if($AwaitRecording){$deadline=[DateTime]::UtcNow.AddSeconds(45);while(!(Test-Path "$out/record.ready")){if([DateTime]::UtcNow-gt$deadline){throw 'Recorder not ready'};Start-Sleep -Milliseconds 50}}
 foreach($r in 'host','client'){C $r 'coop_shadow_stream 15000'};Start-Sleep -Milliseconds 400;C $target 'coop_movement_bank 60000'
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();$watch=[Diagnostics.Stopwatch]::StartNew();$next=0
 while($watch.ElapsedMilliseconds-lt12000){
  if($next-lt$events.Count-and$watch.ElapsedMilliseconds-ge$events[$next][0]){$e=$events[$next];I $e[1] $e[2] $e[3];$next++}
  $rows.Add([pscustomobject]@{time=$watch.ElapsedMilliseconds;event=$events[[Math]::Max(0,$next-1)][4];source=(B $Source);target=(B $target);movement=(Get-Content "$root/work/lab-instances/$target/coop-status.json.movement.json" -Raw|ConvertFrom-Json)})
  Start-Sleep -Milliseconds 35
 }
 if(($rows.movement.runMix|Measure-Object -Maximum).Maximum-lt.5){throw 'Recording did not exercise running'}
}catch{$failure=$_.Exception.Message}finally{
 I;foreach($r in 'host','client'){try{C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off'}catch{}}
 $rows|ConvertTo-Json -Depth 8|Set-Content "$out/states.json"
 [ordered]@{character=$(if($Source-eq'host'){'corvo'}else{'emily'});source=$Source;observer=$target;start=$start;end=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();failure=$failure;dll=(Get-FileHash "$lab/bin/dinput8.dll").Hash;bank=(Get-FileHash "$root/work/lab-instances/$target/coop-status.json.movement.bin").Hash;events=$events;firstPersonChanged=$false;scope='Actual player inputs, native Steam root stream, authored native walk/run and directional blending. Phase advances with travelled distance.'}|ConvertTo-Json -Depth 8|Set-Content "$out/take.json"
 'done'|Set-Content "$out/record.done"
}
if($failure){throw $failure};'GAIT_RECORDING_COMPLETE'
