[CmdletBinding()]
param([switch]$Pose)
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$probe="$PSScriptRoot/bin/d2_runtime_probe.exe";$pids=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[object]]::new();$failure=$null
function Cmd($r,$s){& "$PSScriptRoot/Send-D2Command.ps1" -Role $r -Command $s -WaitConsumed|Out-Null}
function State($r){Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json}
function Native($r,$flag,$h){for($i=0;$i-lt8;$i++){$v=& $probe $pids[$r] $flag $h;if(!$LASTEXITCODE){return($v|ConvertFrom-Json)};Start-Sleep -Milliseconds 10};throw "No native observation $flag"}
function Distance($a,$b){$s=0.;for($i=0;$i-lt$a.Count;$i++){$s+=[Math]::Pow($a[$i]-$b[$i],2)};[Math]::Sqrt($s)}
function Capture($r,$stage,$time){
 $s=State $r;if(!$s.remoteFresh-or!$s.probeFollowEnabled-or$s.localGameLogicState-ne1){throw 'Reference scope lost'}
 $sk=Native $r '--skeleton' $s.probeActorHandle;$a=Native $r '--animation' $s.probeActorHandle;$w=Native $r '--avatar-sword-attachment' $s.probeActorHandle
 $expected=@(0.,0.,0.);for($i=0;$i-lt3;$i++){$expected[$i]=$w.parentNativeRoot[$i];for($j=0;$j-lt3;$j++){$expected[$i]+=$w.parentNativeRoot[3+$j*3+$i]*$w.jointMatrix[12+$j]}}
 $rout=[pscustomobject]@{Role=$r;Stage=$stage;ElapsedMs=$time;Stable=($sk.stable-and$w.stable);Input=@($a.declaredInputs|Where-Object name -eq 'animBasicInputData1')[0];Root=$sk.root;Hip=$sk.joints[2].globalPose[14];Joints=@($sk.joints|Where-Object {$_.index-lt27}|Select-Object index,nativeName,localPose,globalPose);AttachmentError=(Distance $expected $w.weaponNativeRoot[0..2]);RootError=(Distance $sk.root[0..2] $s.probeTargetPosition)}
 $rows.Add($rout);$rout
}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne$l.executable){throw 'Unexpected process'};$pids[$r]=$l.pid
  foreach($c in 'coop_shadow_stream off','coop_probe_body off','coop_shadow_action off','coop_probe_follow exact','coop_probe_locomotion on'){Cmd $r $c}
 }
 Cmd host 'setviewpos -33 -153 68.1 90 0';Cmd client 'setviewpos -28.0921 -153.962 68.1 169 10';Start-Sleep -Seconds 2
 foreach($r in 'host','client'){
  $health=Native $r '--health' 1;$before=Capture $r Baseline 0
  Cmd $r $(if($Pose){'coop_shadow_action crouchpose'}else{'coop_shadow_action crouchref'});$clock=[Diagnostics.Stopwatch]::StartNew()
  while($clock.ElapsedMilliseconds-lt11000){Capture $r NativeReference $clock.ElapsedMilliseconds|Out-Null;Start-Sleep -Milliseconds 45}
  Cmd $r 'coop_shadow_action off';Start-Sleep -Milliseconds 600;Capture $r Returned 12000|Out-Null
  $stable=@($rows|Where-Object {$_.Role-eq$r-and$_.Stage-eq'NativeReference'-and$_.Stable})
  $seen=@($stable|Where-Object {($_.Input.rawWords[0]-band255)-eq11})
  $minimum=$stable|Sort-Object Hip|Select-Object -First 1
  $spine=0.;$arm=0.;foreach($row in $stable){foreach($name in 'center_spine01_jnt','center_spine02_jnt','right_arm_jnt','right_elbow_jnt'){$j=@($row.Joints|Where-Object nativeName -eq $name)[0];$b=@($before.Joints|Where-Object nativeName -eq $name)[0];foreach($k in 0,1,2,4,5,6,8,9,10){$delta=[Math]::Abs($j.localPose[$k]-$b.localPose[$k]);if($name-like'center*'){$spine=[Math]::Max($spine,$delta)}else{$arm=[Math]::Max($arm,$delta)}}}}
  $passed=$seen.Count-ge3-and$before.Hip-$minimum.Hip-gt.20-and$spine-gt.1-and$arm-gt.1-and@($stable|Where-Object {$_.AttachmentError-gt.03-or$_.RootError-gt.03}).Count-eq0-and(Native $r '--health' 1).current-eq$health.current
  $checks.Add([pscustomobject]@{Role=$r;Passed=$passed;Stable=$stable.Count;NativeIndexSamples=$seen.Count;HipDrop=$before.Hip-$minimum.Hip;MinimumHipMs=$minimum.ElapsedMs;SpineRotation=$spine;ArmRotation=$arm;MaxAttachmentError=($stable.AttachmentError|Measure-Object -Maximum).Maximum;MaxRootError=($stable.RootError|Measure-Object -Maximum).Maximum})
 }
 if(@($checks|Where-Object {!$_.Passed}).Count){throw 'Native full-body reference assertions failed'}
}catch{$failure=$_.Exception.Message}finally{foreach($r in 'host','client'){try{Cmd $r 'coop_shadow_action off';Cmd $r 'coop_probe_body off'}catch{}}}
$output=if($Pose){'doppelganger-crouch-native-pose.json'}else{'doppelganger-crouch-native-reference.json'}
[ordered]@{At=[DateTimeOffset]::Now.ToString('o');Passed=(!$failure);Failure=$failure;PoseReference=[bool]$Pose;Dll=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;Scope='Native under-table search clip reference, full clone rig, body-pose transfer disabled. Optional pose trial requests rate0 and startTime1.2 float seconds through native AnimBasic. Measures torso/arm/hip response, attachment, root ownership and unchanged health. Not a complete crouch locomotion or combat implementation.';Checks=$checks;Samples=$rows}|ConvertTo-Json -Depth 15|Set-Content "$PSScriptRoot/research/$output"
$checks|Format-Table -AutoSize
if($failure){throw $failure}
