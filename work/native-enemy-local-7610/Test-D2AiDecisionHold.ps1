[CmdletBinding()]
param([switch]$ReloadClient,[switch]$DisconnectHost)
$ErrorActionPreference='Stop'
$workspaceRoot=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$samples=[Collections.Generic.List[object]]::new()
$checks=[Collections.Generic.List[string]]::new()
$handles=@{};$failure=$null
function State([string]$role) {
    $root=Join-Path $workspaceRoot "work/lab-instances/$role"
    $launch=Get-Content (Join-Path $root 'launch.json') -Raw|ConvertFrom-Json
    if((Get-Process -Id $launch.pid).Path -ne $launch.executable){throw 'Unexpected lab process.'}
    $file=Get-Item (Join-Path $root 'coop-status.json')
    if(([DateTime]::UtcNow-$file.LastWriteTimeUtc).TotalSeconds -gt 1.5){throw 'Stale engine status.'}
    Get-Content $file.FullName -Raw|ConvertFrom-Json
}
function CatalogGuard([string]$role) {
    $launch=Get-Content (Join-Path $workspaceRoot "work/lab-instances/$role/launch.json") -Raw|ConvertFrom-Json
    $native=& (Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe') $launch.pid --world-npcs
    if($LASTEXITCODE){throw 'Independent NPC catalog unavailable.'}
    $guard=@(($native|ConvertFrom-Json).npcs|Where-Object spawnerName -eq 'ld_spawner_guard_tutorial_1')
    if($guard.Count -ne 1 -or !$guard[0].unique -or !$guard[0].reciprocal){throw 'Guard identity is not unique and reciprocal.'}
    $handles[$role]=$guard[0].handle
}
function Snapshot([string]$role,[string]$stage) {
    $s=State $role
    $launch=Get-Content (Join-Path $workspaceRoot "work/lab-instances/$role/launch.json") -Raw|ConvertFrom-Json
    $native=& (Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe') $launch.pid --ai-decision $handles[$role]
    if($LASTEXITCODE){throw 'Independent AI scheduler snapshot unavailable or changing.'}
    $entry=[pscustomobject]@{Stage=$stage;Role=$role;At=[DateTimeOffset]::Now.ToString('o');Status=$s;Native=($native|ConvertFrom-Json)}
    $samples.Add($entry);$entry
}
function Command([string]$role,[string]$text) { & (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role $role -Command $text|Out-Null }
function Button([string]$role,[string]$name) { & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -Button $name -DurationMs 200|Out-Null }
function Active($sample) {
    if(!$sample.Native.started -or !$sample.Native.schedulerActive -or $sample.Native.pauseReferences -ne 0 -or $sample.Native.pauseState -ne 0 -or $sample.Status.aiHoldActive){throw "AI did not return to the active scheduler: $($sample.Stage)"}
}
function ReleasedForMenu($sample,[bool]$localMenu) {
    $a=$sample.Native
    $active=$a.schedulerActive -and $a.pauseState -eq 0
    $queued=$localMenu -and !$a.schedulerActive -and $a.pauseState -eq 3
    if(!$a.started -or $a.pauseReferences -ne 0 -or $sample.Status.aiHoldActive -or (!$active -and !$queued)){throw "AI reference was not released for native menu: $($sample.Stage)"}
}
function Held($sample,$before,[bool]$unchangedOtherMembers=$true) {
    $a=$sample.Native;$b=$before.Native
    $remaining=@($b.activeIndices|Where-Object {$_ -ne $b.poolIndex})
    if(!$sample.Status.aiHoldActive -or !$a.started -or $a.schedulerActive -or $a.pauseReferences -ne 1 -or $a.pauseState -ne 2 -or
       $a.component -ne $b.component -or $a.actor -ne $b.actor -or $a.tree -ne $b.tree -or $a.poolIndex -ne $b.poolIndex -or
       ($unchangedOtherMembers -and ($a.liveCount -ne $b.liveCount -or ($a.activeIndices -join ',') -ne ($remaining -join ',')))){throw "Pause affected the wrong scheduler membership: $($sample.Stage)"}
}
function WaitGameplay([string]$role) {
    $watch=[Diagnostics.Stopwatch]::StartNew()
    do {
        $s=State $role
        if($s.localGameLogicState -eq 1 -and $s.localPendingGameLogicState -eq 0 -and $s.remoteFresh){return}
        if($watch.ElapsedMilliseconds -gt 6000 -and $s.localGameLogicState -in 8,15){Button $role A}
        Start-Sleep -Milliseconds 150
    }while($watch.ElapsedMilliseconds -lt 25000)
    throw 'Native gameplay did not become ready.'
}
function StableScheduler([string]$role) {
    $watch=[Diagnostics.Stopwatch]::StartNew();$lastKey='';$stableSince=0
    do {
        $s=Snapshot $role SchedulerSettling;Active $s
        $key="$($s.Native.component)/$($s.Native.liveCount)/$($s.Native.activeIndices -join ',')"
        if($key -ne $lastKey){$lastKey=$key;$stableSince=$watch.ElapsedMilliseconds}
        if($watch.ElapsedMilliseconds -ge 1500 -and $watch.ElapsedMilliseconds-$stableSince -ge 1000){return $s}
        Start-Sleep -Milliseconds 200
    }while($watch.ElapsedMilliseconds -lt 8000)
    throw 'Native scheduler did not settle after loading.'
}
try {
    foreach($role in 'host','client') {
        $s=State $role
        if(!$s.remoteFresh -or $s.connectionState -ne 3 -or $s.localGameLogicState -ne 1 -or $s.localPendingGameLogicState -ne 0 -or
           $s.localMap -ne 'campaign/dunwall/escape/tower/dunwall_escape_tower_p'){throw 'Connected Dunwall gameplay required.'}
        CatalogGuard $role
    }
    $before=StableScheduler client;$hostBefore=StableScheduler host
    Active $before;Active $hostBefore
    Command host 'coop_world_hold_ai 2000';Start-Sleep -Milliseconds 350
    $hostRefused=Snapshot host WrongRole;Active $hostRefused
    if($hostRefused.Status.aiHoldRefused -ne $hostBefore.Status.aiHoldRefused+1){throw 'Host-side hold was not refused.'}
    $checks.Add('Host mutation refused')

    Command client 'coop_world_hold_ai 2000'
    $watch=[Diagnostics.Stopwatch]::StartNew();$duplicate=$false;$heldSamples=0
    do {
        Start-Sleep -Milliseconds 100
        $s=Snapshot client TimedHold
        $elapsed=$watch.ElapsedMilliseconds
        if($elapsed -ge 350 -and $elapsed -lt 1700){Held $s $before;++$heldSamples}
        $peer=Snapshot host HostDuringClientHold;Active $peer
        if(($peer.Native.activeIndices -join ',') -ne ($hostBefore.Native.activeIndices -join ',')){throw 'Peer scheduler changed during client hold.'}
        if(!$duplicate -and $elapsed -ge 500){Command client 'coop_world_hold_ai 2000';$duplicate=$true}
    }while($watch.ElapsedMilliseconds -lt 2500)
    $after=Snapshot client AfterTimedHold;Active $after
    if($heldSamples -lt 5 -or $after.Native.component -ne $before.Native.component -or $after.Native.tree -ne $before.Native.tree -or
       ($after.Native.activeIndices -join ',') -ne ($before.Native.activeIndices -join ',') -or
       $after.Status.aiHoldStarted -ne $before.Status.aiHoldStarted+1 -or $after.Status.aiHoldReleased -ne $before.Status.aiHoldReleased+1 -or
       $after.Status.aiHoldRefused -ne $before.Status.aiHoldRefused+1 -or $after.Status.aiHoldResult -ne 'expired' -or
       $after.Status.sent -le $before.Status.sent -or $after.Status.received -le $before.Status.received){throw 'Timed release, duplicate refusal or live Steam traffic failed.'}
    $checks.Add('Only one client decision component removed and restored; duplicate did not renew; host and Steam continued')

    Command client 'coop_world_hold_ai 2000';Start-Sleep -Milliseconds 350
    Held (Snapshot client BeforeCancel) $after
    Command client 'coop_world_hold_ai off';Start-Sleep -Milliseconds 350
    $cancel=Snapshot client Cancelled;Active $cancel
    if($cancel.Status.aiHoldResult -ne 'cancelled'){throw 'Explicit cancellation did not release the reference.'}
    $checks.Add('Explicit cancellation')

    foreach($pauseRole in 'client','host') {
        Command client 'coop_world_hold_ai 2000';Start-Sleep -Milliseconds 350
        Held (Snapshot client BeforePause) $cancel
        Button $pauseRole Start;Start-Sleep -Milliseconds 450
        if((State $pauseRole).localGameLogicState -ne 2){throw 'Native pause not reached.'}
        $paused=Snapshot client "ReleasedFor-$pauseRole-Pause";ReleasedForMenu $paused ($pauseRole -eq 'client')
        if($paused.Status.aiHoldResult -ne 'scope_changed'){throw 'Phase change did not release the owned pause reference.'}
        Command client 'coop_world_hold_ai 2000';Start-Sleep -Milliseconds 300
        $refused=Snapshot client RefusedDuringPause;ReleasedForMenu $refused ($pauseRole -eq 'client')
        if($refused.Status.aiHoldRefused -ne $paused.Status.aiHoldRefused+1){throw 'Paused pair accepted a new hold.'}
        Button $pauseRole A;WaitGameplay $pauseRole;Start-Sleep -Milliseconds 250
        Active (Snapshot client "ResumedAfter-$pauseRole-Pause")
        $checks.Add("$pauseRole native pause releases hold and refuses reacquisition")
    }

    if($ReloadClient) {
        $preReload=Snapshot client BeforeReload
        Command client 'coop_world_hold_ai 2000';Start-Sleep -Milliseconds 350
        Held (Snapshot client HeldBeforeReload) $preReload
        Command client 'LoadGame 81';Start-Sleep -Milliseconds 1000
        $loading=State client
        $samples.Add([pscustomobject]@{Stage='ReloadTransition';Role='client';Status=$loading})
        if($loading.aiHoldActive -or $loading.aiHoldReleased -ne $preReload.Status.aiHoldReleased+1 -or $loading.aiHoldResult -ne 'load_requested'){throw 'Reload did not release the live component first.'}
        WaitGameplay client;CatalogGuard client
        $reloaded=StableScheduler client
        if($reloaded.Status.localEpoch -eq $preReload.Status.localEpoch){throw 'Reload did not establish a new world epoch.'}
        Command client 'coop_world_hold_ai 2000';Start-Sleep -Milliseconds 350
        # Only the targeted component is an invariant across a freshly loaded
        # mission. Other NPCs enter/leave their native LOD/story update lists.
        # Exact whole-list isolation is checked separately in the timed test.
        Held (Snapshot client ReboundAfterReload) $reloaded $false
        Command client 'coop_world_hold_ai off';Start-Sleep -Milliseconds 350
        Active (Snapshot client RestoredAfterReload)
        $checks.Add('Native checkpoint reload releases first and permits fresh identity binding')
    }

    if($DisconnectHost) {
        $preDisconnect=Snapshot client BeforeDisconnect
        Command client 'coop_world_hold_ai 2000';Start-Sleep -Milliseconds 350
        Held (Snapshot client HeldBeforeDisconnect) $preDisconnect $false
        $launch=Get-Content (Join-Path $workspaceRoot 'work/lab-instances/host/launch.json') -Raw|ConvertFrom-Json
        $hostProcess=Get-Process -Id $launch.pid
        if($hostProcess.Path -ne $launch.executable){throw 'Unexpected host process before disconnect.'}
        & taskkill.exe /PID $hostProcess.Id /F|Out-Null
        if(!$hostProcess.WaitForExit(10000)){throw 'Host did not exit.'}
        Start-Sleep -Milliseconds 1200
        $disconnected=Snapshot client HostDisconnected;Active $disconnected
        if($disconnected.Status.remoteFresh -or $disconnected.Status.aiHoldResult -ne 'scope_changed'){throw 'Host loss did not release the client hold.'}
        Command client 'coop_world_hold_ai 2000';Start-Sleep -Milliseconds 350
        $refused=Snapshot client RefusedDisconnected;Active $refused
        if($refused.Status.aiHoldRefused -ne $disconnected.Status.aiHoldRefused+1){throw 'Disconnected hold was not refused.'}
        $checks.Add('Actual host termination releases the client hold and prevents reacquisition')
    }
    $final=Snapshot client Final;Active $final
    if($final.Status.aiHoldInvalidated -ne $before.Status.aiHoldInvalidated){throw 'A hold lost its identity without verified release.'}
}catch{$failure=$_.Exception.Message}finally{
    foreach($role in 'host','client') {
        $launch=Get-Content (Join-Path $workspaceRoot "work/lab-instances/$role/launch.json") -Raw|ConvertFrom-Json
        if(Get-Process -Id $launch.pid -ErrorAction SilentlyContinue) {
            Command $role 'coop_world_hold_ai off'
            & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -DurationMs 100|Out-Null
        }
    }
}
$record=[ordered]@{Passed=($null -eq $failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');
    DllSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/dinput8.dll')).Hash;ProbeSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe')).Hash;
    ReloadClient=[bool]$ReloadClient;DisconnectHost=[bool]$DisconnectHost;Checks=$checks.ToArray();Samples=$samples.ToArray();
    Scope='Native AIDecision reference-counted pause and independent scheduler membership only. Does not establish suspension of reflexes, combat, movement, scripts, or complete AI authority.'}
$record|ConvertTo-Json -Depth 14|Set-Content (Join-Path $PSScriptRoot 'research/ai-decision-hold-verification.json')
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;Checks=$checks.Count;HeldSamples=$heldSamples;ReloadClient=[bool]$ReloadClient;DisconnectHost=[bool]$DisconnectHost}
