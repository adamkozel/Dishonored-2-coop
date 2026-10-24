$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
# 1214kc: the build only writes work/build; staging and deploy take the lab mutex.
$owned=$false
try{
 $cmake='C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe'
 $node='C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/bin/node.exe'
 if(!(Test-Path -LiteralPath $cmake)-or!(Test-Path -LiteralPath $node)){throw 'Pinned build tools missing'}
 & $node "$root/outputs/d2-coop-lab/tooling/run-clean.cjs" $cmake --build "$root/work/build" --config Release --target d2_lab_proxy d2coop_setup d2_runtime_probe d2_world_identity_tests d2_shadow_action_tests d2_movement_bank_tests d2_movement_resume_tests d2_checkpoint_protocol_tests d2_pose_interpolation_tests d2_native_tables_tests d2_bank_rebuild_tests d2_movement_arm_reach_tests *> "$PSScriptRoot/partnerchoke-build-v1214kc.log"
 if($LASTEXITCODE){Get-Content "$PSScriptRoot/partnerchoke-build-v1214kc.log" -Tail 35;throw 'Build failed'}
 foreach($test in @(@('d2_world_identity_tests','world-tests'),@('d2_shadow_action_tests','action-tests'),@('d2_movement_resume_tests','resume-tests'),@('d2_checkpoint_protocol_tests','checkpoint-tests'),@('d2_pose_interpolation_tests','pose-tests'),@('d2_movement_arm_reach_tests','arm-reach-tests'),@('d2_native_tables_tests','tables-tests'),@('d2_bank_rebuild_tests','rebuild-tests'))){
  & "$root/work/build/Release/$($test[0]).exe" *> "$PSScriptRoot/partnerchoke-$($test[1])-v1214kc.log"
  if($LASTEXITCODE){Get-Content "$PSScriptRoot/partnerchoke-$($test[1])-v1214kc.log" -Tail 20;throw "Test failed: $($test[0])"}
 }
 & "$root/work/build/Release/d2_proxy_smoke.exe" "$root/work/build/Release/dinput8.dll" *> "$PSScriptRoot/partnerchoke-smoke-v1214kc.log"
 if($LASTEXITCODE){throw 'Proxy smoke failed'}
 & "$root/work/build/Release/d2_movement_bank_tests.exe" "$root/work/natural-moveset/shop-banks-v1409/corvo-choke-bank.bin" "$root/work/natural-moveset/shop-banks-v1409/emily-choke-bank.bin" *> "$PSScriptRoot/partnerchoke-bank-tests-v1214kc.log"
 if($LASTEXITCODE){Get-Content "$PSScriptRoot/partnerchoke-bank-tests-v1214kc.log" -Tail 15;throw 'Actual candidate bank test failed'}
 'Build1214kc (the host NPC vulnerable bit on the partner replica; protocol80/bank75) and native ownership, action protocol, proxy smoke, actual382 clip bank tests passed'
}finally{}
