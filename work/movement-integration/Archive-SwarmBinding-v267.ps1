$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$archive=Join-Path $PSScriptRoot 'candidate-swarm-binding-v267'
if(Test-Path $archive){throw 'Archive already exists'}
$logs=@('build','identity','wire','smoke','bank-fixtures','bank-actual')
foreach($kind in $logs){$f=Join-Path $PSScriptRoot "swarm-binding-$kind-v267.log";if(!(Test-Path $f)-or(Get-Item $f).Length-eq0-or(Select-String -Path $f -Pattern '^FAIL|error C\d|error LNK\d')){throw "Unverified candidate log $kind"}}
New-Item -ItemType Directory $archive|Out-Null
Copy-Item "$lab/src" "$archive/src" -Recurse
foreach($binary in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item "$root/work/build/Release/$binary" (Join-Path $archive $binary)}
foreach($kind in $logs){Copy-Item (Join-Path $PSScriptRoot "swarm-binding-$kind-v267.log") $archive}
foreach($file in 'Invoke-NativeLabOperation.ps1','prepare_swarm_binding_v267.py','prepare_swarm_network_v267.py'){Copy-Item (Join-Path $PSScriptRoot $file) $archive}
foreach($file in 'Test-SwarmNetwork-v267.ps1','build_swarm_bank_v267.py','swarm-bank-manifest-v267.json'){Copy-Item "$root/work/natural-moveset/$file" $archive}
Copy-Item "$lab/Send-D2Command.ps1" $archive
$banks=@(Get-ChildItem "$root/work/natural-moveset/swarm-banks-v267/*-swarm-bank.bin"|ForEach-Object {[ordered]@{path=$_.FullName;sha256=(Get-FileHash $_.FullName).Hash}})
[ordered]@{created=(Get-Date).ToString('o');dll=(Get-FileHash "$archive/dinput8.dll").Hash;probe=(Get-FileHash "$archive/d2_runtime_probe.exe").Hash;protocol=46;protocolBytes=336;mesmerizeWorldProtocol=4;dominoWorldProtocol=2;dominoWorldBytes=1000;bankVersion=49;clips=304;banks=$banks;runtimeVerified=$false;scope='Preserves candidate265 and all301 prior bank payloads. Adds only v266-observed standing armed Corvo Swarm scene phases23/24/25, exact durations400/333/1600, reciprocal body+left arm, native UsePower63 and bounded ground recovery. Semantic wire46 retains336 bytes; receiver full-body animation preserves root/network control and executes no gameplay/VFX events. No remote rat world or shared damage replication. Build, ownership/wire/load, all versions1..49 and both actual banks pass. Authoring sheets viewed; paired native-speed verification pending.'}|ConvertTo-Json -Depth 8|Set-Content "$archive/identity.json"
foreach($binary in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item (Join-Path $archive $binary) (Join-Path "$lab/bin" $binary)}
Get-Content "$archive/identity.json"
