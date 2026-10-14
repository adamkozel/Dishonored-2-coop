param([ValidatePattern('^[a-z0-9-]+$')][string]$Take,[ValidateRange(-15,45)][int]$Pitch=30,[ValidateSet('standing','crouched')][string]$Stance='standing',[switch]$Holstered,[ValidateRange(0,12000)][int]$Move=0)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(!$Take-or(Test-Path $out)){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
$pids=@{};$inventory=@{};$rows=[Collections.Generic.List[object]]::new();$worldRows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$recorders=@();$capture=$null;$monitor=$null;$dirty=$false;$failure=$null
$initialDll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;$initialProbe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$lt=0,$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button Neutral -LeftTrigger $lt -DurationMs $ms|Out-Null}
function S($r,$suffix=''){for($a=0;$a-lt8;$a++){try{$v=Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json;if($null-ne$v){return $v}}catch{};Start-Sleep -Milliseconds 15};throw 'Status unavailable'}
function Q($r,$mode,$arg=0){for($a=0;$a-lt8;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read $mode failed"}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid;I $r
  foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_potion_fixture off','coop_bendtime_fixture off','coop_darkvision_fixture off','coop_mesmerize_fixture off','coop_domino_fixture off','coop_swarm_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Clean living fixture required'}
  $inventory[$r]=Q $r '--player-inventory';$inventory[$r]|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-before-$r.json"
 }
 $visualBaseline=S client '.swarm-rat-visual.json';$visualBaseline|ConvertTo-Json -Depth 8|Set-Content "$out/visual-baseline.json"
 $dirty=$true
 C host 'setviewpos -48.59 -155.07 68.1 90 0';C client 'setviewpos -46.3 -151.8 68.1 220 0';
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch 45 -Evidence "$out/observer-camera.json" -Quick
Start-Sleep -Milliseconds 800
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch $Pitch -Evidence "$out/source-camera.json" -Quick
 $ground=Q host '--player-states';$ground|ConvertTo-Json -Depth 10|Set-Content "$out/source-grounded.json"
 if(!$ground.motionValid-or$ground.motion-ne0-or$ground.stance-ne0-or[Math]::Abs([double]$ground.position[2]-66.4370575)-gt.03){throw 'Corvo is not grounded on measured test floor'}
 $before=Q host '--player-swarm';$before|ConvertTo-Json -Depth 8|Set-Content "$out/before.json"
 if($before.quantity-or$before.equipped-or$before.started){throw 'Swarm already owned or active'}
 $manaBefore=& "$PSScriptRoot/Read-NativeMana.ps1" -Role host -Evidence "$out/mana-before.json"
 C host 'coop_swarm_fixture 40000';$w=[Diagnostics.Stopwatch]::StartNew()
 do{$f=S host '.swarm-fixture.json';if($f.result-eq'native_swarm_ready'){break};Start-Sleep -Milliseconds 60}while($w.ElapsedMilliseconds-lt6500)
 if($f.result-ne'native_swarm_ready'){
  $f|ConvertTo-Json -Depth 8|Set-Content "$out/acquisition-fixture.json"
  $inv=Q host '--player-inventory';$inv|ConvertTo-Json -Depth 10|Set-Content "$out/acquisition-inventory.json"
  $item=@($inv.items|Where-Object name -eq 'models/powers/devouring_swarm/devouring_swarm.inventoryitem')
  if($item.Count-eq1-and$item[0].component-ne'0x0'){Q host '--object' $item[0].component|ConvertTo-Json -Depth 12|Set-Content "$out/acquisition-component.json";Q host '--components' $item[0].handle|ConvertTo-Json -Depth 12|Set-Content "$out/acquisition-components.json"}
  throw "Native Swarm acquisition failed: $($f.result)"
 }
 $power=Q host '--player-swarm';$power|ConvertTo-Json -Depth 8|Set-Content "$out/enabled.json"
 if(!$power.instantiated-or!$power.started-or!$power.equipped-or$power.quantity-ne1){throw 'Exact native Swarm item not ready'}
 $checks.Add('Exact Corvo Swarm item acquired through native inventory API; no direct spawn, AI or damage invocation')
 Copy-Item "$PSScriptRoot/swarm-context-banks-v272/corvo-swarm-context-bank.bin" "$root/work/lab-instances/client/coop-status.json.movement.bin"
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 600;C client 'coop_movement_bank on'
 $w.Restart();do{$m=S client '.movement.json';$stream=S client '.shadow-stream.json';if($m.active-and$m.version-eq50-and$m.weight-gt.9-and$m.presentedDrawn-and$stream.ready-and$stream.armed){break};Start-Sleep -Milliseconds 50}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne50-or$m.weight-lt.9-or!$m.presentedDrawn-or!$stream.ready-or!$stream.armed){throw 'Exact Swarm bank/native Steam stream not ready'}
 $checks.Add('Remote Corvo uses exact bank50/313 and native Steam pose transport')
 $expectedStance=if($Stance-eq'crouched'){1}else{0}
 if($expectedStance){& "$lab/Send-D2Input.ps1" -Role host -Button B -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 300}
 if($Holstered){
  & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch -60 -Evidence "$out/holster-look-away.json" -Quick
  & "$lab/Send-D2Input.ps1" -Role host -Button X -DurationMs 1000|Out-Null;Start-Sleep -Milliseconds 1200
  & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch $Pitch -Evidence "$out/holster-look-restored.json" -Quick
 }
 $w.Restart();$setupRows=[Collections.Generic.List[object]]::new()
 do{
  $m=S client '.movement.json';$native=Q host '--player-states';$nativeSword=Q host '--player-sword'
  $setupRows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;native=$native;sword=$nativeSword;movement=$m;source=(S host '.shadow-stream.json');peer=(S client '.shadow-stream.json')})
  if($native.stance-eq$expectedStance-and[Math]::Abs($m.stanceMix-$expectedStance)-lt.02-and$m.active-and$m.presentedDrawn-ne[bool]$Holstered-and$nativeSword.equipped-ne[bool]$Holstered-and[Math]::Abs($m.armedMix-$(if($Holstered){0}else{1}))-lt.02){break};Start-Sleep -Milliseconds 40
 }while($w.ElapsedMilliseconds-lt4500)
 $setupRows|ConvertTo-Json -Depth 14|Set-Content "$out/context-setup.json"
 if($native.stance-ne$expectedStance-or[Math]::Abs($m.stanceMix-$expectedStance)-gt.02-or!$m.active-or$m.presentedDrawn-eq[bool]$Holstered-or$nativeSword.equipped-eq[bool]$Holstered){throw 'Native/remote stance and equipment context not ready'}
 [ordered]@{stance=$Stance;holstered=[bool]$Holstered;move=$Move;native=$native;sword=$nativeSword;remote=$m}|ConvertTo-Json -Depth 10|Set-Content "$out/context-ready.json"
 $checks.Add('Native controller stance and holster inputs establish the same complete-body remote context before casting')

 $hwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64();if(!$hwnd){throw 'Source window missing'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','16','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take-source.mp4")
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();$capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 $recorders+=$capture
 $hwnd=(Get-Process -Id $pids.client).MainWindowHandle.ToInt64();if(!$hwnd){throw 'Observer window missing'}
 $idx=[Array]::IndexOf($args,($args|Where-Object {$_ -like 'gfxcapture=*'}));if($idx-lt0){throw 'Capture argument absent'}
 $args[$idx]="gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra";$args[-1]="$PSScriptRoot/$Take.mp4"
 $recorders+=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture-peer.log" -RedirectStandardOutput "$out/capture-peer.stdout" -PassThru
 Start-Sleep -Milliseconds 600;if(@($recorders|Where-Object HasExited).Count){throw 'Source capture failed'}
 $monitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.host,'--watch-swarm-scenes','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/power-scenes.ndjson" -RedirectStandardError "$out/power-scenes.stderr" -PassThru
 Start-Sleep -Milliseconds 100;if($monitor.HasExited){throw 'Strict Swarm watcher exited before input; see stderr'}
 $fxWatcher=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.host,'--watch-swarm-effects','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/swarm-effects.ndjson" -RedirectStandardError "$out/swarm-effects.stderr" -PassThru
 $recorders+=$fxWatcher
 $w.Restart();$cast=$false;$stood=$false;$releaseWalking=$false
 while($w.ElapsedMilliseconds-lt14000){
  if(!$cast-and$w.ElapsedMilliseconds-ge1100){& "$lab/Send-D2Input.ps1" -Role host -Button Neutral -LeftY $Move -LeftTrigger 255 -DurationMs 1800|Out-Null;$cast=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input="Native Corvo $Stance LT1800 then release; LeftY=$Move; holstered=$Holstered"})}
  if($Move-and$cast-and!$releaseWalking-and$w.ElapsedMilliseconds-ge3000){& "$lab/Send-D2Input.ps1" -Role host -Button Neutral -LeftY $Move -DurationMs 1400|Out-Null;$releaseWalking=$true}
  if(!$stood-and$w.ElapsedMilliseconds-ge6500){if($expectedStance){& "$lab/Send-D2Input.ps1" -Role host -Button B -DurationMs 180|Out-Null};$stood=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native standing recovery after cast'})}
  $m=S client '.movement.json';$stream=S client '.shadow-stream.json'
  $rows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;player=(Q host '--player-states');sword=(Q host '--player-sword');movement=$m;stream=$stream})
  if(!$m.active-or!$stream.ready-or!$stream.armed){throw "Native transport/presentation retired: $($stream.result)"}
  $worldRows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;source=(S host '.swarm-world.json');peer=(S client '.swarm-world.json');visual=(S client '.swarm-rat-visual.json')})
  if($w.ElapsedMilliseconds-ge5500-and!(Test-Path "$out/visual-early-components.json")){
   $v=S client '.swarm-rat-visual.json'
   if($v.active-and$v.handle){
    $cp=Q client '--components' $v.handle;$cp|ConvertTo-Json -Depth 12|Set-Content "$out/visual-early-components.json"
    Q client '--object' $cp.components[0].owner|ConvertTo-Json -Depth 12|Set-Content "$out/visual-early-entity.json"
    & "$lab/bin/d2_runtime_probe.exe" $pids.client '--rat-skeleton' $v.handle > "$out/visual-early-skeleton-0.json"
    Start-Sleep -Milliseconds 120
    & "$lab/bin/d2_runtime_probe.exe" $pids.client '--rat-skeleton' $v.handle > "$out/visual-early-skeleton-1.json"
   }
  }
  Start-Sleep -Milliseconds 30
 }
 I host;$manaAfter=& "$PSScriptRoot/Read-NativeMana.ps1" -Role host -Evidence "$out/mana-after.json"
 $monitor.WaitForExit(4000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){throw 'Raw Midnight watcher failed'}
 $samples=@(Get-Content "$out/power-scenes.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $scenes=@($samples|Where-Object valid)
 foreach($v in 23,24,25){
  if(!@($scenes|Where-Object variant -eq $v).Count){throw "Strict native Swarm phase $v absent"}
  if(!@($rows|Where-Object {$_.movement.powerVariant-eq$v-and$_.movement.powerWeight-gt.5}).Count){throw "Remote Swarm phase $v absent"}
 }
 if(@($scenes|Where-Object {!$_.armHandle-or!$_.equipmentHandle}).Count-or@($scenes.armHandle|Sort-Object -Unique).Count-ne1-or@($scenes.equipmentHandle|Sort-Object -Unique).Count-ne1){throw 'Source Swarm ownership changed'}
 $checks.Add('All three exact native phases with reciprocal Corvo arm/body ownership drive remote full-body phases')
 $sourceStream=S host '.shadow-stream.json';$peerStream=S client '.shadow-stream.json'
 [ordered]@{source=$sourceStream;peer=$peerStream}|ConvertTo-Json -Depth 12|Set-Content "$out/power-event-history.json"
 $sent=@($sourceStream.powerSentHistory|Where-Object {$_.variant-in23,24,25})
 $received=@($peerStream.powerReceivedHistory|Where-Object {$_.variant-in23,24,25})
 if(!$sent.Count-or$sent.Count-ne$received.Count){throw 'Swarm sender/receiver retained event counts differ'}
 if(Compare-Object @($sent|ForEach-Object {"$($_.event):$($_.variant):$($_.durationMs)"}) @($received|ForEach-Object {"$($_.event):$($_.variant):$($_.durationMs)"})){throw 'Swarm accepted receiver events differ from sent events'}
 $phaseCoverage=@(foreach($v in 23,24,25){
  $nativeIds=@($scenes|Where-Object variant -eq $v|ForEach-Object handle|Sort-Object -Unique)
  $sourceEvents=@($sent|Where-Object variant -eq $v)
  $nativeSent=@($sourceEvents.nativeScene|Sort-Object -Unique)
  if(!$nativeIds.Count-or(Compare-Object $nativeIds $nativeSent)){throw "Phase $v strict-native scene IDs differ from actual sent scene IDs"}
  [pscustomobject]@{variant=$v;nativeHandles=$nativeIds;sourceEvents=$sourceEvents;acceptedEvents=@($received|Where-Object variant -eq $v);sampledPoseEvents=@($rows|Where-Object {$_.movement.powerVariant-eq$v-and$_.movement.powerWeight-gt.5}|ForEach-Object {$_.movement.powerEvent}|Sort-Object -Unique)}
 })
 $phaseCoverage|ConvertTo-Json -Depth 8|Set-Content "$out/phase-incarnations.json"
 $checks.Add('Bounded send/accept histories exactly match strict native scene incarnations, including sub-status-interval final aim loops')
 $continuity=@($rows|Where-Object {$_.ms-ge900-and$_.ms-le6200})
 if($continuity.Count-lt20-or@($continuity|Where-Object {$_.movement.weight-lt.95-or$_.movement.clockRejection.count-ne0}).Count){throw 'Swarm root/body continuity failed'}
 [ordered]@{samples=$continuity.Count;minimumBodyWeight=($continuity.movement.weight|Measure-Object -Minimum).Minimum;rejections=@($continuity|ForEach-Object {$_.movement.clockRejection.count}|Sort-Object -Unique)}|ConvertTo-Json|Set-Content "$out/continuity.json"
 $checks.Add('Swarm cast preserves root/body continuity without rejected presentation clocks')
 if($m.powerWeight-gt.02-or$m.weight-lt.9-or$stream.remoteState-ne0){throw 'Remote cast did not recover to standing'}
 $sword=Q client '--avatar-sword' (S client).probeActorHandle;$sword|ConvertTo-Json -Depth 8|Set-Content "$out/avatar-sword-after.json"
 if($sword.equipped-eq[bool]$Holstered-or(Q host '--player-sword').equipped-eq[bool]$Holstered){throw 'Native sword selection changed'}
 $castRows=@($rows|Where-Object {$_.movement.powerVariant-in23,24,25-and$_.movement.powerWeight-gt.5-and$_.ms-lt6200})
 if(!$castRows.Count-or@($castRows|Where-Object {$_.player.stance-ne$expectedStance-or[Math]::Abs($_.movement.stanceMix-$expectedStance)-gt.02-or$_.movement.presentedDrawn-eq[bool]$Holstered-or$_.sword.equipped-eq[bool]$Holstered}).Count){throw 'Swarm cast left native/remote stance or sword context'}
 $standing=@($rows|Where-Object {$_.ms-gt8500-and$_.player.stance-eq0-and$_.movement.stanceMix-lt.02-and$_.movement.weight-gt.95})
 if(!$standing.Count){throw 'Standing recovery not recorded'}
 [ordered]@{stance=$Stance;holstered=[bool]$Holstered;move=$Move;contextPhaseSamples=$castRows.Count;standingRecoverySamples=$standing.Count;minimumCrouchedWeight=($castRows.movement.stanceMix|Measure-Object -Minimum).Minimum}|ConvertTo-Json|Set-Content "$out/context-summary.json"
 $checks.Add('Remote body retains native cast stance/sword context and then recovers to standing with sword selection retained')
 $sourceEntities=Q host '--entities';$peerEntities=Q client '--entities'
 $nativeRats=@($sourceEntities.entities|Where-Object {$_.definitionName-match'^models/characters/animals/rat/devouring_swarm(_big)?\.def$'})
 $peerRats=@($peerEntities.entities|Where-Object {$_.definitionName-match'^models/characters/animals/rat/devouring_swarm(_big)?\.def$'})
 [ordered]@{source=$nativeRats;peer=$peerRats;scope='Source native rats are observational; body playback must not duplicate gameplay spawns'}|ConvertTo-Json -Depth 10|Set-Content "$out/swarm-world-observation.json"
 if(!$nativeRats.Count-or$peerRats.Count){throw 'Expected native source rats only; missing source or duplicated peer swarm'}
 $checks.Add('Native source swarm remains source-owned; body playback creates no peer gameplay rats or duplicate power actions')
 $worldRows|ConvertTo-Json -Depth 16|Set-Content "$out/world-states.json"
 $visualRows=@($worldRows|Where-Object {$_.visual.active-and$_.visual.playing-eq2-and!$_.visual.pendingHide})
 $visual=S client '.swarm-rat-visual.json';$visual|ConvertTo-Json -Depth 8|Set-Content "$out/visual-before-stop.json"
 if($visualRows.Count-lt20-or($visual.created-$visualBaseline.created)-ne25-or($visual.started-$visualBaseline.started)-lt25-or($visual.updated-$visualBaseline.updated)-lt20-or($visual.refused-$visualBaseline.refused)-ne0){throw "Cosmetic rat proof failed: $($visual.result), stage=$($visual.stage), samples=$($visualRows.Count)"}
 $verified=@($visualRows|Where-Object {$_.visual.verifiedTransforms-gt20})
 if($verified.Count-lt20-or@($verified|Where-Object {$_.visual.maxPositionError-gt.001}).Count){throw 'Live rat transform readback did not match source target'}
 $extent=@(foreach($axis in 0,1,2){$values=@($verified|ForEach-Object {$_.visual.actualTransform[$axis]});$range=$values|Measure-Object -Minimum -Maximum;[double]$range.Maximum-[double]$range.Minimum})
 if(($extent|Measure-Object -Maximum).Maximum-lt.1){throw 'Readback rat body did not travel at least10cm'}
 [ordered]@{samples=$verified.Count;axisExtent=$extent;maximumPositionError=($verified.visual.maxPositionError|Measure-Object -Maximum).Maximum;verifiedTransforms=$visual.verifiedTransforms}|ConvertTo-Json|Set-Content "$out/live-transform-summary.json"
 $checks.Add('Owned live rat transform readback matches received target within1mm and moves at least10cm; animation output assessed separately')
 Q client '--components' $visual.handle|ConvertTo-Json -Depth 12|Set-Content "$out/visual-components.json"
 Q client '--midnight'|ConvertTo-Json -Depth 20|Set-Content "$out/visual-midnight.json"
 $ratEntities=Q client '--entities';$rats=@($ratEntities.entities|Where-Object definitionName -eq 'models/powers/d2coop/swarm_rat_visual.def')
 $rats|ConvertTo-Json -Depth 8|Set-Content "$out/visual-entities.json"
 if($rats.Count-eq25){
  Q client '--object' $rats[0].pointer|ConvertTo-Json -Depth 12|Set-Content "$out/visual-entity-object.json"
  & "$lab/bin/d2_runtime_probe.exe" $pids.client '--rat-skeleton' $visual.handle > "$out/visual-skeleton-0.json"
  Start-Sleep -Milliseconds 120
  & "$lab/bin/d2_runtime_probe.exe" $pids.client '--rat-skeleton' $visual.handle > "$out/visual-skeleton-1.json"
 }

 $pose0=Get-Content "$out/visual-early-skeleton-0.json" -Raw|ConvertFrom-Json
 $pose1=Get-Content "$out/visual-early-skeleton-1.json" -Raw|ConvertFrom-Json
 if(!$pose0.stable-or!$pose1.stable-or$pose0.joints.Count-ne31-or$pose1.joints.Count-ne31){throw 'Stable31-joint animation observations required'}
 $poseDeltas=@(for($i=1;$i-lt31;$i++){
  if($pose0.joints[$i].nativeName-ne$pose1.joints[$i].nativeName){throw 'Joint identity changed'}
  $delta=0.0;for($j=0;$j-lt12;$j++){$delta=[Math]::Max($delta,[Math]::Abs($pose0.joints[$i].localPose[$j]-$pose1.joints[$i].localPose[$j]))}
  [pscustomobject]@{joint=$pose0.joints[$i].nativeName;maxLocalDelta=$delta}
 })
 $changing=@($poseDeltas|Where-Object maxLocalDelta -gt.01)
 [ordered]@{treeActive=$pose1.treeActive;treeOutput=$pose1.treeHasOutput;changingJoints=$changing.Count;deltas=$poseDeltas;visualAcceptance=$false}|ConvertTo-Json -Depth 5|Set-Content "$out/animation-output-summary.json"
 if($changing.Count-lt4){throw 'Native rat walking did not produce significant joint-local motion'}
 $checks.Add('At least4 joints change locally above0.01 over120ms; root displacement and floating-point noise cannot satisfy this animation check')
 if($rats.Count-ne25){throw 'Expected25 cached cosmetic rat entities'}
 $checks.Add('25 owned cosmetic native rat models follow opaque peer identities with native walking animleafs; no peer gameplay swarm')
 $fullPool=@($worldRows|Where-Object {$_.visual.activeModels-eq$_.peer.count-and$_.peer.count-gt0})
 if($fullPool.Count-lt20){throw 'Insufficient snapshots with a complete match of currently indexed source rats'}
 foreach($row in $fullPool){
  $models=@($row.visual.models|Where-Object active)
  if($models.Count-ne$row.peer.count-or@($models.handle|Sort-Object -Unique).Count-ne$row.peer.count){throw 'Cosmetic local handles not unique'}
  foreach($model in $models){
   $source=@($row.peer.agents|Where-Object {$_.index-eq$model.index-and$_.handle-eq$model.sourceHandle-and$_.incarnation-eq$model.incarnation})
   if($source.Count-ne1-or$model.maxPositionError-gt.001-or!$model.verifiedTransforms){throw 'Cosmetic slot source identity/readback mismatch'}
  }
 }
 [ordered]@{fullPoolSamples=$fullPool.Count;maximumActive=($worldRows.visual.activeModels|Measure-Object -Maximum).Maximum;created=$visual.created-$visualBaseline.created;serviceMaxMs=$visual.serviceMaxMs;fullGameplay=$false}|ConvertTo-Json|Set-Content "$out/pool-summary.json"
 $checks.Add('All currently indexed source rats match unique owned cosmetic identities for at least20 snapshots; native source retirement is preserved')
 $gaits=@($fullPool|ForEach-Object {$_.visual.models}|Where-Object {$_.active-and$_.gaitUpdates-gt10})
 if($gaits.Count-lt500){throw 'Insufficient native gait input readback samples'}
 foreach($g in $gaits){$expected=[Math]::Min(3.0,$g.gaitSpeed/(1.11*$g.sourceScale));if([Math]::Abs($g.gaitRate-$expected)-gt.0001-or$g.gaitMoving-lt0-or$g.gaitMoving-gt1){throw 'Native gait rate or blend differs from measured travel'}}
 $moving=@($gaits|Where-Object {$_.gaitSpeed-gt.05-and$_.gaitRate-gt.04});$held=@($gaits|Where-Object {$_.gaitSpeed-lt.001-and$_.gaitRate-lt.001})
 if($moving.Count-lt100-or$held.Count-lt10){throw 'Measured moving/held gait contexts missing'}
 [ordered]@{nativeInputSamples=$gaits.Count;movingSamples=$moving.Count;heldSamples=$held.Count;minimumRate=($gaits.gaitRate|Measure-Object -Minimum).Minimum;maximumRate=($gaits.gaitRate|Measure-Object -Maximum).Maximum;strideSpeed=1.11;visualAccepted=$false}|ConvertTo-Json|Set-Content "$out/gait-summary.json"
 $checks.Add('Named native gait inputs read back correctly and track measured horizontal travel divided by native1.11m/s stride speed multiplied by each rat scale; idle/moving samples covered')
 $scaled=@($gaits|Where-Object {$_.scaleVerified-gt5})
 if($scaled.Count-lt500){throw 'Insufficient native scale settlement samples'}
 foreach($g in $scaled){if(!$g.scaleReady){throw 'Previously settled scale became unready'};foreach($v in @($g.frameScale)+@($g.entityScale)+@($g.renderScale)){if([Math]::Abs($v-$g.sourceScale)-gt.0001){throw 'Native frame/entity/render scale differs from source'}}}
 if(@($scaled.sourceScale|Sort-Object -Unique).Count-lt10){throw 'Source size variation missing'}
 foreach($row in $fullPool){foreach($g in @($row.visual.models|Where-Object active)){$src=@($row.peer.agents|Where-Object index -eq $g.index);if($src.Count-ne1-or[Math]::Abs($src[0].scale-$g.sourceScale)-gt.0001){throw 'Cosmetic scale is not the matching source rat scale'}}}
 [ordered]@{settledSamples=$scaled.Count;distinctScales=@($scaled.sourceScale|Sort-Object -Unique).Count;minimum=($scaled.sourceScale|Measure-Object -Minimum).Minimum;maximum=($scaled.sourceScale|Measure-Object -Maximum).Maximum;nativeDispatch='Exact owned entity+0x330 write, native0x14087c5d0 render flush, self-frame settlement';visualAccepted=$false}|ConvertTo-Json|Set-Content "$out/scale-summary.json"
 $checks.Add('Each owned rat uses its replicated size through a guarded cosmetic-only entity scale write and native render/frame propagation; frame/entity/render readbacks all match and gait stride accounts for size')


 if($visual.interpolatedTransforms-lt500){throw 'Insufficient per-frame rat interpolation between native Steam snapshots'}
 [ordered]@{interpolatedUpdates=$visual.interpolatedTransforms;verifiedUpdates=$visual.verifiedTransforms;ratio=$visual.interpolatedTransforms/[double]$visual.verifiedTransforms;extrapolation=$false}|ConvertTo-Json|Set-Content "$out/interpolation-summary.json"
 $checks.Add('More than500 actual native-transform updates interpolate between source snapshots; scope changes and missing samples hold rather than extrapolate')
 if($visual.interpolatedTransforms/[double]$visual.verifiedTransforms-lt.85){throw 'Rat interpolation coverage below85percent with120ms cosmetic buffer'}
 $checks.Add('At least85percent of verified native rat updates have both source samples available at the 120ms cosmetic presentation delay')


 $sourceWorld=S host '.swarm-world.json';$peerWorld=S client '.swarm-world.json' 
 [ordered]@{source=$sourceWorld;peer=$peerWorld}|ConvertTo-Json -Depth 12|Set-Content "$out/world-history.json"
 $live=@($worldRows|Where-Object {$_.peer.leased-and$_.peer.present-eq1-and$_.peer.count-gt0})
 if($live.Count-lt20-or$sourceWorld.sent-lt20-or$peerWorld.presented-lt20){throw 'Insufficient live native Steam rat snapshots'}
 $matched=@(foreach($r in $peerWorld.history){$sent=@($sourceWorld.history|Where-Object sequence -eq $r.sequence);if(!$sent.Count){continue};if($sent.Count-ne1-or$sent[0].digest-ne$r.digest-or$sent[0].count-ne$r.count-or$sent[0].handle-ne$r.handle){throw 'Accepted rat wire differs from source'};$r})
 if($matched.Count-lt20){throw 'Insufficient overlapping exact source/accepted rat packets'}
 $checks.Add('More than20 accepted Steam world snapshots exactly match source wire digests, native rat identities and counts')
 if(@($live|Where-Object {$_.peer.bytes-ne(124+72*$_.peer.count)}).Count){throw 'Rat wire length/count mismatch'}
 [ordered]@{liveSamples=$live.Count;matchedPackets=$matched.Count;counts=@($live|ForEach-Object {$_.peer.count}|Sort-Object -Unique);rejected=$peerWorld.rejected;unknown=$sourceWorld.unknown;scopedReadMaxMs=$sourceWorld.scopedReadMaxMs;remoteCosmeticVisuals=$true;remoteDamage=$false}|ConvertTo-Json|Set-Content "$out/world-summary.json"
 $checks.Add('World samples preserve bounded native agent sets and exact variable payload size; source reader timing retained')
 C host 'coop_shadow_stream off';Start-Sleep -Milliseconds 1200
 $stopped=S client '.swarm-world.json';$stopped|ConvertTo-Json -Depth 12|Set-Content "$out/world-after-source-stop.json"
 if($stopped.leased){throw 'Remote rat lease survives source stream retirement'}
 $checks.Add('Remote rat lease retires when source Steam semantic stream stops')
 $hidden=S client '.swarm-rat-visual.json';$hidden|ConvertTo-Json -Depth 8|Set-Content "$out/visual-after-source-stop.json"
 if($hidden.active-or$hidden.pendingHide-or$hidden.scene-or($hidden.hidden-$visualBaseline.hidden)-lt25){throw 'Cosmetic rat scene/model survives stream stop'}
 $checks.Add('Source stream retirement hides cached native rat animleaf model')

 $scenes|ConvertTo-Json -Depth 16|Set-Content "$out/swarm-scenes.json"
 C host 'coop_swarm_fixture off';$w.Restart()
 do{$f=S host '.swarm-fixture.json';if(!$f.active){break};Start-Sleep -Milliseconds 70}while($w.ElapsedMilliseconds-lt6500)
 if($f.active-or$f.result-ne'quantity_and_selection_restored'){throw 'Native Swarm inventory restoration unconfirmed'}
 foreach($r in 'host','client'){
  $after=Q $r '--player-inventory';$after|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-after-$r.json"
  if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($after.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Inventory quantities differ'}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Player or partner health changed'}
 }
 $checks.Add('Both original inventory quantities and player/partner health retained; world reset still required')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{S $r '.pose-clock.json'|ConvertTo-Json -Depth 9|Set-Content "$out/clock-before-cleanup-$r.json"}catch{}}
 $worldRows|ConvertTo-Json -Depth 16|Set-Content "$out/world-states.json"

 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_swarm_fixture off'}catch{}}
 foreach($job in (@($recorders)+@($monitor))){if($job){$job.WaitForExit(18000)|Out-Null;if(!$job.HasExited-or$job.ExitCode){$failure=($failure+'; capture/watcher failed').TrimStart('; ')}}}
 foreach($r in 'host','client'){try{C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 22|Set-Content "$out/states.json"
 if($dirty){try{
  & "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log"
  foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'}}
  $p=Q host '--player-swarm';if($p.quantity-or$p.equipped-or$p.started){throw 'Swarm remains acquired after checkpoint'}
  $entities=Q host '--entities';if(@($entities.entities|Where-Object {$_.definitionName-match'^models/characters/animals/rat/devouring_swarm(_big)?\.def$'}).Count){throw 'Spawned swarm remains after checkpoint'}
  $afterWorld=S client '.swarm-world.json';$afterWorld|ConvertTo-Json -Depth 12|Set-Content "$out/world-after-checkpoint.json";if($afterWorld.leased-or$afterWorld.count){throw 'Rat world presentation survives checkpoint'}
  $checks.Add('Both isolated checkpoints restore original health/inventory and retire spawned Swarm entities and peer world presentation')
 }catch{$failure=($failure+'; checkpoint restore: '+$_.Exception.Message).TrimStart('; ')}}
 try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All14 protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
 [ordered]@{passed=(!$failure);failure=$failure;source='host';stance=$Stance;holstered=[bool]$Holstered;move=$Move;checks=$checks;events=$events;dll=$initialDll;probe=$initialProbe;bankSha256=(Get-FileHash "$PSScriptRoot/swarm-context-banks-v272/corvo-swarm-context-bank.bin").Hash;scope='Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. A bounded cached cosmetic pool receives live transforms through guarded native dispatch with actual position readback. A dedicated native walking animleaf is tested with joint-local motion thresholds; visual acceptance remains separately assessed. Upgraded100-rat capacity, source-specific animation phase/state, color, subjective gait/interpolation quality, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory.'}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_NATIVE_SWARM_OBSERVATION $($checks.Count) checks"
