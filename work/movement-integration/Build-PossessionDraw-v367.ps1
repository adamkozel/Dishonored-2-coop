$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true}
 if(!$owned){throw 'Another native operation or cleanup remains active'}
 $cmake='C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe'
 $node='C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/bin/node.exe'
 if(!(Test-Path -LiteralPath $cmake)-or!(Test-Path -LiteralPath $node)){throw 'Pinned build tools missing'}
 & $node "$root/outputs/d2-coop-lab/tooling/run-clean.cjs" $cmake --build "$root/work/build" --config Release --target d2_lab_proxy d2_runtime_probe d2_world_identity_tests d2_shadow_action_tests d2_movement_bank_tests *> "$PSScriptRoot/possession-draw-build-v367.log"
 if($LASTEXITCODE){Get-Content "$PSScriptRoot/possession-draw-build-v367.log" -Tail 35;throw 'Build failed'}
 foreach($test in @(@('d2_world_identity_tests','world-tests'),@('d2_shadow_action_tests','action-tests'))){
  & "$root/work/build/Release/$($test[0]).exe" *> "$PSScriptRoot/possession-draw-$($test[1])-v367.log"
  if($LASTEXITCODE){Get-Content "$PSScriptRoot/possession-draw-$($test[1])-v367.log" -Tail 20;throw "Test failed: $($test[0])"}
 }
 & "$root/work/build/Release/d2_proxy_smoke.exe" "$root/work/build/Release/dinput8.dll" *> "$PSScriptRoot/possession-draw-smoke-v367.log"
 if($LASTEXITCODE){throw 'Proxy smoke failed'}
 & "$root/work/build/Release/d2_movement_bank_tests.exe" "$root/work/natural-moveset/possession-crouch-banks-v363/corvo-possession-crouch-bank.bin" "$root/work/natural-moveset/possession-crouch-banks-v363/emily-possession-crouch-bank.bin" *> "$PSScriptRoot/possession-draw-bank-tests-v367.log"
 if($LASTEXITCODE){Get-Content "$PSScriptRoot/possession-draw-bank-tests-v367.log" -Tail 15;throw 'Actual candidate bank test failed'}
 'Build367 and native ownership, action protocol, proxy smoke, actual318 clip bank tests passed'
}finally{if($owned){$mutex.ReleaseMutex()};$mutex.Dispose()}




