param([ValidatePattern('^[a-z0-9-]+$')][string]$Take,[ValidateRange(-15,45)][int]$Pitch=12)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(!$Take-or(Test-Path $out)){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
$pids=@{};$inventory=@{};$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$recorders=@();$capture=$null;$monitor=$null;$targetMonitor=$null;$worldMonitor=$null;$dirty=$false;$failure=$null
$initialDll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;$initialProbe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$lt=0,$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button Neutral -LeftTrigger $lt -DurationMs $ms|Out-Null}
function S($r,$suffix=''){for($a=0;$a-lt8;$a++){try{$v=Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json;if($null-ne$v){return $v}}catch{};Start-Sleep -Milliseconds 15};throw 'Status unavailable'}
function Q($r,$mode,$arg=0){for($a=0;$a-lt8;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read $mode failed"}
function OptionalFx($r){$raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$r] --mesmerize-effects;$code=$LASTEXITCODE;if($code){return [pscustomobject]@{stable=$false;exitCode=$code}};return($raw|ConvertFrom-Json)}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid;I $r
  foreach($cmd in 'coop_mesmerize_visual off','coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_potion_fixture off','coop_bendtime_fixture off','coop_darkvision_fixture off','coop_mesmerize_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Clean living fixture required'}
  $inventory[$r]=Q $r '--player-inventory';$inventory[$r]|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-before-$r.json"
 }
 $linksBefore=S host '.mesmerize-links.json';$linksBefore|ConvertTo-Json -Depth 6|Set-Content "$out/links-before.json"
 $visualBefore=S host '.mesmerize-visual.json';$worldBefore=S host '.mesmerize-world-stream.json'
 $filterBefore=S client '.mesmerize-partner-filter.json';$filterBefore|ConvertTo-Json|Set-Content "$out/filter-before.json"
 $visualBefore|ConvertTo-Json|Set-Content "$out/visual-before.json"
 $catalog=Q client '--world-npcs';$catalog|ConvertTo-Json -Depth 8|Set-Content "$out/source-npcs-before.json"
 # Fixed native floor placements from inspected v99; patrol positions are
 # evidence of eligible nearby targets, never camera placement instructions.
 $sx=-48.59;$sy=-155.07;$sz=68.1
 $npcs=@($catalog.npcs|Where-Object {
  $_.spawnerName-in@('ld_spawner_guard_tutorial_1','ld_spawner_guard_tutorial_2')-and
  [Math]::Abs([double]$_.position[2]-66.4)-lt0.5-and
  [Math]::Pow([double]$_.position[0]+48.59,2)+[Math]::Pow([double]$_.position[1]+150.87,2)-lt100
 })
 if(!$npcs.Count){throw 'No upper-floor tutorial guard near the proven balcony cast placement'}
 foreach($n in $npcs){if(!$n.unique-or!$n.reciprocal-or$n.definition-ne'models/characters/def/generic/dunwall_guard_male_small_01.def'){throw 'Tutorial guard identity ambiguous'}}
 $npc=$npcs[0];$allowedTargetHandles=@($npcs|ForEach-Object {[Convert]::ToUInt32($_.handle.Substring(2),16)})
 $npcs|ConvertTo-Json -Depth 6|Set-Content "$out/eligible-native-guards.json"
 $dirty=$true
 C client ('setviewpos {0} {1} {2} 90 0' -f $sx,$sy,$sz)
 C host ('setviewpos {0} {1} {2} 203 0' -f ($sx+2.8),($sy+1.2),$sz)

 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch 15 -Evidence "$out/observer-camera.json" -Quick
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch $Pitch -Evidence "$out/source-camera.json" -Quick
 foreach($r in 'host','client'){
  $placement=Q $r '--player-stance';$placement|ConvertTo-Json -Depth 5|Set-Content "$out/placement-$r.json"
  if([Math]::Abs([double]$placement.position[2]-($sz-1.7))-gt0.5){throw 'Player did not remain on the native upper-floor surface'}
 }
 $before=Q client '--player-mesmerize';$before|ConvertTo-Json -Depth 7|Set-Content "$out/before.json"
 if($before.quantity-or$before.equipped-or$before.started){throw 'Mesmerize already owned or active'}
 $manaBefore=& "$PSScriptRoot/Read-NativeMana.ps1" -Role client -Evidence "$out/mana-before.json"
 $dirty=$true;C client 'coop_mesmerize_fixture 40000';$w=[Diagnostics.Stopwatch]::StartNew()
 do{$f=S client '.mesmerize-fixture.json';if($f.result-eq'native_mesmerize_ready'){break};Start-Sleep -Milliseconds 60}while($w.ElapsedMilliseconds-lt6000)
 if($f.result-ne'native_mesmerize_ready'){throw "Native Mesmerize acquisition failed: $($f.result)"}
 $power=Q client '--player-mesmerize';$power|ConvertTo-Json -Depth 7|Set-Content "$out/enabled.json"
 if(!$power.equipped-or!$power.started-or!$power.instantiated-or$power.quantity-ne1){throw 'Native Mesmerize equipment not confirmed'}
 $checks.Add('Exact native Emily Mesmerize item acquired and selected without directly invoking power or VFX')
 Copy-Item "$PSScriptRoot/mesmerize-banks-v77/emily-mesmerize-bank.bin" "$root/work/lab-instances/host/coop-status.json.movement.bin"
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 600;C host 'coop_movement_bank on'
 $w.Restart();do{$m=S host '.movement.json';$stream=S host '.shadow-stream.json';if($m.active-and$m.version-eq45-and$m.weight-gt.9-and$m.presentedDrawn-and$stream.ready-and$stream.armed){break};Start-Sleep -Milliseconds 50}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne45-or$m.weight-lt.9-or!$m.presentedDrawn-or!$stream.ready-or!$stream.armed){throw 'Exact Mesmerize bank and native Steam stream not ready'}
 $checks.Add('Native Steam transport and exact bank45/276 remote Emily standing armed context ready')

 $hwnd=(Get-Process -Id $pids.client).MainWindowHandle.ToInt64();if(!$hwnd){throw 'No source window'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','38','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take-source.mp4")
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();$capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 $recorders+=$capture
 $hwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64()
 $captureInputIndex=[Array]::IndexOf($args,($args|Where-Object {$_ -like 'gfxcapture=*'}))
 if($captureInputIndex-lt0){throw 'Capture input argument missing'}
 $args[$captureInputIndex]="gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra"
 $args[-1]="$PSScriptRoot/$Take.mp4"
 $recorders+=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture-peer.log" -RedirectStandardOutput "$out/capture-peer.stdout" -PassThru
 Start-Sleep -Milliseconds 600;if(@($recorders|Where-Object HasExited).Count){throw 'Capture failed'}
 $targetMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-mesmerize-targets','36000') -WindowStyle Hidden -RedirectStandardOutput "$out/targets.ndjson" -RedirectStandardError "$out/targets.stderr" -PassThru
 $monitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-power-scenes','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/power-scenes.ndjson" -RedirectStandardError "$out/power-scenes.stderr" -PassThru
 $worldMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-mesmerize-world','36000') -WindowStyle Hidden -RedirectStandardOutput "$out/native-world.ndjson" -RedirectStandardError "$out/native-world.stderr" -PassThru
 $w.Restart();$cast=$false;$duplicated=$false;$manaCommit=$null;$worldFramed=$false
 while($w.ElapsedMilliseconds-lt36000){
  if(!$cast-and$w.ElapsedMilliseconds-ge1100){I client 255 1800;$cast=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native LT held 1800ms then released'})}
  if($cast-and!$manaCommit-and$w.ElapsedMilliseconds-ge5500){$manaCommit=& "$PSScriptRoot/Read-NativeMana.ps1" -Role client -Evidence "$out/mana-after-commit.json"}
  if(!$worldFramed-and$w.ElapsedMilliseconds-ge5500){
   $committed=Q client '--mesmerize-world';$committed|ConvertTo-Json -Depth 8|Set-Content "$out/source-world-committed.json"
   if(!$committed.present){throw 'Committed world unavailable for evidence framing'}
   # Frame the effect separately after the close body phase. Keep the
   # camera within the previously tested room corridor, not inside its east wall.
   $cameraX=[double]$committed.transform[0]+3;$cameraY=[double]$committed.transform[1]-4
   if([Math]::Abs($committed.transform[0]-$sx)-gt10-or[Math]::Abs($committed.transform[1]-$sy)-gt15){throw 'Committed NPC-control world outside source corridor'}
   C host ('setviewpos {0} {1} {2} 90 0' -f ($sx+0.8),$sy,$sz)
   & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch 3 -Evidence "$out/world-camera.json" -Quick
   $worldFramed=$true
  }
  if(!$duplicated-and$w.ElapsedMilliseconds-ge8000){Q host '--mesmerize-effects'|ConvertTo-Json -Depth 10|Set-Content "$out/observer-effects-active.json";C client 'coop_mesmerize_visual duplicate';$duplicated=$true}
  $m=S host '.movement.json';$stream=S host '.shadow-stream.json'
  $fx=if(($rows.Count%4)-eq0){OptionalFx client}else{$null}
  $rows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;player=(Q client '--player-states');movement=$m;stream=$stream;world=(S host '.mesmerize-world-stream.json');visual=(S host '.mesmerize-visual.json');filter=(S client '.mesmerize-partner-filter.json');eyes=(S host '.mesmerize-eyes.json');links=(S host '.mesmerize-links.json');sourceWorld=(S client '.mesmerize-world-stream.json');fixture=(S client '.mesmerize-fixture.json');effects=$fx})
  if(!$m.active-or!$stream.ready-or!$stream.armed){throw "Native transport/presentation retired: $($stream.result)"}
  Start-Sleep -Milliseconds 25
 }
 I client;$manaAfter=& "$PSScriptRoot/Read-NativeMana.ps1" -Role client -Evidence "$out/mana-after.json"
 $monitor.WaitForExit(4000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){throw 'Native scene watcher failed'}
 $fastScenes=@(Get-Content "$out/power-scenes.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $scenes=@($fastScenes|Where-Object {$_.valid-and$_.variant-in16,17,18})
 foreach($phase in 'in','loop','out'){if(!@($scenes|Where-Object asset -eq "midnight/gameplay/player/base/powers/power_mesmerize_cast_$phase.midnightscene").Count){throw "Native Mesmerize $phase strict phase not observed"}}
 if(@($scenes|Where-Object {$_.armSide-ne1-or$_.character-ne2-or!$_.armHandle}).Count){throw 'Owned Emily left-arm scope failed'}
 $checks.Add('Strict scene reader validates all three native phases with reciprocal owned Emily left arm')
 foreach($variant in 16,17,18){if(!@($rows|Where-Object {$_.movement.powerVariant-eq$variant-and$_.movement.powerWeight-gt.5}).Count){throw "Remote phase $variant not observed"}}
 $eventsSeen=@($rows|Where-Object {$_.movement.powerVariant-in16,17,18-and$_.movement.powerWeight-gt.5}|ForEach-Object {$_.movement.powerEvent}|Select-Object -Unique)
 $phaseCoverage=@(foreach($variant in 16,17,18){
  $nativeIds=@($scenes|Where-Object variant -eq $variant|ForEach-Object handle|Select-Object -Unique)
  $remoteIds=@($rows|Where-Object {$_.movement.powerVariant-eq$variant-and$_.movement.powerWeight-gt.5}|ForEach-Object {$_.movement.powerEvent}|Select-Object -Unique)
  if(!$nativeIds.Count-or$nativeIds.Count-ne$remoteIds.Count){throw "Phase $variant source/remote incarnation mismatch: $($nativeIds.Count)/$($remoteIds.Count)"}
  [pscustomobject]@{variant=$variant;nativeHandles=$nativeIds;remoteEvents=$remoteIds}
 })
 $phaseCoverage|ConvertTo-Json -Depth 5|Set-Content "$out/phase-incarnations.json"
 $checks.Add('All exact source scene incarnations, including native aim-loop renewal, match remote phase events through Steam')
 if($m.powerWeight-gt.02-or$m.weight-lt.9-or$stream.remoteState-ne0){throw 'Remote cast did not recover to standing'}
 $sword=Q host '--avatar-sword' (S host).probeActorHandle;$sword|ConvertTo-Json -Depth 8|Set-Content "$out/avatar-sword-after.json"
 if(!$sword.equipped-or!(Q client '--player-sword').equipped){throw 'Native sword selection changed'}
 foreach($r in 'host','client'){if((Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Remote avatar health changed'}}
 $visual=S host '.mesmerize-visual.json';$world=S host '.mesmerize-world-stream.json'
 if($visual.refused-ne$visualBefore.refused-or($visual.created-$visualBefore.created)-ne1-or($visual.started[0]-$visualBefore.started[0])-ne1-or($visual.started[1]-$visualBefore.started[1])-ne1-or($visual.started[2]-$visualBefore.started[2])-ne1){throw "Remote world animation lifecycle incomplete: $($visual|ConvertTo-Json -Compress)"}
 if($visual.active-or$visual.pendingHide-or$visual.fxActive-or($visual.fxStarted-$visualBefore.fxStarted)-ne1-or($visual.fxStopped-$visualBefore.fxStopped)-ne1-or$world.leased){throw 'Remote world lease/effect did not retire'}
 foreach($state in 1,3,2){if(!@($rows|Where-Object {$_.world.state-eq$state-and$_.world.leased}).Count){throw "Remote world state $state missing"}}
 if($world.rejected-le$worldBefore.rejected){throw 'Duplicate packet rejection not observed'}
 $remainingFx=Q host '--mesmerize-effects';$remainingFx|ConvertTo-Json -Depth 10|Set-Content "$out/observer-effects-after-retirement.json";if(@($remainingFx.effects).Count){throw 'Observer native body effect did not finish its native stop tail before end of recording'}
 $checks.Add('Native committed world incarnation and transform drive one cosmetic model, all three scene phases and one owned body effect through native Steam; source retirement hides the model and requests native effect stop; the native particle tail then leaves the manager')
 $checks.Add('Duplicate lifecycle packet rejected without a second model or effect; no late world revival')
 $checks.Add('Remote body recovers, original sword selections and all avatar health retained; remote model/effect has retired')
 if((!$manaCommit)-or$manaCommit.currentCandidate-ge$manaBefore.currentCandidate){throw 'Native Mesmerize mana expenditure not established'};$checks.Add('Native cast spends source mana without resource writes')
 $bodyFx=@($rows|ForEach-Object {$_.effects.effects}|Where-Object name -eq 'models/effects/gameplay/powers/mesmerize_v2/body/mesmerize_v2_01.fx')
 if(!$bodyFx.Count){throw 'Native Mesmerize world body effect not observed'};$checks.Add('Exact native world body effect exists after source cast; presentation/AI ownership still under investigation')
 $worldMonitor.WaitForExit(4000)|Out-Null;if(!$worldMonitor.HasExited-or$worldMonitor.ExitCode){throw 'Native lifetime watcher failed'}
 $nativeRows=@(Get-Content "$out/native-world.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $nativeActive=@($nativeRows|Where-Object {$_.valid-and$_.present-and$_.state-eq3-and$_.lifetimeObserved})
 if($nativeActive.Count-lt20){throw 'Too few native active countdown observations'}
 $last=$nativeActive[-1];$maximum=($nativeActive.remainingSeconds|Measure-Object -Maximum).Maximum
 if($maximum-lt3-or$last.remainingSeconds-gt.2-or$last.remainingSeconds-lt-.1){throw 'Native power retirement is not supported by an observed countdown reaching zero'}
 if(@($nativeRows|Where-Object {$_.valid-and$_.tick-gt$last.tick-and!$_.present}).Count-lt5){throw 'Native world did not leave its equipment after countdown expiry'}
 if(@($rows|Where-Object {!$_.fixture.active-or$_.fixture.restoring}).Count){throw 'Equipment fixture interrupted the native lifetime experiment'}
 $stillOwned=Q client '--player-mesmerize';$stillOwned|ConvertTo-Json -Depth 7|Set-Content "$out/equipment-after-natural-expiry.json"
 if(!$stillOwned.equipped-or!$stillOwned.started-or$stillOwned.quantity-ne1){throw 'Equipment retired before natural lifetime was established'}
 [ordered]@{activeSamples=$nativeActive.Count;maximumSeconds=$maximum;lastActive=$last;firstActive=$nativeActive[0];fixture=(S client '.mesmerize-fixture.json');equipment=$stillOwned}|ConvertTo-Json -Depth 9|Set-Content "$out/natural-expiry-summary.json"
 $checks.Add('Source-native countdown reaches zero and its committed object retires while the original power stays selected, quantity one and fixture active; no forced unequip or timer writes')
 C client 'coop_mesmerize_fixture off';$w.Restart()
 do{$f=S client '.mesmerize-fixture.json';if(!$f.active){break};Start-Sleep -Milliseconds 75}while($w.ElapsedMilliseconds-lt6500)
 if($f.active-or$f.result-ne'quantity_and_selection_restored'){throw 'Native Mesmerize inventory did not retire'}
 foreach($r in 'host','client'){$after=Q $r '--player-inventory';$after|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-after-$r.json";if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($after.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Inventory quantities differ'};if((Q $r '--health' 1).current-ne56){throw 'Player health changed'}}
 $checks.Add('Both native inventories restore and player health stays unchanged; world effects require separate checkpoint cleanup')
 $targetMonitor.WaitForExit(5000)|Out-Null;if(!$targetMonitor.HasExited-or$targetMonitor.ExitCode){throw 'Target watcher failed'}
 $targetRows=@(Get-Content "$out/targets.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $validTargets=@($targetRows|Where-Object {$_.valid-and$_.worldPresent})
 $filter=S client '.mesmerize-partner-filter.json';$filter|ConvertTo-Json|Set-Content "$out/filter-after.json"
 [ordered]@{samples=$targetRows.Count;validPresent=$validTargets.Count;targetSamples=@($validTargets|Where-Object {@($_.targets).Count}).Count;blocked=$filter.blocked-$filterBefore.blocked;considered=$filter.considered-$filterBefore.considered;readFailed=$filter.readFailed-$filterBefore.readFailed;forwarded=$filter.forwarded-$filterBefore.forwarded;otherAccepted=$filter.otherAccepted-$filterBefore.otherAccepted}|ConvertTo-Json|Set-Content "$out/partner-exclusion-summary.json"
 if(!$filter.installed-or($filter.otherAccepted-$filterBefore.otherAccepted)-lt1-or$filter.readFailed-ne$filterBefore.readFailed){throw 'Ordinary native NPC eligibility was not preserved'}
 if($validTargets.Count-lt20-or@($validTargets.targets|Where-Object {$_.definition-eq$npc.definition-and$_.handle-in$allowedTargetHandles}).Count-lt10-or@($validTargets.targets|Where-Object {$_.definition-like'*/d2coop/*'}).Count){throw 'Ordinary NPC target missing or partner acquired'}
 $eyeEntries=@($rows|ForEach-Object {$_.eyes.eyes}|Where-Object {$null-ne$_})
 if(!$eyeEntries.Count-or@((S host '.mesmerize-eyes.json').eyes).Count-or$rows[-1].eyes.started-le$rows[0].eyes.started){throw 'Enemy target did not produce and retire a remote eye'}
 $checks.Add('Ordinary native tutorial NPC remains eligible and receives source-owned beam/head effects; co-op partner is absent from native targets, remote eye presents and retires')
 $links=S host '.mesmerize-links.json';$links|ConvertTo-Json -Depth 6|Set-Content "$out/links-after.json"
 $linkEntries=@($rows|ForEach-Object {$_.links.links}|Where-Object {$null-ne$_})
 $sourceEffects=@($validTargets|ForEach-Object {$_.targets.effects}|Where-Object {$null-ne$_})
 $sourceTargetHandles=@($validTargets|ForEach-Object {$_.targets.handle}|Where-Object {$null-ne$_}|Select-Object -Unique)
 if($links.refused-ne$linksBefore.refused-or@($links.links).Count){throw 'Remote target effect identity refused or active after retirement'}
 foreach($kind in 1,2,3){
  $ids=@($sourceEffects|Where-Object kind -eq $kind|Select-Object -ExpandProperty id -Unique)
  $received=@($linkEntries|Where-Object kind -eq $kind)
  if(!$ids.Count-or!$received.Count-or$links.started[$kind-1]-le$linksBefore.started[$kind-1]-or
    ($links.started[$kind-1]-$linksBefore.started[$kind-1])-ne($links.stopped[$kind-1]-$linksBefore.stopped[$kind-1])){throw "Native target effect kind $kind did not present and retire"}
  if(@($received|Where-Object {$_.sourceId-notin$ids-or$_.target-notin$sourceTargetHandles}).Count){throw 'Receiver invented an unobserved native effect or target incarnation'}
 }
 foreach($row in $rows){foreach($link in $row.links.links){if($link.owner-ne$row.visual.handle){throw 'Target effect belongs to something other than the cosmetic presentation model'}}}
 $active=Get-Content "$out/observer-effects-active.json" -Raw|ConvertFrom-Json
 $nativeLinks=@($active.effects|Where-Object {$_.name-like'*/mesmerize_v2/link/*'})
 if(!@($nativeLinks|Where-Object name -like '*mesmerize_v2_link_01.fx').Count-or!@($nativeLinks|Where-Object name -like '*mesmerize_v2_link_head_01.fx').Count){throw 'Receiver native FX manager did not confirm beam and head records'}
 [ordered]@{started=@(0..2|ForEach-Object {$links.started[$_]-$linksBefore.started[$_]});stopped=@(0..2|ForEach-Object {$links.stopped[$_]-$linksBefore.stopped[$_]});updated=@(0..2|ForEach-Object {$links.updated[$_]-$linksBefore.updated[$_]});refused=$links.refused-$linksBefore.refused;sourceNativeEffects=@($sourceEffects|Group-Object kind|Select-Object Name,Count);nativeReceiverRecords=$nativeLinks}|ConvertTo-Json -Depth 8|Set-Content "$out/target-links-summary.json"
 $checks.Add('Source-observed beam, trail and target-head effect incarnations drive bounded native receiver FX owned only by the cosmetic model; all retire without NPC writes or duplicate damage calls')


}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_mesmerize_visual off';C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_mesmerize_fixture off';C $r 'coop_movement_fixture off'}catch{}}
 if($worldMonitor){$worldMonitor.WaitForExit(45000)|Out-Null;if(!$worldMonitor.HasExited-or$worldMonitor.ExitCode){$failure=($failure+'; native lifetime watcher failed').TrimStart('; ')}}
 foreach($capture in $recorders){$capture.WaitForExit(40000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode){$failure=($failure+'; capture failed').TrimStart('; ')}}
 if($targetMonitor){$targetMonitor.WaitForExit(40000)|Out-Null;if(!$targetMonitor.HasExited-or$targetMonitor.ExitCode){$failure=($failure+'; target watcher failed').TrimStart('; ')}}
 if($monitor){$monitor.WaitForExit(40000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){$failure=($failure+'; scene watcher failed').TrimStart('; ')}}
 $rows|ConvertTo-Json -Depth 20|Set-Content "$out/states.json"
 if($dirty){try{& "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log";foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'}};$p=Q client '--player-mesmerize';if($p.quantity-or$p.equipped-or$p.started){throw 'Mesmerize remains after checkpoint'};$fx=Q client '--mesmerize-effects';if(@($fx.effects).Count){throw 'Mesmerize effects remain after checkpoint'};$checks.Add('Both isolated checkpoints restore clean health/inventory and retire native Mesmerize effects')}catch{$failure=($failure+'; checkpoint restore: '+$_.Exception.Message).TrimStart('; ')}}
 try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All fourteen protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
 [ordered]@{passed=(!$failure);failure=$failure;source='client';checks=$checks;events=$events;dll=$initialDll;probe=$initialProbe;bankSha256=(Get-FileHash "$PSScriptRoot/mesmerize-banks-v77/emily-mesmerize-bank.bin").Hash;scope='Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Native source beam/trail/head transforms are reproduced by cosmetic-only native FX; target gameplay and NPC positions remain separately owned and unsynchronized. Five target identities and fifteen native FX records are the current bound. Continuous visual acceptance and moving-target smoothing remain pending. FX sound declarations retained but recordings silent. Native countdown expiry is observed while a bounded 40-second equipment fixture remains active and the power stays selected. Source alone owns gameplay; the remote model and native particles retire from source lifecycle. Aim cancellation and menu recovery have separate recordings. Other stances, directions and authoritative NPC outcomes remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged.'}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_NATIVE_MESMERIZE_OBSERVATION $($checks.Count) checks"




