$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$archive=Join-Path $PSScriptRoot 'candidate-domino-links-v222'
if(Test-Path $archive){throw 'Archive already exists'}
New-Item -ItemType Directory $archive|Out-Null
Copy-Item "$lab/src" "$archive/src" -Recurse
foreach($binary in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item "$root/work/build/Release/$binary" (Join-Path $archive $binary)}
foreach($file in 'Invoke-NativeLabOperation.ps1','domino-beam-build-v222.log','domino-beam-identity-v222.log','domino-beam-wire-v222.log','domino-beam-smoke-v222.log','prepare_domino_beam_network_v223.py'){Copy-Item (Join-Path $PSScriptRoot $file) $archive}
Copy-Item "$root/work/natural-moveset/Test-DominoLinks-v223.ps1" $archive
[ordered]@{created=(Get-Date).ToString('o');dll=(Get-FileHash "$archive/dinput8.dll").Hash;probe=(Get-FileHash "$archive/d2_runtime_probe.exe").Hash;protocol=44;mesmerizeWorldProtocol=4;dominoWorldProtocol=2;dominoWorldBytes=1000;bankVersion=47;clips=289;runtimeVerified=$false;scope='Strict native connection reader verifiedv221:477 valid snapshots,227 linked,two directed edges/one beam. World2 carries at most4 marks/6 observed beams, checked endpoint identities and bounded transforms. Receiver owns cosmetic native FX and updates both endpoint matrices; no connection/NPC/damage methods. Prior partner exclusion remains. Identity,protocol and proxy smoke pass. Paired beam runtime pending.'}|ConvertTo-Json -Depth 6|Set-Content "$archive/identity.json"
foreach($binary in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item (Join-Path $archive $binary) (Join-Path "$lab/bin" $binary)}
Get-Content "$archive/identity.json"
