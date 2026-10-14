param([ValidatePattern('^[a-z0-9-]+$')][string]$Take,[ValidateRange(-15,45)][int]$Pitch=0)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(!$Take-or(Test-Path $out)){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
$pids=@{};$inventory=@{};$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$capture=$null;$monitor=$null;$ownedMonitor=$null;$targetMonitor=$null;$dirty=$false;$failure=$null
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
  foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_potion_fixture off','coop_bendtime_fixture off','coop_darkvision_fixture off','coop_mesmerize_fixture off','coop_domino_fixture off','coop_swarm_fixture off','coop_possession_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Clean living fixture required'}
  $inventory[$r]=Q $r '--player-inventory';$inventory[$r]|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-before-$r.json"
 }
 $preflight=Q host '--watch-possession-state' 100
 $preflight|ConvertTo-Json -Depth 8|Set-Content "$out/probe-preflight.json"
 if(!$preflight.Count){throw 'Read-only target watcher preflight emitted no records'}
 $dirty=$true
 C host 'setviewpos -48.59 -155.07 68.1 90 0';Start-Sleep -Milliseconds 800
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch $Pitch -Evidence "$out/source-camera.json" -Quick
 $ground=Q host '--player-states';$ground|ConvertTo-Json -Depth 10|Set-Content "$out/source-grounded.json"
 if(!$ground.motionValid-or$ground.motion-ne0-or$ground.stance-ne0-or[Math]::Abs([double]$ground.position[2]-66.4370575)-gt.03){throw 'Corvo is not grounded on measured test floor'}
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
 $eligible=@($catalog.npcs|Where-Object {$_.unique-and$_.reciprocal-and$_.spawnerName-in@('ld_spawner_guard_tutorial_1','ld_spawner_guard_tutorial_2')-and$_.definition-eq'models/characters/def/generic/dunwall_guard_male_small_01.def'-and[Math]::Abs([double]$_.position[2]-66.4)-lt.5-and[Math]::Pow([double]$_.position[0]+48.59,2)+[Math]::Pow([double]$_.position[1]+155.07,2)-lt400}|Sort-Object {[Math]::Pow([double]$_.position[0]+48.59,2)+[Math]::Pow([double]$_.position[1]+155.07,2)})
 if(!$eligible.Count){throw 'No verified upper-floor tutorial guard within twenty metres'}
 $target=$eligible[0];$yaw=[Math]::Atan2([double]$target.position[1]+155.07,[double]$target.position[0]+48.59)*180/[Math]::PI
 C host ('setviewpos -48.59 -155.07 68.1 {0} 0' -f $yaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
 [ordered]@{target=$target;yaw=$yaw;placement='Only yaw aims at verified native guard; original player floor retained'}|ConvertTo-Json -Depth 8|Set-Content "$out/aim-guard.json"
 Q host '--components' $target.handle|ConvertTo-Json -Depth 12|Set-Content "$out/target-components-before.json"
 $checks.Add('Exact human-upgrade native quantity acquired and stable reciprocal tutorial guard selected by read-only world identity')

 $hwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64();if(!$hwnd){throw 'Source window missing'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','16','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();$capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 Start-Sleep -Milliseconds 600;if($capture.HasExited){throw 'Source capture failed'}
 $monitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.host,'--watch-midnight','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/midnight.ndjson" -RedirectStandardError "$out/midnight.stderr" -PassThru
 $ownedMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.host,'--watch-possession-scenes','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/owned-scenes.ndjson" -RedirectStandardError "$out/owned-scenes.stderr" -PassThru
 $targetMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.host,'--watch-possession-state','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/target-state.ndjson" -RedirectStandardError "$out/target-state.stderr" -PassThru
 $w.Restart();$cast=$false;$exitSent=$false
 while($w.ElapsedMilliseconds-lt14000){
  if(!$cast-and$w.ElapsedMilliseconds-ge1100){
   $fresh=@((Q host '--world-npcs').npcs|Where-Object {$_.handle-eq$target.handle-and$_.spawnerName-eq$target.spawnerName-and$_.reciprocal-and$_.unique})
   if($fresh.Count-ne1){throw 'Original guard identity changed before cast'}
   $yaw=[Math]::Atan2([double]$fresh[0].position[1]+155.07,[double]$fresh[0].position[0]+48.59)*180/[Math]::PI
   C host ('setviewpos -48.59 -155.07 68.1 {0} 0' -f $yaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
   $fresh[0]|ConvertTo-Json -Depth 8|Set-Content "$out/aim-guard-at-cast.json"
   I host 255 500;$cast=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native Corvo LT500 then release at freshly revalidated human target'})}
  if($cast-and!$exitSent-and$w.ElapsedMilliseconds-ge7600){$exitState=Q host '--player-states';if($exitState.stance-ne18-or$exitState.defaultMode){throw 'No occupied-host mode before leave-host input'};I host 255 120;$exitSent=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native LT120 to leave host body'})}
  $entities=Q host '--entities'
  $rows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;fixture=(S host '.possession-fixture.json');isolation=(S host '.observer-isolation.json');player=(O host '--player-states');sword=(O host '--player-sword');power=(O host '--player-possession');component=(O host '--object' $power.component);targetComponents=(O host '--components' $target.handle);entities=@($entities.entities|Where-Object {$_.definitionName-match'(?i)(possession|rat)'})})
  Start-Sleep -Milliseconds 30
 }
 I host;$manaAfter=& "$PSScriptRoot/Read-NativeMana.ps1" -Role host -Evidence "$out/mana-after.json"
 $monitor.WaitForExit(4000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){throw 'Raw Midnight watcher failed'}
 $samples=@(Get-Content "$out/midnight.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $scenes=@($samples|ForEach-Object instances|Where-Object {$_.state-eq3-and$_.asset-like'*power_possession_cast*'})
 if(!$scenes.Count){throw 'No native Possession cast scene observed'}
 $ownedMonitor.WaitForExit(4000)|Out-Null;if(!$ownedMonitor.HasExited-or$ownedMonitor.ExitCode){throw 'Strict Possession watcher incomplete'}
 $owned=@(Get-Content "$out/owned-scenes.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $accepted=@($owned|Where-Object valid)
 if(!$accepted.Count){throw 'No strict pre-transfer Possession phase observed'}
 if(@($accepted|Where-Object {$_.variant-eq28-and$_.durationMs-eq400-and($_.activeStates-join',')-eq'26'}).Count-lt2){throw 'Strict successful release with exact target and EnterPossessedHost26 not repeatedly observed'}
 $accepted|ConvertTo-Json -Depth 8|Set-Content "$out/accepted-scenes.json"
 $incomplete=@($scenes|Where-Object {$_.bindingsResearch.Count-ne1-or@($_.bindingsResearch|Where-Object {!$_.reciprocal}).Count})
 [ordered]@{rawActive=$scenes.Count;incompleteRaw=$incomplete.Count;strictAccepted=$accepted.Count;strictRejected=($owned.Count-$accepted.Count);scope='Raw observations may catch incomplete loop initialization. Only double-validated arm/equipment scenes are accepted.'}|ConvertTo-Json|Set-Content "$out/reader-summary.json"
 $checks.Add('Strict probe observes entry/aim and successful release400ms with EnterPossessedHost26/internal0 and exact NPC target; runtime transport remains entry/aim only')
 [ordered]@{scenes=@($scenes|Group-Object asset,subscene|ForEach-Object {[ordered]@{scene=$_.Name;samples=$_.Count;durationMs=@($_.Group.sceneDurationMs|Sort-Object -Unique)}});manaBefore=$manaBefore.currentCandidate;manaAfter=$manaAfter.currentCandidate;nativeStateSets=@($rows|ForEach-Object {($_.player.active.id|Sort-Object)-join','}|Sort-Object -Unique);observedEntityDefinitions=@($rows.entities.definitionName|Sort-Object -Unique);scope='Source-only native phase/lifecycle observation in isolated AI-held Dunwall; no remote gameplay or body binding'}|ConvertTo-Json -Depth 12|Set-Content "$out/possession-observations.json"
 $checks.Add('Native input produces reciprocal Possession cast scenes; observed phase set recorded without assuming complete coverage')
 $scenes|ConvertTo-Json -Depth 16|Set-Content "$out/possession-scenes.json"
 $targetMonitor.WaitForExit(4000)|Out-Null;if(!$targetMonitor.HasExited-or$targetMonitor.ExitCode){throw 'Possession target watcher failed'}
 $targets=@(Get-Content "$out/target-state.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $occupiedRows=@($rows|Where-Object {$_.player.stance-eq18-and!$_.player.defaultMode})
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
  if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($after.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Inventory quantities differ'}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Player or partner health changed'}
 }
 $checks.Add('Both original inventory quantities and player/partner health retained; world reset still required')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_possession_fixture off'}catch{}}
 foreach($job in @($capture,$monitor,$ownedMonitor,$targetMonitor)){if($job){$job.WaitForExit(18000)|Out-Null;if(!$job.HasExited-or$job.ExitCode){$failure=($failure+'; capture/watcher failed').TrimStart('; ')}}}
 foreach($r in 'host','client'){try{C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 22|Set-Content "$out/states.json"
 if($dirty){try{
  & "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log"
  foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'}}
  $p=Q host '--player-possession';if($p.quantity-or$p.equipped-or$p.started){throw 'Possession remains acquired after checkpoint'}
  $u=@((Q host '--player-inventory').items|Where-Object name -eq 'components/characters/player/base/attributes/possession_parasite.inventoryitem');if($u.Count-ne1-or$u[0].quantity){throw 'Human upgrade remains acquired after checkpoint'}
  $checks.Add('Both isolated checkpoints restore original health/inventory and clear the temporary Possession acquisition')
 }catch{$failure=($failure+'; checkpoint restore: '+$_.Exception.Message).TrimStart('; ')}}
 try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All14 protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
 [ordered]@{passed=(!$failure);failure=$failure;source='host';checks=$checks;events=$events;dll=$initialDll;probe=$initialProbe;scope='Native source Corvo Possession acquisition, controller input, raw scene clocks/state/equipment and candidate target/entity observations. AI held in the isolated Dunwall fixture; finite native human-upgrade and verified guard-target experiment; generation-validated human target, native internal state lifecycle, release400ms and normal-input exit investigated; remote movement/VFX/gameplay remain unverified. Checkpoint and protected save restoration mandatory.'}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_NATIVE_POSSESSION_OBSERVATION $($checks.Count) checks"


