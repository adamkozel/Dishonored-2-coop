param([string]$Take='route-dunwall-v1300a',[string]$MapInfo='campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo',[ValidateSet('low','high')][string]$Chaos='low',[string]$Candidate='candidate-npc-combat-v1214bf',[Parameter(Mandatory)][string]$StepsFile,[int]$LoadSeconds=300)
# Campaign route take (chapter 97): both lab worlds load a mission start in lab
# product mode with the mission end mirror, pass the native character choice
# (host Corvo, client Emily), then the host runs a step list:
#   {"do":"trigger","name":<arkTrigger>}            stand in the volume
#   {"do":"use","name":<entity>,"z":<eye z>?}       stand beside it and press X
#   {"do":"at","pos":[x,y,z],"yaw":<deg>}           move there
#   {"do":"wait","ms":<n>}
#   {"do":"attack","name":<npc regex>,"near":[x,y],"max":<presses>}  fight it until its health is 0
#   {"do":"loot","name":<npc regex>,"near":[x,y]}  use the body (search)
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
 [ordered]@{logic=$s.localGameLogicState;pending=$s.localPendingGameLogicState;map=$s.localMap;epoch=$s.localEpoch;cine=$s.localCinematicEpoch;pos=$s.localPosition;fresh=$s.remoteFresh;gameflow=$(if($m){$m.gameflowStates}else{$null})}}
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
 foreach($r in 'host','client'){C $r 'coop_mission_end_mirror on';C $r 'coop_product_auto on'}
 C host "campaign $MapInfo $Chaos corvo";C client "campaign $MapInfo $Chaos emily"
 if(!(Settle 'start' $LoadSeconds)){throw 'Both worlds did not reach mission gameplay'}
 C host 'coop_player_health_hold on';C client 'coop_player_health_hold on'
 Start-Sleep 3;$rows=[Collections.Generic.List[object]]::new();$rows.Add([ordered]@{step='start';host=(Snap host);client=(Snap client)});foreach($r in 'host','client'){Frame $r 'step00-start'}
 $i=0
 function Npc($rx,$near){$all=@((Q host '--entities').entities|Where-Object {$_.type-eq'.?AVarkNPC@@'-and$_.name-match$rx})
  if(!$all.Count){throw "No NPC matches $rx"};if($near){$all=@($all|Sort-Object {[Math]::Pow($_.position[0]-$near[0],2)+[Math]::Pow($_.position[1]-$near[1],2)})};$all[0]}
 function Face($p,$dist,$eyeUp){$h=(Snap host).pos;$dx=$h[0]-$p[0];$dy=$h[1]-$p[1];$n=[Math]::Max([Math]::Sqrt($dx*$dx+$dy*$dy),.001)
  $x=$p[0]+$dist*$dx/$n;$y=$p[1]+$dist*$dy/$n;$yaw=[Math]::Atan2($p[1]-$y,$p[0]-$x)*180/[Math]::PI
  C host ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$x,$y,($p[2]+$eyeUp),$yaw)}
 foreach($st in $steps){$i++;$tag='step{0:D2}-{1}'-f$i,$st.do;$ents=(Q host '--entities').entities
  switch($st.do){
   'trigger'{$e=@($ents|Where-Object name -eq $st.name);if($e.Count-ne1){throw "Missing trigger $($st.name)"};$p=$e[0].position;C host ("setviewpos {0:F3} {1:F3} {2:F3} 0 0"-f$p[0],$p[1],($p[2]+1.0))}
   'at'{C host ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$st.pos[0],$st.pos[1],$st.pos[2],$(if($st.yaw){$st.yaw}else{0}))}
   'use'{$e=@($ents|Where-Object name -eq $st.name);if($e.Count-ne1){throw "Missing usable $($st.name)"};$p=$e[0].position;$z=if($st.z){$st.z}else{$p[2]+1.0};$hit=$false
    foreach($o in @(@(0.9,0),@(-0.9,0),@(0,0.9),@(0,-0.9))){$x=$p[0]+$o[0];$y=$p[1]+$o[1];$yaw=[Math]::Atan2($p[1]-$y,$p[0]-$x)*180/[Math]::PI
     C host ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$x,$y,$z,$yaw);Start-Sleep -Milliseconds 900;$b=Snap host;I host X 250;Start-Sleep -Milliseconds 1500;$a=Snap host
     if($a.logic-eq13){throw "Host died at $($st.name)"};if($a.cine-ne$b.cine-or$a.logic-ne1-or"$($a.gameflow)"-ne"$($b.gameflow)"){$hit=$true;break}}
    if(!$hit){throw "Use of $($st.name) not registered"}}
   'wait'{Start-Sleep -Milliseconds $st.ms}
   'attack'{$npc=Npc $st.name $st.near;$hd=[Convert]::ToInt64($npc.handle.Substring(2),16);$trace=[Collections.Generic.List[object]]::new();$max=if($st.max){$st.max}else{40};$dead=$false
    for($k=0;$k-lt$max-and!$dead;$k++){if($k%4-eq0){$npc=Npc $st.name $st.near;Face $npc.position 1.5 1.6;Start-Sleep -Milliseconds 500}
     & "$lab/Send-D2Input.ps1" -Role host -RightTrigger 255 -DurationMs 150|Out-Null;Start-Sleep -Milliseconds 700
     $hp=$null;try{$hp=Q host '--health' $hd}catch{};$trace.Add(@{press=$k;health=$(if($hp){$hp.current}else{$null});host=(Snap host).logic});if($hp-and$hp.current-le0){$dead=$true}
     if((Snap host).logic-eq13){throw 'Host died in the fight'}}
    Save "$out/$tag-fight.json" @($trace);if(!$dead){throw "$($st.name) still alive after $max attacks"}}
   'loot'{$npc=Npc $st.name $st.near;$hit=$false
    foreach($d in 1.0,0.7,1.3){Face $npc.position $d 1.2;Start-Sleep -Milliseconds 700;& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch 45 -Quick|Out-Null
     $b=Snap host;I host X 600;Start-Sleep -Milliseconds 1800;$a=Snap host;if($a.cine-ne$b.cine-or$a.logic-ne1-or"$($a.gameflow)"-ne"$($b.gameflow)"){$hit=$true;break}}
    if(!$hit){throw "Loot of $($st.name) not registered"}}}
  Start-Sleep -Milliseconds $(if($st.settle){$st.settle}else{3000})
  [void](Settle $tag 120);Start-Sleep 2
  $rows.Add([ordered]@{step=$tag;spec=$st;host=(Snap host);client=(Snap client)});foreach($r in 'host','client'){Frame $r $tag;try{Copy-Item "$root/work/lab-instances/$r/coop-status.json.triggers.json" "$out/$tag-triggers-$r.json"}catch{}}
  Save "$out/route.json" @($rows)}
 $notes.final=@{host=(Snap host);client=(Snap client)}
 $notes.gameflowEqual=("$($notes.final.host.gameflow)"-eq"$($notes.final.client.gameflow)")
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  if($dirty){
   foreach($r in 'host','client'){try{I $r;C $r 'coop_player_health_hold off';C $r 'coop_product_auto off';C $r 'coop_mission_end_mirror off'}catch{}}
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
