[CmdletBinding()]
param([switch]$Airborne,[switch]$SkipReload)
$ErrorActionPreference='Stop';$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent;$checks=@();$events=@()
function C($r,$s){& "$PSScriptRoot/Send-D2Command.ps1" -Role $r -Command $s -WaitConsumed|Out-Null}
function S($r){Get-Content "$root/work/lab-instances/$r/coop-status.json.shadow-stream.json" -Raw|ConvertFrom-Json}
function B($r){Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json}
function I($r,$b,$ms=180){& "$PSScriptRoot/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}
function Arm{
 foreach($r in 'host','client'){C $r 'coop_shadow_stream off';C $r 'coop_probe_follow exact';C $r 'coop_probe_locomotion on'}
 foreach($r in 'host','client'){C $r 'coop_shadow_stream 15000'}
 Start-Sleep -Milliseconds 700
 if(!(S host).ready-or!(S client).ready){throw 'Arming failed'}
}
$failure=$null
try{
 foreach($source in 'host','client'){
  $target=if($source-eq'host'){'client'}else{'host'};Arm
  if($Airborne){I $source A 220}else{I $source RightShoulder 3000;Start-Sleep -Milliseconds 500}
  $expected=if($Airborne){3}else{2}
  if($Airborne){$wait=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 20;$pending=S $target}while((!$pending.actionActive-or$pending.remoteState-ne3)-and$wait.ElapsedMilliseconds-lt650)}
  $before=S $target;if(!$before.actionActive-or$before.remoteState-ne$expected){throw 'No live remote action before pause'}
  if($Airborne){
   $launch=Get-Content "$root/work/lab-instances/$target/launch.json" -Raw|ConvertFrom-Json
   $animation=& "$PSScriptRoot/bin/d2_runtime_probe.exe" $launch.pid --animation (B $target).probeActorHandle|ConvertFrom-Json
   if($LASTEXITCODE-or((@($animation.declaredInputs|Where-Object name -eq 'animBasicInputData1')[0].rawWords[0]-band255)-ne9)){throw 'Native airborne clip not observed before pause'}
   $events+=[pscustomobject]@{Source=$source;Stage='BeforePause';Stream=$before;Animation=$animation}
  }
  I $source Start;Start-Sleep -Milliseconds 800
  $events+=[pscustomobject]@{Source=$source;Stage='Paused';Host=(S host);Client=(S client);HostBridge=(B host);ClientBridge=(B client)}
  if((B $source).localGameLogicState-ne2-or(S $target).armed-or(S $target).actionActive){throw 'Pause did not cancel active remote presentation'}
  $pausedApplied=(S $target).applied;C $target 'coop_shadow_stream 15000'
  if((S $target).armed){throw 'Rearm accepted against a paused peer'}
  I $source A;Start-Sleep -Milliseconds 500
  if((B $source).localGameLogicState-ne1){throw 'Native resume failed'}
  if((S $target).applied-ne$pausedApplied-or(S $target).actionActive){throw 'Resume replayed an old action'}
  $checks+="$source native pause cancels the active remote clip; rearm fails while paused and resume does not replay"
  Start-Sleep -Milliseconds 1600
 }
 if(!$SkipReload){
 Arm
 $beforeEpoch=@{host=(B host).localEpoch;client=(B client).localEpoch}
 & "$PSScriptRoot/Restore-D2Lab.ps1" -Doppelganger|Out-Null
 foreach($r in 'host','client'){if((S $r).armed-or(S $r).actionActive-or(B $r).localEpoch-eq$beforeEpoch[$r]){throw 'Reload did not retire the old stream'}}
 $events+=[pscustomobject]@{Stage='Restored';Host=(S host);Client=(S client);HostBridge=(B host);ClientBridge=(B client)}
 $checks+='Restoring both isolated checkpoints retires both old-world streams and their actions'
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r Neutral 100;C $r 'coop_shadow_stream off'}catch{}}
}
$report=if($Airborne){'doppelganger-airborne-guards.json'}else{'doppelganger-steam-action-guards.json'}
[ordered]@{Passed=(!$failure);Failure=$failure;At=[DateTimeOffset]::Now.ToString('o');Dll=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;Airborne=[bool]$Airborne;ReloadTested=(!$SkipReload);Checks=$checks;Events=$events;Scope='Either native pause cancels the held animation; rearm while paused and replay on resume refused. Optional independent preserved-checkpoint restoration retires old-world streams.'}|ConvertTo-Json -Depth 12|Set-Content "$PSScriptRoot/research/$report"
[pscustomobject]@{Passed=(!$failure);Failure=$failure;Checks=$checks.Count}
if($failure){throw $failure}
