$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true}
 if(!$owned){throw 'Another native operation or cleanup remains active'}
 $cmake='C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe'
 $node='C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/bin/node.exe'
 if(!(Test-Path -LiteralPath $cmake)-or!(Test-Path -LiteralPath $node)){throw 'Pinned build tools missing'}
 & $node "$root/outputs/d2-coop-lab/tooling/run-clean.cjs" $cmake --build "$root/work/build" --config Release --target d2_lab_proxy d2_runtime_probe d2_world_identity_tests d2_shadow_action_tests d2_movement_bank_tests d2_movement_resume_tests d2_checkpoint_protocol_tests d2_pose_interpolation_tests d2_movement_arm_reach_tests *> "$PSScriptRoot/npc-parry-build-v1214bp.log"
 if($LASTEXITCODE){Get-Content "$PSScriptRoot/npc-parry-build-v1214bp.log" -Tail 35;throw 'Build failed'}
 foreach($test in @(@('d2_world_identity_tests','world-tests'),@('d2_shadow_action_tests','action-tests'),@('d2_movement_resume_tests','resume-tests'),@('d2_checkpoint_protocol_tests','checkpoint-tests'),@('d2_pose_interpolation_tests','pose-tests'),@('d2_movement_arm_reach_tests','arm-reach-tests'))){
  & "$root/work/build/Release/$($test[0]).exe" *> "$PSScriptRoot/npc-parry-$($test[1])-v1214bp.log"
  if($LASTEXITCODE){Get-Content "$PSScriptRoot/npc-parry-$($test[1])-v1214bp.log" -Tail 20;throw "Test failed: $($test[0])"}
 }
 & "$root/work/build/Release/d2_proxy_smoke.exe" "$root/work/build/Release/dinput8.dll" *> "$PSScriptRoot/npc-parry-smoke-v1214bp.log"
 if($LASTEXITCODE){throw 'Proxy smoke failed'}
 & "$root/work/build/Release/d2_movement_bank_tests.exe" "$root/work/natural-moveset/use-banks-v1266/corvo-choke-bank.bin" "$root/work/natural-moveset/use-banks-v1266/emily-choke-bank.bin" *> "$PSScriptRoot/npc-parry-bank-tests-v1214bp.log"
 if($LASTEXITCODE){Get-Content "$PSScriptRoot/npc-parry-bank-tests-v1214bp.log" -Tail 15;throw 'Actual candidate bank test failed'}
 'Build1214bp and native ownership, action protocol, proxy smoke, actual369 clip bank tests passed'
}finally{if($owned){$mutex.ReleaseMutex()};$mutex.Dispose()}
