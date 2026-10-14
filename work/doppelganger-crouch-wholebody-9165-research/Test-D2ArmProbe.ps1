[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent;$probe="$PSScriptRoot/bin/d2_runtime_probe.exe"
$pids=@{};$cases=[Collections.Generic.List[object]]::new();$samples=[Collections.Generic.List[object]]::new();$failure=$null
function Command($role,$text){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $text -WaitConsumed|Out-Null}
function Bridge($role){Get-Content "$root/work/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json}
function State($role){Get-Content "$root/work/lab-instances/$role/coop-status.json.arm-probe.json" -Raw|ConvertFrom-Json}
function Native($role,$option,$handle=''){
 $arguments=@($pids[$role],$option);if($handle){$arguments+=$handle}
 for($i=0;$i-lt8;$i++){$raw=& $probe @arguments;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw "Native arm-probe read unavailable: $role $option"
}
function Arm($role){
 $before=State $role
 for($i=0;$i-lt4;$i++){
  Command $role 'coop_arm_probe 2000';Start-Sleep -Milliseconds 60;$now=State $role
  if($now.active-and$now.started-eq$before.started+1){return $now}
 }
 throw "Native arm pose did not arm: $($now.result)"
}
try{
 foreach($role in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if((Get-Process -Id $l.pid).Path-ne$l.executable){throw 'Unexpected process'};$pids[$role]=$l.pid
  Command $role 'coop_arm_probe off';Command $role 'coop_probe_body off'
 }
 foreach($role in 'host','client'){
  $other=if($role-eq'host'){'client'}else{'host'}
  Command $other 'setviewpos -28.0921 -153.962 68.1 270 0';Command $role 'setviewpos -28.0921 -148 68.1 270 0'
  foreach($who in 'host','client'){Command $who 'coop_probe_follow exact'}
  Start-Sleep -Milliseconds 400
  $handle=(Bridge $role).probeActorHandle;$entities=Native $role '--entities';$arm=@($entities.entities|Where-Object name -eq right_arm_2)
  if($arm.Count-ne1){throw 'Right-arm identity is ambiguous'}
  $source=Native $role '--arm-skeleton' $arm[0].handle;$target=Native $role '--skeleton' $handle
  $before=State $role;$otherBefore=State $other
  & "$PSScriptRoot/Send-D2Input.ps1" -Role $role -RightTrigger 255 -DurationMs 220|Out-Null
  Start-Sleep -Milliseconds 100;$armed=Arm $role
  for($i=0;$i-lt12;$i++){
   $state=State $role;$native=Native $role '--skeleton' $handle
   $samples.Add([pscustomobject]@{Role=$role;Index=$i;At=[DateTimeOffset]::Now.ToString('o');State=$state;Target=$native})
   if($i-eq1){Command $role 'coop_arm_probe 2000';$duplicate=State $role;if($duplicate.started-ne$armed.started-or$duplicate.remainingMs-ge$armed.remainingMs){throw 'Duplicate renewed the fixed arm lease'}}
   Start-Sleep -Milliseconds 60
  }
  $watch=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 80;$expired=State $role}while($expired.active-and$watch.ElapsedMilliseconds-lt2500)
  if($expired.active-or$expired.result-ne'expired'-or$expired.applied-le$before.applied){throw 'Native arm application or automatic expiry failed'}
  $restored=Native $role '--skeleton' $handle;Start-Sleep -Milliseconds 250;$settled=State $role
  if($settled.applied-ne$expired.applied-or(State $other).applied-ne$otherBefore.applied){throw 'Expired pose continued applying or changed the other counterpart'}
  $second=Arm $role;Command $role 'coop_arm_probe off';Start-Sleep -Milliseconds 150;$cancelled=State $role
  Start-Sleep -Milliseconds 200;$cancelledLater=State $role
  if($cancelled.active-or$cancelledLater.applied-ne$cancelled.applied){throw 'Cancelled arm pose continued applying'}
  $third=Arm $role
  & "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button Start -DurationMs 200|Out-Null
  Start-Sleep -Milliseconds 450;$paused=State $role
  if($paused.active-or(Bridge $role).localGameLogicState-ne2){throw 'Native pause did not invalidate the arm pose'}
  & "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button A -DurationMs 200|Out-Null
  Start-Sleep -Milliseconds 400;$resumed=State $role
  if($resumed.active-or$resumed.applied-ne$paused.applied){throw 'Native resume resurrected an expired arm pose'}
  $cases.Add([pscustomobject]@{Role=$role;Source=$source;Target=$target;Before=$before;Armed=$armed;Duplicate=$duplicate;Expired=$expired;Restored=$restored;Cancelled=$cancelled;Paused=$paused;Resumed=$resumed;OtherBefore=$otherBefore;OtherAfter=(State $other)})
 }
}catch{$failure=$_.Exception.Message}finally{foreach($role in 'host','client'){Command $role 'coop_arm_probe off'}}
[ordered]@{Passed=(!$failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;
 ProbeSha256=(Get-FileHash $probe).Hash;Scope='Two-second local snapshot of the actual right arm onto the same-instance owned counterpart; native expiry, cancellation, duplicate and pause guards. Independent matrix verification is required separately. No arm networking, sword equipment or visual parity claim.';Cases=$cases;Samples=$samples}|ConvertTo-Json -Depth 24|Set-Content "$PSScriptRoot/research/local-arm-probe-observations.json"
if($failure){throw $failure};[pscustomobject]@{Passed=$true;Cases=$cases.Count;Samples=$samples.Count}
