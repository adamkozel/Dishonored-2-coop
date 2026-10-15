param([string]$StatesProbe='work/build/Release/d2_runtime_probe.exe',[string]$Take='window-mantle-states-v1237c',[string]$Candidate='candidate-npc-combat-v1213j',[string]$Probe='climbable-windows-v1231b',[string]$Window='script_rich_small_01_sndctrl')
# 1237c: 1237b named the scene: the window mantle plays the known mantle_low_crouch.midnightscene (phase 3,
# 500 ms, Scene_2, the player bound alone), yet the reader stopped at stage 4. The build probe (research
# build, hash recorded) adds traversalResearch to --player-states: kind, next exit, current capsule, the
# scene asset vs the asset the reader expected, subscene, duration, bindings, arms and the refusing line.
# 1237b: 1237 showed a lone native Mantle (id 8, ~620 ms) then AutoCrouch 10 and Sneaking 6, with the
# traversal reader stopping at stage 4 (scene asset/phase/duration); its second attempt fell to death from
# the start spot. One crossing only, with d2_runtime_probe --watch-midnight 3000 recording the native
# midnight scene instances (asset names, durations) in parallel.
# 1237 (#7 diagnostic, read-only probes): why the source traversal reader does not classify the window
# mantle (1235c: both crossings mantled natively, but the partner showed only the jump/air presentation,
# traversalVariant 0). The host alone loads Edge of the World with its partner, starts at the probe's spot
# and pushes forward + jump through script_rich_small_01_sndctrl while the FULL d2_runtime_probe
# --player-states output (active states, traversalSceneValid, traversalReaderStage, ...) is recorded every
# ~80 ms into mantle-states.ndjson. No product controller. Dunwall fixture restored.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab"
$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$captures=[Collections.Generic.List[object]]::new();$rows=[Collections.Generic.List[object]]::new();$samplers=[Collections.Generic.List[object]]::new()
function Save($p,$v){$v|ConvertTo-Json -Depth 15|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed $r $mode"}
function F($r){for($i=0;$i-lt6;$i++){$raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$r] --focus;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Focus query failed $r"}
function I($r,[string]$b='Neutral',[int]$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}
function ProductStatus($r){S $r '.product.json'}
function Live($r){$s=S $r;$m=S $r '.movement.json';$x=S $r '.shadow-stream.json';$a=$s.probeRenderPosition;$b=$s.remotePosition
 $d=if($a-and$b){[Math]::Sqrt(($a[0]-$b[0])*($a[0]-$b[0])+($a[1]-$b[1])*($a[1]-$b[1])+($a[2]-$b[2])*($a[2]-$b[2]))}else{999}
 @{follow=$s.probeFollowEnabled;distance=[Math]::Round($d,3);product=(S $r '.product.json').result;bank=[bool]$m.active;bankVersion=$m.version;stream=[bool]($x.armed-and$x.ready);ok=($s.probeActorHandle-and$s.probeFollowEnabled-and$d-lt1.0-and$m.active-and$x.armed-and$x.ready-and$s.remoteFresh)}}
function WaitLive($tag,$limit=30000){$w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 300;$p=@{host=(Live host);client=(Live client)};$ok=$p.host.ok-and$p.client.ok}while(!$ok-and$w.ElapsedMilliseconds-lt$limit);$p.ok=$ok;$p.waitMs=$w.ElapsedMilliseconds;Save "$out/live-$tag.json" $p;return $p}
$modals=[Collections.Generic.List[object]]::new()
function ClearModals($tag){for($i=0;$i-lt4;$i++){$hit=$false;foreach($r in 'host','client'){$l=(S $r).localGameLogicState;if($l-in12,2){$hit=$true;$modals.Add(@{tag=$tag;role=$r;logic=$l;at=[DateTime]::Now.ToString('o')});I $r B 200;Start-Sleep -Milliseconds 300;I $r}};if(!$hit){return};Start-Sleep -Milliseconds 900}}
function Pitch($r,$deg){try{& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $r -Pitch $deg -Quick|Out-Null;$true}catch{$false}}
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
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;if((S $r).localGameLogicState-ne1){throw 'Gameplay required'}}
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 $probeResult=Get-Content (Join-Path $PSScriptRoot "$Probe/result.json") -Raw|ConvertFrom-Json;$w=@($probeResult.windows|Where-Object name -eq $Window)[0]
 $a=@($w.attempts|Where-Object crossed)[0];if(!$a){throw "Probe $Probe has no crossing for $Window"}
 Save "$out/identity.json" @{candidate=$Candidate;dll=$pin.dll;probe=$Probe;window=$Window;attempt=$a;statesProbe=$StatesProbe;statesProbeHash=(Get-FileHash (Join-Path $root $StatesProbe)).Hash}
 $dirty=$true
 foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_checkpoint_sync off','coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off'){C $r $cmd}}
 C host 'campaign campaign/edge_of_the_world/eotw.mapinfo low corvo';C client 'campaign campaign/edge_of_the_world/eotw.mapinfo low emily'
 $g=WaitGameplay 300000;Save "$out/load-log.json" $g.log;if(!$g.ready){throw 'Both instances did not reach the same mission gameplay'}
 $bx=$a.before[0];$by=$a.before[1];$dx=$a.after[0]-$bx;$dy=$a.after[1]-$by;$len=[Math]::Sqrt($dx*$dx+$dy*$dy);$nx=$dx/$len;$ny=$dy/$len;$yaw=[Math]::Atan2($ny,$nx)*180/[Math]::PI
 foreach($attempt in 1){
  C host ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$bx,$by,($a.before[2]+0.3),$yaw);Start-Sleep -Milliseconds 1200;ClearModals "start-$attempt"
  $file="$out/mantle-states-$attempt.ndjson";$wr=[IO.StreamWriter]::new($file,$false)
  $watch=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.host,'--watch-midnight',3000) -WindowStyle Hidden -RedirectStandardOutput "$out/midnight-$attempt.json" -RedirectStandardError "$out/midnight-$attempt.err" -PassThru
  Start-Sleep -Milliseconds 150
  try{$sw=[Diagnostics.Stopwatch]::StartNew();& "$lab/Send-D2Input.ps1" -Role host -Button A -DurationMs 1600 -LeftY 32767|Out-Null
   while($sw.ElapsedMilliseconds-lt2600){$raw=& (Join-Path $root $StatesProbe) $pids.host --player-states 2>$null;if(!$LASTEXITCODE){$wr.WriteLine('{"ms":'+$sw.ElapsedMilliseconds+',"states":'+(($raw|Out-String).Trim())+'}')};Start-Sleep -Milliseconds 60}}finally{$wr.Dispose()}
  I host;if(!$watch.WaitForExit(8000)){Stop-Process -Id $watch.Id};Start-Sleep -Milliseconds 800;$rows.Add(@{attempt=$attempt;end=(Q host '--player').candidatePosition;watchExit=$watch.ExitCode})}
 Save "$out/verified.json" @{passed=$true;attempts=$rows.Count}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  foreach($s in $samplers){try{if(!$s.HasExited){Stop-Process -Id $s.Id}}catch{}}
  foreach($c in @($captures)){try{if(!$c.HasExited-and!$c.WaitForExit(30000)){Stop-Process -Id $c.Id}}catch{}}
  if($dirty){
   $cw=[Diagnostics.Stopwatch]::StartNew()
   do{$off=$true;foreach($r in 'host','client'){try{I $r;C $r 'coop_observer_camera off';C $r 'coop_product_auto off';C $r 'coop_movement_resume off';foreach($cmd in 'coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off','coop_shadow_stream off','coop_movement_bank off','coop_trigger_mirror off'){C $r $cmd};C $r 'coop_checkpoint_sync off';$st=S $r
     if((S $r '.reset.json').enabled){$off=$false;if($st.localGameLogicState-in8,15){I $r A 300}elseif($st.localGameLogicState-eq2){I $r B 200}}}catch{}}
    if(!$off){Start-Sleep -Milliseconds 1500}}while(!$off-and$cw.ElapsedMilliseconds-lt60000)
   foreach($r in 'host','client'){try{if((S $r).localGameLogicState-in2,12){I $r B 200;Start-Sleep -Milliseconds 800}}catch{}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{$s=S $r;$hp=Q $r '--health' 1;if($s.localGameLogicState-ne1-or$hp.current-ne56-or!$s.remoteFresh){throw 'Final gameplay/health/transport not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;cases=@($rows);scope='window_enter_exit diagnostic: native --player-states during the window mantle (host only); Dunwall fixture restored.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
