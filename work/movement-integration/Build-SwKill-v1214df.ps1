$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
# 1214df: the build only writes work/build; staging and deploy take the lab mutex.
$owned=$false
try{
 $cmake='C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe'
 $node='C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/bin/node.exe'
 if(!(Test-Path -LiteralPath $cmake)-or!(Test-Path -LiteralPath $node)){throw 'Pinned build tools missing'}
 & $node "$root/outputs/d2-coop-lab/tooling/run-clean.cjs" $cmake --build "$root/work/build" --config Release --target d2_lab_proxy d2coop_setup d2_runtime_probe d2_world_identity_tests d2_shadow_action_tests d2_movement_bank_tests d2_movement_resume_tests d2_checkpoint_protocol_tests d2_pose_interpolation_tests d2_native_tables_tests d2_bank_rebuild_tests d2_movement_arm_reach_tests *> "$PSScriptRoot/swkill-build-v1214df.log"
 if($LASTEXITCODE){Get-Content "$PSScriptRoot/swkill-build-v1214df.log" -Tail 35;throw 'Build failed'}
 foreach($test in @(@('d2_world_identity_tests','world-tests'),@('d2_shadow_action_tests','action-tests'),@('d2_movement_resume_tests','resume-tests'),@('d2_checkpoint_protocol_tests','checkpoint-tests'),@('d2_pose_interpolation_tests','pose-tests'),@('d2_movement_arm_reach_tests','arm-reach-tests'),@('d2_native_tables_tests','tables-tests'),@('d2_bank_rebuild_tests','rebuild-tests'))){
  & "$root/work/build/Release/$($test[0]).exe" *> "$PSScriptRoot/swkill-$($test[1])-v1214df.log"
  if($LASTEXITCODE){Get-Content "$PSScriptRoot/swkill-$($test[1])-v1214df.log" -Tail 20;throw "Test failed: $($test[0])"}
 }
 & "$root/work/build/Release/d2_proxy_smoke.exe" "$root/work/build/Release/dinput8.dll" *> "$PSScriptRoot/swkill-smoke-v1214df.log"
 if($LASTEXITCODE){throw 'Proxy smoke failed'}
 & "$root/work/build/Release/d2_movement_bank_tests.exe" "$root/work/natural-moveset/recoil-banks-v1365/corvo-choke-bank.bin" "$root/work/natural-moveset/recoil-banks-v1365/emily-choke-bank.bin" *> "$PSScriptRoot/swkill-bank-tests-v1214df.log"
 if($LASTEXITCODE){Get-Content "$PSScriptRoot/swkill-bank-tests-v1214df.log" -Tail 15;throw 'Actual candidate bank test failed'}
 'Build1214df (#11 shadow-form kill capture and upgrade grant protocol77/bank73) and native ownership, action protocol, proxy smoke, actual380 clip bank tests passed'
}finally{}
