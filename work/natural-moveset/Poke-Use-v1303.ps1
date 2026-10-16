param([Parameter(Mandatory)][string]$Take,[Parameter(Mandatory)][string]$Tag,[double]$X,[double]$Y,[double]$Z,[double]$Yaw,[double]$Pitch=0,[int]$Hold=250,[switch]$NoPress)
# Interactive probe on a mission held by Route-Mission-v1300 -HoldAtEnd: place
# the host, set the pitch, save a frame, press X (unless -NoPress) and report
# logic, cinematic epoch and gameflow before and after. Frames go to
# <take>/pokes. Restore with Restore-RouteHold-v1303.ps1 when done.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path (Join-Path $PSScriptRoot $Take) 'pokes'
New-Item -ItemType Directory $out -Force|Out-Null
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
$hostPid=(Get-Content "$root/work/lab-instances/host/launch.json" -Raw|ConvertFrom-Json).pid
function C($cmd){& "$lab/Send-D2Command.ps1" -Role host -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function St{$s=Get-Content "$root/work/lab-instances/host/coop-status.json" -Raw|ConvertFrom-Json;$m=Get-Content "$root/work/lab-instances/host/coop-status.json.missionend.json" -Raw|ConvertFrom-Json
 [ordered]@{logic=$s.localGameLogicState;cine=$s.localCinematicEpoch;pos=$s.localPosition;gameflow="$($m.gameflowStates)"}}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 C ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$X,$Y,$Z,$Yaw);Start-Sleep -Milliseconds 700
 if($Pitch){& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch $Pitch -Quick|Out-Null}
 $hwnd=(Get-Process -Id $hostPid).MainWindowHandle.ToInt64()
 & (Get-Command ffmpeg).Source -hide_banner -loglevel error -nostdin -f lavfi -i "gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra" -frames:v 1 -y "$out/$Tag.png" 2>$null|Out-Null
 $eye=(& "$lab/bin/d2_runtime_probe.exe" $hostPid --player|ConvertFrom-Json).nativeViewPosition
 $b=St;if(!$NoPress){& "$lab/Send-D2Input.ps1" -Role host -Button X -DurationMs $Hold|Out-Null;Start-Sleep -Milliseconds 1800};$a=St
 [ordered]@{tag=$Tag;eye=$eye;before=$b;after=$a;changed=($a.logic-ne$b.logic-or$a.cine-ne$b.cine-or$a.gameflow-ne$b.gameflow)}|ConvertTo-Json -Depth 5
}finally{if($owned){$mutex.ReleaseMutex()};$mutex.Dispose()}
