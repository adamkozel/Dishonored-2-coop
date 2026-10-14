param([ValidatePattern('^[a-z0-9-]+$')][string]$Take,[ValidateRange(-15,45)][int]$Pitch=30)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(!$Take-or(Test-Path $out)){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
$pids=@{};$inventory=@{};$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$recorders=@();$capture=$null;$monitor=$null;$dirty=$false;$failure=$null
$initialDll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;$initialProbe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$lt=0,$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button Neutral -LeftTrigger $lt -DurationMs $ms|Out-Null}
function S($r,$suffix=''){for($a=0;$a-lt8;$a++){try{$v=Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json;if($null-ne$v){return $v}}catch{};Start-Sleep -Milliseconds 15};throw 'Status unavailable'}
function Q($r,$mode,$arg=0){for($a=0;$a-lt8;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read $mode failed"}
try{
 foreach($r in 'client','host'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid;I $r
  foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_potion_fixture off','coop_bendtime_fixture off','coop_darkvision_fixture off','coop_mesmerize_fixture off','coop_domino_fixture off','coop_swarm_fixture off','coop_possession_fixture off','coop_doppelganger_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Clean living fixture required'}
  $inventory[$r]=Q $r '--player-inventory';$inventory[$r]|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-before-$r.json"
 }
 $visualStart=S host '.doppelganger-visual.json';$visualStart|ConvertTo-Json -Depth 8|Set-Content "$out/visual-start.json"
 $dirty=$true
 C client 'setviewpos -48.59 -155.07 68.1 90 0';C host 'setviewpos -47.39 -156.87 68.1 90 0';
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch 10 -Evidence "$out/observer-camera.json" -Quick
Start-Sleep -Milliseconds 800
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch $Pitch -Evidence "$out/source-camera.json" -Quick
 $ground=Q client '--player-states';$ground|ConvertTo-Json -Depth 10|Set-Content "$out/source-grounded.json"
 if(!$ground.motionValid-or$ground.motion-ne0-or$ground.stance-ne0-or[Math]::Abs([double]$ground.position[2]-66.4370575)-gt.03){throw 'Emily is not grounded on measured test floor'}
 $before=Q client '--player-doppelganger';$before|ConvertTo-Json -Depth 8|Set-Content "$out/before.json"
 if($before.quantity-or$before.equipped-or$before.started){throw 'Doppelganger already owned or active'}
 $manaBefore=& "$PSScriptRoot/Read-NativeMana.ps1" -Role client -Evidence "$out/mana-before.json"
 C client 'coop_doppelganger_fixture 40000';$w=[Diagnostics.Stopwatch]::StartNew()
 do{$f=S client '.doppelganger-fixture.json';if($f.result-eq'native_doppelganger_ready'){break};Start-Sleep -Milliseconds 60}while($w.ElapsedMilliseconds-lt6500)
 if($f.result-ne'native_doppelganger_ready'){
  $f|ConvertTo-Json -Depth 8|Set-Content "$out/acquisition-fixture.json"
  $inv=Q client '--player-inventory';$inv|ConvertTo-Json -Depth 10|Set-Content "$out/acquisition-inventory.json"
  $item=@($inv.items|Where-Object name -eq 'models/powers/doppelganger/doppelganger.inventoryitem')
  if($item.Count-eq1-and$item[0].component-ne'0x0'){Q client '--object' $item[0].component|ConvertTo-Json -Depth 12|Set-Content "$out/acquisition-component.json";Q client '--components' $item[0].handle|ConvertTo-Json -Depth 12|Set-Content "$out/acquisition-components.json"}
  throw "Native Doppelganger acquisition failed: $($f.result)"
 }
 $power=Q client '--player-doppelganger';$power|ConvertTo-Json -Depth 8|Set-Content "$out/enabled.json"
 if(!$power.instantiated-or!$power.started-or!$power.equipped-or$power.quantity-ne1){throw 'Exact native Doppelganger item not ready'}
 $checks.Add('Exact Emily Doppelganger item acquired through native inventory API; no direct spawn, AI or damage invocation')
 Copy-Item "$PSScriptRoot/doppelganger-banks-v407/emily-doppelganger-bank.bin" "$root/work/lab-instances/host/coop-status.json.movement.bin"
 foreach($r in 'client','host'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 600;C host 'coop_movement_bank on'
 $w.Restart();do{$m=S host '.movement.json';$stream=S host '.shadow-stream.json';if($m.active-and$m.version-eq55-and$m.weight-gt.9-and$m.presentedDrawn-and$stream.ready-and$stream.armed){break};Start-Sleep -Milliseconds 50}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne55-or$m.weight-lt.9-or!$m.presentedDrawn-or!$stream.ready-or!$stream.armed){throw 'Exact Doppelganger bank/native Steam stream not ready'}
 $checks.Add('Remote Emily uses exact bank55/322 and native Steam pose transport')
 $hwnd=(Get-Process -Id $pids.client).MainWindowHandle.ToInt64();if(!$hwnd){throw 'Source window missing'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','34','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take-source.mp4")
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();$capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 $recorders+=$capture
 $hwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64();if(!$hwnd){throw 'Observer window missing'}
 $idx=[Array]::IndexOf($args,($args|Where-Object {$_ -like 'gfxcapture=*'}));if($idx-lt0){throw 'Capture argument absent'}
 $args[$idx]="gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra";$args[-1]="$PSScriptRoot/$Take.mp4"
 $recorders+=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture-peer.log" -RedirectStandardOutput "$out/capture-peer.stdout" -PassThru
 Start-Sleep -Milliseconds 600;if(@($recorders|Where-Object HasExited).Count){throw 'Source capture failed'}
 $monitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-doppelganger-scenes','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/power-scenes.ndjson" -RedirectStandardError "$out/power-scenes.stderr" -PassThru
 $fxMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-doppelganger-effects','30000') -WindowStyle Hidden -RedirectStandardOutput "$out/native-effects.ndjson" -RedirectStandardError "$out/native-effects.stderr" -PassThru
 $recorders+=$fxMonitor
 $midnightMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-midnight','30000') -WindowStyle Hidden -RedirectStandardOutput "$out/native-midnight.ndjson" -RedirectStandardError "$out/native-midnight.stderr" -PassThru
 $recorders+=$midnightMonitor
 $peerFxMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.host,'--watch-doppelganger-effects','30000') -WindowStyle Hidden -RedirectStandardOutput "$out/peer-effects.ndjson" -RedirectStandardError "$out/peer-effects.stderr" -PassThru
 $peerMidnightMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.host,'--watch-midnight','30000') -WindowStyle Hidden -RedirectStandardOutput "$out/peer-midnight.ndjson" -RedirectStandardError "$out/peer-midnight.stderr" -PassThru
 $recorders+=$peerFxMonitor;$recorders+=$peerMidnightMonitor

 Start-Sleep -Milliseconds 100;if($monitor.HasExited-or$fxMonitor.HasExited-or$midnightMonitor.HasExited-or$peerFxMonitor.HasExited-or$peerMidnightMonitor.HasExited){throw 'Strict Doppelganger watcher exited before input; see stderr'}
 $w.Restart();$cast=$false;$worldNext=4600;$worldSamples=[Collections.Generic.List[object]]::new();$cloneCaptured=$false;$cloneSceneCaptured=$false;$sceneNext=2900
 while($w.ElapsedMilliseconds-lt32000){
  if(!$cast-and$w.ElapsedMilliseconds-ge1100){I client 255 1800;$cast=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native Emily LT1800 then release at grounded floor'})}
  if($cast-and!$cloneSceneCaptured-and$w.ElapsedMilliseconds-ge$sceneNext-and$w.ElapsedMilliseconds-lt6500){
   $sceneNext=$w.ElapsedMilliseconds+80
   $sceneSnapshot=Q client '--midnight'
   $candidates=@($sceneSnapshot.instances|Where-Object {$_.asset-eq'midnight/gameplay/human/common/doppelganger/doppelganger_spawn.midnightscene'-and$_.state-eq3})
   if($candidates.Count-eq1){
    $cloneScene=$candidates[0];$cloneScene|ConvertTo-Json -Depth 10|Set-Content "$out/clone-spawn-scene.json"
    $raw=Q client '--object' $cloneScene.pointer;$raw|ConvertTo-Json -Depth 10|Set-Content "$out/clone-spawn-instance-object.json"
    $cloneSceneCaptured=$true
   }
  }
  if($w.ElapsedMilliseconds-ge$worldNext){
   $worldNext=$w.ElapsedMilliseconds+2000
   $se=Q client '--entities';$pe=Q host '--entities'
   $clones=@($se.entities|Where-Object {$_.definitionName-match'^models/powers/doppelganger/(decoy_entity|deadlyshadow_entity)(|_corvo)\.def$'})
   $peerClones=@($pe.entities|Where-Object {$_.definitionName-match'^models/powers/doppelganger/(decoy_entity|deadlyshadow_entity)(|_corvo)\.def$'})
   $obj=Q client '--object' $power.component
   $worldSamples.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;source=$clones;peer=$peerClones;equipment=$obj})
   if($clones.Count-eq1-and!$cloneCaptured){
    Q client '--components' $clones[0].handle|ConvertTo-Json -Depth 12|Set-Content "$out/native-clone-components.json"
    Q client '--animation' $clones[0].handle|ConvertTo-Json -Depth 12|Set-Content "$out/native-clone-animation.json"
    S client '.doppelganger-world.json'|ConvertTo-Json -Depth 12|Set-Content "$out/source-before-rig.json"
    $visual=S host '.doppelganger-visual.json';$visual|ConvertTo-Json -Depth 8|Set-Content "$out/visual-before-rig.json"
    if(!$visual.visible-or!$visual.handle){throw "Clone model not visible: $($visual.result)"}
    $components=Q host '--components' $visual.handle;$components|ConvertTo-Json -Depth 12|Set-Content "$out/visual-components.json"
    if($components.components.Count-ne4-or@($components.components|Where-Object {$_.type-match'(?i)(health|physics|decoy|aiDecision)'}).Count){throw 'Cosmetic clone has unexpected gameplay components'}
    $pose=$null
    for($attempt=0;$attempt-lt30;$attempt++){
     $raw=& "$lab/bin/d2_runtime_probe.exe" $pids.host '--doppelganger-skeleton-final' $visual.handle $visual.publication
     if(!$LASTEXITCODE){$pose=$raw|ConvertFrom-Json;if($pose.stable-and$pose.finalPublicationComplete-and$pose.cloneExpectedSequence-and$pose.cloneExpectedPoseError-lt.0001){break}}
     Start-Sleep -Milliseconds 10
    }
    $pose|ConvertTo-Json -Depth 16|Set-Content "$out/visual-final-skeleton.json"
    if($pose.cloneRig-ne'5921d3450a60d7e2'-or!$pose.stable-or!$pose.finalPublicationComplete-or$pose.joints.Count-ne219-or$pose.cloneExpectedPoseError-ge.0001){throw 'Complete219-joint native final pose not independently confirmed'}
    $checks.Add('Separate four-component Emily visual and all219 final native joints match the received/interpolated pose')
    $cloneCaptured=$true
   }
  }
  $m=S host '.movement.json';$stream=S host '.shadow-stream.json'
  $rows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;player=(Q client '--player-states');movement=$m;stream=$stream;cloneSource=(S client '.doppelganger-world.json');clonePeer=(S host '.doppelganger-world.json');cloneVisual=(S host '.doppelganger-visual.json')})
  if(!$m.active-or!$stream.ready-or!$stream.armed){throw "Native transport/presentation retired: $($stream.result)"}
  Start-Sleep -Milliseconds 30
 }
 I client;$manaAfter=& "$PSScriptRoot/Read-NativeMana.ps1" -Role client -Evidence "$out/mana-after.json"
 $monitor.WaitForExit(4000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){throw 'Raw Midnight watcher failed'}
 $samples=@(Get-Content "$out/power-scenes.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $scenes=@($samples|Where-Object valid)
 foreach($v in 29,30,31){
  if(!@($scenes|Where-Object variant -eq $v).Count){throw "Strict native Doppelganger phase $v absent"}
  if(!@($rows|Where-Object {$_.movement.powerVariant-eq$v-and$_.movement.powerWeight-gt.5}).Count){throw "Remote Doppelganger phase $v absent"}
 }
 if(@($scenes|Where-Object {!$_.armHandle-or!$_.equipmentHandle}).Count-or@($scenes.armHandle|Sort-Object -Unique).Count-ne1-or@($scenes.equipmentHandle|Sort-Object -Unique).Count-ne1){throw 'Source Doppelganger ownership changed'}
 $checks.Add('All three exact native phases with reciprocal Emily arm/body ownership drive remote full-body phases')
 $sourceStream=S client '.shadow-stream.json';$peerStream=S host '.shadow-stream.json'
 [ordered]@{source=$sourceStream;peer=$peerStream}|ConvertTo-Json -Depth 12|Set-Content "$out/power-event-history.json"
 $sent=@($sourceStream.powerSentHistory|Where-Object {$_.variant-in29,30,31})
 $received=@($peerStream.powerReceivedHistory|Where-Object {$_.variant-in29,30,31})
 if(!$sent.Count-or$sent.Count-ne$received.Count){throw 'Doppelganger sender/receiver retained event counts differ'}
 if(Compare-Object @($sent|ForEach-Object {"$($_.event):$($_.variant):$($_.durationMs)"}) @($received|ForEach-Object {"$($_.event):$($_.variant):$($_.durationMs)"})){throw 'Doppelganger accepted receiver events differ from sent events'}
 $phaseCoverage=@(foreach($v in 29,30,31){
  $nativeIds=@($scenes|Where-Object variant -eq $v|ForEach-Object handle|Sort-Object -Unique)
  $sourceEvents=@($sent|Where-Object variant -eq $v)
  $nativeSent=@($sourceEvents.nativeScene|Sort-Object -Unique)
  if(!$nativeIds.Count-or(Compare-Object $nativeIds $nativeSent)){throw "Phase $v strict-native scene IDs differ from actual sent scene IDs"}
  [pscustomobject]@{variant=$v;nativeHandles=$nativeIds;sourceEvents=$sourceEvents;acceptedEvents=@($received|Where-Object variant -eq $v);sampledPoseEvents=@($rows|Where-Object {$_.movement.powerVariant-eq$v-and$_.movement.powerWeight-gt.5}|ForEach-Object {$_.movement.powerEvent}|Sort-Object -Unique)}
 })
 $phaseCoverage|ConvertTo-Json -Depth 8|Set-Content "$out/phase-incarnations.json"
 $checks.Add('Bounded send/accept histories exactly match strict native scene incarnations, including sub-status-interval final aim loops')
 $continuity=@($rows|Where-Object {$_.ms-ge900-and$_.ms-le6200})
 if($continuity.Count-lt20-or@($continuity|Where-Object {$_.movement.weight-lt.95-or$_.movement.clockRejection.count-ne0}).Count){throw 'Doppelganger root/body continuity failed'}
 [ordered]@{samples=$continuity.Count;minimumBodyWeight=($continuity.movement.weight|Measure-Object -Minimum).Minimum;rejections=@($continuity|ForEach-Object {$_.movement.clockRejection.count}|Sort-Object -Unique)}|ConvertTo-Json|Set-Content "$out/continuity.json"
 $checks.Add('Doppelganger cast preserves root/body continuity without rejected presentation clocks')
 if($m.powerWeight-gt.02-or$m.weight-lt.9-or$stream.remoteState-ne0){throw 'Remote cast did not recover to standing'}
 $sword=Q host '--avatar-sword' (S host).probeActorHandle;$sword|ConvertTo-Json -Depth 8|Set-Content "$out/avatar-sword-after.json"
 if(!$sword.equipped-or!(Q client '--player-sword').equipped){throw 'Sword selection changed'}
 $checks.Add('Remote body recovers to standing with native sword selection retained')
 $sourceEntities=Q client '--entities';$peerEntities=Q host '--entities'
 $nativeClones=@($sourceEntities.entities|Where-Object {$_.definitionName-match'^models/powers/doppelganger/(decoy_entity|deadlyshadow_entity)(|_corvo)\.def$'})
 $peerClones=@($peerEntities.entities|Where-Object {$_.definitionName-match'^models/powers/doppelganger/(decoy_entity|deadlyshadow_entity)(|_corvo)\.def$'})
 [ordered]@{source=$nativeClones;peer=$peerClones;scope='Source native clones are observational; body playback must not duplicate gameplay spawns'}|ConvertTo-Json -Depth 10|Set-Content "$out/doppelganger-world-observation.json"
 $worldSamples|ConvertTo-Json -Depth 15|Set-Content "$out/clone-lifecycle-samples.json"
 if(!@($worldSamples|Where-Object {$_.source.Count-gt0}).Count-or@($worldSamples|Where-Object {$_.peer.Count-gt0}).Count-or$peerClones.Count){throw 'Native source clone never observed, or peer duplicated a clone'}
 $checks.Add('Native source clone observed during its lifetime; body playback creates no peer clones or duplicate power actions')
 if(!$cloneSceneCaptured-or$cloneScene.bindingsResearch.Count-ne1-or!$cloneScene.bindingsResearch[0].reciprocal-or$cloneScene.sceneDurationMs-ne1000){throw 'Native clone spawn scene binding/clock observation missing'}
 $nativeCloneHandles=@($worldSamples.source.handle|ForEach-Object {[Convert]::ToUInt32($_,16)}|Sort-Object -Unique)
 if([uint32]$cloneScene.bindingsResearch[0].handle-notin$nativeCloneHandles){throw 'Spawn scene actor differs from independently enumerated clone'}
 $checks.Add('Native1000ms clone spawn scene observed with reciprocal binding to independently enumerated source clone; raw instance retained for strict reader development')
 $midnightSamples=@(Get-Content "$out/native-midnight.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $fxScenes=@($midnightSamples.instances|Where-Object {$_.asset-eq'midnight/gameplay/human/common/doppelganger/doppelganger_spawn.midnightscene'-and$_.state-eq3})
 $resolved=@($fxScenes|Where-Object {$_.bindingsResearch.Count-eq1-and$_.bindingsResearch[0].reciprocal-and$_.bindingsResearch[0].fxResearch.valid-and@($_.bindingsResearch[0].fxResearch.tracks.records|Where-Object {$null-ne$_}).Count-eq5})
 if(!$resolved.Count){throw 'No stable reciprocal native five-helper FX track snapshot; research capture retained'}
 $proof=$resolved[0];$fxRecords=@($proof.bindingsResearch[0].fxResearch.tracks.records|Where-Object {$null-ne$_})
 if(@($fxRecords|Where-Object {!$_.keyResolved-or$_.helperType-ne'.?AVidEntity@@'}).Count-or@($fxRecords.helperHandle|Sort-Object -Unique).Count-ne5){throw 'Native key/helper identity unresolved'}
 $expectedSockets=@('decoy_root','sl_l_arm','sl_r_arm','decoy_spawn_l_hand','decoy_spawn_r_hand')|Sort-Object
 if(Compare-Object $expectedSockets ($fxRecords.socket|Sort-Object)){throw 'Native spawn sockets differ from exported exact scene'}
 $fxRows=@(Get-Content "$out/native-effects.ndjson"|ForEach-Object {$_|ConvertFrom-Json}|Where-Object valid)
 foreach($rec in $fxRecords){if(!@($fxRows.effects|Where-Object {$_.owner-eq$rec.helperHandle-and$_.name-eq$rec.effect}).Count){throw 'Track helper/effect absent from independent native FX map'}}
 $proof|ConvertTo-Json -Depth 16|Set-Content "$out/confirmed-fx-scene.json"
 $checks.Add('Native clone scene group and five FX keys resolve exact sockets and distinct helpers independently matched to native FX-map owners/declarations')
 $strict=@($fxRows|Where-Object {$_.worldStable-and$_.cloneScene.valid-and$_.cloneScene.variant})
 $coverage=@(foreach($variant in 1,2){
  $phase=@($strict|Where-Object {$_.cloneScene.variant-eq$variant})
  if($phase.Count-lt3){throw "Strict source clone scene variant $variant lacks three observations"}
  $nativeIds=@($midnightSamples.instances|Where-Object {$_.asset-eq$(if($variant-eq1){'midnight/gameplay/human/common/doppelganger/doppelganger_spawn.midnightscene'}else{'midnight/gameplay/human/common/doppelganger/doppelganger_dispawn.midnightscene'})-and$_.state-eq3}|ForEach-Object id|Sort-Object -Unique)
  $ids=@($phase.cloneScene.id|Sort-Object -Unique)
  if(Compare-Object $ids $nativeIds){throw 'Strict clone scene IDs differ from independent Midnight observer'}
  if(@($phase.cloneHandle|Sort-Object -Unique).Count-ne1-or$phase[0].cloneHandle-notin$nativeCloneHandles){throw 'Strict source scene changed native clone ownership'}
  foreach($row in $phase){if(@($row.cloneScene.helpers|Sort-Object -Unique).Count-ne5-or@($row.cloneScene.helpers|Where-Object {$_-notin$row.effects.owner}).Count){throw 'Strict helper handle missing from same-snapshot native FX map'}}
  [pscustomobject]@{variant=$variant;ids=$ids;samples=$phase.Count;firstMs=$phase[0].cloneScene.timeMs;lastMs=$phase[-1].cloneScene.timeMs;maxReadMs=($phase.cloneScene.readMs|Measure-Object -Maximum).Maximum}
 })
 $coverage|ConvertTo-Json -Depth 10|Set-Content "$out/strict-scene-coverage.json"
 $checks.Add('Strict source reader confirms both native spawn1000ms and dispawn466ms scene incarnations, clocks and all five helpers against independent Midnight and same-snapshot FX map')



 $sent=@($rows|ForEach-Object {$_.cloneSource.history}|Sort-Object sequence -Unique)
 $accepted=@($rows|ForEach-Object {$_.clonePeer.history}|Sort-Object sequence -Unique)
 $sentBySeq=@{};foreach($record in $sent){$sentBySeq[[string]$record.sequence]=$record}
 $live=@($accepted|Where-Object present -eq 1);$cleared=@($accepted|Where-Object present -eq 0)
 if($live.Count-lt20-or!$cleared.Count){throw 'Clone transport lacks20 live frames or authoritative clear'}
 foreach($record in $accepted){$source=$sentBySeq[[string]$record.sequence];if(!$source-or$record.digest-ne$source.digest){throw 'Accepted clone payload has no exact sender digest'}}
 $nativeHandles=@($worldSamples.source.handle|ForEach-Object {[Convert]::ToUInt32($_,16)}|Sort-Object -Unique)
 if(@($live.handle|Sort-Object -Unique).Count-ne1-or[uint32]$live[0].handle-notin$nativeHandles){throw 'Transport clone differs from separately enumerated native clone'}
 if($cleared[-1].event-le$live[-1].event-or$rows[-1].clonePeer.present-or$rows[-1].clonePeer.leased){throw 'Clone retirement did not clear receiver lease'}
 [ordered]@{sent=$sent.Count;accepted=$accepted.Count;live=$live.Count;clear=$cleared.Count;nativeHandles=$nativeHandles;generations=@($live.generation|Sort-Object -Unique);allAcceptedDigestsMatch=$true;sourceReadMaxMs=($rows.cloneSource.readMaxMs|Measure-Object -Maximum).Maximum;receiverRejected=@($rows.clonePeer.rejected|Sort-Object -Unique);scope='Native Steam clone identity, position and lifetime transport only. Receiver creates one cosmetic model; no effect, AI or damage; source handles remain opaque.'}|ConvertTo-Json -Depth 10|Set-Content "$out/clone-transport-verification.json"
 $checks.Add('Native clone identity/root/lifetime traverses Steam with exact accepted packet digests and authoritative receiver retirement')
 $visuals=@($rows.cloneVisual|Where-Object {$_.updated-gt0});$lastVisual=S host '.doppelganger-visual.json'
 $lastVisual|ConvertTo-Json -Depth 8|Set-Content "$out/visual-final.json"
 if(!$visuals.Count-or$lastVisual.created-$visualStart.created-ne1-or$lastVisual.shown-$visualStart.shown-ne1-or$lastVisual.hidden-$visualStart.hidden-ne1-or$lastVisual.visible-or$lastVisual.applied-$visualStart.applied-lt20-or$lastVisual.interpolated-$visualStart.interpolated-lt20-or($lastVisual.poseRefused-$visualStart.poseRefused)-or$lastVisual.maxTransformError-gt.001){throw 'Clone visible pose, interpolation, exact transform or retirement check failed'}
 $checks.Add('One isolated cosmetic clone follows native219-joint pose and interpolated root, then hides on source retirement')
 $peerMidnight=@(Get-Content "$out/peer-midnight.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $peerFxRows=@(Get-Content "$out/peer-effects.ndjson"|ForEach-Object {$_|ConvertFrom-Json}|Where-Object valid)
 $fxHistory=@($lastVisual.fx.history|Where-Object {$_.event-eq$live[-1].event})
 if($fxHistory.Count-ne2-or@($fxHistory|Where-Object {!$_.verified}).Count-or$lastVisual.fx.scene-or$lastVisual.fx.refused-$visualStart.fx.refused){throw "Remote effect start/cleanup refused: $($lastVisual.fx|ConvertTo-Json -Depth 6 -Compress)"}
 $fxEvidence=@(foreach($v in 1,2){
  $h=@($fxHistory|Where-Object variant -eq $v);if($h.Count-ne1-or[Math]::Abs([int]$h[0].elapsed-[int]$h[0].nativeTime)-gt35){throw 'Native remote effect clock mismatch or duplicate start'}
  $sourceIds=@($strict|Where-Object {$_.cloneScene.variant-eq$v}|ForEach-Object {$_.cloneScene.id}|Sort-Object -Unique)
  if($h[0].sourceScene-notin$sourceIds){throw 'Remote FX scene has no strict native source incarnation'}
  $resource=if($v-eq1){'midnight/d2coop/doppelganger_fx_spawn.midnightscene'}else{'midnight/d2coop/doppelganger_fx_despawn.midnightscene'}
  $shown=@($peerMidnight.instances|Where-Object {$_.id-eq$h[0].nativeScene-and$_.asset-eq$resource-and$_.state-eq3-and$_.bindingsResearch.Count-eq1-and$_.bindingsResearch[0].reciprocal-and$_.bindingsResearch[0].handle-eq$lastVisual.handle-and$_.bindingsResearch[0].fxResearch.valid})
  if(!$shown.Count){throw 'Remote cosmetic effect scene lacks independent native binding'}
  $trace=$shown[0].bindingsResearch[0].fxResearch
  $types=@($trace.tracks.type|Sort-Object);$wanted=@('.?AVarkMidnightTrackInstFx@@','.?AVarkMidnightTrackInstMaterialRenderParm@@')|Sort-Object
  if(Compare-Object $types $wanted){throw 'Remote scene has a non-cosmetic track'}
  $records=@($trace.tracks.records|Where-Object {$null-ne$_});if($records.Count-ne5-or@($records|Where-Object {!$_.keyResolved}).Count){throw 'Remote effect helpers unresolved'}
  foreach($rec in $records){if(!@($peerFxRows.effects|Where-Object {$_.owner-eq$rec.helperHandle-and$_.name-eq$rec.effect}).Count){throw 'Remote effect helper absent from independent FX map'}}
  [pscustomobject]@{variant=$v;sourceScene=$h[0].sourceScene;nativeScene=$h[0].nativeScene;startElapsed=$h[0].elapsed;nativeElapsed=$h[0].nativeTime;observations=$shown.Count;trace=$trace}
 })
 $fxEvidence|ConvertTo-Json -Depth 18|Set-Content "$out/remote-effects-verification.json"
 $checks.Add('One native cosmetic spawn and disappearance scene follow exact source scene IDs/clocks, with five independently verified FX helpers each and only FX/material tracks')



 $scenes|ConvertTo-Json -Depth 16|Set-Content "$out/doppelganger-scenes.json"
 C client 'coop_doppelganger_fixture off';$w.Restart()
 do{$f=S client '.doppelganger-fixture.json';if(!$f.active){break};Start-Sleep -Milliseconds 70}while($w.ElapsedMilliseconds-lt6500)
 if($f.active-or$f.result-ne'quantity_and_selection_restored'){throw 'Native Doppelganger inventory restoration unconfirmed'}
 foreach($r in 'client','host'){
  $after=Q $r '--player-inventory';$after|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-after-$r.json"
  if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($after.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Inventory quantities differ'}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Player or partner health changed'}
 }
 $checks.Add('Both original inventory quantities and player/partner health retained; world reset still required')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'client','host'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_doppelganger_fixture off'}catch{}}
 foreach($job in (@($recorders)+@($monitor))){if($job){$job.WaitForExit(45000)|Out-Null;if(!$job.HasExited-or$job.ExitCode){$failure=($failure+'; capture/watcher failed').TrimStart('; ')}}}
 foreach($r in 'client','host'){try{C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 22|Set-Content "$out/states.json"
 if($null-ne$worldSamples){$worldSamples|ConvertTo-Json -Depth 15|Set-Content "$out/clone-lifecycle-samples.json"}
 if($dirty){try{
  & "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log"
  foreach($r in 'client','host'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'}}
  $p=Q client '--player-doppelganger';if($p.quantity-or$p.equipped-or$p.started){throw 'Doppelganger remains acquired after checkpoint'}
  $entities=Q client '--entities';if(@($entities.entities|Where-Object {$_.definitionName-match'^models/powers/doppelganger/(decoy_entity|deadlyshadow_entity)(|_corvo)\.def$'}).Count){throw 'Spawned doppelganger remains after checkpoint'}
  $checks.Add('Both isolated checkpoints restore original health/inventory and retire spawned Doppelganger entities')
 }catch{$failure=($failure+'; checkpoint restore: '+$_.Exception.Message).TrimStart('; ')}}
 try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All14 protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
 [ordered]@{passed=(!$failure);failure=$failure;source='client';checks=$checks;events=$events;dll=$initialDll;probe=$initialProbe;bankSha256=(Get-FileHash "$PSScriptRoot/doppelganger-banks-v407/emily-doppelganger-bank.bin").Hash;scope='Standing armed native Emily Doppelganger phases drive remote full-body bank55 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute gameplay clone or damage events; separate native FX/material-only scenes reproduce source-owned summon/disappearance effects. Clone identity/root/lifetime Steam transport verified separately; shared clone gameplay, alternate contexts and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory.'}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_NATIVE_DOPPELGANGER_NETWORK $($checks.Count) checks"


