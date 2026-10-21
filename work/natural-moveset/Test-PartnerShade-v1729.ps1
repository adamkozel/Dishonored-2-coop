param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='partner-shade-v1729a',[string]$Candidate='candidate-npc-combat-v1214gt',[int]$SampleSeconds=30,[switch]$Control,[string[]]$Upgrades=@())
# 1729 (#131): the shade upgrades. -Upgrades is split on commas (pwsh -File passes a,b as one string). Her clone's
# definition is recorded when it appears (--faction), every second sample adds both guards' health in both worlds, and
# both peers' NPC combat counters are saved before and after the cast.
$Upgrades=@($Upgrades|ForEach-Object {$_-split','}|Where-Object {$_})
# 1728 (#131): the shade is cast in front of guard 1 (aim 4 m ahead of it from 6 m behind; 1727c cast at its back), both
# guards' native attention is sampled (--ai-targets: combat target and attention list) and checked for the puppet, and
# the counters are taken as deltas from the start of the take (1727d judged the cumulative spawn counter). Pass needs
# the puppet in a host guard's attention (or its health falling); the control needs no puppet.
# 1727 (#131, stage 4): Emily's own Doppelganger shade in the host world. Candidate 1214gr spawns the native shade
# (models/powers/doppelganger/decoy_entity.def) on the host at her streamed shade, posts the native clone stimulus, keeps
# it at her shade and removes it when hers ends (native_shade_host.h). Under the product controller Emily gets the
# Doppelganger (with -Upgrades: doppelganger_deadlyshade, doppelganger_baffling_shade), stands 6 m behind the lower-floor
# tutorial guard 1 and casts at it (LeftTrigger 800 ms, as in carry-body-v1658b, where the host NPCs never perceived her
# shade). For SampleSeconds both worlds are sampled: the host puppet (shade-host.json, its health), her shade (her
# Doppelganger stream, its health), both tutorial guards (position, health) in both worlds. Both windows are filmed, the
# host camera held beyond her shade from the fixture spot. -Control casts with the host puppet off.
# Pass (runtime): the host spawned the puppet at her shade and removed it when hers ended (or it was lost natively); a
# perception sign is recorded (the puppet's health fell, or a host guard came within 3 m of it). Control: no puppet.
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
function Disarm{$cw=[Diagnostics.Stopwatch]::StartNew();try{C host 'coop_npc_combat fixture_off'}catch{};try{C host 'coop_shade_native follow_on'}catch{}
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
 [ordered]@{pid=$PID;operation='PartnerShade1729';take=$Take;started=(Get-Date).ToString('o');state='running'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;$dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;if($dll-ne$pin.dll){throw "Expected $Candidate"}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid;I $r
  if((S $r).localGameLogicState-ne1){throw "$r not in gameplay"}
  $inventory[$r]=Q $r '--player-inventory';Save "$out/inventory-before-$r.json" $inventory[$r]
 }
 Save "$out/identity.json" ([ordered]@{candidate=$Candidate;dll=$dll;probe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash})
 $before=S host '.shade-host.json';Save "$out/host-before.shade-host.json" $before
 $dirty=$true;Arm
 foreach($q in 'host','client'){C $q 'setviewpos -33 -153 68.1 90 0'}
 $live=WaitLive 'armed' 45000;if(!$live.ok){throw 'Partner presentation not live after arming'}
 function U($v){if($v-is[string]-and$v.StartsWith('0x')){[Convert]::ToUInt32($v.Substring(2),16)}else{[uint32]$v}}
 function Npc($list,$spawner){@($list|Where-Object {$_.spawnerName-eq$spawner-and$_.unique-and$_.reciprocal})|Select-Object -First 1}
 function Dist($a,$b){if(!$a-or!$b){return $null};[Math]::Round([Math]::Sqrt([Math]::Pow([double]$a[0]-[double]$b[0],2)+[Math]::Pow([double]$a[1]-[double]$b[1],2)),2)}
 if($Control){C host 'coop_shade_native follow_off';Start-Sleep -Milliseconds 300;$row.control=$true;$row.followEnabled=(S host '.shade-host.json').enabled}
 foreach($q in 'host','client'){try{Copy-Item -LiteralPath "$root/work/lab-instances/$q/coop-status.json.npc-combat.json" -Destination "$out/before-$q.npc-combat.json" -ErrorAction Stop}catch{}}
 C client 'coop_player_health_hold on'
 $row.granted=@();foreach($item in @('models/powers/doppelganger/doppelganger.inventoryitem')+@($Upgrades|ForEach-Object {"components/characters/player/base/attributes/$_.inventoryitem"})){C client "coop_grant_named $item";Start-Sleep -Milliseconds 1200
  $row.granted+=[pscustomobject]@{item=$item;result=$(try{(S client '.observer-camera.json').itemGrant.named.result}catch{$null})}}
 foreach($pair in @(@('client','source'),@('host','observer'))){$hwnd=(Get-Process -Id $pids[$pair[0]]).MainWindowHandle.ToInt64()
  $caps+=Start-Process (Get-Command ffmpeg).Source -ArgumentList @('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',"$($SampleSeconds+8)",'-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/cast-$($pair[1]).mp4") -WindowStyle Hidden -RedirectStandardError "$out/cast-$($pair[1])-capture.log" -RedirectStandardOutput "$out/cast-$($pair[1])-capture.stdout" -PassThru}
 # Stand 6 m behind the lower-floor guard 1 (opposite its facing) and aim at it.
 $g1=Npc @((Q client '--world-npcs').npcs) 'ld_spawner_guard_tutorial_1';if(!$g1){throw 'Lower-floor guard 1 absent'};$row.guard1Before=$g1.position
 $fx=[double]$g1.orientation[0];$fy=[double]$g1.orientation[1];$stand=@(([double]$g1.position[0]-6*$fx),([double]$g1.position[1]-6*$fy),[double]$g1.position[2])
 $ahead=@(([double]$g1.position[0]+4*$fx),([double]$g1.position[1]+4*$fy),[double]$g1.position[2]);$row.castAt4m=$ahead
 $row.castAim=(& $aim $ahead $stand);Start-Sleep -Milliseconds 800
 & "$lab/Send-D2Input.ps1" -Role client -LeftTrigger 255 -DurationMs 800|Out-Null;$row.castAt=(Get-Date).ToString('o')
 $series=[Collections.Generic.List[object]]::new();$w=[Diagnostics.Stopwatch]::StartNew()
 while($w.ElapsedMilliseconds-lt($SampleSeconds*1000)){$sample=[ordered]@{ms=$w.ElapsedMilliseconds}
  $sh=try{S host '.shade-host.json'}catch{$null};$dw=try{S client '.doppelganger-world.json'}catch{$null}
  $slot=if($sh){$sh.slots[0]}else{$null};$sample.host=$(if($sh){[ordered]@{result=$sh.result;spawned=$sh.spawned;removed=$sh.removed;lost=$sh.lost;pins=$sh.pins;stimuli=$sh.stimuli;slot=$slot}}else{$null})
  $sample.puppetHealth=$(if($slot-and$slot.puppet-and$slot.handle){try{$h=Q host '--health' $slot.handle;[ordered]@{current=$h.current;maximum=$h.maximum}}catch{$null}}else{$null})
  $sample.her=$(if($dw){[ordered]@{present=$dw.present;handle=$dw.nativeHandle;event=$dw.event;transform=@($dw.transform|Select-Object -First 3)}}else{$null})
  $sample.herHealth=$(if($dw-and$dw.present-and$dw.nativeHandle){try{$h=Q client '--health' $dw.nativeHandle;[ordered]@{current=$h.current;maximum=$h.maximum}}catch{$null}}else{$null})
  if($sample.her-and$sample.her.present-and$sample.her.handle-and!$row.herDefinition){$row.herDefinition=try{(Q client '--faction' $sample.her.handle).definition}catch{"unread: $($_.Exception.Message)"};$row.herHandle=$sample.her.handle}
  $npcs=@{};foreach($r in 'host','client'){$npcs[$r]=try{@((Q $r '--world-npcs').npcs)}catch{@()}}
  foreach($sp in 'ld_spawner_guard_tutorial_1','ld_spawner_guard_tutorial_2'){$k=$sp.Substring($sp.Length-1)
   $a=Npc $npcs.host $sp;$b=Npc $npcs.client $sp;$att=$(if($a){try{$x=Q host '--ai-targets' (U $a.handle);[ordered]@{combat=$x.combat.handle;attention=@($x.attention|ForEach-Object {$_.handle});perceived=@($x.attention|Where-Object {$_.perceived}|ForEach-Object {$_.handle})}}catch{$null}}else{$null})
   $sample["attention$k"]=$att;if($series.Count%2-eq0){$sample["health$k"]=[ordered]@{host=$(if($a){try{(Q host '--health' (U $a.handle)).current}catch{$null}}else{$null});client=$(if($b){try{(Q client '--health' (U $b.handle)).current}catch{$null}}else{$null})}}
   $sample["guard$k"]=[ordered]@{host=$a.position;client=$b.position;toPuppet=$(if($slot-and$slot.puppet){Dist $a.position $slot.at}else{$null});toHerShade=$(if($sample.her-and$sample.her.present){Dist $b.position $sample.her.transform}else{$null})}}
  if(!$row.filmed-and$slot-and[double]$slot.partnerAt[0]-ne0){$at=$slot.partnerAt;$dx=[double]$at[0]+33;$dy=[double]$at[1]+153;$d=[Math]::Max(.5,[Math]::Sqrt($dx*$dx+$dy*$dy));$ux=$dx/$d;$uy=$dy/$d
   $cx=[double]$at[0]+5*$ux;$cy=[double]$at[1]+5*$uy;$cz=[double]$at[2]+2;$yaw=[Math]::Atan2(-$uy,-$ux)*180/[Math]::PI
   try{C host ('setviewpos -33 -153 68.1 {0} 0' -f $yaw.ToString('F4',$inv));Start-Sleep -Milliseconds 300;& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch 12 -Quick|Out-Null
    C host ('coop_observer_camera hold_at {0} {1} {2}' -f (F $cx),(F $cy),(F $cz));Start-Sleep -Milliseconds 300;$row.hostCamera=[ordered]@{at=$at;camera=@($cx,$cy,$cz);yaw=$yaw;active=(S host '.observer-camera.json').active};$row.filmed=$true}catch{$row.hostCameraError=$_.Exception.Message;$row.filmed=$true}}
  $series.Add([pscustomobject]$sample);Save "$out/cast-series.json" @($series);Start-Sleep -Milliseconds 250}
 # After her shade: the host puppet removed (or lost natively).
 $tail=[Collections.Generic.List[object]]::new();$tw=[Diagnostics.Stopwatch]::StartNew()
 do{$sh=S host '.shade-host.json';$dw=S client '.doppelganger-world.json';$tail.Add([pscustomobject][ordered]@{ms=$tw.ElapsedMilliseconds;result=$sh.result;puppet=$sh.slots[0].puppet;removed=$sh.removed;lost=$sh.lost;herPresent=$dw.present})
  if(!$sh.slots[0].puppet-and!$dw.present){break};Start-Sleep -Milliseconds 500}while($tw.ElapsedMilliseconds-lt20000)
 Save "$out/after-series.json" @($tail);Save "$out/host-after.shade-host.json" (S host '.shade-host.json')
 foreach($q in 'host','client'){foreach($suffix in '.npc-stream.json','.npc-combat.json','.doppelganger-world.json','.shade-host.json','.doppelganger-visual.json'){try{Copy-Item -LiteralPath "$root/work/lab-instances/$q/coop-status.json$suffix" -Destination "$out/cast-$q$suffix" -ErrorAction Stop}catch{}}}
 foreach($c in $caps){try{if(!$c.WaitForExit(30000)){Stop-Process -Id $c.Id}}catch{}}
 $end=$tail[$tail.Count-1];$sh=S host '.shade-host.json'
 $delta=[ordered]@{};foreach($k in 'spawned','removed','lost','pins','stimuli','continuous','teleports'){$delta[$k]=[int64]$sh.$k-[int64]$before.$k}
 $puppetHandles=@($series|Where-Object {$_.host.slot.puppet}|ForEach-Object {[uint32]$_.host.slot.handle}|Select-Object -Unique)
 $attended=@($series|Where-Object {$s1=$_;@($s1.attention1.attention+$s1.attention2.attention+$s1.attention1.combat+$s1.attention2.combat)|Where-Object {$_-and$puppetHandles-contains[uint32]$_}})
 $herAvatarSeen=@($series|Where-Object {@($_.attention1.attention+$_.attention2.attention).Count})
 $withPuppet=@($series|Where-Object {$_.host.slot.puppet});$herPresent=@($series|Where-Object {$_.her.present})
 $hp=@($series|Where-Object {$_.puppetHealth.maximum}|ForEach-Object {[double]$_.puppetHealth.current});$hh=@($series|Where-Object {$_.herHealth.maximum}|ForEach-Object {[double]$_.herHealth.current})
 $near=@($series|Where-Object {($_.guard1.toPuppet-ne$null-and$_.guard1.toPuppet-lt3)-or($_.guard2.toPuppet-ne$null-and$_.guard2.toPuppet-lt3)})
 $herNear=@($series|Where-Object {($_.guard1.toHerShade-ne$null-and$_.guard1.toHerShade-lt3)-or($_.guard2.toHerShade-ne$null-and$_.guard2.toHerShade-lt3)})
 $row.summary=[ordered]@{delta=$delta;puppetHandles=$puppetHandles;attendedSamples=$attended.Count;attentionSamples=$herAvatarSeen.Count;spawned=$delta.spawned;removed=$delta.removed;lost=$delta.lost;pins=$delta.pins;stimuli=$delta.stimuli;result=$sh.result;herPresentSamples=$herPresent.Count;puppetSamples=$withPuppet.Count
  puppetHealthFirst=$(if($hp.Count){$hp[0]}else{$null});puppetHealthMin=$(if($hp.Count){($hp|Measure-Object -Minimum).Minimum}else{$null});herHealthFirst=$(if($hh.Count){$hh[0]}else{$null});herHealthMin=$(if($hh.Count){($hh|Measure-Object -Minimum).Minimum}else{$null})
  guardNearPuppetSamples=$near.Count;guardNearHerShadeSamples=$herNear.Count;maxDrift=($withPuppet|ForEach-Object {[double]$_.host.slot.drift}|Measure-Object -Maximum).Maximum;endPuppet=$end.puppet;endHerPresent=$end.herPresent}
 $s=$row.summary
 $gh=@{};foreach($k in '1','2'){$hs=@($series|Where-Object {$_."health$k"});$gh[$k]=[ordered]@{hostFirst=$(if($hs.Count){$hs[0]."health$k".host});hostLast=$(if($hs.Count){$hs[$hs.Count-1]."health$k".host});clientFirst=$(if($hs.Count){$hs[0]."health$k".client});clientLast=$(if($hs.Count){$hs[$hs.Count-1]."health$k".client})}}
 $row.guardHealth=$gh;$row.checks.Add("Her clone definition $($row.herDefinition); guard health host/client: 1 $($gh['1'].hostFirst)->$($gh['1'].hostLast) / $($gh['1'].clientFirst)->$($gh['1'].clientLast), 2 $($gh['2'].hostFirst)->$($gh['2'].hostLast) / $($gh['2'].clientFirst)->$($gh['2'].clientLast)")
 $row.checks.Add("Her shade present in $($s.herPresentSamples) samples (health $($s.herHealthFirst) to $($s.herHealthMin)); a guard within 3 m of it in her world in $($s.guardNearHerShadeSamples)")
 $row.checks.Add("Host guards' attention holds the puppet in $($s.attendedSamples) samples (any attention in $($s.attentionSamples)); moves continuous $($delta.continuous), teleports $($delta.teleports)")
 $row.checks.Add("Host puppet: spawned $($s.spawned), in $($s.puppetSamples) samples, health $($s.puppetHealthFirst) to $($s.puppetHealthMin), max drift $($s.maxDrift) m, pins $($s.pins), stimuli $($s.stimuli); a host guard within 3 m in $($s.guardNearPuppetSamples); removed $($s.removed), lost $($s.lost) ($($s.result))")
 if(!$herPresent.Count){throw 'Her shade never appeared'}
 if($Control){if([int]$s.spawned){throw "Control: the host spawned $($s.spawned) puppet(s)"};$row.checks.Add('Control: no host puppet');$row.passed=$true;return}
 if([int]$s.spawned-lt1-or!$withPuppet.Count){throw "The host spawned no puppet ($($s.result))"}
 if($end.puppet){throw "The host puppet remains after her shade ($($end.result))"}
 if(!(($s.puppetHealthMin-ne$null-and$s.puppetHealthMin-lt$s.puppetHealthFirst)-or$attended.Count)){throw "No perception: no host guard attended the puppet (a guard within 3 m in $($near.Count) samples) and its health stayed $($s.puppetHealthMin)"}
 $row.checks.Add('The host puppet followed her shade, was perceived, and ended with it');$row.passed=$true
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
   Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;case=[pscustomobject]$row;scope='Stage 4 partner shade (#131): Emily casts her own Doppelganger near the Dunwall lower-floor tutorial guard 1 under the product controller; the host spawns the native shade at her streamed shade, the host NPCs perceive it, and it ends with hers; fixture restored.'})
  }
  [ordered]@{pid=$PID;operation='PartnerShade1729';take=$Take;ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
if($failure){throw $failure};'PASS_PARTNER_SHADE'
