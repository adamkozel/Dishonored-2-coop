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
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid;I $r
  foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_potion_fixture off','coop_bendtime_fixture off','coop_darkvision_fixture off','coop_mesmerize_fixture off','coop_domino_fixture off','coop_swarm_fixture off','coop_possession_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Clean living fixture required'}
  $inventory[$r]=Q $r '--player-inventory';$inventory[$r]|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-before-$r.json"
 }
 $dirty=$true
 C host 'setviewpos -48.59 -155.07 68.1 90 0';C client 'setviewpos -44.39 -153.27 68.1 203 0';
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch -3 -Evidence "$out/observer-camera.json" -Quick
Start-Sleep -Milliseconds 800
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch $Pitch -Evidence "$out/source-camera.json" -Quick
 $ground=Q host '--player-states';$ground|ConvertTo-Json -Depth 10|Set-Content "$out/source-grounded.json"
 if(!$ground.motionValid-or$ground.motion-ne0-or$ground.stance-ne0-or[Math]::Abs([double]$ground.position[2]-66.4)-gt.1){throw 'Corvo is not grounded on measured test floor'}
 $before=Q host '--player-possession';$before|ConvertTo-Json -Depth 8|Set-Content "$out/before.json"
 if($before.quantity-or$before.equipped-or$before.started){throw 'Possession already owned or active'}
 $manaBefore=& "$PSScriptRoot/Read-NativeMana.ps1" -Role host -Evidence "$out/mana-before.json"
 C host 'coop_possession_fixture 40000';$w=[Diagnostics.Stopwatch]::StartNew()
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
 & "$lab/Send-D2Input.ps1" -Role host -Button B -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 500
 $context=Q host '--player-states';$context|ConvertTo-Json -Depth 10|Set-Content "$out/context-before.json"
 if($context.stance-ne1-or!$context.motionValid-or$context.motion-ne1-or!(Q host '--player-sword').equipped){throw 'Exact native armed crouch not established'}
 $checks.Add('Native Sneaking6 and armed sword verified before crouched cast')
 $bankPin=Get-Content "$PSScriptRoot/possession-crouch-bank-manifest-v363.json" -Raw|ConvertFrom-Json|Where-Object role -eq 'corvo'
 if((Get-FileHash "$PSScriptRoot/possession-crouch-banks-v363/corvo-possession-crouch-bank.bin").Hash-ne$bankPin.sha256){throw 'Candidate bank hash differs'}
 Copy-Item "$PSScriptRoot/possession-crouch-banks-v363/corvo-possession-crouch-bank.bin" "$root/work/lab-instances/client/coop-status.json.movement.bin"
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 600;C client 'coop_movement_bank on'
 $w.Restart();do{$m=S client '.movement.json';$stream=S client '.shadow-stream.json';if($m.active-and$m.version-eq53-and$m.weight-gt.9-and$m.presentedDrawn-and$stream.ready-and$stream.armed){break};Start-Sleep -Milliseconds 50}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne53-or$m.weight-lt.9-or!$m.presentedDrawn-or!$stream.ready-or!$stream.armed){throw 'Exact Possession bank/native Steam stream not ready'}
 $checks.Add('Remote Corvo uses exact bank53/318 and native Steam pose transport')
 $hwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64();if(!$hwnd){throw 'Source window missing'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','16','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take-source.mp4")
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();$capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 $recorders+=$capture
 $hwnd=(Get-Process -Id $pids.client).MainWindowHandle.ToInt64();if(!$hwnd){throw 'Observer window missing'}
 $idx=[Array]::IndexOf($args,($args|Where-Object {$_ -like 'gfxcapture=*'}));if($idx-lt0){throw 'Capture argument absent'}
 $args[$idx]="gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra";$args[-1]="$PSScriptRoot/$Take.mp4"
 $recorders+=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture-peer.log" -RedirectStandardOutput "$out/capture-peer.stdout" -PassThru
 Start-Sleep -Milliseconds 600;if(@($recorders|Where-Object HasExited).Count){throw 'Source capture failed'}
 $monitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.host,'--watch-possession-scenes','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/power-scenes.ndjson" -RedirectStandardError "$out/power-scenes.stderr" -PassThru
 Start-Sleep -Milliseconds 100;if($monitor.HasExited){throw 'Strict Possession watcher exited before input; see stderr'}
 $w.Restart();$cast=$false
 while($w.ElapsedMilliseconds-lt14000){
  if(!$cast-and$w.ElapsedMilliseconds-ge1100){I host 255 1800;$cast=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native Corvo LT1800 then release at grounded floor'})}
  $m=S client '.movement.json';$stream=S client '.shadow-stream.json'
  $rows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;player=(Q host '--player-states');movement=$m;stream=$stream})
  if(!$m.active-or!$stream.ready-or!$stream.armed){throw "Native transport/presentation retired: $($stream.result)"}
  Start-Sleep -Milliseconds 30
 }
 I host;$manaAfter=& "$PSScriptRoot/Read-NativeMana.ps1" -Role host -Evidence "$out/mana-after.json"
 $monitor.WaitForExit(4000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){throw 'Raw Midnight watcher failed'}
 $samples=@(Get-Content "$out/power-scenes.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $scenes=@($samples|Where-Object valid)
 foreach($v in 26,27){
  if(!@($scenes|Where-Object variant -eq $v).Count){throw "Strict native Possession phase $v absent"}
  if(!@($rows|Where-Object {$_.movement.powerVariant-eq$v-and$_.movement.powerWeight-gt.5}).Count){throw "Remote Possession phase $v absent"}
 }
 if(@($scenes|Where-Object {!$_.armHandle-or!$_.equipmentHandle}).Count-or@($scenes.armHandle|Sort-Object -Unique).Count-ne1-or@($scenes.equipmentHandle|Sort-Object -Unique).Count-ne1){throw 'Source Possession ownership changed'}
 $checks.Add('Both observed native phases with reciprocal Corvo left-arm/equipment ownership drive remote full-body phases')
 $sourceStream=S host '.shadow-stream.json';$peerStream=S client '.shadow-stream.json'
 [ordered]@{source=$sourceStream;peer=$peerStream}|ConvertTo-Json -Depth 12|Set-Content "$out/power-event-history.json"
 $sent=@($sourceStream.powerSentHistory|Where-Object {$_.variant-in26,27})
 $received=@($peerStream.powerReceivedHistory|Where-Object {$_.variant-in26,27})
 if(!$sent.Count-or$sent.Count-ne$received.Count){throw 'Possession sender/receiver retained event counts differ'}
 if(Compare-Object @($sent|ForEach-Object {"$($_.event):$($_.variant):$($_.durationMs)"}) @($received|ForEach-Object {"$($_.event):$($_.variant):$($_.durationMs)"})){throw 'Possession accepted receiver events differ from sent events'}
 $phaseCoverage=@(foreach($v in 26,27){
  $nativeIds=@($scenes|Where-Object variant -eq $v|ForEach-Object handle|Sort-Object -Unique)
  $sourceEvents=@($sent|Where-Object variant -eq $v)
  $nativeSent=@($sourceEvents.nativeScene|Sort-Object -Unique)
  if(!$nativeIds.Count-or(Compare-Object $nativeIds $nativeSent)){throw "Phase $v strict-native scene IDs differ from actual sent scene IDs"}
  [pscustomobject]@{variant=$v;nativeHandles=$nativeIds;sourceEvents=$sourceEvents;acceptedEvents=@($received|Where-Object variant -eq $v);sampledPoseEvents=@($rows|Where-Object {$_.movement.powerVariant-eq$v-and$_.movement.powerWeight-gt.5}|ForEach-Object {$_.movement.powerEvent}|Sort-Object -Unique)}
 })
 $phaseCoverage|ConvertTo-Json -Depth 8|Set-Content "$out/phase-incarnations.json"
 $checks.Add('Bounded send/accept histories exactly match strict native scene incarnations, including sub-status-interval final aim loops')
 $continuity=@($rows|Where-Object {$_.ms-ge900-and$_.ms-le6200})
 if($continuity.Count-lt20-or@($continuity|Where-Object {$_.movement.weight-lt.95-or$_.movement.clockRejection.count-ne0}).Count){throw 'Possession root/body continuity failed'}
 [ordered]@{samples=$continuity.Count;minimumBodyWeight=($continuity.movement.weight|Measure-Object -Minimum).Minimum;rejections=@($continuity|ForEach-Object {$_.movement.clockRejection.count}|Sort-Object -Unique)}|ConvertTo-Json|Set-Content "$out/continuity.json"
 $checks.Add('Possession cast preserves root/body continuity without rejected presentation clocks')
 if($m.powerWeight-gt.02-or$m.weight-lt.9-or$stream.remoteState-ne0){throw 'Remote cast did not recover to crouched idle'}
 $low=@($rows|Where-Object {$_.movement.powerVariant-in26,27-and$_.movement.powerWeight-gt.5})
 if(!$low.Count-or@($low|Where-Object {$_.player.stance-ne1-or$_.movement.stanceMix-lt.95}).Count){throw 'Crouched source/remote stance diverged during cast'}
 $checks.Add('Both native source and remote body remain crouched throughout sampled cast phases')
 $sword=Q client '--avatar-sword' (S client).probeActorHandle;$sword|ConvertTo-Json -Depth 8|Set-Content "$out/avatar-sword-after.json"
 if(!$sword.equipped-or!(Q host '--player-sword').equipped){throw 'Sword selection changed'}
 $checks.Add('Remote body recovers to crouched idle with native sword selection retained')
 if([Math]::Abs($manaBefore.currentCandidate-$manaAfter.currentCandidate)-gt.001){throw 'Empty-target cast unexpectedly consumed mana'}
 $peerInventory=Q client '--player-inventory'
 $peerPower=@($peerInventory.items|Where-Object name -eq 'models/powers/possession/possession.inventoryitem')
 if($peerPower.Count-ne1-or$peerPower[0].quantity-ne0-or$peerPower[0].component-ne'0x0'){throw 'Remote cosmetic body acquired native Possession'}
 $checks.Add('Empty-target source keeps mana and peer acquires no native Possession component; animation executes no native transfer')
 $scenes|ConvertTo-Json -Depth 16|Set-Content "$out/possession-scenes.json"
 C host 'coop_possession_fixture off';$w.Restart()
 do{$f=S host '.possession-fixture.json';if(!$f.active){break};Start-Sleep -Milliseconds 70}while($w.ElapsedMilliseconds-lt6500)
 if($f.active-or$f.result-ne'quantity_and_selection_restored'){throw 'Native Possession inventory restoration unconfirmed'}
 foreach($r in 'host','client'){
  $after=Q $r '--player-inventory';$after|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-after-$r.json"
  if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($after.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Inventory quantities differ'}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Player or partner health changed'}
 }
 $checks.Add('Both original inventory quantities and player/partner health retained; world reset still required')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_possession_fixture off'}catch{}}
 foreach($job in (@($recorders)+@($monitor))){if($job){$job.WaitForExit(18000)|Out-Null;if(!$job.HasExited-or$job.ExitCode){$failure=($failure+'; capture/watcher failed').TrimStart('; ')}}}
 foreach($r in 'host','client'){try{C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 22|Set-Content "$out/states.json"
 if($dirty){try{
  & "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log"
  foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'}}
  $p=Q host '--player-possession';if($p.quantity-or$p.equipped-or$p.started){throw 'Possession remains acquired after checkpoint'}
  foreach($r in 'host','client'){
   $restored=Q $r '--player-inventory';$restored|ConvertTo-Json -Depth 10|Set-Content "$out/inventory-checkpoint-restored-$r.json"
   if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($restored.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Complete checkpoint inventory differs'}
  }
  $checks.Add('Both isolated checkpoints restore complete original health/inventory and clear the native Possession acquisition')
 }catch{$failure=($failure+'; checkpoint restore: '+$_.Exception.Message).TrimStart('; ')}}
 try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All14 protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
 [ordered]@{passed=(!$failure);failure=$failure;source='host';checks=$checks;events=$events;dll=$initialDll;probe=$initialProbe;bankSha256=(Get-FileHash "$PSScriptRoot/possession-crouch-banks-v363/corvo-possession-crouch-bank.bin").Hash;scope='Crouched armed native Corvo empty-target Possession entry333ms/aim200ms drive remote full-body bank53/318 through native Steam protocol50. Source scene retirement drives idle recovery; no invented successful-target release. Local first-person remains native; peer animation executes no transfer, VFX or damage event. Standing human-target lifecycle and target particles have separate evidence. This take tests crouched empty-target entry/aim and recovery only; native draw overlap, crouched successful-host release, shared NPC authority and overall visual acceptance remain pending. Checkpoint and protected14 restoration mandatory.'}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_NATIVE_POSSESSION_OBSERVATION $($checks.Count) checks"
