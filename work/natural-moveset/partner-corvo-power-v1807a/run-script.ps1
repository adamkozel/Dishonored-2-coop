param([ValidateSet('windblast','swarm','possession','blinkkick')][string]$Power='blinkkick',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='partner-corvo-power-v1807a',[ValidatePattern('^[a-z_]*$')][string]$Upgrade='',[string]$Candidate='candidate-npc-combat-v1214ij',[ValidateSet(80,81)][int]$HostSlot=81,[int]$SampleSeconds=12)
# 1807: Windblast and Swarm also wait for the guard's patrol pause, cast from the host guard's authoritative facing
# with her replica ready, and frame the host's camera on it (v1806a missed the walking guard; v1806c never cast).
# 1806 (#168): -Upgrade names a native power upgrade (components/characters/player/base/attributes/<name>.inventoryitem,
# for example windblast_greater, devouring_swarm_double), granted after the power. Recorded with the same samples.
# 1805 (#168): Blink Assault stands 6 m in front of the host's guard (its authoritative position and facing, as
# Record-Coverage blinkkick does), then waits up to 3 s for her replica to be ready within 0.3 m of it before she aims
# at the host's position: in partner-corvo-power-v1804a her stale replica's facing put her behind the guard, and her
# attack was an assassination, not the kick. The host's camera is framed on the guard from 4.5 m to its side (the
# coverage observer spot) for the visual review.
# 1804: the pause is read from the host's guard (authoritative), and her NPC channel for it must be a ready replica
# (client_replica) at the same spot (within 0.5 m): in partner-corvo-power-v1803a her channel for the guard never
# formed (host negotiating, then peer_timeout), her stale copy stood at the pause spot and passed the check at once,
# and her blink was never sent.
# 1803: Blink Assault waits for the guard at its long patrol pause, within 0.4 m of (-47.95, -152.45) and still for
# 600 ms, up to 150 s (Record-Coverage blinkkick). In partner-corvo-power-v1802b a 0.7 s stop at the start passed
# the still check, and the guard walked on before her kick.
# 1802 (1214ij): Blink Assault waits for the guard's patrol pause (it stands still 600 ms) before she aims, as
# Record-Coverage blinkkick does: in partner-corvo-power-v1801a the guard walked on and her kick hit nothing. Each
# sample records both channels' knockdown counters (the host's down poses sent, hers received). Judged for
# blinkkick: the host's guard goes down (its channel sends down poses) and her copy receives them.
# 1801 (#160): -Power blinkkick, Blink Assault: she gets Blink and blink_assault (as Record-Coverage blinkkick),
# stands 7 m in front of the guard, aims Blink at it for 1 s (left trigger) and kicks on arrival (right trigger 1.1 s
# after the release). Her blink effect stream (power-fx-stream) confirms the cast; both worlds' guard is sampled with
# the combat counters. Recorded, not yet judged: what of her kick reaches the host's guard.
# 1800: back to the 4 m front stand for every power (partner-corvo-power-v1799a: 6 m behind the guard is off the upper
# floor, 5.4 m lower, and a checkpoint barrier stopped the channels). The guard sees her there and the host follows her
# possessed body without the AI lease (host_paired_follow).
# 1799 (#160, 1214ig): her channel now survives her Possession. For possession she stands 6 m behind the guard: in
# v1798a she stood 4 m in front, the guard was alert and the host refused the AI lease (host_paired_follow).
# 1798 (#160, 1214if): her Possession of the guard is held on the host as a paired scene (her possessed body as reverse
# pose). Each sample also records both worlds' npc-stream channel for the guard (her localPaired; the host's paired
# variant, result, lease and reverse poses). Judged for possession: her copy walks at least 2 m, the host's guard stays
# within 0.75 m of her copy through the walk, and the host's channel takes the possession variant (1000).
# 1795: possessing a human needs the native human upgrade (possession_parasite, as native_player_possession_upgrade.h
# reads it; partner-corvo-power-v1794b showed Cancel Possession on the guard and never possessed): granted with the power.
# 1794 (#160, 1214id): partner-corvo-power-v1792c (1214id) cast no swarm (her swarm stream sent nothing; the guard had
# patrolled off the aim): each cast is now confirmed (swarm: her swarm stream sends; possession: her possession effect
# stream sends) and retried up to 3 times, re-aimed at the guard's current position; the host's swarm puppet status
# (swarm-host) is recorded.
# 1792 (#160): any Corvo power used by the partner on guard 2 (-Power windblast | swarm | possession): the baseline of what
# reaches the host. Swarm: the rats are summoned at the guard (left trigger tap) and both worlds are sampled for 12 s.
# Possession: she aims at the guard and holds the left trigger to possess it, then walks forward 3 s (the possessed
# body moves in her world); both worlds' guard are sampled. Windblast as v1791. Recorded, not yet judged.
# 1791: the pairing fixture now gets its sword (Enter-PairingFixture-v1790: LoadGame 80 then the native Mayhew scene;
# partner-windblast-v1787a and diag-partner-sword-v1789a: the SLOT80 checkpoint holds no sword, so the Corvo partner's
# shadow stream never armed and the presentation never went live).
# 1787 (#160): Corvo's Windblast used by the partner, on the host's NPC. Today a partner never plays Corvo, so this is the
# baseline of what reaches the host before any #160 code. On the local lab at the Dunwall fixture the partner loads
# SLOT80 (the host's Corvo checkpoint at the same spot, Prepare-PairingFixtures-v1784) and the host keeps its Corvo
# (-HostSlot 81) or loads Emily (-HostSlot 80). Under the product controller (avatars by character) the partner gets
# Windblast (coop_grant_named; the lab equip switches the hand), stands 4 m in front of the upper-floor tutorial guard 2
# facing it, and blasts it. For SampleSeconds the guard is sampled in both worlds (position, health) with the host's
# and her npc-combat counters (what was forwarded and applied). Both windows are filmed.
# Recorded, not yet judged: whether the host's guard moves, falls or loses health from her blast (the forwarded
# windblast_npc hit, chapter 108 #160 plan). The fixture is restored (both instances reload SLOT81) after the case.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null
$pids=@{};$inventory=@{};$inv=[Globalization.CultureInfo]::InvariantCulture;$row=[ordered]@{passed=$false;failure=$null;checks=[Collections.Generic.List[string]]::new()}
function Save($p,$v){$v|ConvertTo-Json -Depth 15|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$b='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}
function S($r,$suffix=''){for($a=0;$a-lt40;$a++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function Q($r,$mode,$arg=0){for($a=0;$a-lt8;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read failed $mode"}
function F($v){([double]$v).ToString('F5',$inv)}
function Guard($r){$g=@((Q $r '--world-npcs').npcs|Where-Object {$_.spawnerName-eq'ld_spawner_guard_tutorial_2'-and$_.unique-and$_.reciprocal-and$_.definition-eq'models/characters/def/generic/dunwall_guard_male_small_01.def'});if($g.Count-ne1){throw "Exact upper-floor guard2 absent ($r)"};$g[0]}
function Live($r){$s=S $r;$m=S $r '.movement.json';$x=S $r '.shadow-stream.json';$a=$s.probeRenderPosition;$b=$s.remotePosition
 $d=if($a-and$b){[Math]::Sqrt(($a[0]-$b[0])*($a[0]-$b[0])+($a[1]-$b[1])*($a[1]-$b[1])+($a[2]-$b[2])*($a[2]-$b[2]))}else{999}
 @{follow=$s.probeFollowEnabled;distance=[Math]::Round($d,3);product=(S $r '.product.json').result;bank=[bool]$m.active;stream=[bool]($x.armed-and$x.ready);ok=($s.probeActorHandle-and$s.probeFollowEnabled-and$d-lt1.0-and$m.active-and$x.armed-and$x.ready-and$s.remoteFresh)}}
function WaitLive($tag,$limit=30000){$w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 300;$p=@{host=(Live host);client=(Live client)};$ok=$p.host.ok-and$p.client.ok}while(!$ok-and$w.ElapsedMilliseconds-lt$limit);$p.ok=$ok;$p.waitMs=$w.ElapsedMilliseconds;Save "$out/live-$tag.json" $p;$p}
function Arm{foreach($r in 'host','client'){I $r;C $r 'coop_movement_fixture off';C $r 'coop_product_auto on'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$ok=(S host '.product.json').result-eq'armed'-and(S client '.product.json').result-eq'armed'}while(!$ok-and$w.ElapsedMilliseconds-lt60000)
 if(!$ok){throw 'Product controller not armed'}}
function Disarm{$cw=[Diagnostics.Stopwatch]::StartNew();try{C host 'coop_npc_combat fixture_off'}catch{};try{C host 'coop_mesmerize_native follow_on'}catch{}
 do{$off=$true;foreach($r in 'host','client'){try{I $r;C $r 'coop_player_health_hold off';C $r 'coop_observer_camera off';C $r 'coop_product_auto off';C $r 'coop_movement_resume off';foreach($cmd in 'coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off','coop_shadow_stream off','coop_movement_bank off','coop_trigger_mirror off'){C $r $cmd};C $r 'coop_checkpoint_sync off'
   if((S $r '.reset.json').enabled){$off=$false;$st=S $r;if($st.localGameLogicState-in8,15){I $r A 300}elseif($st.localGameLogicState-eq2){I $r B 200}}}catch{}}
  if(!$off){Start-Sleep -Milliseconds 1500}}while(!$off-and$cw.ElapsedMilliseconds-lt60000)
 foreach($r in 'host','client'){try{if((S $r).localGameLogicState-in2,12){I $r B 200;Start-Sleep -Milliseconds 800}}catch{}}}
function Restore($tag){Disarm
 & "$root/work/PrepareShadowSword.ps1" *>> "$out/checkpoint-restore.log"
 & "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$PSScriptRoot/native-coin-reward-v786c" -Evidence "$out/note-restoration-$tag" *>> "$out/note-restoration.log"
 foreach($r in 'host','client'){$restored=Q $r '--player-inventory'
  if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($restored.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw "Checkpoint inventory differs ($r)"}
  if((Q $r '--health' 1).current-ne56){throw "Restored player health differs ($r)"}}}
$aim={param($target,$stand)
 C client ('setviewpos {0} {1} {2} 90 0' -f (F $stand[0]),(F $stand[1]),(F ([double]$stand[2]+1.7)));Start-Sleep -Milliseconds 700
 $pl=Q client '--player';$me=$pl.candidatePosition;$eye=[double]$pl.nativeViewPosition[2]
 $yaw=[Math]::Atan2([double]$target[1]-[double]$me[1],[double]$target[0]-[double]$me[0])*180/[Math]::PI;$h=[Math]::Max(.5,[Math]::Sqrt([Math]::Pow([double]$target[0]-[double]$me[0],2)+[Math]::Pow([double]$target[1]-[double]$me[1],2)))
 $pitch=[Math]::Max(-60,[Math]::Min(60,[Math]::Atan2($eye-([double]$target[2]+1.3),$h)*180/[Math]::PI))
 C client ('setviewpos {0} {1} {2} {3} 0' -f (F $me[0]),(F $me[1]),(F ([double]$me[2]+1.7)),(F $yaw));Start-Sleep -Milliseconds 300;& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch $pitch -Quick|Out-Null
 [ordered]@{target=$target;stand=$me;yaw=$yaw;pitch=$pitch;distance=$h}}
$caps=@()
$caps=@()
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Another native operation remains active'}
 if(Test-Path -LiteralPath $out){throw 'Unique evidence take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 [ordered]@{pid=$PID;operation='PartnerCorvoPower1807';take=$Take;started=(Get-Date).ToString('o');state='running'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;$dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;if($dll-ne$pin.dll){throw "Expected $Candidate"}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid;I $r
  if((S $r).localGameLogicState-ne1){throw "$r not in gameplay"}
  $inventory[$r]=Q $r '--player-inventory';Save "$out/inventory-before-$r.json" $inventory[$r]
 }
 Save "$out/identity.json" ([ordered]@{candidate=$Candidate;dll=$dll;probe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash})
 $dirty=$true
 # The pairing: the partner loads Corvo (SLOT80); the host keeps or swaps its character.
 $loads=@('client');if($HostSlot-eq80){$loads+='host'}
 foreach($r in $loads){& "$root/work/release/Enter-PairingFixture-v1790.ps1" -Role $r -Slot 80 -EvidencePath "$out/fixture-$r.json"|Out-Null}
 Start-Sleep 3;$row.definitions=[ordered]@{host=(Q host '--character').definition;client=(Q client '--character').definition}
 if("$($row.definitions.client)"-notmatch'/corvo/'){throw "The partner does not play Corvo ($($row.definitions.client))"}
 Arm
 foreach($q in 'host','client'){C $q 'setviewpos -33 -153 68.1 90 0'}
 $live=WaitLive 'armed' 45000;if(!$live.ok){throw 'Partner presentation not live after arming'}
 $row.characters=[ordered]@{host=(S host '.character.json');client=(S client '.character.json')}
 C client 'coop_player_health_hold on';C host 'coop_player_health_hold on'
 $item=@{windblast='models/powers/windblast/windblast.inventoryitem';swarm='models/powers/devouring_swarm/devouring_swarm.inventoryitem';possession='models/powers/possession/possession.inventoryitem';blinkkick='models/powers/blink/blink.inventoryitem'}[$Power]
 C client "coop_grant_named $item"
 if($Power-eq'possession'){Start-Sleep -Milliseconds 1500;C client 'coop_grant_named components/characters/player/base/attributes/possession_parasite.inventoryitem'}
 if($Power-eq'blinkkick'){Start-Sleep -Milliseconds 1500;C client 'coop_grant_named components/characters/player/base/attributes/blink_assault.inventoryitem'}
 if($Upgrade){Start-Sleep -Milliseconds 1500;C client "coop_grant_named components/characters/player/base/attributes/$Upgrade.inventoryitem";$row.upgrade=$Upgrade}
 $e=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 400;$gr=try{(S client '.observer-camera.json').itemGrant.named}catch{$null}}while($e.ElapsedMilliseconds-lt4000-and"$($gr.result)"-notmatch'equip_requested|timeout');$row.grant=$gr
 if($Power-in'blinkkick','windblast','swarm'){$bw=[Diagnostics.Stopwatch]::StartNew();$still=$false
  do{$b0=Guard host;$atPause=[Math]::Sqrt([Math]::Pow([double]$b0.position[0]+47.95,2)+[Math]::Pow([double]$b0.position[1]+152.45,2))-lt.4
   if($atPause){$cg0=Guard client;$cch=try{@((S client '.npc-stream.json').entries|Where-Object {$_.handle-eq$cg0.handle})|Select-Object -First 1}catch{$null}
    $atPause=$cch-and$cch.result-eq'client_replica'-and[Math]::Sqrt([Math]::Pow([double]$cg0.position[0]-[double]$b0.position[0],2)+[Math]::Pow([double]$cg0.position[1]-[double]$b0.position[1],2))-lt.5}
   if($atPause){Start-Sleep -Milliseconds 600;$b1=Guard host;$bd=[Math]::Sqrt([Math]::Pow([double]$b1.position[0]-[double]$b0.position[0],2)+[Math]::Pow([double]$b1.position[1]-[double]$b0.position[1],2));if($bd-lt.03){$still=$true}}
   else{Start-Sleep -Milliseconds 500}}while(!$still-and$bw.ElapsedMilliseconds-lt150000)
  if(!$still){throw 'Guard 2 never stood still'};$row.guardWaitMs=$bw.ElapsedMilliseconds}
 $g=Guard host;$cg=Guard client;$row.guardHandleHost=$g.handle;$row.guardHandleClient=$cg.handle
 if($Power-in'blinkkick','windblast','swarm'){$cg=$g;$bgx=[double]$g.position[0];$bgy=[double]$g.position[1];$bfx=[double]$g.orientation[0];$bfy=[double]$g.orientation[1];$bfl=[Math]::Max(1e-3,[Math]::Sqrt($bfx*$bfx+$bfy*$bfy));$bfx/=$bfl;$bfy/=$bfl
  $hx=$bgx+1.0*$bfx+4.5*$bfy;$hy=$bgy+1.0*$bfy-4.5*$bfx;C host ('setviewpos {0} {1} {2} {3} 0' -f (F $hx),(F $hy),(F ([double]$g.position[2]+1.9)),(F ([Math]::Atan2($bgy-$hy,$bgx-$hx)*180/[Math]::PI)));$row.hostCamera=@($hx,$hy)}
 $cfx=[double]$cg.orientation[0];$cfy=[double]$cg.orientation[1];$reach=if($Power-eq'blinkkick'){6}else{4};$stand=@(([double]$cg.position[0]+$reach*$cfx),([double]$cg.position[1]+$reach*$cfy),[double]$cg.position[2])
 $row.castAim=(& $aim $cg.position $stand);Start-Sleep -Milliseconds 800
 if($Power-in'blinkkick','windblast','swarm'){$rw=[Diagnostics.Stopwatch]::StartNew();$ready=$false
  do{$hg=Guard host;$cg2=Guard client;$cch=try{@((S client '.npc-stream.json').entries|Where-Object {$_.handle-eq$cg2.handle})|Select-Object -First 1}catch{$null}
   $ready=$cch-and$cch.result-eq'client_replica'-and[Math]::Sqrt([Math]::Pow([double]$cg2.position[0]-[double]$hg.position[0],2)+[Math]::Pow([double]$cg2.position[1]-[double]$hg.position[1],2))-lt.3
   if(!$ready){Start-Sleep -Milliseconds 200}}while(!$ready-and$rw.ElapsedMilliseconds-lt3000)
  $row.replicaReady=[ordered]@{ready=$ready;ms=$rw.ElapsedMilliseconds};if(!$ready){throw 'her replica of the guard is not ready at the host guard'}
  $row.castAim=(& $aim $hg.position $stand)}
 foreach($pair in @(@('client','source'),@('host','observer'))){$hwnd=(Get-Process -Id $pids[$pair[0]]).MainWindowHandle.ToInt64()
  $caps+=Start-Process (Get-Command ffmpeg).Source -ArgumentList @('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',"$($SampleSeconds+6)",'-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-movflags','+frag_keyframe+empty_moov+default_base_moof',"$out/$($pair[1]).mp4") -WindowStyle Hidden -RedirectStandardError "$out/$($pair[1])-capture.log" -PassThru}
 Start-Sleep 2
 $before=[ordered]@{host=[ordered]@{position=$g.position;health=(Q host '--health' $g.handle).current;combat=(S host '.npc-combat.json')};client=[ordered]@{position=$cg.position;health=(Q client '--health' $cg.handle).current;combat=(S client '.npc-combat.json')}}
 $row.before=$before
 $proof=@{swarm='.swarm-world.json';possession='.possession-fx.json';windblast='.windblast-fx.json';blinkkick='.power-fx-stream.json'}[$Power];$row.attempts=@()
 for($attempt=1;$attempt-le3;$attempt++){$s0=[int](S client $proof).sent
  if($attempt-gt1){$cg=Guard client;$cfx=[double]$cg.orientation[0];$cfy=[double]$cg.orientation[1];$stand=@(([double]$cg.position[0]+$reach*$cfx),([double]$cg.position[1]+$reach*$cfy),[double]$cg.position[2]);$null=(& $aim $cg.position $stand);Start-Sleep -Milliseconds 600}
  switch($Power){
   'windblast'{& "$lab/Send-D2Input.ps1" -Role client -LeftTrigger 255 -DurationMs 350|Out-Null}
   'swarm'{& "$lab/Send-D2Input.ps1" -Role client -LeftTrigger 255 -DurationMs 600|Out-Null}
   'possession'{& "$lab/Send-D2Input.ps1" -Role client -LeftTrigger 255 -DurationMs 1500|Out-Null}
   'blinkkick'{& "$lab/Send-D2Input.ps1" -Role client -LeftTrigger 255 -DurationMs 1000|Out-Null;Start-Sleep -Milliseconds 1120;& "$lab/Send-D2Input.ps1" -Role client -RightTrigger 255 -DurationMs 250|Out-Null;$row.kickAt=(Get-Date).ToString('o')}}
  Start-Sleep 3;$s1=[int](S client $proof).sent;$row.attempts+=[ordered]@{attempt=$attempt;sentBefore=$s0;sentAfter=$s1}
  if($s1-gt$s0){break}}
 if($Power-eq'possession'){& "$lab/Send-D2Input.ps1" -Role client -LeftY 32000 -DurationMs 3000|Out-Null}
 $row.castAt=(Get-Date).ToString('o')
 $series=[Collections.Generic.List[object]]::new();$sw=[Diagnostics.Stopwatch]::StartNew()
 while($sw.ElapsedMilliseconds-lt($SampleSeconds*1000)){$sample=[ordered]@{ms=$sw.ElapsedMilliseconds}
  foreach($r in 'host','client'){$h=if($r-eq'host'){$g.handle}else{$cg.handle};$n=@((Q $r '--world-npcs').npcs|Where-Object {$_.handle-eq$h})|Select-Object -First 1
   $sample[$r]=[ordered]@{position=$n.position;health=$(try{(Q $r '--health' $h).current}catch{$null})}}
  $sample.swarmHost=$(try{$q=S host '.swarm-host.json';[ordered]@{alive=$q.alive;result=$q.result;spawned=$q.spawned}}catch{$null})
  foreach($r in 'host','client'){$h=if($r-eq'host'){$g.handle}else{$cg.handle};$ch=try{@((S $r '.npc-stream.json').entries|Where-Object {$_.handle-eq$h})|Select-Object -First 1}catch{$null}
   $sample["channel_$r"]=$(if($ch){[ordered]@{localPaired=$ch.localPaired;pairedVariant=$ch.pairedVariant;pairedResult=$ch.pairedResult;hostPairedLease=$ch.hostPairedLease;reversePoses=$ch.reversePoses;presentedRoot=$ch.presentedRoot;downSent=$ch.downSent;downReceived=$ch.downReceived;result=$ch.result}}else{$null})}
  $series.Add([pscustomobject]$sample);Start-Sleep -Milliseconds 250}
 Save "$out/cast-series.json" @($series)
 foreach($q in 'host','client'){foreach($suffix in '.npc-combat.json','.npc-stream.json','.windblast-fx.json','.character.json','.swarm-world.json','.possession-fx.json','.swarm-host.json'){try{Copy-Item -LiteralPath "$root/work/lab-instances/$q/coop-status.json$suffix" -Destination "$out/after-$q$suffix" -Force}catch{}}}
 foreach($c in $caps){try{if(!$c.WaitForExit(30000)){Stop-Process -Id $c.Id}}catch{}}
 $dist={param($a,$b)if(!$a-or!$b){return $null};[Math]::Round([Math]::Sqrt([Math]::Pow([double]$a[0]-[double]$b[0],2)+[Math]::Pow([double]$a[1]-[double]$b[1],2)),2)}
 $last=$series[$series.Count-1]
 $hostMove=(@($series|ForEach-Object{& $dist $before.host.position $_.host.position}|Where-Object{$_-ne$null})|Measure-Object -Maximum).Maximum
 $clientMove=(@($series|ForEach-Object{& $dist $before.client.position $_.client.position}|Where-Object{$_-ne$null})|Measure-Object -Maximum).Maximum
 $row.summary=[ordered]@{hostGuardMaxMove=$hostMove;herCopyMaxMove=$clientMove;hostHealth=@($before.host.health,$last.host.health);herHealth=@($before.client.health,$last.client.health);grant=$row.grant.result}
 if($Power-eq'blinkkick'){$hd=@($series|ForEach-Object {$_.channel_host.downSent}|Where-Object {$_-ne$null});$cd=@($series|ForEach-Object {$_.channel_client.downReceived}|Where-Object {$_-ne$null})
  $hostDown=if($hd.Count){($hd|Measure-Object -Maximum).Maximum-($hd|Measure-Object -Minimum).Minimum}else{0};$herDown=if($cd.Count){($cd|Measure-Object -Maximum).Maximum-($cd|Measure-Object -Minimum).Minimum}else{0}
  $row.blinkKick=[ordered]@{hostDownSent=$hostDown;herDownReceived=$herDown;herCombat=(S client '.npc-combat.json'|Select-Object localHits,requests,committed,applied,rejected,passedThrough,lastDecl)}
  $row.checks.Add("Blink Assault: host down poses sent $hostDown, hers received $herDown")
  if(!$hostDown){$row.checks.Add("Host guard moved up to $hostMove m; her copy up to $clientMove m");throw 'host guard never went down from her kick'}}
 if($Power-eq'possession'){$fails=[Collections.Generic.List[string]]::new()
  $gap=@($series|Where-Object {$_.host.position-and$_.client.position}|ForEach-Object {& $dist $_.host.position $_.client.position});$maxGap=($gap|Measure-Object -Maximum).Maximum
  $variants=@($series|ForEach-Object {$_.channel_host.pairedVariant}|Where-Object {$_}|Select-Object -Unique);$results=@($series|ForEach-Object {$_.channel_host.pairedResult}|Select-Object -Unique)
  $herPaired=@($series|Where-Object {$_.channel_client.localPaired}).Count
  $row.possession=[ordered]@{maxGap=$maxGap;gaps=$gap;hostVariants=$variants;hostResults=$results;herPairedSamples=$herPaired}
  if(!$clientMove-or$clientMove-lt2){$fails.Add("her possessed copy moved $clientMove m")}
  if($maxGap-eq$null-or$maxGap-gt0.75){$fails.Add("host guard up to $maxGap m from her copy")}
  if($variants-notcontains1000){$fails.Add('host channel never took the possession variant')}
  $row.checks.Add("Possession: host guard within $maxGap m of her copy; host variants $($variants -join ','), results $($results -join ','); her paired samples $herPaired")
  if($fails.Count){$row.checks.Add("Host guard moved up to $hostMove m; her copy up to $clientMove m");throw ($fails -join '; ')}}
 $row.checks.Add("Host guard moved up to $hostMove m (health $($before.host.health) -> $($last.host.health)); her copy moved up to $clientMove m (health $($before.client.health) -> $($last.client.health))")
 $row.passed=$true
}catch{$failure=$_.Exception.Message;$row.failure=$failure;if(Test-Path -LiteralPath $out){Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}}finally{
 foreach($c in $caps){if($c){try{if(!$c.HasExited){Stop-Process -Id $c.Id}}catch{}}}
 if($owned){
  if($dirty){try{Restore 'final';$row.restored=$true}catch{$row.restored=$false;$failure=($failure+'; restore: '+$_.Exception.Message).TrimStart('; ')}}
  $errors=[Collections.Generic.List[string]]::new()
  try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}}catch{$errors.Add($_.Exception.Message)}
  foreach($r in 'host','client'){try{$s=S $r;if($s.localGameLogicState-ne1-or!$s.remoteFresh-or(Q $r '--health' 1).current-ne56){$errors.Add("$r gameplay/transport/health not restored")}}catch{$errors.Add("$r status unavailable")}}
  if(Test-Path -LiteralPath $out){
   Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)}
   if($errors.Count-and!$failure){$failure=$errors-join '; '}
   Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;case=[pscustomobject]$row;scope="#160 baseline: a Corvo partner (lab SLOT80) uses $Power on the Dunwall tutorial guard 2 under the product controller; the guard is sampled in both worlds with the combat counters."})
  }
  [ordered]@{pid=$PID;operation='PartnerCorvoPower1807';take=$Take;ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
if($failure){throw $failure};'PASS_PARTNER_CORVO_POWER_BASELINE'
