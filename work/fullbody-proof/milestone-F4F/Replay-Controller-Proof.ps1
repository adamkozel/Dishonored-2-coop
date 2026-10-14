param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='controller-take-1',[int]$DelayMs=2000)
$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab'
$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take already exists'}
[void](New-Item -ItemType Directory -Path $out)
foreach($role in 'host','client'){
 $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
 if(!$l.assetExperiment -or (Get-Process -Id $l.pid).Path -ne (Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'}
 $s=Get-Content "$root/work/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json
 if(!$s.remoteFresh -or !$s.probeFollowEnabled -or $s.localGameLogicState -ne 1){throw 'Fixture not ready'}
}
function Cmd($role,$text){& "$lab/Send-D2Command.ps1" -Role $role -Command $text -WaitConsumed|Out-Null}
function Input($y,$ms){& "$lab/Send-D2Input.ps1" -Role host -LeftY $y -DurationMs $ms|Out-Null}
$target=Get-Content "$root/work/lab-instances/client/launch.json" -Raw|ConvertFrom-Json
$state=Get-Content "$root/work/lab-instances/client/coop-status.json" -Raw|ConvertFrom-Json
Start-Sleep -Milliseconds $DelayMs
Cmd host 'setviewpos -33 -153 68.1 270 0';Start-Sleep -Milliseconds 1200
Cmd client 'coop_authoring_proof play'
$a=Get-Content "$root/work/lab-instances/client/coop-status.json.authoring.json" -Raw|ConvertFrom-Json
if(!$a.active -or $a.fixed -ge 0){throw 'Proof not accepted'}
$start=[long]$a.start
$capture=Start-Process -FilePath "$PSScriptRoot/build/Release/d2_capture_pose.exe" -ArgumentList @($target.pid,$state.probeActorHandle,9,('"'+(Join-Path $out 'poses.json')+'"'),'actor',$state.bodyPublicationAddress) -WindowStyle Hidden -PassThru -RedirectStandardOutput "$out/capture.log" -RedirectStandardError "$out/capture.err"
$states=[Collections.Generic.List[object]]::new()
try{
 $wait=$start+720-[Environment]::TickCount64;if($wait -gt 0){Start-Sleep -Milliseconds $wait}
 $inputTick=[Environment]::TickCount64;Input 9750 1500
 while([Environment]::TickCount64 -lt $start+8500){
  $states.Add([pscustomobject]@{tick=[Environment]::TickCount64;proof=(Get-Content "$root/work/lab-instances/client/coop-status.json.authoring.json" -Raw|ConvertFrom-Json);source=(Get-Content "$root/work/lab-instances/host/coop-status.json" -Raw|ConvertFrom-Json).localPosition})
  Start-Sleep -Milliseconds 50
 }
}finally{
 Input 0 100;Cmd client 'coop_authoring_proof off'
 $states|ConvertTo-Json -Depth 6|Set-Content "$out/states.json"
 $a|ConvertTo-Json|Set-Content "$out/start.json"
 Copy-Item "$root/work/lab-instances/client/coop-status.json.authoring.json" "$out/final-status.json"
}
if(!$capture.WaitForExit(10000)-or$capture.ExitCode){throw 'Pose capture failed'}
[pscustomobject]@{Take=$Take;Start=$start;InputTick=$inputTick;Input='native controller LeftY 9750, 1500 ms';RootDriver='native player locomotion over existing Steam stream';ClipSha256=(Get-FileHash "$PSScriptRoot/corvo-guard-proof.bin").Hash;DllSha256=(Get-FileHash "$lab/bin/dinput8.dll").Hash}|ConvertTo-Json|Set-Content "$out/manifest.json"
