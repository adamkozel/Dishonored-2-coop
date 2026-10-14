[CmdletBinding()]
param([switch]$DisconnectHost,[switch]$LeaseExpiry)
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent;$probe="$PSScriptRoot/bin/d2_runtime_probe.exe"
$handles=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null
function Command($role,$command){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $command -WaitConsumed|Out-Null}
function State($role){
 $p="$root/work/lab-instances/$role/coop-status.json";$l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
 if((Get-Process -Id $l.pid).Path-ne$l.executable){throw 'Unexpected process'}
 if(([DateTime]::UtcNow-(Get-Item $p).LastWriteTimeUtc).TotalSeconds-gt1.5){throw 'Stale status'}
 [pscustomobject]@{Launch=$l;Bridge=(Get-Content $p -Raw|ConvertFrom-Json);Guard=(Get-Content "$p.guard.json" -Raw|ConvertFrom-Json);Melee=(Get-Content "$p.enemy-melee.json" -Raw|ConvertFrom-Json)}
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
  Command $role 'coop_enemy_melee off';Command $role 'coop_enemy_outcome off';Command $role 'coop_guard_link off';Command $role 'coop_damage_observe off';Command $role 'coop_probe_follow exact'
  $s=State $role;$cat=& $probe $s.Launch.pid --world-npcs|ConvertFrom-Json;$n=@($cat.npcs|Where-Object spawnerName -eq ld_spawner_guard_escape_1)
  if($n.Count-ne1-or!$n[0].unique-or!$n[0].reciprocal){throw 'Ambiguous guard'};$handles[$role]=$n[0].handle
  Command $role 'coop_guard_link escape 60000'
 }
 Start-Sleep -Milliseconds 500
 foreach($role in 'host','client'){Command $role 'coop_enemy_melee 15000'}
 $watch=[Diagnostics.Stopwatch]::StartNew()
 do{Start-Sleep -Milliseconds 100;$h=State host;$c=State client}while((!$h.Melee.ready-or!$c.Melee.ready)-and$watch.ElapsedMilliseconds-lt2500)
 if(!$h.Melee.ready-or!$c.Melee.ready){throw 'Melee handshake unavailable'}
 return(Capture Armed)
}
function CheckClosed($before,$after,$hostPresent=$true,$guardMustRelease=$true){
 foreach($role in 'host','client'){
  if($role-eq'host'-and!$hostPresent){continue}
  $a=$after.$role;$b=$before.$role
  if($a.State.Melee.armed-or$a.State.Melee.applied-ne$b.State.Melee.applied-or$a.State.Melee.captured-ne$b.State.Melee.captured-or$a.Health.current-ne40-or$a.Player.current-ne56){throw "Melee survived invalidation or damaged an actor: $role $($after.Stage)"}
  if($guardMustRelease){
   if($a.State.Guard.phase-or$a.State.Guard.heldReferences-or$a.State.Guard.releaseLost-ne$b.State.Guard.releaseLost){throw 'Guard AI ownership survived invalidation'}
   foreach($ai in $a.Decision,$a.Reflex){if($ai.pauseReferences-or!$ai.schedulerActive){throw 'Native AI ownership was not returned'}}
  }
 }
}
try{
 $armed=Arm
 Command client 'coop_enemy_melee 15000';Start-Sleep -Milliseconds 300;$duplicate=Capture DuplicateArm
 if(!$duplicate.client.State.Melee.armed-or$duplicate.client.State.Melee.hostArm-ne$armed.client.State.Melee.hostArm-or$duplicate.client.State.Melee.clientArm-ne$armed.client.State.Melee.clientArm-or
    $duplicate.client.State.Melee.remainingMs-ge$armed.client.State.Melee.remainingMs-or$duplicate.client.State.Melee.rejected-ne$armed.client.State.Melee.rejected+1){throw 'Duplicate arm renewed or replaced a local lease'}
 Command client 'coop_enemy_melee off';Start-Sleep -Milliseconds 400;$off=Capture LocalOff;CheckClosed $armed $off $true $false
 if($off.client.State.Guard.phase-ne7-or$off.client.State.Guard.heldReferences-ne2){throw 'Melee toggle changed independent guard ownership'}
 $checks.Add('Duplicate local arm does not renew tokens/deadline; local off closes both melee paths without stealing guard ownership')
 Command host 'coop_guard_health 35';Start-Sleep -Milliseconds 500;$passive=Capture PassiveHealthRestored
 if($passive.host.Health.current-ne35-or$passive.client.Health.current-ne35-or$passive.client.State.Melee.healthBypassed-ne$off.client.State.Melee.healthBypassed-or
    $passive.client.State.Guard.healthApplied-ne$off.client.State.Guard.healthApplied+1-or$passive.client.State.Melee.applied-ne$off.client.State.Melee.applied){throw 'Ordinary guard health ownership did not return after melee cancellation'}
 $checks.Add('After cancellation the existing native health setter resumes: host stimulus 35 reaches the client once while melee health bypass stops')
 foreach($role in 'host','client'){
  $armed=Arm
  & "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button Start -DurationMs 200|Out-Null
  Start-Sleep -Milliseconds 600;$paused=Capture "Paused-$role";CheckClosed $armed $paused
  if((State $role).Bridge.localGameLogicState-ne2){throw 'Expected native pause'}
  & "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button A -DurationMs 200|Out-Null
  Start-Sleep -Milliseconds 500;$resumed=Capture "Resumed-$role";CheckClosed $armed $resumed
  $checks.Add("$role native pause and resume leave no enemy melee authority or new damage")
 }
 $armed=Arm;Command host 'coop_guard_stall 1000';Start-Sleep -Milliseconds 850;$stale=Capture StaleRoot;CheckClosed $armed $stale
 if(!$stale.client.State.Bridge.remoteFresh){throw 'Expected ordinary Steam traffic to remain fresh'}
 $checks.Add('Stale guard poses invalidate the kill binding while ordinary Steam traffic remains fresh')
 $armed=Arm;Command client 'coop_damage_observe off';Start-Sleep -Milliseconds 400;$capture=Capture CaptureStopped;CheckClosed $armed $capture $true $false
 $checks.Add('Loss of native source-event capture closes both melee leases without damage')
 $armed=Arm;Command client 'coop_probe_follow off';Start-Sleep -Milliseconds 400;$follow=Capture FollowStopped;CheckClosed $armed $follow $true $false
 $checks.Add('Removing exact native counterpart following closes both melee leases without damage')
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
 if($fresh.client.State.Melee.hostArm-eq$armed.client.State.Melee.hostArm-or$fresh.client.State.Melee.clientArm-eq$armed.client.State.Melee.clientArm-or$fresh.client.State.Melee.binding-eq$armed.client.State.Melee.binding){throw 'World replacement reused an melee binding'}
 $checks.Add('Checkpoint replacement invalidates both melees; re-arming requires fresh guard and local arm tokens')
 if($LeaseExpiry){
  $armed=$fresh;$watch.Restart()
  do{Start-Sleep -Milliseconds 250;$c=State client}while($c.Melee.armed-and$watch.ElapsedMilliseconds-lt17000)
  $expired=Capture MeleeExpired;CheckClosed $armed $expired $true $false
  if($expired.client.State.Guard.phase-ne7-or$expired.client.State.Guard.heldReferences-ne2){throw 'Melee expiry stole independent guard ownership'}
  $checks.Add('The fixed 15-second melee lease expires without renewal while the separate native guard binding remains active')
 }
 if($DisconnectHost){
  $armed=Arm;$p=Get-Process -Id $armed.host.State.Launch.pid
  if($p.Path-ne$armed.host.State.Launch.executable){throw 'Unexpected host process'};Stop-Process -Id $p.Id -Force
  if(!$p.WaitForExit(10000)){throw 'Host did not exit'}
  Start-Sleep -Milliseconds 1300;$lost=Capture HostDisconnected $false;CheckClosed $armed $lost $false
  if($lost.client.State.Bridge.remoteFresh){throw 'Lost host remains fresh'}
  $checks.Add('Actual host termination closes enemy melee and guard ownership without killing the client guard')
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($role in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if(Get-Process -Id $l.pid -ErrorAction SilentlyContinue){try{Command $role 'coop_enemy_melee off';Command $role 'coop_guard_link off';Command $role 'coop_damage_observe off'}catch{if(!$failure){$failure=$_.Exception.Message}}}
 }
}
[ordered]@{Passed=(!$failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;
 Scope='Armed fixed-guard ordinary melee lifetime and native no-damage checks. These cases deliberately contain no assassination.';DisconnectHost=[bool]$DisconnectHost;LeaseExpiry=[bool]$LeaseExpiry;Checks=$checks;Samples=$rows}|ConvertTo-Json -Depth 20|Set-Content "$PSScriptRoot/research/enemy-melee-guards-verification.json"
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;Checks=$checks.Count}
