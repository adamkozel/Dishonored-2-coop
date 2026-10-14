[CmdletBinding()]
param()
$ErrorActionPreference='Stop';$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent;$probe="$PSScriptRoot/bin/d2_runtime_probe.exe"
$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$pids=@{}
function Cmd($r,$s){& "$PSScriptRoot/Send-D2Command.ps1" -Role $r -Command $s -WaitConsumed|Out-Null}
function State($r){Get-Content "$root/work/lab-instances/$r/coop-status.json.shadow-stream.json" -Raw|ConvertFrom-Json}
function Bridge($r){Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json}
function Native($r,$flag,$h=''){$args=@($pids[$r],$flag);if($h){$args+=$h};for($n=0;$n-lt8;$n++){$raw=& $probe @args;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 10};throw "Probe unavailable $r $flag"}
function Capture($source,$stage){
 $target=if($source-eq'host'){'client'}else{'host'};$b=Bridge $target
 if($b.localGameLogicState-ne1-or!$b.remoteFresh-or!$b.probeFollowExact-or!$b.probeFollowEnabled){throw 'Target scope lost'}
 $a=Native $target '--animation' $b.probeActorHandle;$sk=Native $target '--skeleton' $b.probeActorHandle;$w=Native $target '--avatar-sword-attachment' $b.probeActorHandle
 $r=[pscustomobject]@{Role=$source;Stage=$stage;At=[DateTimeOffset]::Now.ToString('o');Source=(State $source);Target=(State $target);Player=(Native $source '--player-states');
  Input=@($a.declaredInputs|Where-Object name -eq animBasicInputData1)[0];Root=$sk.root;Stable=$sk.stable;Attachment=$w;
  Joints=@($sk.joints|Where-Object nativeName -in 'right_arm_jnt','right_elbow_jnt','right_wrist_jnt'|Select-Object nativeName,localPose)}
 $rows.Add($r);$r
}
function OffBoth{foreach($r in 'host','client'){Cmd $r 'coop_shadow_stream off'};Start-Sleep -Milliseconds 450}
function ArmBoth{
 foreach($r in 'host','client'){Cmd $r 'coop_shadow_stream 15000'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 80;$h=State host;$c=State client}while((!$h.ready-or!$c.ready-or!$h.accepted-or!$c.accepted)-and$w.ElapsedMilliseconds-lt2000)
 if(!$h.ready-or!$c.ready-or$h.token-ne$c.peerToken-or$c.token-ne$h.peerToken){throw 'Both action-stream opt-ins did not agree'}
}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne$l.executable){throw 'Unexpected process'};$pids[$r]=$l.pid
  $b=Bridge $r;if($b.bodyStreamEnabled){throw 'Body stream must be disabled for the native clone experiment'}
  if((Native $r '--health' 1).current-ne56){throw 'Expected untouched player health 56'}
 }
 OffBoth;$before=State client;Cmd client 'coop_shadow_stream 15000';Start-Sleep -Milliseconds 600;$one=State client
 if($one.ready-or$one.applied-ne$before.applied-or$one.actionActive){throw 'One-sided opt-in activated an action'}
 OffBoth;$checks.Add('Receiver-only opt-in cannot start an animation')
 foreach($source in 'host','client'){
  $target=if($source-eq'host'){'client'}else{'host'};ArmBoth;Start-Sleep -Milliseconds 150
  $base=Capture $source Baseline
  & "$PSScriptRoot/Send-D2Input.ps1" -Role $source -RightTrigger 255 -DurationMs 220|Out-Null
  for($i=0;$i-lt14;$i++){Capture $source Attack|Out-Null;Start-Sleep -Milliseconds 35}
  $after=State $target
  if($after.applied-ne$base.Target.applied+1){throw "$source real slash did not produce exactly one remote animation"}
  $attack=@($rows|Where-Object {$_.Role-eq$source-and$_.Stage-eq'Attack'})
  if(!@($attack|Where-Object {($_.Input.rawWords[0]-band255)-eq5}).Count){throw 'Remote native attack index never observed'}
  if(!@($attack.Player.active|Where-Object id -eq 39).Count){throw 'Real native source slash was not observed'}
  $movement=0.;foreach($r in @($attack|Where-Object Stable)){foreach($j in $r.Joints){$idle=@($base.Joints|Where-Object nativeName -eq $j.nativeName)[0];foreach($k in 0,1,2,4,5,6,8,9,10){$movement=[Math]::Max($movement,[Math]::Abs($j.localPose[$k]-$idle.localPose[$k]))}}}
  if($movement-lt0.1){throw 'Remote native arm pose did not change'}
  $rejectBefore=$after.rejected;Cmd $source 'coop_shadow_stream_duplicate';Start-Sleep -Milliseconds 350;$duplicate=State $target
  if($duplicate.applied-ne$after.applied-or$duplicate.rejected-le$rejectBefore){throw 'Duplicated state packet was not rejected'}
  & "$PSScriptRoot/Send-D2Input.ps1" -Role $source -Button RightShoulder -DurationMs 1300|Out-Null
  for($i=0;$i-lt20;$i++){Capture $source BlockRelease|Out-Null;Start-Sleep -Milliseconds 35}
  $end=State $target;$block=@($rows|Where-Object {$_.Role-eq$source-and$_.Stage-eq'BlockRelease'})
  if($end.applied-ne$duplicate.applied+2){throw "$source block and release did not produce exactly one parry and one recovery"}
  if(!@($block|Where-Object {($_.Input.rawWords[0]-band255)-eq6}).Count-or!@($block.Player.active|Where-Object id -eq 44).Count){throw 'Native source block and remote held parry were not both observed'}
  if(!$end.armed-or$end.refused-ne$base.Target.refused){throw 'Action stream expired or native playback was refused during the case'}
  $checks.Add("$source real native slash, block and release drive the other clone over Steam exactly once per transition; duplicate state is rejected and arm pose changes")
  OffBoth
  foreach($r in 'host','client'){if((State $r).armed-or(State $r).actionActive){throw 'Stop did not release clone animation'}}
 }
 ArmBoth
 Cmd host 'coop_shadow_stream off';Start-Sleep -Milliseconds 800
 if((State client).armed-or(State client).actionActive){throw 'Peer withdrawal did not retire the receiver'}
 $checks.Add('Peer withdrawal retires the receiver and its native clip')
 OffBoth;ArmBoth;Start-Sleep -Milliseconds 15800
 foreach($r in 'host','client'){if((State $r).armed-or(State $r).actionActive){throw 'Nonrenewable stream exceeded its lease'}}
 $checks.Add('Both streams stop at the nonrenewable fifteen-second lease')
 foreach($r in 'host','client'){if((Native $r '--health' 1).current-ne56){throw 'Player health changed'};if((Native $r '--health' (Bridge $r).probeActorHandle).current-ne40){throw 'Clone health changed'}}
 $checks.Add('Both players retain health 56 and clones health 40; no finger or first-person animation stream is used')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{Cmd $r 'coop_shadow_stream off';& "$PSScriptRoot/Send-D2Input.ps1" -Role $r -DurationMs 100|Out-Null}catch{}}
}
[ordered]@{Passed=(!$failure);Failure=$failure;At=[DateTimeOffset]::Now.ToString('o');Dll=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;
 Scope='Two isolated Steam loopback instances; actual native source slash and block states, native clone clip playback and poses. Single slash episodes only; no combo direction, real hit/parry outcome, online account, or full moveset claim.';
 Checks=$checks.ToArray();Samples=$rows.ToArray()}|ConvertTo-Json -Depth 15|Set-Content "$PSScriptRoot/research/doppelganger-steam-action-verification.json"
[pscustomobject]@{Passed=(!$failure);Failure=$failure;Checks=$checks.Count;Samples=$rows.Count}
if($failure){throw $failure}
