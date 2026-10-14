param([string]$Take='observer-camera-v1027',[double]$X=-47.5,[double]$Y=-118,[double]$Z=63.5,[double]$Yaw=-41.6,[double]$Pitch=-5)
# Lab-only observer camera check on the host: view from (X,Y,Z,Yaw,Pitch) is
# held while the host player returns to its parked position. Read-only for
# gameplay apart from the ordinary setviewpos moves; restores the fixture.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$capture=$null;$dirty=$false
function Save($p,$v){$v|ConvertTo-Json -Depth 12|Set-Content -LiteralPath $p}
function C($cmd){& "$lab/Send-D2Command.ps1" -Role host -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($suffix){Get-Content "$root/work/lab-instances/host/coop-status.json$suffix" -Raw|ConvertFrom-Json}
function Q($mode,$arg=0){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $gamePid $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $gamePid $mode};if($LASTEXITCODE){throw "Native query failed $mode"};$raw|ConvertFrom-Json}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 $l=Get-Content "$root/work/lab-instances/host/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid;$gamePid=$l.pid
 if($p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'}
 $before=Q '--player';Save "$out/player-before.json" $before;$dirty=$true
 C ("setviewpos {0} {1} {2} {3} {4}"-f$X,$Y,$Z,$Yaw,$Pitch);Start-Sleep -Milliseconds 600
 $atView=Q '--player';Save "$out/player-at-view.json" $atView
 C 'coop_observer_camera hold';Start-Sleep -Milliseconds 300
 $held=S '.observer-camera.json';Save "$out/hold.json" $held
 if(!$held.active){throw "Hold failed: $($held.result)"}
 C 'setviewpos -33 -153 68.1 90 0';Start-Sleep -Milliseconds 600
 $parked=Q '--player';Save "$out/player-parked.json" $parked
 $hwnd=$p.MainWindowHandle.ToInt64()
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','4','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-movflags','+faststart',"$out/host-held.mp4")
 $capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -PassThru
 if(!$capture.WaitForExit(20000)-or$capture.ExitCode){throw 'Capture failed'}
 $after=S '.observer-camera.json';Save "$out/hold-after.json" $after
 C 'coop_observer_camera off';Start-Sleep -Milliseconds 300;$off=S '.observer-camera.json';Save "$out/off.json" $off
 $d=[Math]::Sqrt([Math]::Pow($parked.candidatePosition[0]+33,2)+[Math]::Pow($parked.candidatePosition[1]+153,2))
 $checks=@{held=$held.active;overridesGrew=($after.overrides-gt$held.overrides);hostParked=($d-lt1.5);released=(!$off.active);heldNearView=([Math]::Abs($held.position[0]-$atView.nativeViewPosition[0])-lt1.5-and[Math]::Abs($held.position[1]-$atView.nativeViewPosition[1])-lt1.5)}
 Save "$out/checks.json" $checks
 if(@($checks.Values|Where-Object {!$_}).Count){throw 'Observer camera checks failed'}
}catch{$failure=$_.Exception.Message}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  try{C 'coop_observer_camera off'}catch{$errors.Add($_.Exception.Message)}
  if($dirty){try{C 'setviewpos -33 -153 68.1 90 0'}catch{$errors.Add($_.Exception.Message)}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;scope='Lab-only observer camera hold on host; no gameplay assertion.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
