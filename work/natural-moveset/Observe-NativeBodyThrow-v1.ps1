[CmdletBinding()]
param([ValidateSet('host','client')][string]$Role='host',[Parameter(Mandatory)][string]$Take,[switch]$Throw,[switch]$Acquire,[switch]$CrouchedRelease,[switch]$OpenPath)
$ErrorActionPreference='Stop'
$lab=Join-Path (Split-Path (Split-Path $PSScriptRoot)) 'outputs/d2-coop-lab'
$out=Join-Path $PSScriptRoot $Take
if(Test-Path -LiteralPath $out){throw 'Refusing to overwrite carry observation'}
$null=New-Item -ItemType Directory -Path $out
$launch=Get-Content (Join-Path (Split-Path $PSScriptRoot) "lab-instances/$Role/launch.json") -Raw|ConvertFrom-Json
$process=Get-Process -Id $launch.pid
if($process.Path-ne$launch.executable-or$launch.executable-notlike '*\work\asset-game\Dishonored2.exe'){throw 'Wrong isolated process'}
function Q([string]$flag,[string]$value=''){if($value){$raw=& "$lab/bin/d2_runtime_probe.exe" $launch.pid $flag $value}else{$raw=& "$lab/bin/d2_runtime_probe.exe" $launch.pid $flag};if($LASTEXITCODE){return $null};$raw|ConvertFrom-Json}
function I([string]$button='Neutral',[int]$y=0,[int]$duration=150,[int]$rt=0,[int]$x=0){& "$lab/Send-D2Input.ps1" -Role $Role -Button $button -LeftX $x -LeftY $y -DurationMs $duration -RightTrigger $rt|Out-Null}
$before=Q '--player-states'
if(!$Acquire-and!@($before.active|Where-Object id -eq 45).Count){throw 'Expected already-acquired native corpse'}
$before|ConvertTo-Json -Depth 12|Set-Content -Encoding utf8 "$out/before.json"
$healthBefore=Q '--health' '1';if(!$healthBefore){throw 'Native source health unavailable'}
[ordered]@{role=$Role;pid=$launch.pid;executableSha256=(Get-FileHash $launch.executable).Hash;proxySha256=(Get-FileHash (Join-Path (Split-Path $launch.executable) 'dinput8.dll')).Hash;probeSha256=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash;readerSha256=(Get-FileHash "$lab/src/native_player_carry_scene.h").Hash;harnessSha256=(Get-FileHash $PSCommandPath).Hash;healthBefore=$healthBefore}|ConvertTo-Json -Depth 8|Set-Content -Encoding utf8 "$out/identity.json"
$window=$process.MainWindowHandle.ToInt64();if(!$window){throw 'No native window'}
$duration=14
$args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',"$duration",'-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/native.mp4")
$capture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new()
$watch=[Diagnostics.Stopwatch]::StartNew();$next=0
# The caller prepares a fresh isolated fixture and looks at the floor body.
$schedule=@(@{ms=700;button='X';y=0;duration=900},@{ms=7000;button='Neutral';y=0;duration=500;rt=255})
$leveled=$false
try {
 while($watch.ElapsedMilliseconds-lt($duration*1000-1500)){
  if($capture.HasExited){throw 'Native capture exited early'}
  if($next-lt$schedule.Count-and$watch.ElapsedMilliseconds-ge$schedule[$next].ms){$e=$schedule[$next];I $e.button $e.y $e.duration ([int]$e.rt) ([int]$e.x);$events.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;input=$e});$next++}
  if(!$leveled-and$watch.ElapsedMilliseconds-ge4000){
   $held=Q '--player-states';if(!$held.carrySceneValid-or$held.carryScene.variant-notin @(2,3)){throw 'Native body acquisition did not reach loaded state'}
   & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Role -Pitch 0 -Evidence "$out/throw-camera.json"|Out-Null
   $leveled=$true
  }
  $states=Q '--player-states';$midnight=Q '--midnight'
  $rows.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;native=$states;carryScenes=@($midnight.instances|Where-Object asset -like '*carrycorpse*')})
  Start-Sleep -Milliseconds 20
 }
} finally {
 I
 $rows|ConvertTo-Json -Depth 20|Set-Content -Encoding utf8 "$out/observations.json"
 $events|ConvertTo-Json -Depth 8|Set-Content -Encoding utf8 "$out/events.json"
 if(!$capture.HasExited){$capture.WaitForExit(10000)|Out-Null}
}
$last=Q '--player-states';$last|ConvertTo-Json -Depth 12|Set-Content -Encoding utf8 "$out/after.json"
(Q '--health' '1')|ConvertTo-Json -Depth 8|Set-Content -Encoding utf8 "$out/health-after.json"
[pscustomobject]@{take=$Take;samples=$rows.Count;carryAfter=@($last.active|Where-Object id -eq 45).Count;scenes=@($rows.carryScenes.asset|Sort-Object -Unique)}|ConvertTo-Json -Depth 5
