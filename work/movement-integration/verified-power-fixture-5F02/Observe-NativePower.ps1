param([ValidateSet('host','client')][string]$Source='host',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-power-corvo-v1')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$pids=@{};$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$failure=$null;$capture=$null;$start=0
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function InputState($r,$trigger=0,$ms=100,$button='Neutral'){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms -LeftTrigger $trigger|Out-Null}
function Probe($r,$mode,$id=0){for($i=0;$i-lt10;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read $r $mode"}
function State($r,$suffix=''){return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}
try{
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;InputState $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_power_fixture off';C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000';if((Probe $r '--health' 1).current-ne56){throw 'Clean living fixture required'}}
 C $Source 'setviewpos -33.5 -153 68.1 270 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Source -Pitch 12 -Evidence "$out/camera.json"
 Probe $Source '--player-power'|ConvertTo-Json -Depth 6|Set-Content "$out/power-before.json"
 C $Source 'coop_power_fixture 20000';$w=[Diagnostics.Stopwatch]::StartNew();do{$s=State $Source '.power-fixture.json';if($s.result-eq'native_power_ready'){break};Start-Sleep -Milliseconds 50}while($w.ElapsedMilliseconds-lt6000)
 if($s.result-ne'native_power_ready'){throw "Power did not equip: $($s.result)"}
 $power=Probe $Source '--player-power';$power|ConvertTo-Json -Depth 6|Set-Content "$out/power-enabled.json"
 $window=(Get-Process -Id $pids[$Source]).MainWindowHandle.ToInt64();if(!$window){throw 'No source game window'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','12','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();$capture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 Start-Sleep -Milliseconds 700;if($capture.HasExited){throw 'Recorder failed'}
 $w.Restart();$fired=$false;$second=$false
 while($w.ElapsedMilliseconds-lt10300){
  if(!$fired-and$w.ElapsedMilliseconds-ge1200){InputState $Source 255 1800;$events.Add([pscustomobject]@{seconds=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;action='Native LT held 1800ms then released'});$fired=$true}
  if(!$second-and$w.ElapsedMilliseconds-ge6100){InputState $Source 255 800;$events.Add([pscustomobject]@{seconds=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;action='Native LT held 800ms then released'});$second=$true}
  $rows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;status=(State $Source);player=(Probe $Source '--player-states');midnight=(Probe $Source '--midnight');component=(Probe $Source '--object' $power.component)})
  if(!(State $Source '.power-fixture.json').active){throw 'Power fixture retired during capture'};Start-Sleep -Milliseconds 20
 }
 InputState $Source;$capture.WaitForExit(8000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode-ne0){throw 'Recorder did not finalize'}
 foreach($r in 'host','client'){if((Probe $r '--health' 1).current-ne56){throw 'Native health changed'}}
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{InputState $r;C $r 'coop_power_fixture off';Start-Sleep -Milliseconds 1500;C $r 'setviewpos -33.5 -153 68.1 0 0';C $r 'coop_movement_fixture off'}catch{}}
 if($capture-and!$capture.HasExited){$capture.WaitForExit(16000)|Out-Null}
 $rows|ConvertTo-Json -Depth 18|Set-Content "$out/states.json"
 [ordered]@{source=$Source;failure=$failure;events=$events;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;capture='Native source game POV at real elapsed timing. Read-only component and scene observations.';scope='Native power cast investigation; not remote animation/VFX integration or movement completion.'}|ConvertTo-Json -Depth 6|Set-Content "$out/result.json"
}
if($failure){throw $failure};"OBSERVED_NATIVE_POWER $Source $($rows.Count) samples"
