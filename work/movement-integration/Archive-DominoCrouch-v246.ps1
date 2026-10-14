$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$archive=Join-Path $PSScriptRoot 'candidate-domino-crouch-v246'
if(Test-Path $archive){throw 'Archive already exists'}
New-Item -ItemType Directory $archive|Out-Null
Copy-Item "$lab/src" "$archive/src" -Recurse
foreach($binary in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item "$root/work/build/Release/$binary" (Join-Path $archive $binary)}
foreach($file in 'Invoke-NativeLabOperation.ps1','domino-crouch-build-v246.log','domino-crouch-identity-v246.log','domino-crouch-wire-v246.log','domino-crouch-smoke-v246.log','domino-crouch-bank-fixtures-v246.log','domino-crouch-bank-actual-v246.log','prepare_domino_context_authoring_v243.py','prepare_domino_crouch_network_v246.py'){Copy-Item (Join-Path $PSScriptRoot $file) $archive}
foreach($file in 'Test-DominoCrouchNetwork-v246.ps1','Test-DominoFinalRecovery-v248.ps1','build_domino_contexts_v243.py','domino-context-bank-manifest-v243.json','domino-context-source-manifest-v243.json'){Copy-Item "$root/work/natural-moveset/$file" $archive}
[ordered]@{created=(Get-Date).ToString('o');dll=(Get-FileHash "$archive/dinput8.dll").Hash;probe=(Get-FileHash "$archive/d2_runtime_probe.exe").Hash;protocol=44;mesmerizeWorldProtocol=4;dominoWorldProtocol=2;dominoWorldBytes=1000;bankVersion=48;clips=301;runtimeVerified=$false;scope='Observedv245 native Emily crouched armed In/Loop/Out with exact Sneaking6/Domino72 enables strict scene and root recognition. Complete-body bank48 appends twelve stance/equipment candidates per role while preserving prior289 payloads. Native holstered/air and crouched target-select/final remain refused until observed; Corvo Domino is offline skeleton comparison. Standing final recovery from verifiedcandidate241 retained. Identity, wire, load, bank-format fixtures and actual Corvo/Emily bank checks pass. Runtime crouch and visual comparison pending.'}|ConvertTo-Json -Depth 6|Set-Content "$archive/identity.json"
foreach($binary in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item (Join-Path $archive $binary) (Join-Path "$lab/bin" $binary)}
Get-Content "$archive/identity.json"
