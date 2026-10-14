param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-interactions-corvo-crouch-coin-v688',[ValidateSet('host','client')][string]$Source='host',[ValidateSet('all','food','coin','note')][string]$OnlyCase='coin',[switch]$CrouchedSource)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$capture=$null;$peerCapture=$null;$stateMonitor=$null
$pids=@{};$inventory=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$peer=if($Source-eq'host'){'client'}else{'host'}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$b='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}
function S($r,$suffix=''){for($a=0;$a-lt8;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function Q($r,$mode,$arg=0){for($a=0;$a-lt6;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read failed $mode"}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path -LiteralPath $out){throw 'Unique evidence take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 [ordered]@{pid=$PID;operation='NativeInteractions688';take=$Take;state='running'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
 $dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;$pin=Get-Content "$root/work/movement-integration/candidate-readable-body-v681/identity.json" -Raw|ConvertFrom-Json;if($dll-ne$pin.dll){throw 'Expected681 candidate'}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid;I $r
  foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_movement_fixture off'){C $r $cmd}
  $inventory[$r]=Q $r '--player-inventory';$inventory[$r]|ConvertTo-Json -Depth 10|Set-Content "$out/inventory-before-$r.json"
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Living baseline required'}
 }
 $dirty=$true
 $cases=@(
  @{name='food';entity='pickups_fruit_grape_03_3';x=-34.975;y=-146.85;yaw=90;pitch=45.5},
  @{name='coin';entity='pickups_coin_emily_lowvalue_9';x=-50.6;y=-146.6;yaw=180;pitch=32},
  @{name='note';entity='pickups_note_01_shipcere';x=-30.9315;y=-147.0;yaw=90;pitch=47}
 )
 foreach($case in $cases){
  if($OnlyCase-ne'all'-and$case.name-ne$OnlyCase){continue}
  foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_shadow_stream off','coop_movement_fixture off'){C $r $cmd}}
  foreach($r in 'host','client'){C $r 'coop_movement_fixture 60000'}
  $isolationWait=[Diagnostics.Stopwatch]::StartNew();do{$isolated=$true;foreach($r in 'host','client'){$iso=S $r '.observer-isolation.json';if(!$iso.ready-or$iso.heldReferences-ne10){$isolated=$false}};if(!$isolated){Start-Sleep -Milliseconds 50}}while(!$isolated-and$isolationWait.ElapsedMilliseconds-lt3000)
  if(!$isolated){throw 'Both native NPC isolation holds required before approaching pickup'}
  $target=@((Q $Source '--entities').entities|Where-Object name -eq $case.entity);if($target.Count-ne1-or$target[0].type-ne'.?AVarkPickup@@'){throw "Exact pickup missing $($case.name)"}
  $target|ConvertTo-Json -Depth 7|Set-Content "$out/$($case.name)-target-before.json"
  $targetHandle=[Convert]::ToUInt32($target[0].handle.Replace('0x',''),16)
  (Q $Source '--components' $targetHandle)|ConvertTo-Json -Depth 9|Set-Content "$out/$($case.name)-components.json"
  C $Source "setviewpos $($case.x) $($case.y) 68.1 $($case.yaw) 0"
  C $peer "setviewpos $($case.x+3) $($case.y-1.2) 68.1 158.2 0"
  if($CrouchedSource){
   $initialPlayer=Q $Source '--player-states';if($initialPlayer.stance-ne0){throw 'Expected standing before crouch input'}
   I $Source B 200;Start-Sleep -Milliseconds 800
   $crouchedPlayer=Q $Source '--player-states';$crouchedPlayer|ConvertTo-Json -Depth 12|Set-Content "$out/$($case.name)-crouch-before.json"
   if($crouchedPlayer.stance-ne1-or!$crouchedPlayer.motionValid-or@($crouchedPlayer.active|Where-Object {$_.id-eq6-and$_.vtableRva-eq'0x1c84258'}).Count-ne1){throw 'Exact native Sneaking6 required'}
  }
  & "$PSScriptRoot/Set-NativeInteractionAim-v663.ps1" -Role $Source -X $target[0].position[0] -Y $target[0].position[1] -Z ($target[0].position[2]+.03) -Evidence "$out/$($case.name)-source-camera.json"
  & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $peer -Pitch 12 -Evidence "$out/$($case.name)-observer-camera.json"
  foreach($r in 'host','client'){
   C $r 'coop_probe_follow exact';C $r 'coop_probe_locomotion on';C $r 'coop_shadow_loco combat';$iso=S $r '.observer-isolation.json';if(!$iso.ready-or$iso.remainingMs-lt15000){throw 'Isolation lease insufficient after aiming'}
   $role=if($r-eq'client'){'corvo'}else{'emily'};$entry=Get-Content "$PSScriptRoot/heavy-bank-manifest-v633.json" -Raw|ConvertFrom-Json|Where-Object role -eq $role
   $bank=Join-Path $PSScriptRoot $entry.bank;if((Get-FileHash $bank).Hash-ne$entry.sha256){throw 'Bank hash differs'}
   Copy-Item $bank "$root/work/lab-instances/$r/coop-status.json.movement.bin" -Force;C $r 'coop_movement_bank on'
  }
  foreach($r in 'host','client'){C $r 'coop_shadow_stream on'}
  $ready=[Diagnostics.Stopwatch]::StartNew();do{$ok=$true;foreach($r in 'host','client'){$m=S $r '.movement.json';if(!$m.active-or$m.weight-lt.95-or!(S $r '.shadow-stream.json').ready){$ok=$false}};if(!$ok){Start-Sleep -Milliseconds 50}}while(!$ok-and$ready.ElapsedMilliseconds-lt8000)
  if(!$ok){[ordered]@{source=(S $Source);peer=(S $peer);observerAvatar=(Q $peer '--health' (S $peer).probeActorHandle);sourceIsolation=(S $Source '.observer-isolation.json');peerIsolation=(S $peer '.observer-isolation.json');sourceStream=(S $Source '.shadow-stream.json');stream=(S $peer '.shadow-stream.json');sourceMovement=(S $Source '.movement.json');movement=(S $peer '.movement.json');sourcePlayer=(Q $Source '--player-states');peerPlayer=(Q $peer '--player-states')}|ConvertTo-Json -Depth 15|Set-Content "$out/$($case.name)-ready-failure.json";throw 'Initial body/Steam not ready'};foreach($r in 'host','client'){C $r 'coop_movement_resume on'}
  $invBefore=Q $Source '--player-inventory'
  foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56){throw 'Enemy damage before capture invalidates this movement comparison'}}
  foreach($r in @($Source,$peer)){
   $view=if($r-eq$Source){'source'}else{'observer'};$hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64();if(!$hwnd){throw 'Game window absent'}
   $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','14','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$($case.name)-$view.mp4")
   $process=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$($case.name)-$view-capture.log" -RedirectStandardOutput "$out/$($case.name)-$view-capture.stdout" -PassThru
   if($r-eq$Source){$capture=$process}else{$peerCapture=$process}
  }
  $stateMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids[$Source],'--watch-player-states',14000) -WindowStyle Hidden -RedirectStandardOutput "$out/$($case.name)-state-transitions.json" -RedirectStandardError "$out/$($case.name)-state-transitions.log" -PassThru
  $samples=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$watch=[Diagnostics.Stopwatch]::StartNew();$sent=$false;$closed=$false;$movedAfterRead=$false
  while($watch.ElapsedMilliseconds-lt13200){
   if(!$sent-and$watch.ElapsedMilliseconds-ge1600){I $Source X 200;$events.Add(@{ms=$watch.ElapsedMilliseconds;input='X200'});$sent=$true}
   if($case.name-eq'note'-and!$closed-and$watch.ElapsedMilliseconds-ge5000){I $Source B 200;$events.Add(@{ms=$watch.ElapsedMilliseconds;input='B200 close native readable'});$closed=$true}
   if($case.name-eq'note'-and!$movedAfterRead-and$watch.ElapsedMilliseconds-ge11000){if(!(S $peer '.shadow-stream.json').ready-or!(S $peer '.movement.json').active){throw 'Native note recovery did not complete before movement'};& "$lab/Send-D2Input.ps1" -Role $Source -LeftY -20000 -DurationMs 300|Out-Null;$events.Add(@{ms=$watch.ElapsedMilliseconds;input='LeftY-20000 300ms after note close'});$movedAfterRead=$true}
   foreach($r in 'host','client'){if(!(S $r '.observer-isolation.json').ready){throw 'NPC isolation retired during recording'}}
   $status=S $Source;$native=$null
   if($status.localGameLogicState-eq1-and!$status.localPendingGameLogicState){try{$native=Q $Source '--player-states'}catch{}}
   $samples.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;player=$native;logic=$(if($case.name-eq'note'){Q $Source '--logic'}else{$null});source=$status;peer=(S $peer);observerAvatar=(Q $peer '--health' (S $peer).probeActorHandle);sourceIsolation=(S $Source '.observer-isolation.json');peerIsolation=(S $peer '.observer-isolation.json');sourceStream=(S $Source '.shadow-stream.json');stream=(S $peer '.shadow-stream.json');movement=(S $peer '.movement.json');resume=(S $peer '.movement-resume.json')})
   Start-Sleep -Milliseconds 40
  }
  foreach($process in @($capture,$peerCapture)){$process.WaitForExit(6000)|Out-Null;if(!$process.HasExited-or$process.ExitCode){throw 'Capture incomplete'}};$capture=$null;$peerCapture=$null
  $samples|ConvertTo-Json -Depth 15|Set-Content "$out/$($case.name)-observations.json";$events|ConvertTo-Json|Set-Content "$out/$($case.name)-inputs.json"
  if((S $Source).localGameLogicState-ne1){I $Source B 200;Start-Sleep -Milliseconds 700}
  $stateMonitor.WaitForExit(5000)|Out-Null;if(!$stateMonitor.HasExited-or$stateMonitor.ExitCode){throw 'Compact native state observation incomplete'};$stateMonitor=$null
  foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56){throw 'Enemy damage invalidates recorded comparison'}}
  if($case.name-eq'note'){if(!(S $peer '.shadow-stream.json').ready-or!(S $Source '.shadow-stream.json').ready-or!(S $peer '.movement.json').active){throw 'Note recovery incomplete at capture end'};if(@($samples|Where-Object {$_.observerAvatar.explicitHidden}).Count){throw 'Remote body disappeared during readable UI'};$checks.Add('Native readable closes, body stays visible and both streams recover before ordinary movement')}
  $invAfter=Q $Source '--player-inventory'
  [ordered]@{before=$invBefore;after=$invAfter}|ConvertTo-Json -Depth 11|Set-Content "$out/$($case.name)-inventory.json"
  ConvertTo-Json -InputObject @((Q $Source '--entities').entities|Where-Object name -eq $case.entity) -Depth 7|Set-Content "$out/$($case.name)-target-after.json"
  $rows.Add([pscustomobject]@{case=$case.name;target=$case.entity;samples=$samples.Count;sourceLogic=@($samples.source.localGameLogicState|Sort-Object -Unique);states=@($samples.player.active.id|Sort-Object -Unique)})
 }
 $checks.Add('Selected ordinary-input native interaction sequences and both actual game views recorded; activation and body continuity require independent audit')
}catch{$failure=$_.Exception.Message;if($samples-and(Test-Path -LiteralPath $out)){$samples|ConvertTo-Json -Depth 15|Set-Content "$out/partial-observations.json"}}finally{
 if($owned){
  try{if($pids[$Source]){$logic=Q $Source '--logic';if($logic.stack[-1].id-eq4){I $Source B 200;Start-Sleep -Milliseconds 900}}}catch{}
  foreach($r in 'host','client'){try{I $r;foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_shadow_stream off','coop_movement_fixture off'){C $r $cmd}}catch{}}
  foreach($process in @($capture,$peerCapture)){if($process){$process.WaitForExit(15000)|Out-Null;if(!$process.HasExited-or$process.ExitCode){$failure=($failure+'; capture incomplete').TrimStart('; ')}}}
  if($stateMonitor){$stateMonitor.WaitForExit(15000)|Out-Null;if(!$stateMonitor.HasExited-or$stateMonitor.ExitCode){$failure=($failure+'; native state monitor incomplete').TrimStart('; ')}}
  if($dirty){try{
   & "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log"
   foreach($r in 'host','client'){
    $restored=Q $r '--player-inventory';$restored|ConvertTo-Json -Depth 10|Set-Content "$out/inventory-restored-$r.json"
    if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($restored.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Checkpoint inventory differs'}
    if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'}
   };$checks.Add('Both complete inventories and player/partner health restored from isolated checkpoint')
  }catch{$failure=($failure+'; restore: '+$_.Exception.Message).TrimStart('; ')}}
  try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All14 protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
  if(Test-Path -LiteralPath $out){[ordered]@{passed=(!$failure);failure=$failure;dll=$dll;source=$Source;checks=$checks;cases=$rows;scope='Native food/coin/note source interaction with existing full-body and Steam transport. No automatic acceptance of target activation, a new gesture, shared world outcome or complete movement.'}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"}
  [ordered]@{pid=$PID;operation='NativeInteractions688';take=$Take;state='finished'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json";$mutex.ReleaseMutex()
 };$mutex.Dispose()
}
if($failure){throw $failure};"PASS_NATIVE_INTERACTION_OBSERVATION $($checks.Count) checks"







