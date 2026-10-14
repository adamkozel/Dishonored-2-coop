$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true}
 if(!$owned){throw 'Another native operation or cleanup remains active'}
 $cmake='C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe'
 $node='C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/bin/node.exe'
 if(!(Test-Path -LiteralPath $cmake)-or!(Test-Path -LiteralPath $node)){throw 'Pinned build tools missing'}
 & $node "$root/outputs/d2-coop-lab/tooling/run-clean.cjs" $cmake --build "$root/work/build" --config Release --target d2_runtime_probe d2_world_identity_tests *> "$PSScriptRoot/shadowwalk-state-build-v522.log"
 if($LASTEXITCODE){Get-Content "$PSScriptRoot/shadowwalk-state-build-v522.log" -Tail 35;throw 'Build failed'}
 & "$root/work/build/Release/d2_world_identity_tests.exe" *> "$PSScriptRoot/shadowwalk-state-world-tests-v522.log"
 if($LASTEXITCODE){Get-Content "$PSScriptRoot/shadowwalk-state-world-tests-v522.log" -Tail 20;throw 'Native ownership tests failed'}
 'Read-only probe522 and native ownership tests passed'
}finally{if($owned){$mutex.ReleaseMutex()};$mutex.Dispose()}
