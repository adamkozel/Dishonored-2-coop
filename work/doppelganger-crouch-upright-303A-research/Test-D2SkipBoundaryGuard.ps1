[CmdletBinding()]
param([ValidateSet('host','client')][string]$SourceRole='host')
$ErrorActionPreference='Stop'
$workspaceRoot=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$targetRole=if($SourceRole -eq 'host'){'client'}else{'host'}
function Read-State([string]$role) {
    $root=Join-Path $workspaceRoot "work/lab-instances/$role"
    $launch=Get-Content (Join-Path $root 'launch.json') -Raw|ConvertFrom-Json
    if((Get-Process -Id $launch.pid).Path -ne $launch.executable){throw 'Unexpected game process.'}
    $file=Get-Item (Join-Path $root 'coop-status.json')
    if(([DateTime]::UtcNow-$file.LastWriteTimeUtc).TotalSeconds -gt 1.5){throw 'Stale status.'}
    $s=Get-Content $file.FullName -Raw|ConvertFrom-Json
    if(!$s.remoteFresh -or $s.connectionState -ne 3){throw 'Steam peer unavailable.'}
    return $s
}
$before=@{host=(Read-State host);client=(Read-State client)}
foreach($s in $before.Values) {
    if($s.poseProtocol -notin @(6,7,8,9,10) -or !$s.cinematicSyncEnabled -or $s.remoteCinematicSyncEnabled -ne 1 -or
       $s.localGameLogicState -ne 5 -or $s.localPendingGameLogicState -or $s.localCinematicCanSkip -ne 1 -or
       $s.localSkipSceneId -eq '0' -or $s.localSkipSceneId -ne $s.remoteSkipSceneId -or
       $s.localSequenceMetadataValid -ne 1 -or $s.remoteSequenceMetadataValid -ne 1 -or
       $s.localSequenceBoundaryId -eq '0' -or $s.remoteSequenceBoundaryId -eq '0' -or
       $s.localSequenceBoundaryId -eq $s.remoteSequenceBoundaryId){throw 'Need the same story observer boundary but different active skippable segments.'}
}
$targetInput=Join-Path $workspaceRoot "work/lab-instances/$targetRole/input.bin"
$inputHash=(Get-FileHash $targetInput).Hash
$timeline=[Collections.Generic.List[object]]::new()
& (Join-Path $PSScriptRoot 'Send-D2Input.ps1') -Role $SourceRole -Button X -DurationMs 3000|Out-Null
$watch=[Diagnostics.Stopwatch]::StartNew();$failure=$null
try {
    do {
        Start-Sleep -Milliseconds 150
        $after=@{host=(Read-State host);client=(Read-State client)}
        $timeline.Add([pscustomobject]@{Ms=$watch.ElapsedMilliseconds;Source=$after[$SourceRole];Target=$after[$targetRole]})
        if($after[$SourceRole].skipSent -ne $before[$SourceRole].skipSent -or
           $after[$targetRole].skipApplied -ne $before[$targetRole].skipApplied -or
           $after[$targetRole].nativeSkipCallbacks -ne $before[$targetRole].nativeSkipCallbacks -or
           $after[$targetRole].localSequenceBoundaryId -ne $before[$targetRole].localSequenceBoundaryId){throw 'A mismatched native segment was advanced or a skip message was sent.'}
    }while($watch.ElapsedMilliseconds -lt 5000)
    if($after[$SourceRole].nativeSkipCallbacks -ne $before[$SourceRole].nativeSkipCallbacks+1){throw 'Source native skip was not exercised exactly once.'}
    if((Get-FileHash $targetInput).Hash -ne $inputHash){throw 'Target input changed.'}
}catch{$failure=$_.Exception.Message}
$entry=[pscustomobject]@{Passed=($null -eq $failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');SourceRole=$SourceRole;DllSha256=(Get-FileHash (Join-Path $PSScriptRoot 'bin/dinput8.dll')).Hash;Before=$before;After=$after;Timeline=$timeline.ToArray();PeerInputUnchanged=((Get-FileHash $targetInput).Hash -eq $inputHash)}
$record=Join-Path $PSScriptRoot 'research/native-skip-boundary-guard-verification.json'
$entries=if(Test-Path $record){@((Get-Content $record -Raw|ConvertFrom-Json).Tests)}else{@()}
[ordered]@{Tests=@($entries)+$entry;Scope='One native confirmed skip while both peers have the same broad story boundary but different active Midnight segments. The other segment must remain untouched.'}|ConvertTo-Json -Depth 12|Set-Content $record
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;SourceRole=$SourceRole;PeerInputUnchanged=$true;SourceCallbacksDelta=1;SentDelta=0;PeerAppliedDelta=0}



