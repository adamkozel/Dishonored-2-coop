$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$archive=Join-Path $PSScriptRoot 'candidate-domino-network-v193'
if(Test-Path $archive){throw 'Archive already exists'}
New-Item -ItemType Directory $archive|Out-Null
Copy-Item "$lab/src" "$archive/src" -Recurse
foreach($binary in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item "$root/work/build/Release/$binary" (Join-Path $archive $binary)}
foreach($file in 'Invoke-NativeLabOperation.ps1','domino-network-build-v193.log','domino-network-identity-v193.log','domino-network-wire-v193.log','domino-network-smoke-v193.log','domino-network-realbanks-v193.log','domino-network-allbanks-v193.log','prepare_domino_network_v193.py','prepare_domino_network_test_v193.py'){
 Copy-Item (Join-Path $PSScriptRoot $file) $archive
}
foreach($file in 'Test-DominoNetwork-v193.ps1','build_domino_bank_v192.py','domino-bank-manifest-v192.json','domino-bank-source-manifest-v192.json'){Copy-Item "$root/work/natural-moveset/$file" $archive}
[ordered]@{created=(Get-Date).ToString('o');dll=(Get-FileHash "$archive/dinput8.dll").Hash;probe=(Get-FileHash "$archive/d2_runtime_probe.exe").Hash;protocol=44;mesmerizeWorldProtocol=4;bankVersion=47;clips=289;runtimeVerified=$false;scope='Four exact observed standing armed Emily Domino phases source from validated native scene/state72 and stream through existing native Steam semantic transport. Bank47 appends four full-body clips preserving bank46 payloads; semantic44 refuses old peers/Corvo/air/holstered Domino. Receiver executes cosmetic poses only. Native target marks/links and shared outcomes not replicated. Actual both-role bank tests, formats1-47 regression, ownership/wire tests and proxy smoke pass; first native network test pending.'}|ConvertTo-Json -Depth 6|Set-Content "$archive/identity.json"
foreach($binary in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item (Join-Path $archive $binary) (Join-Path "$lab/bin" $binary)}
Get-Content "$archive/identity.json"
