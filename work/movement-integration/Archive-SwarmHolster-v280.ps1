$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$archive="$PSScriptRoot/candidate-swarm-holster-v280"
if(Test-Path $archive){throw 'Archive exists'}
foreach($kind in 'build','identity','smoke'){$p="$PSScriptRoot/swarm-holster-$kind-v280.log";if(!(Test-Path $p)-or!(Get-Item $p).Length-or(Select-String $p -Pattern '^FAIL|error C[0-9]|error LNK[0-9]')){throw "Unverified log $kind"}}
New-Item -ItemType Directory $archive|Out-Null;Copy-Item "$lab/src" "$archive/src" -Recurse
foreach($binary in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item "$root/work/build/Release/$binary" "$archive/$binary"}
foreach($kind in 'build','identity','smoke'){Copy-Item "$PSScriptRoot/swarm-holster-$kind-v280.log" $archive}
Copy-Item "$PSScriptRoot/Invoke-NativeLabOperation.ps1" $archive;Copy-Item "$root/work/natural-moveset/Test-SwarmContextNetwork-v276.ps1" $archive
Copy-Item "$root/work/natural-moveset/swarm-network-standing-holstered-v277/result.json" "$archive/retained-holster-failure-v277.json"
Copy-Item "$root/work/natural-moveset/swarm-native-world-holster-observe-v279/result.json" "$archive/retained-world-observer-failure-v279.json"
$identity=Get-Content "$PSScriptRoot/candidate-swarm-context-v276/identity.json" -Raw|ConvertFrom-Json
$identity.created=(Get-Date).ToString('o');$identity.dll=(Get-FileHash "$archive/dinput8.dll").Hash;$identity.probe=(Get-FileHash "$archive/d2_runtime_probe.exe").Hash
$identity.scope='Extends candidate276 only with native Corvo simultaneous power/sword holster support. Source v279 raw scenes establish exact state64 power_unequip Scene_1/333 with reciprocal Corvo left arm and separate state53 sword_unequip Scene_2/1266 right-arm/sword pair. Requires matching character across both arms; ownership, clocks, scene uniqueness and stable rereads retained. Fixes v277 native_state_stale during holster, without relaxing timeout. Identity tests cover both roles, foreign arms, aliases, expired scenes and retirement races. Wire47, bank50/313 and all animation bytes unchanged. Build/identity/load checks pass; native paired verification pending. v279 world inspection failed on an inactive pool slot; its holster evidence and cleanup are preserved separately.'
$identity|ConvertTo-Json -Depth 8|Set-Content "$archive/identity.json"
foreach($binary in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item "$archive/$binary" "$lab/bin/$binary"}
$identity|Select-Object dll,probe,protocol,bankVersion|ConvertTo-Json
