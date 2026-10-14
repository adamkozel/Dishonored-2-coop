[CmdletBinding()]
param([switch]$ReloadClient)
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$observations=[Collections.Generic.List[object]]::new();$known=@{};$matched=[Collections.Generic.HashSet[uint64]]::new()
$failure=$null;$reload=$null;$pause=$null;$maxPoseError=0.0
$healthPackets=0;$healthChecks=[Collections.Generic.List[object]]::new()
function State([string]$role){
    $dir=Join-Path $root "work/lab-instances/$role"
    $launch=Get-Content (Join-Path $dir 'launch.json') -Raw|ConvertFrom-Json
    if((Get-Process -Id $launch.pid).Path -ne $launch.executable){throw 'Unexpected lab process.'}
    foreach($name in 'coop-status.json','coop-status.json.world.json'){
        if(([DateTime]::UtcNow-(Get-Item (Join-Path $dir $name)).LastWriteTimeUtc).TotalSeconds -gt 1.5){throw 'Stale status.'}
    }
    [pscustomobject]@{Launch=$launch;Status=(Get-Content (Join-Path $dir 'coop-status.json') -Raw|ConvertFrom-Json);World=(Get-Content (Join-Path $dir 'coop-status.json.world.json') -Raw|ConvertFrom-Json)}
}
function Catalog([string]$role){
    $s=State $role;$raw=& (Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe') $s.Launch.pid --world-npcs
    if($LASTEXITCODE){throw 'Independent world catalog read failed.'};$raw|ConvertFrom-Json
}
function Health([string]$role,[uint32]$handle){
    $s=State $role
    for($attempt=0;$attempt -lt 6;$attempt++){
        $raw=& (Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe') $s.Launch.pid --health $handle
        if(!$LASTEXITCODE){return ($raw|ConvertFrom-Json)}
        if($LASTEXITCODE -ne 54){throw 'Independent native health read failed.'}
        Start-Sleep -Milliseconds 40
    }
    throw 'Native health remained unavailable.'
}
function Pair([string]$stage){
    $p=[pscustomobject]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o');Host=(State host);Client=(State client)}
    $observations.Add($p);$p
}
try{
    foreach($role in 'host','client'){
        & (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role $role -Command 'coop_checkpoint_sync off'|Out-Null
        & (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role $role -Command 'coop_world_observe on'|Out-Null
    }
    Start-Sleep -Milliseconds 1800
    $before=Pair Ready
    foreach($s in $before.Host,$before.Client){if($s.Status.localGameLogicState -ne 1 -or !$s.Status.remoteFresh -or !$s.World.enabled -or !$s.World.observationOnly -or $s.World.entries.Count -lt 40){throw 'Connected gameplay with at least 40 native NPC identities required.'}}
    foreach($s in $before.Host,$before.Client){if($s.World.protocolVersion -ne 2 -or $s.World.packetBytes -ne 516){throw 'World health protocol v2 required.'}}
    $nativeHost=Catalog host;$nativeClient=Catalog client
    $nativeHostByName=@{};foreach($n in $nativeHost.npcs){if($n.unique){$nativeHostByName[$n.spawnerName]=$n}}
    $nativeByName=@{};foreach($n in $nativeClient.npcs){if($n.unique){$nativeByName[$n.spawnerName]=$n}}
    for($i=0;$i -lt 20;$i++){
        $p=Pair Transfer
        foreach($e in $p.Host.World.entries){
            $n=$nativeHostByName[$e.spawner]
            if(!$n -or $e.definition -ne $n.definition -or $e.sourceHandle -ne [Convert]::ToUInt32($n.handle.Substring(2),16)){throw 'Host source identity disagrees with independent reader.'}
            $known[[uint64]$e.sequence]=$e
        }
        foreach($e in $p.Client.World.entries){
            if(!$nativeByName.ContainsKey($e.spawner)){throw 'Received an unbound NPC identity.'}
            $n=$nativeByName[$e.spawner]
            if($e.definition -ne $n.definition -or $e.localHandle -ne [Convert]::ToUInt32($n.handle.Substring(2),16)){throw 'Native identity mapping disagrees with independent reader.'}
            if(!$known.ContainsKey([uint64]$e.sequence)){continue}
            $source=$known[[uint64]$e.sequence]
            foreach($field in 'sourceSession','targetSession','sourceEpoch','targetEpoch','mapId','spawner','definition','sourceHandle','healthValid','health','maximumHealth'){
                if($source.$field -cne $e.$field){throw "Transport metadata mismatch: $field"}
            }
            if($e.healthValid){$healthPackets++}
            for($j=0;$j -lt 3;$j++){$maxPoseError=[Math]::Max($maxPoseError,[Math]::Abs($source.position[$j]-$e.position[$j]))}
            for($j=0;$j -lt 9;$j++){$maxPoseError=[Math]::Max($maxPoseError,[Math]::Abs($source.orientation[$j]-$e.orientation[$j]))}
            [void]$matched.Add([uint64]$e.sequence)
        }
        Start-Sleep -Milliseconds 150
    }
    if($matched.Count -lt 100 -or $maxPoseError -gt 0.000001){throw 'Insufficient or mismatched packet observations.'}
    if($healthPackets -lt 100){throw 'Insufficient valid health packet observations.'}
    foreach($entry in @($p.Client.World.entries|Where-Object healthValid|Select-Object -First 12)){
        $a=Health host $entry.sourceHandle
        Start-Sleep -Milliseconds 600
        $fresh=Pair Health
        $wire=@($fresh.Client.World.entries|Where-Object {$_.spawner -ceq $entry.spawner})
        $b=Health host $entry.sourceHandle
        if($wire.Count -ne 1 -or !$wire[0].healthValid -or $a.current -ne $b.current -or $a.maximum -ne $b.maximum -or
           $wire[0].health -ne $b.current -or $wire[0].maximumHealth -ne $b.maximum){throw 'Health transport disagrees with stable independent native reads.'}
        $local=Health client $wire[0].localHandle
        $healthChecks.Add([pscustomobject]@{Spawner=$entry.spawner;Before=$a;Received=$wire[0];After=$b;LocalClient=$local})
    }
    if($healthChecks.Count -lt 12){throw 'Insufficient independent NPC health checks.'}
    if($p.Client.World.rejected -ne $before.Client.World.rejected -or $p.Client.World.unmatched -ne $before.Client.World.unmatched -or $p.Client.World.sent -ne 0){throw 'Unexpected rejection, unmapped NPC, or client authority during stable gameplay.'}
    & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role client -Button Start -DurationMs 200|Out-Null
    Start-Sleep -Milliseconds 1200;$pause=Pair Paused
    if($pause.Client.Status.localGameLogicState -ne 2 -or $pause.Host.World.entries.Count -or $pause.Client.World.entries.Count){throw 'Pause did not clear the world observation scope.'}
    Start-Sleep -Milliseconds 700;$stable=Pair PausedStable
    if($stable.Host.World.sent -ne $pause.Host.World.sent -or $stable.Client.World.matched -ne $pause.Client.World.matched){throw 'World observations continued during pause.'}
    & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role client -Button A -DurationMs 200|Out-Null
    Start-Sleep -Milliseconds 1500;$resumed=Pair Resumed
    if($resumed.Client.World.entries.Count -lt 40 -or $resumed.Client.World.matched -le $stable.Client.World.matched){throw 'Valid observations did not resume.'}
    if($ReloadClient){
        $priorEpoch=$resumed.Client.Status.localEpoch
        & (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role client -Command 'LoadGame 81'|Out-Null
        $watch=[Diagnostics.Stopwatch]::StartNew();$sawEmpty=$false;$seenChange=$false
        do{
            Start-Sleep -Milliseconds 200;$p=Pair Reloading
            if(!$p.Client.World.entries.Count){$sawEmpty=$true}
            if($p.Client.Status.localEpoch -gt $priorEpoch){$seenChange=$true}
            if($seenChange -and $p.Client.Status.localGameLogicState -eq 1 -and $p.Client.World.entries.Count -ge 40){break}
            # Native arkGameLogicStateMapTransition (15) waits for the
            # controller's continue action after checkpoint data is ready.
            if($watch.ElapsedMilliseconds -gt 6000 -and $p.Client.Status.localGameLogicState -in 8,15){
                & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role client -Button A -DurationMs 200|Out-Null
            }
        }while($watch.ElapsedMilliseconds -lt 25000)
        $reload=Pair Reloaded
        if(!$sawEmpty -or !$seenChange -or $reload.Client.Status.localGameLogicState -ne 1 -or $reload.Client.World.entries.Count -lt 40){throw 'Client reload did not invalidate and rebuild world observations.'}
        foreach($e in $reload.Client.World.entries){if($e.targetEpoch -ne $reload.Client.Status.localEpoch){throw 'A prior-world NPC observation survived reload.'}}
        if(!$reload.Client.Status.probeActorHandle){& (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role client -Command 'coop_spawn_probe corvo'|Out-Null;Start-Sleep -Milliseconds 800}
    }
    foreach($role in 'host','client'){
        & (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role $role -Command 'coop_probe_follow on'|Out-Null
        Start-Sleep -Milliseconds 150
        & (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role $role -Command 'coop_probe_locomotion on'|Out-Null
    }
}catch{$failure=$_.Exception.Message}finally{
    foreach($role in 'host','client'){& (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -DurationMs 100|Out-Null}
}
$record=[ordered]@{Passed=($null -eq $failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/dinput8.dll')).Hash;ProbeSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe')).Hash;MatchedPackets=$matched.Count;HealthPackets=$healthPackets;HealthChecks=$healthChecks;MaxPoseError=$maxPoseError;ReloadClient=[bool]$ReloadClient;NativeHost=$nativeHost;NativeClient=$nativeClient;Samples=$observations;Scope='Read-only native NPC identity, pose and health transport through Steam, with independent native health reads, native pause and optional client checkpoint reload. Does not establish NPC authority, damage/death application, combat, interactions or birth/despawn replication.'}
$record|ConvertTo-Json -Depth 14|Set-Content (Join-Path $PSScriptRoot 'research/world-observation-verification.json')
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;MatchedPackets=$matched.Count;HealthPackets=$healthPackets;HealthChecks=$healthChecks.Count;MaxPoseError=$maxPoseError;ReloadClient=[bool]$ReloadClient}
