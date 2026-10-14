$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true}
 if(!$owned){throw 'Another native operation or cleanup remains active'}
 $cmake='C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe'
 $node='C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/bin/node.exe'
 if(!(Test-Path -LiteralPath $cmake)-or!(Test-Path -LiteralPath $node)){throw 'Pinned build tools missing'}
 & $node "$root/outputs/d2-coop-lab/tooling/run-clean.cjs" $cmake --build "$root/work/build" --config Release --target d2_lab_proxy d2_runtime_probe d2_world_identity_tests d2_shadow_action_tests d2_movement_bank_tests d2_movement_resume_tests d2_checkpoint_protocol_tests *> "$PSScriptRoot/shadowwalk-body-build-v907.log"
 if($LASTEXITCODE){Get-Content "$PSScriptRoot/shadowwalk-body-build-v907.log" -Tail 35;throw 'Build failed'}
 foreach($test in @(@('d2_world_identity_tests','world-tests'),@('d2_shadow_action_tests','action-tests'),@('d2_movement_resume_tests','resume-tests'),@('d2_checkpoint_protocol_tests','checkpoint-tests'))){
  & "$root/work/build/Release/$($test[0]).exe" *> "$PSScriptRoot/shadowwalk-body-$($test[1])-v907.log"
  if($LASTEXITCODE){Get-Content "$PSScriptRoot/shadowwalk-body-$($test[1])-v907.log" -Tail 20;throw "Test failed: $($test[0])"}
 }
 & "$root/work/build/Release/d2_proxy_smoke.exe" "$root/work/build/Release/dinput8.dll" *> "$PSScriptRoot/shadowwalk-body-smoke-v907.log"
 if($LASTEXITCODE){throw 'Proxy smoke failed'}
 & "$root/work/build/Release/d2_movement_bank_tests.exe" "$root/work/natural-moveset/heavy-banks-v633/corvo-choke-bank.bin" "$root/work/natural-moveset/heavy-banks-v633/emily-choke-bank.bin" *> "$PSScriptRoot/shadowwalk-body-bank-tests-v907.log"
 if($LASTEXITCODE){Get-Content "$PSScriptRoot/shadowwalk-body-bank-tests-v907.log" -Tail 15;throw 'Actual candidate bank test failed'}
 'Build907 and native ownership, action protocol, proxy smoke, actual357 clip bank tests passed'
}finally{if($owned){$mutex.ReleaseMutex()};$mutex.Dispose()}
































