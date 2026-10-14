param([ValidatePattern('^[a-z0-9-]+$')][string]$Take,[ValidateRange(-15,45)][int]$Pitch=12,[ValidateSet('receiver-stop','source-pause','receiver-pause')][string]$Interrupt='source-pause')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(!$Take-or(Test-Path $out)){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
$pids=@{};$inventory=@{};$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$recorders=@();$capture=$null;$monitor=$null;$targetMonitor=$null;$dirty=$false;$failure=$null
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
 $visualBefore=S host '.mesmerize-visual.json';$worldBefore=S host '.mesmerize-world-stream.json'
 $eyesBefore=S host '.mesmerize-eyes.json';$eyesBefore|ConvertTo-Json -Depth 8|Set-Content "$out/eyes-before.json"
 $visualBefore|ConvertTo-Json|Set-Content "$out/visual-before.json"
 C client 'setviewpos -33.5 -153 68.1 90 0';C host 'setviewpos -30.7 -151.8 68.1 203 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch 15 -Evidence "$out/observer-camera.json" -Quick
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch $Pitch -Evidence "$out/source-camera.json" -Quick
 $before=Q client '--player-mesmerize';$before|ConvertTo-Json -Depth 7|Set-Content "$out/before.json"
 if($before.quantity-or$before.equipped-or$before.started){throw 'Mesmerize already owned or active'}
 $manaBefore=& "$PSScriptRoot/Read-NativeMana.ps1" -Role client -Evidence "$out/mana-before.json"
 $dirty=$true;C client 'coop_mesmerize_fixture 20000';$w=[Diagnostics.Stopwatch]::StartNew()
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
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','34','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take-source.mp4")
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();$capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 $recorders+=$capture
 $hwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64()
 $captureInputIndex=[Array]::IndexOf($args,($args|Where-Object {$_ -like 'gfxcapture=*'}))
 if($captureInputIndex-lt0){throw 'Capture input argument missing'}
 $args[$captureInputIndex]="gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra"
 $args[-1]="$PSScriptRoot/$Take.mp4"
 $recorders+=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture-peer.log" -RedirectStandardOutput "$out/capture-peer.stdout" -PassThru
 Start-Sleep -Milliseconds 600;if(@($recorders|Where-Object HasExited).Count){throw 'Capture failed'}
 $targetMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-mesmerize-targets','28000') -WindowStyle Hidden -RedirectStandardOutput "$out/targets.ndjson" -RedirectStandardError "$out/targets.stderr" -PassThru
 $monitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-power-scenes','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/power-scenes.ndjson" -RedirectStandardError "$out/power-scenes.stderr" -PassThru
 $w.Restart();$cast=$false;$duplicated=$false;$manaCommit=$null;$worldFramed=$false;$interrupted=$false;$pausedRole=$null
 while($w.ElapsedMilliseconds-lt32000){
  if(!$cast-and$w.ElapsedMilliseconds-ge1100){I client 255 1800;$cast=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native LT held 1800ms then released'})}
  if($cast-and!$manaCommit-and$w.ElapsedMilliseconds-ge5500){$manaCommit=& "$PSScriptRoot/Read-NativeMana.ps1" -Role client -Evidence "$out/mana-after-commit.json"}
  if(!$worldFramed-and$w.ElapsedMilliseconds-ge5500){
   $committed=Q client '--mesmerize-world';$committed|ConvertTo-Json -Depth 8|Set-Content "$out/source-world-committed.json"
   if(!$committed.present){throw 'Committed world unavailable for evidence framing'}
   # Frame the effect separately after the close body phase. Keep the
   # camera within the previously tested room corridor, not inside its east wall.
   $cameraX=[double]$committed.transform[0]+3;$cameraY=[double]$committed.transform[1]-4
   if($cameraX-lt-31.5-or$cameraX-gt-30-or$cameraY-lt-161-or$cameraY-gt-147){throw 'World evidence camera outside tested room bounds'}
   C host ('setviewpos {0} {1} 68.1 126.8699 0' -f $cameraX,$cameraY)
   & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch 3 -Evidence "$out/world-camera.json" -Quick
   $worldFramed=$true
  }
  if(!$duplicated-and$w.ElapsedMilliseconds-ge8000){Q host '--mesmerize-effects'|ConvertTo-Json -Depth 10|Set-Content "$out/observer-effects-active.json";C client 'coop_mesmerize_visual duplicate';$duplicated=$true}
  if(!$interrupted-and$w.ElapsedMilliseconds-ge10000){
   $active=S host '.mesmerize-visual.json';if(!$active.active-or!$active.fxActive){throw 'No active world effect at interruption'}
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input=$Interrupt})
   if($Interrupt-eq'receiver-stop'){C host 'coop_shadow_stream off'}else{
    $pausedRole=if($Interrupt-eq'source-pause'){'client'}else{'host'}
    & "$lab/Send-D2Input.ps1" -Role $pausedRole -Button Start -DurationMs 180|Out-Null
    $pauseWait=[Diagnostics.Stopwatch]::StartNew()
    do{if((S $pausedRole).localGameLogicState-eq2){break};Start-Sleep -Milliseconds 30}while($pauseWait.ElapsedMilliseconds-lt2500)
    if((S $pausedRole).localGameLogicState-ne2){throw 'Native pause did not start'}
    [ordered]@{role=$pausedRole;status=(S $pausedRole);visual=(S host '.mesmerize-visual.json');world=(S host '.mesmerize-world-stream.json')}|ConvertTo-Json -Depth 8|Set-Content "$out/paused.json"
    Start-Sleep -Milliseconds 700
    & "$lab/Send-D2Input.ps1" -Role $pausedRole -Button A -DurationMs 180|Out-Null
    $pauseWait.Restart()
    do{if((S $pausedRole).localGameLogicState-eq1){break};Start-Sleep -Milliseconds 30}while($pauseWait.ElapsedMilliseconds-lt4000)
    if((S $pausedRole).localGameLogicState-ne1){throw 'Native pause did not resume'};$pausedRole=$null
   }
   $interrupted=$true
  }
  $m=S host '.movement.json';$stream=S host '.shadow-stream.json'
  $fx=if(($rows.Count%4)-eq0){OptionalFx client}else{$null}
  $rows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;player=(Q client '--player-states');movement=$m;stream=$stream;world=(S host '.mesmerize-world-stream.json');visual=(S host '.mesmerize-visual.json');eyes=(S host '.mesmerize-eyes.json');sourceWorld=(S client '.mesmerize-world-stream.json');effects=$fx})
  if(!$interrupted-and(!$m.active-or!$stream.ready-or!$stream.armed)){throw "Native transport/presentation retired: $($stream.result)"}
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
 if(!$interrupted){throw 'Interruption stimulus not applied'}
 if($m.powerWeight-gt.02){throw 'Interrupted remote cast retained weight'}
 $sword=Q host '--avatar-sword' (S host).probeActorHandle;$sword|ConvertTo-Json -Depth 8|Set-Content "$out/avatar-sword-after.json"
 if(!$sword.equipped-or!(Q client '--player-sword').equipped){throw 'Native sword selection changed'}
 foreach($r in 'host','client'){if((Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Remote avatar health changed'}}
 $visual=S host '.mesmerize-visual.json';$world=S host '.mesmerize-world-stream.json'
 if($visual.refused-ne$visualBefore.refused-or($visual.created-$visualBefore.created)-ne1-or($visual.started[0]-$visualBefore.started[0])-ne1-or($visual.started[1]-$visualBefore.started[1])-ne1){throw 'Interrupted world creation/entry/idle not unique'}
 if($visual.active-or$visual.pendingHide-or$visual.fxActive-or($visual.fxStarted-$visualBefore.fxStarted)-ne1-or($visual.fxStopped-$visualBefore.fxStopped)-ne1-or$world.leased){throw 'Interrupted world lease/effect did not retire'}
 if(@($rows|Where-Object {$_.ms-gt15000-and($_.visual.active-or$_.visual.pendingHide-or$_.world.leased)}).Count){throw 'Old world event revived after interruption'}
 foreach($state in 1,3){if(!@($rows|Where-Object {$_.world.state-eq$state-and$_.world.leased}).Count){throw "Remote pre-interruption state $state missing"}}
 if($world.rejected-le$worldBefore.rejected){throw 'Duplicate packet rejection not observed'}
 $remainingFx=Q host '--mesmerize-effects';$remainingFx|ConvertTo-Json -Depth 10|Set-Content "$out/observer-effects-after-retirement.json";if(@($remainingFx.effects).Count){throw 'Observer native body effect did not finish its native stop tail before end of recording'}
 $peerEyes=S host '.mesmerize-eyes.json'
 $eyeStarts=$peerEyes.started-$eyesBefore.started;$eyeStops=$peerEyes.stopped-$eyesBefore.stopped
 if($eyeStarts-lt1-or$eyeStarts-ne$eyeStops-or$peerEyes.refused-ne$eyesBefore.refused-or($peerEyes.updated-$eyesBefore.updated)-lt3-or@($peerEyes.eyes).Count){throw 'Interrupted remote eye lifecycle failed'}
 if(@($rows|Where-Object {$_.ms-gt15000-and@($_.eyes.eyes).Count}).Count){throw 'Old eye revived after interruption'}
 $activeFx=Get-Content "$out/observer-effects-active.json" -Raw|ConvertFrom-Json
 $eyeFx=@($activeFx.effects|Where-Object name -eq 'models/effects/gameplay/powers/mesmerize_v2/eye/mesmerize_v2_eye_01.fx')
 if(!$eyeFx.Count-or@($eyeFx|Where-Object ownerDefinition -ne 'models/powers/d2coop/mesmerize_visual.def').Count){throw 'No cosmetic-owned native eye before interruption'}
 [ordered]@{started=$eyeStarts;stopped=$eyeStops;updated=$peerEyes.updated-$eyesBefore.updated;refused=$peerEyes.refused-$eyesBefore.refused;sourceIds=@($rows.eyes.eyes.sourceId|Select-Object -Unique);interruption=$Interrupt}|ConvertTo-Json|Set-Content "$out/eye-interrupt-summary.json"
 $checks.Add('Native cosmetic-owned eye updates before interruption, retires once per incarnation with the world, and cannot revive from stale packets')
 $checks.Add('Committed source world drives one model and body effect; explicit stream stop or native pause retires ownership, hides model and lets the native effect tail finish')
 $checks.Add('Duplicate lifecycle packet rejected without a second model or effect; no late world revival')
 $checks.Add('Interrupted cast releases body weight; source/avatar sword selections and avatar health retained; no old world revival')
 if((!$manaCommit)-or$manaCommit.currentCandidate-ge$manaBefore.currentCandidate){throw 'Native Mesmerize mana expenditure not established'};$checks.Add('Native cast spends source mana without resource writes')
 $bodyFx=@($rows|ForEach-Object {$_.effects.effects}|Where-Object name -eq 'models/effects/gameplay/powers/mesmerize_v2/body/mesmerize_v2_01.fx')
 if(!$bodyFx.Count){throw 'Native Mesmerize world body effect not observed'};$checks.Add('Exact native world body effect exists after source cast; presentation/AI ownership still under investigation')
 C client 'coop_mesmerize_fixture off';$w.Restart()
 do{$f=S client '.mesmerize-fixture.json';if(!$f.active){break};Start-Sleep -Milliseconds 75}while($w.ElapsedMilliseconds-lt6500)
 if($f.active-or$f.result-ne'quantity_and_selection_restored'){throw 'Native Mesmerize inventory did not retire'}
 foreach($r in 'host','client'){$after=Q $r '--player-inventory';$after|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-after-$r.json";if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($after.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Inventory quantities differ'};if((Q $r '--health' 1).current-ne56){throw 'Player health changed'}}
 $checks.Add('Both native inventories restore and player health stays unchanged; world effects require separate checkpoint cleanup')
 $targetMonitor.WaitForExit(5000)|Out-Null;if(!$targetMonitor.HasExited-or$targetMonitor.ExitCode){throw 'Target watcher failed'}
 $targetRows=@(Get-Content "$out/targets.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $validTargets=@($targetRows|Where-Object {$_.valid-and$_.worldPresent-and@($_.targets.effects).Count})
 [ordered]@{samples=$targetRows.Count;valid=@($targetRows|Where-Object valid).Count;withEffects=$validTargets.Count;stages=@($targetRows|Group-Object stage|Select-Object Name,Count);targetNames=@($validTargets.targets.definition|Select-Object -Unique);kinds=@($validTargets.targets.effects.kind|Select-Object -Unique);effects=@($validTargets.targets.effects|Select-Object kind,id,owner,name -Unique)}|ConvertTo-Json -Depth 8|Set-Content "$out/target-summary.json"
 if($validTargets.Count-lt10-or!@($validTargets.targets.effects|Where-Object kind -eq 1).Count){throw 'Native target/beam references not proven'}
 $checks.Add('Read-only native target records resolve exact target incarnations, world-owned beam effects and bounded source/destination transforms')

}catch{$failure=$_.Exception.Message}finally{
 if($pausedRole){try{& "$lab/Send-D2Input.ps1" -Role $pausedRole -Button A -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 600}catch{}}
 foreach($r in 'host','client'){try{I $r;C $r 'coop_mesmerize_visual off';C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_mesmerize_fixture off';C $r 'coop_movement_fixture off'}catch{}}
 foreach($capture in $recorders){$capture.WaitForExit(40000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode){$failure=($failure+'; capture failed').TrimStart('; ')}}
 if($targetMonitor){$targetMonitor.WaitForExit(40000)|Out-Null;if(!$targetMonitor.HasExited-or$targetMonitor.ExitCode){$failure=($failure+'; target watcher failed').TrimStart('; ')}}
 if($monitor){$monitor.WaitForExit(40000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){$failure=($failure+'; scene watcher failed').TrimStart('; ')}}
 $rows|ConvertTo-Json -Depth 20|Set-Content "$out/states.json"
 if($dirty){try{& "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log";foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'}};$p=Q client '--player-mesmerize';if($p.quantity-or$p.equipped-or$p.started){throw 'Mesmerize remains after checkpoint'};$fx=Q client '--mesmerize-effects';if(@($fx.effects).Count){throw 'Mesmerize effects remain after checkpoint'};$checks.Add('Both isolated checkpoints restore clean health/inventory and retire native Mesmerize effects')}catch{$failure=($failure+'; checkpoint restore: '+$_.Exception.Message).TrimStart('; ')}}
 try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All fourteen protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
 [ordered]@{passed=(!$failure);failure=$failure;source='client';interruption=$Interrupt;checks=$checks;events=$events;dll=$initialDll;probe=$initialProbe;bankSha256=(Get-FileHash "$PSScriptRoot/mesmerize-banks-v77/emily-mesmerize-bank.bin").Hash;scope='Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Source-referenced eye effects are owned by the cosmetic model and retire on interruption. Native target/beam records are inspected read-only; no beam playback is added. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Interruption after commitment checks finite cosmetic retirement and no stale revival. Same-world automatic resume/reacquisition, natural lifetime and interrupted aiming remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged.'}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_NATIVE_MESMERIZE_OBSERVATION $($checks.Count) checks"
