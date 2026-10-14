[CmdletBinding()]
param([switch]$ReloadClient,[switch]$DisconnectHost)
$ErrorActionPreference='Stop'
$workspaceRoot=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$samples=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new()
$handles=@{};$failure=$null;$maximumPoseError=0.0;$maximumBodyError=0.0;$bodyObservations=0
function State([string]$role) {
    $root=Join-Path $workspaceRoot "work/lab-instances/$role"
    $launch=Get-Content (Join-Path $root 'launch.json') -Raw|ConvertFrom-Json
    if((Get-Process -Id $launch.pid).Path -ne $launch.executable){throw 'Unexpected lab process.'}
    foreach($name in 'coop-status.json','coop-status.json.guard.json') {
        if(([DateTime]::UtcNow-(Get-Item (Join-Path $root $name)).LastWriteTimeUtc).TotalSeconds -gt 1.5){throw 'Stale engine status.'}
    }
    [pscustomobject]@{Launch=$launch;Bridge=(Get-Content (Join-Path $root 'coop-status.json') -Raw|ConvertFrom-Json);
        Guard=(Get-Content (Join-Path $root 'coop-status.json.guard.json') -Raw|ConvertFrom-Json)}
}
function Native([string]$role,[string]$option) {
    $state=State $role
    for($attempt=0;$attempt -lt 4;$attempt++) {
        $raw=& (Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe') $state.Launch.pid $option $handles[$role]
        if(!$LASTEXITCODE){return ($raw|ConvertFrom-Json)}
        if($LASTEXITCODE -ne 46){break}
        Start-Sleep -Milliseconds 5
    }
    throw "Native $role $option snapshot unavailable."
}
function Catalog([string]$role) {
    $s=State $role;$raw=& (Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe') $s.Launch.pid --world-npcs
    if($LASTEXITCODE){throw 'Native NPC catalog failed.'}
    $n=@(($raw|ConvertFrom-Json).npcs|Where-Object spawnerName -eq 'ld_spawner_guard_tutorial_1')
    if($n.Count -ne 1 -or !$n[0].unique -or !$n[0].reciprocal){throw 'Native guard identity not unique.'}
    $handles[$role]=$n[0].handle
}
function Command([string]$role,[string]$text){& (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role $role -Command $text|Out-Null}
function Button([string]$role,[string]$name){& (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -Button $name -DurationMs 200|Out-Null}
function Sample([string]$stage,[bool]$withBodies=$false) {
    $entry=[pscustomobject]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o');Host=(State host);Client=(State client);
        HostPose=(Native host --npc-pose);ClientPose=(Native client --npc-pose);Decision=(Native client --ai-decision);Reflex=(Native client --ai-reflex);
        HostDecision=(Native host --ai-decision);HostReflex=(Native host --ai-reflex);Bodies=$null}
    if($withBodies){$entry.Bodies=Native client --bones}
    $samples.Add($entry);$entry
}
function AssertReleased($sample,[bool]$allowQueuedResume=$false) {
    if($sample.Client.Guard.phase -ne 0 -or $sample.Client.Guard.heldReferences -or $sample.Client.Guard.frameOwned){throw 'Guard control did not release.'}
    foreach($a in $sample.Decision,$sample.Reflex) {
        $active=$a.schedulerActive -and $a.pauseState -eq 0
        # The native pause menu may suspend the scheduler before its queued resume
        # callback executes. References must already be zero; verify active again after Resume.
        $queued=$allowQueuedResume -and !$a.schedulerActive -and $a.pauseState -eq 3
        if(!$a.started -or $a.pauseReferences -ne 0 -or (!$active -and !$queued)){throw "Native AI reference or scheduler membership not restored: $($sample.Stage)"}
    }
}
function AssertHeld($sample) {
    if($sample.Host.Guard.phase -ne 7 -or $sample.Client.Guard.phase -ne 7 -or $sample.Client.Guard.heldReferences -ne 2 -or
       !$sample.Client.Guard.frameOwned -or $sample.Host.Guard.binding -ne $sample.Client.Guard.binding -or
       $sample.Host.Guard.request -ne $sample.Client.Guard.request -or !$sample.Client.Guard.binding){throw 'Guard handshake or ownership not established.'}
    foreach($a in $sample.Decision,$sample.Reflex){if(!$a.started -or $a.schedulerActive -or $a.pauseReferences -ne 1 -or $a.pauseState -ne 2){throw 'Both local AI components must be natively paused.'}}
    foreach($a in $sample.HostDecision,$sample.HostReflex){if(!$a.started -or !$a.schedulerActive -or $a.pauseReferences){throw 'Host AI was affected by client ownership.'}}
}
function Distance($a,$b){$sum=0.0;for($i=0;$i -lt $a.Count;$i++){$sum+=([double]$a[$i]-[double]$b[$i])*([double]$a[$i]-[double]$b[$i])};[Math]::Sqrt($sum)}
function StartLink {
    Command host 'coop_guard_link 8000';Start-Sleep -Milliseconds 200
    Command client 'coop_guard_link 8000'
    $watch=[Diagnostics.Stopwatch]::StartNew()
    do {
        Start-Sleep -Milliseconds 100;$h=State host;$c=State client
        if($h.Guard.phase -eq 7 -and $c.Guard.phase -eq 7 -and $c.Guard.frameOwned){return}
    }while($watch.ElapsedMilliseconds -lt 2200)
    throw "Guard handshake failed: host=$($h.Guard.result), client=$($c.Guard.result)."
}
function WaitGameplay([string]$role) {
    $watch=[Diagnostics.Stopwatch]::StartNew()
    do {
        $s=State $role
        if($s.Bridge.localGameLogicState -eq 1 -and !$s.Bridge.localPendingGameLogicState -and $s.Bridge.remoteFresh){return}
        if($watch.ElapsedMilliseconds -gt 6000 -and $s.Bridge.localGameLogicState -in 8,15){Button $role A}
        Start-Sleep -Milliseconds 150
    }while($watch.ElapsedMilliseconds -lt 25000)
    throw 'Native gameplay did not resume.'
}
function WaitSchedulersSettled {
    # Gameplay becomes ready before unrelated post-load AI components finish stopping.
    # Observe both registries without holding anything before taking the isolation baseline.
    $watch=[Diagnostics.Stopwatch]::StartNew();$lastKey='';$stableSince=0
    do {
        $rows=@(foreach($role in 'host','client') {
            foreach($option in '--ai-decision','--ai-reflex') {
                $a=Native $role $option
                if(!$a.started -or !$a.schedulerActive -or $a.pauseReferences -or $a.pauseState){throw 'Guard AI must be active while settling.'}
                [pscustomobject]@{Role=$role;Option=$option;Native=$a}
            }
        })
        $samples.Add([pscustomobject]@{Stage='SchedulerSettling';At=[DateTimeOffset]::Now.ToString('o');Registries=$rows})
        $key=($rows|ForEach-Object {"$($_.Role)/$($_.Option)/$($_.Native.component)/$($_.Native.liveCount)/$($_.Native.activeIndices -join ',')"}) -join ';'
        if($key -ne $lastKey){$lastKey=$key;$stableSince=$watch.ElapsedMilliseconds}
        if($watch.ElapsedMilliseconds -ge 8000 -and $watch.ElapsedMilliseconds-$stableSince -ge 2000){return}
        Start-Sleep -Milliseconds 250
    }while($watch.ElapsedMilliseconds -lt 15000)
    throw 'Native decision/reflex schedulers did not settle without guard control.'
}
try {
    foreach($role in 'host','client'){Catalog $role;Command $role 'coop_guard_link off'}
    Start-Sleep -Milliseconds 350
    WaitSchedulersSettled
    $before=Sample Before;AssertReleased $before
    if(!$before.Client.Bridge.remoteFresh -or !$before.Host.Bridge.remoteFresh){throw 'Connected lab required.'}
    Command client 'coop_guard_link 8000';Start-Sleep -Milliseconds 1900
    $unarmed=Sample UnarmedHost;AssertReleased $unarmed
    if($unarmed.Client.Guard.applied -ne $before.Client.Guard.applied -or $unarmed.Client.Guard.bindings -ne $before.Client.Guard.bindings){throw 'Unarmed host allowed mutation.'}
    $checks.Add('No binding or mutation when only client arms')

    $isolationBefore=Sample BeforeBinding;AssertReleased $isolationBefore
    StartLink;$active=Sample Active $true;AssertHeld $active
    if($active.Client.Guard.bindings -ne $before.Client.Guard.bindings+1){throw 'Unexpected binding count.'}
    foreach($name in 'Decision','Reflex') {
        $original=$isolationBefore.$name;$held=$active.$name
        if($held.component -ne $original.component -or $held.tree -ne $original.tree -or
           ($held.activeIndices -join ',') -ne (@($original.activeIndices|Where-Object {$_ -ne $original.poolIndex}) -join ',')){throw 'AI pause changed unrelated scheduler membership.'}
    }
    $hostOrigin=$active.HostPose.root[0..2]
    if((Distance $active.HostPose.root $active.ClientPose.root) -gt 0.01){throw 'Client did not align with host native root.'}
    $remaining=$active.Client.Guard.remainingMs
    Command client 'coop_guard_link 8000';Start-Sleep -Milliseconds 300
    if((State client).Guard.remainingMs -ge $remaining){throw 'Duplicate arm renewed the lease.'}
    Command host 'coop_guard_nudge 1000';Start-Sleep -Milliseconds 200
    $moved=0
    for($i=0;$i -lt 4;$i++) {
        $s=Sample HostMovement $true;AssertHeld $s
        if(!$s.HostPose.stable -or !$s.ClientPose.stable){continue}
        $delta=Distance $hostOrigin $s.HostPose.root[0..2]
        if($delta -gt 0.25) {
            $error=Distance $s.HostPose.root $s.ClientPose.root
            $renderError=Distance $s.ClientPose.root $s.ClientPose.render
            $maximumPoseError=[Math]::Max($maximumPoseError,[Math]::Max($error,$renderError))
            if($error -gt 0.015 -or $renderError -gt 0.015){throw 'Received host movement does not match native cached/rendered client transforms.'}
            ++$moved
        }
        if($s.Bodies.stable){foreach($body in $s.Bodies.bodies){$maximumBodyError=[Math]::Max($maximumBodyError,(Distance $body.position $body.expectedPosition));++$bodyObservations}}
        Start-Sleep -Milliseconds 70
    }
    if($moved -lt 2 -or $bodyObservations -lt 34 -or $maximumBodyError -gt 0.08){throw 'Insufficient movement or native physics-body agreement.'}
    Start-Sleep -Milliseconds 1200
    $returned=Sample HostMovementRestored;AssertHeld $returned
    if((Distance $hostOrigin $returned.HostPose.root[0..2]) -gt 0.02 -or (Distance $returned.HostPose.root $returned.ClientPose.root) -gt 0.015){throw 'Bounded host nudge did not restore and replicate.'}
    $checks.Add('Fresh incarnation handshake, both native AI holds, host movement, cached/rendered client pose and animated body alignment')
    $watch=[Diagnostics.Stopwatch]::StartNew()
    do {Start-Sleep -Milliseconds 200;$s=State client}while($s.Guard.phase -ne 0 -and $watch.ElapsedMilliseconds -lt 8500)
    $expired=Sample Expired;AssertReleased $expired
    if($expired.Client.Guard.released -ne $before.Client.Guard.released+2 -or $expired.Client.Guard.releaseLost -ne $before.Client.Guard.releaseLost){throw 'Lease did not release exactly the two owned references.'}
    $checks.Add('Fixed lease expiry and duplicate-arm refusal')

    StartLink;$stallBefore=Sample BeforeStall;AssertHeld $stallBefore
    Command host 'coop_guard_stall 1000';Start-Sleep -Milliseconds 850
    $stalled=Sample StreamStalled;AssertReleased $stalled
    if(!$stalled.Client.Bridge.remoteFresh -or $stalled.Client.Guard.result -ne 'pose_stale'){throw 'Guard stream expiry not distinguished from general peer freshness.'}
    $checks.Add('Guard poses expire while ordinary Steam traffic stays connected')

    foreach($pauseRole in 'client','host') {
        StartLink;AssertHeld (Sample BeforePause)
        Button $pauseRole Start;Start-Sleep -Milliseconds 600
        $paused=Sample "Paused-$pauseRole";AssertReleased $paused ($pauseRole -eq 'client')
        if($paused.Host.Guard.phase -ne 0 -or (State $pauseRole).Bridge.localGameLogicState -ne 2){throw 'Native pause did not end both links.'}
        $applied=$paused.Client.Guard.applied;Start-Sleep -Milliseconds 300
        if((State client).Guard.applied -ne $applied){throw 'Native pose mutation continued after pause.'}
        Button $pauseRole A;WaitGameplay $pauseRole;Start-Sleep -Milliseconds 300
        AssertReleased (Sample "Resumed-$pauseRole")
        $checks.Add("$pauseRole native pause releases both references and stops mutations")
    }
    if($ReloadClient) {
        StartLink;$preReload=Sample BeforeReload;AssertHeld $preReload
        Command client 'LoadGame 81';Start-Sleep -Milliseconds 1200
        $loading=State client;$samples.Add([pscustomobject]@{Stage='ReloadTransition';Client=$loading;Host=(State host)})
        if($loading.Guard.phase -ne 0 -or $loading.Guard.released -ne $preReload.Client.Guard.released+2 -or
           $loading.Guard.releaseLost -ne $preReload.Client.Guard.releaseLost){throw 'Reload did not release native controls first.'}
        WaitGameplay client;Catalog client;Start-Sleep -Milliseconds 1800
        $reloaded=Sample Reloaded;AssertReleased $reloaded
        if($reloaded.Client.Bridge.localEpoch -eq $preReload.Client.Bridge.localEpoch){throw 'Reload did not invalidate world scope.'}
        StartLink;$rebound=Sample Rebound;AssertHeld $rebound
        if($rebound.Client.Guard.binding -eq $preReload.Client.Guard.binding -or $rebound.Client.Guard.request -eq $preReload.Client.Guard.request){throw 'Reload reused stale binding or arming nonce.'}
        Command client 'coop_guard_link off';Start-Sleep -Milliseconds 500
        AssertReleased (Sample AfterRebind)
        $checks.Add('Checkpoint reload releases controls and requires new binding, nonce and world epoch')
    }
    if($DisconnectHost) {
        StartLink;$preDisconnect=Sample BeforeDisconnect;AssertHeld $preDisconnect
        $hostProcess=Get-Process -Id $preDisconnect.Host.Launch.pid
        if($hostProcess.Path -ne $preDisconnect.Host.Launch.executable){throw 'Host process changed before disconnect.'}
        & taskkill.exe /PID $hostProcess.Id /F|Out-Null
        if(!$hostProcess.WaitForExit(10000)){throw 'Host did not exit.'}
        Start-Sleep -Milliseconds 1400
        $c=State client;$d=Native client --ai-decision;$r=Native client --ai-reflex
        $samples.Add([pscustomobject]@{Stage='HostDisconnected';Client=$c;Decision=$d;Reflex=$r})
        if($c.Guard.phase -ne 0 -or $c.Guard.frameOwned -or $c.Guard.heldReferences -or $c.Bridge.remoteFresh -or
           !$d.schedulerActive -or !$r.schedulerActive -or $d.pauseReferences -or $r.pauseReferences -or
           $c.Guard.released -ne $preDisconnect.Client.Guard.released+2 -or $c.Guard.releaseLost -ne $before.Client.Guard.releaseLost){throw 'Actual host loss did not release native control.'}
        $checks.Add('Actual host termination releases native AI and frame ownership')
    }
}catch{$failure=$_.Exception.Message}finally{
    foreach($role in 'host','client') {
        $launch=Get-Content (Join-Path $workspaceRoot "work/lab-instances/$role/launch.json") -Raw|ConvertFrom-Json
        if(Get-Process -Id $launch.pid -ErrorAction SilentlyContinue){Command $role 'coop_guard_link off'; & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -DurationMs 100|Out-Null}
    }
}
[ordered]@{Passed=($null -eq $failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');
    DllSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/dinput8.dll')).Hash;ProbeSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe')).Hash;
    ReloadClient=[bool]$ReloadClient;DisconnectHost=[bool]$DisconnectHost;Checks=$checks.ToArray();MaxPoseError=$maximumPoseError;
    MaxAnimatedBodyError=$maximumBodyError;BodyObservations=$bodyObservations;Samples=$samples.ToArray();
    Scope='Eight-second, explicitly armed, host-to-client pose transfer for one authored guard with native decision/reflex pause. Host movement stimulus is a bounded native transform nudge. Combat must stay inactive. Does not establish combat/damage/quest authority, arbitrary NPC support, or complete remote animation.'}|
    ConvertTo-Json -Depth 16|Set-Content (Join-Path $PSScriptRoot 'research/guard-link-verification.json')
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;Checks=$checks.Count;MaxPoseError=$maximumPoseError;MaxAnimatedBodyError=$maximumBodyError;BodyObservations=$bodyObservations}
