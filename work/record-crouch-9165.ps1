$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$lab=Join-Path $root 'outputs/d2-coop-lab'
$backup=Join-Path $PSScriptRoot 'doppelganger-crouch-wholebody-9165-research'
$sha='916546D6EBCEC336BDF553D14739E7F11E853F2E9B1CAA74B9D4193C7808CB8D'
if((Get-FileHash "$lab/bin/dinput8.dll").Hash-ne$sha){throw 'Unexpected candidate'}
$reports=[ordered]@{
 'doppelganger-action-verification.json'='doppelganger-actions-9165.json'
 'doppelganger-steam-action-verification.json'='doppelganger-steam-actions-9165.json'
 'doppelganger-airborne-verification.json'='doppelganger-airborne-9165.json'
}
foreach($pair in $reports.GetEnumerator()){
 $v=Get-Content "$lab/research/$($pair.Key)" -Raw|ConvertFrom-Json
 $hash=if($v.Dll){$v.Dll}else{$v.DllSha256}
 if(!$v.Passed-or$hash-ne$sha){throw "Unexpected regression: $($pair.Key)"}
 Copy-Item -LiteralPath "$lab/research/$($pair.Key)" -Destination "$lab/research/$($pair.Value)"
 Copy-Item -LiteralPath "$lab/research/$($pair.Value)" -Destination "$backup/research/$($pair.Value)"
}
$now=[DateTimeOffset]::Now.ToString('o')
$scope='Whole-body crouch mechanical checks pass both roles on 9165. Full native torso/head/arms/hands with player-driven pelvis and stepping. Visual pose and crouched combat remain unfinished; disabled by default. Original local first-person behavior preserved.'
$next='Improve complete crouch sword/arm posture and Emily torso bend using coordinated native references, then integrate crouched combat. Validate directional gait and terrain. Lower-body-only solution rejected; finger tuning paused. Preserve stable 424B and protected Steam files.'
$done='9165: whole-body crouch both roles; pause both directions and checkpoint invalidation; first-person 3 checks/32 observations; direct actions 8 cases; Steam action stream 6 checks/70 observations; airborne 4 cycles; original file/save integrity all passed. Overlap-fixture failure retained; general partner collision remains open. No test process pending.'
$state=Get-Content "$PSScriptRoot/current-task-state.json" -Raw|ConvertFrom-Json
$state.UpdatedAt=$now
$state.LoaderSha256=$sha
$state.WorkInProgress.UpdatedAt=$now
$state.WorkInProgress.Scope=$scope
$state.WorkInProgress.PendingTest=$done
$state.Next=$next
$state.NextAction=$next
$state.ActiveTest=$done
$state.Doppelganger|Add-Member -Force NoteProperty WorkInProgress $state.WorkInProgress
$state.Doppelganger|Add-Member -Force NoteProperty Next $next
$state.Crouch|Add-Member -Force NoteProperty RegressionReports @($reports.Values)
$state.Crouch|Add-Member -Force NoteProperty DirectActionsRegressionPassed $true
$state.Crouch|Add-Member -Force NoteProperty SteamActionsRegressionPassed $true
$state.Crouch|Add-Member -Force NoteProperty AirborneRegressionPassed $true
$state.Live=@(foreach($r in 'host','client'){
 $l=Get-Content "$PSScriptRoot/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
 $s=Get-Content "$PSScriptRoot/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json
 if(!$s.remoteFresh-or$s.bodyStreamEnabled-or$s.localPlayerStance-ne0-or$s.localGameLogicState-ne1){throw 'Unexpected final lab state'}
 [pscustomobject]@{Role=$r;Pid=$l.pid;Epoch=$s.localEpoch;Handle=$s.probeActorHandle;Follow=$s.probeFollowEnabled;Exact=$s.probeFollowExact;Fresh=$s.remoteFresh;BodyStream=$s.bodyStreamEnabled;NativeCrouchReferenceReady=$s.nativeCrouchReferenceReady;NativeInterface=$s.nativeInterface;Locomotion=$s.locomotionEnabled;PoseSmoothing=$s.poseSmoothingEnabled;InterpolationMs=$s.poseInterpolationMs;Position=$s.localPosition;PlayerHealth=56;CloneHealth=40;HealthVerifiedAfterRegressions=$true}
})
$state|ConvertTo-Json -Depth 50|Set-Content "$PSScriptRoot/current-task-state.json"
Copy-Item "$PSScriptRoot/current-task-state.json" "$backup/state-after-regressions.json"
$supplement="$lab/analysis/ghidra/doppelganger-crouch-supplement.json"
$g=Get-Content $supplement -Raw|ConvertFrom-Json
$g.Confirmed+=@('9165 both roles pass whole-body crouch, walk and standing return with phase-3-complete observation bracketing.','9165 native pause, checkpoint invalidation, first-person, direct/Steam actions and airborne regressions pass. Visual pose remains incomplete; research only.')
$g.Hypotheses=@('Direct playback of player-body clips on the clone rig is not verified.','A better native combat-ready torso/arm reference may improve the full crouch composition; current search pose has a low sword and excessive Emily forward bend.','Crouched action arbitration, terrain/capsule behavior and full campaign use remain unverified.')
$g|ConvertTo-Json -Depth 15|Set-Content $supplement
$timing="$lab/analysis/ghidra/doppelganger-crouch-playback-timing.json"
$g=Get-Content $timing -Raw|ConvertFrom-Json
$g|Add-Member -Force NoteProperty RuntimeResolution '888A verifies float1.2 seconds/rate0 held reference on both rigs. 9165 verifies complete crouch composition mechanically on both roles. Reports distinguish mechanical pass from unfinished visual quality.'
$g|ConvertTo-Json -Depth 15|Set-Content $timing
Copy-Item "$lab/analysis/void-tools/doppelganger-crouch-investigation.md" "$backup/analysis/doppelganger-crouch-investigation.md"
Copy-Item $supplement "$backup/analysis/doppelganger-crouch-supplement.json"
Copy-Item $timing "$backup/analysis/doppelganger-crouch-playback-timing.json"
Copy-Item "$lab/Test-D2ShadowActions.ps1" "$backup/Test-D2ShadowActions.ps1"
Copy-Item "$lab/research/doppelganger-actions-9165-overlap-fixture.json" "$backup/research/doppelganger-actions-9165-overlap-fixture.json"
[ordered]@{At=$now;Dll=$sha;StableMilestone='work/doppelganger-smoothing-424B';Scope=$scope;Validation=$done;Next=$next;DefaultEnabled=$false;VisualComplete=$false;OriginalSteamModified=$false;CompleteCoop=$false}|ConvertTo-Json -Depth 5|Set-Content "$backup/scope.json"
$paths=@('src','bin','research','analysis')|ForEach-Object {Get-ChildItem -LiteralPath "$backup/$_" -File -Recurse}
$manifest=@($paths|ForEach-Object{[pscustomobject]@{Path=[IO.Path]::GetRelativePath($backup,$_.FullName);Sha256=(Get-FileHash -LiteralPath $_.FullName).Hash}})
$manifest|ConvertTo-Json -Depth 4|Set-Content "$backup/research-code-manifest.json"
[pscustomobject]@{Candidate=$sha;ReportsPinned=$reports.Count;SnapshotFilesHashed=$manifest.Count;DefaultEnabled=$false;Live=$state.Live}
