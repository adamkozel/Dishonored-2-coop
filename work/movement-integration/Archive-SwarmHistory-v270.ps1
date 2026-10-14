$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$archive="$PSScriptRoot/candidate-swarm-history-v270";$prior="$PSScriptRoot/candidate-swarm-binding-v268"
if(Test-Path $archive){throw 'Archive exists'}
if(!(Select-String "$PSScriptRoot/swarm-event-history-smoke-v270.log" -Pattern '^PASS')){throw 'Load smoke not checked'}
New-Item -ItemType Directory $archive|Out-Null;Copy-Item "$lab/src" "$archive/src" -Recurse
foreach($b in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item "$root/work/build/Release/$b" "$archive/$b"}
foreach($f in 'swarm-event-history-build-v270.log','swarm-event-history-smoke-v270.log','Invoke-NativeLabOperation.ps1','prepare_swarm_history_harness_v270.py'){Copy-Item "$PSScriptRoot/$f" $archive}
Copy-Item "$root/work/natural-moveset/Test-SwarmNetwork-v270.ps1" $archive
foreach($v in 267,268,269){Copy-Item "$root/work/natural-moveset/swarm-network-standing-v$v/result.json" "$archive/retained-failed-v$v.json"}
$identity=Get-Content "$prior/identity.json" -Raw|ConvertFrom-Json
$identity.created=(Get-Date).ToString('o');$identity.dll=(Get-FileHash "$archive/dinput8.dll").Hash;$identity.probe=(Get-FileHash "$archive/d2_runtime_probe.exe").Hash
$identity.scope='Extends preserved v268 body binding with bounded32-entry sent/accepted power-event diagnostic history, reset with stream tokens. Does not change packets, native readers, animation, world effects or gameplay. Protocol46/bank49/304 unchanged, v267 full-bank/identity/wire checks still apply. New build/load smoke pass. v268 proved seven native loops/seven remote sampled events but failed an incorrect one-loop assertion; v269 sampled six of seven loops because the last native loop lasted about30ms, shorter than status sampling. The new harness compares exact sender/accepted event identities and strict native scene handles. All prior failures retained with checkpoint/protected14 PASS. Runtime verification pending.'
$identity|ConvertTo-Json -Depth 8|Set-Content "$archive/identity.json"
foreach($b in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item "$archive/$b" "$lab/bin/$b"}
$identity|Select-Object dll,probe,protocol,bankVersion|ConvertTo-Json
