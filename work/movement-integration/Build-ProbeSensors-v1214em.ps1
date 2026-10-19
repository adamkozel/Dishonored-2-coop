$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
# 1214em (#11): probe-only candidate (--window-sensor, --sitting, --on-ground); the DLL sources are unchanged from 1214el,
# so only d2_runtime_probe is built and the 1214el DLL is kept. The build only writes work/build.
$cmake='C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe'
$node='C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/bin/node.exe'
if(!(Test-Path -LiteralPath $cmake)-or!(Test-Path -LiteralPath $node)){throw 'Pinned build tools missing'}
& $node "$root/outputs/d2-coop-lab/tooling/run-clean.cjs" $cmake --build "$root/work/build" --config Release --target d2_runtime_probe *> "$PSScriptRoot/probesensors-build-v1214em.log"
if($LASTEXITCODE){Get-Content "$PSScriptRoot/probesensors-build-v1214em.log" -Tail 35;throw 'Build failed'}
'Build1214em (probe only: --window-sensor, --sitting, --on-ground) passed'
