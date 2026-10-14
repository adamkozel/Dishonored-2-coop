[CmdletBinding()]
param([switch]$EscapeGuard)
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$probe="$PSScriptRoot/bin/d2_runtime_probe.exe"
$rows=[Collections.Generic.List[object]]::new();$pids=@{};$handles=@{};$failure=$null
$spawner=if($EscapeGuard){'ld_spawner_guard_escape_1'}else{'ld_spawner_guard_tutorial_1'}
$armCommand=if($EscapeGuard){'coop_partner_enemy escape 8000'}else{'coop_partner_enemy 8000'}
function Command([string]$role,[string]$text){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $text -WaitConsumed|Out-Null}
function State([string]$role){$dir="$root/work/lab-instances/$role";[pscustomobject]@{Bridge=(Get-Content "$dir/coop-status.json" -Raw|ConvertFrom-Json);Lease=(Get-Content "$dir/coop-status.json.partner-ai.json" -Raw|ConvertFrom-Json)}}
function Resolve {
 foreach($role in 'host','client'){
  $launch=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if((Get-Process -Id $launch.pid).Path -ne $launch.executable){throw 'Unexpected process'};$pids[$role]=$launch.pid
  $catalog=(& $probe $launch.pid --world-npcs)|ConvertFrom-Json;if($LASTEXITCODE){throw 'Catalog unavailable'}
  $guard=@($catalog.npcs|Where-Object spawnerName -eq $spawner)
  if($guard.Count-ne1-or!$guard[0].unique){throw 'Unique authored guard required'}
  $handles["$role/guard"]=$guard[0].handle;$handles["$role/partner"]=(State $role).Bridge.probeActorHandle
 }
}
function Faction([string]$role,[string]$handle){
 for($n=0;$n-lt6;$n++){$raw=& $probe $pids[$role] --faction $handle;if(!$LASTEXITCODE){return ($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 25};throw 'Stable faction unavailable'
}
function Sample([string]$stage){
 $row=[ordered]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o')}
 foreach($role in 'host','client'){$row[$role]=[pscustomobject]@{State=(State $role);Guard=(Faction $role $handles["$role/guard"]);Partner=(Faction $role $handles["$role/partner"]);Player=(Faction $role '1')}}
 $v=[pscustomobject]$row;$rows.Add($v);return $v
}
function Check($row,[bool]$active){
 foreach($role in 'host','client'){
  $target=[uint32]$handles["$role/partner"]
  foreach($kind in 'Guard','Partner','Player'){
   $before=$initial.$role.$kind
   $now=$row.$role.$kind
   if($now.name-ne$before.name-or$now.index-ne$before.index-or$now.registered-ne$before.registered){throw 'Native faction identity changed'}
   foreach($r in $now.relations){
    $old=@($before.relations|Where-Object handle -eq $r.handle);if($old.Count-ne1){throw 'Unexpected relationship target'}
    if($role-eq'host'-and$kind-eq'Guard'-and$r.handle-eq$target-and$active){
     if(!$r.overridden-or$r.overrideValue-ne2-or$r.effective-ne2-or$r.base-ne$old[0].base){throw 'Native single-actor enemy override absent'}
    }elseif(($r|ConvertTo-Json -Compress)-ne($old[0]|ConvertTo-Json -Compress)){throw "Unrelated relationship changed: $role $kind $($r.handle)"}
   }
  }
 }
}
function Begin {
 Command host $armCommand;Start-Sleep -Milliseconds 250
 $s=Sample Active;Check $s $true
 if(!$s.host.State.Lease.active-or$s.host.State.Lease.thread-ne$s.host.State.Bridge.engineThread){throw 'Lease must be active on engine thread'}
 return $s
}
try {
 foreach($role in 'host','client'){Command $role 'coop_partner_enemy off';Command $role 'coop_probe_follow on'}
 Start-Sleep -Milliseconds 500
 Resolve;$initial=Sample Initial;Check $initial $false
 Command client $armCommand;$wrong=Sample WrongRole;Check $wrong $false
 if($wrong.client.State.Lease.active-or$wrong.client.State.Lease.refused-le$initial.client.State.Lease.refused){throw 'Client authority refusal missing'}
 $armed=Begin;Start-Sleep -Milliseconds 450
 Command host $armCommand;$duplicate=Sample Duplicate;Check $duplicate $true
 if($duplicate.host.State.Lease.started-ne$armed.host.State.Lease.started-or$duplicate.host.State.Lease.remainingMs-ge$armed.host.State.Lease.remainingMs){throw 'Duplicate renewed lease'}
 $watch=[Diagnostics.Stopwatch]::StartNew()
 do{Start-Sleep -Milliseconds 300;$s=State host}while($s.Lease.active-and$watch.ElapsedMilliseconds-lt8500)
 $expired=Sample Expired;Check $expired $false
 if($expired.host.State.Lease.active-or$expired.host.State.Lease.released-ne$initial.host.State.Lease.released+1-or$expired.host.State.Lease.lost-ne$initial.host.State.Lease.lost){throw 'Expiry did not remove the native override'}
 foreach($role in 'client','host'){
  $armed=Begin
  & "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button Start -DurationMs 200|Out-Null;Start-Sleep -Milliseconds 650
  $paused=Sample "$role pause";Check $paused $false
  if($paused.$role.State.Bridge.localGameLogicState-ne2-or$paused.host.State.Lease.active-or$paused.host.State.Lease.released-ne$armed.host.State.Lease.released+1){throw 'Pause failed to remove override'}
  Command host $armCommand;$refused=Sample PausedRefusal;Check $refused $false
  & "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button A -DurationMs 200|Out-Null;Start-Sleep -Milliseconds 650
  # Native pause deliberately invalidates follow ownership. Rebind the same
  # counterpart explicitly before starting the next independent experiment.
  foreach($peer in 'host','client'){Command $peer 'coop_probe_follow on'}
  Start-Sleep -Milliseconds 350
 }
 $armed=Begin;Command host 'coop_probe_follow off';Start-Sleep -Milliseconds 350
 $off=Sample FollowOff;Check $off $false;if($off.host.State.Lease.active){throw 'Follow disable retained override'}
 Command host $armCommand;$offRefusal=Sample FollowOffRefusal;Check $offRefusal $false
 Command host 'coop_probe_follow on';Start-Sleep -Milliseconds 500
 $armed=Begin;Command host 'coop_partner_enemy off';Start-Sleep -Milliseconds 300
 $cancelled=Sample Cancelled;Check $cancelled $false
 if($cancelled.host.State.Lease.lost-ne$initial.host.State.Lease.lost){throw 'Relationship ownership lost'}
 $armed=Begin
 & "$PSScriptRoot/Restore-D2Lab.ps1"|Out-Null;Resolve
 $restored=Sample Restored
 $r=@($restored.host.Guard.relations|Where-Object handle -eq ([uint32]$handles['host/partner']))
 if($r.Count-ne1-or$r[0].overridden-or$r[0].effective-ne0-or$restored.host.State.Lease.active-or$restored.host.State.Bridge.localEpoch-le$armed.host.State.Bridge.localEpoch){throw 'Old relationship survived reload'}
}catch{$failure=$_.Exception.Message}finally{foreach($role in 'host','client'){Command $role 'coop_partner_enemy off'}}
$suffix=if($EscapeGuard){'-escape'}else{''}
[ordered]@{Passed=(!$failure);Failure=$failure;Spawner=$spawner;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;ProbeSha256=(Get-FileHash $probe).Hash;
 Scope='Host-only eight-second native override: one authored guard regards the spawned partner as enemy. Independent directional relationship reads, unrelated relation isolation, client refusal, duplicate/expiry, both pauses, follow disable, cancellation and reload. Does not establish enemy perception, combat, partner damage or network authority.';Samples=$rows}|ConvertTo-Json -Depth 18|Set-Content "$PSScriptRoot/research/partner-relationship$suffix-verification.json"
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;Samples=$rows.Count;NativeRelationshipCleanup=$true;FullCombat=$false}
