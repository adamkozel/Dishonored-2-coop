$ErrorActionPreference='Stop';$root=Split-Path $PSScriptRoot;$lab="$root/outputs/d2-coop-lab"
$expected='6FDE1885980C35A48E3EDB2CDC1FFD905BE95DB501637FB718E3CEDC209B265F'
if((Get-FileHash "$lab/bin/dinput8.dll").Hash-ne$expected){throw 'Unexpected build'}
$destination="$root/work/doppelganger-crouch-relative-6FDE-research"
if(Test-Path $destination){throw 'Preserved candidate already exists; do not overwrite'}
$reports=@('doppelganger-crouch-relative-6FDE.json','doppelganger-crouch-relative-6FDE-component-metric.json','doppelganger-crouch-relative-6FDE-angular-reanalysis.json','doppelganger-crouch-guards-6FDE.json','doppelganger-firstperson-6FDE.json','doppelganger-actions-6FDE.json','doppelganger-steam-actions-6FDE-cold-start.json','doppelganger-steam-actions-6FDE-fixture-damage.json','doppelganger-crouch-directions-6FDE.json','doppelganger-crouch-baseline-6FDE.json','doppelganger-firstperson-damage-observation-6FDE.json')
foreach($name in $reports){if(!(Test-Path "$lab/research/$name")){throw "Missing report: $name"}}
New-Item -ItemType Directory -Path "$destination/bin","$destination/research","$destination/installed-assets"|Out-Null
Copy-Item "$lab/src","$lab/assets","$lab/tooling" $destination -Recurse
Copy-Item "$lab/bin/dinput8.dll","$lab/bin/d2_runtime_probe.exe" "$destination/bin"
Get-ChildItem $lab -File -Filter '*.ps1'|Copy-Item -Destination $destination
foreach($name in $reports){Copy-Item "$lab/research/$name" "$destination/research/$name"}
$prior=Get-Content "$root/work/doppelganger-crouch-wholebody-9165-research/installed-assets-manifest.json" -Raw|ConvertFrom-Json
$files=@(foreach($item in $prior){
 if($item.Name-notmatch'^(game[1-4]voidmod\.(index|resources)|master\.index)$'){throw 'Unexpected asset filename'}
 $src=Join-Path "$root/work/asset-game/base" $item.Name;$dst=Join-Path "$destination/installed-assets" $item.Name
 Copy-Item -LiteralPath $src -Destination $dst
 $hash=(Get-FileHash $src).Hash;if((Get-FileHash $dst).Hash-ne$hash){throw 'Asset snapshot hash mismatch'}
 [pscustomobject]@{Name=$item.Name;Hash=$hash}
})
$files|ConvertTo-Json|Set-Content "$destination/installed-assets-manifest.json"
[ordered]@{At=[DateTimeOffset]::Now.ToString('o');Dll=$expected;StableMilestone='work/doppelganger-smoothing-424B';ResearchOnly=$true;NativeReferenceMilliseconds=600;NativeHelperRva='0x23fca0';SourceReference='Settled grounded crouch; per actor, skeleton, sessions and worlds';Reports=$reports;SteamRegression='Unresolved: failed cases followed by dead clones. First-person passive repeat retained clone health 40; later source remains undiagnosed.';Remaining=@('User-requested upright torso A/B','Visual gait and foot planting polish','Crouched attacks and parries','Uneven terrain and collision capsule agreement','Automatic campaign reference setup') }|ConvertTo-Json -Depth 5|Set-Content "$destination/scope.json"
Get-ChildItem $destination -File -Recurse|Where-Object Name -ne 'files-manifest.json'|ForEach-Object {[pscustomobject]@{File=$_.FullName.Substring($destination.Length+1);Sha256=(Get-FileHash $_.FullName).Hash}}|ConvertTo-Json|Set-Content "$destination/files-manifest.json"
[pscustomobject]@{Preserved=$destination;Dll=$expected;InstalledAssets=$files.Count}
