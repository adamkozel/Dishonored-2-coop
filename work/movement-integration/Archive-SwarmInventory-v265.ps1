$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$archive=Join-Path $PSScriptRoot 'candidate-swarm-inventory-v265'
if(Test-Path $archive){throw 'Archive already exists'}
New-Item -ItemType Directory $archive|Out-Null
Copy-Item "$lab/src" "$archive/src" -Recurse
foreach($binary in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item "$root/work/build/Release/$binary" (Join-Path $archive $binary)}
foreach($file in 'Invoke-NativeLabOperation.ps1','swarm-inventory-build-v265.log','swarm-inventory-build-v265-initial-failure.log','swarm-inventory-identity-v265.log','swarm-inventory-wire-v265.log','swarm-inventory-smoke-v265.log','inspect_static_rtti_v265.py','prepare_swarm_inventory_v265.py','swarm-equipment-rtti-v265.json','swarm-declaration-rtti-v265.json'){Copy-Item (Join-Path $PSScriptRoot $file) $archive}
Copy-Item "$root/work/natural-moveset/Observe-NativeSwarm-v265.ps1" $archive
[ordered]@{created=(Get-Date).ToString('o');dll=(Get-FileHash "$archive/dinput8.dll").Hash;probe=(Get-FileHash "$archive/d2_runtime_probe.exe").Hash;protocol=45;mesmerizeWorldProtocol=4;dominoWorldProtocol=2;dominoWorldBytes=1000;bankVersion=48;clips=301;runtimeVerified=$false;scope='Preserves candidate264 holstered Domino final recovery with both paired contexts PASS12. Adds opt-in read-only Corvo Swarm item probe and finite native-inventory fixture, exact static RTTI component1b51f28/declaration1b51de8 and Void names, character1, reciprocal ownership/rereads. Existing serialized lab isolation and native inventory APIs only. Does not call swarm spawn, damage or AI actions directly and adds no remote Swarm binding. First build exposed two accidentally copied Domino-specific test calls; corrected before deployment, failed log preserved. Build/identity/wire/load pass. Native runtime ownership and phases pending.'}|ConvertTo-Json -Depth 6|Set-Content "$archive/identity.json"
foreach($binary in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item (Join-Path $archive $binary) (Join-Path "$lab/bin" $binary)}
Get-Content "$archive/identity.json"
