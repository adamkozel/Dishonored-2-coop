$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$lab=Join-Path $root 'outputs/d2-coop-lab';$research=Join-Path $lab 'research'
$hash=(Get-FileHash (Join-Path $lab 'bin/dinput8.dll')).Hash
$reports=@('native-loading-character-verification.json','native-loading-hold-verification.json','guard-link-verification.json',
    'steam-transport-verification.json','world-observation-verification.json','locomotion-verification.json','native-reconnect-verification.json',
    'lab-restoration-verification.json','baseline-integrity-verification.json')
$records=@{}
foreach($name in $reports){
    $r=Get-Content (Join-Path $research $name) -Raw|ConvertFrom-Json
    $testedHash=if($r.LoaderSha256){$r.LoaderSha256}else{$r.DllSha256}
    if($testedHash -ne $hash){throw "Build attribution mismatch: $name"}
    if($r.PSObject.Properties.Name -contains 'Passed' -and !$r.Passed){throw "Failed report: $name"}
    $records[$name]=$r
}
foreach($name in 'steam-transport-verification.json','locomotion-verification.json'){
    $tests=if($name -eq 'locomotion-verification.json'){$records[$name].Results}else{$records[$name].Tests}
    if($tests.Count -ne 2 -or @($tests|Where-Object {!$_.Passed}).Count){throw "Incomplete tests: $name"}
}
$character=$records['native-loading-character-verification.json'];$hold=$records['native-loading-hold-verification.json']
$guard=$records['guard-link-verification.json'];$restored=$records['lab-restoration-verification.json']
$checkpoint=[ordered]@{
    DllSha256=$hash;NativeRoleRestorationPassed=$true;AutomaticRoleRestorationBeforeGameplay=$true;ExplicitLocalArmingRequired=$true
    SourceCheckpointSha256=$character.SourceCheckpointSha256;SourceRole='Corvo';RestoredRole='Emily'
    RoleRestorationRecord='native-loading-character-verification.json';LoadingHoldRecord='native-loading-hold-verification.json'
    NativeCharacterRoutineRva='0x8a7650';NativeMaintenanceRoutineRva='0x8ab380';NativeLoadingSwitchRva='0x7885b0'
    NativeLogicDuringRestoration=15;NativeWorldPhaseDuringRestoration=3;ReplacementQueueEmptyBeforeRelease=$true
    ReplacementDisplacement=$character.Replacement.restore.displacement;PreservedNpcIdentities=$character.After.Npcs.npcs.Count
    MovementDistance=$character.MovementDistance;BoundedLocalLoadingHoldMilliseconds=2000;SteamTrafficDuringHoldVerified=$true
    BinarySaveOrMetadataModifiedByConversion=$false;BodyDefinitionVerifiedBeforeGameplay=$true
    CheckpointFilesAfterExperiments=(Get-Content (Join-Path $root 'work/pre-gameplay-fixture-hashes.json') -Raw|ConvertFrom-Json)
    NativeCommandObservers=@('LoadGame','RestartFromCheckpoint','RestartFromMemoryCheckpoint')
    EarlierPostGameplayConversionRecord='native-character-restore-verification.json'
    EarlierNativeUiRecord='native-character-ui-guards-verification.json';EarlierNativeUiRecordDll='DC5D01BF30EBD254CF0197625F911ED0A08078DFA9A144C9CD25102390975E70'
    F9Verified=$false;GameOverDispatchVerified=$false;CanonicalSaveTransferImplemented=$false
    SharedResetTransactionImplemented=$false;BothReadyLoadingBarrierImplemented=$false
    Scope='Explicitly armed client-only Dunwall load experiment. An unchanged Corvo checkpoint becomes Emily while the new world remains in native loading state 15, then a two-second local hold precedes gameplay. Position and 64 authored NPC identities preserved. No network reset, save transfer, general quest/inventory/power compatibility or both-ready transaction.'
}
$currentGuard=[ordered]@{Passed=$true;DllSha256=$hash;Record='guard-link-verification.json';Checks=$guard.Checks;Scope=$guard.Scope;
    MaxPoseError=$guard.MaxPoseError;MaxAnimatedBodyError=$guard.MaxAnimatedBodyError;BodyObservations=$guard.BodyObservations;
    ReloadClient=$guard.ReloadClient;DisconnectHost=$guard.DisconnectHost;HistoricalFullRecord='guard-link-full-verification.json';
    HistoricalFullRecordDll='31D5221EC8FC5360249FC49F2B28C5D3027973CB44B25B0BC13F55B5C73C3916';CombatAuthorityImplemented=$false;NaturalAiNavigationVerified=$false;SharedCheckpointReloadImplemented=$false}
