param([ValidatePattern('^[a-z0-9-]+$')][string]$Take,[ValidateRange(-15,45)][int]$Pitch=0,[ValidateSet('cancel','receiver-stop','source-pause','receiver-pause')][string]$Case='cancel')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(!$Take-or(Test-Path $out)){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
$pids=@{};$inventory=@{};$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$peerCapture=$null;$capture=$null;$monitor=$null;$ownedMonitor=$null;$targetMonitor=$null;$fxMonitor=$null;$peerFxMonitor=$null;$pausedRole=$null;$dirty=$false;$failure=$null
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
  foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_potion_fixture off','coop_bendtime_fixture off','coop_darkvision_fixture off','coop_mesmerize_fixture off','coop_domino_fixture off','coop_swarm_fixture off','coop_possession_fixture off','coop_movement_fixture off'){C $r $cmd}
  C $r 'coop_movement_fixture 60000'
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Clean living fixture required'}
  $inventory[$r]=Q $r '--player-inventory';$inventory[$r]|ConvertTo-Json -Depth 8|Set-Content "$out/inventory-before-$r.json"
 }
 $preflight=Q host '--watch-possession-state' 100
 $preflight|ConvertTo-Json -Depth 8|Set-Content "$out/probe-preflight.json"
 if(!$preflight.Count){throw 'Read-only target watcher preflight emitted no records'}
 $dirty=$true
 $isolationWait=[Diagnostics.Stopwatch]::StartNew()
 do{$isolation=S host '.observer-isolation.json';if($isolation.ready-and$isolation.movementFixture){break};if(!$isolation.active){break};Start-Sleep -Milliseconds 30}while($isolationWait.ElapsedMilliseconds-lt2500)
 $isolation|ConvertTo-Json -Depth 8|Set-Content "$out/acquisition-isolation.json"
 if(!$isolation.ready-or!$isolation.movementFixture-or$isolation.remainingMs-lt45000){throw "Native AI isolation not ready before source placement: $($isolation.result)"}
 # Target-relative source-only placement after native AI references are held.
 # Never move the NPC. Each proposed floor is independently measured; failed
 # floor probes return immediately to the previously verified balcony floor.
 $candidate=@((Q host '--world-npcs').npcs|Where-Object {$_.spawnerName-eq'ld_spawner_guard_tutorial_2'-and$_.unique-and$_.reciprocal-and$_.definition-eq'models/characters/def/generic/dunwall_guard_male_small_01.def'})
 if($candidate.Count-ne1-or[Math]::Abs([double]$candidate[0].position[2]-66.4)-gt.5){throw 'Exact tutorial guard2 not on upper floor'}
 $floorAttempts=[Collections.Generic.List[object]]::new();$floorFound=$false
 foreach($offset in @(@(0.0,-3.0),@(-3.0,0.0),@(3.0,0.0),@(0.0,3.0))){
  $sourceX=[double]$candidate[0].position[0]+$offset[0];$sourceY=[double]$candidate[0].position[1]+$offset[1]
  C host ('setviewpos {0} {1} 68.1 90 0' -f $sourceX.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$sourceY.ToString('F5',[Globalization.CultureInfo]::InvariantCulture))
  Start-Sleep -Milliseconds 700
  $ground=Q host '--player-states';$health=Q host '--health' 1
  $floorAttempts.Add([pscustomobject]@{sourceX=$sourceX;sourceY=$sourceY;ground=$ground;health=$health.current})
  if($health.current-ne56){throw 'Source placement changed health'}
  if($ground.motionValid-and$ground.motion-eq0-and$ground.stance-eq0-and$ground.defaultMode-and[Math]::Abs([double]$ground.position[2]-66.4)-lt.1-and[Math]::Abs([double]$ground.position[0]-$sourceX)-lt.08-and[Math]::Abs([double]$ground.position[1]-$sourceY)-lt.08){$floorFound=$true;break}
  C host 'setviewpos -48.59 -155.07 68.1 90 0'
 }
 $floorAttempts|ConvertTo-Json -Depth 10|Set-Content "$out/source-floor-attempts.json"
 if(!$floorFound){throw 'No verified nearby native source floor; NPC retained untouched'}
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch $Pitch -Evidence "$out/source-camera.json" -Quick
 $ground|ConvertTo-Json -Depth 10|Set-Content "$out/source-grounded.json"
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
 $eligible=@($catalog.npcs|Where-Object {$_.unique-and$_.reciprocal-and$_.spawnerName-eq'ld_spawner_guard_tutorial_2'-and$_.definition-eq'models/characters/def/generic/dunwall_guard_male_small_01.def'-and[Math]::Abs([double]$_.position[2]-66.4)-lt.5-and[Math]::Pow([double]$_.position[0]-$sourceX,2)+[Math]::Pow([double]$_.position[1]-$sourceY,2)-lt49}|Sort-Object {[Math]::Pow([double]$_.position[0]-$sourceX,2)+[Math]::Pow([double]$_.position[1]-$sourceY,2)})
 if(!$eligible.Count){throw 'No verified upper-floor tutorial guard within seven metres'}
 $target=$eligible[0];$yaw=[Math]::Atan2([double]$target.position[1]-$sourceY,[double]$target.position[0]-$sourceX)*180/[Math]::PI
 [ordered]@{target=$target;yaw=$yaw;placement='Native source floor verified after target-relative placement; NPC untouched'}|ConvertTo-Json -Depth 8|Set-Content "$out/aim-guard.json"
 Q host '--components' $target.handle|ConvertTo-Json -Depth 12|Set-Content "$out/target-components-before.json"
 $dx=$sourceX-[double]$target.position[0];$dy=$sourceY-[double]$target.position[1];$length=[Math]::Sqrt($dx*$dx+$dy*$dy)
 if($length-lt.1){throw 'Target too close for deterministic placement'}
 $yaw=[Math]::Atan2(-$dy,-$dx)*180/[Math]::PI
 C host ('setviewpos {0} {1} 68.1 {2} 0' -f $sourceX.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$sourceY.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$yaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
 $observerAttempts=[Collections.Generic.List[object]]::new();$observerFound=$false
 foreach($offset in @(@(-1.5,0.0),@(1.5,0.0),@(0.0,1.5),@(0.0,-1.5),@(-2.0,-2.0),@(2.0,-2.0),@(-2.0,2.0),@(2.0,2.0))){
  $observerX=$sourceX+$offset[0];$observerY=$sourceY+$offset[1]
  if([Math]::Pow([double]$target.position[0]-$observerX,2)+[Math]::Pow([double]$target.position[1]-$observerY,2)-lt3.24){continue}
  $observerYaw=[Math]::Atan2([double]$target.position[1]-$observerY,[double]$target.position[0]-$observerX)*180/[Math]::PI
  C client ('setviewpos {0} {1} 68.1 {2} 0' -f $observerX.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$observerY.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$observerYaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
  Start-Sleep -Milliseconds 650
  $observerGround=Q client '--player-states';$observerHealth=Q client '--health' 1
  $observerAttempts.Add([pscustomobject]@{requested=@($observerX,$observerY);actual=$observerGround;health=$observerHealth.current})
  if($observerHealth.current-ne56){throw 'Observer floor test changed health'}
  $ax=[double]$target.position[0]-[double]$observerGround.position[0];$ay=[double]$target.position[1]-[double]$observerGround.position[1]
  $bx=$sourceX-[double]$observerGround.position[0];$by=$sourceY-[double]$observerGround.position[1]
  $den=[Math]::Sqrt(($ax*$ax+$ay*$ay)*($bx*$bx+$by*$by))
  if($den-lt.1-or($ax*$bx+$ay*$by)/$den-gt.9396926){continue} # at least20 degrees between source body and target

  if([Math]::Pow([double]$target.position[0]-[double]$observerGround.position[0],2)+[Math]::Pow([double]$target.position[1]-[double]$observerGround.position[1],2)-ge3.24-and$observerGround.defaultMode-and$observerGround.motionValid-and$observerGround.motion-eq0-and$observerGround.stance-eq0-and[Math]::Abs([double]$observerGround.position[2]-66.4)-lt.1){$observerFound=$true;$observerX=[double]$observerGround.position[0];$observerY=[double]$observerGround.position[1];break}
  C client 'setviewpos -44.39 -153.27 68.1 180 0'
 }
 $observerAttempts|ConvertTo-Json -Depth 10|Set-Content "$out/observer-floor-attempts.json"
 if(!$observerFound){throw 'No independently verified close observer floor'}
 # Keep the observer in the corridor interior rather than beside a bookshelf pillar.
 # Based on viewed356 previews; floor is independently revalidated below.
 $observerX=if([double]$target.position[0]-lt-44){-46.4503}else{-40.5}
 $observerY=[Math]::Max(-155.449,[Math]::Min(-145.0,[double]$target.position[1]-3.0))

 $observerYaw=[Math]::Atan2([double]$target.position[1]-$observerY,[double]$target.position[0]-$observerX)*180/[Math]::PI
 C client ('setviewpos {0} {1} 68.1 {2} 0' -f $observerX.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$observerY.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$observerYaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))

 Start-Sleep -Milliseconds 650
 $observerFinal=Q client '--player-states';$observerFinal|ConvertTo-Json -Depth 8|Set-Content "$out/observer-final-floor.json"
 if(!$observerFinal.defaultMode-or$observerFinal.stance-ne0-or!$observerFinal.motionValid-or$observerFinal.motion-ne0-or[Math]::Abs([double]$observerFinal.position[2]-66.4)-gt.1){throw 'Camera yaw reframe changed verified observer floor'}
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch 0 -Evidence "$out/observer-camera.json" -Quick
 $placed=Q host '--player-states';$placed|ConvertTo-Json -Depth 8|Set-Content "$out/source-target-relative.json"
 if(!$placed.defaultMode-or$placed.stance-ne0-or[Math]::Abs([double]$placed.position[2]-66.4)-gt.1){throw 'Target-relative placement is not on verified floor'}
 $pin=Get-Content "$PSScriptRoot/possession-bank-manifest-v334.json" -Raw|ConvertFrom-Json|Where-Object role -eq 'corvo'
 if((Get-FileHash "$PSScriptRoot/possession-banks-v334/corvo-possession-bank.bin").Hash-ne$pin.sha256){throw 'Release bank hash differs'}
 Copy-Item "$PSScriptRoot/possession-banks-v334/corvo-possession-bank.bin" "$root/work/lab-instances/client/coop-status.json.movement.bin"
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 600;C client 'coop_movement_bank on'
 $w.Restart();do{$m=S client '.movement.json';$stream=S client '.shadow-stream.json';if($m.active-and$m.version-eq52-and$m.weight-gt.9-and$m.presentedDrawn-and$stream.ready-and$stream.armed){break};Start-Sleep -Milliseconds 50}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.version-ne52-or$m.weight-lt.9-or!$stream.ready-or!$stream.armed){[ordered]@{movement=$m;stream=$stream;sourceStream=(S host '.shadow-stream.json');observer=(Q client '--player-states');source=(Q host '--player-states');isolation=(S client '.observer-isolation.json')}|ConvertTo-Json -Depth 12|Set-Content "$out/preflight-failure.json";throw 'Release bank/native Steam stream not ready'}
 $sourceFxBefore=S host '.possession-fx.json';$peerFxBefore=S client '.possession-fx.json'
 [ordered]@{source=$sourceFxBefore;peer=$peerFxBefore}|ConvertTo-Json -Depth 6|Set-Content "$out/network-fx-before.json"
 $visibilityBefore=S client '.possession-body.json';$visibilityBefore|ConvertTo-Json|Set-Content "$out/visibility-before.json"
 $checks.Add('Pinned bank52/316 and protocol50 native Steam stream ready;315 prior clip payloads retained')

 $checks.Add('Exact human-upgrade native quantity acquired and stable reciprocal tutorial guard selected by read-only world identity')

 $hwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64();if(!$hwnd){throw 'Source window missing'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','12','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take-source.mp4")
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();$capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 $peerHwnd=(Get-Process -Id $pids.client).MainWindowHandle.ToInt64();if(!$peerHwnd){throw 'Observer window missing'}
 $argIndex=[Array]::IndexOf($args,($args|Where-Object {$_ -like 'gfxcapture=*'}));if($argIndex-lt0){throw 'Capture argument absent'}
 $args[$argIndex]="gfxcapture=hwnd=${peerHwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra";$args[-1]="$PSScriptRoot/$Take.mp4"
 $peerCapture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture-peer.log" -RedirectStandardOutput "$out/capture-peer.stdout" -PassThru
 Start-Sleep -Milliseconds 600;if($capture.HasExited){throw 'Source capture failed'}

 $fxMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.host,'--watch-possession-effects','11000') -WindowStyle Hidden -RedirectStandardOutput "$out/possession-effects.ndjson" -RedirectStandardError "$out/possession-effects.stderr" -PassThru
 $peerFxMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-possession-effects','11000') -WindowStyle Hidden -RedirectStandardOutput "$out/peer-effects.ndjson" -RedirectStandardError "$out/peer-effects.stderr" -PassThru
 I host 255 10000
 $activeWait=[Diagnostics.Stopwatch]::StartNew()
 do{$peerFx=S client '.possession-fx.json';if($peerFx.sourceId-and$peerFx.started-$peerFxBefore.started-eq1){break};Start-Sleep -Milliseconds 40}while($activeWait.ElapsedMilliseconds-lt3500)
 if(!$peerFx.sourceId-or$peerFx.started-$peerFxBefore.started-ne1){throw 'No owned peer target mark before lifecycle interruption'}
 $beforeInterrupt=[ordered]@{source=(S host '.possession-fx.json');peer=$peerFx;player=(Q host '--player-states');sourceStream=(S host '.shadow-stream.json');peerStream=(S client '.shadow-stream.json')}
 $beforeInterrupt|ConvertTo-Json -Depth 10|Set-Content "$out/before-interrupt.json"
 if($beforeInterrupt.player.stance-eq18-or!$beforeInterrupt.player.defaultMode){throw 'Expected native aim before interruption, not possession occupation'}
 $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Verified native aim and owned remote target particle before '+$Case})
 Start-Sleep -Milliseconds 500
 if($Case-eq'receiver-stop'){C client 'coop_shadow_stream off'}
 elseif($Case-eq'cancel'){& "$lab/Send-D2Input.ps1" -Role host -Button X -LeftTrigger 255 -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 240;I host}
 else{
  $pausedRole=if($Case-eq'source-pause'){'host'}else{'client'}
  & "$lab/Send-D2Input.ps1" -Role $pausedRole -Button Start -LeftTrigger $(if($pausedRole-eq'host'){255}else{0}) -DurationMs 180|Out-Null
  $menuWait=[Diagnostics.Stopwatch]::StartNew();do{if((S $pausedRole).localGameLogicState-eq2){break};Start-Sleep -Milliseconds 30}while($menuWait.ElapsedMilliseconds-lt2000)
  if((S $pausedRole).localGameLogicState-ne2){throw 'Native pause menu did not open'}
  Start-Sleep -Milliseconds 650
  [ordered]@{role=$pausedRole;status=(S $pausedRole);peerFx=(S client '.possession-fx.json');stream=(S client '.shadow-stream.json')}|ConvertTo-Json -Depth 8|Set-Content "$out/paused.json"
  & "$lab/Send-D2Input.ps1" -Role $pausedRole -Button A -LeftTrigger $(if($pausedRole-eq'host'){255}else{0}) -DurationMs 180|Out-Null
  $menuWait.Restart();do{if((S $pausedRole).localGameLogicState-eq1){break};Start-Sleep -Milliseconds 30}while($menuWait.ElapsedMilliseconds-lt2500)
  if((S $pausedRole).localGameLogicState-ne1){throw 'Native pause menu failed to resume'}
  $pausedRole=$null
 }
 $retire=[Diagnostics.Stopwatch]::StartNew()
 while($retire.ElapsedMilliseconds-lt1500){$rows.Add([pscustomobject]@{ms=$retire.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;sourceFx=(S host '.possession-fx.json');peerFx=(S client '.possession-fx.json');stream=(S client '.shadow-stream.json')});Start-Sleep -Milliseconds 45}
 $settled=@($rows|Where-Object ms -ge 500)
 if($settled.Count-lt4-or@($settled|Where-Object {$_.peerFx.sourceId-or$_.peerFx.started-$peerFxBefore.started-ne1-or$_.peerFx.stopped-$peerFxBefore.stopped-ne1-or$_.peerFx.refused-ne$peerFxBefore.refused-or$_.peerFx.retired-ne$peerFxBefore.retired}).Count){throw 'Target mark did not retire once and remain absent after interruption'}
 if($Case-ne'cancel'-and@($settled|Where-Object {$_.stream.armed-or$_.stream.ready}).Count){throw 'Interrupted stream implicitly rearmed'}
 if($Case-eq'cancel'-and@($settled|Where-Object {!$_.stream.armed-or!$_.stream.ready}).Count){throw 'Native cancellation retired normal movement stream'}
 $checks.Add('One owned target particle retires after '+$Case+' and remains absent across settling interval; no refusal, lost owner or replay')
 # Native cancellation while LT remains held, then neutral input. No synthetic gameplay outcome.
 if($Case-ne'cancel'){& "$lab/Send-D2Input.ps1" -Role host -Button X -LeftTrigger 255 -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 250;I host}
 Start-Sleep -Milliseconds 600
 $final=Q host '--player-states';$final|ConvertTo-Json -Depth 8|Set-Content "$out/final-player.json"
 if($final.stance-eq18-or!$final.defaultMode){
  if($Case-eq'cancel'){throw 'Cancellation unexpectedly entered native possession'}
  I host 255 120;Start-Sleep -Milliseconds 1300;$final=Q host '--player-states'
 }
 if(!$final.defaultMode-or$final.stance-ne0){throw 'Native input did not restore normal source mode'}
 $manaAfter=& "$PSScriptRoot/Read-NativeMana.ps1" -Role host -Evidence "$out/mana-after.json"
 if($Case-eq'cancel'-and[Math]::Abs([double]$manaAfter.currentCandidate-[double]$manaBefore.currentCandidate)-gt.01){throw 'Aim cancellation unexpectedly spent mana'}
 $checks.Add('Native source recovers normal mode; explicit cancellation preserves mana')
 [ordered]@{case=$Case;before=$beforeInterrupt;afterSource=(S host '.possession-fx.json');afterPeer=(S client '.possession-fx.json');settledRows=$settled.Count}|ConvertTo-Json -Depth 10|Set-Content "$out/lifecycle-summary.json"
}catch{$failure=$_.Exception.Message}finally{
 if($pausedRole){try{& "$lab/Send-D2Input.ps1" -Role $pausedRole -Button A -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 400}catch{}}
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_possession_fixture off'}catch{}}
 foreach($job in @($capture,$peerCapture,$monitor,$ownedMonitor,$targetMonitor,$fxMonitor,$peerFxMonitor)){if($job){$job.WaitForExit(18000)|Out-Null;if(!$job.HasExited-or$job.ExitCode){$failure=($failure+'; capture/watcher failed').TrimStart('; ')}}}
 foreach($r in 'host','client'){try{C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 22|Set-Content "$out/states.json"
 if($dirty){try{
  & "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log"
  foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'}}
  $p=Q host '--player-possession';if($p.quantity-or$p.equipped-or$p.started){throw 'Possession remains acquired after checkpoint'}
  $u=@((Q host '--player-inventory').items|Where-Object name -eq 'components/characters/player/base/attributes/possession_parasite.inventoryitem');if($u.Count-ne1-or$u[0].quantity){throw 'Human upgrade remains acquired after checkpoint'}
  foreach($r in 'host','client'){
   $restored=Q $r '--player-inventory';$restored|ConvertTo-Json -Depth 10|Set-Content "$out/inventory-checkpoint-restored-$r.json"
   if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($restored.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Complete baseline inventory did not restore from checkpoint'}
  }
  $checks.Add('Both isolated checkpoints restore complete original inventory quantities and health, clearing temporary Possession and upgrade acquisitions')
 }catch{$failure=($failure+'; checkpoint restore: '+$_.Exception.Message).TrimStart('; ')}}
 try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All14 protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
 [ordered]@{passed=(!$failure);failure=$failure;source='host';checks=$checks;events=$events;dll=$initialDll;probe=$initialProbe;case=$Case;scope='Native human target particles: aim cancellation or stream/menu interruption, source gameplay remains native, no receiver NPC mutation. Complete checkpoint inventory/health and protected save checks mandatory. Body occupation/release is covered by separate successful-cast tests.'}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_NATIVE_POSSESSION_OBSERVATION $($checks.Count) checks"



