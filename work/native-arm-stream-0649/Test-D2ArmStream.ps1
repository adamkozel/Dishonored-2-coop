[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent;$probe="$PSScriptRoot/bin/d2_runtime_probe.exe"
$pids=@{};$cases=[Collections.Generic.List[object]]::new();$samples=[Collections.Generic.List[object]]::new();$failure=$null
function Command($role,$text){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $text -WaitConsumed|Out-Null}
function Bridge($role){Get-Content "$root/work/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json}
function State($role){Get-Content "$root/work/lab-instances/$role/coop-status.json.arm-stream.json" -Raw|ConvertFrom-Json}
function Native($role,$option,$handle=''){
 $arguments=@($pids[$role],$option);if($handle){$arguments+=$handle}
 for($i=0;$i-lt8;$i++){$raw=& $probe @arguments;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw "Native arm stream read unavailable: $role $option"
}
function Input($role,$button){& "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button $button -DurationMs 150|Out-Null}
function Capture($role,$other,$stage,$count,$handle){
 for($i=0;$i-lt$count;$i++){
  if($stage-like'*Slash'-and$i%8-eq0){& "$PSScriptRoot/Send-D2Input.ps1" -Role $role -RightTrigger 255 -DurationMs 180|Out-Null}
  $before=State $other;$native=Native $other '--skeleton' $handle;$after=State $other
  $samples.Add([pscustomobject]@{Role=$role;Observer=$other;Stage=$stage;Index=$i;At=[DateTimeOffset]::Now.ToString('o');Before=$before;Target=$native;After=$after;SourceBridge=(Bridge $role);TargetBridge=(Bridge $other)})
  Start-Sleep -Milliseconds 65
 }
}
try{
 foreach($role in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if((Get-Process -Id $l.pid).Path-ne$l.executable){throw 'Unexpected process'};$pids[$role]=$l.pid
  Command $role 'coop_probe_arm off';Command $role 'coop_arm_probe off';Command $role 'coop_probe_body on'
 }
 foreach($role in 'host','client'){
  $other=if($role-eq'host'){'client'}else{'host'}
  Command $role 'setviewpos -28.0921 -153.962 68.1 90 0';Command $other 'setviewpos -28.0921 -148 68.1 270 0'
  foreach($who in 'host','client'){Command $who 'coop_probe_follow exact'}
  Start-Sleep -Milliseconds 600
  $handle=(Bridge $other).probeActorHandle;$entities=Native $other '--entities';$arm=@($entities.entities|Where-Object name -eq right_arm_2)
  if($arm.Count-ne1){throw 'Right-arm identity is ambiguous'}
  $sourceRig=Native $other '--arm-skeleton' $arm[0].handle;$targetRig=Native $other '--skeleton' $handle
  $before=State $other;$sourceBefore=State $role
  Command $role 'coop_probe_arm on';Start-Sleep -Milliseconds 400;$oneSided=State $other
  if($oneSided.active-or$oneSided.applied-ne$before.applied){throw 'Disabled receiver applied source arm'}
  Command $other 'coop_probe_arm on';Start-Sleep -Milliseconds 700;$armed=State $other
  if(!$armed.active-or$armed.applied-le$before.applied){throw "Arm stream unavailable: $role to $other"}
  Capture $role $other Idle 8 $handle
  Capture $role $other StandingSlash 24 $handle
  Input $role B;Start-Sleep -Milliseconds 650
  Capture $role $other CrouchSlash 24 $handle
  Input $role B;Start-Sleep -Milliseconds 650
  Capture $role $other ReturnedIdle 8 $handle
  $after=State $other
  Command $role 'coop_arm_duplicate';Start-Sleep -Milliseconds 400;$duplicate=State $other
  if($duplicate.rejected-le$after.rejected){throw 'Repeated arm packet was not rejected'}
  $freshBefore=Bridge $other;Command $role 'coop_probe_arm off';Start-Sleep -Milliseconds 650;$expired=State $other;$freshAfter=Bridge $other
  Start-Sleep -Milliseconds 300;$later=State $other;$restored=Native $other '--skeleton' $handle
  if($expired.active-or$later.applied-ne$expired.applied-or!$freshAfter.remoteFresh-or$freshAfter.received-le$freshBefore.received){throw 'Arm silence failed to expire independently of fresh root traffic'}
  if((Native $role '--health' 1).current-ne56-or(Native $other '--health' 1).current-ne56){throw 'Safe-room arm stream damaged a player'}
  $cases.Add([pscustomobject]@{Role=$role;Observer=$other;SourceRig=$sourceRig;TargetRig=$targetRig;Before=$before;SourceBefore=$sourceBefore;OneSided=$oneSided;Armed=$armed;After=$after;Duplicate=$duplicate;Expired=$expired;Later=$later;Restored=$restored})
  foreach($who in 'host','client'){Command $who 'coop_probe_arm off'}
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($role in 'host','client'){Input $role Neutral;Command $role 'coop_probe_arm off';if((Bridge $role).localPlayerStance-eq1){Input $role B}}
}
[ordered]@{Passed=(!$failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;
 ProbeSha256=(Get-FileHash $probe).Hash;Scope='Both actual native sword arms transmitted through Steam and applied to their owned counterpart rigs, alongside body crouch stream, receiver opt-in, duplicate and arm-only silence expiry. Independent rotation verification required separately; no sword attachment, finger, full aiming or combat parity claim.';Cases=$cases;Samples=$samples}|ConvertTo-Json -Depth 26|Set-Content "$PSScriptRoot/research/arm-stream-observations.json"
if($failure){throw $failure};[pscustomobject]@{Passed=$true;Cases=$cases.Count;Samples=$samples.Count}
