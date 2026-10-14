param([ValidatePattern('^[a-z0-9-]+$')][string]$Take,[ValidateRange(-15,45)][int]$Pitch=0)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(!$Take-or(Test-Path $out)){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
$pids=@{};$inventory=@{};$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$peerCapture=$null;$capture=$null;$monitor=$null;$ownedMonitor=$null;$targetMonitor=$null;$fxMonitor=$null;$dirty=$false;$failure=$null
$initialDll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;$initialProbe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$lt=0,$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button Neutral -LeftTrigger $lt -DurationMs $ms|Out-Null}
function S($r,$suffix=''){for($a=0;$a-lt8;$a++){try{$v=Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json;if($null-ne$v){return $v}}catch{};Start-Sleep -Milliseconds 15};throw 'Status unavailable'}
function O($r,$mode,$arg=0){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};$code=$LASTEXITCODE;if($code){return [pscustomobject]@{readValid=$false;exitCode=$code}};return($raw|ConvertFrom-Json)}
function Q($r,$mode,$arg=0){for($a=0;$a-lt8;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read $mode failed"}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid;I $r
  foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_potion_fixture off','coop_bendtime_fixture off','coop_darkvision_fixture off','coop_mesmerize_fixture off','coop_domino_fixture off','coop_swarm_fixture off','coop_possession_fixture off','coop_movement_fixture off'){C $r $cmd}
  C $r 'coop_movement_fixture 60000'
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Clean living fixture required'}
  $inventory[$r]=Q $r '--player-inventory';$inventory[$r]|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-before-$r.json"
 }
 $preflight=Q host '--watch-possession-state' 100
 $preflight|ConvertTo-Json -Depth 8|Set-Content "$out/probe-preflight.json"
 if(!$preflight.Count){throw 'Read-only target watcher preflight emitted no records'}
 $dirty=$true
 $isolationWait=[Diagnostics.Stopwatch]::StartNew()
 do{$isolation=S host '.observer-isolation.json';if($isolation.ready-and$isolation.movementFixture){break};if(!$isolation.active){break};Start-Sleep -Milliseconds 30}while($isolationWait.ElapsedMilliseconds-lt2500)
 $isolation|ConvertTo-Json -Depth 8|Set-Content "$out/acquisition-isolation.json"
 if(!$isolation.ready-or!$isolation.movementFixture-or$isolation.remainingMs-lt45000){throw "Native AI isolation not ready before source placement: $($isolation.result)"}
 # Target-relative source-only placement after native AI references are held.
 # Never move the NPC. Each proposed floor is independently measured; failed
 # floor probes return immediately to the previously verified balcony floor.
 $candidate=@((Q host '--world-npcs').npcs|Where-Object {$_.spawnerName-eq'ld_spawner_guard_tutorial_2'-and$_.unique-and$_.reciprocal-and$_.definition-eq'models/characters/def/generic/dunwall_guard_male_small_01.def'})
 if($candidate.Count-ne1-or[Math]::Abs([double]$candidate[0].position[2]-66.4)-gt.5){throw 'Exact tutorial guard2 not on upper floor'}
 $floorAttempts=[Collections.Generic.List[object]]::new();$floorFound=$false
 foreach($offset in @(@(0.0,-3.0),@(-3.0,0.0),@(3.0,0.0),@(0.0,3.0))){
  $sourceX=[double]$candidate[0].position[0]+$offset[0];$sourceY=[double]$candidate[0].position[1]+$offset[1]
  C host ('setviewpos {0} {1} 68.1 90 0' -f $sourceX.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$sourceY.ToString('F5',[Globalization.CultureInfo]::InvariantCulture))
  Start-Sleep -Milliseconds 700
  $ground=Q host '--player-states';$health=Q host '--health' 1
  $floorAttempts.Add([pscustomobject]@{sourceX=$sourceX;sourceY=$sourceY;ground=$ground;health=$health.current})
  if($health.current-ne56){throw 'Source placement changed health'}
  if($ground.motionValid-and$ground.motion-eq0-and$ground.stance-eq0-and$ground.defaultMode-and[Math]::Abs([double]$ground.position[2]-66.4)-lt.1-and[Math]::Abs([double]$ground.position[0]-$sourceX)-lt.08-and[Math]::Abs([double]$ground.position[1]-$sourceY)-lt.08){$floorFound=$true;break}
  C host 'setviewpos -48.59 -155.07 68.1 90 0'
 }
 $floorAttempts|ConvertTo-Json -Depth 10|Set-Content "$out/source-floor-attempts.json"
 if(!$floorFound){throw 'No verified nearby native source floor; NPC retained untouched'}
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch $Pitch -Evidence "$out/source-camera.json" -Quick
 $ground|ConvertTo-Json -Depth 10|Set-Content "$out/source-grounded.json"
 $before=Q host '--player-possession';$before|ConvertTo-Json -Depth 8|Set-Content "$out/before.json"
 if($before.quantity-or$before.equipped-or$before.started){throw 'Possession already owned or active'}
 $manaBefore=& "$PSScriptRoot/Read-NativeMana.ps1" -Role host -Evidence "$out/mana-before.json"
 C host 'coop_possession_fixture human40000';$w=[Diagnostics.Stopwatch]::StartNew()
 do{$f=S host '.possession-fixture.json';if($f.result-eq'native_possession_ready'){break};Start-Sleep -Milliseconds 60}while($w.ElapsedMilliseconds-lt6500)
 if($f.result-ne'native_possession_ready'){
  $f|ConvertTo-Json -Depth 8|Set-Content "$out/acquisition-fixture.json"
  $inv=Q host '--player-inventory';$inv|ConvertTo-Json -Depth 10|Set-Content "$out/acquisition-inventory.json"
  $item=@($inv.items|Where-Object name -eq 'models/powers/possession/possession.inventoryitem')
  if($item.Count-eq1-and$item[0].component-ne'0x0'){Q host '--object' $item[0].component|ConvertTo-Json -Depth 12|Set-Content "$out/acquisition-component.json";Q host '--components' $item[0].handle|ConvertTo-Json -Depth 12|Set-Content "$out/acquisition-components.json"}
  throw "Native Possession acquisition failed: $($f.result)"
 }
 $power=Q host '--player-possession';$power|ConvertTo-Json -Depth 8|Set-Content "$out/enabled.json"
 if(!$power.instantiated-or!$power.started-or!$power.equipped-or$power.quantity-ne1){throw 'Exact native Possession item not ready'}
 $checks.Add('Exact Corvo Possession item acquired through native inventory API; no direct spawn, AI or damage invocation')
 $upgrade=@((Q host '--player-inventory').items|Where-Object name -eq 'components/characters/player/base/attributes/possession_parasite.inventoryitem')
 if($upgrade.Count-ne1-or$upgrade[0].quantity-ne1-or$upgrade[0].type-ne20){throw 'Exact human-Possession upgrade not acquired'}
 $upgrade|ConvertTo-Json -Depth 8|Set-Content "$out/human-upgrade.json"
 $catalog=Q host '--world-npcs';$catalog|ConvertTo-Json -Depth 8|Set-Content "$out/aim-npc-catalog.json"
 $eligible=@($catalog.npcs|Where-Object {$_.unique-and$_.reciprocal-and$_.spawnerName-eq'ld_spawner_guard_tutorial_2'-and$_.definition-eq'models/characters/def/generic/dunwall_guard_male_small_01.def'-and[Math]::Abs([double]$_.position[2]-66.4)-lt.5-and[Math]::Pow([double]$_.position[0]-$sourceX,2)+[Math]::Pow([double]$_.position[1]-$sourceY,2)-lt49}|Sort-Object {[Math]::Pow([double]$_.position[0]-$sourceX,2)+[Math]::Pow([double]$_.position[1]-$sourceY,2)})
 if(!$eligible.Count){throw 'No verified upper-floor tutorial guard within seven metres'}
 $target=$eligible[0];$yaw=[Math]::Atan2([double]$target.position[1]-$sourceY,[double]$target.position[0]-$sourceX)*180/[Math]::PI
 [ordered]@{target=$target;yaw=$yaw;placement='Native source floor verified after target-relative placement; NPC untouched'}|ConvertTo-Json -Depth 8|Set-Content "$out/aim-guard.json"
 Q host '--components' $target.handle|ConvertTo-Json -Depth 12|Set-Content "$out/target-components-before.json"
 $dx=$sourceX-[double]$target.position[0];$dy=$sourceY-[double]$target.position[1];$length=[Math]::Sqrt($dx*$dx+$dy*$dy)
 if($length-lt.1){throw 'Target too close for deterministic placement'}
 $yaw=[Math]::Atan2(-$dy,-$dx)*180/[Math]::PI
 C host ('setviewpos {0} {1} 68.1 {2} 0' -f $sourceX.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$sourceY.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$yaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
 $observerAttempts=[Collections.Generic.List[object]]::new();$observerFound=$false
 foreach($offset in @(@(0.0,-1.5),@(-1.5,0.0),@(1.5,0.0),@(0.0,1.5),@(0.0,-3.0),@(-3.0,0.0),@(3.0,0.0))){
  $observerX=$sourceX+$offset[0];$observerY=$sourceY+$offset[1]
  if([Math]::Pow([double]$target.position[0]-$observerX,2)+[Math]::Pow([double]$target.position[1]-$observerY,2)-lt3.24){continue}
  $observerYaw=[Math]::Atan2([double]$target.position[1]-$observerY,[double]$target.position[0]-$observerX)*180/[Math]::PI
  C client ('setviewpos {0} {1} 68.1 {2} 0' -f $observerX.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$observerY.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$observerYaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
  Start-Sleep -Milliseconds 650
  $observerGround=Q client '--player-states';$observerHealth=Q client '--health' 1
  $observerAttempts.Add([pscustomobject]@{requested=@($observerX,$observerY);actual=$observerGround;health=$observerHealth.current})
  if($observerHealth.current-ne56){throw 'Observer floor test changed health'}
  if([Math]::Pow([double]$target.position[0]-[double]$observerGround.position[0],2)+[Math]::Pow([double]$target.position[1]-[double]$observerGround.position[1],2)-ge3.24-and$observerGround.defaultMode-and$observerGround.motionValid-and$observerGround.motion-eq0-and$observerGround.stance-eq0-and[Math]::Abs([double]$observerGround.position[2]-66.4)-lt.1){$observerFound=$true;$observerX=[double]$observerGround.position[0];$observerY=[double]$observerGround.position[1];break}
  C client 'setviewpos -44.39 -153.27 68.1 180 0'
 }
 $observerAttempts|ConvertTo-Json -Depth 10|Set-Content "$out/observer-floor-attempts.json"
 if(!$observerFound){throw 'No independently verified close observer floor'}
 $observerYaw=[Math]::Atan2([double]$target.position[1]-$observerY,[double]$target.position[0]-$observerX)*180/[Math]::PI
 C client ('setviewpos {0} {1} 68.1 {2} 0' -f $observerX.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$observerY.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$observerYaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))

 Start-Sleep -Milliseconds 650
 $observerFinal=Q client '--player-states';$observerFinal|ConvertTo-Json -Depth 8|Set-Content "$out/observer-final-floor.json"
 if(!$observerFinal.defaultMode-or$observerFinal.stance-ne0-or!$observerFinal.motionValid-or$observerFinal.motion-ne0-or[Math]::Abs([double]$observerFinal.position[2]-66.4)-gt.1){throw 'Camera yaw reframe changed verified observer floor'}
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch 0 -Evidence "$out/observer-camera.json" -Quick
 $placed=Q host '--player-states';$placed|ConvertTo-Json -Depth 8|Set-Content "$out/source-target-relative.json"
 if(!$placed.defaultMode-or$placed.stance-ne0-or[Math]::Abs([double]$placed.position[2]-66.4)-gt.1){throw 'Target-relative placement is not on verified floor'}
 $pin=Get-Content "$PSScriptRoot/possession-bank-manifest-v334.json" -Raw|ConvertFrom-Json|Where-Object role -eq 'corvo'
 if((Get-FileHash "$PSScriptRoot/possession-banks-v334/corvo-possession-bank.bin").Hash-ne$pin.sha256){throw 'Release bank hash differs'}
 Copy-Item "$PSScriptRoot/possession-banks-v334/corvo-possession-bank.bin" "$root/work/lab-instances/client/coop-status.json.movement.bin"
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 600;C client 'coop_movement_bank on'
 $w.Restart();do{$m=S client '.movement.json';$stream=S client '.shadow-stream.json';if($m.active-and$m.version-eq52-and$m.weight-gt.9-and$m.presentedDrawn-and$stream.ready-and$stream.armed){break};Start-Sleep -Milliseconds 50}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne52-or$m.weight-lt.9-or!$stream.ready-or!$stream.armed){[ordered]@{movement=$m;stream=$stream;sourceStream=(S host '.shadow-stream.json');observer=(Q client '--player-states');source=(Q host '--player-states');isolation=(S client '.observer-isolation.json')}|ConvertTo-Json -Depth 12|Set-Content "$out/preflight-failure.json";throw 'Release bank/native Steam stream not ready'}
 $sourceFxBefore=S host '.possession-fx.json';$peerFxBefore=S client '.possession-fx.json'
 [ordered]@{source=$sourceFxBefore;peer=$peerFxBefore}|ConvertTo-Json -Depth 6|Set-Content "$out/network-fx-before.json"
 $visibilityBefore=S client '.possession-body.json';$visibilityBefore|ConvertTo-Json|Set-Content "$out/visibility-before.json"
 $checks.Add('Pinned bank52/316 and protocol50 native Steam stream ready;315 prior clip payloads retained')

 $checks.Add('Exact human-upgrade native quantity acquired and stable reciprocal tutorial guard selected by read-only world identity')

 $hwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64();if(!$hwnd){throw 'Source window missing'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','11','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take-source.mp4")
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();$capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 $peerHwnd=(Get-Process -Id $pids.client).MainWindowHandle.ToInt64();if(!$peerHwnd){throw 'Observer window missing'}
 $argIndex=[Array]::IndexOf($args,($args|Where-Object {$_ -like 'gfxcapture=*'}));if($argIndex-lt0){throw 'Capture argument absent'}
 $args[$argIndex]="gfxcapture=hwnd=${peerHwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra";$args[-1]="$PSScriptRoot/$Take.mp4"
 $peerCapture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture-peer.log" -RedirectStandardOutput "$out/capture-peer.stdout" -PassThru
 Start-Sleep -Milliseconds 600;if($capture.HasExited){throw 'Source capture failed'}
 $monitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.host,'--watch-midnight','10000') -WindowStyle Hidden -RedirectStandardOutput "$out/midnight.ndjson" -RedirectStandardError "$out/midnight.stderr" -PassThru
 $ownedMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.host,'--watch-possession-scenes','10000') -WindowStyle Hidden -RedirectStandardOutput "$out/owned-scenes.ndjson" -RedirectStandardError "$out/owned-scenes.stderr" -PassThru
 $targetMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.host,'--watch-possession-state','10000') -WindowStyle Hidden -RedirectStandardOutput "$out/target-state.ndjson" -RedirectStandardError "$out/target-state.stderr" -PassThru
 $fxMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.host,'--watch-possession-effects','10000') -WindowStyle Hidden -RedirectStandardOutput "$out/possession-effects.ndjson" -RedirectStandardError "$out/possession-effects.stderr" -PassThru
 $w.Restart();$cast=$false;$exitSent=$false;$returnFramed=$false;$moveSent=$false;$returnBeforeMove=$null
 while($w.ElapsedMilliseconds-lt9000){
  if(!$cast-and$w.ElapsedMilliseconds-ge1100){
   $fresh=@((Q host '--world-npcs').npcs|Where-Object {$_.handle-eq$target.handle-and$_.spawnerName-eq$target.spawnerName-and$_.reciprocal-and$_.unique})
   if($fresh.Count-ne1){throw 'Original guard identity changed before cast'}
   if([Math]::Abs([double]$fresh[0].position[2]-66.4)-gt.5){throw 'Verified target left the measured test floor'}
   $yaw=[Math]::Atan2([double]$fresh[0].position[1]-$sourceY,[double]$fresh[0].position[0]-$sourceX)*180/[Math]::PI
   C host ('setviewpos {0} {1} 68.1 {2} 0' -f $sourceX.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$sourceY.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$yaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
   $fresh[0]|ConvertTo-Json -Depth 8|Set-Content "$out/aim-guard-at-cast.json"
   I host 255 1000;$cast=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native Corvo LT1000 then release at freshly revalidated human target'})}
  if($cast-and!$exitSent-and$w.ElapsedMilliseconds-ge5200){$exitState=Q host '--player-states';if($exitState.stance-ne18-or$exitState.defaultMode){throw 'No occupied-host mode before leave-host input'};I host 255 120;$exitSent=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native LT120 to leave host body'})}
  if($exitSent-and!$returnFramed-and$w.ElapsedMilliseconds-ge6500){
   $returned=O host '--player-states'
   if($returned.defaultMode-and$returned.stance-eq0){
    # Source floor measured in this take; camera-only reframing.
    $returnYaw=[Math]::Atan2([double]$returned.position[1]-$sourceY,[double]$returned.position[0]-$sourceX)*180/[Math]::PI
    C client ('setviewpos {0} {1} 68.1 {2} 0' -f $sourceX.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$sourceY.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$returnYaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
    [ordered]@{source=$returned;observer=(Q client '--player-states');nativeCamera=(Q client '--player');visibility=(S client '.possession-body.json')}|ConvertTo-Json -Depth 8|Set-Content "$out/return-camera.json"
    $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Observer camera-only reframe from previously measured floor after native exit'});$returnFramed=$true
   }
  }
  if($returnFramed-and!$moveSent-and$w.ElapsedMilliseconds-ge7200){
   $returnBeforeMove=Q host '--player-states'
   if(!(S client '.shadow-stream.json').ready-or(S client '.possession-body.json').hidden){throw 'Possession exit did not retain live visible peer stream'}
   & "$lab/Send-D2Input.ps1" -Role host -Button Neutral -LeftY -18000 -DurationMs 500|Out-Null
   $moveSent=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native half-second backward walk after host exit; no stream rearm'})
  }
  $rows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;fixture=(S host '.possession-fixture.json');player=(O host '--player-states');power=(O host '--player-possession');playerAfter=(O host '--player-states');isolation=(S host '.observer-isolation.json');movement=(S client '.movement.json');stream=(S client '.shadow-stream.json');sourceStream=(S host '.shadow-stream.json');visibility=(S client '.possession-body.json');sourceFx=(S host '.possession-fx.json');peerFx=(S client '.possession-fx.json')})
  Start-Sleep -Milliseconds 30
 }
 if(!$returnFramed){throw 'Native exit return not reframed for visual review'}
 $recovered=@($rows|Where-Object {$_.ms-ge6300})
 $dual=@($rows|Where-Object {$_.player.equipmentSceneValid-and($_.player.active.id-contains64)})
 $equipped=@($rows|Where-Object {$_.ms-gt5200-and$_.movement.equipmentVariant-eq1-and$_.movement.equipmentWeight-gt.3})
 $walk=@($rows|Where-Object {$_.ms-ge7200-and$_.movement.weight-gt.9-and$_.movement.speed-gt.15})
 $finalMove=Q host '--player-states';$distance=if($returnBeforeMove){[Math]::Sqrt([Math]::Pow($finalMove.position[0]-$returnBeforeMove.position[0],2)+[Math]::Pow($finalMove.position[1]-$returnBeforeMove.position[1],2))}else{0}
 [ordered]@{recoveredRows=$recovered.Count;dualOwnedSamples=$dual.Count;remoteDrawSamples=$equipped.Count;walkSamples=$walk.Count;distance=$distance;before=$returnBeforeMove;after=$finalMove;sourceStream=(S host '.shadow-stream.json');peerStream=(S client '.shadow-stream.json')}|ConvertTo-Json -Depth 12|Set-Content "$out/exit-continuity.json"
 if($recovered.Count-lt3-or@($recovered|Where-Object {!$_.stream.ready-or!$_.sourceStream.ready-or$_.visibility.hidden}).Count){throw 'Native exit retired stream or retained hidden body'}
 if(!$dual.Count-or!$equipped.Count-or!$moveSent-or!$walk.Count-or$distance-lt.15){throw 'Owned dual draw or subsequent native movement was not demonstrated'}
 $checks.Add('Native paired sword1066/power333 equip scenes are owned, remote draw plays, both Steam streams stay ready through exit and native backward walk resumes without rearm')

 I host;$manaAfter=& "$PSScriptRoot/Read-NativeMana.ps1" -Role host -Evidence "$out/mana-after.json"
 $monitor.WaitForExit(4000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){throw 'Raw Midnight watcher failed'}
 $samples=@(Get-Content "$out/midnight.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $scenes=@($samples|ForEach-Object instances|Where-Object {$_.state-eq3-and$_.asset-like'*power_possession_cast*'})
 if(!$scenes.Count){throw 'No native Possession cast scene observed'}
 $ownedMonitor.WaitForExit(4000)|Out-Null;if(!$ownedMonitor.HasExited-or$ownedMonitor.ExitCode){throw 'Strict Possession watcher incomplete'}
 $owned=@(Get-Content "$out/owned-scenes.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $accepted=@($owned|Where-Object valid)
 if(!$accepted.Count){throw 'No strict pre-transfer Possession phase observed'}
 $accepted|ConvertTo-Json -Depth 8|Set-Content "$out/accepted-scenes.json"
 if(@($accepted|Where-Object {$_.variant-eq28-and$_.durationMs-eq400-and($_.activeStates-join',')-eq'26'}).Count-lt2){throw 'Strict successful release with exact target and EnterPossessedHost26 not repeatedly observed'}
 $accepted|ConvertTo-Json -Depth 8|Set-Content "$out/accepted-scenes.json"
 $incomplete=@($scenes|Where-Object {$_.bindingsResearch.Count-ne1-or@($_.bindingsResearch|Where-Object {!$_.reciprocal}).Count})
 [ordered]@{rawActive=$scenes.Count;incompleteRaw=$incomplete.Count;strictAccepted=$accepted.Count;strictRejected=($owned.Count-$accepted.Count);scope='Raw observations may catch incomplete loop initialization. Only double-validated arm/equipment scenes are accepted.'}|ConvertTo-Json|Set-Content "$out/reader-summary.json"
 $checks.Add('Strict probe observes entry/aim and successful release400ms with EnterPossessedHost26/internal0 and exact NPC target')
 $sent=@($rows.sourceStream.powerSentHistory|Where-Object variant -eq 28|Sort-Object event -Unique)
 $received=@($rows.stream.powerReceivedHistory|Where-Object variant -eq 28|Sort-Object event -Unique)
 $presented=@($rows|Where-Object {$_.movement.powerVariant-eq28-and$_.movement.powerWeight-gt.5-and!$_.movement.presentedDrawn})
 [ordered]@{sent=$sent;received=$received;presented=$presented.Count;strictHandles=@($accepted|Where-Object variant -eq 28|ForEach-Object handle|Sort-Object -Unique)}|ConvertTo-Json -Depth 8|Set-Content "$out/release-network-evidence.json"
 if(!$sent.Count-or!$received.Count-or!$presented.Count){throw 'Native release not sent, accepted and visibly posed with holstered sword'}
 if(Compare-Object @($sent|ForEach-Object {"$($_.event):$($_.variant):$($_.durationMs)"}) @($received|ForEach-Object {"$($_.event):$($_.variant):$($_.durationMs)"})){throw 'Release source/peer event identities differ'}
 if(Compare-Object @($sent.nativeScene|Sort-Object -Unique) @($accepted|Where-Object variant -eq 28|ForEach-Object handle|Sort-Object -Unique)){throw 'Release native scene incarnation differs from sender'}
 $checks.Add('Exact native400ms release scene reaches peer over Steam and drives bank52 full-body release with holstered sword')
 $hidden=@($rows|Where-Object {$_.stream.remoteState-eq17-and$_.visibility.active-and$_.visibility.hidden})
 $visibilityAfter=S client '.possession-body.json';$visibilitySource=S host '.possession-body.json'
 [ordered]@{before=$visibilityBefore;after=$visibilityAfter;source=$visibilitySource;hiddenSamples=$hidden.Count;hiddenRows=$hidden}|ConvertTo-Json -Depth 16|Set-Content "$out/visibility-evidence.json"
 if($hidden.Count-lt3-or$visibilityAfter.active-or$visibilityAfter.restoring-or$visibilityAfter.hidden-or
    $visibilityAfter.hideCalls-$visibilityBefore.hideCalls-ne1-or$visibilityAfter.showCalls-$visibilityBefore.showCalls-ne1-or
    $visibilityAfter.lost-ne$visibilityBefore.lost-or$visibilityAfter.refused-ne$visibilityBefore.refused-or$visibilitySource.hideCalls){throw 'Remote Corvo body hide/restore ownership or continuity failed'}
 $checks.Add('Native transfer/occupied context maintains fresh Corvo-only semantic17; exactly one native clone hide and show, with verified restoration and no local-player/Emily visibility mutation')


 [ordered]@{scenes=@($scenes|Group-Object asset,subscene|ForEach-Object {[ordered]@{scene=$_.Name;samples=$_.Count;durationMs=@($_.Group.sceneDurationMs|Sort-Object -Unique)}});manaBefore=$manaBefore.currentCandidate;manaAfter=$manaAfter.currentCandidate;nativeStateSets=@($rows|ForEach-Object {($_.player.active.id|Sort-Object)-join','}|Sort-Object -Unique);observedEntityDefinitions=@($rows.entities.definitionName|Sort-Object -Unique);scope='Source-only native phase/lifecycle observation in isolated AI-held Dunwall; no remote gameplay or body binding'}|ConvertTo-Json -Depth 12|Set-Content "$out/possession-observations.json"
 $checks.Add('Native input produces reciprocal Possession cast scenes; observed phase set recorded without assuming complete coverage')
 $fxMonitor.WaitForExit(4000)|Out-Null;if(!$fxMonitor.HasExited-or$fxMonitor.ExitCode){throw 'Read-only Possession FX watcher incomplete'}
 $fx=@(Get-Content "$out/possession-effects.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $fxValid=@($fx|Where-Object valid);$effectNames=@($fxValid.effects.name|Sort-Object -Unique)
 [ordered]@{samples=$fx.Count;valid=$fxValid.Count;unknown=($fx.Count-$fxValid.Count);names=$effectNames;maxReadMs=($fx.readMs|Measure-Object -Maximum).Maximum;scope='Read-only source FX inventory and candidate controller-slot correlation; no remote playback or game-hook scan'}|ConvertTo-Json -Depth 6|Set-Content "$out/fx-research-summary.json"
 if(!$fxValid.Count-or!$effectNames.Count){throw 'No stable native Possession effects observed'}
 $checks.Add('Read-only source Possession FX records captured with target-context stability and candidate controller slots; remote effect verified separately')
 $scoped=@($fx|Where-Object {$_.scoped.valid-and$_.scoped.present})
 $matched=@($scoped|Where-Object {$r=$_; $r.valid-and@($r.effects|Where-Object {$_.id-eq$r.scoped.id-and$_.owner-eq$r.scoped.owner-and$_.name-eq'models/effects/gameplay/powers/possession/hosts_target/possession_target_human_01.fx'}).Count-eq1})
 [ordered]@{samples=$fx.Count;valid=@($fx|Where-Object {$_.scoped.valid}).Count;selected=@($fx|Where-Object {$_.scoped.valid-and$_.scoped.selected}).Count;present=$scoped.Count;fullScanMatched=$matched.Count;maxScopedMs=($fx.scoped.ms|Measure-Object -Maximum).Maximum;stages=@($fx|Group-Object {$_.scoped.stage}|Select-Object Name,Count);scope='Read-only exact selected human NPC component and native FX bucket lookup; no remote invocation'}|ConvertTo-Json -Depth 6|Set-Content "$out/scoped-fx-summary.json"
 if($scoped.Count-lt3-or$matched.Count-lt3){throw 'Scoped human target FX ownership not demonstrated'}
 $checks.Add('Scoped selected NPC generation, exact declaration, temporary FX actor and bucket lookup match independently inventoried native human target particles')
 $sourceFx=S host '.possession-fx.json';$peerFx=S client '.possession-fx.json'
 $activeFx=@($rows|Where-Object {$_.peerFx.sourceId-ne0})
 [ordered]@{source=$sourceFx;peer=$peerFx;activeSamples=$activeFx.Count;sourceIds=@($activeFx.peerFx.sourceId|Sort-Object -Unique);nativeIds=@($activeFx.peerFx.nativeId|Sort-Object -Unique)}|ConvertTo-Json -Depth 8|Set-Content "$out/network-fx-summary.json"
 if($sourceFx.sent-$sourceFxBefore.sent-lt3-or$peerFx.received-$peerFxBefore.received-lt3-or$peerFx.started-$peerFxBefore.started-ne1-or$peerFx.stopped-$peerFxBefore.stopped-ne1-or$peerFx.refused-ne$peerFxBefore.refused-or$peerFx.retired-ne$peerFxBefore.retired-or$peerFx.sourceId-or$activeFx.Count-lt2){throw 'Remote native target particle lifecycle not demonstrated'}
 $checks.Add('Exact native human target FX travels through Steam; one native-derived no-rumble particle effect starts on owned Corvo presentation, updates and retires without NPC gameplay invocation')



 $scenes|ConvertTo-Json -Depth 16|Set-Content "$out/possession-scenes.json"
 $targetMonitor.WaitForExit(4000)|Out-Null;if(!$targetMonitor.HasExited-or$targetMonitor.ExitCode){throw 'Possession target watcher failed'}
 $targets=@(Get-Content "$out/target-state.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $occupiedRows=@($rows|Where-Object {$_.player.stance-eq18-and!$_.player.defaultMode-and$_.playerAfter.stance-eq18-and!$_.playerAfter.defaultMode-and$_.power.readOnly})
 $transitionReads=@($rows|Where-Object {$_.player.stance-eq18-and(!$_.power.readOnly-or$_.playerAfter.stance-ne18)})
 [ordered]@{stableOccupied=$occupiedRows.Count;transitionReads=$transitionReads.Count;scope='Independent reads are bracketed by occupied player snapshots; invalid transition reads are unknown, never zero quantity or proof of revocation'}|ConvertTo-Json|Set-Content "$out/occupied-read-stability.json"
 if($occupiedRows.Count-lt3-or@($occupiedRows|Where-Object {!$_.visibility.hidden-or!$_.visibility.active-or!$_.stream.ready-or$_.stream.remoteState-ne17}).Count){throw 'Remote body visible or stream stale during bracketed native human occupation'}
 $checks.Add('All bracketed stable occupied-player observations retain hidden clone and fresh semantic17; transition read gaps reported separately')
 if(!$occupiedRows.Count-or@($occupiedRows|Where-Object {$_.power.quantity-ne1-or!$_.fixture.active}).Count){throw 'Fixture revoked acquisition while native host was occupied'}
 $checks.Add('Native acquired power retained throughout occupied-host samples; inventory cleanup waits for verified native exit')
 $validTargets=@($targets|Where-Object {$_.valid-and$_.hasTarget})
 $expected=[Convert]::ToUInt32($target.handle.Substring(2),16)
 $possessed=@($validTargets|Where-Object {$_.internalState-ne14-and$_.handle-eq$expected-and$_.physicsStance-eq18-and!$_.defaultMode})
 [ordered]@{ownedTargetSamples=$validTargets.Count;occupiedInternalStateSamples=$possessed.Count;targetHandle=$expected;internalStates=@($targets|Where-Object valid|ForEach-Object internalState|Sort-Object -Unique);manaBefore=$manaBefore.currentCandidate;manaAfter=$manaAfter.currentCandidate;strictPhases=@($accepted.variant|Sort-Object -Unique);nativeScenes=@($scenes.asset|Sort-Object -Unique);scope='Internal state14 is the inactive sentinel per824e40, not a possessed state. Physics stance18 and active-state IDs are separate fields; target weak reference validates generation, exact NPCPossession RTTI, entity handle and component membership'}|ConvertTo-Json -Depth 8|Set-Content "$out/target-observation-summary.json"
 if($possessed.Count-lt3-or$manaAfter.currentCandidate-ge$manaBefore.currentCandidate){throw 'Owned human-target internal state/mana evidence not established'}
 if(@($validTargets|Where-Object handle -ne $expected).Count){throw 'Foreign human target observed'}
 if(@($scenes|Where-Object {$_.asset-like'*power_possession_cast_out.midnightscene'-and$_.sceneDurationMs-eq400}).Count-lt2){throw 'Native successful release400ms not repeatedly observed'}
 $final=Q host '--player-states';$final|ConvertTo-Json -Depth 8|Set-Content "$out/after-exit-player.json"
 if(!$final.defaultMode-or$final.stance-ne0){throw 'Native leave-host input did not recover grounded default mode'}
 $checks.Add('Exact human target ownership under active internal state/physics stance18, native release400ms and normal-input exit recovery observed')
 Q host '--player-inventory'|ConvertTo-Json -Depth 10|Set-Content "$out/inventory-before-restore.json"
 S host '.possession-fixture.json'|ConvertTo-Json -Depth 8|Set-Content "$out/fixture-before-restore.json"
 C host 'coop_possession_fixture off';$w.Restart()
 do{$f=S host '.possession-fixture.json';if(!$f.active){break};Start-Sleep -Milliseconds 70}while($w.ElapsedMilliseconds-lt6500)
 $f|ConvertTo-Json -Depth 8|Set-Content "$out/fixture-after-restore.json"
 if($f.active-or$f.result-ne'quantity_and_selection_restored'){Q host '--player-inventory'|ConvertTo-Json -Depth 10|Set-Content "$out/inventory-failed-restore.json";throw 'Native Possession inventory restoration unconfirmed'}
 foreach($r in 'host','client'){
  $after=Q $r '--player-inventory';$after|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-after-$r.json"
  $changes=@(foreach($old in $inventory[$r].items){
   $new=@($after.items|Where-Object name -eq $old.name);if($new.Count-ne1){throw 'Inventory identity changed'}
   if($old.quantity-ne$new[0].quantity){[pscustomobject]@{name=$old.name;before=$old.quantity;after=$new[0].quantity}}
  })
  if($after.items.Count-ne$inventory[$r].items.Count){throw 'Inventory item count changed'}
  $changes|ConvertTo-Json -Depth 6|Set-Content "$out/native-inventory-changes-$r.json"
  if($changes.Count){
   #330 observed this exact progression change with tutorial guard2. Its
   #cause remains unproven; allow only this bounded native-world delta here,
   #then require the entire baseline inventory after checkpoint restoration.
   if($r-ne'host'-or$target.spawnerName-ne'ld_spawner_guard_tutorial_2'-or$changes.Count-ne1-or
      $changes[0].name-ne'models/interactive/keys/dunwall/dunwall_key_emilyoffice.inventoryitem'-or$changes[0].before-ne0-or$changes[0].after-ne1){throw 'Unexpected native inventory change'}
   $checks.Add('Recorded exact native office-key0-to1 progression delta with tutorial guard2; causal attribution and peer progression remain unverified; complete checkpoint inventory rollback required')
  }

  $health=[ordered]@{player=(Q $r '--health' 1);partner=(Q $r '--health' (S $r).probeActorHandle)}
  $health|ConvertTo-Json -Depth 8|Set-Content "$out/health-after-$r.json"
  if($health.player.current-ne56-or$health.partner.current-ne40){throw "Player or partner health changed: $r player=$($health.player.current) partner=$($health.partner.current)"}
 }
 $checks.Add('Temporary power/upgrade and unrelated inventory quantities restored; player/partner health retained; any separately recorded native office-key delta awaits mandatory checkpoint reset')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_possession_fixture off'}catch{}}
 foreach($job in @($capture,$peerCapture,$monitor,$ownedMonitor,$targetMonitor,$fxMonitor)){if($job){$job.WaitForExit(18000)|Out-Null;if(!$job.HasExited-or$job.ExitCode){$failure=($failure+'; capture/watcher failed').TrimStart('; ')}}}
 foreach($r in 'host','client'){try{C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 22|Set-Content "$out/states.json"
 if($dirty){try{
  & "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log"
  foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'}}
  $p=Q host '--player-possession';if($p.quantity-or$p.equipped-or$p.started){throw 'Possession remains acquired after checkpoint'}
  $u=@((Q host '--player-inventory').items|Where-Object name -eq 'components/characters/player/base/attributes/possession_parasite.inventoryitem');if($u.Count-ne1-or$u[0].quantity){throw 'Human upgrade remains acquired after checkpoint'}
  foreach($r in 'host','client'){
   $restored=Q $r '--player-inventory';$restored|ConvertTo-Json -Depth 10|Set-Content "$out/inventory-checkpoint-restored-$r.json"
   if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($restored.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Complete baseline inventory did not restore from checkpoint'}
  }
  $checks.Add('Both isolated checkpoints restore complete original inventory quantities and health, clearing temporary Possession and upgrade acquisitions')
 }catch{$failure=($failure+'; checkpoint restore: '+$_.Exception.Message).TrimStart('; ')}}
 try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All14 protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
 [ordered]@{passed=(!$failure);failure=$failure;source='host';checks=$checks;events=$events;dll=$initialDll;probe=$initialProbe;scope='Native source Corvo Possession acquisition, controller input, raw scene clocks/state/equipment and candidate target/entity observations. AI held in the isolated Dunwall fixture; finite native human-upgrade and verified guard-target experiment; generation-validated human target, native internal state lifecycle, release400ms and normal-input exit investigated; Remote native successful release over protocol50/bank52 investigated. Human transfer/internal1 and occupation/internal8 maintain a finite cosmetic clone visibility lease; remote target control and shared gameplay remain unimplemented; scoped target particles use a native-derived asset through Steam. Body visibility uses native API on the exact held Corvo clone only; Corvo standing dual-equip exit and normal backward walk tested without rearm; interruption/alternate contexts pending. Checkpoint and protected save restoration mandatory.'}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_NATIVE_POSSESSION_OBSERVATION $($checks.Count) checks"



