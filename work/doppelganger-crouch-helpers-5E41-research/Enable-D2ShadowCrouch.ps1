[CmdletBinding()]
param()
# Opt-in visual prototype for the existing isolated Dunwall session.
# Captures each clone's native whole-body reference; does not reload or save.
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$expected='5E41A801077E4206D07029ABFCB1FD5F8EE6AA8A522656DDFC667836807C50E5'
function State($r){
 $file=Get-Item "$root/work/lab-instances/$r/coop-status.json"
 if(([DateTime]::UtcNow-$file.LastWriteTimeUtc).TotalSeconds-gt2){throw "Stale $r bridge"}
 Get-Content $file.FullName -Raw|ConvertFrom-Json
}
function Cmd($r,$s){& "$PSScriptRoot/Send-D2Command.ps1" -Role $r -Command $s -WaitConsumed|Out-Null}
$before=@{};$results=[Collections.Generic.List[object]]::new();$mutated=$false
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  $p=Get-Process -Id $l.pid
  if($p.Path-ne$l.executable-or!$l.assetExperiment-or(Get-FileHash (Join-Path (Split-Path $l.executable) 'dinput8.dll')).Hash-ne$expected){throw 'Unexpected live isolated build'}
  $s=State $r
  if(!$s.remoteFresh-or!$s.probeFollowEnabled-or$s.localGameLogicState-ne1-or$s.remoteGameLogicState-ne1){throw 'Both isolated players must be in live gameplay with bound clones'}
  $before[$r]=$s
 }
 $mutated=$true
 foreach($r in 'host','client'){
  foreach($c in 'coop_shadow_stream off','coop_shadow_action off','coop_shadow_crouch off','coop_probe_follow exact','coop_probe_locomotion on','coop_probe_smoothing on'){Cmd $r $c}
 }
 foreach($r in 'host','client'){
  Cmd $r 'coop_shadow_action crouchpose'
  Start-Sleep -Milliseconds 750
  for($attempt=0;$attempt-lt18;$attempt++){
   Cmd $r 'coop_shadow_crouch capture';Start-Sleep -Milliseconds 90
   if((State $r).nativeCrouchReferenceReady){break}
  }
  if(!(State $r).nativeCrouchReferenceReady){throw "No whole-body native reference for $r"}
  Cmd $r 'coop_shadow_action off'
  Start-Sleep -Milliseconds 650
 }
 foreach($r in 'host','client'){Cmd $r 'coop_probe_body on'}
 Start-Sleep -Milliseconds 750
 foreach($r in 'host','client'){
  $s=State $r
  # Standing deliberately leaves the native graph untouched; application counts
  # increase only after the peer enters crouch. Check arming here, not writes.
  if(!$s.bodyStreamEnabled-or!$s.nativeCrouchReferenceReady-or$s.localEpoch-ne$before[$r].localEpoch-or$s.probeActorHandle-ne$before[$r].probeActorHandle){throw "Whole-body stream did not arm on $r"}
  $results.Add([pscustomobject]@{Role=$r;Epoch=$s.localEpoch;Handle=$s.probeActorHandle;ReferenceJoints=$s.nativeCrouchJointCount;Applied=$s.bodyStreamApplied-$before[$r].bodyStreamApplied;BodyStream=$s.bodyStreamEnabled;Steam=$s.nativeInterface})
 }
}catch{
 if($mutated){foreach($r in 'host','client'){try{Cmd $r 'coop_probe_body off';Cmd $r 'coop_shadow_action off'}catch{}}}
 throw
}
[ordered]@{At=[DateTimeOffset]::Now.ToString('o');Dll=$expected;Enabled=$true;Scope='Existing isolated session only; native whole-body crouch research. Unfinished sword posture and crouched combat. Checkpoint/restart needs a new reference.';Roles=$results}|ConvertTo-Json -Depth 5|Set-Content "$PSScriptRoot/research/doppelganger-crouch-live-enable.json"
$results
