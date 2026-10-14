param([ValidatePattern('^[a-z0-9-]+$')][string]$Take,[ValidateRange(-15,45)][int]$Pitch=0,[ValidateSet('standing','crouched')][string]$Stance='crouched',[switch]$Holstered)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(!$Take-or(Test-Path $out)){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
$pids=@{};$inventory=@{};$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$recorders=@();$capture=$null;$monitor=$null;$rawMonitor=$null;$dirty=$false;$failure=$null
$initialDll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;$initialProbe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$lt=0,$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button Neutral -LeftTrigger $lt -DurationMs $ms|Out-Null}
function S($r,$suffix=''){for($a=0;$a-lt8;$a++){try{$v=Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json;if($null-ne$v){return $v}}catch{};Start-Sleep -Milliseconds 15};throw 'Status unavailable'}
function Q($r,$mode,$arg=0){for($a=0;$a-lt8;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read $mode failed"}
function OptionalFx($r){$raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$r] --domino-effects;$code=$LASTEXITCODE;if($code){return [pscustomobject]@{stable=$false;exitCode=$code}};return($raw|ConvertFrom-Json)}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid;I $r
  foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_potion_fixture off','coop_bendtime_fixture off','coop_darkvision_fixture off','coop_mesmerize_fixture off','coop_domino_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Clean living fixture required'}
  $inventory[$r]=Q $r '--player-inventory';$inventory[$r]|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-before-$r.json"
 }
 $dirty=$true
 # Suspending AI decisions does not stop an existing native navigation animation.
 $settleRows=[Collections.Generic.List[object]]::new();$previousGuard=$null;$stableGuard=0;$settleClock=[Diagnostics.Stopwatch]::StartNew()
 do{
  $catalog=Q client '--world-npcs'
  $guard=@($catalog.npcs|Where-Object {$_.unique-and$_.reciprocal-and$_.spawnerName-eq'ld_spawner_guard_tutorial_2'-and$_.definition-eq'models/characters/def/generic/dunwall_guard_male_small_01.def'})
  if($guard.Count-ne1){throw 'Exact guard missing during settling'}
  $same=$null-ne$previousGuard-and$previousGuard.handle-eq$guard[0].handle
  if($same){for($k=0;$k-lt3;$k++){if([Math]::Abs([double]$guard[0].position[$k]-[double]$previousGuard.position[$k])-gt.01){$same=$false}}}
  $stableGuard=if($same){$stableGuard+1}else{0};$previousGuard=$guard[0]
  $settleRows.Add([pscustomobject]@{ms=$settleClock.ElapsedMilliseconds;stableSamples=$stableGuard;target=$guard[0]})
  if($stableGuard-ge3){break};Start-Sleep -Milliseconds 220
 }while($settleClock.ElapsedMilliseconds-lt7500)
 $settleRows|ConvertTo-Json -Depth 9|Set-Content "$out/native-guard-settling.json"
 if($stableGuard-lt3){throw 'Native first guard still moving after bounded settling'}
 $initialCatalog=Q client '--world-npcs'
 $initialTargets=@($initialCatalog.npcs|Where-Object {$_.unique-and$_.reciprocal-and$_.spawnerName-eq'ld_spawner_guard_tutorial_2'-and$_.definition-eq'models/characters/def/generic/dunwall_guard_male_small_01.def'-and[Math]::Abs([double]$_.position[2]-66.4)-lt.5})
 if($initialTargets.Count-ne1){throw 'Exact first guard unavailable before choosing source floor'}
 $anchor=$null
 foreach($candidate in @(@(-44.39,-153.27,66.4036484),@(-48.59,-155.07,66.4370575))){
  $distance=[Math]::Pow([double]$initialTargets[0].position[0]-$candidate[0],2)+[Math]::Pow([double]$initialTargets[0].position[1]-$candidate[1],2)
  if($distance-ge9-and$distance-le225){$anchor=$candidate;break}
 }
 if(!$anchor){throw 'Neither measured upper-floor anchor has a three-metre targeting gap'}
 # The initial eastern guard position is behind the closed door from the balcony anchors.
 # Use a source-only point along his corridor; native physics must confirm the floor.
 if([double]$initialTargets[0].position[0]-gt-43){$anchor=@(([double]$initialTargets[0].position[0]),([double]$initialTargets[0].position[1]-2.5),([double]$initialTargets[0].position[2]))}
 $firstX=$anchor[0];$firstY=$anchor[1];$ci=[Globalization.CultureInfo]::InvariantCulture
 C client ('setviewpos {0} {1} 68.1 90 0'-f$firstX.ToString('F4',$ci),$firstY.ToString('F4',$ci))
 Start-Sleep -Milliseconds 750
 $floors=@();foreach($n in 1..3){$floors+=Q client '--player-states';Start-Sleep -Milliseconds 100}
 [ordered]@{anchor=$anchor;target=$initialTargets[0];samples=$floors}|ConvertTo-Json -Depth 10|Set-Content "$out/first-source-floor.json"
 foreach($floor in $floors){if(!$floor.motionValid-or$floor.motion-ne0-or$floor.stance-ne0-or[Math]::Abs([double]$floor.position[2]-$anchor[2])-gt.03){throw 'First source not grounded at measured anchor'};for($a=0;$a-lt3;$a++){if([Math]::Abs([double]$floor.position[$a]-[double]$floors[0].position[$a])-gt.002){throw 'First source floor unstable'}}}

 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch $Pitch -Evidence "$out/source-camera.json" -Quick
 $initialTarget=$initialTargets[0];$yaw=[Math]::Atan2([double]$initialTarget.position[1]-$firstY,[double]$initialTarget.position[0]-$firstX)*180/[Math]::PI
 C client ('setviewpos {0} {1} 68.1 {2} 0'-f$firstX.ToString('F4',$ci),$firstY.ToString('F4',$ci),$yaw.ToString('F4',$ci))
 Start-Sleep -Milliseconds 600
 & "$PSScriptRoot/Aim-NativeDominoGuard-v218.ps1" -Role client -Evidence "$out/native-target-aim.json"

 $ground=Q client '--player-states';$ci=[Globalization.CultureInfo]::InvariantCulture
 $observerAnchor=$null
 foreach($candidate in @(@(-44.39,-153.27,66.4036484),@(-48.59,-155.07,66.4370575))){
  $gap=[Math]::Pow([double]$ground.position[0]-$candidate[0],2)+[Math]::Pow([double]$ground.position[1]-$candidate[1],2)
  if($gap-ge9-and$gap-le100){$observerAnchor=$candidate;break}
 }
 if(!$observerAnchor){throw 'No previously measured observer floor with bounded source separation'}
 $ox=$observerAnchor[0];$oy=$observerAnchor[1];$oz=$observerAnchor[2]+1.7
 $oyaw=[Math]::Atan2([double]$ground.position[1]-$oy,[double]$ground.position[0]-$ox)*180/[Math]::PI
 C host ('setviewpos {0} {1} {2} {3} 0'-f$ox.ToString('F4',$ci),$oy.ToString('F4',$ci),$oz.ToString('F4',$ci),$oyaw.ToString('F4',$ci))
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch 12 -Evidence "$out/observer-camera.json" -Quick
 $floorRows=[Collections.Generic.List[object]]::new();$stableFloor=0;$lastFloor=$null;$floorWatch=[Diagnostics.Stopwatch]::StartNew()
 do{
  $f=Q host '--player-states';$floorRows.Add($f)
  $same=$f.motionValid-and$f.motion-eq0-and$f.stance-eq0-and[Math]::Abs([double]$f.position[2]-$observerAnchor[2])-lt.03-and$null-ne$lastFloor
  if($same){for($k=0;$k-lt3;$k++){if([Math]::Abs([double]$f.position[$k]-[double]$lastFloor.position[$k])-gt.005){$same=$false}}}
  $stableFloor=if($same){$stableFloor+1}else{0};$lastFloor=$f;if($stableFloor-ge4){break};Start-Sleep -Milliseconds 125
 }while($floorWatch.ElapsedMilliseconds-lt4000)
 [ordered]@{anchor=$observerAnchor;source=$ground;stable=$stableFloor;rows=$floorRows}|ConvertTo-Json -Depth 10|Set-Content "$out/peer-floor-before-stream.json"
 if($stableFloor-lt4){throw 'Observer not stably grounded at previously measured floor'}
 $before=Q client '--player-domino';$before|ConvertTo-Json -Depth 7|Set-Content "$out/before.json"
 if($before.quantity-or$before.equipped-or$before.started){throw 'Domino already owned or active'}
 $manaBefore=& "$PSScriptRoot/Read-NativeMana.ps1" -Role client -Evidence "$out/mana-before.json"
 $dirty=$true;C client 'coop_domino_fixture 40000';$w=[Diagnostics.Stopwatch]::StartNew()
 do{$f=S client '.domino-fixture.json';if($f.result-eq'native_domino_ready'){break};Start-Sleep -Milliseconds 60}while($w.ElapsedMilliseconds-lt6000)
 if($f.result-ne'native_domino_ready'){throw "Native Domino acquisition failed: $($f.result)"}
 $power=Q client '--player-domino';$power|ConvertTo-Json -Depth 7|Set-Content "$out/enabled.json"
 if(!$power.equipped-or!$power.started-or!$power.instantiated-or$power.quantity-ne1){throw 'Native Domino equipment not confirmed'}
 $checks.Add('Exact native Emily Domino item acquired and selected without directly invoking power or VFX')
 Start-Sleep -Milliseconds 750
 $grounded=Q client '--player-states';$grounded|ConvertTo-Json -Depth 8|Set-Content "$out/source-grounded-after-aim.json"
 if(!$grounded.motionValid-or$grounded.motion-ne0-or[Math]::Abs([double]$grounded.position[2]-[double]$anchor[2])-gt.03){throw 'Source aim placement has not returned to proven native floor'}
 Copy-Item "$PSScriptRoot/domino-context-banks-v243/emily-domino-context-bank.bin" "$root/work/lab-instances/host/coop-status.json.movement.bin"
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 600;C host 'coop_movement_bank on'
 $w.Restart();do{$m=S host '.movement.json';$stream=S host '.shadow-stream.json';if($m.active-and$m.version-eq48-and$m.weight-gt.9-and$m.presentedDrawn-and$stream.ready-and$stream.armed){break};Start-Sleep -Milliseconds 50}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne48-or$m.weight-lt.9-or!$m.presentedDrawn-or!$stream.ready-or!$stream.armed){throw 'Exact Domino bank and native Steam stream not ready'}
 $checks.Add('Native Steam transport and exact bank48/301 remote Emily standing armed context ready')

 $expectedStance=if($Stance-eq'crouched'){1}else{0}
 if($expectedStance){& "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 300}
 if($Holstered){
  & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch -60 -Evidence "$out/holster-look-away.json" -Quick
  & "$lab/Send-D2Input.ps1" -Role client -Button X -DurationMs 1000|Out-Null;Start-Sleep -Milliseconds 1200
  & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch 0 -Evidence "$out/holster-look-restored.json" -Quick
 }
 $w.Restart();$setupRows=[Collections.Generic.List[object]]::new()
 do{
  $m=S host '.movement.json';$native=Q client '--player-states';$nativeSword=Q client '--player-sword'
  $setupRows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;native=$native;sword=$nativeSword;movement=$m;source=(S client '.shadow-stream.json');peer=(S host '.shadow-stream.json')})
  if($native.stance-eq$expectedStance-and[Math]::Abs($m.stanceMix-$expectedStance)-lt.02-and$m.active-and$m.presentedDrawn-ne[bool]$Holstered-and$nativeSword.equipped-ne[bool]$Holstered-and[Math]::Abs($m.armedMix-$(if($Holstered){0}else{1}))-lt.02){break};Start-Sleep -Milliseconds 40
 }while($w.ElapsedMilliseconds-lt4500)
 $setupRows|ConvertTo-Json -Depth 14|Set-Content "$out/context-setup.json"
 if($native.stance-ne$expectedStance-or[Math]::Abs($m.stanceMix-$expectedStance)-gt.02-or!$m.active-or$m.presentedDrawn-eq[bool]$Holstered-or$nativeSword.equipped-eq[bool]$Holstered){throw 'Native/remote stance and equipment context not ready'}
 [ordered]@{stance=$Stance;holstered=[bool]$Holstered;move=0;native=$native;sword=$nativeSword;remote=$m}|ConvertTo-Json -Depth 10|Set-Content "$out/context-ready.json"
 $checks.Add('Native controller stance and holster inputs establish the same complete-body remote context before casting')
 $groundState=if($expectedStance){6}else{0}
 $hwnd=(Get-Process -Id $pids.client).MainWindowHandle.ToInt64();if(!$hwnd){throw 'No source window'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','18','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take-source.mp4")
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();$capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 $recorders+=$capture
 $hwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64()
 $captureInputIndex=[Array]::IndexOf($args,($args|Where-Object {$_ -like 'gfxcapture=*'}))
 if($captureInputIndex-lt0){throw 'Capture input argument missing'}
 $args[$captureInputIndex]="gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra"
 $args[-1]="$PSScriptRoot/$Take.mp4"
 $recorders+=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture-peer.log" -RedirectStandardOutput "$out/capture-peer.stdout" -PassThru
 Start-Sleep -Milliseconds 600;if(@($recorders|Where-Object HasExited).Count){throw 'Capture failed'}
 $monitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-domino-scenes','17000') -WindowStyle Hidden -RedirectStandardOutput "$out/power-scenes.ndjson" -RedirectStandardError "$out/power-scenes.stderr" -PassThru
 $rawMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-midnight','17000') -WindowStyle Hidden -RedirectStandardOutput "$out/midnight.ndjson" -RedirectStandardError "$out/midnight.stderr" -PassThru
 $w.Restart();$cast=$false;$stood=$false;$secondCast=$false
 while($w.ElapsedMilliseconds-lt16000){
  if(!$cast-and$w.ElapsedMilliseconds-ge1100){

   I client 255 1800;$cast=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native LT held 1800ms then released'})}
  if($cast-and!$secondCast-and$w.ElapsedMilliseconds-ge5500){
   $targets=Q client '--domino-targets';$peerMarks=S host '.domino-world.json'
   [ordered]@{source=$targets;peer=$peerMarks}|ConvertTo-Json -Depth 12|Set-Content "$out/targets-before-reselection.json"
   if(!$targets.valid-or$targets.targets.Count-ne1-or$peerMarks.marks.Count-ne1-or$targets.targets[0].handle-ne$peerMarks.marks[0].target){throw 'One corresponding source/peer selected target required'}
   I client 255 1800;$secondCast=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Second native LT1800 toward already marked target while crouched'})
  }
  if(!$stood-and$w.ElapsedMilliseconds-ge9500){if($expectedStance){& "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180|Out-Null};$stood=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native standing recovery after crouched cast'})}
  $m=S host '.movement.json';$stream=S host '.shadow-stream.json'
  $fx=if(($rows.Count%4)-eq0){OptionalFx client}else{$null}
  $rows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;player=(Q client '--player-states');sword=(Q client '--player-sword');movement=$m;stream=$stream;sourceStream=(S client '.shadow-stream.json');effects=$fx})
  if(!$m.active-or!$stream.ready-or!$stream.armed){throw "Native transport/presentation retired: $($stream.result)"}
  Start-Sleep -Milliseconds 25
 }
 I client;$manaAfter=& "$PSScriptRoot/Read-NativeMana.ps1" -Role client -Evidence "$out/mana-after.json"
 $monitor.WaitForExit(4000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){throw 'Native scene watcher failed'}
 $fastScenes=@(Get-Content "$out/power-scenes.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $scenes=@($fastScenes|Where-Object {$_.valid-and$_.variant-in19,20,21,22})
 $phases=@(19,20,22);if(@($scenes|Where-Object variant -eq 21).Count){$phases=@(19,20,21,22)}
 foreach($v in $phases){if(!@($scenes|Where-Object variant -eq $v).Count){throw "Native strict Domino phase $v absent"}}
 if(@($scenes|Where-Object {!$_.armHandle-or!$_.equipmentHandle}).Count-or@($scenes.armHandle|Sort-Object -Unique).Count-ne1-or@($scenes.equipmentHandle|Sort-Object -Unique).Count-ne1){throw 'Native Domino arm/equipment ownership changed'}
 $final=@($scenes|Where-Object subscene -eq 'Cast_final_Select')
 $recovery=@($final|Where-Object {$_.activeStates.Count-eq1-and$_.activeStates[0]-eq$groundState-and$_.timeMs-ge150})
 if(!$secondCast-or$final.Count-lt8-or$recovery.Count-lt4-or($recovery.timeMs|Measure-Object -Maximum).Maximum-lt300-or@($final|Where-Object {$_.stance-ne$expectedStance-or$_.durationMs-ne400-or$_.timeMs-gt400}).Count){throw 'Owned crouched final400 recovery not verified'}
 if(!@($final|Where-Object {$_.activeStates-contains72}).Count){throw 'Native casting state absent during final entry'}
 $final|ConvertTo-Json -Depth 10|Set-Content "$out/final-select-native-proof.json"
 $lateFinal=@($rows|Where-Object {$_.ms-ge7000-and$_.ms-le8500-and$_.movement.powerVariant-eq21-and$_.movement.powerWeight-ge.25})
 if(!$lateFinal.Count){throw 'Crouched final selection gesture absent on peer'}
 $lateFinal|ConvertTo-Json -Depth 16|Set-Content "$out/final-select-peer-proof.json"
 $afterTargets=Q client '--domino-targets';$afterPeer=S host '.domino-world.json'
 [ordered]@{source=$afterTargets;peer=$afterPeer}|ConvertTo-Json -Depth 12|Set-Content "$out/targets-after-reselection.json"
 if(!$afterTargets.valid-or$afterTargets.targets.Count-or$afterPeer.marks.Count){throw 'Native and peer selected mark did not retire after reselection'}
 $checks.Add('Native crouched final400 preserves owned scene through the matching grounded recovery state past300ms and reaches peer; native and cosmetic selected mark retire together')
 $checks.Add('Strict native Domino reader confirms observed crouched phases including ordinary selection when targeted, exact state72 or exit recovery and reciprocal Emily arm/body ownership')
 foreach($variant in $phases){if(!@($rows|Where-Object {$_.movement.powerVariant-eq$variant-and$_.movement.powerWeight-gt.5}).Count){throw "Remote phase $variant not observed"}}
 $eventsSeen=@($rows|Where-Object {$_.movement.powerVariant-in19,20,21,22-and$_.movement.powerWeight-gt.5}|ForEach-Object {$_.movement.powerEvent}|Select-Object -Unique)
 $phaseCoverage=@(foreach($variant in $phases){
  $nativeIds=@($scenes|Where-Object variant -eq $variant|ForEach-Object handle|Select-Object -Unique)
  $remoteIds=@($rows|Where-Object {$_.movement.powerVariant-eq$variant-and$_.movement.powerWeight-gt.5}|ForEach-Object {$_.movement.powerEvent}|Select-Object -Unique)
  if(!$nativeIds.Count-or$nativeIds.Count-ne$remoteIds.Count){throw "Phase $variant source/remote incarnation mismatch: $($nativeIds.Count)/$($remoteIds.Count)"}
  [pscustomobject]@{variant=$variant;nativeHandles=$nativeIds;remoteEvents=$remoteIds}
 })
 $phaseCoverage|ConvertTo-Json -Depth 5|Set-Content "$out/phase-incarnations.json"
 $checks.Add('Observed native crouched subscene transitions within the owned cast produce corresponding distinct remote phase events through native Steam')
 $continuity=@($rows|Where-Object {$_.ms-ge900-and$_.ms-le11500})
 if($continuity.Count-lt30-or@($continuity|Where-Object {$_.movement.weight-lt.95-or$_.movement.clockRejection.count-ne0}).Count){throw 'Domino body root continuity failed during native cast'}
 [ordered]@{samples=$continuity.Count;minimumBodyWeight=($continuity|ForEach-Object {$_.movement.weight}|Measure-Object -Minimum).Minimum;rejectionCounts=@($continuity|ForEach-Object {$_.movement.clockRejection.count}|Sort-Object -Unique)}|ConvertTo-Json|Set-Content "$out/continuity.json"
 $checks.Add('Native cast keeps full root/body presentation without clock rejections; aiming placement settled before stream activation')

 if($m.powerWeight-gt.02-or$m.weight-lt.9-or$stream.remoteState-ne0){throw 'Remote cast did not recover to standing'}
 $sword=Q host '--avatar-sword' (S host).probeActorHandle;$sword|ConvertTo-Json -Depth 8|Set-Content "$out/avatar-sword-after.json"
 if($sword.equipped-eq[bool]$Holstered-or(Q client '--player-sword').equipped-eq[bool]$Holstered){throw 'Native sword selection changed'}
 foreach($r in 'host','client'){if((Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Remote avatar health changed'}}
 [ordered]@{source=(OptionalFx client);peer=(OptionalFx host)}|ConvertTo-Json -Depth 12|Set-Content "$out/world-effects-after.json"
 $checks.Add('Remote body recovers, original sword selections and all avatar health retained; source and peer world effects recorded without claiming shared NPC outcomes')
 $nativeMarks=@($rows|ForEach-Object {$_.effects.effects}|Where-Object name -like '*/phantasmal_link/target/*')
 # Native target selection is allowed; existing scoped cosmetic replication is recorded separately.
 $castRows=@($rows|Where-Object {$_.movement.powerVariant-in19,20,21,22-and$_.movement.powerWeight-gt.5})
 if(!$castRows.Count-or@($castRows|Where-Object {$_.player.stance-ne$expectedStance-or[Math]::Abs($_.movement.stanceMix-$expectedStance)-gt.02-or$_.movement.presentedDrawn-eq[bool]$Holstered-or$_.sword.equipped-eq[bool]$Holstered}).Count){throw 'Cast left native or complete-body remote crouched context'}
 $standing=@($rows|Where-Object {$_.ms-gt11500-and$_.player.stance-eq0-and$_.movement.stanceMix-lt.02-and$_.movement.weight-gt.95})
 if($standing.Count-lt5){throw 'Native standing recovery not observed'}
 [ordered]@{stance=$Stance;holstered=[bool]$Holstered;move=0;finalSamples=$final.Count;finalRecoverySamples=$recovery.Count;finalPeerSamples=$lateFinal.Count;observedPhases=$phases;nativeTargetEffectSamples=$nativeMarks.Count;crouchedPhaseSamples=$castRows.Count;standingRecoverySamples=$standing.Count;minimumCrouchedWeight=($castRows.movement.stanceMix|Measure-Object -Minimum).Minimum}|ConvertTo-Json|Set-Content "$out/crouch-context-summary.json"
 $checks.Add('All observed native phases retain whole-body crouched context, then recover to standing without root-clock rejection and preserve any native target events')
 C client 'coop_domino_fixture off';$w.Restart()
 do{$f=S client '.domino-fixture.json';if(!$f.active){break};Start-Sleep -Milliseconds 75}while($w.ElapsedMilliseconds-lt6500)
 if($f.active-or$f.result-ne'quantity_and_selection_restored'){throw 'Native Domino inventory did not retire'}
 foreach($r in 'host','client'){$after=Q $r '--player-inventory';$after|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-after-$r.json";if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($after.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Inventory quantities differ'};if((Q $r '--health' 1).current-ne56){throw 'Player health changed'}}
 $checks.Add('Both native inventories restore and player health stays unchanged; world effects require separate checkpoint cleanup')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_domino_fixture off'}catch{}}
 foreach($capture in $recorders){$capture.WaitForExit(18000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode){$failure=($failure+'; capture failed').TrimStart('; ')}}
 if($monitor){$monitor.WaitForExit(18000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){$failure=($failure+'; scene watcher failed').TrimStart('; ')}}
 if($rawMonitor){$rawMonitor.WaitForExit(18000)|Out-Null;if(!$rawMonitor.HasExited-or$rawMonitor.ExitCode){$failure=($failure+'; raw watcher failed').TrimStart('; ')}}
 foreach($r in 'host','client'){try{C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 20|Set-Content "$out/states.json"
 if($dirty){try{& "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log";foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'}};$p=Q client '--player-domino';if($p.quantity-or$p.equipped-or$p.started){throw 'Domino remains after checkpoint'};$fx=Q client '--domino-effects';if(@($fx.effects).Count){throw 'Domino effects remain after checkpoint'};$checks.Add('Both isolated checkpoints restore clean health/inventory and retire native Domino effects')}catch{$failure=($failure+'; checkpoint restore: '+$_.Exception.Message).TrimStart('; ')}}
 try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All fourteen protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
 [ordered]@{passed=(!$failure);failure=$failure;source='client';stance=$Stance;holstered=[bool]$Holstered;move=0;checks=$checks;events=$events;dll=$initialDll;probe=$initialProbe;bankSha256=(Get-FileHash "$PSScriptRoot/domino-context-banks-v243/emily-domino-context-bank.bin").Hash;scope='Native source Domino acquisition and native controller casting drives the explicit Emily stance/equipment phases then standing recovery through native Steam, with source scene timing and unchanged equipment state. One native scene incarnation changes subscenes. This take verifies one native target selection and reselection, exact final400 crouched recovery, corresponding peer gesture and source/cosmetic mark retirement. Earlier scoped mark/beam presentation proofs remain separate; NPC AI and shared outcomes are unfinished. No claim of complete Domino support or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged.'}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_NATIVE_DOMINO_OBSERVATION $($checks.Count) checks"

