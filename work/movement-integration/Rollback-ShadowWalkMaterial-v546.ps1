$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$good="$PSScriptRoot/candidate-shadowwalk-fx-v533";$trial="$PSScriptRoot/candidate-shadowwalk-material-v544"
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true}
 if(!$owned){throw 'Native operation active'}
 if((Get-Content "$PSScriptRoot/native-operation-owner.json" -Raw|ConvertFrom-Json).state-ne'finished'){throw 'Previous cleanup unconfirmed'}
 $goodId=Get-Content "$good/identity.json" -Raw|ConvertFrom-Json;$trialId=Get-Content "$trial/identity.json" -Raw|ConvertFrom-Json
 if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$trialId.dll-or(Get-FileHash "$good/dinput8.dll").Hash-ne$goodId.dll){throw 'Unexpected candidate hash'}
 if((Get-FileHash "$lab/src/engine_bridge.cpp").Hash-ne(Get-FileHash "$trial/src/engine_bridge.cpp").Hash){throw 'Engine source changed after candidate archive'}
 [ordered]@{pid=$PID;operation='MaterialRollback546';state='running';started=(Get-Date).ToString('o')}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json"
 Copy-Item "$good/src/engine_bridge.cpp" "$lab/src/engine_bridge.cpp"
 Copy-Item "$good/dinput8.dll" "$lab/bin/dinput8.dll"
 & "$PSScriptRoot/Deploy-SourceClock.ps1"
 if(!$?){throw 'Native rollback restore failed'}
 if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$goodId.dll){throw 'Rollback hash differs'}
 [ordered]@{at=(Get-Date).ToString('o');dll=$goodId.dll;probe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash;candidate='candidate-shadowwalk-fx-v533';rejected='candidate-shadowwalk-material-v544';reason='Visual rejection: bright white teleportation patches on ordinary third-person materials. Source bridge restored exactly; read-only material research/tests remain available.';checkpointRestored=$true}|ConvertTo-Json|Set-Content "$PSScriptRoot/shadowwalk-material-rollback-v546.json"
 'Verified533 restored; experimental material code disconnected from live source and runtime'
}finally{if($owned){[ordered]@{pid=$PID;operation='MaterialRollback546';state='finished';ended=(Get-Date).ToString('o')}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json";$mutex.ReleaseMutex()};$mutex.Dispose()}
