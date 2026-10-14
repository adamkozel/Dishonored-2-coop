$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$lab=Join-Path $root 'outputs/d2-coop-lab'
$research=Join-Path $lab 'research'
function Report([string]$name){Get-Content (Join-Path $research $name) -Raw|ConvertFrom-Json -AsHashtable}
$hash=(Get-FileHash (Join-Path $lab 'bin/dinput8.dll')).Hash
$guard=Report 'guard-link-full-verification.json'
$coexist=Report 'guard-link-coexistence-verification.json'
$ai=Report 'ai-decision-hold-verification.json'
$world=Report 'world-observation-verification.json'
$loco=Report 'locomotion-verification.json'
$locoGuard=Report 'locomotion-guards-verification.json'
$steam=Report 'steam-transport-verification.json'
$baseline=Report 'baseline-integrity-verification.json'
foreach($r in @($guard,$coexist,$ai,$world,$loco,$locoGuard)) {
    if(!$r.Passed -or $r.DllSha256 -ne $hash){throw 'Current-build regression evidence missing.'}
}
if(!$baseline.Passed -or $baseline.LoaderSha256 -ne $hash -or $steam.DllSha256 -ne $hash -or @($steam.Tests|Where-Object {!$_.Passed}).Count){throw 'Transport or integrity evidence missing.'}
$live=@(foreach($role in 'host','client') {
    $dir=Join-Path $root "work/lab-instances/$role"
    $launch=Get-Content (Join-Path $dir 'launch.json') -Raw|ConvertFrom-Json
    $file=Get-Item (Join-Path $dir 'coop-status.json')
    $s=Get-Content $file.FullName -Raw|ConvertFrom-Json
    $g=Get-Content (Join-Path $dir 'coop-status.json.guard.json') -Raw|ConvertFrom-Json
    $w=Get-Content (Join-Path $dir 'coop-status.json.world.json') -Raw|ConvertFrom-Json
    if(([DateTime]::UtcNow-$file.LastWriteTimeUtc).TotalSeconds -gt 1.5 -or !$s.remoteFresh -or $s.localGameLogicState -ne 1 -or
       !$s.probeFollowEnabled -or !$s.locomotionEnabled -or $g.phase -or $g.heldReferences -or $g.frameOwned -or $s.aiHoldActive){throw 'Restored lab is not ready and released.'}
    [ordered]@{Role=$role;Pid=$launch.pid;GameLogic=$s.localGameLogicState;Map=$s.localMap;Follow=$s.probeFollowEnabled;
        Locomotion=$s.locomotionEnabled;Fresh=$s.remoteFresh;AiHoldActive=$s.aiHoldActive;GuardPhase=$g.phase;GuardHeldReferences=$g.heldReferences;
        WorldObservation=$w.enabled;WorldIdentities=$w.entries.Count;CinematicSync=$s.cinematicSyncEnabled;CinematicBarrier=$s.barrierEnabled;
        PlayerDefinition=$(if($role -eq 'host'){'models/characters/player/corvo/body.def'}else{'models/characters/player/emily/body.def'});
        PlayerDefinitionEvidence='Independent runtime --entities read after final checkpoint restoration'}
})
$v=Report 'verification.json'
$v.timestamp=[DateTimeOffset]::Now.ToString('o');$v.LoaderSha256=$hash
$v.NativeGuardPoseLink=[ordered]@{
    Passed=$guard.Passed;DllSha256=$hash;Record='guard-link-full-verification.json';CoexistenceRecord='guard-link-coexistence-verification.json';
    Scope=$guard.Scope;Target='ld_spawner_guard_tutorial_1';LeaseMs=8000;PoseFreshnessMs=250;ProtocolVersion=1;PacketBytes=152;
    MaxPoseError=$guard.MaxPoseError;MaxAnimatedBodyError=$guard.MaxAnimatedBodyError;BodyObservations=$guard.BodyObservations;
    Checks=$guard.Checks;CoexistencePassed=$coexist.Passed;NativeAiKinds=@('Decision','Reflex');CombatAuthorityImplemented=$false;
    NaturalAiNavigationVerified=$false;SharedCheckpointReloadImplemented=$false
}
$a=@($ai.Samples|Where-Object {$_.Role -eq 'client' -and $_.Native});$first=$a[0].Status;$last=$a[-1].Status
$v.NativeAiDecisionHold.DllSha256=$hash;$v.NativeAiDecisionHold.Checks=$ai.Checks
$v.NativeAiDecisionHold.Acquired=$last.aiHoldStarted-$first.aiHoldStarted
$v.NativeAiDecisionHold.Released=$last.aiHoldReleased-$first.aiHoldReleased
$v.NativeAiDecisionHold.Invalidated=$last.aiHoldInvalidated-$first.aiHoldInvalidated
$v.NativeAiDecisionHold.CurrentBuildReloadAndDisconnectTested=$false
$v.NativeAiDecisionHold.HistoricalFullRecord='ai-decision-hold-before-guard-verification.json'
$v.NativeLocomotion.DllSha256=$hash
$v.NativeLocomotion.MaxAnimatedBodyError=(@($loco.Results|ForEach-Object {$_.Metrics}|ForEach-Object {$_.MaxBodyError})|Measure-Object -Maximum).Maximum
$v.NativeLocomotion.PausePassed=$locoGuard.Passed
$v.NativeWorldNpcObservation.DllSha256=$hash
$v.NativeWorldNpcObservation.MatchedPacketComparisons=$world.MatchedPackets
$v.NativeWorldNpcObservation.MaxPoseDifference=$world.MaxPoseError
$v.EarlierRegressionsScope='Current 31D5221E build: eight guard pose-link checks including native reload and actual host loss, coexistence with NPC observation and avatar features, decision-only hold, Steam movement, both-character walking, avatar pause/resume, and NPC observation with client reload passed. Separate decision-only reload/disconnect evidence remains on 46A10BA5; avatar and NPC-observation disconnect evidence remains on 0EC33CB7; cinematic skip/opening rendezvous evidence remains on 23D5911E. Per-build results do not establish complete co-op.'
$v.Outstanding[0]='Host-authoritative AI beyond the bounded single-guard pose experiment, combat/perception control, and general entity birth/despawn negotiation'
$v.RestoredLab=$live
$v.NativeCheckpointEntrypointResearch='native-checkpoint-entrypoints.json'
$v|ConvertTo-Json -Depth 18|Set-Content (Join-Path $research 'verification.json')
[ordered]@{UpdatedAt=$v.timestamp;LoaderSha256=$hash;GuardPoseLink=$v.NativeGuardPoseLink;AiDecisionScope=$v.NativeAiDecisionHold;
    UserSteering=$v.SharedCheckpointReset;Live=$live;Next='Advance natural host guard movement/perception/combat control and shared native checkpoint recovery. Static checkpoint entry points are recorded; UI dispatch, canonical save compatibility across character roles, deduplication and both-ready reset remain unfinished.'}|
    ConvertTo-Json -Depth 12|Set-Content (Join-Path $root 'work/current-task-state.json')
[pscustomobject]@{Recorded=$true;Loader=$hash;GuardChecks=$guard.Checks.Count;CoexistenceChecks=$coexist.Checks.Count;Live=$live}
