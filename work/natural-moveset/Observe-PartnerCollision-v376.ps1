param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='partner-collision-audit-v376')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$checks=[Collections.Generic.List[string]]::new();$cases=[Collections.Generic.List[object]]::new();$pids=@{};$inventory=@{};$captures=[Collections.Generic.List[object]]::new()
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$y=0,$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -LeftY $y -DurationMs $ms|Out-Null}
function S($r){for($i=0;$i-lt8;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read failed: $mode"}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation still running'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 [ordered]@{pid=$PID;operation='PartnerCollisionObserve';take=$Take;started=(Get-Date).ToString('o');state='running'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
 $dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;$probe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;I $r
  $inventory[$r]=Q $r '--player-inventory';$inventory[$r]|ConvertTo-Json -Depth 10|Set-Content "$out/inventory-before-$r.json"
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Clean living fixture required'}
  foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_possession_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}
 }
 $dirty=$true
 foreach($mover in 'host','client'){
  $other=if($mover-eq'host'){'client'}else{'host'}
  foreach($condition in 'clear','partner'){
   I host;I client
   C $mover 'setviewpos -33.5 -153 68.1 90 0'
   C $other $(if($condition-eq'partner'){'setviewpos -33.5 -150 68.1 270 0'}else{'setviewpos -39.4 -161.4 68.1 270 0'})
   Start-Sleep -Milliseconds 650
   $before=Q $mover '--player-states';$otherBefore=Q $other '--player-states';$bridge=S $mover
   if(!$before.motionValid-or$before.stance-ne0-or[math]::Abs([double]$before.position[2]-66.4)-gt.1){throw 'Mover not grounded/standing in measured corridor'}
   if($condition-eq'partner'-and([math]::Abs([double]$bridge.probeRenderPosition[0]+33.5)-gt.15-or[math]::Abs([double]$bridge.probeRenderPosition[1]+150)-gt.15)){throw 'Partner not in test corridor'}
   $hwnd=(Get-Process -Id $pids[$mover]).MainWindowHandle.ToInt64();if(!$hwnd){throw 'Missing capture window'}
   $video="$Take-$mover-$condition.mp4"
   $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','4','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$video")
   $cap=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$mover-$condition-capture.log" -RedirectStandardOutput "$out/$mover-$condition-capture.stdout" -PassThru;$captures.Add($cap)
   Start-Sleep -Milliseconds 650;if($cap.HasExited){throw 'Capture failed'}
   $rows=[Collections.Generic.List[object]]::new();$watch=[Diagnostics.Stopwatch]::StartNew();I $mover 32767 1800
   while($watch.ElapsedMilliseconds-lt2300){$a=S $mover;$b=S $other;$rows.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;position=$a.localPosition;remoteAvatar=$a.probeRenderPosition;otherPosition=$b.localPosition;sourceAvatar=$b.probeRenderPosition});Start-Sleep -Milliseconds 25}
   I $mover;Start-Sleep -Milliseconds 150;$after=Q $mover '--player-states';$otherAfter=Q $other '--player-states'
   $case=[ordered]@{mover=$mover;condition=$condition;video=$video;input='LeftY32767 for1800ms then neutral';before=$before;after=$after;otherBefore=$otherBefore;otherAfter=$otherAfter;samples=$rows}
   $case|ConvertTo-Json -Depth 14|Set-Content "$out/$mover-$condition.json";$cases.Add($case)
   $cap.WaitForExit(6000)|Out-Null;if(!$cap.HasExited-or$cap.ExitCode){throw 'Capture incomplete'}
   foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Player/partner health changed'}}
  }
 }
 $checks.Add('Both role directions recorded with identical corridor/input and clear versus partner-present control')
 $checks.Add('Player and partner health stable after every movement case; collision verdict requires trajectory and video analysis')
}catch{$failure=$_.Exception.Message}finally{
 if($owned-and(Test-Path $out)){
  foreach($r in 'host','client'){if($pids[$r]){try{I $r;C $r 'coop_movement_fixture off'}catch{}}}
  foreach($cap in $captures){$cap.WaitForExit(8000)|Out-Null;if(!$cap.HasExited-or$cap.ExitCode){$failure=($failure+'; capture incomplete').TrimStart('; ')}}
  if($dirty){try{
   & "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log"
   foreach($r in 'host','client'){
    if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Restored health differs'}
    $restored=Q $r '--player-inventory';$restored|ConvertTo-Json -Depth 10|Set-Content "$out/inventory-restored-$r.json"
    if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($restored.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Restored inventory differs'}
   };$checks.Add('Both checkpoints restore original complete inventory and health')
  }catch{$failure=($failure+'; restoration: '+$_.Exception.Message).TrimStart('; ')}}
  try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('All14 protected save/settings hashes retained')}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
  [ordered]@{passed=(!$failure);failure=$failure;checks=$checks;dll=$dll;probe=$probe;cases=@($cases|ForEach-Object {"$($_.mover)-$($_.condition)"});collisionExclusionVerified=$false;scope='Read-only movement/physics observation with native controller input. Clear versus partner-present corridor on both roles. No collision filter or physics mutation. Passing means collection/restoration succeeded; trajectory/visual verdict is separate.'}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
  [ordered]@{pid=$PID;operation='PartnerCollisionObserve';take=$Take;ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
 }
 if($owned){$mutex.ReleaseMutex()};$mutex.Dispose()
}
if($failure){throw $failure};"PARTNER_COLLISION_OBSERVATION $($checks.Count) checks"
