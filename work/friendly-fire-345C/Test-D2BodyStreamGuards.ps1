[CmdletBinding()]
param([switch]$DisconnectClient)
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$rows=[Collections.Generic.List[object]]::new();$failure=$null
function State([string]$role){Get-Content "$root/work/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json}
function Command([string]$role,[string]$cmd){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $cmd|Out-Null}
function Input([string]$role,[string]$button){& "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button $button -DurationMs 150|Out-Null}
function Pair([string]$stage){$r=[pscustomobject]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o');Host=(State host);Client=(State client)};$rows.Add($r);return $r}
try {
 foreach($role in 'host','client'){Command $role 'coop_probe_body on'}
 foreach($source in 'host','client'){
  $target=if($source -eq 'host'){'client'}else{'host'}
  $before=Pair "$source before crouch"
  Input $source B;Start-Sleep -Milliseconds 700
  $active=Pair "$source streaming"
  if($active.$source.localPlayerStance -ne 1 -or $active.$target.bodyStreamApplied -le $before.$target.bodyStreamApplied){throw 'Body stream did not start'}
  Command $target 'coop_probe_body off';Start-Sleep -Milliseconds 400
  $off=Pair Disabled;Start-Sleep -Milliseconds 400;$still=Pair DisabledStable
  if($still.$target.bodyStreamApplied -ne $off.$target.bodyStreamApplied -or !$still.$target.remoteFresh){throw 'Disabled body stream continued'}
  Command $target 'coop_probe_body on';Start-Sleep -Milliseconds 450
  $on=Pair Reenabled
  if($on.$target.bodyStreamApplied -le $off.$target.bodyStreamApplied){throw 'Body stream did not restart'}
  Input $source Start;Start-Sleep -Milliseconds 650
  $paused=Pair "$source native pause"
  if($paused.$source.localGameLogicState -ne 2 -or $paused.$source.localBodyPoseValid -or $paused.$target.remoteBodyPoseValid){throw 'Pause did not invalidate source body data'}
  Command $target 'coop_probe_follow on';Start-Sleep -Milliseconds 450
  $held=Pair PauseGuard
  foreach($r in 'Host','Client'){if($held.$r.bodyStreamApplied -ne $paused.$r.bodyStreamApplied -or $held.$r.probeFollowEnabled){throw 'Body control survived pause'}}
  Input $source A;Start-Sleep -Milliseconds 700
  $resumed=Pair ResumeBeforeFollow
  foreach($r in 'Host','Client'){if($resumed.$r.bodyStreamApplied -ne $paused.$r.bodyStreamApplied){throw 'Body binding resumed without follow'}}
  foreach($role in 'host','client'){Command $role 'coop_probe_follow on'}
  Start-Sleep -Milliseconds 450
  $following=Pair FollowAgain
  if($following.$target.bodyStreamApplied -le $resumed.$target.bodyStreamApplied){throw 'Rebound body did not follow'}
  Input $source B;Start-Sleep -Milliseconds 650
 }
 if($DisconnectClient){
  Input client B;Start-Sleep -Milliseconds 650
  $active=Pair BeforeDisconnect
  $launch=Get-Content "$root/work/lab-instances/client/launch.json" -Raw|ConvertFrom-Json
  $game=Get-Process -Id $launch.pid
  if($game.Path -ne $launch.executable){throw 'Unexpected client process'}
  & taskkill.exe /PID $game.Id /F|Out-Null
  if(!$game.WaitForExit(10000)){throw 'Client failed to exit'}
  Start-Sleep -Milliseconds 600
  $lost=State host
  $rows.Add([pscustomobject]@{Stage='PeerLost600ms';Host=$lost})
  Start-Sleep -Milliseconds 450
  $still=State host
  $rows.Add([pscustomobject]@{Stage='PeerLost1050ms';Host=$still})
  if($still.bodyStreamApplied -ne $lost.bodyStreamApplied){throw 'Remote body pose survived its freshness lease'}
  & "$PSScriptRoot/Start-D2Lab.ps1" -Role client|Out-Null
  & "$PSScriptRoot/Restore-D2Lab.ps1"|Out-Null
  foreach($role in 'host','client'){Command $role 'coop_probe_body on'}
  $restored=Pair Restored
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($role in 'host','client'){
  $launch=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if(Get-Process -Id $launch.pid -ErrorAction SilentlyContinue){Input $role Neutral}
 }
 [ordered]@{Passed=($null -eq $failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;DisconnectClient=[bool]$DisconnectClient;Scope='Body stream disable/re-enable, both native source pauses, explicit follow refusal/rebinding and optional actual peer termination with the 250ms body lease. No online two-account validation.';Samples=$rows.ToArray()}|ConvertTo-Json -Depth 15|Set-Content "$PSScriptRoot/research/body-stream-guards-verification.json"
}
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;Samples=$rows.Count;DisconnectClient=[bool]$DisconnectClient}
