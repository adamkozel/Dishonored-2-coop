[CmdletBinding()]
param([ValidateSet('Ready','ClientPause','ClientResumed','ClientMenu','ClientCinematic','ClientChoice','ClientMovie')][string]$Stage='Ready')
$ErrorActionPreference='Stop'
$workspaceRoot=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$record=Join-Path $PSScriptRoot 'research\native-logic-guard-verification.json'
$states=@{}
foreach($role in 'host','client') {
    $root=Join-Path $workspaceRoot "work\lab-instances\$role"
    $launch=Get-Content (Join-Path $root 'launch.json') -Raw|ConvertFrom-Json
    $gameProcess=Get-Process -Id $launch.pid -ErrorAction Stop
    if($gameProcess.Path -ne $launch.executable){throw 'Unexpected process path.'}
    $file=Get-Item (Join-Path $root 'coop-status.json')
    if(([DateTime]::UtcNow-$file.LastWriteTimeUtc).TotalSeconds -gt 1.5){throw 'Stale bridge status.'}
    $status=Get-Content $file.FullName -Raw|ConvertFrom-Json
    if(!$status.remoteFresh -or $status.connectionState -ne 3){throw 'Native Steam peer unavailable.'}
    $logic=& (Join-Path $PSScriptRoot 'bin\d2_runtime_probe.exe') $launch.pid --logic
    if($LASTEXITCODE){throw 'Independent native logic inspection failed.'}
    $states[$role]=[pscustomobject]@{Status=$status;Logic=($logic|ConvertFrom-Json)}
}
foreach($role in 'host','client') {
    $other=if($role -eq 'host'){'client'}else{'host'}
    $top=$states[$role].Logic.stack[-1].id
    if($states[$role].Status.localGameLogicState -ne $top -or $states[$other].Status.remoteGameLogicState -ne $top){throw 'Native state and replicated state differ.'}
}
$hostStatus=$states.host.Status;$clientStatus=$states.client.Status
$expectedClient=switch($Stage){ClientPause {2};ClientMenu {6};ClientCinematic {5};ClientChoice {14};ClientMovie {15};default {1}}
if($clientStatus.localGameLogicState -ne $expectedClient -or $hostStatus.localGameLogicState -ne 1){throw 'Unexpected native state for this stage.'}
if($Stage -eq 'ClientMovie' -and $clientStatus.localPendingGameLogicState -ne 5){throw 'Expected movie loop with a pending native cinematic state.'}
if($Stage -in 'Ready','ClientResumed') {
    if(!$hostStatus.probeFollowEnabled -or !$clientStatus.probeFollowEnabled -or !$hostStatus.probeActorHandle -or !$clientStatus.probeActorHandle){throw 'Enable both test actors in matching gameplay first.'}
    if($Stage -eq 'ClientResumed') {
        $prior=Get-Content -LiteralPath $record -Raw|ConvertFrom-Json
        foreach($role in 'host','client') {if($states[$role].Status.probeActorHandle -ne $prior.Stages[0].States.$role.Status.probeActorHandle){throw 'Pause/resume lost the tracked clone.'}}
    }
}else {
    if($hostStatus.probeFollowEnabled -or $clientStatus.probeFollowEnabled){throw 'Follow remained active outside native gameplay.'}
    # A new explicit follow request must also be refused while the peer is
    # cinematic, choosing a character, paused, or in the main menu.
    & (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role host -Command 'coop_probe_follow on'|Out-Null
    Start-Sleep -Milliseconds 500
    $after=Get-Content (Join-Path $workspaceRoot 'work\lab-instances\host\coop-status.json') -Raw|ConvertFrom-Json
    if($after.probeFollowEnabled){throw 'Follow request incorrectly accepted outside native gameplay.'}
}
$entry=[pscustomobject]@{Stage=$Stage;ObservedAt=[DateTimeOffset]::Now.ToString('o');States=$states;Passed=$true}
if($Stage -eq 'Ready') {$entries=@($entry)}else {
    $prior=Get-Content -LiteralPath $record -Raw|ConvertFrom-Json
    $entries=@($prior.Stages|Where-Object Stage -ne $Stage)+$entry
}
[ordered]@{Stages=$entries;Scope='Independent native game-logic stack and its native Steam replication; follow stops/refuses outside gameplay and resumes with preserved clone identity after an explicit request. Does not synchronize cutscene start, time, skip, or completion.'}|
    ConvertTo-Json -Depth 12|Set-Content -LiteralPath $record
[pscustomobject]@{Stage=$Stage;HostState=$hostStatus.localGameLogicState;ClientState=$clientStatus.localGameLogicState;HostFollow=$hostStatus.probeFollowEnabled;Passed=$true}
