[CmdletBinding()]
param([string]$ProbePath="$PSScriptRoot/bin/d2_runtime_probe.exe",[switch]$Doppelganger)
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$pids=@{};$arms=@{};$damage=@{};$samples=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null
function Command($role,$command){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $command -WaitConsumed|Out-Null}
function Bridge($role){Get-Content "$root/work/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json}
function Native($role,$option,$handle=''){
 $arguments=@($pids[$role],$option);if($handle){$arguments+=$handle}
 for($i=0;$i-lt8;$i++){$raw=& $ProbePath @arguments;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20}
 throw "Native arm observation unavailable: $role $option $handle"
}
function Resolve {
 foreach($role in 'host','client'){
  $launch=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if((Get-Process -Id $launch.pid).Path-ne$launch.executable){throw 'Unexpected native process'};$pids[$role]=$launch.pid
  $entities=Native $role '--entities';$arms[$role]=@{}
  foreach($side in 'left','right'){
   $name=if($side-eq'left'){'left_arm_1'}else{'right_arm_2'};$entity=@($entities.entities|Where-Object name -eq $name)
   if($entity.Count-ne1){throw 'Arm entity identity is ambiguous'};$arms[$role][$side]=$entity[0]
  }
 }
}
function Sample($role,$stage){
 $before=Bridge $role;$left=Native $role '--arm-skeleton' $arms[$role].left.handle;$right=Native $role '--arm-skeleton' $arms[$role].right.handle;$after=Bridge $role
 if($before.localEpoch-ne$after.localEpoch-or$after.localGameLogicState-ne1){throw 'Arm observation crossed a world/state boundary'}
 $character=if($role-eq'host'){1}else{2}
 foreach($s in $left,$right){if($s.armCharacter-ne$character-or$s.joints.Count-ne69-or$s.phases.Count-ne4){throw 'Unexpected arm rig identity'}}
 if($left.armSide-ne1-or$right.armSide-ne2){throw 'Arm sides were exchanged'}
  $cloneHealth=$null
  if($after.probeActorHandle){$cloneHealth=Native $role '--health' $after.probeActorHandle}
  $sample=[pscustomobject]@{Role=$role;Stage=$stage;At=[DateTimeOffset]::Now.ToString('o');Epoch=$after.localEpoch;Left=$left;Right=$right;LocalPosition=$after.localPosition;ClonePosition=$after.probeRenderPosition;CloneHealth=$cloneHealth;Follow=$after.probeFollowEnabled}
 $samples.Add($sample);return $sample
}
try{
 & "$PSScriptRoot/Restore-D2Lab.ps1" -Doppelganger:$Doppelganger|Out-Null;Resolve
 foreach($role in 'host','client'){
  Command $role 'setviewpos -39.4 -161.4 68.1 270 20'; & "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button X -DurationMs 400|Out-Null
 }
 $seen=@{host=$false;client=$false};$watch=[Diagnostics.Stopwatch]::StartNew()
 do{
  $ready=$true;foreach($role in 'host','client'){$s=Bridge $role;if($s.localGameLogicState-eq5){$seen[$role]=$true};if(!$seen[$role]-or$s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}}
  if(!$ready){Start-Sleep -Milliseconds 150}
 }while(!$ready-and$watch.ElapsedMilliseconds-lt45000)
 if(!$ready){throw 'Both native Mayhew scenes did not complete'}
 foreach($role in 'host','client'){Command $role 'setviewpos -28.0921 -153.962 68.1 270 0'}
 Start-Sleep -Milliseconds 300;Resolve
 foreach($role in 'host','client'){Command $role 'coop_damage_observe off';Command $role 'coop_damage_observe 60000'}
 foreach($role in 'host','client'){
  foreach($side in 'left','right'){
   & $ProbePath $pids[$role] --skeleton $arms[$role][$side].handle|Out-Null
   if($LASTEXITCODE-ne53){throw 'Ordinary actor scope unexpectedly accepted a separate player arm'}
  }
  $idle=Sample $role Idle
  & "$PSScriptRoot/Send-D2Input.ps1" -Role $role -RightTrigger 255 -DurationMs 220|Out-Null
  for($i=0;$i-lt14;$i++){Sample $role Swing|Out-Null;Start-Sleep -Milliseconds 25}
  Start-Sleep -Milliseconds 800;Sample $role Settled|Out-Null
  $stable=@($samples|Where-Object {$_.Role-eq$role-and$_.Stage-eq'Swing'-and$_.Right.stable})
  if($stable.Count-lt4){throw 'Too few stable native sword-arm observations'}
  $changed=$false
  foreach($sample in $stable){foreach($j in 1,2,3,4){foreach($k in 0,1,2,4,5,6,8,9,10){if([Math]::Abs($sample.Right.joints[$j].localPose[$k]-$idle.Right.joints[$j].localPose[$k])-gt.05){$changed=$true}}}}
  if(!$changed){throw 'No independent native upper-arm or wrist rotation change during the real controller slash'}
  $checks.Add("$role has two separately owned 69-joint arm rigs; ordinary actor scope rejects both, and a real sword slash changes the native right-arm joints")
 }
 foreach($role in 'host','client'){if((Native $role '--health' 1).current-ne56){throw 'Safe-room observation unexpectedly damaged a player'}}
 $checks.Add('Both players retain checkpoint health 56; the probe performs no arm or animation mutations')
}catch{$failure=$_.Exception.Message}finally{
 foreach($role in 'host','client'){try{Command $role 'coop_damage_observe off';Start-Sleep -Milliseconds 200;$damage[$role]=Get-Content "$root/work/lab-instances/$role/coop-status.json.damage.json" -Raw|ConvertFrom-Json}catch{}}
}
[ordered]@{Passed=(!$failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;
 ProbeSha256=(Get-FileHash $ProbePath).Hash;Scope='Read-only native first-person arm identity and real local slash observation for both roles. No remote arm application, weapon replication or visual parity claim.';
 Checks=$checks;Arms=$arms;Samples=$samples;DamageObservation=$damage}|ConvertTo-Json -Depth 22|Set-Content "$PSScriptRoot/research/player-arm-observation-verification.json"
if($failure){throw $failure};[pscustomobject]@{Passed=$true;Checks=$checks.Count;Samples=$samples.Count}
