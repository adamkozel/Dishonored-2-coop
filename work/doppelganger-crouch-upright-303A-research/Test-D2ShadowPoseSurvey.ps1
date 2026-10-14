[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$probe="$PSScriptRoot/bin/d2_runtime_probe.exe";$pids=@{};$rows=[Collections.Generic.List[object]]::new();$failure=$null
function Cmd($r,$s){& "$PSScriptRoot/Send-D2Command.ps1" -Role $r -Command $s -WaitConsumed|Out-Null}
function State($r){$f=Get-Item "$root/work/lab-instances/$r/coop-status.json";if(([DateTime]::UtcNow-$f.LastWriteTimeUtc).TotalSeconds-gt2){throw 'Stale bridge'};Get-Content $f.FullName -Raw|ConvertFrom-Json}
function Read($r,$flag,$h){for($i=0;$i-lt20;$i++){$raw=& $probe $pids[$r] $flag $h;if(!$LASTEXITCODE){$v=$raw|ConvertFrom-Json;if($flag-ne'--skeleton'-or$v.stable){return $v}};Start-Sleep -Milliseconds 20};throw "No coherent native observation: $r $flag"}
function Distance($a,$b){$n=0.;for($i=0;$i-lt$a.Count;$i++){$n+=[Math]::Pow($a[$i]-$b[$i],2)};[Math]::Sqrt($n)}
$cases=@(
 @{Pose='idle';Ms=0;Index=8},
 @{Pose='dodgeleft';Ms=100;Index=12},@{Pose='dodgeleft';Ms=200;Index=12},@{Pose='dodgeleft';Ms=300;Index=12},
 @{Pose='dodgeright';Ms=100;Index=13},@{Pose='dodgeright';Ms=200;Index=13},@{Pose='dodgeright';Ms=300;Index=13},
 @{Pose='crouchpose';Ms=400;Index=11},@{Pose='crouchpose';Ms=600;Index=11},@{Pose='crouchpose';Ms=800;Index=11},@{Pose='crouchpose';Ms=1200;Index=11}
)
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if((Get-Process -Id $l.pid).Path-ne$l.executable-or!$l.assetExperiment){throw 'Not an isolated asset experiment'};$pids[$r]=$l.pid
  foreach($c in 'coop_shadow_stream off','coop_shadow_action off','coop_probe_body off','coop_probe_follow exact','coop_probe_locomotion on'){Cmd $r $c}
  if((State $r).localPlayerStance){throw 'Survey requires standing local players'}
 }
 Cmd host 'setviewpos -33 -153 68.1 90 0';Cmd client 'setviewpos -28.0921 -153.962 68.1 169 10';Start-Sleep -Milliseconds 1200
 foreach($r in 'host','client'){
  $start=State $r;$health=Read $r '--health' 1;$cloneHealth=Read $r '--health' $start.probeActorHandle
  foreach($case in $cases){
   Cmd $r "coop_shadow_pose $($case.Pose) $($case.Ms)";Start-Sleep -Milliseconds 700
   $state=State $r
   if(!$state.remoteFresh-or!$state.probeFollowEnabled-or$state.bodyStreamEnabled-or$state.localEpoch-ne$start.localEpoch-or$state.probeActorHandle-ne$start.probeActorHandle){throw 'Pose sample scope changed'}
   $sk=Read $r '--skeleton' $state.probeActorHandle;$anim=Read $r '--animation' $state.probeActorHandle;$weapon=Read $r '--avatar-sword-attachment' $state.probeActorHandle
   $input=@($anim.declaredInputs|Where-Object name -eq 'animBasicInputData1')[0]
   if(($input.rawWords[0]-band255)-ne$case.Index){throw "Wrong native clip: $r $($case.Pose)"}
   if((Distance $sk.root[0..2] $state.probeTargetPosition)-gt.03){throw 'Native pose moved the network root'}
   $rows.Add([pscustomobject]@{Role=$r;Pose=$case.Pose;Milliseconds=$case.Ms;State=$state;Skeleton=$sk;Input=$input;Weapon=$weapon})
   Cmd $r 'coop_shadow_action off';Start-Sleep -Milliseconds 250
  }
  if((Read $r '--health' 1).current-ne$health.current-or(Read $r '--health' $start.probeActorHandle).current-ne$cloneHealth.current){throw 'Health changed during native pose survey'}
 }
}catch{$failure=$_.Exception.Message}finally{foreach($r in 'host','client'){try{Cmd $r 'coop_shadow_action off';Cmd $r 'coop_probe_body off'}catch{}}}
[ordered]@{At=[DateTimeOffset]::Now.ToString('o');Dll=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;Passed=(!$failure);Failure=$failure;Scope='Native fixed-time full-skeleton references only, rate zero, ten-second leases, network root retained. No retargeted pose writes. Passing survey is not crouch acceptance.';Samples=$rows}|ConvertTo-Json -Depth 20|Set-Content "$PSScriptRoot/research/doppelganger-native-pose-survey.json"
$rows|Select-Object Role,Pose,Milliseconds,@{n='Hip';e={$_.Skeleton.joints[2].globalPose[14]}},@{n='Head';e={$_.Skeleton.joints[12].globalPose[14]}},@{n='SwordHand';e={$_.Skeleton.joints[17].globalPose[14]}}
if($failure){throw $failure}
