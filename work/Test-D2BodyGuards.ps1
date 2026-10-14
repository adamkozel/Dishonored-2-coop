$ErrorActionPreference='Stop'
$lab=Join-Path (Split-Path $PSScriptRoot -Parent) 'outputs/d2-coop-lab'
$rows=[Collections.Generic.List[object]]::new();$failure=$null
function State([string]$role){Get-Content "$PSScriptRoot/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json}
function Command([string]$role,[string]$cmd){& "$lab/Send-D2Command.ps1" -Role $role -Command $cmd|Out-Null}
function Pair([string]$stage){$r=[pscustomobject]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o');Host=(State host);Client=(State client)};$rows.Add($r);return $r}
function Arm(){
 $before=Pair BeforeArm
 foreach($role in 'host','client'){Command $role 'coop_body_probe 2000'}
 Start-Sleep -Milliseconds 350
 $active=Pair Armed
 foreach($role in 'Host','Client'){if($active.$role.bodyProbeApplied -le $before.$role.bodyProbeApplied){throw "$role did not apply an armed pose"}}
}
try {
 Arm
 foreach($role in 'host','client'){Command $role 'coop_body_probe 2000'}
 Start-Sleep -Milliseconds 1850
 $expired=Pair LeaseExpired
 Start-Sleep -Milliseconds 400
 $stable=Pair LeaseRemainsExpired
 foreach($role in 'Host','Client'){if($stable.$role.bodyProbeApplied -ne $expired.$role.bodyProbeApplied){throw 'Duplicate request extended lease'};if($stable.$role.bodyProbeRefused -lt 1){throw 'Duplicate was not refused'}}
 foreach($pauseRole in 'host','client'){
  Arm
  & "$lab/Send-D2Input.ps1" -Role $pauseRole -Button Start -DurationMs 150|Out-Null
  Start-Sleep -Milliseconds 500
  $paused=Pair "$pauseRole pause"
  if($paused.$pauseRole.localGameLogicState -ne 2){throw 'Native pause not reached'}
  foreach($role in 'host','client'){Command $role 'coop_body_probe 2000'}
  Start-Sleep -Milliseconds 500
  $held=Pair PauseRefusal
  foreach($role in 'Host','Client'){if($held.$role.bodyProbeApplied -ne $paused.$role.bodyProbeApplied -or $held.$role.probeFollowEnabled){throw 'Pose survived native pause'}}
  & "$lab/Send-D2Input.ps1" -Role $pauseRole -Button A -DurationMs 150|Out-Null
  Start-Sleep -Milliseconds 700
  foreach($role in 'host','client'){Command $role 'coop_probe_follow on'}
  Start-Sleep -Milliseconds 350
  $resumed=Pair ResumeWithoutRearm
  foreach($role in 'Host','Client'){if($resumed.$role.bodyProbeApplied -ne $paused.$role.bodyProbeApplied){throw 'Pose unexpectedly resumed'}}
 }
 Arm
 foreach($role in 'host','client'){Command $role 'coop_body_probe off'}
 Start-Sleep -Milliseconds 400
 $cancelled=Pair Cancelled
 Start-Sleep -Milliseconds 400
 $stopped=Pair CancelRemainsStopped
 foreach($role in 'Host','Client'){if($stopped.$role.bodyProbeApplied -ne $cancelled.$role.bodyProbeApplied){throw 'Explicit cancellation did not stop pose'}}
 Arm
 & "$lab/Restore-D2Lab.ps1"|Out-Null
 $restored=Pair RestoredWorld
 Start-Sleep -Milliseconds 450
 $later=Pair NewWorldStable
 foreach($role in 'Host','Client'){if(!$later.$role.probeFollowEnabled -or $later.$role.bodyProbeApplied -ne $restored.$role.bodyProbeApplied){throw 'Pose binding survived world replacement'}}
}catch{$failure=$_.Exception.Message}finally {
 foreach($role in 'host','client'){Command $role 'coop_body_probe off'; & "$lab/Send-D2Input.ps1" -Role $role -DurationMs 100|Out-Null}
 [ordered]@{Passed=($null -eq $failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$lab/bin/dinput8.dll").Hash;Scope='Bounded local pose lease, duplicate refusal, both native pauses, explicit cancellation and world replacement; no networked body pose.';Samples=$rows.ToArray()}|ConvertTo-Json -Depth 12|Set-Content "$lab/research/body-probe-guards-verification.json"
}
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;Samples=$rows.Count}
