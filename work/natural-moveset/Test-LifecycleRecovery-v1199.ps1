param([string]$Take='mission-lifecycle-v1199d',[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo',[ValidateSet('low','high')][string]$Chaos='low')
# Stage 3 lifecycle requirement map_load_checkpoint_recovery (candidate 1203f; 1199 failed on a harness receipt; 1199c restart facing offset 1.44 m; facing from load-time mission-start spots, 3 m apart, 1.5 m facing tolerance).
# Only the HOST is told to load Edge of the World; the client follows natively
# (coop_follow_host on) and the product controller (coop_product_auto on) arms
# every system for the new shared world. The partner avatar must then be
# present on BOTH peers (probe actor live at the remote player's position) and
# is filmed face to face from both windows. A shared death and native checkpoint
# restart follows; the partner avatar must be restored and filmed again on both
# peers (client-side visibility after restart was open since audit 1161).
# Dunwall fixture restored.
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
function Presence($r){$s=S $r;$a=$s.probeRenderPosition;$b=$s.remotePosition;$d=if($a-and$b){[Math]::Sqrt(($a[0]-$b[0])*($a[0]-$b[0])+($a[1]-$b[1])*($a[1]-$b[1])+($a[2]-$b[2])*($a[2]-$b[2]))}else{999}
 $m=S $r '.movement.json';$x=S $r '.shadow-stream.json'
 @{handle=$s.probeActorHandle;follow=$s.probeFollowEnabled;render=$a;remote=$b;distance=[Math]::Round($d,3);bank=[bool]$m.active;stream=[bool]($x.armed-and$x.ready);bodyApplied=$s.bodyProbeApplied;fresh=$s.remoteFresh;map=$s.localMapId}}
function WaitPresence($tag,$maxDistance=1.0){$w=[Diagnostics.Stopwatch]::StartNew()
 do{Start-Sleep -Milliseconds 400;$p=@{host=(Presence host);client=(Presence client)};$ok=$true;foreach($r in 'host','client'){if(!$p[$r].handle-or!$p[$r].follow-or$p[$r].distance-gt$maxDistance-or!$p[$r].fresh-or!$p[$r].bank-or!$p[$r].stream){$ok=$false}}}while(!$ok-and$w.ElapsedMilliseconds-lt45000)
 $p.tag=$tag;$p.waitMs=$w.ElapsedMilliseconds;$p.ok=$ok;Save "$out/presence-$tag.json" $p;return $p}
# Face to face on the Edge of the World docks (spot 1 of soak 1186), ~4.2 m apart.
$spots=@{host=@(-6.758,142.542,14.555);client=@(-3.436,140.039,14.550)}
# Facing spots from the peers' own mission-start positions (line of sight on
# the docks, as in renewal 1160); eye height 1.7 m, at least 2.5 m apart along
# their connecting line (the soak spots 1186 are separated by a wall).
function SetStartSpots(){$h=(Q host '--player').candidatePosition;$c=(Q client '--player').candidatePosition
 $dx=$c[0]-$h[0];$dy=$c[1]-$h[1];$n=[Math]::Sqrt($dx*$dx+$dy*$dy);if($n-lt.05){$dx=1;$dy=0;$n=1}
 $ux=$dx/$n;$uy=$dy/$n;$gap=[Math]::Max(0,(3.0-$n)/2)
 $script:spots=@{host=@(($h[0]-$ux*$gap),($h[1]-$uy*$gap),($h[2]+1.7));client=@(($c[0]+$ux*$gap),($c[1]+$uy*$gap),($c[2]+1.7))}
 Save "$out/spots.json" $script:spots}

function Face($tag){
 foreach($r in 'host','client'){$o=if($r-eq'host'){'client'}else{'host'};$a=$spots[$r];$b=$spots[$o];$yaw=[Math]::Atan2($b[1]-$a[1],$b[0]-$a[0])*180/[Math]::PI
  C $r ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$a[0],$a[1],$a[2],$yaw)}
 Start-Sleep -Milliseconds 400
 foreach($r in 'host','client'){& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $r -Pitch 12 -Quick|Out-Null}
 Start-Sleep -Milliseconds 1500
 $p=WaitPresence "$tag-facing" 1.5;if(!$p.ok){throw "Partner avatar not present when facing ($tag)"}
 foreach($r in 'host','client'){Save "$out/player-$tag-$r.json" (Q $r '--player')}
 Capture $tag 14;WaitCaptures $tag 12;return $p}
function WaitGameplay($limitMs){$w=[Diagnostics.Stopwatch]::StartNew();$log=[Collections.Generic.List[object]]::new();$last=@{host=0;client=0}
 do{Start-Sleep -Milliseconds 500;$ready=$true
  foreach($r in 'host','client'){$s=S $r;$log.Add(@{ms=$w.ElapsedMilliseconds;role=$r;logic=$s.localGameLogicState;pending=$s.localPendingGameLogicState;map=$s.localMapId})
   if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}
   if($s.localGameLogicState-in8,15-and$w.ElapsedMilliseconds-gt5000-and$w.ElapsedMilliseconds-$last[$r]-gt1500){I $r A 300;$last[$r]=$w.ElapsedMilliseconds}
   elseif($s.localGameLogicState-eq5-and$w.ElapsedMilliseconds-$last[$r]-gt4000){I $r X 3000;$last[$r]=$w.ElapsedMilliseconds}
   elseif($s.localGameLogicState-eq12-and$w.ElapsedMilliseconds-$last[$r]-gt1500){I $r B 200;$last[$r]=$w.ElapsedMilliseconds}}
  if($ready-and(S host).localMapId-ne(S client).localMapId){$ready=$false}
 }while(!$ready-and$w.ElapsedMilliseconds-lt$limitMs);return @{ready=$ready;log=@($log)}}
