$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$archive="$PSScriptRoot/candidate-swarm-world-probe-v286"
if(Test-Path $archive){throw 'Archive exists'}
foreach($kind in 'build','identity'){$p="$PSScriptRoot/swarm-world-probe-$kind-v286.log";if(!(Test-Path $p)-or!(Get-Item $p).Length-or(Select-String $p -Pattern '^FAIL|error C[0-9]|error LNK[0-9]')){throw "Unverified log $kind"}}
if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne'06E1DA2C679CFF6E420F9BCA8E0E2B3558F2CFB399A21D982C91DE5D9E1A0747'){throw 'Live proxy changed'}
New-Item -ItemType Directory $archive|Out-Null;Copy-Item "$lab/src" "$archive/src" -Recurse
Copy-Item "$root/work/asset-game/dinput8.dll" "$archive/dinput8.dll";Copy-Item "$root/work/build/Release/d2_runtime_probe.exe" "$archive/d2_runtime_probe.exe"
Copy-Item "$PSScriptRoot/swarm-world-probe-*-v286*.log" $archive;Copy-Item "$PSScriptRoot/prepare_swarm_world_*v286.py" $archive
Copy-Item "$PSScriptRoot/Invoke-NativeLabOperation.ps1" $archive;Copy-Item "$root/work/natural-moveset/Observe-NativeSwarmWorld-v286.ps1" $archive
$identity=Get-Content "$PSScriptRoot/candidate-swarm-holster-v280/identity.json" -Raw|ConvertFrom-Json
$identity.created=(Get-Date).ToString('o');$identity.dll=(Get-FileHash "$archive/dinput8.dll").Hash;$identity.probe=(Get-FileHash "$archive/d2_runtime_probe.exe").Hash
$identity.scope='Read-only Swarm world probe added alongside unchanged candidate280 proxy/wire47/bank50. Equipment committed handle, world/component exact identities, native pool index list, reciprocal rat/entity/frame/animation owners, generations, capacity and stable transforms validated. New identity tests pass including nonzero pool slots, bounds, foreign owners and retirement races. Initial test mistaken expectation that a stable new valid handle must be refused is retained and corrected; stable new handles are discovered as new identities. Live world verification pending. No remote rat/VFX/AI/damage implementation.'
$identity|ConvertTo-Json -Depth 8|Set-Content "$archive/identity.json"
Copy-Item "$archive/d2_runtime_probe.exe" "$lab/bin/d2_runtime_probe.exe"
$identity|Select-Object dll,probe,protocol,bankVersion|ConvertTo-Json
