param([string]$Take='state-trigger-v1585a',[string]$Frame='',[switch]$Film,[string]$MapInfo='',[string]$Role='host',[string]$Triggers='RightThumb:1500;Back:800',[int]$SampleMs=120,[int]$SettleMs=1500,[string]$Candidate='candidate-npc-combat-v1214es')
# 1585 (#102): -Frame 'sx,sy,sz,syaw;px,py,pz,pyaw;cx,cy,cz' stands the source and the partner and holds the partner's
# observer camera at a known-good spot once before the triggers (default use: the Edge of the World sash-window framing of
# takes 1573/1576, the source in the room seen from the terrace); -Film then records each trigger without re-aiming.
# 1584 (#102): the partner films from where it stands: it is turned to face the source (setviewpos at its own position),
# without the held observer camera (1583: the camera placed 3 m along the source's heading faced away from it).
# 1583 (#102): -Film records both windows through each trigger, the partner's observer camera held 3 m in front of the
# source looking back at it (the partner first stood facing the source), so the partner's view of the state is reviewable.
# 1581 (#102): Probe-StateTrigger-v1580 with -MapInfo: both instances load the mission map first (low chaos, host Corvo,
# client Emily) and the Dunwall fixture is restored afterwards, as Survey-Sitting-v1528 does.
# 1580 (#102): which native player states an input enters, read-only. In the current lab fixture each trigger
# 'Button[+Button]:holdMs' (or 'LT:ms' / 'RT:ms' for the triggers) is held on -Role while --player-states is sampled
# every SampleMs until SettleMs after the release; every active state id with its RTTI class is recorded per trigger.
# A modal game logic state (menus, journal) is closed with B afterwards. No commands, no grants; Dunwall fixture kept.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$pids=@{};$rows=[Collections.Generic.List[object]]::new()
function Save($p,$v){$v|ConvertTo-Json -Depth 12|Set-Content -LiteralPath $p}
function S($r,$suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed $r $mode"}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,[string]$b='Neutral',[int]$ms=100,[int]$ly=0){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms -LeftY $ly|Out-Null}
function WaitGameplay($limitMs){$w=[Diagnostics.Stopwatch]::StartNew();$log=[Collections.Generic.List[object]]::new();$last=@{host=0;client=0}
 do{Start-Sleep -Milliseconds 500;$ready=$true
  foreach($r in 'host','client'){$s=S $r;$log.Add(@{ms=$w.ElapsedMilliseconds;role=$r;logic=$s.localGameLogicState;pending=$s.localPendingGameLogicState;map=$s.localMapId})
   if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}
   if($s.localGameLogicState-in8,15-and$w.ElapsedMilliseconds-gt5000-and$w.ElapsedMilliseconds-$last[$r]-gt1500){I $r A 300;$last[$r]=$w.ElapsedMilliseconds}
   elseif($s.localGameLogicState-eq5-and$w.ElapsedMilliseconds-$last[$r]-gt4000){I $r X 3000;$last[$r]=$w.ElapsedMilliseconds}
   elseif($s.localGameLogicState-eq12-and$w.ElapsedMilliseconds-$last[$r]-gt1500){I $r B 200;$last[$r]=$w.ElapsedMilliseconds}}
  if($ready-and(S host).localMapId-ne(S client).localMapId){$ready=$false}
 }while(!$ready-and$w.ElapsedMilliseconds-lt$limitMs);return @{ready=$ready;log=@($log)}}
