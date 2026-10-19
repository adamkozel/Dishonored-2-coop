$ErrorActionPreference='Stop'
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 [ordered]@{pid=$PID;operation='RecoilDiagDeploy1214du';state='running'}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json"
 & 'C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe' -X utf8 "$PSScriptRoot/stage_recoildiag_v1214du.py"
 if($LASTEXITCODE){throw 'Stage failed'}
 # Install bank75 (1409: +1 shop gesture clip): the client renders remote Corvo, the host renders remote Emily.
 $root0=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
 foreach($pair in @(@('client','corvo'),@('host','emily'))){Copy-Item "$root0/work/natural-moveset/shop-banks-v1409/$($pair[1])-choke-bank.bin" "$root0/work/lab-instances/$($pair[0])/coop-status.json.movement.bin" -Force}
 # 1214cf (#30): prop mechanism clips and ranged hand tracks come from the tables the installer rebuilds
 # (d2coop_setup tables; lab copy rebuilt from the original install, work/release/native-tables-v1).
 foreach($role in 'client','host'){Copy-Item "$root0/work/release/native-tables-v1/native-tables.bin" "$root0/work/lab-instances/$role/coop-status.json.tables.bin" -Force}
 & "$PSScriptRoot/Deploy-SourceClock.ps1"
 if(!$?){throw 'Deploy failed'}
 $root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab"
 & "$root/work/natural-moveset/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$PSScriptRoot/candidate-npc-combat-v1214du/note-restoration"
 foreach($role in 'host','client'){
  & "$lab/Send-D2Command.ps1" -Role $role -Command 'coop_decl_probe' -WaitConsumed | Out-Null
  Start-Sleep -Milliseconds 100
  Copy-Item "$root/work/lab-instances/$role/coop-status.json.climb-definition.json" "$PSScriptRoot/candidate-npc-combat-v1214du/query-$role.json"
  # 1214cf+: the tables the DLL looks up, loaded from the lab copy of the installer's file.
  & "$lab/Send-D2Command.ps1" -Role $role -Command 'coop_tables_probe' -WaitConsumed | Out-Null
  Start-Sleep -Milliseconds 100
  Copy-Item "$root/work/lab-instances/$role/coop-status.json.native-tables.json" "$PSScriptRoot/candidate-npc-combat-v1214du/native-tables-$role.json"
  $tables=Get-Content "$PSScriptRoot/candidate-npc-combat-v1214du/native-tables-$role.json" -Raw|ConvertFrom-Json
  if(!$tables.loaded-or$tables.found-ne$tables.expected){throw "Native tables incomplete on $role"}
  Get-Content "$PSScriptRoot/candidate-npc-combat-v1214du/query-$role.json"
 }
}finally{if($owned){[ordered]@{pid=$PID;operation='RecoilDiagDeploy1214du';state='finished'}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json";$mutex.ReleaseMutex()};$mutex.Dispose()}















