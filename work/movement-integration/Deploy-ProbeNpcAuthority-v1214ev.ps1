$ErrorActionPreference='Stop'
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 [ordered]@{pid=$PID;operation='ProbeNpcAuthorityDeploy1214ev';state='running'}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json"
 # 1214ev is probe-only: the lab games keep running the 1214eu DLL; staging copies the new probe into the lab bin.
 & 'C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe' -X utf8 "$PSScriptRoot/stage_probenpcauthority_v1214ev.py"
 if($LASTEXITCODE){throw 'Stage failed'}
 $root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab"
 foreach($role in 'host','client'){$l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  foreach($mode in '--npc-authority','--npc-authority-all','--player-timepiece'){$raw=& "$lab/bin/d2_runtime_probe.exe" $l.pid $mode;"$role $mode exit=$LASTEXITCODE";$raw|Set-Content "$PSScriptRoot/candidate-npc-combat-v1214ev/smoke-$role$($mode-replace'^--','-').json"}}
}finally{if($owned){[ordered]@{pid=$PID;operation='ProbeNpcAuthorityDeploy1214ev';state='finished'}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json";$mutex.ReleaseMutex()};$mutex.Dispose()}
