param([ValidateSet('host','client')][string]$Source='host',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='integrated-air-combat-corvo-v1',[switch]$Record,[switch]$AuthoredFeet)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$target=if($Source-eq'host'){'client'}else{'host'};$character=if($Source-eq'host'){'corvo'}else{'emily'};$pids=@{};$handles=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$capture=$null
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$button='Neutral',$ms=100,$trigger=0){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms -RightTrigger $trigger|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt8;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function Probe($r,$mode,$id=0){for($i=0;$i-lt12;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 10};throw "Native read $r $mode"}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'}
  $pids[$r]=$l.pid;$handles[$r]=(S $r).probeActorHandle;I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_power_fixture off'
  C $r 'coop_probe_follow exact';C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000'
  if((Probe $r '--health' 1).current-ne56){throw 'Clean living fixture required'}
  if((S $r).localPlayerStance-eq1){I $r B 180;Start-Sleep -Milliseconds 900}
 }
 C $target 'setviewpos -28.5 -151.2 68.1 200 0';C $Source 'setviewpos -33.5 -153 68.1 0 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $target -Pitch 4 -Evidence "$out/observer-camera.json"
 Copy-Item "$PSScriptRoot/air-combat-banks-v1/$character-air-combat-bank.bin" "$root/work/lab-instances/$target/coop-status.json.movement.bin"
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 550;C $target ('coop_movement_feet '+$(if($AuthoredFeet){'authored'}else{'off'}));C $target 'coop_movement_bank on'
 $w=[Diagnostics.Stopwatch]::StartNew();do{$m=S $target '.movement.json';if($m.active-and$m.version-eq16-and$m.weight-gt.9){break};Start-Sleep -Milliseconds 40}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne16-or$m.weight-lt.9){throw 'Air combat bank not ready'};$checks.Add('Native Steam stream and v16 bank ready')
 if($Record){
  $window=(Get-Process -Id $pids[$target]).MainWindowHandle.ToInt64();if(!$window){throw 'No observer game window'}
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','14','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
  $capture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru;Start-Sleep -Milliseconds 700;if($capture.HasExited){throw 'Recorder failed'}
 }
 $events=@(@(1000,'A',180,0),@(1150,'Neutral',180,255),@(5000,'A',180,0),@(5100,'RightShoulder',1000,0),@(9000,'A',180,0),@(9420,'Neutral',180,255));$next=0;$w.Restart()
 while($w.ElapsedMilliseconds-lt12000){
  $ms=$w.ElapsedMilliseconds
  if($next-lt$events.Count-and$ms-ge$events[$next][0]){$e=$events[$next];I $Source $e[1] $e[2] $e[3];$next++}
  $native=Probe $Source '--player-states';$m=S $target '.movement.json';$stream=S $target '.shadow-stream.json'
  if(!$m.active-or!$stream.armed-or!$stream.ready){throw "Air combat retired: $($stream.result)"}
  $rows.Add([pscustomobject]@{ms=$ms;native=$native;movement=$m;stream=$stream;source=(S $Source);target=(S $target)})
  Start-Sleep -Milliseconds 15
 }
 foreach($trial in @(@('early strike',1000,3000,1),@('air guard',5000,7000,2),@('late strike',9000,11000,1))){
  $part=@($rows|Where-Object {$_.ms-ge$trial[1]-and$_.ms-lt$trial[2]})
  $nativeAir=@($part|Where-Object {$_.native.motion-in2,3-and$_.native.combatSceneValid-and$_.native.combatScene.action-eq$trial[3]})
  $remoteAir=@($part|Where-Object {$_.movement.airState-in1,2-and$_.movement.airWeight-gt.7-and$_.movement.combatWeight-gt.5-and$_.movement.weight-gt.85})
  if(!$nativeAir.Count-or!$remoteAir.Count){throw "Missing native/remote simultaneous air combat: $($trial[0])"}
  $checks.Add("$character $($trial[0]): native airborne combat scene and remote flight/strike coexist")
 }
 if($m.airWeight-gt.01-or$m.combatWeight-gt.01-or$m.weight-lt.9){throw 'Air combat failed to recover'};$checks.Add('Landing and native combat end restore standing locomotion')
 foreach($r in 'host','client'){if((Probe $r '--health' 1).current-ne56-or(Probe $r '--health' $handles[$r]).current-ne40){throw 'Health changed'}}
 $observer=Probe $target '--player-states';if($observer.active.id|Where-Object {$_-in39,44}){throw 'Observer first-person action changed'}
 if(!(Probe $Source '--player-sword').equipped-or!(Probe $target '--avatar-sword' $handles[$target]).equipped){throw 'Native sword attachment lost'}
 $checks.Add('Native health, observer first-person controls and both sword attachments preserved')
 foreach($f in (Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('Original game/Steam files and protected saves unchanged')
 if($capture){$capture.WaitForExit(7000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode-ne0){throw 'Recorder did not finalize'}}
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'setviewpos -33.5 -153 68.1 0 0';C $r 'coop_movement_fixture off'}catch{}}
 if($capture-and!$capture.HasExited){$capture.WaitForExit(16000)|Out-Null}
 $rows|ConvertTo-Json -Depth 14|Set-Content "$out/states.json"
 [ordered]@{source=$Source;character=$character;failure=$failure;checks=$checks;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;bank=(Get-FileHash "$PSScriptRoot/air-combat-banks-v1/$character-air-combat-bank.bin").Hash;recorded=[bool]$Record;authoredFeet=[bool]$AuthoredFeet;scope='Native source jump/fall and owned sword scene clocks over Steam. Presentation-only body; no remote physics, native attack events or damage. Visual acceptance and complete moveset remain separate.'}|ConvertTo-Json -Depth 6|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_AIR_COMBAT $character $($checks.Count) checks"
