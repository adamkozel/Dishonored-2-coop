$ErrorActionPreference='Stop'
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 [ordered]@{pid=$PID;operation='CoinFixtureDeploy790';state='running'}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json"
 & 'C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe' -X utf8 "$PSScriptRoot/stage_coin_fixture_v790.py"
 if($LASTEXITCODE){throw 'Stage failed'}
 & "$PSScriptRoot/Deploy-SourceClock.ps1"
 if(!$?){throw 'Deploy failed'}
 $root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab"
 & "$root/work/natural-moveset/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$PSScriptRoot/candidate-coin-fixture-v790/note-restoration"
 foreach($role in 'host','client'){
  & "$lab/Send-D2Command.ps1" -Role $role -Command 'coop_decl_probe' -WaitConsumed | Out-Null
  Start-Sleep -Milliseconds 100
  Copy-Item "$root/work/lab-instances/$role/coop-status.json.climb-definition.json" "$PSScriptRoot/candidate-coin-fixture-v790/query-$role.json"
  Get-Content "$PSScriptRoot/candidate-coin-fixture-v790/query-$role.json"
 }
}finally{if($owned){[ordered]@{pid=$PID;operation='CoinFixtureDeploy790';state='finished'}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json";$mutex.ReleaseMutex()};$mutex.Dispose()}






