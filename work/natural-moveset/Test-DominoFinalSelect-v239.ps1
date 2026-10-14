param([ValidatePattern('^[a-z0-9-]+$')][string]$Take,[ValidateRange(-15,45)][int]$Pitch=12,[switch]$AimGuard)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(!$Take-or(Test-Path $out)){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
$pids=@{};$inventory=@{};$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$finalMonitor=$null;$cancelMonitor=$null;$peerCapture=$null;$networkStarted=$false;$peerRows=[Collections.Generic.List[object]]::new();$capture=$null;$monitor=$null;$targetMonitor=$null;$linkMonitor=$null;$strictMonitor=$null;$dirty=$false;$failure=$null
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
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','40','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take-source.mp4")
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();$capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 $peerArgs=@($args);$peerHwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64()
 $inputIndex=[Array]::IndexOf($peerArgs,($peerArgs|Where-Object {$_ -like 'gfxcapture=*'}));if($inputIndex-lt0-or!$peerHwnd){throw 'Peer capture input unavailable'}
 $peerArgs[$inputIndex]="gfxcapture=hwnd=${peerHwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra";$peerArgs[-1]="$PSScriptRoot/$Take.mp4"
 $peerCapture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $peerArgs -WindowStyle Hidden -RedirectStandardError "$out/capture-peer.log" -RedirectStandardOutput "$out/capture-peer.stdout" -PassThru
 Start-Sleep -Milliseconds 600;if($capture.HasExited){throw 'Capture failed'}
 $monitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-midnight','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/midnight.ndjson" -RedirectStandardError "$out/midnight.stderr" -PassThru
 $strictMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-domino-scenes','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/domino-scenes.ndjson" -RedirectStandardError "$out/domino-scenes.stderr" -PassThru
 $targetMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-domino-targets','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/domino-targets.ndjson" -RedirectStandardError "$out/domino-targets.stderr" -PassThru
 $linkMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-domino-links','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/domino-links.ndjson" -RedirectStandardError "$out/domino-links.stderr" -PassThru
 $w.Restart();$cast=$false;$secondCast=$false;$networkSnapshot=$false
 while($w.ElapsedMilliseconds-lt18000-or($networkStarted-and$peerRows.Count-lt12-and$w.ElapsedMilliseconds-lt23000)){
  if(!$cast-and$w.ElapsedMilliseconds-ge1100){
   if($AimGuard){
    $catalog=Q client '--world-npcs';$catalog|ConvertTo-Json -Depth 8|Set-Content "$out/aim-npc-catalog.json"
    $eligible=@($catalog.npcs|Where-Object {$_.unique-and$_.reciprocal-and$_.spawnerName-eq'ld_spawner_guard_tutorial_2'-and$_.definition-eq'models/characters/def/generic/dunwall_guard_male_small_01.def'-and[Math]::Abs([double]$_.position[2]-66.4)-lt.5-and[Math]::Pow([double]$_.position[0]+44.39,2)+[Math]::Pow([double]$_.position[1]+153.27,2)-lt400}|Sort-Object {[Math]::Pow([double]$_.position[0]+44.39,2)+[Math]::Pow([double]$_.position[1]+153.27,2)})
    if(!$eligible.Count){throw 'No verified upper-floor tutorial guard within twenty metres'}
    $target=$eligible[0];$distance=[Math]::Pow([double]$target.position[0]-$firstX,2)+[Math]::Pow([double]$target.position[1]-$firstY,2)
    if($distance-lt4){throw 'First guard moved within two metres of source'}
    $yaw=[Math]::Atan2([double]$target.position[1]-$firstY,[double]$target.position[0]-$firstX)*180/[Math]::PI
    C client ('setviewpos {0} {1} 68.1 {2} 0'-f$firstX.ToString('F4',$ci),$firstY.ToString('F4',$ci),$yaw.ToString('F4',$ci))
    Start-Sleep -Milliseconds 600
    & "$PSScriptRoot/Aim-NativeDominoGuard-v218.ps1" -Role client -Evidence "$out/native-guard-aim.json"
    [ordered]@{target=$target;yaw=$yaw;anchor=$anchor;placement='Verified grounded source point in the same guard corridor or measured balcony anchor, at least two metres separation and independent native look feedback; no NPC mutation'}|ConvertTo-Json -Depth 6|Set-Content "$out/aim-guard.json"

   }
   I client 255 1800;$cast=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native LT held 1800ms then released'})
  }
  if($cast-and!$secondCast-and$w.ElapsedMilliseconds-ge5500){
   $catalog2=Q client '--world-npcs';$catalog2|ConvertTo-Json -Depth 8|Set-Content "$out/second-target-catalog.json"
   $lower=@($catalog2.npcs|Where-Object {$_.unique-and$_.reciprocal-and$_.spawnerName-eq'ld_spawner_guard_tutorial_1'-and$_.definition-eq'models/characters/def/generic/dunwall_guard_male_small_01.def'-and[double]$_.position[2]-ge60.5-and[double]$_.position[2]-le67-and[Math]::Pow([double]$_.position[0]+48.59,2)+[Math]::Pow([double]$_.position[1]+155.07,2)-lt400})
   if($lower.Count-ne1){throw 'Exact tutorial guard1 outside observed upper/lower patrol area'}
   $target2=$lower[0];$sourceX=[double]$target2.position[0];$sourceY=[double]$target2.position[1]-3.5;$sourceZ=[double]$target2.position[2]+1.7
   $ci=[Globalization.CultureInfo]::InvariantCulture
   C client ('setviewpos {0} {1} {2} 90 0'-f$sourceX.ToString('F4',$ci),$sourceY.ToString('F4',$ci),$sourceZ.ToString('F4',$ci))
   & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch 0 -Evidence "$out/second-target-pitch.json" -Quick
   Start-Sleep -Milliseconds 550;$ground=Q client '--player-states';$ground|ConvertTo-Json -Depth 8|Set-Content "$out/second-source-floor.json"
   if(!$ground.motionValid-or$ground.motion-ne0-or[Math]::Abs([double]$ground.position[2]-[double]$target2.position[2])-gt5){throw 'Source not grounded within measured target elevation range'}
   Start-Sleep -Milliseconds 150;$ground2=Q client '--player-states'
   for($k=0;$k-lt3;$k++){if([Math]::Abs([double]$ground2.position[$k]-[double]$ground.position[$k])-gt.01){throw 'Second source floor still moving'}}
   & "$PSScriptRoot/Aim-NativeDominoGuard-v239.ps1" -Role client -TargetSpawner ld_spawner_guard_tutorial_1 -Evidence "$out/second-native-aim.json"

   [ordered]@{target=$target2;source=@($sourceX,$sourceY,$sourceZ);yaw=90;pitch=0;note='Source-only test moves near exact second guard, validates stable native ground and uses independent native camera/target yaw and elevation feedback. Guard may occupy the stairs above source. No active body stream and no NPC mutation. Movement fixture owns reversible AI holds.'}|ConvertTo-Json -Depth 6|Set-Content "$out/second-target-aim.json"
   Start-Sleep -Milliseconds 350;I client 255 1800;$secondCast=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Second native LT hold1800ms toward tutorial guard1 at its current native patrol position'})
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
   $linkBefore=Q client '--domino-links';$linkBefore|ConvertTo-Json -Depth 10|Set-Content "$out/links-before-stream.json"
   if(!$linkBefore.valid-or@($linkBefore.connections|Where-Object hasBeam).Count-ne1){throw 'One strict native beam required before streaming'}
   $ci=[Globalization.CultureInfo]::InvariantCulture;$observerX=$sourceX-.4;$observerY=$sourceY-.8
   $observerYaw=[Math]::Atan2([double]$target2.position[1]-$observerY,[double]$target2.position[0]-$observerX)*180/[Math]::PI
   C host ('setviewpos {0} {1} {2} {3} 0'-f$observerX.ToString('F4',$ci),$observerY.ToString('F4',$ci),$sourceZ.ToString('F4',$ci),$observerYaw.ToString('F4',$ci))
   & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch 0 -Evidence "$out/peer-camera.json" -Quick
   Copy-Item "$PSScriptRoot/domino-banks-v192/emily-domino-bank.bin" "$root/work/lab-instances/host/coop-status.json.movement.bin"
   $worldBefore=S host '.domino-world.json';$beamBefore=S host '.domino-links.json'
   foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 600;C host 'coop_movement_bank on'
   $readyWatch=[Diagnostics.Stopwatch]::StartNew()
   do{$movement=S host '.movement.json';$stream=S host '.shadow-stream.json';if($movement.active-and$movement.version-eq47-and$movement.weight-gt.9-and$stream.ready-and$stream.armed){break};Start-Sleep -Milliseconds 75}while($readyWatch.ElapsedMilliseconds-lt5000)
   if(!$movement.active-or$movement.version-ne47-or$movement.weight-lt.9-or!$stream.ready-or!$stream.armed){throw 'Native beam transport did not arm'}
   $networkStarted=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native links established before stream; exact body bank and native Steam presentation enabled after source settled'})
   $networkSnapshot=$true
  }
  if($networkStarted){$peerRows.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;movement=(S host '.movement.json');stream=(S host '.shadow-stream.json');marks=(S host '.domino-world.json');links=(S host '.domino-links.json')})}
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
 if(!$networkStarted-or$peerRows.Count-lt10){throw 'Insufficient paired beam samples'}
 $peerWorld=S host '.domino-world.json';$peerLinks=S host '.domino-links.json';$sourceLinks=Q client '--domino-links';$peerFx=Q host '--domino-effects'
 [ordered]@{source=$sourceLinks;peer=$peerLinks;world=$peerWorld;effects=$peerFx}|ConvertTo-Json -Depth 12|Set-Content "$out/network-link-proof.json"
 if($peerLinks.beams.Count-ne1-or$peerLinks.started-ne($beamBefore.started+1)-or$peerLinks.refused-ne$beamBefore.refused-or$peerLinks.updated-lt($beamBefore.updated+20)-or$peerWorld.marks.Count-ne2-or$peerWorld.started-ne($worldBefore.started+2)-or!$peerWorld.leased){throw 'Exactly one stable beam and two owned marks not reproduced'}
 $native=@($sourceLinks.connections|Where-Object hasBeam);$remoteBeam=$peerLinks.beams[0]
 if($native.Count-ne1-or$native[0].id-ne$remoteBeam.sourceId-or$native[0].from-ne$remoteBeam.from-or$native[0].to-ne$remoteBeam.to-or$remoteBeam.owner-ne(S host).probeActorHandle){throw 'Beam incarnation/endpoint keys differ across native Steam'}
 $beamFx=@($peerFx.effects|Where-Object {$_.id-eq$remoteBeam.id-and$_.owner-eq$remoteBeam.owner-and$_.name-eq'models/effects/gameplay/powers/phantasmal_link/links/phlink_links_01.fx'})
 if($beamFx.Count-ne1-or$peerFx.effects.Count-ne3){throw 'Unexpected duplicate or missing receiver effects'}
 $raw=Q host '--object' $beamFx[0].record;$raw|ConvertTo-Json -Depth 8|Set-Content "$out/receiver-beam-record.json"
 $endTransform=@();foreach($offset in 0x80,0x88,0x90,0x98,0xa0,0xa8){$word=($raw.words|Where-Object offset -eq ('0x{0:x}'-f$offset)).word;$bytes=[BitConverter]::GetBytes([Convert]::ToUInt64($word.Substring(2),16));$endTransform+=[BitConverter]::ToSingle($bytes,0);$endTransform+=[BitConverter]::ToSingle($bytes,4)}
 $sourceCompare=Q client '--domino-links';$sourceBeam=@($sourceCompare.connections|Where-Object hasBeam)[0]
 $startDistance=0;$endDistance=0;for($k=0;$k-lt3;$k++){$startDistance+=[Math]::Pow([double]$beamFx[0].origin[$k]-[double]$sourceBeam.start[$k],2);$endDistance+=[Math]::Pow([double]$endTransform[$k]-[double]$sourceBeam.end[$k],2)}
 [ordered]@{source=$sourceBeam;receiverEnd=$endTransform;startSquaredDistance=$startDistance;endSquaredDistance=$endDistance}|ConvertTo-Json -Depth 8|Set-Content "$out/beam-endpoint-comparison.json"
 if($startDistance-gt.25-or$endDistance-gt.25){throw 'Beam endpoints do not follow observed native source'}
 $checks.Add('Native Steam reproduces two owned marks and exactly one native cosmetic beam, preserving its source FX incarnation, opaque target keys and independently read start/end positions; no receiver NPC or Domino gameplay call')
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch 0 -Evidence "$out/deselect-pitch.json" -Quick
 & "$PSScriptRoot/Aim-NativeDominoGuard-v239.ps1" -Role client -TargetSpawner ld_spawner_guard_tutorial_1 -Evidence "$out/deselect-aim.json"
 $beforeDeselect=Q client '--domino-targets';$beforeDeselect|ConvertTo-Json -Depth 10|Set-Content "$out/targets-before-deselect.json"
 $removedHandle=[Convert]::ToUInt32($target2.handle.Substring(2),16);$keptHandle=[Convert]::ToUInt32($target.handle.Substring(2),16)
 $keptBefore=@($peerWorld.marks|Where-Object target -eq $keptHandle)
 if(!$beforeDeselect.valid-or$beforeDeselect.targets.Count-ne2-or$keptBefore.Count-ne1){throw 'Two native targets and retained peer mark required before deselection'}
 $finalMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-domino-scenes','10000') -WindowStyle Hidden -RedirectStandardOutput "$out/native-final-scenes.ndjson" -RedirectStandardError "$out/native-final-scenes.stderr" -PassThru
 $cancelMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-midnight','10000') -WindowStyle Hidden -RedirectStandardOutput "$out/native-deselect-scenes.ndjson" -RedirectStandardError "$out/native-deselect-scenes.stderr" -PassThru
 I client 255 1800;$stop=[Diagnostics.Stopwatch]::StartNew()
 $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native LT hold1800ms toward already-linked guard1 then release; native target deselection observed, no direct network/target mutation'})
 $deselectRows=[Collections.Generic.List[object]]::new();$stable=0
 do{
  $sourceNow=Q client '--domino-links';$targetsNow=Q client '--domino-targets';$peerNow=S host '.domino-links.json';$marksNow=S host '.domino-world.json'
  $movementNow=S host '.movement.json';$streamNow=S host '.shadow-stream.json'
  $deselectRows.Add([pscustomobject]@{ms=$stop.ElapsedMilliseconds;source=$sourceNow;targets=$targetsNow;peer=$peerNow;marks=$marksNow;movement=$movementNow;stream=$streamNow;sourceFx=(OptionalFx client);peerFx=(OptionalFx host)})
  $matched=$sourceNow.valid-and$targetsNow.valid-and$sourceNow.targets-eq1-and!$sourceNow.connections.Count-and$targetsNow.targets.Count-eq1-and$targetsNow.targets[0].handle-eq$keptHandle-and!$peerNow.beams.Count-and$marksNow.marks.Count-eq1-and$marksNow.marks[0].target-eq$keptHandle
  $stable=if($matched){$stable+1}else{0};if($stable-ge6){break};Start-Sleep -Milliseconds 100
 }while($stop.ElapsedMilliseconds-lt6500)
 I client
 $deselectRows|ConvertTo-Json -Depth 16|Set-Content "$out/native-deselect-diagnostics.json"
 if($stable-lt6){throw 'Native target deselection or its replicated world state did not settle'}
 if($marksNow.marks[0].id-ne$keptBefore[0].id-or$peerNow.stopped-ne($peerLinks.stopped+1)-or$peerNow.started-ne$peerLinks.started-or$peerNow.refused-ne$peerLinks.refused){throw 'Deselection recreated retained mark or did not stop exact beam'}
 if(@($deselectRows|Where-Object {$_.movement.weight-lt.95-or!$_.movement.active-or!$_.stream.ready-or!$_.stream.armed}).Count){throw 'Target deselection broke body/root stream continuity'}
 $checks.Add('Native target reselection removes only guard1, leaving guard2 marked; native Steam retires the exact beam and removed mark while preserving the surviving mark incarnation and full body/root stream')
 $finalPeers=@($deselectRows|Where-Object {$_.movement.powerVariant-eq21-and$_.movement.powerWeight-ge.25})
 if(!$finalPeers.Count){throw 'Native final-select full-body gesture did not reach peer'}
 if(@($deselectRows|Where-Object {$_.movement.clockRejection.count-ne0}).Count){throw 'Final selection rejected body source clock'}
 $finalPeers|ConvertTo-Json -Depth 16|Set-Content "$out/final-select-peer-proof.json"

 $stop.Restart();C host 'coop_movement_bank off';Start-Sleep -Milliseconds 700
 $afterStop=S host '.domino-links.json';$afterMarks=S host '.domino-world.json'
 if($afterStop.beams.Count-or$afterStop.stopped-ne($beamBefore.stopped+1)-or$afterMarks.marks.Count){throw 'Independent presentation stop did not retire remaining mark'}
 $fade=[Collections.Generic.List[object]]::new()
 do{$fx=Q host '--domino-effects';$status=S host '.domino-links.json';$fade.Add([pscustomobject]@{ms=$stop.ElapsedMilliseconds;effects=$fx;links=$status});if($status.beams.Count-or$status.started-ne$peerLinks.started-or$status.updated-ne$afterStop.updated){throw 'Retired beam resumed'};if(!$fx.effects.Count){break};Start-Sleep -Milliseconds 150}while($stop.ElapsedMilliseconds-lt5000)
 $fade|ConvertTo-Json -Depth 10|Set-Content "$out/beam-fade.json"
 if($fx.effects.Count){throw 'Native beam/marks survive bounded fade'}
 $sourceAfter=Q client '--domino-links';$sourceAfter|ConvertTo-Json -Depth 10|Set-Content "$out/links-after-peer-presentation-stop.json"
 if(!$sourceAfter.valid-or$sourceAfter.targets-ne1-or$sourceAfter.connections.Count-ne0){throw 'Peer presentation stop unexpectedly changed source native links'}
 $checks.Add('Independent peer presentation stop retires the remaining cosmetic mark within bounded native fade while the one native source target persists')
 C client 'coop_domino_fixture off';$w.Restart()
 do{$f=S client '.domino-fixture.json';if(!$f.active){break};Start-Sleep -Milliseconds 75}while($w.ElapsedMilliseconds-lt6500)
 if($f.active-or$f.result-ne'quantity_and_selection_restored'){throw 'Native Domino inventory did not retire'}
 foreach($r in 'host','client'){$after=Q $r '--player-inventory';$after|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-after-$r.json";if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($after.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Inventory quantities differ'};if((Q $r '--health' 1).current-ne56){throw 'Player health changed'}}
 $checks.Add('Both native inventories restore and player health stays unchanged; world effects require separate checkpoint cleanup')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_domino_fixture off'}catch{}}
 if($finalMonitor){
  $finalMonitor.WaitForExit(12000)|Out-Null
  try{
   if(!$finalMonitor.HasExited-or$finalMonitor.ExitCode){throw 'Native final phase observer failed'}
   $finalRows=@(Get-Content "$out/native-final-scenes.ndjson"|ForEach-Object {$_|ConvertFrom-Json}|Where-Object {$_.valid-and$_.subscene-eq'Cast_final_Select'})
   if($finalRows.Count-lt4-or@($finalRows|Where-Object {$_.variant-ne21-or$_.durationMs-ne400-or!$_.playerStatesValid-or!($_.activeStates-contains72)}).Count){throw 'Exact native final_Select state72/400ms not verified'}
   if(@($finalRows.armHandle|Sort-Object -Unique).Count-ne1-or@($finalRows.equipmentHandle|Sort-Object -Unique).Count-ne1){throw 'Final selection arm/equipment incarnation changed'}
   $finalRows|ConvertTo-Json -Depth 8|Set-Content "$out/final-select-native-proof.json"
   $checks.Add('Exact native400ms final_Select is observed with state72 and reciprocal owned arm/equipment; shared native selection clip maps to peer whole-body selection gesture and native idle recovery without root-clock rejection')
  }catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
 }
 if($cancelMonitor){$cancelMonitor.WaitForExit(10000)|Out-Null;if(!$cancelMonitor.HasExited-or$cancelMonitor.ExitCode){$failure=($failure+'; cancel watcher failed').TrimStart('; ')}}
 if($peerCapture){$peerCapture.WaitForExit(50000)|Out-Null;if(!$peerCapture.HasExited-or$peerCapture.ExitCode){$failure=($failure+'; peer capture failed').TrimStart('; ')}}
 if($capture){$capture.WaitForExit(50000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode){$failure=($failure+'; capture failed').TrimStart('; ')}}
 if($monitor){$monitor.WaitForExit(18000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){$failure=($failure+'; scene watcher failed').TrimStart('; ')}}
 if($strictMonitor){$strictMonitor.WaitForExit(18000)|Out-Null;if(!$strictMonitor.HasExited-or$strictMonitor.ExitCode){$failure=($failure+'; strict watcher failed').TrimStart('; ')}}
 if($linkMonitor){$linkMonitor.WaitForExit(18000)|Out-Null;if(!$linkMonitor.HasExited-or$linkMonitor.ExitCode){$failure=($failure+'; link watcher failed').TrimStart('; ')}}
 if($targetMonitor){$targetMonitor.WaitForExit(18000)|Out-Null;if(!$targetMonitor.HasExited-or$targetMonitor.ExitCode){$failure=($failure+'; target watcher failed').TrimStart('; ')}}
 $peerRows|ConvertTo-Json -Depth 14|Set-Content "$out/peer-states.json"
 $rows|ConvertTo-Json -Depth 20|Set-Content "$out/states.json"
 foreach($r in 'host','client'){try{C $r 'coop_movement_fixture off'}catch{}}
 if($dirty){try{& "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log";foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'}};$p=Q client '--player-domino';if($p.quantity-or$p.equipped-or$p.started){throw 'Domino remains after checkpoint'};foreach($r in 'host','client'){$fx=Q $r '--domino-effects';if(@($fx.effects).Count){throw 'Domino effects remain after checkpoint'}};$checks.Add('Both isolated checkpoints restore clean health/inventory and no filtered native Domino effects remain')}catch{$failure=($failure+'; checkpoint restore: '+$_.Exception.Message).TrimStart('; ')}}
 try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All fourteen protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
 [ordered]@{passed=(!$failure);failure=$failure;source='client';checks=$checks;events=$events;dll=$initialDll;probe=$initialProbe;scope='Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. Native target deselection is tested with LT on an already linked guard, requiring source and peer to retain only the other target and preserve its mark incarnation. The observed400ms Cast_final_Select maps to the existing full-body selection clip because native scene resources use the identical native hand clip/duration; exact state72, arm/equipment identity and paired body gesture are verified. Fire, slide and other unobserved phases remain unsupported. NPC alignment, AI, damage/outcomes, unobserved body phases and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required.'}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_NATIVE_DOMINO_OBSERVATION $($checks.Count) checks"
