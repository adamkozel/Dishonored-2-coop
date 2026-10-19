param([string]$Take='window-sensor-v1527a',[string]$Regions='',[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo',[string]$Candidate='candidate-npc-combat-v1214em',[int]$MaxBeyond=60,[switch]$Shots,[string]$Yaws='0,45,90,135,180,225,270,315')
# 1527 (#11 wall_and_window_variants): find natural "window" geometry for the native window takedown with the native
# sensor itself (probe --window-sensor, 1214em; chapter 100). The sensor's shin sweep and opening queries run every frame
# whatever the target (0x1403d64c0, confirmed static), so no NPC is needed: the host is stood on a grid in each region
# (-Regions 'x,y,floorZ,radius,step;...') facing 8 headings, and a stand passes when the sensor has a window point (a wall
# under the sill within its sweep), the opening is free with a sill 0.9-1.1 m above the floor (+0x1e9, +0x1e4) and the
# player is grounded. For each pass (up to MaxBeyond) the far side is tested: the host is stood 0.7 m past the window
# point and the landing is recorded (an NPC must stand there at about the same level). Host only, health held; the
# client idles at its spawn. Read-only probes, setviewpos and the lab health hold; Dunwall fixture restored.
# -Shots (1527b): one frame of the host window from each passing stand (shot-<n>.jpg) for review.
# -Yaws (1527c): the headings tried at each stand (default every 45 degrees), for a fine local scan of one window.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$inv=[Globalization.CultureInfo]::InvariantCulture
$samples=[Collections.Generic.List[object]]::new();$beyond=[Collections.Generic.List[object]]::new();$modals=[Collections.Generic.List[object]]::new()
function Save($p,$v){$v|ConvertTo-Json -Depth 15|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed $r $mode"}
function I($r,[string]$b='Neutral',[int]$ms=100,[int]$ly=0){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms -LeftY $ly|Out-Null}
function ClearModals($tag){for($i=0;$i-lt4;$i++){$hit=$false;foreach($r in 'host','client'){$l=(S $r).localGameLogicState;if($l-in12,2){$hit=$true;$modals.Add(@{tag=$tag;role=$r;logic=$l});I $r B 200;Start-Sleep -Milliseconds 300;I $r}};if(!$hit){return};Start-Sleep -Milliseconds 900}}
function Pos{(Q host '--player').candidatePosition}
function Shot($name){$hwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64();$sp=Start-Process (Get-Command ffmpeg).Source -ArgumentList @('-hide_banner','-loglevel','warning','-nostdin','-y','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=10:width=-2:height=-2,hwdownload,format=bgra",'-frames:v','1','-q:v','4',"$out/shot-$name.jpg") -WindowStyle Hidden -PassThru;[void]$sp.WaitForExit(8000)}
$script:safe=$null
function Place($x,$y,$z,$yaw){C host ("setviewpos {0} {1} {2} {3} 0"-f([double]$x).ToString('F3',$inv),([double]$y).ToString('F3',$inv),([double]$z+1.7).ToString('F3',$inv),([double]$yaw).ToString('F2',$inv))
 for($i=0;$i-lt2;$i++){Start-Sleep -Milliseconds 110;$c=Pos;if($c[2]-lt$z-3.0){if($script:safe){C host ("setviewpos {0} {1} {2} 0 0"-f([double]$script:safe[0]).ToString('F3',$inv),([double]$script:safe[1]).ToString('F3',$inv),([double]$script:safe[2]+1.9).ToString('F3',$inv))};Start-Sleep -Milliseconds 600;return $null}}
 $c=Pos;$script:safe=$c;$c}
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
 [ordered]@{pid=$PID;operation='WindowSensor1527';take=$Take;started=(Get-Date).ToString('o');state='running'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;if((S $r).localGameLogicState-ne1){throw 'Gameplay required'}}
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 if((Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash-ne$pin.probe){throw "Expected the $Candidate probe"}
 $regionList=@($Regions-split';'|Where-Object {$_}|ForEach-Object {$v=@($_-split','|ForEach-Object {[double]$_});if($v.Count-ne5){throw 'Region needs x,y,floorZ,radius,step'};,$v})
 Save "$out/identity.json" @{candidate=$Candidate;dll=$pin.dll;probe=$pin.probe;mapInfo=$MapInfo;regions=$Regions}
 foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_checkpoint_sync off','coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off'){C $r $cmd}}
 $dirty=$true
 C host "campaign $MapInfo low corvo";C client "campaign $MapInfo low emily"
 $g=WaitGameplay 300000;Save "$out/load-log.json" $g.log;if(!$g.ready){throw 'Both instances did not reach the same mission gameplay'}
 C host 'coop_player_health_hold on'
 $first=Q host '--window-sensor';Save "$out/sensor-at-spawn.json" $first;if(!$first.enabled){throw 'Window sensor disabled at spawn'}
 $script:safe=Pos
 foreach($rg in $regionList){$n=[int][Math]::Floor($rg[3]/$rg[4])
  for($i=-$n;$i-le$n;$i++){for($j=-$n;$j-le$n;$j++){if([Math]::Sqrt($i*$i+$j*$j)*$rg[4]-gt$rg[3]){continue}
   if((S host).localGameLogicState-eq13){throw 'The host died during the scan (logic 13)'}
   $x=$rg[0]+$i*$rg[4];$y=$rg[1]+$j*$rg[4]
   foreach($yw in @($Yaws-split','|ForEach-Object {[double]$_})){$c=Place $x $y $rg[2] $yw;if(!$c){$samples.Add([ordered]@{x=$x;y=$y;yaw=$yw;drop=$true});break}
    Start-Sleep -Milliseconds 120;$w=Q host '--window-sensor'
    $row=[ordered]@{x=$x;y=$y;yaw=$yw;at=$c;point=$(if($w.hasPoint){$w.point}else{$null});sill=$w.sill;sillOk=$w.sillOk;grounded=$w.grounded;angleOk=$w.angleOk;target=$w.targetName;forward=$w.forward}
    $row.pass=($w.hasPoint-and$w.sillOk-and$w.grounded-and$w.sill-ge0.9-and$w.sill-le1.1);$samples.Add($row)}
   if($samples.Count%64-lt8){ClearModals 'scan';Save "$out/samples-progress.json" @($samples)}}}}
 Save "$out/samples.json" @($samples)
 $passes=@($samples|Where-Object {$_.pass})
 foreach($p in @($passes|Select-Object -First $MaxBeyond)){$fx=[double]$p.forward[0];$fy=[double]$p.forward[1];$fl=[Math]::Max(.1,[Math]::Sqrt($fx*$fx+$fy*$fy));$fx/=$fl;$fy/=$fl
  $bx=[double]$p.point[0]+0.7*$fx;$by=[double]$p.point[1]+0.7*$fy;$c=Place $bx $by ([double]$p.at[2]) $p.yaw
  $n=$beyond.Count;if($Shots){$null=Place $p.x $p.y ([double]$p.at[2]) $p.yaw;Start-Sleep -Milliseconds 400;Shot $n;$null=Place $bx $by ([double]$p.at[2]) $p.yaw}
  $beyond.Add([ordered]@{n=$n;x=$p.x;y=$p.y;yaw=$p.yaw;stand=$p.at;point=$p.point;sill=$p.sill;far=@($bx,$by);landing=$c;dz=$(if($c){[Math]::Round([double]$c[2]-[double]$p.at[2],3)}else{$null});push=$(if($c){[Math]::Round([Math]::Sqrt(([double]$c[0]-$bx)*([double]$c[0]-$bx)+([double]$c[1]-$by)*([double]$c[1]-$by)),3)}else{$null})})}
 Save "$out/beyond.json" @($beyond)
 $good=@($beyond|Where-Object {$null-ne$_.dz-and[Math]::Abs($_.dz)-le0.45-and$_.push-lt0.3})
 Save "$out/verified.json" @{samples=$samples.Count;passes=$passes.Count;sameLevelFarSide=@($good)}
 if(!$passes.Count){$failure='No stand passed the window sensor in the scanned regions'}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  if($dirty){
   try{C host 'coop_player_health_hold off'}catch{}
   $cw=[Diagnostics.Stopwatch]::StartNew()
   do{$off=$true;foreach($r in 'host','client'){try{I $r;C $r 'coop_product_auto off';C $r 'coop_movement_resume off';foreach($cmd in 'coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off','coop_shadow_stream off','coop_movement_bank off','coop_trigger_mirror off'){C $r $cmd};C $r 'coop_checkpoint_sync off';$st=S $r
     if((S $r '.reset.json').enabled){$off=$false;if($st.localGameLogicState-in8,15){I $r A 300}elseif($st.localGameLogicState-eq2){I $r B 200}}}catch{}}
    if(!$off){Start-Sleep -Milliseconds 1500}}while(!$off-and$cw.ElapsedMilliseconds-lt60000)
   foreach($r in 'host','client'){try{if((S $r).localGameLogicState-in2,12){I $r B 200;Start-Sleep -Milliseconds 800}}catch{}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{$s=S $r;$hp=Q $r '--health' 1;if($s.localGameLogicState-ne1-or$hp.current-ne56-or!$s.remoteFresh){throw 'Final gameplay/health/transport not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;samples=$samples.Count;passes=@($samples|Where-Object {$_.pass}).Count;beyond=@($beyond);scope='Native window-takedown sensor scan (geometry only, no NPC): grid stands x 8 headings per region, far side landing for passes; host only, health held; Dunwall fixture restored.'}
  [ordered]@{pid=$PID;operation='WindowSensor1527';take=$Take;ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json"|Select-Object -First 5;if($failure){throw $failure}
