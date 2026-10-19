param([Parameter(Mandatory)][string]$TakeDir)
# 1531: both lab games were gone after the Claude Code session ended mid-take (window-sensor-v1527a): the games had been
# started from the session's own shell (1214el deploy), so they ended with it, and the take's cleanup never ran. This
# restarts both isolated instances with the deployed DLL (Deploy-SourceClock.ps1: protected saves checked before and after,
# Dunwall fixture prepared), restores the lab note inventory and records the recovery in the interrupted take.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab"
$take=Join-Path $PSScriptRoot $TakeDir;if(!(Test-Path $take)){throw 'Take folder missing'}
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$errors=[Collections.Generic.List[string]]::new()
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 [ordered]@{pid=$PID;operation='LabRestart1531';take=$TakeDir;started=(Get-Date).ToString('o');state='running'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
 $before=foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;[ordered]@{role=$r;pid=$l.pid;running=[bool](Get-Process -Id $l.pid -ErrorAction SilentlyContinue)}}
 try{& "$root/work/movement-integration/Deploy-SourceClock.ps1"|Out-Null}catch{$errors.Add("restart: $($_.Exception.Message)")}
 try{& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$take/recovery-note-restoration"|Out-Null}catch{$errors.Add("notes: $($_.Exception.Message)")}
 $after=foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$s=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json
  $hp=try{(& "$lab/bin/d2_runtime_probe.exe" $l.pid --health 1|ConvertFrom-Json).current}catch{$null}
  if($s.localGameLogicState-ne1-or$hp-ne56-or!$s.remoteFresh){$errors.Add("${r}: gameplay/health/transport not restored")}
  [ordered]@{role=$r;pid=$l.pid;logic=$s.localGameLogicState;health=$hp;remoteFresh=$s.remoteFresh}}
 [ordered]@{interrupted=$true;reason='The Claude Code session ended mid-take; the lab games (children of that session) ended with it and the take cleanup never ran';before=@($before);after=@($after);passed=(!$errors.Count);errors=@($errors);recoveredAt=(Get-Date).ToString('o')}|ConvertTo-Json -Depth 6|Set-Content "$take/recovery.json"
}finally{if($owned){[ordered]@{pid=$PID;operation='LabRestart1531';take=$TakeDir;ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json";$mutex.ReleaseMutex()};$mutex.Dispose()}
if($errors.Count){throw ($errors-join '; ')};'LAB_RESTARTED'
