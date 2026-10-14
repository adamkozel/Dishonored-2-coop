[CmdletBinding()]
param([switch]$DisconnectClient,[switch]$Cumulative)
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent;$probe="$PSScriptRoot/bin/d2_runtime_probe.exe";$pids=@{};$rows=[Collections.Generic.List[object]]::new();$failure=$null;$gone=$false
$duration=if($Cumulative){60000}else{15000};$otherDuration=if($Cumulative){15000}else{60000}
function Command($role,$command){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $command -WaitConsumed|Out-Null}
function Bridge($role){Get-Content "$root/work/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json}
function Link($role){Get-Content "$root/work/lab-instances/$role/coop-status.json.sword-link.json" -Raw|ConvertFrom-Json}
function Sample($stage){
 $r=[ordered]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o')}
 foreach($role in 'host','client'){
  $health=$null
  if(!$gone-or$role-ne'client'){$raw=& $probe $pids[$role] --health 1;if($LASTEXITCODE){throw 'Independent player health unavailable'};$health=$raw|ConvertFrom-Json;if($health.current-ne56){throw 'Guard-only test changed health'}}
  $r[$role]=@{Link=(Link $role);Bridge=(Bridge $role);Health=$health}
 }
 $v=[pscustomobject]$r;$rows.Add($v);return $v
}
function Begin {
 foreach($role in 'host','client'){Command $role "coop_sword_link $duration"}
 $watch=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 100;$h=Link host;$c=Link client}while((!$h.ready-or!$c.ready)-and$watch.ElapsedMilliseconds-lt2000)
 if(!$h.ready-or!$c.ready-or$h.hostArm-ne$c.hostArm-or$h.clientArm-ne$c.clientArm){throw 'Fresh handshake failed'}
 return (Sample Armed)
}
function Stopped($stage){
 Start-Sleep -Milliseconds 450;$s=Sample $stage
 if($s.host.Link.armed-or(!$gone-and$s.client.Link.armed)){throw "$stage retained a lease"};return $s
}
try {
 & "$PSScriptRoot/Restore-D2Lab.ps1"|Out-Null
 foreach($role in 'host','client'){
  $launch=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if((Get-Process -Id $launch.pid).Path-ne$launch.executable){throw 'Unexpected process'};$pids[$role]=$launch.pid
  Command $role 'coop_sword_link off';Command $role 'coop_partner_enemy off'
 }
 $initial=Sample Initial;Command host 'coop_sword_duplicate';$unarmed=Sample UnarmedDuplicate
 if($unarmed.client.Link.applied-ne$initial.client.Link.applied){throw 'Unarmed duplicate mutated health'}
 Command host "coop_sword_link $duration";Command client "coop_sword_link $otherDuration";Start-Sleep -Milliseconds 700
 $mismatch=Sample MismatchedLimits
 if($mismatch.host.Link.ready-or$mismatch.client.Link.ready-or$mismatch.host.Link.clientArm-or$mismatch.client.Link.hostArm){throw 'Different hit limits negotiated a binding'}
 foreach($role in 'host','client'){Command $role 'coop_sword_link off'};Stopped MismatchCancelled|Out-Null
 $armed=Begin;Start-Sleep -Milliseconds 400
 foreach($role in 'host','client'){Command $role "coop_sword_link $otherDuration"}
 $duplicate=Sample DuplicateArm
 foreach($role in 'host','client'){if($duplicate.$role.Link.hostArm-ne$armed.$role.Link.hostArm-or$duplicate.$role.Link.clientArm-ne$armed.$role.Link.clientArm-or$duplicate.$role.Link.remainingMs-ge$armed.$role.Link.remainingMs){throw 'Duplicate renewed arming'}}
 Command host 'coop_sword_link off';Stopped Cancelled|Out-Null
 foreach($role in 'client','host'){
  $armed=Begin
  & "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button Start -DurationMs 200|Out-Null
  $paused=Stopped "$role pause";if($paused.$role.Bridge.localGameLogicState-ne2){throw 'Native pause missing'}
  foreach($peer in 'host','client'){Command $peer "coop_sword_link $duration"};Stopped PausedRefusal|Out-Null
  & "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button A -DurationMs 200|Out-Null;Start-Sleep -Milliseconds 600
  foreach($peer in 'host','client'){Command $peer 'coop_probe_follow on'};Start-Sleep -Milliseconds 300
 }
 $armed=Begin;Command host 'coop_damage_observe off';Stopped CaptureInvalidation|Out-Null
 $armed=Begin;$watch=[Diagnostics.Stopwatch]::StartNew()
 do{Start-Sleep -Milliseconds 350;$h=Link host;$c=Link client}while(($h.armed-or$c.armed)-and$watch.ElapsedMilliseconds-lt($duration+1000))
 Stopped Expired|Out-Null
 $armed=Begin;& "$PSScriptRoot/Restore-D2Lab.ps1"|Out-Null;$reloaded=Stopped Reloaded
 if($reloaded.host.Bridge.localEpoch-le$armed.host.Bridge.localEpoch-or$reloaded.client.Bridge.localEpoch-le$armed.client.Bridge.localEpoch){throw 'Reload did not replace both worlds'}
 $fresh=Begin
 if($fresh.host.Link.hostArm-eq$armed.host.Link.hostArm-or$fresh.client.Link.clientArm-eq$armed.client.Link.clientArm){throw 'Old arm token reused'}
 if($DisconnectClient){
  $launch=Get-Content "$root/work/lab-instances/client/launch.json" -Raw|ConvertFrom-Json
  if((Get-Process -Id $launch.pid).Path-ne$launch.executable){throw 'Unexpected process to terminate'}
  & taskkill.exe /PID $launch.pid /F|Out-Null;$gone=$true;Start-Sleep -Milliseconds 1500;Stopped PeerDisconnected|Out-Null
 }else{Command client 'coop_sword_link off';Stopped ClientCancelled|Out-Null}
}catch{$failure=$_.Exception.Message}finally{
 foreach($role in 'host','client'){if($gone-and$role-eq'client'){continue};Command $role 'coop_sword_link off';Command $role 'coop_partner_enemy off'}
}
[ordered]@{Passed=(!$failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;ProbeSha256=(Get-FileHash $probe).Hash;
 Scope='Steam sword relay lifetime without combat stimulus: different hit limits cannot bind, duplicate/different-mode arms do not renew, both native pauses refuse arming, capture generation invalidates, fixed expiry, both checkpoint replacements require new tokens and optional real peer termination. Independent health stays 56.';DurationMs=$duration;Cumulative=[bool]$Cumulative;DisconnectClient=[bool]$DisconnectClient;Samples=$rows}|ConvertTo-Json -Depth 18|Set-Content "$PSScriptRoot/research/sword-relay-guards$(if($Cumulative){'-cumulative'})-verification.json"
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;Samples=$rows.Count;DisconnectClient=[bool]$DisconnectClient}
