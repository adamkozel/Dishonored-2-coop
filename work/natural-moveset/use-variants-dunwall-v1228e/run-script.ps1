param([ValidateSet('dunwall','eotw')][string]$Mission='dunwall',[string]$Take,[string]$Candidate='candidate-npc-combat-v1213g')
# 1228e: 1228d's modal helper was named Clear, which PowerShell resolves to the Clear-Host ALIAS
# before any function ("Exception setting CursorPosition: The handle is invalid" in every case); it is
# ClearModals now. Otherwise identical to 1228d.
# 1228d also clears native modals: in 1228c (eotw) teleporting the host to the wind-turbine lever raised
# a tutorial modal (game logic 12) on both peers; look input was then ignored and every seek failed
# (take stopped and recovered). ClearModals() dismisses logic 12 (B) and a pause menu (logic 2, B) on both
# peers before every seek pose and every press, and records each dismissal in modals.json.
# 1228d: small rooms (the Dunwall sink tap) put a 2.4-3.4 m observer behind a wall (1228/1228b tap
# videos show only a wall). The observer now extends the source's own focus line - known clear for
# the first metre - to 2.2 m from the target, 25 degrees off it, eye 1.6 m up, pitched 15 down.
# 1228c: 1228b's held press registered every use on the first press. Observer framing: 1228/1228b put
# the observer beside the source, which can land behind a wall (1228 tap: only a wall in frame). The
# observer now stands on the source's own open approach side, 3.4 m from the target and 35 degrees
# off the source's line, at chest height, looking at the point between the source and the target,
# pitched 10 degrees down so the whole body (not only the torso) is in frame.
# 1228b: 1228 (dunwall) passed the ring lock in both roles and the client tap, but (a) presses were
# effectively ~50-150 ms: Send-D2Input publishes an expiring packet and returns at once, so the Neutral
# packet right after 'X 600' cut the press short (the 'first press does not register' pattern of 1227b);
# the press is now held for real (X 400, wait, then Neutral) and sourceUses is logged per attempt;
# (b) the host window use ended by movement (endReason 1, #5): the source's own status is now sampled
# during the use to measure the displacement; (c) observer framing was dark/occluded: the observer now
# films side-on (90 degrees off the source's facing) from 2.4 m at chest height.
# 1228 (#4, candidate 1213g): every use variant on a real mechanism, both roles, filmed on the partner.
# Targets come from the static mission entity scan (tools/diag/d2_resource_index.py) and the native
# interaction names (confirmed static): Dunwall fixture - ring locker (Usable_lock -> 5 key/lock), sink
# tap (Usable_tap -> 4 wheel/valve) and the window handle (generic 1; #5 early-end re-check);
# Edge of the World - turbine lever (Usable_lever -> 2) and door latches (Usable_latch -> 5).
# Robustness over 1227b: aim uses Set-NativeInteractionAim-v691 (native eye + view feedback) at the
# target point, falls back to a pitch sweep, and NO aim or pitch failure can throw ("Observer pitch did
# not settle" ended 1227b). Each case records pass/fail and the take continues; required cases decide
# the result. Focus is re-checked (and re-sought) before every Use press. Fixture restored.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab"
if(!$Take){$Take="use-variants-$Mission-v1228e"};$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$captures=[Collections.Generic.List[object]]::new();$rows=[Collections.Generic.List[object]]::new();$samplers=[Collections.Generic.List[object]]::new()
function Save($p,$v){$v|ConvertTo-Json -Depth 15|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed $r $mode"}
function F($r){for($i=0;$i-lt6;$i++){$raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$r] --focus;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Focus query failed $r"}
function I($r,[string]$b='Neutral',[int]$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}
function ProductStatus($r){S $r '.product.json'}
function Live($r){$s=S $r;$m=S $r '.movement.json';$x=S $r '.shadow-stream.json';$a=$s.probeRenderPosition;$b=$s.remotePosition
 $d=if($a-and$b){[Math]::Sqrt(($a[0]-$b[0])*($a[0]-$b[0])+($a[1]-$b[1])*($a[1]-$b[1])+($a[2]-$b[2])*($a[2]-$b[2]))}else{999}
 @{follow=$s.probeFollowEnabled;distance=[Math]::Round($d,3);product=(S $r '.product.json').result;bank=[bool]$m.active;bankVersion=$m.version;stream=[bool]($x.armed-and$x.ready);ok=($s.probeActorHandle-and$s.probeFollowEnabled-and$d-lt1.0-and$m.active-and$x.armed-and$x.ready-and$s.remoteFresh)}}
function WaitLive($tag,$limit=30000){$w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 300;$p=@{host=(Live host);client=(Live client)};$ok=$p.host.ok-and$p.client.ok}while(!$ok-and$w.ElapsedMilliseconds-lt$limit);$p.ok=$ok;$p.waitMs=$w.ElapsedMilliseconds;Save "$out/live-$tag.json" $p;return $p}
$modals=[Collections.Generic.List[object]]::new()
function ClearModals($tag){for($i=0;$i-lt4;$i++){$hit=$false;foreach($r in 'host','client'){$l=(S $r).localGameLogicState;if($l-in12,2){$hit=$true;$modals.Add(@{tag=$tag;role=$r;logic=$l;at=[DateTime]::Now.ToString('o')});I $r B 200;Start-Sleep -Milliseconds 300;I $r}};if(!$hit){return};Start-Sleep -Milliseconds 900}}
function Pitch($r,$deg){try{& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $r -Pitch $deg -Quick|Out-Null;$true}catch{$false}}
# Focus search: stand-off 1.0 then 0.75 m on four sides; at each pose aim the native eye at the target
# point (and +-0.12 m), then sweep pitch. Returns the focusing pose or $null; never throws on aim.
function Seek($src,$p,$match,$tag){$log=[Collections.Generic.List[object]]::new();$tries=0
 foreach($dist in 1.0,0.75){foreach($sd in 0,90,180,270){$a=$sd*[Math]::PI/180;$x=$p[0]+$dist*[Math]::Cos($a);$y=$p[1]+$dist*[Math]::Sin($a)
  C $src ("setviewpos {0:F3} {1:F3} {2:F3} {3} 0"-f$x,$y,($p[2]+0.7),(($sd+180)%360));Start-Sleep -Milliseconds 700;ClearModals "seek-$tag"
  $stand=(Q $src '--player').candidatePosition
  foreach($dz in 0,0.12,-0.12){$tries++;$aimed=$true;try{& "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role $src -X $p[0] -Y $p[1] -Z ($p[2]+$dz) -Evidence "$out/aim-$tag.json"}catch{$aimed=$false}
   Start-Sleep -Milliseconds 250;$f=F $src;$log.Add(@{dist=$dist;side=$sd;stand=$stand;dz=$dz;aimed=$aimed;focused=$f.focused;name=$f.name})
   if($f.focused-and$f.name-match$match){Save "$out/seek-$tag.json" @{found=$true;tries=$tries;log=@($log)};return @{dist=$dist;side=$sd;dz=$dz;name=$f.name;handle=$f.handle;tries=$tries}}}
  foreach($deg in 30,45,15,60,0){$tries++;$ok=Pitch $src $deg;Start-Sleep -Milliseconds 250;$f=F $src;$log.Add(@{dist=$dist;side=$sd;stand=$stand;pitch=$deg;pitched=$ok;focused=$f.focused;name=$f.name})
   if($f.focused-and$f.name-match$match){Save "$out/seek-$tag.json" @{found=$true;tries=$tries;log=@($log)};return @{dist=$dist;side=$sd;pitch=$deg;name=$f.name;handle=$f.handle;tries=$tries}}}}}
 Save "$out/seek-$tag.json" @{found=$false;tries=$tries;log=@($log)};$null}
function UseCase($src,$obs,[string[]]$names,$want,$match,[switch]$Optional){
 $row=[ordered]@{source=$src;observer=$obs;candidates=$names;expected=$want;optional=$Optional.IsPresent;passed=$false;failure=$null}
 try{
  $all=@((Q $src '--entities').entities);$focus=$null;$target=$null;$p=$null
  foreach($n in $names){$e=@($all|Where-Object name -eq $n);if($e.Count-ne1){continue}
   $focus=Seek $src $e[0].position $match "$src-$n";if($focus){$target=$n;$p=$e[0].position;break}}
  $row.target=$target;$row.focus=$focus
  if(!$focus){throw "No candidate could be focused ($($names-join ', '))"}
  $live=WaitLive "before-$src-$target" 25000;if(!$live.ok){throw "Partner presentation not live before the $src use of $target"}
  $sp=(Q $src '--player').candidatePosition;$oa=($focus.side+25)*[Math]::PI/180;$ox=$p[0]+2.2*[Math]::Cos($oa);$oy=$p[1]+2.2*[Math]::Sin($oa)
  $mx=($sp[0]+$p[0])/2;$my=($sp[1]+$p[1])/2;$oyaw=[Math]::Atan2($my-$oy,$mx-$ox)*180/[Math]::PI
  C $obs ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$ox,$oy,($sp[2]+1.75),$oyaw);Start-Sleep -Milliseconds 700
  $row.observerPitch=Pitch $obs 15;Start-Sleep -Milliseconds 300
  for($h=0;$h-lt3;$h++){C $obs 'coop_observer_camera hold';Start-Sleep -Milliseconds 500;$hold=S $obs '.observer-camera.json';if($hold.active){break};C $obs 'coop_observer_camera off';Start-Sleep -Milliseconds 700}
  if(!$hold.active){throw "Observer camera hold failed on ${obs}: $($hold.result)"}
  C $obs ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$ox,$oy,($sp[2]+1.6),$oyaw);Start-Sleep -Milliseconds 1200
  $live=WaitLive "camera-$src-$target" 20000;if(!$live.ok){throw "Partner presentation not live with the $obs camera held"}
  $before=(S $src '.movement.json').sourceUses
  $hwnd=(Get-Process -Id $pids[$obs]).MainWindowHandle.ToInt64()
  $ff=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','11','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/use-$src-$target-observed.mp4")
  $cap=Start-Process (Get-Command ffmpeg).Source -ArgumentList $ff -WindowStyle Hidden -RedirectStandardError "$out/use-$src-$target-capture.log" -PassThru;$captures.Add($cap)
  $sampler=Start-Process (Get-Process -Id $PID).Path -ArgumentList @('-NoProfile','-File',"$PSScriptRoot/Sample-StatusFile-v1023.ps1",'-Path',"$root/work/lab-instances/$obs/coop-status.json.movement.json",'-Out',"$out/use-$src-$target-observer-movement.ndjson",'-DurationMs',10000) -WindowStyle Hidden -PassThru;$samplers.Add($sampler)
  $srcSampler=Start-Process (Get-Process -Id $PID).Path -ArgumentList @('-NoProfile','-File',"$PSScriptRoot/Sample-StatusFile-v1023.ps1",'-Path',"$root/work/lab-instances/$src/coop-status.json",'-Out',"$out/use-$src-$target-source-status.ndjson",'-DurationMs',10000,'-IntervalMs',100) -WindowStyle Hidden -PassThru;$samplers.Add($srcSampler)
  Start-Sleep -Milliseconds 1000
  $pressLog=[Collections.Generic.List[object]]::new();$used=$false
  for($k=0;$k-lt3-and!$used;$k++){ClearModals "press-$src-$target";$f=F $src;$ok=$f.focused-and$f.name-match$match;$re=$null
   if(!$ok){$re=Seek $src $p $match "$src-$target-press$k";$f=F $src;$ok=$f.focused-and$f.name-match$match}
   if(!$ok){$pressLog.Add(@{attempt=$k;focusedAtPress=$ok;name=$f.name;reaimed=[bool]$re});continue}
   I $src X 400;Start-Sleep -Milliseconds 450;I $src;Start-Sleep -Milliseconds 1200;$uses=(S $src '.movement.json').sourceUses;$used=$uses-gt$before
   $pressLog.Add(@{attempt=$k;focusedAtPress=$ok;name=$f.name;reaimed=[bool]$re;heldMs=400;sourceUsesAfter=$uses;used=$used})}
  Save "$out/press-$src-$target.json" @($pressLog)
  if(!$sampler.WaitForExit(15000)){Stop-Process -Id $sampler.Id};if(!$srcSampler.WaitForExit(15000)){Stop-Process -Id $srcSampler.Id};if(!$cap.WaitForExit(30000)){Stop-Process -Id $cap.Id}
  $track=@(Get-Content "$out/use-$src-$target-source-status.ndjson"|ForEach-Object {$j=$_|ConvertFrom-Json;@{ms=$j.ms;p=$j.status.localPosition}}|Where-Object {$_.p})
  if($track.Count){$o=$track[0].p;$row.sourceMaxDisplacement=[Math]::Round(($track|ForEach-Object {[Math]::Sqrt(($_.p[0]-$o[0])*($_.p[0]-$o[0])+($_.p[1]-$o[1])*($_.p[1]-$o[1])+($_.p[2]-$o[2])*($_.p[2]-$o[2]))}|Measure-Object -Maximum).Maximum,3)}
  C $obs 'coop_observer_camera off'
  $sm=S $src '.movement.json'
  $samples=@(Get-Content "$out/use-$src-$target-observer-movement.ndjson"|ForEach-Object {($_|ConvertFrom-Json).status})
  $shown=@($samples|Where-Object {$_.useVariant-eq$want-and$_.useWeight-gt.5})
  $row.used=$used;$row.sourceUses=$sm.sourceUses;$row.sourceUseName=$sm.sourceUseName;$row.sourceLastVariant=$sm.sourceUseLastVariant;$row.endReason=$sm.sourceUseEndReason;$row.endAction=$sm.sourceUseEndAction
  $row.observerSamples=$samples.Count;$row.shownSamples=$shown.Count;$row.observedVariants=@($samples|ForEach-Object {$_.useVariant}|Sort-Object -Unique)
  $row.maxWeight=(($samples|ForEach-Object {[double]$_.useWeight}|Measure-Object -Maximum).Maximum);$row.maxAmount=(($samples|ForEach-Object {[double]$_.useAmount}|Measure-Object -Maximum).Maximum)
  if(!$used){throw "Source did not register a native use of $target ($src)"}
  if($sm.sourceUseLastVariant-ne$want){throw "Source classified $target as variant $($sm.sourceUseLastVariant), expected $want"}
  if(!$shown.Count){throw "Observer remote body did not present variant $want ($src -> $obs)"}
  $row.passed=$true
 }catch{$row.failure=$_.Exception.Message;try{C $obs 'coop_observer_camera off'}catch{}}
 $rows.Add($row);Save "$out/use-$src-$($names[0]).json" $row}
