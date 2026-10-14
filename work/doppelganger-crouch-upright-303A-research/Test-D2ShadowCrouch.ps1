[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$probe="$PSScriptRoot/bin/d2_runtime_probe.exe"
$pids=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[object]]::new();$references=@{};$failure=$null
function Cmd($r,$s){& "$PSScriptRoot/Send-D2Command.ps1" -Role $r -Command $s -WaitConsumed|Out-Null}
function Input($r,$b){& "$PSScriptRoot/Send-D2Input.ps1" -Role $r -Button $b -DurationMs 150|Out-Null}
function State($r){$f=Get-Item "$root/work/lab-instances/$r/coop-status.json";if(([DateTime]::UtcNow-$f.LastWriteTimeUtc).TotalSeconds-gt1.5){throw 'Stale bridge'};Get-Content $f.FullName -Raw|ConvertFrom-Json}
function Native($r,$flag,$h='',$publication=''){$args=@($pids[$r],$flag);if($h){$args+=$h};if($publication){$args+=$publication};for($i=0;$i-lt20;$i++){$v=& $probe @args;if(!$LASTEXITCODE){return($v|ConvertFrom-Json)};Start-Sleep -Milliseconds 10};throw "Native read failed: $r $flag handle=$h exit=$LASTEXITCODE"}
function Distance($a,$b){$sum=0.;for($i=0;$i-lt$a.Count;$i++){$sum+=[Math]::Pow($a[$i]-$b[$i],2)};[Math]::Sqrt($sum)}
function Capture($source,$stage,$count){
 $target=if($source-eq'host'){'client'}else{'host'}
 for($n=0;$n-lt$count;$n++){
  $b=State $target;$a=State $source
  if(!$b.probeFollowEnabled-or!$b.remoteFresh-or$b.localGameLogicState-ne1-or!$b.poseSmoothingEnabled){throw 'Crouch scope lost'}
  $src=Native $source '--skeleton' 1
  $dst=if($stage-eq'Standing'){Native $target '--skeleton' $b.probeActorHandle}else{Native $target '--skeleton-final' $b.probeActorHandle $b.bodyPublicationAddress}
  $w=Native $target '--avatar-sword-attachment' $b.probeActorHandle
  $after=State $target;if($after.localEpoch-ne$b.localEpoch-or$after.probeActorHandle-ne$b.probeActorHandle){throw 'Actor changed during capture'}
  $expected=@(0.,0.,0.);for($i=0;$i-lt3;$i++){$expected[$i]=$w.parentNativeRoot[$i];for($j=0;$j-lt3;$j++){$expected[$i]+=$w.parentNativeRoot[3+$j*3+$i]*$w.jointMatrix[12+$j]}}
  $hip=@($dst.phases[3].matrices|Where-Object joint -eq 2)[0].matrix[14]
  $directions=@(foreach($side in 'left','right'){
   $sf=@($src.joints|Where-Object nativeName -eq "${side}_femur_jnt")[0];$st=@($src.joints|Where-Object nativeName -eq "${side}_tibia_jnt")[0]
   $df=@($dst.joints|Where-Object nativeName -eq "${side}_femur_jnt")[0];$dt=@($dst.joints|Where-Object nativeName -eq "${side}_tibia_jnt")[0]
   $fm=@($dst.phases[3].matrices|Where-Object joint -eq $df.index)[0].matrix;$tm=@($dst.phases[3].matrices|Where-Object joint -eq $dt.index)[0].matrix
   $dot=0.;$an=0.;$bn=0.;for($i=12;$i-lt15;$i++){$av=$st.globalPose[$i]-$sf.globalPose[$i];$bv=$tm[$i]-$fm[$i];$dot+=$av*$bv;$an+=$av*$av;$bn+=$bv*$bv};$dot/[Math]::Sqrt($an*$bn)
  })
  $wanted=$dst.joints[2].bindLocalQTS[6]+$src.joints[1].globalPose[14]-$src.joints[1].bindLocalQTS[6]
  $row=[pscustomobject]@{Source=$source;Stage=$stage;At=[DateTimeOffset]::Now.ToString('o');SourceState=$a;TargetState=$b;Stable=($src.stable-and$dst.stable-and$w.stable);FinalPublicationComplete=$dst.finalPublicationComplete;PublicationBefore=$dst.publicationBefore;PublicationAfter=$dst.publicationAfter;SourceModel=$src.model;TargetModel=$dst.model;Hip=$hip;ExpectedHip=$wanted;HipError=[Math]::Abs($hip-$wanted);LegDirectionAgreement=($directions|Measure-Object -Minimum).Minimum;Root=$dst.root;AttachmentError=(Distance $expected $w.weaponNativeRoot[0..2]);WeaponRenderError=(Distance $w.weaponRenderRoot $w.weaponNativeRoot);Joints=@($dst.joints|Where-Object {$_.index-lt27-or$_.nativeName-match'toes_end|helper_(upper_leg|upper_knee|kneeangle|hips|buttock)'}|Select-Object index,nativeName,parent,localPose,globalPose,bindLocalQTS)}
  $rows.Add($row);Start-Sleep -Milliseconds 35
 }
}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne$l.executable){throw 'Wrong game process'};$pids[$r]=$l.pid
  foreach($cmd in 'coop_checkpoint_sync off','coop_shadow_stream off','coop_shadow_action off','coop_probe_body off','coop_probe_follow exact','coop_probe_locomotion on','coop_probe_smoothing on'){Cmd $r $cmd}
  if((State $r).localPlayerStance){Input $r B;Start-Sleep -Milliseconds 650}
 }
 foreach($source in 'host','client'){
  $target=if($source-eq'host'){'client'}else{'host'}
  Cmd $source 'setviewpos -33 -153 68.1 90 0';Cmd $target 'setviewpos -28.0921 -153.962 68.1 180 0';Start-Sleep -Seconds 2
  $before=State $target;$observer=$before.localPosition;$ownBefore=State $source
  $health=Native $target '--health' 1
  Cmd $target 'coop_shadow_crouch off';Cmd $target 'coop_shadow_action crouchpose';Start-Sleep -Milliseconds 700
  for($attempt=0;$attempt-lt18;$attempt++){Cmd $target 'coop_shadow_crouch capture';Start-Sleep -Milliseconds 90;if((State $target).nativeCrouchReferenceReady){break}}
  if(!(State $target).nativeCrouchReferenceReady){throw 'Complete native crouch reference did not capture'}
  $references[$source]=Native $target '--skeleton' (State $target).probeActorHandle
  Cmd $target 'coop_shadow_action off';Start-Sleep -Milliseconds 800
  Cmd $target 'coop_probe_body on';Capture $source Standing 16
  Input $source B;Capture $source Enter 8;Start-Sleep -Milliseconds 600
  if((State $source).localPlayerStance-ne1){throw 'Native crouch input did not engage'}
  Capture $source Crouched 24
  & "$PSScriptRoot/Send-D2Input.ps1" -Role $source -LeftY 12000 -DurationMs 3800|Out-Null
  Capture $source Walking 36;Input $source Neutral;Start-Sleep -Milliseconds 600
  Input $source B;Capture $source Exit 12;Start-Sleep -Milliseconds 700;Capture $source Returned 24
  $final=State $target;$ownFinal=State $source
  $static=@($rows|Where-Object {$_.Source-eq$source-and$_.Stage-eq'Crouched'-and$_.Stable})
  $stand=@($rows|Where-Object {$_.Source-eq$source-and$_.Stage-eq'Standing'-and$_.Stable})
  $returned=@($rows|Where-Object {$_.Source-eq$source-and$_.Stage-eq'Returned'-and$_.Stable})
  $walking=@($rows|Where-Object {$_.Source-eq$source-and$_.Stage-eq'Walking'-and$_.Stable})
  $errors=[Collections.Generic.List[string]]::new()
  if($static.Count-lt6-or$stand.Count-lt2-or$returned.Count-lt3-or$walking.Count-lt4){$errors.Add('Insufficient coherent observations')}
  $hipDrop=($stand.Hip|Measure-Object -Average).Average-($static.Hip|Measure-Object -Average).Average
  $returnError=[Math]::Abs(($stand.Hip|Measure-Object -Average).Average-($returned.Hip|Measure-Object -Average).Average)
  $maxHip=($static.HipError|Measure-Object -Maximum).Maximum
  if($hipDrop-lt.45-or$returnError-gt.08-or$maxHip-gt.03){$errors.Add('Crouch height mapping or return failed')}
  if(@($static|Where-Object LegDirectionAgreement -lt .90).Count){$errors.Add('Rendered knees do not follow the native crouch pose')}
  $stable=@($rows|Where-Object {$_.Source-eq$source-and$_.Stable})
  if(@($stable|Where-Object {$_.AttachmentError-gt.03-or$_.WeaponRenderError-gt.03}).Count){$errors.Add('Native sword attachment separated')}
  $travel=if($walking.Count-ge2){Distance $walking[0].Root[0..2] $walking[-1].Root[0..2]}else{0}
  $legDelta=0.;foreach($row in $walking){foreach($name in 'left_femur_jnt','left_tibia_jnt','right_femur_jnt','right_tibia_jnt'){$j=@($row.Joints|Where-Object nativeName -eq $name)[0];$initial=@($walking[0].Joints|Where-Object nativeName -eq $name)[0];foreach($k in 0,1,2,4,5,6,8,9,10){$legDelta=[Math]::Max($legDelta,[Math]::Abs($j.localPose[$k]-$initial.localPose[$k]))}}}
  if($travel-lt.25-or$legDelta-lt.1){$errors.Add('No crouch-walk root and leg movement')}
  $spineDelta=0.;$armDelta=0.
  if($stand.Count){foreach($row in $static){foreach($name in 'center_spine01_jnt','center_spine02_jnt','right_arm_jnt','right_elbow_jnt'){
   $j=@($row.Joints|Where-Object nativeName -eq $name)[0];$base=@($stand[0].Joints|Where-Object nativeName -eq $name)[0]
   foreach($k in 0,1,2,4,5,6,8,9,10){$d=[Math]::Abs($j.localPose[$k]-$base.localPose[$k]);if($name-like'center*'){$spineDelta=[Math]::Max($spineDelta,$d)}else{$armDelta=[Math]::Max($armDelta,$d)}}
  }}}
  if($spineDelta-lt.1-or$armDelta-lt.1){$errors.Add('Crouched torso and arm posture absent')}
  if((Distance $observer $final.localPosition)-gt.02-or(Native $target '--health' 1).current-ne$health.current-or$ownFinal.bodyStreamApplied-ne$ownBefore.bodyStreamApplied){$errors.Add('Local observer or opposite clone changed')}
  if($final.bodyStreamApplied-le$before.bodyStreamApplied){$errors.Add('No full-body crouch application')}
  $checks.Add([pscustomobject]@{Role=$source;Passed=(!$errors.Count);Failures=$errors;CoherentCrouch=$static.Count;CoherentWalking=$walking.Count;HipDrop=$hipDrop;MaxHipError=$maxHip;ReturnError=$returnError;Travel=$travel;LegRotationDelta=$legDelta;SpineRotationDelta=$spineDelta;ArmRotationDelta=$armDelta;MaxAttachmentError=($stable.AttachmentError|Measure-Object -Maximum).Maximum;Applications=$final.bodyStreamApplied-$before.bodyStreamApplied})
  Cmd $target 'coop_probe_body off'
 }
 if(@($checks|Where-Object {!$_.Passed}).Count){throw 'Crouch assertions failed'}
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{Input $r Neutral;Cmd $r 'coop_probe_body off';if((State $r).localPlayerStance){Input $r B}}catch{}}
}
[ordered]@{At=[DateTimeOffset]::Now.ToString('o');Passed=(!$failure);Failure=$failure;Dll=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;Scope='Whole-body research blend: captured native crouched torso/head/arms/hands, player pelvis and stepping, all-joint stance blend after IK and before final cache. Both roles over existing native Steam body channel. Requires visual review; no crouched combat/capsule/general traversal claim.';References=$references;Checks=$checks;Samples=$rows}|ConvertTo-Json -Depth 19|Set-Content "$PSScriptRoot/research/doppelganger-crouch-verification.json"
$checks|Format-Table -AutoSize
if($failure){throw $failure}
