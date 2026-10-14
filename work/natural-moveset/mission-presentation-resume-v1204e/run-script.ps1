param([string]$Take='mission-presentation-resume-v1204e',[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo',[ValidateSet('low','high')][string]$Chaos='low')
# Stage 3 gate 4 (pause/session boundaries) in the product controller (1204e on 1203d, players staged before arming + warp diagnostic; 1204d on 1203d: post-arming 60 m warp; 1204c on 1203c: 250 ms freshness; 1204 on 1203 failed: watchdog ping-pong; 1204b on 1203b: stream handshake), candidate
# 1203. Both peers on Edge of the World with only coop_product_auto on. The
# partner presentation (avatar follow + shadow stream + movement bank) must
# come back by itself, in the same world, after:
#  1) the client's native pause menu (movement resume policy),
#  2) the host's native pause menu (movement resume policy),
#  3) the shared native tutorial modal (logic 12, raised on both peers by the
#     mirrored street tutorial trigger) - presentation watchdog repair.
# After each case the partner must follow a 3 m move of the other player and
# both windows are filmed face to face. Dunwall fixture restored.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$captures=[Collections.Generic.List[object]]::new();$rows=[Collections.Generic.List[object]]::new()
function Save($p,$v){$v|ConvertTo-Json -Depth 15|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed $r $mode"}
function I($r,[string]$b='',[int]$ms=100){$a=@{Role=$r;DurationMs=$ms};if($b){$a.Button=$b};& "$lab/Send-D2Input.ps1" @a|Out-Null}
function RS($r){S $r '.reset.json'}
function ProductStatus($r){S $r '.product.json'}
function Capture($name,$seconds){foreach($r in 'host','client'){$hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64()
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',[string]$seconds,'-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$name-$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$name-$r-capture.log" -PassThru))}}
function WaitCaptures($name,$min){
 foreach($c in @($captures)){if(!$c.WaitForExit(120000)){throw 'Recording did not finish'}}
 foreach($r in 'host','client'){$d=[double](& ffprobe -v error -show_entries format=duration -of csv=p=0 "$out/$name-$r.mp4");if($d-lt$min){throw "Recording too short: $name-$r $d s"}}
 $captures.Clear()}
# Live partner presentation on one observing peer: avatar at the remote player,
# bank and stream running.
function Live($r){$s=S $r;$m=S $r '.movement.json';$x=S $r '.shadow-stream.json';$a=$s.probeRenderPosition;$b=$s.remotePosition
 $d=if($a-and$b){[Math]::Sqrt(($a[0]-$b[0])*($a[0]-$b[0])+($a[1]-$b[1])*($a[1]-$b[1])+($a[2]-$b[2])*($a[2]-$b[2]))}else{999}
 @{handle=$s.probeActorHandle;follow=$s.probeFollowEnabled;distance=[Math]::Round($d,3);render=$a;remote=$b;local=$s.localPosition;target=$s.probeTargetPosition;clockRejection=$m.clockRejection;weight=$m.weight;bank=[bool]$m.active;stream=[bool]($x.armed-and$x.ready);fresh=$s.remoteFresh;logic=$s.localGameLogicState;ok=($s.probeActorHandle-and$s.probeFollowEnabled-and$d-lt1.0-and$m.active-and$x.armed-and$x.ready-and$s.remoteFresh)}}
function WaitLive($tag,$limit=30000){$w=[Diagnostics.Stopwatch]::StartNew()
 do{Start-Sleep -Milliseconds 300;$p=@{host=(Live host);client=(Live client)};$ok=$p.host.ok-and$p.client.ok}while(!$ok-and$w.ElapsedMilliseconds-lt$limit)
 $p.tag=$tag;$p.waitMs=$w.ElapsedMilliseconds;$p.ok=$ok;$p.product=@{host=(ProductStatus host);client=(ProductStatus client)};$p.resume=@{host=(S host '.movement-resume.json');client=(S client '.movement-resume.json')}
 Save "$out/live-$tag.json" $p;return $p}
$spots=@{host=@(-6.758,142.542,14.555);client=@(-3.436,140.039,14.550)}
function Face($tag){
 foreach($r in 'host','client'){$o=if($r-eq'host'){'client'}else{'host'};$a=$spots[$r];$b=$spots[$o];$yaw=[Math]::Atan2($b[1]-$a[1],$b[0]-$a[0])*180/[Math]::PI
  C $r ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$a[0],$a[1],$a[2],$yaw)}
 Start-Sleep -Milliseconds 400;foreach($r in 'host','client'){& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $r -Pitch 12 -Quick|Out-Null};Start-Sleep -Milliseconds 1200
 $p=WaitLive "$tag-facing" 15000;if(!$p.ok){throw "Partner presentation not live when facing ($tag)"}
 Capture $tag 8;WaitCaptures $tag 7;return $p}
# The partner must follow a fresh 3 m move of the source player.
function Follows($source,$tag){$a=$spots[$source];C $source ("setviewpos {0:F3} {1:F3} {2:F3} 90 0"-f($a[0]+2.1),($a[1]+2.1),$a[2]);Start-Sleep -Milliseconds 1500
 $p=WaitLive "$tag-follows" 10000;if(!$p.ok){throw "Partner did not follow after $tag"};return $p}
function PauseCase($r,$tag){$before=@{host=(S host '.movement-resume.json');client=(S client '.movement-resume.json');repairs=(ProductStatus $r).repairs}
 I $r Start 150;$w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 200}while((S $r).localGameLogicState-ne2-and$w.ElapsedMilliseconds-lt5000)
 $paused=(S $r).localGameLogicState-eq2;if(!$paused){throw "Pause menu did not open ($tag)"}
 Start-Sleep -Seconds 3;$during=@{host=(Live host);client=(Live client)}
 I $r B 200;$w.Restart();do{Start-Sleep -Milliseconds 200}while((S $r).localGameLogicState-ne1-and$w.ElapsedMilliseconds-lt5000)
 if((S $r).localGameLogicState-ne1){I $r B 200;Start-Sleep -Milliseconds 800}
 $after=WaitLive $tag;if(!$after.ok){throw "Partner presentation not restored after $tag"}
 return @{before=$before;during=$during;after=$after}}
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
 $pin=Get-Content "$root/work/movement-integration/candidate-npc-combat-v1203d/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Expected 1203d candidate'}
 Save "$out/identity.json" @{dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;probe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash;mapInfo=$MapInfo}
 foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_checkpoint_sync off','coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off'){C $r $cmd}}
 $dirty=$true
 C host "campaign $MapInfo $Chaos corvo";C client "campaign $MapInfo $Chaos emily"
 $g=WaitGameplay 300000;Save "$out/load-log.json" $g.log;if(!$g.ready){throw 'Both instances did not reach the same mission gameplay'}
 $clientStart=(Q client '--player').candidatePosition
 # 1204e: stage both players at the facing spots BEFORE arming (1204d: a 60 m
 # harness warp after arming left the host's partner 60 m off; studied below).
 foreach($r in 'host','client'){$o=if($r-eq'host'){'client'}else{'host'};$a=$spots[$r];$b=$spots[$o];$yaw=[Math]::Atan2($b[1]-$a[1],$b[0]-$a[0])*180/[Math]::PI;C $r ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$a[0],$a[1],$a[2],$yaw)}
 Start-Sleep -Seconds 2
 foreach($r in 'host','client'){C $r $(if($r-eq'client'){'coop_checkpoint_delay 1500'}else{'coop_checkpoint_delay 0'});C $r 'coop_product_auto on'}
 $series=[Collections.Generic.List[object]]::new()
 function Sample($tag){$row=@{tag=$tag;ms=$w.ElapsedMilliseconds};foreach($r in 'host','client'){$s=S $r;$x=S $r '.shadow-stream.json';$m=S $r '.movement.json';$pp=ProductStatus $r;$rr=S $r '.movement-resume.json'
   $row[$r]=@{logic=$s.localGameLogicState;fresh=$s.remoteFresh;follow=$s.probeFollowEnabled;handle=$s.probeActorHandle;bank=$m.active;stream=$x.result;armed=$x.armed;ready=$x.ready;refused=$x.refused;pairBits=$x.pairBits;product=$pp.result;missing=$pp.missing;repairs=$pp.repairs;quick=$pp.quickRepairs;step=$pp.step;resume=$rr.result;resumePhase=$rr.phase}};$series.Add($row)}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;Sample 'arming';$ok=(ProductStatus host).result-eq'armed'-and(ProductStatus client).result-eq'armed'-and(S host '.movement-resume.json').enabled-and(S client '.movement-resume.json').enabled}while(!$ok-and$w.ElapsedMilliseconds-lt90000)
 Save "$out/arm-series.json" @($series)
 foreach($r in 'host','client'){Save "$out/armed-$r.json" @{product=(ProductStatus $r);resume=(S $r '.movement-resume.json')}};if(!$ok){throw 'Product controller (incl. movement resume) not armed'}
 $base=Face 'armed'
 $cases=@{}
 $cases.pauseClient=PauseCase client 'pause-client';$cases.pauseClientFollow=Follows client 'pause-client';$cases.pauseClientFacing=Face 'pause-client'
 $cases.pauseHost=PauseCase host 'pause-host';$cases.pauseHostFollow=Follows host 'pause-host';$cases.pauseHostFacing=Face 'pause-host'
 # Diagnostic (non-fatal): a harness warp of the client to its mission start (~60 m) and back; the
 # host-side partner presentation is sampled every 300 ms for 15 s each way.
 $warp=[Collections.Generic.List[object]]::new()
 try{$far=$spots.client;C client ("setviewpos {0:F3} {1:F3} {2:F3} 0 0"-f$clientStart[0],$clientStart[1],($clientStart[2]+1.7));$ww=[Diagnostics.Stopwatch]::StartNew()
  while($ww.ElapsedMilliseconds-lt15000){$warp.Add(@{leg='away';ms=$ww.ElapsedMilliseconds;host=(Live host);client=(Live client);product=(ProductStatus host)});Start-Sleep -Milliseconds 300}
  C client ("setviewpos {0:F3} {1:F3} {2:F3} 0 0"-f$far[0],$far[1],$far[2]);$ww.Restart()
  while($ww.ElapsedMilliseconds-lt15000){$warp.Add(@{leg='back';ms=$ww.ElapsedMilliseconds;host=(Live host);client=(Live client);product=(ProductStatus host)});Start-Sleep -Milliseconds 300}}catch{$warp.Add(@{error=$_.Exception.Message})}
 Save "$out/warp-diagnostic.json" @($warp)
 $cases.warpRecovered=WaitLive 'after-warp' 30000
 # Shared tutorial modal: the client walks into the street tutorial trigger.
 $repairsBefore=@{host=(ProductStatus host).repairs;client=(ProductStatus client).repairs}
 $t=@((Q client '--entities').entities|Where-Object name -eq 'script_trigger_streets_wol_tutorial')[0];if(!$t){throw 'Missing tutorial trigger'}
 C client ("setviewpos {0:F3} {1:F3} {2:F3} 0 0"-f$t.position[0],$t.position[1],($t.position[2]+1.0))
 $w.Restart();do{Start-Sleep -Milliseconds 250;$logic=@{host=(S host).localGameLogicState;client=(S client).localGameLogicState}}while(!($logic.host-eq12-or$logic.client-eq12)-and$w.ElapsedMilliseconds-lt8000)
 $modal=@{host=$logic.host;client=$logic.client;ms=$w.ElapsedMilliseconds};Start-Sleep -Seconds 2;$modal.during=@{host=(Live host);client=(Live client)}
 foreach($r in 'host','client'){if((S $r).localGameLogicState-eq12){I $r B 200;Start-Sleep -Milliseconds 800}}
 if(!($modal.host-eq12-or$modal.client-eq12)){throw 'Tutorial modal (logic 12) not raised'}
 $modal.after=WaitLive 'modal' 30000;if(!$modal.after.ok){throw 'Partner presentation not restored after the tutorial modal'}
 $modal.repairs=@{host=(ProductStatus host).repairs-$repairsBefore.host;client=(ProductStatus client).repairs-$repairsBefore.client}
 $cases.modal=$modal;$cases.modalFollow=Follows client 'modal';$cases.modalFacing=Face 'modal'
 $rows.Add(@{base=$base;cases=$cases})
 Save "$out/verified.json" @{passed=$true;scope='Partner presentation restored after pause (both roles) and the shared tutorial modal.'}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};try{if($series){Save "$out/arm-series.json" @($series)}}catch{};try{foreach($r in 'host','client'){Save "$out/failure-live-$r.json" (Live $r);Save "$out/failure-product-$r.json" (ProductStatus $r);Save "$out/failure-resume-$r.json" (S $r '.movement-resume.json')}}catch{}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  foreach($c in @($captures)){try{if(!$c.HasExited-and!$c.WaitForExit(30000)){Stop-Process -Id $c.Id}}catch{}}
  foreach($r in 'host','client'){try{Copy-Item "$root/work/lab-instances/$r/coop-status.json.native-service-timing.json" "$out/timing-$r.json";Save "$out/product-end-$r.json" (ProductStatus $r)}catch{}}
  if($dirty){
   $cw=[Diagnostics.Stopwatch]::StartNew()
   do{$off=$true;foreach($r in 'host','client'){try{I $r;C $r 'coop_product_auto off';C $r 'coop_movement_resume off';foreach($cmd in 'coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off','coop_shadow_stream off','coop_movement_bank off','coop_trigger_mirror off'){C $r $cmd};C $r 'coop_checkpoint_sync off';$st=S $r
     if((S $r '.reset.json').enabled){$off=$false;if($st.localGameLogicState-in8,15){I $r A 300}elseif($st.localGameLogicState-eq2){I $r B 200}}}catch{}}
    if(!$off){Start-Sleep -Milliseconds 1500}}while(!$off-and$cw.ElapsedMilliseconds-lt60000)
   foreach($r in 'host','client'){try{if((S $r).localGameLogicState-in2,12){I $r B 200;Start-Sleep -Milliseconds 800}}catch{}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{$s=S $r;$hp=Q $r '--health' 1;Save "$out/final-$r.json" @{bridge=$s;health=$hp};if($s.localGameLogicState-ne1-or$hp.current-ne56-or!$s.remoteFresh){throw 'Final gameplay/health/transport not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;cases=@($rows);scope='Product controller: partner presentation restored by itself after pause menus (both roles) and the shared tutorial modal on Edge of the World; Dunwall fixture restored.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
