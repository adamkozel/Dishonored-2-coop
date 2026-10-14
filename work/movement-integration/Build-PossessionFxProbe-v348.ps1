$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation or cleanup active'}
 $cmake='C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe'
 $node='C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/bin/node.exe'
 & $node "$root/outputs/d2-coop-lab/tooling/run-clean.cjs" $cmake --build "$root/work/build" --config Release --target d2_runtime_probe *> "$PSScriptRoot/possession-fx-probe-build-v348.log"
 if($LASTEXITCODE){Get-Content "$PSScriptRoot/possession-fx-probe-build-v348.log" -Tail 30;throw 'Probe build failed'}
 if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne'7AF4A9718092E25F17F8078882B175B52E50BACB8831D334DB4F5891258E002C'){throw 'Live DLL changed'}
 'Read-only probe348 built; live DLL343 retained'
}finally{if($owned){$mutex.ReleaseMutex()};$mutex.Dispose()}
