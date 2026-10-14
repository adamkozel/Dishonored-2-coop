param([ValidatePattern('^[a-z0-9-]+$')][string]$Take,[ValidateRange(-15,45)][int]$Pitch=12,[switch]$AimGuard)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(!$Take-or(Test-Path $out)){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
$pids=@{};$inventory=@{};$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$capture=$null;$monitor=$null;$targetMonitor=$null;$linkMonitor=$null;$strictMonitor=$null;$dirty=$false;$failure=$null
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
 $dirty=$true;C client 'setviewpos -44.39 -153.27 68.1 90 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch $Pitch -Evidence "$out/source-camera.json" -Quick
 $before=Q client '--player-domino';$before|ConvertTo-Json -Depth 7|Set-Content "$out/before.json"
 if($before.quantity-or$before.equipped-or$before.started){throw 'Domino already owned or active'}
 $manaBefore=& "$PSScriptRoot/Read-NativeMana.ps1" -Role client -Evidence "$out/mana-before.json"
 $dirty=$true;C client 'coop_domino_fixture 40000';$w=[Diagnostics.Stopwatch]::StartNew()
 do{$f=S client '.domino-fixture.json';if($f.result-eq'native_domino_ready'){break};Start-Sleep -Milliseconds 60}while($w.ElapsedMilliseconds-lt6000)
 if($f.result-ne'native_domino_ready'){
  $f|ConvertTo-Json -Depth 6|Set-Content "$out/acquisition-fixture.json"
  $debugInv=Q client '--player-inventory';$debugInv|ConvertTo-Json -Depth 10|Set-Content "$out/acquisition-inventory.json"
  $debugItem=@($debugInv.items|Where-Object {$_.name-eq'models/powers/phantasmallink/phantasmal_link.inventoryitem'})
  if($debugItem.Count-eq1-and$debugItem[0].component-ne'0x0'){
   $obj=Q client '--object' $debugItem[0].component;$obj|ConvertTo-Json -Depth 10|Set-Content "$out/acquisition-component.json"
   Q client '--object' $before.entry|ConvertTo-Json -Depth 10|Set-Content "$out/acquisition-entry.json"
   foreach($field in '0x8','0x10'){$ptr=($obj.words|Where-Object offset -eq $field).word;if($ptr-and$ptr-ne'0x0'){Q client '--object' $ptr|ConvertTo-Json -Depth 10|Set-Content "$out/acquisition-owner-$field.json"}}
   Q client '--components' $debugItem[0].handle|ConvertTo-Json -Depth 10|Set-Content "$out/acquisition-components.json"
  }
  throw "Native Domino acquisition failed: $($f.result)"
 }
 $power=Q client '--player-domino';$power|ConvertTo-Json -Depth 7|Set-Content "$out/enabled.json"
 if(!$power.equipped-or!$power.started-or!$power.instantiated-or$power.quantity-ne1){throw 'Native Domino equipment not confirmed'}
 $checks.Add('Exact native Emily Domino item acquired and selected without directly invoking power or VFX')
 $hwnd=(Get-Process -Id $pids.client).MainWindowHandle.ToInt64();if(!$hwnd){throw 'No source window'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','16','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();$capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 Start-Sleep -Milliseconds 600;if($capture.HasExited){throw 'Capture failed'}
 $monitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-midnight','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/midnight.ndjson" -RedirectStandardError "$out/midnight.stderr" -PassThru
 $strictMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-domino-scenes','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/domino-scenes.ndjson" -RedirectStandardError "$out/domino-scenes.stderr" -PassThru
 $targetMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-domino-targets','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/domino-targets.ndjson" -RedirectStandardError "$out/domino-targets.stderr" -PassThru
 $linkMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-domino-links','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/domino-links.ndjson" -RedirectStandardError "$out/domino-links.stderr" -PassThru
 $w.Restart();$cast=$false;$secondCast=$false;$networkSnapshot=$false
 while($w.ElapsedMilliseconds-lt14000){
  if(!$cast-and$w.ElapsedMilliseconds-ge1100){
   if($AimGuard){
    $catalog=Q client '--world-npcs';$catalog|ConvertTo-Json -Depth 8|Set-Content "$out/aim-npc-catalog.json"
    $eligible=@($catalog.npcs|Where-Object {$_.unique-and$_.reciprocal-and$_.spawnerName-in@('ld_spawner_guard_tutorial_1','ld_spawner_guard_tutorial_2')-and$_.definition-eq'models/characters/def/generic/dunwall_guard_male_small_01.def'-and[Math]::Abs([double]$_.position[2]-66.4)-lt.5-and[Math]::Pow([double]$_.position[0]+44.39,2)+[Math]::Pow([double]$_.position[1]+153.27,2)-lt400}|Sort-Object {[Math]::Pow([double]$_.position[0]+44.39,2)+[Math]::Pow([double]$_.position[1]+153.27,2)})
    if(!$eligible.Count){throw 'No verified upper-floor tutorial guard within twenty metres'}
    $target=$eligible[0];$yaw=[Math]::Atan2([double]$target.position[1]+153.27,[double]$target.position[0]+44.39)*180/[Math]::PI
    C client ('setviewpos -44.39 -153.27 68.1 {0} 0' -f $yaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
    [ordered]@{target=$target;yaw=$yaw;placement='Unchanged proven source floor; only yaw points toward read-only verified native guard position'}|ConvertTo-Json -Depth 6|Set-Content "$out/aim-guard.json"
   }
   I client 255 1800;$cast=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native LT held 1800ms then released'})
  }
  if($cast-and!$secondCast-and$w.ElapsedMilliseconds-ge5500){
   $catalog2=Q client '--world-npcs';$catalog2|ConvertTo-Json -Depth 8|Set-Content "$out/second-target-catalog.json"
   $lower=@($catalog2.npcs|Where-Object {$_.unique-and$_.reciprocal-and$_.spawnerName-eq'ld_spawner_guard_tutorial_1'-and$_.definition-eq'models/characters/def/generic/dunwall_guard_male_small_01.def'-and[Math]::Abs([double]$_.position[2]-61.0278)-lt.5})
   if($lower.Count-ne1){throw 'Exact lower-floor tutorial guard unavailable'}
   $target2=$lower[0];$dx=[double]$target2.position[0]+44.39;$dy=[double]$target2.position[1]+153.27;$dz=68.1-([double]$target2.position[2]+1.65)
   $yaw2=[Math]::Atan2($dy,$dx)*180/[Math]::PI;$pitch2=[Math]::Round([Math]::Atan2($dz,[Math]::Sqrt($dx*$dx+$dy*$dy))*180/[Math]::PI)
   if($pitch2-lt0-or$pitch2-gt40){throw 'Second native target outside bounded balcony camera pitch'}
   C client ('setviewpos -44.39 -153.27 68.1 {0} 0'-f$yaw2.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
   & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch $pitch2 -Evidence "$out/second-target-pitch.json" -Quick
   [ordered]@{target=$target2;yaw=$yaw2;pitch=$pitch2;note='Native controller aim from unchanged upper balcony source; no NPC relocation or spawn'}|ConvertTo-Json -Depth 6|Set-Content "$out/second-target-aim.json"
   Start-Sleep -Milliseconds 350;I client 255 1800;$secondCast=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Second native LT hold1800ms toward lower tutorial guard'})
  }
  $scene=Q client '--midnight';$entities=Q client '--entities'
  $rows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;player=(Q client '--player-states');midnight=$scene;power=(Q client '--player-domino');component=(Q client '--object' $power.component);effects=(OptionalFx client);entities=@($entities.entities|Where-Object {$_.definitionName-match'phantasmal'})})
  if(!$networkSnapshot-and$secondCast-and$w.ElapsedMilliseconds-ge10500){
   $cp=Q client '--object' $power.component;$cp|ConvertTo-Json -Depth 10|Set-Content "$out/network-equipment.json"
   $networkAddress=($cp.words|Where-Object offset -eq '0xb8').word
   if(!$networkAddress-or$networkAddress-eq'0x0'){throw 'No native Domino network component to inspect'}
   $net=Q client '--object' $networkAddress;$net|ConvertTo-Json -Depth 10|Set-Content "$out/network-component.json"
   if($net.type-ne'.?AVarkCpntPhantasmalNetwork@@'){throw 'Unexpected native network component type'}
   foreach($field in '0x8','0x10'){$ptr=($net.words|Where-Object offset -eq $field).word;Q client '--object' $ptr|ConvertTo-Json -Depth 10|Set-Content "$out/network-owner-$field.json"}
   $list=($net.words|Where-Object offset -eq '0x28').word;$count=[Convert]::ToUInt64(($net.words|Where-Object offset -eq '0x30').word.Substring(2),16)-band0xffffff
   if($count-gt4){throw 'Native selected-target count outside bounded research scope'}
   if($count){
    $table=Q client '--object' $list;$table|ConvertTo-Json -Depth 10|Set-Content "$out/network-selected-table.json"
    for($index=0;$index-lt$count;$index++){
     $ptr=($table.words|Where-Object offset -eq ('0x{0:x}'-f($index*8))).word
     Q client '--object' $ptr|ConvertTo-Json -Depth 10|Set-Content "$out/network-node-$index.json"
    }
   }
   $connections=Q client '--object' ('0x{0:x}'-f([Convert]::ToUInt64($networkAddress.Substring(2),16)+0x15b0));$connections|ConvertTo-Json -Depth 10|Set-Content "$out/network-connections-header.json"
   $connectionList=($connections.words|Where-Object offset -eq '0x0').word;$connectionCount=[Convert]::ToUInt64(($connections.words|Where-Object offset -eq '0x8').word.Substring(2),16)-band0xffffff
   if($connectionCount-gt16){throw 'Native connection count exceeds bounded observation'}
   if($connectionCount){$connectionTable=Q client '--object' $connectionList;$connectionTable|ConvertTo-Json -Depth 10|Set-Content "$out/network-connections-table.json"
    for($n=0;$n-lt$connectionCount;$n++){$pointer=($connectionTable.words|Where-Object offset -eq ('0x{0:x}'-f($n*8))).word;Q client '--object' $pointer|ConvertTo-Json -Depth 10|Set-Content "$out/network-connection-$n.json"}
   }
   Q client '--entities'|ConvertTo-Json -Depth 10|Set-Content "$out/network-entities.json"
   Q client '--world-npcs'|ConvertTo-Json -Depth 10|Set-Content "$out/network-npcs.json"
   Q client '--domino-effects'|ConvertTo-Json -Depth 10|Set-Content "$out/network-effects.json"
   [ordered]@{nativeComponent=$networkAddress;count=$count;list=$list;note='Read-only raw objects after native controller selection. Field semantics and target ownership must be validated before any network serialization.'}|ConvertTo-Json|Set-Content "$out/network-snapshot.json"
   $networkSnapshot=$true
  }
  Start-Sleep -Milliseconds 25
 }
 I client;$manaAfter=& "$PSScriptRoot/Read-NativeMana.ps1" -Role client -Evidence "$out/mana-after.json"
 $monitor.WaitForExit(4000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){throw 'Native scene watcher failed'}
 $rawSamples=@(Get-Content "$out/midnight.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $rawScenes=@($rawSamples|ForEach-Object {$_.instances}|Where-Object {$_.state-eq3-and$_.asset-like'*power_phantasmal_cast*'})
 [ordered]@{inputSent=$cast;sceneSamples=$rawScenes.Count;subscenes=@($rawScenes|Group-Object subscene|ForEach-Object {[ordered]@{name=$_.Name;samples=$_.Count;durations=@($_.Group.sceneDurationMs|Sort-Object -Unique)}});manaBefore=$manaBefore.currentCandidate;manaAfter=$manaAfter.currentCandidate;nativeStates=@($rows.player);raw=$rawScenes}|ConvertTo-Json -Depth 16|Set-Content "$out/domino-observations.json"
 if(!$cast-or!$rawScenes.Count){throw 'No native Domino phase observed after controller input'}
 if(@($rawScenes|Where-Object {@($_.bindingsResearch|Where-Object {!$_.reciprocal}).Count}).Count){throw 'Observed Domino scene includes a nonreciprocal entity binding'}
 $checks.Add('Controller input produces recorded native PhantasmalLink scenes with reciprocal bindings; phase names and counts are observed, not inferred')
 $strictMonitor.WaitForExit(4000)|Out-Null;if(!$strictMonitor.HasExited-or$strictMonitor.ExitCode){throw 'Strict Domino watcher failed'}
 $strict=@(Get-Content "$out/domino-scenes.ndjson"|ForEach-Object {$_|ConvertFrom-Json}|Where-Object valid)
 foreach($v in 19,20,22){if(!@($strict|Where-Object variant -eq $v).Count){throw "Native Domino strict phase $v was not observed"}}
 if(($AimGuard-or@($rawScenes|Where-Object subscene -eq 'Cast_Select').Count)-and!@($strict|Where-Object variant -eq 21).Count){throw 'Native Domino strict selection phase was not observed'}
 if(@($strict.armHandle|Sort-Object -Unique).Count-ne1-or@($strict.equipmentHandle|Sort-Object -Unique).Count-ne1){throw 'Domino arm/equipment identity changed'}
 $checks.Add('Strict read-only Domino observer verifies observed native phases, exact state72 or native exit recovery, reciprocal player/left arm and one owned equipment incarnation; a targeted take additionally requires selection')
 $targetMonitor.WaitForExit(4000)|Out-Null;if(!$targetMonitor.HasExited-or$targetMonitor.ExitCode){throw 'Strict native target watcher failed'}
 $targetRows=@(Get-Content "$out/domino-targets.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $markedRows=@($targetRows|Where-Object {$_.valid-and$_.present-and@($_.targets|Where-Object hasMark).Count})
 [ordered]@{samples=$targetRows.Count;valid=@($targetRows|Where-Object valid).Count;marked=$markedRows.Count;maximumReadMs=($targetRows.readMs|Measure-Object -Maximum).Maximum;failures=@($targetRows|Where-Object {!$_.valid}|Group-Object stage|ForEach-Object {[ordered]@{stage=$_.Name;count=$_.Count}});handles=@($markedRows.targets.handle|Sort-Object -Unique)}|ConvertTo-Json -Depth 8|Set-Content "$out/target-reader-summary.json"
 if($markedRows.Count-lt10){throw 'Insufficient strictly owned native Domino target/mark observations'}
 if(!$secondCast-or!@($markedRows|Where-Object {$_.targets.Count-eq2}).Count){throw 'Two native selected targets not established'}
 if($count-ne2-or$connectionCount-ne2){throw 'Expected two selected nodes with two directed native connections'}
 $checks.Add('Strict reader verifies equipment-derived native network, bounded selected-node pool, reciprocal NPC animation generation and the exact temporary mark owner in at least ten real samples, including two selected targets and two directed raw connections')


 $linkMonitor.WaitForExit(4000)|Out-Null;if(!$linkMonitor.HasExited-or$linkMonitor.ExitCode){throw 'Strict native link watcher failed'}
 $linkRows=@(Get-Content "$out/domino-links.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $linked=@($linkRows|Where-Object {$_.valid-and$_.present-and$_.connections.Count-eq2-and@($_.connections|Where-Object hasBeam).Count-eq1})
 [ordered]@{samples=$linkRows.Count;valid=@($linkRows|Where-Object valid).Count;linked=$linked.Count;maximumReadMs=($linkRows.readMs|Measure-Object -Maximum).Maximum;failures=@($linkRows|Where-Object {!$_.valid}|Group-Object stage|ForEach-Object {[ordered]@{stage=$_.Name;count=$_.Count}})}|ConvertTo-Json -Depth 8|Set-Content "$out/link-reader-summary.json"
 if($linked.Count-lt30){throw 'Insufficient strictly owned native Domino links with both validated endpoints'}
 $checks.Add('Strict read-only connection reader verifies two selected reciprocal enemies, directed pool membership, one Emily-owned native beam, both endpoint bases and native opening interpolation in at least thirty real samples')
 $checks.Add('Native player state, equipment, raw component/entity and filtered effect samples retained; mana change and targeting outcome remain observations')
 if(!$networkSnapshot){throw 'Native network snapshot absent'}; $checks.Add('Read-only native network, selected-node and target/effect objects preserved without invoking link or damage methods')
 C client 'coop_domino_fixture off';$w.Restart()
 do{$f=S client '.domino-fixture.json';if(!$f.active){break};Start-Sleep -Milliseconds 75}while($w.ElapsedMilliseconds-lt6500)
 if($f.active-or$f.result-ne'quantity_and_selection_restored'){throw 'Native Domino inventory did not retire'}
 foreach($r in 'host','client'){$after=Q $r '--player-inventory';$after|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-after-$r.json";if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($after.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Inventory quantities differ'};if((Q $r '--health' 1).current-ne56){throw 'Player health changed'}}
 $checks.Add('Both native inventories restore and player health stays unchanged; world effects require separate checkpoint cleanup')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_domino_fixture off';C $r 'coop_movement_fixture off'}catch{}}
 if($capture){$capture.WaitForExit(18000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode){$failure=($failure+'; capture failed').TrimStart('; ')}}
 if($monitor){$monitor.WaitForExit(18000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){$failure=($failure+'; scene watcher failed').TrimStart('; ')}}
 if($strictMonitor){$strictMonitor.WaitForExit(18000)|Out-Null;if(!$strictMonitor.HasExited-or$strictMonitor.ExitCode){$failure=($failure+'; strict watcher failed').TrimStart('; ')}}
 if($linkMonitor){$linkMonitor.WaitForExit(18000)|Out-Null;if(!$linkMonitor.HasExited-or$linkMonitor.ExitCode){$failure=($failure+'; link watcher failed').TrimStart('; ')}}
 if($targetMonitor){$targetMonitor.WaitForExit(18000)|Out-Null;if(!$targetMonitor.HasExited-or$targetMonitor.ExitCode){$failure=($failure+'; target watcher failed').TrimStart('; ')}}
 $rows|ConvertTo-Json -Depth 20|Set-Content "$out/states.json"
 if($dirty){try{& "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log";foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'}};$p=Q client '--player-domino';if($p.quantity-or$p.equipped-or$p.started){throw 'Domino remains after checkpoint'};$fx=Q client '--domino-effects';if(@($fx.effects).Count){throw 'Domino effects remain after checkpoint'};$checks.Add('Both isolated checkpoints restore clean health/inventory and no filtered native Domino effects remain')}catch{$failure=($failure+'; checkpoint restore: '+$_.Exception.Message).TrimStart('; ')}}
 try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All fourteen protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
 [ordered]@{passed=(!$failure);failure=$failure;source='client';checks=$checks;events=$events;dll=$initialDll;probe=$initialProbe;scope='Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. No remote body/VFX or gameplay replication. Full isolated inventory/health and checkpoint recovery plus protected hashes are required.'}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_NATIVE_DOMINO_OBSERVATION $($checks.Count) checks"
