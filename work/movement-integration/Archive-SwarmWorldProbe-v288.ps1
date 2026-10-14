$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$archive="$PSScriptRoot/candidate-swarm-world-probe-v288"
if(Test-Path $archive){throw 'Archive exists'}
foreach($kind in 'build','identity'){$p="$PSScriptRoot/swarm-world-probe-$kind-v288.log";if(!(Test-Path $p)-or!(Get-Item $p).Length-or(Select-String $p -Pattern '^FAIL|error C[0-9]|error LNK[0-9]')){throw "Unverified log $kind"}}
if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne'06E1DA2C679CFF6E420F9BCA8E0E2B3558F2CFB399A21D982C91DE5D9E1A0747'){throw 'Live proxy changed'}
New-Item -ItemType Directory $archive|Out-Null;Copy-Item "$lab/src" "$archive/src" -Recurse
Copy-Item "$root/work/asset-game/dinput8.dll" "$archive/dinput8.dll";Copy-Item "$root/work/build/Release/d2_runtime_probe.exe" "$archive/d2_runtime_probe.exe"
Copy-Item "$PSScriptRoot/swarm-world-probe-*-v288.log" $archive
Copy-Item "$PSScriptRoot/Invoke-NativeLabOperation.ps1" $archive;Copy-Item "$root/work/natural-moveset/Observe-NativeSwarmWorld-v288.ps1" $archive
Copy-Item "$root/work/natural-moveset/swarm-native-world-reader-v286/result.json" "$archive/retained-world-parent-failure-v286.json"
$identity=Get-Content "$PSScriptRoot/candidate-swarm-world-probe-v287/identity.json" -Raw|ConvertFrom-Json
$identity.created=(Get-Date).ToString('o');$identity.probe=(Get-FileHash "$archive/d2_runtime_probe.exe").Hash
$identity.scope='Read-only Swarm probe adds native contextual frame-driver support: entity+358 supplies the registered reference-frame component; rat+8 may be zero only for state3 with frame+d8 pointing to frame+20. Direct driving requires rat+8=frame and frame+d8=rat. Native6b8fc0 restores the rat driver after attachment. Strict ownership, generation, pool indices and stable pose checks remain. v287 initial full-rat samples passed but219 later rows were refused because rat+8 is not a permanent entity-frame pointer. New tests cover contextual and foreign drivers. Sustained native verification pending. Live proxy, wire47 and bank50 unchanged; remote rat gameplay remains unfinished.'
$identity|ConvertTo-Json -Depth 8|Set-Content "$archive/identity.json"
Copy-Item "$archive/d2_runtime_probe.exe" "$lab/bin/d2_runtime_probe.exe"
$identity|Select-Object dll,probe,protocol,bankVersion|ConvertTo-Json
