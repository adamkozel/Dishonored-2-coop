$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true}
 if(!$owned){throw 'Native operation still active'}
 $cmake='C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe'
 $node='C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/bin/node.exe'
 & $node "$root/outputs/d2-coop-lab/tooling/run-clean.cjs" $cmake --build "$root/work/build" --config Release --target d2_runtime_probe d2_world_identity_tests *> "$PSScriptRoot/doppelganger-world-build-v416.log"
 if($LASTEXITCODE){Get-Content "$PSScriptRoot/doppelganger-world-build-v416.log" -Tail 30;throw 'Build failed'}
 & "$root/work/build/Release/d2_world_identity_tests.exe" *> "$PSScriptRoot/doppelganger-world-tests-v416.log"
 if($LASTEXITCODE){Get-Content "$PSScriptRoot/doppelganger-world-tests-v416.log" -Tail 20;throw 'World tests failed'}
 'Read-only clone ownership probe416 builds and ownership/refusal tests pass; runtime pending'
}finally{if($owned){$mutex.ReleaseMutex()};$mutex.Dispose()}
