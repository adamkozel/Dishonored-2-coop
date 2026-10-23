param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='mission-partner-powers-v1816a',[string]$Candidate='candidate-npc-combat-v1214ix',[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo',[ValidateSet('corvo','emily')][string]$HostCharacter='corvo',[ValidateSet('corvo','emily')][string]$PartnerCharacter='corvo',[string[]]$Powers=@('windblast'),[string]$Guard='script_spawner_checkpoint_guard_01',[string]$SecondGuard='script_spawner_checkpoint_guard_02',[int]$SampleSeconds=14,[switch]$EngageFirst)
# 1816: Domino marks each guard from 8 m behind it, then 8 m behind-left and 6 m behind, crouched (B) before each cast
# (mission-partner-powers-v1815a: 5 m to the side, the alerted guards killed her through the health hold, Game Over at
# 19 s, so guard 2 was never marked). The sample window starts when the cast ends (v1815a's 3-try casts outlasted it).
# 1815: Domino marks each guard from 5 m to its side, crouched, and checks the host's presented marks (her marks at the
# host's NPCs) for that guard, re-aiming up to 3 times (mission-partner-powers-v1814a: one mark, on another NPC, so no
# link). Domino passes only when the second guard ends as the first in both worlds (the linked death or knockout).
# 1814: on 1214ix. Domino stands 7 m to the side of the two checkpoint guards' midpoint (then 8 m behind), crouched, as
# Record-MissionCoverage domino does (mission-partner-powers-v1813c: 7 m in front of guard 1 it saw her, guard 2 was
# 18.5 m away, and her takedown never landed); after both marks she takes guard 1 down from 0.9 m behind (lethal, then
# a choke if it lives). The end gap of a live guard is the client's last position against the host's positions in the
# last 1.5 s (v1813c: 1.96 and 2 m with both channels bound while the guard ran; poses go every 400 ms).
# 1813: on 1214iw. -EngageFirst stands her 1.5 m in front of the guard for 2.5 s before her stand, so it is fighting her
# when the host binds it (the forced lease path; mission-partner-powers-v1810a/v1811c/v1812a found it by chance).
# Possession on a mission map: her copy runs, and the host guard follows it within 1.5 m and ends within 0.3 m of it
# (v1811b and v1812b: 1.0 and 1.41 m while she ran, 0 m at the end; the lab rule of 0.75 m was set on a walk).
# 1812: on 1214iv; through the replica wait, her copy of the guard is read every 500 ms with the probe's
# --npc-authority-all (decision, reflex and attack AI flags, pause references, mode, scheduler; <power>-wait-ai.json): in
# mission-partner-powers-v1811c her forced lease was acquired but the guard never read as paused within 4 s.
# 1811: from the cast to the first sample, both games' native phase fields (logic, pending, cinematic nodes and metadata,
# the peer's as seen) and NPC stream session results are sampled every 100 ms (phase-<power>.json): in
# mission-partner-powers-v1810a her NPC stream session ended at her Blink Assault (world_or_native_phase_changed).
# 1810: on 1214it (NPC stream stop and Hello traces); both worlds' NPC stream status is also saved right after the cast.
# 1809: both worlds' NPC stream status is saved after the replica wait (up to 25 s); mission-partner-powers-v1808a found
# no channel for the host's checkpoint guard in her world.
# 1808 (#180): the partner's powers on the host's guards on a mission map. Both lab games load the mission with the
# native campaign command (the host as -HostCharacter, the partner as -PartnerCharacter) and arm the product controller.
# For each of -Powers the mission is loaded fresh, so every power meets an untouched guard: the partner's power is put in
# her own inventory (coop_grant_named, the native item a rune purchase adds, with the upgrade the cast needs), she stands
# by the host's checkpoint guard (-Guard) once her copy of it is a ready replica, and casts it with pad input. The host
# films from a free camera beside the guard; both windows are filmed. The guard is sampled in both worlds every 250 ms
# for -SampleSeconds (position, health, its NPC channel and the power's host status).
# Passed per power: the power took effect in her world, its host-world effect is there (below), and the guard ends the
# same in both worlds (alive, unconscious or dead; alive ones within 1.5 m).
# - Corvo partner: windblast (the host guard moves or is hurt), swarm (the host's native swarm spawns), bendtime (Stop
#   Time: the host holds her time request), possession (the host guard follows her possessed copy within 0.75 m, paired
#   variant 1000), blinkkick (Blink Assault: the host guard is moved or knocked down);
# - Emily partner: farreach (Far Reach with pull NPCs: the host guard follows the pull), domino (both checkpoint guards
#   linked, then a lethal takedown of one: both end the same in both worlds), doppelganger (the host spawns the shade's
#   puppet), mesmerize (the host's Mesmerize object takes a host NPC), shadowwalk (the shadow-form attack: the guard
#   ends dead or unconscious in both worlds).
# The lab is relaunched and restored at the end (Deploy-SourceClock) and the protected files are checked. Generated by
# make_mission_powers_1816_gen.py.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$inv=[Globalization.CultureInfo]::InvariantCulture
$Powers=@($Powers|ForEach-Object {$_-split','}|Where-Object {$_})
$known=@{corvo=@('windblast','swarm','bendtime','possession','blinkkick');emily=@('farreach','domino','doppelganger','mesmerize','shadowwalk')}
foreach($pw in $Powers){if($pw-notin$known[$PartnerCharacter]){throw "$pw is not a $PartnerCharacter power"}}
if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
$pids=@{}
function Save($p,$v){$v|ConvertTo-Json -Depth 15|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$b='Neutral',$ms=100,$ly=0,$lx=0){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms -LeftY $ly -LeftX $lx|Out-Null}
function S($r,$suffix=''){for($a=0;$a-lt40;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function RawQ($r,$mode){$o=& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode 2>&1|Out-String;[pscustomobject]@{exit=$LASTEXITCODE;text=$(if($o.Length-gt600){$o.Substring(0,600)}else{$o})}}
function Q($r,$mode,$arg=0){for($a=0;$a-lt8;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read failed $mode"}
function P($x,$y,$z,$yaw,$pitch=0){'setviewpos {0} {1} {2} {3} {4}' -f ([double]$x).ToString('F3',$inv),([double]$y).ToString('F3',$inv),([double]$z).ToString('F3',$inv),([double]$yaw).ToString('F2',$inv),([double]$pitch).ToString('F1',$inv)}
function Live($r){$s=S $r;$m=S $r '.movement.json';$x=S $r '.shadow-stream.json';$a=$s.probeRenderPosition;$b=$s.remotePosition
 $d=if($a-and$b){[Math]::Sqrt(($a[0]-$b[0])*($a[0]-$b[0])+($a[1]-$b[1])*($a[1]-$b[1])+($a[2]-$b[2])*($a[2]-$b[2]))}else{999}
 @{follow=$s.probeFollowEnabled;distance=[Math]::Round($d,3);product=(S $r '.product.json').result;bank=[bool]$m.active;stream=[bool]($x.armed-and$x.ready);ok=($s.probeActorHandle-and$s.probeFollowEnabled-and$d-lt1.0-and$m.active-and$x.armed-and$x.ready-and$s.remoteFresh)}}
function WaitLive($tag,$limit=30000){$w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 300;$p=@{host=(Live host);client=(Live client)};$ok=$p.host.ok-and$p.client.ok}while(!$ok-and$w.ElapsedMilliseconds-lt$limit);$p.ok=$ok;$p.waitMs=$w.ElapsedMilliseconds;Save "$out/live-$tag.json" $p;$p}
function WaitGameplay($limitMs){$w=[Diagnostics.Stopwatch]::StartNew();$log=[Collections.Generic.List[object]]::new();$last=@{host=0;client=0}
 do{Start-Sleep -Milliseconds 500;$ready=$true
  foreach($r in 'host','client'){$s=S $r;$log.Add(@{ms=$w.ElapsedMilliseconds;role=$r;logic=$s.localGameLogicState;pending=$s.localPendingGameLogicState;map=$s.localMapId})
   if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}
   if($s.localGameLogicState-in8,15-and$w.ElapsedMilliseconds-gt5000-and$w.ElapsedMilliseconds-$last[$r]-gt1500){I $r A 300;$last[$r]=$w.ElapsedMilliseconds}
   elseif($s.localGameLogicState-eq5-and$w.ElapsedMilliseconds-$last[$r]-gt4000){I $r X 3000;$last[$r]=$w.ElapsedMilliseconds}
   elseif($s.localGameLogicState-eq12-and$w.ElapsedMilliseconds-$last[$r]-gt1500){I $r B 200;$last[$r]=$w.ElapsedMilliseconds}}
  if($ready-and(S host).localMapId-ne(S client).localMapId){$ready=$false}
 }while(!$ready-and$w.ElapsedMilliseconds-lt$limitMs);@{ready=$ready;log=@($log)}}

$attr='components/characters/player/base/attributes'
$items=@{windblast=@('models/powers/windblast/windblast.inventoryitem');swarm=@('models/powers/devouring_swarm/devouring_swarm.inventoryitem')
 bendtime=@('models/powers/bendtime/bendtime.inventoryitem',"$attr/bendtime_stoptime.inventoryitem");possession=@('models/powers/possession/possession.inventoryitem',"$attr/possession_parasite.inventoryitem")
 blinkkick=@('models/powers/blink/blink.inventoryitem',"$attr/blink_assault.inventoryitem");farreach=@('models/powers/grapple/grapple.inventoryitem',"$attr/grapple_pull_objects.inventoryitem","$attr/grapple_pull_npcs.inventoryitem")
 domino=@('models/powers/phantasmallink/phantasmal_link.inventoryitem');doppelganger=@('models/powers/doppelganger/doppelganger.inventoryitem')
 mesmerize=@('models/powers/mesmerize/mesmerize.inventoryitem',"$attr/mesmerize_incombat.inventoryitem");shadowwalk=@('models/powers/shadow_walk/shadow_walk.inventoryitem')}
# Where she stands: side of the guard (front = along its facing) and distance.
$stands=@{windblast=@('front',4);swarm=@('front',4);bendtime=@('front',5);possession=@('front',4);blinkkick=@('front',6);farreach=@('front',7);domino=@('front',7);doppelganger=@('behind',5);mesmerize=@('behind',5);shadowwalk=@('behind',2.4)}
# Her status file whose sent counter confirms a cast (retried once if it did not rise).
$proofs=@{windblast='.windblast-fx.json';swarm='.swarm-world.json';possession='.possession-fx.json';blinkkick='.power-fx-stream.json'}
function F($v){([double]$v).ToString('F3',$inv)}
function Npc($r,$spawner){$n=@((Q $r '--world-npcs').npcs|Where-Object {$_.spawnerName-eq$spawner});if($n.Count){$n[0]}else{$null}}
function Outcome($r,$spawner){$n=Npc $r $spawner;if(!$n){return 'absent'};$h=try{Q $r '--health' $n.handle}catch{$null}
 if(!$h){return 'unreadable'};if($h.current-le0){'dead'}elseif($h.ownerSuppressed){'unconscious'}else{'alive'}}
function D2($a,$b){if(!$a-or!$b){return $null};[Math]::Round([Math]::Sqrt([Math]::Pow([double]$a[0]-[double]$b[0],2)+[Math]::Pow([double]$a[1]-[double]$b[1],2)),2)}
function Channel($r,$handle){try{@((S $r '.npc-stream.json').entries|Where-Object {$_.handle-eq$handle})|Select-Object -First 1}catch{$null}}
function Film($key,$seconds){$list=@();foreach($pair in @(@('client','partner'),@('host','host'))){$hwnd=(Get-Process -Id $pids[$pair[0]]).MainWindowHandle.ToInt64()
  $list+=Start-Process (Get-Command ffmpeg).Source -ArgumentList @('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',[string]$seconds,'-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-movflags','+faststart',"$out/$key-$($pair[1]).mp4") -WindowStyle Hidden -RedirectStandardError "$out/$key-$($pair[1])-capture.log" -PassThru}
 $list}
function Aim($target){$pl=Q client '--player';$me=$pl.candidatePosition;$eye=[double]$pl.nativeViewPosition[2]
 $yaw=[Math]::Atan2([double]$target[1]-[double]$me[1],[double]$target[0]-[double]$me[0])*180/[Math]::PI;$h=[Math]::Max(.5,[double](D2 $target $me))
 $pitch=[Math]::Max(-60,[Math]::Min(60,[Math]::Atan2($eye-([double]$target[2]+1.3),$h)*180/[Math]::PI))
 C client (P $me[0] $me[1] ([double]$me[2]+1.7) $yaw);Start-Sleep -Milliseconds 300;try{& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch $pitch -Quick|Out-Null}catch{}
 [ordered]@{target=$target;stand=$me;yaw=[Math]::Round($yaw,1);pitch=[Math]::Round($pitch,1);distance=$h}}
# Stands her at -Side/-Distance of the guard (her world's copy), trying the other sides when the spot is off its floor.
function Stand($spawner,$side,$dist){$g=Npc client $spawner;if(!$g){throw "Her copy of $spawner absent"};$gp=$g.position
 $fx=[double]$g.orientation[0];$fy=[double]$g.orientation[1];$fl=[Math]::Max(1e-3,[Math]::Sqrt($fx*$fx+$fy*$fy));$fx/=$fl;$fy/=$fl
 $dirs=[ordered]@{front=@($fx,$fy);behind=@(-$fx,-$fy);left=@(-$fy,$fx);right=@($fy,-$fx)};$order=@($side)+@($dirs.Keys|Where-Object {$_-ne$side});$trials=@()
 foreach($s in $order){$d=$dirs[$s];$tx=[double]$gp[0]+$dist*$d[0];$ty=[double]$gp[1]+$dist*$d[1]
  C client (P $tx $ty ([double]$gp[2]+1.7) ([Math]::Atan2([double]$gp[1]-$ty,[double]$gp[0]-$tx)*180/[Math]::PI));Start-Sleep -Milliseconds 1200;$me=(Q client '--player').candidatePosition
  $ok=[Math]::Abs([double]$me[2]-[double]$gp[2])-lt.8-and[double](D2 $me @($tx,$ty))-lt.6;$trials+=[ordered]@{side=$s;asked=@($tx,$ty);rest=$me;ok=$ok}
  if($ok){return [ordered]@{side=$s;stand=$me;guard=$gp;facing=@($fx,$fy);trials=$trials}}}
 throw "No stand at $dist m from $spawner on its floor ($(($trials|ForEach-Object {$_.side+':'+($_.rest-join'/')})-join ' '))"}
function DominoStand{$ga=Npc client $Guard;$gb=Npc client $SecondGuard;if(!$ga-or!$gb){throw 'Checkpoint guards missing'};$pa=$ga.position;$pb=$gb.position
 $mx=([double]$pa[0]+[double]$pb[0])/2;$my=([double]$pa[1]+[double]$pb[1])/2;$mz=([double]$pa[2]+[double]$pb[2])/2
 $ox=[double]$ga.orientation[0];$oy=[double]$ga.orientation[1];$ol=[Math]::Max(1e-3,[Math]::Sqrt($ox*$ox+$oy*$oy));$ox/=$ol;$oy/=$ol;$trials=@()
 foreach($o in @(@(-$oy,$ox,7,'left'),@($oy,-$ox,7,'right'),@(-$ox,-$oy,8,'behind'))){$tx=$mx+$o[2]*$o[0];$ty=$my+$o[2]*$o[1]
  C client (P $tx $ty ($mz+1.7) ([Math]::Atan2([double]$pa[1]-$ty,[double]$pa[0]-$tx)*180/[Math]::PI));Start-Sleep -Milliseconds 1200;$me=(Q client '--player').candidatePosition
  $ok=[Math]::Abs([double]$me[2]-$mz)-lt.8-and[double](D2 $me @($mx,$my))-lt9.5;$trials+=[ordered]@{side=$o[3];asked=@($tx,$ty);rest=$me;ok=$ok}
  if($ok){& "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 200|Out-Null;Start-Sleep -Milliseconds 900;return [ordered]@{side=$o[3];stand=$me;guard=$pa;facing=@($ox,$oy);trials=$trials;crouched=$true}}}
 throw "No Domino stand near the checkpoint guards ($(($trials|ForEach-Object {$_.side+':'+($_.rest-join'/')})-join ' '))"}
function HostCamera($gp,$stand){$mx=([double]$gp[0]+[double]$stand[0])/2;$my=([double]$gp[1]+[double]$stand[1])/2;$dx=[double]$stand[0]-[double]$gp[0];$dy=[double]$stand[1]-[double]$gp[1];$dl=[Math]::Max(.1,[Math]::Sqrt($dx*$dx+$dy*$dy))
 $cx=$mx-6*$dy/$dl;$cy=$my+6*$dx/$dl;$cz=[double]$gp[2]+2.6;$cyaw=[Math]::Atan2($my-$cy,$mx-$cx)*180/[Math]::PI;$hp=(Q host '--player').candidatePosition
 C host (P $hp[0] $hp[1] ([double]$hp[2]+1.7) $cyaw);Start-Sleep -Milliseconds 400;try{& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch 18 -Quick|Out-Null}catch{}
 C host ('coop_observer_camera hold_at {0} {1} {2}' -f (F $cx),(F $cy),(F $cz));@($cx,$cy,$cz)}
function Load($tag){foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_player_health_hold off','coop_observer_camera off','coop_product_auto off','coop_checkpoint_sync off','coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off'){C $r $cmd}}
 C host "campaign $MapInfo low $HostCharacter";C client "campaign $MapInfo low $PartnerCharacter"
 $g=WaitGameplay 300000;Save "$out/$tag-load-log.json" $g.log;if(!$g.ready){throw 'Both instances did not reach the same mission gameplay'}
 foreach($r in 'host','client'){I $r;C $r 'coop_movement_fixture off';C $r 'coop_product_auto on'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$ok=(S host '.product.json').result-eq'armed'-and(S client '.product.json').result-eq'armed'}while(!$ok-and$w.ElapsedMilliseconds-lt90000)
 if(!$ok){throw 'Product controller not armed on the mission'}
 $l=WaitLive "$tag-armed" 60000;if(!$l.ok){throw 'Partner presentation not live after arming'}
 foreach($r in 'host','client'){C $r 'coop_player_health_hold on'}}
function Phase($t){$x=[ordered]@{ms=$t};foreach($r in 'host','client'){$s=S $r;$n=try{S $r '.npc-stream.json'}catch{$null}
  $x[$r]=[ordered]@{logic=$s.localGameLogicState;pending=$s.localPendingGameLogicState;nodes=$s.localCinematicNodeCount;meta=$s.localCinematicMetadataValid;signature=$s.localCinematicSignature;remoteLogic=$s.remoteGameLogicState;remoteNodes=$s.remoteCinematicNodeCount;remoteMeta=$s.remoteCinematicMetadataValid;stream=$(if($n){$n.result}else{$null});session=$(if($n){$n.session.result}else{$null});bound=$(if($n){$n.relevance.bound}else{$null})}}
 [pscustomobject]$x}
function Extra($pw){$x=[ordered]@{}
 switch($pw){
  'swarm'{$x.swarmHost=$(try{$q=S host '.swarm-host.json';[ordered]@{alive=$q.alive;result=$q.result;spawned=$q.spawned}}catch{$null})}
  'bendtime'{foreach($r in 'host','client'){$x["timeBend_$r"]=$(try{$tb=S $r '.time-bend.json';[ordered]@{localActive=$tb.local.active;localPercent=$tb.local.percent;remoteActive=$tb.remote.active;remotePercent=$tb.remote.percent;request=$tb.host.request;worldFactor=$tb.worldFactor;requests=$tb.requests;result=$tb.result}}catch{$null})}}
  'mesmerize'{$x.mesmerizeHost=$(try{$mh=S host '.mesmerize-host.json';[ordered]@{state=$mh.state;targets=$mh.targets;targetHandles=$mh.targetHandles;follow=$mh.follow;result=$mh.result}}catch{$null})}
  'doppelganger'{$x.shadeHost=$(try{$sh=S host '.shade-host.json';[ordered]@{spawned=$sh.spawned;puppet=$sh.puppet;result=$sh.result}}catch{$null})}}
 $x}
function Sample($t,$pw,$handles){$x=[ordered]@{ms=$t}
 foreach($k in $handles.Keys){$r=$k.Split('_')[0];$h=$handles[$k];$n=@((Q $r '--world-npcs').npcs|Where-Object {$_.handle-eq$h})|Select-Object -First 1;$hl=try{Q $r '--health' $h}catch{$null}
  $x[$k]=[ordered]@{present=[bool]$n;position=$n.position;health=$hl.current;suppressed=$hl.ownerSuppressed}
  $ch=Channel $r $h;$x["channel_$k"]=$(if($ch){[ordered]@{result=$ch.result;localPaired=$ch.localPaired;pairedVariant=$ch.pairedVariant;pairedResult=$ch.pairedResult;reversePoses=$ch.reversePoses;downSent=$ch.downSent;downReceived=$ch.downReceived}}else{$null})}
 foreach($e in (Extra $pw).GetEnumerator()){$x[$e.Key]=$e.Value};[pscustomobject]$x}
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$rows=[Collections.Generic.List[object]]::new()
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 [ordered]@{pid=$PID;operation='MissionPartnerPowers1816';take=$Take;started=(Get-Date).ToString('o');state='running'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;$dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;if($dll-ne$pin.dll){throw "Expected $Candidate"}
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$pids[$r]=$l.pid;if((S $r).localGameLogicState-ne1){throw "$r not in gameplay"}}
 Save "$out/identity.json" ([ordered]@{candidate=$Candidate;dll=$dll;mapInfo=$MapInfo;hostCharacter=$HostCharacter;partnerCharacter=$PartnerCharacter;powers=$Powers;guard=$Guard})
 foreach($pw in $Powers){$row=[ordered]@{power=$pw;pairing="$HostCharacter-$PartnerCharacter";passed=$false;failure=$null;checks=[Collections.Generic.List[string]]::new()};$caps=@()
  try{
   Load $pw
   $row.definitions=[ordered]@{host=(Q host '--character').definition;client=(Q client '--character').definition}
   if("$($row.definitions.client)"-notmatch"/$PartnerCharacter/"-or"$($row.definitions.host)"-notmatch"/$HostCharacter/"){throw "Pairing not loaded ($($row.definitions.host) / $($row.definitions.client))"}
   $before=Q client '--player-inventory';Save "$out/$pw-inventory-before.json" $before
   $row.grants=@();foreach($it in $items[$pw]){C client "coop_grant_named $it";Start-Sleep -Milliseconds 1300
    $row.grants+=[ordered]@{item=$it;result=$(try{(S client '.observer-camera.json').itemGrant.named.result}catch{$null})}}
   if($EngageFirst){$eg=Npc client $Guard;$ex=[double]$eg.orientation[0];$ey=[double]$eg.orientation[1];$el=[Math]::Max(1e-3,[Math]::Sqrt($ex*$ex+$ey*$ey));$ex/=$el;$ey/=$el
    C client (P ([double]$eg.position[0]+1.5*$ex) ([double]$eg.position[1]+1.5*$ey) ([double]$eg.position[2]+1.7) ([Math]::Atan2(-$ey,-$ex)*180/[Math]::PI));Start-Sleep -Milliseconds 2500;$row.engaged=$true}
   $sd=$stands[$pw];$st=if($pw-eq'domino'){DominoStand}else{Stand $Guard $sd[0] $sd[1]};$row.stand=$st
   $hg=Npc host $Guard;if(!$hg){throw "Host guard $Guard absent"}
   $aiWait=[Collections.Generic.List[object]]::new();$aiAt=-1000
   $rw=[Diagnostics.Stopwatch]::StartNew();do{$hg=Npc host $Guard;$cg=Npc client $Guard;$ch=if($cg){Channel client $cg.handle}else{$null}
    if($rw.ElapsedMilliseconds-$aiAt-ge500){$aiAt=$rw.ElapsedMilliseconds;$ai=try{@((Q client '--npc-authority-all').entries|Where-Object {$_.name-eq$Guard})|Select-Object -First 1}catch{$null}
     $aiWait.Add([pscustomobject][ordered]@{ms=$rw.ElapsedMilliseconds;channel=$(if($ch){$ch.result}else{$null});ai=$ai})}
    $ready=$cg-and$ch-and$ch.result-eq'client_replica'-and[double](D2 $cg.position $hg.position)-lt.5;if(!$ready){Start-Sleep -Milliseconds 300}}while(!$ready-and$rw.ElapsedMilliseconds-lt25000)
   foreach($r in 'host','client'){try{Copy-Item -LiteralPath "$root/work/lab-instances/$r/coop-status.json.npc-stream.json" -Destination "$out/$pw-wait-$r.npc-stream.json" -Force}catch{}};Save "$out/$pw-wait-ai.json" @($aiWait)
   $row.guardAtWait=[ordered]@{host=$(if($hg){[ordered]@{handle=$hg.handle;position=$hg.position}}else{$null});client=$(if($cg){[ordered]@{handle=$cg.handle;position=$cg.position}}else{$null})}
   $row.replicaReady=[ordered]@{ready=[bool]$ready;ms=$rw.ElapsedMilliseconds;channel=$(if($ch){$ch.result}else{$null});gap=$(if($cg){D2 $cg.position $hg.position}else{$null})}
   if(!$ready){throw "Her copy of the host guard is not a ready replica ($($row.replicaReady.channel), gap $($row.replicaReady.gap))"}
   $handles=[ordered]@{host=$hg.handle;client=$cg.handle}
   if($pw-eq'domino'){$hg2=Npc host $SecondGuard;$cg2=Npc client $SecondGuard;if(!$hg2-or!$cg2){throw "Second guard $SecondGuard absent"};$handles.host_2=$hg2.handle;$handles.client_2=$cg2.handle}
   $row.camera=HostCamera $hg.position $st.stand
   $row.aim=Aim $cg.position;Start-Sleep -Milliseconds 600
   $caps=Film $pw ($SampleSeconds+14);Start-Sleep -Milliseconds 1500
   $first=Sample 0 $pw $handles;$row.before=$first
   $series=[Collections.Generic.List[object]]::new();$sw=[Diagnostics.Stopwatch]::StartNew()
   $cast={switch($pw){
     'windblast'{& "$lab/Send-D2Input.ps1" -Role client -LeftTrigger 255 -DurationMs 350|Out-Null}
     'swarm'{& "$lab/Send-D2Input.ps1" -Role client -LeftTrigger 255 -DurationMs 600|Out-Null}
     'bendtime'{& "$lab/Send-D2Input.ps1" -Role client -LeftTrigger 255 -DurationMs 350|Out-Null}
     'possession'{& "$lab/Send-D2Input.ps1" -Role client -LeftTrigger 255 -DurationMs 1500|Out-Null}
     'blinkkick'{& "$lab/Send-D2Input.ps1" -Role client -LeftTrigger 255 -DurationMs 1000|Out-Null;Start-Sleep -Milliseconds 1120;& "$lab/Send-D2Input.ps1" -Role client -RightTrigger 255 -DurationMs 250|Out-Null}
     'farreach'{& "$lab/Send-D2Input.ps1" -Role client -LeftTrigger 255 -DurationMs 1000|Out-Null}
     'doppelganger'{& "$lab/Send-D2Input.ps1" -Role client -LeftTrigger 255 -DurationMs 800|Out-Null}
     'mesmerize'{& "$lab/Send-D2Input.ps1" -Role client -LeftTrigger 255 -DurationMs 1200|Out-Null}
     'shadowwalk'{& "$lab/Send-D2Input.ps1" -Role client -LeftTrigger 255 -DurationMs 250|Out-Null;Start-Sleep -Milliseconds 1500;& "$lab/Send-D2Input.ps1" -Role client -LeftY 14000 -DurationMs 1200|Out-Null;& "$lab/Send-D2Input.ps1" -Role client -RightTrigger 255 -DurationMs 250|Out-Null}
     'domino'{$row.aims=@();$row.marks=[ordered]@{}
      foreach($sp in @($Guard,$SecondGuard)){$marked=$false
       for($try=1;$try-le3-and!$marked;$try++){$tg=Npc client $sp;$fx=[double]$tg.orientation[0];$fy=[double]$tg.orientation[1];$fl=[Math]::Max(1e-3,[Math]::Sqrt($fx*$fx+$fy*$fy));$fx/=$fl;$fy/=$fl
        $off=@(@(8,0),@(8,-3),@(6,0))[$try-1];$sx=[double]$tg.position[0]-$off[0]*$fx-$off[1]*$fy;$sy=[double]$tg.position[1]-$off[0]*$fy+$off[1]*$fx
        C client (P $sx $sy ([double]$tg.position[2]+1.7) ([Math]::Atan2([double]$tg.position[1]-$sy,[double]$tg.position[0]-$sx)*180/[Math]::PI));Start-Sleep -Milliseconds 700
        if(try{(Q client '--player-states').stance-ne1}catch{$true}){& "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 200|Out-Null;Start-Sleep -Milliseconds 700}
        $row.aims+=(Aim (Npc client $sp).position);& "$lab/Send-D2Input.ps1" -Role client -LeftTrigger 255 -DurationMs 1800|Out-Null;Start-Sleep -Milliseconds 1200
        $hm=try{@((S host '.domino-world.json').marks|ForEach-Object {[int64]$_.target})}catch{@()};$hh=(Npc host $sp).handle
        $marked=$hm-contains[int64]$hh;$row.marks[$sp]=[ordered]@{tries=$try;marked=$marked;hostMarks=$hm}}}
      $row.dominoLinked=$(try{$dw=S client '.domino-world.json';[ordered]@{sent=$dw.sent;markCount=$dw.markCount;readyMask=$dw.readyMask}}catch{$null})
      $cgA=Npc client $Guard;$ax=[double]$cgA.orientation[0];$ay=[double]$cgA.orientation[1];$al=[Math]::Max(1e-3,[Math]::Sqrt($ax*$ax+$ay*$ay));$ax/=$al;$ay/=$al
      C client (P ([double]$cgA.position[0]-.9*$ax) ([double]$cgA.position[1]-.9*$ay) ([double]$cgA.position[2]+1.7) ([Math]::Atan2($ay,$ax)*180/[Math]::PI));Start-Sleep -Milliseconds 250
      & "$lab/Send-D2Input.ps1" -Role client -RightTrigger 255 -DurationMs 200|Out-Null;Start-Sleep -Milliseconds 1500;$row.killPress='lethal'
      if((Outcome client $Guard)-eq'alive'){$cgA=Npc client $Guard;$ax=[double]$cgA.orientation[0];$ay=[double]$cgA.orientation[1];$al=[Math]::Max(1e-3,[Math]::Sqrt($ax*$ax+$ay*$ay));$ax/=$al;$ay/=$al
       C client (P ([double]$cgA.position[0]-.9*$ax) ([double]$cgA.position[1]-.9*$ay) ([double]$cgA.position[2]+1.7) ([Math]::Atan2($ay,$ax)*180/[Math]::PI));Start-Sleep -Milliseconds 250
       & "$lab/Send-D2Input.ps1" -Role client -Button RightShoulder -DurationMs 6500|Out-Null;$row.killPress='lethal,choke'}}}}
   $proof=$proofs[$pw];$row.attempts=@()
   $phase=[Collections.Generic.List[object]]::new();$phaseJob=$null
   for($attempt=1;$attempt-le2;$attempt++){$s0=if($proof){[int](S client $proof).sent}else{0};$row.castAt=$sw.ElapsedMilliseconds;$phase.Add((Phase $sw.ElapsedMilliseconds));& $cast
    $pw0=$sw.ElapsedMilliseconds;while($sw.ElapsedMilliseconds-$pw0-lt$(if($proof){2500}else{1500})){$phase.Add((Phase $sw.ElapsedMilliseconds));Start-Sleep -Milliseconds 100}
    if(!$proof){break};$s1=[int](S client $proof).sent;$row.attempts+=[ordered]@{attempt=$attempt;sentBefore=$s0;sentAfter=$s1};if($s1-gt$s0){break}
    $cg=Npc client $Guard;if($cg){$null=Aim $cg.position;Start-Sleep -Milliseconds 500}}
   foreach($r in 'host','client'){try{Copy-Item -LiteralPath "$root/work/lab-instances/$r/coop-status.json.npc-stream.json" -Destination "$out/$pw-cast-$r.npc-stream.json" -Force}catch{}}
   if($pw-eq'possession'){& "$lab/Send-D2Input.ps1" -Role client -LeftY 32000 -DurationMs 3000|Out-Null}
   $endPressed=$false;$row.castEnd=$sw.ElapsedMilliseconds
   while($sw.ElapsedMilliseconds-lt(($SampleSeconds*1000)+[int]$row.castEnd)){$series.Add((Sample $sw.ElapsedMilliseconds $pw $handles))
    if($pw-eq'bendtime'-and!$endPressed-and$sw.ElapsedMilliseconds-gt[int]$row.castAt+8000){if((S client '.time-bend.json').local.active){& "$lab/Send-D2Input.ps1" -Role client -LeftTrigger 255 -DurationMs 350|Out-Null};$endPressed=$true}
    Start-Sleep -Milliseconds 200}
   Save "$out/$pw-series.json" @($series);Save "$out/phase-$pw.json" @($phase)
   foreach($r in 'host','client'){foreach($suffix in '.npc-combat.json','.npc-stream.json','.swarm-host.json','.swarm-world.json','.time-bend.json','.mesmerize-host.json','.shade-host.json','.doppelganger-world.json','.domino-world.json','.windblast-fx.json','.possession-fx.json','.power-fx-stream.json','.character.json'){
    try{Copy-Item -LiteralPath "$root/work/lab-instances/$r/coop-status.json$suffix" -Destination "$out/$pw-after-$r$suffix" -Force}catch{}}}
   foreach($c in $caps){try{if(!$c.WaitForExit(40000)){Stop-Process -Id $c.Id}}catch{}};$caps=@()
   $row.outcome=[ordered]@{host=(Outcome host $Guard);client=(Outcome client $Guard)}
   if($pw-eq'domino'){$row.outcome2=[ordered]@{host=(Outcome host $SecondGuard);client=(Outcome client $SecondGuard)}}
   $moveH=(@($series|ForEach-Object {D2 $first.host.position $_.host.position}|Where-Object {$_-ne$null})+@(0)|Measure-Object -Maximum).Maximum
   $moveC=(@($series|ForEach-Object {D2 $first.client.position $_.client.position}|Where-Object {$_-ne$null})+@(0)|Measure-Object -Maximum).Maximum
   $last=$series[$series.Count-1];$rawGap=D2 $last.host.position $last.client.position
   $trail=@($series|Where-Object {$_.ms-ge[int]$last.ms-1500-and$_.host.position}|ForEach-Object {D2 $_.host.position $last.client.position}|Where-Object {$_-ne$null})
   $gap=if($trail.Count){($trail|Measure-Object -Minimum).Minimum}else{$rawGap}
   $hurtH=$null-ne$last.host.health-and[double]$last.host.health-lt[double]$first.host.health;$hurtC=$null-ne$last.client.health-and[double]$last.client.health-lt[double]$first.client.health
   $row.summary=[ordered]@{hostGuardMaxMove=$moveH;herCopyMaxMove=$moveC;endGap=$gap;rawEndGap=$rawGap;hostHealth=@($first.host.health,$last.host.health);herHealth=@($first.client.health,$last.client.health);outcome=$row.outcome}
   $fails=[Collections.Generic.List[string]]::new()
   $herEffect=$moveC-ge.5-or$hurtC-or$row.outcome.client-ne'alive'
   switch($pw){
    'windblast'{if(!$herEffect){$fails.Add("her blast did nothing to her copy (moved $moveC m)")};if(!($moveH-ge.5-or$hurtH-or$row.outcome.host-ne'alive')){$fails.Add("the host guard was not moved or hurt (moved $moveH m)")}}
    'swarm'{$sp=(@($series|ForEach-Object {[int]$_.swarmHost.spawned})+@(0)|Measure-Object -Maximum).Maximum;$row.checks.Add("host swarm spawned $sp");if($sp-lt1){$fails.Add('the host spawned no native swarm')}}
    'bendtime'{$req=@($series|Where-Object {$null-ne$_.timeBend_host.request-and[int]$_.timeBend_host.request-ge0});$her=@($series|Where-Object {$_.timeBend_client.localActive});$row.checks.Add("her Stop Time in $($her.Count) samples; the host held her request in $($req.Count)")
     if(!$her.Count){$fails.Add('her Bend Time never ran')};if(!$req.Count){$fails.Add('the host never held her time request')}}
    'possession'{$gaps=@($series|Where-Object {$_.host.position-and$_.client.position}|ForEach-Object {D2 $_.host.position $_.client.position});$maxGap=($gaps+@(0)|Measure-Object -Maximum).Maximum
     $variants=@($series|ForEach-Object {$_.channel_host.pairedVariant}|Where-Object {$_}|Select-Object -Unique);$row.possession=[ordered]@{maxGap=$maxGap;hostVariants=$variants}
     $row.checks.Add("Possession: her copy moved $moveC m, the host guard stayed within $maxGap m, host variants $($variants-join ',')")
     if($moveC-lt2){$fails.Add("her possessed copy moved $moveC m")};if($maxGap-gt1.5){$fails.Add("the host guard was up to $maxGap m from her copy")};if($null-eq$gap-or$gap-gt.3){$fails.Add("the host guard ends $gap m from her copy")};if($variants-notcontains1000){$fails.Add('the host channel never took the possession variant')}}
    'blinkkick'{$downs=@($series|ForEach-Object {[int]$_.channel_host.downSent});$down=if($downs.Count){($downs|Measure-Object -Maximum).Maximum-($downs|Measure-Object -Minimum).Minimum}else{0};$row.checks.Add("Blink Assault: host down poses sent $down, host guard moved $moveH m")
     if(!$herEffect){$fails.Add("her kick did nothing to her copy (moved $moveC m)")};if(!($moveH-ge.3-or$down-gt0-or$hurtH-or$row.outcome.host-ne'alive')){$fails.Add('the host guard was not moved or knocked down')}}
    'farreach'{if($moveC-lt1){$fails.Add("her pull moved her copy $moveC m")};if($moveH-lt1){$fails.Add("the host guard moved $moveH m")}}
    'domino'{$row.checks.Add("Domino: guard 1 $($row.outcome.host)/$($row.outcome.client), guard 2 $($row.outcome2.host)/$($row.outcome2.client); marks $(($row.marks.Values|ForEach-Object {$_.marked})-join ',')");if($row.outcome.client-eq'alive'){$fails.Add('her takedown left the linked guard alive')}
     if(@($row.marks.Values|Where-Object {!$_.marked}).Count){$fails.Add('a guard was never marked')};if($row.outcome2.client-ne$row.outcome.client){$fails.Add("the second guard ends $($row.outcome2.client) in her world, the first $($row.outcome.client)")}
     if($row.outcome2.host-ne$row.outcome2.client){$fails.Add("the second linked guard ends $($row.outcome2.host) on the host and $($row.outcome2.client) in her world")}}
    'doppelganger'{$sp=(@($series|ForEach-Object {[int]$_.shadeHost.spawned})+@(0)|Measure-Object -Maximum).Maximum;$row.checks.Add("host puppets spawned $sp");if($sp-lt1){$fails.Add('the host spawned no puppet for her shade')}}
    'mesmerize'{$taken=@($series|Where-Object {@($_.mesmerizeHost.targetHandles|Where-Object {$_}).Count-gt0-or[int]$_.mesmerizeHost.targets-gt0});$row.checks.Add("host Mesmerize object took an NPC in $($taken.Count) samples");if(!$taken.Count){$fails.Add('the host Mesmerize object took no host NPC')}}
    'shadowwalk'{if($row.outcome.client-eq'alive'){$fails.Add('her shadow-form attack left her copy alive')}}}
   if($row.outcome.host-ne$row.outcome.client){$fails.Add("the guard ends $($row.outcome.host) on the host and $($row.outcome.client) in her world")}
   elseif($row.outcome.host-eq'alive'-and($null-eq$gap-or$gap-gt1.5)){$fails.Add("the live guard ends $gap m apart in the two worlds")}
   $row.checks.Add("Host guard moved up to $moveH m (health $($first.host.health) -> $($last.host.health)); her copy up to $moveC m (health $($first.client.health) -> $($last.client.health)); outcome $($row.outcome.host)/$($row.outcome.client), end gap $gap m (last samples $rawGap m)")
   if($fails.Count){throw ($fails-join '; ')};$row.passed=$true
  }catch{$row.failure=$_.Exception.Message;try{Save "$out/$pw-failure.json" @{message=$row.failure;stack=$_.ScriptStackTrace}}catch{}}
  finally{foreach($c in $caps){try{if(!$c.HasExited){Stop-Process -Id $c.Id}}catch{}};try{C host 'coop_observer_camera off'}catch{}}
  $rows.Add([pscustomobject]$row);Save "$out/rows.json" @($rows)}
 $failed=@($rows|Where-Object {!$_.passed});if($failed.Count){$failure="Failed: $(($failed|ForEach-Object {"$($_.power) ($($_.failure))"})-join '; ')"}
}catch{$failure=$_.Exception.Message;try{Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}catch{}}
finally{
 if($owned){foreach($r in 'host','client'){try{C $r 'coop_player_health_hold off';C $r 'coop_observer_camera off';C $r 'coop_product_auto off'}catch{}}
  $errors=[Collections.Generic.List[string]]::new()
  try{& "$root/work/movement-integration/Deploy-SourceClock.ps1"|Out-Null}catch{$errors.Add("restore: $($_.Exception.Message)")}
  foreach($r in 'host','client'){try{$s=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json;if($s.localGameLogicState-ne1-or!$s.remoteFresh){$errors.Add("$r not restored")}}catch{$errors.Add("$r status")}}
  try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}}catch{$errors.Add($_.Exception.Message)}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  [ordered]@{pid=$PID;operation='MissionPartnerPowers1816';take=$Take;ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
  $mutex.ReleaseMutex();$owned=$false}
 Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;rows=@($rows);scope="1816 (#180): the partner ($PartnerCharacter, host $HostCharacter) uses $($Powers-join ',') on the host's checkpoint guard on a mission map; the guard is sampled and filmed in both worlds"})
 $mutex.Dispose()}
if($failure){throw $failure};"PASS $HostCharacter-$PartnerCharacter $($Powers-join ',')"
