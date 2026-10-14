$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath($PWD.Path)
$lab=Join-Path $root 'outputs/d2-coop-lab'
$destination=Join-Path $root 'work/doppelganger-smoothing-424B'
$expected='424BE19EA9BF87FD134787D99D1637E2A249CF1A827E51D77D5B28AE333A9B7D'
if(Test-Path -LiteralPath $destination){throw 'Milestone already exists; do not overwrite it'}
if((Get-FileHash "$lab/bin/dinput8.dll").Hash-ne$expected){throw 'Wrong loader'}
$reports=@('doppelganger-airborne-424B.json','doppelganger-first-person-424B.json','doppelganger-sword-stream-424B.json','doppelganger-airborne-guards-424B.json','doppelganger-native-actions-424B.json','doppelganger-recovery-424B.json','doppelganger-final-fixture-424B.json','baseline-integrity-verification.json','pose-smoothing-verification.json')
foreach($name in $reports){
 $report=Get-Content "$lab/research/$name" -Raw|ConvertFrom-Json
 if(!$report.Passed){throw "Missing passing evidence: $name"}
}
$old=Get-Content "$root/work/doppelganger-airborne-7D7A/manifest.json" -Raw|ConvertFrom-Json
$assets=@(foreach($asset in $old.Assets){
 $backup=Join-Path "$root/work/doppelganger-airborne-7D7A/installed-assets" ([IO.Path]::GetFileName($asset.Path))
 if((Get-FileHash -LiteralPath $asset.Path).Hash-ne$asset.Hash-or(Get-FileHash -LiteralPath $backup).Hash-ne$asset.Hash){throw 'Installed asset or preserved backup changed'}
 [pscustomobject]@{Path=$asset.Path;Hash=$asset.Hash;Backup=$backup}
})
New-Item -ItemType Directory -Path $destination|Out-Null
foreach($name in 'src','bin','assets','tooling','third_party'){Copy-Item -LiteralPath "$lab/$name" -Destination $destination -Recurse}
Get-ChildItem -LiteralPath $lab -File|Where-Object {$_.Extension-eq'.ps1'-or$_.Name-eq'CMakeLists.txt'}|Copy-Item -Destination $destination
New-Item -ItemType Directory -Path "$destination/evidence","$destination/analysis/void-tools","$destination/analysis/ghidra" -Force|Out-Null
Get-ChildItem -LiteralPath "$lab/research" -File|Where-Object {$_.Name-like'*424B*'-or$_.Name-like'pose-cadence-*'-or$_.Name-like'doppelganger-recovery-*-native.json'-or$_.Name-in$reports-or$_.Name-eq'doppelganger-asset-install.json'}|Copy-Item -Destination "$destination/evidence"
Get-ChildItem -LiteralPath "$root/work" -File|Where-Object {$_.Name-like'pose-smoothing*.log'-or$_.Name-in@('FinalizePoseSmoothing.ps1','PreservePoseSmoothing.ps1')}|Copy-Item -Destination "$destination/evidence"
foreach($name in 'doppelganger-investigation.md','doppelganger-airborne-assets.json','shadow-movement-asset-catalog.json','shadow-crouch-asset-catalog.json','toolchain-lock.json'){Copy-Item -LiteralPath "$lab/analysis/void-tools/$name" -Destination "$destination/analysis/void-tools"}
Copy-Item -LiteralPath "$lab/analysis/ghidra/doppelganger-airborne-and-presentation.json" -Destination "$destination/analysis/ghidra"
Copy-Item -LiteralPath "$root/work/current-task-state.json" -Destination $destination
foreach($role in 'host','client'){
 New-Item -ItemType Directory -Path "$destination/isolated-profiles/$role" -Force|Out-Null
 Copy-Item -LiteralPath "$root/work/lab-instances/$role/base" -Destination "$destination/isolated-profiles/$role" -Recurse
 Copy-Item -LiteralPath "$root/work/lab-instances/$role/launch.json" -Destination "$destination/isolated-profiles/$role"
}
$manifest=[ordered]@{
 At=[DateTimeOffset]::Now.ToString('o');Loader=$expected;CompleteCoop=$false
 Scope='80ms remote XYZ/orientation interpolation; matched animation transition delay; native airborne/landing, sword, pause and fixed Dunwall recovery regressions. Local first-person unchanged. Original Steam and baseline saves preserved.'
 MotionComparisonBuild='80078F9F68B055D7F8A5D918ACB8D1F548252378A08CF53AD8A51FE834BF073A'
 MotionComparisonScope='Identical root interpolation code before 424B added animation-transition delay. Transform callback cadence, not display FPS.'
 Assets=$assets;AssetsBackup='work/doppelganger-airborne-7D7A/installed-assets'
 Reports=$reports
 Files=@(Get-ChildItem -LiteralPath $destination -File -Recurse|ForEach-Object {[pscustomobject]@{Path=[IO.Path]::GetRelativePath($destination,$_.FullName);Sha256=(Get-FileHash -LiteralPath $_.FullName).Hash}})
}
$manifest|ConvertTo-Json -Depth 8|Set-Content "$destination/manifest.json"
[pscustomobject]@{Saved=$destination;Files=$manifest.Files.Count;AssetsVerified=$assets.Count;Loader=$expected}
