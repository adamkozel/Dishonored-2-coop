$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
# 1214ew (#113): probe-only candidate (--npc-ai-registries finds the AI-manager registry of each AI component of one NPC); the DLL sources are
# unchanged from 1214eu, so only d2_runtime_probe is built and the 1214eu DLL is kept. The build only writes work/build.
$cmake='C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe'
$node='C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/bin/node.exe'
if(!(Test-Path -LiteralPath $cmake)-or!(Test-Path -LiteralPath $node)){throw 'Pinned build tools missing'}
& $node "$root/outputs/d2-coop-lab/tooling/run-clean.cjs" $cmake --build "$root/work/build" --config Release --target d2_runtime_probe *> "$PSScriptRoot/probeairegistry-build-v1214ew.log"
if($LASTEXITCODE){Get-Content "$PSScriptRoot/probeairegistry-build-v1214ew.log" -Tail 35;throw 'Build failed'}
'Build1214ew (probe only: --npc-ai-registries) passed'
