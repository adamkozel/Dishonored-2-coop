param([double]$X=-37,[double]$Y=-176.5,[double]$Z=68.1,[double]$Yaw=180,[string]$Take='terrain-scout-01')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab'
if($Take-notmatch'^[a-z0-9-]+$'){throw 'Invalid take'}
$out=Join-Path $PSScriptRoot $Take;if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'}
  & "$lab/Send-D2Input.ps1" -Role $r -DurationMs 100|Out-Null
  C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000'
 }
 C host "setviewpos $X $Y $Z $Yaw 0";Start-Sleep -Milliseconds 1000
 & "$root/work/natural-moveset/Set-ObserverPitch.ps1" -Role host -Pitch 12 -Evidence "$out/camera.json"
 $l=Get-Content "$root/work/lab-instances/host/launch.json" -Raw|ConvertFrom-Json;$window=(Get-Process -Id $l.pid).MainWindowHandle.ToInt64()
 & ffmpeg -hide_banner -loglevel error -nostdin -f lavfi -i "gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=10:width=-2:height=-2,hwdownload,format=bgra" -frames:v 1 "$out/view.png"
 if($LASTEXITCODE){throw 'Capture failed'}
 Copy-Item "$root/work/lab-instances/host/coop-status.json" "$out/status.json"
}finally{foreach($r in 'host','client'){C $r 'setviewpos -33.5 -153 68.1 0 0';C $r 'coop_movement_fixture off'}}
