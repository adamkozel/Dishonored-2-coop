[CmdletBinding()]
param([ValidateSet('host','client')][string]$Role='host')
$ErrorActionPreference='Stop'
$workspaceRoot=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$peerRole=if($Role -eq 'host'){'client'}else{'host'}
function Read-Sample([string]$role) {
    $root=Join-Path $workspaceRoot "work/lab-instances/$role"
    $launch=Get-Content (Join-Path $root 'launch.json') -Raw|ConvertFrom-Json
    if((Get-Process -Id $launch.pid).Path -ne $launch.executable){throw 'Unexpected game process.'}
    $file=Get-Item (Join-Path $root 'coop-status.json')
    if(([DateTime]::UtcNow-$file.LastWriteTimeUtc).TotalSeconds -gt 1.5){throw 'Stale game status.'}
    $status=Get-Content $file.FullName -Raw|ConvertFrom-Json
    if(!$status.remoteFresh -or $status.connectionState -ne 3){throw 'Native Steam peer unavailable.'}
    $native=& (Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe') $launch.pid --midnight
    if($LASTEXITCODE){throw 'Independent sequence read failed.'}
    $native=$native|ConvertFrom-Json
    $sequences=@($native.instances|Where-Object playerChannel -ge 0)
    if($sequences.Count -ne 1){throw 'Need exactly one player cinematic sequence.'}
    return [pscustomobject]@{Status=$status;Sequence=$sequences[0];Channels=$native.channels}
}
$before=Read-Sample $Role;$peerBefore=Read-Sample $peerRole
if($before.Status.localGameLogicState -ne 5 -or $before.Status.localCinematicCanSkip -ne 1 -or
   $before.Sequence.state -ne 3 -or $before.Sequence.sceneDurationMs - $before.Sequence.clockWords[1] -lt 5000){throw 'Need a stable playing cinematic with enough time remaining.'}
$timeline=[Collections.Generic.List[object]]::new();$failure=$null
& (Join-Path $PSScriptRoot 'Send-D2Command.ps1') -Role $Role -Command 'coop_cinematic_hold 2000'|Out-Null
$watch=[Diagnostics.Stopwatch]::StartNew()
try {
    do {
        Start-Sleep -Milliseconds 100
        $sample=Read-Sample $Role
        $timeline.Add([pscustomobject]@{Ms=$watch.ElapsedMilliseconds;Sample=$sample})
    }while($watch.ElapsedMilliseconds -lt 3300)
    $after=Read-Sample $Role;$peerAfter=Read-Sample $peerRole
    $held=@($timeline|Where-Object {$_.Ms -ge 300 -and $_.Ms -le 1600})
    if($held.Count -lt 5){throw 'Insufficient independent samples during the hold.'}
    $clock=$held[0].Sample.Sequence.clockWords[1]
    foreach($entry in $held) {
        $s=$entry.Sample
        if($s.Sequence.id -ne $before.Sequence.id -or $s.Sequence.pointer -ne $before.Sequence.pointer -or
           $s.Sequence.state -ne 5 -or $s.Sequence.listOffset -ne '0x610' -or $s.Sequence.clockWords[1] -ne $clock -or
           !$s.Status.cinematicHoldActive -or $s.Status.localCinematicCanSkip -ne 0){throw 'Native pause did not hold the same sequence clock safely.'}
    }
    if($after.Sequence.id -ne $before.Sequence.id -or $after.Sequence.state -ne 3 -or $after.Sequence.listOffset -ne '0x0' -or
       $after.Sequence.clockWords[1] -lt $clock+500 -or $after.Status.cinematicHoldActive -or
       $after.Status.cinematicHoldStarted -ne $before.Status.cinematicHoldStarted+1 -or
       $after.Status.cinematicHoldResumed -ne $before.Status.cinematicHoldResumed+1 -or
       $after.Status.cinematicHoldAbandoned -ne $before.Status.cinematicHoldAbandoned){throw 'Native pause did not automatically resume the same sequence.'}
    if($peerAfter.Sequence.id -ne $peerBefore.Sequence.id -or $peerAfter.Sequence.clockWords[1] -lt $peerBefore.Sequence.clockWords[1]+2000 -or
       $after.Status.sent -le $before.Status.sent -or $after.Status.received -le $before.Status.received){throw 'Peer playback or Steam transport stopped during the isolated hold.'}
}catch{$failure=$_.Exception.Message}
$entry=[pscustomobject]@{Passed=($null -eq $failure);Failure=$failure;Role=$Role;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/dinput8.dll')).Hash;Before=$before;After=$after;PeerBefore=$peerBefore;PeerAfter=$peerAfter;Timeline=$timeline.ToArray()}
$record=Join-Path $PSScriptRoot 'research/native-cinematic-hold-verification.json'
$entries=if(Test-Path $record){@((Get-Content $record -Raw|ConvertFrom-Json).Tests)}else{@()}
[ordered]@{Tests=@($entries)+$entry;Scope='Bounded native pause/resume of the player cinematic sequence only. Other local cinematic sequences, audio continuity, shared entry and network timeline correction are not verified.'}|ConvertTo-Json -Depth 12|Set-Content $record
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;Role=$Role;HeldSamples=$held.Count;ClockStayedFixed=$true;AutomaticNativeResume=$true;PeerContinued=$true}