function WaitGameplay($limitMs){$w=[Diagnostics.Stopwatch]::StartNew();$log=[Collections.Generic.List[object]]::new();$last=@{host=0;client=0}
 do{Start-Sleep -Milliseconds 500;$ready=$true
  foreach($r in 'host','client'){$s=S $r;$log.Add(@{ms=$w.ElapsedMilliseconds;role=$r;logic=$s.localGameLogicState;pending=$s.localPendingGameLogicState;map=$s.localMapId})
   if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}
   if($s.localGameLogicState-in8,15-and$w.ElapsedMilliseconds-gt5000-and$w.ElapsedMilliseconds-$last[$r]-gt1500){I $r A 300;$last[$r]=$w.ElapsedMilliseconds}
   elseif($s.localGameLogicState-eq5-and$w.ElapsedMilliseconds-$last[$r]-gt4000){I $r X 3000;$last[$r]=$w.ElapsedMilliseconds}
   elseif($s.localGameLogicState-eq12-and$w.ElapsedMilliseconds-$last[$r]-gt1500){I $r B 200;$last[$r]=$w.ElapsedMilliseconds}}
  if($ready-and(S host).localMapId-ne(S client).localMapId){$ready=$false}
 }while(!$ready-and$w.ElapsedMilliseconds-lt$limitMs);return @{ready=$ready;log=@($log)}}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;if((S $r).localGameLogicState-ne1){throw 'Gameplay required'}}
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 if((Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash-ne$pin.probe){throw "Deployed probe is not the $Candidate probe"}
 Save "$out/identity.json" @{candidate=$Candidate;dll=$pin.dll;probe=$pin.probe;mission=$Mission}
 $dirty=$true
 if($Mission-eq'eotw'){
  foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_checkpoint_sync off','coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off'){C $r $cmd}}
  C host 'campaign campaign/edge_of_the_world/eotw.mapinfo low corvo';C client 'campaign campaign/edge_of_the_world/eotw.mapinfo low emily'
  $g=WaitGameplay 300000;Save "$out/load-log.json" $g.log;if(!$g.ready){throw 'Both instances did not reach the same mission gameplay'}
  foreach($r in 'host','client'){C $r $(if($r-eq'client'){'coop_checkpoint_delay 1500'}else{'coop_checkpoint_delay 0'})}
 }
 foreach($r in 'host','client'){C $r 'coop_product_auto on'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$ok=(ProductStatus host).result-eq'armed'-and(ProductStatus client).result-eq'armed'}while(!$ok-and$w.ElapsedMilliseconds-lt60000)
 if(!$ok){throw 'Product controller not armed'}
 if($Mission-eq'dunwall'){
  $locks=@('ld_ring_locker_01_1_lock','ld_ring_locker_01_2_lock','ld_ring_locker_01_1_open','ld_ring_locker_01_2_open','ld_ring_locker_01_3')
  UseCase host client $locks 5 'ring_locker|lock'
  UseCase client host $locks 5 'ring_locker|lock'
  UseCase host client @('esc_env_rich_sink_tap_01_1') 4 'tap'
  UseCase client host @('esc_env_rich_sink_tap_01_1') 4 'tap'
  # #5 re-check: the host-source generic use of the window ended at 24% in 1214c.
  UseCase host client @('ld_rich_small_gp_window_handle_37') 1 'window_handle' -Optional
  UseCase client host @('ld_rich_small_gp_window_handle_37') 1 'window_handle' -Optional
 }else{
  UseCase host client @('script_lever_windturbine_1_1_1') 2 'lever'
  UseCase client host @('script_lever_windturbine_1_1_1') 2 'lever'
  $latches=@('script_door_latch_blackmarket_apartment','script_door_latch_01_right_1','script_doorlatch_bm_backyard')
  UseCase host client $latches 5 'latch' -Optional
  UseCase client host $latches 5 'latch' -Optional
  # Visual-only: a tap in a larger room than the Dunwall sink (dissection table, then a sink).
  $taps=@('env_interiors_tap_1','env_interiors_tap_right','env_interiors_rich_sink_tap_01_1_6')
  UseCase host client $taps 4 'tap' -Optional
  UseCase client host $taps 4 'tap' -Optional
 }
 Save "$out/modals.json" @($modals)
 $bad=@($rows|Where-Object {!$_.optional-and!$_.passed})
 if($bad.Count){$failure=($bad|ForEach-Object {"$($_.source) $($_.candidates[0]): $($_.failure)"})-join '; '}
 Save "$out/verified.json" @{passed=!$bad.Count;required=@($rows|Where-Object {!$_.optional}).Count;failedRequired=$bad.Count}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  foreach($s in $samplers){try{if(!$s.HasExited){Stop-Process -Id $s.Id}}catch{}}
  foreach($c in @($captures)){try{if(!$c.HasExited-and!$c.WaitForExit(30000)){Stop-Process -Id $c.Id}}catch{}}
  if($dirty){
   $cw=[Diagnostics.Stopwatch]::StartNew()
   do{$off=$true;foreach($r in 'host','client'){try{I $r;C $r 'coop_observer_camera off';C $r 'coop_product_auto off';C $r 'coop_movement_resume off';foreach($cmd in 'coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off','coop_shadow_stream off','coop_movement_bank off','coop_trigger_mirror off'){C $r $cmd};C $r 'coop_checkpoint_sync off';$st=S $r
     if((S $r '.reset.json').enabled){$off=$false;if($st.localGameLogicState-in8,15){I $r A 300}elseif($st.localGameLogicState-eq2){I $r B 200}}}catch{}}
    if(!$off){Start-Sleep -Milliseconds 1500}}while(!$off-and$cw.ElapsedMilliseconds-lt60000)
   foreach($r in 'host','client'){try{if((S $r).localGameLogicState-in2,12){I $r B 200;Start-Sleep -Milliseconds 800}}catch{}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{$s=S $r;$hp=Q $r '--health' 1;if($s.localGameLogicState-ne1-or$hp.current-ne56-or!$s.remoteFresh){throw 'Final gameplay/health/transport not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;cases=@($rows);scope="Use variants on real mechanisms ($Mission): native use -> authored interaction on the partner body, both roles; fixture restored."}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
