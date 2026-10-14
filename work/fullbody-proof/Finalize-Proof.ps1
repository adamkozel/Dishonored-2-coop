$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab'
$dll=(Get-FileHash "$lab/bin/dinput8.dll").Hash;$clip=(Get-FileHash "$PSScriptRoot/corvo-guard-proof.bin").Hash
if($dll-ne'F4F035D954BFC39F59E170E494C6ED1B0093E332C55167649D1C43F3C5A1817D'-or$clip-ne'109956C449EC4AB9DA9D68F944C173F844D391074B3A5D3310264B9F180484FF'){throw 'Artifact identity changed'}
$archive=Join-Path $PSScriptRoot 'milestone-F4F'
if(Test-Path $archive){throw 'Preserved milestone already exists; do not overwrite'}
[void](New-Item -ItemType Directory -Path $archive)
Copy-Item "$lab/src" "$archive/src" -Recurse
Copy-Item "$lab/bin/dinput8.dll","$lab/CMakeLists.txt","$lab/Build-D2Lab.ps1","$lab/Send-D2Command.ps1","$lab/Send-D2Input.ps1" $archive
foreach($name in 'inputs','clean-take-2'){Copy-Item "$PSScriptRoot/$name" "$archive/$name" -Recurse}
Get-ChildItem $PSScriptRoot -File | Where-Object {$_.Extension-in'.py','.ps1','.cpp','.txt','.bin','.blend','.mp4','.json' -and $_.Name-ne'LauncherInstalled-before-repair.dat'} | Copy-Item -Destination $archive
Copy-Item "$lab/research/player-arm-observation-verification.json","$lab/research/baseline-integrity-verification.json" $archive
$report=[ordered]@{
 at=[DateTimeOffset]::Now.ToString('o');dllSha256=$dll;clipSha256=$clip;layoutHashHex='00db32d37719931f'
 result='One complete Corvo forward advance, native-FP-inspired guard and recovery authored in Blender and played on the owned native D2 Doppelganger'
 pipeline='Offline full-body Blender authoring to validated native-skeleton local-pose clip; existing Steam-driven root; native correctives and equipment'
 productionIntegrated=$false;animGenUsed=$false;userVisualAcceptance='Pending user review of recorded proof, not a milestone approval gate'
 visualReview=@{
  evidence='controller-video-4, authored-ready/step/settled/guard/recovery images, final-authoring-guard.png'
  assessment='Useful limited proof; skin proportions coherent, support contacts retained, guard hand on hilt, complete-body action survives transfer'
  limitations=@('Restrained offhand and narrow stance still deserve artistic refinement','Existing relaxed native idle and open hand return outside the authored clip','Low capture cadence limits judder assessment','No Emily, direction changes, crouch, terrain or complete moveset acceptance')
 }
 runtimeMetrics='runtime-verification.json';runtimeSafety='safety-verification.json'
 additionalSafety=@{NoMovementExpirySeconds=10;ObservedActiveAfterExpiry=$false;FinalStatus=(Get-Content "$root/work/lab-instances/client/coop-status.json.authoring.json" -Raw|ConvertFrom-Json)}
 firstPersonRegression='milestone-F4F/player-arm-observation-verification.json';baselineIntegrity='milestone-F4F/baseline-integrity-verification.json'
 videos=@('d2-fullbody-sequence.mp4','blender-sequence.mp4');authoring='corvo-fullbody-authored.blend';instructions='README.txt'
 transport='Existing native SteamNetworkingSockets012 on one PC; separate-account Internet hosting/joining not established by this proof'
 preserved=@('work/friendly-fire-345C','work/sword-grip-fit/789d-grip-milestone','work/sword-grip-fit/2997-combined-research','work/fullbody-proof/milestone-F4F')
 blocker=@{component='UE 5.8.2 / Epic AnimGen';reason='Registered engine directory missing; repair IS-0002-DP-01';approvalReview='Rejected uninstall/removal because confirmation deletes engine data; cancelled, not retried';fallback='Blender 5.2.0 LTS';existingProjectsModified=$false}
 confirmedFindings=@('Native skinned 215-joint Corvo imported with preserved weights and rest axes','Set edit-bone length before assigning native matrix; full rest-matrix comparison catches inverse-bind corruption','Authoring clip must leave root/anchor identity and preserve anatomical local translations','Native corrective helper routine edits local and global matrices; final native pass required','Repeated native setviewpos is not a locomotion driver; controller-driven step removed observed vertical oscillation')
 hypotheses=@('Dynamic view-height subtraction plus deferred position event likely explains repeated-setviewpos height oscillation; exact causal path not completely traced')
 pending=@('Automatic selection by replicated movement/combat state and interruption arbitration','Convincing start/stop/turn/stance transition set','Emily adaptation and broad sword contact review','Native UI/session setup, campaign and complete no-friendly-fire coverage')
}
$report|ConvertTo-Json -Depth 10|Set-Content "$PSScriptRoot/result.json"
Copy-Item "$PSScriptRoot/result.json" "$archive/result.json"
Copy-Item "$PSScriptRoot/result.json" "$lab/analysis/ghidra/fullbody-authoring-proof.json"
$files=@(Get-ChildItem $archive -File -Recurse|ForEach-Object{[pscustomobject]@{Path=[IO.Path]::GetRelativePath($archive,$_.FullName);Sha256=(Get-FileHash $_.FullName).Hash;Bytes=$_.Length}})
$files|ConvertTo-Json -Depth 3|Set-Content "$archive/manifest.json"
$statePath=Join-Path $root 'work/current-task-state.json';Copy-Item $statePath "$archive/previous-task-state.json"
$state=Get-Content $statePath -Raw|ConvertFrom-Json
$state.UpdatedAt=[DateTimeOffset]::Now.ToString('o');$state.LoaderSha256=$dll
$state.Live=@(foreach($r in 'host','client'){$s=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json;$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;[pscustomobject]@{Role=$r;Pid=$l.pid;Epoch=$s.localEpoch;Handle=$s.probeActorHandle;Follow=$s.probeFollowEnabled;NativeInterface=$s.nativeInterface;BodyStream=$s.bodyStreamEnabled;AuthoringProof=(Get-Content "$root/work/lab-instances/$r/coop-status.json.authoring.json" -Raw|ConvertFrom-Json)}})
$state.ActiveTest=[pscustomobject]@{Dll=$dll;Fixture='Two isolated physical-copy Dunwall instances; native Steam sockets; windowed side by side';Experiment='Bounded Corvo full-body authoring proof';Status='Blender to native D2 playback verified and recorded; proof currently off; not a complete or automatically driven moveset';Reports=@('work/fullbody-proof/result.json','work/fullbody-proof/README.txt');FullMovesetReady=$false}
$state.WorkInProgress='One native first-person-inspired Corvo full-body proof delivered as editable Blender source and actual D2 recording. Verified movement-driven phase, native corrective/attachment publication, pause/cancel/expiry and original baseline. Previous crouched combat decoder bug remains unfixed and separate.'
$state.Next='Use the full-body proof to assess authoring quality before scaling; implement replicated action selection and interruption handling, then extend clip coverage. Do not claim AnimGen generation or a complete moveset.'
$state.AnimationStrategy.KnownPipeline.NativeSkinnedCharacterExportToAuthoringToolVerified=$true
$state.AnimationStrategy|Add-Member -NotePropertyName LatestProof -NotePropertyValue 'work/fullbody-proof/result.json' -Force
$state.AnimationStrategy|Add-Member -NotePropertyName UnrealBlocker -NotePropertyValue $report.blocker -Force
$state|Add-Member -NotePropertyName FullbodyProof -NotePropertyValue $report -Force
$state|ConvertTo-Json -Depth 30|Set-Content $statePath
$preflight=Get-Content "$root/work/animgen-feasibility/preflight.json" -Raw|ConvertFrom-Json
$preflight.KnownPipeline.NativeSkinnedCharacterExportToAuthoringToolVerified=$true
$preflight|Add-Member -NotePropertyName LatestProof -NotePropertyValue 'work/fullbody-proof/result.json' -Force
$preflight|Add-Member -NotePropertyName UnrealBlocker -NotePropertyValue $report.blocker -Force
$preflight|ConvertTo-Json -Depth 12|Set-Content "$root/work/animgen-feasibility/preflight.json"
[pscustomobject]@{Archive=$archive;Files=$files.Count;RuntimeDll=$dll;Clip=$clip;Report="$PSScriptRoot/result.json"}
