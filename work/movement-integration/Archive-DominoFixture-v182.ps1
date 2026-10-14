$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab'
$archive=Join-Path $PSScriptRoot 'candidate-domino-fixture-v182'
if(Test-Path -LiteralPath $archive){throw 'Candidate archive already exists'}
New-Item -ItemType Directory -Path $archive|Out-Null
Copy-Item -LiteralPath "$lab/src" -Destination "$archive/src" -Recurse
foreach($binary in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item -LiteralPath "$root/work/build/Release/$binary" -Destination (Join-Path $archive $binary)}
foreach($file in 'Invoke-NativeLabOperation.ps1','domino-fixture-build-v182.log','domino-fixture-identity-v182.log','domino-fixture-smoke-v182.log','domino-native-inventory-v182.json','prepare_domino_reader_v182.py','prepare_domino_fixture_v182.py','prepare_domino_observation_v182.py'){
 Copy-Item -LiteralPath (Join-Path $PSScriptRoot $file) -Destination (Join-Path $archive $file)
}
Copy-Item -LiteralPath "$root/work/natural-moveset/Observe-NativeDomino-v182.ps1" -Destination $archive
[ordered]@{created=(Get-Date).ToString('o');dll=(Get-FileHash "$archive/dinput8.dll").Hash;probe=(Get-FileHash "$archive/d2_runtime_probe.exe").Hash;protocol=43;mesmerizeWorldProtocol=4;bankVersion=46;clips=285;runtimeVerified=$false;scope='Domino PhantasmalLink exact asset/RTTI inventory reader, read-only native effect filter and finite isolated native acquisition/restoration fixture. World identity tests and proxy smoke pass; direct reader confirms unowned Emily inventory and rejects Corvo. Controller-only use, no forced cast/link/NPC effect calls and no remote integration. First native acquisition/scene observation pending. Retains verified Mesmerize empty-hand air and root continuity.'}|ConvertTo-Json -Depth 5|Set-Content "$archive/identity.json"
foreach($binary in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item -LiteralPath (Join-Path $archive $binary) -Destination (Join-Path "$lab/bin" $binary)}
Get-Content "$archive/identity.json"
