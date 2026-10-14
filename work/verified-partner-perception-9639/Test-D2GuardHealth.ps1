[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$probe=Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe'
$rows=[Collections.Generic.List[object]]::new();$handles=@{};$pids=@{};$failure=$null
function Command([string]$role,[string]$text){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $text -WaitConsumed|Out-Null}
function State([string]$role){
 $dir="$root/work/lab-instances/$role"
 [pscustomobject]@{Bridge=(Get-Content "$dir/coop-status.json" -Raw|ConvertFrom-Json);Guard=(Get-Content "$dir/coop-status.json.guard.json" -Raw|ConvertFrom-Json)}
}
function Resolve {
 foreach($role in 'host','client'){
  $launch=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if((Get-Process -Id $launch.pid).Path -ne $launch.executable){throw 'Unexpected lab process.'};$pids[$role]=$launch.pid
  $raw=& $probe $launch.pid --world-npcs;if($LASTEXITCODE){throw 'Native catalog unavailable'};$catalog=$raw|ConvertFrom-Json
  foreach($suffix in '1','2'){
   $npc=@($catalog.npcs|Where-Object spawnerName -eq "ld_spawner_guard_tutorial_$suffix")
   if($npc.Count -ne 1 -or !$npc[0].unique -or !$npc[0].reciprocal){throw 'Guard identity unavailable'}
   $handles["$role/$suffix"]=$npc[0].handle
  }
 }
}
function Health([string]$role,[string]$handle){
 for($n=0;$n -lt 6;$n++){$raw=& $probe $pids[$role] --health $handle;if(!$LASTEXITCODE){return ($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 25}
 throw 'Stable native health unavailable'
}
function Sample([string]$stage){
 $row=[ordered]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o')}
 foreach($role in 'host','client'){$row[$role]=[pscustomobject]@{State=(State $role);Health=(Health $role $handles["$role/1"]);Other=(Health $role $handles["$role/2"]);Player=(Health $role '1')}}
 $value=[pscustomobject]$row;$rows.Add($value);return $value
}
function AssertHealth($row,[float]$hostValue,[float]$clientValue){
 if($row.host.Health.current -ne $hostValue -or $row.client.Health.current -ne $clientValue -or
    $row.host.Health.maximum -ne 40 -or $row.client.Health.maximum -ne 40){throw "Unexpected native guard health at $($row.Stage)"}
 foreach($role in 'host','client'){
  if($row.$role.Other.current -ne $initial.$role.Other.current -or $row.$role.Player.current -ne $initial.$role.Player.current){throw 'Health change leaked into unrelated actor.'}
 }
}
try {
 Resolve
 foreach($role in 'host','client'){Command $role 'coop_guard_link off'}
 $initial=Sample Initial;AssertHealth $initial 40 40
 foreach($role in 'host','client'){if($initial.$role.State.Bridge.localGameLogicState -ne 1 -or $initial.$role.State.Guard.protocolVersion -ne 2){throw 'Guard protocol v2 in gameplay required.'}}
 Command host 'coop_guard_health 35';$unarmed=Sample Unarmed;AssertHealth $unarmed 40 40
 Command client 'coop_guard_link 8000';Command host 'coop_guard_health 35'
 $clientOnly=Sample ClientOnly;AssertHealth $clientOnly 40 40
 Command client 'coop_guard_link off'
 Command host 'coop_guard_link 8000';Command client 'coop_guard_link 8000'
 $watch=[Diagnostics.Stopwatch]::StartNew()
 do {Start-Sleep -Milliseconds 75;$h=State host;$c=State client}while(($h.Guard.phase -ne 7 -or $c.Guard.phase -ne 7) -and $watch.ElapsedMilliseconds -lt 1800)
 if($h.Guard.phase -ne 7 -or $c.Guard.phase -ne 7){throw "Guard handshake failed: $($h.Guard.result)/$($c.Guard.result)"}
 Command client 'coop_guard_health 35';$wrongRole=Sample WrongRole;AssertHealth $wrongRole 40 40
 Command host 'coop_guard_health 35';Start-Sleep -Milliseconds 400
 $applied=Sample Applied;AssertHealth $applied 35 35
 if($applied.host.State.Guard.healthStimuli -ne $initial.host.State.Guard.healthStimuli+1 -or
    $applied.client.State.Guard.healthApplied -ne $initial.client.State.Guard.healthApplied+1 -or
    $applied.host.State.Guard.healthThread -ne $applied.host.State.Bridge.engineThread -or
    $applied.client.State.Guard.healthThread -ne $applied.client.State.Bridge.engineThread){throw 'Native setter must execute once per actor on its engine command thread.'}
 Command host 'coop_guard_health 35'
 for($n=0;$n -lt 4;$n++){
  Start-Sleep -Milliseconds 200;$stable=Sample DuplicateAndRepeatedPackets;AssertHealth $stable 35 35
  if($stable.client.State.Guard.healthApplied -ne $applied.client.State.Guard.healthApplied -or $stable.host.State.Guard.healthStimuli -ne $applied.host.State.Guard.healthStimuli){throw 'Duplicate request or repeated pose packet reapplied native health.'}
 }
 & "$PSScriptRoot/Send-D2Input.ps1" -Role client -Button Start -DurationMs 200|Out-Null
 Start-Sleep -Milliseconds 600
 Command host 'coop_guard_health 35';$paused=Sample Paused;AssertHealth $paused 35 35
 if($paused.host.State.Guard.phase -or $paused.client.State.Guard.phase -or $paused.client.State.Guard.heldReferences -or $paused.client.State.Guard.frameOwned){throw 'Native pause failed to release guard ownership.'}
 & "$PSScriptRoot/Send-D2Input.ps1" -Role client -Button A -DurationMs 200|Out-Null
 Start-Sleep -Milliseconds 500
 & "$PSScriptRoot/Restore-D2Lab.ps1"|Out-Null;Resolve
 for($n=0;$n -lt 5;$n++){
  Start-Sleep -Milliseconds 250;$restored=Sample Restored;AssertHealth $restored 40 40
  if($restored.client.State.Guard.healthApplied -ne $applied.client.State.Guard.healthApplied -or $restored.client.State.Bridge.localEpoch -le $applied.client.State.Bridge.localEpoch){throw 'Old health binding survived checkpoint replacement.'}
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($role in 'host','client'){Command $role 'coop_guard_link off'}
}
[ordered]@{Passed=(!$failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;ProbeSha256=(Get-FileHash $probe).Hash;
 Scope='Explicit eight-second native Steam binding for one inactive authored guard, native nonfatal health setter 40 to 35 on host and client, independent health/identity reads, unrelated actor isolation, duplicate suppression, native pause and checkpoint lifetime reset. No damage/hit/death replication, healing, general NPC authority or combat ownership.';Samples=$rows}|ConvertTo-Json -Depth 15|Set-Content "$PSScriptRoot/research/guard-health-verification.json"
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;Samples=$rows.Count;NativeHostChanges=1;NativeClientChanges=1;UnrelatedActorsUnchanged=$true;PostCheckpointHealth=40}
