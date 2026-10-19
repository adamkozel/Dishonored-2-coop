$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
# 1214er (#11): probe-only candidate (--choke-sensor reads the native choke sensor's target); the DLL sources are
# unchanged from 1214eo, so only d2_runtime_probe is built and the 1214eo DLL is kept. The build only writes work/build.
$cmake='C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe'
$node='C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/bin/node.exe'
if(!(Test-Path -LiteralPath $cmake)-or!(Test-Path -LiteralPath $node)){throw 'Pinned build tools missing'}
& $node "$root/outputs/d2-coop-lab/tooling/run-clean.cjs" $cmake --build "$root/work/build" --config Release --target d2_runtime_probe *> "$PSScriptRoot/probechoke-build-v1214er.log"
if($LASTEXITCODE){Get-Content "$PSScriptRoot/probechoke-build-v1214er.log" -Tail 35;throw 'Build failed'}
'Build1214er (probe only: --choke-sensor) passed'
