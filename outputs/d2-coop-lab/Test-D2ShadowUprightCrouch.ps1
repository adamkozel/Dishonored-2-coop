[CmdletBinding()]
param([ValidateSet('both','host','client')][string]$SourceRole='both')
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$probe="$PSScriptRoot/bin/d2_runtime_probe.exe"
$pids=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[object]]::new();$references=@{};$sourceReferences=@{};$failure=$null
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
  if(($stage-in'Standing','Returned')-and$a.localPlayerStance-ne0){throw "Uncommanded source stance change during $stage invalidated the fixture"}
  if(($stage-in'Crouched','Walking')-and$a.localPlayerStance-ne1){throw "Uncommanded source stance change during $stage invalidated the fixture"}
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
  $wanted=$references[$source].joints[2].globalPose[14]+$src.joints[1].globalPose[14]-$sourceReferences[$source].joints[1].globalPose[14]
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
 foreach($source in @('host','client')|Where-Object {$SourceRole-eq'both'-or$_-eq$SourceRole}){
  $target=if($source-eq'host'){'client'}else{'host'}
  Cmd $source 'setviewpos -33 -153 68.1 90 0';Cmd $target 'setviewpos -28.0921 -153.962 68.1 180 0';Start-Sleep -Seconds 2
  $before=State $target;$observer=$before.localPosition;$ownBefore=State $source
  $health=Native $target '--health' 1
  Input $source B;Start-Sleep -Milliseconds 1000
  Cmd $target 'coop_shadow_crouch off';Cmd $target 'coop_shadow_pose crouchpose 600';Start-Sleep -Milliseconds 700
  for($attempt=0;$attempt-lt18;$attempt++){Cmd $target 'coop_shadow_crouch capture';Start-Sleep -Milliseconds 90;if((State $target).nativeCrouchReferenceReady){break}}
  if(!(State $target).nativeCrouchReferenceReady){throw 'Complete native crouch reference did not capture'}
  $references[$source]=Native $target '--skeleton' (State $target).probeActorHandle
  $sourceReferences[$source]=Native $source '--skeleton' 1
  Cmd $target 'coop_shadow_action off';Input $source B;Start-Sleep -Milliseconds 800
  Cmd $target 'coop_shadow_crouch upright';Cmd $target 'coop_probe_body on';Capture $source Standing 16
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
  if($hipDrop-lt.10-or$returnError-gt.08-or$maxHip-gt.03){$errors.Add('Authored crouch height or return failed')}
  # The authored crouch has higher hips than the first-person pose and lowers
  # the torso too. Test its actual reference, not the rejected FPS hip target.
  $headDrop=0.;if($stand.Count-and$static.Count){$headDrop=($stand[0].Joints|Where-Object nativeName -eq 'center_head_jnt').globalPose[14]-($static[0].Joints|Where-Object nativeName -eq 'center_head_jnt').globalPose[14]}
  if($headDrop-lt.10-or[Math]::Abs($headDrop-$hipDrop)-gt.08){$errors.Add('Upright head height does not track the lowered pelvis')}
  $maxTorsoAngle=0.;$headGain=0.
  if($stand.Count-and$static.Count){
   foreach($row in $static){foreach($name in 'center_spine01_jnt','center_spine02_jnt','center_spine03_jnt','center_head_jnt'){
    $a=($row.Joints|Where-Object nativeName -eq $name).globalPose;$best=[Math]::PI
    foreach($baseline in $stand){$b=($baseline.Joints|Where-Object nativeName -eq $name).globalPose;$tr=0.;foreach($k in 0,1,2,4,5,6,8,9,10){$tr+=$a[$k]*$b[$k]};$best=[Math]::Min($best,[Math]::Acos([Math]::Clamp(($tr-1.)*.5,-1.0,1.0)))}
    $maxTorsoAngle=[Math]::Max($maxTorsoAngle,$best)
   }}
   $headGain=($static[0].Joints|Where-Object nativeName -eq 'center_head_jnt').globalPose[14]-$references[$source].joints[12].globalPose[14]
  }
  if($maxTorsoAngle-gt.25-or$headGain-lt.15){$errors.Add('Torso is not upright relative to native standing or is not distinguishable from the forward search pose')}
  $minKnee=10.;$minToe=10.;$maxBoneLengthError=0.
  foreach($row in @($static)+@($walking)){
   foreach($name in 'left_tibia_jnt','right_tibia_jnt'){$minKnee=[Math]::Min($minKnee,($row.Joints|Where-Object nativeName -eq $name).globalPose[14])}
   foreach($name in 'left_toes_jnt','right_toes_jnt'){$minToe=[Math]::Min($minToe,($row.Joints|Where-Object nativeName -eq $name).globalPose[14])}
   # 10/17 are hand attachment locators, not anatomical limb segments. Native
   # standing and search clips intentionally give them different offsets.
   # Validate their actual weapon alignment separately, above/below, while
   # keeping the 1mm anatomical bone-length tolerance unchanged.
   foreach($joint in $row.Joints|Where-Object {$_.index-ge3-and$_.index-lt27-and$_.index-notin10,17}){
    $a=Distance $joint.localPose[12..14] @(0.,0.,0.);$b=Distance $references[$source].joints[$joint.index].localPose[12..14] @(0.,0.,0.)
    $maxBoneLengthError=[Math]::Max($maxBoneLengthError,[Math]::Abs($a-$b))
   }
  }
  if($minKnee-lt0-or$minToe-lt0-or$maxBoneLengthError-gt.001){$errors.Add('Ground penetration or changed native limb length')}
  $stable=@($rows|Where-Object {$_.Source-eq$source-and$_.Stable})
  if(@($stable|Where-Object {$_.AttachmentError-gt.03-or$_.WeaponRenderError-gt.03}).Count){$errors.Add('Native sword attachment separated')}
  $travel=if($walking.Count-ge2){Distance $walking[0].Root[0..2] $walking[-1].Root[0..2]}else{0}
  $legDelta=0.;foreach($row in $walking){foreach($name in 'left_femur_jnt','left_tibia_jnt','right_femur_jnt','right_tibia_jnt'){$j=@($row.Joints|Where-Object nativeName -eq $name)[0];$initial=@($walking[0].Joints|Where-Object nativeName -eq $name)[0];foreach($k in 0,1,2,4,5,6,8,9,10){$legDelta=[Math]::Max($legDelta,[Math]::Abs($j.localPose[$k]-$initial.localPose[$k]))}}}
  # Measure actual angular excursion, independent of joint coordinate basis
  # and of which gait phase happens to be the first coherent observation.
  $legAngle=0.
  foreach($name in 'left_femur_jnt','left_tibia_jnt','right_femur_jnt','right_tibia_jnt'){
   foreach($a in $walking){$aj=($a.Joints|Where-Object nativeName -eq $name).localPose
    foreach($b in $walking){$bj=($b.Joints|Where-Object nativeName -eq $name).localPose;$trace=0.
     foreach($k in 0,1,2,4,5,6,8,9,10){$trace+=$aj[$k]*$bj[$k]}
     $legAngle=[Math]::Max($legAngle,[Math]::Acos([Math]::Clamp(($trace-1.0)*.5,-1.0,1.0)))
    }
   }
  }
  if($travel-lt.25-or$legAngle-lt.1){$errors.Add('No crouch-walk root and leg movement')}
  $spineDelta=0.;$armDelta=0.
  if($stand.Count){foreach($row in $static){foreach($name in 'center_spine01_jnt','center_spine02_jnt','right_arm_jnt','right_elbow_jnt'){
   $j=@($row.Joints|Where-Object nativeName -eq $name)[0];$base=@($stand[0].Joints|Where-Object nativeName -eq $name)[0]
   foreach($k in 0,1,2,4,5,6,8,9,10){$d=[Math]::Abs($j.localPose[$k]-$base.localPose[$k]);if($name-like'center*'){$spineDelta=[Math]::Max($spineDelta,$d)}else{$armDelta=[Math]::Max($armDelta,$d)}}
  }}}
  if(!$final.bodyCrouchUpright){$errors.Add('Upright comparison was not selected')}
  if((Distance $observer $final.localPosition)-gt.02-or(Native $target '--health' 1).current-ne$health.current-or$ownFinal.bodyStreamApplied-ne$ownBefore.bodyStreamApplied){$errors.Add('Local observer or opposite clone changed')}
  if($final.bodyStreamApplied-le$before.bodyStreamApplied){$errors.Add('No full-body crouch application')}
  if($final.bodyCrouchHelpersApplied-$before.bodyCrouchHelpersApplied-ne$final.bodyStreamApplied-$before.bodyStreamApplied){$errors.Add('Native corrective updates missing')}
  $checks.Add([pscustomobject]@{Role=$source;Passed=(!$errors.Count);Failures=$errors;CoherentCrouch=$static.Count;CoherentWalking=$walking.Count;HipDrop=$hipDrop;MaxHipError=$maxHip;ReturnError=$returnError;Travel=$travel;LegRotationDelta=$legDelta;SpineRotationDelta=$spineDelta;ArmRotationDelta=$armDelta;MaxAttachmentError=($stable.AttachmentError|Measure-Object -Maximum).Maximum;Applications=$final.bodyStreamApplied-$before.bodyStreamApplied})
  $checks[$checks.Count-1]|Add-Member -NotePropertyName MinimumKneeHeight -NotePropertyValue $minKnee
  $checks[$checks.Count-1]|Add-Member -NotePropertyName MaximumLegExcursionRadians -NotePropertyValue $legAngle
  $checks[$checks.Count-1]|Add-Member -NotePropertyName MinimumToeHeight -NotePropertyValue $minToe
  $checks[$checks.Count-1]|Add-Member -NotePropertyName HeadDrop -NotePropertyValue $headDrop
  $checks[$checks.Count-1]|Add-Member -NotePropertyName MaximumBoneLengthError -NotePropertyValue $maxBoneLengthError
  $checks[$checks.Count-1]|Add-Member -NotePropertyName MaximumTorsoAngleRadians -NotePropertyValue $maxTorsoAngle
  $checks[$checks.Count-1]|Add-Member -NotePropertyName HeadHeightGainOverNativeSearch -NotePropertyValue $headGain
  Cmd $target 'coop_probe_body off'
 }
 if(@($checks|Where-Object {!$_.Passed}).Count){throw 'Crouch assertions failed'}
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{Input $r Neutral;Cmd $r 'coop_probe_body off';if((State $r).localPlayerStance){Input $r B}}catch{}}
}
[ordered]@{At=[DateTimeOffset]::Now.ToString('o');Passed=(!$failure);Failure=$failure;Dll=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;Scope='User-requested upright torso comparison: calibrated native crouch legs, standing native spine/head/arms, counter-rotation at attached spine root, native correctives before publication. Flat-floor ground clearance, unchanged limb lengths, standing-relative torso angle and head/pelvis drop checked. No crouched combat or general traversal claim.';References=$references;SourceReferences=$sourceReferences;Checks=$checks;Samples=$rows}|ConvertTo-Json -Depth 19|Set-Content "$PSScriptRoot/research/doppelganger-crouch-upright-verification.json"
$checks|Format-Table -AutoSize
if($failure){throw $failure}

