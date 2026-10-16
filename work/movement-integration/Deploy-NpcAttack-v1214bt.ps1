$ErrorActionPreference='Stop'
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 [ordered]@{pid=$PID;operation='NpcCombatDeploy1214bt';state='running'}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json"
 & 'C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe' -X utf8 "$PSScriptRoot/stage_npc_attack_v1214bt.py"
 if($LASTEXITCODE){throw 'Stage failed'}
 # Install bank71 (1212: +5 interaction clips): the client renders remote Corvo, the host renders remote Emily.
 $root0=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
 foreach($pair in @(@('client','corvo'),@('host','emily'))){Copy-Item "$root0/work/natural-moveset/use-banks-v1266/$($pair[1])-choke-bank.bin" "$root0/work/lab-instances/$($pair[0])/coop-status.json.movement.bin" -Force}
 & "$PSScriptRoot/Deploy-SourceClock.ps1"
 if(!$?){throw 'Deploy failed'}
 $root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab"
 & "$root/work/natural-moveset/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$PSScriptRoot/candidate-npc-combat-v1214bt/note-restoration"
 foreach($role in 'host','client'){
  & "$lab/Send-D2Command.ps1" -Role $role -Command 'coop_decl_probe' -WaitConsumed | Out-Null
  Start-Sleep -Milliseconds 100
  Copy-Item "$root/work/lab-instances/$role/coop-status.json.climb-definition.json" "$PSScriptRoot/candidate-npc-combat-v1214bt/query-$role.json"
  Get-Content "$PSScriptRoot/candidate-npc-combat-v1214bt/query-$role.json"
 }
}finally{if($owned){[ordered]@{pid=$PID;operation='NpcCombatDeploy1214bt';state='finished'}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json";$mutex.ReleaseMutex()};$mutex.Dispose()}















