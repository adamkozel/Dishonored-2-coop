param([ValidateSet('host','client')][string]$Source='host',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='corvo-video-1',[switch]$AwaitRecording)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$target=if($Source-eq'host'){'client'}else{'host'}
function B($r){Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json}
function C($r,$s){& "$lab/Send-D2Command.ps1" -Role $r -Command $s -WaitConsumed|Out-Null}
function I($x=0,$y=0,$ms=100){& "$lab/Send-D2Input.ps1" -Role $Source -LeftX $x -LeftY $y -DurationMs $ms|Out-Null}
$events=@(@(0,0,0,100,'ready'),@(1000,0,14000,2200,'forward'),@(3300,0,0,100,'stop'),@(3900,-14000,0,1700,'left'),@(5750,0,0,100,'stop'),@(6200,0,-14000,1700,'back'),@(8050,0,0,100,'stop'),@(8500,14000,0,1700,'right'),@(10300,0,0,100,'recovery'))
$rows=[Collections.Generic.List[object]]::new();$failure=$null;$capture=$null
try{
 foreach($r in 'host','client'){C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_probe_follow exact';C $r 'coop_probe_locomotion on';C $r 'coop_shadow_loco combat'}
 C $Source 'setviewpos -34.3 -154.8 68.1 0 0';C $target 'setviewpos -29 -151.5 68.1 214 11';Start-Sleep -Milliseconds 600
 if($AwaitRecording){$deadline=[DateTime]::UtcNow.AddSeconds(45);while(!(Test-Path "$out/record.ready")){if([DateTime]::UtcNow-gt$deadline){throw 'Recorder not ready'};Start-Sleep -Milliseconds 50}}
 foreach($r in 'host','client'){C $r 'coop_shadow_stream 15000'};Start-Sleep -Milliseconds 400
 C $target 'coop_movement_bank 60000'
 $launch=Get-Content "$root/work/lab-instances/$target/launch.json" -Raw|ConvertFrom-Json;$s=B $target
 $capture=Start-Process -FilePath "$root/work/fullbody-proof/build/Release/d2_capture_pose.exe" -ArgumentList @($launch.pid,$s.probeActorHandle,12,('"'+"$out/poses.json"+'"'),'actor',$s.bodyPublicationAddress) -WindowStyle Hidden -PassThru -RedirectStandardOutput "$out/capture.log" -RedirectStandardError "$out/capture.err"
 $watch=[Diagnostics.Stopwatch]::StartNew();$next=0
 while($watch.ElapsedMilliseconds-lt12000){
  if($next-lt$events.Count-and$watch.ElapsedMilliseconds-ge$events[$next][0]){$e=$events[$next];I $e[1] $e[2] $e[3];$next++}
  $rows.Add([pscustomobject]@{time=$watch.ElapsedMilliseconds;event=$events[[Math]::Max(0,$next-1)][4];source=(B $Source);target=(B $target);movement=(Get-Content "$root/work/lab-instances/$target/coop-status.json.movement.json" -Raw|ConvertFrom-Json)})
  Start-Sleep -Milliseconds 35
 }
}catch{$failure=$_.Exception.Message}finally{
 I;foreach($r in 'host','client'){try{C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off'}catch{}}
 $rows|ConvertTo-Json -Depth 8|Set-Content "$out/states.json"
 [ordered]@{source=$Source;target=$target;failure=$failure;dll=(Get-FileHash "$lab/bin/dinput8.dll").Hash;bank=(Get-FileHash "$root/work/lab-instances/$target/coop-status.json.movement.bin").Hash;events=$events;firstPersonChanged=$false;root='Native controller and native Steam root stream'}|ConvertTo-Json -Depth 5|Set-Content "$out/manifest.json"
}
if($capture-and(!$capture.WaitForExit(15000)-or$capture.ExitCode)){throw 'Pose capture failed'}
if($failure){throw $failure};'WALK_REPLAY_COMPLETE'
