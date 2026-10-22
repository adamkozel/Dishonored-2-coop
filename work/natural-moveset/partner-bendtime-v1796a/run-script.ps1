param([ValidateSet('bendtime','stoptime')][string]$Power='stoptime',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='partner-bendtime-v1796a',[string]$Candidate='candidate-npc-combat-v1214ie',[ValidateSet(80,81)][int]$HostSlot=81,[int]$BendSeconds=7)
# 1796 (#160, 1214ie): a Corvo partner's Bend Time (-Power bendtime) or Stop Time (-Power stoptime, the native upgrade
# bendtime_stoptime granted with the power) acts on the host's world. Same fixture as Test-PartnerCorvoPower-v1795: the
# partner loads SLOT80 (Corvo), stands 4 m in front of the upper-floor tutorial guard 2, and gets the power
# (coop_grant_named). Timeline: a baseline window, her cast (left trigger tap, confirmed by her time-bend status and
# retried up to 3 times), BendSeconds of her Bend Time, her second tap to end it, and an after window. Through all of it
# the guard is sampled in both worlds (position and head joint), both players' positions and both time-bend statuses.
# The host walks 1 s forward and 1 s back in the baseline and again during her Bend Time: players keep real time.
# Judged:
# - her Bend Time runs (her time-bend status: local active, its percent);
# - the host holds a request at her factor (host request, world factor = her percent) while it runs, and none after;
# - the host's guard head moves less during than before (Stop Time: frozen, under 5 mm), and her copy follows;
# - the host's walks during her Bend Time cover at least 80 percent of the baseline walks.
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
$flat={param($a,$b)if(!$a-or!$b){return $null};[Math]::Round([Math]::Sqrt([Math]::Pow([double]$a[0]-[double]$b[0],2)+[Math]::Pow([double]$a[1]-[double]$b[1],2)),3)}
$caps=@();$series=[Collections.Generic.List[object]]::new();$sw=$null;$handles=@{}
function Sample($phase){
 $x=[ordered]@{ms=$sw.ElapsedMilliseconds;phase=$phase}
 foreach($r in 'host','client'){$h=$handles[$r];$n=@((Q $r '--world-npcs').npcs|Where-Object {$_.handle-eq$h})|Select-Object -First 1
  $sk=try{Q $r '--skeleton' $h}catch{$null};$hd=@($sk.joints|Where-Object {$_.nativeName-eq'center_head_jnt'})
  $tb=try{S $r '.time-bend.json'}catch{$null}
  $x[$r]=[ordered]@{guard=$n.position;head=$(if($hd.Count){@($hd[0].globalPose[12],$hd[0].globalPose[13],$hd[0].globalPose[14])}else{$null});player=$(try{(Q $r '--player').candidatePosition}catch{$null})
   timeBend=$(if($tb){[ordered]@{localActive=$tb.local.active;localPercent=$tb.local.percent;remoteActive=$tb.remote.active;remotePercent=$tb.remote.percent;request=$tb.host.request;worldFactor=$tb.worldFactor;requests=$tb.requests;result=$tb.result}}else{$null})}}
 $series.Add([pscustomobject]$x)}
