$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true}
 if(!$owned){throw 'Another native operation or cleanup remains active'}
 $cmake='C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe'
 $node='C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/bin/node.exe'
 & $node "$root/outputs/d2-coop-lab/tooling/run-clean.cjs" $cmake --build "$root/work/build" --config Release --target d2_runtime_probe *> "$PSScriptRoot/material-probe-build-v541.log"
 if($LASTEXITCODE){Get-Content "$PSScriptRoot/material-probe-build-v541.log" -Tail 35;throw 'Probe build failed'}
 $archive=Join-Path $PSScriptRoot 'candidate-material-probe-v541'
 if(Test-Path $archive){throw 'Archive already exists'}
 New-Item -ItemType Directory $archive|Out-Null
 foreach($file in 'runtime_probe.cpp','midnight_fx_probe.h','material_parm_probe.h'){Copy-Item "$root/outputs/d2-coop-lab/src/$file" $archive}
 Copy-Item "$root/work/build/Release/d2_runtime_probe.exe" $archive
 $identity=[ordered]@{probe=(Get-FileHash "$archive/d2_runtime_probe.exe").Hash;previousProbe=(Get-FileHash "$root/outputs/d2-coop-lab/bin/d2_runtime_probe.exe").Hash;observedLiveDll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;scope='Read-only per-entity material parameter and Midnight key correlation. Does not mutate game memory or claim a runtime setter.';runtimeVerified=$false}
 $identity|ConvertTo-Json|Set-Content "$archive/identity.json"
 Copy-Item "$archive/d2_runtime_probe.exe" "$root/outputs/d2-coop-lab/bin/d2_runtime_probe.exe"
 $identity|ConvertTo-Json
}finally{if($owned){$mutex.ReleaseMutex()};$mutex.Dispose()}
