$ErrorActionPreference='Stop'
$lab=Join-Path (Split-Path $PSScriptRoot -Parent) 'outputs/d2-coop-lab'
$records=[Collections.Generic.List[object]]::new();$failure=$null
function Status([string]$role){Get-Content "$PSScriptRoot/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json}
function ReadSkeleton([int]$gamePid,[uint32]$handle){$raw=& "$lab/bin/d2_runtime_probe.exe" $gamePid --skeleton $handle;if($LASTEXITCODE -eq 0){return $raw|ConvertFrom-Json};return $null}
try {
 foreach($role in 'host','client'){
  $launch=Get-Content "$PSScriptRoot/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  $initial=Status $role;$handle=$initial.probeActorHandle
  if(!$initial.probeFollowEnabled -or !$initial.remoteFresh){throw 'Fixture is not following'}
  $before=ReadSkeleton $launch.pid $handle
  & "$lab/Send-D2Input.ps1" -Role $role -Button B -DurationMs 150|Out-Null
  Start-Sleep -Milliseconds 600
  $source=ReadSkeleton $launch.pid 1
  & "$lab/Send-D2Command.ps1" -Role $role -Command 'coop_body_probe 2000'|Out-Null
  $samples=[Collections.Generic.List[object]]::new();$watch=[Diagnostics.Stopwatch]::StartNew()
  while($watch.ElapsedMilliseconds -lt 3200){
   $s=ReadSkeleton $launch.pid $handle
   $raw=& "$lab/bin/d2_runtime_probe.exe" $launch.pid --bones $handle;$boneCode=$LASTEXITCODE
   $samples.Add([pscustomobject]@{Ms=$watch.ElapsedMilliseconds;Skeleton=$s;Bones=$(if(!$boneCode){$raw|ConvertFrom-Json}else{$null});State=(Status $role)})
   Start-Sleep -Milliseconds 75
  }
  $after=ReadSkeleton $launch.pid $handle
  & "$lab/Send-D2Input.ps1" -Role $role -Button B -DurationMs 150|Out-Null
  Start-Sleep -Milliseconds 500
  $final=Status $role
  $records.Add([pscustomobject]@{Role=$role;Initial=$initial;Before=$before;Source=$source;Samples=$samples.ToArray();After=$after;Final=$final})
  if($final.bodyProbeApplied -le $initial.bodyProbeApplied){throw "$role body probe was not applied; refused=$($final.bodyProbeRefused-$initial.bodyProbeRefused)"}
 }
}catch{$failure=$_.Exception.Message}finally {
 foreach($role in 'host','client'){& "$lab/Send-D2Command.ps1" -Role $role -Command 'coop_body_probe off'|Out-Null;& "$lab/Send-D2Input.ps1" -Role $role -DurationMs 100|Out-Null}
 [ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');Failure=$failure;DllSha256=(Get-FileHash "$lab/bin/dinput8.dll").Hash;ProbeSha256=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash;Scope='Two-second local source-pose retarget experiment; no remote skeleton networking.';Records=$records.ToArray()}|ConvertTo-Json -Depth 24|Set-Content "$PSScriptRoot/body-probe-observations.json"
}
if($failure){throw $failure}
$records|ForEach-Object{[pscustomobject]@{Role=$_.Role;Applied=$_.Final.bodyProbeApplied-$_.Initial.bodyProbeApplied;Refused=$_.Final.bodyProbeRefused-$_.Initial.bodyProbeRefused;Samples=$_.Samples.Count;BeforeHip=$_.Before.joints[2].globalPose[14];LowestHip=($_.Samples|Where-Object {$_.Skeleton.stable}|ForEach-Object {$_.Skeleton.joints[2].globalPose[14]}|Measure-Object -Minimum).Minimum;AfterHip=$_.After.joints[2].globalPose[14]}}|ConvertTo-Json
