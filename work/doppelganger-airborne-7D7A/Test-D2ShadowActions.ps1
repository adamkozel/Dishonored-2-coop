[CmdletBinding()]
param([ValidateSet('both','host','client')][string]$Target='both')
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$probe=Join-Path $PSScriptRoot 'bin/d2_runtime_probe.exe'
$rows=[Collections.Generic.List[object]]::new();$results=[Collections.Generic.List[object]]::new();$failure=$null
function Cmd($role,$command){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $command -WaitConsumed|Out-Null}
function State($role){
 $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
 if((Get-Process -Id $l.pid).Path-ne$l.executable){throw 'Unexpected process'}
 $f=Get-Item "$root/work/lab-instances/$role/coop-status.json"
 if(([DateTime]::UtcNow-$f.LastWriteTimeUtc).TotalSeconds-gt1.5){throw 'Stale status'}
 $s=Get-Content $f.FullName -Raw|ConvertFrom-Json
 if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState-or!$s.remoteFresh-or$s.connectionState-ne3-or!$s.probeFollowEnabled-or!$s.locomotionEnabled-or$s.localMap-ne'campaign/dunwall/escape/tower/dunwall_escape_tower_p'){throw 'Connected owned Dunwall clones required'}
 [pscustomobject]@{Launch=$l;Status=$s}
}
function Native($pidValue,$option,$handle){for($attempt=0;$attempt-lt8;$attempt++){$r=& $probe $pidValue $option $handle;if(!$LASTEXITCODE){return($r|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw "Probe unavailable $option code $LASTEXITCODE"}
function Distance($a,$b){$v=0.;for($i=0;$i-lt$a.Count;$i++){$v+=[Math]::Pow($a[$i]-$b[$i],2)};[Math]::Sqrt($v)}
function Sample($role,$action){
 $st=State $role;$p=$st.Launch.pid;$h=$st.Status.probeActorHandle
 $a=Native $p '--animation' $h;$s=Native $p '--skeleton' $h;$w=Native $p '--avatar-sword-attachment' $h
 $end=(State $role).Status
 if($st.Status.localEpoch-ne$end.localEpoch-or$h-ne$end.probeActorHandle){throw 'Actor changed during sample'}
 $model=if($role-eq'host'){'models/characters/d2coop/shadow_emily.md6'}else{'models/characters/d2coop/shadow_corvo.md6'}
 if($s.model-ne$model-or$a.locomotionNodes[0].controller-eq'0x0'-or$a.modelConfig.'148'.entries.Count-ne11){throw 'Wrong clone model or missing animation controller'}
 $expected=@(0.,0.,0.)
 for($i=0;$i-lt3;$i++){$expected[$i]=$w.parentNativeRoot[$i];for($j=0;$j-lt3;$j++){$expected[$i]+=$w.parentNativeRoot[3+$j*3+$i]*$w.jointMatrix[12+$j]}}
 $r=[pscustomobject]@{Role=$role;Action=$action;At=[DateTimeOffset]::Now.ToString('o');Handle=$h;Epoch=$end.localEpoch;
  SkeletonStable=$s.stable;Model=$s.model;Root=$s.root;Joints=@($s.joints|Where-Object {$_.nativeName-match'^(right_(arm|elbow|wrist|hand|thumb|index|middle|ring|pinky)|center_spine)' }|Select-Object index,nativeName,localPose,globalPose);
  Input=@($a.declaredInputs|Where-Object name -eq 'animBasicInputData1')[0];Attachment=$w;
  AttachmentPositionError=(Distance $expected $w.weaponNativeRoot[0..2]);WeaponRenderError=(Distance $w.weaponRenderRoot $w.weaponNativeRoot);
  RootFollowError=(Distance $s.root[0..2] $end.probeRenderPosition);LocalPosition=$end.localPosition}
 $rows.Add($r);$r
}
try{
 $targets=if($Target-eq'both'){@('host','client')}else{@($Target)}
 foreach($role in $targets){
  $init=State $role;$playerBefore=Native $init.Launch.pid '--health' 1
  Cmd $role 'coop_shadow_action off';Cmd $role 'coop_shadow_loco combat';Start-Sleep -Milliseconds 1800
  foreach($action in 'attack','parry','recover','idle'){
   Cmd $role 'coop_shadow_action off';Cmd $role 'coop_shadow_loco combat';Start-Sleep -Milliseconds 900
   $idle=Sample $role 'baseline'
   & "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command "coop_shadow_action $action"|Out-Null
   $pulse=@();for($poll=0;$poll-lt12;$poll++){$quick=Native $init.Launch.pid "--animation" $init.Status.probeActorHandle;$input=@($quick.declaredInputs|Where-Object name -eq "animBasicInputData1")[0];$pulse+=$input;if(($input.rawWords[0]-band255)-ne255){break};Start-Sleep -Milliseconds 10}
   for($j=0;$j-lt24;$j++){Sample $role $action|Out-Null;Start-Sleep -Milliseconds 55}
   Cmd $role 'coop_shadow_action off';Start-Sleep -Milliseconds 350
   $settled=Sample $role 'stopped'
   $samples=@($rows|Where-Object {$_.Role-eq$role-and$_.Action-eq$action})
   $stable=@($samples|Where-Object {$_.SkeletonStable-and$_.Attachment.stable})
   $index=@{attack=5;parry=6;recover=7;idle=8}[$action]
   $seen=@($samples|Where-Object {($_.Input.rawWords[0]-band255)-eq$index}).Count+@($pulse|Where-Object {($_.rawWords[0]-band255)-eq$index}).Count
   $maxPose=0.;foreach($r in $stable){foreach($joint in $r.Joints){if($joint.nativeName-notmatch'^right_(arm|elbow|wrist)_jnt$'){continue};$before=@($idle.Joints|Where-Object index -eq $joint.index)[0];foreach($k in 0,1,2,4,5,6,8,9,10){$maxPose=[Math]::Max($maxPose,[Math]::Abs($joint.localPose[$k]-$before.localPose[$k]))}}}
   $errors=[Collections.Generic.List[string]]::new()
   if($seen-lt1){$errors.Add('No native command index observed')}
   if($stable.Count-lt6){$errors.Add('Insufficient stable pose and attachment observations')}
   if($action-ne'idle'-and$maxPose-lt0.1){$errors.Add('No clear native right-arm pose response')}
   if(@($stable|Where-Object AttachmentPositionError -gt 0.03).Count){$errors.Add('Native attachment origin separates from hand joint')}
   if(@($stable|Where-Object WeaponRenderError -gt 0.03).Count){$errors.Add('Weapon render and native roots separate')}
   if(@($samples|Where-Object RootFollowError -gt 0.03).Count){$errors.Add('Action displaces the network-controlled root')}
   if(($settled.Input.rawWords[0]-band255)-ne255){$errors.Add('Stop did not clear the native channel')}
   $results.Add([pscustomobject]@{Role=$role;Action=$action;Passed=($errors.Count-eq0);Failures=$errors;ObservedNativeIndex=$seen;ImmediateInputs=$pulse;StableSamples=$stable.Count;MaxArmRotationDelta=$maxPose;MaxAttachmentPositionError=($stable.AttachmentPositionError|Measure-Object -Maximum).Maximum;MaxWeaponRenderError=($stable.WeaponRenderError|Measure-Object -Maximum).Maximum;MaxRootFollowError=($samples.RootFollowError|Measure-Object -Maximum).Maximum})
  }
  $playerAfter=Native $init.Launch.pid '--health' 1
  if($playerAfter.current-ne$playerBefore.current){throw 'Local player health changed'}
 }
 if(@($results|Where-Object {!$_.Passed}).Count){throw 'One or more clone action assertions failed'}
}catch{$failure=$_.Exception.Message}finally{
 foreach($role in 'host','client'){try{Cmd $role 'coop_shadow_action off';Cmd $role 'coop_shadow_loco off'}catch{}}
}
[ordered]@{Passed=(!$failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;
 Scope='Bounded native AnimBasic commands for isolated owned clone models. Tests native right-arm motion, attachment origin, root ownership, native stop and unchanged player health. Not a network action stream, full combat correctness, finger grip judgement, or real animation-event damage test.';
 Results=$results.ToArray();Samples=$rows.ToArray()}|ConvertTo-Json -Depth 15|Set-Content "$PSScriptRoot/research/doppelganger-action-verification.json"
$results|Format-Table -AutoSize
if($failure){throw $failure}