function Walk($phase){$p0=(Q host '--player').candidatePosition;& "$lab/Send-D2Input.ps1" -Role host -LeftY 32000 -DurationMs 1000|Out-Null;$p1=(Q host '--player').candidatePosition
 & "$lab/Send-D2Input.ps1" -Role host -LeftY -32000 -DurationMs 1000|Out-Null;$p2=(Q host '--player').candidatePosition
 [ordered]@{phase=$phase;forward=(& $flat $p0 $p1);back=(& $flat $p1 $p2)}}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Another native operation remains active'}
 if(Test-Path -LiteralPath $out){throw 'Unique evidence take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 [ordered]@{pid=$PID;operation='PartnerBendTime1796';take=$Take;started=(Get-Date).ToString('o');state='running'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;$dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;if($dll-ne$pin.dll){throw "Expected $Candidate"}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid;I $r
  if((S $r).localGameLogicState-ne1){throw "$r not in gameplay"}
  $inventory[$r]=Q $r '--player-inventory';Save "$out/inventory-before-$r.json" $inventory[$r]
 }
 Save "$out/identity.json" ([ordered]@{candidate=$Candidate;dll=$dll;probe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash})
 $dirty=$true
 $loads=@('client');if($HostSlot-eq80){$loads+='host'}
 foreach($r in $loads){& "$root/work/release/Enter-PairingFixture-v1790.ps1" -Role $r -Slot 80 -EvidencePath "$out/fixture-$r.json"|Out-Null}
 Start-Sleep 3;$row.definitions=[ordered]@{host=(Q host '--character').definition;client=(Q client '--character').definition}
 if("$($row.definitions.client)"-notmatch'/corvo/'){throw "The partner does not play Corvo ($($row.definitions.client))"}
 Arm
 foreach($q in 'host','client'){C $q 'setviewpos -33 -153 68.1 90 0'}
 $live=WaitLive 'armed' 45000;if(!$live.ok){throw 'Partner presentation not live after arming'}
 $row.characters=[ordered]@{host=(S host '.character.json');client=(S client '.character.json')}
 C client 'coop_player_health_hold on';C host 'coop_player_health_hold on'
 C client 'coop_grant_named models/powers/bendtime/bendtime.inventoryitem'
 if($Power-eq'stoptime'){Start-Sleep -Milliseconds 1500;C client 'coop_grant_named components/characters/player/base/attributes/bendtime_stoptime.inventoryitem'}
 $e=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 400;$gr=try{(S client '.observer-camera.json').itemGrant.named}catch{$null}}while($e.ElapsedMilliseconds-lt4000-and"$($gr.result)"-notmatch'equip_requested|timeout');$row.grant=$gr
 $g=Guard host;$cg=Guard client;$handles.host=$g.handle;$handles.client=$cg.handle;$row.guardHandleHost=$g.handle;$row.guardHandleClient=$cg.handle
 $cfx=[double]$cg.orientation[0];$cfy=[double]$cg.orientation[1];$stand=@(([double]$cg.position[0]+4*$cfx),([double]$cg.position[1]+4*$cfy),[double]$cg.position[2])
 $row.castAim=(& $aim $cg.position $stand);Start-Sleep -Milliseconds 800
 $total=4+5+$BendSeconds+6+10
 foreach($pair in @(@('client','source'),@('host','observer'))){$hwnd=(Get-Process -Id $pids[$pair[0]]).MainWindowHandle.ToInt64()
  $caps+=Start-Process (Get-Command ffmpeg).Source -ArgumentList @('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',"$total",'-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-movflags','+frag_keyframe+empty_moov+default_base_moof',"$out/$($pair[1]).mp4") -WindowStyle Hidden -RedirectStandardError "$out/$($pair[1])-capture.log" -PassThru}
 Start-Sleep 2
 $sw=[Diagnostics.Stopwatch]::StartNew();$row.walks=@()
 $until=$sw.ElapsedMilliseconds+3000;while($sw.ElapsedMilliseconds-lt$until){Sample 'before';Start-Sleep -Milliseconds 150}
 $row.walks+=(Walk 'before');Sample 'before'
 $row.attempts=@();$cast=$false
 for($attempt=1;$attempt-le3-and!$cast;$attempt++){
  & "$lab/Send-D2Input.ps1" -Role client -LeftTrigger 255 -DurationMs 350|Out-Null
  $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 200;$tb=S client '.time-bend.json'}while(!$tb.local.active-and$w.ElapsedMilliseconds-lt2500)
  $cast=[bool]$tb.local.active;$row.attempts+=[ordered]@{attempt=$attempt;ms=$sw.ElapsedMilliseconds;active=$cast;percent=$tb.local.percent}
  if(!$cast){Sample 'retry'}}
 if(!$cast){throw 'Her Bend Time never ran'}
 $row.castMs=$sw.ElapsedMilliseconds
 $until=$sw.ElapsedMilliseconds+1500;while($sw.ElapsedMilliseconds-lt$until){Sample 'during';Start-Sleep -Milliseconds 100}
 $row.walks+=(Walk 'during');Sample 'during'
 $until=$row.castMs+$BendSeconds*1000;while($sw.ElapsedMilliseconds-lt$until){Sample 'during';Start-Sleep -Milliseconds 100}
 $tb=S client '.time-bend.json'
 if($tb.local.active){& "$lab/Send-D2Input.ps1" -Role client -LeftTrigger 255 -DurationMs 350|Out-Null;$row.endPressed=$true}else{$row.endPressed=$false}
 $row.endMs=$sw.ElapsedMilliseconds
 $until=$sw.ElapsedMilliseconds+1500;while($sw.ElapsedMilliseconds-lt$until){Sample 'ending';Start-Sleep -Milliseconds 150}
 $until=$sw.ElapsedMilliseconds+4000;while($sw.ElapsedMilliseconds-lt$until){Sample 'after';Start-Sleep -Milliseconds 150}
 Save "$out/series.json" @($series)
 foreach($q in 'host','client'){foreach($suffix in '.npc-combat.json','.npc-stream.json','.character.json','.time-bend.json'){try{Copy-Item -LiteralPath "$root/work/lab-instances/$q/coop-status.json$suffix" -Destination "$out/after-$q$suffix" -Force}catch{}}}
 foreach($c in $caps){try{if(!$c.WaitForExit(40000)){Stop-Process -Id $c.Id}}catch{}}
 # Judge.
 $headMax={param($k,$ph)$in=@($series|Where-Object {$_.phase-eq$ph-and$_.$k.head});if($in.Count-lt2){return $null};$m=0;$p=$in[0].$k.head
  foreach($s in $in){$d=[Math]::Sqrt([Math]::Pow([double]$s.$k.head[0]-$p[0],2)+[Math]::Pow([double]$s.$k.head[1]-$p[1],2)+[Math]::Pow([double]$s.$k.head[2]-$p[2],2));if($d-gt$m){$m=$d}};[Math]::Round($m*1000,1)}
 $guardMove={param($k,$ph)$in=@($series|Where-Object {$_.phase-eq$ph-and$_.$k.guard});if($in.Count-lt2){return $null};$m=0;foreach($s in $in){$d=& $flat $in[0].$k.guard $s.$k.guard;if($d-gt$m){$m=$d}};$m}
 $row.head=[ordered]@{hostBefore=(& $headMax 'host' 'before');hostDuring=(& $headMax 'host' 'during');hostAfter=(& $headMax 'host' 'after');clientBefore=(& $headMax 'client' 'before');clientDuring=(& $headMax 'client' 'during');clientAfter=(& $headMax 'client' 'after')}
 $row.guard=[ordered]@{hostBefore=(& $guardMove 'host' 'before');hostDuring=(& $guardMove 'host' 'during');hostAfter=(& $guardMove 'host' 'after');clientDuring=(& $guardMove 'client' 'during')}
 $during=@($series|Where-Object {$_.phase-eq'during'});$after=@($series|Where-Object {$_.phase-eq'after'})
 $herPercent=@($during|ForEach-Object {$_.client.timeBend.localPercent}|Where-Object {$_-ne$null}|Select-Object -Unique)
 $hostHeld=@($during|Where-Object {$_.host.timeBend.request-ge0}).Count;$hostFactors=@($during|ForEach-Object {$_.host.timeBend.worldFactor}|Select-Object -Unique)
 $afterHeld=@($after|Where-Object {$_.host.timeBend.request-ge0}).Count;$afterFactor=@($after|ForEach-Object {$_.host.timeBend.worldFactor}|Select-Object -Unique)
 $wb=$row.walks[0];$wd=$row.walks[1]
 $row.summary=[ordered]@{herPercent=$herPercent;hostHeldDuring="$hostHeld/$($during.Count)";hostWorldFactorsDuring=$hostFactors;hostHeldAfter="$afterHeld/$($after.Count)";hostWorldFactorAfter=$afterFactor;head=$row.head;guard=$row.guard;walks=$row.walks}
 $fails=[Collections.Generic.List[string]]::new()
 if(!$herPercent.Count){$fails.Add('her Bend Time percent never sampled')}
 $expect=if($herPercent.Count){[double]$herPercent[0]/100}else{-1}
 if($hostHeld-lt[Math]::Max(1,$during.Count-3)){$fails.Add("host request held in $hostHeld of $($during.Count) during samples")}
 if(@($during|Where-Object {$_.host.timeBend.request-ge0-and[Math]::Abs([double]$_.host.timeBend.worldFactor-$expect)-gt.011}).Count){$fails.Add("host world factor differs from her $expect")}
 if($afterHeld){$fails.Add('host request still held after her Bend Time')}
 if(@($after|Where-Object {[double]$_.host.timeBend.worldFactor-ne1}).Count){$fails.Add('host world factor not back to 1')}
 if($Power-eq'stoptime'){if($row.head.hostDuring-eq$null-or$row.head.hostDuring-ge5){$fails.Add("host guard head moved $($row.head.hostDuring) mm in Stop Time")}}
 elseif($row.head.hostDuring-eq$null-or$row.head.hostBefore-eq$null-or$row.head.hostDuring-ge$row.head.hostBefore){$fails.Add("host guard head not slowed ($($row.head.hostBefore) -> $($row.head.hostDuring) mm)")}
 foreach($k in 'forward','back'){if(!$wb.$k-or!$wd.$k-or$wd.$k-lt0.8*$wb.$k){$fails.Add("host walk $k during her Bend Time $($wd.$k) m against $($wb.$k) m")}}
 $row.checks.Add("Her $Power at $($herPercent -join ',') percent; host request held $hostHeld/$($during.Count), world factors $($hostFactors -join ','), after $afterHeld/$($after.Count) at $($afterFactor -join ',')")
 $row.checks.Add("Guard head mm host $($row.head.hostBefore)/$($row.head.hostDuring)/$($row.head.hostAfter), her copy $($row.head.clientBefore)/$($row.head.clientDuring)/$($row.head.clientAfter) (before/during/after)")
 $row.checks.Add("Host walks before $($wb.forward)/$($wb.back) m, during $($wd.forward)/$($wd.back) m")
 if($fails.Count){throw ($fails -join '; ')}
 $row.passed=$true
}catch{$failure=$_.Exception.Message;$row.failure=$failure;if(Test-Path -LiteralPath $out){Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};if($series.Count){Save "$out/series.json" @($series)}}}finally{
 foreach($c in $caps){if($c){try{if(!$c.HasExited){Stop-Process -Id $c.Id}}catch{}}}
 if($owned){
  if($dirty){try{Restore 'final';$row.restored=$true}catch{$row.restored=$false;$failure=($failure+'; restore: '+$_.Exception.Message).TrimStart('; ')}}
  $errors=[Collections.Generic.List[string]]::new()
  try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}}catch{$errors.Add($_.Exception.Message)}
  foreach($r in 'host','client'){try{$s=S $r;if($s.localGameLogicState-ne1-or!$s.remoteFresh-or(Q $r '--health' 1).current-ne56){$errors.Add("$r gameplay/transport/health not restored")}}catch{$errors.Add("$r status unavailable")}}
  if(Test-Path -LiteralPath $out){
   Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)}
   if($errors.Count-and!$failure){$failure=$errors-join '; '}
   Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;case=[pscustomobject]$row;scope="#160: a Corvo partner (lab SLOT80) uses $Power by the Dunwall tutorial guard 2 under the product controller; the host must hold her time factor for its world while both players keep real time."})
  }
  [ordered]@{pid=$PID;operation='PartnerBendTime1796';take=$Take;ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
if($failure){throw $failure};'PASS_PARTNER_BENDTIME'
