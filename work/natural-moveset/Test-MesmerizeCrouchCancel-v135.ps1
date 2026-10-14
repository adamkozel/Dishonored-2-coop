param([ValidatePattern('^[a-z0-9-]+$')][string]$Take,[ValidateRange(-15,45)][int]$Pitch=12)
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
 $dirty=$true;C client 'coop_mesmerize_fixture 20000';$w=[Diagnostics.Stopwatch]::StartNew()
 do{$f=S client '.mesmerize-fixture.json';if($f.result-eq'native_mesmerize_ready'){break};Start-Sleep -Milliseconds 60}while($w.ElapsedMilliseconds-lt6000)
 if($f.result-ne'native_mesmerize_ready'){throw "Native Mesmerize acquisition failed: $($f.result)"}
 $power=Q client '--player-mesmerize';$power|ConvertTo-Json -Depth 7|Set-Content "$out/enabled.json"
 if(!$power.equipped-or!$power.started-or!$power.instantiated-or$power.quantity-ne1){throw 'Native Mesmerize equipment not confirmed'}
 $checks.Add('Exact native Emily Mesmerize item acquired and selected without directly invoking power or VFX')
 Copy-Item "$PSScriptRoot/mesmerize-context-banks-v132/emily-mesmerize-context-bank.bin" "$root/work/lab-instances/host/coop-status.json.movement.bin"
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 600;C host 'coop_movement_bank on'
 $w.Restart();do{$m=S host '.movement.json';$stream=S host '.shadow-stream.json';if($m.active-and$m.version-eq46-and$m.weight-gt.9-and$m.presentedDrawn-and$stream.ready-and$stream.armed){break};Start-Sleep -Milliseconds 50}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne46-or$m.weight-lt.9-or!$m.presentedDrawn-or!$stream.ready-or!$stream.armed){throw 'Exact Mesmerize bank and native Steam stream not ready'}
 $checks.Add('Native Steam transport and exact bank46/285 remote Emily standing armed context ready')

 & "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180|Out-Null
 $w.Restart();do{$m=S host '.movement.json';$native=Q client '--player-states';if($native.stance-eq1-and$m.stanceMix-gt.98-and$m.active){break};Start-Sleep -Milliseconds 30}while($w.ElapsedMilliseconds-lt3000)
 if($native.stance-ne1-or$m.stanceMix-lt.98-or!$m.active){throw 'Native and remote crouch not ready before cast'}
 [ordered]@{native=$native;remote=$m}|ConvertTo-Json -Depth 8|Set-Content "$out/crouch-ready.json"
 $checks.Add('Native crouch drives the remote Posture-100 stance before the cast')
 $hwnd=(Get-Process -Id $pids.client).MainWindowHandle.ToInt64();if(!$hwnd){throw 'No source window'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','12','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take-source.mp4")
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();$capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 $recorders+=$capture
 $hwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64()
 $captureInputIndex=[Array]::IndexOf($args,($args|Where-Object {$_ -like 'gfxcapture=*'}))
 if($captureInputIndex-lt0){throw 'Capture input argument missing'}
 $args[$captureInputIndex]="gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra"
 $args[-1]="$PSScriptRoot/$Take.mp4"
 $recorders+=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture-peer.log" -RedirectStandardOutput "$out/capture-peer.stdout" -PassThru
 Start-Sleep -Milliseconds 600;if(@($recorders|Where-Object HasExited).Count){throw 'Capture failed'}
 $targetMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-mesmerize-targets','10000') -WindowStyle Hidden -RedirectStandardOutput "$out/targets.ndjson" -RedirectStandardError "$out/targets.stderr" -PassThru
 $monitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-power-scenes','10000') -WindowStyle Hidden -RedirectStandardOutput "$out/power-scenes.ndjson" -RedirectStandardError "$out/power-scenes.stderr" -PassThru
 $w.Restart();$aimed=$false;$cancelled=$false
 while($w.ElapsedMilliseconds-lt10000){
  if(!$aimed-and$w.ElapsedMilliseconds-ge1000){I client 255 6000;$aimed=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native LT hold begins; no release-to-cast'})}
  if(!$cancelled-and$w.ElapsedMilliseconds-ge3200){
   $aim=Q client '--player-mesmerize';$aim|ConvertTo-Json -Depth 8|Set-Content "$out/native-before-cancel.json"
   & "$lab/Send-D2Input.ps1" -Role client -Button X -LeftTrigger 255 -DurationMs 250|Out-Null
   Start-Sleep -Milliseconds 300;I client;$cancelled=$true
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native X cancel while LT held, followed by neutral'})
  }
  $m=S host '.movement.json';$stream=S host '.shadow-stream.json'
  $rows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;player=(Q client '--player-states');movement=$m;stream=$stream;nativeWorld=(Q client '--mesmerize-world');world=(S host '.mesmerize-world-stream.json');visual=(S host '.mesmerize-visual.json');eyes=(S host '.mesmerize-eyes.json');links=(S host '.mesmerize-links.json')})
  if(!$m.active-or!$stream.ready-or!$stream.armed){throw 'Native movement or transport retired during cancellation'}
  Start-Sleep -Milliseconds 25
 }
 I client
 $manaAfter=& "$PSScriptRoot/Read-NativeMana.ps1" -Role client -Evidence "$out/mana-after.json"
 $monitor.WaitForExit(4000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){throw 'Native scene watcher failed'}
 $fastScenes=@(Get-Content "$out/power-scenes.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $scenes=@($fastScenes|Where-Object {$_.valid-and$_.variant-in16,17,18})
 foreach($variant in 16,17){
  if(!@($scenes|Where-Object variant -eq $variant).Count-or!@($rows|Where-Object {$_.movement.powerVariant-eq$variant-and$_.movement.powerWeight-gt.5}).Count){throw "Native and remote aim phase $variant not both observed"}
 }
 $scenes|Select-Object variant,asset,handle,armSide,character,armHandle -Unique|ConvertTo-Json -Depth 5|Set-Content "$out/phase-incarnations.json"
 if(@($scenes|Where-Object {$_.armSide-ne1-or$_.character-ne2-or!$_.armHandle}).Count){throw 'Emily source scene ownership changed'}
 $checks.Add('Native Emily enter and aim scenes drive corresponding complete-body remote phases through Steam')
 if(!$aimed-or!$cancelled-or$m.powerWeight-gt.02-or$m.weight-lt.9){throw 'Cancelled aim did not recover the crouched movement bank'}
 if(@($rows|Where-Object {$_.ms-gt6500-and$_.movement.powerWeight-gt.02}).Count){throw 'Cancelled power animation returned after recovery'}
 $checks.Add('Native X while LT remains held cancels the aim; remote body returns to its normal movement bank without stale power weight')
 $aimRows=@($rows|Where-Object {$_.movement.powerVariant-in16,17-and$_.movement.powerWeight-gt.5})
 $recovered=@($rows|Where-Object {$_.ms-gt6500-and$_.movement.powerWeight-lt.02})
 if(!$aimRows.Count-or!$recovered.Count-or@($rows|Where-Object {$_.player.stance-ne1-or$_.movement.stanceMix-lt.98}).Count){throw 'Native or remote crouch changed during aim/cancel/recovery'}
 [ordered]@{aimSamples=$aimRows.Count;recoverySamples=$recovered.Count;allCrouched=$true}|ConvertTo-Json|Set-Content "$out/crouch-context-summary.json"
 $checks.Add('Native and remote remain crouched through aim, cancellation and return to the normal crouched movement bank')
 if([Math]::Abs($manaAfter.currentCandidate-$manaBefore.currentCandidate)-gt.05){throw 'Aim cancellation changed native mana'}
 $checks.Add('Cancelled aim consumes no source mana; no resource writes are used')
 $visual=S host '.mesmerize-visual.json'
 if($visual.created-ne$visualBefore.created-or$visual.fxStarted-ne$visualBefore.fxStarted-or$visual.active-or$visual.fxActive){throw 'Cancelled aim created a cosmetic world or effect'}
 if(@($rows|Where-Object {$_.nativeWorld.present-or$_.world.leased-or$_.visual.active}).Count){throw 'Cancelled aim committed a native or remote world'}
 foreach($r in 'host','client'){
  $fx=Q $r '--mesmerize-effects';$fx|ConvertTo-Json -Depth 8|Set-Content "$out/effects-after-$r.json"
  if(@($fx.effects).Count){throw 'Native effect remains after cancellation'}
  $h=Q $r '--health' (S $r).probeActorHandle;$h|ConvertTo-Json -Depth 6|Set-Content "$out/avatar-health-after-$r.json"
  if($h.current-ne40-or(Q $r '--health' 1).current-ne56){throw 'Player or avatar health changed'}
 }
 $checks.Add('Cancelled aim creates neither source-native nor cosmetic world, target links or particles; native players and avatars retain health')
 if(!(Q host '--avatar-sword' (S host).probeActorHandle).equipped-or!(Q client '--player-sword').equipped){throw 'Sword equipment changed'}
 $checks.Add('Native and remote sword selections remain equipped after cancellation')
 C client 'coop_mesmerize_fixture off';$w.Restart()
 do{$f=S client '.mesmerize-fixture.json';if(!$f.active){break};Start-Sleep -Milliseconds 75}while($w.ElapsedMilliseconds-lt6500)
 if($f.active-or$f.result-ne'quantity_and_selection_restored'){throw 'Native Mesmerize inventory did not retire'}
 foreach($r in 'host','client'){
  $after=Q $r '--player-inventory';$after|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-after-$r.json"
  if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($after.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Inventory quantities differ'}
 }
 $checks.Add('Both native inventory quantities restore exactly before checkpoint cleanup')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_mesmerize_visual off';C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_mesmerize_fixture off';C $r 'coop_movement_fixture off'}catch{}}
 foreach($capture in $recorders){$capture.WaitForExit(40000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode){$failure=($failure+'; capture failed').TrimStart('; ')}}
 if($targetMonitor){$targetMonitor.WaitForExit(40000)|Out-Null;if(!$targetMonitor.HasExited-or$targetMonitor.ExitCode){$failure=($failure+'; target watcher failed').TrimStart('; ')}}
 if($monitor){$monitor.WaitForExit(40000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){$failure=($failure+'; scene watcher failed').TrimStart('; ')}}
 $rows|ConvertTo-Json -Depth 20|Set-Content "$out/states.json"
 if($dirty){try{& "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log";foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'}};$p=Q client '--player-mesmerize';if($p.quantity-or$p.equipped-or$p.started){throw 'Mesmerize remains after checkpoint'};$fx=Q client '--mesmerize-effects';if(@($fx.effects).Count){throw 'Mesmerize effects remain after checkpoint'};$checks.Add('Both isolated checkpoints restore clean health/inventory and retire native Mesmerize effects')}catch{$failure=($failure+'; checkpoint restore: '+$_.Exception.Message).TrimStart('; ')}}
 try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All fourteen protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
 [ordered]@{passed=(!$failure);failure=$failure;source='client';checks=$checks;events=$events;dll=$initialDll;probe=$initialProbe;bankSha256=(Get-FileHash "$PSScriptRoot/mesmerize-context-banks-v132/emily-mesmerize-context-bank.bin").Hash;scope='Crouched armed Emily Mesmerize native aim cancellation via controller X while LT remains held. Confirms native first-person enter/aim provenance, remote body recovery and absence of a committed native/cosmetic power or mana cost. Visual acceptance and shared NPC outcomes remain incomplete; standing cancellation and natural lifetime have separate receipts. Both isolated checkpoints restore and protected originals remain unchanged.'}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_NATIVE_MESMERIZE_OBSERVATION $($checks.Count) checks"




