[CmdletBinding()]
param([ValidateSet('host','client')][string]$FirstRole='host',[switch]$NoPeer)
$ErrorActionPreference='Stop'
$workspaceRoot=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$secondRole=if($FirstRole -eq 'host'){'client'}else{'host'}
function Read-State([string]$role) {
    $root=Join-Path $workspaceRoot "work/lab-instances/$role"
    $launch=Get-Content (Join-Path $root 'launch.json') -Raw|ConvertFrom-Json
    if((Get-Process -Id $launch.pid).Path -ne $launch.executable){throw 'Unexpected game process.'}
    $file=Get-Item (Join-Path $root 'coop-status.json')
    if(([DateTime]::UtcNow-$file.LastWriteTimeUtc).TotalSeconds -gt 1.5){throw 'Stale status.'}
    $s=Get-Content $file.FullName -Raw|ConvertFrom-Json
    if(!$s.remoteFresh -or $s.connectionState -ne 3 -or $s.poseProtocol -notin @(7,8,9,10)){throw 'Native Steam peer with opening-barrier support unavailable.'}
    return $s
}
function Read-Sequence([string]$role) {
    $launch=Get-Content (Join-Path $workspaceRoot "work/lab-instances/$role/launch.json") -Raw|ConvertFrom-Json
    $native=& (Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe') $launch.pid --midnight
    if($LASTEXITCODE){throw 'Independent sequence read failed.'}
    $instances=@(($native|ConvertFrom-Json).instances|Where-Object playerChannel -ge 0)
    if($instances.Count -ne 1){throw 'Need one native player sequence.'}
    return $instances[0]
}
$before=@{host=(Read-State host);client=(Read-State client)}
foreach($s in $before.Values){if($s.localGameLogicState -ne 3 -or !$s.barrierEnabled -or !$s.remoteBarrierEnabled){throw 'Both native character confirmation dialogs must be open, with both rendezvous modes enabled.'}}
$timeline=[Collections.Generic.List[object]]::new();$held=[Collections.Generic.List[object]]::new();$aligned=[Collections.Generic.List[object]]::new();$failure=$null
foreach($role in 'host','client'){& (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $role -Button Right -DurationMs 150|Out-Null}
Start-Sleep -Milliseconds 400
$watch=[Diagnostics.Stopwatch]::StartNew()
try {
    & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $FirstRole -Button A -DurationMs 150|Out-Null
    do {
        Start-Sleep -Milliseconds 100
        $first=Read-State $FirstRole
        $timeline.Add([pscustomobject]@{Ms=$watch.ElapsedMilliseconds;Role=$FirstRole;Status=$first})
    }while(!$first.barrierHeld -and $watch.ElapsedMilliseconds -lt 5000)
    if(!$first.barrierHeld){throw 'The first character did not stop at the opening boundary.'}
    for($i=0;$i -lt 5;$i++) {
        Start-Sleep -Milliseconds 200
        $native=Read-Sequence $FirstRole;$state=Read-State $FirstRole;$other=Read-State $secondRole
        $expectedBranch=if($FirstRole -eq 'host'){'player_is_corvo'}else{'player_is_emily'}
        if($native.subscene -ne $expectedBranch){throw 'The native character branch does not match the assigned role.'}
        $held.Add([pscustomobject]@{Ms=$watch.ElapsedMilliseconds;Native=$native;Status=$state;OtherStatus=$other})
        if($native.state -ne 5 -or $native.listOffset -ne '0x610' -or !$state.barrierHeld -or $other.localGameLogicState -ne 3){throw 'First sequence did not remain natively paused while its peer was choosing.'}
        if($i -gt 0 -and $native.clockWords[1] -ne $held[0].Native.clockWords[1]){throw 'First sequence clock advanced before the peer was ready.'}
    }
    if($NoPeer) {
        do {
            Start-Sleep -Milliseconds 150
            $after=@{host=(Read-State host);client=(Read-State client)}
            $timeline.Add([pscustomobject]@{Ms=$watch.ElapsedMilliseconds;Host=$after.host;Client=$after.client})
        }while($after[$FirstRole].barrierHeld -and $watch.ElapsedMilliseconds -lt 10000)
        Start-Sleep -Milliseconds 500
        $recovery=Read-Sequence $FirstRole;$after=@{host=(Read-State host);client=(Read-State client)}
        $s=$after[$FirstRole];$b=$before[$FirstRole];$other=$after[$secondRole];$otherBefore=$before[$secondRole]
        if($s.barrierHeld -or $s.barrierStarted -ne $b.barrierStarted+1 -or $s.barrierTimedOut -ne $b.barrierTimedOut+1 -or
           $s.barrierReleased -ne $b.barrierReleased -or $s.cinematicHoldResumed -ne $b.cinematicHoldResumed+1 -or
           $s.cinematicHoldAbandoned -ne $b.cinematicHoldAbandoned -or $s.barrierSent -ne $b.barrierSent -or
           $recovery.state -ne 3 -or $recovery.clockWords[1] -lt 100 -or $other.localGameLogicState -ne 3 -or
           $other.barrierStarted -ne $otherBefore.barrierStarted){throw 'Missing-peer timeout did not restore native playback without advancing the other dialog.'}
    } else {
    & (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $secondRole -Button A -DurationMs 150|Out-Null
    do {
        Start-Sleep -Milliseconds 100
        $after=@{host=(Read-State host);client=(Read-State client)}
        $timeline.Add([pscustomobject]@{Ms=$watch.ElapsedMilliseconds;Host=$after.host;Client=$after.client})
    }while(($after.host.barrierReleased -eq $before.host.barrierReleased -or $after.client.barrierReleased -eq $before.client.barrierReleased) -and $watch.ElapsedMilliseconds -lt 12000)
    foreach($role in 'host','client') {
        $s=$after[$role];$b=$before[$role]
        if($s.barrierStarted -ne $b.barrierStarted+1 -or $s.barrierReleased -ne $b.barrierReleased+1 -or
           $s.barrierTimedOut -ne $b.barrierTimedOut -or $s.barrierMissed -ne $b.barrierMissed -or $s.barrierRejected -ne $b.barrierRejected -or
           $s.cinematicHoldAbandoned -ne $b.cinematicHoldAbandoned -or $s.barrierHeld){throw 'Expected one coordinated hold/release per character without timeout, missed entry or rejection.'}
    }
    if($after.host.barrierSent -ne $before.host.barrierSent+1 -or $after.client.barrierReceived -ne $before.client.barrierReceived+1){throw 'Expected exactly one reliable host release and client receipt.'}
    for($i=0;$i -lt 3;$i++) {
        Start-Sleep -Milliseconds 250
        $h=Read-Sequence host;$c=Read-Sequence client
        if($h.subscene -ne 'player_is_corvo' -or $c.subscene -ne 'player_is_emily'){throw 'Expected actual Corvo and Emily native character branches.'}
        $drift=[Math]::Abs([int64]$h.clockWords[1]-[int64]$c.clockWords[1])
        $aligned.Add([pscustomobject]@{Host=$h;Client=$c;ObservedClockDifferenceMs=$drift})
        if($h.state -ne 3 -or $c.state -ne 3 -or $h.sceneDurationMs -ne 28000 -or $c.sceneDurationMs -ne 28000 -or $drift -gt 100){throw 'Released native segment clocks were not aligned within the 100 ms observation tolerance.'}
    }
    }
}catch{$failure=$_.Exception.Message}
$entry=[pscustomobject]@{Passed=($null -eq $failure);Failure=$failure;FirstRole=$FirstRole;MissingPeer=[bool]$NoPeer;RecoverySample=$recovery;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/dinput8.dll')).Hash;Before=$before;After=$after;HeldSamples=$held.ToArray();AlignedSamples=$aligned.ToArray();Timeline=$timeline.ToArray()}
$record=Join-Path $PSScriptRoot 'research/native-opening-barrier-verification.json'
$entries=if(Test-Path $record){@((Get-Content $record -Raw|ConvertFrom-Json).Tests)}else{@()}
[ordered]@{Tests=@($entries)+$entry;Scope='Bounded rendezvous of the two player sequences at the exact opening post-choice boundary. One character deliberately confirms later. Other local sequences, audio continuity, campaign-wide entry and ongoing clock correction remain unverified.'}|ConvertTo-Json -Depth 12|Set-Content $record
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;FirstRole=$FirstRole;MissingPeer=[bool]$NoPeer;FrozenSamples=$held.Count;MaximumObservedClockDifferenceMs=($aligned.ObservedClockDifferenceMs|Measure-Object -Maximum).Maximum;ElapsedMs=$watch.ElapsedMilliseconds}


