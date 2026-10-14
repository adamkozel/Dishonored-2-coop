param([ValidatePattern('^[a-z0-9-]+$')][string]$Take,[ValidateRange(-15,45)][int]$Pitch=12)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(!$Take-or(Test-Path $out)){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
$pids=@{};$inventory=@{};$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$recorders=@();$capture=$null;$monitor=$null;$dirty=$false;$failure=$null
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
 $dirty=$true;C client 'setviewpos -44.39 -153.27 68.1 90 0';C host 'setviewpos -47.4 -156.3 68.1 203 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch -3 -Evidence "$out/observer-camera.json" -Quick
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch 3 -Evidence "$out/source-camera.json" -Quick
 $filterBefore=S client '.domino-partner-filter.json';$filterBefore|ConvertTo-Json|Set-Content "$out/filter-before.json"
 $before=Q client '--player-domino';$before|ConvertTo-Json -Depth 7|Set-Content "$out/before.json"
 if($before.quantity-or$before.equipped-or$before.started){throw 'Domino already owned or active'}
 $manaBefore=& "$PSScriptRoot/Read-NativeMana.ps1" -Role client -Evidence "$out/mana-before.json"
 $dirty=$true;C client 'coop_domino_fixture 40000';$w=[Diagnostics.Stopwatch]::StartNew()
 do{$f=S client '.domino-fixture.json';if($f.result-eq'native_domino_ready'){break};Start-Sleep -Milliseconds 60}while($w.ElapsedMilliseconds-lt6000)
 if($f.result-ne'native_domino_ready'){throw "Native Domino acquisition failed: $($f.result)"}
 $power=Q client '--player-domino';$power|ConvertTo-Json -Depth 7|Set-Content "$out/enabled.json"
 if(!$power.equipped-or!$power.started-or!$power.instantiated-or$power.quantity-ne1){throw 'Native Domino equipment not confirmed'}
 $checks.Add('Exact native Emily Domino item acquired and selected without directly invoking power or VFX')
    $catalog=Q client '--world-npcs';$catalog|ConvertTo-Json -Depth 8|Set-Content "$out/aim-npc-catalog.json"
    $eligible=@($catalog.npcs|Where-Object {$_.unique-and$_.reciprocal-and$_.spawnerName-eq'ld_spawner_guard_tutorial_2'-and$_.definition-eq'models/characters/def/generic/dunwall_guard_male_small_01.def'-and[Math]::Abs([double]$_.position[2]-66.4)-lt.5-and[Math]::Pow([double]$_.position[0]+44.39,2)+[Math]::Pow([double]$_.position[1]+153.27,2)-lt400}|Sort-Object {[Math]::Pow([double]$_.position[0]+44.39,2)+[Math]::Pow([double]$_.position[1]+153.27,2)})
    if(!$eligible.Count){throw 'No verified upper-floor tutorial guard within twenty metres'}
    $target=$eligible[0];$yaw=[Math]::Atan2([double]$target.position[1]+153.27,[double]$target.position[0]+44.39)*180/[Math]::PI
    C client ('setviewpos -44.39 -153.27 68.1 {0} 0' -f $yaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
    [ordered]@{target=$target;yaw=$yaw;placement='Closer v214 source position; v215 measured floor and three stable native motion samples required before streaming'}|ConvertTo-Json -Depth 6|Set-Content "$out/aim-guard.json"
 $observerYaw=[Math]::Atan2([double]$target.position[1]+156.3,[double]$target.position[0]+47.4)*180/[Math]::PI
 C host ('setviewpos -47.4 -156.3 68.1 {0} 0' -f $observerYaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch 3 -Evidence "$out/mark-observer-camera.json" -Quick
 Start-Sleep -Milliseconds 750
 $grounded=Q client '--player-states';$grounded|ConvertTo-Json -Depth 8|Set-Content "$out/source-grounded-after-aim.json"
 $floorSamples=@($grounded)
 foreach($n in 1..2){Start-Sleep -Milliseconds 100;$floorSamples+=Q client '--player-states'}
 $floorSamples|ConvertTo-Json -Depth 8|Set-Content "$out/source-floor-stability.json"
 foreach($sample in $floorSamples){
  if(!$sample.motionValid-or$sample.motion-ne0-or$sample.stance-ne0-or[Math]::Abs([double]$sample.position[2]-66.4036484)-gt.03){throw 'Source has not settled on the observed v215 floor at the closer aim position'}
  for($axis=0;$axis-lt3;$axis++){if([Math]::Abs([double]$sample.position[$axis]-[double]$grounded.position[$axis])-gt.002){throw 'Source position changed during pre-stream floor stability check'}}
 }

 Copy-Item "$PSScriptRoot/domino-banks-v192/emily-domino-bank.bin" "$root/work/lab-instances/host/coop-status.json.movement.bin"
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 600;C host 'coop_movement_bank on'
 $w.Restart();do{$m=S host '.movement.json';$stream=S host '.shadow-stream.json';if($m.active-and$m.version-eq47-and$m.weight-gt.9-and$m.presentedDrawn-and$stream.ready-and$stream.armed){break};Start-Sleep -Milliseconds 50}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne47-or$m.weight-lt.9-or!$m.presentedDrawn-or!$stream.ready-or!$stream.armed){throw 'Exact Domino bank and native Steam stream not ready'}
 $checks.Add('Native Steam transport and exact bank47/289 remote Emily standing armed context ready')
 Start-Sleep -Milliseconds 350
 & "$PSScriptRoot/Aim-NativeDominoGuard-v218.ps1" -Role client -Evidence "$out/native-guard-aim.json"
 $worldBaseline=S host '.domino-world.json';$worldBaseline|ConvertTo-Json -Depth 8|Set-Content "$out/world-before-capture.json"


 $hwnd=(Get-Process -Id $pids.client).MainWindowHandle.ToInt64();if(!$hwnd){throw 'No source window'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','23','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take-source.mp4")
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();$capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 $recorders+=$capture
 $hwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64()
 $captureInputIndex=[Array]::IndexOf($args,($args|Where-Object {$_ -like 'gfxcapture=*'}))
 if($captureInputIndex-lt0){throw 'Capture input argument missing'}
 $args[$captureInputIndex]="gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra"
 $args[-1]="$PSScriptRoot/$Take.mp4"
 $recorders+=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture-peer.log" -RedirectStandardOutput "$out/capture-peer.stdout" -PassThru
 Start-Sleep -Milliseconds 600;if(@($recorders|Where-Object HasExited).Count){throw 'Capture failed'}
 $monitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-domino-scenes','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/power-scenes.ndjson" -RedirectStandardError "$out/power-scenes.stderr" -PassThru
 $w.Restart();$cast=$false
 while($w.ElapsedMilliseconds-lt14000){
  if(!$cast-and$w.ElapsedMilliseconds-ge1100){

   I client 255 1800;$cast=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native LT held 1800ms then released'})}
  $m=S host '.movement.json';$stream=S host '.shadow-stream.json'
  $fx=if(($rows.Count%4)-eq0){OptionalFx client}else{$null}
  $rows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;player=(Q client '--player-states');movement=$m;stream=$stream;effects=$fx;dominoSource=(S client '.domino-world.json');dominoPeer=(S host '.domino-world.json')})
  if(!$m.active-or!$stream.ready-or!$stream.armed){throw "Native transport/presentation retired: $($stream.result)"}
  Start-Sleep -Milliseconds 25
 }
 I client;$manaAfter=& "$PSScriptRoot/Read-NativeMana.ps1" -Role client -Evidence "$out/mana-after.json"
 $monitor.WaitForExit(4000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){throw 'Native scene watcher failed'}
 $fastScenes=@(Get-Content "$out/power-scenes.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $scenes=@($fastScenes|Where-Object {$_.valid-and$_.variant-in19,20,21,22})
 foreach($v in 19,20,21,22){if(!@($scenes|Where-Object variant -eq $v).Count){throw "Native strict Domino phase $v absent"}}
 if(@($scenes|Where-Object {!$_.armHandle-or!$_.equipmentHandle}).Count-or@($scenes.armHandle|Sort-Object -Unique).Count-ne1-or@($scenes.equipmentHandle|Sort-Object -Unique).Count-ne1){throw 'Native Domino arm/equipment ownership changed'}
 $checks.Add('Strict native Domino reader confirms all four phases, exact state72 or exit recovery and reciprocal Emily arm/body ownership')
 foreach($variant in 19,20,21,22){if(!@($rows|Where-Object {$_.movement.powerVariant-eq$variant-and$_.movement.powerWeight-gt.5}).Count){throw "Remote phase $variant not observed"}}
 $eventsSeen=@($rows|Where-Object {$_.movement.powerVariant-in19,20,21,22-and$_.movement.powerWeight-gt.5}|ForEach-Object {$_.movement.powerEvent}|Select-Object -Unique)
 $phaseCoverage=@(foreach($variant in 19,20,21,22){
  $nativeIds=@($scenes|Where-Object variant -eq $variant|ForEach-Object handle|Select-Object -Unique)
  $remoteIds=@($rows|Where-Object {$_.movement.powerVariant-eq$variant-and$_.movement.powerWeight-gt.5}|ForEach-Object {$_.movement.powerEvent}|Select-Object -Unique)
  if(!$nativeIds.Count-or$nativeIds.Count-ne$remoteIds.Count){throw "Phase $variant source/remote incarnation mismatch: $($nativeIds.Count)/$($remoteIds.Count)"}
  [pscustomobject]@{variant=$variant;nativeHandles=$nativeIds;remoteEvents=$remoteIds}
 })
 $phaseCoverage|ConvertTo-Json -Depth 5|Set-Content "$out/phase-incarnations.json"
 $checks.Add('Four native subscene transitions within the owned cast produce corresponding distinct remote phase events through native Steam')
 $continuity=@($rows|Where-Object {$_.ms-ge900-and$_.ms-le5500})
 if($continuity.Count-lt30-or@($continuity|Where-Object {$_.movement.weight-lt.95-or$_.movement.clockRejection.count-ne0}).Count){throw 'Domino body root continuity failed during native cast'}
 [ordered]@{samples=$continuity.Count;minimumBodyWeight=($continuity|ForEach-Object {$_.movement.weight}|Measure-Object -Minimum).Minimum;rejectionCounts=@($continuity|ForEach-Object {$_.movement.clockRejection.count}|Sort-Object -Unique)}|ConvertTo-Json|Set-Content "$out/continuity.json"
 $checks.Add('Native cast keeps full root/body presentation without clock rejections; aiming placement settled before stream activation')

 if($m.powerWeight-gt.02-or$m.weight-lt.9-or$stream.remoteState-ne0){throw 'Remote cast did not recover to standing'}
 $sword=Q host '--avatar-sword' (S host).probeActorHandle;$sword|ConvertTo-Json -Depth 8|Set-Content "$out/avatar-sword-after.json"
 if(!$sword.equipped-or!(Q client '--player-sword').equipped){throw 'Native sword selection changed'}
 foreach($r in 'host','client'){if((Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Remote avatar health changed'}}
 $peerFx=Q host '--domino-effects';$peerFx|ConvertTo-Json -Depth 8|Set-Content "$out/peer-effects.json"
 $sourceTargets=Q client '--domino-targets';$sourceTargets|ConvertTo-Json -Depth 8|Set-Content "$out/source-targets.json"
 $sourceWorld=S client '.domino-world.json';$peerWorld=S host '.domino-world.json'
 [ordered]@{source=$sourceWorld;peer=$peerWorld}|ConvertTo-Json -Depth 8|Set-Content "$out/world-status.json"
 if(!$sourceTargets.valid-or!$sourceTargets.present-or$sourceTargets.targets.Count-ne1-or!$sourceTargets.targets[0].hasMark){throw 'Exact native single-target mark unavailable'}
 if($peerWorld.marks.Count-ne1-or$peerWorld.started-ne($worldBaseline.started+1)-or$peerWorld.refused-ne$worldBaseline.refused-or$peerWorld.rejected-ne$worldBaseline.rejected-or$peerWorld.updated-lt($worldBaseline.updated+15)-or!$peerWorld.leased){throw 'Owned remote mark stream did not remain stable'}
 $sourceMark=$sourceTargets.targets[0];$peerMark=$peerWorld.marks[0]
 if($peerMark.sourceId-ne$sourceMark.mark-or$peerMark.target-ne$sourceMark.handle-or$peerMark.owner-ne(S host).probeActorHandle){throw 'Remote mark incarnation or local presentation ownership differs'}
 $matches=@($peerFx.effects|Where-Object {$_.id-eq$peerMark.id-and$_.owner-eq$peerMark.owner-and$_.name-eq'models/effects/gameplay/powers/phantasmal_link/target/phlink_validated_01.fx'})
 if($matches.Count-ne1-or$peerFx.effects.Count-ne1){throw 'Expected one local native cosmetic mark without duplicates'}
 $delta=0;for($k=0;$k-lt3;$k++){$delta+=[Math]::Pow([double]$matches[0].origin[$k]-[double]$sourceMark.effectPosition[$k],2)}
 if($delta-gt.25){throw 'Receiver mark is not near observed source target position'}
 $filterAfter=S client '.domino-partner-filter.json';$filterAfter|ConvertTo-Json|Set-Content "$out/filter-after.json"
 if(!$filterAfter.installed-or$filterAfter.otherAccepted-le$filterBefore.otherAccepted-or$filterAfter.readFailed-ne$filterBefore.readFailed){throw 'Ordinary guard picker forwarding not verified'}
 $checks.Add('Native picker still forwards ordinary guard candidates with partner filter installed and no ownership read failures')
 $checks.Add('Native validated mark ID and target incarnation traverse Steam; exactly one receiver-owned native effect tracks the observed world position without duplicate target or power methods')
 $checks.Add('Body phases recover and native sword/player/avatar health stay unchanged; target positions remain source-world cosmetic coordinates, not proof of synchronized NPC AI')
 $stopWatch=[Diagnostics.Stopwatch]::StartNew()
 C host 'coop_movement_bank off';$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Receiver presentation disabled; native particle fade begins'})
 Start-Sleep -Milliseconds 700
 $stopped=S host '.domino-world.json';$stopped|ConvertTo-Json -Depth 8|Set-Content "$out/mark-stopped.json"
 if($stopped.marks.Count-or$stopped.stopped-ne($worldBaseline.stopped+1)){throw 'Receiver mark slot not stopped within 700ms'}
 $fade=[Collections.Generic.List[object]]::new();$fadeFailure=$null;$stopOrigin=$null
 do{
  $afterStopFx=Q host '--domino-effects';$afterStopWorld=S host '.domino-world.json'
  $fade.Add([pscustomobject]@{ms=$stopWatch.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;effects=$afterStopFx;status=$afterStopWorld})
  if($afterStopWorld.marks.Count-or$afterStopWorld.started-ne$peerWorld.started-or$afterStopWorld.updated-ne$stopped.updated-or$afterStopWorld.stopped-ne$stopped.stopped){$fadeFailure='Stopped mark resumed or ownership changed';break}
  if(@($afterStopFx.effects).Count){
   if(@($afterStopFx.effects).Count-ne1-or$afterStopFx.effects[0].id-ne$peerMark.id-or$afterStopFx.effects[0].owner-ne$peerMark.owner){$fadeFailure='Unexpected FX appeared during retirement';break}
   if($null-eq$stopOrigin){$stopOrigin=@($afterStopFx.effects[0].origin)}elseif(Compare-Object $stopOrigin @($afterStopFx.effects[0].origin)){$fadeFailure='Stopped native mark transform continued moving';break}
  }else{break}
  Start-Sleep -Milliseconds 150
 }while($stopWatch.ElapsedMilliseconds-lt5000)
 $fade|ConvertTo-Json -Depth 12|Set-Content "$out/mark-stop-timeline.json"
 if($fadeFailure){throw $fadeFailure};if(@($afterStopFx.effects).Count){throw 'Native mark particle record survives bounded five-second fade'}
 $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native owned mark FX record absent after bounded fade'})
 $checks.Add('Receiver presentation stop retires mark ownership within 700ms with no new effects or transform updates; native particle record disappears within five seconds, observed separately from stream shutdown')
 C client 'coop_domino_fixture off';$w.Restart()
 do{$f=S client '.domino-fixture.json';if(!$f.active){break};Start-Sleep -Milliseconds 75}while($w.ElapsedMilliseconds-lt6500)
 if($f.active-or$f.result-ne'quantity_and_selection_restored'){throw 'Native Domino inventory did not retire'}
 foreach($r in 'host','client'){$after=Q $r '--player-inventory';$after|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-after-$r.json";if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($after.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Inventory quantities differ'};if((Q $r '--health' 1).current-ne56){throw 'Player health changed'}}
 $checks.Add('Both native inventories restore and player health stays unchanged; world effects require separate checkpoint cleanup')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_domino_fixture off';C $r 'coop_movement_fixture off'}catch{}}
 foreach($capture in $recorders){$capture.WaitForExit(18000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode){$failure=($failure+'; capture failed').TrimStart('; ')}}
 if($monitor){$monitor.WaitForExit(18000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){$failure=($failure+'; scene watcher failed').TrimStart('; ')}}
 $rows|ConvertTo-Json -Depth 20|Set-Content "$out/states.json"
 if($dirty){try{& "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log";foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'}};$p=Q client '--player-domino';if($p.quantity-or$p.equipped-or$p.started){throw 'Domino remains after checkpoint'};$fx=Q client '--domino-effects';if(@($fx.effects).Count){throw 'Domino effects remain after checkpoint'};$checks.Add('Both isolated checkpoints restore clean health/inventory and retire native Domino effects')}catch{$failure=($failure+'; checkpoint restore: '+$_.Exception.Message).TrimStart('; ')}}
 try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All fourteen protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
 [ordered]@{passed=(!$failure);failure=$failure;source='client';checks=$checks;events=$events;dll=$initialDll;probe=$initialProbe;bankSha256=(Get-FileHash "$PSScriptRoot/domino-banks-v192/emily-domino-bank.bin").Hash;scope='Native source Domino acquisition and targeted controller selection drive four standing armed Emily full-body phases through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. Remote validated target mark is sourced by exact native ownership and recreated as a bounded cosmetic FX; target handles are opaque and no link, NPC or damage method runs on the receiver. Source-world position is preserved; corresponding NPC AI/pose and link gameplay remain unsynchronized. No claim of complete Domino support, NPC attachment accuracy or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged.'}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_NATIVE_DOMINO_OBSERVATION $($checks.Count) checks"
