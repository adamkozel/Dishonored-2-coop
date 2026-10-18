param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='player-death-probe-v1361a',[string]$Role='host',[int]$WatchMs=60000)
# 1361 (#11, death_variants): what does a player's death play natively? The receiver presents no player death; the game has
# a Dead state, NpcFatality (an NPC finishing the player: npc_fatality_guards/elite/clockwork) and KillCam (confirmed static,
# not-applicable-audit-v1240 state census). The source faces tutorial guard 2 while he stands at his patrol pause, attacks
# him once (he turns hostile) and then stands still, health not held, until native GameOver (logic 13) or $WatchMs; the
# player scenes, native states and logic are polled every ~100 ms and both windows are filmed.
# 1361d: the observer camera aims 3/4 of the way from the guard to the source, pitched 18 deg down, so the partner's
# death collapse (1214cv) stays in frame; 1361b filmed it just off the right edge. The lab is then restored
# (PrepareShadowSword reloads the checkpoint; it has recovered shared deaths since 782) with the note baseline.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$inv=[Globalization.CultureInfo]::InvariantCulture
$peer=if($Role-eq'host'){'client'}else{'host'};$pids=@{}
foreach($r in 'host','client'){$pids[$r]=(Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json).pid}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function Q($r,$mode,$arg=0){for($a=0;$a-lt4;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 30};$null}
function S($r){try{Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json}catch{$null}}
function F($v){([double]$v).ToString('F4',$inv)}
$trace=[Collections.Generic.List[object]]::new();$caps=@()
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Another native operation remains active'}
 if(Test-Path -LiteralPath $out){throw 'Unique evidence take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($r in 'host','client'){try{C $r 'coop_player_health_hold off'}catch{};& "$lab/Send-D2Input.ps1" -Role $r -DurationMs 100|Out-Null;C $r 'coop_movement_fixture off';C $r 'coop_product_auto on'}
 $aw=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$ok=(Get-Content "$root/work/lab-instances/host/coop-status.json.product.json" -Raw|ConvertFrom-Json).result-eq'armed'-and(Get-Content "$root/work/lab-instances/client/coop-status.json.product.json" -Raw|ConvertFrom-Json).result-eq'armed'}while(!$ok-and$aw.ElapsedMilliseconds-lt60000);if(!$ok){throw 'Product controller not armed'};Start-Sleep -Seconds 3
 $w=[Diagnostics.Stopwatch]::StartNew();$g=$null
 do{$a=@((Q $Role '--world-npcs').npcs|Where-Object {$_.spawnerName-eq'ld_spawner_guard_tutorial_2'});if($a.Count-and[Math]::Sqrt([Math]::Pow([double]$a[0].position[0]+47.95,2)+[Math]::Pow([double]$a[0].position[1]+152.45,2))-lt.4){Start-Sleep -Milliseconds 600;$b=@((Q $Role '--world-npcs').npcs|Where-Object {$_.spawnerName-eq'ld_spawner_guard_tutorial_2'});if($b.Count-and[Math]::Sqrt([Math]::Pow([double]$b[0].position[0]-[double]$a[0].position[0],2)+[Math]::Pow([double]$b[0].position[1]-[double]$a[0].position[1],2))-lt.03){$g=$b[0]}}else{Start-Sleep -Milliseconds 500}}while(!$g-and$w.ElapsedMilliseconds-lt150000)
 if(!$g){throw 'Guard never paused'};$gx=[double]$g.position[0];$gy=[double]$g.position[1];$fx=[double]$g.orientation[0];$fy=[double]$g.orientation[1];$fl=[Math]::Sqrt($fx*$fx+$fy*$fy);$fx/=$fl;$fy/=$fl
 $sx=$gx+1.2*$fx;$sy=$gy+1.2*$fy;C $Role ('setviewpos {0} {1} 68.1 {2} 0' -f (F $sx),(F $sy),(F ([Math]::Atan2($gy-$sy,$gx-$sx)*180/[Math]::PI)))
 $cx=$gx+4*$fx+2*$fy;$cy=$gy+4*$fy-2*$fx;$ax=.25*$gx+.75*$sx;$ay=.25*$gy+.75*$sy;$cyaw=[Math]::Atan2($ay-$cy,$ax-$cx)*180/[Math]::PI;$op=(Q $peer '--player').candidatePosition
 C $peer ('setviewpos {0} {1} {2} {3} 0' -f (F $op[0]),(F $op[1]),(F ([double]$op[2]+1.7)),(F $cyaw));Start-Sleep -Milliseconds 300;try{& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $peer -Pitch 18 -Quick|Out-Null}catch{}
 C $peer ('coop_observer_camera hold_at {0} {1} {2}' -f (F $cx),(F $cy),(F 68.4));Start-Sleep -Milliseconds 300
 foreach($pair in @(@($Role,'source'),@($peer,'observer'))){$hwnd=(Get-Process -Id $pids[$pair[0]]).MainWindowHandle.ToInt64()
  $caps+=Start-Process (Get-Command ffmpeg).Source -ArgumentList @('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',[string]([Math]::Ceiling($WatchMs/1000)+4),'-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$($pair[1]).mp4") -WindowStyle Hidden -RedirectStandardError "$out/$($pair[1])-capture.log" -RedirectStandardOutput "$out/$($pair[1])-capture.stdout" -PassThru}
 Start-Sleep -Milliseconds 800;& "$lab/Send-D2Input.ps1" -Role $Role -RightTrigger 255 -DurationMs 200|Out-Null
 # 1361f: guard hits knock the source back toward the camera (1361d filmed Corvo walking out of frame), so after each
 # health drop the observer camera is re-aimed at the source from 3.5 m along the line toward the original open spot.
 function Frame{$sp=(Q $Role '--player').candidatePosition;if(!$sp){return};$vx=$cx-[double]$sp[0];$vy=$cy-[double]$sp[1];$d=[Math]::Sqrt($vx*$vx+$vy*$vy);if($d-lt.5){$vx=$fx;$vy=$fy;$d=1}
  $nx=[double]$sp[0]+$vx/$d*3.5;$ny=[double]$sp[1]+$vy/$d*3.5;$yaw=[Math]::Atan2([double]$sp[1]-$ny,[double]$sp[0]-$nx)*180/[Math]::PI;$o=(Q $peer '--player').candidatePosition
  C $peer 'coop_observer_camera off';C $peer ('setviewpos {0} {1} {2} {3} 0' -f (F $o[0]),(F $o[1]),(F ([double]$o[2]+1.7)),(F $yaw));Start-Sleep -Milliseconds 150
  try{& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $peer -Pitch 20 -Quick|Out-Null}catch{};C $peer ('coop_observer_camera hold_at {0} {1} {2}' -f (F $nx),(F $ny),(F 68.6));$script:frames++}
 $frames=0;$lastHp=$null
 $ws=[Diagnostics.Stopwatch]::StartNew()
 while($ws.ElapsedMilliseconds-lt$WatchMs){$t=$ws.ElapsedMilliseconds;$ss=S $Role;$ps=S $peer;$m=Q $Role '--midnight';$st=Q $Role '--player-states';$h=Q $Role '--health' 1;$mv=try{Get-Content "$root/work/lab-instances/$peer/coop-status.json.movement.json" -Raw|ConvertFrom-Json}catch{$null}
  $trace.Add([pscustomobject]@{ms=$t;logic=$ss.localGameLogicState;peerLogic=$ps.localGameLogicState;health=$h.current;scenes=@($m.instances|Where-Object {$_.asset-like'*gameplay/player/*'}|ForEach-Object {"$(($_.asset-split'/')[-1])|$($_.subscene)|$($_.sceneDurationMs)"});active=@($st.active|ForEach-Object {"$($_.id):$(($_.type-replace'.?AVarkPlayerState','')-replace'@@','')"});partnerWeight=$mv.weight;partnerActive=$mv.active;partnerRender=$ps.probeRenderPosition;sourcePosition=$ss.localPosition;partnerCombat=$mv.combatVariant;partnerCombatWeight=$mv.combatWeight;partnerElig=$mv.eligibility.mask;sourceDeath=$(try{(Get-Content "$root/work/lab-instances/$Role/coop-status.json.shadow-stream.json" -Raw|ConvertFrom-Json).death}catch{$null})})
  if($null-ne$h-and$null-ne$lastHp-and[double]$h.current-lt$lastHp-and[double]$h.current-gt0){Frame};if($null-ne$h){$lastHp=[double]$h.current}
  if($ss.localGameLogicState-eq13-and$ps.localGameLogicState-eq13){Start-Sleep -Milliseconds 3000;break};Start-Sleep -Milliseconds 80}
}finally{
 foreach($c in $caps){try{if(!$c.WaitForExit(90000)){Stop-Process -Id $c.Id}}catch{}}
 try{C $peer 'coop_observer_camera off'}catch{};foreach($r in 'host','client'){try{C $r 'coop_product_auto off'}catch{}}
 try{& "$root/work/PrepareShadowSword.ps1" *>> "$out/checkpoint-restore.log"}catch{"restore failed: $_"|Add-Content "$out/checkpoint-restore.log"}
 try{& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$PSScriptRoot/native-coin-reward-v786c" -Evidence "$out/note-restoration" *>> "$out/note-restoration.log"}catch{"note restore failed: $_"|Add-Content "$out/note-restoration.log"}
 [ordered]@{role=$Role;trace=@($trace)}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
 if($owned){$mutex.ReleaseMutex()}}
$prev='';foreach($r in $trace){$k="L$($r.logic)/$($r.peerLogic) hp$($r.health) $(@($r.scenes)-join ' + ') || $(@($r.active)-join ',') || pw$([Math]::Round([double]$r.partnerWeight,1))";if($k-ne$prev){"$($r.ms) $k";$prev=$k}}
