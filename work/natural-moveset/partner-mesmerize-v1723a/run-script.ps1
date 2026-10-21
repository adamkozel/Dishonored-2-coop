param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='partner-mesmerize-v1723a',[string]$Candidate='candidate-npc-combat-v1214gn',[int]$SampleSeconds=30)
# 1723 (#131, stage 4): Emily's own Mesmerize mesmerizes the host's NPCs. Candidate 1214gn makes the host run the native
# Mesmerize object for her cast (serviceMesmerizeHostFollow: her world stream drives the host object proven in
# mesmerize-host-v1722b). Under the product controller (both lab peers armed), Emily gets Mesmerize with its in-combat
# upgrade, casts it at the upper-floor tutorial guard 2 from 5 m behind it (LeftTrigger 1200 ms, as in carry-body-v1658a,
# where only her world's copy was taken), and for SampleSeconds both worlds are sampled: guard 2 (position, health), the
# host object (mesmerize-host.json: state, targets, the follow record), the host's view of her stream and her own
# native targets. Both windows are filmed; the host camera is held behind guard 2.
# Pass (runtime): the host object follows her cast (begun, state 3 requested), the host's native target list holds the
# host's guard 2 (or another host NPC), and after her spirit ends the host object is back in state 0 with the phantom
# power slot cleared. Fixture restored after the case.
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
function Disarm{$cw=[Diagnostics.Stopwatch]::StartNew();try{C host 'coop_npc_combat fixture_off'}catch{}
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
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Another native operation remains active'}
 if(Test-Path -LiteralPath $out){throw 'Unique evidence take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 [ordered]@{pid=$PID;operation='PartnerMesmerize1723';take=$Take;started=(Get-Date).ToString('o');state='running'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;$dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;if($dll-ne$pin.dll){throw "Expected $Candidate"}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid;I $r
  if((S $r).localGameLogicState-ne1){throw "$r not in gameplay"}
  $inventory[$r]=Q $r '--player-inventory';Save "$out/inventory-before-$r.json" $inventory[$r]
 }
 Save "$out/identity.json" ([ordered]@{candidate=$Candidate;dll=$dll;probe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash})
 Save "$out/host-before.mesmerize-host.json" (S host '.mesmerize-host.json')
 $dirty=$true;Arm
 foreach($q in 'host','client'){C $q 'setviewpos -33 -153 68.1 90 0'}
 $live=WaitLive 'armed' 45000;if(!$live.ok){throw 'Partner presentation not live after arming'}
 # The host camera held behind the host's guard 2, along its facing (carry-body framing); the host player then returns.
 $g=Guard host;$row.guardHandleHost=$g.handle;$fx=[double]$g.orientation[0];$fy=[double]$g.orientation[1];$look=[Math]::Atan2($fy,$fx)*180/[Math]::PI;$picked=$null
 foreach($back in 4.5,3.5,5.5){$tx=[double]$g.position[0]-$back*$fx;$ty=[double]$g.position[1]-$back*$fy
  C host ('setviewpos {0} {1} {3} {2} 0' -f (F $tx),(F $ty),$look.ToString('F4',$inv),(F ([double]$g.position[2]+1.7)));Start-Sleep -Milliseconds 650;$feet=(Q host '--player-states').position
  if([math]::Abs($feet[2]-$g.position[2])-lt.3-and[math]::Abs($feet[0]-$tx)-lt.4-and[math]::Abs($feet[1]-$ty)-lt.4){$picked=$back;$ax=$tx;$ay=$ty;break}}
 $row.filmed=$null-ne$picked
 if($row.filmed){& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch 18 -Quick|Out-Null
  C host ('setviewpos {0} {1} {3} {2} 0' -f (F $ax),(F $ay),$look.ToString('F4',$inv),(F ([double]$g.position[2]+2.6)));Start-Sleep -Milliseconds 120
  C host 'coop_observer_camera hold';Start-Sleep -Milliseconds 300;Save "$out/host-observer-camera.json" (S host '.observer-camera.json')}
 C host 'setviewpos -33 -153 68.1 90 0';Start-Sleep -Milliseconds 400
 $cg=Guard client;$row.guardHandleClient=$cg.handle;$row.guardBefore=@{host=$g.position;client=$cg.position}
 C client 'coop_player_health_hold on'
 $row.granted=@();foreach($item in 'models/powers/mesmerize/mesmerize.inventoryitem','components/characters/player/base/attributes/mesmerize_incombat.inventoryitem'){C client "coop_grant_named $item";Start-Sleep -Milliseconds 1200
  $row.granted+=[pscustomobject]@{item=$item;result=$(try{(S client '.observer-camera.json').itemGrant.named.result}catch{$null})}}
 foreach($pair in @(@('client','source'),@('host','observer'))){$hwnd=(Get-Process -Id $pids[$pair[0]]).MainWindowHandle.ToInt64()
  $caps+=Start-Process (Get-Command ffmpeg).Source -ArgumentList @('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',"$($SampleSeconds+8)",'-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/cast-$($pair[1]).mp4") -WindowStyle Hidden -RedirectStandardError "$out/cast-$($pair[1])-capture.log" -RedirectStandardOutput "$out/cast-$($pair[1])-capture.stdout" -PassThru}
 $cg=Guard client;$cfx=[double]$cg.orientation[0];$cfy=[double]$cg.orientation[1];$stand=@(([double]$cg.position[0]-5*$cfx),([double]$cg.position[1]-5*$cfy),[double]$cg.position[2])
 $row.castAim=(& $aim $cg.position $stand);Start-Sleep -Milliseconds 800
 & "$lab/Send-D2Input.ps1" -Role client -LeftTrigger 255 -DurationMs 1200|Out-Null;$row.castAt=(Get-Date).ToString('o')
 $series=[Collections.Generic.List[object]]::new();$w=[Diagnostics.Stopwatch]::StartNew()
 while($w.ElapsedMilliseconds-lt($SampleSeconds*1000)){$sample=[ordered]@{ms=$w.ElapsedMilliseconds}
  foreach($r in 'host','client'){$n=try{Guard $r}catch{$null};$sample[$r]=$(if($n){$hh=try{Q $r '--health' $n.handle}catch{$null};[ordered]@{handle=$n.handle;position=$n.position;current=$hh.current}}else{$null})}
  $mh=try{S host '.mesmerize-host.json'}catch{$null}
  $sample.hostObject=$(if($mh){[ordered]@{state=$mh.state;targets=$mh.targets;targetHandles=$mh.targetHandles;candidates=$mh.candidates;activeTimer=$mh.activeTimer;phantomSet=$mh.phantomSet;anchor=$mh.anchor;follow=$mh.follow;result=$mh.result}}else{$null})
  $sample.hostStream=try{$x=S host '.mesmerize-world-stream.json';[ordered]@{state=$x.state;event=$x.event;leased=$x.leased;presented=$x.presented}}catch{$null}
  $sample.clientTargets=try{$t=& "$lab/bin/d2_runtime_probe.exe" $pids['client'] '--mesmerize-targets'|ConvertFrom-Json;[ordered]@{worldState=$t.worldState;targets=@($t.targets|ForEach-Object {$_.handle})}}catch{$null}
  $series.Add([pscustomobject]$sample);Save "$out/cast-series.json" @($series);Start-Sleep -Milliseconds 300}
 # After her spirit: the host object back in state 0 and the phantom slot cleared (1214gm clears it 3 s after the stop).
 $tail=[Collections.Generic.List[object]]::new();$tw=[Diagnostics.Stopwatch]::StartNew()
 do{$mh=S host '.mesmerize-host.json';$tail.Add([pscustomobject][ordered]@{ms=$tw.ElapsedMilliseconds;state=$mh.state;phantomSet=$mh.phantomSet;follow=$mh.follow});if($mh.state-eq0-and!$mh.phantomSet-and$mh.follow.ended){break};Start-Sleep -Milliseconds 500}while($tw.ElapsedMilliseconds-lt25000)
 Save "$out/after-series.json" @($tail);Save "$out/host-after.mesmerize-host.json" (S host '.mesmerize-host.json')
 foreach($q in 'host','client'){foreach($suffix in '.npc-stream.json','.npc-combat.json','.mesmerize-world-stream.json','.mesmerize-partner-filter.json','.mesmerize-host.json'){try{Copy-Item -LiteralPath "$root/work/lab-instances/$q/coop-status.json$suffix" -Destination "$out/cast-$q$suffix" -ErrorAction Stop}catch{}}}
 foreach($c in $caps){try{if(!$c.WaitForExit(30000)){Stop-Process -Id $c.Id}}catch{}}
 $end=$tail[$tail.Count-1];$fol=$end.follow
 $hostTargetSamples=@($series|Where-Object {$_.hostObject-and[int]$_.hostObject.targets-gt0})
 $guardTaken=@($hostTargetSamples|Where-Object {@($_.hostObject.targetHandles)-contains[uint32]$row.guardHandleHost})
 $active=@($series|Where-Object {$_.hostObject-and$_.hostObject.state-eq3})
 $move={param($r,$set)$a=@($set|Where-Object {$_.$r});if($a.Count-lt2){return $null};$p=$a[0].$r.position;$q=$a[$a.Count-1].$r.position;[Math]::Round([Math]::Sqrt([Math]::Pow([double]$q[0]-[double]$p[0],2)+[Math]::Pow([double]$q[1]-[double]$p[1],2)),2)}
 $row.summary=[ordered]@{followBegins=$fol.begins;followForced=$fol.forced;followStops=$fol.stops;followRefused=$fol.refused;followResult=$fol.result;anchor=$fol.anchor;anchorDistance=$fol.anchorDistance
  activeSamples=$active.Count;hostTargetSamples=$hostTargetSamples.Count;guardTakenSamples=$guardTaken.Count;maxHostTargets=($series|ForEach-Object {[int]$_.hostObject.targets}|Measure-Object -Maximum).Maximum
  clientTargetSamples=@($series|Where-Object {@($_.clientTargets.targets).Count-gt0}).Count
  guardMoveWhileActive=[ordered]@{host=(& $move 'host' $active);client=(& $move 'client' $active)};endState=$end.state;endPhantom=$end.phantomSet;endEnded=$fol.ended}
 $row.checks.Add("Host follow: begins $($fol.begins), state 3 requested $($fol.forced), stops $($fol.stops), refused $($fol.refused) (result $($fol.result), anchor $($fol.anchor) at $($fol.anchorDistance) m)")
 $row.checks.Add("Host object in state 3 in $($active.Count) samples; host targets in $($hostTargetSamples.Count) samples (max $($row.summary.maxHostTargets)); host guard 2 taken in $($guardTaken.Count); her own targets in $($row.summary.clientTargetSamples)")
 $row.checks.Add("Guard 2 moved while the host object was active: host $($row.summary.guardMoveWhileActive.host) m, her copy $($row.summary.guardMoveWhileActive.client) m")
 if([int]$fol.begins-lt1-or[int]$fol.forced-lt1){throw "The host object did not follow her cast (begins $($fol.begins), forced $($fol.forced), result $($fol.result))"}
 if(!$hostTargetSamples.Count){throw 'The host object took no host NPC'}
 if($end.state-ne0-or$end.phantomSet){throw "After her spirit the host object is in state $($end.state), phantom slot set $($end.phantomSet)"}
 $row.checks.Add('After her spirit: host object in state 0, phantom slot cleared');$row.passed=$true
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
   Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;case=[pscustomobject]$row;scope='Stage 4 partner Mesmerize (#131): Emily casts her own Mesmerize at the Dunwall tutorial guard 2 under the product controller; the host runs the native Mesmerize object for her cast, takes host NPCs and ends with her spirit; fixture restored.'})
  }
  [ordered]@{pid=$PID;operation='PartnerMesmerize1723';take=$Take;ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
if($failure){throw $failure};'PASS_PARTNER_MESMERIZE'
