[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$probe=Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe'
$rows=[Collections.Generic.List[object]]::new();$cases=[Collections.Generic.List[object]]::new();$launch=@{};$failure=$null
function State([string]$role){Get-Content "$root/work/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json}
function Command([string]$role,[string]$cmd){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $cmd|Out-Null}
function Input([string]$role,[string]$button){& "$PSScriptRoot/Send-D2Input.ps1" -Role $role -Button $button -DurationMs 150|Out-Null}
function Skeleton([string]$role,[uint32]$handle){$raw=& $probe $launch[$role].pid --skeleton $handle;if(!$LASTEXITCODE){return ($raw|ConvertFrom-Json)};if($LASTEXITCODE -ne 53){throw 'Native skeleton probe failed'};return $null}
function Capture([string]$role,[string]$stage,[int]$count){
 $other=if($role -eq 'host'){'client'}else{'host'}
 $handle=(State $other).probeActorHandle
 for($i=0;$i -lt $count;$i++){
  $row=[pscustomobject]@{Role=$role;Stage=$stage;At=[DateTimeOffset]::Now.ToString('o');Source=(Skeleton $role 1);Target=(Skeleton $other $handle);SourceState=(State $role);TargetState=(State $other)}
  $rows.Add($row);Start-Sleep -Milliseconds 60
 }
}
foreach($role in 'host','client'){
 $launch[$role]=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
 if((Get-Process -Id $launch[$role].pid).Path -ne $launch[$role].executable){throw 'Unexpected lab process'}
}
try {
 foreach($role in 'host','client'){Command $role 'coop_probe_body on'}
 Start-Sleep -Milliseconds 500
 foreach($role in 'host','client'){
  $other=if($role -eq 'host'){'client'}else{'host'}
  $initial=State $other;$ownInitial=State $role
  if(!$initial.bodyStreamEnabled -or !$initial.probeFollowEnabled -or $initial.poseProtocol -ne 11 -or $initial.posePacketBytes -ne 1164){throw 'Body stream fixture unavailable'}
  Capture $role Standing 5
  Input $role B
  Capture $role CrouchTransition 4
  # Stance changes before the authored crouch animation finishes. Preserve
  # transition samples separately, then compare the stationary native pose.
  Start-Sleep -Milliseconds 500
  Capture $role Crouched 9
  $crouched=State $other
  & "$PSScriptRoot/Send-D2Input.ps1" -Role $role -LeftY 17000 -DurationMs 800|Out-Null
  Capture $role CrouchWalk 9
  Input $role Neutral;Start-Sleep -Milliseconds 400
  Input $role A
  Capture $role CrouchJump 15
  Start-Sleep -Milliseconds 500
  Input $role B;Start-Sleep -Milliseconds 700
  Capture $role ReturnedStanding 5
  Input $role A
  Capture $role StandingJump 15
  Start-Sleep -Milliseconds 600
  $final=State $other;$ownFinal=State $role
  $static=@($rows|Where-Object {$_.Role -eq $role -and $_.Stage -eq 'Crouched' -and $_.Source.stable -and $_.Target.stable})
  if($static.Count -lt 4){throw 'Too few coherent crouch samples'}
  $maxError=0.0
  foreach($r in $static){
   $expected=$r.Target.joints[2].bindLocalQTS[6]+$r.Source.joints[1].globalPose[14]-$r.Source.joints[1].bindLocalQTS[6]
   $actual=($r.Target.phases[0].matrices|Where-Object joint -eq 2).matrix[14]
   $maxError=[Math]::Max($maxError,[Math]::Abs($expected-$actual))
   if($r.SourceState.localPlayerStance -ne 1 -or !$r.TargetState.remoteBodyPoseValid){throw 'Crouch metadata missing'}
  }
  $standing=@($rows|Where-Object {$_.Role -eq $role -and $_.Stage -eq 'Standing' -and $_.Target.stable})
  $returned=@($rows|Where-Object {$_.Role -eq $role -and $_.Stage -eq 'ReturnedStanding' -and $_.Target.stable})
  if(!$standing.Count -or !$returned.Count){throw 'Missing standing controls'}
  $startHip=$standing[0].Target.joints[2].globalPose[14]
  $lowHip=($static|ForEach-Object {$_.Target.joints[2].globalPose[14]}|Measure-Object -Minimum).Minimum
  $endHip=$returned[-1].Target.joints[2].globalPose[14]
  if($maxError -gt .003 -or $startHip-$lowHip -lt .55 -or [Math]::Abs($endHip-$startHip) -gt .04 -or
     $crouched.bodyStreamApplied -le $initial.bodyStreamApplied -or $final.rejected -ne $initial.rejected -or
     $ownFinal.bodyStreamApplied -ne $ownInitial.bodyStreamApplied){throw 'Remote crouch application, recovery or isolation failed'}
  foreach($stage in 'CrouchJump','StandingJump'){
   $jump=@($rows|Where-Object {$_.Role -eq $role -and $_.Stage -eq $stage -and $_.Target.stable})
   if($jump.Count -lt 5){throw 'Too few jump observations'}
   $range=$jump|ForEach-Object {$_.Target.root[2]}|Measure-Object -Minimum -Maximum
   if($range.Maximum-$range.Minimum -lt .45){throw 'Remote counterpart did not follow the jump'}
  }
  $cases.Add([pscustomobject]@{Role=$role;Before=$initial;Crouched=$crouched;After=$final;CoherentCrouchSamples=$static.Count;MaxPrePhysicsHipError=$maxError;HipDrop=$startHip-$lowHip;ReturnHipDifference=$endHip-$startHip;OtherCounterpartApplications=$ownFinal.bodyStreamApplied-$ownInitial.bodyStreamApplied})
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($role in 'host','client'){
  Input $role Neutral
  $s=State $role
  if($s.localPlayerStanceValid -and $s.localPlayerStance -eq 1){Input $role B;Start-Sleep -Milliseconds 500}
 }
 [ordered]@{Passed=($null -eq $failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;ProbeSha256=(Get-FileHash $probe).Hash;Scope='Native Steam body-pose stream, both counterpart rigs, stationary crouch hip mapping, movement/jumps, return to native gait and opposite counterpart isolation. Rendering appearance, combat and physics hitbox agreement are separate checks.';Cases=$cases.ToArray();Samples=$rows.ToArray()}|ConvertTo-Json -Depth 24|Set-Content "$root/work/body-stream-observations.json"
}
if($failure){throw $failure}
$cases|Select-Object Role,CoherentCrouchSamples,MaxPrePhysicsHipError,HipDrop,ReturnHipDifference,OtherCounterpartApplications