function Capture($key,$seconds){$list=@();foreach($pair in @(@($Role,'source'),@($peer,'observer'))){$hwnd=(Get-Process -Id $pids[$pair[0]]).MainWindowHandle.ToInt64()
  $list+=Start-Process (Get-Command ffmpeg).Source -ArgumentList @('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',[string]$seconds,'-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$key-$($pair[1]).mp4") -WindowStyle Hidden -RedirectStandardError "$out/$key-$($pair[1])-capture.log" -RedirectStandardOutput "$out/$key-$($pair[1])-capture.stdout" -PassThru};$list}
$peer=if($Role-eq'host'){'client'}else{'host'};$inv=[Globalization.CultureInfo]::InvariantCulture
$dirty=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 [ordered]@{pid=$PID;operation='StateTrigger1580';take=$Take;started=(Get-Date).ToString('o');state='running'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;if((S $r).localGameLogicState-ne1){throw 'Gameplay required'}}
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 Save "$out/identity.json" @{candidate=$Candidate;dll=$pin.dll;role=$Role;triggers=$Triggers}
 if($MapInfo){foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_checkpoint_sync off','coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off'){C $r $cmd}};$dirty=$true
  C host "campaign $MapInfo low corvo";C client "campaign $MapInfo low emily"
  $g=WaitGameplay 300000;Save "$out/load-log.json" $g.log;if(!$g.ready){throw 'Map did not reach gameplay'};Start-Sleep -Milliseconds 1500}
 Save "$out/state-table.json" (Q $Role '--player-state-table')
 if($Frame){$fr=@($Frame-split';'|ForEach-Object {,@($_-split','|ForEach-Object {[double]$_})})
  C $Role ('setviewpos {0} {1} {2} {3} 0' -f $fr[0][0].ToString('F3',$inv),$fr[0][1].ToString('F3',$inv),$fr[0][2].ToString('F3',$inv),$fr[0][3].ToString('F2',$inv))
  C $peer ('setviewpos {0} {1} {2} {3} 0' -f $fr[1][0].ToString('F3',$inv),$fr[1][1].ToString('F3',$inv),$fr[1][2].ToString('F3',$inv),$fr[1][3].ToString('F2',$inv));Start-Sleep -Milliseconds 500
  C $peer ('coop_observer_camera hold_at {0} {1} {2}' -f $fr[2][0].ToString('F3',$inv),$fr[2][1].ToString('F3',$inv),$fr[2][2].ToString('F3',$inv));Start-Sleep -Milliseconds 800;$dirty=$true}
 foreach($spec in @($Triggers-split';'|Where-Object {$_})){$name,$ms=$spec-split':';$ms=[int]$ms
  $before=@((Q $Role '--player-states').active|ForEach-Object {"$($_.id):$($_.type)"});$seen=[ordered]@{};$trace=[Collections.Generic.List[object]]::new()
  $ia=@{Role=$Role;DurationMs=$ms};if($name-eq'LT'){$ia.LeftTrigger=255}elseif($name-eq'RT'){$ia.RightTrigger=255}else{$ia.Button=@($name-split'\+')}
  $caps=@();if($Film-and$Frame){$caps=@(Capture "trigger-$($rows.Count+1)" ([Math]::Ceiling(($ms+$SettleMs+1500)/1000)));Start-Sleep -Milliseconds 700}elseif($Film){$cp=Q $Role '--player';$pp=[double[]]$cp.candidatePosition;$po=[double[]]$cp.candidateOrientation;$fx=$po[0];$fy=$po[1];$fl=[Math]::Max(.1,[Math]::Sqrt($fx*$fx+$fy*$fy));$fx/=$fl;$fy/=$fl
   $qp=[double[]](Q $peer '--player').candidatePosition;$yaw=[Math]::Atan2($pp[1]-$qp[1],$pp[0]-$qp[0])*180/[Math]::PI
   C $peer ('setviewpos {0} {1} {2} {3} 0' -f $qp[0].ToString('F3',$inv),$qp[1].ToString('F3',$inv),($qp[2]+1.7).ToString('F3',$inv),$yaw.ToString('F2',$inv));Start-Sleep -Milliseconds 400
   $dirty=$true
   $caps=@(Capture "trigger-$($rows.Count+1)" ([Math]::Ceiling(($ms+$SettleMs+1500)/1000)));Start-Sleep -Milliseconds 700}
  & "$lab/Send-D2Input.ps1" @ia|Out-Null;$w=[Diagnostics.Stopwatch]::StartNew()
  while($w.ElapsedMilliseconds-lt($ms+$SettleMs)){$ps=try{Q $Role '--player-states'}catch{$null}
   if($ps){$ids=@($ps.active|ForEach-Object {"$($_.id):$($_.type)"});$trace.Add([ordered]@{ms=$w.ElapsedMilliseconds;active=$ids;stance=$ps.stance;logic=(S $Role).localGameLogicState});foreach($x in $ids){if(!$seen.Contains($x)){$seen[$x]=$w.ElapsedMilliseconds}}}
   Start-Sleep -Milliseconds $SampleMs}
  $logic=(S $Role).localGameLogicState;$closed=0;while($logic-ne1-and$closed-lt4){& "$lab/Send-D2Input.ps1" -Role $Role -Button B -DurationMs 200|Out-Null;Start-Sleep -Milliseconds 900;$closed++;$logic=(S $Role).localGameLogicState}
  foreach($cpx in $caps){try{$cpx.WaitForExit(15000)|Out-Null}catch{}};if($Film-and!$Frame){C $peer 'coop_observer_camera off'}
  $rows.Add([ordered]@{trigger=$spec;before=$before;entered=@($seen.Keys|Where-Object {$_-notin$before});firstSeenMs=$seen;closedWithB=$closed;logicAfter=$logic});Save "$out/trace-$($rows.Count).json" @($trace)
  Start-Sleep -Milliseconds 600}
 Save "$out/rows.json" @($rows);if($Frame){C $peer 'coop_observer_camera off'}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}
finally{
 if($owned){$errors=[Collections.Generic.List[string]]::new()
  if($dirty){
   $cw=[Diagnostics.Stopwatch]::StartNew()
   do{$off=$true;foreach($r in 'host','client'){try{I $r;C $r 'coop_product_auto off';C $r 'coop_movement_resume off';foreach($cmd in 'coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off','coop_shadow_stream off','coop_movement_bank off','coop_trigger_mirror off'){C $r $cmd};C $r 'coop_checkpoint_sync off';$st=S $r
     if((S $r '.reset.json').enabled){$off=$false;if($st.localGameLogicState-in8,15){I $r A 300}elseif($st.localGameLogicState-eq2){I $r B 200}}}catch{}}
    if(!$off){Start-Sleep -Milliseconds 1500}}while(!$off-and$cw.ElapsedMilliseconds-lt60000)
   foreach($r in 'host','client'){try{if((S $r).localGameLogicState-in2,12){I $r B 200;Start-Sleep -Milliseconds 800}}catch{}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{& "$lab/Send-D2Input.ps1" -Role $r -Button Neutral -DurationMs 100|Out-Null;$s=S $r;$hp=Q $r '--health' 1;if($s.localGameLogicState-ne1-or$hp.current-ne56-or!$s.remoteFresh){throw 'Final gameplay/health/transport not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;rows=@($rows);scope='Native player states entered by held inputs in the current lab fixture (read-only probes; no commands).'}
  [ordered]@{pid=$PID;operation='StateTrigger1580';take=$Take;ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$root/work/movement-integration/native-operation-owner.json"
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json"|Select-Object -First 3;if($failure){throw $failure}
