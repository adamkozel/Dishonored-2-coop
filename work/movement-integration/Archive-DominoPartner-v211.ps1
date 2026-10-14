$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$archive=Join-Path $PSScriptRoot 'candidate-domino-partner-v211'
if(Test-Path $archive){throw 'Archive already exists'}
New-Item -ItemType Directory $archive|Out-Null
Copy-Item "$lab/src" "$archive/src" -Recurse
foreach($binary in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item "$root/work/build/Release/$binary" (Join-Path $archive $binary)}
foreach($file in 'Invoke-NativeLabOperation.ps1','domino-partner-build-v211.log','domino-partner-identity-v211.log','domino-partner-smoke-v211.log','prepare_domino_partner_test_v211.py','disassemble_domino_picker_v211.py'){Copy-Item (Join-Path $PSScriptRoot $file) $archive}
Copy-Item "$root/work/natural-moveset/Test-DominoPartner-v211.ps1" $archive
Copy-Item "$root/work/domino-picker-disk-disassembly-v211.json" $archive
[ordered]@{created=(Get-Date).ToString('o');dll=(Get-FileHash "$archive/dinput8.dll").Hash;probe=(Get-FileHash "$archive/d2_runtime_probe.exe").Hash;protocol=44;mesmerizeWorldProtocol=4;dominoWorldProtocol=1;bankVersion=47;clips=289;runtimeVerified=$false;scope='Native Domino picker447250 called once, then only an exactly owned/current registered co-op partner is replaced by null before4478c0 stores c8 or creates aim FX. Scoped native Emily equipment ownership, reciprocal avatar identity and exact18byte hook prologue verified; synthetic identity tests and proxy smoke pass. No native link/damage calls. Body44/bank47/world1 unchanged. Partner runtime and guard regression pending.'}|ConvertTo-Json -Depth 6|Set-Content "$archive/identity.json"
foreach($binary in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item (Join-Path $archive $binary) (Join-Path "$lab/bin" $binary)}
Get-Content "$archive/identity.json"
