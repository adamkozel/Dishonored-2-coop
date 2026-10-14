[CmdletBinding()]
param()
$ErrorActionPreference='Stop';$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$probe="$PSScriptRoot/bin/d2_runtime_probe.exe";$rows=[Collections.Generic.List[object]]::new();$failure=$null
function Command($r,$c){& "$PSScriptRoot/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function ReadState($r,$suffix){Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json}
function Capture($stage){
 $x=@{}
 foreach($r in 'host','client'){
  $b=ReadState $r '';$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  $raw=& $probe $l.pid --avatar-sword $b.probeActorHandle
  if($LASTEXITCODE){throw "Native inventory unavailable: $r $stage"}
  $x[$r]=[pscustomobject]@{Bridge=$b;Stream=(ReadState $r '.avatar-weapon.json');Lease=(ReadState $r '.avatar-sword.json');Native=($raw|ConvertFrom-Json)}
 }
 $row=[pscustomobject]@{Stage=$stage;Pair=$x};$rows.Add($row);return $x
}
function StopBoth{foreach($r in 'host','client'){Command $r 'coop_avatar_weapon off'}}
function Activate{
 StopBoth;Start-Sleep -Milliseconds 400
 foreach($r in 'host','client'){Command $r 'coop_probe_follow exact';Command $r 'coop_avatar_weapon 15000'}
 $w=[Diagnostics.Stopwatch]::StartNew()
 do{Start-Sleep -Milliseconds 100;$x=Capture Activating;$ok=$true;foreach($r in 'host','client'){if(!$x[$r].Stream.ready-or!$x[$r].Lease.active-or!$x[$r].Native.equipped){$ok=$false}}}while(!$ok-and$w.ElapsedMilliseconds-lt2500)
 if(!$ok){throw 'Both swords did not activate'}
 return $x
}
function Restored($before,$stage){
 $w=[Diagnostics.Stopwatch]::StartNew()
 do{Start-Sleep -Milliseconds 100;$x=Capture $stage;$ok=$true;foreach($r in 'host','client'){if($x[$r].Lease.active-or$x[$r].Lease.restoring-or$x[$r].Native.equipped){$ok=$false}}}while(!$ok-and$w.ElapsedMilliseconds-lt2500)
 if(!$ok){throw "Both swords did not restore: $stage"}
 foreach($r in 'host','client'){
  $a=$x[$r];$b=$before[$r]
  if($a.Native.started-or$a.Native.attachedTag-ne'holster_wpn'-or$a.Lease.lost-ne$b.Lease.lost-or
     ($a.Lease.restored-$b.Lease.restored)+($a.Lease.retired-$b.Lease.retired)-ne1){throw "Cleanup counter or attachment mismatch: $r $stage"}
 }
 return $x
}
try{
 foreach($role in 'host','client'){
  $before=Activate
  & "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button Start -DurationMs 200|Out-Null
  Start-Sleep -Milliseconds 450;$paused=Capture "$role-Paused"
  if($paused[$role].Bridge.localGameLogicState-ne2){throw 'Did not reach native pause'}
  foreach($r in 'host','client'){if($paused[$r].Stream.armed-or$paused[$r].Lease.active){throw 'Pause retained equipment'};Command $r 'coop_avatar_weapon 15000'}
  Start-Sleep -Milliseconds 250;$blocked=Capture "$role-PauseRearm"
  foreach($r in 'host','client'){if($blocked[$r].Stream.armed-or$blocked[$r].Lease.started-ne$before[$r].Lease.started){throw 'Rearm accepted during pause'}}
  & "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button A -DurationMs 200|Out-Null
  $null=Restored $before "$role-Resumed"
 }
 foreach($role in 'host','client'){
  $before=Activate;Command $role 'coop_avatar_weapon off'
  $null=Restored $before "$role-Disabled"
 }
 $before=Activate
 foreach($r in 'host','client'){Command $r 'coop_checkpoint_sync on'}
 & "$PSScriptRoot/Test-D2SharedCheckpoint.ps1" -RequestRole client -DelayRole host -Duplicate|Out-Null
 $after=Restored $before SharedReload
 foreach($r in 'host','client'){
  if($after[$r].Bridge.localEpoch-eq$before[$r].Bridge.localEpoch-or$after[$r].Stream.armed-or$after[$r].Lease.restoring){throw 'Old equipment survived shared reload'}
 }
}catch{$failure=$_.Exception.Message}finally{
 StopBoth
 foreach($r in 'host','client'){if((ReadState $r '').localGameLogicState-eq2){& "$PSScriptRoot/Send-D2Input.ps1" -Role $r -Button A -DurationMs 200|Out-Null}}
}
[ordered]@{Time=(Get-Date).ToString('o');Passed=(!$failure);Failure=$failure;DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;Samples=$rows;Scope='Both native swords across either native pause, rearm refusal, either stream disable and one duplicated shared checkpoint reload. Each lease restores or retires once.'}|ConvertTo-Json -Depth 24|Set-Content "$PSScriptRoot/research/partner-weapon-guards-verification.json"
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;Samples=$rows.Count;BothPauseRoles=$true;BothStopRoles=$true;SharedReload=$true}
