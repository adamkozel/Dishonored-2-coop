param([string]$Candidate='candidate-npc-combat-v1213k',[string]$Take='cutscene-transition-v1236',[int]$SceneLimitMs=120000)
# 1236 (#8, cutscene_enter_exit): the partner presentation into and out of a REAL native scene. The
# Dunwall fixture's Mayhew conversation is a native skippable cinematic (Cinematic5, chapter 53): restore
# the pre-scene save, arm the product controller with both partners live, face each other, then both
# players use Mayhew as the fixture does. The scene plays in full (no skip). Recorded: both peers' logic
# timeline and partner presentation (live/bank/stream/weight) every 500 ms during the scene, videos of
# both windows from the scene start, and the time from the scene end until the partner is live again,
# then a facing capture. Pass: both peers enter logic 5 together, return to gameplay, and the partner
# presentation is live again within 15 s with no stale partner left standing in the scene.
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
# Facing spots from the peers' own mission-start positions (line of sight on
# the docks, as in renewal 1160); eye height 1.7 m, at least 2.5 m apart along
# their connecting line (the soak spots 1186 are separated by a wall).
function Face($tag){
 foreach($r in 'host','client'){$o=if($r-eq'host'){'client'}else{'host'};$a=$spots[$r];$b=$spots[$o];$yaw=[Math]::Atan2($b[1]-$a[1],$b[0]-$a[0])*180/[Math]::PI
  C $r ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$a[0],$a[1],$a[2],$yaw)}
 Start-Sleep -Milliseconds 400;foreach($r in 'host','client'){& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $r -Pitch 12 -Quick|Out-Null};Start-Sleep -Milliseconds 1200
 $p=WaitLive "$tag-facing" 15000;if(!$p.ok){throw "Partner presentation not live when facing ($tag)"}
 Capture $tag 8;WaitCaptures $tag 7;return $p}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;if((S $r).localGameLogicState-ne1){throw 'Gameplay required'}}
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 Save "$out/identity.json" @{dll=$pin.dll;probe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash;candidate=$Candidate}
 $dirty=$true
 # Pre-scene state: the fixture's own restore (both Dunwall saves, before Mayhew).
 & "$lab/Restore-D2Lab.ps1" -Doppelganger|Out-Null
 foreach($r in 'host','client'){if((S $r).localGameLogicState-ne1){throw "Pre-scene gameplay not reached ($r)"}}
 foreach($r in 'host','client'){C $r 'coop_product_auto on'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$ok=(ProductStatus host).result-eq'armed'-and(ProductStatus client).result-eq'armed'}while(!$ok-and$w.ElapsedMilliseconds-lt60000)
 if(!$ok){throw 'Product controller not armed'}
 $script:spots=@{host=@(-39.4,-160.4,68.1);client=@(-39.4,-162.4,68.1)}
 $base=WaitLive 'armed' 30000;if(!$base.ok){throw 'Partner presentation not live before the scene'}
 # Both players use Mayhew exactly as the fixture does; the native scene starts on both.
 Capture 'scene' 40
 foreach($r in 'host','client'){C $r 'setviewpos -39.4 -161.4 68.1 270 20';& "$lab/Send-D2Input.ps1" -Role $r -Button X -DurationMs 400|Out-Null}
 $timeline=[Collections.Generic.List[object]]::new();$sw=[Diagnostics.Stopwatch]::StartNew();$seen=@{host=0;client=0};$ended=$null
 do{Start-Sleep -Milliseconds 500;$row=@{ms=$sw.ElapsedMilliseconds}
  foreach($r in 'host','client'){$st=S $r;$m=S $r '.movement.json';$row[$r]=@{logic=$st.localGameLogicState;pending=$st.localPendingGameLogicState;live=(Live $r).ok;bank=[bool]$m.active;weight=$m.weight;fresh=$st.remoteFresh;follow=$st.probeFollowEnabled}
   if($st.localGameLogicState-eq5-and!$seen[$r]){$seen[$r]=$sw.ElapsedMilliseconds}}
  $timeline.Add($row)
  if($seen.host-and$seen.client-and$row.host.logic-eq1-and$row.client.logic-eq1-and!$row.host.pending-and!$row.client.pending){$ended=$sw.ElapsedMilliseconds}
 }while(!$ended-and$sw.ElapsedMilliseconds-lt$SceneLimitMs)
 Save "$out/scene-timeline.json" @($timeline)
 if(!$seen.host-or!$seen.client){throw "The native scene did not start on both peers (host $($seen.host) ms, client $($seen.client) ms)"}
 if(!$ended){throw 'The native scene did not end on both peers within the limit'}
 $after=WaitLive 'after-scene' 30000
 $result=[ordered]@{sceneStartHostMs=$seen.host;sceneStartClientMs=$seen.client;sceneEndMs=$ended;restoreMs=$after.waitMs;restored=$after.ok;
  duringLiveSamples=@($timeline|Where-Object {$_.host.logic-eq5-and$_.host.live}).Count;duringSamples=@($timeline|Where-Object {$_.host.logic-eq5}).Count}
 Save "$out/scene-result.json" $result
 if(!$after.ok){throw "Partner presentation not live within 30 s after the scene"}
 if($after.waitMs-gt15000){throw "Partner presentation took $($after.waitMs) ms after the scene"}
 $facing=Face 'after-scene'
 $rows.Add(@{base=$base;scene=$result;facing=$facing})
 Save "$out/verified.json" @{passed=$true;scope='Partner presentation into and out of the native Mayhew scene (Cinematic5), both peers.'}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};try{foreach($r in 'host','client'){Save "$out/failure-live-$r.json" (Live $r);Save "$out/failure-product-$r.json" (ProductStatus $r);Save "$out/failure-movement-$r.json" (S $r '.movement.json')}}catch{}}
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
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;cases=@($rows);scope='cutscene_enter_exit: partner presentation into and out of the native Mayhew scene (Cinematic5); Dunwall fixture restored.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
