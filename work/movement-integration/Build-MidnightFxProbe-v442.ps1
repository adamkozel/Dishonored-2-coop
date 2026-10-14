$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true}
 if(!$owned){throw 'Native operation still active'}
 $archive=Join-Path $PSScriptRoot 'candidate-midnight-fx-probe-v442'
 if(Test-Path -LiteralPath $archive){throw 'Archive already exists'}
 $previous=Get-Content "$PSScriptRoot/candidate-doppelganger-fx-probe-v434/identity.json" -Raw|ConvertFrom-Json
 if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$previous.dll){throw 'Unexpected live DLL'}
 $cmake='C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe'
 $node='C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/bin/node.exe'
 & $node "$root/outputs/d2-coop-lab/tooling/run-clean.cjs" $cmake --build "$root/work/build" --config Release --target d2_runtime_probe d2_world_identity_tests *> "$PSScriptRoot/midnight-fx-probe-build-v442.log"
 if($LASTEXITCODE){Get-Content "$PSScriptRoot/midnight-fx-probe-build-v442.log" -Tail 40;throw 'Probe build failed'}
 & "$root/work/build/Release/d2_world_identity_tests.exe" *> "$PSScriptRoot/midnight-fx-reader-tests-v442.log"
 if($LASTEXITCODE){Get-Content "$PSScriptRoot/midnight-fx-reader-tests-v442.log" -Tail 20;throw 'Native scene reader tests failed'}
 New-Item -ItemType Directory $archive|Out-Null
 Copy-Item -LiteralPath "$root/outputs/d2-coop-lab/src" -Destination "$archive/src" -Recurse
 Copy-Item -LiteralPath "$root/work/build/Release/d2_runtime_probe.exe" -Destination "$archive/d2_runtime_probe.exe"
 Copy-Item -LiteralPath "$root/work/asset-game/dinput8.dll" -Destination "$archive/dinput8.dll"
 Copy-Item -LiteralPath $PSCommandPath -Destination $archive
 Copy-Item -LiteralPath "$PSScriptRoot/midnight-fx-probe-build-v442.log" -Destination $archive
 $identity=[ordered]@{created=(Get-Date).ToString('o');dll=$previous.dll;probe=(Get-FileHash "$archive/d2_runtime_probe.exe").Hash;previous='candidate-doppelganger-fx-probe-v434';scope='External read-only scene/group/FX-track/key/helper observation; unchanged DLL427 and bank55/322; native playback remains pending';runtimeVerified=$false}
 $identity|ConvertTo-Json -Depth 6|Set-Content "$archive/identity.json"
 Copy-Item -LiteralPath "$archive/d2_runtime_probe.exe" -Destination "$root/outputs/d2-coop-lab/bin/d2_runtime_probe.exe"
 $identity|ConvertTo-Json -Compress
}finally{if($owned){$mutex.ReleaseMutex()};$mutex.Dispose()}

