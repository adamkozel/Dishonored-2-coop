param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-guard-choke-v574',[ValidateSet('host','client')][string]$Source='host')
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
 [ordered]@{pid=$PID;operation='ChokeNetwork589';take=$Take;started=(Get-Date).ToString('o');state='running'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
 $dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash
 $pin=Get-Content "$root/work/movement-integration/candidate-choke-network-v584/identity.json" -Raw|ConvertFrom-Json
 if($dll-ne$pin.dll){throw 'Expected archived584 candidate'}
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
  foreach($case in @(@{name='rear-choke';yaw=90;stance=0;distance=1.15})){
   I $source;I $peer
   $state=Q $source '--player-states';if($state.stance-ne$case.stance){I $source B 180;Start-Sleep -Milliseconds 500}
   $guard=@((Q $source '--world-npcs').npcs|Where-Object {$_.spawnerName-eq'ld_spawner_guard_tutorial_2'-and$_.unique-and$_.reciprocal-and$_.definition-eq'models/characters/def/generic/dunwall_guard_male_small_01.def'})
   if($guard.Count-ne1-or[Math]::Abs([double]$guard[0].position[2]-66.4)-gt.1){throw 'Exact upper-floor guard2 absent'}
   $target=$guard[0];$forwardX=[double]$target.orientation[0];$forwardY=[double]$target.orientation[1]
   if([Math]::Abs($forwardX*$forwardX+$forwardY*$forwardY-1)-gt.01){throw 'Guard forward not horizontal'}
   $x=[double]$target.position[0]-1.15*$forwardX;$y=[double]$target.position[1]-1.15*$forwardY;$yaw=[Math]::Atan2($forwardY,$forwardX)*180/[Math]::PI
   $target|ConvertTo-Json -Depth 8|Set-Content "$out/guard-before.json"
   C $peer 'setviewpos -28.5 -158 68.1 90 0'
   C $source ('setviewpos {0} {1} 68.1 {2} 0' -f $x.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$y.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$yaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
   Start-Sleep -Milliseconds 800
   & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $source -Pitch 0 -Evidence "$out/$source-$($case.name)-camera.json" -Quick
   $components=Q $source '--components' 1
   $sensors=@($components.components|Where-Object {$_.type-match'(?i)(PlayerSensor.*(Assassination|Interaction|Use|Grab)|PlayerFocus)'})
   if(!@($sensors|Where-Object type -eq '.?AVarkCpntPlayerSensorGlobalAssassination@@').Count){throw 'Global assassination sensor not found'}
   $character=if($source-eq'host'){'corvo'}else{'emily'}
   $entry=Get-Content "$PSScriptRoot/choke-bank-manifest-v581.json" -Raw|ConvertFrom-Json|Where-Object role -eq $character
   $bank=Join-Path $PSScriptRoot $entry.bank;if((Get-FileHash $bank).Hash-ne$entry.sha256){throw 'Bank hash differs'}
   Copy-Item $bank "$root/work/lab-instances/$peer/coop-status.json.movement.bin" -Force;C $peer 'coop_movement_bank on'
   $bw=[Diagnostics.Stopwatch]::StartNew();do{$m=Get-Content "$root/work/lab-instances/$peer/coop-status.json.movement.json" -Raw|ConvertFrom-Json;if($m.active-and$m.version-eq63){break};Start-Sleep -Milliseconds 40}while($bw.ElapsedMilliseconds-lt5000)
   if(!$m.active-or$m.version-ne63){throw 'Choke bank preload failed'}
   foreach($r in 'host','client'){C $r 'coop_shadow_stream on'}
   $key="$source-$($case.name)";$hwnd=(Get-Process -Id $pids[$source]).MainWindowHandle.ToInt64();if(!$hwnd){throw 'Source window missing'}
   $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','18','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$key.mp4")
   $capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$key-capture.log" -RedirectStandardOutput "$out/$key-capture.stdout" -PassThru
   $peerHwnd=(Get-Process -Id $pids[$peer]).MainWindowHandle.ToInt64();$peerArgs=$args.Clone();$graphIndex=[Array]::FindIndex($peerArgs,[Predicate[object]]{param($x) "$x" -like "gfxcapture=*"});if($graphIndex-lt0){throw "Capture graph absent"};$peerArgs[$graphIndex]="gfxcapture=hwnd=${peerHwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra";$peerArgs[$peerArgs.Length-1]="$out/observer.mp4"
   $peerCapture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $peerArgs -WindowStyle Hidden -RedirectStandardError "$out/observer-capture.log" -RedirectStandardOutput "$out/observer-capture.stdout" -PassThru
   $monitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids[$source],'--watch-midnight','17000') -WindowStyle Hidden -RedirectStandardOutput "$out/scenes.ndjson" -RedirectStandardError "$out/scenes.stderr" -PassThru
   $filterBefore=Get-Content "$root/work/lab-instances/$source/coop-status.json.assassination-partner-filter.json" -Raw|ConvertFrom-Json;if(!$filterBefore.installed){throw "Takedown filter missing"}
   $watch=[Diagnostics.Stopwatch]::StartNew();$samples=[Collections.Generic.List[object]]::new();$sent=$false
   while($watch.ElapsedMilliseconds-lt16000){
    if(!$sent-and$watch.ElapsedMilliseconds-ge1100){
     # The client guard can still finish native walking animation while AI
     # decisions are held. Refresh the source fixture position immediately
     # before ordinary input; never move or force the guard itself.
     $fresh=@((Q $source '--world-npcs').npcs|Where-Object {$_.spawnerName-eq'ld_spawner_guard_tutorial_2'-and$_.unique-and$_.reciprocal-and$_.definition-eq$target.definition})
     if($fresh.Count-ne1){throw 'Guard identity changed'};$g=$fresh[0];$fx=[double]$g.orientation[0];$fy=[double]$g.orientation[1]
     $px=[double]$g.position[0]-.9*$fx;$py=[double]$g.position[1]-.9*$fy;$look=[Math]::Atan2($fy,$fx)*180/[Math]::PI
     C $source ('setviewpos {0} {1} 68.1 {2} 0' -f $px.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$py.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$look.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
     $cameraTrials=[Collections.Generic.List[object]]::new();$cameraOk=$false
     foreach($side in @(2,-2,0)){
      $ox=$px-2*$fx-$side*$fy;$oy=$py-2*$fy+$side*$fx;$oyaw=[Math]::Atan2($py-$oy,$px-$ox)*180/[Math]::PI
      C $peer ('setviewpos {0} {1} 68.1 {2} 0' -f $ox.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$oy.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$oyaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
      Start-Sleep -Milliseconds 600;$feet=(Q $peer '--player-states').position
      $cameraTrials.Add([pscustomobject]@{side=$side;x=$ox;y=$oy;actual=$feet})
      if([math]::Abs($feet[2]-$g.position[2])-lt.3-and[math]::Abs($feet[0]-$ox)-lt.6-and[math]::Abs($feet[1]-$oy)-lt.6){$cameraOk=$true;$ox=[double]$feet[0];$oy=[double]$feet[1];break}
     }
     $cameraTrials|ConvertTo-Json -Depth 5|Set-Content "$out/observer-floor-trials.json"
     if(!$cameraOk){throw 'No same-floor observer position found'}
     & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $peer -Pitch 12 -Evidence "$out/observer-camera.json" -Quick
     # Camera setup itself takes time. Refresh once more after it completes,
     # so a walking client-side guard cannot leave the source's reach again.
     $fresh=@((Q $source '--world-npcs').npcs|Where-Object {$_.spawnerName-eq'ld_spawner_guard_tutorial_2'-and$_.unique-and$_.reciprocal-and$_.definition-eq$target.definition})
     if($fresh.Count-ne1){throw 'Guard identity changed before input'};$g=$fresh[0];$fx=[double]$g.orientation[0];$fy=[double]$g.orientation[1]
     $px=[double]$g.position[0]-.85*$fx;$py=[double]$g.position[1]-.85*$fy;$look=[Math]::Atan2($fy,$fx)*180/[Math]::PI
     C $source ('setviewpos {0} {1} 68.1 {2} 0' -f $px.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$py.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$look.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
     $g|ConvertTo-Json -Depth 8|Set-Content "$out/guard-before-input.json"
     $oyaw=[Math]::Atan2($py-$oy,$px-$ox)*180/[Math]::PI
     C $peer ('setviewpos {0} {1} 68.1 {2} 0' -f $ox.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$oy.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$oyaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
     Start-Sleep -Milliseconds 90;I $source RightShoulder 6500;$sent=$true
    }
    $objects=@(foreach($c in $sensors){[pscustomobject]@{type=$c.type;pointer=$c.pointer;object=(Q $source '--object' $c.pointer)}})
    $samples.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;player=(Q $source '--player-states');source=(S $source);peer=(S $peer);sensors=$objects;midnight=(Q $source '--midnight');movement=(Get-Content "$root/work/lab-instances/$peer/coop-status.json.movement.json" -Raw|ConvertFrom-Json);stream=(Get-Content "$root/work/lab-instances/$peer/coop-status.json.shadow-stream.json" -Raw|ConvertFrom-Json);sourceStream=(Get-Content "$root/work/lab-instances/$source/coop-status.json.shadow-stream.json" -Raw|ConvertFrom-Json)})
    Start-Sleep -Milliseconds 80
   }
   $monitor.WaitForExit(6000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){throw "Native scene monitor incomplete"};$monitor=$null
   $capture.WaitForExit(6000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode){throw 'Native capture failed'};$capture=$null
   $samples|ConvertTo-Json -Depth 17|Set-Content "$out/$key-observations.json"
   $filterAfter=Get-Content "$root/work/lab-instances/$source/coop-status.json.assassination-partner-filter.json" -Raw|ConvertFrom-Json
   [ordered]@{before=$filterBefore;after=$filterAfter}|ConvertTo-Json -Depth 6|Set-Content "$out/filter.json"
   if(!$filterAfter.installed-or$filterAfter.otherAccepted-le$filterBefore.otherAccepted){throw 'Ordinary native target eligibility not accepted'}
   if(@($samples.player.active|Where-Object {$_.id-eq41}).Count-lt3){throw 'Ordinary guard choke state41 not demonstrated'}
   $allScenes=@(Get-Content "$out/scenes.ndjson"|ForEach-Object {$_|ConvertFrom-Json}|ForEach-Object {$_.instances});$paired=@($allScenes|Where-Object {$_.state-eq3-and$_.asset-match'(?i)(/choke/|assassin)'})
   if(@($paired|Where-Object asset -like '*choke_back*').Count-lt1-or@($paired|Where-Object asset -like '*choke_loop*').Count-lt2){throw 'Ordinary native guard choke scenes not demonstrated'}
   $paired|ConvertTo-Json -Depth 8|Set-Content "$out/choke-scenes.json"
   foreach($v in 1,2,3){
    if(!@($samples|Where-Object {$_.player.chokeSceneValid-and$_.player.chokeScene.variant-eq$v}).Count){throw "Owned native choke phase$v absent"};$checks.Add("Owned native choke phase$v read independently")
    if(!@($samples|Where-Object {$_.movement.chokeVariant-eq$v-and$_.movement.chokeWeight-gt.8-and$_.movement.weight-gt.85}).Count){throw "Remote choke phase$v absent"};$checks.Add("Steam remote whole-body choke phase$v observed")
   }
   if(!@($samples|Where-Object {$_.ms-gt9000-and$_.movement.chokeWeight-lt.05-and$_.movement.weight-gt.85}).Count){throw 'Standing recovery absent'};$checks.Add('Native standing recovery restores existing locomotion')
   $aligned=@($samples|Where-Object {$_.movement.chokeWeight-gt.8-and$_.peer.probeFollowEnabled-and[math]::Abs($_.peer.probeRenderPosition[0]-$_.player.position[0])-lt.3-and[math]::Abs($_.peer.probeRenderPosition[1]-$_.player.position[1])-lt.3})
   if($aligned.Count-lt3){throw 'Remote body root did not follow native source'};$checks.Add('Remote actor follows independent native source root during choke')
   C $peer 'coop_movement_bank off';foreach($r in 'host','client'){C $r 'coop_shadow_stream off'}
   $checks.Add('Ordinary guard native eligibility accepted and normal-input choke reaches state41/back/loop; no direct NPC mutation or force-state call')

   $rows.Add([pscustomobject]@{source=$source;case=$case;key=$key;count=$samples.Count;sensors=@($sensors.type);partnerHandle=(S $source).probeActorHandle})
   foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Health changed during neutral targeting observation'}}
  }
 }
 $checks.Add('Native RightShoulder held6500ms behind exact authored guard2; paired scene owner verification and visual review remain required.')
}catch{$failure=$_.Exception.Message}finally{
 if($owned){
  foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_movement_fixture off'}catch{}}
  if($peerCapture){$peerCapture.WaitForExit(15000)|Out-Null;if(!$peerCapture.HasExited-or$peerCapture.ExitCode){$failure=($failure+'; observer capture incomplete').TrimStart('; ')}}
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
  if(Test-Path -LiteralPath $out){[ordered]@{passed=(!$failure);failure=$failure;dll=$dll;checks=$checks;cases=$rows;scope='Native rear choke phase and target-binding observation using existing ordinary guard fixture; standard controller input only. Captures source scenes, native root/stance/state and actual recording. Source-timed remote actor body through native Steam. Remote victim animation/alignment and shared knockout outcomes are not implemented by this binding. Restore checkpoint inventory/health and14 protected files.'}|ConvertTo-Json -Depth 10|Set-Content "$out/result.json"}
  [ordered]@{pid=$PID;operation='ChokeNetwork589';take=$Take;ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
if($failure){throw $failure};"PASS_CHOKE_NETWORK $($checks.Count) checks"

