param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='doppelganger-fx-v453')
$ErrorActionPreference='Stop'
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true}
 if(!$owned){throw 'Native operation still active'}
 [ordered]@{pid=$PID;operation='DoppelgangerNetwork';take=$Take;started=(Get-Date).ToString('o');state='running'}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json"
 & "$PSScriptRoot/../natural-moveset/Test-DoppelgangerFx-v453.ps1" -Take $Take
 if(!$?){throw 'Native observation failed'}
}finally{
 if($owned){[ordered]@{pid=$PID;operation='DoppelgangerNetwork';take=$Take;ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json";$mutex.ReleaseMutex()};$mutex.Dispose()
}

