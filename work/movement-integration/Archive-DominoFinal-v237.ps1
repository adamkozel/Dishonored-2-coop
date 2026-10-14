$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$archive=Join-Path $PSScriptRoot 'candidate-domino-final-v237'
if(Test-Path $archive){throw 'Archive already exists'}
New-Item -ItemType Directory $archive|Out-Null
Copy-Item "$lab/src" "$archive/src" -Recurse
foreach($binary in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item "$root/work/build/Release/$binary" (Join-Path $archive $binary)}
foreach($file in 'Invoke-NativeLabOperation.ps1','domino-final-build-v237.log','domino-final-identity-v237.log','domino-final-wire-v237.log','domino-final-smoke-v237.log','prepare_domino_final_binding_v238.py'){Copy-Item (Join-Path $PSScriptRoot $file) $archive}
Copy-Item "$root/work/natural-moveset/Test-DominoFinalSelect-v238.ps1" $archive
[ordered]@{created=(Get-Date).ToString('o');dll=(Get-FileHash "$archive/dinput8.dll").Hash;probe=(Get-FileHash "$archive/d2_runtime_probe.exe").Hash;protocol=44;mesmerizeWorldProtocol=4;dominoWorldProtocol=2;dominoWorldBytes=1000;bankVersion=47;clips=289;runtimeVerified=$false;scope='Native deselectionv235 proves Cast_final_Select400ms. Exact installed asset uses same native hand clip/duration as Cast_Select; reader accepts exact final subscene as existing logical selection variant21 under unchanged state72/owned-arm guards. No bank/wire layout change. Native Fire/slide/other contexts remain refused. Probe adds native state IDs for phase audit. Identity/protocol/load tests pass; paired final body runtime pending. Candidate222 preserved.'}|ConvertTo-Json -Depth 6|Set-Content "$archive/identity.json"
foreach($binary in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item (Join-Path $archive $binary) (Join-Path "$lab/bin" $binary)}
Get-Content "$archive/identity.json"
