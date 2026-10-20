$ErrorActionPreference='Stop'
# 1214fy: completes the deploy after Deploy-TickAge-v1214fy.ps1 stopped in Restore-LabNoteInventory ("Native inventory
# read failed") once Deploy-SourceClock had already deployed and relaunched the lab with the 1214fy DLL. The candidate
# folder cannot be restaged, so this runs only the remaining steps: the note restoration (to a new evidence folder) and
# the decl and native-table probes.
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 [ordered]@{pid=$PID;operation='TickAgeComplete1214fy';state='running'}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json"
 $root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$cand="$PSScriptRoot/candidate-npc-combat-v1214fy"
 $id=Get-Content "$cand/identity.json" -Raw|ConvertFrom-Json
 if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$id.dll){throw 'The lab DLL is not 1214fy'}
 & "$root/work/natural-moveset/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$cand/note-restoration-retry"
 foreach($role in 'host','client'){
  & "$lab/Send-D2Command.ps1" -Role $role -Command 'coop_decl_probe' -WaitConsumed | Out-Null
  Start-Sleep -Milliseconds 100
  Copy-Item "$root/work/lab-instances/$role/coop-status.json.climb-definition.json" "$cand/query-$role.json"
  & "$lab/Send-D2Command.ps1" -Role $role -Command 'coop_tables_probe' -WaitConsumed | Out-Null
  Start-Sleep -Milliseconds 100
  Copy-Item "$root/work/lab-instances/$role/coop-status.json.native-tables.json" "$cand/native-tables-$role.json"
  $tables=Get-Content "$cand/native-tables-$role.json" -Raw|ConvertFrom-Json
  if(!$tables.loaded-or$tables.found-ne$tables.expected){throw "Native tables incomplete on $role"}
  Get-Content "$cand/query-$role.json"
 }
 'Complete1214fy: note restoration, decl and native-table probes passed'
}finally{if($owned){[ordered]@{pid=$PID;operation='TickAgeComplete1214fy';state='finished'}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json";$mutex.ReleaseMutex()};$mutex.Dispose()}
