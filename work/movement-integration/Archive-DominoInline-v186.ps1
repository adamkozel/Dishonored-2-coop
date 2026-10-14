$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab'
$archive=Join-Path $PSScriptRoot 'candidate-domino-inline-v186'
if(Test-Path -LiteralPath $archive){throw 'Archive already exists'}
New-Item -ItemType Directory -Path $archive|Out-Null
Copy-Item -LiteralPath "$lab/src" -Destination "$archive/src" -Recurse
foreach($binary in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item -LiteralPath "$root/work/build/Release/$binary" -Destination (Join-Path $archive $binary)}
foreach($file in 'Invoke-NativeLabOperation.ps1','domino-inline-build-v186.log','domino-inline-identity-v186.log','domino-inline-smoke-v186.log'){Copy-Item -LiteralPath (Join-Path $PSScriptRoot $file) -Destination $archive}
Copy-Item -LiteralPath "$root/work/natural-moveset/Observe-NativeDomino-v182.ps1" -Destination $archive
Copy-Item -LiteralPath "$lab/Send-D2Command.ps1" -Destination $archive
[ordered]@{created=(Get-Date).ToString('o');dll=(Get-FileHash "$archive/dinput8.dll").Hash;probe=(Get-FileHash "$archive/d2_runtime_probe.exe").Hash;protocol=43;mesmerizeWorldProtocol=4;bankVersion=46;clips=285;runtimeVerified=$false;scope='v185 exact native Domino inline declaration plus named +48 parent validation. Wrong/missing/self/racing parent rejected by identity tests. Retains reciprocal equipment/owner/registration checks. Fixture observation pending; no remote Domino integration.'}|ConvertTo-Json -Depth 5|Set-Content "$archive/identity.json"
foreach($binary in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item -LiteralPath (Join-Path $archive $binary) -Destination (Join-Path "$lab/bin" $binary)}
Get-Content "$archive/identity.json"
