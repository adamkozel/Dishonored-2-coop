param([string]$Take='route-dunwall-v1300a',[string]$MapInfo='campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo',[ValidateSet('low','high')][string]$Chaos='low',[string]$Candidate='candidate-npc-combat-v1214bo',[Parameter(Mandatory)][string]$StepsFile,[int]$LoadSeconds=300,[switch]$NoLoad,[int]$VideoSeconds=0)
# Campaign route take (chapter 97): both lab worlds load a mission start in lab
# product mode with the mission end mirror, pass the native character choice
# (host Corvo, client Emily), then the host runs a step list:
#   {"do":"trigger","name":<arkTrigger>}            stand in the volume
#   {"do":"use","name":<entity>,"z":<eye z>?}       stand beside it and press X
#   {"do":"at","pos":[x,y,z],"yaw":<deg>}           move there
#   {"do":"wait","ms":<n>}
#   {"do":"attack","name":<npc regex>,"near":[x,y],"max":<presses>}  fight it until its health is 0
#   {"do":"loot","name":<npc regex>,"near":[x,y]}  use the body (search)
#   {"do":"talk","name":<npc regex>,"near":[x,y]}  face and talk (scene must start)
#   {"do":"press","button":"B","ms":200,"role":"host"}  one controller press (close a note)
#   {"do":"die","role":"host"|"client"}  native fall: shared GameOver, both restart
#     from their own last checkpoint (chapter 88), held and released together
# After each step both worlds' logic, map, gameflow counts by state and a
# frame per window are recorded; scenes are skipped with the ordinary hold.
# Dunwall fixture restored.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$notes=[ordered]@{}
function Save($p,$v){$v|ConvertTo-Json -Depth 12|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed $r $mode"}
function I($r,[string]$b='',[int]$ms=100){$a=@{Role=$r;DurationMs=$ms};if($b){$a.Button=$b};& "$lab/Send-D2Input.ps1" @a|Out-Null}
function Frame($r,$name){$hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64()
 & (Get-Command ffmpeg).Source -hide_banner -loglevel error -nostdin -f lavfi -i "gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra" -frames:v 1 -y "$out/$name-$r.png" 2>$null|Out-Null}
function Snap($r){$s=S $r;$m=$null;try{$m=S $r '.missionend.json'}catch{}
 [ordered]@{logic=$s.localGameLogicState;pending=$s.localPendingGameLogicState;map=$s.localMap;epoch=$s.localEpoch;cine=$s.localCinematicEpoch;pos=$s.localPosition;fresh=$s.remoteFresh;gameflow=$(if($m){$m.gameflowStates}else{$null});gfMirror=$(if($m-and$m.gameflowMirror){$m.gameflowMirror.result});idHash=$(if($m-and$m.gameflowMirror){$m.gameflowMirror.idHash});gfApplied=$(if($m-and$m.gameflowMirror){$m.gameflowMirror.applied});lt=$(if($m-and$m.levelTransition){"$($m.levelTransition.result)/$($m.levelTransition.sent)/$($m.levelTransition.applied)"});hold=$(if($m-and$m.hold){"$($m.hold.result)/$($m.hold.lastWaitMs)"});ka=$(try{$k=S $r '.kiscule.json';"$($k.local)/$($k.applied)/$($k.suppressed)"}catch{$null})}}
# Drives both worlds through scenes, prompts, the character choice and loads
# until both are in gameplay or the time runs out.
function Settle($label,$sec){
 $w=[Diagnostics.Stopwatch]::StartNew();$last=@{host=0;client=0};$choice=@{host=0;client=0}
 do{Start-Sleep -Milliseconds 700;$ready=$true
  foreach($r in 'host','client'){$x=Snap $r;if($x.logic-ne1-or$x.pending){$ready=$false}
   if($w.ElapsedMilliseconds-$last[$r]-lt3500){continue}
   switch($x.logic){
    14{$choice[$r]++;I $r $(if($r-eq'host'){'Left'}else{'Right'}) 250;Start-Sleep -Milliseconds 400;I $r A 400;$last[$r]=$w.ElapsedMilliseconds}
    3{I $r Right 250;Start-Sleep -Milliseconds 400;I $r A 400;$last[$r]=$w.ElapsedMilliseconds}
    5{I $r X 3000;$last[$r]=$w.ElapsedMilliseconds}
    {$_-in 8,15,12,9,11}{I $r A 300;$last[$r]=$w.ElapsedMilliseconds}
    4{I $r B 250;$last[$r]=$w.ElapsedMilliseconds} # a readable (ReadNote) left open
    13{throw "$r died during $label (GameOver)"}}}
 }while(!$ready-and$w.ElapsedMilliseconds-lt($sec*1000))
 return $ready}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1";Copy-Item $StepsFile "$out/steps.json"
 $steps=Get-Content $StepsFile -Raw|ConvertFrom-Json
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;if((S $r).localGameLogicState-ne1){throw 'Gameplay required'}}
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 Save "$out/identity.json" @{dll=$pin.dll;candidate=$Candidate;mapInfo=$MapInfo;chaos=$Chaos}
 $dirty=$true
 # -VideoSeconds records both windows for the whole take (evidence video; never committed).
 $captures=[Collections.Generic.List[object]]::new()
 if($VideoSeconds){foreach($r in 'host','client'){$hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64()
  $va=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',[string]$VideoSeconds,'-an','-c:v','libx264','-preset','veryfast','-crf','23','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/take-$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $va -WindowStyle Hidden -RedirectStandardError "$out/take-$r-capture.log" -PassThru))}}
 # The script action mirror is armed before the load: product auto arms its steps
 # only after both worlds reach gameplay, after the intro's grants (1214bj).
 foreach($r in 'host','client'){C $r 'coop_mission_end_mirror on';C $r 'coop_kiscule_mirror on';C $r 'coop_product_auto on'}
 # -NoLoad starts from the current Dunwall fixture (story state after Mayhew) to iterate later steps faster.
 if(!$NoLoad){C host "campaign $MapInfo $Chaos corvo";C client "campaign $MapInfo $Chaos emily"}
 if(!(Settle 'start' $LoadSeconds)){$notes.startState=@{host=(Snap host);client=(Snap client)};foreach($r in 'host','client'){Frame $r 'start-timeout'};throw 'Both worlds did not reach mission gameplay'}
 C host 'coop_player_health_hold on';C client 'coop_player_health_hold on'
 Start-Sleep 3;$rows=[Collections.Generic.List[object]]::new();$rows.Add([ordered]@{step='start';host=(Snap host);client=(Snap client)});foreach($r in 'host','client'){Frame $r 'step00-start'}
 $i=0
 function Npc($rx,$near){$all=@((Q host '--entities').entities|Where-Object {$_.type-eq'.?AVarkNPC@@'-and$_.name-match$rx})
  if(!$all.Count){$near2=if($near){$near}else{(Snap host).pos};$names=@((Q host '--entities').entities|Where-Object {$_.type-eq'.?AVarkNPC@@'}|Sort-Object {[Math]::Pow($_.position[0]-$near2[0],2)+[Math]::Pow($_.position[1]-$near2[1],2)}|Select-Object -First 8|ForEach-Object {"$($_.name)@$([int]$_.position[0]),$([int]$_.position[1])"});throw "No NPC matches $rx (nearest: $($names -join ' '))"};if($near){$all=@($all|Sort-Object {[Math]::Pow($_.position[0]-$near[0],2)+[Math]::Pow($_.position[1]-$near[1],2)})};$all[0]}
 function Face($p,$dist,$eyeUp){$h=(Snap host).pos;$dx=$h[0]-$p[0];$dy=$h[1]-$p[1];$n=[Math]::Max([Math]::Sqrt($dx*$dx+$dy*$dy),.001)
  $x=$p[0]+$dist*$dx/$n;$y=$p[1]+$dist*$dy/$n;$yaw=[Math]::Atan2($p[1]-$y,$p[0]-$x)*180/[Math]::PI
  C host ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$x,$y,($p[2]+$eyeUp),$yaw)}
 # Entity list for a step; a world still loading from the previous step is settled first.
 function Ents(){for($k=0;$k-lt6;$k++){try{return (Q host '--entities').entities}catch{[void](Settle 'entities' 60);Start-Sleep 2}};throw 'Native query failed host --entities'}
 foreach($st in $steps){$i++;$tag='step{0:D2}-{1}'-f$i,$st.do;$ents=Ents
  switch($st.do){
   'trigger'{$e=@($ents|Where-Object name -eq $st.name);if($e.Count-ne1){throw "Missing trigger $($st.name)"};$p=$e[0].position;$back=(Snap host).pos
    C host ("setviewpos {0:F3} {1:F3} {2:F3} 0 0"-f$p[0],$p[1],($p[2]+1.0))
    # bounce: the enter event fires on entry; return to the last stand before a volume without a floor drops the player.
    if($st.bounce){Start-Sleep -Milliseconds 600;C host ("setviewpos {0:F3} {1:F3} {2:F3} 0 0"-f$back[0],$back[1],($back[2]+1.7))}}
   'at'{C host ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$st.pos[0],$st.pos[1],$st.pos[2],$(if($st.yaw){$st.yaw}else{0}))}
   'use'{$e=@($ents|Where-Object name -eq $st.name);if($e.Count-ne1){throw "Missing usable $($st.name)"};$p=$e[0].position;$z=if($st.z){$st.z}else{$p[2]+1.0};$hit=$false
    $offs=if($st.offsets){$st.offsets}else{@(@(0.9,0),@(-0.9,0),@(0,0.9),@(0,-0.9))}
    # Sweep pitch and bearing at each stand (small targets such as signet buttons).
    $pitches=if($st.pitches){$st.pitches}else{@(0)};$yaws=if($st.yaws){$st.yaws}else{@(0)};$k=0
    # Objectives can change a few seconds after the use (a door's task): compare with the step's start too.
    $g0="$((Snap host).gameflow)"
    foreach($o in $offs){$x=$p[0]+$o[0];$y=$p[1]+$o[1];$yaw0=[Math]::Atan2($p[1]-$y,$p[0]-$x)*180/[Math]::PI
     foreach($pt in $pitches){foreach($yo in $yaws){$k++
      C host ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$x,$y,$z,($yaw0+$yo));Start-Sleep -Milliseconds 700
      # aim: the pitch from the native eye to the target, plus the listed offsets.
      $pitch=$pt;if($st.aim){try{$eye=(Q host '--player').nativeViewPosition;$hd=[Math]::Sqrt([Math]::Pow($p[0]-$eye[0],2)+[Math]::Pow($p[1]-$eye[1],2));$pitch=[Math]::Max(-60,[Math]::Min(75,[Math]::Atan2($eye[2]-$p[2],[Math]::Max($hd,.2))*180/[Math]::PI+$pt))}catch{}}
      if($pitch){try{& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch $pitch -Quick|Out-Null}catch{}}
      $b=Snap host;if($k-le6){Frame host "$tag-try$k"};I host X $(if($st.hold){$st.hold}else{250});Start-Sleep -Milliseconds 1500;$a=Snap host
      if($a.logic-eq13){throw "Host died at $($st.name)"}
      # gone: a pickup counts only when the entity has left the world.
      if($st.gone){if($a.logic-eq4){I host B 250;Start-Sleep -Milliseconds 800};$left=@((Q host '--entities').entities|Where-Object name -eq $st.name).Count-eq0;if($left){$hit=$true;break};continue}
      if($a.cine-ne$b.cine-or$a.logic-ne1-or"$($a.gameflow)"-ne"$($b.gameflow)"-or"$($a.gameflow)"-ne$g0){$hit=$true;break}}
      if($hit){break}}
     if($hit){break}}
    if(!$hit){throw "Use of $($st.name) not registered"}}
   'wait'{Start-Sleep -Milliseconds $st.ms}
   'press'{I $(if($st.role){$st.role}else{'host'}) $st.button $(if($st.ms){$st.ms}else{200})}
   'die'{$who=if($st.role){$st.role}else{'host'};$other=if($who-eq'host'){'client'}else{'host'};$e0=@{host=(Snap host).epoch;client=(Snap client).epoch}
    # Same sequence as Test-MissionCheckpoint-v1151: both death screens ready, one
    # A each, then A only on load prompts; the checkpoint session state is traced.
    function DS($tag){$row=[ordered]@{ms=$dw.ElapsedMilliseconds;event=$tag};foreach($r in 'host','client'){$b=Snap $r;$x=$null;try{$x=S $r '.reset.json'}catch{}
      $row[$r]=[ordered]@{logic=$b.logic;pending=$b.pending;map=$b.map;epoch=$b.epoch;phase=$(if($x){$x.phase});result=$(if($x){$x.result});deathReady=$(if($x){$x.deathReady});peerDeathReady=$(if($x){$x.peerDeathReady});mission=$(if($x){$x.mission});slot=$(if($x){$x.missionSlot});loads=$(if($x){$x.loads});completed=$(if($x){$x.completed});aborted=$(if($x){$x.aborted});requests=$(if($x){$x.requests});duplicates=$(if($x){$x.duplicates});rejected=$(if($x){$x.rejected});arm=$(if($x){$x.arm});peerArm=$(if($x){$x.peerArm});remaps=$(if($x){$x.remaps});digest=$(if($x){$x.digest});details=$(if($x){$x.missionDetails});received=$(if($x){$x.received});pair=$(if($x-and$x.autosavePair){$y=$x.autosavePair;"$($y.result) a$([int]$y.active) s$($y.saves)/o$($y.originated)/m$($y.mirrored)/p$([int]$y.pending) peer s$($y.peerSaves)/o$($y.peerOriginated)/m$($y.peerMirrored)/p$([int]$y.peerPending) req$($y.requests) ref$($y.refused) rej$($y.rejected)"})}};$trace.Add($row);$row}
    $dw=[Diagnostics.Stopwatch]::StartNew();$trace=[Collections.Generic.List[object]]::new();[void](DS 'before')
    C $who 'coop_player_health_hold off';Start-Sleep -Milliseconds 400;$pos=(Snap $who).pos
    C $who ("setviewpos {0:F3} {1:F3} {2:F3} 0 0"-f$pos[0],$pos[1],($pos[2]+45));$dw.Restart()
    do{Start-Sleep -Milliseconds 250;$d=DS 'death';$ready=$d.host.logic-eq13-and$d.client.logic-eq13-and$d.host.deathReady-and$d.host.peerDeathReady-and$d.client.deathReady-and$d.client.peerDeathReady
     foreach($r in 'host','client'){if($d[$r].phase-eq7){Save "$out/$tag-death.json" @($trace);throw "Death failed on $r ($($d[$r].result))"}}}while(!$ready-and$dw.ElapsedMilliseconds-lt30000)
    if(!$ready){Save "$out/$tag-death.json" @($trace);throw "Shared death screens not ready ($who fell)"}
    foreach($r in 'host','client'){Frame $r "$tag-gameover"};Start-Sleep -Seconds 3;I $who A 200;Start-Sleep -Milliseconds 300;I $other A 200;$dw.Restart();$lastA=@{host=0;client=0};$confirm=0
    do{Start-Sleep -Milliseconds 300;$d=DS 'restart'
     # Both still on GameOver: a refused attempt fails within 8 s and the session
     # re-arms 3 s later (1214bn); press Restart again, as a player would.
     if($d.host.logic-eq13-and$d.client.logic-eq13-and$dw.ElapsedMilliseconds-gt(12000+$confirm*12000)-and$confirm-lt4){
      $confirm++;foreach($r in 'host','client'){Frame $r "$tag-retry$confirm"};[void](DS "retry$confirm");I $who A 300}
     # A failed attempt re-arms after 3 s (1214bn); the retry below presses again.
     foreach($r in 'host','client'){
      if($d[$r].logic-in 8,15-and$dw.ElapsedMilliseconds-$lastA[$r]-gt2500-and$dw.ElapsedMilliseconds-gt6000){I $r A 300;$lastA[$r]=$dw.ElapsedMilliseconds}
      elseif($d[$r].logic-eq5-and$dw.ElapsedMilliseconds-$lastA[$r]-gt3500){I $r X 3000;$lastA[$r]=$dw.ElapsedMilliseconds}}
     $back=$d.host.logic-eq1-and!$d.host.pending-and$d.client.logic-eq1-and!$d.client.pending-and$d.host.epoch-ne$e0.host-and$d.client.epoch-ne$e0.client}while(!$back-and$dw.ElapsedMilliseconds-lt180000)
    [void](DS 'end');Save "$out/$tag-death.json" @($trace)
    if(!$back){throw "Shared checkpoint restart did not complete ($who fell)"};C $who 'coop_player_health_hold on'}
   'attack'{$npc=Npc $st.name $st.near;$hd=[Convert]::ToInt64($npc.handle.Substring(2),16);$trace=[Collections.Generic.List[object]]::new();$max=if($st.max){$st.max}else{40};$dead=$false
    for($k=0;$k-lt$max-and!$dead;$k++){if($k%4-eq0){$npc=Npc $st.name $st.near;Face $npc.position 1.5 1.6;Start-Sleep -Milliseconds 500}
     & "$lab/Send-D2Input.ps1" -Role host -RightTrigger 255 -DurationMs 150|Out-Null;Start-Sleep -Milliseconds 700
     $hp=$null;try{$hp=Q host '--health' $hd}catch{};$trace.Add(@{press=$k;health=$(if($hp){$hp.current}else{$null});host=(Snap host).logic});if($hp-and$hp.current-le0){$dead=$true}
     if((Snap host).logic-eq13){throw 'Host died in the fight'}}
    Save "$out/$tag-fight.json" @($trace);if(!$dead){throw "$($st.name) still alive after $max attacks"}}
   'talk'{$npc=$null;try{$npc=Npc $st.name $st.near}catch{if($st.optional){$notes["$tag"]='skipped: npc gone (scene already ran)';break}else{throw}};$hit=$false
    foreach($dp in @(@(1.5,5),@(1.2,10),@(2.0,0),@(1.0,15))){Face $npc.position $dp[0] 1.65;Start-Sleep -Milliseconds 700;try{& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch $dp[1] -Quick|Out-Null}catch{}
     $b=Snap host;I host X 400;Start-Sleep -Milliseconds 2000;$a=Snap host;if($a.cine-ne$b.cine-or$a.logic-ne1-or"$($a.gameflow)"-ne"$($b.gameflow)"){$hit=$true;break}}
    if(!$hit){throw "Talk to $($st.name) not registered"}}
   'loot'{$npc=Npc $st.name $st.near;$hit=$false
    foreach($dp in @(@(1.0,45),@(0.8,60),@(1.2,35),@(0.7,70),@(1.4,30))){$d=$dp[0];Face $npc.position $d 1.2;Start-Sleep -Milliseconds 700;try{& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch $dp[1] -Quick|Out-Null}catch{}
     # Done only when a gameflow node completes (state 3 count rises), after any scene.
     $b=Snap host;I host X 600;$w2=[Diagnostics.Stopwatch]::StartNew()
     do{Start-Sleep -Milliseconds 600;$a=Snap host;if($a.logic-eq5){I host X 3000}
      if($a.gameflow-and$b.gameflow){if($st.expect){if($a.gameflow[3]-ge$st.expect){$hit=$true}}elseif($a.gameflow[3]-gt$b.gameflow[3]){$hit=$true}}}while(!$hit-and$w2.ElapsedMilliseconds-lt9000)
     if($hit){break}}
    if(!$hit){throw "Loot of $($st.name) not registered"}}}
  Start-Sleep -Milliseconds $(if($st.settle){$st.settle}else{3000})
  [void](Settle $tag 120);Start-Sleep 2
  $rows.Add([ordered]@{step=$tag;spec=$st;host=(Snap host);client=(Snap client)});foreach($r in 'host','client'){Frame $r $tag;try{Copy-Item "$root/work/lab-instances/$r/coop-status.json.triggers.json" "$out/$tag-triggers-$r.json";Copy-Item "$root/work/lab-instances/$r/coop-status.json.loading.json" "$out/$tag-loading-$r.json"}catch{}}
  Save "$out/route.json" @($rows)}
 $notes.final=@{host=(Snap host);client=(Snap client)}
 $notes.gameflowEqual=("$($notes.final.host.gameflow)"-eq"$($notes.final.client.gameflow)")
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  if($captures){foreach($c in @($captures)){try{if(!$c.HasExited){Stop-Process -Id $c.Id}}catch{}}}
  if($dirty){
   foreach($r in 'host','client'){try{I $r;C $r 'coop_product_auto off';C $r 'coop_mission_end_mirror off';C $r 'coop_gameflow_mirror off';C $r 'coop_kiscule_mirror off'}catch{}}
   # Product mode armed checkpoint sync: a shared death would block the fixture restore.
   foreach($r in 'host','client'){try{if((S $r).localGameLogicState-eq13){I $r A 400;Start-Sleep 5}}catch{}}
   foreach($r in 'host','client'){try{C $r 'coop_checkpoint_sync off';C $r 'coop_usable_sync off';C $r 'coop_player_health_hold off'}catch{}}
   $w=[Diagnostics.Stopwatch]::StartNew()
   do{$ok=$true;foreach($r in 'host','client'){try{$s=S $r;if($s.localGameLogicState-notin 1,2,6,13-or$s.localPendingGameLogicState){$ok=$false;if($s.localGameLogicState-eq5){I $r X 3000}elseif($s.localGameLogicState-eq3){I $r Right 250;Start-Sleep -Milliseconds 300;I $r A 600}elseif($s.localGameLogicState-eq14){I $r A 600}else{I $r A 300}}}catch{$ok=$false}};if(!$ok){Start-Sleep -Milliseconds 1500}}while(!$ok-and$w.ElapsedMilliseconds-lt120000)
   foreach($r in 'host','client'){try{if((S $r).localGameLogicState-eq13){I $r A 400}}catch{}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{$s=S $r;$hp=Q $r '--health' 1;if($s.localGameLogicState-ne1-or$hp.current-ne56-or!$s.remoteFresh){throw 'Final gameplay/health/transport not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;notes=$notes;scope="Campaign route steps for $MapInfo in lab product mode; Dunwall fixture restored."}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
