$ErrorActionPreference='Stop'
$lab=Join-Path $PSScriptRoot '../outputs/d2-coop-lab';$rows=[Collections.Generic.List[object]]::new()
function Exact {
 foreach($role in 'host','client'){& "$lab/Send-D2Command.ps1" -Role $role -Command 'coop_probe_follow exact' -WaitConsumed|Out-Null}
 Start-Sleep -Milliseconds 600
}
function Check($stage){
 foreach($role in 'host','client'){
  $s=Get-Content "$PSScriptRoot/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json
  if(!$s.probeFollowEnabled-or!$s.probeFollowExact-or!$s.remoteFresh-or$s.localGameLogicState-ne1){throw "$stage $role follow scope failed"}
  for($i=0;$i-lt3;$i++){if($s.probeOffset[$i]-ne0-or[Math]::Abs($s.probeRenderPosition[$i]-$s.remotePosition[$i])-gt0.005){throw "$stage $role world coordinate mismatch"}}
  $rows.Add([pscustomobject]@{Stage=$stage;Role=$role;Bridge=$s})
 }
}
Exact;Check Initial
& "$lab/Test-D2SharedCheckpoint.ps1" -RequestRole client -DelayRole client|Out-Null;Check ClientReload
& "$lab/Test-D2SharedCheckpoint.ps1" -RequestRole host -DelayRole host|Out-Null;Check HostReload
& "$lab/Test-D2SharedDeath.ps1" -DeathRole client -RetryRole host -DamageTelemetry|Out-Null;Check ClientDeath
& "$lab/Test-D2SharedDeath.ps1" -DeathRole host -RetryRole client -DamageTelemetry|Out-Null;Check HostDeath
[ordered]@{Passed=$true;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$lab/bin/dinput8.dll").Hash;Scope='Both avatars use exact remote world coordinates before and after either player shared reload or natural death and coordinated retry.';Samples=$rows}|ConvertTo-Json -Depth 15|Set-Content "$lab/research/exact-partner-recovery-verification.json"
& "$lab/Restore-D2Lab.ps1"|Out-Null
& "$lab/Test-D2GuardHealth.ps1"|Out-Null
& "$lab/Restore-D2Lab.ps1"|Out-Null
& "$lab/Test-D2BodyStream.ps1"|Out-Null
& "$lab/Restore-D2Lab.ps1"|Out-Null
& "$lab/Test-D2Baseline.ps1"|Out-Null
'Current partner build regressions passed'
