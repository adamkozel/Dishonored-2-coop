[CmdletBinding()]
param()
$ErrorActionPreference='Stop';$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$probe="$PSScriptRoot/bin/d2_runtime_probe.exe";$pids=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[object]]::new();$failure=$null
function Cmd($r,$s){& "$PSScriptRoot/Send-D2Command.ps1" -Role $r -Command $s -WaitConsumed|Out-Null}
function Input($r,$b){& "$PSScriptRoot/Send-D2Input.ps1" -Role $r -Button $b -DurationMs 150|Out-Null}
function State($r){$f=Get-Item "$root/work/lab-instances/$r/coop-status.json";if(([DateTime]::UtcNow-$f.LastWriteTimeUtc).TotalSeconds-gt2){throw 'Stale bridge'};Get-Content $f.FullName -Raw|ConvertFrom-Json}
function Distance($a,$b){$v=0.;for($i=0;$i-lt3;$i++){$v+=[Math]::Pow($a[$i]-$b[$i],2)};[Math]::Sqrt($v)}
$directions=@(@{Name='forward';X=0;Y=12000},@{Name='backward';X=0;Y=-12000},@{Name='left';X=-12000;Y=0},@{Name='right';X=12000;Y=0},@{Name='forward-left';X=-8500;Y=8500},@{Name='backward-right';X=8500;Y=-8500})
try{
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne$l.executable-or!$l.assetExperiment){throw 'Wrong process'};$pids[$r]=$l.pid;Input $r Neutral;if((State $r).localPlayerStance){Input $r B}}
 & "$PSScriptRoot/Enable-D2ShadowCrouch.ps1"|Out-Null
 foreach($source in 'host','client'){
  $target=if($source-eq'host'){'client'}else{'host'};Cmd $source 'coop_probe_body off';Cmd $target 'coop_probe_body on'
  foreach($d in $directions){
   Cmd $source 'setviewpos -33 -153 68.1 90 0';Cmd $target 'setviewpos -28.0921 -153.962 68.1 169 10'
   if(!(State $source).localPlayerStance){Input $source B};Start-Sleep -Milliseconds 900
   $before=State $target;$origin=(State $source).localPosition;$observer=$before.localPosition;$case=[Collections.Generic.List[object]]::new()
   & "$PSScriptRoot/Send-D2Input.ps1" -Role $source -LeftX $d.X -LeftY $d.Y -DurationMs 2400|Out-Null
   $watch=[Diagnostics.Stopwatch]::StartNew()
   while($watch.ElapsedMilliseconds-lt2600){
    $s=State $target
    if(!$s.remoteFresh-or!$s.probeFollowEnabled-or!$s.bodyStreamEnabled-or$s.localEpoch-ne$before.localEpoch-or$s.probeActorHandle-ne$before.probeActorHandle){throw 'Directional test scope changed'}
    $raw=& $probe $pids[$target] --skeleton-final $s.probeActorHandle $s.bodyPublicationAddress
    if(!$LASTEXITCODE){$sk=$raw|ConvertFrom-Json;if($sk.stable-and$sk.finalPublicationComplete){
     $knees=@($sk.joints[20].globalPose[14],$sk.joints[24].globalPose[14]);$toes=@($sk.joints[22].globalPose[14],$sk.joints[26].globalPose[14])
     $row=[pscustomobject]@{Source=$source;Direction=$d.Name;At=[DateTimeOffset]::Now.ToString('o');ElapsedMs=$watch.ElapsedMilliseconds;Root=$sk.root;MinimumKnee=($knees|Measure-Object -Minimum).Minimum;MinimumToe=($toes|Measure-Object -Minimum).Minimum;Joints=@($sk.joints|Where-Object index -in 19,20,23,24|Select-Object index,nativeName,localPose,globalPose)}
     $case.Add($row);$rows.Add($row)
    }}
    Start-Sleep -Milliseconds 45
   }
   Input $source Neutral;Start-Sleep -Milliseconds 350;$after=State $target
   $travel=Distance $origin (State $source).localPosition
   $minKnee=($case.MinimumKnee|Measure-Object -Minimum).Minimum;$minToe=($case.MinimumToe|Measure-Object -Minimum).Minimum
   $angle=0.;foreach($joint in 19,20,23,24){foreach($a in $case){$av=($a.Joints|Where-Object index -eq $joint).localPose;foreach($b in $case){$bv=($b.Joints|Where-Object index -eq $joint).localPose;$tr=0.;foreach($k in 0,1,2,4,5,6,8,9,10){$tr+=$av[$k]*$bv[$k]};$angle=[Math]::Max($angle,[Math]::Acos([Math]::Clamp(($tr-1.0)*.5,-1.0,1.0)))}}}
   $passed=$case.Count-ge5-and$travel-gt.3-and$minKnee-ge0-and$minToe-ge0-and$angle-gt.04-and(Distance $observer $after.localPosition)-lt.02-and$after.bodyStreamApplied-gt$before.bodyStreamApplied
   $checks.Add([pscustomobject]@{Role=$source;Direction=$d.Name;Passed=$passed;Coherent=$case.Count;Travel=$travel;MinimumKnee=$minKnee;MinimumToe=$minToe;MaximumLegAngle=$angle;Applications=$after.bodyStreamApplied-$before.bodyStreamApplied})
  }
  Input $source B;Cmd $target 'coop_probe_body off'
 }
 if(@($checks|Where-Object {!$_.Passed}).Count){throw 'Directional crouch checks failed'}
}catch{$failure=$_.Exception.Message}finally{foreach($r in 'host','client'){try{Input $r Neutral;Cmd $r 'coop_probe_body off';if((State $r).localPlayerStance){Input $r B}}catch{}}}
[ordered]@{At=[DateTimeOffset]::Now.ToString('o');Dll=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;Passed=(!$failure);Failure=$failure;Scope='Six bounded flat-floor directions per character. Coherent final skeletons, nonpenetrating knees/toes, angular gait excursion, root travel and observer isolation. Not uneven terrain, foot planting or visual acceptance.';Checks=$checks;Samples=$rows}|ConvertTo-Json -Depth 16|Set-Content "$PSScriptRoot/research/doppelganger-crouch-directions.json"
$checks|Format-Table -AutoSize
if($failure){throw $failure}
