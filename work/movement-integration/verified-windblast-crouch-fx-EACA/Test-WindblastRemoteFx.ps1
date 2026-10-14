param([ValidateSet('host','client')][string]$Role='client',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='remote-windblast-fx-corvo-v1',[ValidateSet('windblast')][string]$Family='windblast')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$source=if($Role-eq'client'){'host'}else{'client'};$pids=@{};$handles=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$capture=$null
$first='windblast_remote_probe';$second='windblast_ground_probe';$kinds=@('8','7')
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt8;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function P($r,$mode,$id){$raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id;if($LASTEXITCODE){throw 'Probe failed'};return($raw|ConvertFrom-Json)}
try{
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong process'};$pids[$r]=$l.pid;$handles[$r]=(S $r).probeActorHandle;& "$lab/Send-D2Input.ps1" -Role $r -Button Neutral -DurationMs 100|Out-Null;C $r 'coop_shadow_stream off';C $r 'coop_movement_bank off';C $r 'coop_movement_fixture off';C $r 'coop_power_fx off'}
 Start-Sleep -Milliseconds 250;$before=S $Role '.power-fx.json';C $Role "coop_power_fx $first";Start-Sleep -Milliseconds 250;$refused=S $Role '.power-fx.json'
 if($refused.started-ne$before.started-or$refused.refused-ne($before.refused+1)){throw 'Effect did not refuse outside isolated fixture'};$checks.Add('Native effect start refused without scoped NPC isolation')
 foreach($r in 'host','client'){C $r 'coop_movement_fixture 60000';if((P $r '--health' 1).current-ne56-or(P $r '--health' $handles[$r]).current-ne40){throw 'Healthy fixture required'}}
 C $Role 'setviewpos -29 -151 68.1 190 0';Start-Sleep -Milliseconds 250;C $source 'setviewpos -33.5 -153 68.1 0 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Role -Pitch 8 -Evidence "$out/camera.json"
 $window=(Get-Process -Id $pids[$Role]).MainWindowHandle.ToInt64();if(!$window){throw 'No observer window'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','9','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
 $capture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru;Start-Sleep -Milliseconds 700;if($capture.HasExited){throw 'Recorder failed'}
 $watch=[Diagnostics.Stopwatch]::StartNew();$stage=0;$seen=@{}
 while($watch.ElapsedMilliseconds-lt7400){$ms=$watch.ElapsedMilliseconds
  if($stage-eq0-and$ms-ge700){C $Role "coop_power_fx $first";$stage=1}
  if($stage-eq1-and$ms-ge2900){C $Role "coop_power_fx $second";$stage=2}
  if($stage-eq2-and$ms-ge5300){C $Role "coop_power_fx $first";$stage=3}
  if($stage-eq3-and$ms-ge5550){C $Role 'coop_power_fx off';$stage=4}
  $fx=S $Role '.power-fx.json';$rows.Add([pscustomobject]@{ms=$ms;stage=$stage;fx=$fx})
  foreach($e in $fx.effects){if($e.owner-ne$handles[$Role]-or$e.record-eq'0x0'){throw 'Native FX ownership mismatch'};$seen[[string]$e.kind]=$true}
  Start-Sleep -Milliseconds 35
 }
 if(!$seen[$kinds[0]]-or!$seen[$kinds[1]]-or$fx.started-ne($before.started+3)-or$fx.active){throw "Native Windblast effects/lifecycle mismatch: $($fx|ConvertTo-Json -Compress)"}
 $checks.Add("Both native $Family declarations create effects with exact clone owner, unique FX ID and expected declaration")
 $checks.Add('Finite effects retire and explicit cancellation stops only tracked native records')
 foreach($r in 'host','client'){if((P $r '--health' 1).current-ne56-or(P $r '--health' $handles[$r]).current-ne40){throw 'Native health changed'}};$checks.Add('Both players and native avatar health preserved')
 foreach($f in (Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('Original executable, Steam API and protected save hashes unchanged')
 $capture.WaitForExit(6000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode-ne0){throw 'Recorder did not finalize'}
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{C $r 'coop_power_fx off';C $r 'coop_movement_fixture off'}catch{}}
 if($capture-and!$capture.HasExited){$capture.WaitForExit(10000)|Out-Null}
 $rows|ConvertTo-Json -Depth 8|Set-Content "$out/states.json"
 [ordered]@{role=$Role;family=$Family;failure=$failure;checks=$checks;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;scope='Isolated world-space reuse of native Windblast particles and sound, plus native ground-dust feasibility. Dedicated effect declaration omits first-person fullscreen distortion/rumble. Manual finite native visual effects, without casting or gameplay outcomes. Screen distortion and visible world effects require visual review before any network integration.'}|ConvertTo-Json -Depth 5|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_NATIVE_POWER_FX $Role $($checks.Count) checks"
