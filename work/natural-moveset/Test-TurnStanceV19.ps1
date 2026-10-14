param([ValidateSet('host','client')][string]$Source='host',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='turn-corvo-v18-v1',[switch]$Record,[switch]$NativeFeet,[switch]$CloseCamera,[switch]$AuthoredFeet,[switch]$Unarmed )
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
 if($Unarmed){
  & "$lab/Send-D2Input.ps1" -Role $Source -Button X -DurationMs 1500|Out-Null;Start-Sleep -Milliseconds 3200
  $m=S $target '.movement.json'
  if($m.armedMix-gt.01-or(Probe $Source '--player-sword').equipped-or(Probe $target '--avatar-sword' $handles[$target]).equipped){throw 'Native swords did not holster'}
  $checks.Add('Native hands-free presentation ready')
 }
 if($Record){
  $window=(Get-Process -Id $pids[$target]).MainWindowHandle.ToInt64();if(!$window){throw 'No observer game window'}
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','24','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
  $capture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru;Start-Sleep -Milliseconds 700;if($capture.HasExited){throw 'Recorder failed'}
 }
 $overall=[Diagnostics.Stopwatch]::StartNew();$chapters=[Collections.Generic.List[object]]::new()
 foreach($trial in @(@('left-stance-turn',-22000),@('right-stance-turn',22000))){
  I $Source;C $Source 'setviewpos -34.5 -153 68.1 0 0';Start-Sleep -Milliseconds 700
  $start=(S $Source).localPosition;$chapters.Add([pscustomobject]@{time=(.7+$overall.Elapsed.TotalSeconds);label=$trial[0]+' stand / crouch / stand'})
  $w.Restart();$part=[Collections.Generic.List[object]]::new();$stage=0;$poses=@{}
  while($w.ElapsedMilliseconds-lt7200){
   $ms=$w.ElapsedMilliseconds
   if($stage-eq0){& "$lab/Send-D2Input.ps1" -Role $Source -RightX $trial[1] -DurationMs 6000|Out-Null;$stage=1}
   if($stage-eq1-and$ms-ge1000){& "$lab/Send-D2Input.ps1" -Role $Source -RightX $trial[1] -Button B -DurationMs 180|Out-Null;$stage=2}
   if($stage-eq2-and$ms-ge1250){& "$lab/Send-D2Input.ps1" -Role $Source -RightX $trial[1] -DurationMs 4200|Out-Null;$stage=3}
   if($stage-eq3-and$ms-ge3600){& "$lab/Send-D2Input.ps1" -Role $Source -RightX $trial[1] -Button B -DurationMs 180|Out-Null;$stage=4}
   if($stage-eq4-and$ms-ge3850){& "$lab/Send-D2Input.ps1" -Role $Source -RightX $trial[1] -DurationMs 1700|Out-Null;$stage=5}
   if($stage-eq5-and$ms-ge5600){I $Source;$stage=6}
   $m=S $target '.movement.json';$stream=S $target '.shadow-stream.json'
   if(!$m.active-or!$stream.armed-or!$stream.ready){throw 'Turn stance stream retired'}
   $row=[pscustomobject]@{trial=$trial[0];ms=$ms;videoTime=(.7+$overall.Elapsed.TotalSeconds);movement=$m;stream=$stream;source=(S $Source);target=(S $target)};$rows.Add($row);$part.Add($row)
   $key=if($ms-ge750-and$ms-lt1000){'standing'}elseif($ms-ge2400-and$ms-lt3300){'crouched'}elseif($ms-ge4850-and$ms-lt5400){'recovered'}else{$null}
   if($key-and!$poses.ContainsKey($key)){
    for($a=0;$a-lt8;$a++){$raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$target] --skeleton-final $handles[$target] $row.target.bodyPublicationAddress;if(!$LASTEXITCODE){$pose=$raw|ConvertFrom-Json;if($pose.stable-and$pose.finalPublicationComplete){$raw|Set-Content "$out/pose-$($trial[0])-$key.json";$poses[$key]=$true;break}}}
   }
   Start-Sleep -Milliseconds 25
  }
  $low=@($part|Where-Object {$_.ms-ge2000-and$_.ms-lt3300-and$_.movement.stanceMix-gt.99-and$_.movement.turnWeight-gt.8})
  $high=@($part|Where-Object {$_.ms-ge4850-and$_.ms-lt5400-and$_.movement.stanceMix-lt.01-and$_.movement.turnWeight-gt.8})
  if($low.Count-lt8-or$high.Count-lt5-or$poses.Count-ne3){throw "Incomplete turn stance transition low=$($low.Count) high=$($high.Count) poses=$($poses.Count)"}
  if($m.turnWeight-gt.01-or$m.stanceMix-gt.01){throw 'Turn stance final recovery failed'}
  $end=(S $Source).localPosition;$distance=[Math]::Sqrt([Math]::Pow($end[0]-$start[0],2)+[Math]::Pow($end[1]-$start[1],2));if($distance-gt.05){throw 'Stance turn moved root'}
  $checks.Add("$($trial[0]): complete standing / crouched / standing turn and stopped recovery; three stable native pose samples")
 }
 foreach($r in 'host','client'){if((Probe $r '--health' 1).current-ne56-or(Probe $r '--health' $handles[$r]).current-ne40){throw 'Native health changed'}}
 if((Probe $Source '--player-sword').equipped-eq[bool]$Unarmed-or(Probe $target '--avatar-sword' $handles[$target]).equipped-eq[bool]$Unarmed){throw 'Native sword state changed'};$checks.Add('Native health and equipped/holstered sword states preserved through crouch recovery')
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('Original files and protected saves unchanged')
 if($capture){$capture.WaitForExit(10000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode-ne0){throw 'Capture incomplete'}}
}catch{$failure=$_.Exception.Message}finally{
 if($Unarmed){try{if(!(Probe $Source '--player-sword').equipped){& "$lab/Send-D2Input.ps1" -Role $Source -Button X -DurationMs 650|Out-Null;Start-Sleep -Milliseconds 2400}}catch{}}
 foreach($r in 'host','client'){try{I $r;if((S $r).localPlayerStance-eq1){I $r 0 0 180 0 B;Start-Sleep -Milliseconds 1000};C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'setviewpos -33.5 -153 68.1 0 0';C $r 'coop_movement_fixture off'}catch{}}
 if($capture-and!$capture.HasExited){$capture.WaitForExit(20000)|Out-Null}
 $rows|ConvertTo-Json -Depth 14|Set-Content "$out/states.json"
 [ordered]@{source=$Source;character=$character;failure=$failure;checks=$checks;chapters=$chapters;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;bank=(Get-FileHash "$PSScriptRoot/crouch-turn-banks-v1/$character-crouch-turn-bank.bin").Hash;nativeFeet=[bool]$AuthoredFeet;unarmed=[bool]$Unarmed;recorded=[bool]$Record;scope='Native stance changes while continuously turning, both directions. Complete-body stance and turn poses follow native source input, followed by standing recovery. Native actor root stays in place.'}|ConvertTo-Json -Depth 6|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_TURN_STANCE $character $($checks.Count) checks"
