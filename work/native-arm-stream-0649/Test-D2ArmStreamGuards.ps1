[CmdletBinding()]
param([switch]$DisconnectClient)
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$samples=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null
function Command($role,$text){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $text -WaitConsumed|Out-Null}
function Input($role,$button){& "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button $button -DurationMs 150|Out-Null}
function State($role){Get-Content "$root/work/lab-instances/$role/coop-status.json.arm-stream.json" -Raw|ConvertFrom-Json}
function Bridge($role){Get-Content "$root/work/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json}
function Pair($stage){$p=[pscustomobject]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o');host=(State host);client=(State client);HostBridge=(Bridge host);ClientBridge=(Bridge client)};$samples.Add($p);return $p}
function Activate {
 foreach($role in 'host','client'){Command $role 'coop_probe_follow exact';Command $role 'coop_probe_arm on'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 100;$p=Pair Activating}while((!$p.host.active-or!$p.client.active)-and$w.ElapsedMilliseconds-lt2500)
 if(!$p.host.active-or!$p.client.active){throw 'Arm pair did not activate'}
}
function BothStopped($stage){
 Start-Sleep -Milliseconds 450;$p=Pair $stage;Start-Sleep -Milliseconds 350;$later=Pair "$stage stable"
 foreach($role in 'host','client'){if($p.$role.active-or$later.$role.active-or$p.$role.applied-ne$later.$role.applied){throw "Arm control survived $stage"}}
 return $later
}
try{
 Activate
 foreach($source in 'host','client'){
  $target=if($source-eq'host'){'client'}else{'host'}
  $before=Pair "$source active";Command $target 'coop_probe_arm off';Start-Sleep -Milliseconds 450;$off=Pair ReceiverOff
  Start-Sleep -Milliseconds 300;$later=Pair ReceiverOffStable
  if($later.$target.active-or$later.$target.applied-ne$off.$target.applied){throw 'Receiver disable failed'}
  Activate;$active=Pair Reenabled
  if($active.$target.applied-le$off.$target.applied){throw 'Receiver re-enable failed'}
  $checks.Add("$target native arm stops when disabled and resumes only after explicit opt-in")
  Input $source Start;Start-Sleep -Milliseconds 350
  if((Bridge $source).localGameLogicState-ne2){throw 'Native source pause unavailable'}
  $paused=BothStopped "$source pause"
  foreach($role in 'host','client'){Command $role 'coop_probe_follow exact'}
  $refused=BothStopped PauseRebindRefused
  Input $source A;Start-Sleep -Milliseconds 400;$resumed=BothStopped ResumeWithoutBinding
  foreach($role in 'host','client'){if($resumed.$role.applied-ne$paused.$role.applied){throw 'Resume resurrected native arm binding'}}
  Activate;$checks.Add("$source native pause cancels both arm leases; follow attempts during pause and native resume cannot resurrect them")
 }
 foreach($role in 'host','client'){Command $role 'coop_probe_follow on'}
 $offset=BothStopped OffsetFollow
 if(!$offset.HostBridge.remoteFresh-or!$offset.ClientBridge.remoteFresh){throw 'Offset-follow test lost unrelated Steam transport'}
 Activate;$checks.Add('Offset follow cannot retain the exact-position arm stream; explicit exact binding resumes it')
 foreach($role in 'host','client'){Command $role 'coop_probe_follow off'}
 $disabled=BothStopped FollowDisabled;Activate;$checks.Add('Disabling the owned counterpart releases both native arm poses')
 $old=Pair BeforeSharedReload
 & "$PSScriptRoot/Test-D2SharedCheckpoint.ps1" -RequestRole host -DelayRole client -Duplicate|Out-Null
 Activate;$new=Pair AfterSharedReload
 foreach($role in 'host','client'){
  $peer=if($role-eq'host'){'client'}else{'host'}
  if($new.$role.epoch-eq$old.$role.epoch-or$new.$role.peerEpoch-eq$old.$role.peerEpoch-or$new.$role.peerEpoch-ne$new.$peer.epoch-or$new.$role.sequence-le$old.$role.sequence){throw 'Arm binding did not require the new two-world identity'}
 }
 $checks.Add('Shared checkpoint reload with duplicate requests rebinds both rigs to new world epochs and fresh arm sequences')
 if($DisconnectClient){
  $before=Pair BeforeClientExit;$launch=Get-Content "$root/work/lab-instances/client/launch.json" -Raw|ConvertFrom-Json;$game=Get-Process -Id $launch.pid
  if($game.Path-ne$launch.executable){throw 'Unexpected client process'}
  & taskkill.exe /PID $game.Id /F|Out-Null;if(!$game.WaitForExit(10000)){throw 'Client did not exit'}
  Start-Sleep -Milliseconds 600;$lost=State host;Start-Sleep -Milliseconds 400;$later=State host
  $samples.Add([pscustomobject]@{Stage='ActualClientExit';host=$lost;Later=$later})
  if($lost.active-or$later.active-or$lost.applied-ne$later.applied){throw 'Arm stream survived actual peer termination'}
  & "$PSScriptRoot/Start-D2Lab.ps1" -Role client|Out-Null;& "$PSScriptRoot/Restore-D2Lab.ps1"|Out-Null
  Activate;$restored=Pair RestoredAfterClientExit
  if($restored.host.peerSession-eq$before.host.peerSession){throw 'Restart reused old peer identity'}
  $checks.Add('Actual client termination expires the host arm lease; reconnected stream uses the new Steam peer session')
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($role in 'host','client'){
  $launch=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if(Get-Process -Id $launch.pid -ErrorAction SilentlyContinue){Input $role Neutral;Command $role 'coop_probe_arm off'}
 }
}
[ordered]@{Passed=(!$failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;DisconnectClient=[bool]$DisconnectClient;
 Scope='Right-arm native application lifetime: local disable, both source pauses, pause rebind refusal, exact/offset follow, shared reload and optional actual peer termination. No online two-account validation.';Checks=$checks;Samples=$samples}|ConvertTo-Json -Depth 22|Set-Content "$PSScriptRoot/research/arm-stream-guards-verification.json"
if($failure){throw $failure};[pscustomobject]@{Passed=$true;Checks=$checks.Count;Samples=$samples.Count}
