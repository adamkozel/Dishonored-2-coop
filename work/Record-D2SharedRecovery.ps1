$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$lab=Join-Path $root 'outputs/d2-coop-lab'
$research=Join-Path $lab 'research'
$hash=(Get-FileHash -LiteralPath (Join-Path $lab 'bin/dinput8.dll')).Hash
$required=@(
    'avatar-ai-ownership-verification.json',
    'shared-checkpoint-client-delay-client-verification.json','shared-checkpoint-host-delay-host-verification.json',
    'shared-checkpoint-duplicates-verification.json','shared-checkpoint-mismatch-verification.json',
    'shared-checkpoint-disconnect-client-verification.json','shared-checkpoint-disconnect-host-verification.json',
    'shared-checkpoint-disconnect-client-loading-verification.json','shared-checkpoint-disconnect-host-loading-verification.json',
    'session-disconnect-client-gameplay-verification.json','session-disconnect-host-death-verification.json',
    'shared-death-client-verification.json','shared-death-host-verification.json','shared-death-concurrent-verification.json',
    'shared-death-host-from-pause-verification.json','native-loading-character-verification.json','native-loading-hold-verification.json',
    'guard-link-verification.json','world-observation-verification.json','locomotion-verification.json',
    'native-reconnect-verification.json','lab-restoration-verification.json','steam-transport-verification.json','baseline-integrity-verification.json')
$rows=@(foreach($name in $required) {
    $r=Get-Content -LiteralPath (Join-Path $research $name) -Raw|ConvertFrom-Json
    $attribution=if($name -eq 'baseline-integrity-verification.json'){$r.LoaderSha256}else{$r.DllSha256}
    if($attribution -ne $hash){throw "Wrong build: $name"}
    if($name -eq 'steam-transport-verification.json') {
        if($r.Tests.Count -ne 2 -or @($r.Tests|Where-Object {!$_.Passed}).Count){throw 'Steam movement checks incomplete.'}
    }elseif(!$r.Passed){throw "Failed report: $name"}
    if($name -eq 'locomotion-verification.json' -and ($r.Results.Count -ne 2 -or @($r.Results|Where-Object {!$_.Passed}).Count)){throw 'Both character movement regressions are required.'}
    [ordered]@{Report=$name;Passed=$true;ObservedAt=$r.ObservedAt;Sha256=(Get-FileHash -LiteralPath (Join-Path $research $name)).Hash}
})
$live=@(foreach($role in 'host','client') {
    $directory=Join-Path $root "work/lab-instances/$role"
    $launch=Get-Content -LiteralPath (Join-Path $directory 'launch.json') -Raw|ConvertFrom-Json
    $process=Get-Process -Id $launch.pid -ErrorAction Stop
    if($process.Path -ne $launch.executable){throw 'Unexpected process.'}
    $file=Get-Item -LiteralPath (Join-Path $directory 'coop-status.json')
    if(([DateTime]::UtcNow-$file.LastWriteTimeUtc).TotalSeconds -gt 2){throw 'Stale live state.'}
    $s=Get-Content -LiteralPath $file.FullName -Raw|ConvertFrom-Json
    if($s.localGameLogicState -ne 1 -or $s.localPendingGameLogicState -or !$s.remoteFresh -or !$s.probeFollowEnabled){throw 'Lab pair is not restored.'}
    [ordered]@{Role=$role;Pid=$launch.pid;GameLogic=$s.localGameLogicState;Map=$s.localMap;Epoch=$s.localEpoch;Follow=$s.probeFollowEnabled;Locomotion=$s.locomotionEnabled;Fresh=$s.remoteFresh;ProbeHandle=$s.probeActorHandle}
})
$summary=[ordered]@{
    ObservedAt=[DateTimeOffset]::Now.ToString('o');Passed=$true;DllSha256=$hash;Reports=$rows
    Scope='Fixed byte-identical pre-staged Dunwall SLOT82. Native reload requests and either natural fall death coordinate one shared reset with pre-gameplay Corvo/Emily restoration and both-ready gate. Previously enabled counterpart presentation returns once. Native UI state and Flash bindings inspected, without screenshot verification.'
    CanonicalDigest='a296b888c17eff7465e0c9b5023672925cb2be9ebec37960ac20b70ed9eff519'
    SharedReload=$true;SharedDeath=$true;NativeGameOverState=13;BothReadyGate=$true;RolePreservation=$true
    CounterpartRestoredAfterReset=$true;EitherLoadingPeerLossPauses=$true;LostGameplaySessionPauses=$true
    NativeUiVisuallyVerified=$false;SaveTransferImplemented=$false;ArbitraryCheckpointSelection=$false
    SharedCombat=$false;SharedWorldAuthority=$false;CampaignSupport=$false;TwoAccountOnlineVerified=$false;PlayableCoop=$false
    CurrentLive=$live
}
$summary|ConvertTo-Json -Depth 12|Set-Content -LiteralPath (Join-Path $research 'shared-recovery-summary.json')
$statePath=Join-Path $root 'work/current-task-state.json'
$s=Get-Content -LiteralPath $statePath -Raw|ConvertFrom-Json
$s|Add-Member -NotePropertyName PreviousBroadMilestoneDll -NotePropertyValue $s.LoaderSha256 -Force
$s.UpdatedAt=$summary.ObservedAt;$s.LoaderSha256=$hash;$s.Live=$live;$s.CurrentBuildReports=$required
$s|Add-Member -NotePropertyName SharedRecovery -NotePropertyValue $summary -Force
$s.Next='Continue incomplete player actions/combat, authoritative world and campaign support. Fixed-fixture shared reload/death and counterpart restoration now verified; arbitrary save selection/transfer, native session UI and real two-account Steam testing remain unfinished. Do not stop at milestones.'
$s.WorkInProgress.UpdatedAt=$summary.ObservedAt;$s.WorkInProgress.CandidateDll=$hash
$s.WorkInProgress.PendingTest='Required shared-recovery and regression reports validated against current binary; proceed to the next implementation task.'
$s.WorkInProgress.Next=$s.Next
$s|ConvertTo-Json -Depth 30|Set-Content -LiteralPath $statePath
$vPath=Join-Path $research 'verification.json'
$v=Get-Content -LiteralPath $vPath -Raw|ConvertFrom-Json
$v|Add-Member -NotePropertyName PreviousBroadMilestoneDll -NotePropertyValue $v.LoaderSha256 -Force
$v.LoaderSha256=$hash;$v.timestamp=$summary.ObservedAt
$v|Add-Member -NotePropertyName SharedRecovery -NotePropertyValue $summary -Force
$v|Add-Member -NotePropertyName CurrentBuildReports -NotePropertyValue $required -Force
$v|Add-Member -NotePropertyName CurrentLive -NotePropertyValue $live -Force
$v|Add-Member -NotePropertyName VerificationAttribution -NotePropertyValue 'CurrentBuildReports are hash-checked for this build. Other feature entries are historical and retain their own build attribution. This is a development lab, not playable co-op.' -Force
$v|ConvertTo-Json -Depth 30|Set-Content -LiteralPath $vPath
[pscustomobject]@{Passed=$true;DllSha256=$hash;Reports=$rows.Count;PlayableCoop=$false}
