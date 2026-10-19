$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
# 1214ez (#113): probe-only candidate (--npc-authority-all also lists non-unique NPCs and their AI component types); the DLL sources are
# unchanged from 1214ey, so only d2_runtime_probe is built and the 1214ey DLL is kept. The build only writes work/build.
$cmake='C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe'
$node='C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/bin/node.exe'
if(!(Test-Path -LiteralPath $cmake)-or!(Test-Path -LiteralPath $node)){throw 'Pinned build tools missing'}
& $node "$root/outputs/d2-coop-lab/tooling/run-clean.cjs" $cmake --build "$root/work/build" --config Release --target d2_runtime_probe *> "$PSScriptRoot/probenonhuman-build-v1214ez.log"
if($LASTEXITCODE){Get-Content "$PSScriptRoot/probenonhuman-build-v1214ez.log" -Tail 35;throw 'Build failed'}
'Build1214ez (probe only: --npc-authority-all with non-unique NPCs) passed'
