[CmdletBinding()]
param([switch]$DisconnectHost,[switch]$GuardExpiry)
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent;$probe="$PSScriptRoot/bin/d2_runtime_probe.exe"
$handles=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null
function Command($role,$command){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $command -WaitConsumed|Out-Null}
function State($role){
 $p="$root/work/lab-instances/$role/coop-status.json";$l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
 if((Get-Process -Id $l.pid).Path-ne$l.executable){throw 'Unexpected process'}
 if(([DateTime]::UtcNow-(Get-Item $p).LastWriteTimeUtc).TotalSeconds-gt1.5){throw 'Stale status'}
 [pscustomobject]@{Launch=$l;Bridge=(Get-Content $p -Raw|ConvertFrom-Json);Guard=(Get-Content "$p.guard.json" -Raw|ConvertFrom-Json);Outcome=(Get-Content "$p.enemy-outcome.json" -Raw|ConvertFrom-Json)}
}
function Native($role,$kind,$handle){
 $s=State $role
 for($i=0;$i-lt8;$i++){$raw=& $probe $s.Launch.pid $kind $handle;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Unavailable $role $kind"
}
function Capture($stage,$hostPresent=$true){
 $r=[ordered]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o')}
 foreach($role in 'host','client'){
  if($role-eq'host'-and!$hostPresent){continue}
  $r[$role]=[pscustomobject]@{State=(State $role);Health=(Native $role '--health' $handles[$role]);Player=(Native $role '--health' 1);
   Decision=(Native $role '--ai-decision' $handles[$role]);Reflex=(Native $role '--ai-reflex' $handles[$role])}
 }
 $v=[pscustomobject]$r;$rows.Add($v);return $v
}
function Arm {
 & "$PSScriptRoot/Restore-D2Lab.ps1"|Out-Null
 foreach($role in 'host','client'){
  Command $role 'coop_enemy_outcome off';Command $role 'coop_guard_link off';Command $role 'coop_damage_observe off'
  $s=State $role;$cat=& $probe $s.Launch.pid --world-npcs|ConvertFrom-Json;$n=@($cat.npcs|Where-Object spawnerName -eq ld_spawner_guard_escape_1)
  if($n.Count-ne1-or!$n[0].unique-or!$n[0].reciprocal){throw 'Ambiguous guard'};$handles[$role]=$n[0].handle
  Command $role 'coop_guard_link escape 60000'
 }
 Start-Sleep -Milliseconds 500
 foreach($role in 'host','client'){Command $role 'coop_enemy_outcome 60000'}
 $watch=[Diagnostics.Stopwatch]::StartNew()
 do{Start-Sleep -Milliseconds 100;$h=State host;$c=State client}while((!$h.Outcome.ready-or!$c.Outcome.ready)-and$watch.ElapsedMilliseconds-lt2500)
 if(!$h.Outcome.ready-or!$c.Outcome.ready){throw 'Outcome handshake unavailable'}
 return(Capture Armed)
}
function CheckClosed($before,$after,$hostPresent=$true,$guardMustRelease=$true){
 foreach($role in 'host','client'){
  if($role-eq'host'-and!$hostPresent){continue}
  $a=$after.$role;$b=$before.$role
  if($a.State.Outcome.armed-or$a.State.Outcome.applied-ne$b.State.Outcome.applied-or$a.State.Outcome.captured-ne$b.State.Outcome.captured-or$a.Health.current-ne40-or$a.Player.current-ne56){throw "Outcome survived invalidation or damaged an actor: $role $($after.Stage)"}
  if($guardMustRelease){
   if($a.State.Guard.phase-or$a.State.Guard.heldReferences-or$a.State.Guard.releaseLost-ne$b.State.Guard.releaseLost){throw 'Guard AI ownership survived invalidation'}
   foreach($ai in $a.Decision,$a.Reflex){if($ai.pauseReferences-or!$ai.schedulerActive){throw 'Native AI ownership was not returned'}}
  }
 }
}
try{
 $armed=Arm
 Command client 'coop_enemy_outcome 60000';Start-Sleep -Milliseconds 300;$duplicate=Capture DuplicateArm
 if(!$duplicate.client.State.Outcome.armed-or$duplicate.client.State.Outcome.hostArm-ne$armed.client.State.Outcome.hostArm-or$duplicate.client.State.Outcome.clientArm-ne$armed.client.State.Outcome.clientArm-or
    $duplicate.client.State.Outcome.remainingMs-ge$armed.client.State.Outcome.remainingMs-or$duplicate.client.State.Outcome.rejected-ne$armed.client.State.Outcome.rejected+1){throw 'Duplicate arm renewed or replaced a local lease'}
 Command client 'coop_enemy_outcome off';Start-Sleep -Milliseconds 400;$off=Capture LocalOff;CheckClosed $armed $off $true $false
 if($off.client.State.Guard.phase-ne7-or$off.client.State.Guard.heldReferences-ne2){throw 'Outcome toggle changed independent guard ownership'}
 $checks.Add('Duplicate local arm does not renew tokens/deadline; local off closes both outcome paths without stealing guard ownership')
 foreach($role in 'host','client'){
  $armed=Arm
  & "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button Start -DurationMs 200|Out-Null
  Start-Sleep -Milliseconds 600;$paused=Capture "Paused-$role";CheckClosed $armed $paused
  if((State $role).Bridge.localGameLogicState-ne2){throw 'Expected native pause'}
  & "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button A -DurationMs 200|Out-Null
  Start-Sleep -Milliseconds 500;$resumed=Capture "Resumed-$role";CheckClosed $armed $resumed
  $checks.Add("$role native pause and resume leave no enemy outcome authority or new damage")
 }
 $armed=Arm;Command host 'coop_guard_stall 1000';Start-Sleep -Milliseconds 850;$stale=Capture StaleRoot;CheckClosed $armed $stale
 if(!$stale.client.State.Bridge.remoteFresh){throw 'Expected ordinary Steam traffic to remain fresh'}
 $checks.Add('Stale guard poses invalidate the kill binding while ordinary Steam traffic remains fresh')
 $armed=Arm;Command client 'coop_damage_observe off';Start-Sleep -Milliseconds 400;$capture=Capture CaptureStopped;CheckClosed $armed $capture $true $false
 $checks.Add('Loss of native source-event capture closes both outcome leases without damage')
 $armed=Arm;Command client 'LoadGame 81';$watch=[Diagnostics.Stopwatch]::StartNew();$last=0
 do{
  $s=State client
  if($s.Bridge.localGameLogicState-eq1-and!$s.Bridge.localPendingGameLogicState-and$s.Bridge.localEpoch-ne$armed.client.State.Bridge.localEpoch){break}
  if($watch.ElapsedMilliseconds-gt6000-and$s.Bridge.localGameLogicState-in8,15-and$watch.ElapsedMilliseconds-$last-gt900){& "$PSScriptRoot/Send-D2Input.ps1" -Role client -Button A -DurationMs 200|Out-Null;$last=$watch.ElapsedMilliseconds}
  Start-Sleep -Milliseconds 150
 }while($watch.ElapsedMilliseconds-lt25000)
 if($s.Bridge.localEpoch-eq$armed.client.State.Bridge.localEpoch-or$s.Bridge.localGameLogicState-ne1){throw 'Native checkpoint reload did not complete'}
 $cat=& $probe $s.Launch.pid --world-npcs|ConvertFrom-Json;$n=@($cat.npcs|Where-Object spawnerName -eq ld_spawner_guard_escape_1);$handles.client=$n[0].handle
 $loaded=Capture Reloaded;CheckClosed $armed $loaded
 $fresh=Arm
 if($fresh.client.State.Outcome.hostArm-eq$armed.client.State.Outcome.hostArm-or$fresh.client.State.Outcome.clientArm-eq$armed.client.State.Outcome.clientArm-or$fresh.client.State.Outcome.binding-eq$armed.client.State.Outcome.binding){throw 'World replacement reused an outcome binding'}
 $checks.Add('Checkpoint replacement invalidates both outcomes; re-arming requires fresh guard and local arm tokens')
 if($GuardExpiry){
  $armed=$fresh;$watch.Restart()
  do{Start-Sleep -Milliseconds 250;$c=State client}while($c.Outcome.armed-and$watch.ElapsedMilliseconds-lt62000)
  $expired=Capture GuardExpired;CheckClosed $armed $expired
  $checks.Add('The fixed native guard lease expires without automatic renewal and closes its dependent kill outcome')
 }
 if($DisconnectHost){
  $armed=Arm;$p=Get-Process -Id $armed.host.State.Launch.pid
  if($p.Path-ne$armed.host.State.Launch.executable){throw 'Unexpected host process'};Stop-Process -Id $p.Id -Force
  if(!$p.WaitForExit(10000)){throw 'Host did not exit'}
  Start-Sleep -Milliseconds 1300;$lost=Capture HostDisconnected $false;CheckClosed $armed $lost $false
  if($lost.client.State.Bridge.remoteFresh){throw 'Lost host remains fresh'}
  $checks.Add('Actual host termination closes enemy outcome and guard ownership without killing the client guard')
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($role in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if(Get-Process -Id $l.pid -ErrorAction SilentlyContinue){try{Command $role 'coop_enemy_outcome off';Command $role 'coop_guard_link off';Command $role 'coop_damage_observe off'}catch{if(!$failure){$failure=$_.Exception.Message}}}
 }
}
[ordered]@{Passed=(!$failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;
 Scope='Armed fixed-guard enemy outcome lifetime and native no-damage checks. These cases deliberately contain no assassination.';DisconnectHost=[bool]$DisconnectHost;GuardExpiry=[bool]$GuardExpiry;Checks=$checks;Samples=$rows}|ConvertTo-Json -Depth 20|Set-Content "$PSScriptRoot/research/enemy-outcome-guards-verification.json"
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;Checks=$checks.Count}
