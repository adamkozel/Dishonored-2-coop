param([ValidateSet('host','client')][string]$Source='host',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='turn-corvo-v18-v1',[switch]$Record,[switch]$NativeFeet,[switch]$CloseCamera,[switch]$AuthoredFeet,[switch]$Unarmed,[switch]$Crouched)
if($AuthoredFeet){$NativeFeet=$true}
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
  if(!(Probe $r '--player-sword').equipped){& "$lab/Send-D2Input.ps1" -Role $r -Button X -DurationMs 650|Out-Null;Start-Sleep -Milliseconds 2400;if(!(Probe $r '--player-sword').equipped){throw 'Initial native sword readiness failed'}}
 }
 if($CloseCamera){C $target 'setviewpos -30.5 -150.5 68.1 212 0'}else{C $target 'setviewpos -28.5 -151.2 68.1 195 0'};C $Source 'setviewpos -34.5 -153 68.1 0 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $target -Pitch $(if($CloseCamera){16}else{7}) -Evidence "$out/observer-camera.json"
 Copy-Item "$PSScriptRoot/crouch-turn-banks-v1/$character-crouch-turn-bank.bin" "$root/work/lab-instances/$target/coop-status.json.movement.bin"
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 550;C $target ('coop_movement_feet '+$(if($AuthoredFeet){'authored'}elseif($NativeFeet){'on'}else{'off'}));C $target 'coop_movement_bank on'
 $w=[Diagnostics.Stopwatch]::StartNew();do{$m=S $target '.movement.json';if($m.active-and$m.version-eq19-and$m.weight-gt.9){break};Start-Sleep -Milliseconds 40}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne19-or$m.weight-lt.9-or[bool]$m.nativeFeet-ne[bool]$NativeFeet){throw 'Movement bank not ready'};$checks.Add('Native Steam stream and v19 full-stance turn bank ready')
 if($Crouched){I $Source 0 0 180 0 B;Start-Sleep -Milliseconds 1400;if((S $target '.movement.json').stanceMix-lt.99){throw 'Native crouch did not reach full-body stance'};$checks.Add('Replicated native crouch stance ready')}
 if($Unarmed){
  & "$lab/Send-D2Input.ps1" -Role $Source -Button X -DurationMs 1500|Out-Null;Start-Sleep -Milliseconds 3200
  $m=S $target '.movement.json'
  if($m.armedMix-gt.01-or(Probe $Source '--player-sword').equipped-or(Probe $target '--avatar-sword' $handles[$target]).equipped){throw 'Native swords did not holster'}
  $checks.Add('Native hands-free presentation ready')
 }
 if($Record){
  $window=(Get-Process -Id $pids[$target]).MainWindowHandle.ToInt64();if(!$window){throw 'No observer game window'}
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','30','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
  $capture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru;Start-Sleep -Milliseconds 700;if($capture.HasExited){throw 'Recorder failed'}
 }
 $overall=[Diagnostics.Stopwatch]::StartNew();$chapters=[Collections.Generic.List[object]]::new()
 foreach($trial in @(@('left-slow',-22000),@('right-slow',22000),@('left-medium',-28000),@('right-medium',28000),@('left-fast',-32767),@('right-fast',32767))){
  I $Source;C $Source 'setviewpos -34.5 -153 68.1 0 0';Start-Sleep -Milliseconds 650
  $start=(S $Source).localPosition;$chapters.Add([pscustomobject]@{time=(.7+$overall.Elapsed.TotalSeconds);label=$trial[0]})
  & "$lab/Send-D2Input.ps1" -Role $Source -RightX $trial[1] -DurationMs 1800|Out-Null
  $w.Restart();$part=[Collections.Generic.List[object]]::new();$poseSaved=$false
  while($w.ElapsedMilliseconds-lt3100){
   $ms=$w.ElapsedMilliseconds;if($ms-gt1900){I $Source}
   $m=S $target '.movement.json';$stream=S $target '.shadow-stream.json'
   if(!$m.active-or!$stream.armed-or!$stream.ready){throw 'Turn stream retired'}
   $row=[pscustomobject]@{trial=$trial[0];ms=$ms;videoTime=(.7+$overall.Elapsed.TotalSeconds);movement=$m;stream=$stream;source=(S $Source);target=(S $target)};$rows.Add($row);$part.Add($row)
   if(!$poseSaved-and$ms-ge1000){
    for($a=0;$a-lt8;$a++){$raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$target] --skeleton-final $handles[$target] $row.target.bodyPublicationAddress;if(!$LASTEXITCODE){$pose=$raw|ConvertFrom-Json;if($pose.stable-and$pose.finalPublicationComplete){$raw|Set-Content "$out/pose-$($trial[0]).json";$poseSaved=$true;break}}}
   }
   Start-Sleep -Milliseconds 30
  }
  $expected=if($trial[1]-gt0){1}else{0};$active=@($part|Where-Object {$_.movement.turnWeight-gt.8-and(($_.movement.turnClip-149)%2)-eq$expected})
  if($active.Count-lt8){throw "Native directional turn missing: $($trial[0]) $($active.Count)"}
  if($m.turnWeight-gt.04){throw 'Turn failed to recover standing'}
  $end=(S $Source).localPosition;$distance=[Math]::Sqrt([Math]::Pow($end[0]-$start[0],2)+[Math]::Pow($end[1]-$start[1],2))
  if($distance-gt.05){throw "In-place turning moved the source root: $distance"}
  if(!$poseSaved){throw 'Stable turn pose missing'}
  $checks.Add("$($trial[0]): native facing drives remote footwork; no translation; standing recovery; complete pose")
 }
 foreach($r in 'host','client'){if((Probe $r '--health' 1).current-ne56-or(Probe $r '--health' $handles[$r]).current-ne40){throw 'Native health changed'}}
 if((Probe $Source '--player-sword').equipped-eq[bool]$Unarmed-or(Probe $target '--avatar-sword' $handles[$target]).equipped-eq[bool]$Unarmed){throw 'Native sword state changed'};$checks.Add('Native health and equipped/holstered sword states preserved')
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('Original files and protected saves unchanged')
 if($capture){$capture.WaitForExit(10000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode-ne0){throw 'Capture incomplete'}}
}catch{$failure=$_.Exception.Message}finally{
 if($Unarmed){try{if(!(Probe $Source '--player-sword').equipped){& "$lab/Send-D2Input.ps1" -Role $Source -Button X -DurationMs 650|Out-Null;Start-Sleep -Milliseconds 2400}}catch{}}
 foreach($r in 'host','client'){try{I $r;if((S $r).localPlayerStance-eq1){I $r 0 0 180 0 B;Start-Sleep -Milliseconds 1000};C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'setviewpos -33.5 -153 68.1 0 0';C $r 'coop_movement_fixture off'}catch{}}
 if($capture-and!$capture.HasExited){$capture.WaitForExit(20000)|Out-Null}
 $rows|ConvertTo-Json -Depth 14|Set-Content "$out/states.json"
 [ordered]@{source=$Source;character=$character;failure=$failure;checks=$checks;chapters=$chapters;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;bank=(Get-FileHash "$PSScriptRoot/crouch-turn-banks-v1/$character-crouch-turn-bank.bin").Hash;nativeFeet=[bool]$AuthoredFeet;unarmed=[bool]$Unarmed;crouched=[bool]$Crouched;recorded=[bool]$Record;scope='Native controller look only, zero movement input: left/right turns at three look speeds. Root yaw selects angular-phase whole-body turn donors. Armed/holstered and standing/crouched cases selected explicitly per take. Continuous-turn action interruptions remain separate.'}|ConvertTo-Json -Depth 6|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_TURN_BANK $character $($checks.Count) checks"
