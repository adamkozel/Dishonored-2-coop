$ErrorActionPreference='Stop'
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 [ordered]@{pid=$PID;operation='PossessLeaseDeploy1214ij';state='running'}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json"
 & 'C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe' -X utf8 "$PSScriptRoot/stage_possesslease_v1214ij.py"
 if($LASTEXITCODE){throw 'Stage failed'}
 # Install bank75 (1409: +1 shop gesture clip): the client renders remote Corvo, the host renders remote Emily.
 $root0=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
 foreach($pair in @(@('client','corvo'),@('host','emily'))){Copy-Item "$root0/work/natural-moveset/shop-banks-v1409/$($pair[1])-choke-bank.bin" "$root0/work/lab-instances/$($pair[0])/coop-status.json.movement.bin" -Force}
 # 1214ij (#159): each lab instance also keeps both partner banks; the DLL picks the one for the partner's character.
 foreach($role in 'client','host'){foreach($c in 'corvo','emily'){Copy-Item "$root0/work/natural-moveset/shop-banks-v1409/$c-choke-bank.bin" "$root0/work/lab-instances/$role/coop-status.json.movement-$c.bin" -Force}}
 # 1214cf (#30): prop mechanism clips and ranged hand tracks come from the tables the installer rebuilds
 # (d2coop_setup tables; lab copy rebuilt from the original install, work/release/native-tables-v1).
 foreach($role in 'client','host'){Copy-Item "$root0/work/release/native-tables-v1/native-tables.bin" "$root0/work/lab-instances/$role/coop-status.json.tables.bin" -Force}
 # 1214el+: the remote Windblast upgrade effects (windblast-upgrade-v1) go into the isolated asset game while the lab
 # games are stopped (the asset builder refuses running instances); skipped when already installed.
 $root1=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$exe1=Join-Path $root1 'work/asset-game/Dishonored2.exe'
 $have=Select-String -LiteralPath "$root1/work/asset-game/base/game1voidmod.index" -Pattern 'windblast_wave_remote' -SimpleMatch -Quiet
 if(!$have){
  foreach($role in 'host','client'){$l=Get-Content "$root1/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json;$gp=Get-Process -Id $l.pid -ErrorAction SilentlyContinue
   if(!$gp){continue};if($gp.Path-ne$exe1-or[Math]::Abs(($gp.StartTime-([DateTime]$l.startedAt)).TotalSeconds)-gt5){throw 'Game identity changed'}
   Stop-Process -Id $gp.Id;if(!$gp.WaitForExit(15000)){throw 'Isolated process did not fully exit'}}
  & "$root1/outputs/d2-coop-lab/Build-D2WindblastUpgradeAssets.ps1" -Install *> "$PSScriptRoot/candidate-npc-combat-v1214ij/windblast-upgrade-assets-install.log"
  if(!$?){throw 'Windblast upgrade asset install failed'}
  if(!(Select-String -LiteralPath "$root1/work/asset-game/base/game1voidmod.index" -Pattern 'windblast_wave_remote' -SimpleMatch -Quiet)){throw 'Windblast upgrade assets missing after install'}
 }
 # 1214eu+ (#102): the render-only Timepiece prop (timepiece-prop-presentation-v1) goes in the same way when missing.
 if(!(Select-String -LiteralPath "$root1/work/asset-game/base/game1voidmod.index" -Pattern 'timeshift_prop' -SimpleMatch -Quiet)){
  foreach($role in 'host','client'){$l=Get-Content "$root1/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json;$gp=Get-Process -Id $l.pid -ErrorAction SilentlyContinue
   if(!$gp){continue};if($gp.Path-ne$exe1-or[Math]::Abs(($gp.StartTime-([DateTime]$l.startedAt)).TotalSeconds)-gt5){throw 'Game identity changed'}
   Stop-Process -Id $gp.Id;if(!$gp.WaitForExit(15000)){throw 'Isolated process did not fully exit'}}
  & "$root1/outputs/d2-coop-lab/Build-D2TimepiecePropAssets.ps1" -Install *> "$PSScriptRoot/candidate-npc-combat-v1214ij/timepiece-prop-assets-install.log"
  if(!$?){throw 'Timepiece prop asset install failed'}
  if(!(Select-String -LiteralPath "$root1/work/asset-game/base/game1voidmod.index" -Pattern 'timeshift_prop' -SimpleMatch -Quiet)){throw 'Timepiece prop asset missing after install'}
 }
 & "$PSScriptRoot/Deploy-SourceClock.ps1"
 if(!$?){throw 'Deploy failed'}
 $root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab"
 & "$root/work/natural-moveset/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$PSScriptRoot/candidate-npc-combat-v1214ij/note-restoration"
 foreach($role in 'host','client'){
  & "$lab/Send-D2Command.ps1" -Role $role -Command 'coop_decl_probe' -WaitConsumed | Out-Null
  Start-Sleep -Milliseconds 100
  Copy-Item "$root/work/lab-instances/$role/coop-status.json.climb-definition.json" "$PSScriptRoot/candidate-npc-combat-v1214ij/query-$role.json"
  # 1214cf+: the tables the DLL looks up, loaded from the lab copy of the installer's file.
  & "$lab/Send-D2Command.ps1" -Role $role -Command 'coop_tables_probe' -WaitConsumed | Out-Null
  Start-Sleep -Milliseconds 100
  Copy-Item "$root/work/lab-instances/$role/coop-status.json.native-tables.json" "$PSScriptRoot/candidate-npc-combat-v1214ij/native-tables-$role.json"
  $tables=Get-Content "$PSScriptRoot/candidate-npc-combat-v1214ij/native-tables-$role.json" -Raw|ConvertFrom-Json
  if(!$tables.loaded-or$tables.found-ne$tables.expected){throw "Native tables incomplete on $role"}
  Get-Content "$PSScriptRoot/candidate-npc-combat-v1214ij/query-$role.json"
 }
}finally{if($owned){[ordered]@{pid=$PID;operation='PossessLeaseDeploy1214ij';state='finished'}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json";$mutex.ReleaseMutex()};$mutex.Dispose()}















