[CmdletBinding()]
param([ValidateSet('host','client')][string]$Role='host',[Parameter(Mandatory)][string]$Take)
$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab'
$peer=if($Role-eq'host'){'client'}else{'host'};$character=if($Role-eq'host'){'corvo'}else{'emily'}
$out=Join-Path $PSScriptRoot $Take;if(Test-Path -LiteralPath $out){throw 'Take already exists'}
New-Item -ItemType Directory -Path $out|Out-Null
$launch=@{};foreach($r in 'host','client'){$launch[$r]=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
 if((Get-Process -Id $launch[$r].pid).Path-ne"$root/work/asset-game/Dishonored2.exe".Replace('/','\')){throw 'Wrong process'} }
function C($r,$s){& "$lab/Send-D2Command.ps1" -Role $r -Command $s -WaitConsumed|Out-Null}
function Q($r,$flag,$value=''){if($value){$v=& "$lab/bin/d2_runtime_probe.exe" $launch[$r].pid $flag $value}else{$v=& "$lab/bin/d2_runtime_probe.exe" $launch[$r].pid $flag};if($LASTEXITCODE){return $null};$v|ConvertFrom-Json}
function J($r,$suffix=''){try{Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json}catch{$null}}
function I([string]$b='Neutral',[int]$y=0,[int]$ms=150){& "$lab/Send-D2Input.ps1" -Role $Role -Button $b -LeftY $y -DurationMs $ms|Out-Null}
$capture=$null;$failure=$null;$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new()
try{
 foreach($r in 'host','client'){foreach($c in 'coop_movement_resume off','coop_movement_bank off','coop_shadow_stream off','coop_movement_fixture off','coop_world_observe on','coop_probe_follow exact','coop_probe_locomotion on','coop_movement_feet authored'){C $r $c}}
 Copy-Item -LiteralPath "$PSScriptRoot/carry-banks-v1/$character-carry-bank.bin" -Destination "$root/work/lab-instances/$peer/coop-status.json.movement.bin" -Force
 C $Role 'setviewpos -38.95 -161.5 68.1 270 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Role -Pitch 50 -Evidence "$out/pickup-camera.json"|Out-Null
 $healthBefore=Q $Role '--health' '1';I X 0 900;Start-Sleep -Milliseconds 1900
 $pickup=Q $Role '--player-states';$pickup|ConvertTo-Json -Depth 12|Set-Content "$out/pickup.json"
 if(!$pickup.carrySceneValid-or$pickup.carryScene.variant-ne2){throw 'Native pickup did not settle into owned carry loop'}
 $carryState=@($pickup.active|Where-Object id -eq 45)[0]
 if($carryState.pointer){(Q $Role '--object' $carryState.pointer)|ConvertTo-Json -Depth 8|Set-Content "$out/native-carry-state-object.json"}
 C $Role 'setviewpos -33 -153 68.1 90 0';C $peer 'setviewpos -29 -151.5 68.1 180 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Role -Pitch 0 -Evidence "$out/source-camera.json"|Out-Null
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $peer -Pitch 12 -Evidence "$out/observer-camera.json"|Out-Null
 foreach($r in 'host','client'){C $r 'coop_movement_fixture 60000';C $r 'coop_shadow_stream on'}
 Start-Sleep -Milliseconds 500;C $peer 'coop_movement_bank on';Start-Sleep -Milliseconds 1400
 $bank=J $peer '.movement.json';$stream=J $peer '.shadow-stream.json'
 $bank|ConvertTo-Json -Depth 8|Set-Content "$out/bank-before.json";$stream|ConvertTo-Json -Depth 8|Set-Content "$out/stream-before.json"
 if(!$bank.active-or$bank.version-ne31-or$bank.carryWeight-lt.9-or!$stream.ready-or$stream.remoteState-ne14-or!$stream.carryLocalHandle){throw 'Carrier bank or mapped Steam carry stream not ready'}
 (Q $peer '--world-npcs')|ConvertTo-Json -Depth 12|Set-Content "$out/observer-world-before.json"
 [ordered]@{character=$character;role=$Role;peer=$peer;dllSha256=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;exeSha256=(Get-FileHash $launch[$Role].executable).Hash;
 bankSha256=(Get-FileHash "$root/work/lab-instances/$peer/coop-status.json.movement.bin").Hash;healthBefore=$healthBefore;scope='Carrier-only native Steam integration. Authored loaded idle and forward walking, standing and crouched. Matched corpse is observed only; attachment, pickup, release and physical outcomes are not ported.';corpseMutations=$false}|ConvertTo-Json -Depth 8|Set-Content "$out/identity.json"
 $window=(Get-Process -Id $launch[$peer].pid).MainWindowHandle.ToInt64();if(!$window){throw 'Missing observer window'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','19','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/native.mp4")
 $capture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 $schedule=@(@{ms=3000;button='Neutral';y=17000;duration=1300},@{ms=6500;button='B';y=0;duration=150},@{ms=9000;button='Neutral';y=17000;duration=1300},@{ms=12000;button='B';y=0;duration=150},@{ms=15000;button='X';y=0;duration=500})
 $watch=[Diagnostics.Stopwatch]::StartNew();$next=0
 while($watch.ElapsedMilliseconds-lt17500){
  if($capture.HasExited){throw 'Capture exited early'}
  if($next-lt$schedule.Count-and$watch.ElapsedMilliseconds-ge$schedule[$next].ms){$e=$schedule[$next];I $e.button $e.y $e.duration;$events.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;input=$e});$next++}
  $states=Q $Role '--player-states';$rows.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;source=$states;sourceRoot=(J $Role);observerRoot=(J $peer);stream=(J $peer '.shadow-stream.json');movement=(J $peer '.movement.json')})
  Start-Sleep -Milliseconds 55
 }
 (Q $Role '--health' '1')|ConvertTo-Json -Depth 8|Set-Content "$out/health-after.json"
 (Q $peer '--world-npcs')|ConvertTo-Json -Depth 12|Set-Content "$out/observer-world-after.json"
}catch{$failure=$_.Exception.Message;throw}finally{
 I
 $rows|ConvertTo-Json -Depth 20|Set-Content "$out/observations.json";$events|ConvertTo-Json -Depth 8|Set-Content "$out/events.json"
 foreach($r in 'host','client'){try{C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_movement_fixture off'}catch{}}
 if($capture-and!$capture.HasExited){$capture.WaitForExit(12000)|Out-Null}
 [ordered]@{failure=$failure;samples=$rows.Count;events=$events;corpseMutations=$false}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
}
