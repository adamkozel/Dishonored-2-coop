param([string]$Take='mission-end-carriage-v1291a',[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo',[ValidateSet('low','high')][string]$Chaos='low',[switch]$FollowHost,[switch]$MirrorEnd,[string]$Candidate='candidate-npc-combat-v1214aw',[int]$WatchSeconds=170,[int]$ClientContinueDelayMs=0)
# Stage 6 mission end (chapter 89, 97): Edge of the World ends when the player
# uses the Addermire Station carriage (script_kiscule_carriage), rides it, and
# the gameflow mission completes (result screen, then institute.mapinfo).
# With the trigger mirror and the lab health hold on, the host is placed in
# the station trigger, then uses the carriage. Both worlds' logic state, map,
# epochs and cinematic epochs are sampled through the prompt, ride, result
# screen and load; both windows are recorded. -FollowHost also enables the
# 1188 join-follow. -MirrorEnd (1214ax+) enables coop_mission_end_mirror on
# both: the partner world must complete its own mission, show its own result
# screen and load the next mission. -ClientContinueDelayMs delays the
# client's Continue on its result screen (1214ay both-ready hold); window
# responsiveness and the native switch trace are recorded. Lab SLOT81 checkpoints are hashed and
# kept; Dunwall fixture restored.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$captures=[Collections.Generic.List[object]]::new();$rows=[Collections.Generic.List[object]]::new()
$slotBackup=Join-Path $env:TEMP "d2-slot81-$Take";$slotHashes=@{}
function Save($p,$v){$v|ConvertTo-Json -Depth 15|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed $r $mode"}
function I($r,[string]$b='',[int]$ms=100){$a=@{Role=$r;DurationMs=$ms};if($b){$a.Button=$b};& "$lab/Send-D2Input.ps1" @a|Out-Null}
function Capture($name,$seconds){foreach($r in 'host','client'){$hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64()
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',[string]$seconds,'-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$name-$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$name-$r-capture.log" -PassThru))}}
function WaitCaptures(){foreach($c in @($captures)){if(!$c.WaitForExit(240000)-or$c.ExitCode){throw 'Recording failed'}};$captures.Clear()}
function Snap($r){$s=S $r;$o=[ordered]@{logic=$s.localGameLogicState;pending=$s.localPendingGameLogicState;map=$s.localMap;mapId=$s.localMapId;epoch=$s.localEpoch;cine=$s.localCinematicEpoch;canSkip=$s.localCinematicCanSkip;remoteMapId=$s.remoteMapId;remoteLogic=$s.remoteGameLogicState;remoteFresh=$s.remoteFresh;connection=$s.connectionState;pos=$s.localPosition}
 if($FollowHost){try{$f=S $r '.follow.json';$o.follow=$f.result;$o.followLoads=$f.loads}catch{}}
 if($MirrorEnd){try{$m=S $r '.missionend.json';$o.mirror=$m.result;$o.mirrorSent=$m.sent;$o.mirrorApplied=$m.applied;$o.mirrorState=$m.appliedState;if($m.hold){$o.hold=$m.hold.result;$o.holdWaitMs=$m.hold.lastWaitMs}}catch{}}
 try{$o.responding=(Get-Process -Id $pids[$r]).Responding}catch{};$o}
function SlotFiles($r){Get-ChildItem "$root/work/lab-instances/$r/base/savegame/SLOT81" -File}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;if((S $r).localGameLogicState-ne1){throw 'Gameplay required'}}
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 New-Item -ItemType Directory $slotBackup -Force|Out-Null
 foreach($r in 'host','client'){New-Item -ItemType Directory "$slotBackup/$r" -Force|Out-Null;$slotHashes[$r]=@(SlotFiles $r|ForEach-Object {Copy-Item $_.FullName "$slotBackup/$r/$($_.Name)";@{name=$_.Name;hash=(Get-FileHash $_.FullName).Hash}})}
 Save "$out/identity.json" @{dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;probe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash;candidate=$Candidate;mapInfo=$MapInfo;chaos=$Chaos;followHost=[bool]$FollowHost;mirrorEnd=[bool]$MirrorEnd;clientContinueDelayMs=$ClientContinueDelayMs;slot81=$slotHashes}
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
 foreach($r in 'host','client'){C $r 'coop_trigger_mirror on';C $r 'coop_player_health_hold on'}
 if($FollowHost){foreach($r in 'host','client'){C $r 'coop_follow_host on'}}
 if($MirrorEnd){foreach($r in 'host','client'){C $r 'coop_mission_end_mirror on'}}
 $ents=(Q host '--entities').entities
 $names='script_trigger_addermirestation_inside_area','script_kar_cable_car_inv_use','script_kar_cable_car_inv_use_1','script_kar_cable_car_inv_use_2','script_kar_cable_car_01_1','script_kar_cable_car_r_door_01_usable','script_kar_cable_car_l_door_01_usable','script_target_carriage_player'
 $at=@{};foreach($n in $names){$e=@($ents|Where-Object name -eq $n);if($e.Count-ne1){throw "Missing $n"};$at[$n]=$e[0].position}
 Save "$out/entities.json" $at
 $timeline=[Collections.Generic.List[object]]::new();$clock=[Diagnostics.Stopwatch]::StartNew()
 function Mark($what){$timeline.Add([ordered]@{ms=$clock.ElapsedMilliseconds;event=$what;host=(Snap host);client=(Snap client)})}
 Capture 'end' $WatchSeconds;Start-Sleep -Milliseconds 800;Mark 'capture'
 $t=$at['script_trigger_addermirestation_inside_area']
 C host ("setviewpos {0:F3} {1:F3} {2:F3} 0 0"-f$t[0],$t[1],($t[2]+1.0));Start-Sleep -Milliseconds 4000;Mark 'station-trigger'
 # Use stands: inside facing the lever, then beside each door facing the car.
 $lever=$at['script_kar_cable_car_inv_use'];$car=$at['script_kar_cable_car_01_1'];$inside=$at['script_target_carriage_player']
 $stands=@(@{name='inside-lever';x=$inside[0];y=$inside[1];z=$inside[2]+1.6;look=$lever},
  @{name='door-east';x=$car[0]+2.0;y=$car[1]+0.7;z=$car[2]+1.7;look=@($car[0],($car[1]+0.7),($car[2]+1.2))},
  @{name='door-west';x=$car[0]-2.0;y=$car[1]+0.7;z=$car[2]+1.7;look=@($car[0],($car[1]+0.7),($car[2]+1.2))},
  @{name='door-south';x=$car[0];y=$car[1]-2.0;z=$car[2]+1.7;look=@($car[0],$car[1],($car[2]+1.2))},
  @{name='door-north';x=$car[0];y=$car[1]+3.4;z=$car[2]+1.7;look=@($car[0],($car[1]+1.4),($car[2]+1.2))})
 $used=$null;$attempts=[Collections.Generic.List[object]]::new()
 foreach($st in $stands){
  $yaw=[Math]::Atan2($st.look[1]-$st.y,$st.look[0]-$st.x)*180/[Math]::PI
  C host ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$st.x,$st.y,$st.z,$yaw);Start-Sleep -Milliseconds 900
  $v=Q host '--player';$eye=$v.nativeViewPosition;$hd=[Math]::Sqrt(($st.look[0]-$eye[0])*($st.look[0]-$eye[0])+($st.look[1]-$eye[1])*($st.look[1]-$eye[1]))
  $pitch=[Math]::Max(-45,[Math]::Min(45,[Math]::Atan2($eye[2]-$st.look[2],[Math]::Max($hd,.2))*180/[Math]::PI))
  & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch $pitch -Quick|Out-Null
  $before=Snap host;I host X 250;$tw=[Diagnostics.Stopwatch]::StartNew();$hit=$false
  do{Start-Sleep -Milliseconds 150;$now=Snap host
   $moved=[Math]::Sqrt([Math]::Pow($now.pos[0]-$before.pos[0],2)+[Math]::Pow($now.pos[1]-$before.pos[1],2)+[Math]::Pow($now.pos[2]-$before.pos[2],2))
   if($now.cine-ne$before.cine-or$now.logic-ne1-or$now.pending-or$moved-gt0.8){$hit=$true}}while(!$hit-and$tw.ElapsedMilliseconds-lt2500)
  $attempts.Add([ordered]@{stand=$st.name;yaw=$yaw;pitch=$pitch;eye=$eye;hit=$hit;before=$before;after=$now});Mark "use-$($st.name)-$hit"
  if($hit){$used=$st.name;break}}
 Save "$out/use-attempts.json" @($attempts)
 if(!$used){throw 'No carriage use registered'}
 # Watch the end sequence. Pokes probe the leave/stay prompt and the result
 # screen; skippable scenes are left to play (their states are the data).
 $pokes=@(@{at=8000;b='X'},@{at=14000;b='A'},@{at=20000;b='Up'},@{at=23000;b='X'},@{at=29000;b='Down'},@{at=32000;b='X'});$poked=@{};$stateSince=@{host=@{logic=1;ms=0};client=@{logic=1;ms=0}};$lastMap=(Snap host).mapId;$done=$false;$resultPresses=@{host=0;client=0};$arrived=@{host=$false;client=$false}
 $useAt=$clock.ElapsedMilliseconds;$progressAt=$useAt;$lastCine=(Snap host).cine
 do{
  Start-Sleep -Milliseconds 400;Mark 'watch';$h=$timeline[$timeline.Count-1].host
  if($h.cine-ne$lastCine-or$h.logic-ne1-or$h.mapId-ne$lastMap){$progressAt=$clock.ElapsedMilliseconds;$lastCine=$h.cine}
  foreach($r in 'host','client'){$x=$timeline[$timeline.Count-1][$r];if($x.logic-ne$stateSince[$r].logic){$stateSince[$r]=@{logic=$x.logic;ms=$clock.ElapsedMilliseconds}}}
  $since=$clock.ElapsedMilliseconds-$useAt
  if($h.logic-eq1-and!$h.pending-and$h.mapId-eq$lastMap){foreach($pk in $pokes){if(!$poked[$pk.at]-and$since-ge$pk.at-and$clock.ElapsedMilliseconds-$progressAt-gt5000){$poked[$pk.at]=$true;I host $pk.b 250;Mark "poke-$($pk.b)";break}}}
  foreach($r in 'host','client'){if($r-eq'client'-and!$MirrorEnd){continue};$x=$timeline[$timeline.Count-1][$r]
   $wait=if($r-eq'client'-and$x.logic-eq11){5000+$ClientContinueDelayMs}else{5000}
   if($x.logic-notin 1,5,8,15,0-and!$x.pending-and$clock.ElapsedMilliseconds-$stateSince[$r].ms-gt$wait-and$resultPresses[$r]-lt6){I $r A 300;$resultPresses[$r]++;$stateSince[$r].ms=$clock.ElapsedMilliseconds;Mark "result-A-$r-$($x.logic)"}
   if($x.logic-in 8,15-and$clock.ElapsedMilliseconds-$stateSince[$r].ms-gt8000){I $r A 300;$stateSince[$r].ms=$clock.ElapsedMilliseconds;Mark "load-A-$r"}
   if(!$arrived[$r]-and$x.map-like'*institute*'-and$x.logic-eq1-and!$x.pending){$arrived[$r]=$true;Mark "$r-next-mission"}}
  if($arrived.host-and(!$MirrorEnd-or$arrived.client)){Start-Sleep -Seconds 8;Mark 'next-mission';$done=$true}
 }while(!$done-and$clock.ElapsedMilliseconds-lt(($WatchSeconds-6)*1000))
 WaitCaptures
 Save "$out/timeline.json" @($timeline)
 foreach($r in 'host','client'){Save "$out/player-$r.json" (Q $r '--player')}
 $states=@{};foreach($x in $timeline){foreach($r in 'host','client'){$k="$r";if(!$states[$k]){$states[$k]=[Collections.Generic.List[string]]::new()};$v="$($x[$r].logic)/$($x[$r].pending)@$($x[$r].map)";if($states[$k].Count-eq0-or$states[$k][$states[$k].Count-1]-ne$v){$states[$k].Add($v)}}}
 foreach($r in 'host','client'){try{Save "$out/missionend-$r.json" (S $r '.missionend.json')}catch{};try{Copy-Item "$root/work/lab-instances/$r/coop-status.json.loading.json" "$out/loading-$r.json"}catch{}}
 $rows.Add(@{used=$used;hostReachedNextMission=$arrived.host;clientReachedNextMission=$arrived.client;hostStates=@($states.host);clientStates=@($states.client);final=@{host=(Snap host);client=(Snap client)}})
 Save "$out/verified.json" @{passed=$done;scope=$(if($MirrorEnd){'Mission end through the carriage reaches the next mission in both worlds.'}else{'Host mission end through the carriage; client behaviour observed.'})}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  foreach($c in @($captures)){try{if(!$c.HasExited-and!$c.WaitForExit(30000)){Stop-Process -Id $c.Id}}catch{}}
  if($dirty){
   foreach($r in 'host','client'){try{I $r;C $r 'coop_trigger_mirror off';C $r 'coop_player_health_hold off';C $r 'coop_follow_host off';C $r 'coop_mission_end_mirror off'}catch{}}
   # Advance any screen that blocks the restore (result screen, load prompt).
   $w=[Diagnostics.Stopwatch]::StartNew()
   do{$ok=$true;foreach($r in 'host','client'){try{$s=S $r;if($s.localGameLogicState-notin 1,2,6,13-or$s.localPendingGameLogicState){$ok=$false;if($s.localGameLogicState-eq5){I $r X 3000}else{I $r A 300}}}catch{$ok=$false}};if(!$ok){Start-Sleep -Milliseconds 1500}}while(!$ok-and$w.ElapsedMilliseconds-lt120000)
   foreach($r in 'host','client'){try{$now=@(SlotFiles $r|ForEach-Object {@{name=$_.Name;hash=(Get-FileHash $_.FullName).Hash}});$changed=@($now|Where-Object {$n=$_;!($slotHashes[$r]|Where-Object {$_.name-eq$n.name-and$_.hash-eq$n.hash})})
     if($changed.Count-or$now.Count-ne$slotHashes[$r].Count){Save "$out/slot81-changed-$r.json" @{before=$slotHashes[$r];after=$now};foreach($f in $slotHashes[$r]){Copy-Item "$slotBackup/$r/$($f.name)" "$root/work/lab-instances/$r/base/savegame/SLOT81/$($f.name)" -Force}}}catch{$errors.Add("slot81 ${r}: $($_.Exception.Message)")}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{$s=S $r;$hp=Q $r '--health' 1;Save "$out/final-$r.json" @{bridge=$s;health=$hp};if($s.localGameLogicState-ne1-or$hp.current-ne56-or!$s.remoteFresh){throw 'Final gameplay/health/transport not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;cases=@($rows);scope='Edge of the World mission end through the carriage (host); both worlds sampled through prompt, ride, result screen and load; Dunwall fixture restored.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
