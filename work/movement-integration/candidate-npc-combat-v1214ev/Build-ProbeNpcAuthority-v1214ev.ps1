$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
# 1214ev (#113): probe-only candidate (--npc-authority-all reads every unique NPC's AI flags and eligibility); the DLL sources are
# unchanged from 1214eu, so only d2_runtime_probe is built and the 1214eu DLL is kept. The build only writes work/build.
$cmake='C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe'
$node='C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/bin/node.exe'
if(!(Test-Path -LiteralPath $cmake)-or!(Test-Path -LiteralPath $node)){throw 'Pinned build tools missing'}
& $node "$root/outputs/d2-coop-lab/tooling/run-clean.cjs" $cmake --build "$root/work/build" --config Release --target d2_runtime_probe *> "$PSScriptRoot/probenpcauthority-build-v1214ev.log"
if($LASTEXITCODE){Get-Content "$PSScriptRoot/probenpcauthority-build-v1214ev.log" -Tail 35;throw 'Build failed'}
'Build1214ev (probe only: --npc-authority-all) passed'
