param([string]$Take='mission-soak-v1186',[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo',[ValidateSet('low','high')][string]$Chaos='low')
# Stage 7 product controller (candidate 1181): the harness arms NOTHING except
# coop_product_auto on; the DLL controller must arm every system itself and keep
# it armed across both shared restarts. Earlier header (1153): every system is armed
# (partner avatar follow, door/coin/usable sessions, NPC stream) and must
# renew exactly once across each shared mission restart, both roles dying.
# Earlier header (1147): both peers arm checkpoint sync in
# mission mode (+ usable session). The host dies by a native fall; the client
# must enter its native GameOver too (shared death). The host chooses the
# native "Restart From Last Checkpoint" and the client presses it as a
# duplicate; both peers restart exactly once from their own mission autosave
# behind the both-ready barrier. Dunwall fixture restored.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$captures=[Collections.Generic.List[object]]::new();$rows=[Collections.Generic.List[object]]::new()
function Save($p,$v){$v|ConvertTo-Json -Depth 15|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed $r $mode"}
function I($r,[string]$b='',[int]$ms=100){$a=@{Role=$r;DurationMs=$ms};if($b){$a.Button=$b};& "$lab/Send-D2Input.ps1" @a|Out-Null}
function W($o,$offset){$w=@($o.words|Where-Object offset -eq $offset);if($w.Count-ne1){throw "Missing object field $offset"};[Convert]::ToUInt64($w[0].word.Substring(2),16)}
function Bind($r,$name){$door=@((Q $r '--entities').entities|Where-Object name -eq $name);if($door.Count-ne1){return $null}
 $cp=@((Q $r '--components' $door[0].handle).components|Where-Object type -eq '.?AVarkCpntOpenable@@');if($cp.Count-ne1-or$cp[0].owner-ne$door[0].pointer){return $null}
 $o=Q $r '--object' $cp[0].pointer;@{name=$name;position=$door[0].position;component=$cp[0].pointer;owner=[Convert]::ToUInt64($door[0].pointer.Substring(2),16);generation=((W $o '0x18')-band [uint64]4294967295)}}
function NativeDoor($r,$b){$o=Q $r '--object' $b.component;if($o.type-ne'.?AVarkCpntOpenable@@'-or(W $o '0x10')-ne$b.owner-or((W $o '0x18')-band [uint64]4294967295)-ne$b.generation){throw 'Native door lifetime changed'};$s=W $o '0x48';[pscustomobject]@{current=[uint32]($s-band [uint64]4294967295);pending=[uint32]($s-shr32);side=[int]((W $o '0xc8')-band255)}}
function Entry($r,$name){@((S $r '.door-sync.json').entries|Where-Object name -eq $name)[0]}
function Capture($name,$seconds){foreach($r in 'host','client'){$hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64()
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',[string]$seconds,'-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$name-$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$name-$r-capture.log" -PassThru))}}
function UseSweep($r,$b,$want,[int[]]$Offsets=@(0,20,-20,40,-40)){
 # The door origin is the hinge: sweep the bearing until native use changes it.
 $v=Q $r '--player';$here=$v.candidatePosition
 foreach($off in $Offsets){
  if($off){C $r ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$here[0],$here[1],($here[2]+1.7),($script:baseYaw+$off));Start-Sleep -Milliseconds 500}
  I $r X 200;$t=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 80;$n=NativeDoor $r $b}while($n.current-ne$want-and$n.pending-ne$want-and$t.ElapsedMilliseconds-lt1200)
  if($n.current-eq$want-or$n.pending-eq$want){return $off}}
 return $null}
function WaitCaptures(){foreach($c in @($captures)){if(!$c.WaitForExit(200000)-or$c.ExitCode){throw 'Recording failed'}};$captures.Clear()}
function Approach($r,$door,$from){
 $p=$door.position;$dx=$from[0]-$p[0];$dy=$from[1]-$p[1];$n=[Math]::Max([Math]::Sqrt($dx*$dx+$dy*$dy),.001);$ux=$dx/$n;$uy=$dy/$n
 $script:standX=$p[0]+1.4*$ux;$script:standY=$p[1]+1.4*$uy;$script:standZ=$p[2]+1.7;$script:baseYaw=[Math]::Atan2(-$uy,-$ux)*180/[Math]::PI
 C $r ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$standX,$standY,$standZ,$baseYaw);Start-Sleep -Milliseconds 1300
 $v=Q $r '--player';$eye=$v.nativeViewPosition;$hd=[Math]::Sqrt(($p[0]-$eye[0])*($p[0]-$eye[0])+($p[1]-$eye[1])*($p[1]-$eye[1]))
 $pitch=[Math]::Max(-45,[Math]::Min(45,[Math]::Atan2($eye[2]-($p[2]+1.0),$hd)*180/[Math]::PI))
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $r -Pitch $pitch -Quick|Out-Null;return @{eye=$eye;pitch=$pitch}}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;if((S $r).localGameLogicState-ne1){throw 'Gameplay required'}}
 $pin=Get-Content "$root/work/movement-integration/candidate-npc-combat-v1181/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Expected 1181 candidate'}
 Save "$out/identity.json" @{dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;probe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash;mapInfo=$MapInfo}
 $dirty=$true
 C host "campaign $MapInfo $Chaos corvo";C client "campaign $MapInfo $Chaos emily"
 $w=[Diagnostics.Stopwatch]::StartNew();$log=[Collections.Generic.List[object]]::new();$last=@{host=0;client=0}
 do{
  Start-Sleep -Milliseconds 500;$ready=$true
  foreach($r in 'host','client'){$s=S $r;$log.Add(@{ms=$w.ElapsedMilliseconds;role=$r;logic=$s.localGameLogicState;pending=$s.localPendingGameLogicState})
   if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}
   if($s.localGameLogicState-in8,15-and$w.ElapsedMilliseconds-gt5000-and$w.ElapsedMilliseconds-$last[$r]-gt1500){I $r A 300;$last[$r]=$w.ElapsedMilliseconds}
   elseif($s.localGameLogicState-eq5-and$w.ElapsedMilliseconds-$last[$r]-gt4000){I $r X 3000;$last[$r]=$w.ElapsedMilliseconds}}
 }while(!$ready-and$w.ElapsedMilliseconds-lt240000)
 Save "$out/load-log.json" @($log)
 if(!$ready){throw 'Both instances did not reach mission gameplay'}
 Start-Sleep -Seconds 2
 if(!(S host).remoteFresh-or!(S client).remoteFresh){throw 'Transport not fresh on mission map'}
 function RS($r){S $r '.reset.json'}
 function Y($r){S $r '.usable-sync.json'}
 function T($r){S $r '.checkpoint.json'}
 $timeline=[Collections.Generic.List[object]]::new();$presses=[Collections.Generic.List[object]]::new()
 function Snap($tag){$row=@{tag=$tag;ms=$clock.ElapsedMilliseconds};foreach($r in 'host','client'){$b=S $r;$x=RS $r;$row[$r]=@{logic=$b.localGameLogicState;pending=$b.localPendingGameLogicState;epoch=$b.localEpoch;phase=$x.phase;result=$x.result;loads=$x.loads;completed=$x.completed;aborted=$x.aborted;held=$x.held;expectBoundary=$x.expectBoundary;loadingEntryAt=$x.loadingEntryAt;continuationCalls=$x.continuationCalls;continuationAccepted=$x.continuationAccepted;continuationDeferred=$x.continuationDeferred;hostReady=$x.hostReady;clientReady=$x.clientReady;deathActive=$x.deathActive;deathReady=$x.deathReady;peerDeathReady=$x.peerDeathReady;mission=$x.mission;slot=$x.missionSlot;gen=$x.missionGeneration;details=$x.missionDetails;digest=$x.digest}};$timeline.Add($row);$row}
 $clock=[Diagnostics.Stopwatch]::StartNew()
 $start=@{};foreach($r in 'host','client'){Save "$out/player-start-$r.json" (Q $r '--player');$start[$r]=(Q $r '--player').candidatePosition}
 $traceBefore=@{host=(T host).callbacks;client=(T client).callbacks}
 function DS($r){S $r '.door-sync.json'}
 function CS($r){S $r '.coin-sync.json'}
 function NS($r){S $r '.npc-stream.json'}
 foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_checkpoint_sync off','coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off'){C $r $cmd}}
 function ProductStatus($r){S $r '.product.json'}
 foreach($r in 'host','client'){C $r $(if($r-eq'client'){'coop_checkpoint_delay 1500'}else{'coop_checkpoint_delay 0'});C $r 'coop_product_auto on'}
 $pw=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$ok=(ProductStatus host).result-eq'armed'-and(ProductStatus client).result-eq'armed'}while(!$ok-and$pw.ElapsedMilliseconds-lt40000)
 foreach($r in 'host','client'){Save "$out/product-armed-$r.json" (ProductStatus $r)}
 if(!$ok){throw 'Product controller did not finish arming'}
 $w=[Diagnostics.Stopwatch]::StartNew()
 # 1147b refuses an arm until that peer's mission-start autosave exists;
 # re-arm a refused peer (recorded) for up to 60 s.
 $rearms=[Collections.Generic.List[object]]::new()
 do{Start-Sleep -Milliseconds 400;$s=Snap 'arming'
  foreach($r in 'host','client'){if(!$s[$r].mission-or$s[$r].result-in'enable_refused','map_scope_unavailable','disabled'){$rearms.Add(@{ms=$w.ElapsedMilliseconds;role=$r;result=$s[$r].result;observedOnly=$true})}} # 1182: the product controller re-arms; the harness only observes
  $ok=$s.host.result-eq'armed'-and$s.client.result-eq'armed'-and$s.host.mission-and$s.client.mission}while(!$ok-and$w.ElapsedMilliseconds-lt60000)
 Save "$out/arm-retries.json" @($rearms)
 foreach($r in 'host','client'){Save "$out/armed-reset-$r.json" (RS $r);Save "$out/armed-usable-$r.json" (Y $r)}
 if(!$ok){throw "Mission checkpoint arm failed host=$($s.host.result) client=$($s.client.result)"}
 if($s.host.digest-ne$s.client.digest){throw 'Mission checkpoint identities differ'}
 # Two cycles: host dies and requests; then the client dies and requests (the
 # other peer presses restart as a duplicate). Counts are relative per cycle.
 # 1186 stability soak: 20 minutes with the product controller armed.
 $soak=[Collections.Generic.List[object]]::new();$deaths=[Collections.Generic.List[object]]::new()
 $spots=@(@{host=@($start.host[0],$start.host[1],($start.host[2]+1.7));client=@($start.client[0],$start.client[1],($start.client[2]+1.7))},@{host=@(-6.758,142.542,14.555);client=@(-3.436,140.039,14.550)},@{host=@(62.226,4.468,34.6);client=@(63.4,3.5,34.6)})
 function Proc($r){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid -ErrorAction Stop;@{ws=$p.WorkingSet64;priv=$p.PrivateMemorySize64;handles=$p.HandleCount;threads=$p.Threads.Count}}
 function Sample($tag){$row=@{tag=$tag;ms=$soakClock.ElapsedMilliseconds}
  foreach($r in 'host','client'){$b=S $r;$x=RS $r;$n=NS $r;$row[$r]=@{logic=$b.localGameLogicState;pending=$b.localPendingGameLogicState;fresh=$b.remoteFresh;epoch=$b.localEpoch;product=(ProductStatus $r).result;checkpoint=$x.result;phase=$x.phase;door=(DS $r).session.result;usable=(Y $r).session.result;coin=(CS $r).result;npc=$n.session.result;npcBound=$n.relevance.bound;queueUs=$n.transport.queueUs;proc=(Proc $r)}}
  $soak.Add($row);$row}
 $soakClock=[Diagnostics.Stopwatch]::StartNew();$spot=0;$nextMove=30000;$nextDeath=360000;$dyingRole='host';$nextSample=0
 $first=Sample 'start'
 while($soakClock.ElapsedMilliseconds-lt1200000){
  Start-Sleep -Milliseconds 500;$ms=$soakClock.ElapsedMilliseconds
  if($ms-ge$nextSample){$s=Sample 'tick';$nextSample=$ms+10000
   foreach($r in 'host','client'){if($s[$r].phase-eq7){throw "Checkpoint session failed during soak ($r): $($s[$r].checkpoint)"}}}
  foreach($r in 'host','client'){$st=S $r;if($st.localGameLogicState-eq12){I $r B 200}}
  if($ms-ge$nextMove){$spot=($spot+1)%3;foreach($r in 'host','client'){$p=$spots[$spot][$r];C $r ("setviewpos {0:F3} {1:F3} {2:F3} 0 0"-f$p[0],$p[1],$p[2])};$nextMove=$ms+30000}
  if($ms-ge$nextDeath){
   $other=if($dyingRole-eq'host'){'client'}else{'host'};$base=@{host=(RS host).completed;client=(RS client).completed}
   $pos=(Q $dyingRole '--player').candidatePosition;C $dyingRole ("setviewpos {0:F3} {1:F3} {2:F3} 0 0"-f$pos[0],$pos[1],($pos[2]+45))
   $w.Restart();do{Start-Sleep -Milliseconds 250;$h=RS host;$c=RS client}while(!($h.deathReady-and$c.deathReady-and$h.peerDeathReady-and$c.peerDeathReady)-and$w.ElapsedMilliseconds-lt25000)
   $deathOk=$h.deathReady-and$c.deathReady
   if($deathOk){Start-Sleep -Seconds 2;I $dyingRole A 200;Start-Sleep -Milliseconds 300;I $other A 200
    $w.Restart();$lastA=@{host=0;client=0}
    do{Start-Sleep -Milliseconds 300;foreach($r in 'host','client'){$st=S $r;if($st.localGameLogicState-in8,15-and$w.ElapsedMilliseconds-$lastA[$r]-gt2500-and$w.ElapsedMilliseconds-gt6000){I $r A 300;$lastA[$r]=$w.ElapsedMilliseconds}}
     $done=(RS host).completed-ge$base.host+1-and(RS client).completed-ge$base.client+1-and(S host).localGameLogicState-eq1-and(S client).localGameLogicState-eq1}while(!$done-and$w.ElapsedMilliseconds-lt120000)}
   $deaths.Add(@{ms=$ms;dying=$dyingRole;deathScreens=$deathOk;restarted=$done})
   if(!$deathOk-or!$done){throw "Shared death/restart failed during soak ($dyingRole)"}
   $dyingRole=$other;$nextDeath=$soakClock.ElapsedMilliseconds+360000;$nextMove=$soakClock.ElapsedMilliseconds+20000}
 }
 $last=Sample 'end'
 Save "$out/soak-samples.json" @($soak);Save "$out/soak-deaths.json" @($deaths)
 $memory=@{};foreach($r in 'host','client'){$a=@($soak|Where-Object {$_.ms-ge120000}|Select-Object -First 1);$base=if($a.Count){$a[0][$r].proc.priv}else{$first[$r].proc.priv};$memory[$r]=@{startPriv=$first[$r].proc.priv;baselinePriv=$base;endPriv=$last[$r].proc.priv;growthMB=[Math]::Round(($last[$r].proc.priv-$base)/1MB,1);handlesStart=$first[$r].proc.handles;handlesEnd=$last[$r].proc.handles}}
 $armedEnd=@{};foreach($r in 'host','client'){$armedEnd[$r]=@{product=$last[$r].product;door=$last[$r].door;usable=$last[$r].usable;npc=$last[$r].npc;checkpoint=$last[$r].checkpoint;fresh=$last[$r].fresh}}
 foreach($r in 'host','client'){if(!$last[$r].fresh-or$last[$r].product-ne'armed'){throw "Soak ended without a fresh, armed $r"};if($memory[$r].growthMB-gt512){throw "Memory grew $($memory[$r].growthMB) MB on $r"}}
 $rows.Add(@{soakMinutes=20;samples=$soak.Count;deaths=@($deaths);memory=$memory;armedEnd=$armedEnd})
 Save "$out/verified.json" @{passed=$true;scope='Shared mission death and checkpoint restart.'}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};try{Save "$out/timeline.json" @($timeline);Save "$out/presses.json" @($presses);foreach($r in 'host','client'){Save "$out/failure-reset-$r.json" (RS $r);Save "$out/failure-trace-$r.json" (T $r)}}catch{}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  foreach($c in @($captures)){try{if(!$c.HasExited-and!$c.WaitForExit(30000)){Stop-Process -Id $c.Id}}catch{}}
  if($dirty){
   # A failed mission checkpoint may still own a load boundary; finish native
   # prompts (A on 8/15, B on pause 2) until both sessions disable.
   $cw=[Diagnostics.Stopwatch]::StartNew()
   do{$off=$true;foreach($r in 'host','client'){try{I $r;C $r 'coop_product_auto off';foreach($cmd in 'coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off','coop_shadow_stream off','coop_movement_bank off'){C $r $cmd};C $r 'coop_checkpoint_sync off';$st=S $r
     if((S $r '.reset.json').enabled){$off=$false;if($st.localGameLogicState-in8,15){I $r A 300}elseif($st.localGameLogicState-eq2){I $r B 200}}}catch{}}
    if(!$off){Start-Sleep -Milliseconds 1500}}while(!$off-and$cw.ElapsedMilliseconds-lt60000)
   foreach($r in 'host','client'){try{if((S $r).localGameLogicState-eq2){I $r B 200;Start-Sleep -Milliseconds 800}}catch{}}
   foreach($r in 'host','client'){try{if((S $r).localGameLogicState-eq12){I $r B 200;Start-Sleep -Milliseconds 800}}catch{}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{$s=S $r;$hp=Q $r '--health' 1;Save "$out/final-$r.json" @{bridge=$s;health=$hp};if($s.localGameLogicState-ne1-or$hp.current-ne56-or!$s.remoteFresh){throw 'Final gameplay/health/transport not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;cases=@($rows);scope='20-minute stability soak with the product controller: movement across the mission, shared deaths/restarts, transport/session/memory sampling; Dunwall fixture restored.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
