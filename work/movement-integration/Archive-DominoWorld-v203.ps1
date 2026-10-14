$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$archive=Join-Path $PSScriptRoot 'candidate-domino-world-v203'
if(Test-Path $archive){throw 'Archive already exists'}
New-Item -ItemType Directory $archive|Out-Null
Copy-Item "$lab/src" "$archive/src" -Recurse
foreach($binary in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item "$root/work/build/Release/$binary" (Join-Path $archive $binary)}
foreach($file in 'Invoke-NativeLabOperation.ps1','domino-world-build-v203.log','domino-world-wire-v203.log','domino-world-smoke-v203.log','domino-target-scoped-identity-v201.log','prepare_domino_world_v203.py','prepare_domino_world_test_v203.py'){Copy-Item (Join-Path $PSScriptRoot $file) $archive}
Copy-Item "$root/work/natural-moveset/Test-DominoWorld-v203.ps1" $archive
[ordered]@{created=(Get-Date).ToString('o');dll=(Get-FileHash "$archive/dinput8.dll").Hash;probe=(Get-FileHash "$archive/d2_runtime_probe.exe").Hash;protocol=44;mesmerizeWorldProtocol=4;dominoWorldProtocol=1;bankVersion=47;clips=289;runtimeVerified=$false;scope='Native Domino target marks use a new bounded Steam world snapshot, scoped source ownership, source clock/tokens/epoch/root validation and receiver-owned native validated FX. Handles are opaque and positions remain source world coordinates. No native Domino/NPC/link/damage calls on receiver. Body/bank unchanged. Read-only scoped native reader v202 has420 mark samples, median0.53ms external cached reads. Ownership/wire tests and proxy smoke pass; first paired FX test pending.'}|ConvertTo-Json -Depth 6|Set-Content "$archive/identity.json"
foreach($binary in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item (Join-Path $archive $binary) (Join-Path "$lab/bin" $binary)}
Get-Content "$archive/identity.json"
