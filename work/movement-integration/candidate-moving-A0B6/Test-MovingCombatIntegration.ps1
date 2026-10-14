param([ValidateSet('host','client')][string]$Source='host',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='integrated-moving-combat-corvo-v1',[switch]$Record)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$target=if($Source-eq'host'){'client'}else{'host'};$character=if($Source-eq'host'){'corvo'}else{'emily'};$pids=@{};$handles=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$capture=$null
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$x=0,$y=0,$ms=100,$trigger=0,$button='Neutral'){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms -LeftX $x -LeftY $y -RightTrigger $trigger|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt8;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function Probe($r,$mode,$id=0){for($i=0;$i-lt12;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 10};throw "Native read $r $mode"}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'}
  $pids[$r]=$l.pid;$handles[$r]=(S $r).probeActorHandle;I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_power_fixture off'
  C $r 'coop_probe_follow exact';C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000'
  if((Probe $r '--health' 1).current-ne56){throw 'Clean living fixture required'}
  if((S $r).localPlayerStance-eq1){I $r 0 0 180 0 B;Start-Sleep -Milliseconds 900}
 }
 C $target 'setviewpos -28.5 -151.2 68.1 195 0';C $Source 'setviewpos -34.5 -153 68.1 0 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $target -Pitch 7 -Evidence "$out/observer-camera.json"
 Copy-Item "$PSScriptRoot/air-combat-banks-v1/$character-air-combat-bank.bin" "$root/work/lab-instances/$target/coop-status.json.movement.bin"
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 550;C $target 'coop_movement_bank on'
 $w=[Diagnostics.Stopwatch]::StartNew();do{$m=S $target '.movement.json';if($m.active-and$m.version-eq16-and$m.weight-gt.9){break};Start-Sleep -Milliseconds 40}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne16-or$m.weight-lt.9){throw 'Movement bank not ready'};$checks.Add('Native Steam stream and v16 bank ready')
 if($Record){
  $window=(Get-Process -Id $pids[$target]).MainWindowHandle.ToInt64();if(!$window){throw 'No observer game window'}
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','24','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
  $capture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru;Start-Sleep -Milliseconds 700;if($capture.HasExited){throw 'Recorder failed'}
 }
 $overall=[Diagnostics.Stopwatch]::StartNew();$chapters=[Collections.Generic.List[object]]::new()
 $trials=@(@('forward-strike',0,18000,0,0),@('side-strike',18000,0,0,0),@('backward-strike',0,-18000,0,0),@('forward-guard',0,18000,0,1),@('crouch-strike',0,18000,1,0))
 foreach($trial in $trials){
  I $Source;C $Source 'setviewpos -34.5 -153 68.1 0 0'
  if((S $Source).localPlayerStance-ne$trial[3]){I $Source 0 0 180 0 B;Start-Sleep -Milliseconds 1000}
  Start-Sleep -Milliseconds 650;$chapters.Add([pscustomobject]@{time=(.7+$overall.Elapsed.TotalSeconds);label=$trial[0]})
  $w.Restart();$stage=0;$part=[Collections.Generic.List[object]]::new()
  while($w.ElapsedMilliseconds-lt3000){
   $ms=$w.ElapsedMilliseconds
   if($stage-eq0){I $Source $trial[1] $trial[2] 1800;$stage=1}
   if($stage-eq1-and$ms-ge350){if($trial[4]){I $Source $trial[1] $trial[2] 1050 0 RightShoulder}else{I $Source $trial[1] $trial[2] 180 255};$stage=2}
   if(!$trial[4]-and$stage-eq2-and$ms-ge600){I $Source $trial[1] $trial[2] 800;$stage=3}
   if($stage-lt4-and$ms-ge1450){I $Source;$stage=4}
   $native=Probe $Source '--player-states';$m=S $target '.movement.json';$stream=S $target '.shadow-stream.json'
   if(!$m.active-or!$stream.armed-or!$stream.ready){throw "Moving combat retired: $($stream.result)"}
   $row=[pscustomobject]@{trial=$trial[0];ms=$ms;videoTime=(.7+$overall.Elapsed.TotalSeconds);native=$native;movement=$m;stream=$stream;source=(S $Source);target=(S $target)};$rows.Add($row);$part.Add($row)
   Start-Sleep -Milliseconds 15
  }
  $active=@($part|Where-Object {$_.movement.combatWeight-gt.7-and$_.movement.combatMoveMix-gt.8-and$_.movement.speed-gt.5-and$_.movement.weight-gt.85})
  $nativeAction=@($part|Where-Object {$_.native.combatSceneValid-and$_.native.motion-in0,1})
  if($active.Count-lt2-or!$nativeAction.Count){throw "No native travelling sword action: $($trial[0])"}
  $phaseA=[double]$active[0].movement.phase;$phaseB=[double]$active[-1].movement.phase;$phase=($phaseB-$phaseA+1)%1
  if($phase-lt.025){throw "Travelling feet froze during $($trial[0])"}
  if($m.combatWeight-gt.02-or$m.combatMoveMix-gt.02-or$m.weight-lt.9){throw 'Stop/recovery failed'}
  $checks.Add("$character $($trial[0]): native combat, travelled gait phase and complete bank coexist; stop/recovery succeeds")
 }
 foreach($r in 'host','client'){if((Probe $r '--health' 1).current-ne56-or(Probe $r '--health' $handles[$r]).current-ne40){throw 'Health changed'}}
 $observer=Probe $target '--player-states';if($observer.active.id|Where-Object {$_-in39,44}){throw 'Observer first-person action changed'}
 if(!(Probe $Source '--player-sword').equipped-or!(Probe $target '--avatar-sword' $handles[$target]).equipped){throw 'Native sword attachment lost'}
 $checks.Add('Native health, observer first-person controls and both sword attachments preserved')
 foreach($f in (Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('Original game/Steam files and protected saves unchanged')
 if($capture){$capture.WaitForExit(9000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode-ne0){throw 'Recorder did not finalize'}}
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';if((S $r).localPlayerStance-eq1){I $r 0 0 180 0 B;Start-Sleep -Milliseconds 900};C $r 'setviewpos -33.5 -153 68.1 0 0';C $r 'coop_movement_fixture off'}catch{}}
 if($capture-and!$capture.HasExited){$capture.WaitForExit(26000)|Out-Null}
 $rows|ConvertTo-Json -Depth 14|Set-Content "$out/states.json"
 [ordered]@{source=$Source;character=$character;failure=$failure;checks=$checks;chapters=$chapters;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;bank=(Get-FileHash "$PSScriptRoot/air-combat-banks-v1/$character-air-combat-bank.bin").Hash;recorded=[bool]$Record;scope='Native source walking/sneaking and sword-scene clocks over Steam. Grounded gait pelvis/legs retain travelled phase; authored torso/arms retain sword action. No FP anchors or remote gameplay events. Visual acceptance, contact and full movement remain separate.'}|ConvertTo-Json -Depth 6|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_MOVING_COMBAT $character $($checks.Count) checks"
