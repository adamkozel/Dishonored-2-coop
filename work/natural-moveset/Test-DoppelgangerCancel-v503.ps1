param([ValidatePattern('^[a-z0-9-]+$')][string]$Take,[ValidateRange(-15,45)][int]$Pitch=30,[switch]$RetainWorldForNextTake,[switch]$ExpectVisualReuse,[ValidateSet('standing','crouched')][string]$Stance='standing',[switch]$Holstered)
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

  $health=[ordered]@{player=(Q $r '--health' 1);partner=(Q $r '--health' (S $r).probeActorHandle);isolation=(S $r '.observer-isolation.json')}
  $health|ConvertTo-Json -Depth 10|Set-Content "$out/health-before-$r.json"
  if($health.player.current-ne56-or$health.partner.current-ne40){throw "Clean living fixture required: $r player=$($health.player.current) partner=$($health.partner.current) isolation=$($health.isolation.result)"}

  $inventory[$r]=Q $r '--player-inventory';$inventory[$r]|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-before-$r.json"
 }
 $cloneStart=S client '.doppelganger-world.json';$sequenceFloor=[uint64](($cloneStart.history.sequence|Measure-Object -Maximum).Maximum)
 $cloneStart|ConvertTo-Json -Depth 8|Set-Content "$out/sender-start.json"
 $epochStart=@{host=(S host).localEpoch;client=(S client).localEpoch}
 $epochStart|ConvertTo-Json|Set-Content "$out/world-start.json"
 $visualStart=S host '.doppelganger-visual.json';$visualStart|ConvertTo-Json -Depth 8|Set-Content "$out/visual-start.json"
 $dirty=$true
 C client 'setviewpos -48.59 -155.07 68.1 90 0';C host 'setviewpos -44.39 -153.27 68.1 203 0';
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
 Copy-Item "$PSScriptRoot/doppelganger-holstered-banks-v495/emily-doppelganger-holstered-bank.bin" "$root/work/lab-instances/host/coop-status.json.movement.bin"
 foreach($r in 'client','host'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 600;C host 'coop_movement_bank on'
 $w.Restart();do{$m=S host '.movement.json';$stream=S host '.shadow-stream.json';if($m.active-and$m.version-eq57-and$m.weight-gt.9-and$m.presentedDrawn-and$stream.ready-and$stream.armed){break};Start-Sleep -Milliseconds 50}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne57-or$m.weight-lt.9-or!$m.presentedDrawn-or!$stream.ready-or!$stream.armed){throw 'Exact Doppelganger bank/native Steam stream not ready'}
 $checks.Add('Remote Emily uses exact bank57/331 and native Steam pose transport')
 $expectedStance=if($Stance-eq'crouched'){1}else{0}
 if($expectedStance){& "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 300}
 if($Holstered){
  & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch -60 -Evidence "$out/holster-look-away.json" -Quick
  & "$lab/Send-D2Input.ps1" -Role client -Button X -DurationMs 1000|Out-Null;Start-Sleep -Milliseconds 1200
  & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch $Pitch -Evidence "$out/holster-look-restored.json" -Quick
 }
 $w.Restart();$setupRows=[Collections.Generic.List[object]]::new()
 do{
  $m=S host '.movement.json';$native=Q client '--player-states';$nativeSword=Q client '--player-sword'
  $setupRows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;native=$native;sword=$nativeSword;movement=$m;source=(S client '.shadow-stream.json');peer=(S host '.shadow-stream.json')})
  if($native.stance-eq$expectedStance-and[Math]::Abs($m.stanceMix-$expectedStance)-lt.02-and$m.active-and$m.presentedDrawn-ne[bool]$Holstered-and$nativeSword.equipped-ne[bool]$Holstered-and[Math]::Abs($m.armedMix-$(if($Holstered){0}else{1}))-lt.02){break};Start-Sleep -Milliseconds 40
 }while($w.ElapsedMilliseconds-lt4500)
 $setupRows|ConvertTo-Json -Depth 14|Set-Content "$out/context-setup.json"
 if($native.stance-ne$expectedStance-or[Math]::Abs($m.stanceMix-$expectedStance)-gt.02-or!$m.active-or$m.presentedDrawn-eq[bool]$Holstered-or$nativeSword.equipped-eq[bool]$Holstered){throw 'Native/remote stance and equipment context not ready'}
 [ordered]@{stance=$Stance;holstered=[bool]$Holstered;native=$native;sword=$nativeSword;remote=$m}|ConvertTo-Json -Depth 10|Set-Content "$out/context-ready.json"
 $checks.Add('Native controller stance and holster inputs establish the same complete-body remote context before casting')

 $hwnd=(Get-Process -Id $pids.client).MainWindowHandle.ToInt64();if(!$hwnd){throw 'Source window missing'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','16','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take-source.mp4")
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();$capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 $recorders+=$capture
 $hwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64();if(!$hwnd){throw 'Observer window missing'}
 $idx=[Array]::IndexOf($args,($args|Where-Object {$_ -like 'gfxcapture=*'}));if($idx-lt0){throw 'Capture argument absent'}
 $args[$idx]="gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra";$args[-1]="$PSScriptRoot/$Take.mp4"
 $recorders+=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture-peer.log" -RedirectStandardOutput "$out/capture-peer.stdout" -PassThru
 Start-Sleep -Milliseconds 600;if(@($recorders|Where-Object HasExited).Count){throw 'Source capture failed'}
 $monitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-doppelganger-scenes','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/power-scenes.ndjson" -RedirectStandardError "$out/power-scenes.stderr" -PassThru

 $rawMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-midnight','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/native-midnight.ndjson" -RedirectStandardError "$out/native-midnight.stderr" -PassThru
 $recorders+=$rawMonitor
 $w.Restart();$aimed=$false;$cancelled=$false
 while($w.ElapsedMilliseconds-lt14000){
  if(!$aimed-and$w.ElapsedMilliseconds-ge1100){I client 255 6000;$aimed=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native LT aim; release-to-cast will be cancelled'})}
  if(!$cancelled-and$w.ElapsedMilliseconds-ge2400){
   & "$lab/Send-D2Input.ps1" -Role client -Button X -LeftTrigger 255 -DurationMs 250|Out-Null
   $cancelled=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native X while LT held; both released after250ms'})
  }
  $m=S host '.movement.json';$stream=S host '.shadow-stream.json'
  $rows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;player=(Q client '--player-states');sword=(Q client '--player-sword');component=(Q client '--object' $power.component);movement=$m;stream=$stream;cloneSource=(S client '.doppelganger-world.json');clonePeer=(S host '.doppelganger-world.json');cloneVisual=(S host '.doppelganger-visual.json')})
  if(!$m.active-or!$stream.ready-or!$stream.armed){throw 'Native body/transport retired during cancellation'}
  Start-Sleep -Milliseconds 30
 }
 I client;$manaAfter=& "$PSScriptRoot/Read-NativeMana.ps1" -Role client -Evidence "$out/mana-after.json"
 $monitor.WaitForExit(4000)|Out-Null;$rawMonitor.WaitForExit(4000)|Out-Null
 if(!$monitor.HasExited-or$monitor.ExitCode-or!$rawMonitor.HasExited-or$rawMonitor.ExitCode){throw 'Native source watchers failed'}
 $samples=@(Get-Content "$out/power-scenes.ndjson"|ForEach-Object {$_|ConvertFrom-Json});$scenes=@($samples|Where-Object valid)
 $raw=@(Get-Content "$out/native-midnight.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 foreach($v in 29,30){
  if(!@($scenes|Where-Object variant -eq $v).Count-or!@($rows|Where-Object {$_.movement.powerVariant-eq$v-and$_.movement.powerWeight-gt.5}).Count){throw "Native/remote aim phase $v absent"}
 }
 if(@($scenes|Where-Object variant -eq 31).Count-or@($rows|Where-Object {$_.movement.powerVariant-eq31-and$_.movement.powerWeight-gt.5}).Count){throw 'Cancellation unexpectedly played successful cast release'}
 $scenes|ConvertTo-Json -Depth 12|Set-Content "$out/strict-phases.json"
 $checks.Add('Native owned entry/aim phases drive remote full-body poses; cancellation plays no successful-cast release')
 $cloneScenes=@($raw.instances|Where-Object {$_.state-eq3-and$_.asset-match'^midnight/gameplay/human/common/doppelganger/doppelganger_(spawn|dispawn|death_fx)\.midnightscene$'})
 $counters=@($rows|ForEach-Object {$word=@($_.component.words|Where-Object offset -eq '0x60');if($word.Count-ne1){throw 'Power activation counter absent'};[Convert]::ToUInt64($word[0].word,16)-shr32})
 $visual=S host '.doppelganger-visual.json'
 [ordered]@{nativeCloneSceneSamples=$cloneScenes.Count;counterBefore=$counters[0];counterAfter=$counters[-1];manaBefore=$manaBefore.currentCandidate;manaAfter=$manaAfter.currentCandidate;visualBefore=$visualStart;visualAfter=$visual}|ConvertTo-Json -Depth 12|Set-Content "$out/no-spawn-proof.json"
 if($cloneScenes.Count-or@($counters|Where-Object {$_-ne$counters[0]}).Count-or[Math]::Abs($manaAfter.currentCandidate-$manaBefore.currentCandidate)-gt.05){throw 'Native cast committed during cancellation'}
 if($visual.created-ne$visualStart.created-or$visual.shown-ne$visualStart.shown-or$visual.fx.started.Count-ne2-or$visualStart.fx.started.Count-ne2-or$visual.fx.started[0]-ne$visualStart.fx.started[0]-or$visual.fx.started[1]-ne$visualStart.fx.started[1]-or$visual.visible-or$visual.fx.scene-or@($rows|Where-Object {$_.clonePeer.present-or$_.clonePeer.leased-or$_.cloneVisual.visible}).Count){throw 'Cancelled aim produced a remote clone or effect'}
 $checks.Add('Native cancellation spends no mana, increments no activation counter, starts no native clone scene and creates no remote clone/FX')
 $active=@($rows|Where-Object {$_.ms-ge900-and$_.ms-le6500})
 $recovered=@($rows|Where-Object {$_.ms-ge5000})
 if(!$aimed-or!$cancelled-or$active.Count-lt15-or$recovered.Count-lt10){throw 'Cancellation coverage too short'}
 if(@($active|Where-Object {$_.movement.weight-lt.95-or$_.movement.clockRejection.count}).Count){throw 'Cancellation lost body continuity'}
 if(@($recovered|Where-Object {$_.movement.powerWeight-gt.02-or$_.movement.weight-lt.95-or$_.player.stance-ne$expectedStance-or[Math]::Abs($_.movement.stanceMix-$expectedStance)-gt.05-or$_.movement.presentedDrawn-eq[bool]$Holstered-or$_.sword.equipped-eq[bool]$Holstered}).Count){throw 'Cancellation left stale animation or incorrect stance/equipment'}
 [ordered]@{stance=$Stance;holstered=[bool]$Holstered;activeSamples=$active.Count;recoverySamples=$recovered.Count;minimumBodyWeight=($active.movement.weight|Measure-Object -Minimum).Minimum;maxRecoveredPowerWeight=($recovered.movement.powerWeight|Measure-Object -Maximum).Maximum}|ConvertTo-Json|Set-Content "$out/cancel-recovery.json"
 $checks.Add('Body remains continuous through native Cancel/Undraw and recovers its original stance/equipment without stale power posing')
 $avatar=Q host '--avatar-sword' (S host).probeActorHandle;$avatar|ConvertTo-Json -Depth 8|Set-Content "$out/avatar-sword-after.json"
 if($avatar.equipped-eq[bool]$Holstered){throw 'Remote native sword selection changed'}
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

 # Retain the already bounded native AI pause while handing the same world to
 # the immediate second take. The previous harness released it here, leaving
 # nearby native enemies free to attack before the next setup health check.
 # The next setup refreshes its own60s fixture; any failure restores checkpoint.
 foreach($r in 'client','host'){
  if($RetainWorldForNextTake-and!$failure){
   try{$hold=S $r '.observer-isolation.json';$hold|ConvertTo-Json -Depth 8|Set-Content "$out/isolation-handoff-$r.json";
    if(!$hold.ready-or!$hold.movementFixture-or$hold.remainingMs-lt5000){throw 'Finite AI hold lacks time for immediate same-world handoff'}
   }catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
  }else{try{C $r 'coop_movement_fixture off'}catch{}}
 }

 $rows|ConvertTo-Json -Depth 22|Set-Content "$out/states.json"
 if($null-ne$worldSamples){$worldSamples|ConvertTo-Json -Depth 15|Set-Content "$out/clone-lifecycle-samples.json"}
 if($dirty-and$RetainWorldForNextTake-and!$failure){
  $end=@{host=(S host).localEpoch;client=(S client).localEpoch};$end|ConvertTo-Json|Set-Content "$out/world-retained.json"
  if($end.host-ne$epochStart.host-or$end.client-ne$epochStart.client){$failure='Unexpected world replacement before reuse'}
 }
 if($dirty-and(!$RetainWorldForNextTake-or$failure)){try{
  & "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log"
  foreach($r in 'client','host'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'}}
  $p=Q client '--player-doppelganger';if($p.quantity-or$p.equipped-or$p.started){throw 'Doppelganger remains acquired after checkpoint'}
  $entities=Q client '--entities';if(@($entities.entities|Where-Object {$_.definitionName-match'^models/powers/doppelganger/(decoy_entity|deadlyshadow_entity)(|_corvo)\.def$'}).Count){throw 'Spawned doppelganger remains after checkpoint'}
  $checks.Add('Both isolated checkpoints restore original health/inventory and retire spawned Doppelganger entities')
 }catch{$failure=($failure+'; checkpoint restore: '+$_.Exception.Message).TrimStart('; ')}}
 try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All14 protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
 [ordered]@{passed=(!$failure);failure=$failure;retainedWorld=($RetainWorldForNextTake-and!$failure);expectedVisualReuse=$ExpectVisualReuse.IsPresent;source='client';stance=$Stance;holstered=[bool]$Holstered;checks=$checks;events=$events;dll=$initialDll;probe=$initialProbe;bankSha256=(Get-FileHash "$PSScriptRoot/doppelganger-holstered-banks-v495/emily-doppelganger-holstered-bank.bin").Hash;scope='Native X while LT is held cancels aimed Doppelganger in the specified ground/equipment context. Verify entry/aim body, no cast-release, source activation/mana/clone-scene absence, no remote clone/FX and full-weight stance/equipment recovery. Local first-person remains native. General interruptions and full power/shared gameplay remain separate. Checkpoint and protected14 restoration mandatory.'}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_NATIVE_DOPPELGANGER_NETWORK $($checks.Count) checks"


