[CmdletBinding()]
param([ValidateSet('host','client')][string]$Role='host',[Parameter(Mandatory)][ValidatePattern('^[a-z0-9-]+$')][string]$Take,[switch]$CrouchedDrop,[switch]$CrouchedStart,[switch]$Paired,[switch]$TraceContact,
 [ValidatePattern('^[a-z0-9-]+\.json$')][string]$BankManifest='high-throw-bank-manifest-v31.json')
$ErrorActionPreference='Stop'
if(!$Paired-or$CrouchedStart-or$CrouchedDrop){throw 'This experiment requires standing paired high throw'}
if($TraceContact-and!$Paired){throw 'Contact trace requires paired study'}
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab'
$peer=if($Role-eq'host'){'client'}else{'host'};$character=if($Role-eq'host'){'corvo'}else{'emily'}
$out=Join-Path $PSScriptRoot $Take;if(Test-Path -LiteralPath $out){throw 'Take exists'}
New-Item -ItemType Directory -Path $out|Out-Null
$launch=@{};foreach($r in 'host','client'){
 $launch[$r]=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
 if(!$launch[$r].assetExperiment-or(Get-Process -Id $launch[$r].pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'}
}
function C($r,$s){& "$lab/Send-D2Command.ps1" -Role $r -Command $s -WaitConsumed|Out-Null}
function Q($r,$flag,$value=''){
 for($i=0;$i-lt4;$i++){
  $v=if($value){& "$lab/bin/d2_runtime_probe.exe" $launch[$r].pid $flag $value}else{& "$lab/bin/d2_runtime_probe.exe" $launch[$r].pid $flag}
  if(!$LASTEXITCODE){return($v|ConvertFrom-Json)};Start-Sleep -Milliseconds 12
 };throw 'Native read unavailable'
}
function J($r,$suffix=''){
 for($i=0;$i-lt8;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 12}};throw 'Status unavailable'
}
function ContactPair($r,$carrier,$victim){
 $publication=(J $r).bodyPublicationAddress
 for($i=0;$i-lt15;$i++){
  $v=& "$lab/bin/d2_runtime_probe.exe" $launch[$r].pid --contact-pair-final $carrier $victim $publication
  if(!$LASTEXITCODE){return($v|ConvertFrom-Json)};Start-Sleep -Milliseconds 4
 };throw 'Stable compact contact pair unavailable'
}
function I($button='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $Role -Button $button -DurationMs $ms|Out-Null}
$capture=$null;$failure=$null;$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$contacts=[Collections.Generic.List[object]]::new();$nextContact=0.0;$nextSettleTrace=0;$releasePoses=[Collections.Generic.List[object]]::new();$nextReleasePose=8200;$stanceContactSaved=$false
try{
 foreach($r in 'host','client'){
  & "$lab/Send-D2Input.ps1" -Role $r -DurationMs 100|Out-Null
  foreach($c in 'coop_carry_attachment off','coop_review_clip off','coop_movement_resume off','coop_movement_bank off','coop_shadow_stream off','coop_shadow_equipment off','coop_movement_fixture off','coop_world_observe on','coop_probe_follow exact','coop_probe_locomotion on','coop_movement_feet authored'){C $r $c}
 }
 $bank=Get-Content (Join-Path $PSScriptRoot $BankManifest) -Raw|ConvertFrom-Json|Where-Object role -eq $character
 $file=Join-Path $PSScriptRoot $bank.bank;if((Get-FileHash -LiteralPath $file).Hash-ne$bank.sha256){throw 'Bank changed'}
 Copy-Item -LiteralPath $file -Destination "$root/work/lab-instances/$peer/coop-status.json.movement.bin" -Force
 C $Role 'setviewpos -38.95 -161.5 68.1 270 0';C $peer 'setviewpos -36.7 -161.8 68.1 173 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Role -Pitch 50 -Evidence "$out/source-camera.json"|Out-Null
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $peer -Pitch 20 -Evidence "$out/observer-camera.json"|Out-Null
 if($CrouchedStart){
  $attempts=[Collections.Generic.List[object]]::new()
  for($attempt=0;$attempt-lt3;$attempt++){
   $st=Q $Role '--player-states';$attempts.Add($st);if($st.stance-eq1){break}
   I B 180;Start-Sleep -Milliseconds 650
  }
  $st=Q $Role '--player-states';if($st.stance-ne1){throw 'Native crouch entry failed'}
  $st|ConvertTo-Json -Depth 12|Set-Content "$out/crouched-before.json"
  $attempts|ConvertTo-Json -Depth 12|Set-Content "$out/crouch-entry-attempts.json"
 }
 $attachmentBefore=J $peer '.carry-attachment.json';$attachmentBefore|ConvertTo-Json -Depth 8|Set-Content "$out/attachment-before.json"
 $nativeBefore=Q $Role '--player-states';if($nativeBefore.carrySceneValid){throw 'Source already holds a body'}
 $healthBefore=@{source=(Q $Role '--health' '1');observer=(Q $peer '--health' '1');avatar=(Q $peer '--health' ([string](J $peer).probeActorHandle))}
 (Q $peer '--avatar-sword' ([string](J $peer).probeActorHandle))|ConvertTo-Json -Depth 12|Set-Content "$out/sword-before.json"
 $world=Q $peer '--world-npcs';$targets=@($world.npcs|Where-Object {$_.spawnerName-eq'ld_spawner_alexi_escape'-and$_.definition-eq'models/characters/def/story/alexi_mayhew.def'})
 if($targets.Count-ne1-or!$targets[0].unique-or!$targets[0].reciprocal){throw 'Unique authored corpse missing'}
 $target=[string]$targets[0].handle;if((Q $peer '--health' $target).current-ne0){throw 'Expected dead test body'}
 (Q $peer '--npc-pose' $target)|ConvertTo-Json -Depth 10|Set-Content "$out/corpse-before.json"
 if($Paired){
  $parts=Q $peer '--components' $target;$physics=@($parts.components|Where-Object type -eq '.?AVarkCpntPhysicsSystemEvolved@@')
  if($physics.Count-ne1){throw 'Unique corpse physics missing'}
  (Q $peer '--object' $physics[0].pointer)|ConvertTo-Json -Depth 12|Set-Content "$out/physics-before.json"
 }
 foreach($r in 'host','client'){C $r 'coop_movement_fixture 60000';C $r 'coop_shadow_stream on'}
 Start-Sleep -Milliseconds 550;C $peer 'coop_movement_bank on';Start-Sleep -Milliseconds 900
 $s=J $peer '.shadow-stream.json';$b=J $peer '.movement.json'
 $bankVersion=[int]$bank.format.Substring(6)
 if(!$s.ready-or!$b.active-or$b.version-ne$bankVersion){throw 'Pre-armed stream or exact bank version unavailable'}
 $s|ConvertTo-Json -Depth 6|Set-Content "$out/stream-before.json"
 $b|ConvertTo-Json -Depth 6|Set-Content "$out/bank-before.json"
 $window=(Get-Process -Id $launch[$peer].pid).MainWindowHandle.ToInt64();if(!$window){throw 'No observer window'}
 $captureDuration=if($Paired){'15'}else{'10'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',$captureDuration,'-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/native.mp4")
 $capture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 $clock=[Diagnostics.Stopwatch]::StartNew();$inputMs=$null;$pairMs=$null;$cancelMs=$null;$heldSaved=$false;$moveSent=$false;$crouchSent=$false
 $observeUntil=if($Paired){12500}else{7000}
 while($clock.ElapsedMilliseconds-lt$observeUntil){
  if($capture.HasExited){throw 'Capture ended early'}
  if($null-eq$inputMs-and$clock.ElapsedMilliseconds-ge1100){I X 900;$inputMs=$clock.ElapsedMilliseconds}
  $begin=$clock.ElapsedMilliseconds;$streamAtBegin=J $peer '.shadow-stream.json';$st=Q $Role '--player-states'
  $m=J $peer '.movement.json'
  if($Paired-and$null-eq$pairMs-and($m.carryVariant-eq7-or($m.carryVariant-eq1-and$m.carrySampleAmount-lt.10))){
   C $peer 'coop_carry_attachment pickup8000';$pairMs=$clock.ElapsedMilliseconds
  }
  $traceLift=$m.carryVariant-in @(7,1)-and$m.carryAuthoredAmount-ge$nextContact
  $traceSettle=$bankVersion-ge34-and$m.carryVariant-eq2-and$m.carrySettle-lt1-and$clock.ElapsedMilliseconds-ge$nextSettleTrace
  if($TraceContact-and$null-ne$pairMs-and($traceLift-or$traceSettle)){
   $cb=$clock.ElapsedMilliseconds;$pair=ContactPair $peer ([string](J $peer).probeActorHandle) $target
   $contacts.Add([pscustomobject]@{beginMs=$cb;endMs=$clock.ElapsedMilliseconds;movementBefore=$m;movementAfter=(J $peer '.movement.json');readBeginTick=$pair.readBeginTick;readEndTick=$pair.readEndTick;carrierFinalPublicationComplete=$pair.carrierFinalPublicationComplete;publicationSequence=$pair.publicationSequence;atomic=$false;carrier=$pair.carrier;victim=$pair.victim})
   $nextContact=$m.carryAuthoredAmount+.045
   $nextSettleTrace=$clock.ElapsedMilliseconds+45
  }
  if($Paired-and!$heldSaved-and$clock.ElapsedMilliseconds-gt4400){
   $a=J $peer '.carry-attachment.json';if(!$a.active-or!$a.anchorCalibrated-or!$a.sourcePickup){throw 'Source-timed paired lift did not attach'}
   (Q $peer '--skeleton' $target)|ConvertTo-Json -Depth 12|Set-Content "$out/victim-held.json"
   (Q $peer '--skeleton' ([string](J $peer).probeActorHandle))|ConvertTo-Json -Depth 12|Set-Content "$out/carrier-held.json"
   (Q $peer '--retargeting' ([string](J $peer).probeActorHandle))|ConvertTo-Json -Depth 12|Set-Content "$out/retarget-held.json"
   & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Role -Pitch 0 -Evidence "$out/drop-camera.json"|Out-Null; $heldSaved=$true
  }
  if($heldSaved-and!$moveSent-and$clock.ElapsedMilliseconds-ge4700){
   & "$lab/Send-D2Input.ps1" -Role $Role -LeftY 12000 -DurationMs 600|Out-Null;$moveSent=$true
  }
  if(($CrouchedDrop-ne$CrouchedStart)-and!$crouchSent-and$clock.ElapsedMilliseconds-ge5550){I B 150;$crouchSent=$true}
  if(!$stanceContactSaved-and$clock.ElapsedMilliseconds-ge8050){
   $cb=$clock.ElapsedMilliseconds;$pair=ContactPair $peer ([string](J $peer).probeActorHandle) $target
   [ordered]@{beginMs=$cb;endMs=$clock.ElapsedMilliseconds;movement=(J $peer '.movement.json');native=(Q $Role '--player-states');pair=$pair}|ConvertTo-Json -Depth 18|Set-Content "$out/loaded-stance-contact.json"
   $stanceContactSaved=$true
  }
  if($null-eq$cancelMs-and$clock.ElapsedMilliseconds-ge8500){& "$lab/Send-D2Input.ps1" -Role $Role -RightTrigger 255 -DurationMs 500|Out-Null;$cancelMs=$clock.ElapsedMilliseconds}
  $rows.Add([pscustomobject]@{beginMs=$begin;endMs=$clock.ElapsedMilliseconds;observerStreamAtBegin=$streamAtBegin;native=$st;observerStream=(J $peer '.shadow-stream.json');sourceStream=(J $Role '.shadow-stream.json');movement=$m;equipment=(J $peer '.shadow-equipment.json');observerRoot=(J $peer);sourceRoot=(J $Role);attachment=(J $peer '.carry-attachment.json')})
  if($clock.ElapsedMilliseconds-ge$nextReleasePose-and$clock.ElapsedMilliseconds-lt11300){
   $b=$clock.ElapsedMilliseconds;$p=Q $peer '--npc-pose' $target
   $releasePoses.Add([pscustomobject]@{beginMs=$b;endMs=$clock.ElapsedMilliseconds;pose=$p;attachment=(J $peer '.carry-attachment.json')})
   $nextReleasePose=$clock.ElapsedMilliseconds+100
  }
  Start-Sleep -Milliseconds 25
 }
 $after=Q $Role '--player-states';if($after.carrySceneValid-or@($after.active|Where-Object {$_.id-in @(45,46)}).Count){throw 'Native drop did not finish'}
 $after|ConvertTo-Json -Depth 12|Set-Content "$out/source-after-drop.json"; $checks.Add('Source native pickup, carry walk and high throw completed')
 $healthAfter=@{source=(Q $Role '--health' '1');observer=(Q $peer '--health' '1');avatar=(Q $peer '--health' ([string](J $peer).probeActorHandle))}
 foreach($k in 'source','observer','avatar'){if($healthAfter[$k].current-ne$healthBefore[$k].current){throw 'Health changed'}}
 $checks.Add('Source, observer and avatar health unchanged')
 (Q $peer '--avatar-sword' ([string](J $peer).probeActorHandle))|ConvertTo-Json -Depth 12|Set-Content "$out/held-sword.json"
 (Q $peer '--npc-pose' $target)|ConvertTo-Json -Depth 10|Set-Content "$out/corpse-after.json"
 if($Paired){
  (Q $peer '--object' $physics[0].pointer)|ConvertTo-Json -Depth 12|Set-Content "$out/physics-after.json"
  (Q $peer '--retargeting' ([string](J $peer).probeActorHandle))|ConvertTo-Json -Depth 12|Set-Content "$out/retarget-after.json"
  (Q $peer '--health' $target)|ConvertTo-Json -Depth 10|Set-Content "$out/corpse-health-after.json"
 }
 (J $peer '.carry-attachment.json')|ConvertTo-Json -Depth 8|Set-Content "$out/attachment-after.json"
 $scope=if($Paired){'Explicit bounded Mayhew paired experiment: ordinary native X pickup drives the carrier through Steam and the victim snapshot blend through the same source hoist clock. Late native retargeting supports the load. Source walks and requests native high throw using RT; source-clock remote release queues a single native event149 after physics handoff; native first-person is unchanged. Contacts and full pickup quality unaccepted.'}else{'Real native player X pickup drives remote carrier through native Steam semantic stream. Standing carrier-only gather/hoist study; crouched take tests continuity only. Remote corpse is deliberately unmodified and not lifted. No complete pickup claim.'}
 [ordered]@{role=$Role;peer=$peer;character=$character;crouched=[bool]$CrouchedStart;nativeDrop=$false;nativeThrow=$true;crouchedDrop=[bool]$CrouchedDrop;paired=[bool]$Paired;dllSha256=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;bankSha256=$bank.sha256;bankVersion=$bankVersion;bankManifest=$BankManifest;clipSha256=$bank.clip_sha256;probeSha256=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash;harnessSha256=(Get-FileHash $PSCommandPath).Hash;inputMs=$inputMs;pairMs=$pairMs;cancelMs=$cancelMs;healthBefore=$healthBefore;healthAfter=$healthAfter;corpse=$target;scope=$scope}|ConvertTo-Json -Depth 12|Set-Content "$out/identity.json"
}catch{$failure=$_.Exception.Message;throw}finally{
 I
 foreach($r in 'host','client'){try{C $r 'coop_carry_attachment off';C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_movement_fixture off'}catch{}}
 Start-Sleep -Milliseconds 350
 [ordered]@{movement=(J $peer '.movement.json');equipment=(J $peer '.shadow-equipment.json');stream=(J $peer '.shadow-stream.json')}|ConvertTo-Json -Depth 10|Set-Content "$out/stopped.json"
 (Q $peer '--avatar-sword' ([string](J $peer).probeActorHandle))|ConvertTo-Json -Depth 12|Set-Content "$out/sword-restored.json"
 if($capture-and!$capture.HasExited){$capture.WaitForExit(12000)|Out-Null}
 $releasePoses|ConvertTo-Json -Depth 12|Set-Content "$out/release-poses.json"
 $rows|ConvertTo-Json -Depth 18|Set-Content "$out/observations.json"
 if($TraceContact){$contacts|ConvertTo-Json -Depth 18|Set-Content "$out/contact-trace.json"}
 [ordered]@{passed=(!$failure);failure=$failure;samples=$rows.Count;checks=$checks;complete=$false;visual_accepted=$false}|ConvertTo-Json -Depth 6|Set-Content "$out/result.json"
}
