$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot;$lab=Join-Path $root 'outputs/d2-coop-lab'
$expected='303AF7E16B85CF061195163FA557D35FDFEEBEC65CB2C1EF2620D0224BF9B2C1'
if((Get-FileHash "$lab/bin/dinput8.dll").Hash-ne$expected){throw 'Unexpected candidate'}
$destination=Join-Path $root 'work/doppelganger-crouch-upright-303A-research'
if(Test-Path $destination){throw 'Never replace an archived candidate'}
New-Item -ItemType Directory -Path "$destination/bin","$destination/research"|Out-Null
Copy-Item "$lab/src","$lab/assets","$lab/tooling" $destination -Recurse
Copy-Item "$lab/bin/dinput8.dll","$lab/bin/d2_runtime_probe.exe" "$destination/bin"
Get-ChildItem $lab -File -Filter '*.ps1'|Copy-Item -Destination $destination
$reports=@(Get-ChildItem "$lab/research" -File |Where-Object {$_.Name-match'303A'})
foreach($r in $reports){Copy-Item $r.FullName "$destination/research"}
$prior=Get-Content "$root/work/doppelganger-crouch-relative-6FDE-research/installed-assets-manifest.json" -Raw|ConvertFrom-Json
foreach($a in $prior){if($a.Name-notmatch'^(game[1-4]voidmod\.(index|resources)|master\.index)$'){throw 'Unexpected asset manifest path'};if((Get-FileHash "$root/work/asset-game/base/$($a.Name)").Hash-ne$a.Hash){throw 'Asset pack differs from preserved 6FDE pack'}}
Copy-Item "$lab/analysis/void-tools/doppelganger-crouch-investigation.md" $destination
[ordered]@{At=[DateTimeOffset]::Now.ToString('o');Dll=$expected;ResearchOnly=$true;StableMilestone='work/doppelganger-smoothing-424B';NativeAlternative='work/doppelganger-crouch-relative-6FDE-research';IdenticalPreservedAssets='work/doppelganger-crouch-relative-6FDE-research/installed-assets';Scope='User-requested upright torso comparison. Calibrated crouch legs and lowered pelvis; native standing spine/head/arms, attached spine compensation, native correctives. Full moveset and crouched combat are incomplete.';Reports=@($reports.Name);FriendlyFire='Required in both directions, implementation deferred by user.'}|ConvertTo-Json -Depth 5|Set-Content "$destination/scope.json"
Get-ChildItem $destination -Recurse -File|ForEach-Object {[pscustomobject]@{File=$_.FullName.Substring($destination.Length+1);Hash=(Get-FileHash $_.FullName).Hash}}|ConvertTo-Json|Set-Content "$destination/files-manifest.json"
[pscustomobject]@{Preserved=$destination;Reports=$reports.Count;AssetHashesVerified=$prior.Count}
