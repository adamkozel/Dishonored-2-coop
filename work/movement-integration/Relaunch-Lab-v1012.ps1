# Restore the two-instance lab on the unchanged live candidate1012 after both
# isolated instances exited (native ExitProcess 0, 2026-10-14 ~07:44).
$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$pin=Get-Content "$PSScriptRoot/candidate-npc-combat-v1012/identity.json" -Raw|ConvertFrom-Json
foreach($f in 'work/build/Release/dinput8.dll','outputs/d2-coop-lab/bin/dinput8.dll','work/asset-game/dinput8.dll'){if((Get-FileHash "$root/$f").Hash-ne$pin.dll){throw "Not candidate1012: $f"}}
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 [ordered]@{pid=$PID;operation='Relaunch1012';state='running'}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json"
 & "$PSScriptRoot/Deploy-SourceClock.ps1"
 & "$root/work/natural-moveset/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$PSScriptRoot/relaunch-v1012-$(Get-Date -Format yyyyMMddHHmmss)"
 if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Deployed DLL changed'}
 'Relaunched candidate1012'
}finally{if($owned){[ordered]@{pid=$PID;operation='Relaunch1012';state='finished'}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json";$mutex.ReleaseMutex()};$mutex.Dispose()}
