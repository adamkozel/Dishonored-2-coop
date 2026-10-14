$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$dll="$root/outputs/d2-coop-lab/bin/dinput8.dll"
$hash=(Get-FileHash $dll).Hash
if($hash-ne'CF60C5EAB9A08573F128F2EF3958F9237BC92FCDC5B551682601DF44C6BDBD46'){throw 'Unexpected milestone DLL'}
$out=Join-Path $PSScriptRoot 'milestone-CF60'
if(Test-Path $out){throw 'Milestone already exists'}
New-Item -ItemType Directory -Path $out|Out-Null
Copy-Item -LiteralPath $dll -Destination "$out/dinput8.dll"
foreach($r in 'host','client'){
 Copy-Item -LiteralPath "$root/work/lab-instances/$r/base" -Destination "$out/$r-base" -Recurse
 Copy-Item -LiteralPath "$root/work/lab-instances/$r/coop-status.json" -Destination "$out/$r-status.json"
 Copy-Item -LiteralPath "$root/work/lab-instances/$r/coop-status.json.movement.bin" -Destination "$out/$r-movement.bin"
}
foreach($p in 'gait-priority-1/result.json','gaits-emily-1/result.json','gaits-corvo-1/result.json','native-gait-manifest.json','review-roundtrip.json','library-format-validation.json'){
 Copy-Item -LiteralPath (Join-Path $PSScriptRoot $p) -Destination (Join-Path $out ($p.Replace('/','-')))
}
Copy-Item -LiteralPath "$root/outputs/d2-coop-lab/src" -Destination "$out/src" -Recurse
Get-ChildItem -LiteralPath $out -File -Recurse|Get-FileHash|ConvertTo-Json -Depth 4|Set-Content "$out/manifest.json"
$statePath="$root/work/current-task-state.json"
Copy-Item -LiteralPath $statePath -Destination "$out/prior-task-state.json"
$state=Get-Content $statePath -Raw|ConvertFrom-Json
$state.UpdatedAt=[DateTimeOffset]::Now.ToString('o');$state.LoaderSha256=$hash
$natural=[ordered]@{
 Scope='Natural full-body moveset. First-person data is reference only; no fixed first-person wrist constraints.'
 CompleteMoveset=$false
 Milestone='natural-moveset/milestone-CF60'
 ExactExecutableSha256='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180'
 NativeSourceClipsSampled=1453
 NativeCandidateClips=354
 PairedRolePreviewVideos=177
 NativeLibraryManifest='natural-moveset/library-retarget-manifest.json'
 ValidPreviewFolder='natural-moveset/library-videos-v3'
 RejectedPreviewFolders=@('natural-moveset/library-videos','natural-moveset/library-videos-v2')
 Runtime='D2MOVE02: armed idle and eight-direction walking/running, shared distance phase, native action priority, isolated 60-second lease'
 RuntimeTests=@('natural-moveset/gaits-emily-1/result.json','natural-moveset/gait-priority-1/result.json','natural-moveset/review-roundtrip.json')
 CorvoTestCaveat='gaits-corvo-1 has 16 valid directional measurements but overall failed because the obsolete priority test used the wrong block button. Corrected separate priority suite passes both roles.'
 ValidGamePoseRecordings=@('natural-moveset/native-corvo-1.mp4','natural-moveset/native-emily-1.mp4')
 GameRecordingCaveat='Initial videos are pose-only. New travelling takes failed capture handshake/window capture and are excluded.'
 VisualAcceptance='Pending; source availability and numerical runtime checks do not imply acceptable complete animation.'
 Pending='Full 105-item functional library, missing authored actions, equipment/contact adaptation, gameplay binding, transition validation and visual review.'
 PreservedOriginalAssets=$true
}
$state|Add-Member -NotePropertyName NaturalFullBodyMoveset -NotePropertyValue $natural -Force
$state.Live=@(foreach($r in 'host','client'){$b=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json;[ordered]@{Role=$r;Status=$b}})
$state|ConvertTo-Json -Depth 30|Set-Content $statePath
'NATURAL_MILESTONE_PRESERVED'