function WaitArmed($tag){$w=[Diagnostics.Stopwatch]::StartNew()
 do{Start-Sleep -Milliseconds 500;$ok=(ProductStatus host).result-eq'armed'-and(ProductStatus client).result-eq'armed'-and(RS host).result-eq'armed'-and(RS client).result-eq'armed'-and(RS host).mission-and(RS client).mission}while(!$ok-and$w.ElapsedMilliseconds-lt90000)
 foreach($r in 'host','client'){Save "$out/armed-$tag-$r.json" @{product=(ProductStatus $r);reset=(RS $r)}};if(!$ok){throw "Product controller/checkpoint not armed ($tag)"}}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;if((S $r).localGameLogicState-ne1){throw 'Gameplay required'}}
 $pin=Get-Content "$root/work/movement-integration/candidate-npc-combat-v1203f/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Expected 1203f candidate'}
 Save "$out/identity.json" @{dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;probe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash;mapInfo=$MapInfo}
 # Start clean: lab sessions off; the product controller re-arms everything.
 foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_checkpoint_sync off','coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off'){C $r $cmd};C $r 'coop_follow_host on'}
 $dirty=$true
 # 1) Map load: only the host loads; the client follows by itself.
 C host "campaign $MapInfo $Chaos corvo"
 # 1199b: never command a loading peer (1199 failed on an unconfirmed receipt);
 # the product controller is enabled once both peers play the mission.
 $g=WaitGameplay 300000;Save "$out/load-log.json" $g.log;if(!$g.ready){throw 'Both instances did not reach the same mission gameplay'}
 foreach($r in 'host','client'){C $r $(if($r-eq'client'){'coop_checkpoint_delay 1500'}else{'coop_checkpoint_delay 0'});C $r 'coop_product_auto on'}
 foreach($r in 'host','client'){Save "$out/follow-$r.json" (S $r '.follow.json')}
 if((S client '.follow.json').loads-lt1){throw 'Client did not load the mission by following the host'}
 SetStartSpots
 WaitArmed 'load'
 $load=WaitPresence 'load';if(!$load.ok){throw 'Partner avatar not present on both peers after the map load'}
 $loadFacing=Face 'load'
 # 2) Shared death (host falls) and native checkpoint restart on both peers.
 $base=@{host=(RS host).completed;client=(RS client).completed};$epochs=@{host=(S host).localEpoch;client=(S client).localEpoch}
 $pos=(Q host '--player').candidatePosition;C host ("setviewpos {0:F3} {1:F3} {2:F3} 0 0"-f$pos[0],$pos[1],($pos[2]+45))
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 250;$h=RS host;$c=RS client}while(!($h.deathReady-and$c.deathReady-and$h.peerDeathReady-and$c.peerDeathReady)-and$w.ElapsedMilliseconds-lt25000)
 if(!($h.deathReady-and$c.deathReady)){throw 'Shared death screens not reached'}
 Start-Sleep -Seconds 2;I host A 200;Start-Sleep -Milliseconds 300;I client A 200
 $w.Restart();$lastA=@{host=0;client=0}
 do{Start-Sleep -Milliseconds 300;foreach($r in 'host','client'){$st=S $r;if($st.localGameLogicState-in8,15-and$w.ElapsedMilliseconds-$lastA[$r]-gt2500-and$w.ElapsedMilliseconds-gt6000){I $r A 300;$lastA[$r]=$w.ElapsedMilliseconds}}
  $done=(RS host).completed-ge$base.host+1-and(RS client).completed-ge$base.client+1-and(S host).localGameLogicState-eq1-and(S client).localGameLogicState-eq1}while(!$done-and$w.ElapsedMilliseconds-lt120000)
 if(!$done){throw 'Shared checkpoint restart did not complete'}
 $restartEpochs=@{host=(S host).localEpoch;client=(S client).localEpoch}
 # 1199d: reuse the load-time spots (1199c restart spots left a 1.44 m collision/geometry offset).
 WaitArmed 'restart'
 $restart=WaitPresence 'restart';if(!$restart.ok){throw 'Partner avatar not restored on both peers after the shared restart'}
 $restartFacing=Face 'restart'
 $rows.Add(@{clientFollow=(S client '.follow.json');load=$load;loadFacing=$loadFacing;restart=$restart;restartFacing=$restartFacing;epochsBefore=$epochs;epochsAfter=$restartEpochs;completed=@{host=(RS host).completed;client=(RS client).completed}})
 Save "$out/verified.json" @{passed=$true;scope='Partner avatar present on both peers after a native map load (join-follow) and after a shared checkpoint restart.'}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};try{foreach($r in 'host','client'){Save "$out/failure-reset-$r.json" (RS $r);Save "$out/failure-presence-$r.json" (Presence $r)}}catch{}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  foreach($c in @($captures)){try{if(!$c.HasExited-and!$c.WaitForExit(30000)){Stop-Process -Id $c.Id}}catch{}}
  foreach($r in 'host','client'){try{Copy-Item "$root/work/lab-instances/$r/coop-status.json.send-tally.json" "$out/send-tally-$r.json";Copy-Item "$root/work/lab-instances/$r/coop-status.json.native-service-timing.json" "$out/timing-$r.json";Save "$out/product-end-$r.json" (ProductStatus $r);Save "$out/npc-transport-$r.json" (S $r '.npc-stream.json').transport}catch{}}
  if($dirty){
   $cw=[Diagnostics.Stopwatch]::StartNew()
   do{$off=$true;foreach($r in 'host','client'){try{I $r;C $r 'coop_product_auto off';C $r 'coop_follow_host off';foreach($cmd in 'coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off','coop_shadow_stream off','coop_movement_bank off','coop_trigger_mirror off'){C $r $cmd};C $r 'coop_checkpoint_sync off';$st=S $r
     if((S $r '.reset.json').enabled){$off=$false;if($st.localGameLogicState-in8,15){I $r A 300}elseif($st.localGameLogicState-eq2){I $r B 200}}}catch{}}
    if(!$off){Start-Sleep -Milliseconds 1500}}while(!$off-and$cw.ElapsedMilliseconds-lt60000)
   foreach($r in 'host','client'){try{if((S $r).localGameLogicState-eq2){I $r B 200;Start-Sleep -Milliseconds 800}}catch{}}
   foreach($r in 'host','client'){try{if((S $r).localGameLogicState-eq12){I $r B 200;Start-Sleep -Milliseconds 800}}catch{}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{$s=S $r;$hp=Q $r '--health' 1;Save "$out/final-$r.json" @{bridge=$s;health=$hp};if($s.localGameLogicState-ne1-or$hp.current-ne56-or!$s.remoteFresh){throw 'Final gameplay/health/transport not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;cases=@($rows);scope='Lifecycle: partner avatar after a native map load (join-follow + product controller) and after a shared checkpoint restart, filmed from both windows; Dunwall fixture restored.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
