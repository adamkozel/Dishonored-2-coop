$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$archive=Join-Path $PSScriptRoot 'candidate-domino-crouch-final-v252'
if(Test-Path $archive){throw 'Archive already exists'}
New-Item -ItemType Directory $archive|Out-Null
Copy-Item "$lab/src" "$archive/src" -Recurse
foreach($binary in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item "$root/work/build/Release/$binary" (Join-Path $archive $binary)}
foreach($file in 'Invoke-NativeLabOperation.ps1','domino-crouch-final-build-v252.log','domino-crouch-final-identity-v252.log','domino-crouch-final-wire-v252.log','domino-crouch-final-smoke-v252.log','domino-crouch-bank-fixtures-v246.log','domino-crouch-bank-actual-v246.log','prepare_domino_context_authoring_v243.py','prepare_domino_crouch_final_v252.py'){Copy-Item (Join-Path $PSScriptRoot $file) $archive}
foreach($file in 'Test-DominoCrouchFinalNetwork-v252.ps1','Test-DominoFinalRecovery-v248.ps1','build_domino_contexts_v243.py','domino-context-bank-manifest-v243.json','domino-context-source-manifest-v243.json'){Copy-Item "$root/work/natural-moveset/$file" $archive}
[ordered]@{created=(Get-Date).ToString('o');dll=(Get-FileHash "$archive/dinput8.dll").Hash;probe=(Get-FileHash "$archive/d2_runtime_probe.exe").Hash;protocol=44;mesmerizeWorldProtocol=4;dominoWorldProtocol=2;dominoWorldBytes=1000;bankVersion=48;clips=301;runtimeVerified=$false;scope='Native v251 source PASS7 proves crouched final_Select400 and Sneaking6 recovery with14 reciprocal raw samples through383ms. Candidate252 allows exact final scene with matched crouched state, source clock bounded400ms and reciprocal body/arm/equipment, retaining standing and ordinary-selection safeguards. Bank48/301 payloads unchanged; format and actual-bank checks retained from246. Reader fixtures, build, wire and load pass. Paired final-crouch/runtime visual verification pending.'}|ConvertTo-Json -Depth 6|Set-Content "$archive/identity.json"
foreach($binary in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item (Join-Path $archive $binary) (Join-Path "$lab/bin" $binary)}
Get-Content "$archive/identity.json"
