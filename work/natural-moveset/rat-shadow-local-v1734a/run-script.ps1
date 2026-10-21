param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='rat-shadow-local-v1734a',[string]$Candidate='candidate-npc-combat-v1214gx',[int]$SampleSeconds=14)
# 1734 (#131, discovery): what the Rat Shadow upgrade does natively. In co-op the host orders Emily's Shadow Walk kill, so
# her native kill (and its corpse-to-rats outcome) never runs (takedown-variants-v1656a). Here the products stay disarmed
# and Emily alone kills the upper-floor tutorial guard 2 from Shadow Walk with shadow_walk_rat_shadow granted (the 1254y
# swback approach: Shadow Walk 2.4 m behind the guard, crawl, attack; re-placed up to 3 times). Her world's entities new
# since a baseline (handle, definition, position) and the guard's presence and health are sampled for SampleSeconds, her
# window is filmed, and the checkpoint is restored. Pass: the guard died; the new entities are the finding.
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
function Disarm{$cw=[Diagnostics.Stopwatch]::StartNew();try{C host 'coop_npc_combat fixture_off'}catch{};
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
 [ordered]@{pid=$PID;operation='RatShadowLocal1734';take=$Take;started=(Get-Date).ToString('o');state='running'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;$dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;if($dll-ne$pin.dll){throw "Expected $Candidate"}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'};$pids[$r]=$l.pid;I $r
  if((S $r).localGameLogicState-ne1){throw "$r not in gameplay"}
  $inventory[$r]=Q $r '--player-inventory';Save "$out/inventory-before-$r.json" $inventory[$r]
 }
 Save "$out/identity.json" ([ordered]@{candidate=$Candidate;dll=$dll;probe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash})
 $dirty=$true;Disarm
 function Guard2($r){@((Q $r '--world-npcs').npcs|Where-Object {$_.spawnerName-eq'ld_spawner_guard_tutorial_2'-and$_.unique-and$_.reciprocal})|Select-Object -First 1}
 C client 'coop_player_health_hold on'
 $row.granted=@();foreach($item in 'models/powers/shadow_walk/shadow_walk.inventoryitem','components/characters/player/base/attributes/shadow_walk_rat_shadow.inventoryitem'){C client "coop_grant_named $item";Start-Sleep -Milliseconds 1500
  $row.granted+=[pscustomobject]@{item=$item;result=$(try{(S client '.observer-camera.json').itemGrant.named.result}catch{$null})}}
 $g=Guard2 client;if(!$g){throw 'Guard 2 absent'};$row.guardHandle=$g.handle;$row.guardBefore=$g.position
 $baseline=@{};foreach($e in @((Q client '--entities').entities)){$baseline[[string]$e.handle]=$true};$fresh=[ordered]@{}
 $scan={param($ms)foreach($e in @((Q client '--entities').entities)){$hk=[string]$e.handle;if(!$baseline.ContainsKey($hk)-and!$fresh.Contains($hk)){$fresh[$hk]=[ordered]@{firstMs=$ms;handle=$e.handle;name=$e.name;type=$e.type;definition=$e.definitionName;position=$e.position}}}}
 $hwnd=(Get-Process -Id $pids['client']).MainWindowHandle.ToInt64()
 $caps+=Start-Process (Get-Command ffmpeg).Source -ArgumentList @('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',"$($SampleSeconds+6)",'-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/kill-source.mp4") -WindowStyle Hidden -RedirectStandardError "$out/kill-source-capture.log" -RedirectStandardOutput "$out/kill-source-capture.stdout" -PassThru
 $place={$g=Guard2 client;$fx=[double]$g.orientation[0];$fy=[double]$g.orientation[1];$look=[Math]::Atan2($fy,$fx)*180/[Math]::PI
  C client ('setviewpos {0} {1} 68.10 {2} 0' -f (F ([double]$g.position[0]-2.4*$fx)),(F ([double]$g.position[1]-2.4*$fy)),$look.ToString('F4',$inv))}
 $st0=try{[int](Q client '--player-states').stance}catch{-1};if($st0-ne1){I client B 150;Start-Sleep -Milliseconds 500}
 $press={& "$lab/Send-D2Input.ps1" -Role client -LeftTrigger 255 -DurationMs 250|Out-Null;Start-Sleep -Milliseconds 1500
  & "$lab/Send-D2Input.ps1" -Role client -LeftY 14000 -DurationMs 1200|Out-Null;& "$lab/Send-D2Input.ps1" -Role client -RightTrigger 255 -DurationMs 250|Out-Null}
 & $place;Start-Sleep -Milliseconds 150;& $press;$attempts=1;$last=0;$series=[Collections.Generic.List[object]]::new();$w=[Diagnostics.Stopwatch]::StartNew();$deadAt=$null
 while($w.ElapsedMilliseconds-lt($SampleSeconds*1000)){
  $gs=try{Guard2 client}catch{$null};$hh=if($gs){try{(Q client '--health' $gs.handle).current}catch{$null}}else{$null}
  $ps=try{Q client '--player-states'}catch{$null}
  if($null-ne$hh-and[double]$hh-le0-and$null-eq$deadAt){$deadAt=$w.ElapsedMilliseconds}
  if($null-eq$deadAt-and$attempts-lt3-and$w.ElapsedMilliseconds-$last-ge4000){& $place;Start-Sleep -Milliseconds 150;& $press;$attempts++;$last=$w.ElapsedMilliseconds}
  try{& $scan $w.ElapsedMilliseconds}catch{}
  $series.Add([pscustomobject][ordered]@{ms=$w.ElapsedMilliseconds;guardPresent=$null-ne$gs;guardHealth=$hh;guardAt=$gs.position;states=@($ps.active|ForEach-Object {$_.type})});Save "$out/kill-series.json" @($series);Start-Sleep -Milliseconds 300}
 Save "$out/new-entities.json" $fresh;$row.newEntities=$fresh.Count;$row.attempts=$attempts;$row.guardDeadAt=$deadAt
 $row.guardEnd=$series[$series.Count-1];foreach($c in $caps){try{if(!$c.WaitForExit(30000)){Stop-Process -Id $c.Id}}catch{}}
 $row.checks.Add("Guard 2 dead at $deadAt ms after $attempts attempt(s); present at the end $($row.guardEnd.guardPresent); $($fresh.Count) new entities in her world: "+(($fresh.Values|ForEach-Object {"$($_.firstMs) $($_.definition) $($_.name)"})-join '; '))
 if($null-eq$deadAt){throw 'Guard 2 did not die'}
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
   Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;case=[pscustomobject]$row;scope='Discovery for #131: Emily alone (products disarmed) kills the tutorial guard 2 from Shadow Walk with Rat Shadow; her world new entities are recorded; checkpoint restored.'})
  }
  [ordered]@{pid=$PID;operation='RatShadowLocal1734';take=$Take;ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
if($failure){throw $failure};'PASS_RAT_SHADOW_LOCAL'
