$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$lab=Join-Path $root 'outputs/d2-coop-lab'
$research=Join-Path $lab 'research'
$hash=(Get-FileHash (Join-Path $lab 'bin/dinput8.dll')).Hash
function Record([string]$name) {
    $r=Get-Content (Join-Path $research $name) -Raw|ConvertFrom-Json
    if($r.DllSha256 -ne $hash){throw "Build attribution mismatch: $name"}
    if($r.PSObject.Properties.Name -contains 'Passed' -and !$r.Passed){throw "Failed record: $name"}
    $r
}
$character=Record 'native-character-restore-verification.json'
$ui=Record 'native-character-ui-guards-verification.json'
$guard=Record 'guard-link-verification.json'
$steam=Record 'steam-transport-verification.json'
if($steam.Tests.Count -ne 2 -or @($steam.Tests|Where-Object {!$_.Passed}).Count){throw 'Steam tests incomplete.'}
$world=Record 'world-observation-verification.json'
$loco=Record 'locomotion-verification.json'
$reconnect=Record 'native-reconnect-verification.json'
$restored=Record 'lab-restoration-verification.json'
$baseline=Get-Content (Join-Path $research 'baseline-integrity-verification.json') -Raw|ConvertFrom-Json
if(!$baseline.Passed -or $baseline.LoaderSha256 -ne $hash){throw 'Baseline integrity not verified for this build.'}
$checkpoint=[ordered]@{
    DllSha256=$hash;NativeRoleRestorationPassed=$true;SourceCheckpointSha256=$character.SourceCheckpointSha256
    RoleRestorationRecord='native-character-restore-verification.json';NativeUiRecord='native-character-ui-guards-verification.json'
    SourceRole='Corvo';RestoredRole='Emily';NativeCharacterRoutineRva='0x8a7650';MovementDistance=$character.MovementDistance
    PreservedNpcIdentities=$character.Samples[0].Npcs.npcs.Count
    BodyAndBothArmDefinitionsVerified=$true;NativeEmilyPausePortraitVerified=$true
    HostPauseAndDuplicateGuardsPassed=$true;BinarySaveOrMetadataModifiedByConversion=$false
    NativeCommandObservers=@('LoadGame','RestartFromCheckpoint','RestartFromMemoryCheckpoint')
    NativeUiLoadEvidence='native-menu-reload-verification.json';NativeUiLoadEvidenceDll='AE9111A8173F524D29136B525564318791129BEC4A47D2D52530F517A9BF7679'
    F9Verified=$false;GameOverDispatchVerified=$false;AutomaticRoleRestorationBeforeGameplay=$false
    CanonicalSaveTransferImplemented=$false;SharedResetTransactionImplemented=$false;BothReadyLoadingBarrierImplemented=$false
    Scope='Dunwall explicit developer experiment: unchanged Corvo checkpoint loaded, native conversion to Emily after gameplay is ready, native movement, reverse conversion, original Emily fixture restored. Does not establish campaign-wide powers, inventory, quests or synchronized resets.'
}
$currentGuard=[ordered]@{Passed=$true;DllSha256=$hash;Record='guard-link-verification.json';Checks=$guard.Checks;Scope=$guard.Scope;
    MaxPoseError=$guard.MaxPoseError;MaxAnimatedBodyError=$guard.MaxAnimatedBodyError;BodyObservations=$guard.BodyObservations;
    ReloadClient=$guard.ReloadClient;DisconnectHost=$guard.DisconnectHost;HistoricalFullRecord='guard-link-full-verification.json';
    HistoricalFullRecordDll='31D5221EC8FC5360249FC49F2B28C5D3027973CB44B25B0BC13F55B5C73C3916';CombatAuthorityImplemented=$false;NaturalAiNavigationVerified=$false;SharedCheckpointReloadImplemented=$false}
$live=@(foreach($row in $restored.Instances){
    $s=Get-Content (Join-Path $root "work/lab-instances/$($row.Role)/coop-status.json") -Raw|ConvertFrom-Json
    $trace=Get-Content (Join-Path $root "work/lab-instances/$($row.Role)/coop-status.json.checkpoint.json") -Raw|ConvertFrom-Json
    if(!$s.remoteFresh -or $s.localGameLogicState -ne 1 -or !$s.probeFollowEnabled -or $trace.hookMask -ne 7 -or $trace.hookFailed){throw 'Final live state differs from restored fixture.'}
    [ordered]@{Role=$row.Role;Pid=$row.Pid;GameLogic=$s.localGameLogicState;Map=$s.localMap;Epoch=$s.localEpoch;Follow=$s.probeFollowEnabled;
        Locomotion=$s.locomotionEnabled;Fresh=$s.remoteFresh;AiHoldActive=$s.aiHoldActive;GuardPhase=$row.Guard.phase;
        GuardHeldReferences=$row.Guard.heldReferences;WorldObservation=$row.World.enabled;WorldIdentities=$row.World.entries.Count;
        CinematicSync=$s.cinematicSyncEnabled;CinematicBarrier=$s.barrierEnabled;PlayerDefinition=$row.Character.definition;CheckpointHookMask=$trace.hookMask}
})
$reports=@('native-character-restore-verification.json','native-character-ui-guards-verification.json','guard-link-verification.json',
    'steam-transport-verification.json','world-observation-verification.json','locomotion-verification.json','native-reconnect-verification.json',
    'lab-restoration-verification.json','baseline-integrity-verification.json')
$statePath=Join-Path $root 'work/current-task-state.json'
$state=Get-Content $statePath -Raw|ConvertFrom-Json
$state.UpdatedAt=[DateTimeOffset]::Now.ToString('o');$state.LoaderSha256=$hash;$state.Live=$live;$state.GuardPoseLink=$currentGuard
$state|Add-Member -NotePropertyName CheckpointResearch -NotePropertyValue $checkpoint -Force
$state|Add-Member -NotePropertyName CurrentBuildReports -NotePropertyValue $reports -Force
$state.Next='Implement a native loading barrier so role conversion completes before gameplay resumes; then a single host-arbitrated canonical checkpoint transaction over Steam with deduplication, native death/UI routes and peer-loss recovery. Do not treat two unrelated local slot 81 saves as a canonical shared checkpoint.'
$state|ConvertTo-Json -Depth 15|Set-Content $statePath
$verificationPath=Join-Path $research 'verification.json'
$v=Get-Content $verificationPath -Raw|ConvertFrom-Json
$previousHash=$v.LoaderSha256;$v.LoaderSha256=$hash;$v.timestamp=$state.UpdatedAt
$v|Add-Member -NotePropertyName PreviousBroadMilestoneDll -NotePropertyValue $previousHash -Force
$v|Add-Member -NotePropertyName VerificationAttribution -NotePropertyValue 'Legacy feature flags summarize historical checks. Current-build checks are explicitly listed in CurrentBuildReports; individual records identify their loader. Not every historical scenario was rerun.' -Force
$v|Add-Member -NotePropertyName CheckpointResearch -NotePropertyValue $checkpoint -Force
$v|Add-Member -NotePropertyName CurrentGuardRegression -NotePropertyValue $currentGuard -Force
$v|Add-Member -NotePropertyName CurrentBuildReports -NotePropertyValue $reports -Force
$v|Add-Member -NotePropertyName CurrentLive -NotePropertyValue $live -Force
$v|ConvertTo-Json -Depth 16|Set-Content $verificationPath
[pscustomobject]@{LoaderSha256=$hash;CurrentBuildReports=$reports.Count;NativeRoleRestoration=$true;SharedResetImplemented=$false;Live=$live}
