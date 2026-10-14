$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab'
$archive=Join-Path $PSScriptRoot 'candidate-mesmerize-empty-air-v178'
if(Test-Path -LiteralPath $archive){throw 'Candidate archive already exists'}
New-Item -ItemType Directory -Path $archive|Out-Null
Copy-Item -LiteralPath "$lab/src" -Destination "$archive/src" -Recurse
foreach($binary in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item -LiteralPath "$root/work/build/Release/$binary" -Destination (Join-Path $archive $binary)}
foreach($file in 'Invoke-NativeLabOperation.ps1','mesmerize-empty-air-build-v178.log','mesmerize-empty-air-build-wire-v178.log','mesmerize-empty-air-identity-v178.log','mesmerize-empty-air-wire-v178.log','mesmerize-empty-air-smoke-v178.log','prepare_mesmerize_empty_air_network_v178.py'){
 Copy-Item -LiteralPath (Join-Path $PSScriptRoot $file) -Destination (Join-Path $archive $file)
}
Copy-Item -LiteralPath "$root/work/natural-moveset/Test-MesmerizeAirNetwork-v178.ps1" -Destination $archive
[ordered]@{created=(Get-Date).ToString('o');dll=(Get-FileHash "$archive/dinput8.dll").Hash;probe=(Get-FileHash "$archive/d2_runtime_probe.exe").Hash;protocol=43;mesmerizeWorldProtocol=4;bankVersion=46;clips=285;runtimeVerified=$false;scope='Native v175/v177 observations confirm holstered Emily Jump/Fall entry, aim and release without drawing the sword. Admit these exact owned scene contexts in source reader, body43/336 wire and bank46 playback; prior42 peer rejected. Retain v170 bounded root interpolation continuity fix. Identity and wire suites plus proxy smoke pass. Initial build request named the wire target incorrectly; corrected d2_shadow_action_tests target builds and passes. Native integration pending.'}|ConvertTo-Json -Depth 5|Set-Content "$archive/identity.json"
foreach($binary in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item -LiteralPath (Join-Path $archive $binary) -Destination (Join-Path "$lab/bin" $binary)}
Get-Content "$archive/identity.json"
