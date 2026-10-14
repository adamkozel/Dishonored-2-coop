param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-gait-batch-v643')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$capture=$null;$monitor=$null;$peerCapture=$null
$pids=@{};$inventory=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new()
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$b='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}
function S($r){for($a=0;$a-lt8;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function Q($r,$mode,$arg=0){for($a=0;$a-lt8;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read failed $mode"}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Another native operation remains active'}
 if(Test-Path -LiteralPath $out){throw 'Unique evidence take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 [ordered]@{pid=$PID;operation='GaitBatch643';take=$Take;started=(Get-Date).ToString('o');state='running'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
 $dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash
 $pin=Get-Content "$root/work/movement-integration/candidate-heavy-v635/identity.json" -Raw|ConvertFrom-Json
 if($dll-ne$pin.dll){throw 'Expected archived635 candidate'}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid;I $r
  foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_probe_follow exact','coop_probe_locomotion on','coop_shadow_loco combat','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Living fixture required'}
  $inventory[$r]=Q $r '--player-inventory';$inventory[$r]|ConvertTo-Json -Depth 10|Set-Content "$out/inventory-before-$r.json"
 }
 $dirty=$true
 foreach($source in @('host','client')){
  $peer=if($source-eq'host'){'client'}else{'host'}
  foreach($case in @(@{name='gait-batch';yaw=0;stance=0;distance=0.0})){
   I $source;I $peer
   $state=Q $source '--player-states';if($state.stance-ne$case.stance){I $source B 180;Start-Sleep -Milliseconds 500}
   foreach($r in 'host','client'){C $r 'coop_shadow_stream off';C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000'}
   C $peer 'setviewpos -28.5 -151.2 68.1 210 0'
   C $source 'setviewpos -34.3 -154.8 68.1 0 0'
   & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $source -Pitch 0 -Evidence "$out/$source-camera.json" -Quick
   & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $peer -Pitch 11 -Evidence "$out/$source-observer-camera.json" -Quick
   $character=if($source-eq'host'){'corvo'}else{'emily'}
   $entry=Get-Content "$PSScriptRoot/heavy-bank-manifest-v633.json" -Raw|ConvertFrom-Json|Where-Object role -eq $character
   $bank=Join-Path $PSScriptRoot $entry.bank;if((Get-FileHash $bank).Hash-ne$entry.sha256){throw 'Bank hash differs'}
   Copy-Item $bank "$root/work/lab-instances/$peer/coop-status.json.movement.bin" -Force;C $peer 'coop_movement_bank on'
   $bw=[Diagnostics.Stopwatch]::StartNew();do{$m=Get-Content "$root/work/lab-instances/$peer/coop-status.json.movement.json" -Raw|ConvertFrom-Json;if($m.active-and$m.version-eq67){break};Start-Sleep -Milliseconds 40}while($bw.ElapsedMilliseconds-lt5000)
   if(!$m.active-or$m.version-ne67){throw 'Choke bank preload failed'}
   foreach($r in 'host','client'){C $r 'coop_shadow_stream on'}
   $ready=[Diagnostics.Stopwatch]::StartNew();do{
    $m=Get-Content "$root/work/lab-instances/$peer/coop-status.json.movement.json" -Raw|ConvertFrom-Json
    $t=Get-Content "$root/work/lab-instances/$peer/coop-status.json.shadow-stream.json" -Raw|ConvertFrom-Json
    if($m.active-and$m.weight-gt.95-and$t.ready){break};Start-Sleep -Milliseconds 40
   }while($ready.ElapsedMilliseconds-lt6000)
   if(!$m.active-or$m.weight-le.95-or!$t.ready){throw 'Native Steam/body not ready'}
   $key="$source-$($case.name)";$hwnd=(Get-Process -Id $pids[$source]).MainWindowHandle.ToInt64();if(!$hwnd){throw 'Source window missing'}
   $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','30','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$key.mp4")
   $capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$key-capture.log" -RedirectStandardOutput "$out/$key-capture.stdout" -PassThru
   $peerHwnd=(Get-Process -Id $pids[$peer]).MainWindowHandle.ToInt64();$peerArgs=$args.Clone();$graphIndex=[Array]::FindIndex($peerArgs,[Predicate[object]]{param($x) "$x" -like "gfxcapture=*"});if($graphIndex-lt0){throw "Capture graph absent"};$peerArgs[$graphIndex]="gfxcapture=hwnd=${peerHwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra";$peerArgs[$peerArgs.Length-1]="$out/$source-observer.mp4"
   $peerCapture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $peerArgs -WindowStyle Hidden -RedirectStandardError "$out/$source-observer-capture.log" -RedirectStandardOutput "$out/$source-observer-capture.stdout" -PassThru
   $filterBefore=Get-Content "$root/work/lab-instances/$source/coop-status.json.assassination-partner-filter.json" -Raw|ConvertFrom-Json;if(!$filterBefore.installed){throw "Takedown filter missing"}
   # Timed ordinary controller inputs; every pulse expires without further input.
   # Same known clear-room course as the established gait sequence, with a moving turn and native holster/redraw.
   $events=@(
    @(0,0,0,0,100,'Neutral','armed-idle'),
    @(1200,0,14000,0,900,'Neutral','walk-forward'),
    @(2200,0,32767,0,650,'Neutral','accelerate-run'),
    @(2950,0,0,0,100,'Neutral','run-stop'),
    @(4200,0,14000,0,750,'Neutral','walk-before-reverse'),
    @(4950,0,-14000,0,850,'Neutral','reverse-back'),
    @(6000,0,0,0,100,'Neutral','walk-stop'),
    @(7200,-14000,14000,0,700,'Neutral','diagonal-left'),
    @(7900,14000,-14000,0,700,'Neutral','diagonal-reverse'),
    @(8800,0,0,0,100,'Neutral','diagonal-stop'),
    @(10000,0,14000,24000,800,'Neutral','moving-turn-right'),
    @(10900,0,-14000,-24000,800,'Neutral','moving-turn-back'),
    @(11900,0,0,0,100,'Neutral','turn-stop'),
    @(13500,0,0,0,1500,'X','native-holster'),
    @(17000,0,0,0,100,'Neutral','unarmed-idle'),
    @(18500,0,14000,0,650,'Neutral','unarmed-forward'),
    @(19300,0,-14000,0,650,'Neutral','unarmed-back'),
    @(20300,0,0,0,100,'Neutral','unarmed-stop'),
    @(21800,0,0,0,650,'X','native-redraw'),
    @(25000,0,0,0,100,'Neutral','armed-recovery'))
   $events|ConvertTo-Json -Depth 4|Set-Content "$out/$source-planned-inputs.json"
   $watch=[Diagnostics.Stopwatch]::StartNew();$samples=[Collections.Generic.List[object]]::new();$inputs=[Collections.Generic.List[object]]::new();$next=0
   while($watch.ElapsedMilliseconds-lt28000){
    if($next-lt$events.Count-and$watch.ElapsedMilliseconds-ge$events[$next][0]){
     $e=$events[$next];& "$lab/Send-D2Input.ps1" -Role $source -LeftX $e[1] -LeftY $e[2] -RightX $e[3] -DurationMs $e[4] -Button $e[5]|Out-Null
     $inputs.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;planned=$e[0];label=$e[6];x=$e[1];y=$e[2];turn=$e[3];duration=$e[4];button=$e[5]});$next++
    }
    $samples.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;label=$events[[Math]::Max(0,$next-1)][6];player=(Q $source '--player-states');nativeView=(Q $source '--player');source=(S $source);peer=(S $peer);sword=(Q $source '--player-sword');avatarSword=(Q $peer '--avatar-sword' (S $peer).probeActorHandle);movement=(Get-Content "$root/work/lab-instances/$peer/coop-status.json.movement.json" -Raw|ConvertFrom-Json);stream=(Get-Content "$root/work/lab-instances/$peer/coop-status.json.shadow-stream.json" -Raw|ConvertFrom-Json)})
    Start-Sleep -Milliseconds 30
   }
   $capture.WaitForExit(6000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode){throw 'Native capture failed'};$capture=$null
   $samples|ConvertTo-Json -Depth 17|Set-Content "$out/$key-observations.json"
   $filterAfter=Get-Content "$root/work/lab-instances/$source/coop-status.json.assassination-partner-filter.json" -Raw|ConvertFrom-Json
   [ordered]@{before=$filterBefore;after=$filterAfter}|ConvertTo-Json -Depth 6|Set-Content "$out/filter.json"
   if(!$filterAfter.installed){throw 'Partner eligibility filter not installed'}
   $inputs|ConvertTo-Json -Depth 4|Set-Content "$out/$source-inputs.json"
   if($inputs.Count-ne$events.Count-or$samples.Count-lt100){throw 'Batched input/sampling incomplete'}
   if(($samples.movement.runMix|Measure-Object -Maximum).Maximum-lt.5){throw 'Native run blend not exercised'}
   $unarmed=@($samples|Where-Object {$_.ms-gt17500-and$_.ms-lt18200})
   if($unarmed.Count-lt2-or@($unarmed|Where-Object {$_.sword.equipped-or$_.avatarSword.equipped-or$_.movement.armedMix-gt.05}).Count){throw 'Native holster did not reach both source and remote'}
   $recovery=@($samples|Where-Object {$_.ms-gt26500})
   if($recovery.Count-lt2-or@($recovery|Where-Object {!$_.sword.equipped-or!$_.avatarSword.equipped-or$_.movement.armedMix-lt.95-or$_.movement.speed-gt.05}).Count){throw 'Armed stationary recovery incomplete'}
   $checks.Add("${source}: walk/run, unarmed native idle and equipped stationary recovery present; detailed direction/root checks follow independent audit")
   $peerCapture.WaitForExit(6000)|Out-Null;if(!$peerCapture.HasExited-or$peerCapture.ExitCode){throw 'Observer capture failed'};$peerCapture=$null
   C $peer 'coop_movement_bank off';foreach($r in 'host','client'){C $r 'coop_shadow_stream off'}
   $rows.Add([pscustomobject]@{source=$source;case=$case;key=$key;count=$samples.Count;partnerHandle=(S $source).probeActorHandle})
   foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Health changed during neutral targeting observation'}}
  }
 }
 $checks.Add('Both roles recorded in one isolated native operation with one checkpoint restoration; existing movement clips and native first person retained.')
}catch{$failure=$_.Exception.Message;if($samples){$samples|ConvertTo-Json -Depth 17|Set-Content "$out/partial-observations.json"};if($inputs){$inputs|ConvertTo-Json|Set-Content "$out/partial-inputs.json"}}finally{
 if($owned){
  foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_movement_fixture off'}catch{}}
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
  if(Test-Path -LiteralPath $out){[ordered]@{passed=(!$failure);failure=$failure;dll=$dll;checks=$checks;cases=$rows;scope='Batched native locomotion transitions on both roles using candidate635 and existing bank67: walk/run/stop, reverse, diagonal direction changes, moving turns, holstered idle/movement and armed recovery. Both native views and Steam/body telemetry retained. Detailed direction/root/phase checks and visual inspection recorded separately; no new animation authoring or final visual acceptance.'}|ConvertTo-Json -Depth 10|Set-Content "$out/result.json"}
  [ordered]@{pid=$PID;operation='GaitBatch643';take=$Take;ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
if($failure){throw $failure};"PASS_NATIVE_GAIT_BATCH $($checks.Count) checks"


