$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$archive="$PSScriptRoot/candidate-swarm-world-probe-v287"
if(Test-Path $archive){throw 'Archive exists'}
foreach($kind in 'build','identity'){$p="$PSScriptRoot/swarm-world-probe-$kind-v287.log";if(!(Test-Path $p)-or!(Get-Item $p).Length-or(Select-String $p -Pattern '^FAIL|error C[0-9]|error LNK[0-9]')){throw "Unverified log $kind"}}
if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne'06E1DA2C679CFF6E420F9BCA8E0E2B3558F2CFB399A21D982C91DE5D9E1A0747'){throw 'Live proxy changed'}
New-Item -ItemType Directory $archive|Out-Null;Copy-Item "$lab/src" "$archive/src" -Recurse
Copy-Item "$root/work/asset-game/dinput8.dll" "$archive/dinput8.dll";Copy-Item "$root/work/build/Release/d2_runtime_probe.exe" "$archive/d2_runtime_probe.exe"
Copy-Item "$PSScriptRoot/swarm-world-probe-*-v287.log" $archive
Copy-Item "$PSScriptRoot/Invoke-NativeLabOperation.ps1" $archive;Copy-Item "$root/work/natural-moveset/Observe-NativeSwarmWorld-v286.ps1" $archive
Copy-Item "$root/work/natural-moveset/swarm-native-world-reader-v286/result.json" "$archive/retained-world-parent-failure-v286.json"
$identity=Get-Content "$PSScriptRoot/candidate-swarm-world-probe-v286/identity.json" -Raw|ConvertFrom-Json
$identity.created=(Get-Date).ToString('o');$identity.probe=(Get-FileHash "$archive/d2_runtime_probe.exe").Hash
$identity.scope='Read-only probe286 corrected to require committed Swarm entity parent handle1, matching the actual native local-caster relationship seen in v286 world-declaration captures. The earlier unparented-world assumption caused all244 post-cast reads to fail stage2 and is retained as a failed experiment; cleanup/protected14 passed. Tests now refuse an unparented swarm and retain all pool/agent/race checks. Build and identity tests pass; live verification pending. Candidate280 proxy, wire47 and bank50/313 unchanged. No remote rats, AI or damage enabled.'
$identity|ConvertTo-Json -Depth 8|Set-Content "$archive/identity.json"
Copy-Item "$archive/d2_runtime_probe.exe" "$lab/bin/d2_runtime_probe.exe"
$identity|Select-Object dll,probe,protocol,bankVersion|ConvertTo-Json
