param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='runtime-take-1',[int]$DelayMs=2000)
$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab'
. "$lab/tooling/Publish-D2Packet.ps1"
$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take already exists'}
[void](New-Item -ItemType Directory -Path $out)
$meta=Get-Content "$PSScriptRoot/authored-sequence.json" -Raw|ConvertFrom-Json
foreach($role in 'host','client'){
 $launch=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
 $p=Get-Process -Id $launch.pid
 if(!$launch.assetExperiment -or $p.Path -ne (Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'}
 $s=Get-Content "$root/work/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json
 if(!$s.remoteFresh -or !$s.probeFollowEnabled -or $s.localGameLogicState -ne 1){throw 'Fixture not ready'}
}
$target=Get-Content "$root/work/lab-instances/client/launch.json" -Raw|ConvertFrom-Json
$state=Get-Content "$root/work/lab-instances/client/coop-status.json" -Raw|ConvertFrom-Json
$rows=[Collections.Generic.List[object]]::new()
function NativeCommand($role,$text){& "$lab/Send-D2Command.ps1" -Role $role -Command $text -WaitConsumed|Out-Null}
Start-Sleep -Milliseconds $DelayMs
NativeCommand host 'setviewpos -33 -153 68.1 270 0'
NativeCommand client 'coop_authoring_proof play'
$a=Get-Content "$root/work/lab-instances/client/coop-status.json.authoring.json" -Raw|ConvertFrom-Json
if(!$a.active -or $a.fixed -ge 0){throw 'Proof not accepted'}
$capture=Start-Process -FilePath "$PSScriptRoot/build/Release/d2_capture_pose.exe" -ArgumentList @($target.pid,$state.probeActorHandle,8,('"'+(Join-Path $out 'poses.json')+'"'),'actor') -WindowStyle Hidden -PassThru -RedirectStandardOutput "$out/capture.log" -RedirectStandardError "$out/capture.err"
$start=[long]$a.start;$sequence=[uint32]([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()%4294967296)
try{
 $next=[Environment]::TickCount64
 while([Environment]::TickCount64 -lt $start+7000){
  $tick=[Environment]::TickCount64;$t=($tick-$start+80)/1000.0 # compensate the verified 80 ms presentation buffer
  $frame=[Math]::Clamp($t*60,0,360);$i=[int][Math]::Floor($frame);$k=[Math]::Min($i+1,360);$distance=$meta.rootTrack[$i]+($meta.rootTrack[$k]-$meta.rootTrack[$i])*($frame-$i)
  # Source player only. The remote clone receives its usual Steam root poses.
  # Native Corvo's captured +X axis is world -Y in this fixture.
  $y=-153-$distance;$cmd=[string]::Format([Globalization.CultureInfo]::InvariantCulture,'setviewpos -33 {0:F6} 68.1 270 0',$y)
  $packet=[byte[]]::new(528);[BitConverter]::GetBytes([uint32]0x43433244).CopyTo($packet,0);$sequence++;[BitConverter]::GetBytes($sequence).CopyTo($packet,4)
  $expiry=[DateTime]::UtcNow.AddMilliseconds(500);[BitConverter]::GetBytes([uint64]$expiry.ToFileTimeUtc()).CopyTo($packet,8);[Text.Encoding]::ASCII.GetBytes($cmd).CopyTo($packet,16)
  Publish-D2Packet -Path "$root/work/lab-instances/host/command.bin" -Bytes $packet -ExpiresUtc $expiry
  $rows.Add([pscustomobject]@{tick=$tick;clipSeconds=$t;distance=$distance;command=$cmd;sequence=$sequence})
  $next+=20;$sleep=$next-[Environment]::TickCount64;if($sleep -gt 0){Start-Sleep -Milliseconds $sleep}else{$next=[Environment]::TickCount64}
 }
}finally{
 NativeCommand client 'coop_authoring_proof off'
 & "$lab/Send-D2Input.ps1" -Role host -Button Neutral -DurationMs 100|Out-Null
 $rows|ConvertTo-Json -Depth 4|Set-Content "$out/source-root-trace.json"
 $a|ConvertTo-Json|Set-Content "$out/start.json"
 Copy-Item "$root/work/lab-instances/client/coop-status.json.authoring.json" "$out/final-status.json"
}
if(!$capture.WaitForExit(10000)-or$capture.ExitCode){throw 'Pose capture failed'}
[pscustomobject]@{Take=$Take;Commands=$rows.Count;Start=$start;ClipSha256=(Get-FileHash "$PSScriptRoot/corvo-guard-proof.bin").Hash;DllSha256=(Get-FileHash "$lab/bin/dinput8.dll").Hash}|ConvertTo-Json|Set-Content "$out/manifest.json"
