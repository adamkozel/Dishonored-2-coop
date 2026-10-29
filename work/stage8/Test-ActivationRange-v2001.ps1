param([string]$Take='activation-range-v2001a',[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo',[ValidateSet('low','high')][string]$Chaos='low',[int]$PhaseSeconds=60)
# Stage 8.1 (#234): the activation range. Both roles load Edge of the World (host Corvo, client Emily).
# Phases, sampling `d2_runtime_probe --npc-activity` (read-only) in both worlds about once a second:
#   spawn     both players at the docks spawn, NPC stream on
#   far       the partner (client) ~205 m into the interior (take 1168's far point), the host stays at spawn, stream on
#   far-noauth the same positions with the NPC stream off (pure native behaviour in each world)
# Both windows are recorded during `far`. Analysis: analyze_activation_range.py. Dunwall fixture restored.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$captures=[Collections.Generic.List[object]]::new();$rows=[Collections.Generic.List[object]]::new()
function Save($p,$v){$v|ConvertTo-Json -Depth 15|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed $r $mode"}
function I($r,[string]$b='',[int]$ms=100){$a=@{Role=$r;DurationMs=$ms};if($b){$a.Button=$b};& "$lab/Send-D2Input.ps1" @a|Out-Null}
function Capture($name,$seconds){foreach($r in 'host','client'){$hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64()
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',[string]$seconds,'-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$name-$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$name-$r-capture.log" -PassThru))}}
function WaitCaptures(){foreach($c in @($captures)){if(!$c.WaitForExit(120000)-or$c.ExitCode){throw 'Recording failed'}};$captures.Clear()}
function Sample($phase,[int]$seconds){
 $w=[Diagnostics.Stopwatch]::StartNew();$n=0;$failed=0
 while($w.ElapsedMilliseconds-lt($seconds*1000)){$tick=$w.ElapsedMilliseconds
  foreach($r in 'host','client'){$raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$r] '--npc-activity'
   if($LASTEXITCODE){$failed++}else{Add-Content -LiteralPath "$out/activity-$phase-$r.jsonl" -Value ("{""phase"":""$phase"",""role"":""$r"",""ms"":$($w.ElapsedMilliseconds),""sample"":$raw}")}}
  $n++;$left=1000-($w.ElapsedMilliseconds-$tick);if($left-gt0){Start-Sleep -Milliseconds $left}}
 $row=@{phase=$phase;samples=$n;failedReads=$failed;seconds=$seconds;hostStream=(S host '.npc-stream.json').session;clientStream=(S client '.npc-stream.json').session}
 $rows.Add($row);Save "$out/phase-$phase.json" $row
 if($failed-gt($n/2)){throw "Probe failed too often in $phase ($failed of $(2*$n))"}}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;if((S $r).localGameLogicState-ne1){throw 'Gameplay required'}}
 $pin=Get-Content "$PSScriptRoot/candidate-probe-activation-v2001/identity.json" -Raw|ConvertFrom-Json
 if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Expected the 1214kq DLL'}
 if((Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash-ne$pin.probe){throw 'Expected the v2001 probe'}
 Save "$out/identity.json" @{dll=$pin.dll;dllCandidate=$pin.dllCandidate;probe=$pin.probe;mapInfo=$MapInfo;chaos=$Chaos;phaseSeconds=$PhaseSeconds;pids=$pids}
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
 # Mode assertion first (coop-anything QA rule): two live peers on the same map, both in gameplay.
 foreach($r in 'host','client'){$s=S $r;if(!$s.remoteFresh){throw "Transport not fresh on mission map ($r)"}}
 $spawn=@{};foreach($r in 'host','client'){$spawn[$r]=(Q $r '--player').candidatePosition}
 Save "$out/spawn-positions.json" $spawn
 foreach($r in 'host','client'){I $r;C $r 'coop_npc_stream off'}
 foreach($r in 'host','client'){C $r 'coop_npc_stream on'}
 Start-Sleep -Seconds 3
 Sample 'spawn' 30
 $far=@(62.226,4.468,34.6)
 C client ("setviewpos {0:F3} {1:F3} {2:F3} 0 0"-f$far[0],$far[1],$far[2])
 Start-Sleep -Seconds 3
 Save "$out/far-positions.json" @{host=(Q host '--player').candidatePosition;client=(Q client '--player').candidatePosition}
 Capture 'far' ($PhaseSeconds+2)
 Sample 'far' $PhaseSeconds
 WaitCaptures
 foreach($r in 'host','client'){C $r 'coop_npc_stream off'}
 Start-Sleep -Seconds 3
 Sample 'far-noauth' $PhaseSeconds
 Save "$out/verified.json" @{measured=$true;scope='Activation-range samples recorded; the decision comes from analyze_activation_range.py.'}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  foreach($c in @($captures)){try{if(!$c.HasExited-and!$c.WaitForExit(30000)){Stop-Process -Id $c.Id}}catch{}}
  if($dirty){
   foreach($r in 'host','client'){try{I $r;C $r 'coop_npc_stream off'}catch{}}
   foreach($r in 'host','client'){try{if((S $r).localGameLogicState-eq12){I $r B 200;Start-Sleep -Milliseconds 800}}catch{}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$root/work/natural-moveset/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{$s=S $r;$hp=Q $r '--health' 1;Save "$out/final-$r.json" @{bridge=$s;health=$hp};if($s.localGameLogicState-ne1-or$hp.current-ne56-or!$s.remoteFresh){throw 'Final gameplay/health/transport not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;phases=@($rows);scope='Activation-range measurement on Edge of the World: spawn, partner far with NPC authority, partner far without; Dunwall fixture restored.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
