$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$archive="$PSScriptRoot/candidate-swarm-binding-v268";$prior="$PSScriptRoot/candidate-swarm-binding-v267"
if(Test-Path $archive){throw 'Archive exists'}
if((Get-FileHash "$root/work/build/Release/dinput8.dll").Hash-ne(Get-FileHash "$prior/dinput8.dll").Hash){throw 'Unexpected runtime change'}
if(!(Select-String "$PSScriptRoot/swarm-binding-probe-cli-v268.log" -Pattern '^PASS')){throw 'Watcher CLI not checked'}
New-Item -ItemType Directory $archive|Out-Null;Copy-Item "$lab/src" "$archive/src" -Recurse
foreach($b in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item "$root/work/build/Release/$b" "$archive/$b"}
foreach($f in 'swarm-binding-probe-build-v268.log','swarm-binding-probe-cli-v268.log','Invoke-NativeLabOperation.ps1','prepare_swarm_binding_v267.py'){Copy-Item "$PSScriptRoot/$f" $archive}
Copy-Item "$root/work/natural-moveset/Test-SwarmNetwork-v267.ps1" $archive
Copy-Item "$root/work/natural-moveset/swarm-network-standing-v267/result.json" "$archive/retained-failed-v267.json"
$identity=Get-Content "$prior/identity.json" -Raw|ConvertFrom-Json
$identity.created=(Get-Date).ToString('o');$identity.probe=(Get-FileHash "$archive/d2_runtime_probe.exe").Hash
$identity.scope+=' v268 changes only probe CLI registration for --watch-swarm-scenes; DLL and banks identical. Invalid non-game PID proves dispatcher passes argument parsing and reaches the identity guard. v267 test recorded remote phases23/24/25 but failed its strict watcher due to missing registration; checkpoint/protected14 passed. Failed receipt retained. Paired verification must be repeated.'
$identity|ConvertTo-Json -Depth 8|Set-Content "$archive/identity.json"
foreach($b in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item "$archive/$b" "$lab/bin/$b"}
$identity|Select-Object dll,probe,protocol,bankVersion|ConvertTo-Json
