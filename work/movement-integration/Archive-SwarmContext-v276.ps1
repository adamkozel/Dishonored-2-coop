$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$archive=Join-Path $PSScriptRoot 'candidate-swarm-context-v276'
if(Test-Path $archive){throw 'Archive already exists'}
$logs=@('build','identity','wire','smoke','bank-fixtures','bank-actual')
foreach($kind in $logs){$f=Join-Path $PSScriptRoot "swarm-context-$kind-v276.log";if(!(Test-Path $f)-or(Get-Item $f).Length-eq0-or(Select-String -Path $f -Pattern '^FAIL|error C\d|error LNK\d')){throw "Unverified candidate log $kind"}}
New-Item -ItemType Directory $archive|Out-Null
Copy-Item "$lab/src" "$archive/src" -Recurse
foreach($binary in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item "$root/work/build/Release/$binary" (Join-Path $archive $binary)}
foreach($kind in $logs){Copy-Item (Join-Path $PSScriptRoot "swarm-context-$kind-v276.log") $archive}
foreach($file in 'Invoke-NativeLabOperation.ps1','prepare_swarm_context_binding_v276.py'){Copy-Item (Join-Path $PSScriptRoot $file) $archive}
foreach($file in 'Test-SwarmContextNetwork-v276.ps1','build_swarm_contexts_v272.py','swarm-context-bank-manifest-v272.json','swarm-context-source-manifest-v272.json'){Copy-Item "$root/work/natural-moveset/$file" $archive}
Copy-Item "$lab/Send-D2Command.ps1" $archive
$banks=@(Get-ChildItem "$root/work/natural-moveset/swarm-context-banks-v272/*-swarm-context-bank.bin"|ForEach-Object {[ordered]@{path=$_.FullName;sha256=(Get-FileHash $_.FullName).Hash}})
[ordered]@{created=(Get-Date).ToString('o');dll=(Get-FileHash "$archive/dinput8.dll").Hash;probe=(Get-FileHash "$archive/d2_runtime_probe.exe").Hash;protocol=47;protocolBytes=336;mesmerizeWorldProtocol=4;dominoWorldProtocol=2;dominoWorldBytes=1000;bankVersion=50;clips=313;banks=$banks;runtimeVerified=$false;scope='Preserves candidate270 and every304 bank49 payload. Adds observed native Corvo Swarm grounded standing/crouched and armed/holstered context matrix, exact native phases23/24/25, durations400/333/1600, reciprocal body+left arm, matching Running0 or Sneaking6 and UsePower63. Semantic wire47 retains336 bytes. Reader refuses unknown ownership/state and uses stable identity rereads. Bank50/313 includes nine complete-body stance/equipment additions with matching sword sockets; network retains root control. No peer native rat AI, damage or VFX invocation. Build, identity/wire/load, versions1..50 and both actual banks pass. Authoring sheets inspected; native-speed paired context testing pending. Source context274 failed a non-atomic teardown observation and remains retained; corrected bounded observation275 passed all6 including restoration.'}|ConvertTo-Json -Depth 8|Set-Content "$archive/identity.json"
foreach($binary in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item (Join-Path $archive $binary) (Join-Path "$lab/bin" $binary)}
Get-Content "$archive/identity.json"
