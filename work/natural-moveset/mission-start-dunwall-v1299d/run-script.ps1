param([string]$Take='mission-start-dunwall-v1299a',[string]$MapInfo='campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo',[ValidateSet('low','high')][string]$Chaos='low',[string]$Candidate='candidate-npc-combat-v1214bf',[int]$LoadSeconds=300)
# Campaign route scouting (chapter 97): both lab worlds load a mission's start
# in lab product mode (coop_product_auto) with the mission end mirror; native
# scenes are skipped with the ordinary hold-to-skip (cinematic sync pairs the
# skip). Records the load sequence, where each player starts, gameflow node
# counts by state, the trigger trace and one frame per window. Dunwall
# fixture restored.
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
 [ordered]@{logic=$s.localGameLogicState;pending=$s.localPendingGameLogicState;map=$s.localMap;epoch=$s.localEpoch;cine=$s.localCinematicEpoch;canSkip=$s.localCinematicCanSkip;pos=$s.localPosition;fresh=$s.remoteFresh;gameflow=$(if($m){$m.gameflowStates}else{$null})}}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;if((S $r).localGameLogicState-ne1){throw 'Gameplay required'}}
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 Save "$out/identity.json" @{dll=$pin.dll;candidate=$Candidate;mapInfo=$MapInfo;chaos=$Chaos}
 $dirty=$true
 foreach($r in 'host','client'){C $r 'coop_mission_end_mirror on';C $r 'coop_product_auto on'}
 C host "campaign $MapInfo $Chaos corvo";C client "campaign $MapInfo $Chaos emily"
 $w=[Diagnostics.Stopwatch]::StartNew();$log=[Collections.Generic.List[object]]::new();$last=@{host=0;client=0};$frames=@{};$choice=@{host=0;client=0};$confirm=@{host=0;client=0}
 do{
  Start-Sleep -Milliseconds 700;$ready=$true;$row=[ordered]@{ms=$w.ElapsedMilliseconds}
  foreach($r in 'host','client'){$x=Snap $r;$row[$r]=$x
   if($x.logic-ne1-or$x.pending){$ready=$false}
   if(!$frames["$r-$($x.logic)"]-and$x.logic-in 5,6,11,12,13){$frames["$r-$($x.logic)"]=$true;Frame $r "logic$($x.logic)"}
   if($x.logic-eq14-and$w.ElapsedMilliseconds-$last[$r]-gt4000){
    # Native character choice (logic 14): each world picks its own character.
    $choice[$r]++;$dir=if($r-eq'host'){'Left'}else{'Right'}
    if($choice[$r]-le4){I $r $dir 250;Start-Sleep -Milliseconds 400;Frame $r "choice$($choice[$r])";I $r A 400}else{I $r A 2000}
    $last[$r]=$w.ElapsedMilliseconds}
   elseif($x.logic-eq3-and$w.ElapsedMilliseconds-$last[$r]-gt3000){
    # The confirmation defaults to No: move to Yes, then confirm.
    $confirm[$r]++;if($confirm[$r]-le3){Frame $r "logic3-$($confirm[$r])"};I $r Right 250;Start-Sleep -Milliseconds 400;I $r A 400;$last[$r]=$w.ElapsedMilliseconds}
   elseif($x.logic-in 8,15,12-and$w.ElapsedMilliseconds-$last[$r]-gt3000){I $r A 300;$last[$r]=$w.ElapsedMilliseconds}
   elseif($x.logic-eq5-and$w.ElapsedMilliseconds-$last[$r]-gt4000){I $r X 3000;$last[$r]=$w.ElapsedMilliseconds}}
  $log.Add($row)
 }while(!$ready-and$w.ElapsedMilliseconds-lt($LoadSeconds*1000))
 Save "$out/load-log.json" @($log)
 if(!$ready){throw 'Both worlds did not reach mission gameplay'}
 Start-Sleep 4
 foreach($r in 'host','client'){$notes[$r]=Snap $r;Save "$out/player-$r.json" (Q $r '--player');Save "$out/triggers-$r.json" (S $r '.triggers.json');Frame $r 'gameplay'}
 $states=@{host=[Collections.Generic.List[string]]::new();client=[Collections.Generic.List[string]]::new()}
 foreach($x in $log){foreach($r in 'host','client'){$v="$($x[$r].logic)/$($x[$r].pending)@$($x[$r].map)";if($states[$r].Count-eq0-or$states[$r][$states[$r].Count-1]-ne$v){$states[$r].Add($v)}}}
 $notes.hostStates=@($states.host);$notes.clientStates=@($states.client);$notes.loadMs=$w.ElapsedMilliseconds
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  if($dirty){
   foreach($r in 'host','client'){try{I $r;C $r 'coop_product_auto off';C $r 'coop_mission_end_mirror off'}catch{}}
   $w=[Diagnostics.Stopwatch]::StartNew()
   do{$ok=$true;foreach($r in 'host','client'){try{$s=S $r;if($s.localGameLogicState-notin 1,2,6,13-or$s.localPendingGameLogicState){$ok=$false;if($s.localGameLogicState-eq5){I $r X 3000}elseif($s.localGameLogicState-eq3){I $r Right 250;Start-Sleep -Milliseconds 300;I $r A 600}elseif($s.localGameLogicState-eq14){I $r A 600}else{I $r A 300}}}catch{$ok=$false}};if(!$ok){Start-Sleep -Milliseconds 1500}}while(!$ok-and$w.ElapsedMilliseconds-lt120000)
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{$s=S $r;$hp=Q $r '--health' 1;if($s.localGameLogicState-ne1-or$hp.current-ne56-or!$s.remoteFresh){throw 'Final gameplay/health/transport not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;notes=$notes;scope="Mission start scouting for $MapInfo in lab product mode; Dunwall fixture restored."}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
