[CmdletBinding()]
param([ValidateSet('host','client')][string]$SourceRole='host',
      [ValidateSet('Choice','Gameplay','Segment')][string]$Destination='Choice')
$ErrorActionPreference='Stop'
$workspaceRoot=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$targetRole=if($SourceRole -eq 'host'){'client'}else{'host'}
function Read-State([string]$role) {
    $root=Join-Path $workspaceRoot "work/lab-instances/$role"
    $launch=Get-Content (Join-Path $root 'launch.json') -Raw|ConvertFrom-Json
    $gameProcess=Get-Process -Id $launch.pid -ErrorAction Stop
    if($gameProcess.Path -ne $launch.executable){throw 'Unexpected game process.'}
    $statusFile=Get-Item (Join-Path $root 'coop-status.json')
    if(([DateTime]::UtcNow-$statusFile.LastWriteTimeUtc).TotalSeconds -gt 1.5){throw 'Stale game status.'}
    $status=Get-Content $statusFile.FullName -Raw|ConvertFrom-Json
    if(!$status.remoteFresh -or $status.connectionState -ne 3){throw 'Steam peer is unavailable.'}
    return $status
}
$before=@{host=(Read-State host);client=(Read-State client)}
foreach($role in 'host','client') {
    $s=$before[$role]
    if($s.localGameLogicState -ne 5 -or $s.localPendingGameLogicState -or !$s.cinematicSyncEnabled -or
       $s.localCinematicCanSkip -ne 1 -or $s.remoteCinematicCanSkip -ne 1 -or $s.remoteCinematicSyncEnabled -ne 1 -or
       $s.localSkipSceneId -eq '0' -or $s.localSkipSceneId -ne $s.remoteSkipSceneId -or
       $s.poseProtocol -notin @(6,7,8,9,10) -or $s.localSequenceMetadataValid -ne 1 -or $s.remoteSequenceMetadataValid -ne 1 -or
       $s.localSequenceBoundaryId -eq '0' -or $s.localSequenceBoundaryId -ne $s.remoteSequenceBoundaryId){throw 'Both instances must have matching native skippable segments and an enabled relay.'}
}
$inputPath=Join-Path $workspaceRoot "work/lab-instances/$targetRole/input.bin"
$targetInputHash=(Get-FileHash $inputPath).Hash
$timeline=[Collections.Generic.List[object]]::new()
& (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $SourceRole -Button X -DurationMs 3000|Out-Null
$expected=if($Destination -eq 'Choice'){14}elseif($Destination -eq 'Gameplay'){1}else{5}
$watch=[Diagnostics.Stopwatch]::StartNew()
function Destination-Reached($state) {
    if($state.host.localGameLogicState -ne $expected -or $state.client.localGameLogicState -ne $expected){return $false}
    if($Destination -ne 'Segment'){return $true}
    return $state.host.localSequenceMetadataValid -eq 1 -and $state.client.localSequenceMetadataValid -eq 1 -and
        $state.host.localSequenceBoundaryId -ne '0' -and $state.host.localSequenceBoundaryId -eq $state.client.localSequenceBoundaryId -and
        $state.host.localSequenceBoundaryId -ne $before.host.localSequenceBoundaryId -and
        $state.client.localSequenceBoundaryId -ne $before.client.localSequenceBoundaryId
}
$failure=$null
try { do {
    Start-Sleep -Milliseconds 100
    $after=@{host=(Read-State host);client=(Read-State client)}
    $timeline.Add([pscustomobject]@{Ms=$watch.ElapsedMilliseconds;HostState=$after.host.localGameLogicState;ClientState=$after.client.localGameLogicState;HostSegment=$after.host.localSequenceBoundaryId;ClientSegment=$after.client.localSequenceBoundaryId;HostEpoch=$after.host.localCinematicEpoch;ClientEpoch=$after.client.localCinematicEpoch;SourceCallbacks=$after[$SourceRole].nativeSkipCallbacks;Sent=$after[$SourceRole].skipSent;Applied=$after[$targetRole].skipApplied})
}while($watch.ElapsedMilliseconds -lt 15000 -and !(Destination-Reached $after))
if(!(Destination-Reached $after)){throw 'Native shared skip did not reach its destination in both instances.'}
if($after[$SourceRole].nativeSkipCallbacks -ne $before[$SourceRole].nativeSkipCallbacks+1 -or
   $after[$SourceRole].skipSent -ne $before[$SourceRole].skipSent+1 -or
   $after[$targetRole].skipApplied -ne $before[$targetRole].skipApplied+1 -or
   $after[$targetRole].nativeSkipCallbacks -ne $before[$targetRole].nativeSkipCallbacks -or
   $after[$targetRole].skipSent -ne $before[$targetRole].skipSent -or
   $after[$targetRole].skipDropped -ne $before[$targetRole].skipDropped){throw 'Expected exactly one native UI callback, one reliable message, one peer application, and no echo or dropped request.'}
if((Get-FileHash $inputPath).Hash -ne $targetInputHash){throw 'Peer input changed during the shared-skip test.'}
} catch {$failure=$_.Exception.Message}
$entry=[pscustomobject]@{Passed=($null -eq $failure);Failure=$failure;PoseProtocol=$before[$SourceRole].poseProtocol;DllSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/dinput8.dll')).Hash;SourceRole=$SourceRole;Destination=$Destination;ObservedAt=[DateTimeOffset]::Now.ToString('o');ElapsedMs=$watch.ElapsedMilliseconds;PeerInputUnchanged=((Get-FileHash $inputPath).Hash -eq $targetInputHash);Before=$before;After=$after;Timeline=$timeline.ToArray()}
$record=Join-Path $PSScriptRoot 'research/native-shared-skip-verification.json'
$entries=if(Test-Path $record){@((Get-Content $record -Raw|ConvertFrom-Json).Tests)}else{@()}
[ordered]@{LatestTestPassed=$entry.Passed;Tests=@($entries)+$entry;Scope='An existing native UI skip confirmation relayed reliably through Steam to the matching peer native callback. This does not synchronize scene entry, playback time, or world authority. Historical entries retain their individual build hashes; an entry without a hash predates per-test build attribution.'}|ConvertTo-Json -Depth 12|Set-Content $record
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;SourceRole=$SourceRole;Destination=$Destination;ElapsedMs=$watch.ElapsedMilliseconds;PeerInputUnchanged=$true}



