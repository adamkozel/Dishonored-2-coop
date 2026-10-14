$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent;$lab=Join-Path $root 'outputs/d2-coop-lab';$dest=Join-Path $PSScriptRoot 'friendly-fire-345C'
$hash='345CD1D409BE275C6817C96C5E5C8F5CB64FDCFAF88715B2E4DFBAB5E82CE303'
if((Get-FileHash "$lab/bin/dinput8.dll").Hash-ne$hash){throw 'Unexpected build'}
if(Test-Path -LiteralPath $dest){throw 'Milestone already exists; do not overwrite'}
$reports=@('friendly-fire-native-first-345C.json','friendly-fire-native.json','friendly-fire-strikes.json','native-sword-damage-verification.json','native-enemy-damage-verification.json','baseline-integrity-verification.json','doppelganger-crouch-live-enable.json','mixamo-crouch-feasibility.json','blender-mcp-working-versions.json')
foreach($name in $reports[0..5]){$report=Get-Content "$lab/research/$name" -Raw|ConvertFrom-Json;if(!$report.Passed){throw "Failed report $name"}}
$live=@(foreach($role in 'host','client'){
 $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
 if((Get-Process -Id $l.pid).Path-ne$l.executable-or!$l.assetExperiment){throw 'Unexpected test process'}
 $s=Get-Content "$root/work/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json
 $f=Get-Content "$root/work/lab-instances/$role/coop-status.json.damage.json" -Raw|ConvertFrom-Json
 $raw=& "$lab/bin/d2_runtime_probe.exe" $l.pid --health 1;if($LASTEXITCODE){throw 'Player health unavailable'};$player=$raw|ConvertFrom-Json
 $raw=& "$lab/bin/d2_runtime_probe.exe" $l.pid --health $s.probeActorHandle;if($LASTEXITCODE){throw 'Avatar health unavailable'};$avatar=$raw|ConvertFrom-Json
 if($player.current-ne56-or$avatar.current-ne40-or!$f.ownedAvatarValid-or!$s.remoteFresh-or!$s.bodyStreamEnabled-or!$s.nativeCrouchReferenceReady){throw 'Final live state differs from tested fixture'}
 [ordered]@{Role=$role;Pid=$l.pid;Epoch=$s.localEpoch;Handle=$s.probeActorHandle;Follow=$s.probeFollowEnabled;Exact=$s.probeFollowExact;Fresh=$s.remoteFresh;BodyStream=$s.bodyStreamEnabled;Upright=$s.bodyCrouchUpright;NativeCrouchReferenceReady=$s.nativeCrouchReferenceReady;NativeInterface=$s.nativeInterface;PlayerHealth=$player.current;CloneHealth=$avatar.current;FriendlyFireLocalBlocked=$f.friendlyFireLocalBlocked;FriendlyFirePartnerBlocked=$f.friendlyFirePartnerBlocked;OwnedAvatarValid=$f.ownedAvatarValid}
})
New-Item -ItemType Directory -Path $dest,"$dest/research"|Out-Null
foreach($folder in 'src','assets','tooling','bin'){Copy-Item -LiteralPath "$lab/$folder" -Destination $dest -Recurse}
Get-ChildItem -LiteralPath $lab -Filter '*.ps1'|Copy-Item -Destination $dest
Copy-Item -LiteralPath "$lab/CMakeLists.txt","$lab/analysis/ghidra/friendly-fire.md" -Destination $dest
foreach($name in $reports){Copy-Item -LiteralPath "$lab/research/$name" -Destination "$dest/research"}
Copy-Item -LiteralPath "$root/work/friendly-fire-build.log" -Destination $dest
$scope=[ordered]@{At=[DateTimeOffset]::Now.ToString('o');Dll=$hash;Live=$live;ResearchOnly=$true;Scope='Owned-player damage attribution firewall before native health handling, including real sword-contact checks in both roles. Every power/indirect hazard and pre-damage incapacitation remain unverified.';StableMilestone='work/doppelganger-smoothing-424B';Previous='work/doppelganger-crouch-upright-303A-research';IdenticalPreservedAssets='work/doppelganger-crouch-relative-6FDE-research/installed-assets';Reports=$reports}
$scope|ConvertTo-Json -Depth 12|Set-Content "$dest/scope.json"
Get-ChildItem -LiteralPath $dest -Recurse -File|ForEach-Object{[ordered]@{Path=$_.FullName.Substring($dest.Length+1);Sha256=(Get-FileHash -LiteralPath $_.FullName).Hash}}|ConvertTo-Json -Depth 5|Set-Content "$dest/files-manifest.json"
$state=Get-Content "$root/work/current-task-state.json" -Raw|ConvertFrom-Json
$state.UpdatedAt=$scope.At;$state.LoaderSha256=$hash;$state.Live=$live
$state|Add-Member -Force NoteProperty FriendlyFireRequirement ([ordered]@{DisabledRequired=$true;ImplementedForAttributedNativeHealthDamage=$true;FullNoHarmCoverageComplete=$false;NativeAndRealSwordChecksPassed=$true;NormalEnemyDamagePassed=$true;ReloadPassed=$true;Milestone='work/friendly-fire-345C';Report='analysis/ghidra/friendly-fire.md';Remaining=@('Every projectile and power','Unattributed indirect/physics hazards','Non-health incapacitation and pre-damage takedown choreography')})
$state|Add-Member -Force NoteProperty MixamoExperiment ([ordered]@{Status='Awaiting Adobe sign-in; exact non-rifle clip and settings verified';Clip='Crouched Walking';Description='Crouched Walk';InPlaceRequired=$true;UserLikesPreview=$true;BlenderMcpConnected=$true;DonorDownloaded=$false;RuntimeImprovementProven=$false;Report='research/mixamo-crouch-feasibility.json'})
$state|Add-Member -Force NoteProperty WorkInProgress 'Friendly-fire build 345C verified and preserved. Blender MCP connection recovered by starting existing Blender. User wants Mixamo crouch preview; waiting for Adobe sign-in before donor download. Existing crouch unchanged; complete no-harm coverage still requires powers/indirect-hazard and incapacitation tests.'
$state|Add-Member -Force NoteProperty Next 'After Mixamo sign-in, download the exact in-place Crouched Walk donor, record export hash/settings and prove offline import/playback before bounded one-rig retargeting. Preserve 345C and native first-person animation.'
$state|ConvertTo-Json -Depth 40|Set-Content "$root/work/current-task-state.json"
$scope|ConvertTo-Json -Depth 12|Set-Content "$lab/research/friendly-fire-live-345C.json"
[pscustomobject]@{Preserved=$dest;Dll=$hash;Instances=$live.Count}
