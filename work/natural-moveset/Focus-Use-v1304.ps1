param([Parameter(Mandatory)][string]$Take,[Parameter(Mandatory)][string]$Tag,[Parameter(Mandatory)][string]$Target,
 [double[]]$Radii=@(0.9,1.3),[double[]]$Bearings=@(0,45,90,135,180,225,270,315),[double]$EyeUp=1.35,[double]$FloorZ=[double]::NaN,
 [double[]]$Pitches=@(0,15,30,-10),[double[]]$YawOffsets=@(0,-15,15),[int]$Hold=400,[switch]$NoPress)
# Focus-guided use on a mission held by Route-Mission-v1300 -HoldAtEnd: stand
# around the target usable, sweep yaw and pitch, and read the native use focus
# (arkCpntPlayerSensorInteraction+0x80 high dword = focused entity handle,
# confirmed runtime in take 1133). Press X only when the target has the focus,
# then report logic, cinematic epoch and position. Frames go to <take>/pokes.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path (Join-Path $PSScriptRoot $Take) 'pokes'
New-Item -ItemType Directory $out -Force|Out-Null
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
$hostPid=(Get-Content "$root/work/lab-instances/host/launch.json" -Raw|ConvertFrom-Json).pid
function Q($mode,$arg){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $hostPid $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $hostPid $mode};if($LASTEXITCODE){throw "probe $mode failed"};$raw|ConvertFrom-Json}
function C($cmd){& "$lab/Send-D2Command.ps1" -Role host -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function St{$s=Get-Content "$root/work/lab-instances/host/coop-status.json" -Raw|ConvertFrom-Json;[ordered]@{logic=$s.localGameLogicState;cine=$s.localCinematicEpoch;pos=$s.localPosition}}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 $ents=(Q '--entities').entities;$t=@($ents|Where-Object name -eq $Target);if($t.Count-ne1){throw "Target $Target not unique"}
 $th=[Convert]::ToUInt32(($t[0].handle-replace'^0x',''),16);$p=$t[0].position;$floor=if([double]::IsNaN($FloorZ)){$p[2]}else{$FloorZ}
 $pl=Q '--player';$me=@($ents|Where-Object pointer -eq $pl.player);if($me.Count-ne1){throw 'Player entity not unique'}
 $sensor=@((Q '--components' $me[0].handle).components|Where-Object type -eq '.?AVarkCpntPlayerSensorInteraction@@')[0].pointer
 $tries=[Collections.Generic.List[object]]::new();$hit=$null
 foreach($r in $Radii){foreach($b in $Bearings){
  $a=$b*[Math]::PI/180;$x=$p[0]+$r*[Math]::Cos($a);$y=$p[1]+$r*[Math]::Sin($a);$yaw0=[Math]::Atan2($p[1]-$y,$p[0]-$x)*180/[Math]::PI
  foreach($yo in $YawOffsets){C ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$x,$y,($floor+$EyeUp),($yaw0+$yo));Start-Sleep -Milliseconds 500
   foreach($pt in $Pitches){try{& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch $pt -Quick|Out-Null}catch{}
    $w=@((Q '--object' $sensor).words|Where-Object offset -eq '0x80');$fh=[uint32]([Convert]::ToUInt64($w[0].word.Substring(2),16)-shr32)
    $tries.Add([ordered]@{radius=$r;bearing=$b;yaw=$yaw0+$yo;pitch=$pt;focus=('0x{0:x}'-f$fh)})
    if($fh-eq$th){$hit=$tries[$tries.Count-1];break}}
   if($hit){break}}
  if($hit){break}}
  if($hit){break}}
 $before=St;$after=$before
 if($hit){
  $hwnd=(Get-Process -Id $hostPid).MainWindowHandle.ToInt64()
  & (Get-Command ffmpeg).Source -hide_banner -loglevel error -nostdin -f lavfi -i "gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra" -frames:v 1 -y "$out/$Tag.png" 2>$null|Out-Null
  if(!$NoPress){& "$lab/Send-D2Input.ps1" -Role host -Button X -DurationMs $Hold|Out-Null;Start-Sleep -Milliseconds 2500;$after=St}}
 [ordered]@{tag=$Tag;target=$Target;found=[bool]$hit;hit=$hit;tries=$tries.Count;before=$before;after=$after}|ConvertTo-Json -Depth 5
}finally{if($owned){$mutex.ReleaseMutex()};$mutex.Dispose()}
