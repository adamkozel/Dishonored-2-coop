$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$archive=Join-Path $PSScriptRoot 'candidate-domino-target-scoped-v201'
if(Test-Path $archive){throw 'Archive already exists'}
New-Item -ItemType Directory $archive|Out-Null
Copy-Item "$lab/src" "$archive/src" -Recurse
Copy-Item "$root/work/asset-game/dinput8.dll" "$archive/dinput8.dll"
Copy-Item "$root/work/build/Release/d2_runtime_probe.exe" "$archive/d2_runtime_probe.exe"
foreach($file in 'Invoke-NativeLabOperation.ps1','domino-target-scoped-build-v201.log','domino-target-scoped-build-v201.log','domino-target-scoped-identity-v201.log','prepare_domino_target_probe_v199.py','prepare_domino_target_test_v199.py'){Copy-Item (Join-Path $PSScriptRoot $file) $archive}
Copy-Item "$root/work/natural-moveset/Observe-NativeDominoTargets-v199.ps1" $archive
[ordered]@{created=(Get-Date).ToString('o');dll=(Get-FileHash "$archive/dinput8.dll").Hash;probe=(Get-FileHash "$archive/d2_runtime_probe.exe").Hash;protocol=44;mesmerizeWorldProtocol=4;bankVersion=47;clips=289;runtimeVerified=$false;scope='Probe-only scoped Domino ownership reader; cached discovery revalidates native inventory, owner, generation, inline parent and logic on every sample, independent of first-person pose. Equipment-derived entity/component, native selected pool, animation generation, target/effect identities and two-frame retirement checked. Production DLL and native Steam stream unchanged from candidate193. Synthetic identity tests pass; v200 full discovery reader passed nine checks with212 mark samples; scoped performance verification pending.'}|ConvertTo-Json -Depth 6|Set-Content "$archive/identity.json"
Copy-Item "$archive/d2_runtime_probe.exe" "$lab/bin/d2_runtime_probe.exe"
Get-Content "$archive/identity.json"

