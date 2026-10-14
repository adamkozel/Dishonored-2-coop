param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='ranged-corvo-gun-network-v29-v1',[ValidateSet('host','client')][string]$Source='host',[ValidateSet('gun','crossbow')][string]$Kind='gun',[switch]$Mechanisms,[switch]$ReadEffects,[switch]$Muzzle,[switch]$Crouched,[ValidateSet(29,30)][int]$BankVersion=29,[ValidateSet(1,2,3,4)][int]$BankRevision=1,[switch]$Travel,[switch]$InspectSword,[ValidateSet('oblique','front')][string]$CameraView='oblique',[switch]$TransitionStance)
if($Travel-and$CameraView-ne'oblique'){throw 'Travelling comparison requires the wide observer camera'}
if($TransitionStance-and(!$Crouched-or$BankVersion-ne30-or$Travel)){throw 'Stance transitions require the v30 crouched stationary fixture'}
if($Muzzle){if($Kind-ne'gun'){throw 'Muzzle test requires gun'};$ReadEffects=$true}
$peer=if($Source-eq'host'){'client'}else{'host'};$Holstered=$false;$initialStance=$null;$inputX=0;$inputY=0
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$pids=@{};$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$capture=$null;$sourceCapture=$null;$start=0
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$trigger=0,$ms=100,$button='Neutral'){$x=if($r-eq$Source){$inputX}else{0};$y=if($r-eq$Source){$inputY}else{0};& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms -LeftTrigger $trigger -LeftX $x -LeftY $y|Out-Null}
function Q($r,$mode,$id=0){for($i=0;$i-lt10;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read $r $mode"}
function S($r,$suffix=''){return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;I $r
  foreach($c in 'coop_movement_resume off','coop_movement_bank off','coop_shadow_stream off','coop_ranged_prop off','coop_review_clip off','coop_power_fixture off','coop_darkvision_fixture off','coop_ranged_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $c}
  if((Q $r '--health' 1).current-ne56){throw 'Clean living fixture required'}
 }
 C $Source 'setviewpos -33.5 -153 68.1 90 0'
 if($Travel){C $peer 'setviewpos -27.5 -149.3 68.1 212 0'}elseif($CameraView-eq'front'){C $peer 'setviewpos -33.5 -150 68.1 270 0'}else{C $peer 'setviewpos -30.7 -151.8 68.1 203 0'}
 $initialStance=(Q $Source '--player-states').stance
 if($initialStance-ne[int][bool]$Crouched){I $Source 0 100 B;Start-Sleep -Milliseconds 700}
 if((Q $Source '--player-sword').equipped-eq[bool]$Holstered){$duration=if($Holstered){1500}else{650};I $Source 0 $duration X;Start-Sleep -Milliseconds 2300}
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Source -Pitch 0 -Evidence "$out/camera.json"
 $before=Q $Source "--player-$Kind";$inventory=Q $Source '--player-inventory';$peerBefore=Q $peer '--player-inventory'
 $before|ConvertTo-Json -Depth 6|Set-Content -Encoding utf8 "$out/before.json";$inventory|ConvertTo-Json -Depth 8|Set-Content -Encoding utf8 "$out/inventory-before.json"
 if($before.quantity-or$before.equipped-or$before.started){throw 'Native ranged weapon already owned or active'}
 C $Source "coop_ranged_fixture ${Kind}_loaded";$w=[Diagnostics.Stopwatch]::StartNew();do{$s=S $Source '.ranged-fixture.json';if($s.result-eq'native_ranged_ready'){break};Start-Sleep -Milliseconds 60}while($w.ElapsedMilliseconds-lt6000)
 if($s.result-ne'native_ranged_ready'){throw "ranged weapon did not equip: $($s.result)"}
 $power=Q $Source "--player-$Kind";$power|ConvertTo-Json -Depth 6|Set-Content -Encoding utf8 "$out/enabled.json"
 if(!$power.equipped-or!$power.started-or$power.quantity-ne1){throw 'ranged weapon acquisition unconfirmed'}
 # Inventory acquisition alone does not enter the native left-arm equipment
 # presentation. Use ordinary native holster/draw input before recording.
 I $Source 0 1500 X;$w.Restart();do{Start-Sleep -Milliseconds 100;$weapon=Q $Source "--player-$Kind";$sword=Q $Source '--player-sword'}while(($weapon.equipped-or$sword.equipped)-and$w.ElapsedMilliseconds-lt5000)
 if($weapon.equipped-or$sword.equipped){throw 'Initial native holster incomplete'}
 I $Source 0 650 X;$w.Restart();do{Start-Sleep -Milliseconds 100;$weapon=Q $Source "--player-$Kind";$sword=Q $Source '--player-sword'}while((!$weapon.equipped-or!$sword.equipped)-and$w.ElapsedMilliseconds-lt5000)
 if(!$weapon.equipped-or!$sword.equipped){throw 'Initial native draw incomplete'}
 Start-Sleep -Milliseconds 1200
 $checks.Add('Exact native ranged equipment acquired; normal controller holster/draw initializes native arm presentation')
 $observerPitch=if($CameraView-eq'front'){18}else{12}
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $peer -Pitch $observerPitch -Evidence "$out/observer-camera.json"
 $character=if($Source-eq'host'){'corvo'}else{'emily'}
 $bank=if($BankVersion-eq30){"$PSScriptRoot/crouched-ranged-banks-v$BankRevision/$character-crouched-ranged-bank.bin"}else{"$PSScriptRoot/ranged-banks-v1/$character-ranged-bank.bin"}
 Copy-Item $bank "$root/work/lab-instances/$peer/coop-status.json.movement.bin" -Force
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 550;C $peer 'coop_movement_bank on'
 $w.Restart();do{$m=S $peer '.movement.json';$prop=S $peer '.ranged-prop.json';if($m.active-and$m.version-eq$BankVersion-and$m.rangedWeight-gt.9-and$prop.active){break};Start-Sleep -Milliseconds 50}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne$BankVersion-or$m.rangedWeight-lt.9-or!$prop.active){[ordered]@{movement=$m;prop=$prop;stream=(S $peer '.shadow-stream.json');sourceStream=(S $Source '.shadow-stream.json');player=(Q $Source '--player-states')}|ConvertTo-Json -Depth 10|Set-Content "$out/ready-failure.json";throw 'Ranged network body/prop did not become ready'}
 $checks.Add("Native Steam v27 scene stream, v$BankVersion body bank and automatic cosmetic weapon attach ready")
 if($InspectSword){
  $base=S $peer;$stablePose=$false
  for($attempt=0;$attempt-lt30;$attempt++){
   $raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$peer] --skeleton-final $base.probeActorHandle $base.bodyPublicationAddress
   if(!$LASTEXITCODE){$pose=$raw|ConvertFrom-Json;if($pose.stable-and$pose.finalPublicationComplete){$raw|Set-Content -Encoding utf8 "$out/idle-final-skeleton.json";$stablePose=$true;break}}
  }
  if(!$stablePose){throw 'Stable final ranged sword pose unavailable'}
  $attachment=$null
  for($attempt=0;$attempt-lt30;$attempt++){$attachment=Q $peer '--avatar-sword-attachment' $base.probeActorHandle;if($attachment.stable){break}}
  $attachment|ConvertTo-Json -Depth 12|Set-Content -Encoding utf8 "$out/idle-sword-attachment.json"
  (S $peer '.movement.json')|ConvertTo-Json|Set-Content -Encoding utf8 "$out/idle-movement.json"
 }
 $fxBefore=S $peer '.power-fx.json';$duplicated=$false
 $prop|ConvertTo-Json|Set-Content "$out/prop-initial.json"
 $window=(Get-Process -Id $pids[$peer]).MainWindowHandle.ToInt64();if(!$window){throw 'No peer game window'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','18','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();$capture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 $sourceWindow=(Get-Process -Id $pids[$Source]).MainWindowHandle.ToInt64()
 # Replace by its observed value, not by a guessed positional argument index.
 $sourceArgs=@($args|ForEach-Object {if($_-like 'gfxcapture=*'){"gfxcapture=hwnd=${sourceWindow}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra"}elseif($_-eq"$PSScriptRoot/$Take.mp4"){"$PSScriptRoot/$Take-source.mp4"}else{$_}})
 $sourceCapture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $sourceArgs -WindowStyle Hidden -RedirectStandardError "$out/capture-source.log" -RedirectStandardOutput "$out/capture-source.stdout" -PassThru
 Start-Sleep -Milliseconds 700;if($capture.HasExited){throw 'Recorder failed'}
 $watch=[Diagnostics.Stopwatch]::StartNew();$next=0;$times=@(1000,4000,7000,10000,13000,15000);$sceneCount=0;$fireCount=0;$emptyCount=0;$scenes=[Collections.Generic.HashSet[string]]::new();$inputRenewAt=0;$stanceNext=0;$stanceTimes=@(3000,6000,9000)
 $directions=@(@(0,20000),@(14142,14142),@(20000,0),@(14142,-14142),@(0,-20000),@(-14142,-14142),@(-20000,0),@(-14142,14142))
 while($watch.ElapsedMilliseconds-lt16300){
  if($Travel){$direction=[int][Math]::Floor($watch.ElapsedMilliseconds/1000);if($direction-lt$directions.Count){$inputX=$directions[$direction][0];$inputY=$directions[$direction][1]}else{$inputX=0;$inputY=0}}
  if($next-lt$times.Count-and$watch.ElapsedMilliseconds-ge$times[$next]){if($next-eq3){$hold=1500;I $Source 0 $hold X;$action='Native X hold 1500ms'}elseif($next-eq4){$hold=650;I $Source 0 $hold X;$action='Native X draw 650ms'}else{$hold=250;I $Source 255 $hold;$action='Native LT press 250ms'};$inputRenewAt=$watch.ElapsedMilliseconds+$hold+30;$events.Add([pscustomobject]@{seconds=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;action=$action;index=$next});$next++}
  elseif($TransitionStance-and$stanceNext-lt$stanceTimes.Count-and$watch.ElapsedMilliseconds-ge$stanceTimes[$stanceNext]){I $Source 0 100 B;$events.Add([pscustomobject]@{seconds=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;action='Native crouch toggle during ranged sequence';index=$stanceNext;testTime=$watch.ElapsedMilliseconds});$stanceNext++}
  elseif($Travel-and$watch.ElapsedMilliseconds-lt8500-and$watch.ElapsedMilliseconds-ge$inputRenewAt){I $Source 0 350;$inputRenewAt=$watch.ElapsedMilliseconds+200}
  $native=Q $Source '--player-states';$midnight=Q $Source '--midnight';$ranged=$null;try{$ranged=Q $Source "--player-$Kind"}catch{};foreach($scene in $midnight.instances){if($scene.asset-match'/guns/'){$null=$scenes.Add("$($scene.asset)|$($scene.subscene)|$($scene.sceneDurationMs)");if($scene.asset-match('/'+$Kind+'_fire')){$fireCount++};if($scene.asset-match('/'+$Kind+'_ooa')){$emptyCount++}}}
  $prop=S $peer '.ranged-prop.json';$sourceRig=$null;$remoteRig=$null;$effects=$null
  if($Muzzle-and!$duplicated-and$watch.ElapsedMilliseconds-gt1150){C $Source 'coop_shadow_stream_duplicate';$duplicated=$true}
  if($Mechanisms){
   $raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$Source] --weapon-skeleton $power.handle;if(!$LASTEXITCODE){$sourceRig=$raw|ConvertFrom-Json}
   $raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$peer] --prop-skeleton $prop.handle;if(!$LASTEXITCODE){$remoteRig=$raw|ConvertFrom-Json}
  }
  if($ReadEffects){$raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$Source] --ranged-effects;if($raw){$effects=$raw|ConvertFrom-Json}}
  $rows.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;player=$native;ranged=$ranged;midnight=$midnight;movement=(S $peer '.movement.json');prop=$prop;stream=(S $peer '.shadow-stream.json');sourceStream=(S $Source '.shadow-stream.json');sourceRig=$sourceRig;remoteRig=$remoteRig;effects=$effects;remoteFx=(S $peer '.power-fx.json')})
  if(($midnight|ConvertTo-Json -Depth 12 -Compress)-match('guns/'+$Kind+'_(ooa|fire)')){$sceneCount++}
  if(!(S $Source '.ranged-fixture.json').active){throw 'ranged weapon fixture retired during native capture'};Start-Sleep -Milliseconds 25
 }
 if(!$fireCount-or!$emptyCount){throw 'Native fire and empty action sequence incomplete'};$checks.Add('Two native shots and a subsequent native empty trigger action observed');
 $expectedVariants=if($Kind-eq'gun'){@(1,3,4,5,6)}else{@(2,7,8,9,10)}
 $observedVariants=@($rows|Where-Object {$_.player.rangedSceneValid}|ForEach-Object {$_.player.rangedScene.variant}|Sort-Object -Unique)
 foreach($variant in $expectedVariants){if($variant-notin$observedVariants){throw "Owned ranged scene reader missed variant $variant"}}
 $remoteVariants=@($rows|Where-Object {$_.movement.rangedWeight-gt.1}|ForEach-Object {$_.movement.rangedVariant}|Sort-Object -Unique)
 foreach($variant in $expectedVariants){if($variant-notin$remoteVariants){throw "Remote body missed ranged variant $variant"}}
 if(@($rows|Where-Object {$_.movement.refused-or$_.prop.refused-or!$_.movement.active}).Count){throw 'Remote pose or prop refused during playback'}
 if($Crouched-and!$TransitionStance){
  if(@($rows|Where-Object {$_.player.stance-ne1-or!$_.player.motionValid-or$_.player.motion-ne1-or$_.movement.stanceMix-lt.95}).Count){throw 'Crouched ranged source or remote body lost its locomotion context'}
  $checks.Add('Both source motion and complete remote body remain crouched through all ranged phases')
 }
 if($TransitionStance){
  $toggles=@($events|Where-Object {$_.action-eq'Native crouch toggle during ranged sequence'})
  if($toggles.Count-ne3){throw 'Native stance sequence incomplete'}
  for($i=0;$i-lt3;$i++){
   $expected=if($i-eq1){1}else{0};$at=$toggles[$i].testTime
   $nativeChange=@($rows|Where-Object {$_.ms-ge$at-and$_.ms-le$at+600-and$_.player.stance-eq$expected})|Select-Object -First 1
   if(!$nativeChange){throw "Native stance toggle $i was not observed within 600ms"}
   # MovementClock intentionally blends the complete body over 800ms.
   # Allow its 800ms plus the delayed Steam timeline, measured from observed
   # native stance rather than prematurely from a queued input command.
   $settled=@($rows|Where-Object {$_.ms-ge$nativeChange.ms+1000-and$_.ms-le$nativeChange.ms+1800})
   if($settled.Count-lt3-or@($settled|Where-Object {$_.player.stance-ne$expected-or!$_.player.motionValid-or$_.player.motion-ne$expected-or[Math]::Abs($_.movement.stanceMix-$expected)-gt.05}).Count){throw "Ranged stance transition $i did not settle on source and peer"}
  }
  if(!@($rows|Where-Object {$_.player.stance-eq0-and$_.movement.rangedWeight-gt.9-and$_.movement.rangedVariant-in@(3,7)}).Count-or!@($rows|Where-Object {$_.player.stance-eq1-and$_.movement.rangedWeight-gt.9-and$_.movement.rangedVariant-in@(3,7)}).Count){throw 'Native firing did not cover both stances'}
  $checks.Add('Three native standing/crouching changes settle on the remote complete body; firing remains source-owned in both stances')
 }
 if($Travel){
  $distance=0.0;for($i=1;$i-lt$rows.Count;$i++){$a=$rows[$i-1].player.position;$b=$rows[$i].player.position;$distance+=[Math]::Sqrt([Math]::Pow($b[0]-$a[0],2)+[Math]::Pow($b[1]-$a[1],2))}
  $travelling=@($rows|Where-Object {$_.movement.speed-gt.25-and$_.movement.rangedWeight-gt.9})
  if($distance-lt1-or$travelling.Count-lt10-or!@($travelling|Where-Object {$_.movement.rangedVariant-eq3-or$_.movement.rangedVariant-eq7}).Count){throw 'Moving ranged test did not combine native travel and firing'}
  $phases=@($travelling|ForEach-Object {$_.movement.phase}|Sort-Object -Unique);if($phases.Count-lt10){throw 'Travelling feet did not advance with native movement'}
  $checks.Add("Native directional travel and firing overlap; distance-controlled remote gait advances ($([Math]::Round($distance,3)) m source path)")
 }
 if($Mechanisms){
  if(!@($rows|Where-Object {$_.sourceRig.stable}).Count-or!@($rows|Where-Object {$_.remoteRig.stable}).Count){throw 'Independent mechanism skeleton observations unavailable'}
  if($Kind-eq'crossbow'-and(!@($rows|Where-Object {$_.prop.mechanismApplied-gt50}).Count-or@($rows|Where-Object {$_.prop.mechanismRefused}).Count)){throw 'Crossbow mechanical pose refused or never applied'}
  $checks.Add('Independent source/remote skeleton observations captured; exact cosmetic crossbow phase callback applied without refusals')
  if($Kind-eq'crossbow'){
   $emptyHeld=@($rows|Where-Object {$_.ranged.mechanismValid-and$_.ranged.mechanism-eq2-and$_.ranged.ammo-eq0-and$_.movement.rangedVariant-eq0-and$_.movement.rangedWeight-gt.9-and$_.ms-gt5500-and$_.ms-lt10000})
   if($emptyHeld.Count-lt5-or@($emptyHeld|Where-Object {$_.movement.rangedMechanism-ne2}).Count){throw 'Native fired state did not persist on the remote idle crossbow'}
   if(!@($rows|Where-Object {$_.ranged.mechanismValid-and$_.ranged.mechanism-eq1-and$_.movement.rangedMechanism-eq1}).Count){throw 'Native cocked state did not reach remote presentation'}
   $checks.Add('Source-owned cocked/fired clip state reaches the remote body and remains fired after the last round, across scene completion')
  }
 }
 $checks.Add('All five source-owned ranged phases reached the remote body through Steam')
 if($Muzzle){
  $fxAfter=S $peer '.power-fx.json'
  if($fxAfter.started-$fxBefore.started-ne2-or$fxAfter.refused-ne$fxBefore.refused){throw "Expected exactly two remote muzzle effects: started=$($fxAfter.started-$fxBefore.started), refused=$($fxAfter.refused-$fxBefore.refused)"}
  $name=if($Source-eq'host'){'models/effects/weapons/gun/muzzle_1p_corvo.fx'}else{'models/effects/weapons/gun/muzzle_1p_emily.fx'}
  $ids=@($rows|ForEach-Object {$_.effects.effects}|Where-Object {$_.name-eq$name}|ForEach-Object {$_.id}|Sort-Object -Unique)
  if($ids.Count-ne2){throw 'Expected two independently observed source native muzzle IDs'}
  if(!@($rows|Where-Object {$_.movement.rangedShot-eq1-and$_.movement.rangedVariant-eq3}).Count){throw 'No delayed native shot confirmation observed'}
  if($fxAfter.active){throw 'Finite muzzle effects did not finish before the empty-trigger check'}
  $checks.Add('Two source native flashes produce exactly two finite remote muzzle effects; empty triggers and duplicate packet do not add flashes')
 }
 $checks.Add('Owned native scene reader validates fire, reload, empty, equip and unequip identities and clocks');$scenes|Sort-Object|Set-Content -Encoding utf8 "$out/scenes.txt"
 I $Source;C $peer 'coop_movement_bank off';C $peer 'coop_ranged_prop off';foreach($r in 'host','client'){C $r 'coop_shadow_stream off'};C $Source 'coop_ranged_fixture off';$w.Restart();do{$s=S $Source '.ranged-fixture.json';if(!$s.active){break};Start-Sleep -Milliseconds 80}while($w.ElapsedMilliseconds-lt7000)
 if($s.active-or$s.result-ne'quantity_and_selection_restored'){throw "ranged weapon cleanup unconfirmed: $($s.result)"}
 $after=Q $Source "--player-$Kind";$after|ConvertTo-Json -Depth 6|Set-Content -Encoding utf8 "$out/after.json"
 if($after.quantity-or$after.equipped-or$after.started){throw 'ranged weapon remained owned, equipped or active'}
 $afterInventory=Q $Source '--player-inventory';$afterInventory|ConvertTo-Json -Depth 8|Set-Content -Encoding utf8 "$out/inventory-after.json"
 $a=@($inventory.items|ForEach-Object {"$($_.name):$($_.quantity)"});$b=@($afterInventory.items|ForEach-Object {"$($_.name):$($_.quantity)"});if(!$a.Count-or(Compare-Object $a $b)){throw 'Inventory quantities changed after cleanup'}
 $peerAfter=Q $peer '--player-inventory';if(Compare-Object @($peerBefore.items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($peerAfter.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Peer inventory changed'}
 $checks.Add('Native cleanup restores source inventory quantity/selection; peer inventory unchanged')
 foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56){throw 'Native player health changed'}}
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected save changed'}}
 $checks.Add('Both player health and protected files unchanged; fixture restores its two-round ammunition grant')
}catch{$failure=$_.Exception.Message}finally{
 $inputX=0;$inputY=0
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_ranged_prop off';C $r 'coop_ranged_fixture off';Start-Sleep -Milliseconds 1500;C $r 'coop_movement_fixture off'}catch{}}
 if($null-ne$initialStance){try{if((Q $Source '--player-states').stance-ne$initialStance){I $Source 0 100 B;Start-Sleep -Milliseconds 700};if((Q $Source '--player-states').stance-ne$initialStance){throw 'Stance restore failed'}}catch{$failure='Native initial stance restoration failed'}}
 if($capture){$capture.WaitForExit(16000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode-ne0){$failure='Native capture incomplete'}}
 if($sourceCapture){$sourceCapture.WaitForExit(16000)|Out-Null;if(!$sourceCapture.HasExited-or$sourceCapture.ExitCode-ne0){$failure='Source capture incomplete'}}
 $rows|ConvertTo-Json -Depth 18|Set-Content -Encoding utf8 "$out/states.json"
 [ordered]@{passed=(!$failure);bankVersion=$BankVersion;bankRevision=$BankRevision;bankSha256=$(if($bank-and(Test-Path $bank)){(Get-FileHash $bank).Hash}else{$null});source=$Source;kind=$Kind;cameraView=$CameraView;stanceTransitions=[bool]$TransitionStance;travelling=[bool]$Travel;crouched=[bool]$Crouched;holstered=[bool]$Holstered;failure=$failure;checks=$checks;events=$events;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;capture='Actual peer body and native source POV; source native input drives Steam presentation';scope='Two-round native ranged body/hand attachment integration candidate. Muzzle checks, when explicitly listed, validate source-confirmed finite native effects. Visual acceptance, remote projectile/impact synchronization, audio validation, upgrades and unlisted stance/movement contexts remain unfinished. Native gameplay may move destructible objects; reload the isolated checkpoint before other tests. Native projectile and impact state requires an isolated checkpoint reload after this test. No saves or original assets modified.'}|ConvertTo-Json -Depth 7|Set-Content -Encoding utf8 "$out/result.json"
}
if($failure){throw $failure};"RANGED_NETWORK_VERIFIED $Source $Kind $($rows.Count) samples"
