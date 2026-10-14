param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-door-traversal-corvo-v722',[ValidateSet('host','client')][string]$Source='host',[ValidateSet('finish','breakout')][string]$Outcome='breakout')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$capture=$null;$monitor=$null;$peerCapture=$null
$pids=@{};$inventory=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new()
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$b='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}
function DS($r){Get-Content "$root/work/lab-instances/$r/coop-status.json.door-sync.json" -Raw|ConvertFrom-Json}
function S($r){for($a=0;$a-lt8;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function Q($r,$mode,$arg=0){for($a=0;$a-lt8;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read failed $mode"}
function W($o,$offset){$w=@($o.words|Where-Object offset -eq $offset);if($w.Count-ne1){throw "Missing object field $offset"};[Convert]::ToUInt64($w[0].word.Substring(2),16)}
function DoorSample($r){
 $d=$doorBindings[$r];$o=Q $r '--object' $d.component
 if($o.type-ne'.?AVarkCpntOpenable@@'-or(W $o '0x10')-ne$d.owner-or((W $o '0x18')-band [uint64]4294967295)-ne$d.generation){$o|ConvertTo-Json -Depth 7|Set-Content "$out/failed-door-$r.json";throw 'Door component lifetime changed'}
 $state=W $o '0x48';$flags=W $o '0x1e8'
 [pscustomobject]@{handle=$d.handle;current=[uint32]($state-band [uint64]4294967295);pending=[uint32]($state-shr 32);locked=[int]($flags-band 255);flags1e8=('0x{0:x}'-f$flags);state90=('0x{0:x}'-f(W $o '0x90'));movingTarget=('0x{0:x}'-f(W $o '0xb0'));child=[uint32]((W $o '0xd8')-shr 32)}
}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Another native operation remains active'}
 if(Test-Path -LiteralPath $out){throw 'Unique evidence take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 [ordered]@{pid=$PID;operation='DoorTraversal722';take=$Take;started=(Get-Date).ToString('o');state='running'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
 $dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash
 $pin=Get-Content "$root/work/movement-integration/candidate-door-sync-v716/identity.json" -Raw|ConvertFrom-Json
 if($dll-ne$pin.dll){throw 'Expected archived716 candidate'}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid;I $r
  foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_probe_follow exact','coop_probe_locomotion on','coop_shadow_loco combat','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Living fixture required'}
  $inventory[$r]=Q $r '--player-inventory';$inventory[$r]|ConvertTo-Json -Depth 10|Set-Content "$out/inventory-before-$r.json"
 }
 $dirty=$true
 foreach($source in @($Source)){
  $peer=if($source-eq'host'){'client'}else{'host'}
  foreach($case in @(@{name='unlocked-door-tap';yaw=270;stance=0;distance=1.0})){
   I $source;I $peer
   $state=Q $source '--player-states';if($state.stance-ne$case.stance){I $source B 180;Start-Sleep -Milliseconds 500}
   C $peer 'setviewpos -47.05 -163.0 68.1 270 0'
   C $source 'setviewpos -44 -161 68.1 220 0'
   & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $source -Pitch 0 -Evidence "$out/source-camera.json" -Quick
   & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $peer -Pitch 0 -Evidence "$out/observer-camera.json" -Quick
   $components=Q $source '--components' 1
   $components|ConvertTo-Json -Depth 8|Set-Content "$out/player-components-before.json"
   $sensors=@($components.components|Where-Object {$_.type-match'(?i)(PlayerSensor.*(Interaction|Use)|PlayerFocus)'})
   foreach($r in 'host','client'){@((Q $r '--entities').entities|Where-Object {$_.type-match'Door'})|ConvertTo-Json -Depth 8|Set-Content "$out/doors-before-$r.json"}
   $doorBindings=@{}
   foreach($r in 'host','client'){
    $door=@((Q $r '--entities').entities|Where-Object name -eq 'ld_door_350cm_int_rich_wood_01_3')
    if($door.Count-ne1-or$door[0].type-ne'.?AVarkDoor@@'){throw 'Exact ordinary door missing'};$door=$door[0]
    $cp=@((Q $r '--components' $door.handle).components|Where-Object type -eq '.?AVarkCpntOpenable@@')
    if($cp.Count-ne1-or$cp[0].owner-ne$door.pointer){throw 'Door component owner mismatch'};$cp=$cp[0]
    $obj=Q $r '--object' $cp.pointer;$obj|ConvertTo-Json -Depth 7|Set-Content "$out/openable-before-$r.json"
    $doorBindings[$r]=[pscustomobject]@{handle=$door.handle;component=$cp.pointer;owner=[Convert]::ToUInt64($door.pointer.Substring(2),16);generation=((W $obj '0x18')-band [uint64]4294967295)}
    if((DoorSample $r).locked-ne0){throw 'Unlocked door required'}
   }
   $doorBindings|ConvertTo-Json -Depth 5|Set-Content "$out/door-bindings.json"
   foreach($bankTarget in 'host','client'){
    $character=if($bankTarget-eq'host'){'emily'}else{'corvo'}
    $entry=Get-Content "$PSScriptRoot/heavy-bank-manifest-v633.json" -Raw|ConvertFrom-Json|Where-Object role -eq $character
    $bank=Join-Path $PSScriptRoot $entry.bank;if((Get-FileHash $bank).Hash-ne$entry.sha256){throw 'Bank hash differs'}
    Copy-Item $bank "$root/work/lab-instances/$bankTarget/coop-status.json.movement.bin" -Force;C $bankTarget 'coop_movement_bank on'
    $bw=[Diagnostics.Stopwatch]::StartNew();do{$m=Get-Content "$root/work/lab-instances/$bankTarget/coop-status.json.movement.json" -Raw|ConvertFrom-Json;if($m.active-and$m.version-eq67){break};Start-Sleep -Milliseconds 40}while($bw.ElapsedMilliseconds-lt5000)
    if(!$m.active-or$m.version-ne67){throw 'Movement bank preload failed'}
   }
   foreach($r in 'host','client'){C $r 'coop_shadow_stream on'}
   foreach($r in 'host','client'){C $r 'coop_door_sync on'}
   $dw=[Diagnostics.Stopwatch]::StartNew();do{$dh=DS host;$dc=DS client;if($dh.ready-and$dc.ready-and$dh.revision-eq1-and$dc.revision-eq1-and$dh.applied-eq1-and$dc.applied-eq1){break};Start-Sleep -Milliseconds 40}while($dw.ElapsedMilliseconds-lt6000)
   if(!$dh.ready-or!$dc.ready-or$dh.revision-ne1-or$dc.revision-ne1){throw 'Door sync pair not armed'}
   $key="$source-$($case.name)";$hwnd=(Get-Process -Id $pids[$source]).MainWindowHandle.ToInt64();if(!$hwnd){throw 'Source window missing'}
   $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','20','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$key.mp4")
   $capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$key-capture.log" -RedirectStandardOutput "$out/$key-capture.stdout" -PassThru
   $peerHwnd=(Get-Process -Id $pids[$peer]).MainWindowHandle.ToInt64();$peerArgs=$args.Clone();$graphIndex=[Array]::FindIndex($peerArgs,[Predicate[object]]{param($x) "$x" -like "gfxcapture=*"});if($graphIndex-lt0){throw "Capture graph absent"};$peerArgs[$graphIndex]="gfxcapture=hwnd=${peerHwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra";$peerArgs[$peerArgs.Length-1]="$out/observer.mp4"
   $peerCapture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $peerArgs -WindowStyle Hidden -RedirectStandardError "$out/observer-capture.log" -RedirectStandardOutput "$out/observer-capture.stdout" -PassThru
   $monitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids[$source],'--watch-midnight','19500') -WindowStyle Hidden -RedirectStandardOutput "$out/scenes.ndjson" -RedirectStandardError "$out/scenes.stderr" -PassThru
   $filterBefore=Get-Content "$root/work/lab-instances/$source/coop-status.json.assassination-partner-filter.json" -Raw|ConvertFrom-Json;if(!$filterBefore.installed){throw "Takedown filter missing"}
   $watch=[Diagnostics.Stopwatch]::StartNew();$samples=[Collections.Generic.List[object]]::new();$inputs=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$collision=[ordered]@{};$eventIndex=0
   $schedule=@(
    @{at=1000;name='closed_walk';run={$collision.closedBefore=(S $peer).localPosition;& "$lab/Send-D2Input.ps1" -Role $peer -LeftY 32767 -DurationMs 700|Out-Null}},
    @{at=2500;name='closed_stop';run={I $peer;$collision.closedAfter=(S $peer).localPosition}},
    @{at=3500;name='prepare_source_open';run={C $peer 'setviewpos -44 -161 68.1 220 0';C $source 'setviewpos -47.05 -163.5 68.1 270 0'}},
    @{at=4200;name='native_open';run={I $source X 200;$inputs.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;input='native open';attempt=1})}},
    @{at=6500;name='clear_source';run={C $source 'setviewpos -44 -161 68.1 220 0'}},
    @{at=7100;name='prepare_open_walk';run={C $peer 'setviewpos -47.05 -163.0 68.1 270 0'}},
    @{at=7900;name='open_walk';run={if((DoorSample $peer).current-ne2){throw 'Peer door is not natively open before traversal'};$collision.openBefore=(S $peer).localPosition;& "$lab/Send-D2Input.ps1" -Role $peer -LeftY 32767 -DurationMs 1100|Out-Null}},
    @{at=9500;name='open_stop';run={I $peer;$collision.openAfter=(S $peer).localPosition}},
    @{at=10600;name='prepare_source_close';run={C $peer 'setviewpos -44 -161 68.1 220 0';C $source 'setviewpos -47.05 -163.5 68.1 270 0'}},
    @{at=11500;name='native_close';run={I $source X 200;$inputs.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;input='native close';attempt=2})}},
    @{at=14000;name='clear_source_again';run={C $source 'setviewpos -44 -161 68.1 220 0'}},
    @{at=14600;name='prepare_reclosed_walk';run={C $peer 'setviewpos -47.05 -163.0 68.1 270 0'}},
    @{at=15400;name='reclosed_walk';run={if((DoorSample $peer).current-ne0){throw 'Peer door is not closed before collision retest'};$collision.reclosedBefore=(S $peer).localPosition;& "$lab/Send-D2Input.ps1" -Role $peer -LeftY 32767 -DurationMs 700|Out-Null}},
    @{at=17100;name='reclosed_stop';run={I $peer;$collision.reclosedAfter=(S $peer).localPosition}}
   )
   while($watch.ElapsedMilliseconds-lt19000){
    if($eventIndex-lt$schedule.Count-and$watch.ElapsedMilliseconds-ge$schedule[$eventIndex].at){$event=$schedule[$eventIndex];& $event.run;$events.Add([pscustomobject]@{name=$event.name;ms=$watch.ElapsedMilliseconds});$eventIndex++}
    $apiPath="$root/work/lab-instances/$source/coop-status.json.door-fixture.json";$api=if(Test-Path $apiPath){Get-Content $apiPath -Raw|ConvertFrom-Json}else{$null}
    $samples.Add([pscustomobject]@{phase=$eventIndex;syncSource=(DS $source);syncPeer=(DS $peer);api=$api;ms=$watch.ElapsedMilliseconds;doorSource=(DoorSample $source);doorPeer=(DoorSample $peer);player=(Q $source '--player-states');source=(S $source);peer=(S $peer);sword=(Q $source '--player-sword');movement=(Get-Content "$root/work/lab-instances/$peer/coop-status.json.movement.json" -Raw|ConvertFrom-Json);stream=(Get-Content "$root/work/lab-instances/$peer/coop-status.json.shadow-stream.json" -Raw|ConvertFrom-Json)})
    Start-Sleep -Milliseconds 30
   }
   $monitor.WaitForExit(6000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){throw "Native scene monitor incomplete"};$monitor=$null
   $capture.WaitForExit(6000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode){throw 'Native capture failed'};$capture=$null
   $samples|ConvertTo-Json -Depth 17|Set-Content "$out/$key-observations.json"
   $filterAfter=Get-Content "$root/work/lab-instances/$source/coop-status.json.assassination-partner-filter.json" -Raw|ConvertFrom-Json
   [ordered]@{before=$filterBefore;after=$filterAfter}|ConvertTo-Json -Depth 6|Set-Content "$out/filter.json"
   if(!$filterAfter.installed){throw 'Partner eligibility filter not installed'}
   $allScenes=@(Get-Content "$out/scenes.ndjson"|ForEach-Object {$_|ConvertFrom-Json}|ForEach-Object {$_.instances})
   $allScenes|ConvertTo-Json -Depth 9|Set-Content "$out/native-scenes.json"
   $inputs|ConvertTo-Json|Set-Content "$out/inputs.json"
   $samples|ForEach-Object { [pscustomobject]@{ms=$_.ms;stance=$_.player.stance;mode=$_.player.mode;active=$_.player.active} }|ConvertTo-Json -Depth 9|Set-Content "$out/native-action-states.json"
   foreach($r in 'host','client'){@((Q $r '--entities').entities|Where-Object {$_.type-match'Door'})|ConvertTo-Json -Depth 8|Set-Content "$out/doors-after-$r.json"}
   if($inputs.Count-ne2-or$samples.Count-lt40){throw 'Door observation input/sampling incomplete'}
   $ds=DS $source;$dp=DS $peer
   [ordered]@{source=$ds;peer=$dp;baselineSource=$samples[0].syncSource;baselinePeer=$samples[0].syncPeer}|ConvertTo-Json -Depth 6|Set-Content "$out/native-sync-final.json"
   if(!$ds.enabled-or!$dp.enabled-or$ds.nativeCalls-ne0-or$dp.nativeCalls-ne2-or$ds.revision-ne3-or$dp.revision-ne3-or$ds.applied-ne3-or$dp.applied-ne3-or$ds.rejected-ne$samples[0].syncSource.rejected-or$dp.rejected-ne$samples[0].syncPeer.rejected){throw 'Native Steam door ordering, application, duplicate handling or rejection delta differs'}
   $collision|ConvertTo-Json -Depth 5|Set-Content "$out/collision-positions.json";$events|ConvertTo-Json -Depth 5|Set-Content "$out/input-events.json"
   if($eventIndex-ne$schedule.Count){throw 'Traversal schedule incomplete'}
   foreach($phase in 'closed','reclosed'){if($collision[$phase+'After'][1]-lt-164.4-or$collision[$phase+'After'][1]-gt$collision[$phase+'Before'][1]-.1){throw "Expected forward movement stopped in front of $phase door"}}
   if($collision.openAfter[1]-ge-165.0-or[math]::Abs($collision.openAfter[0]-$collision.openBefore[0])-gt.35-or[math]::Abs($collision.openAfter[2]-$collision.openBefore[2])-gt.4){throw 'Open-door traversal did not cross the doorway on the same floor'}
   $checks.Add('Native forward input is blocked by the closed and reclosed peer door and crosses the remotely opened doorway on the same floor')
   $states=@();foreach($x in $samples){$v=$x.doorSource.current;if(!$states.Count-or$states[-1]-ne$v){$states+=$v}}
   if(($states-join ',')-ne'0,1,2,1,0'){throw "Native API door cycle differs: $($states-join ',')"}
   $peerStates=@();foreach($x in $samples){$v=$x.doorPeer.current;if(!$peerStates.Count-or$peerStates[-1]-ne$v){$peerStates+=$v}}
   if(($peerStates-join ',')-ne'0,1,2,1,0'){throw "Peer native door cycle differs: $($peerStates-join ',')"}
   if($Source-eq'client'-and($ds.requests-ne2-or$dp.requests-ne2)){throw 'Expected two client requests ordered by host'}
   $checks.Add('Two ordinary native X interactions replicate both0->1->2->1->0 door cycles; exactly two remote native calls; no rejected packets during measured actions; both finish host revision3')
   C $peer 'coop_movement_bank off';foreach($r in 'host','client'){C $r 'coop_shadow_stream off'}
   $rows.Add([pscustomobject]@{source=$source;case=$case;key=$key;count=$samples.Count;sensors=@($sensors.type);partnerHandle=(S $source).probeActorHandle})
   foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Health changed during neutral targeting observation'}}
  }
 }
 $checks.Add('Bounded native Steam ordinary-door synchronization with host desired-state ordering; both viewpoints recorded')
}catch{$failure=$_.Exception.Message;if($samples){$samples|ConvertTo-Json -Depth 17|Set-Content "$out/partial-observations.json"};if($inputs){$inputs|ConvertTo-Json|Set-Content "$out/partial-inputs.json"};if($collision){$collision|ConvertTo-Json -Depth 5|Set-Content "$out/partial-collision.json"};if($events){$events|ConvertTo-Json|Set-Content "$out/partial-events.json"}}finally{
 if($owned){
  foreach($r in 'host','client'){try{I $r;C $r 'coop_door_sync off';C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_movement_fixture off'}catch{}}
  if($peerCapture){$peerCapture.WaitForExit(35000)|Out-Null;if(!$peerCapture.HasExited-or$peerCapture.ExitCode){$failure=($failure+'; observer capture incomplete').TrimStart('; ')}}
  if($monitor){$monitor.WaitForExit(15000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){$failure=($failure+'; scene monitor incomplete').TrimStart('; ')}}
  if($capture){$capture.WaitForExit(10000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode){$failure=($failure+'; capture incomplete').TrimStart('; ')}}
  if($dirty){try{
   & "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log"
   foreach($r in 'host','client'){
    $restored=Q $r '--player-inventory';$restored|ConvertTo-Json -Depth 10|Set-Content "$out/inventory-checkpoint-restored-$r.json"
    if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($restored.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Complete checkpoint inventory differs'}
    if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'}
   };$checks.Add('Both checkpoint inventories and player/partner health restored')
  }catch{$failure=($failure+'; restore: '+$_.Exception.Message).TrimStart('; ')}}
  try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All14 protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
  if(Test-Path -LiteralPath $out){[ordered]@{passed=(!$failure);failure=$failure;dll=$dll;checks=$checks;cases=$rows;scope='Native Steam door traversal: receiver native forward movement is blocked before/after and crosses while remotely opened. Source uses real native X inputs; receiver uses74a400 queue. Both engine state cycles and host revision/duplicate counters verified; locks/scripts/collision/campaign-wide doors remain unverified.'}|ConvertTo-Json -Depth 10|Set-Content "$out/result.json"}
  [ordered]@{pid=$PID;operation='DoorTraversal722';take=$Take;ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
if($failure){throw $failure};"PASS_NATIVE_DOOR_OBSERVATION $($checks.Count) checks"


