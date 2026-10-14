[CmdletBinding()]
param([switch]$DisconnectHost)
$ErrorActionPreference='Stop';$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent;$probe="$PSScriptRoot/bin/d2_runtime_probe.exe"
$samples=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null
function Command($role,$text){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $text -WaitConsumed|Out-Null}
function Input($role,$button){& "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button $button -DurationMs 200|Out-Null}
function State($role){Get-Content "$root/work/lab-instances/$role/coop-status.json.avatar-weapon.json" -Raw|ConvertFrom-Json}
function Sword{Get-Content "$root/work/lab-instances/client/coop-status.json.avatar-sword.json" -Raw|ConvertFrom-Json}
function Bridge($role){Get-Content "$root/work/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json}
function Capture($stage){
 $b=Bridge client;$l=Get-Content "$root/work/lab-instances/client/launch.json" -Raw|ConvertFrom-Json
 $raw=& $probe $l.pid --avatar-sword $b.probeActorHandle;if($LASTEXITCODE){throw 'Native counterpart sword unavailable'}
 $r=[pscustomobject]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o');Host=(State host);Client=(State client);Sword=(Sword);Native=($raw|ConvertFrom-Json);Bridge=$b};$samples.Add($r);return $r
}
function Activate{
 foreach($role in 'host','client'){Command $role 'coop_avatar_weapon off'}
 Start-Sleep -Milliseconds 400
 foreach($role in 'host','client'){Command $role 'coop_probe_follow exact';Command $role 'coop_probe_arm on'}
 foreach($role in 'host','client'){Command $role 'coop_avatar_weapon 15000'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 100;$r=Capture Activating}while((!$r.Host.ready-or!$r.Client.ready-or!$r.Sword.active-or$r.Sword.observed-ne$r.Sword.started-or!$r.Native.equipped)-and$w.ElapsedMilliseconds-lt2500)
 if(!$r.Host.ready-or!$r.Client.ready-or!$r.Sword.active-or!$r.Native.equipped){throw 'Native remote sword failed to activate'};return $r
}
function Restored($before,$stage){
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 100;$r=Capture $stage}while(($r.Sword.active-or$r.Sword.restoring-or$r.Native.equipped)-and$w.ElapsedMilliseconds-lt2500)
 if($r.Sword.active-or$r.Sword.restoring-or$r.Native.equipped-or$r.Native.started-or$r.Native.attachedTag-ne'holster_wpn'-or$r.Sword.lost-ne$before.Sword.lost-or$r.Sword.restored-ne$before.Sword.restored+1){throw "Native weapon cleanup failed: $stage"};return $r
}
try{
 $a=Activate;Command client 'coop_avatar_weapon off';$off=Restored $a ReceiverOff
 if(!$off.Bridge.remoteFresh){throw 'Receiver disable lost unrelated root transport'}
 $checks.Add('Receiver disable restores its original holster while ordinary Steam traffic stays fresh')
 foreach($role in 'host','client'){
  $a=Activate;Input $role Start;Start-Sleep -Milliseconds 450;$p=Capture "$role pause"
  if((Bridge $role).localGameLogicState-ne2-or$p.Host.armed-or$p.Client.armed-or$p.Sword.active){throw 'Native pause retained equipment ownership'}
  foreach($side in 'host','client'){Command $side 'coop_avatar_weapon 15000'}
  Start-Sleep -Milliseconds 250;$refused=Capture PauseRearmRefused
  if($refused.Host.armed-or$refused.Client.armed-or$refused.Sword.started-ne$a.Sword.started){throw 'Equipment rearmed during native pause'}
  Input $role A;$r=Restored $a "$role resume";Start-Sleep -Milliseconds 350;$later=Capture ResumeStable
  if($later.Sword.active-or$later.Sword.started-ne$r.Sword.started){throw 'Native resume resurrected weapon stream'}
  $checks.Add("$role native pause cancels both equipment opt-ins, refuses rearming and restores holster after resume")
 }
 $a=Activate;Command client 'coop_probe_follow on';$r=Restored $a OffsetFollow
 Command client 'coop_avatar_weapon 15000';Start-Sleep -Milliseconds 300;$reject=Capture OffsetRefused
 if($reject.Client.armed-or$reject.Sword.started-ne$r.Sword.started){throw 'Offset-follow counterpart accepted equipment stream'}
 $checks.Add('Changing to offset follow restores native holster and refuses a new equipment stream')
 $a=Activate;Command host 'coop_avatar_weapon off';Command host 'coop_avatar_weapon 15000';$r=Restored $a OldReceiverToken
 if($r.Host.token-eq$a.Host.token-or$r.Client.token-ne$a.Client.token-or$r.Sword.started-ne$a.Sword.started){throw 'Changed source token rebound the old receiver lease'}
 $checks.Add('A new source opt-in token cannot replace an existing receiver binding or resurrect its native equipment')
 $a=Activate
 foreach($role in 'host','client'){Command $role 'coop_avatar_weapon off';Command $role 'coop_checkpoint_sync on'}
 Start-Sleep -Milliseconds 500;$a=Activate
 & "$PSScriptRoot/Test-D2SharedCheckpoint.ps1" -RequestRole client -DelayRole host -Duplicate|Out-Null
 $r=Capture SharedReload
 if($r.Host.armed-or$r.Client.armed-or$r.Sword.active-or$r.Sword.restoring-or$r.Native.equipped-or$r.Native.started-or$r.Bridge.localEpoch-eq$a.Bridge.localEpoch-or$r.Sword.lost-ne$a.Sword.lost-or($r.Sword.restored-$a.Sword.restored)+($r.Sword.retired-$a.Sword.retired)-ne1){throw 'Shared reload retained or lost the old equipment ownership'}
 $checks.Add('Client native reload with overlapping requests invalidates equipment tokens, retires/restores the old lease once and restores a holstered new-world sword')
 if($DisconnectHost){
  & "$PSScriptRoot/Test-D2PlayerArms.ps1"|Out-Null;$a=Activate
  $l=Get-Content "$root/work/lab-instances/host/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid;if($p.Path-ne$l.executable){throw 'Unexpected host process'}
  & taskkill.exe /PID $p.Id /F|Out-Null;if(!$p.WaitForExit(10000)){throw 'Host did not exit'}
  $r=Restored $a ActualHostExit
  & "$PSScriptRoot/Start-D2Lab.ps1" -Role host|Out-Null;& "$PSScriptRoot/Restore-D2Lab.ps1"|Out-Null
  $r=Capture Reconnected
  if($r.Client.armed-or$r.Sword.active-or$r.Native.equipped){throw 'Reconnection resurrected native equipment'}
  $checks.Add('Actual host termination restores the native holster; reconnect and restored fixtures do not reactivate prior equipment')
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($role in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if(Get-Process -Id $l.pid -ErrorAction SilentlyContinue){Input $role Neutral;Command $role 'coop_avatar_weapon off';Command $role 'coop_probe_arm off'}
 }
}
[ordered]@{Passed=(!$failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;ProbeSha256=(Get-FileHash $probe).Hash;DisconnectHost=[bool]$DisconnectHost;
 Scope='Corvo native equipment network lifecycle: receiver disable, either pause, follow mode, changed source opt-in, shared checkpoint and optional actual host termination. No two-account internet validation.';Checks=$checks;Samples=$samples}|ConvertTo-Json -Depth 22|Set-Content "$PSScriptRoot/research/avatar-weapon-guards-verification.json"
if($failure){throw $failure};[pscustomobject]@{Passed=$true;Checks=$checks.Count;Samples=$samples.Count}
