$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$archive=Join-Path $PSScriptRoot 'candidate-domino-crouch-select-v250'
if(Test-Path $archive){throw 'Archive already exists'}
New-Item -ItemType Directory $archive|Out-Null
Copy-Item "$lab/src" "$archive/src" -Recurse
foreach($binary in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item "$root/work/build/Release/$binary" (Join-Path $archive $binary)}
foreach($file in 'Invoke-NativeLabOperation.ps1','domino-crouch-select-build-v250.log','domino-crouch-select-identity-v250.log','domino-crouch-select-wire-v250.log','domino-crouch-select-smoke-v250.log','domino-crouch-bank-fixtures-v246.log','domino-crouch-bank-actual-v246.log','prepare_domino_context_authoring_v243.py','prepare_domino_crouch_select_v250.py'){Copy-Item (Join-Path $PSScriptRoot $file) $archive}
foreach($file in 'Test-DominoCrouchNetwork-v250.ps1','Test-DominoFinalRecovery-v248.ps1','build_domino_contexts_v243.py','domino-context-bank-manifest-v243.json','domino-context-source-manifest-v243.json'){Copy-Item "$root/work/natural-moveset/$file" $archive}
[ordered]@{created=(Get-Date).ToString('o');dll=(Get-FileHash "$archive/dinput8.dll").Hash;probe=(Get-FileHash "$archive/d2_runtime_probe.exe").Hash;protocol=44;mesmerizeWorldProtocol=4;dominoWorldProtocol=2;dominoWorldBytes=1000;bankVersion=48;clips=301;runtimeVerified=$false;scope='Native v249 directly confirms crouched ordinary Cast_Select400 with reciprocal Emily bindings and Sneaking6/Domino72. Reader now accepts it; crouched final selection remains refused. Standing final recovery retained. Bank48/301 payloads unchanged from candidate246 with its full format/actual-bank checks retained. This candidate adds reader ownership/state fixtures, wire and load checks. Two-player crouch runtime and visual acceptance pending.'}|ConvertTo-Json -Depth 6|Set-Content "$archive/identity.json"
foreach($binary in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item (Join-Path $archive $binary) (Join-Path "$lab/bin" $binary)}
Get-Content "$archive/identity.json"