$live=@(foreach($row in $restored.Instances){
    $s=Get-Content (Join-Path $root "work/lab-instances/$($row.Role)/coop-status.json") -Raw|ConvertFrom-Json
    $trace=Get-Content (Join-Path $root "work/lab-instances/$($row.Role)/coop-status.json.checkpoint.json") -Raw|ConvertFrom-Json
    $loading=Get-Content (Join-Path $root "work/lab-instances/$($row.Role)/coop-status.json.loading.json") -Raw|ConvertFrom-Json
    if(!$s.remoteFresh -or $s.localGameLogicState -ne 1 -or !$s.probeFollowEnabled -or $trace.hookMask -ne 7 -or $trace.hookFailed -or $loading.probe.armed -or $loading.probe.held){throw 'Final live fixture mismatch.'}
    [ordered]@{Role=$row.Role;Pid=$row.Pid;GameLogic=$s.localGameLogicState;Map=$s.localMap;Epoch=$s.localEpoch;Follow=$s.probeFollowEnabled;
        Locomotion=$s.locomotionEnabled;Fresh=$s.remoteFresh;AiHoldActive=$s.aiHoldActive;GuardPhase=$row.Guard.phase;
        GuardHeldReferences=$row.Guard.heldReferences;WorldObservation=$row.World.enabled;WorldIdentities=$row.World.entries.Count;
        CinematicSync=$s.cinematicSyncEnabled;CinematicBarrier=$s.barrierEnabled;PlayerDefinition=$row.Character.definition;CheckpointHookMask=$trace.hookMask}
})
$statePath=Join-Path $root 'work/current-task-state.json';$state=Get-Content $statePath -Raw|ConvertFrom-Json
$state.UpdatedAt=[DateTimeOffset]::Now.ToString('o');$state.LoaderSha256=$hash;$state.Live=$live;$state.GuardPoseLink=$currentGuard
$state.CheckpointResearch=$checkpoint;$state.CurrentBuildReports=$reports
$state.Next='Implement one host-arbitrated checkpoint transaction over native Steam: canonical save identity/transfer, native requests from either player, duplicate rejection, restore roles while loading, both-ready release, native death-screen flow and peer-loss recovery. The local pre-gameplay role restoration and two-second loading hold now pass; do not treat the independent role-specific slot 81 fixtures as a shared checkpoint.'
$state|ConvertTo-Json -Depth 16|Set-Content $statePath
$vPath=Join-Path $research 'verification.json';$v=Get-Content $vPath -Raw|ConvertFrom-Json
if($v.LoaderSha256 -ne $hash){$v.PreviousBroadMilestoneDll=$v.LoaderSha256};$v.LoaderSha256=$hash;$v.timestamp=$state.UpdatedAt
$v.VerificationAttribution="Historical feature flags retain their original scopes. CurrentBuildReports lists $($reports.Count) reports verified on this loader; each record carries its hash. Earlier UI/cinematic scenarios were not all rerun."
$v.CheckpointResearch=$checkpoint;$v.CurrentGuardRegression=$currentGuard;$v.CurrentBuildReports=$reports;$v.CurrentLive=$live
$v.ReadyForFinalUserCheck=$false
$v|ConvertTo-Json -Depth 17|Set-Content $vPath
[pscustomobject]@{LoaderSha256=$hash;CurrentBuildReports=$reports.Count;RoleRestoredBeforeGameplay=$true;SharedResetImplemented=$false;Live=$live}
